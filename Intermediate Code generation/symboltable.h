/**Bismillahir Rahmanir Rahimm**/

//#pragma GCC target ("avx2")
//#pragma GCC optimization ("O3")
//#pragma GCC optimization ("unroll-loops")
#include<bits/stdc++.h>
//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>
//using namespace __gnu_pbds;
using namespace std;

#define mp make_pair
#define pb push_back
#define ffr(i,a,b) for(int i=a;i<b;i++)
#define mm(a,b) memset(a,b,sizeof(a))
#define _(x) {cout << #x << " = " << x << " ";}
#define FastIO ios::sync_with_stdio(false); cin.tie(0);cout.tie(0)

typedef long long ll;
typedef long double ld;
typedef double db;
typedef pair<string,int> psi;
typedef pair<ll,ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef set<int> si;
typedef multiset<int> msi;
typedef map<int,int> mii;

const int maxn=1010;
const int INF = 1e5+10;

struct parameter {//mainly for function parameters when $$ holds a function.See parameter_list in parser.y
    string param_type;
    string param_name;  //set to empty string "" for function declaration
} ;

struct variable{ //when $$ is a declaration_list,vector<var>var holds the variables
    string name;
    string type;
    int sz;
};

struct arg_{
  string name;
  string type;
  int sz; ///to chk if its array.ifsz>0 its array
  bool already_error_in_arg;
};

class SymbolInfo{
    string name;
    string type;
    string return_type,variable_type,identity;
    bool already_error_in_param;//only if $$ is parameter_list.It is set if parameter_list has some error .For ex: array name c is used instead of c[0] or c[1]
    bool is_declared_func;// to keep track of declared function and verify if they are defined later
    bool is_func;//to chk (at time of function calling) if $$ is really a function(see 2nd rule of factor)
    string code;
    string assembly_value;
    SymbolInfo* next;
public:
    vector<parameter>param;
    vector<variable>var;
    vector<arg_>arg_list;
    int idx;

    void set_assembly_value(string s){
      assembly_value = s;
    }

    string get_assembly_value(){
      return assembly_value;
    }

    void set_code(string code_){
      code = code_;
    }

    string get_code(){
      return code;
    }

    void add_code(string str){
      code += str;
    }

    void set_already_error_in_param(){
      already_error_in_param = true;
    }

    bool get_already_error_in_param(){
      return already_error_in_param;
    }

    void set_is_declared_func(bool state){
      is_declared_func = state;
    }

    bool get_is_declared_func(){
      return is_declared_func;
    }

    bool get_is_func(){
      return is_func;
    }

    void  set_is_func(bool state) {
      is_func = state;
    }

    void push_in_param(string nm , string tp){
      parameter temp_param;
        temp_param.param_type = tp;
        temp_param.param_name = nm;
        param.pb(temp_param);
    }

    void push_in_var(string nm , string tp , int n){
        variable temp_var;
        temp_var.type = tp;
        temp_var.name = nm;
        temp_var.sz = n;
        var.pb(temp_var);
    }

    void push_in_arg(string nm , string tp , int n){
        arg_ temp_arg;
        temp_arg.type = tp;
        temp_arg.name = nm;
        temp_arg.sz = n;
        temp_arg.already_error_in_arg =  false;
        arg_list.pb(temp_arg);
    }

    void push_in_arg_AR(string nm , string tp , int n){
        arg_ temp_arg;
        temp_arg.type = tp;
        temp_arg.name = nm;
        temp_arg.sz = n;
        temp_arg.already_error_in_arg =  true;
        arg_list.pb(temp_arg);
    }

    string getReturnType() {
		return return_type;
	}

	void setReturnType(string rtype) {
		this->return_type = rtype;
	}

	string getIdentity() {
        return identity;
	}

	void setIdentity(string identity) {
        this->identity = identity;
	}

	string getVariableType() {
		return variable_type;
	}

	void setVariableType(string variable_type) {
		this->variable_type = variable_type;
	}

    SymbolInfo(string nm,string tp){
        name = nm;
        type = tp;
        return_type = "";
        variable_type = "";
        identity = "";
        code="";
        is_declared_func = false;
        next = nullptr;
    }

    SymbolInfo(string type) {
        this->type=type;
        this->name="";
		next = 0;
	}

    SymbolInfo(){
        this->type="";
        this->name="";
        next = nullptr;
    }

    ///getters
    string get_name(){
        return name;
    }

    string get_type(){
        return type;
    }

    SymbolInfo* get_next(){
        return next;
    }

    ///setters
    void set_name(string nm){
        name = nm;
    }

    void settype(string tp){
        type = tp;
    }

    void setnext(SymbolInfo* new_){
        next = new_;
    }


};
class ScopeTable{
    int no_of_buckets ;
    int pr_condition; ///to follow output specification
    string unique_id;
    SymbolInfo **sc_table;
    ScopeTable *parentScope;

public:
    ScopeTable(int n){
        no_of_buckets = n;
        parentScope = NULL;
        //pr_condition = 1;
        sc_table = new SymbolInfo*[n];
        ffr(i,0,n){
            sc_table[i] = NULL;
        }
    }

