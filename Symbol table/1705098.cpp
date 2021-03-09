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
        delete next;
    }

};
class ScopeTable{
    int no_of_buckets;
    string unique_id;
    SymbolInfo **sc_table;
    ScopeTable *parentScope;

public:
    ScopeTable(int n){
        no_of_buckets = n;
        parentScope = NULL;
        sc_table = new SymbolInfo*[n];
        ffr(i,0,n){
            sc_table[i] = NULL;
        }
    }

    ScopeTable(int n , string id ,ScopeTable* prev){
        no_of_buckets = n;
        unique_id = id;
        parentScope = prev;
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
                //cout<<"Found in ScopeTable# "<<unique_id<<" at position "<<hash_val<<", "<<pos<<endl;
                return tmp;
            }
            tmp = tmp->get_next();
            pos++;
        }
        //cout<<"Not found ";
        return tmp;
    }

     SymbolInfo* Lookup(string nm , string tp){
        int hash_val = hash(nm),pos = 0;
        SymbolInfo *tmp = sc_table[hash_val];
        while (tmp!=NULL)
        {
            if(tmp->get_name() == nm and tmp->get_type()==tp){
                return tmp;
            }
            tmp = tmp->get_next();
        }
        return tmp;
    }

    bool Insert(string nm , string tp){
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
            cout<<i<<" --> ";
            SymbolInfo *tmp = sc_table[i];
            while(tmp != NULL){
                cout<<"<"<<tmp->get_name()<<" : "<<tmp->get_type()<<"> ";
                tmp = tmp->get_next();
            }
            cout<<endl;
        }
        cout<<endl;
    }

    bool Delete(string nm){
        if(!Lookup(nm)){
            return false;
        }
        int hash_val = hash(nm);
        SymbolInfo *target = sc_table[hash_val];
        if(target->get_name()==nm){
            sc_table[hash_val] = target->get_next();
            cout<<" Deleted entry at "<<hash_val<<", 0 from current ScopeTable"<<endl;
            return true;
        }
        
            SymbolInfo *prev ;
            int pos = 0;
            while (target != NULL)
            {
                if(target->get_name() == nm){
                     prev->setnext(target->get_next());
                     cout<<" Deleted entry at "<<hash_val<<", "<<pos<<" from current ScopeTable"<<endl;
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

    ~ScopeTable()
    {
        delete(parentScope);
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
            cout<<" New ScopeTable with id "<<current_id<<" created"<<endl;
        }
        else{
            new_scopetable = new ScopeTable(n, current_id , NULL);
        }
        Scopes.push(new_scopetable);
        current = new_scopetable; 
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

    void printcurrent()
    {
        return current->Print();
    }

    void printall()
    {
        ScopeTable *temp=current;
        while(temp)
        {
            temp->Print();
            temp=temp->get_parent();
        }
    }



    ~SymbolTable(){
        delete current;
    }


};


int main(){
    freopen("input.txt", "r", stdin);
    /*
    ScopeTable x(7);
    cout<<x.hash("==")<<endl;
    x.Insert("ab" , "cd");
    x.Insert("xy" , "zz");
    x.Insert("ac" , "gh");
    x.Insert("<=" , "RELOP");
    x.Insert("foo" , "baar");
    x.Insert("a"  ,"a");
    x.Insert("h" , "k");
    x.Print();
    x.Delete("ab");
    x.Print();

    cout<<s.get_new_id()<<endl;
    s.set_current_id("1.1.1");
    cout<<s.get_new_id()<<endl;
    s.set_current_id("1.1.3.4");
    cout<<s.get_new_id()<<endl;
    //s.set_del_recent(true);
    s.set_del_recent_scope("1.3.4");cout<<s.get_new_id()<<endl;
    */
    SymbolTable s(7);
    s.Enter_Scope();
    //s.Enter_Scope();
    s.printall();
    //s.Exit_Scope();
    //s.printall();
}