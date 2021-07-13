%{
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
#include<vector>
#include<bits/stdc++.h>
#include "symboltable.h"
//#define YYSTYPE SymbolInfo*

using namespace std;

#define mp make_pair
#define pb push_back
#define ffr(i,a,b) for(int i=a;i<b;i++)

typedef pair<string,string> pss;

int yyparse(void);
int yylex(void);

extern FILE *yyin;
extern int line;
extern int error_cnt;

FILE *fp,*asmCode,*optimized_asmCode;
FILE *errorfile = fopen("error.txt","w");
FILE *logfile = fopen("log.txt" , "w");

SymbolTable table(30);
vector<pair<string,string>> variableList_to_be_Initialized;
string assembly_codes;

struct var{
    string var_name;
    int var_size;  // it is set to -1 for variables
} temp_var;
vector<var> var_list;  // for identifier(variable, array) insertion into symbolTable

struct function_{
  string f_name;
  vector<pss> params;
  string return_type;
  int return_reg_no;
} temp_func;

vector<function_> func_list;

void insert_in_func_list(string nm , vector<parameter>& p , string ret_type , int return_reg_number){
    temp_func.f_name = nm;
    temp_func.return_type = ret_type;
    temp_func.return_reg_no = return_reg_number;
    for(int i=0;i<p.size();i++){
        temp_func.params.pb(mp(p[i].param_type , p[i].param_name));
    }
    func_list.pb(temp_func);
    temp_func.params.clear();
}

//in case no parameter in a function
void insert_in_func_list(string nm , string ret_type){
    temp_func.f_name = nm;
    temp_func.return_type = ret_type;
    func_list.pb(temp_func);
}

//chk if a function name is in the func_list
bool is_in_func_list(string nm){
  for(int i=0;i<func_list.size();i++){
    if(func_list[i].f_name==nm){
      return true;
    }
  }
  return false;
}

function_ get_func(string nm){
  for(int i=0;i<func_list.size();i++){
    if(func_list[i].f_name==nm){
      return func_list[i];
    }
  }
}
//debugging purpose
void print_func_list(){
  for(int i=0;i<func_list.size();i++)
  {
    cout<<func_list[i].f_name<<endl;
    cout<<func_list[i].return_type<<endl;
    for(int j=0;j<func_list[i].params.size();j++){
      cout<<func_list[i].params[j].first<<" "<<func_list[i].params[j].second<<endl;
    }
    cout<<"return reg : "<<func_list[i].return_reg_no<<endl;
  }
}

string modified_name(string str){
  string done="";
  for(int i=0;i<str.size();i++){

    if(str[i]!='['){
      done+=str[i];
    }else{
      return done;
    }
  }
  return str;
}

int get_index(string str)
{
  //str is a[2]
  //we need to return 2
  string idx="";
  int i=0;
  while(str[i]!='[')i++;
  i++;
  while(str[i]!=']' and i<str.size())
  {
    idx+=str[i];i++;
  }
  stringstream geek(idx);
  int x = 0;
  geek >> x;
  return x;
}

string modified_name_while_func_calling(string str){
  //str :- foo(4,5.6) and we need to return foo
  string done="";
  for(int i=0;i<str.size();i++){

    if(str[i]!='('){
      done+=str[i];
    }else{
      return done;
    }
  }
  return str;
}

bool is_ara_idx_valid(string nm , int sz){
  //nm = a[4] , sz = 5 ; so returns true
  //float idx by default handled yet;
  string idx="";
  for(int i=0;i<nm.size();i++){
    if(nm[i]=='['){
      i++;
      while(nm[i]!=']'){
        if(nm[i]=='.')return false;
        idx += nm[i];
        i++;
      }
    }
  }
  stringstream geek(idx);
  int x = 0;
  geek >> x;

  if(x<sz && x>=0)return true;
  else return false;
}

void yyerror(char *s)
{
	//write your code
  error_cnt++;
	fprintf(logfile,"Error at Line no %d : %s\n\n",line,s);
  fprintf(errorfile,"Error at Line no %d : %s\n\n",line,s);
}

int labelcnt=1, tempcnt=1;

string print_function(){
    string func=";------printing procedure----\n";
 		func+="PRINT_ID PROC\n\n";
 		func+="\t;SAVE IN STACK\n";
 		func+="\tPUSH AX\n";
 		func+="\tPUSH BX\n";
 		func+="\tPUSH CX\n";
 		func+="\tPUSH DX\n\n";

 		func+="\t;CHECK IF NEGATIVE\n";
 		func+="\tOR AX, AX\n";
 		func+="\tJGE PRINT_NUMBER\n\n";
 		func+="\t;PRINT MINUS SIGN\n";
 		func+="\tPUSH AX\n";
 		func+="\tMOV AH, 2\n";
 		func+="\tMOV DL, '-'\n";
 		func+="\tINT 21H\n";
 		func+="\tPOP AX\n\n";
 		func+="\tNEG AX\n\n";
 		func+="\tPRINT_NUMBER:\n";
 		func+="\tXOR CX, CX\n";
 		func+="\tMOV BX, 10D\n\n";
 		func+="\tREPEAT_CALC:\n\n";
 		func+="\t\t;AX:DX- QUOTIENT:REMAINDER\n";
 		func+="\t\tXOR DX, DX\n";
 		func+="\t\tDIV BX  ;DIVIDE BY 10\n";
 		func+="\t\tPUSH DX ;PUSH THE REMAINDER IN STACK\n\n";
 		func+="\t\tINC CX\n\n";
 		func+="\t\tOR AX, AX\n";
 		func+="\t\tJNZ REPEAT_CALC\n\n";

 		func+="\tMOV AH, 2\n\n";
 		func+="\tPRINT_LOOP:\n";
 		func+="\t\tPOP DX\n";
 		func+="\t\tADD DL, 30H\n";
 		func+="\t\tINT 21H\n";
 		func+="\t\tLOOP PRINT_LOOP\n";

 		func+="\n\t;NEWLINE\n";
 		func+="\tMOV AH, 2\n";
 		func+="\tMOV DL, 0AH\n";
 		func+="\tINT 21H\n";
 		func+="\tMOV DL, 0DH\n";
 		func+="\tINT 21H\n\n";

 		func+="\tPOP DX\n";
 		func+="\tPOP CX\n";
 		func+="\tPOP BX\n";
 		func+="\tPOP AX\n\n";
 		func+="\tRET\n";
 		func+="PRINT_ID ENDP\n\n";

    return func;
}

string pushCode(){
  return "\tPUSH AX\n\tPUSH BX\n\tPUSH CX\n\tPUSH DX\n";
}

string popCode(){
    return "\tPOP DX\n\tPOP CX\n\tPOP BX\n\tPOP AX\n";
}

string main_proc_start_code(){
  return "\t;INITIALIZE DATA SEGMENT\n\tMOV AX, @DATA\n\tMOV DS, AX\n\n";
}

string main_proc_ending_code(){
  return "\n\tMOV AX, 4CH\n\tINT 21H\nMAIN ENDP\n\nEND MAIN";
}

//following 2 methos are helper function.get_first_index returns the first position of the forbidden part
//of proc_code.get_last_Index returns the neding position of forbidden part.We then generate a valid new_proc_code
//inside the modify_proc method and return it.
int get_first_index(string str, string s)
{
    bool flag = false;
    for (int i = 0; i < str.length(); i++) {
        if (str.substr(i, s.length()) == s) {
            return i+s.length();
        }
    }
}
int get_last_Index(string str, string s)
{

    bool flag = false;
    for (int i = 0; i < str.length(); i++) {
        if (str.substr(i, s.length()) == s) {
            return i;
        }
    }
}
//removes anything from the procedure body if written after return statement
string modify_proc(string fnm , string proc_code)
{
  function_ f = get_func(fnm);
  string ret_reg = "T" + to_string(f.return_reg_no);
  int start_idx = get_first_index(proc_code , "MOV "+ret_reg+", AX");
  int end_idx = get_last_Index(proc_code , "POP DX");
  if(start_idx==0)return proc_code;//no return statement
  string new_proc_code="";
  for(int i=0;i<proc_code.length();i++){
    if(i<=start_idx || i>=end_idx-2){
      new_proc_code+=proc_code[i];
    }
  }
  return new_proc_code;
}

///return true for cases:
///s1 := MOV AX, a1
///s2 := MOV a1, AX
bool check_if_equivalent_command(string s1 , string s2)
{
  int len1 = s1.size();
  int len2 = s2.size();

  int i=0;
  //getting the first M from s1 cause s1 might contain \t at the beginning too.
  for(;i<len1;i++){
    if(s1[i]=='M')break;
  }
  if(i==len1)return false;
  if(s1.substr(i , 3)!="MOV")return false;
  int i1 = i;

  i=0;
  for(;i<len2;i++){
    if(s2[i]=='M')break;
  }
  if(i==len2)return false;
  if(s2.substr(i , 3)!="MOV")return false;
  int i2 = i;

  string src1="",dest1="",src2="",dest2="";

  dest1 = s1.substr(i1+4 , 2);
  src1 = s1.substr(i1+8 , 2);

  dest2 = s2.substr(i2+4 , 2);
  src2 = s2.substr(i2+8 , 2);

  //cout<<src1<<" "<<dest1<<" "<<src2<<" "<<dest2<<endl;

  if(src1==dest2 and src2==dest1)return true;
  else return false;
}

string assembly_procs="";

string newLabel()
{
	string temp="Label"+to_string(labelcnt);
	labelcnt++;
	return temp;
}

string newTemp()
{
	string temp="T"+to_string(tempcnt);
	tempcnt++;

	variableList_to_be_Initialized.push_back({temp,"0"});
	return temp;
}

string remove_unused_label(string s){

    istringstream origStream(s);

    vector<string>lbl_list;
    string curLine;
    while (getline(origStream, curLine))
    {
        if (!curLine.empty() and curLine.size()>0 and curLine[1]=='L' and curLine[2]=='a' and curLine[3]=='b' and curLine[4]=='e'){
          lbl_list.push_back(curLine);
          //cout<<curLine<<endl;
        }
    }

    bool got=false;
    string newval="";
    istringstream origStream2(s);
    while(getline(origStream2, curLine))
    {
        if (!curLine.empty() and curLine.size()>0 and curLine[1]=='J'){
          istringstream lbl(curLine);
          vector<string> tokens{istream_iterator<string>{lbl},
                      istream_iterator<string>{}};
          string l_nm = tokens[1];

          for(int i=0;i<lbl_list.size();i++){
            if(lbl_list[i]=="\t"+l_nm+":"){
              got = true;break;
            }
          }
          if(got){
            newval+=(curLine+"\n");
          }
          if(l_nm=="PRINT_NUMBER")newval+=(curLine+"\n");
          got = false;
        }
        else {
          newval+=(curLine+"\n");
        }
    }
    return newval;

}

bool doesnt_affect(string s1 , string s2){
  if(s2.size()<=1)return true;
  else if(s2[0]==';' or s2[1]==';')return true;
  else if(s2[1]=='I' and s2[2]=='N' and s2[3]=='C')return false;
  else if(s2[1]=='D' and s2[2]=='E' and s2[3]=='C')return false;


  else return false;

}

void optimize_code(FILE *basecode){
   optimized_asmCode=fopen("optimized_code.asm","w");
   char *line = NULL;
   size_t n = 0;
   ssize_t if_read;
	 vector<string>v;
   while ((if_read = getline(&line, &n, basecode)) != -1) {
     v.push_back(string(line));
   }
   int sz = v.size();
   int to_be_removed[sz];
   for(int i=0;i<sz;i++){
     to_be_removed[i] = 0;
   }

   for(int i=0;i<sz-3;i++){
     if(check_if_equivalent_command(v[i] ,v[i+1])){
       to_be_removed[i+1] = 1;
     }
     else if (doesnt_affect(v[i] , v[i+1])){


       if(check_if_equivalent_command(v[i] ,v[i+2])){
         to_be_removed[i+2] = 1;
       }
       else if(check_if_equivalent_command(v[i] ,v[i+3])){
         to_be_removed[i+3] = 1;
       }
     }

   }

   for(int i=0;i<sz;i++){
     if(to_be_removed[i]==0){
       fprintf(optimized_asmCode , "%s" , v[i].c_str());
     }
   }
   fclose(optimized_asmCode);
}

%}

