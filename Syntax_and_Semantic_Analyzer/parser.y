%{
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
#include<vector>
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

SymbolTable table(7);

extern int line;
extern int error_cnt;
vector<string>code_snippet;

string variable_type;
string codes;
struct var{
    string var_name;
    int var_size;  // it is set to -1 for variables
} temp_var;
vector<var> var_list;  // for identifier(variable, array) insertion into symbolTable

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

%type<symbol>compound_statement type_specifier parameter_list declaration_list var_declaration unit func_declaration statement statements variable expression factor arguments argument_list expression_statement unary_expression simple_expression logic_expression rel_expression term func_definition


%%

start : program
	{
		//write your code in this block in all the similar blocks below
	}
	;

program : program unit 
	{
		fprintf(logfile,"At line no: %d  program : program unit\n",line);
			
			code_snippet.pb($2->get_name());
			
			for(int i=0;i<code_snippet.size();i++){
				fprintf(logfile,"%s\n",code_snippet[i].c_str());
			}
			fprintf(logfile,"\n");
	}
	| unit
	{
		fprintf(logfile,"At line no: %d: program : unit\n\n",line);
		fprintf(logfile,"%s\n\n",$1->get_name().c_str());

		code_snippet.pb($1->get_name());
	}
	;
	
unit : var_declaration
	{
		fprintf(logfile,"At line no: %d: unit : var_declation\n\n",line);
		fprintf(logfile,"%s\n\n",$1->get_name().c_str());

		SymbolInfo *x=new SymbolInfo($1->get_name(),"unit");
		$$=x;	
	}
     | func_declaration
	{

	} 
     | func_definition
	{

	}
     ;
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
	{

	}
	| type_specifier ID LPAREN RPAREN SEMICOLON
	{

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
		| parameter_list COMMA type_specifier
 		| type_specifier ID
		| type_specifier
 		;

 		
compound_statement : LCURL statements RCURL
 		    | LCURL RCURL
 		    ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON
		{
			fprintf(logfile,"At line no: %d var_declaration : type_specifier declaration_list SEMICOLON\n\n",line);
			fprintf(logfile,"%s ",$1->get_type().c_str());
			
			codes="";
			codes += $1->get_type()+" ";

			//print the declaration_list i.e. the variables
			for(int i=0;i<$2->edge.size();i++){
				fprintf(logfile,"%s",$2->edge[i]->get_name().c_str());
				codes += $2->edge[i]->get_name();
				
				if($2->edge[i]->sz>0)
					fprintf(logfile,"[%d]",$2->edge[i]->sz), codes+="["+to_str($2->edge[i]->sz)+"]";
				
				if(i<$2->edge.size()-1)
					fprintf(logfile,","), codes+=",";
				cout<<$2->edge[i]->get_name()<<endl;
			}

			fprintf(logfile,";\n\n");
			codes+=";";

			SymbolInfo *newSymbol=new SymbolInfo(codes,"var_declaration");
			$$=newSymbol;

			$2->edge.clear();
		}
 		 ;
 		 
type_specifier	: INT
		{
			fprintf(logfile,"At line no: %d: type_specifier : INT \n",line);
			variable_type = "int";

			SymbolInfo *newSymbol = new SymbolInfo("int");
			$$ = newSymbol;
			fprintf(logfile,"%s\n\n",$$->get_type().c_str());
		}
 		| FLOAT
 		| VOID
 		;
 		
declaration_list : declaration_list COMMA ID
		{
			fprintf(logfile,"At line no: %d: declaration_list : declaration_list COMMA ID\n",line);
			
			$3->setIdentity("var");
			$3->setVariableType(variable_type);
			
			$$->edge.push_back($3);

			//print the declaration_list
			for(int i=0;i<$$->edge.size();i++)
			{
				fprintf(logfile,"%s",$$->edge[i]->get_name().c_str());

				if($$->edge[i]->sz>0)
					fprintf(logfile,"[%d]",$$->edge[i]->sz);
				
				if(i<$$->edge.size()-1)
					fprintf(logfile,",");

				else
					fprintf(logfile,"\n\n");
			}
			/*
			//---------------------------------------------------------------------------
			//semantics and insertion in the table
 			if(variable_type=="void") {
 				fprintf(error,"semantic error found at line %d: variable cannot be of type void\n\n",line);
 				error_cnt++;
 			}

 			else
 			{
 				//insert in SymbolTable directly if not declared before
 				if(!table.Insert($3->get_name(),"ID",logfile)) {
 					fprintf(error,"semantic error found at line %d: variable \'%s\' declared before\n\n",line,$1->get_name().c_str());
 					error_cnt++;
 				}

				else {
 					SymbolInfo *temp=table.Lookup($3->get_name());
 					temp->setVariableType(variable_type);
 					temp->allocateMemory(variable_type,1);
 					temp->setIdentity("var");
 				}
 			}
 			//---------------------------------------------------------------------------
			*/
		}
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
 		  | ID
		{
			fprintf(logfile,"At line no %d: declaration_list : ID\n\n",line);
 			fprintf(logfile,"%s\n\n",$1->get_name().c_str());

 			SymbolInfo *newSymbol = new SymbolInfo("declaration_list");
 			$$ = newSymbol;

 			$1->setVariableType(variable_type);$1->setIdentity("var");

 			$$->setIdentity("declaration_list");
 			$$->edge.push_back($1);

			/*
 			//---------------------------------------------------------------------------
			//semantics and insertion in the table
 			if(variable_type=="void") {
 				fprintf(error,"semantic error found at line %d: variable cannot be of type void\n\n",line);
 				semanticErr++;
 			}

 			else {
 				//insert in SymbolTable directly if not declared before
 				if(!table.Insert($1->get_name(),"ID",logfile)) {
 					fprintf(error,"semantic error found at line %d: variable %s declared before\n\n",line,$1->get_name().c_str());
 					semanticErr++;
 				}

 				else {
 					SymbolInfo *temp=table.lookUp($1->get_name());
 					temp->setVariableType(variable_type);
 					temp->allocateMemory(variable_type,1);
 					temp->setIdentity("var");
 				}
 			}
 			//---------------------------------------------------------------------------
		
			*/
		}   
 		  | ID LTHIRD CONST_INT RTHIRD
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
	fprintf(errorfile,"Total Errors : %d \n\n",error_cnt);
	
	fclose(fp);
	fclose(logfile);
	fclose(errorfile);

	return 0;
}

