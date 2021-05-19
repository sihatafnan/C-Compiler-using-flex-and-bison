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
FILE *fp,*asmCode,*optimized_asmCode;
FILE *errorfile = fopen("error.txt","w");
FILE *logfile = fopen("log.txt" , "w");

SymbolTable table(30);
SymbolInfo *currentFunction;
vector<string> statement_list;
vector<pair<string,string>> variableListForInit;

extern int line;
extern int error_cnt;

string codes, assemblyCodes;
string variable_type;
string current_return_type;
struct var{
    string var_name;
    int var_size;  // it is set to -1 for variables
} temp_var;
vector<var> var_list;  // for identifier(variable, array) insertion into symbolTable

struct function_{
  string f_name;
  vector<pss> params;
  string return_type;
} temp_func;

vector<function_> func_list;

void insert_in_func_list(string nm , vector<parameter>& p , string ret_type){
    temp_func.f_name = nm;
    temp_func.return_type = ret_type;
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
  }
}
string to_str(int n)
{
  if(n==0)return "0";
	string temp;
	while(n){
		int r=n%10;
		n/=10;
		temp.push_back(r+48);
  }

	reverse(temp.begin(),temp.end());
	return temp;
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


int labelCount=1, tempCount=1;
bool isReturning;
string isReturningType;
string newLabel()
{
	string temp="L"+to_str(labelCount);
	labelCount++;
	return temp;
}

string newTemp()
{
	string temp="T"+to_str(tempCount);
	tempCount++;

	variableListForInit.push_back({temp,"0"});
	return temp;
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
    $$->set_code(assemblyCodes);
    if(error_cnt==0){
      string init=".MODEL SMALL\n.STACK 100H\n";

		 		init+=".DATA\n";
        //for(int i=0;i<variableListForInit.size();i++){cout<<variableListForInit[i].first<<" "<<variableListForInit[i].second<<endl;}
		 		//variables
		 		for(int i=0;i<variableListForInit.size();i++){
		 			if(variableListForInit[i].second=="0")
		 				init+=("\t"+variableListForInit[i].first+" DW ?\n");
		 			else
		 				init+=("\t"+variableListForInit[i].first+" DW "+variableListForInit[i].second+" DUP(?)\n");
		 		}

		 		init+=".CODE\n";

		 		//function for PRINTLN
		 		init+="PRINT_ID PROC\n\n";
		 		init+="\t;SAVE IN STACK\n";
		 		init+="\tPUSH AX\n";
		 		init+="\tPUSH BX\n";
		 		init+="\tPUSH CX\n";
		 		init+="\tPUSH DX\n\n";

		 		init+="\t;CHECK IF NEGATIVE\n";
		 		init+="\tOR AX, AX\n";
		 		init+="\tJGE PRINT_NUMBER\n\n";
		 		init+="\t;PRINT MINUS SIGN\n";
		 		init+="\tPUSH AX\n";
		 		init+="\tMOV AH, 2\n";
		 		init+="\tMOV DL, '-'\n";
		 		init+="\tINT 21H\n";
		 		init+="\tPOP AX\n\n";
		 		init+="\tNEG AX\n\n";
		 		init+="\tPRINT_NUMBER:\n";
		 		init+="\tXOR CX, CX\n";
		 		init+="\tMOV BX, 10D\n\n";
		 		init+="\tREPEAT_CALC:\n\n";
		 		init+="\t\t;AX:DX- QUOTIENT:REMAINDER\n";
		 		init+="\t\tXOR DX, DX\n";
		 		init+="\t\tDIV BX  ;DIVIDE BY 10\n";
		 		init+="\t\tPUSH DX ;PUSH THE REMAINDER IN STACK\n\n";
		 		init+="\t\tINC CX\n\n";
		 		init+="\t\tOR AX, AX\n";
		 		init+="\t\tJNZ REPEAT_CALC\n\n";

		 		init+="\tMOV AH, 2\n\n";
		 		init+="\tPRINT_LOOP:\n";
		 		init+="\t\tPOP DX\n";
		 		init+="\t\tADD DL, 30H\n";
		 		init+="\t\tINT 21H\n";
		 		init+="\t\tLOOP PRINT_LOOP\n";

		 		init+="\n\t;NEWLINE\n";
		 		init+="\tMOV AH, 2\n";
		 		init+="\tMOV DL, 0AH\n";
		 		init+="\tINT 21H\n";
		 		init+="\tMOV DL, 0DH\n";
		 		init+="\tINT 21H\n\n";

		 		init+="\tPOP AX\n";
		 		init+="\tPOP BX\n";
		 		init+="\tPOP CX\n";
		 		init+="\tPOP DX\n\n";
		 		init+="\tRET\n";
		 		init+="PRINT_ID ENDP\n\n";
        //cout<<assemblyCodes<<endl;
		 		fprintf(asmCode,"%s",init.c_str());
		 		fprintf(asmCode,"%s",$$->get_code().c_str());
		 	}
    }
	;

program : program unit
	{

		SymbolInfo *x = new SymbolInfo((string)$1->get_name()+(string)$2->get_name(), "NON_TERMINAL");
		$$ = x;
    $$->set_code($1->get_code() + $2->get_code());
	}
	| unit
	{
		$$ = new SymbolInfo($1->get_name()+"\n", "NON_TERMINAL");
	}
	;

unit : var_declaration
	{
		$$ = $1;
	}
     | func_declaration
	{
		$$ = $1;
	}
     | func_definition
	{
		$$ = $1;
	}
     ;


func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
	{
    $$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"("+$4->get_name()+");", "NON_TERMINAL");

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
      insert_in_func_list($2->get_name() , $4->param , $1->get_name());
    }

	}
	| type_specifier ID LPAREN RPAREN SEMICOLON
	{
		$$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"();", "NON_TERMINAL");
		//fprintf(logfile , "At line no: %d func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON\n\n",line);
		//fprintf(logfile , "%s %s();\n\n" , $1->get_name().c_str() , $2->get_name().c_str());


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
			table.Insert($2->get_name() , "ID" , logfile);
      //insert in func_list
      insert_in_func_list($2->get_name() , $4->param , $1->get_name());
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

  {
      //-------------------------------Assembly generation------------------------------------
      if($2->get_name()=="main")
				assemblyCodes="MAIN PROC\n\n";
			else
				assemblyCodes=$2->get_name()+" PROC\n\n";

			//if main function then initialize data segment
			if($2->get_name()=="main"){
				assemblyCodes+="\t;INITIALIZE DATA SEGMENT\n";
				assemblyCodes+="\tMOV AX, @DATA\n";
				assemblyCodes+="\tMOV DS, AX\n\n";
			}

			else{
				assemblyCodes+="\tPUSH AX\n";
				assemblyCodes+="\tPUSH BX\n";
				assemblyCodes+="\tPUSH CX\n";
				assemblyCodes+="\tPUSH DX\n";
			}

			//function body
			assemblyCodes+=$7->get_code();

			//ending of function
			if($2->get_name()=="main") {
				assemblyCodes+="\n\tMOV AX, 4CH\n\tINT 21H";
				assemblyCodes+=("\nMAIN ENDP\n\nEND MAIN");
			}

			else{
				assemblyCodes+="\tPOP AX\n";
				assemblyCodes+="\tPOP BX\n";
				assemblyCodes+="\tPOP CX\n";
				assemblyCodes+="\tPOP DX\n";

				assemblyCodes+="RET\n";
				assemblyCodes+=$2->get_name()+" ENDP\n\n";
			}

      //-------------------------------Assembly done-------------------------------------------
      $$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"("+$4->get_name()+")"+$7->get_name()+"\n\n", "NON_TERMINAL");
      $$->set_code(assemblyCodes);

  }
		| type_specifier ID LPAREN RPAREN
    {
      current_return_type = $1->get_name();
      /* checking whether already declared or not */
  		SymbolInfo* temp = table.Lookup_in_current($2->get_name());
      if(temp != NULL and (!temp->get_is_declared_func())) {
  			error_cnt++;
  			fprintf(errorfile , "Error at line %d: Multiple definition of %s\n\n" , line , $2->get_name().c_str());
  		}
  		else if(temp == NULL){
  			table.Insert($2->get_name() , "ID" , logfile);
        insert_in_func_list($2->get_name() , $1->get_name());
  		}

      table.Enter_Scope(logfile);

    }
    compound_statement {table.printall(logfile);table.Exit_Scope(logfile);var_list.clear();}
 	{
      //------------------------------------assembly generation------------------------------
      if($2->get_name()=="main")
				assemblyCodes="MAIN PROC\n\n";
			else
				assemblyCodes=$2->get_name()+" PROC\n\n";

			//if main function then initialize data segment
			if($2->get_name()=="main"){
				assemblyCodes+="\t;INITIALIZE DATA SEGMENT\n";
				assemblyCodes+="\tMOV AX, @DATA\n";
				assemblyCodes+="\tMOV DS, AX\n\n";
			}

			else{
				assemblyCodes+="\tPUSH AX\n";
				assemblyCodes+="\tPUSH BX\n";
				assemblyCodes+="\tPUSH CX\n";
				assemblyCodes+="\tPUSH DX\n";
			}

			//function body
			assemblyCodes+=$6->get_code();

			//ending of function
			if($2->get_name()=="main") {
				assemblyCodes+="\n\tMOV AX, 4CH\n\tINT 21H";
				assemblyCodes+=("\nMAIN ENDP\n\nEND MAIN");
			}

			else{
				assemblyCodes+="\tPOP AX\n";
				assemblyCodes+="\tPOP BX\n";
				assemblyCodes+="\tPOP CX\n";
				assemblyCodes+="\tPOP DX\n";

				assemblyCodes+="RET\n";
				assemblyCodes+=$2->get_name()+" ENDP\n\n";
			}
			//-------------------------------------------------------------------------

      $$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"()"+$6->get_name()+"\n\n", "NON_TERMINAL");
      $$->set_code(assemblyCodes);
	}

	 	;