%union {
     SymbolInfo *symbol;
}

%token IF ELSE FOR WHILE DO BREAK INT CHAR FLOAT DOUBLE
%token VOID RETURN SWITCH CASE DEFAULT CONTINUE
%token ASSIGNOP INCOP DECOP NOT LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD COMMA SEMICOLON PRINTLN

%token<symbol>CONST_INT
%token<symbol>CONST_FLOAT
%token<symbol>CONST_CHAR
%token<symbol>ID
%token<symbol>ADDOP
%token<symbol>MULOP
%token<symbol>RELOP
%token<symbol>LOGICOP

%type<symbol>start compound_statement type_specifier parameter_list declaration_list var_declaration unit func_declaration statement statements variable expression factor arguments argument_list expression_statement unary_expression simple_expression logic_expression rel_expression term func_definition program
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

start : program
	{
    $$=$1;
    //$$->set_code(assembly_codes);
    if(error_cnt==0){
        string starting=".MODEL SMALL\n.STACK 100H\n.DATA\n";
        map<string, int>alreay_declared;
        for(int i=0;i<variableList_to_be_Initialized.size();i++){
          alreay_declared[variableList_to_be_Initialized[i].first]=0;
        }
		 		for(int i=0;i<variableList_to_be_Initialized.size();i++){
          if(alreay_declared[variableList_to_be_Initialized[i].first]>0)continue;
		 			if(variableList_to_be_Initialized[i].second=="0"){
            starting+=("\t"+variableList_to_be_Initialized[i].first+" DW ?\n");
            alreay_declared[variableList_to_be_Initialized[i].first]++;
          }

		 			else
		 				starting+=("\t"+variableList_to_be_Initialized[i].first+" DW "+variableList_to_be_Initialized[i].second+" DUP(?)\n");
		 		}

		 		starting+=".CODE\n";
        ///adding print function
		 		string print_func = print_function();
        starting+=print_func;

        starting+=assembly_procs;
        //cout<<remove_unused_label(starting)<<endl;

        starting = remove_unused_label(starting);
        fprintf(asmCode,"%s",starting.c_str());
		 		fprintf(asmCode,"%s",$$->get_code().c_str());
        fclose(asmCode);
        ///write optimized code
        FILE *basecode = fopen("code.asm" , "r");
        optimize_code(basecode);
        fclose(basecode);
      }
    }
	;

program : program unit
	{
		$$ = new SymbolInfo((string)$1->get_name()+(string)$2->get_name(), "NON_TERMINAL");;
    $$->set_code($1->get_code() + $2->get_code());

    fprintf(logfile,"Line %d:  program : program unit\n\n",line);
    fprintf(logfile , "%s%s\n\n" , $1->get_name().c_str() , $2->get_name().c_str());
	}
	| unit
	{
    $$=$1;
    fprintf(logfile,"Line %d: program : unit\n\n",line);
		fprintf(logfile,"%s\n",$1->get_name().c_str());
	}
	;

unit : var_declaration
	{
		$$ = $1;
    fprintf(logfile,"Line %d: unit : var_declaration\n\n",line);
		fprintf(logfile,"%s\n\n",$1->get_name().c_str());
	}
     | func_declaration
	{
		$$ = $1;
    fprintf(logfile,"Line %d: unit : func_declaration\n\n",line);
		fprintf(logfile,"%s\n\n",$1->get_name().c_str());
	}
     | func_definition
	{
		$$ = $1;
    fprintf(logfile,"Line %d: unit : func_definition\n\n",line);
		fprintf(logfile,"%s\n",$1->get_name().c_str());
	}
     ;


