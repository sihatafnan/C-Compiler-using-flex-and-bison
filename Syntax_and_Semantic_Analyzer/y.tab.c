/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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



#line 170 "y.tab.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

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

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IF = 258,
    ELSE = 259,
    FOR = 260,
    WHILE = 261,
    DO = 262,
    BREAK = 263,
    INT = 264,
    CHAR = 265,
    FLOAT = 266,
    DOUBLE = 267,
    VOID = 268,
    RETURN = 269,
    SWITCH = 270,
    CASE = 271,
    DEFAULT = 272,
    CONTINUE = 273,
    ASSIGNOP = 274,
    INCOP = 275,
    DECOP = 276,
    NOT = 277,
    LPAREN = 278,
    RPAREN = 279,
    LCURL = 280,
    RCURL = 281,
    LTHIRD = 282,
    RTHIRD = 283,
    COMMA = 284,
    SEMICOLON = 285,
    PRINTLN = 286,
    CONST_INT = 287,
    CONST_FLOAT = 288,
    CONST_CHAR = 289,
    ID = 290,
    ADDOP = 291,
    MULOP = 292,
    RELOP = 293,
    LOGICOP = 294,
    LOWER_THAN_ELSE = 295
  };
#endif
/* Tokens.  */
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
#line 101 "parser.y"

     SymbolInfo *symbol;

#line 306 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYLAST   164

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  28
/* YYNRULES -- Number of rules.  */
#define YYNRULES  68
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  122

#define YYUNDEFTOK  2
#define YYMAXUTOK   295


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

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
       0,   124,   124,   130,   137,   146,   152,   158,   166,   185,
     208,   235,   207,   244,   259,   243,   270,   281,   291,   301,
     314,   321,   330,   374,   383,   392,   403,   419,   440,   455,
     475,   482,   491,   498,   505,   512,   519,   527,   535,   542,
     548,   561,   567,   578,   599,   612,   618,   686,   693,   712,
     719,   737,   744,   758,   764,   790,   801,   812,   821,   861,
     872,   881,   889,   897,   915,   934,   941,   946,   956
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IF", "ELSE", "FOR", "WHILE", "DO",
  "BREAK", "INT", "CHAR", "FLOAT", "DOUBLE", "VOID", "RETURN", "SWITCH",
  "CASE", "DEFAULT", "CONTINUE", "ASSIGNOP", "INCOP", "DECOP", "NOT",
  "LPAREN", "RPAREN", "LCURL", "RCURL", "LTHIRD", "RTHIRD", "COMMA",
  "SEMICOLON", "PRINTLN", "CONST_INT", "CONST_FLOAT", "CONST_CHAR", "ID",
  "ADDOP", "MULOP", "RELOP", "LOGICOP", "LOWER_THAN_ELSE", "$accept",
  "start", "program", "unit", "func_declaration", "func_definition", "$@1",
  "$@2", "$@3", "$@4", "parameter_list", "compound_statement",
  "var_declaration", "type_specifier", "declaration_list", "statements",
  "statement", "expression_statement", "variable", "expression",
  "logic_expression", "rel_expression", "simple_expression", "term",
  "unary_expression", "factor", "argument_list", "arguments", YY_NULLPTR
};
#endif

# ifdef YYPRINT
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
# endif

#define YYPACT_NINF (-72)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      40,   -72,   -72,   -72,    19,    40,   -72,   -72,   -72,   -72,
     -25,   -72,   -72,     7,    37,     4,    -5,    -3,   -72,    20,
      -6,    28,    11,    31,   -72,    30,    39,    40,   -72,   -72,
      43,    51,   -72,   -72,    30,    45,    57,    66,    72,    74,
     104,   104,   104,   -72,   -72,    78,   -72,   -72,    10,   104,
     -72,   -72,    69,    85,   -72,   -72,    23,    79,   -72,    73,
     -16,    76,   -72,   -72,   -72,   -72,   -72,   -72,   104,    70,
     104,    84,    58,   -72,    95,    88,   104,   104,   -72,   102,
     -72,   -72,   104,   -72,   -72,   -72,   104,   104,   104,   104,
     -72,   107,    70,   110,   -72,   -72,   111,   -72,   114,   116,
     115,   -72,   -72,    76,   112,   -72,   119,   104,   119,   117,
     -72,   104,   -72,   142,   129,   -72,   -72,   -72,   119,   119,
     -72,   -72
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
       0,     0,     0,    21,    41,     0,    61,    62,    43,     0,
      34,    32,     0,     0,    30,    33,    58,     0,    45,    47,
      49,    51,    53,    57,    15,    11,    16,    27,     0,     0,
       0,     0,    58,    56,     0,     0,    66,     0,    55,    28,
      20,    31,     0,    63,    64,    42,     0,     0,     0,     0,
      12,     0,     0,     0,    40,    60,     0,    68,     0,    65,
       0,    46,    48,    52,    50,    54,     0,     0,     0,     0,
      59,     0,    44,    36,     0,    38,    39,    67,     0,     0,
      37,    35
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -72,   -72,   -72,   151,   -72,   -72,   -72,   -72,   -72,   -72,
     -72,   -18,    21,     9,   -72,   -72,   -47,   -67,   -41,   -39,
     -71,    71,    75,    77,   -37,   -72,   -72,   -72
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,     7,     8,    34,    90,    25,    64,
      20,    50,    51,    52,    14,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    98,    99
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      72,    71,    92,    74,    73,    97,    81,    32,    72,    10,
      13,   101,    78,     1,    10,     2,    65,     3,    26,    11,
      87,     9,    88,    27,    21,   107,     9,    22,    19,    91,
      15,    93,    23,    76,    16,    72,    35,    77,   100,    29,
     117,    72,    82,    83,    84,    72,    72,    72,    72,     1,
      24,     2,   105,     3,    37,    31,    38,    39,    30,   113,
       1,   115,     2,    28,     3,    40,    17,    18,   114,    33,
      72,   120,   121,    41,    42,    36,    31,    43,    83,    84,
      66,    44,    45,    46,    47,    67,    48,    49,    37,    68,
      38,    39,    41,    42,     1,    69,     2,    70,     3,    40,
      44,    75,    46,    47,    79,    48,    49,    41,    42,    85,
      31,    80,    86,    89,    94,    44,    45,    46,    47,    95,
      48,    49,    37,    96,    38,    39,    41,    42,     1,    16,
       2,   106,     3,    40,   108,   109,    46,    47,   110,    48,
      49,    41,    42,   112,    31,   111,   118,   116,    87,    44,
      45,    46,    47,   119,    48,    49,    12,   102,     0,     0,
       0,     0,     0,   104,   103
};

