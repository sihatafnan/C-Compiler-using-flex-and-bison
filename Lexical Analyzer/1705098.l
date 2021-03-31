%option noyywrap

%{
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include "1705098_symboltable.h"
using namespace std;


int line=1;
int error_cnt=0;

FILE *logfile = fopen("1705098_log.txt","w");
FILE *tokenfile = fopen("1705098_token.txt","w");

SymbolTable st(7);

void newlines_in_comment(string str){
    for(int i=0;i<str.length();i++){
        if(str[i]=='\n')line++;
    }
}

%}
/*keywords*/
BREAK "break"
CASE "case"
CHAR "char"
CONTINUE "continue"
DEFAULT "default"
DO "do"
DOUBLE "double"
ELSE "else"
FLOAT "float"
FOR "for"
IF "if"
INT "int"
RETURN "return"
SWITCH "switch"
WHILE "while"
VOID "void"
/*operators*/
ADDOP [+-]
MULOP [*/%]
INCOP ("++"|"--")
RELOP (">"|">="|"<"|"<="|"=="|"!=")
ASSIGNOP ("=")
LOGICOP ("&&"|"||")
BITOP ("&"|"|"|"^"|"<<"|">>")
NOT ("!")
LPAREN ("(")
RPAREN (")")
LCURL ("{")
RCURL ("}")
LTHIRD ("[")
RTHIRD ("]")
COMMA (",")
SEMICOLON (";")

DIGIT [0-9]
DIGITS {DIGIT}+
LETTER [a-zA-Z]
NEWLINE [\n]
WHITESPACE [ \t\v]+

/* need to be modified*/
SINGLE_LINE_COMMENT "//"(.*)(\n)

MULTI_LINE_COMMENT "/*"([^*]|("*"[^/]))*"*/"|("//"(([\\]+([\n]))|[^\n]*)*)

/*FLOAT_LITERAL {DIGIT}*\.{DIGIT}+([E][-+]?{DIGIT}+)? | {DIGIT}+([E][-+]?{DIGIT}+)?*/

EXPONENTIAL ([Ee][-+]?{DIGITS})
FLOAT_VAR_ONE ({DIGITS}\.{DIGITS})({EXPONENTIAL})?
FLOAT_VAR_TWO (\.{DIGITS})({EXPONENTIAL})?
FLOAT_VAR_THREE ({DIGITS})({EXPONENTIAL})?
FLOAT_LITERAL ({FLOAT_VAR_ONE}|{FLOAT_VAR_TWO}|{FLOAT_VAR_THREE})

INT_LITERAL {DIGITS}

NORMAL_CHAR '([ -~]{-}['\\])'
SPECIAL_CHAR '(\\[nt\\afrbv0\"])'
CHAR_LITERAL {NORMAL_CHAR}|{SPECIAL_CHAR}

IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*


/* Rules Section*/
%%
{WHITESPACE}    {}

{NEWLINE}       {line++; }


{SINGLE_LINE_COMMENT} {
                newlines_in_comment(yytext);     
                fprintf(logfile,"Line no. %d: Token <SINGLE_LINE_COMMENT> Lexeme %s found\n\n",line,yytext);      
}

{MULTI_LINE_COMMENT} {
                newlines_in_comment(yytext); 
                fprintf(logfile,"ending Line no. %d: Token <MULTI_LINE_COMMENT> Lexeme %s found\n\n",line,yytext);          
}

{INT_LITERAL} {
	fprintf(tokenfile,"<CONST_INT,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <CONST_INT> Lexeme %s found\n\n",line,yytext);
	st.Insert(yytext , "CONST_INT");
    st.printcurrent();
}

{FLOAT_LITERAL} {
	fprintf(tokenfile,"<CONST_FLOAT,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <CONST_FLOAT> Lexeme %s found\n\n",line,yytext);
	st.Insert(yytext , "CONST_FLOAT");
}


{CHAR_LITERAL} {
    fprintf(tokenfile,"<CONST_CHAR,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <CONST_CHAR> Lexeme %s found\n\n",line,yytext);
	st.Insert(yytext , "CONST_CHAR");
    st.printcurrent();
}

{BREAK} {
	fprintf(tokenfile,"<BREAK>");
	fprintf(logfile,"Line no. %d: Token <BREAK> Lexeme %s found\n\n",line,yytext);
}

{CASE} {
	fprintf(tokenfile,"<CASE>");
	fprintf(logfile,"Line no. %d: Token <CASE> Lexeme %s found\n\n",line,yytext);
}

{CHAR} {
	fprintf(tokenfile,"<CHAR>");
	fprintf(logfile,"Line no. %d: Token <CHAR> Lexeme %s found\n\n",line,yytext);
}

{CONTINUE} {
	fprintf(tokenfile,"<CONTINUE>");
	fprintf(logfile,"Line no. %d: Token <CONTINUE> Lexeme %s found\n\n",line,yytext);
}

{DEFAULT} {
	fprintf(tokenfile,"<DEFAULT>");
	fprintf(logfile,"Line no. %d: Token <DEFAULT> Lexeme %s found\n\n",line,yytext);
}

