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

int yyparse(void);
int yylex(void);

extern FILE *yyin;
FILE *fp;
FILE *errorfile = fopen("error.txt","w");
FILE *logfile = fopen("log.txt" , "w");

SymbolTable table(30);

extern int line;
extern int error_cnt;


string variable_type;
string current_return_type;
struct var{
    string var_name;
    int var_size;  // it is set to -1 for variables
} temp_var;
vector<var> var_list;  // for identifier(variable, array) insertion into symbolTable


//vector<param> param_list;  // parameter list for function declaration, definition

vector<string> arg_list;  // argument list for function call

string to_str(int n)
{
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

bool is_ara_idx_valid(string nm , int sz){
  //nm = a[4] , sz = 5 ; so returns true
  //float idx not handled yet;
  string idx="";
  for(int i=0;i<nm.size();i++){
    if(nm[i]=='['){
      i++;
      while(nm[i]!=']'){
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
	fprintf(stderr,"Line no %d : %s\n",line,s);
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

%type<symbol>compound_statement type_specifier parameter_list declaration_list var_declaration unit func_declaration statement statements variable expression factor arguments argument_list expression_statement unary_expression simple_expression logic_expression rel_expression term func_definition program
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

start : program
	{
		//write your code in this block in all the similar blocks below
	}
	;

program : program unit
	{
		fprintf(logfile,"At line no: %d  program : program unit\n\n",line);
		SymbolInfo *x = new SymbolInfo((string)$1->get_name()+(string)$2->get_name(), "NON_TERMINAL");
		$$ = x;
		fprintf(logfile , "%s%s\n\n" , $1->get_name().c_str() , $2->get_name().c_str());
	}
	| unit
	{
		fprintf(logfile,"At line no: %d: program : unit\n\n",line);
		fprintf(logfile,"%s\n",$1->get_name().c_str());
		$$ = new SymbolInfo($1->get_name()+"\n", "NON_TERMINAL");

	}
	;

unit : var_declaration
	{
		fprintf(logfile,"At line no: %d: unit : var_declation\n\n",line);
		fprintf(logfile,"%s\n\n",$1->get_name().c_str());
		$$ = new SymbolInfo($1->get_name(), "NON_TERMINAL");
	}
     | func_declaration
	{
		fprintf(logfile,"At line no: %d: unit : func_declaration\n\n",line);
		fprintf(logfile,"%s\n\n",$1->get_name().c_str());
		$$ = new SymbolInfo($1->get_name(), "NON_TERMINAL");
	}
     | func_definition
	{
    fprintf(logfile,"At line no: %d: unit : func_definition\n\n",line);
		fprintf(logfile,"%s\n",$1->get_name().c_str());
		$$ = new SymbolInfo($1->get_name(), "NON_TERMINAL");
	}
     ;

func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
	{
    $$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"("+$4->get_name()+");", "NON_TERMINAL");
    fprintf(logfile , "At line no %d: func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON\n\n",line);
    fprintf(logfile , "%s %s(%s);\n\n" , $1->get_name().c_str() , $2->get_name().c_str(),$4->get_name().c_str());

    /* checking whether already declared or not */
    SymbolInfo* temp = table.Lookup_in_current($2->get_name());
    if(temp != NULL) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , $2->get_name().c_str());
		}
    else{
      table.Insert($2->get_name() , "ID" , logfile);
      SymbolInfo *fd = table.Lookup_in_current($2->get_name());
      fd->set_is_declared_func(true);
    }

	}
	| type_specifier ID LPAREN RPAREN SEMICOLON
	{
		$$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"();", "NON_TERMINAL");
		fprintf(logfile , "At line no %d: func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON\n\n",line);
		fprintf(logfile , "%s %s();\n\n" , $1->get_name().c_str() , $2->get_name().c_str());


		/* checking whether already declared or not */
		SymbolInfo* temp = table.Lookup_in_current($2->get_name());
		if(temp != NULL) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , $2->get_name().c_str());
		}
    else{
      table.Insert($2->get_name() , "ID" , logfile);
      SymbolInfo *fd = table.Lookup_in_current($2->get_name());
      fd->set_is_declared_func(true);
    }

	}
		;