static const yytype_int8 yycheck[] =
{
      41,    40,    69,    42,    41,    76,    53,    25,    49,     0,
      35,    82,    49,     9,     5,    11,    34,    13,    24,     0,
      36,     0,    38,    29,    15,    92,     5,    32,    24,    68,
      23,    70,    35,    23,    27,    76,    27,    27,    77,    28,
     111,    82,    19,    20,    21,    86,    87,    88,    89,     9,
      30,    11,    89,    13,     3,    25,     5,     6,    27,   106,
       9,   108,    11,    35,    13,    14,    29,    30,   107,    30,
     111,   118,   119,    22,    23,    32,    25,    26,    20,    21,
      35,    30,    31,    32,    33,    28,    35,    36,     3,    23,
       5,     6,    22,    23,     9,    23,    11,    23,    13,    14,
      30,    23,    32,    33,    35,    35,    36,    22,    23,    30,
      25,    26,    39,    37,    30,    30,    31,    32,    33,    24,
      35,    36,     3,    35,     5,     6,    22,    23,     9,    27,
      11,    24,    13,    14,    24,    24,    32,    33,    24,    35,
      36,    22,    23,    28,    25,    29,     4,    30,    36,    30,
      31,    32,    33,    24,    35,    36,     5,    86,    -1,    -1,
      -1,    -1,    -1,    88,    87
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
      26,    57,    19,    20,    21,    30,    39,    36,    38,    37,
      48,    60,    58,    60,    30,    24,    35,    61,    67,    68,
      60,    61,    62,    64,    63,    65,    24,    58,    24,    24,
      24,    29,    28,    57,    60,    57,    30,    61,     4,    24,
      57,    57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    41,    42,    43,    43,    44,    44,    44,    45,    45,
      47,    48,    46,    49,    50,    46,    51,    51,    51,    51,
      52,    52,    53,    54,    54,    54,    55,    55,    55,    55,
      56,    56,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    58,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    63,    64,    64,    65,    65,    65,    66,    66,
      66,    66,    66,    66,    66,    67,    67,    68,    68
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     6,     5,
       0,     0,     8,     0,     0,     7,     4,     3,     2,     1,
       3,     2,     3,     1,     1,     1,     3,     6,     1,     4,
       1,     2,     1,     1,     1,     7,     5,     7,     5,     5,
       3,     1,     2,     1,     4,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     2,     2,     1,     1,     4,
       3,     1,     1,     2,     2,     1,     0,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
  case 2:
#line 125 "parser.y"
        {
		//write your code in this block in all the similar blocks below
	}
#line 1578 "y.tab.c"
    break;

  case 3:
#line 131 "parser.y"
        {
		fprintf(logfile,"At line no: %d  program : program unit\n\n",line);
		SymbolInfo *x = new SymbolInfo((string)(yyvsp[-1].symbol)->get_name()+(string)(yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
		(yyval.symbol) = x;
		fprintf(logfile , "%s%s\n\n" , (yyvsp[-1].symbol)->get_name().c_str() , (yyvsp[0].symbol)->get_name().c_str());
	}
#line 1589 "y.tab.c"
    break;

  case 4:
#line 138 "parser.y"
        {
		fprintf(logfile,"At line no: %d: program : unit\n\n",line);
		fprintf(logfile,"%s\n",(yyvsp[0].symbol)->get_name().c_str());
		(yyval.symbol) = new SymbolInfo((yyvsp[0].symbol)->get_name()+"\n", "NON_TERMINAL");

	}
#line 1600 "y.tab.c"
    break;

  case 5:
#line 147 "parser.y"
        {
		fprintf(logfile,"At line no: %d: unit : var_declation\n\n",line);
		fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
		(yyval.symbol) = new SymbolInfo((yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
	}
#line 1610 "y.tab.c"
    break;

  case 6:
#line 153 "parser.y"
        {
		fprintf(logfile,"At line no: %d: unit : func_declaration\n\n",line);
		fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
		(yyval.symbol) = new SymbolInfo((yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
	}
#line 1620 "y.tab.c"
    break;

  case 7:
#line 159 "parser.y"
        {
    fprintf(logfile,"At line no: %d: unit : func_definition\n\n",line);
		fprintf(logfile,"%s\n",(yyvsp[0].symbol)->get_name().c_str());
		(yyval.symbol) = new SymbolInfo((yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
	}
#line 1630 "y.tab.c"
    break;

  case 8:
#line 167 "parser.y"
        {
    (yyval.symbol) = new SymbolInfo((yyvsp[-5].symbol)->get_name()+" "+(yyvsp[-4].symbol)->get_name()+"("+(yyvsp[-2].symbol)->get_name()+");", "NON_TERMINAL");
    fprintf(logfile , "At line no %d: func_declaration: type_specifier ID LPAREN parameter_list RPAREN SEMICOLON\n\n",line);
    fprintf(logfile , "%s %s(%s);\n\n" , (yyvsp[-5].symbol)->get_name().c_str() , (yyvsp[-4].symbol)->get_name().c_str(),(yyvsp[-2].symbol)->get_name().c_str());

    /* checking whether already declared or not */
    SymbolInfo* temp = table.Lookup_in_current((yyvsp[-4].symbol)->get_name());
    if(temp != NULL) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , (yyvsp[-4].symbol)->get_name().c_str());
		}
    else{
      table.Insert((yyvsp[-4].symbol)->get_name() , "ID" , logfile);
      SymbolInfo *fd = table.Lookup_in_current((yyvsp[-4].symbol)->get_name());
      fd->set_is_declared_func(true);
    }

	}
#line 1653 "y.tab.c"
    break;

  case 9:
#line 186 "parser.y"
        {
		(yyval.symbol) = new SymbolInfo((yyvsp[-4].symbol)->get_name()+" "+(yyvsp[-3].symbol)->get_name()+"();", "NON_TERMINAL");
		fprintf(logfile , "At line no %d: func_declaration: type_specifier ID LPAREN RPAREN SEMICOLON\n\n",line);
		fprintf(logfile , "%s %s();\n\n" , (yyvsp[-4].symbol)->get_name().c_str() , (yyvsp[-3].symbol)->get_name().c_str());


		/* checking whether already declared or not */
		SymbolInfo* temp = table.Lookup_in_current((yyvsp[-3].symbol)->get_name());
		if(temp != NULL) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple declaration of %s\n\n" , line , (yyvsp[-3].symbol)->get_name().c_str());
		}
    else{
      table.Insert((yyvsp[-3].symbol)->get_name() , "ID" , logfile);
      SymbolInfo *fd = table.Lookup_in_current((yyvsp[-3].symbol)->get_name());
      fd->set_is_declared_func(true);
    }

	}
#line 1677 "y.tab.c"
    break;

  case 10:
#line 208 "parser.y"
  {
    current_return_type = (yyvsp[-4].symbol)->get_name();
    /* checking whether already declared or not */
		SymbolInfo* temp = table.Lookup_in_current((yyvsp[-3].symbol)->get_name());
		if(temp != NULL and (!temp->get_is_declared_func())) {
			error_cnt++;
			fprintf(errorfile , "Error at line %d: Multiple definition of %s\n\n" , line , (yyvsp[-3].symbol)->get_name().c_str());
		}
		else if(temp == NULL){
			table.Insert((yyvsp[-3].symbol)->get_name() , "ID" , logfile);
		}


    table.Enter_Scope(logfile);
    for(int i=0;i<(yyvsp[-1].symbol)->param.size();i++){
        string nm = (yyvsp[-1].symbol)->param[i].param_name;
        string tp = (yyvsp[-1].symbol)->param[i].param_type;
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
#line 1710 "y.tab.c"
    break;

  case 11:
#line 235 "parser.y"
                       {table.printall(logfile); table.Exit_Scope(logfile);var_list.clear();}
#line 1716 "y.tab.c"
    break;

  case 12:
#line 237 "parser.y"
  {
      (yyval.symbol) = new SymbolInfo((yyvsp[-7].symbol)->get_name()+" "+(yyvsp[-6].symbol)->get_name()+"("+(yyvsp[-4].symbol)->get_name()+")"+(yyvsp[-1].symbol)->get_name()+"\n\n", "NON_TERMINAL");
      fprintf(logfile , "At line no %d: func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement\n\n" , line);
      fprintf(logfile , "%s %s(%s)%s\n\n" , (yyvsp[-7].symbol)->get_name().c_str(),(yyvsp[-6].symbol)->get_name().c_str(),(yyvsp[-4].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str());

  }
#line 1727 "y.tab.c"
    break;

  case 13:
#line 244 "parser.y"
    {
      current_return_type = (yyvsp[-3].symbol)->get_name();
      /* checking whether already declared or not */
  		SymbolInfo* temp = table.Lookup_in_current((yyvsp[-2].symbol)->get_name());
      if(temp != NULL and (!temp->get_is_declared_func())) {
  			error_cnt++;
  			fprintf(errorfile , "Error at line %d: Multiple definition of %s\n\n" , line , (yyvsp[-2].symbol)->get_name().c_str());
  		}
  		else if(temp == NULL){
  			table.Insert((yyvsp[-2].symbol)->get_name() , "ID" , logfile);
  		}

      table.Enter_Scope(logfile);

    }
#line 1747 "y.tab.c"
    break;

  case 14:
#line 259 "parser.y"
                       {table.printall(logfile);table.Exit_Scope(logfile);var_list.clear();}
#line 1753 "y.tab.c"
    break;

  case 15:
#line 260 "parser.y"
        {
      (yyval.symbol) = new SymbolInfo((yyvsp[-6].symbol)->get_name()+" "+(yyvsp[-5].symbol)->get_name()+"()"+(yyvsp[-1].symbol)->get_name()+"\n\n", "NON_TERMINAL");
      fprintf(logfile , "At line no %d: func_definition : type_specifier ID LPAREN RPAREN compound_statement\n\n" , line);
      fprintf(logfile , "%s %s()%s\n\n" , (yyvsp[-6].symbol)->get_name().c_str(),(yyvsp[-5].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str());


	}
#line 1765 "y.tab.c"
    break;

  case 16:
#line 271 "parser.y"
                {
			(yyval.symbol) = new SymbolInfo((yyvsp[-3].symbol)->get_name()+","+(yyvsp[-1].symbol)->get_name()+" "+(yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : parameter_list COMMA type_specifier ID\n\n",line);
			fprintf(logfile , "%s , %s %s\n\n" , (yyvsp[-3].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

			/* adding parameter to parameter list */
      (yyval.symbol)->param = (yyvsp[-3].symbol)->param;
			(yyval.symbol)->push_in_param((yyvsp[0].symbol)->get_name() , "ID");

		}
#line 1780 "y.tab.c"
    break;

  case 17:
#line 282 "parser.y"
                {
			(yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+","+(yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : parameter_list COMMA type_specifier\n\n",line);
			fprintf(logfile , "%s , %s\n\n" , (yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

			/* adding parameter to parameter list */
      (yyval.symbol)->param = (yyvsp[-2].symbol)->param;
			(yyval.symbol)->push_in_param("", (yyvsp[0].symbol)->get_name());
		}
#line 1794 "y.tab.c"
    break;

  case 18:
#line 292 "parser.y"
                {
			(yyval.symbol) = new SymbolInfo((yyvsp[-1].symbol)->get_name()+" "+(yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : type_specifier ID\n\n",line);
			fprintf(logfile , "%s %s\n\n" , (yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

			/* adding parameter to parameter list */

			(yyval.symbol)->push_in_param((yyvsp[0].symbol)->get_name() ,"ID");
		}
#line 1808 "y.tab.c"
    break;

  case 19:
#line 302 "parser.y"
                {
			(yyval.symbol) = new SymbolInfo((yyvsp[0].symbol)->get_name(), "NON_TERMINAL");
			fprintf(logfile,"At line no %d: parameter_list  : type_specifier\n\n",line);
			fprintf(logfile , "%s %s\n\n" , (yyvsp[0].symbol)->get_name().c_str());

			/* adding parameter to parameter list */

			(yyval.symbol)->push_in_param( "" , (yyvsp[0].symbol)->get_name());
		}
#line 1822 "y.tab.c"
    break;

  case 20:
#line 315 "parser.y"
  {
      (yyval.symbol) = new SymbolInfo("{\n"+(yyvsp[-1].symbol)->get_name()+"\n}"+"\n\n", "NON_TERMINAL");
      fprintf(logfile,"At line no %d: compound_statement : LCURL statements RCURL\n\n",line);
			fprintf(logfile,"{\n%s\n}\n\n",(yyvsp[-1].symbol)->get_name().c_str());

  }
#line 1833 "y.tab.c"
    break;

  case 21:
#line 322 "parser.y"
  {
    (yyval.symbol) = new SymbolInfo("{\n}", "NON_TERMINAL");
    fprintf(logfile,"At line no %d: compound_statement : LCURL RCURL\n\n",line);
    fprintf(logfile,"{}\n\n");

  }
#line 1844 "y.tab.c"
    break;

  case 22:
#line 331 "parser.y"
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

			//var_list.clear();
		}
#line 1890 "y.tab.c"
    break;

  case 23:
#line 375 "parser.y"
                {
			fprintf(logfile,"At line no: %d: type_specifier : INT \n\n",line);
			variable_type = "int";

			SymbolInfo *x = new SymbolInfo("int" , "int");
			(yyval.symbol) = x;
			fprintf(logfile,"%s\n\n",(yyval.symbol)->get_name().c_str());
		}
#line 1903 "y.tab.c"
    break;

  case 24:
#line 384 "parser.y"
                {
			fprintf(logfile,"At line no: %d: type_specifier : FLOAT \n",line);
			variable_type = "float";

			SymbolInfo *x = new SymbolInfo("float" , "float");
			(yyval.symbol) = x;
			fprintf(logfile,"%s\n\n",(yyval.symbol)->get_name().c_str());
		}
#line 1916 "y.tab.c"
    break;

  case 25:
#line 393 "parser.y"
                {
			 fprintf(logfile,"At line no: %d: type_specifier : VOID \n",line);
			variable_type = "void";

			SymbolInfo *x = new SymbolInfo("void" , "void");
			(yyval.symbol) = x;
			fprintf(logfile,"%s\n\n",(yyval.symbol)->get_name().c_str());
		}
#line 1929 "y.tab.c"
    break;

  case 26:
#line 404 "parser.y"
                {
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
#line 1949 "y.tab.c"
    break;

  case 27:
#line 420 "parser.y"
                {
			fprintf(logfile,"At line no %d: declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD\n\n",line);
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
#line 1973 "y.tab.c"
    break;

  case 28:
#line 441 "parser.y"
                {
			fprintf(logfile,"At line no %d: declaration_list : ID\n\n",line);
 			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

 			(yyval.symbol) = new SymbolInfo((yyvsp[0].symbol)->get_name() ,  "ID");

			 /* keeping track of identifier(variable) */
      temp_var.var_name = (string)(yyvsp[0].symbol)->get_name();
      temp_var.var_size = -1;
      var_list.pb(temp_var);

      (yyval.symbol)->push_in_var((yyvsp[0].symbol)->get_name() , "" , 0);

		}
#line 1992 "y.tab.c"
    break;

  case 29:
#line 456 "parser.y"
                {
			fprintf(logfile , "At line no %d:  declaration_list: ID LTHIRD CONST_INT RTHIRD\n\n",line);
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
#line 2014 "y.tab.c"
    break;

  case 30:
#line 476 "parser.y"
    {
       (yyval.symbol) = (yyvsp[0].symbol);
       fprintf(logfile , "At line no %d: statements : statement\n\n" , line);
       fprintf(logfile , "%s\n\n" , (yyvsp[0].symbol)->get_name().c_str());

    }
#line 2025 "y.tab.c"
    break;

  case 31:
#line 483 "parser.y"
    {
      (yyval.symbol) = new SymbolInfo((yyvsp[-1].symbol)->get_name()+(yyvsp[0].symbol)->get_name() , "NON_TERMINAL");
      fprintf(logfile , "At line no %d: statements : statements statement\n\n" , line);
      fprintf(logfile , "%s %s\n\n" , (yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

    }
#line 2036 "y.tab.c"
    break;

  case 32:
#line 492 "parser.y"
    {
      fprintf(logfile,"At line no %d: statement : var_declaration\n\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
      (yyvsp[0].symbol)->setname((yyvsp[0].symbol)->get_name()+"\n");
  		(yyval.symbol)=(yyvsp[0].symbol);
    }
#line 2047 "y.tab.c"
    break;

  case 33:
#line 499 "parser.y"
    {
      fprintf(logfile,"At line no %d: statement : expression_statement\n\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
      (yyvsp[0].symbol)->setname((yyvsp[0].symbol)->get_name()+"\n");
			(yyval.symbol)=(yyvsp[0].symbol);
    }
#line 2058 "y.tab.c"
    break;

  case 34:
#line 506 "parser.y"
    {
      fprintf(logfile,"At line no %d: statement : compound_statement\n\n",line);
      fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

      (yyval.symbol)=(yyvsp[0].symbol);
    }
#line 2069 "y.tab.c"
    break;

  case 35:
#line 513 "parser.y"
    {
      string str="for("+(yyvsp[-4].symbol)->get_name()+(yyvsp[-3].symbol)->get_name()+(yyvsp[-2].symbol)->get_name()+")"+(yyvsp[0].symbol)->get_name();
      (yyval.symbol) = new SymbolInfo(str , "NON_TERMINAL");
      fprintf(logfile,"line no. %d: statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement\n",line);
			fprintf(logfile,"%s\n\n",str.c_str());
    }
#line 2080 "y.tab.c"
    break;

  case 36:
#line 520 "parser.y"
    {
      string str = "if("+(yyvsp[-2].symbol)->get_name()+")"+(yyvsp[0].symbol)->get_name();
      (yyval.symbol) = new SymbolInfo(str , "statement");
      fprintf(logfile,"At line no %d: statement : IF LPAREN expression RPAREN statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());

    }
#line 2092 "y.tab.c"
    break;

  case 37:
#line 528 "parser.y"
    {
      string str = "if("+(yyvsp[-4].symbol)->get_name()+")"+(yyvsp[-2].symbol)->get_name()+"else"+(yyvsp[0].symbol)->get_name();
      (yyval.symbol) = new SymbolInfo(str , "statement");
      fprintf(logfile,"At line no %d: statement : IF LPAREN expression RPAREN statement ELSE statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());

    }
#line 2104 "y.tab.c"
    break;

  case 38:
#line 536 "parser.y"
    {
      string str = "while("+(yyvsp[-2].symbol)->get_name()+")"+(yyvsp[0].symbol)->get_name();
      (yyval.symbol) = new SymbolInfo(str , "statement");
      fprintf(logfile,"At line no %d: statement : WHILE LPAREN expression RPAREN statement\n\n",line);
      fprintf(logfile,"%s\n\n",str.c_str());
    }
#line 2115 "y.tab.c"
    break;

  case 39:
#line 543 "parser.y"
    {
      (yyval.symbol) = new SymbolInfo("printf("+(yyvsp[-2].symbol)->get_name()+");" , "statement");
      fprintf(logfile,"At line no %d: statement : PRINTLN LPAREN ID RPAREN SEMICOLON\n\n",line);
      fprintf(logfile , "printf(%s);" , (yyvsp[-2].symbol)->get_name().c_str());
    }
#line 2125 "y.tab.c"
    break;

  case 40:
#line 549 "parser.y"
    {
      (yyval.symbol) = new SymbolInfo("return "+(yyvsp[-1].symbol)->get_name()+";" , "statement");
      fprintf(logfile,"At line no %d: statement : RETURN expression SEMICOLON\n\n",line);
      fprintf(logfile , "return %s;\n\n" , (yyvsp[-1].symbol)->get_name().c_str());

      /* ----do this------
      isReturning=true;
			isReturningType=$2->getVariableType();
      */
    }
#line 2140 "y.tab.c"
    break;

  case 41:
#line 562 "parser.y"
    {
      (yyval.symbol) = new SymbolInfo(";" , "expression_statement");
      fprintf(logfile,"At line no %d: expression_statement : SEMICOLON\n",line);
			fprintf(logfile,";\n\n");
    }
#line 2150 "y.tab.c"
    break;

  case 42:
#line 568 "parser.y"
    {
      (yyval.symbol) = new SymbolInfo((yyvsp[-1].symbol)->get_name()+";" , "expression_statement");
      fprintf(logfile,"At line no %d: expression_statement : expression SEMICOLON\n\n",line);
      fprintf(logfile , "%s;\n\n" , (yyvsp[-1].symbol)->get_name().c_str());

    }
#line 2161 "y.tab.c"
    break;

  case 43:
#line 579 "parser.y"
    {
      //$$ = new SymbolInfo($1->get_name() , "variable");
      fprintf(logfile,"At line no. %d: variable : ID\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
      (yyval.symbol) = (yyvsp[0].symbol);
      //Semantic : chk if variable is declared before

      (yyval.symbol)->setIdentity("var");
      (yyval.symbol)->push_in_var((yyvsp[0].symbol)->get_name(),"",0);
      SymbolInfo *x=table.Lookup((yyvsp[0].symbol)->get_name());
      if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line: %d variable '%s' not declared in this scope\n\n",line,(yyvsp[0].symbol)->get_name().c_str());
      }
      else{
        (yyval.symbol)->setVariableType(x->getVariableType());
      }

    }
#line 2185 "y.tab.c"
    break;

  case 44:
#line 600 "parser.y"
   {
     fprintf(logfile,"At line no %d: variable : ID LTHIRD expression RTHIRD\n",line);
		 fprintf(logfile,"%s[%s]\n\n",(yyvsp[-3].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str());
     (yyval.symbol) = new SymbolInfo((yyvsp[-3].symbol)->get_name()+"["+(yyvsp[-1].symbol)->get_name()+"]" , "variable");

     (yyval.symbol)->setIdentity("array");
     //array index must be integer

   }
#line 2199 "y.tab.c"
    break;

  case 45:
#line 613 "parser.y"
    {
      (yyval.symbol) = (yyvsp[0].symbol);
      fprintf(logfile,"At line no %d: expression : logic_expression\n\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
    }
#line 2209 "y.tab.c"
    break;

  case 46:
#line 619 "parser.y"
    {
      (yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+"="+(yyvsp[0].symbol)->get_name() , "expression");
      fprintf(logfile,"At line no %d: expression : variable ASSIGNOP logic_expression\n\n",line);
			fprintf(logfile,"%s = %s\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

      //semantics
      //todo
      //assign $3's variable_type to $1 after some error chkings

      ///pass arrayname if array otherwise pass varname only
      ///suppose $1->get_name() is a[2].Now modified_name returns only a
      string varname;
      varname = modified_name((yyvsp[-2].symbol)->get_name());
      SymbolInfo *x=table.Lookup(varname);
			if(x)
			{
				//setting type of var(int/float)
        (yyvsp[-2].symbol)->setVariableType(x->getVariableType());

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
              fprintf(errorfile,"Error at line: %d Wrong array index\n\n",line);
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
#line 2279 "y.tab.c"
    break;

  case 47:
#line 687 "parser.y"
    {
      (yyval.symbol) = (yyvsp[0].symbol);
      fprintf(logfile,"At line no %d: logic_expression : rel_expression\n\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

    }
#line 2290 "y.tab.c"
    break;

  case 48:
#line 694 "parser.y"
    {
      (yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+(yyvsp[-1].symbol)->get_name()+(yyvsp[0].symbol)->get_name() , "logic_expression");
      fprintf(logfile,"At line no %d: logic_expression : rel_expression LOGICOP rel_expression\n\n",line);
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
    }
#line 2311 "y.tab.c"
    break;

  case 49:
#line 713 "parser.y"
   {
     (yyval.symbol) = (yyvsp[0].symbol);
     fprintf(logfile,"At line no %d: rel_expression	: simple_expression\n\n",line);
     fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

   }
#line 2322 "y.tab.c"
    break;

  case 50:
#line 720 "parser.y"
   {
     (yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+(yyvsp[-1].symbol)->get_name()+(yyvsp[0].symbol)->get_name() , "rel_expression");
     fprintf(logfile,"At line no %d: rel_expression : simple_expression RELOP simple_expression\n\n",line);
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
   }
#line 2342 "y.tab.c"
    break;

  case 51:
#line 738 "parser.y"
  {
    (yyval.symbol) = (yyvsp[0].symbol);
    fprintf(logfile,"At line no %d: simple_expression : term\n\n",line);
    fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

  }
#line 2353 "y.tab.c"
    break;

  case 52:
#line 745 "parser.y"
  {
    (yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+(yyvsp[-1].symbol)->get_name()+(yyvsp[0].symbol)->get_name() , "simple_expression");
    fprintf(logfile,"At line no %d: simple_expression : simple_expression ADDOP term\n\n",line);
    fprintf(logfile,"%s%s%s\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

    if((yyvsp[-2].symbol)->getVariableType()=="float" || (yyvsp[0].symbol)->getVariableType()=="float")
				(yyval.symbol)->setVariableType("float");
			else
				(yyval.symbol)->setVariableType("int");

  }
#line 2369 "y.tab.c"
    break;

  case 53:
#line 759 "parser.y"
    {
      (yyval.symbol) = (yyvsp[0].symbol);
      fprintf(logfile,"At line no %d: term :	unary_expression\n\n",line);
      fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
    }
#line 2379 "y.tab.c"
    break;

  case 54:
#line 765 "parser.y"
    {
      (yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+(yyvsp[-1].symbol)->get_name()+(yyvsp[0].symbol)->get_name() , "term");
      fprintf(logfile,"At line no %d: term : term MULOP unary_expression\n\n",line);
      fprintf(logfile,"%s%s%s\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

      //features of mod operation
      if((yyvsp[-1].symbol)->get_name()=="%" && ((yyvsp[-2].symbol)->getVariableType()!="int" || (yyvsp[0].symbol)->getVariableType()!="int")){
				error_cnt++;
				fprintf(errorfile,"Error at line %d: type mismatch,(mod operation)\n\n",line);
			}

			//set variable_type
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
#line 2407 "y.tab.c"
    break;

  case 55:
#line 791 "parser.y"
    {
      fprintf(logfile,"At line no %d: unary_expression : ADDOP unary_expression\n",line);
			fprintf(logfile,"%s%s\n\n",(yyvsp[-1].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

			(yyval.symbol) = new SymbolInfo((yyvsp[-1].symbol)->get_name()+(yyvsp[0].symbol)->get_name(),"unary_expression");

      (yyval.symbol)->setVariableType((yyvsp[0].symbol)->getVariableType());
      (yyval.symbol)->setIdentity((yyvsp[0].symbol)->getIdentity());

    }
#line 2422 "y.tab.c"
    break;

  case 56:
#line 802 "parser.y"
    {
      fprintf(logfile,"At line no %d: unary_expression : NOT unary_expression\n",line);
      fprintf(logfile,"!%s\n\n",(yyvsp[0].symbol)->get_name().c_str());

      (yyval.symbol) = new SymbolInfo("!"+(yyvsp[0].symbol)->get_name(),"unary_expression");

      (yyval.symbol)->setVariableType((yyvsp[0].symbol)->getVariableType());
      (yyval.symbol)->setIdentity((yyvsp[0].symbol)->getIdentity());

    }
#line 2437 "y.tab.c"
    break;

  case 57:
#line 813 "parser.y"
     {
       (yyval.symbol) = (yyvsp[0].symbol);
       fprintf(logfile,"At line no %d: unary_expression :	factor\n\n",line);
       fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
     }
#line 2447 "y.tab.c"
    break;

  case 58:
#line 822 "parser.y"
    {
      fprintf(logfile,"At line no %d: factor : variable\n\n",line);
      fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
      (yyval.symbol) = (yyvsp[0].symbol);
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
                fprintf(errorfile,"Error at line: %d Type Mismatch\n\n",line);
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
#line 2491 "y.tab.c"
    break;

  case 59:
#line 862 "parser.y"
    {
      (yyval.symbol) = new SymbolInfo((yyvsp[-3].symbol)->get_name()+"("+(yyvsp[-1].symbol)->get_name()+")" , "factor");
      fprintf(logfile,"At line no %d: factor : ID LPAREN argument_list RPAREN\n\n",line);
      fprintf(logfile,"%s(%s)\n\n",(yyvsp[-3].symbol)->get_name().c_str(),(yyvsp[-1].symbol)->get_name().c_str());

      /*semantic hugeee

      */

    }
#line 2506 "y.tab.c"
    break;

  case 60:
#line 873 "parser.y"
    {
      (yyval.symbol) = new SymbolInfo("("+(yyvsp[-1].symbol)->get_name()+")" , "factor");
      fprintf(logfile,"At line no %d: factor : LPAREN expression RPAREN\n\n",line);
      fprintf(logfile,"(%s)\n\n",(yyvsp[-1].symbol)->get_name().c_str());

      (yyval.symbol)->setVariableType((yyvsp[-1].symbol)->getVariableType());

    }
#line 2519 "y.tab.c"
    break;

  case 61:
#line 882 "parser.y"
    {
      fprintf(logfile,"At line no %d: factor : CONST_INT\n\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
			(yyval.symbol)=(yyvsp[0].symbol);
			(yyval.symbol)->setVariableType("int");

    }
#line 2531 "y.tab.c"
    break;

  case 62:
#line 890 "parser.y"
    {
      fprintf(logfile,"At line no %d: factor : CONST_FLOAT\n\n",line);
			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
			(yyval.symbol)=(yyvsp[0].symbol);
			(yyval.symbol)->setVariableType("float");

    }
#line 2543 "y.tab.c"
    break;

  case 63:
#line 898 "parser.y"
    {
      fprintf(logfile,"At line no %d: factor	: variable INCOP\n\n",line);
			fprintf(logfile,"%s++\n\n",(yyvsp[-1].symbol)->get_name().c_str());
      (yyval.symbol) = new SymbolInfo((yyvsp[-1].symbol)->get_name()+"++","factor");

      SymbolInfo *x=table.Lookup((yyvsp[-1].symbol)->get_name());
			if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,(yyvsp[-1].symbol)->get_name().c_str());
      }
      else{

         (yyval.symbol)->setVariableType((yyvsp[-1].symbol)->getVariableType());
         (yyval.symbol)->setIdentity((yyvsp[-1].symbol)->getIdentity());
      }

    }
#line 2565 "y.tab.c"
    break;

  case 64:
#line 916 "parser.y"
    {
      fprintf(logfile,"At line no %d: factor	: variable DECOP\n\n",line);
			fprintf(logfile,"%s--\n\n",(yyvsp[-1].symbol)->get_name().c_str());
      (yyval.symbol) = new SymbolInfo((yyvsp[-1].symbol)->get_name()+"--","factor");

      SymbolInfo *x=table.Lookup((yyvsp[-1].symbol)->get_name());
			if(!x){
        error_cnt++;
        fprintf(errorfile,"Error at line %d: variable %s not declared in this scope\n\n",line,(yyvsp[-1].symbol)->get_name().c_str());
      }
      else{

         (yyval.symbol)->setVariableType((yyvsp[-1].symbol)->getVariableType());
         (yyval.symbol)->setIdentity((yyvsp[-1].symbol)->getIdentity());
      }
    }
#line 2586 "y.tab.c"
    break;

  case 65:
#line 935 "parser.y"
      {
        fprintf(logfile,"At line no %d: argument_list : arguments\n\n",line);
  			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
  			(yyval.symbol)=(yyvsp[0].symbol);
      }
#line 2596 "y.tab.c"
    break;

  case 66:
#line 941 "parser.y"
        {
          (yyval.symbol) = new SymbolInfo("","argument_list");
        }
#line 2604 "y.tab.c"
    break;

  case 67:
#line 947 "parser.y"
        {
          (yyval.symbol) = new SymbolInfo((yyvsp[-2].symbol)->get_name()+" , "+(yyvsp[0].symbol)->get_name() , "term");
          fprintf(logfile,"At line no %d: arguments : arguments COMMA logic_expression\n\n",line);
          fprintf(logfile,"%s , %s\n\n",(yyvsp[-2].symbol)->get_name().c_str(),(yyvsp[0].symbol)->get_name().c_str());

          /*
          arg_list.push_back($3);
          */
        }
#line 2618 "y.tab.c"
    break;

  case 68:
#line 957 "parser.y"
        {
          fprintf(logfile,"At line no %d: arguments : logic_expression\n\n",line);
    			fprintf(logfile,"%s\n\n",(yyvsp[0].symbol)->get_name().c_str());
    			(yyval.symbol)=(yyvsp[0].symbol);

          /*
          arg_list.push_back($$);
          */
        }
#line 2632 "y.tab.c"
    break;


#line 2636 "y.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 969 "parser.y"

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
