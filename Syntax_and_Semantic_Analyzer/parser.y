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
//vector<string>code_snippet;

string variable_type;
//string codes;
struct var{
    string var_name;
    int var_size;  // it is set to -1 for variables
} temp_var;
vector<var> var_list;  // for identifier(variable, array) insertion into symbolTable

struct param {
    string type;
    string name;  //set to empty string "" for function declaration
} my_param;

vector<param> param_list;  // parameter list for function declaration, definition

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
		fprintf(logfile,"%s\n",$1->get_name().c_str());

		$$ = new SymbolInfo($1->get_name(), "NON_TERMINAL");
	}
     | func_declaration
	{
		fprintf(logfile,"At line no: %d: unit : func_declaration\n\n",line);
		fprintf(logfile,"%s\n",$1->get_name().c_str());

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
    SymbolInfo* temp = table.Lookup($2->get_name());
    if(temp != NULL) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , $2->get_name().c_str());
		}
		else{
			table.Insert($2->get_name() , "ID" , logfile);
		}

	}
	| type_specifier ID LPAREN RPAREN SEMICOLON
	{
		$$ = new SymbolInfo($1->get_name()+" "+$2->get_name()+"();", "NON_TERMINAL");
		fprintf(logfile , "At line no %d: func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON\n\n",line);
		fprintf(logfile , "%s %s();\n\n" , $1->get_name().c_str() , $2->get_name().c_str());


		/* checking whether already declared or not */
		SymbolInfo* temp = table.Lookup($2->get_name());
		if(temp != NULL) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , $2->get_name().c_str());
		}
		else{
			table.Insert($2->get_name() , "ID" , logfile);
		}

	}
		;

func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement
	{

	}
		| type_specifier ID LPAREN RPAREN compound_statement
 	{

	}
	 	;


