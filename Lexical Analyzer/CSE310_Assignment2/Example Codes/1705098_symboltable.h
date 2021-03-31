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

#define IN freopen("input.txt","r+",stdin)
#define OUT freopen("output.txt","w+",stdout)


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

class SymbolInfo{
    string name;
    string type;
    SymbolInfo* next;
public:
    SymbolInfo(string nm,string tp){
        name = nm;
        type = tp;
        next = nullptr;
    }

    SymbolInfo(){
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
    void setname(string nm){
        name = nm;
    }

    void settype(string tp){
        type = tp;
    }

    void setnext(SymbolInfo* new_){
        next = new_;
    }
    ///destructor
    ~SymbolInfo(){
        
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
                    cout<<"Found in ScopeTable# "<<unique_id<<" at position "<<hash_val<<", "<<pos<<endl;
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

    bool Insert(string nm , string tp){
        pr_condition = 0;
        SymbolInfo *fnd = Lookup(nm);
        if(fnd != NULL ){
            cout<<"<"<<nm<<","<<fnd->get_type()<<">"<<" already exists in current ScopeTable"<<endl;
            return false;
        }
        else{
            int hash_val = hash(nm);
            SymbolInfo *new_symbol = new SymbolInfo(nm , tp);
            if(sc_table[hash_val]==NULL){
                sc_table[hash_val] = new_symbol;
                cout<<"Inserted in ScopeTable# "<<unique_id<<" at position "<<hash_val<<", 0"<<endl;
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
                cout<<"Inserted in ScopeTable# "<<unique_id<<" at position "<<hash_val<<", "<<pos+1<<endl;
                return true;
            }

        }
        
    }

    void Print(){
        cout<<"ScopeTable # "<<unique_id<<endl;
        ffr(i,0,no_of_buckets){
            cout<<i<<" -->  ";
            SymbolInfo *tmp = sc_table[i];
            while(tmp != NULL){
                cout<<"< "<<tmp->get_name()<<" : "<<tmp->get_type()<<" > ";
                tmp = tmp->get_next();
            }
            cout<<endl;
        }
        cout<<endl;
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
    void Enter_Scope(){
        current_id = get_new_id();
        ScopeTable *new_scopetable;
        if(!Scopes.empty()){
            new_scopetable = new ScopeTable(n , current_id  , Scopes.top());
            cout<<"New ScopeTable with id "<<current_id<<" created"<<endl;
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
    void Exit_Scope(){
        del_recent = true;
        del_recent_scope = Scopes.top()->get_id();
        Scopes.pop();
        cout<<"ScopeTable with id "<<current_id<<" removed"<<endl;
        current=Scopes.top();
        current_id = current->get_id();
    }

    ///Insert a symbol in current ScopeTable. Return true for
    ///successful insertion and false otherwise
    bool Insert(string name, string type){
		return current->Insert(name, type);
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
        cout<<"Not found"<<endl;
        return NULL;
    }

    ///Print the current ScopeTable
    void printcurrent()
    {
        return current->Print();
    }

    ///Print all the ScopeTables currently in the SymbolTable
    void printall()
    {
        ScopeTable *temp=current;
        while(temp)
        {
            temp->Print();
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

