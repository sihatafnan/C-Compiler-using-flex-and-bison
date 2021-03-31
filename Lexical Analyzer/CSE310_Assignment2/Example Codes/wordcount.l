%option noyywrap

%{
#include<stdio.h>
#include<stdlib.h>
#include "1705098_symboltable.h"

int word_count=0;
int line_count=1;
SymbolTable s(10);

void IncWc(){
	word_count++;
}
%}

Letter [a-zA-Z]
digit [0-9]

%%
[ \t\v]+ {}
{Letter}+ {IncWc(); printf("%s word found\n",yytext);s.Insert(yytext , "ID");s.printall();}
[\n] {line_count++;}
[+-/*] {printf("%s sign found\n",yytext);s.Exit_Scope();}
{digit}* {s.Enter_Scope();s.Insert(yytext , "Number");}
<<EOF>> {printf("Total Line Number: %d and word number: %d\n",line_count,word_count); return 0;}
. {}
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
	fclose(yyin);
	return 0;
}
