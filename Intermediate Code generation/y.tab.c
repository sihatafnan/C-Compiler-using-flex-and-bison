/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "1705098.y"

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


string modify_proc_for_stack(string fnm , string s){
  function_ f = get_func(fnm);
  if(f.params.size()==0)return s;

  stringstream ss(s);
  string line;
  string func="";
  vector<string>v;
  while(getline(ss,line,'\n')){
      //cout << line <<endl;
      v.push_back(line);
  }
  int sz = v.size();

    for(int i=0;i<sz;i++){
      if(v[i]=="\tPUSH DX"){
        if(f.params.size()==1){
          v[i]+="\n\tPUSH a1";
          break;
        }
        else if(f.params.size()==2){
          v[i]+="\n\tPUSH b1\n\tPUSH a1";
          break;
        }
      }
    }


  for(int i=0;i<sz-1;i++){
    ///for 1st parameter a1
    if(v[i]=="\tMOV AX, a1"){
      if(check_if_equivalent_command(v[i] , v[i+1])){
        v[i+1]=" ";
      }
      v[i] = "\tPOP AX\n\tMOV DX, AX";
    }
    if(v[i]=="\tMOV BX, a1"){
      if(check_if_equivalent_command(v[i] , v[i+1])){
        v[i+1]=" ";
      }
      v[i] = "\tPOP BX";
    }
    ///for 2nd parameter b1
    if(v[i]=="\tMOV AX, b1"){
      if(check_if_equivalent_command(v[i] , v[i+1])){
        v[i+1]=" ";
      }
      v[i] = "\tPOP AX";
    }
    if(v[i]=="\tMOV BX, b1"){
      if(check_if_equivalent_command(v[i] , v[i+1])){
        v[i+1]=" ";
      }
      v[i] = "\tPOP BX";
    }
    ///other operation except MOV
    if(v[i]=="\tADD AX, b1"){
      v[i] = "\tPOP BX\n\tADD AX, BX";
    }
    if(v[i]=="\tADD AX, a1"){
      v[i] = "\tADD AX, DX";
    }
    if(v[i]=="\tSUB AX, b1"){
      v[i] = "\tPOP BX\n\tSUB AX, BX";
    }
    if(v[i]=="\tMUL AX, b1"){
      v[i] = "\tPOP BX\n\tMUL AX, BX";
    }

  }

  for(int i=0;i<sz;i++){
    func+=(v[i]+"\n");
  }
  return func;
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


#line 591 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    FOR = 260,                     /* FOR  */
    WHILE = 261,                   /* WHILE  */
    DO = 262,                      /* DO  */
    BREAK = 263,                   /* BREAK  */
    INT = 264,                     /* INT  */
    CHAR = 265,                    /* CHAR  */
    FLOAT = 266,                   /* FLOAT  */
    DOUBLE = 267,                  /* DOUBLE  */
    VOID = 268,                    /* VOID  */
    RETURN = 269,                  /* RETURN  */
    SWITCH = 270,                  /* SWITCH  */
    CASE = 271,                    /* CASE  */
    DEFAULT = 272,                 /* DEFAULT  */
    CONTINUE = 273,                /* CONTINUE  */
    ASSIGNOP = 274,                /* ASSIGNOP  */
    INCOP = 275,                   /* INCOP  */
    DECOP = 276,                   /* DECOP  */
    NOT = 277,                     /* NOT  */
    LPAREN = 278,                  /* LPAREN  */
    RPAREN = 279,                  /* RPAREN  */
    LCURL = 280,                   /* LCURL  */
    RCURL = 281,                   /* RCURL  */
    LTHIRD = 282,                  /* LTHIRD  */
    RTHIRD = 283,                  /* RTHIRD  */
    COMMA = 284,                   /* COMMA  */
    SEMICOLON = 285,               /* SEMICOLON  */
    PRINTLN = 286,                 /* PRINTLN  */
    CONST_INT = 287,               /* CONST_INT  */
    CONST_FLOAT = 288,             /* CONST_FLOAT  */
    CONST_CHAR = 289,              /* CONST_CHAR  */
    ID = 290,                      /* ID  */
    ADDOP = 291,                   /* ADDOP  */
    MULOP = 292,                   /* MULOP  */
    RELOP = 293,                   /* RELOP  */
    LOGICOP = 294,                 /* LOGICOP  */
    LOWER_THAN_ELSE = 295          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IF 258
#define ELSE 259
#define FOR 260
#define WHILE 261
#define DO 262
#define BREAK 263
#define INT 264
#define CHAR 265
#define FLOAT 266
#define DOUBLE 267
#define VOID 268
#define RETURN 269
#define SWITCH 270
#define CASE 271
#define DEFAULT 272
#define CONTINUE 273
#define ASSIGNOP 274
#define INCOP 275
#define DECOP 276
#define NOT 277
#define LPAREN 278
#define RPAREN 279
#define LCURL 280
#define RCURL 281
#define LTHIRD 282
#define RTHIRD 283
#define COMMA 284
#define SEMICOLON 285
#define PRINTLN 286
#define CONST_INT 287
#define CONST_FLOAT 288
#define CONST_CHAR 289
#define ID 290
#define ADDOP 291
#define MULOP 292
#define RELOP 293
#define LOGICOP 294
#define LOWER_THAN_ELSE 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 521 "1705098.y"

     SymbolInfo *symbol;

#line 728 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IF = 3,                         /* IF  */
  YYSYMBOL_ELSE = 4,                       /* ELSE  */
  YYSYMBOL_FOR = 5,                        /* FOR  */
  YYSYMBOL_WHILE = 6,                      /* WHILE  */
  YYSYMBOL_DO = 7,                         /* DO  */
  YYSYMBOL_BREAK = 8,                      /* BREAK  */
  YYSYMBOL_INT = 9,                        /* INT  */
  YYSYMBOL_CHAR = 10,                      /* CHAR  */
  YYSYMBOL_FLOAT = 11,                     /* FLOAT  */
  YYSYMBOL_DOUBLE = 12,                    /* DOUBLE  */
  YYSYMBOL_VOID = 13,                      /* VOID  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_SWITCH = 15,                    /* SWITCH  */
  YYSYMBOL_CASE = 16,                      /* CASE  */
  YYSYMBOL_DEFAULT = 17,                   /* DEFAULT  */
  YYSYMBOL_CONTINUE = 18,                  /* CONTINUE  */
  YYSYMBOL_ASSIGNOP = 19,                  /* ASSIGNOP  */
  YYSYMBOL_INCOP = 20,                     /* INCOP  */
  YYSYMBOL_DECOP = 21,                     /* DECOP  */
  YYSYMBOL_NOT = 22,                       /* NOT  */
  YYSYMBOL_LPAREN = 23,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 24,                    /* RPAREN  */
  YYSYMBOL_LCURL = 25,                     /* LCURL  */
  YYSYMBOL_RCURL = 26,                     /* RCURL  */
  YYSYMBOL_LTHIRD = 27,                    /* LTHIRD  */
  YYSYMBOL_RTHIRD = 28,                    /* RTHIRD  */
  YYSYMBOL_COMMA = 29,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 30,                 /* SEMICOLON  */
  YYSYMBOL_PRINTLN = 31,                   /* PRINTLN  */
  YYSYMBOL_CONST_INT = 32,                 /* CONST_INT  */
  YYSYMBOL_CONST_FLOAT = 33,               /* CONST_FLOAT  */
  YYSYMBOL_CONST_CHAR = 34,                /* CONST_CHAR  */
  YYSYMBOL_ID = 35,                        /* ID  */
  YYSYMBOL_ADDOP = 36,                     /* ADDOP  */
  YYSYMBOL_MULOP = 37,                     /* MULOP  */
  YYSYMBOL_RELOP = 38,                     /* RELOP  */
  YYSYMBOL_LOGICOP = 39,                   /* LOGICOP  */
  YYSYMBOL_LOWER_THAN_ELSE = 40,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 41,                  /* $accept  */
  YYSYMBOL_start = 42,                     /* start  */
  YYSYMBOL_program = 43,                   /* program  */
  YYSYMBOL_unit = 44,                      /* unit  */
  YYSYMBOL_func_declaration = 45,          /* func_declaration  */
  YYSYMBOL_func_definition = 46,           /* func_definition  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_48_2 = 48,                      /* $@2  */
  YYSYMBOL_49_3 = 49,                      /* $@3  */
  YYSYMBOL_50_4 = 50,                      /* $@4  */
  YYSYMBOL_parameter_list = 51,            /* parameter_list  */
  YYSYMBOL_compound_statement = 52,        /* compound_statement  */
  YYSYMBOL_var_declaration = 53,           /* var_declaration  */
  YYSYMBOL_type_specifier = 54,            /* type_specifier  */
  YYSYMBOL_declaration_list = 55,          /* declaration_list  */
  YYSYMBOL_statements = 56,                /* statements  */
  YYSYMBOL_statement = 57,                 /* statement  */
  YYSYMBOL_expression_statement = 58,      /* expression_statement  */
  YYSYMBOL_variable = 59,                  /* variable  */
  YYSYMBOL_expression = 60,                /* expression  */
  YYSYMBOL_logic_expression = 61,          /* logic_expression  */
  YYSYMBOL_rel_expression = 62,            /* rel_expression  */
  YYSYMBOL_simple_expression = 63,         /* simple_expression  */
  YYSYMBOL_term = 64,                      /* term  */
  YYSYMBOL_unary_expression = 65,          /* unary_expression  */
  YYSYMBOL_factor = 66,                    /* factor  */
  YYSYMBOL_argument_list = 67,             /* argument_list  */
  YYSYMBOL_arguments = 68                  /* arguments  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   162

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  69
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  123

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   544,   544,   585,   593,   601,   607,   613,   622,   645,
     671,   734,   670,   774,   791,   773,   830,   841,   851,   861,
     873,   881,   890,   940,   947,   954,   963,   981,  1004,  1021,
    1044,  1050,  1059,  1066,  1073,  1080,  1106,  1126,  1150,  1176,
    1204,  1214,  1220,  1226,  1235,  1252,  1269,  1275,  1387,  1394,
    1461,  1469,  1525,  1532,  1564,  1570,  1640,  1676,  1690,  1698,
    1751,  1812,  1822,  1832,  1842,  1880,  1919,  1926,  1931,  1953
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE", "FOR",
  "WHILE", "DO", "BREAK", "INT", "CHAR", "FLOAT", "DOUBLE", "VOID",
  "RETURN", "SWITCH", "CASE", "DEFAULT", "CONTINUE", "ASSIGNOP", "INCOP",
  "DECOP", "NOT", "LPAREN", "RPAREN", "LCURL", "RCURL", "LTHIRD", "RTHIRD",
  "COMMA", "SEMICOLON", "PRINTLN", "CONST_INT", "CONST_FLOAT",
  "CONST_CHAR", "ID", "ADDOP", "MULOP", "RELOP", "LOGICOP",
  "LOWER_THAN_ELSE", "$accept", "start", "program", "unit",
  "func_declaration", "func_definition", "$@1", "$@2", "$@3", "$@4",
  "parameter_list", "compound_statement", "var_declaration",
  "type_specifier", "declaration_list", "statements", "statement",
  "expression_statement", "variable", "expression", "logic_expression",
  "rel_expression", "simple_expression", "term", "unary_expression",
  "factor", "argument_list", "arguments", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295
};
#endif

#define YYPACT_NINF (-70)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      31,   -70,   -70,   -70,    22,    31,   -70,   -70,   -70,   -70,
     -25,   -70,   -70,     5,    21,     6,     7,    17,   -70,    15,
      -8,    19,    28,    32,   -70,    39,    40,    31,   -70,   -70,
      44,    52,   -70,   -70,    39,    51,    62,    73,    75,    79,
     105,   105,   105,   -70,   -70,    82,   -70,   -70,    10,   105,
     -70,   -70,    78,    86,   -70,   -70,    60,     4,   -70,    76,
     -11,    77,   -70,   -70,   -70,   -70,   -70,   -70,   105,    71,
     105,    80,    47,   -70,    96,    89,   105,   105,   -70,   103,
     -70,   -70,   105,   -70,   -70,   -70,   -70,   105,   105,   105,
     105,   -70,   108,    71,   111,   -70,   -70,   112,   -70,   115,
     117,   116,   -70,   -70,    77,   113,   -70,   120,   105,   120,
     118,   -70,   105,   -70,   143,   130,   -70,   -70,   -70,   120,
     120,   -70,   -70
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    23,    24,    25,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    28,     0,     0,     0,     0,    22,    13,
       0,    19,     0,    26,     9,     0,    10,     0,    18,    29,
       0,     0,    14,     8,     0,    17,     0,     0,     0,     0,
       0,     0,     0,    21,    41,     0,    62,    63,    44,     0,
      34,    32,     0,     0,    30,    33,    59,     0,    46,    48,
      50,    52,    54,    58,    15,    11,    16,    27,     0,     0,
       0,     0,    59,    57,     0,     0,    67,     0,    56,    28,
      20,    31,     0,    64,    65,    43,    42,     0,     0,     0,
       0,    12,     0,     0,     0,    40,    61,     0,    69,     0,
      66,     0,    47,    49,    53,    51,    55,     0,     0,     0,
       0,    60,     0,    45,    36,     0,    38,    39,    68,     0,
       0,    37,    35
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -70,   -70,   -70,   152,   -70,   -70,   -70,   -70,   -70,   -70,
     -70,   -14,    18,     9,   -70,   -70,   -47,   -67,   -41,   -39,
     -69,    72,    69,    74,   -37,   -70,   -70,   -70
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,     7,     8,    34,    91,    25,    64,
      20,    50,    51,    52,    14,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    99,   100
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      72,    71,    93,    74,    73,    85,    81,    98,    72,    10,
      13,    32,    78,   102,    10,     1,    26,     2,     9,     3,
      65,    27,    11,     9,    21,    88,   108,    89,    15,    92,
      19,    94,    16,    76,    86,    72,    35,    77,   101,    22,
       1,    72,     2,   118,     3,    24,    72,    72,    72,    72,
      17,    18,    23,   106,    28,    37,    29,    38,    39,    30,
     114,     1,   116,     2,    31,     3,    40,    83,    84,   115,
      33,    72,   121,   122,    41,    42,    36,    31,    43,    82,
      83,    84,    44,    45,    46,    47,    66,    48,    49,    37,
      67,    38,    39,    41,    42,     1,    68,     2,    69,     3,
      40,    44,    70,    46,    47,    75,    48,    49,    41,    42,
      95,    31,    80,    79,    90,    87,    44,    45,    46,    47,
      96,    48,    49,    37,    97,    38,    39,    41,    42,     1,
      16,     2,   107,     3,    40,   109,   110,    46,    47,   111,
      48,    49,    41,    42,   113,    31,   112,   119,   117,    88,
      44,    45,    46,    47,   120,    48,    49,    12,   105,   103,
       0,     0,   104
};

static const yytype_int8 yycheck[] =
{
      41,    40,    69,    42,    41,     1,    53,    76,    49,     0,
      35,    25,    49,    82,     5,     9,    24,    11,     0,    13,
      34,    29,     0,     5,    15,    36,    93,    38,    23,    68,
      24,    70,    27,    23,    30,    76,    27,    27,    77,    32,
       9,    82,    11,   112,    13,    30,    87,    88,    89,    90,
      29,    30,    35,    90,    35,     3,    28,     5,     6,    27,
     107,     9,   109,    11,    25,    13,    14,    20,    21,   108,
      30,   112,   119,   120,    22,    23,    32,    25,    26,    19,
      20,    21,    30,    31,    32,    33,    35,    35,    36,     3,
      28,     5,     6,    22,    23,     9,    23,    11,    23,    13,
      14,    30,    23,    32,    33,    23,    35,    36,    22,    23,
      30,    25,    26,    35,    37,    39,    30,    31,    32,    33,
      24,    35,    36,     3,    35,     5,     6,    22,    23,     9,
      27,    11,    24,    13,    14,    24,    24,    32,    33,    24,
      35,    36,    22,    23,    28,    25,    29,     4,    30,    36,
      30,    31,    32,    33,    24,    35,    36,     5,    89,    87,
      -1,    -1,    88
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    11,    13,    42,    43,    44,    45,    46,    53,
      54,     0,    44,    35,    55,    23,    27,    29,    30,    24,
      51,    54,    32,    35,    30,    49,    24,    29,    35,    28,
      27,    25,    52,    30,    47,    54,    32,     3,     5,     6,
      14,    22,    23,    26,    30,    31,    32,    33,    35,    36,
      52,    53,    54,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    50,    52,    35,    28,    23,    23,
      23,    60,    59,    65,    60,    23,    23,    27,    65,    35,
      26,    57,    19,    20,    21,     1,    30,    39,    36,    38,
      37,    48,    60,    58,    60,    30,    24,    35,    61,    67,
      68,    60,    61,    62,    64,    63,    65,    24,    58,    24,
      24,    24,    29,    28,    57,    60,    57,    30,    61,     4,
      24,    57,    57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    44,    45,    45,
      47,    48,    46,    49,    50,    46,    51,    51,    51,    51,
      52,    52,    53,    54,    54,    54,    55,    55,    55,    55,
      56,    56,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    58,    58,    58,    59,    59,    60,    60,    61,    61,
      62,    62,    63,    63,    64,    64,    65,    65,    65,    66,
      66,    66,    66,    66,    66,    66,    67,    67,    68,    68
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       0,     0,     8,     0,     0,     7,     4,     3,     2,     1,
       3,     2,     3,     1,     1,     1,     3,     6,     1,     4,
       1,     2,     1,     1,     1,     7,     5,     7,     5,     5,
       3,     1,     2,     2,     1,     4,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     2,     2,     1,     1,
       4,     3,     1,     1,     2,     2,     1,     0,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: program  */
#line 545 "1705098.y"
        {
    (yyval.symbol)=(yyvsp[0].symbol);
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
		 		fprintf(asmCode,"%s",(yyval.symbol)->get_code().c_str());
        fclose(asmCode);
        ///write optimized code
        FILE *basecode = fopen("code.asm" , "r");
        optimize_code(basecode);
        fclose(basecode);
      }
    }
