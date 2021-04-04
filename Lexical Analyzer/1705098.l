%option noyywrap

%x cmstate

%{
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include "1705098_symboltable.h"
using namespace std;

string cm="";
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

pair<string,int> manipulate_string(string s){
	int tmp_line = line;
	string x;
	for(int i=0;i<s.size();i++){
		if(s[i]=='\n')line++;
		if(s[i]=='\\' and s[i+1]=='\n') 
		continue;
		int ok=0;
		if(s[i]=='\\' and s[i+1]=='n') 
			x.push_back('\n');
		else if(s[i]=='\\' and s[i+1]=='t') 
			x.push_back('\t');
		else if(s[i]=='\\' and s[i+1]=='\\') 
			x.push_back('\\');
		else if(s[i]=='\\' and s[i+1]=='\'') 
			x.push_back('\'');
		else if(s[i]=='\\' and s[i+1]=='\"') 
			x.push_back('\"');
		else if(s[i]=='\\' and s[i+1]=='a') 
			x.push_back('\a');
		else if(s[i]=='\\' and s[i+1]=='f') 
			x.push_back('\f');
		else if(s[i]=='\\' and s[i+1]=='r') 
			x.push_back('\r');
		else if(s[i]=='\\' and s[i+1]=='v') 
			x.push_back('\v');
		else if(s[i]=='\\' and s[i+1]=='b') 
			x.push_back('\b');
		else if(s[i]=='\\' and s[i+1]=='\0') 
			x.push_back('\0');
		else if(s[i]=='\n' ) 
			continue;
		else{
			ok=1;
			x.push_back(s[i]);
		}
		if(ok==0) i++;
	}
	
	return mp(x , tmp_line); /* returns modified string and starting line cnt*/
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

SINGLE_LINE_COMMENT ("//"(([\\]+([\n]))|[^\n]*)*)

MULTI_LINE_COMMENT ([^*]|("*"[^\n]))*"*/"


/* const literals */

EXPONENTIAL ([Ee][-+]?{DIGITS})
FLOAT_VAR_ONE ({DIGITS}\.{DIGITS})({EXPONENTIAL})?
FLOAT_VAR_TWO (\.{DIGITS})({EXPONENTIAL})?
FLOAT_VAR_THREE ({DIGITS})({EXPONENTIAL})?
FLOAT_VAR_FOUR ({DIGITS})\.({EXPONENTIAL})?
FLOAT_LITERAL ({FLOAT_VAR_ONE}|{FLOAT_VAR_TWO}|{FLOAT_VAR_THREE}|{FLOAT_VAR_FOUR})

INT_LITERAL {DIGITS}

NORMAL_CHAR '([ -~]{-}['\\])'
SPECIAL_CHAR '(\\['nt\\afrbv0\"])'
CHAR_LITERAL {NORMAL_CHAR}|{SPECIAL_CHAR}

IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*

STRING \"([^\n]|(\\[\n])|(\"))*\" 	

/* errors */

/*Too many decimal point error for character sequence like 1.2.345*/
TMD1 ({DIGITS}\.{DIGITS})
TMD2 (\.{DIGITS})
TOO_MANY_DECIMAL_POINT_ERROR ({TMD1}|{TMD2})({TMD1}|{TMD2})*

/* ill formed number ex- 11E3.5*/
TYPE1 ({DIGITS}\.{DIGITS})([Ee][-+]?)({FLOAT_LITERAL})
TYPE2 (\.{DIGITS})([Ee][-+]?)({FLOAT_LITERAL})
TYPE3 ({DIGITS})([Ee][-+]?)({FLOAT_LITERAL})
TYPE4 ({TOO_MANY_DECIMAL_POINT_ERROR}(\.))([Ee][-+]?)(.*)([Ee][-+]?)?
TYPE5 ([Ee][-+]?)?({FLOAT_LITERAL})
TYPE6 ({DIGITS}(\.))
ILL_FORMED_NUM {TYPE1}|{TYPE2}|{TYPE3}