func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
	{
    $$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"("+$4->get_name()+");", "NON_TERMINAL");
    fprintf(logfile , "Line %d: func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON\n\n",line);
    fprintf(logfile , "%s %s(%s);\n\n" , $1->get_name().c_str() , $2->get_name().c_str(),$4->get_name().c_str());

    /* checking whether already declared or not */
    SymbolInfo* temp = table.Lookup_in_current($2->get_name());
    if(temp != NULL) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , $2->get_name().c_str());
      fprintf(logfile , "Error at line %d: Multiple declaration of %s\n\n" , line , $2->get_name().c_str());
		}
    else{
      table.Insert($2->get_name() , "ID" , logfile);
      SymbolInfo *fd = table.Lookup_in_current($2->get_name());
      fd->set_is_declared_func(true);

      //insert in func_list
      insert_in_func_list($2->get_name() , $4->param , $1->get_name() , tempcnt-1);
    }

	}
	| type_specifier ID LPAREN RPAREN SEMICOLON
	{
		$$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"();", "NON_TERMINAL");
		fprintf(logfile , "At line no: %d func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON\n\n",line);
		fprintf(logfile , "%s %s();\n\n" , $1->get_name().c_str() , $2->get_name().c_str());


		/* checking whether already declared or not */
		SymbolInfo* temp = table.Lookup_in_current($2->get_name());
		if(temp != NULL) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , $2->get_name().c_str());
      fprintf(logfile , "Error at line %d: Multiple declaration of %s\n\n" , line , $2->get_name().c_str());
		}
    else{
      table.Insert($2->get_name() , "ID" , logfile);
      SymbolInfo *fd = table.Lookup_in_current($2->get_name());
      fd->set_is_declared_func(true);

      insert_in_func_list($2->get_name() , $1->get_name());
    }

	}
		;

func_definition : type_specifier ID LPAREN parameter_list RPAREN
  {
    //chking if invalid params given
    if($4->get_name()=="int" or $4->get_name()=="float"){
      error_cnt++;
      fprintf(logfile , "Error at line %d: 1th parameter's name not given in function definition of var\n\n" , line);
      fprintf(errorfile , "Error at line %d: 1th parameter's name not given in function definition of var\n\n" , line);
    }
    //chking if declared previously and now being defined
    //param types,return type must be matched
    //assuming we don't need to handle function overloading
    if(is_in_func_list($2->get_name())){
      function_ f = get_func($2->get_name());
      if(f.return_type != $1->get_name()){
        error_cnt++;
        fprintf(errorfile , "Error at line: %d Type Mismatch (previously declared but not being defined properly now)\n\n",line);
      }
      else {
        bool matched = true;
        int n = $4->param.size();
        if(n == f.params.size()){
          for(int i=0;i<f.params.size();i++){
            if(f.params[i].first != $4->param[i].param_type){
              matched = false;
              break;
            }
          }
        }
        else{
          matched = false;
        }
        if(!matched){
          error_cnt++;
          fprintf(errorfile , "Error at line: %d Type Mismatch (previously declared but not being defined properly now)\n\n",line);
        }
      }
    }


    /* checking whether already defined or not */
		SymbolInfo* temp = table.Lookup_in_current($2->get_name());
		if(temp != NULL and (!temp->get_is_declared_func())) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple definition of %s\n\n" , line , $2->get_name().c_str());
		}
		else if(temp == NULL){
      //insert in table
      table.Insert($2->get_name() , "ID" , logfile);
		}

    table.Enter_Scope(logfile);
    for(int i=0;i<$4->param.size();i++){
        string nm = $4->param[i].param_name;
        string tp = "ID";
        SymbolInfo *tmp = table.Lookup_in_current(nm);
        if(tmp){
          fprintf(errorfile,"Error at line %d: parameter '%s' already declared before\n\n",line,nm.c_str());
          error_cnt++;
        }
        else{
          table.Insert(nm , tp , logfile);
        }
    }

  } compound_statement {table.printall(logfile); table.Exit_Scope(logfile);var_list.clear();}

  {    insert_in_func_list($2->get_name() , $4->param , $1->get_name() , tempcnt-1);
      //Assembly
      $$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"("+$4->get_name()+")"+$7->get_name()+"\n\n", "NON_TERMINAL");
      fprintf(logfile , "Line %d: func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement\n\n" , line);
      fprintf(logfile , "%s %s(%s)%s\n\n" , $1->get_name().c_str(),$2->get_name().c_str(),$4->get_name().c_str(),$7->get_name().c_str());

      function_ f = get_func($2->get_name());
      //cout<<$2->get_name()<<" "<<f.return_reg_no<<endl;
      if($2->get_name()=="main"){
        assembly_codes="MAIN PROC\n\n";
      }
			else{
        assembly_codes=$2->get_name()+" PROC\n\n";
      }

			if($2->get_name()=="main"){
				assembly_codes+=main_proc_start_code();
			}
			else{
			  assembly_codes+=pushCode();
			}

			assembly_codes+=($7->get_code());

			if($2->get_name()=="main") {
				assembly_codes+=main_proc_ending_code();
			}
			else{
				assembly_codes+=popCode();
				assembly_codes+=("RET\n");
				assembly_codes+=($2->get_name()+" ENDP\n\n");
			}
      string final = modify_proc($2->get_name() , assembly_codes);
      if($2->get_name()!="main")assembly_procs += final;
  }
		| type_specifier ID LPAREN RPAREN
    {
      //current_return_type = $1->get_name();
      /* checking whether already declared or not */
  		SymbolInfo* temp = table.Lookup_in_current($2->get_name());
      if(temp != NULL and (!temp->get_is_declared_func())) {
  			error_cnt++;
  			fprintf(errorfile , "Error at line %d: Multiple definition of %s\n\n" , line , $2->get_name().c_str());
        fprintf(logfile , "Error at line %d: Multiple definition of %s\n\n" , line , $2->get_name().c_str());
  		}
  		else if(temp == NULL){
  			table.Insert($2->get_name() , "ID" , logfile);

  		}

      table.Enter_Scope(logfile);

    }
    compound_statement {table.printall(logfile);table.Exit_Scope(logfile);var_list.clear();}
 	{
      insert_in_func_list($2->get_name() , $1->get_name());
      fprintf(logfile , "Line %d: func_definition : type_specifier ID LPAREN RPAREN compound_statement\n\n" , line);
      fprintf(logfile , "%s %s()%s\n\n" , $1->get_name().c_str(),$2->get_name().c_str(),$6->get_name().c_str());

      //assembly
      $$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"()"+$6->get_name()+"\n\n", "NON_TERMINAL");
      if($2->get_name()=="main")
				assembly_codes="MAIN PROC\n\n";
			else
				assembly_codes=$2->get_name()+" PROC\n\n";

			if($2->get_name()=="main"){
				assembly_codes+=main_proc_start_code();
			}
			else{
			  assembly_codes+=pushCode();
			}

			//inside func body
			assembly_codes+=($6->get_code());
			//ending of main proc
			if($2->get_name()=="main") {
				assembly_codes+=main_proc_ending_code();
			}
			else{
				assembly_codes+=popCode();
				assembly_codes+=("RET\n");
				assembly_codes+=($2->get_name()+" ENDP\n\n");
			}
      //cout<<modify_proc($2->get_name() , assembly_codes)<<endl;
      if($2->get_name()=="main")$$->set_code(assembly_codes);
      if($2->get_name()!="main")assembly_procs += modify_proc($2->get_name() , assembly_codes);
	}

	 	;