#line 1912 "y.tab.c"
    break;

  case 3: /* program: program unit  */
#line 586 "1705098.y"
        {
		(yyval.symbol) = new SymbolInfo((string)(yyvsp[-1].symbol)->get_name()+(string)(yyvsp[0].symbol)->get_name(), "NON_TERMINAL");;
    (yyval.symbol)->set_code((yyvsp[-1].symbol)->get_code() + (yyvsp[0].symbol)->get_code());

    fprintf(logfile,"Line %d:  program : program unit\n\n",line);
    fprintf(logfile , "%s%s\n\n" , (yyvsp[-1].symbol)->get_name().c_str() , (yyvsp[0].symbol)->get_name().c_str());
	}
#line 1924 "y.tab.c"
    break;

  case 4: /* program: unit  */
#line 594 "1705098.y"
        {
    (yyval.symbol)=(yyvsp[0].symbol);
    fprintf(logfile,"Line %d: program : unit\n\n",line);
		fprintf(logfile,"%s\n",(yyvsp[0].symbol)->get_name().c_str());
	}
#line 1934 "y.tab.c"
    break;

  case 5: /* unit: var_declaration  */
#line 602 "1705098.y"
        {
		(yyval.symbol) = (yyvsp[0].symbol);
    fprintf(logfile,"Line %d: unit : var_declaration\n\n",line);
		fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
	}
#line 1944 "y.tab.c"
    break;

  case 6: /* unit: func_declaration  */
#line 608 "1705098.y"
        {
		(yyval.symbol) = (yyvsp[0].symbol);
    fprintf(logfile,"Line %d: unit : func_declaration\n\n",line);
		fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
	}
#line 1954 "y.tab.c"
    break;

  case 7: /* unit: func_definition  */
#line 614 "1705098.y"
        {
		(yyval.symbol) = (yyvsp[0].symbol);
    fprintf(logfile,"Line %d: unit : func_definition\n\n",line);
		fprintf(logfile,"%s\n",(yyvsp[0].symbol)->get_name().c_str());
	}
#line 1964 "y.tab.c"
    break;

  case 8: /* func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON  */
#line 623 "1705098.y"
        {
    (yyval.symbol) = new SymbolInfo((yyvsp[-5].symbol)->get_name()+" "+(yyvsp[-4].symbol)->get_name()+"("+(yyvsp[-2].symbol)->get_name()+");", "NON_TERMINAL");
    fprintf(logfile , "Line %d: func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON\n\n",line);
    fprintf(logfile , "%s %s(%s);\n\n" , (yyvsp[-5].symbol)->get_name().c_str() , (yyvsp[-4].symbol)->get_name().c_str(),(yyvsp[-2].symbol)->get_name().c_str());

    /* checking whether already declared or not */
    SymbolInfo* temp = table.Lookup_in_current((yyvsp[-4].symbol)->get_name());
    if(temp != NULL) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , (yyvsp[-4].symbol)->get_name().c_str());
      fprintf(logfile , "Error at line %d: Multiple declaration of %s\n\n" , line , (yyvsp[-4].symbol)->get_name().c_str());
		}
    else{
      table.Insert((yyvsp[-4].symbol)->get_name() , "ID" , logfile);
      SymbolInfo *fd = table.Lookup_in_current((yyvsp[-4].symbol)->get_name());
      fd->set_is_declared_func(true);

      //insert in func_list
      insert_in_func_list((yyvsp[-4].symbol)->get_name() , (yyvsp[-2].symbol)->param , (yyvsp[-5].symbol)->get_name() , tempcnt-1);
    }

	}