func_definition : type_specifier ID LPAREN parameter_list RPAREN
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
		}


    table.Enter_Scope(logfile);
    for(int i=0;i<$4->param.size();i++){
        string nm = $4->param[i].param_name;
        string tp = $4->param[i].param_type;
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
      $$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"("+$4->get_name()+")"+$7->get_name()+"\n\n", "NON_TERMINAL");
      fprintf(logfile , "At line no %d: func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement\n\n" , line);
      fprintf(logfile , "%s %s(%s)%s\n\n" , $1->get_name().c_str(),$2->get_name().c_str(),$4->get_name().c_str(),$7->get_name().c_str());

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
  		}

      table.Enter_Scope(logfile);

    }
    compound_statement {table.printall(logfile);table.Exit_Scope(logfile);var_list.clear();}
 	{
      $$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"()"+$6->get_name()+"\n\n", "NON_TERMINAL");
      fprintf(logfile , "At line no %d: func_definition : type_specifier ID LPAREN RPAREN compound_statement\n\n" , line);
      fprintf(logfile , "%s %s()%s\n\n" , $1->get_name().c_str(),$2->get_name().c_str(),$6->get_name().c_str());


	}
	 	;


parameter_list  : parameter_list COMMA type_specifier ID
		{
			$$ = new SymbolInfo($1->get_name()+","+$3->get_name()+" "+$4->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : parameter_list COMMA type_specifier ID\n\n",line);
			fprintf(logfile , "%s , %s %s\n\n" , $1->get_name().c_str(),$3->get_name().c_str(),$4->get_name().c_str());

			/* adding parameter to parameter list */
      $$->param = $1->param;
			$$->push_in_param($4->get_name() , "ID");

		}
		| parameter_list COMMA type_specifier
		{
			$$ = new SymbolInfo($1->get_name()+","+$3->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : parameter_list COMMA type_specifier\n\n",line);
			fprintf(logfile , "%s , %s\n\n" , $1->get_name().c_str(),$3->get_name().c_str());

			/* adding parameter to parameter list */
      $$->param = $1->param;
			$$->push_in_param("", $3->get_name());
		}
 		| type_specifier ID
		{
			$$ = new SymbolInfo($1->get_name()+" "+$2->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : type_specifier ID\n\n",line);
			fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str(),$2->get_name().c_str());

			/* adding parameter to parameter list */

			$$->push_in_param($2->get_name() ,"ID");
		}
		| type_specifier
		{
			$$ = new SymbolInfo($1->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : type_specifier\n\n",line);
			fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str());

			/* adding parameter to parameter list */

			$$->push_in_param( "" , $1->get_name());
		}
 		;


compound_statement : LCURL statements RCURL
  {
      $$ = new SymbolInfo("{\n"+$2->get_name()+"\n}"+"\n\n", "NON_TERMINAL");
      fprintf(logfile,"At line no %d: compound_statement : LCURL statements RCURL\n\n",line);
			fprintf(logfile,"{\n%s\n}\n\n",$2->get_name().c_str());

  }
 		    | LCURL RCURL
  {
    $$ = new SymbolInfo("{\n}", "NON_TERMINAL");
    fprintf(logfile,"At line no %d: compound_statement : LCURL RCURL\n\n",line);
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

			//var_list.clear();
		}
 		;

type_specifier	: INT
		{
			fprintf(logfile,"At line no: %d: type_specifier : INT \n\n",line);
			variable_type = "int";

			SymbolInfo *x = new SymbolInfo("int" , "int");
			$$ = x;
			fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
 		| FLOAT
		{
			fprintf(logfile,"At line no: %d: type_specifier : FLOAT \n",line);
			variable_type = "float";

			SymbolInfo *x = new SymbolInfo("float" , "float");
			$$ = x;
			fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
 		| VOID
 		{
			 fprintf(logfile,"At line no: %d: type_specifier : VOID \n",line);
			variable_type = "void";

			SymbolInfo *x = new SymbolInfo("void" , "void");
			$$ = x;
			fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
		 ;

declaration_list : declaration_list COMMA ID
		{
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
			fprintf(logfile,"At line no %d: declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD\n\n",line);
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
			fprintf(logfile,"At line no %d: declaration_list : ID\n\n",line);
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
			fprintf(logfile , "At line no %d:  declaration_list: ID LTHIRD CONST_INT RTHIRD\n\n",line);
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
       fprintf(logfile , "At line no %d: statements : statement\n\n" , line);
       fprintf(logfile , "%s\n\n" , $1->get_name().c_str());

    }
	   | statements statement
    {
      $$ = new SymbolInfo($1->get_name()+$2->get_name() , "NON_TERMINAL");
      fprintf(logfile , "At line no %d: statements : statements statement\n\n" , line);
      fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str(),$2->get_name().c_str());

    }
	   ;

statement : var_declaration
    {
      fprintf(logfile,"At line no %d: statement : var_declaration\n\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
      $1->setname($1->get_name()+"\n");
  		$$=$1;
    }
	  | expression_statement
    {
      fprintf(logfile,"At line no %d: statement : expression_statement\n\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
      $1->setname($1->get_name()+"\n");
			$$=$1;
    }
	  | compound_statement
    {
      fprintf(logfile,"At line no %d: statement : compound_statement\n\n",line);
      fprintf(logfile,"%s\n\n",$1->get_name().c_str());

      $$=$1;
    }
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement
    {
      string str="for("+$3->get_name()+$4->get_name()+$5->get_name()+")"+$7->get_name();
      $$ = new SymbolInfo(str , "NON_TERMINAL");
      fprintf(logfile,"line no. %d: statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement\n",line);
			fprintf(logfile,"%s\n\n",str.c_str());
    }
	  | IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE
    {
      string str = "if("+$3->get_name()+")"+$5->get_name();
      $$ = new SymbolInfo(str , "statement");
      fprintf(logfile,"At line no %d: statement : IF LPAREN expression RPAREN statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());

    }
	  | IF LPAREN expression RPAREN statement ELSE statement
    {
      string str = "if("+$3->get_name()+")"+$5->get_name()+"else"+$7->get_name();
      $$ = new SymbolInfo(str , "statement");
      fprintf(logfile,"At line no %d: statement : IF LPAREN expression RPAREN statement ELSE statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());

    }
	  | WHILE LPAREN expression RPAREN statement
    {
      string str = "while("+$3->get_name()+")"+$5->get_name();
      $$ = new SymbolInfo(str , "statement");
      fprintf(logfile,"At line no %d: statement : WHILE LPAREN expression RPAREN statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());
    }
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
    {
      $$ = new SymbolInfo("printf("+$3->get_name()+");" , "statement");
      fprintf(logfile,"At line no %d: statement : PRINTLN LPAREN ID RPAREN SEMICOLON\n\n",line);
      fprintf(logfile , "printf(%s);" , $3->get_name().c_str());
    }
	  | RETURN expression SEMICOLON
    {
      $$ = new SymbolInfo("return "+$2->get_name()+";" , "statement");
      fprintf(logfile,"At line no %d: statement : RETURN expression SEMICOLON\n\n",line);
      fprintf(logfile , "return %s;\n\n" , $2->get_name().c_str());

      /* ----do this------
      isReturning=true;
			isReturningType=$2->getVariableType();
      */
    }
	  ;

expression_statement 	: SEMICOLON
    {
      $$ = new SymbolInfo(";" , "expression_statement");
      fprintf(logfile,"At line no %d: expression_statement : SEMICOLON\n",line);
			fprintf(logfile,";\n\n");
    }
			| expression SEMICOLON
    {
      $$ = new SymbolInfo($1->get_name()+";" , "expression_statement");
      fprintf(logfile,"At line no %d: expression_statement : expression SEMICOLON\n\n",line);
      fprintf(logfile , "%s;\n\n" , $1->get_name().c_str());

    }
			;

variable :

    ID
    {
      //$$ = new SymbolInfo($1->get_name() , "variable");
      fprintf(logfile,"At line no. %d: variable : ID\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
      $$ = $1;
      //Semantic : chk if variable is declared before

      $$->setIdentity("var");
      $$->push_in_var($1->get_name(),"",0);
      SymbolInfo *x=table.Lookup($1->get_name());
      if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line: %d variable '%s' not declared in this scope\n\n",line,$1->get_name().c_str());
      }
      else{
        $$->setVariableType(x->getVariableType());
      }

    }

	 | ID LTHIRD expression RTHIRD
   {
     fprintf(logfile,"At line no %d: variable : ID LTHIRD expression RTHIRD\n",line);
		 fprintf(logfile,"%s[%s]\n\n",$1->get_name().c_str(),$3->get_name().c_str());
     $$ = new SymbolInfo($1->get_name()+"["+$3->get_name()+"]" , "variable");

     $$->setIdentity("array");
     //array index must be integer

   }

	 ;

 expression : logic_expression
    {
      $$ = $1;
      fprintf(logfile,"At line no %d: expression : logic_expression\n\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
    }
	   | variable ASSIGNOP logic_expression
    {
      $$ = new SymbolInfo($1->get_name()+"="+$3->get_name() , "expression");
      fprintf(logfile,"At line no %d: expression : variable ASSIGNOP logic_expression\n\n",line);
			fprintf(logfile,"%s = %s\n\n",$1->get_name().c_str(),$3->get_name().c_str());

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
              fprintf(errorfile,"Error at line: %d Wrong array index\n\n",line);
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

        /*#semantic: expression cannot have void return type functions called
			if(returnType_curr=="void"){
				semanticErr++;
				fprintf(error,"semantic error found in line %d: void type function can't be part of expression\n\n",line);
				returnType_curr="none";
			}
      */
			}

			else{
				error_cnt++;
				fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,varname.c_str());
			}
    }
	   ;

logic_expression : rel_expression
    {
      $$ = $1;
      fprintf(logfile,"At line no %d: logic_expression : rel_expression\n\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());

    }
		 | rel_expression LOGICOP rel_expression
    {
      $$ = new SymbolInfo($1->get_name()+$2->get_name()+$3->get_name() , "logic_expression");
      fprintf(logfile,"At line no %d: logic_expression : rel_expression LOGICOP rel_expression\n\n",line);
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
    }
		 ;

rel_expression	: simple_expression
   {
     $$ = $1;
     fprintf(logfile,"At line no %d: rel_expression	: simple_expression\n\n",line);
     fprintf(logfile,"%s\n\n",$1->get_name().c_str());

   }
		| simple_expression RELOP simple_expression
   {
     $$ = new SymbolInfo($1->get_name()+$2->get_name()+$3->get_name() , "rel_expression");
     fprintf(logfile,"At line no %d: rel_expression : simple_expression RELOP simple_expression\n\n",line);
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
   }
		;

simple_expression : term
  {
    $$ = $1;
    fprintf(logfile,"At line no %d: simple_expression : term\n\n",line);
    fprintf(logfile,"%s\n\n",$1->get_name().c_str());

  }
		  | simple_expression ADDOP term
  {
    $$ = new SymbolInfo($1->get_name()+$2->get_name()+$3->get_name() , "simple_expression");
    fprintf(logfile,"At line no %d: simple_expression : simple_expression ADDOP term\n\n",line);
    fprintf(logfile,"%s%s%s\n\n",$1->get_name().c_str(),$2->get_name().c_str(),$3->get_name().c_str());

    if($1->getVariableType()=="float" || $3->getVariableType()=="float")
				$$->setVariableType("float");
			else
				$$->setVariableType("int");

  }
		  ;

term :	unary_expression
    {
      $$ = $1;
      fprintf(logfile,"At line no %d: term :	unary_expression\n\n",line);
      fprintf(logfile,"%s\n\n",$1->get_name().c_str());
    }
     |  term MULOP unary_expression
    {
      $$ = new SymbolInfo($1->get_name()+$2->get_name()+$3->get_name() , "term");
      fprintf(logfile,"At line no %d: term : term MULOP unary_expression\n\n",line);
      fprintf(logfile,"%s%s%s\n\n",$1->get_name().c_str(),$2->get_name().c_str(),$3->get_name().c_str());

      //features of mod operation
      if($2->get_name()=="%" && ($1->getVariableType()!="int" || $3->getVariableType()!="int")){
				error_cnt++;
				fprintf(errorfile,"Error at line %d: type mismatch,(mod operation)\n\n",line);
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
      fprintf(logfile,"At line no %d: unary_expression : ADDOP unary_expression\n",line);
			fprintf(logfile,"%s%s\n\n",$1->get_name().c_str(),$2->get_name().c_str());

			$$ = new SymbolInfo($1->get_name()+$2->get_name(),"unary_expression");

      $$->setVariableType($2->getVariableType());
      $$->setIdentity($2->getIdentity());

    }
		 | NOT unary_expression
    {
      fprintf(logfile,"At line no %d: unary_expression : NOT unary_expression\n",line);
      fprintf(logfile,"!%s\n\n",$2->get_name().c_str());

      $$ = new SymbolInfo("!"+$2->get_name(),"unary_expression");

      $$->setVariableType($2->getVariableType());
      $$->setIdentity($2->getIdentity());

    }
		 | factor
     {
       $$ = $1;
       fprintf(logfile,"At line no %d: unary_expression :	factor\n\n",line);
       fprintf(logfile,"%s\n\n",$1->get_name().c_str());
     }

		 ;

factor	: variable
    {
      fprintf(logfile,"At line no %d: factor : variable\n\n",line);
      fprintf(logfile,"%s\n\n",$1->get_name().c_str());
      $$ = $1;
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
                fprintf(errorfile,"Error at line: %d Type Mismatch\n\n",line);
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
      $$ = new SymbolInfo($1->get_name()+"("+$3->get_name()+")" , "factor");
      fprintf(logfile,"At line no %d: factor : ID LPAREN argument_list RPAREN\n\n",line);
      fprintf(logfile,"%s(%s)\n\n",$1->get_name().c_str(),$3->get_name().c_str());

      /*semantic hugeee

      */

    }
	| LPAREN expression RPAREN
    {
      $$ = new SymbolInfo("("+$2->get_name()+")" , "factor");
      fprintf(logfile,"At line no %d: factor : LPAREN expression RPAREN\n\n",line);
      fprintf(logfile,"(%s)\n\n",$2->get_name().c_str());

      $$->setVariableType($2->getVariableType());

    }
	| CONST_INT
    {
      fprintf(logfile,"At line no %d: factor : CONST_INT\n\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
			$$=$1;
			$$->setVariableType("int");

    }
	| CONST_FLOAT
    {
      fprintf(logfile,"At line no %d: factor : CONST_FLOAT\n\n",line);
			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
			$$=$1;
			$$->setVariableType("float");

    }
	| variable INCOP
    {
      fprintf(logfile,"At line no %d: factor	: variable INCOP\n\n",line);
			fprintf(logfile,"%s++\n\n",$1->get_name().c_str());
      $$ = new SymbolInfo($1->get_name()+"++","factor");

      SymbolInfo *x=table.Lookup($1->get_name());
			if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,$1->get_name().c_str());
      }
      else{

         $$->setVariableType($1->getVariableType());
         $$->setIdentity($1->getIdentity());
      }

    }
	| variable DECOP
    {
      fprintf(logfile,"At line no %d: factor	: variable DECOP\n\n",line);
			fprintf(logfile,"%s--\n\n",$1->get_name().c_str());
      $$ = new SymbolInfo($1->get_name()+"--","factor");

      SymbolInfo *x=table.Lookup($1->get_name());
			if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,$1->get_name().c_str());
      }
      else{

         $$->setVariableType($1->getVariableType());
         $$->setIdentity($1->getIdentity());
      }
    }
	;

argument_list : arguments
      {
        fprintf(logfile,"At line no %d: argument_list : arguments\n\n",line);
  			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
  			$$=$1;
      }
			  |
        {
          $$ = new SymbolInfo("","argument_list");
        }
			  ;

arguments : arguments COMMA logic_expression
        {
          $$ = new SymbolInfo($1->get_name()+" , "+$3->get_name() , "term");
          fprintf(logfile,"At line no %d: arguments : arguments COMMA logic_expression\n\n",line);
          fprintf(logfile,"%s , %s\n\n",$1->get_name().c_str(),$3->get_name().c_str());

          /*
          arg_list.push_back($3);
          */
        }
	      | logic_expression
        {
          fprintf(logfile,"At line no %d: arguments : logic_expression\n\n",line);
    			fprintf(logfile,"%s\n\n",$1->get_name().c_str());
    			$$=$1;

          /*
          arg_list.push_back($$);
          */
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