parameter_list  : parameter_list COMMA type_specifier ID
		{
			$$ = new SymbolInfo($1->get_name()+","+$3->get_name()+" "+$4->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no: %d parameter_list  : parameter_list COMMA type_specifier ID\n\n",line);
			fprintf(logfile , "%s , %s %s\n\n" , $1->get_name().c_str(),$3->get_name().c_str(),$4->get_name().c_str());

			/* adding parameter to parameter list */
      $$->param = $1->param;
			$$->push_in_param($4->get_name() , $3->get_name());

		}
		| parameter_list COMMA type_specifier
		{
			$$ = new SymbolInfo($1->get_name()+","+$3->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no: %d parameter_list  : parameter_list COMMA type_specifier\n\n",line);
			fprintf(logfile , "%s , %s\n\n" , $1->get_name().c_str(),$3->get_name().c_str());

			/* adding parameter to parameter list */
      $$->param = $1->param;
			$$->push_in_param("", $3->get_name());
		}
 		| type_specifier ID
		{
			$$ = new SymbolInfo($1->get_name()+" "+$2->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no: %d parameter_list  : type_specifier ID\n\n",line);
			fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str(),$2->get_name().c_str());

			/* adding parameter to parameter list */

			$$->push_in_param($2->get_name() ,$1->get_name());
		}
		| type_specifier
		{
			$$ = new SymbolInfo($1->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no: %d parameter_list  : type_specifier\n\n",line);
			fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str());

			/* adding parameter to parameter list */
			$$->push_in_param( "" , $1->get_name());
		}

 		;

compound_statement : LCURL statements RCURL
  {
      $$=$2;
      //$$ = new SymbolInfo("{\n"+$2->get_name()+"\n}"+"\n\n", "NON_TERMINAL");
      fprintf(logfile,"At line no: %d compound_statement : LCURL statements RCURL\n\n",line);
			fprintf(logfile,"{\n%s\n}\n\n",$2->get_name().c_str());

  }
 		    | LCURL RCURL
  {
    $$ = new SymbolInfo("{\n}", "NON_TERMINAL");
    fprintf(logfile,"At line no: %d compound_statement : LCURL RCURL\n\n",line);
    fprintf(logfile,"{}\n\n");

  }
 		    ;

var_declaration : type_specifier declaration_list SEMICOLON
		{
			fprintf(logfile,"At line no: %d var_declaration : type_specifier declaration_list SEMICOLON\n\n",line);
			fprintf(logfile,"%s %s;\n\n",$1->get_name().c_str(),$2->get_name().c_str());
			$$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+";", "NON_TERMINAL");

      $$->setVariableType($1->get_name());

      //setting type of all elements of var in declaration_list
      for(int i=0;i<$2->var.size();i++){
          $2->var[i].type = $1->get_name();
      }
			/* inserting in symboltable */
			if($1->get_name()=="void"){
				error_cnt++;
				fprintf(errorfile,"Error at line %d: variable cannot be of type void\n\n",line);

			}
			else{
				/* checking whether already declared or not */
				for(int i=0;i<$2->var.size();i++){
					SymbolInfo* temp = table.Lookup_in_current($2->var[i].name);
					if(temp != NULL) {
						error_cnt++;
						fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , $2->var[i].name.c_str());
					}
					else{
						table.Insert($2->var[i].name , "ID" , logfile);
            SymbolInfo *tmp = table.Lookup_in_current($2->var[i].name);
            tmp->setVariableType($1->get_name());
            if($2->var[i].sz>0){
              tmp->setIdentity("array");
            }else{
              tmp->setIdentity("var");
            }
					}
				}

			}

		}
    //syntax error recovery
   /* error SEMICOLON
   {
     yyclearin;
     yyerrok;
   }*/

 		;