parameter_list  : parameter_list COMMA type_specifier ID
		{
			$$ = new SymbolInfo($1->get_name()+","+$3->get_name()+" "+$4->get_name(), "NON_TERMINAL");
			////fprintf(logfile,"At line no: %d parameter_list  : parameter_list COMMA type_specifier ID\n\n",line);
			//fprintf(logfile , "%s , %s %s\n\n" , $1->get_name().c_str(),$3->get_name().c_str(),$4->get_name().c_str());

			/* adding parameter to parameter list */
      $$->param = $1->param;
			$$->push_in_param($4->get_name() , $3->get_name());

		}
		| parameter_list COMMA type_specifier
		{
			$$ = new SymbolInfo($1->get_name()+","+$3->get_name(), "NON_TERMINAL");
			////fprintf(logfile,"At line no: %d parameter_list  : parameter_list COMMA type_specifier\n\n",line);
			//fprintf(logfile , "%s , %s\n\n" , $1->get_name().c_str(),$3->get_name().c_str());

			/* adding parameter to parameter list */
      $$->param = $1->param;
			$$->push_in_param("", $3->get_name());
		}
 		| type_specifier ID
		{
			$$ = new SymbolInfo($1->get_name()+" "+$2->get_name(), "NON_TERMINAL");
			////fprintf(logfile,"At line no: %d parameter_list  : type_specifier ID\n\n",line);
			//fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str(),$2->get_name().c_str());

			/* adding parameter to parameter list */

			$$->push_in_param($2->get_name() ,$1->get_name());
		}
		| type_specifier
		{
			$$ = new SymbolInfo($1->get_name(), "NON_TERMINAL");
			////fprintf(logfile,"At line no: %d parameter_list  : type_specifier\n\n",line);
			//fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str());

			/* adding parameter to parameter list */

			$$->push_in_param( "" , $1->get_name());
		}

 		;