#line 1991 "y.tab.c"
    break;

  case 9: /* func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON  */
#line 646 "1705098.y"
        {
		(yyval.symbol) = new SymbolInfo((yyvsp[-4].symbol)->get_name()+" "+(yyvsp[-3].symbol)->get_name()+"();", "NON_TERMINAL");
		fprintf(logfile , "At line no: %d func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON\n\n",line);
		fprintf(logfile , "%s %s();\n\n" , (yyvsp[-4].symbol)->get_name().c_str() , (yyvsp[-3].symbol)->get_name().c_str());


		/* checking whether already declared or not */
		SymbolInfo* temp = table.Lookup_in_current((yyvsp[-3].symbol)->get_name());
		if(temp != NULL) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , (yyvsp[-3].symbol)->get_name().c_str());
      fprintf(logfile , "Error at line %d: Multiple declaration of %s\n\n" , line , (yyvsp[-3].symbol)->get_name().c_str());
		}
    else{
      table.Insert((yyvsp[-3].symbol)->get_name() , "ID" , logfile);
      SymbolInfo *fd = table.Lookup_in_current((yyvsp[-3].symbol)->get_name());
      fd->set_is_declared_func(true);

      insert_in_func_list((yyvsp[-3].symbol)->get_name() , (yyvsp[-4].symbol)->get_name());
    }

	}
#line 2018 "y.tab.c"
    break;

  case 10: /* $@1: %empty  */
