%option noyywrap

%x cmstate

%{
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include "symboltable.h"
#include "y.tab.h"

using namespace std;

string cm="";
int line=1;
extern int error_cnt=0;

extern FILE *logfile;
extern FILE *errorfile;

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
PRINTLN "println"

/*operators*/
ADDOP [+-]
MULOP [*/%]
INCOP ("++")
DECOP ("--")
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
                newlines_in_comment(yytext);
}

<cmstate>"*/" {
				cm += yytext;
                newlines_in_comment(cm);
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
	return 0;

}

{INT_LITERAL} {
	SymbolInfo *x=new SymbolInfo(yytext,"CONST_INT");
	yylval.symbol=x;
	return CONST_INT;
}

{FLOAT_LITERAL} {
	SymbolInfo *x=new SymbolInfo(yytext,"CONST_FLOAT");
	yylval.symbol=x;
	return CONST_FLOAT;
}


{CHAR_LITERAL} {
	string str = yytext;
    if(str.length()==3){
		SymbolInfo *s= new  SymbolInfo(yytext, "CONST_CHAR");
		yylval.symbol = s;
		return CONST_CHAR;
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

		SymbolInfo *s= new  SymbolInfo(yytext, "CONST_CHAR");
		yylval.symbol = s;
		return CONST_CHAR;
	}

}

{BREAK} {
	return BREAK;
}

{PRINTLN} {
  return PRINTLN;
}

{CASE} {
	return CASE;
}

{CHAR} {
	return CHAR;
}

{CONTINUE} {
	return CONTINUE;
}

{DEFAULT} {
	return DEFAULT;
}

{DO} {
	return DO;
}

{DOUBLE} {
	return DOUBLE;
}

{ELSE} {
	return ELSE;
}

{FLOAT} {
	return FLOAT;
}

{FOR} {
	return FOR;
}

{IF} {
	return IF;
}

{INT} {
	return INT;

}

{RETURN} {
	return RETURN;
}

{SWITCH} {
	return SWITCH;
}

{WHILE} {
	return WHILE;
}

{VOID} {
	return VOID;
}

{ADDOP} {
	SymbolInfo *x=new SymbolInfo(yytext,"ADDOP");
	yylval.symbol=x;
	return ADDOP;
}

{MULOP} {
	SymbolInfo *x=new SymbolInfo(yytext,"MULOP");
	yylval.symbol=x;
	return MULOP;
}

{RELOP} {
	SymbolInfo *x=new SymbolInfo(yytext,"RELOP");
	yylval.symbol=x;
	return RELOP;
}

{INCOP} {
	return INCOP;
}

{DECOP} {
	return DECOP;
}

{LOGICOP} {
	SymbolInfo *x=new SymbolInfo(yytext,"LOGICOP");
	yylval.symbol=x;
	return LOGICOP;
}

{ASSIGNOP} {
	return ASSIGNOP;
}

{NOT} {
	return NOT;
}

{LPAREN} {
	return LPAREN;
}

{RPAREN} {
	return RPAREN;
}

{LCURL} {
	return LCURL;
}

{RCURL} {
	return RCURL;
}

{LTHIRD} {
	return LTHIRD;
}

{RTHIRD} {
	return RTHIRD;
}

{COMMA} {
	return COMMA;
}

{SEMICOLON} {
	return SEMICOLON;
}

{IDENTIFIER} {
	SymbolInfo *x=new SymbolInfo(yytext,"ID");
	yylval.symbol=x;
	return ID;
}

{STRING} {
	pair<string , int> processed = manipulate_string(yytext);

}

{TOO_MANY_DECIMAL_POINT_ERROR} {
	error_cnt++;
	fprintf(errorfile,"Error at line. %d: Too many decimal points %s\n\n",line,yytext);
}

{ILL_FORMED_NUM} {
	error_cnt++;
	fprintf(errorfile,"Error at line. %d: Ill formed numer: %s\n\n",line,yytext);
}

{INVALID_SUFFIX_PREFIX} {
	error_cnt++;
	fprintf(errorfile,"Error at line. %d: Invalid prefix on ID or invalid suffix on Number %s\n\n",line,yytext);
}

'' {
	error_cnt++;
	fprintf(errorfile,"Error at line. %d: Empty character constant error %s\n\n",line,yytext);
}

{UNFINISHED_CHAR_ERROR} {
	error_cnt++;
	fprintf(errorfile,"Error at line. %d: Unterminated character %s\n\n",line,yytext);
}

{MULTI_CHAR_ERROR} {
	error_cnt++;
	fprintf(errorfile,"Error at line. %d: Multi character constant error %s\n\n",line,yytext);
}

\"([^\"\n]|(\\[\n])|(\\\"))* {
	error_cnt++;
	int tmp = line;
	pair<string , int> p = manipulate_string(yytext);
	fprintf(errorfile,"Error at line. %d: Unterminated string %s\n\n",tmp,yytext);
}

[.] {
	error_cnt++;
	fprintf(errorfile,"Error at line. %d: Unrecognized character %s\n\n",line,yytext);
  fprintf(logfile,"Error at line. %d: Unrecognized character %s\n\n",line,yytext);
}

%%