type_specifier	: INT
		{
			fprintf(logfile,"At line no: %d: type_specifier : INT \n\n",line);
			SymbolInfo *x = new SymbolInfo("int" , "int");
			$$ = x;
			fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
 		| FLOAT
		{
			fprintf(logfile,"At line no: %d: type_specifier : FLOAT \n",line);
			SymbolInfo *x = new SymbolInfo("float" , "float");
			$$ = x;
			fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
 		| VOID
 		{
			fprintf(logfile,"At line no: %d: type_specifier : VOID \n",line);
			SymbolInfo *x = new SymbolInfo("void" , "void");
			$$ = x;
			fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
		 ;

declaration_list : declaration_list COMMA ID
		{
 			variableList_to_be_Initialized.push_back({$3->get_name()+table.get_current_id()[0],"0"});

			fprintf(logfile,"At line no: %d: declaration_list : declaration_list COMMA ID\n\n",line);
			$$ = new SymbolInfo((string)$1->get_name()+(string)","+(string)$3->get_name(), "NON_TERMINAL");
			fprintf(logfile , "%s,%s\n\n" , $1->get_name().c_str() , $3->get_name().c_str());

			/* keeping track of identifier(variable) */
      temp_var.var_name = $3->get_name();
      temp_var.var_size = -1;
      var_list.push_back(temp_var);

      /* 3 args are name , type, size of variable */
      $$->var = $1->var;
      $$->push_in_var($3->get_name() , "" , 0);

		}
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
		{
 			variableList_to_be_Initialized.push_back({$3->get_name()+table.get_current_id()[0],$5->get_name()});

			fprintf(logfile,"At line no: %d declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD\n\n",line);
			$$ = new SymbolInfo((string)$1->get_name()+(string)","+(string)$3->get_name()+(string)"["+(string)$5->get_name()+(string)"]", "NON_TERMINAL");
			fprintf(logfile , "%s,%s[%s]\n\n" , $1->get_name().c_str() , $3->get_name().c_str() , $5->get_name().c_str());

			/* keeping track of identifier(array) */
      temp_var.var_name = (string)$3->get_name();
      stringstream temp_str((string) $5->get_name());
      temp_str >> temp_var.var_size;
      var_list.pb(temp_var);

      stringstream geek($5->get_name());
      int sz = 0;
      geek >> sz;

      $$->var = $1->var;
      $$->push_in_var($3->get_name() , "" , sz);

		}

 		  | ID
		{
 			variableList_to_be_Initialized.push_back({$1->get_name()+table.get_current_id()[0],"0"});

			fprintf(logfile,"At line no: %d declaration_list : ID\n\n",line);
 			fprintf(logfile,"%s\n\n",$1->get_name().c_str());

 			$$ = new SymbolInfo($1->get_name() ,  "ID");

			 /* keeping track of identifier(variable) */
      temp_var.var_name = (string)$1->get_name();
      temp_var.var_size = -1;
      var_list.pb(temp_var);

      $$->push_in_var($1->get_name() , "" , 0);

		}
 		| ID LTHIRD CONST_INT RTHIRD
		{
 			variableList_to_be_Initialized.push_back({$1->get_name()+table.get_current_id()[0],$3->get_name()});

			fprintf(logfile , "At line no: %d  declaration_list: ID LTHIRD CONST_INT RTHIRD\n\n",line);
			$$ = new SymbolInfo($1->get_name()+"["+$3->get_name()+"]", "NON_TERMINAL");
			fprintf(logfile , "%s[%s]\n\n",$1->get_name().c_str() , $3->get_name().c_str());

			temp_var.var_name = $1->get_name();
      stringstream temp_str($3->get_name());
      temp_str >> temp_var.var_size;
      var_list.pb(temp_var);

      stringstream geek($3->get_name());
      int sz = 0;
      geek >> sz;

      $$->push_in_var($1->get_name() , "" , sz);

		}

 		  ;

statements : statement
    {
       $$ = $1;
       fprintf(logfile , "At line no: %d statements : statement\n\n" , line);
       fprintf(logfile , "%s\n\n" , $1->get_name().c_str());
    }
	   | statements statement
    {
      $$ = new SymbolInfo($1->get_name()+$2->get_name() , "NON_TERMINAL");
      fprintf(logfile , "At line no: %d statements : statements statement\n\n" , line);
      fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str(),$2->get_name().c_str());
      $$->set_code($1->get_code() + " " + $2->get_code());
    }
	   ;

statement : var_declaration
    {
      fprintf(logfile,"At line no: %d statement : var_declaration\n\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
      $1->set_name($1->get_name()+"\n");
  		$$=$1;
    }
	  | expression_statement
    {
      fprintf(logfile,"At line no: %d statement : expression_statement\n\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
      $1->set_name($1->get_name()+"\n");
			$$=$1;
    }
	  | compound_statement
    {
      fprintf(logfile,"At line no: %d statement : compound_statement\n\n",line);
      fprintf(logfile,"%s\n\n",$1->get_name().c_str());

      $$=$1;
    }
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement
    {
      string l1=newLabel();
      string l2=newLabel();

      $$ = $3;

      $$->add_code(";-------for loop starts--------\n\t");
			$$->add_code(l1+":\n");

			$$->add_code($4->get_code());

			$$->add_code("\tMOV AX, "+$4->get_name()+"\n");
			$$->add_code("\tCMP AX, 0\n");
			$$->add_code("\tJE "+l2+"\n");

			$$->add_code($7->get_code());
			$$->add_code($5->get_code());
			$$->add_code("\tJMP "+l1+"\n");

			$$->add_code("\t"+l2+":\n");

      string str="for("+$3->get_name()+$4->get_name()+$5->get_name()+")"+$7->get_name();
      fprintf(logfile,"line no. %d: statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement\n",line);
			fprintf(logfile,"%s\n\n",str.c_str());
    }
	  | IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE
    {
      string str = "if("+$3->get_name()+")"+$5->get_name();
      fprintf(logfile,"At line no: %d statement : IF LPAREN expression RPAREN statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());
      $$=$3;
      //$$->set_name(str);
      $$->settype("if");

			string label=newLabel();

      $$->add_code(";--------if block---------\n");
			$$->add_code("\tMOV AX, "+$3->get_name()+"\n");

			$$->add_code("\tCMP AX, 0\n");
			$$->add_code("\tJE "+label+"\n");
			$$->add_code($5->get_code());
			$$->add_code("\t"+label+":\n");

    }
	  | IF LPAREN expression RPAREN statement ELSE statement
    {
      string str = "if("+$3->get_name()+")"+$5->get_name()+"else"+$7->get_name();
      fprintf(logfile,"At line no: %d statement : IF LPAREN expression RPAREN statement ELSE statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());
      $$=$3;

      $$->settype("if---else---if");

			string else_condition=newLabel();
			string after_else=newLabel();
      $$->add_code(";--------if else block---------\n");
			$$->add_code("\tMOV AX, "+$3->get_name()+"\n");
			$$->add_code("\tCMP AX, 0\n");
			$$->add_code("\tJE "+else_condition+"\n");

			$$->add_code($5->get_code());
			$$->add_code("\tJMP "+after_else);

			$$->add_code("\n\t"+else_condition+":\n");
			$$->add_code($7->get_code());
			$$->add_code("\n\t"+after_else+":\n");

    }
	  | WHILE LPAREN expression RPAREN statement
    {
      string str = "while("+$3->get_name()+")"+$5->get_name();
      fprintf(logfile,"At line no: %d statement : WHILE LPAREN expression RPAREN statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());

      $$=new SymbolInfo("while","loop");

			string l1=newLabel(), l2=newLabel();
      assembly_codes=(";--------while loop---------\n\t");
			assembly_codes+=(l1+":\n");

			assembly_codes+=$3->get_code();

			assembly_codes+=("\tMOV AX, "+$3->get_name()+"\n");
			assembly_codes+="\tCMP AX, 0\n";
			assembly_codes+="\tJE "+l2+"\n";

			assembly_codes+=$5->get_code();
			assembly_codes+="\tJMP "+l1+"\n";

			assembly_codes+=("\t"+l2+":\n");

			$$->set_code(assembly_codes);

    }
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
    {

      fprintf(logfile,"Line %d: statement : PRINTLN LPAREN ID RPAREN SEMICOLON\n\n",line);
      fprintf(logfile , "printf(%s);\n\n" , $3->get_name().c_str());

      $$=new SymbolInfo("println","nonterminal");
      assembly_codes=(";--------print function called---------\n");
			assembly_codes+=("\n\tMOV AX, "+$3->get_name()+table.get_current_id()[0]+"\n");
			assembly_codes+=("\tCALL PRINT_ID\n");
			$$->set_code(assembly_codes);


      if($3->get_is_func()){
        if(!table.Lookup_in_current(modified_name_while_func_calling($3->get_name()))){
          error_cnt++;
          fprintf(errorfile , "Error at line: %d Undeclared function %s\n\n" , line,modified_name_while_func_calling($3->get_name()).c_str());
          fprintf(logfile , "Error at line: %d Undeclared function %s\n\n" , line,modified_name_while_func_calling($3->get_name()).c_str());
        }
      }
      else if(!table.Lookup($3->get_name())){
        error_cnt++;
        fprintf(errorfile , "Error at line: %d Undeclared Variable %s\n\n" , line,$3->get_name().c_str());
        fprintf(logfile , "Error at line: %d Undeclared Variable %s\n\n" , line,$3->get_name().c_str());
      }


    }
	  | RETURN expression SEMICOLON
    {
      fprintf(logfile,"Line %d: statement : RETURN expression SEMICOLON\n\n",line);
      fprintf(logfile , "return %s;\n\n" , $2->get_name().c_str());
      $$=new SymbolInfo("return","statement");
			assembly_codes=$2->get_code();
			$$->set_code(assembly_codes);
    }
	  ;

expression_statement 	: SEMICOLON
    {
      $$=new SymbolInfo("SEMICOLON","SEMICOLON");
      fprintf(logfile,"Line %d: expression_statement : SEMICOLON\n",line);
			fprintf(logfile,";\n\n");
    }
			| expression SEMICOLON
    {
      $$ = $1;
      fprintf(logfile,"Line %d: expression_statement : expression SEMICOLON\n\n",line);
      fprintf(logfile , "%s;\n\n" , $1->get_name().c_str());
    }
    | expression error
   {
     //error_cnt++;
     yyclearin;
   }
			;

variable :

    ID
    {
      fprintf(logfile,"Line %d: variable : ID\n",line);
      fprintf(logfile,"%s\n\n",$1->get_name().c_str());

      $$ = $1;
      $$->idx=-1;
      //Semantic : chk if variable is declared before
      $$->setIdentity("var");
      $$->push_in_var($1->get_name(),"",0);
      SymbolInfo *x=table.Lookup($1->get_name());
      if(x)$$->setVariableType(x->getVariableType());
      //cout<<$$->get_name()<<endl;
      $$->set_assembly_value($$->get_name()+table.get_current_id()[0]);

    }

	 | ID LTHIRD expression RTHIRD
   {
     fprintf(logfile,"At line no: %d variable : ID LTHIRD expression RTHIRD\n",line);
		 fprintf(logfile,"%s[%s]\n\n",$1->get_name().c_str(),$3->get_name().c_str());
     $$ = new SymbolInfo($1->get_name()+"["+$3->get_name()+"]" , "variable");

     $$->setIdentity("array");
     stringstream geek($3->get_name());
     int x = 0;
     geek >> x;
     $$->idx=x;
     $$->set_assembly_value($$->get_name()+table.get_current_id()[0]);

   }

	 ;

 expression : logic_expression
    {
      $$ = $1;
      fprintf(logfile,"Line %d: expression : logic_expression\n\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
    }
	   | variable ASSIGNOP logic_expression
    {
      $$ = new SymbolInfo($1->get_name()+"="+$3->get_name() , "expression");
      fprintf(logfile,"Line %d: expression : variable ASSIGNOP logic_expression\n\n",line);
      fprintf(logfile,"%s = %s\n\n",$1->get_name().c_str(),$3->get_name().c_str());
      //semantics
      //todo
      //assign $3's var_type to $1 after some error chkings

      ///pass arrayname if array otherwise pass varname only
      ///suppose $1->get_name() is a[2].Now modified_name returns only a
      string varname;
      varname = modified_name($1->get_name());
      SymbolInfo *x=table.Lookup(varname);
			if(x)
			{
				//setting type of var(int/float)
        $1->setVariableType(x->getVariableType());

        //chk if variable and written with index
        bool isvar=true;
        for(int i=0;i<var_list.size();i++){
          if(var_list[i].var_name==x->get_name() && var_list[i].var_size>0){
            isvar = false;break;
         }
        }
        if(isvar){
          if(varname != $1->get_name()){
            error_cnt++;
            fprintf(errorfile , "Error at line: %d %s is not an array\n\n" , line,varname.c_str());
          }
        }
        //chk if array
        for(int i=0;i<var_list.size();i++){
          if(var_list[i].var_name==x->get_name() && var_list[i].var_size>0){
            //now we're sure that it's an array
            //let's see if ara is being used without any index
            if(varname==$1->get_name()){
              error_cnt++;
              fprintf(errorfile,"Error at line: %d Type Mismatch\n\n",line);
              break;
            }
            //now chk if wrong index is given
            else if(!is_ara_idx_valid($1->get_name() , var_list[i].var_size)){
              error_cnt++;
              fprintf(errorfile,"Error at line: %d Wrong array index(Expression inside third brackets not an integer)\n\n",line);
              break;
            }
          }
        }

        //check if float is assigned to int
        if(x->getVariableType()=="int" && $3->getVariableType()=="float"){
					error_cnt++;
          $$->setVariableType("int");
					fprintf(errorfile,"Error at line %d: type mismatch in assignment \n\n",line,$3->getVariableType().c_str(),x->getVariableType().c_str());
				}
        else if(x->getVariableType()=="float" && $3->getVariableType()=="int"){
          $$->setVariableType("float");
        }
        else{
          $$->setVariableType(x->getVariableType());
        }


      if($3->get_is_func()){
        ///extract function name cause $3 has name like foo(6) but we need only foo
        string fnm = modified_name_while_func_calling($3->get_name());
        if(is_in_func_list(fnm)){
          function_ f = get_func(fnm);
          ///chk if func is returning to valid type

          if(f.return_type=="void"){
            error_cnt++;
            fprintf(errorfile , "Error at line: %d Void function used in expression\n\n",line);
          }
          else if(f.return_type != $1->getVariableType()){
            error_cnt++;
            fprintf(errorfile , "Error at line: %d Type Mismatch in function returning\n\n",line);
          }

        }
      }

			}

			else{
				error_cnt++;
				fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,varname.c_str());
			}

      //assembly
			$$->set_code($3->get_code()+$1->get_code());
			$$->add_code("\n\tMOV AX, "+$3->get_assembly_value()+"\n");

      string temp = modified_name($1->get_name())+table.get_current_id()[0];
      $$->set_name(temp);
			$$->set_assembly_value(temp);

      //if variable
      if($1->getIdentity()!="array"){
				$$->add_code("\tMOV "+temp+", AX\n");
			}
			//or array
			else{
        int idx=get_index($1->get_name());
				if(idx==0)$$->add_code("\tMOV "+temp+", AX\n");
        else $$->add_code("\tMOV "+temp+"+"+to_string(idx)+"*2, AX\n");
			}
    }
	   ;

logic_expression : rel_expression
    {
      $$ = $1;
      $$->setVariableType("int");
      fprintf(logfile,"Line %d: logic_expression : rel_expression\n\n",line);
      fprintf(logfile,"%s\n\n",$1->get_name().c_str());
    }
		 | rel_expression LOGICOP rel_expression
    {
      $$ = $1;
      fprintf(logfile,"At line no: %d logic_expression : rel_expression LOGICOP rel_expression\n\n",line);
      fprintf(logfile,"%s%s%s\n\n",$1->get_name().c_str(),$2->get_name().c_str(),$3->get_name().c_str());

      /*semantic
      both $1 and $3 must be of type non void
      $$ must be set to type int
      */
      if($1->getVariableType()=="void" || $3->getVariableType()=="void"){
 				error_cnt++;
 				fprintf(errorfile,"Error at line %d found: Type Mismatch(Operands of %s can't be void)\n\n",line,$2->get_name().c_str());
 			}

       $$->setVariableType("int");

       //assembly
			$$->add_code($3->get_code());
			string temp=newTemp();
			string l1=newLabel();
			string l2=newLabel();

      //cout<<$1->get_assembly_value()<<endl;
			$$->add_code("\n\tMOV AX, "+$1->get_assembly_value()+"\n");
			$$->add_code("\tMOV BX, "+$3->get_assembly_value()+"\n");

			if($2->get_name()=="&&"){
				$$->add_code("\tCMP AX, 1\n");
				$$->add_code("\tJNE "+l1+"\n");

				$$->add_code("\tCMP BX, 1\n");
				$$->add_code("\tJNE "+l1+"\n");

				$$->add_code("\tMOV AX, 1\n");
				$$->add_code("\tMOV "+temp+", AX\n");
				$$->add_code("\tJMP "+l2+"\n");

				$$->add_code("\n\t"+l1+":\n");
				$$->add_code("\tMOV AX, 0\n");
				$$->add_code("\tMOV "+temp+", AX\n");

				$$->add_code("\n\t"+l2+":\n");
			}

			else if($2->get_name()=="||"){
				$$->add_code("\tCMP AX, 1\n");
				$$->add_code("\tJE "+l1+"\n");

				$$->add_code("\tCMP BX, 1\n");
				$$->add_code("\tJE "+l1+"\n");

				$$->add_code("\tMOV AX, 0\n");
				$$->add_code("\tMOV "+temp+", AX\n");
				$$->add_code("\tJMP "+l2+"\n");

				$$->add_code("\n\t"+l1+":\n");
				$$->add_code("\tMOV AX, 1\n");
				$$->add_code("\tMOV "+temp+", AX\n");

				$$->add_code("\n\t"+l2+":\n");
			}
      $$->set_name(temp);
			$$->set_assembly_value(temp);
    }
		 ;

rel_expression	: simple_expression
   {
     $$ = $1;
     fprintf(logfile,"Line %d: rel_expression	: simple_expression\n\n",line);
     fprintf(logfile,"%s\n\n",$1->get_name().c_str());
     $$->setVariableType("int");
     //cout<<$$->get_assembly_value()<<endl;
   }
		| simple_expression RELOP simple_expression
   {
     $$ = $1;
     fprintf(logfile,"Line %d: rel_expression : simple_expression RELOP simple_expression\n\n",line);
     fprintf(logfile,"%s%s%s\n\n",$1->get_name().c_str(),$2->get_name().c_str(),$3->get_name().c_str());

     /*semantic
     both $1 and $3 must be of type non void
     $$ must be set to type int
     */
     if($1->getVariableType()=="void" || $3->getVariableType()=="void"){
				error_cnt++;
				fprintf(errorfile,"Error at line %d found: Type Mismatch(Operands of %s can't be void)\n\n",line,$2->get_name().c_str());
			}
      $$->setVariableType("int");


			$$->add_code($3->get_code());

			$$->add_code("\n\tMOV AX, "+$1->get_assembly_value()+"\n");
			$$->add_code("\tCMP AX, "+$3->get_assembly_value()+"\n");

			string temp=newTemp();
			string l1=newLabel();
			string l2=newLabel();

			if($2->get_name()=="<"){
				$$->add_code("\tJL "+l1+"\n");
			}
      else if($2->get_name()==">"){
        $$->add_code("\tJG "+l1+"\n");
      }
      else if($2->get_name()=="=="){
        $$->add_code("\tJE "+l1+"\n");
      }
			else if($2->get_name()=="<="){
				$$->add_code("\tJLE "+l1+"\n");
			}
			else if($2->get_name()==">="){
				$$->add_code("\tJGE "+l1+"\n");
			}
			else{
				$$->add_code("\tJNE "+l1+"\n");
			}

			$$->add_code("\n\tMOV "+temp+", 0\n");
			$$->add_code("\tJMP "+l2+"\n");

			$$->add_code("\n\t"+l1+":\n\tMOV "+temp+", 1\n");
			$$->add_code("\n\t"+l2+":\n");

			$$->set_name(temp);
			$$->set_assembly_value(temp);
   }
		;

simple_expression : term
  {
    $$ = $1;
    fprintf(logfile,"Line %d: simple_expression : term\n\n",line);
    fprintf(logfile,"%s\n\n",$1->get_name().c_str());

  }
		  | simple_expression ADDOP term
  {
    $$ = $1;
    fprintf(logfile,"Line %d: simple_expression : simple_expression ADDOP term\n\n",line);
    fprintf(logfile,"%s%s%s\n\n",$1->get_name().c_str(),$2->get_name().c_str(),$3->get_name().c_str());

    if($1->getVariableType()=="float" || $3->getVariableType()=="float")
				$$->setVariableType("float");
		else
				$$->setVariableType("int");

  			$$->add_code($3->get_code());

  			string temp=newTemp();
  			if($2->get_name()=="+"){
  				$$->add_code("\n\tMOV AX, "+$1->get_assembly_value()+"\n");
  				$$->add_code("\tADD AX, "+$3->get_assembly_value()+"\n");
  				$$->add_code("\tMOV "+temp+", AX\n");
  			}

  			else{
  				$$->add_code("\n\tMOV AX, "+$1->get_assembly_value()+"\n");
  				$$->add_code("\tSUB AX, "+$3->get_assembly_value()+"\n");
  				$$->add_code("\tMOV "+temp+", AX\n");
  			}

  			$$->set_name(temp);
  			$$->set_assembly_value(temp);

  }
		  ;

term :	unary_expression
    {
      $$ = $1;
      fprintf(logfile,"Line %d: term :	unary_expression\n\n",line);
      fprintf(logfile,"%s\n\n",$1->get_name().c_str());
    }
     |  term MULOP unary_expression
    {
      $$=$1;
      fprintf(logfile,"Line %d: term : term MULOP unary_expression\n\n",line);
      fprintf(logfile,"%s%s%s\n\n",$1->get_name().c_str(),$2->get_name().c_str(),$3->get_name().c_str());

			//assembly
			$$->add_code($3->get_code());
			$$->add_code("\n\tMOV AX, "+ $1->get_assembly_value()+"\n");
			$$->add_code("\tMOV BX, "+ $3->get_assembly_value()+"\n");

			string temp=newTemp();
      //perform multiplication
			if($2->get_name()=="*"){
				$$->add_code("\tMUL BX\n");
				$$->add_code("\tMOV "+temp+", AX\n");
			}
      //perform division
			else if($2->get_name()=="/"){
				$$->add_code("\tXOR DX, DX\n");
				$$->add_code("\tDIV BX\n");
				$$->add_code("\tMOV "+temp+" , AX\n");
			}
      //perform mod operation
			else{
				$$->add_code("\tXOR DX, DX\n");
				$$->add_code( "\tDIV BX\n");
				$$->add_code("\tMOV "+temp+" , DX\n");
			}

		 	$$->set_name(temp);
			$$->set_assembly_value(temp);

      //semantic
      //if $3 is void type function
      string fn = modified_name_while_func_calling($3->get_name());
      if(is_in_func_list(fn)){
        function_ f = get_func(fn);
        if(f.return_type=="void"){
          error_cnt++;
          fprintf(errorfile , "Error at line %d: Void function used in expression\n\n",line);
          fprintf(logfile , "Error at line %d: Void function used in expression\n\n",line);
        }
      }
      //features of mod operation
      if($2->get_name()=="%" && ($1->getVariableType()!="int" || $3->getVariableType()!="int")){
				error_cnt++;
				fprintf(errorfile,"Error at line %d: Non-Integer operand on modulus operator\n\n",line);
        fprintf(logfile,"Error at line %d: Non-Integer operand on modulus operator\n\n",line);
			}
      //mod by zero
      else if($2->get_name()=="%" && $3->get_name()=="0"){
				error_cnt++;
				fprintf(errorfile,"Error at line %d: Modulus by Zero\n\n",line);
        fprintf(logfile,"Error at line %d: Modulus by Zero\n\n",line);
			}
			//set var_type
			if($2->get_name()=="%")
				$$->setVariableType("int");
			else
			{
				if($1->getVariableType()=="float" || $3->getVariableType()=="float")
					$$->setVariableType("float");
				else
					$$->setVariableType("int");
			}

    }
     ;

unary_expression : ADDOP unary_expression
    {
      //if $3 is void type function
      string fn = modified_name_while_func_calling($2->get_name());
      if(is_in_func_list(fn)){
        function_ f = get_func(fn);
        if(f.return_type=="void"){
          error_cnt++;
          fprintf(errorfile , "Error at line %d: Void function used in expression\n\n",line);
          fprintf(logfile , "Error at line %d: Void function used in expression\n\n",line);
        }
      }

      fprintf(logfile,"At line no: %d unary_expression : ADDOP unary_expression\n",line);
		  fprintf(logfile,"%s%s\n\n",$1->get_name().c_str(),$2->get_name().c_str());

			$$=$2;

      $$->setVariableType($2->getVariableType());
      $$->setIdentity($2->getIdentity());

      //assembly
			string temp=newTemp();
			if($1->get_name()=="-"){
				$$->add_code("\n\tMOV AX, "+$2->get_assembly_value()+"\n");
				$$->add_code("\tNEG AX\n");
				$$->add_code("\tMOV "+temp+", AX\n");
			}

			else{
				$$->add_code("\n\tMOV AX, "+$2->get_assembly_value()+"\n");
				$$->add_code("\tMOV "+temp+", AX\n");
			}
			$$->set_name(temp);
			$$->set_assembly_value(temp);
    }
		 | NOT unary_expression
    {
      $$=$2;
			string temp=newTemp();

			$$->add_code("\n\tMOV AX, "+$2->get_assembly_value()+"\n");
			$$->add_code("\tNOT AX\n");
			$$->add_code("\tMOV "+temp+", AX\n");

      $$->setVariableType($2->getVariableType());
      $$->setIdentity($2->getIdentity());
			$$->set_name(temp);
			$$->set_assembly_value(temp);
    }
		 | factor
     {
       $$ = $1;
       fprintf(logfile,"Line %d: unary_expression :	factor\n\n",line);
       fprintf(logfile,"%s\n\n",$1->get_name().c_str());
     }
		 ;

factor	: variable
    {
      $$ = $1;
      fprintf(logfile,"Line %d: factor : variable\n\n",line);
      fprintf(logfile,"%s\n\n",$1->get_name().c_str());

      if($1->getIdentity()=="array"){
        int idx = get_index($$->get_name());
        if(idx==0)$$->set_assembly_value(modified_name($$->get_name()) +table.get_current_id()[0]);
        else $$->set_assembly_value(modified_name($$->get_name()) +table.get_current_id()[0]+"+"+to_string(idx)+"*2");
      }
      else{
        //cout<<modified_name($$->get_name()) +table.get_current_id()[0]<<endl;
        $$->set_assembly_value (modified_name($$->get_name()) +table.get_current_id()[0]);
      }

      ///pass arrayname if array otherwise pass varname only
      ///suppose $1->get_name() is a[2].Now modified_name returns only a
      string varname;
      varname = modified_name($1->get_name());
      SymbolInfo *x=table.Lookup(varname);
			if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,varname.c_str());
      }
      else{

          //setting type of var(int/float) and identity(array/normal variable)
  				$$->setVariableType(x->getVariableType());
          $$->setIdentity(x->getIdentity());
          //chk if array
          for(int i=0;i<var_list.size();i++){
            if(var_list[i].var_name==x->get_name() && var_list[i].var_size>0){
              //now we're sure that it's an array
              //let's see if ara is being used without any index
              if(varname==$1->get_name()){
                error_cnt++;
                fprintf(errorfile,"Error at line: %d %s is an array\n\n",line,varname.c_str());
                fprintf(logfile,"Error at line: %d %s is an array\n\n",line,varname.c_str());
                $$->set_already_error_in_param();
                break;
              }
              //now chk if wrong index is given
              else if(!is_ara_idx_valid($1->get_name() , var_list[i].var_size)){
                error_cnt++;
                fprintf(errorfile,"Error at line: %d Wrong array index\n\n",line);
                break;
              }
            }
          }
      }

    }
	| ID LPAREN argument_list RPAREN
    {
      $$ = new SymbolInfo($1->get_name() + "(" + $3->get_name() + ")" , "factor");
      $$->set_is_func(true);

      fprintf(logfile,"Line %d: factor : ID LPAREN argument_list RPAREN\n\n",line);
      fprintf(logfile,"%s(%s)\n\n",$1->get_name().c_str(),$3->get_name().c_str());

      //semantic
      //chk if id is in func_list
      if(!is_in_func_list($1->get_name()) and $1->get_name()!="print_ln"){
        error_cnt++;
        fprintf(errorfile , "Error at line: %d Undeclared Function %s\n\n",line,$1->get_name().c_str());
      }
      else{

        function_ f = get_func($1->get_name());

        //chk args consistency
        bool matched = true;
        bool already_error_in_arg = false;
        if(f.params.size() != $3->arg_list.size())matched = false;
        else{
          for(int i=0;i<f.params.size();i++){
            if($3->arg_list[i].already_error_in_arg){
              already_error_in_arg = true;
            }
            if($3->arg_list[i].sz>0){
              if($3->get_name()==modified_name($3->get_name())){
                matched = false;
                //break;
              }
            }
            if(f.params[i].first != $3->arg_list[i].type){
              matched = false;
              //break;
            }
            //will have to see maybe
            if(matched){
              $$->add_code("\n\tMOV AX, " + $3->arg_list[i].name + table.get_current_id()[0]+"\n");
							$$->add_code("\tMOV "+f.params[i].second +table.get_current_id()[0]+", AX\n");
            }
          }
        }

        if(!matched ){
          error_cnt++;
          fprintf(errorfile , "Error at line: %d Total number of arguments mismatch in function %s\n\n",line,$1->get_name().c_str());
          fprintf(logfile , "Error at line: %d Total number of arguments mismatch in function %s\n\n",line,$1->get_name().c_str());
        }
        else{
          $$->add_code("\tCALL "+$1->get_name()+"\n");

          string fnm = modified_name_while_func_calling($1->get_name());
          function_ f = get_func(fnm);
          $$->set_assembly_value("T" + to_string(f.return_reg_no));

        }
      }

    }
	| LPAREN expression RPAREN
    {
      $$ =$2;
      $$->set_assembly_value($$->get_name());
      $$->setVariableType($2->getVariableType());

      fprintf(logfile,"Line %d: factor : LPAREN expression RPAREN\n\n",line);
      fprintf(logfile,"(%s)\n\n",$2->get_name().c_str());

    }
	| CONST_INT
    {
      $$=$1;
			$$->set_assembly_value($$->get_name());
			$$->setVariableType("int");

      fprintf(logfile,"Line %d: factor : CONST_INT\n\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());

    }
	| CONST_FLOAT
    {
      $$=$1;
      $$->set_assembly_value($$->get_name());
			$$->setVariableType("float");

      fprintf(logfile,"Line %d: factor : CONST_FLOAT\n\n",line);
      fprintf(logfile,"%s\n\n",$1->get_name().c_str());

    }
	| variable INCOP
    {
      $$=new SymbolInfo($1->get_name(),$1->get_type());

      SymbolInfo *x=table.Lookup(modified_name($1->get_name()));
			if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,$1->get_name().c_str());
      }
      else{
         //assembly
				string var_name=modified_name($1->get_name())+table.get_current_id()[0];

				$$->set_name(var_name);

				//array
				if($1->getIdentity()=="array"){
          int idx = get_index($1->get_name());
					if(idx==0)$$->add_code("\tMOV AX, "+var_name + "\n");
          else $$->add_code("\tMOV AX, "+var_name+"+"+to_string(idx)+"*2\n");

          $$->add_code("\tINC AX\n");

					if(idx==0)$$->add_code("\tMOV "+var_name + ", AX\n");
          else $$->add_code("\tMOV "+var_name+"+"+to_string(idx)+"*2, AX\n");
				}

				else{
					$$->add_code("\tMOV AX, "+var_name+"\n");
					$$->add_code("\tINC AX\n");
					$$->add_code("\tMOV "+var_name+", AX\n");
				}

         $$->setVariableType($1->getVariableType());
         $$->setIdentity($1->getIdentity());
      }

    }
	| variable DECOP
    {
      $$ = new SymbolInfo($1->get_name(),$1->get_type());

      SymbolInfo *x=table.Lookup(modified_name($1->get_name()));
			if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,$1->get_name().c_str());
      }
      else{
         //assembly
				string var_name=modified_name($1->get_name())+table.get_current_id()[0];
				string temp_str=newTemp();

				$$->set_name(var_name);

				//array
				if($1->getIdentity()=="array"){

					$$->add_code("\tMOV AX, "+var_name+"+"+to_string($1->idx)+"*2\n");
					$$->add_code("\tMOV "+temp_str+", AX\n");
					$$->add_code("\tDEC AX\n");
					$$->add_code("\tMOV "+var_name+"+"+to_string($1->idx)+"*2, AX\n");
				}

				else{
					$$->add_code("\tMOV AX, "+var_name+"\n");
					$$->add_code("\tMOV "+temp_str+", AX\n");
					$$->add_code("\tDEC AX\n");
					$$->add_code("\tMOV "+var_name+", AX\n");
				}
				$$->set_name(temp_str);

        $$->setVariableType($1->getVariableType());
        $$->setIdentity($1->getIdentity());
      }
    }
	;

argument_list : arguments
        {
    			$$=$1;
          fprintf(logfile,"Line %d: argument_list : arguments\n\n",line);
         fprintf(logfile,"%s\n\n",$1->get_name().c_str());
        }
			  |
        {
          $$ = new SymbolInfo("" , "argument_list");
        }
			  ;

arguments : arguments COMMA logic_expression
        {
          $$ = new SymbolInfo($1->get_name()+" , "+$3->get_name() , "arguments");
          fprintf(logfile,"Line %d: arguments : arguments COMMA logic_expression\n\n",line);
          fprintf(logfile,"%s , %s\n\n",$1->get_name().c_str(),$3->get_name().c_str());

          $$->arg_list = $1->arg_list;
          bool isara=false;
          for(int i=0;i<var_list.size();i++){
            if($3->get_name()==var_list[i].var_name && var_list[i].var_size>0){
                isara = true;
                if($3->get_already_error_in_param()){
                  $$->push_in_arg_AR($3->get_name() , $3->getVariableType() , var_list[i].var_size);break;
                }
                else $$->push_in_arg($3->get_name() , $3->getVariableType() , var_list[i].var_size);break;
            }
          }
          if(!isara){
            $$->push_in_arg($3->get_name() , $3->getVariableType() , 0);
          }

        }
	      | logic_expression
        {
          fprintf(logfile,"At line no: %d arguments : logic_expression\n\n",line);
    			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
    			$$=$1;
          bool isara=false;
          for(int i=0;i<var_list.size();i++){
            if($1->get_name()==var_list[i].var_name && var_list[i].var_size>0){
              isara = true;
              if($1->get_already_error_in_param()){
                $$->push_in_arg_AR($1->get_name() , $1->getVariableType() , var_list[i].var_size);break;
              }
              else $$->push_in_arg($1->get_name() , $1->getVariableType() , var_list[i].var_size);break;
            }
          }
          if(!isara){
            $$->push_in_arg($1->get_name() , $1->getVariableType() , 0);
          }
        }

	      ;


%%
int main(int argc,char *argv[])
{

	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("error in opening input file\n");
		exit(1);
	}

	yyin=fp;
  asmCode=fopen("code.asm","w");
	fclose(asmCode);

  asmCode=fopen("code.asm","a");


	yyparse();

	fprintf(logfile,"\t\tSymbol Table : \n\n");
	table.printall(logfile);
	fprintf(logfile,"Total Lines : %d \n\n",line);
	fprintf(logfile,"Total Errors : %d \n\n",error_cnt);
	fprintf(errorfile,"\nTotal Errors : %d \n\n",error_cnt);

	fclose(fp);
	fclose(logfile);
	fclose(errorfile);

	return 0;
}


/*
input1.txt

-------------------------------------------------------------
int x,y,z; float a;

void foo();

int var(int a, int b){
	return a+b;
}

void foo(){
	x=2;
	y=x-5;
}

int main(){
	int a[2],c,i,j ; float d;
	a[0]=1;
	a[1]=5;
	i= a[0]+a[1];
	j= 2*3+(5%3 < 4 && 8) || 2 ;
	d=var(1,2*3)+3.5*2;
	return 0;
}


------------------------------------------------------


input2.txt

------------------------------------------------------
int func(int a){
	return a;
}

int main(){
	int a[2],c,i,j ; float c;
	a[2.5]=1;
	i=2.3;
	j=2%3.7;
	a=4;
	func(a);
	b=8;
	return 0;
}

-------------------------------------------------------

*/