{DO} {
	fprintf(tokenfile,"<DO>");
	fprintf(logfile,"Line no. %d: Token <DO> Lexeme %s found\n\n",line,yytext);
}

{DOUBLE} {
	fprintf(tokenfile,"<DOUBLE>");
	fprintf(logfile,"Line no. %d: Token <DOUBLE> Lexeme %s found\n\n",line,yytext);
}

{ELSE} {
	fprintf(tokenfile,"<ELSE>");
	fprintf(logfile,"Line no. %d: Token <ELSE> Lexeme %s found\n\n",line,yytext);
}

{FLOAT} {
	fprintf(tokenfile,"<FLOAT>");
	fprintf(logfile,"Line no. %d: Token <FLOAT> Lexeme %s found\n\n",line,yytext);
}

{FOR} {
	fprintf(tokenfile,"<FOR>");
	fprintf(logfile,"Line no. %d: Token <FOR> Lexeme %s found\n\n",line,yytext);
}

{IF} {
	fprintf(tokenfile,"<IF>");
	fprintf(logfile,"Line no. %d: Token <IF> Lexeme %s found\n\n",line,yytext);
}

{INT} {
	fprintf(tokenfile,"<INT>");
	fprintf(logfile,"Line no. %d: Token <INT> Lexeme %s found\n\n",line,yytext);

}

{RETURN} {
	fprintf(tokenfile,"<RETURN>");
	fprintf(logfile,"Line no. %d: Token <RETURN> Lexeme %s found\n\n",line,yytext);
}

{SWITCH} {
	fprintf(tokenfile,"<SWITCH>");
	fprintf(logfile,"Line no. %d: Token <SWITCH> Lexeme %s found\n\n",line,yytext);
}

{WHILE} {
	fprintf(tokenfile,"<WHILE>");
	fprintf(logfile,"Line no. %d: Token <WHILE> Lexeme %s found\n\n",line,yytext);
}

{VOID} {
	fprintf(tokenfile,"<VOID>");
	fprintf(logfile,"Line no. %d: Token <VOID> Lexeme %s found\n\n",line,yytext);
}

{ADDOP} {
	fprintf(tokenfile,"<ADDOP,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <ADDOP> Lexeme %s found\n\n",line,yytext);
}

{MULOP} {
	fprintf(tokenfile,"<MULOP,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <MULOP> Lexeme %s found\n\n",line,yytext);
}

{RELOP} {
	fprintf(tokenfile,"<RELOP,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <RELOP> Lexeme %s found\n\n",line,yytext);
}

{INCOP} {
	fprintf(tokenfile,"<INCOP,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <INCOP> Lexeme %s found\n\n",line,yytext);
}

{LOGICOP} {
	fprintf(tokenfile,"<LOGICOP,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <LOGICOP> Lexeme %s found\n\n",line,yytext);
}

{BITOP} {
	fprintf(tokenfile,"<BITOP,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <BITOP> Lexeme %s found\n\n",line,yytext);
}

{ASSIGNOP} {
	fprintf(tokenfile,"<ASSIGNOP,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <ASSIGNOP> Lexeme %s found\n\n",line,yytext);
}

{NOT} {
	fprintf(tokenfile,"<NOT,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <NOT> Lexeme %s found\n\n",line,yytext);
}

{LPAREN} {
	fprintf(tokenfile,"<LPAREN,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <LPAREN> Lexeme %s found\n\n",line,yytext);
}

{RPAREN} {
	fprintf(tokenfile,"<RPAREN,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <RPAREN> Lexeme %s found\n\n",line,yytext);
}

{LCURL} {
	fprintf(tokenfile,"<LCURL,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <LCURL> Lexeme %s found\n\n",line,yytext);
}

{RCURL} {
	fprintf(tokenfile,"<RCURL,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <RCURL> Lexeme %s found\n\n",line,yytext);
}

{LTHIRD} {
	fprintf(tokenfile,"<LTHIRD,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <LTHIRD> Lexeme %s found\n\n",line,yytext);
}

{RTHIRD} {
	fprintf(tokenfile,"<RTHIRD,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <RTHIRD> Lexeme %s found\n\n",line,yytext);
}

{COMMA} {
	fprintf(tokenfile,"<COMMA,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <COMMA> Lexeme %s found\n\n",line,yytext);
}

{SEMICOLON} {
	fprintf(tokenfile,"<SEMICOLON,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <SEMICOLON> Lexeme %s found\n\n",line,yytext);
}

{IDENTIFIER} {
	fprintf(tokenfile,"<ID,%s>",yytext);
	fprintf(logfile,"Line no. %d: Token <ID> Lexeme %s found\n\n",line,yytext);
	st.printall();
}

<<EOF>> {fprintf(logfile , "Total Line Number: %d\n",line); st.printall();return 0;}

%%

int main(int argc,char *argv[]){
	
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}
	
	
	yyin= fin;
	yylex();
    /*symboltable print dao*/
	fprintf(logfile,"\nTotal Lines : %d\nTotal Errors : %d\n",line,error_cnt);
	fclose(yyin);
	fclose(tokenfile);
	fclose(logfile);
	return 0;
}