compound_statement : LCURL statements RCURL
  {
      $$=$2;
      //$$ = new SymbolInfo("{\n"+$2->get_name()+"\n}"+"\n\n", "NON_TERMINAL");
      ////fprintf(logfile,"At line no: %d compound_statement : LCURL statements RCURL\n\n",line);
			//fprintf(logfile,"{\n%s\n}\n\n",$2->get_name().c_str());

  }
 		    | LCURL RCURL
  {
    $$ = new SymbolInfo("{\n}", "NON_TERMINAL");
    ////fprintf(logfile,"At line no: %d compound_statement : LCURL RCURL\n\n",line);
    //fprintf(logfile,"{}\n\n");

  }
 		    ;

var_declaration : type_specifier declaration_list SEMICOLON
		{
			////fprintf(logfile,"At line no: %d var_declaration : type_specifier declaration_list SEMICOLON\n\n",line);
		//	fprintf(logfile,"%s %s;\n\n",$1->get_name().c_str(),$2->get_name().c_str());
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

			//var_list.clear();
		}

 		;

type_specifier	: INT
		{
			////fprintf(logfile,"At line no: %d: type_specifier : INT \n\n",line);
			variable_type = "int";

			SymbolInfo *x = new SymbolInfo("int" , "int");
			$$ = x;
			//fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
 		| FLOAT
		{
			////fprintf(logfile,"At line no: %d: type_specifier : FLOAT \n",line);
			variable_type = "float";

			SymbolInfo *x = new SymbolInfo("float" , "float");
			$$ = x;
			//fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
 		| VOID
 		{
			 ////fprintf(logfile,"At line no: %d: type_specifier : VOID \n",line);
			variable_type = "void";

			SymbolInfo *x = new SymbolInfo("void" , "void");
			$$ = x;
			//fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
		 ;

declaration_list : declaration_list COMMA ID
		{
      //----------------------------assembly generation---------------------------
 			variableListForInit.push_back({$3->get_name()+table.get_current_id()[0],"0"});
 			//---------------------------------------------------------------------

			////fprintf(logfile,"At line no: %d: declaration_list : declaration_list COMMA ID\n\n",line);
			$$ = new SymbolInfo((string)$1->get_name()+(string)","+(string)$3->get_name(), "NON_TERMINAL");
			//fprintf(logfile , "%s,%s\n\n" , $1->get_name().c_str() , $3->get_name().c_str());

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
      //----------------------------assembly generation---------------------------
 			variableListForInit.push_back({$3->get_name()+table.get_current_id()[0],$5->get_name()});
 			//---------------------------------------------------------------------

			////fprintf(logfile,"At line no: %d declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD\n\n",line);
			$$ = new SymbolInfo((string)$1->get_name()+(string)","+(string)$3->get_name()+(string)"["+(string)$5->get_name()+(string)"]", "NON_TERMINAL");
		//	fprintf(logfile , "%s,%s[%s]\n\n" , $1->get_name().c_str() , $3->get_name().c_str() , $5->get_name().c_str());

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
      //----------------------------assembly generation---------------------------
 			variableListForInit.push_back({$1->get_name()+table.get_current_id()[0],"0"});
 			//---------------------------------------------------------------------

			////fprintf(logfile,"At line no: %d declaration_list : ID\n\n",line);
 		//	fprintf(logfile,"%s\n\n",$1->get_name().c_str());

 			$$ = new SymbolInfo($1->get_name() ,  "ID");

			 /* keeping track of identifier(variable) */
      temp_var.var_name = (string)$1->get_name();
      temp_var.var_size = -1;
      var_list.pb(temp_var);

      $$->push_in_var($1->get_name() , "" , 0);

		}
 		| ID LTHIRD CONST_INT RTHIRD
		{
      //----------------------------assembly generation---------------------------
 			variableListForInit.push_back({$1->get_name()+table.get_current_id()[0],$3->get_name()});
 			//---------------------------------------------------------------------

			//fprintf(logfile , "At line no: %d  declaration_list: ID LTHIRD CONST_INT RTHIRD\n\n",line);
			$$ = new SymbolInfo($1->get_name()+"["+$3->get_name()+"]", "NON_TERMINAL");
			//fprintf(logfile , "%s[%s]\n\n",$1->get_name().c_str() , $3->get_name().c_str());

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
      // fprintf(logfile , "At line no: %d statements : statement\n\n" , line);
       //fprintf(logfile , "%s\n\n" , $1->get_name().c_str());

    }
	   | statements statement
    {
      $$ = new SymbolInfo($1->get_name()+$2->get_name() , "NON_TERMINAL");
    //  fprintf(logfile , "At line no: %d statements : statements statement\n\n" , line);
    //  fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str(),$2->get_name().c_str());
      $$->set_code($1->get_code() + $2->get_code());
    }
	   ;

statement : var_declaration
    {
      ////fprintf(logfile,"At line no: %d statement : var_declaration\n\n",line);
			//fprintf(logfile,"%s\n\n",$1->get_name().c_str());
      $1->setname($1->get_name()+"\n");
  		$$=$1;
    }
	  | expression_statement
    {
      ////fprintf(logfile,"At line no: %d statement : expression_statement\n\n",line);
			//fprintf(logfile,"%s\n\n",$1->get_name().c_str());
      $1->setname($1->get_name()+"\n");
			$$=$1;
    }
	  | compound_statement
    {
      ////fprintf(logfile,"At line no: %d statement : compound_statement\n\n",line);
      //fprintf(logfile,"%s\n\n",$1->get_name().c_str());

      $$=$1;
    }
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement
    {
      string str="for("+$3->get_name()+$4->get_name()+$5->get_name()+")"+$7->get_name();
      //$$ = new SymbolInfo(str , "NON_TERMINAL");
      $$ = $3;
      string label1=newLabel(), label2=newLabel();

			assemblyCodes=$$->get_code();
			assemblyCodes+=(label1+":\n");	//REPEAT

			assemblyCodes+=$4->get_code();

			assemblyCodes+=("\tMOV AX, "+$4->get_name()+"\n");
			assemblyCodes+="\tCMP AX, 0\n";
			assemblyCodes+="\tJE "+label2+"\n";

			assemblyCodes+=$7->get_code();
			assemblyCodes+=$5->get_code();
			assemblyCodes+="\tJMP "+label1+"\n";

			assemblyCodes+=("\t"+label2+":\n");

			$$->set_code(assemblyCodes);
      //fprintf(logfile,"line no. %d: statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement\n",line);
			//fprintf(logfile,"%s\n\n",str.c_str());
    }
	  | IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE
    {
      string str = "if("+$3->get_name()+")"+$5->get_name();
      //$$ = new SymbolInfo(str , "statement");
      ////fprintf(logfile,"At line no: %d statement : IF LPAREN expression RPAREN statement\n\n",line);
      //fprintf(logfile,"%s\n\n",str.c_str());
      $$=$3;

			string label=newLabel();

			assemblyCodes=$$->get_code();
			assemblyCodes+=("\tMOV AX, "+$3->get_name()+"\n");

			assemblyCodes+="\tCMP AX, 0\n";
			assemblyCodes+=("\tJE "+label+"\n");
			assemblyCodes+=$5->get_code();
			assemblyCodes+=("\t"+label+":\n");

			$$->set_code(assemblyCodes);
      $$->setname("statement");$$->settype("if");

    }
	  | IF LPAREN expression RPAREN statement ELSE statement
    {
      string str = "if("+$3->get_name()+")"+$5->get_name()+"else"+$7->get_name();
      //$$ = new SymbolInfo(str , "statement");
      ////fprintf(logfile,"At line no: %d statement : IF LPAREN expression RPAREN statement ELSE statement\n\n",line);
      //fprintf(logfile,"%s\n\n",str.c_str());
      $$=$3;

			string else_condition=newLabel();
			string after_else=newLabel();

			assemblyCodes=$$->get_code();

			assemblyCodes+=("\tMOV AX, "+$3->get_name()+"\n");
			assemblyCodes+="\tCMP AX, 0\n";
			assemblyCodes+=("\tJE "+else_condition+"\n");		//false, jump to else

			assemblyCodes+=$5->get_code();					//true
			assemblyCodes+=("\tJMP "+after_else);

			assemblyCodes+=("\n\t"+else_condition+":\n");
			assemblyCodes+=$7->get_code();
			assemblyCodes+=("\n\t"+after_else+":\n");

			$$->set_code(assemblyCodes);
			$$->setname("statement");$$->settype("if-else if");

    }
	  | WHILE LPAREN expression RPAREN statement
    {
      string str = "while("+$3->get_name()+")"+$5->get_name();
      //$$ = new SymbolInfo(str , "statement");
      ////fprintf(logfile,"At line no: %d statement : WHILE LPAREN expression RPAREN statement\n\n",line);
      //fprintf(logfile,"%s\n\n",str.c_str());
      $$=new SymbolInfo("while","loop");

			string label1=newLabel(), label2=newLabel();

			assemblyCodes=(label1+":\n");	//REPEAT

			//check if we can continue executing
			assemblyCodes+=$3->get_code();

			assemblyCodes+=("\tMOV AX, "+$3->get_name()+"\n");
			assemblyCodes+="\tCMP AX, 0\n";
			assemblyCodes+="\tJE "+label2+"\n";

			assemblyCodes+=$5->get_code();	//execute the statements inside while
			assemblyCodes+="\tJMP "+label1+"\n";

			assemblyCodes+=("\t"+label2+":\n");

			$$->set_code(assemblyCodes);

    }
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
    {
      //$$ = new SymbolInfo("printf("+$3->get_name()+");" , "statement");

      $$=new SymbolInfo("println","nonterminal");
			assemblyCodes=("\n\tMOV AX, "+$3->get_name()+table.get_current_id()[0]+"\n");
			assemblyCodes+=("\tCALL PRINT_ID\n");
			$$->set_code(assemblyCodes);

      if($3->get_is_func()){
        if(!table.Lookup_in_current(modified_name_while_func_calling($3->get_name()))){
          error_cnt++;
          fprintf(errorfile , "Error at line: %d Undeclared function %s\n\n" , line,modified_name_while_func_calling($3->get_name()).c_str());
          fprintf(logfile , "Error at line: %d Undeclared function %s\n\n" , line,modified_name_while_func_calling($3->get_name()).c_str());
        }
      }

      else if(!table.Lookup_in_current($3->get_name())){
        error_cnt++;
        fprintf(errorfile , "Error at line: %d Undeclared Variable %s\n\n" , line,$3->get_name().c_str());
        fprintf(logfile , "Error at line: %d Undeclared Variable %s\n\n" , line,$3->get_name().c_str());
      }


    }
	  | RETURN expression SEMICOLON
    {
      $$=new SymbolInfo("return","statement");

			isReturning=true;
			isReturningType=$2->getVariableType();

			assemblyCodes=$$->get_code();///will have to chk ig
			$$->set_code(assemblyCodes);
    }
	  ;

expression_statement 	: SEMICOLON
    {
      $$=new SymbolInfo("SEMICOLON","SEMICOLON");
    }
			| expression SEMICOLON
    {
      $$ = $1;
    }
			;

variable :

    ID
    {
      $$ = $1;
      $$->idx=-1;
      //Semantic : chk if variable is declared before


      $$->setIdentity("var");
      $$->push_in_var($1->get_name(),"",0);
      SymbolInfo *x=table.Lookup($1->get_name());
      if(x)$$->setVariableType(x->getVariableType());

      $$->asmName=$$->get_name()+table.get_current_id()[0];

/*
      if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line: %d variable %s not declared in this scope\n\n",line,$1->get_name().c_str());
      }
      else{
        $$->setVariableType(x->getVariableType());
      }
*/
    }

	 | ID LTHIRD expression RTHIRD
   {
     ////fprintf(logfile,"At line no: %d variable : ID LTHIRD expression RTHIRD\n",line);
		 //fprintf(logfile,"%s[%s]\n\n",$1->get_name().c_str(),$3->get_name().c_str());
     $$ = new SymbolInfo($1->get_name()+"["+$3->get_name()+"]" , "variable");

     $$->setIdentity("array");
     stringstream geek($3->get_name());
     int x = 0;
     geek >> x;
     $$->idx=x;
     $$->asmName=$$->get_name()+table.get_current_id()[0];

   }

	 ;

 expression : logic_expression
    {
      $$ = $1;
    }
	   | variable ASSIGNOP logic_expression
    {
      $$ = new SymbolInfo($1->get_name()+"="+$3->get_name() , "expression");

      //semantics
      //todo
      //assign $3's variable_type to $1 after some error chkings

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

      //-----------------------------assembly generation--------------------------------

			assemblyCodes=$3->get_code()+$1->get_code();
      //cout<<$3->asmName<<endl;
			assemblyCodes+=("\n\tMOV AX, "+$3->asmName+"\n");

      string temp=modified_name($1->get_name())+table.get_current_id()[0];
			if($1->getIdentity()!="array"){                   ///modified
				assemblyCodes+=("\tMOV "+temp+", AX\n");
			}
			//array

			else{
        int idx=get_index($1->get_name());
				if(idx==0)assemblyCodes+=("\tMOV "+temp+", AX\n");
        else assemblyCodes+=("\tMOV "+temp+"+"+to_str(idx)+"*2, AX\n");
			}

			$$->set_code(assemblyCodes);
			$$->setname(temp);
			$$->asmName=temp;

    }
	   ;

logic_expression : rel_expression
    {
      $$ = $1;
      $$->setVariableType("int");
      //cout<<$1->asmName<<endl;
    }
		 | rel_expression LOGICOP rel_expression
    {
      $$ = $1;
      ////fprintf(logfile,"At line no: %d logic_expression : rel_expression LOGICOP rel_expression\n\n",line);
      //fprintf(logfile,"%s%s%s\n\n",$1->get_name().c_str(),$2->get_name().c_str(),$3->get_name().c_str());

      /*semantic
      both $1 and $3 must be of type non void
      $$ must be set to type int
      */
      if($1->getVariableType()=="void" || $3->getVariableType()=="void"){
 				error_cnt++;
 				fprintf(errorfile,"Error at line %d found: Type Mismatch(Operands of %s can't be void)\n\n",line,$2->get_name().c_str());
 			}

       $$->setVariableType("int");

       //---------------------------------assembly generation---------------------------------
			assemblyCodes=$$->get_code()+$3->get_code();

			string temp=newTemp();
			string label1=newLabel();
			string label2=newLabel();

			assemblyCodes+=("\n\tMOV AX, "+$1->asmName+"\n");
			assemblyCodes+=("\tMOV BX, "+$3->asmName+"\n");

			if($2->get_name()=="&&"){
				assemblyCodes+=("\tCMP AX, 1\n");
				assemblyCodes+=("\tJNE "+label1+"\n");

				assemblyCodes+=("\tCMP BX, 1\n");
				assemblyCodes+=("\tJNE "+label1+"\n");

				assemblyCodes+=("\tMOV AX, 1\n");
				assemblyCodes+=("\tMOV "+temp+", AX\n");
				assemblyCodes+=("\tJMP "+label2+"\n");

				assemblyCodes+=("\n\t"+label1+":\n");
				assemblyCodes+=("\tMOV AX, 0\n");
				assemblyCodes+=("\tMOV "+temp+", AX\n");

				assemblyCodes+=("\n\t"+label2+":\n");
			}

			else if($2->get_name()=="||"){
				assemblyCodes+=("\tCMP AX, 1\n");
				assemblyCodes+=("\tJE "+label1+"\n");

				assemblyCodes+=("\tCMP BX, 1\n");
				assemblyCodes+=("\tJE "+label1+"\n");

				assemblyCodes+=("\tMOV AX, 0\n");
				assemblyCodes+=("\tMOV "+temp+", AX\n");
				assemblyCodes+=("\tJMP "+label2+"\n");

				assemblyCodes+=("\n\t"+label1+":\n");
				assemblyCodes+=("\tMOV AX, 1\n");
				assemblyCodes+=("\tMOV "+temp+", AX\n");

				assemblyCodes+=("\n\t"+label2+":\n");
			}

			$$->set_code(assemblyCodes);
			$$->setname(temp);
			$$->asmName=temp;
			//---------------------------------------------------

    }
		 ;

rel_expression	: simple_expression
   {
     $$ = $1;
     $$->setVariableType("int");
   }
		| simple_expression RELOP simple_expression
   {
     $$ = $1;
     /*semantic
     both $1 and $3 must be of type non void
     $$ must be set to type int
     */
     if($1->getVariableType()=="void" || $3->getVariableType()=="void"){
				error_cnt++;
				fprintf(errorfile,"Error at line %d found: Type Mismatch(Operands of %s can't be void)\n\n",line,$2->get_name().c_str());
			}
      $$->setVariableType("int");

      //------------------------------------------------------------------
			//code generation
			//here two expressions are already in two variables, we compare them
			//if true send them to label1, else assign false to the new temp and jump to label2
			//from label1 assign true, eventually it will get down to label2

			assemblyCodes=$$->get_code()+$3->get_code();

			assemblyCodes+=("\n\tMOV AX, "+$1->asmName+"\n");
			assemblyCodes+=("\tCMP AX, "+$3->asmName+"\n");

			string temp=newTemp();
			string label1=newLabel();
			string label2=newLabel();

			if($2->get_name()=="<"){
				assemblyCodes+=("\tJL "+label1+"\n");
			}

			else if($2->get_name()=="<="){
				assemblyCodes+=("\tJLE "+label1+"\n");
			}

			else if($2->get_name()==">"){
				assemblyCodes+=("\tJG "+label1+"\n");
			}

			else if($2->get_name()==">="){
				assemblyCodes+=("\tJGE "+label1+"\n");
			}

			else if($2->get_name()=="=="){
				assemblyCodes+=("\tJE "+label1+"\n");
			}

			else{
				assemblyCodes+=("\tJNE "+label1+"\n");
			}

			assemblyCodes+=("\n\tMOV "+temp+", 0\n");
			assemblyCodes+=("\tJMP "+label2+"\n");

			assemblyCodes+=("\n\t"+label1+":\n\tMOV "+temp+", 1\n");
			assemblyCodes+=("\n\t"+label2+":\n");

			$$->setname(temp);
			$$->asmName=temp;
			$$->set_code(assemblyCodes);

      //----------------------------------------------------------------------
   }
		;

simple_expression : term
  {
    $$ = $1;

  }
		  | simple_expression ADDOP term
  {
    $$ = $1;

    if($1->getVariableType()=="float" || $3->getVariableType()=="float")
				$$->setVariableType("float");
		else
				$$->setVariableType("int");

        //--------------------------assembly generation----------------------------
        assemblyCodes=$$->get_code();
  			assemblyCodes+=$3->get_code();

  			// move one of the operands to a register
  			//perform addition or subtraction with the other operand and
  			//move the result in a temporary variable

  			string temp=newTemp();
  			if($2->get_name()=="+"){
  				assemblyCodes+=("\n\tMOV AX, "+$1->asmName+"\n");
  				assemblyCodes+=("\tADD AX, "+$3->asmName+"\n");
  				assemblyCodes+=("\tMOV "+temp+", AX\n");
  			}

  			else{
  				assemblyCodes+=("\n\tMOV AX, "+$1->asmName+"\n");
  				assemblyCodes+=("\tSUB AX, "+$3->asmName+"\n");
  				assemblyCodes+=("\tMOV "+temp+", AX\n");
  			}

  			$$->set_code(assemblyCodes);
  			$$->setname(temp);
  			$$->asmName=temp;

  }
		  ;

term :	unary_expression
    {
      $$ = $1;
    }
     |  term MULOP unary_expression
    {
      $$=$1;
			//------------------------------------------------------------------------
			//code generation
      assemblyCodes=$$->get_code();
			assemblyCodes += $3->get_code();
			assemblyCodes += "\n\tMOV AX, "+ $1->asmName+"\n";
			assemblyCodes += "\tMOV BX, "+ $3->asmName+"\n";

			string temp=newTemp();

			if($2->get_name()=="*"){
				assemblyCodes += "\tMUL BX\n";
				assemblyCodes += "\tMOV "+temp+", AX\n";
			}

			else if($2->get_name()=="/"){
				// clear dx, perform 'div bx' and mov ax to temp
				assemblyCodes += "\tXOR DX, DX\n";
				assemblyCodes += "\tDIV BX\n";
				assemblyCodes += "\tMOV "+temp+" , AX\n";
			}

			else{
				// "%" operation clear dx, perform 'div bx' and mov dx to temp
				assemblyCodes += "\tXOR DX, DX\n";
				assemblyCodes += "\tDIV BX\n";
				assemblyCodes += "\tMOV "+temp+" , DX\n";

			}

			$$->setname(temp);
			$$->asmName=temp;
			$$->set_code(assemblyCodes);

			//------------------------------------------------------------------------

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
			//set variable_type
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

      ////fprintf(logfile,"At line no: %d unary_expression : ADDOP unary_expression\n",line);
		//fprintf(logfile,"%s%s\n\n",$1->get_name().c_str(),$2->get_name().c_str());

			$$=$2;

      $$->setVariableType($2->getVariableType());
      $$->setIdentity($2->getIdentity());

      //-------------------------assembly generation---------------------------------------

			string temp=newTemp();

			//codes like "+const" or "+var" or "-const" or "-var"
			//need actions only for negs
			if($1->get_name()=="-"){
				assemblyCodes=$$->get_code();
				assemblyCodes+=("\n\tMOV AX, "+$2->asmName+"\n");
				assemblyCodes+=("\tNEG AX\n");
				assemblyCodes+=("\tMOV "+temp+", AX\n");
			}

			else{
				assemblyCodes=$$->get_code();
				assemblyCodes+=("\n\tMOV AX, "+$2->asmName+"\n");
				assemblyCodes+=("\tMOV "+temp+", AX\n");
			}

			$$->set_code(assemblyCodes);
			$$->setname(temp);
			$$->asmName=temp;
      //------------------------------------------------------------------------------------

    }
		 | NOT unary_expression
    {

      $$=$2;

			//codes like !const or !var_nam
      //------------------------------------assemnbly generation-----------------------------
			string temp=newTemp();

			assemblyCodes=$$->get_code();
			assemblyCodes+=("\n\tMOV AX, "+$2->asmName+"\n");
			assemblyCodes+=("\tNOT AX\n");
			assemblyCodes+=("\tMOV "+temp+", AX\n");

			$$->set_code(assemblyCodes);
			$$->setname(temp);
			$$->asmName=temp;
      //--------------------------------------------------------------------------------------

      $$->setVariableType($2->getVariableType());
      $$->setIdentity($2->getIdentity());

    }
		 | factor
     {
       $$ = $1;
     }

		 ;

factor	: variable
    {
      $$ = $1;

      //for code generation purpose we concatenate the current id with the variable name
      if($1->getIdentity()=="array"){
        int idx = get_index($$->get_name());
        if(idx==0)$$->asmName =modified_name($$->get_name()) +table.get_current_id()[0];
        else $$->asmName =modified_name($$->get_name()) +table.get_current_id()[0]+"+"+to_str(idx)+"*2";
      }
      else{
        $$->asmName =modified_name($$->get_name()) +table.get_current_id()[0];
      }

			//-------------------------------------------------------------------
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
      $$ = new SymbolInfo("func_call","factor");
      $$->set_is_func(true);

      //semantic
      //chk if id is in func_list
      if(!is_in_func_list($1->get_name()) and $1->get_name()!="print_ln"){
        error_cnt++;
        fprintf(errorfile , "Error at line: %d Undeclared Function %s\n\n",line,$1->get_name().c_str());
      }
      else{

        assemblyCodes=$$->get_code();

        function_ f = get_func($1->get_name());

        //chk args consistency
        bool matched = true;
        bool already_error_in_arg = false;
        if(f.params.size() != $3->arg_list.size())matched = false;
        else{
          for(int i=0;i<f.params.size();i++){
            if($3->arg_list[i].already_error_in_arg){
              already_error_in_arg = true;
              //break;
            }
            //cout<<f.params[i].first<<" "<<$3->arg_list[i].name<<" "<<$3->arg_list[i].type<<endl;
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
              assemblyCodes+="\n\tMOV AX, " + $3->arg_list[i].name + table.get_current_id()[0]+"\n";
							assemblyCodes+="\tMOV "+f.params[i].second +table.get_current_id()[0]+", AX\n";
            }
          }
        }
        //cout<<$1->get_name()<<" "<<already_error_in_arg<<endl;
        if(!matched ){
          //
          error_cnt++;
          fprintf(errorfile , "Error at line: %d Total number of arguments mismatch in function %s\n\n",line,$1->get_name().c_str());
          fprintf(logfile , "Error at line: %d Total number of arguments mismatch in function %s\n\n",line,$1->get_name().c_str());
        }
        else{
          assemblyCodes+="\tCALL "+$1->get_name()+"\n";
					$$->set_code(assemblyCodes);
        }
      }

    }
	| LPAREN expression RPAREN
    {
      $$ =$2;
      $$->asmName=$$->get_name();

      $$->setVariableType($2->getVariableType());

    }
	| CONST_INT
    {
      $$=$1;
			$$->asmName=$$->get_name();
			$$->setVariableType("int");

    }
	| CONST_FLOAT
    {
      $$=$1;
      $$->asmName=$$->get_name();
			$$->setVariableType("float");

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
         //-------------------assembly generation----------------------------------
        assemblyCodes=$$->get_code();
				string var_name=modified_name($1->get_name())+table.get_current_id()[0];

				$$->setname(var_name);

				//array
				if($1->getIdentity()=="array"){
					//idx+1 th element will be accessed using array_name+idx*2
          int idx = get_index($1->get_name());
					if(idx==0)assemblyCodes+=("\tMOV AX, "+var_name + "\n");
          else assemblyCodes+=("\tMOV AX, "+var_name+"+"+to_str(idx)+"*2\n");

          assemblyCodes+=("\tINC AX\n");

					if(idx==0)assemblyCodes+=("\tMOV "+var_name + ", AX\n");
          else assemblyCodes+=("\tMOV "+var_name+"+"+to_str(idx)+"*2, AX\n");
				}

				else{
					assemblyCodes+=("\tMOV AX, "+var_name+"\n");
					assemblyCodes+=("\tINC AX\n");
					assemblyCodes+=("\tMOV "+var_name+", AX\n");
				}

				$$->set_code(assemblyCodes);
         //------------------------------------------------------------------------
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
         //-------------------------------assembly generation--------------------------
        assemblyCodes=$$->get_code();
				string var_name=modified_name($1->get_name())+table.get_current_id()[0];
				string temp_str=newTemp();

				$$->setname(var_name);

				//array
				if($1->getIdentity()=="array"){
					//idx+1 th element will be accessed using array_name+idx*2

					assemblyCodes+=("\tMOV AX, "+var_name+"+"+to_str($1->idx)+"*2\n");
					assemblyCodes+=("\tMOV "+temp_str+", AX\n");
					assemblyCodes+=("\tDEC AX\n");
					assemblyCodes+=("\tMOV "+var_name+"+"+to_str($1->idx)+"*2, AX\n");
				}

				else{
					assemblyCodes+=("\tMOV AX, "+var_name+"\n");
					assemblyCodes+=("\tMOV "+temp_str+", AX\n");
					assemblyCodes+=("\tDEC AX\n");
					assemblyCodes+=("\tMOV "+var_name+", AX\n");
				}

				$$->set_code(assemblyCodes);
				$$->setname(temp_str);
        //----------------------------------------------------------------------------
        $$->setVariableType($1->getVariableType());
        $$->setIdentity($1->getIdentity());
      }
    }
	;

argument_list : arguments
        {
    			$$=$1;
        }
			  |
        {
          $$ = new SymbolInfo("" , "argument_list");
        }
			  ;

arguments : arguments COMMA logic_expression
        {
          $$ = new SymbolInfo($1->get_name()+" , "+$3->get_name() , "arguments");


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
          ////fprintf(logfile,"At line no: %d arguments : logic_expression\n\n",line);
    			//fprintf(logfile,"%s\n\n",$1->get_name().c_str());
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
            //cout<<$1->getVariableType()<<endl;
            $$->push_in_arg($1->get_name() , $1->getVariableType() , 0);
          }
        }

	      ;


%%
int main(int argc,char *argv[])
{

	if((fp=fopen(argv[1],"r"))==NULL)
	{
		printf("Cannot Open Input File.\n");
		exit(1);
	}

	yyin=fp;
  asmCode=fopen(argv[2],"w");
	fclose(asmCode);

	optimized_asmCode=fopen(argv[3],"w");
	fclose(optimized_asmCode);

  asmCode=fopen(argv[2],"a");
	//optimized_asmCode=fopen(argv[4],"a");

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