/*Invalid Suffix on numeric constant or invalid prefix on identifier for character sequence
like 12abcd*/
INVALID_SUFFIX_PREFIX [0-9]*[A-Za-z_]*


/*Unfinished character such as ‘a , ‘\n or ‘\’ */
NMCH_FOR_ERR ([ -~]{-}['])
SPCH_FOR_ERR (\\[nt\\afrbv0\"])

UNFINISHED_CHAR_ERROR_ONE '({NMCH_FOR_ERR}|{SPCH_FOR_ERR})*
FORWARD_SLASH '[\\]'

UNFINISHED_CHAR_ERROR {UNFINISHED_CHAR_ERROR_ONE}|{FORWARD_SLASH}


/*Multi character constant error for character sequence like ‘ab’ */
NMCH ([ -~]{-}['])      
SPCH (\\[nt\\afrbv0\"])
MULTI_CHAR_ERROR '(({NMCH}|{SPCH})+)'

/*unfinished comment*/
STAR_FOLLOWED_BY_ANYTHING_EXCEPT_SLASH \*[^\/]
ANYTHING_EXCEPT_STAR [^*]
UNFINISHED_COMMENT "/*"({ANYTHING_EXCEPT_STAR}|{STAR_FOLLOWED_BY_ANYTHING_EXCEPT_SLASH})*


/* Rules Section*/
%%

{WHITESPACE}    {}

{NEWLINE}       {line++; }

"/*" {
	BEGIN cmstate;
	cm += yytext;
}
{SINGLE_LINE_COMMENT} {
				int tmp = line;
                newlines_in_comment(yytext);     
                fprintf(logfile,"Line no. %d: Token <COMMENT> Lexeme %s found\n\n",tmp,yytext);      
}



<cmstate>"*/" {
				int tmp = line;
				cm += yytext;
                newlines_in_comment(cm);
                fprintf(logfile,"Line no. %d: Token <COMMENT> Lexeme %s found\n\n",tmp,cm.c_str());   
                cm="";       
                BEGIN INITIAL;
}

<cmstate>.|\n {
	cm += yytext;
	
}

<cmstate><<EOF>> {
	error_cnt++;
	int tmp = line;
    newlines_in_comment(cm); 
	fprintf(logfile,"Error at line. %d: Unterminated comment %s\n\n",tmp,cm.c_str());
	return 0;
	
}

{INT_LITERAL} {
	fprintf(tokenfile,"<CONST_INT,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <CONST_INT> Lexeme %s found\n\n",line,yytext);
	bool did = st.Insert(yytext , "CONST_INT",logfile);
    if(did)st.printall(logfile);
}

{FLOAT_LITERAL} {
	fprintf(tokenfile,"<CONST_FLOAT,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <CONST_FLOAT> Lexeme %s found\n\n",line,yytext);
	bool did = st.Insert(yytext , "CONST_FLOAT",logfile);
	if(did)st.printall(logfile);
}


{CHAR_LITERAL} {
	string str = yytext;
    if(str.length()==3){
		fprintf(logfile,"Line no. %d: Token <CONST_CHAR> Lexeme %s found --> <CHAR_CONST,%c>\n",line,yytext,str[1]);
		fprintf(tokenfile,"<CONST_CHAR,%c> ",str[1]);
	}
	else {
		
		char ch;
		if(str[2]=='n')ch='\n';
		else if(str[2]=='t')ch='\t';
		else if(str[2]=='a')ch='\a';
		else if(str[2]=='f')ch='\f';
		else if(str[2]=='r')ch='\r';
		else if(str[2]=='b')ch='\b';
		else if(str[2]=='v')ch='\v';
		else if(str[2]=='0')ch='\0';
		else if(str[2]=='\'')ch='\'';
		else if(str[2]=='\"')ch='\"';
		else if(str[2]=='\\')ch='\\';
		fprintf(tokenfile,"<CONST_CHAR,%c> ",ch);
		fprintf(logfile,"Line no. %d: Token <CONST_CHAR> Lexeme %s found --> <CHAR_CONST,%c>\n",line,yytext,ch);
	}
	bool did = st.Insert(yytext , "CONST_CHAR",logfile);
    if(did)st.printall(logfile);
}

{BREAK} {
	fprintf(tokenfile,"<BREAK> ");
	fprintf(logfile,"Line no. %d: Token <BREAK> Lexeme %s found\n\n",line,yytext);
}

{CASE} {
	fprintf(tokenfile,"<CASE> ");
	fprintf(logfile,"Line no. %d: Token <CASE> Lexeme %s found\n\n",line,yytext);
}

{CHAR} {
	fprintf(tokenfile,"<CHAR> ");
	fprintf(logfile,"Line no. %d: Token <CHAR> Lexeme %s found\n\n",line,yytext);
}

{CONTINUE} {
	fprintf(tokenfile,"<CONTINUE> ");
	fprintf(logfile,"Line no. %d: Token <CONTINUE> Lexeme %s found\n\n",line,yytext);
}

{DEFAULT} {
	fprintf(tokenfile,"<DEFAULT> ");
	fprintf(logfile,"Line no. %d: Token <DEFAULT> Lexeme %s found\n\n",line,yytext);
}

{DO} {
	fprintf(tokenfile,"<DO> ");
	fprintf(logfile,"Line no. %d: Token <DO> Lexeme %s found\n\n",line,yytext);
}

{DOUBLE} {
	fprintf(tokenfile,"<DOUBLE> ");
	fprintf(logfile,"Line no. %d: Token <DOUBLE> Lexeme %s found\n\n",line,yytext);
}

{ELSE} {
	fprintf(tokenfile,"<ELSE> ");
	fprintf(logfile,"Line no. %d: Token <ELSE> Lexeme %s found\n\n",line,yytext);
}

{FLOAT} {
	fprintf(tokenfile,"<FLOAT> ");
	fprintf(logfile,"Line no. %d: Token <FLOAT> Lexeme %s found\n\n",line,yytext);
}

{FOR} {
	fprintf(tokenfile,"<FOR> ");
	fprintf(logfile,"Line no. %d: Token <FOR> Lexeme %s found\n\n",line,yytext);
}

{IF} {
	fprintf(tokenfile,"<IF> ");
	fprintf(logfile,"Line no. %d: Token <IF> Lexeme %s found\n\n",line,yytext);
}

{INT} {
	fprintf(tokenfile,"<INT> ");
	fprintf(logfile,"Line no. %d: Token <INT> Lexeme %s found\n\n",line,yytext);

}

{RETURN} {
	fprintf(tokenfile,"<RETURN> ");
	fprintf(logfile,"Line no. %d: Token <RETURN> Lexeme %s found\n\n",line,yytext);
}

{SWITCH} {
	fprintf(tokenfile,"<SWITCH> ");
	fprintf(logfile,"Line no. %d: Token <SWITCH> Lexeme %s found\n\n",line,yytext);
}

{WHILE} {
	fprintf(tokenfile,"<WHILE> ");
	fprintf(logfile,"Line no. %d: Token <WHILE> Lexeme %s found\n\n",line,yytext);
}

{VOID} {
	fprintf(tokenfile,"<VOID> ");
	fprintf(logfile,"Line no. %d: Token <VOID> Lexeme %s found\n\n",line,yytext);
}

{ADDOP} {
	fprintf(tokenfile,"<ADDOP,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <ADDOP> Lexeme %s found\n\n",line,yytext);
}

{MULOP} {
	fprintf(tokenfile,"<MULOP,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <MULOP> Lexeme %s found\n\n",line,yytext);
}

{RELOP} {
	fprintf(tokenfile,"<RELOP,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <RELOP> Lexeme %s found\n\n",line,yytext);
}

{INCOP} {
	fprintf(tokenfile,"<INCOP,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <INCOP> Lexeme %s found\n\n",line,yytext);
}

{LOGICOP} {
	fprintf(tokenfile,"<LOGICOP,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <LOGICOP> Lexeme %s found\n\n",line,yytext);
}

{ASSIGNOP} {
	fprintf(tokenfile,"<ASSIGNOP,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <ASSIGNOP> Lexeme %s found\n\n",line,yytext);
}

{NOT} {
	fprintf(tokenfile,"<NOT,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <NOT> Lexeme %s found\n\n",line,yytext);
}

{LPAREN} {
	fprintf(tokenfile,"<LPAREN,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <LPAREN> Lexeme %s found\n\n",line,yytext);
}

{RPAREN} {
	fprintf(tokenfile,"<RPAREN,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <RPAREN> Lexeme %s found\n\n",line,yytext);
}

{LCURL} {
	fprintf(tokenfile,"<LCURL,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <LCURL> Lexeme %s found\n\n",line,yytext);
	st.Enter_Scope();
}

{RCURL} {
	fprintf(tokenfile,"<RCURL,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <RCURL> Lexeme %s found\n\n",line,yytext);
	st.Exit_Scope();
}

{LTHIRD} {
	fprintf(tokenfile,"<LTHIRD,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <LTHIRD> Lexeme %s found\n\n",line,yytext);
}

{RTHIRD} {
	fprintf(tokenfile,"<RTHIRD,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <RTHIRD> Lexeme %s found\n\n",line,yytext);
}

{COMMA} {
	fprintf(tokenfile,"<COMMA,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <COMMA> Lexeme %s found\n\n",line,yytext);
}

{SEMICOLON} {
	fprintf(tokenfile,"<SEMICOLON,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <SEMICOLON> Lexeme %s found\n\n",line,yytext);
}

{IDENTIFIER} {
	fprintf(tokenfile,"<ID,%s> ",yytext);
	fprintf(logfile,"Line no. %d: Token <ID> Lexeme %s found\n\n",line,yytext);
	bool did = st.Insert(yytext , "ID" , logfile);
	if(did)st.printall(logfile);
}

{STRING} {
	pair<string , int> processed = manipulate_string(yytext);
	fprintf(logfile,"Line no. %d: Token <STRING> Lexeme %s found --> <STRING , %s>\n",processed.second,yytext,processed.first.c_str());
	fprintf(tokenfile,"<STRING,%s> ",processed.first.c_str());
	
}

{TOO_MANY_DECIMAL_POINT_ERROR} {
	error_cnt++;
	fprintf(logfile,"Error at line. %d: Too many decimal points %s\n\n",line,yytext);
}

{ILL_FORMED_NUM} {
	error_cnt++;
	fprintf(logfile,"Error at line. %d: Ill formed numer: %s\n\n",line,yytext);
}

{INVALID_SUFFIX_PREFIX} {
	error_cnt++;
	fprintf(logfile,"Error at line. %d: Invalid prefix on ID or invalid suffix on Number %s\n\n",line,yytext);
}

'' {
	error_cnt++;
	fprintf(logfile,"Error at line. %d: Empty character constant error %s\n\n",line,yytext);
}

{UNFINISHED_CHAR_ERROR} {
	error_cnt++;
	fprintf(logfile,"Error at line. %d: Unterminated character %s\n\n",line,yytext);
}

{MULTI_CHAR_ERROR} {
	error_cnt++;
	fprintf(logfile,"Error at line. %d: Multi character constant error %s\n\n",line,yytext);
}



\"([^\"\n]|(\\[\n])|(\\\"))* {
	error_cnt++;
	int tmp = line;
	pair<string , int> p = manipulate_string(yytext);
	fprintf(logfile,"Error at line. %d: Unterminated string %s\n\n",tmp,yytext);
}

[^\n] {
	error_cnt++;
	fprintf(logfile,"Error at line. %d: Unrecgnized character %s\n\n",line,yytext);
}
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
    st.printall(logfile);
	fprintf(logfile,"\nTotal Lines : %d\nTotal Errors : %d\n",line,error_cnt);
	fclose(yyin);
	fclose(tokenfile);
	fclose(logfile);
	return 0;
}