    ScopeTable(int n , string id ,ScopeTable* prev){
        no_of_buckets = n;
        unique_id = id;
        parentScope = prev;
        pr_condition = 1;
        sc_table = new SymbolInfo*[n];
        ffr(i,0,n){
            sc_table[i] = nullptr;
        }
    }

    int hash(string s){
        int sum=0;
        ffr(i,0,s.length()){
            sum += s[i];
        }
        return sum%no_of_buckets;
    }

    SymbolInfo* Lookup(string nm){
        int hash_val = hash(nm),pos = 0;
        SymbolInfo *tmp = sc_table[hash_val];
        while (tmp!=NULL)
        {
            if(tmp->get_name() == nm){
                if(pr_condition){
                    //cout<<"Found in ScopeTable# "<<unique_id<<" at position "<<hash_val<<", "<<pos<<endl;
                }
                return tmp;
            }
            tmp = tmp->get_next();
            pos++;
        }
        if(pr_condition){
            cout<<"Not found"<<endl;
            cout<<nm<<" Not found"<<endl;
        }
        return tmp;
    }

    bool Insert(string nm , string tp , FILE* logfile_){
        pr_condition = 0;
        SymbolInfo *fnd = Lookup(nm);
        //char* nms = nm;
        if(fnd != NULL ){
            //cout<<"<"<<nm<<","<<fnd->get_type()<<">"<<" already exists in current ScopeTable"<<endl;
            fprintf(logfile_ , "%s already exists in current ScopeTable\n" , nm.c_str() );
            return false;
        }
        else{
            int hash_val = hash(nm);
            SymbolInfo *new_symbol = new SymbolInfo(nm , tp);
            if(sc_table[hash_val]==NULL){
                sc_table[hash_val] = new_symbol;
                //cout<<"Inserted in ScopeTable# "<<unique_id<<" at position "<<hash_val<<", 0"<<endl;
                //fprintf(logfile_ , "Inserted in ScopeTable# %s at position %d, 0\n",unique_id.c_str() , hash_val);
                return true;
            }
            else{
                int pos = 0;
                SymbolInfo *tmp = sc_table[hash_val];
                while (tmp->get_next()!=NULL)
                {
                    tmp = tmp->get_next();
                    pos++;
                }
                tmp->setnext(new_symbol);
                //cout<<"Inserted in ScopeTable# "<<unique_id<<" at position "<<hash_val<<", "<<pos+1<<endl;
                //fprintf(logfile_ , "Inserted in ScopeTable# %s at position %d, %d\n",unique_id.c_str() , hash_val , pos+1);
                return true;
            }

        }

    }

    void Print(FILE* logfile_){
        //cout<<"ScopeTable # "<<unique_id<<endl;
        fprintf(logfile_ , "ScopeTable # %s\n" , unique_id.c_str());
        ffr(i,0,no_of_buckets){
            //cout<<i<<" -->  ";

            SymbolInfo *tmp = sc_table[i];
            if(tmp==NULL)continue;
            fprintf(logfile_ , "%d --> ",i);
            while(tmp != NULL){
                //cout<<"< "<<tmp->get_name()<<" : "<<tmp->get_type()<<" > ";
                fprintf(logfile_ , "< %s : %s >" , tmp->get_name().c_str() , tmp->get_type().c_str());
                tmp = tmp->get_next();
            }
            //cout<<endl;
            fprintf(logfile_ , "\n");
        }
        //cout<<endl;
        fprintf(logfile_ , "\n");

    }

    bool Delete(string nm){
        pr_condition = 1;
        if(!Lookup(nm)){
            return false;
        }
        pr_condition = 0;

        int hash_val = hash(nm);
        SymbolInfo *target = sc_table[hash_val];
        if(target->get_name()==nm){
            sc_table[hash_val] = target->get_next();
            cout<<"Deleted Entry "<<hash_val<<", 0 from current ScopeTable"<<endl;
            return true;
        }

        SymbolInfo *prev ;
        int pos = 0;
        while (target != NULL)
        {
            if(target->get_name() == nm){
                    prev->setnext(target->get_next());
                    cout<<"Deleted entry at "<<hash_val<<", "<<pos<<" from current ScopeTable"<<endl;
                    break;
            }
            prev = target;
            target = target->get_next();
            pos++;
        }
        return true;
    }

    ScopeTable *get_parent(){
        return parentScope;
    }

    string get_id()
    {
        return unique_id;
    }

    void set_pr_condition(int a){
        pr_condition = a;
    }

    ~ScopeTable()
    {
        ffr(i,0,no_of_buckets)
        {
            delete (sc_table[i]);
        }
    }

};

class SymbolTable
{
    stack<ScopeTable*>Scopes;
    ScopeTable * current;
    int n;
    string current_id;
    bool del_recent;
    string del_recent_scope;
public:
    SymbolTable(int num){
        n = num;
        del_recent = false;
        Enter_Scope(num);///overloaded version just to create the first scopeTable
    }