parameter_list  : parameter_list COMMA type_specifier ID
		{
			$$ = new SymbolInfo($1->get_name()+","+$3->get_name()+" "+$4->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : parameter_list COMMA type_specifier ID\n\n",line);
			fprintf(logfile , "%s , %s %s\n\n" , $1->get_name().c_str(),$3->get_name().c_str(),$4->get_name().c_str());

			/* adding parameter to parameter list */
            my_param.type = $3->get_name();
            my_param.name = $4->get_name();
            param_list.pb(my_param);

			$$->push_in_param($4->get_name() , $3->get_name());

		}
		| parameter_list COMMA type_specifier
		{
			$$ = new SymbolInfo($1->get_name()+","+$3->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : parameter_list COMMA type_specifier\n\n",line);
			fprintf(logfile , "%s , %s\n\n" , $1->get_name().c_str(),$3->get_name().c_str());

			/* adding parameter to parameter list */
            my_param.type = $3->get_name();
            my_param.name = "";
            param_list.pb(my_param);

			$$->push_in_param("", $3->get_name());
		}
 		| type_specifier ID
		{
			$$ = new SymbolInfo($1->get_name()+" "+$2->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : type_specifier ID\n\n",line);
			fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str(),$2->get_name().c_str());

			/* adding parameter to parameter list */
            my_param.type = $1->get_name();
            my_param.name = $2->get_name();
            param_list.pb(my_param);

			$$->push_in_param($2->get_name() , $1->get_name());
		}
		| type_specifier
		{
			$$ = new SymbolInfo($1->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : type_specifier\n\n",line);
			fprintf(logfile , "%s %s\n\n" , $1->get_name().c_str());

			/* adding parameter to parameter list */
            my_param.type = "";
            my_param.name = $1->get_name();
            param_list.pb(my_param);

			$$->push_in_param($1->get_name() , "");
		}
 		;


compound_statement : LCURL statements RCURL
 		    | LCURL RCURL
 		    ;

var_declaration : type_specifier declaration_list SEMICOLON
		{
			fprintf(logfile,"At line no: %d var_declaration : type_specifier declaration_list SEMICOLON\n\n",line);
			fprintf(logfile,"%s %s;\n\n",$1->get_name().c_str(),$2->get_name().c_str());

			$$ = new SymbolInfo((string)$1->get_name()+(string)" "+(string)$2->get_name()+(string)";"+(string)"\n"+(string)"\n", "NON_TERMINAL");



			/* inserting in symboltable */

			if($1->get_name()=="void"){
				error_cnt++;
				fprintf(errorfile,"Error at line %d: variable cannot be of type void\n\n",line);

			}
			else{
				/* checking whether already declared or not */
				for(int i=0;i<var_list.size();i++){
					SymbolInfo* temp = table.Lookup(var_list[i].var_name);
					if(temp != NULL) {
						error_cnt++;
						fprintf(errorfile , "Error at line %d: Multiple declaration of %s" , line , var_list[i].var_name.c_str());
					}
					else{
						table.Insert(var_list[i].var_name , "ID" , logfile);
					}
				}

			}
			var_list.clear();
		}
 		;

type_specifier	: INT
		{
			fprintf(logfile,"At line no: %d: type_specifier : INT \n\n",line);
			variable_type = "int";

			SymbolInfo *newSymbol = new SymbolInfo("int" , "NON_TERMINAL");
			$$ = newSymbol;
			fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
 		| FLOAT
		{
			fprintf(logfile,"At line no: %d: type_specifier : FLOAT \n",line);
			variable_type = "float";

			SymbolInfo *newSymbol = new SymbolInfo("float" , "NON_TERMINAL");
			$$ = newSymbol;
			fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
 		| VOID
 		{
			 fprintf(logfile,"At line no: %d: type_specifier : VOID \n",line);
			variable_type = "void";

			SymbolInfo *newSymbol = new SymbolInfo("void" , "NON_TERMINAL");
			$$ = newSymbol;
			fprintf(logfile,"%s\n\n",$$->get_name().c_str());
		}
		 ;

declaration_list : declaration_list COMMA ID
		{
			fprintf(logfile,"At line no: %d: declaration_list : declaration_list COMMA ID\n\n",line);
			$$ = new SymbolInfo((string)$1->get_name()+(string)","+(string)$3->get_name(), "NON_TERMINAL");
			fprintf(logfile , "%s,%s\n\n" , $1->get_name().c_str() , $3->get_name().c_str());

			/* keeping track of identifier(variable) */
            temp_var.var_name = (string)$3->get_name();
            temp_var.var_size = -1;
            var_list.push_back(temp_var);


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


		}
 		| ID LTHIRD CONST_INT RTHIRD
		{
			fprintf(logfile , "At line no %d:  declaration_list: ID LTHIRD CONST_INT RTHIRD\n\n",line);
			$$ = new SymbolInfo((string)$1->get_name()+(string)"["+(string)$3->get_name()+(string)"]", "NON_TERMINAL");
			fprintf(logfile , "%s[%s]\n\n",$1->get_name().c_str() , $3->get_name().c_str());

			temp_var.var_name = (string)$1->get_name();
            stringstream temp_str((string) $3->get_name());
            temp_str >> temp_var.var_size;
            var_list.pb(temp_var);

		}
 		  ;

statements : statement
	   | statements statement
	   ;

statement : var_declaration
	  | expression_statement
	  | compound_statement
	  | FOR LPAREN expression_statement expression_statement expression RPAREN statement
	  | IF LPAREN expression RPAREN statement
	  | IF LPAREN expression RPAREN statement ELSE statement
	  | WHILE LPAREN expression RPAREN statement
	  | PRINTLN LPAREN ID RPAREN SEMICOLON
	  | RETURN expression SEMICOLON
	  ;

expression_statement 	: SEMICOLON
			| expression SEMICOLON
			;

variable : ID
	 | ID LTHIRD expression RTHIRD
	 ;

 expression : logic_expression
	   | variable ASSIGNOP logic_expression
	   ;

logic_expression : rel_expression
		 | rel_expression LOGICOP rel_expression
		 ;

rel_expression	: simple_expression
		| simple_expression RELOP simple_expression
		;

simple_expression : term
		  | simple_expression ADDOP term
		  ;

term :	unary_expression
     |  term MULOP unary_expression
     ;

unary_expression : ADDOP unary_expression
		 | NOT unary_expression
		 | factor
		 ;

factor	: variable
	| ID LPAREN argument_list RPAREN
	| LPAREN expression RPAREN
	| CONST_INT
	| CONST_FLOAT
	| variable INCOP
	| variable DECOP
	;

argument_list : arguments
			  |
			  ;

arguments : arguments COMMA logic_expression
	      | logic_expression
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
