/**Bismillahir Rahmanir Rahimm**/

#include<bits/stdc++.h>
using namespace std;

#define mp make_pair
#define pb push_back
#define ffr(i,a,b) for(int i=a;i<b;i++)
#define mm(a,b) memset(a,b,sizeof(a))
#define _(x) {cout << #x << " = " << x << " ";}

typedef long long ll;
typedef long double ld;
typedef double db;
typedef pair<int,int> pii;
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
        ffr(i,0,no_of_buckets)
        {
            delete (sc_table[i]);
        }
        delete(parentScope);
    }

};


int main(){
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
}