    string get_current_id(){
      return current_id;
    }
    bool get_del_recent(){
        return del_recent;
    }
    void set_del_recent(bool b){
        del_recent = b;
    }
    void set_current_id(string s){
        current_id = s;
    }
    void set_del_recent_scope(string s){
        del_recent_scope = s;
    }

    psi get_last_part(string str){
        string s="";
        int n = str.size();
        int i = n-1;
        char ch='.';
        while(str[i]!= ch){
            s = s + str[i];
            i--;
        }
        reverse(s.begin(), s.end());
        return psi(s , i+1);
    }
    int toint(string str){
        stringstream geek(str);
        int x = 0;
        geek >> x;
        return x;
    }
    ///3 args : replace str(last part) from s with another string.idx is start index of str
    string replace_last_part(string s , int idx , string str){
        int len = str.size();
        int num = toint(str) + 1;
        string sr = to_string(num);
        string tmp = s;
        tmp.replace(idx , len , sr);
        return tmp;
    }
    string get_new_id(){
        int len = current_id.size();
        if(del_recent){   ///if immediate prev operation was a delete from symboltable
            psi p = get_last_part(del_recent_scope);
            string str = p.first; ///last part
            int idx = p.second;///start idx of last part
            string s = replace_last_part(del_recent_scope , idx , str);
            return s;

        }
        else{   ///if immediate prev operation was a insert in symboltable
            /*if(current_id=="1"){
                return current_id + ".1";
            }
            else{
                psi p = get_last_part(current_id);
                string str = p.first; ///last part
                int idx = p.second;///start idx of last part
                //cout<<current_id<<"-"<<idx<<str<<endl;
                string s = replace_last_part(current_id , idx , str);
                return s;
            }*/
            return current_id + ".1";
        }
    }

    /// Create a new ScopeTable and make it current one. Also
    ///make the previous current table as its parentScopeTable
    void Enter_Scope(FILE *logfile_){
        current_id = get_new_id();
        ScopeTable *new_scopetable;
        if(!Scopes.empty()){
            new_scopetable = new ScopeTable(n , current_id  , Scopes.top());
            //cout<<"New ScopeTable with id "<<current_id<<" created"<<endl;
            fprintf(logfile_, "New ScopeTable with id %s created\n\n", current_id.c_str());
        }
        else{
            new_scopetable = new ScopeTable(n, current_id , NULL);
        }
        Scopes.push(new_scopetable);
        current = new_scopetable;
        del_recent = false;
    }

    void Enter_Scope(int num){
        current_id = "1";
        ScopeTable *new_scopetable;
        if(!Scopes.empty()){
            new_scopetable = new ScopeTable(num , current_id  , Scopes.top());
            //cout<<" New ScopeTable with id "<<current_id<<" created"<<endl;
        }
        else{
            new_scopetable = new ScopeTable(num, current_id , NULL);
        }
        Scopes.push(new_scopetable);
        current = new_scopetable;
    }

    ///Remove the current ScopeTable
    void Exit_Scope(FILE *logfile_){
        del_recent = true;
        del_recent_scope = Scopes.top()->get_id();
        Scopes.pop();
        //cout<<"ScopeTable with id "<<current_id<<" removed"<<endl;
        fprintf(logfile_, "ScopeTable with id %s removed\n\n", current_id.c_str());
        current=Scopes.top();
        current_id = current->get_id();
    }

    ///Insert a symbol in current ScopeTable. Return true for
    ///successful insertion and false otherwise
    bool Insert(string name, string type,FILE* logfile_){
		return current->Insert(name, type,logfile_);
	}

    ///Remove a symbol from current ScopeTable. Return true for
    ///successful removal and false otherwise
    bool Remove(string name){
		return current->Delete(name);
	}

    ///Look up a symbol in the ScopeTable. At first search in the
    ///current ScopeTable, if not found then search in its parent ScopeTable
    ///and so on. Return a pointer to the SymbolInfo object representing the
    ///searched symbol
    SymbolInfo *Lookup(string name){
        ScopeTable *temp = current;
        while (temp)
        {
            temp->set_pr_condition(0);
            if(temp->Lookup(name)){
                temp->set_pr_condition(1);
                return temp->Lookup(name);
            }
            temp = temp->get_parent();
        }
        //cout<<"Not found"<<endl;
        return NULL;
    }

    SymbolInfo *Lookup_in_current(string name){
        ScopeTable *temp = current;

        temp->set_pr_condition(0);
        if(temp->Lookup(name)){
            temp->set_pr_condition(1);
            return temp->Lookup(name);
        }
        else return NULL;
    }

    ///Print the current ScopeTable
    void printcurrent(FILE* logfile_)
    {
        return current->Print(logfile_);
    }

    ///Print all the ScopeTables currently in the SymbolTable
    void printall(FILE* logfile_)
    {
        ScopeTable *temp=current;
        while(temp)
        {
            temp->Print(logfile_);
            temp=temp->get_parent();
        }
    }

    void clear_recursively(ScopeTable* p){
        if(p==NULL)return;
        clear_recursively(p->get_parent());
        delete p;
    }
    ~SymbolTable(){
       clear_recursively(current);
    }
};