#line 671 "1705098.y"
  {
    //chking if invalid params given
    if((yyvsp[-1].symbol)->get_name()=="int" or (yyvsp[-1].symbol)->get_name()=="float"){
      error_cnt++;
      fprintf(logfile , "Error at line %d: 1th parameter's name not given in function definition of var\n\n" , line);
      fprintf(errorfile , "Error at line %d: 1th parameter's name not given in function definition of var\n\n" , line);
    }
    //chking if declared previously and now being defined
    //param types,return type must be matched
    //assuming we don't need to handle function overloading
    if(is_in_func_list((yyvsp[-3].symbol)->get_name())){
      function_ f = get_func((yyvsp[-3].symbol)->get_name());
      if(f.return_type != (yyvsp[-4].symbol)->get_name()){
        error_cnt++;
        fprintf(errorfile , "Error at line: %d Type Mismatch (previously declared but not being defined properly now)\n\n",line);
      }
      else {
        bool matched = true;
        int n = (yyvsp[-1].symbol)->param.size();
        if(n == f.params.size()){
          for(int i=0;i<f.params.size();i++){
            if(f.params[i].first != (yyvsp[-1].symbol)->param[i].param_type){
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
		SymbolInfo* temp = table.Lookup_in_current((yyvsp[-3].symbol)->get_name());
		if(temp != NULL and (!temp->get_is_declared_func())) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple definition of %s\n\n" , line , (yyvsp[-3].symbol)->get_name().c_str());
		}
		else if(temp == NULL){
      //insert in table
      table.Insert((yyvsp[-3].symbol)->get_name() , "ID" , logfile);
		}

    table.Enter_Scope(logfile);
    for(int i=0;i<(yyvsp[-1].symbol)->param.size();i++){
        string nm = (yyvsp[-1].symbol)->param[i].param_name;
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

  }
#line 2087 "y.tab.c"
    break;

  case 11: /* $@2: %empty  */
#line 734 "1705098.y"
                       {table.printall(logfile); table.Exit_Scope(logfile);var_list.clear();}
#line 2093 "y.tab.c"
    break;

  case 12: /* func_definition: type_specifier ID LPAREN parameter_list RPAREN $@1 compound_statement $@2  */
#line 736 "1705098.y"
  {    insert_in_func_list((yyvsp[-6].symbol)->get_name() , (yyvsp[-4].symbol)->param , (yyvsp[-7].symbol)->get_name() , tempcnt-1);
      //Assembly
      (yyval.symbol) = new SymbolInfo((yyvsp[-7].symbol)->get_name()+" "+(yyvsp[-6].symbol)->get_name()+"("+(yyvsp[-4].symbol)->get_name()+")"+(yyvsp[-1].symbol)->get_name()+"\n\n", "NON_TERMINAL");
      fprintf(logfile , "Line %d: func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement\n\n" , line);
      fprintf(logfile , "%s %s(%s)%s\n\n" , (yyvsp[-7].symbol)->get_name().c_str(),(yyvsp[-6].symbol)->get_name().c_str(),(yyvsp[-4].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str());

      function_ f = get_func((yyvsp[-6].symbol)->get_name());
      //cout<<$2->get_name()<<" "<<f.return_reg_no<<endl;
      if((yyvsp[-6].symbol)->get_name()=="main"){
        assembly_codes="MAIN PROC\n\n";
      }
			else{
        assembly_codes=(yyvsp[-6].symbol)->get_name()+" PROC\n\n";
      }

			if((yyvsp[-6].symbol)->get_name()=="main"){
				assembly_codes+=main_proc_start_code();
			}
			else{
			  assembly_codes+=pushCode();
			}

			assembly_codes+=((yyvsp[-1].symbol)->get_code());

			if((yyvsp[-6].symbol)->get_name()=="main") {
				assembly_codes+=main_proc_ending_code();
			}
			else{
				assembly_codes+=popCode();
				assembly_codes+=("RET\n");
				assembly_codes+=((yyvsp[-6].symbol)->get_name()+" ENDP\n\n");
			}

      string str = modify_proc_for_stack((yyvsp[-6].symbol)->get_name() , assembly_codes);
      string final = modify_proc((yyvsp[-6].symbol)->get_name() , assembly_codes);
      if((yyvsp[-6].symbol)->get_name()!="main")assembly_procs += final;
  }
#line 2135 "y.tab.c"
    break;

  case 13: /* $@3: %empty  */
#line 774 "1705098.y"
    {
      //current_return_type = $1->get_name();
      /* checking whether already declared or not */
  		SymbolInfo* temp = table.Lookup_in_current((yyvsp[-2].symbol)->get_name());
      if(temp != NULL and (!temp->get_is_declared_func())) {
  			error_cnt++;
  			fprintf(errorfile , "Error at line %d: Multiple definition of %s\n\n" , line , (yyvsp[-2].symbol)->get_name().c_str());
        fprintf(logfile , "Error at line %d: Multiple definition of %s\n\n" , line , (yyvsp[-2].symbol)->get_name().c_str());
  		}
  		else if(temp == NULL){
  			table.Insert((yyvsp[-2].symbol)->get_name() , "ID" , logfile);

  		}

      table.Enter_Scope(logfile);

    }
#line 2157 "y.tab.c"
    break;

  case 14: /* $@4: %empty  */
#line 791 "1705098.y"
                       {table.printall(logfile);table.Exit_Scope(logfile);var_list.clear();}
#line 2163 "y.tab.c"
    break;

  case 15: /* func_definition: type_specifier ID LPAREN RPAREN $@3 compound_statement $@4  */
#line 792 "1705098.y"
        {
      insert_in_func_list((yyvsp[-5].symbol)->get_name() , (yyvsp[-6].symbol)->get_name());
      fprintf(logfile , "Line %d: func_definition : type_specifier ID LPAREN RPAREN compound_statement\n\n" , line);
      fprintf(logfile , "%s %s()%s\n\n" , (yyvsp[-6].symbol)->get_name().c_str(),(yyvsp[-5].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str());

      //assembly
      (yyval.symbol) = new SymbolInfo((yyvsp[-6].symbol)->get_name()+" "+(yyvsp[-5].symbol)->get_name()+"()"+(yyvsp[-1].symbol)->get_name()+"\n\n", "NON_TERMINAL");
      if((yyvsp[-5].symbol)->get_name()=="main")
				assembly_codes="MAIN PROC\n\n";
			else
				assembly_codes=(yyvsp[-5].symbol)->get_name()+" PROC\n\n";

			if((yyvsp[-5].symbol)->get_name()=="main"){
				assembly_codes+=main_proc_start_code();
			}
			else{
			  assembly_codes+=pushCode();
			}

			//inside func body
			assembly_codes+=((yyvsp[-1].symbol)->get_code());
			//ending of main proc
			if((yyvsp[-5].symbol)->get_name()=="main") {
				assembly_codes+=main_proc_ending_code();
			}
			else{
				assembly_codes+=popCode();
				assembly_codes+=("RET\n");
				assembly_codes+=((yyvsp[-5].symbol)->get_name()+" ENDP\n\n");
			}
      //cout<<modify_proc($2->get_name() , assembly_codes)<<endl;
      if((yyvsp[-5].symbol)->get_name()=="main")(yyval.symbol)->set_code(assembly_codes);
      if((yyvsp[-5].symbol)->get_name()!="main")assembly_procs += modify_proc((yyvsp[-5].symbol)->get_name() , assembly_codes);
	}
#line 2202 "y.tab.c"
    break;

  case 16: /* parameter_list: parameter_list COMMA type_specifier ID  */
#line 831 "1705098.y"
                {
			(yyval.symbol) = new SymbolInfo((yyvsp[-3].symbol)->get_name()+","+(yyvsp[-1].symbol)->get_name()+" "+(yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no: %d parameter_list  : parameter_list COMMA type_specifier ID\n\n",line);
			fprintf(logfile , "%s , %s %s\n\n" , (yyvsp[-3].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

			/* adding parameter to parameter list */
      (yyval.symbol)->param = (yyvsp[-3].symbol)->param;
			(yyval.symbol)->push_in_param((yyvsp[0].symbol)->get_name() , (yyvsp[-1].symbol)->get_name());

		}
#line 2217 "y.tab.c"
    break;

  case 17: /* parameter_list: parameter_list COMMA type_specifier  */
#line 842 "1705098.y"
                {
			(yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+","+(yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no: %d parameter_list  : parameter_list COMMA type_specifier\n\n",line);
			fprintf(logfile , "%s , %s\n\n" , (yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

			/* adding parameter to parameter list */
      (yyval.symbol)->param = (yyvsp[-2].symbol)->param;
			(yyval.symbol)->push_in_param("", (yyvsp[0].symbol)->get_name());
		}
#line 2231 "y.tab.c"
    break;

  case 18: /* parameter_list: type_specifier ID  */
#line 852 "1705098.y"
                {
			(yyval.symbol) = new SymbolInfo((yyvsp[-1].symbol)->get_name()+" "+(yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no: %d parameter_list  : type_specifier ID\n\n",line);
			fprintf(logfile , "%s %s\n\n" , (yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

			/* adding parameter to parameter list */

			(yyval.symbol)->push_in_param((yyvsp[0].symbol)->get_name() ,(yyvsp[-1].symbol)->get_name());
		}
#line 2245 "y.tab.c"
    break;

  case 19: /* parameter_list: type_specifier  */
#line 862 "1705098.y"
                {
			(yyval.symbol) = new SymbolInfo((yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no: %d parameter_list  : type_specifier\n\n",line);
			fprintf(logfile , "%s %s\n\n" , (yyvsp[0].symbol)->get_name().c_str());

			/* adding parameter to parameter list */
			(yyval.symbol)->push_in_param( "" , (yyvsp[0].symbol)->get_name());
		}
#line 2258 "y.tab.c"
    break;

  case 20: /* compound_statement: LCURL statements RCURL  */
#line 874 "1705098.y"
  {
      (yyval.symbol)=(yyvsp[-1].symbol);
      //$$ = new SymbolInfo("{\n"+$2->get_name()+"\n}"+"\n\n", "NON_TERMINAL");
      fprintf(logfile,"At line no: %d compound_statement : LCURL statements RCURL\n\n",line);
			fprintf(logfile,"{\n%s\n}\n\n",(yyvsp[-1].symbol)->get_name().c_str());

  }
#line 2270 "y.tab.c"
    break;

  case 21: /* compound_statement: LCURL RCURL  */
#line 882 "1705098.y"
  {
    (yyval.symbol) = new SymbolInfo("{\n}", "NON_TERMINAL");
    fprintf(logfile,"At line no: %d compound_statement : LCURL RCURL\n\n",line);
    fprintf(logfile,"{}\n\n");

  }
#line 2281 "y.tab.c"
    break;

  case 22: /* var_declaration: type_specifier declaration_list SEMICOLON  */
#line 891 "1705098.y"
                {
			fprintf(logfile,"At line no: %d var_declaration : type_specifier declaration_list SEMICOLON\n\n",line);
			fprintf(logfile,"%s %s;\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str());
			(yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+" "+(yyvsp[-1].symbol)->get_name()+";", "NON_TERMINAL");

      (yyval.symbol)->setVariableType((yyvsp[-2].symbol)->get_name());

      //setting type of all elements of var in declaration_list
      for(int i=0;i<(yyvsp[-1].symbol)->var.size();i++){
          (yyvsp[-1].symbol)->var[i].type = (yyvsp[-2].symbol)->get_name();
      }
			/* inserting in symboltable */
			if((yyvsp[-2].symbol)->get_name()=="void"){
				error_cnt++;
				fprintf(errorfile,"Error at line %d: variable cannot be of type void\n\n",line);

			}
			else{
				/* checking whether already declared or not */
				for(int i=0;i<(yyvsp[-1].symbol)->var.size();i++){
					SymbolInfo* temp = table.Lookup_in_current((yyvsp[-1].symbol)->var[i].name);
					if(temp != NULL) {
						error_cnt++;
						fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , (yyvsp[-1].symbol)->var[i].name.c_str());
					}
					else{
						table.Insert((yyvsp[-1].symbol)->var[i].name , "ID" , logfile);
            SymbolInfo *tmp = table.Lookup_in_current((yyvsp[-1].symbol)->var[i].name);
            tmp->setVariableType((yyvsp[-2].symbol)->get_name());
            if((yyvsp[-1].symbol)->var[i].sz>0){
              tmp->setIdentity("array");
            }else{
              tmp->setIdentity("var");
            }
					}
				}

			}

		}
#line 2326 "y.tab.c"
    break;

  case 23: /* type_specifier: INT  */
#line 941 "1705098.y"
                {
			fprintf(logfile,"At line no: %d: type_specifier : INT \n\n",line);
			SymbolInfo *x = new SymbolInfo("int" , "int");
			(yyval.symbol) = x;
			fprintf(logfile,"%s\n\n",(yyval.symbol)->get_name().c_str());
		}
#line 2337 "y.tab.c"
    break;

  case 24: /* type_specifier: FLOAT  */
#line 948 "1705098.y"
                {
			fprintf(logfile,"At line no: %d: type_specifier : FLOAT \n",line);
			SymbolInfo *x = new SymbolInfo("float" , "float");
			(yyval.symbol) = x;
			fprintf(logfile,"%s\n\n",(yyval.symbol)->get_name().c_str());
		}
#line 2348 "y.tab.c"
    break;

  case 25: /* type_specifier: VOID  */
#line 955 "1705098.y"
                {
			fprintf(logfile,"At line no: %d: type_specifier : VOID \n",line);
			SymbolInfo *x = new SymbolInfo("void" , "void");
			(yyval.symbol) = x;
			fprintf(logfile,"%s\n\n",(yyval.symbol)->get_name().c_str());
		}
#line 2359 "y.tab.c"
    break;

  case 26: /* declaration_list: declaration_list COMMA ID  */
#line 964 "1705098.y"
                {
 			variableList_to_be_Initialized.push_back({(yyvsp[0].symbol)->get_name()+table.get_current_id()[0],"0"});

			fprintf(logfile,"At line no: %d: declaration_list : declaration_list COMMA ID\n\n",line);
			(yyval.symbol) = new SymbolInfo((string)(yyvsp[-2].symbol)->get_name()+(string)","+(string)(yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
			fprintf(logfile , "%s,%s\n\n" , (yyvsp[-2].symbol)->get_name().c_str() , (yyvsp[0].symbol)->get_name().c_str());

			/* keeping track of identifier(variable) */
      temp_var.var_name = (yyvsp[0].symbol)->get_name();
      temp_var.var_size = -1;
      var_list.push_back(temp_var);

      /* 3 args are name , type, size of variable */
      (yyval.symbol)->var = (yyvsp[-2].symbol)->var;
      (yyval.symbol)->push_in_var((yyvsp[0].symbol)->get_name() , "" , 0);

		}
#line 2381 "y.tab.c"
    break;

  case 27: /* declaration_list: declaration_list COMMA ID LTHIRD CONST_INT RTHIRD  */
#line 982 "1705098.y"
                {
 			variableList_to_be_Initialized.push_back({(yyvsp[-3].symbol)->get_name()+table.get_current_id()[0],(yyvsp[-1].symbol)->get_name()});

			fprintf(logfile,"At line no: %d declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD\n\n",line);
			(yyval.symbol) = new SymbolInfo((string)(yyvsp[-5].symbol)->get_name()+(string)","+(string)(yyvsp[-3].symbol)->get_name()+(string)"["+(string)(yyvsp[-1].symbol)->get_name()+(string)"]", "NON_TERMINAL");
			fprintf(logfile , "%s,%s[%s]\n\n" , (yyvsp[-5].symbol)->get_name().c_str() , (yyvsp[-3].symbol)->get_name().c_str() , (yyvsp[-1].symbol)->get_name().c_str());

			/* keeping track of identifier(array) */
      temp_var.var_name = (string)(yyvsp[-3].symbol)->get_name();
      stringstream temp_str((string) (yyvsp[-1].symbol)->get_name());
      temp_str >> temp_var.var_size;
      var_list.pb(temp_var);

      stringstream geek((yyvsp[-1].symbol)->get_name());
      int sz = 0;
      geek >> sz;

      (yyval.symbol)->var = (yyvsp[-5].symbol)->var;
      (yyval.symbol)->push_in_var((yyvsp[-3].symbol)->get_name() , "" , sz);

		}
#line 2407 "y.tab.c"
    break;

  case 28: /* declaration_list: ID  */
#line 1005 "1705098.y"
                {
 			variableList_to_be_Initialized.push_back({(yyvsp[0].symbol)->get_name()+table.get_current_id()[0],"0"});

			fprintf(logfile,"At line no: %d declaration_list : ID\n\n",line);
 			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

 			(yyval.symbol) = new SymbolInfo((yyvsp[0].symbol)->get_name() ,  "ID");

			 /* keeping track of identifier(variable) */
      temp_var.var_name = (string)(yyvsp[0].symbol)->get_name();
      temp_var.var_size = -1;
      var_list.pb(temp_var);

      (yyval.symbol)->push_in_var((yyvsp[0].symbol)->get_name() , "" , 0);

		}
#line 2428 "y.tab.c"
    break;

  case 29: /* declaration_list: ID LTHIRD CONST_INT RTHIRD  */
#line 1022 "1705098.y"
                {
 			variableList_to_be_Initialized.push_back({(yyvsp[-3].symbol)->get_name()+table.get_current_id()[0],(yyvsp[-1].symbol)->get_name()});

			fprintf(logfile , "At line no: %d  declaration_list: ID LTHIRD CONST_INT RTHIRD\n\n",line);
			(yyval.symbol) = new SymbolInfo((yyvsp[-3].symbol)->get_name()+"["+(yyvsp[-1].symbol)->get_name()+"]", "NON_TERMINAL");
			fprintf(logfile , "%s[%s]\n\n",(yyvsp[-3].symbol)->get_name().c_str() , (yyvsp[-1].symbol)->get_name().c_str());

			temp_var.var_name = (yyvsp[-3].symbol)->get_name();
      stringstream temp_str((yyvsp[-1].symbol)->get_name());
      temp_str >> temp_var.var_size;
      var_list.pb(temp_var);

      stringstream geek((yyvsp[-1].symbol)->get_name());
      int sz = 0;
      geek >> sz;

      (yyval.symbol)->push_in_var((yyvsp[-3].symbol)->get_name() , "" , sz);

		}
#line 2452 "y.tab.c"
    break;

  case 30: /* statements: statement  */
#line 1045 "1705098.y"
    {
       (yyval.symbol) = (yyvsp[0].symbol);
       fprintf(logfile , "At line no: %d statements : statement\n\n" , line);
       fprintf(logfile , "%s\n\n" , (yyvsp[0].symbol)->get_name().c_str());
    }
#line 2462 "y.tab.c"
    break;

  case 31: /* statements: statements statement  */
#line 1051 "1705098.y"
    {
      (yyval.symbol) = new SymbolInfo((yyvsp[-1].symbol)->get_name()+(yyvsp[0].symbol)->get_name() , "NON_TERMINAL");
      fprintf(logfile , "At line no: %d statements : statements statement\n\n" , line);
      fprintf(logfile , "%s %s\n\n" , (yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());
      (yyval.symbol)->set_code((yyvsp[-1].symbol)->get_code() + " " + (yyvsp[0].symbol)->get_code());
    }
#line 2473 "y.tab.c"
    break;

  case 32: /* statement: var_declaration  */
#line 1060 "1705098.y"
    {
      fprintf(logfile,"At line no: %d statement : var_declaration\n\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
      (yyvsp[0].symbol)->set_name((yyvsp[0].symbol)->get_name()+"\n");
  		(yyval.symbol)=(yyvsp[0].symbol);
    }
#line 2484 "y.tab.c"
    break;

  case 33: /* statement: expression_statement  */
#line 1067 "1705098.y"
    {
      fprintf(logfile,"At line no: %d statement : expression_statement\n\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
      (yyvsp[0].symbol)->set_name((yyvsp[0].symbol)->get_name()+"\n");
			(yyval.symbol)=(yyvsp[0].symbol);
    }
#line 2495 "y.tab.c"
    break;

  case 34: /* statement: compound_statement  */
#line 1074 "1705098.y"
    {
      fprintf(logfile,"At line no: %d statement : compound_statement\n\n",line);
      fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

      (yyval.symbol)=(yyvsp[0].symbol);
    }
#line 2506 "y.tab.c"
    break;

  case 35: /* statement: FOR LPAREN expression_statement expression_statement expression RPAREN statement  */
#line 1081 "1705098.y"
    {
      string l1=newLabel();
      string l2=newLabel();

      (yyval.symbol) = (yyvsp[-4].symbol);

      (yyval.symbol)->add_code(";-------for loop starts--------\n\t");
			(yyval.symbol)->add_code(l1+":\n");

			(yyval.symbol)->add_code((yyvsp[-3].symbol)->get_code());

			(yyval.symbol)->add_code("\tMOV AX, "+(yyvsp[-3].symbol)->get_name()+"\n");
			(yyval.symbol)->add_code("\tCMP AX, 0\n");
			(yyval.symbol)->add_code("\tJE "+l2+"\n");

			(yyval.symbol)->add_code((yyvsp[0].symbol)->get_code());
			(yyval.symbol)->add_code((yyvsp[-2].symbol)->get_code());
			(yyval.symbol)->add_code("\tJMP "+l1+"\n");

			(yyval.symbol)->add_code("\t"+l2+":\n");

      string str="for("+(yyvsp[-4].symbol)->get_name()+(yyvsp[-3].symbol)->get_name()+(yyvsp[-2].symbol)->get_name()+")"+(yyvsp[0].symbol)->get_name();
      fprintf(logfile,"line no. %d: statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement\n",line);
			fprintf(logfile,"%s\n\n",str.c_str());
    }
#line 2536 "y.tab.c"
    break;

  case 36: /* statement: IF LPAREN expression RPAREN statement  */
#line 1107 "1705098.y"
    {
      string str = "if("+(yyvsp[-2].symbol)->get_name()+")"+(yyvsp[0].symbol)->get_name();
      fprintf(logfile,"At line no: %d statement : IF LPAREN expression RPAREN statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());
      (yyval.symbol)=(yyvsp[-2].symbol);
      //$$->set_name(str);
      (yyval.symbol)->settype("if");

			string label=newLabel();

      (yyval.symbol)->add_code(";--------if block---------\n");
			(yyval.symbol)->add_code("\tMOV AX, "+(yyvsp[-2].symbol)->get_name()+"\n");

			(yyval.symbol)->add_code("\tCMP AX, 0\n");
			(yyval.symbol)->add_code("\tJE "+label+"\n");
			(yyval.symbol)->add_code((yyvsp[0].symbol)->get_code());
			(yyval.symbol)->add_code("\t"+label+":\n");

    }
#line 2560 "y.tab.c"
    break;

  case 37: /* statement: IF LPAREN expression RPAREN statement ELSE statement  */
#line 1127 "1705098.y"
    {
      string str = "if("+(yyvsp[-4].symbol)->get_name()+")"+(yyvsp[-2].symbol)->get_name()+"else"+(yyvsp[0].symbol)->get_name();
      fprintf(logfile,"At line no: %d statement : IF LPAREN expression RPAREN statement ELSE statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());
      (yyval.symbol)=(yyvsp[-4].symbol);

      (yyval.symbol)->settype("if---else---if");

			string else_condition=newLabel();
			string after_else=newLabel();
      (yyval.symbol)->add_code(";--------if else block---------\n");
			(yyval.symbol)->add_code("\tMOV AX, "+(yyvsp[-4].symbol)->get_name()+"\n");
			(yyval.symbol)->add_code("\tCMP AX, 0\n");
			(yyval.symbol)->add_code("\tJE "+else_condition+"\n");

			(yyval.symbol)->add_code((yyvsp[-2].symbol)->get_code());
			(yyval.symbol)->add_code("\tJMP "+after_else);

			(yyval.symbol)->add_code("\n\t"+else_condition+":\n");
			(yyval.symbol)->add_code((yyvsp[0].symbol)->get_code());
			(yyval.symbol)->add_code("\n\t"+after_else+":\n");

    }
#line 2588 "y.tab.c"
    break;

  case 38: /* statement: WHILE LPAREN expression RPAREN statement  */
#line 1151 "1705098.y"
    {
      string str = "while("+(yyvsp[-2].symbol)->get_name()+")"+(yyvsp[0].symbol)->get_name();
      fprintf(logfile,"At line no: %d statement : WHILE LPAREN expression RPAREN statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());

      (yyval.symbol)=new SymbolInfo("while","loop");

			string l1=newLabel(), l2=newLabel();
      assembly_codes=(";--------while loop---------\n\t");
			assembly_codes+=(l1+":\n");

			assembly_codes+=(yyvsp[-2].symbol)->get_code();

			assembly_codes+=("\tMOV AX, "+(yyvsp[-2].symbol)->get_name()+"\n");
			assembly_codes+="\tCMP AX, 0\n";
			assembly_codes+="\tJE "+l2+"\n";

			assembly_codes+=(yyvsp[0].symbol)->get_code();
			assembly_codes+="\tJMP "+l1+"\n";

			assembly_codes+=("\t"+l2+":\n");

			(yyval.symbol)->set_code(assembly_codes);

    }
#line 2618 "y.tab.c"
    break;

  case 39: /* statement: PRINTLN LPAREN ID RPAREN SEMICOLON  */
#line 1177 "1705098.y"
    {

      fprintf(logfile,"Line %d: statement : PRINTLN LPAREN ID RPAREN SEMICOLON\n\n",line);
      fprintf(logfile , "printf(%s);\n\n" , (yyvsp[-2].symbol)->get_name().c_str());

      (yyval.symbol)=new SymbolInfo("println","nonterminal");
      assembly_codes=(";--------print function called---------\n");
			assembly_codes+=("\n\tMOV AX, "+(yyvsp[-2].symbol)->get_name()+table.get_current_id()[0]+"\n");
			assembly_codes+=("\tCALL PRINT_ID\n");
			(yyval.symbol)->set_code(assembly_codes);


      if((yyvsp[-2].symbol)->get_is_func()){
        if(!table.Lookup_in_current(modified_name_while_func_calling((yyvsp[-2].symbol)->get_name()))){
          error_cnt++;
          fprintf(errorfile , "Error at line: %d Undeclared function %s\n\n" , line,modified_name_while_func_calling((yyvsp[-2].symbol)->get_name()).c_str());
          fprintf(logfile , "Error at line: %d Undeclared function %s\n\n" , line,modified_name_while_func_calling((yyvsp[-2].symbol)->get_name()).c_str());
        }
      }
      else if(!table.Lookup((yyvsp[-2].symbol)->get_name())){
        error_cnt++;
        fprintf(errorfile , "Error at line: %d Undeclared Variable %s\n\n" , line,(yyvsp[-2].symbol)->get_name().c_str());
        fprintf(logfile , "Error at line: %d Undeclared Variable %s\n\n" , line,(yyvsp[-2].symbol)->get_name().c_str());
      }


    }
#line 2650 "y.tab.c"
    break;

  case 40: /* statement: RETURN expression SEMICOLON  */
#line 1205 "1705098.y"
    {
      fprintf(logfile,"Line %d: statement : RETURN expression SEMICOLON\n\n",line);
      fprintf(logfile , "return %s;\n\n" , (yyvsp[-1].symbol)->get_name().c_str());
      (yyval.symbol)=new SymbolInfo("return","statement");
			assembly_codes=(yyvsp[-1].symbol)->get_code();
			(yyval.symbol)->set_code(assembly_codes);
    }
#line 2662 "y.tab.c"
    break;

  case 41: /* expression_statement: SEMICOLON  */
#line 1215 "1705098.y"
    {
      (yyval.symbol)=new SymbolInfo("SEMICOLON","SEMICOLON");
      fprintf(logfile,"Line %d: expression_statement : SEMICOLON\n",line);
			fprintf(logfile,";\n\n");
    }
#line 2672 "y.tab.c"
    break;

  case 42: /* expression_statement: expression SEMICOLON  */
#line 1221 "1705098.y"
    {
      (yyval.symbol) = (yyvsp[-1].symbol);
      fprintf(logfile,"Line %d: expression_statement : expression SEMICOLON\n\n",line);
      fprintf(logfile , "%s;\n\n" , (yyvsp[-1].symbol)->get_name().c_str());
    }
#line 2682 "y.tab.c"
    break;

  case 43: /* expression_statement: expression error  */
#line 1227 "1705098.y"
   {
     //error_cnt++;
     yyclearin;
   }
#line 2691 "y.tab.c"
    break;

  case 44: /* variable: ID  */
#line 1236 "1705098.y"
    {
      fprintf(logfile,"Line %d: variable : ID\n",line);
      fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

      (yyval.symbol) = (yyvsp[0].symbol);
      (yyval.symbol)->idx=-1;
      //Semantic : chk if variable is declared before
      (yyval.symbol)->setIdentity("var");
      (yyval.symbol)->push_in_var((yyvsp[0].symbol)->get_name(),"",0);
      SymbolInfo *x=table.Lookup((yyvsp[0].symbol)->get_name());
      if(x)(yyval.symbol)->setVariableType(x->getVariableType());
      //cout<<$$->get_name()<<endl;
      (yyval.symbol)->set_assembly_value((yyval.symbol)->get_name()+table.get_current_id()[0]);

    }
#line 2711 "y.tab.c"
    break;

  case 45: /* variable: ID LTHIRD expression RTHIRD  */
#line 1253 "1705098.y"
   {
     fprintf(logfile,"At line no: %d variable : ID LTHIRD expression RTHIRD\n",line);
		 fprintf(logfile,"%s[%s]\n\n",(yyvsp[-3].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str());
     (yyval.symbol) = new SymbolInfo((yyvsp[-3].symbol)->get_name()+"["+(yyvsp[-1].symbol)->get_name()+"]" , "variable");

     (yyval.symbol)->setIdentity("array");
     stringstream geek((yyvsp[-1].symbol)->get_name());
     int x = 0;
     geek >> x;
     (yyval.symbol)->idx=x;
     (yyval.symbol)->set_assembly_value((yyval.symbol)->get_name()+table.get_current_id()[0]);

   }
#line 2729 "y.tab.c"
    break;

  case 46: /* expression: logic_expression  */
#line 1270 "1705098.y"
    {
      (yyval.symbol) = (yyvsp[0].symbol);
      fprintf(logfile,"Line %d: expression : logic_expression\n\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
    }
#line 2739 "y.tab.c"
    break;

  case 47: /* expression: variable ASSIGNOP logic_expression  */
#line 1276 "1705098.y"
    {
      (yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+"="+(yyvsp[0].symbol)->get_name() , "expression");
      fprintf(logfile,"Line %d: expression : variable ASSIGNOP logic_expression\n\n",line);
      fprintf(logfile,"%s = %s\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());
      //semantics
      //todo
      //assign $3's var_type to $1 after some error chkings

      ///pass arrayname if array otherwise pass varname only
      ///suppose $1->get_name() is a[2].Now modified_name returns only a
      string varname;
      varname = modified_name((yyvsp[-2].symbol)->get_name());
      SymbolInfo *x=table.Lookup(varname);
			if(x)
			{
				//setting type of var(int/float)
        (yyvsp[-2].symbol)->setVariableType(x->getVariableType());

        //chk if variable and written with index
        bool isvar=true;
        for(int i=0;i<var_list.size();i++){
          if(var_list[i].var_name==x->get_name() && var_list[i].var_size>0){
            isvar = false;break;
         }
        }
        if(isvar){
          if(varname != (yyvsp[-2].symbol)->get_name()){
            error_cnt++;
            fprintf(errorfile , "Error at line: %d %s is not an array\n\n" , line,varname.c_str());
          }
        }
        //chk if array
        for(int i=0;i<var_list.size();i++){
          if(var_list[i].var_name==x->get_name() && var_list[i].var_size>0){
            //now we're sure that it's an array
            //let's see if ara is being used without any index
            if(varname==(yyvsp[-2].symbol)->get_name()){
              error_cnt++;
              fprintf(errorfile,"Error at line: %d Type Mismatch\n\n",line);
              break;
            }
            //now chk if wrong index is given
            else if(!is_ara_idx_valid((yyvsp[-2].symbol)->get_name() , var_list[i].var_size)){
              error_cnt++;
              fprintf(errorfile,"Error at line: %d Wrong array index(Expression inside third brackets not an integer)\n\n",line);
              break;
            }
          }
        }

        //check if float is assigned to int
        if(x->getVariableType()=="int" && (yyvsp[0].symbol)->getVariableType()=="float"){
					error_cnt++;
          (yyval.symbol)->setVariableType("int");
					fprintf(errorfile,"Error at line %d: type mismatch in assignment \n\n",line,(yyvsp[0].symbol)->getVariableType().c_str(),x->getVariableType().c_str());
				}
        else if(x->getVariableType()=="float" && (yyvsp[0].symbol)->getVariableType()=="int"){
          (yyval.symbol)->setVariableType("float");
        }
        else{
          (yyval.symbol)->setVariableType(x->getVariableType());
        }


      if((yyvsp[0].symbol)->get_is_func()){
        ///extract function name cause $3 has name like foo(6) but we need only foo
        string fnm = modified_name_while_func_calling((yyvsp[0].symbol)->get_name());
        if(is_in_func_list(fnm)){
          function_ f = get_func(fnm);
          ///chk if func is returning to valid type

          if(f.return_type=="void"){
            error_cnt++;
            fprintf(errorfile , "Error at line: %d Void function used in expression\n\n",line);
          }
          else if(f.return_type != (yyvsp[-2].symbol)->getVariableType()){
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
			(yyval.symbol)->set_code((yyvsp[0].symbol)->get_code()+(yyvsp[-2].symbol)->get_code());
			(yyval.symbol)->add_code("\n\tMOV AX, "+(yyvsp[0].symbol)->get_assembly_value()+"\n");

      string temp = modified_name((yyvsp[-2].symbol)->get_name())+table.get_current_id()[0];
      (yyval.symbol)->set_name(temp);
			(yyval.symbol)->set_assembly_value(temp);

      //if variable
      if((yyvsp[-2].symbol)->getIdentity()!="array"){
				(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");
			}
			//or array
			else{
        int idx=get_index((yyvsp[-2].symbol)->get_name());
				if(idx==0)(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");
        else (yyval.symbol)->add_code("\tMOV "+temp+"+"+to_string(idx)+"*2, AX\n");
			}
    }
#line 2853 "y.tab.c"
    break;

  case 48: /* logic_expression: rel_expression  */
#line 1388 "1705098.y"
    {
      (yyval.symbol) = (yyvsp[0].symbol);
      (yyval.symbol)->setVariableType("int");
      fprintf(logfile,"Line %d: logic_expression : rel_expression\n\n",line);
      fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
    }
#line 2864 "y.tab.c"
    break;

  case 49: /* logic_expression: rel_expression LOGICOP rel_expression  */
#line 1395 "1705098.y"
    {
      (yyval.symbol) = (yyvsp[-2].symbol);
      fprintf(logfile,"At line no: %d logic_expression : rel_expression LOGICOP rel_expression\n\n",line);
      fprintf(logfile,"%s%s%s\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

      /*semantic
      both $1 and $3 must be of type non void
      $$ must be set to type int
      */
      if((yyvsp[-2].symbol)->getVariableType()=="void" || (yyvsp[0].symbol)->getVariableType()=="void"){
 				error_cnt++;
 				fprintf(errorfile,"Error at line %d found: Type Mismatch(Operands of %s can't be void)\n\n",line,(yyvsp[-1].symbol)->get_name().c_str());
 			}

       (yyval.symbol)->setVariableType("int");

       //assembly
			(yyval.symbol)->add_code((yyvsp[0].symbol)->get_code());
			string temp=newTemp();
			string l1=newLabel();
			string l2=newLabel();

      //cout<<$1->get_assembly_value()<<endl;
			(yyval.symbol)->add_code("\n\tMOV AX, "+(yyvsp[-2].symbol)->get_assembly_value()+"\n");
			(yyval.symbol)->add_code("\tMOV BX, "+(yyvsp[0].symbol)->get_assembly_value()+"\n");

			if((yyvsp[-1].symbol)->get_name()=="&&"){
				(yyval.symbol)->add_code("\tCMP AX, 1\n");
				(yyval.symbol)->add_code("\tJNE "+l1+"\n");

				(yyval.symbol)->add_code("\tCMP BX, 1\n");
				(yyval.symbol)->add_code("\tJNE "+l1+"\n");

				(yyval.symbol)->add_code("\tMOV AX, 1\n");
				(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");
				(yyval.symbol)->add_code("\tJMP "+l2+"\n");

				(yyval.symbol)->add_code("\n\t"+l1+":\n");
				(yyval.symbol)->add_code("\tMOV AX, 0\n");
				(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");

				(yyval.symbol)->add_code("\n\t"+l2+":\n");
			}

			else if((yyvsp[-1].symbol)->get_name()=="||"){
				(yyval.symbol)->add_code("\tCMP AX, 1\n");
				(yyval.symbol)->add_code("\tJE "+l1+"\n");

				(yyval.symbol)->add_code("\tCMP BX, 1\n");
				(yyval.symbol)->add_code("\tJE "+l1+"\n");

				(yyval.symbol)->add_code("\tMOV AX, 0\n");
				(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");
				(yyval.symbol)->add_code("\tJMP "+l2+"\n");

				(yyval.symbol)->add_code("\n\t"+l1+":\n");
				(yyval.symbol)->add_code("\tMOV AX, 1\n");
				(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");

				(yyval.symbol)->add_code("\n\t"+l2+":\n");
			}
      (yyval.symbol)->set_name(temp);
			(yyval.symbol)->set_assembly_value(temp);
    }
#line 2933 "y.tab.c"
    break;

  case 50: /* rel_expression: simple_expression  */
#line 1462 "1705098.y"
   {
     (yyval.symbol) = (yyvsp[0].symbol);
     fprintf(logfile,"Line %d: rel_expression	: simple_expression\n\n",line);
     fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
     (yyval.symbol)->setVariableType("int");
     //cout<<$$->get_assembly_value()<<endl;
   }
#line 2945 "y.tab.c"
    break;

  case 51: /* rel_expression: simple_expression RELOP simple_expression  */
#line 1470 "1705098.y"
   {
     (yyval.symbol) = (yyvsp[-2].symbol);
     fprintf(logfile,"Line %d: rel_expression : simple_expression RELOP simple_expression\n\n",line);
     fprintf(logfile,"%s%s%s\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

     /*semantic
     both $1 and $3 must be of type non void
     $$ must be set to type int
     */
     if((yyvsp[-2].symbol)->getVariableType()=="void" || (yyvsp[0].symbol)->getVariableType()=="void"){
				error_cnt++;
				fprintf(errorfile,"Error at line %d found: Type Mismatch(Operands of %s can't be void)\n\n",line,(yyvsp[-1].symbol)->get_name().c_str());
			}
      (yyval.symbol)->setVariableType("int");


			(yyval.symbol)->add_code((yyvsp[0].symbol)->get_code());

			(yyval.symbol)->add_code("\n\tMOV AX, "+(yyvsp[-2].symbol)->get_assembly_value()+"\n");
			(yyval.symbol)->add_code("\tCMP AX, "+(yyvsp[0].symbol)->get_assembly_value()+"\n");

			string temp=newTemp();
			string l1=newLabel();
			string l2=newLabel();

			if((yyvsp[-1].symbol)->get_name()=="<"){
				(yyval.symbol)->add_code("\tJL "+l1+"\n");
			}
      else if((yyvsp[-1].symbol)->get_name()==">"){
        (yyval.symbol)->add_code("\tJG "+l1+"\n");
      }
      else if((yyvsp[-1].symbol)->get_name()=="=="){
        (yyval.symbol)->add_code("\tJE "+l1+"\n");
      }
			else if((yyvsp[-1].symbol)->get_name()=="<="){
				(yyval.symbol)->add_code("\tJLE "+l1+"\n");
			}
			else if((yyvsp[-1].symbol)->get_name()==">="){
				(yyval.symbol)->add_code("\tJGE "+l1+"\n");
			}
			else{
				(yyval.symbol)->add_code("\tJNE "+l1+"\n");
			}

			(yyval.symbol)->add_code("\n\tMOV "+temp+", 0\n");
			(yyval.symbol)->add_code("\tJMP "+l2+"\n");

			(yyval.symbol)->add_code("\n\t"+l1+":\n\tMOV "+temp+", 1\n");
			(yyval.symbol)->add_code("\n\t"+l2+":\n");

			(yyval.symbol)->set_name(temp);
			(yyval.symbol)->set_assembly_value(temp);
   }
#line 3003 "y.tab.c"
    break;

  case 52: /* simple_expression: term  */
#line 1526 "1705098.y"
  {
    (yyval.symbol) = (yyvsp[0].symbol);
    fprintf(logfile,"Line %d: simple_expression : term\n\n",line);
    fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

  }
#line 3014 "y.tab.c"
    break;

  case 53: /* simple_expression: simple_expression ADDOP term  */
#line 1533 "1705098.y"
  {
    (yyval.symbol) = (yyvsp[-2].symbol);
    fprintf(logfile,"Line %d: simple_expression : simple_expression ADDOP term\n\n",line);
    fprintf(logfile,"%s%s%s\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

    if((yyvsp[-2].symbol)->getVariableType()=="float" || (yyvsp[0].symbol)->getVariableType()=="float")
				(yyval.symbol)->setVariableType("float");
		else
				(yyval.symbol)->setVariableType("int");

  			(yyval.symbol)->add_code((yyvsp[0].symbol)->get_code());

  			string temp=newTemp();
  			if((yyvsp[-1].symbol)->get_name()=="+"){
  				(yyval.symbol)->add_code("\n\tMOV AX, "+(yyvsp[-2].symbol)->get_assembly_value()+"\n");
  				(yyval.symbol)->add_code("\tADD AX, "+(yyvsp[0].symbol)->get_assembly_value()+"\n");
  				(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");
  			}

  			else{
  				(yyval.symbol)->add_code("\n\tMOV AX, "+(yyvsp[-2].symbol)->get_assembly_value()+"\n");
  				(yyval.symbol)->add_code("\tSUB AX, "+(yyvsp[0].symbol)->get_assembly_value()+"\n");
  				(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");
  			}

  			(yyval.symbol)->set_name(temp);
  			(yyval.symbol)->set_assembly_value(temp);

  }
#line 3048 "y.tab.c"
    break;

  case 54: /* term: unary_expression  */
#line 1565 "1705098.y"
    {
      (yyval.symbol) = (yyvsp[0].symbol);
      fprintf(logfile,"Line %d: term :	unary_expression\n\n",line);
      fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
    }
#line 3058 "y.tab.c"
    break;

  case 55: /* term: term MULOP unary_expression  */
#line 1571 "1705098.y"
    {
      (yyval.symbol)=(yyvsp[-2].symbol);
      fprintf(logfile,"Line %d: term : term MULOP unary_expression\n\n",line);
      fprintf(logfile,"%s%s%s\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

			//assembly
			(yyval.symbol)->add_code((yyvsp[0].symbol)->get_code());
			(yyval.symbol)->add_code("\n\tMOV AX, "+ (yyvsp[-2].symbol)->get_assembly_value()+"\n");
			(yyval.symbol)->add_code("\tMOV BX, "+ (yyvsp[0].symbol)->get_assembly_value()+"\n");

			string temp=newTemp();
      //perform multiplication
			if((yyvsp[-1].symbol)->get_name()=="*"){
				(yyval.symbol)->add_code("\tMUL BX\n");
				(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");
			}
      //perform division
			else if((yyvsp[-1].symbol)->get_name()=="/"){
				(yyval.symbol)->add_code("\tXOR DX, DX\n");
				(yyval.symbol)->add_code("\tDIV BX\n");
				(yyval.symbol)->add_code("\tMOV "+temp+" , AX\n");
			}
      //perform mod operation
			else{
				(yyval.symbol)->add_code("\tXOR DX, DX\n");
				(yyval.symbol)->add_code( "\tDIV BX\n");
				(yyval.symbol)->add_code("\tMOV "+temp+" , DX\n");
			}

		 	(yyval.symbol)->set_name(temp);
			(yyval.symbol)->set_assembly_value(temp);

      //semantic
      //if $3 is void type function
      string fn = modified_name_while_func_calling((yyvsp[0].symbol)->get_name());
      if(is_in_func_list(fn)){
        function_ f = get_func(fn);
        if(f.return_type=="void"){
          error_cnt++;
          fprintf(errorfile , "Error at line %d: Void function used in expression\n\n",line);
          fprintf(logfile , "Error at line %d: Void function used in expression\n\n",line);
        }
      }
      //features of mod operation
      if((yyvsp[-1].symbol)->get_name()=="%" && ((yyvsp[-2].symbol)->getVariableType()!="int" || (yyvsp[0].symbol)->getVariableType()!="int")){
				error_cnt++;
				fprintf(errorfile,"Error at line %d: Non-Integer operand on modulus operator\n\n",line);
        fprintf(logfile,"Error at line %d: Non-Integer operand on modulus operator\n\n",line);
			}
      //mod by zero
      else if((yyvsp[-1].symbol)->get_name()=="%" && (yyvsp[0].symbol)->get_name()=="0"){
				error_cnt++;
				fprintf(errorfile,"Error at line %d: Modulus by Zero\n\n",line);
        fprintf(logfile,"Error at line %d: Modulus by Zero\n\n",line);
			}
			//set var_type
			if((yyvsp[-1].symbol)->get_name()=="%")
				(yyval.symbol)->setVariableType("int");
			else
			{
				if((yyvsp[-2].symbol)->getVariableType()=="float" || (yyvsp[0].symbol)->getVariableType()=="float")
					(yyval.symbol)->setVariableType("float");
				else
					(yyval.symbol)->setVariableType("int");
			}

    }
#line 3130 "y.tab.c"
    break;

  case 56: /* unary_expression: ADDOP unary_expression  */
#line 1641 "1705098.y"
    {
      //if $3 is void type function
      string fn = modified_name_while_func_calling((yyvsp[0].symbol)->get_name());
      if(is_in_func_list(fn)){
        function_ f = get_func(fn);
        if(f.return_type=="void"){
          error_cnt++;
          fprintf(errorfile , "Error at line %d: Void function used in expression\n\n",line);
          fprintf(logfile , "Error at line %d: Void function used in expression\n\n",line);
        }
      }

      fprintf(logfile,"At line no: %d unary_expression : ADDOP unary_expression\n",line);
		  fprintf(logfile,"%s%s\n\n",(yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

			(yyval.symbol)=(yyvsp[0].symbol);

      (yyval.symbol)->setVariableType((yyvsp[0].symbol)->getVariableType());
      (yyval.symbol)->setIdentity((yyvsp[0].symbol)->getIdentity());

      //assembly
			string temp=newTemp();
			if((yyvsp[-1].symbol)->get_name()=="-"){
				(yyval.symbol)->add_code("\n\tMOV AX, "+(yyvsp[0].symbol)->get_assembly_value()+"\n");
				(yyval.symbol)->add_code("\tNEG AX\n");
				(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");
			}

			else{
				(yyval.symbol)->add_code("\n\tMOV AX, "+(yyvsp[0].symbol)->get_assembly_value()+"\n");
				(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");
			}
			(yyval.symbol)->set_name(temp);
			(yyval.symbol)->set_assembly_value(temp);
    }
#line 3170 "y.tab.c"
    break;

  case 57: /* unary_expression: NOT unary_expression  */
#line 1677 "1705098.y"
    {
      (yyval.symbol)=(yyvsp[0].symbol);
			string temp=newTemp();

			(yyval.symbol)->add_code("\n\tMOV AX, "+(yyvsp[0].symbol)->get_assembly_value()+"\n");
			(yyval.symbol)->add_code("\tNOT AX\n");
			(yyval.symbol)->add_code("\tMOV "+temp+", AX\n");

      (yyval.symbol)->setVariableType((yyvsp[0].symbol)->getVariableType());
      (yyval.symbol)->setIdentity((yyvsp[0].symbol)->getIdentity());
			(yyval.symbol)->set_name(temp);
			(yyval.symbol)->set_assembly_value(temp);
    }
#line 3188 "y.tab.c"
    break;

  case 58: /* unary_expression: factor  */
#line 1691 "1705098.y"
     {
       (yyval.symbol) = (yyvsp[0].symbol);
       fprintf(logfile,"Line %d: unary_expression :	factor\n\n",line);
       fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
     }
#line 3198 "y.tab.c"
    break;

  case 59: /* factor: variable  */
#line 1699 "1705098.y"
    {
      (yyval.symbol) = (yyvsp[0].symbol);
      fprintf(logfile,"Line %d: factor : variable\n\n",line);
      fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

      if((yyvsp[0].symbol)->getIdentity()=="array"){
        int idx = get_index((yyval.symbol)->get_name());
        if(idx==0)(yyval.symbol)->set_assembly_value(modified_name((yyval.symbol)->get_name()) +table.get_current_id()[0]);
        else (yyval.symbol)->set_assembly_value(modified_name((yyval.symbol)->get_name()) +table.get_current_id()[0]+"+"+to_string(idx)+"*2");
      }
      else{
        //cout<<modified_name($$->get_name()) +table.get_current_id()[0]<<endl;
        (yyval.symbol)->set_assembly_value (modified_name((yyval.symbol)->get_name()) +table.get_current_id()[0]);
      }

      ///pass arrayname if array otherwise pass varname only
      ///suppose $1->get_name() is a[2].Now modified_name returns only a
      string varname;
      varname = modified_name((yyvsp[0].symbol)->get_name());
      SymbolInfo *x=table.Lookup(varname);
			if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,varname.c_str());
      }
      else{

          //setting type of var(int/float) and identity(array/normal variable)
  				(yyval.symbol)->setVariableType(x->getVariableType());
          (yyval.symbol)->setIdentity(x->getIdentity());
          //chk if array
          for(int i=0;i<var_list.size();i++){
            if(var_list[i].var_name==x->get_name() && var_list[i].var_size>0){
              //now we're sure that it's an array
              //let's see if ara is being used without any index
              if(varname==(yyvsp[0].symbol)->get_name()){
                error_cnt++;
                fprintf(errorfile,"Error at line: %d %s is an array\n\n",line,varname.c_str());
                fprintf(logfile,"Error at line: %d %s is an array\n\n",line,varname.c_str());
                (yyval.symbol)->set_already_error_in_param();
                break;
              }
              //now chk if wrong index is given
              else if(!is_ara_idx_valid((yyvsp[0].symbol)->get_name() , var_list[i].var_size)){
                error_cnt++;
                fprintf(errorfile,"Error at line: %d Wrong array index\n\n",line);
                break;
              }
            }
          }
      }

    }
#line 3255 "y.tab.c"
    break;

  case 60: /* factor: ID LPAREN argument_list RPAREN  */
#line 1752 "1705098.y"
    {
      (yyval.symbol) = new SymbolInfo((yyvsp[-3].symbol)->get_name() + "(" + (yyvsp[-1].symbol)->get_name() + ")" , "factor");
      (yyval.symbol)->set_is_func(true);

      fprintf(logfile,"Line %d: factor : ID LPAREN argument_list RPAREN\n\n",line);
      fprintf(logfile,"%s(%s)\n\n",(yyvsp[-3].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str());

      //semantic
      //chk if id is in func_list
      if(!is_in_func_list((yyvsp[-3].symbol)->get_name()) and (yyvsp[-3].symbol)->get_name()!="print_ln"){
        error_cnt++;
        fprintf(errorfile , "Error at line: %d Undeclared Function %s\n\n",line,(yyvsp[-3].symbol)->get_name().c_str());
      }
      else{

        function_ f = get_func((yyvsp[-3].symbol)->get_name());

        //chk args consistency
        bool matched = true;
        bool already_error_in_arg = false;
        if(f.params.size() != (yyvsp[-1].symbol)->arg_list.size())matched = false;
        else{
          for(int i=0;i<f.params.size();i++){
            if((yyvsp[-1].symbol)->arg_list[i].already_error_in_arg){
              already_error_in_arg = true;
            }
            if((yyvsp[-1].symbol)->arg_list[i].sz>0){
              if((yyvsp[-1].symbol)->get_name()==modified_name((yyvsp[-1].symbol)->get_name())){
                matched = false;
                //break;
              }
            }
            if(f.params[i].first != (yyvsp[-1].symbol)->arg_list[i].type){
              matched = false;
              //break;
            }
            //will have to see maybe
            if(matched){
              (yyval.symbol)->add_code("\n\tMOV AX, " + (yyvsp[-1].symbol)->arg_list[i].name + table.get_current_id()[0]+"\n");
							(yyval.symbol)->add_code("\tMOV "+f.params[i].second +table.get_current_id()[0]+", AX\n");
            }
          }
        }

        if(!matched ){
          error_cnt++;
          fprintf(errorfile , "Error at line: %d Total number of arguments mismatch in function %s\n\n",line,(yyvsp[-3].symbol)->get_name().c_str());
          fprintf(logfile , "Error at line: %d Total number of arguments mismatch in function %s\n\n",line,(yyvsp[-3].symbol)->get_name().c_str());
        }
        else{
          (yyval.symbol)->add_code("\tCALL "+(yyvsp[-3].symbol)->get_name()+"\n");

          string fnm = modified_name_while_func_calling((yyvsp[-3].symbol)->get_name());
          function_ f = get_func(fnm);
          (yyval.symbol)->set_assembly_value("T" + to_string(f.return_reg_no));

        }
      }

    }
#line 3320 "y.tab.c"
    break;

  case 61: /* factor: LPAREN expression RPAREN  */
#line 1813 "1705098.y"
    {
      (yyval.symbol) =(yyvsp[-1].symbol);
      (yyval.symbol)->set_assembly_value((yyval.symbol)->get_name());
      (yyval.symbol)->setVariableType((yyvsp[-1].symbol)->getVariableType());

      fprintf(logfile,"Line %d: factor : LPAREN expression RPAREN\n\n",line);
      fprintf(logfile,"(%s)\n\n",(yyvsp[-1].symbol)->get_name().c_str());

    }
#line 3334 "y.tab.c"
    break;

  case 62: /* factor: CONST_INT  */
#line 1823 "1705098.y"
    {
      (yyval.symbol)=(yyvsp[0].symbol);
			(yyval.symbol)->set_assembly_value((yyval.symbol)->get_name());
			(yyval.symbol)->setVariableType("int");

      fprintf(logfile,"Line %d: factor : CONST_INT\n\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

    }
#line 3348 "y.tab.c"
    break;

  case 63: /* factor: CONST_FLOAT  */
#line 1833 "1705098.y"
    {
      (yyval.symbol)=(yyvsp[0].symbol);
      (yyval.symbol)->set_assembly_value((yyval.symbol)->get_name());
			(yyval.symbol)->setVariableType("float");

      fprintf(logfile,"Line %d: factor : CONST_FLOAT\n\n",line);
      fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

    }
#line 3362 "y.tab.c"
    break;

  case 64: /* factor: variable INCOP  */
#line 1843 "1705098.y"
    {
      (yyval.symbol)=new SymbolInfo((yyvsp[-1].symbol)->get_name(),(yyvsp[-1].symbol)->get_type());

      SymbolInfo *x=table.Lookup(modified_name((yyvsp[-1].symbol)->get_name()));
			if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,(yyvsp[-1].symbol)->get_name().c_str());
      }
      else{
         //assembly
				string var_name=modified_name((yyvsp[-1].symbol)->get_name())+table.get_current_id()[0];

				(yyval.symbol)->set_name(var_name);

				//array
				if((yyvsp[-1].symbol)->getIdentity()=="array"){
          int idx = get_index((yyvsp[-1].symbol)->get_name());
					if(idx==0)(yyval.symbol)->add_code("\tMOV AX, "+var_name + "\n");
          else (yyval.symbol)->add_code("\tMOV AX, "+var_name+"+"+to_string(idx)+"*2\n");

          (yyval.symbol)->add_code("\tINC AX\n");

					if(idx==0)(yyval.symbol)->add_code("\tMOV "+var_name + ", AX\n");
          else (yyval.symbol)->add_code("\tMOV "+var_name+"+"+to_string(idx)+"*2, AX\n");
				}

				else{
					(yyval.symbol)->add_code("\tMOV AX, "+var_name+"\n");
					(yyval.symbol)->add_code("\tINC AX\n");
					(yyval.symbol)->add_code("\tMOV "+var_name+", AX\n");
				}

         (yyval.symbol)->setVariableType((yyvsp[-1].symbol)->getVariableType());
         (yyval.symbol)->setIdentity((yyvsp[-1].symbol)->getIdentity());
      }

    }
#line 3404 "y.tab.c"
    break;

  case 65: /* factor: variable DECOP  */
#line 1881 "1705098.y"
    {
      (yyval.symbol) = new SymbolInfo((yyvsp[-1].symbol)->get_name(),(yyvsp[-1].symbol)->get_type());

      SymbolInfo *x=table.Lookup(modified_name((yyvsp[-1].symbol)->get_name()));
			if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,(yyvsp[-1].symbol)->get_name().c_str());
      }
      else{
         //assembly
				string var_name=modified_name((yyvsp[-1].symbol)->get_name())+table.get_current_id()[0];
				string temp_str=newTemp();

				(yyval.symbol)->set_name(var_name);

				//array
				if((yyvsp[-1].symbol)->getIdentity()=="array"){

					(yyval.symbol)->add_code("\tMOV AX, "+var_name+"+"+to_string((yyvsp[-1].symbol)->idx)+"*2\n");
					(yyval.symbol)->add_code("\tMOV "+temp_str+", AX\n");
					(yyval.symbol)->add_code("\tDEC AX\n");
					(yyval.symbol)->add_code("\tMOV "+var_name+"+"+to_string((yyvsp[-1].symbol)->idx)+"*2, AX\n");
				}

				else{
					(yyval.symbol)->add_code("\tMOV AX, "+var_name+"\n");
					(yyval.symbol)->add_code("\tMOV "+temp_str+", AX\n");
					(yyval.symbol)->add_code("\tDEC AX\n");
					(yyval.symbol)->add_code("\tMOV "+var_name+", AX\n");
				}
				(yyval.symbol)->set_name(temp_str);

        (yyval.symbol)->setVariableType((yyvsp[-1].symbol)->getVariableType());
        (yyval.symbol)->setIdentity((yyvsp[-1].symbol)->getIdentity());
      }
    }
#line 3445 "y.tab.c"
    break;

  case 66: /* argument_list: arguments  */
#line 1920 "1705098.y"
        {
    			(yyval.symbol)=(yyvsp[0].symbol);
          fprintf(logfile,"Line %d: argument_list : arguments\n\n",line);
         fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
        }
#line 3455 "y.tab.c"
    break;

  case 67: /* argument_list: %empty  */
#line 1926 "1705098.y"
        {
          (yyval.symbol) = new SymbolInfo("" , "argument_list");
        }
#line 3463 "y.tab.c"
    break;

  case 68: /* arguments: arguments COMMA logic_expression  */
#line 1932 "1705098.y"
        {
          (yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+" , "+(yyvsp[0].symbol)->get_name() , "arguments");
          fprintf(logfile,"Line %d: arguments : arguments COMMA logic_expression\n\n",line);
          fprintf(logfile,"%s , %s\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

          (yyval.symbol)->arg_list = (yyvsp[-2].symbol)->arg_list;
          bool isara=false;
          for(int i=0;i<var_list.size();i++){
            if((yyvsp[0].symbol)->get_name()==var_list[i].var_name && var_list[i].var_size>0){
                isara = true;
                if((yyvsp[0].symbol)->get_already_error_in_param()){
                  (yyval.symbol)->push_in_arg_AR((yyvsp[0].symbol)->get_name() , (yyvsp[0].symbol)->getVariableType() , var_list[i].var_size);break;
                }
                else (yyval.symbol)->push_in_arg((yyvsp[0].symbol)->get_name() , (yyvsp[0].symbol)->getVariableType() , var_list[i].var_size);break;
            }
          }
          if(!isara){
            (yyval.symbol)->push_in_arg((yyvsp[0].symbol)->get_name() , (yyvsp[0].symbol)->getVariableType() , 0);
          }

        }
#line 3489 "y.tab.c"
    break;

  case 69: /* arguments: logic_expression  */
#line 1954 "1705098.y"
        {
          fprintf(logfile,"At line no: %d arguments : logic_expression\n\n",line);
    			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
    			(yyval.symbol)=(yyvsp[0].symbol);
          bool isara=false;
          for(int i=0;i<var_list.size();i++){
            if((yyvsp[0].symbol)->get_name()==var_list[i].var_name && var_list[i].var_size>0){
              isara = true;
              if((yyvsp[0].symbol)->get_already_error_in_param()){
                (yyval.symbol)->push_in_arg_AR((yyvsp[0].symbol)->get_name() , (yyvsp[0].symbol)->getVariableType() , var_list[i].var_size);break;
              }
              else (yyval.symbol)->push_in_arg((yyvsp[0].symbol)->get_name() , (yyvsp[0].symbol)->getVariableType() , var_list[i].var_size);break;
            }
          }
          if(!isara){
            (yyval.symbol)->push_in_arg((yyvsp[0].symbol)->get_name() , (yyvsp[0].symbol)->getVariableType() , 0);
          }
        }
#line 3512 "y.tab.c"
    break;


#line 3516 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1976 "1705098.y"

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
