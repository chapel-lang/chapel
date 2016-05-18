/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 31 "chapel.ypp"


#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include "lexyacc.h" // all #includes here, for make depend
#include "driver.h"

  static int query_uid = 1;
  int captureTokens;
  char captureString[1024];

 
#define YYLLOC_DEFAULT(Current, Rhs, N)                                 \
  if (N) {                                                              \
        (Current).first_line   = (Rhs)[1].first_line;                   \
        if ((Current).first_line) yystartlineno = (Current).first_line; \
        (Current).first_column = (Rhs)[1].first_column;                 \
        (Current).last_line    = (Rhs)[N].last_line;                    \
        (Current).last_column  = (Rhs)[N].last_column;                  \
        (Current).comment = NULL;                                       \
  } else  {                                                             \
    (Current) = yylloc;                                                 \
  }                                                                     \



/* Line 268 of yacc.c  */
#line 100 "chapel.tab.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TIDENT = 258,
     IMAGLITERAL = 259,
     INTLITERAL = 260,
     REALLITERAL = 261,
     STRINGLITERAL = 262,
     EXTERNCODE = 263,
     TALIGN = 264,
     TATOMIC = 265,
     TBEGIN = 266,
     TBREAK = 267,
     TBY = 268,
     TCLASS = 269,
     TCOBEGIN = 270,
     TCOFORALL = 271,
     TCONFIG = 272,
     TCONST = 273,
     TCONTINUE = 274,
     TDELETE = 275,
     TDMAPPED = 276,
     TDO = 277,
     TDOMAIN = 278,
     TELSE = 279,
     TENUM = 280,
     TEXPORT = 281,
     TEXTERN = 282,
     TFOR = 283,
     TFORALL = 284,
     TIF = 285,
     TIN = 286,
     TINDEX = 287,
     TINLINE = 288,
     TINOUT = 289,
     TITER = 290,
     TLABEL = 291,
     TLAMBDA = 292,
     TLET = 293,
     TLOCAL = 294,
     TMINUSMINUS = 295,
     TMODULE = 296,
     TNEW = 297,
     TNIL = 298,
     TNOINIT = 299,
     TON = 300,
     TOTHERWISE = 301,
     TOUT = 302,
     TPARAM = 303,
     TPLUSPLUS = 304,
     TPRAGMA = 305,
     TPRIMITIVE = 306,
     TPROC = 307,
     TRECORD = 308,
     TREDUCE = 309,
     TREF = 310,
     TRETURN = 311,
     TSCAN = 312,
     TSELECT = 313,
     TSERIAL = 314,
     TSINGLE = 315,
     TSPARSE = 316,
     TSUBDOMAIN = 317,
     TSYNC = 318,
     TTHEN = 319,
     TTYPE = 320,
     TUNDERSCORE = 321,
     TUNION = 322,
     TUSE = 323,
     TVAR = 324,
     TWHEN = 325,
     TWHERE = 326,
     TWHILE = 327,
     TWITH = 328,
     TYIELD = 329,
     TZIP = 330,
     TREFVAR = 331,
     TALIAS = 332,
     TAND = 333,
     TASSIGN = 334,
     TASSIGNBAND = 335,
     TASSIGNBOR = 336,
     TASSIGNBXOR = 337,
     TASSIGNDIVIDE = 338,
     TASSIGNEXP = 339,
     TASSIGNLAND = 340,
     TASSIGNLOR = 341,
     TASSIGNMINUS = 342,
     TASSIGNMOD = 343,
     TASSIGNMULTIPLY = 344,
     TASSIGNPLUS = 345,
     TASSIGNSL = 346,
     TASSIGNSR = 347,
     TBAND = 348,
     TBNOT = 349,
     TBOR = 350,
     TBXOR = 351,
     TCOLON = 352,
     TCOMMA = 353,
     TDIVIDE = 354,
     TDOT = 355,
     TDOTDOT = 356,
     TDOTDOTDOT = 357,
     TEQUAL = 358,
     TEXP = 359,
     TGREATER = 360,
     TGREATEREQUAL = 361,
     THASH = 362,
     TLESS = 363,
     TLESSEQUAL = 364,
     TMINUS = 365,
     TMOD = 366,
     TNOT = 367,
     TNOTEQUAL = 368,
     TOR = 369,
     TPLUS = 370,
     TQUESTION = 371,
     TSEMI = 372,
     TSHIFTLEFT = 373,
     TSHIFTRIGHT = 374,
     TSTAR = 375,
     TSWAP = 376,
     TIO = 377,
     TLCBR = 378,
     TRCBR = 379,
     TLP = 380,
     TRP = 381,
     TLSBR = 382,
     TRSBR = 383,
     TNOELSE = 384,
     TUMINUS = 385,
     TUPLUS = 386
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 61 "chapel.ypp"

  const char* pch;
  Vec<const char*>* vpch;
  RetTag retTag;
  bool b;
  IntentTag pt;
  Expr* pexpr;
  DefExpr* pdefexpr;
  CallExpr* pcallexpr;
  BlockStmt* pblockstmt;
  Type* ptype;
  EnumType* penumtype;
  FnSymbol* pfnsymbol;
  Flag flag;
  ProcIter procIter;
  FlagSet* flagSet;



/* Line 293 of yacc.c  */
#line 287 "chapel.tab.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 312 "chapel.tab.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   10709

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  132
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  105
/* YYNRULES -- Number of rules.  */
#define YYNRULES  419
/* YYNRULES -- Number of states.  */
#define YYNSTATES  790

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   386

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    17,    21,
      23,    25,    27,    29,    31,    33,    35,    37,    39,    41,
      43,    46,    49,    53,    57,    61,    65,    69,    73,    76,
      80,    84,    87,    90,    94,    96,   101,   107,   109,   112,
     116,   118,   121,   125,   130,   135,   140,   145,   150,   155,
     160,   165,   170,   175,   180,   185,   190,   195,   200,   201,
     203,   205,   208,   210,   213,   217,   219,   221,   223,   225,
     227,   229,   232,   238,   242,   249,   256,   261,   267,   273,
     277,   281,   288,   294,   300,   304,   311,   318,   323,   328,
     333,   337,   344,   350,   356,   357,   360,   364,   367,   374,
     382,   383,   385,   387,   389,   391,   392,   395,   396,   399,
     403,   409,   411,   414,   418,   420,   424,   425,   426,   435,
     436,   438,   440,   443,   445,   448,   449,   450,   460,   464,
     468,   474,   480,   482,   485,   487,   489,   491,   493,   495,
     497,   499,   501,   503,   505,   507,   509,   511,   513,   515,
     517,   519,   521,   523,   525,   527,   529,   531,   533,   535,
     537,   539,   541,   543,   545,   547,   549,   551,   553,   555,
     557,   558,   562,   566,   567,   569,   573,   578,   583,   590,
     597,   598,   600,   602,   604,   606,   609,   612,   614,   616,
     618,   619,   621,   623,   625,   627,   628,   630,   632,   634,
     636,   638,   640,   642,   644,   646,   649,   651,   652,   654,
     657,   660,   661,   664,   668,   673,   678,   681,   686,   687,
     690,   693,   698,   703,   708,   714,   719,   720,   722,   724,
     726,   730,   734,   739,   745,   747,   749,   753,   755,   758,
     762,   763,   766,   769,   770,   775,   776,   779,   782,   784,
     789,   794,   801,   803,   804,   806,   808,   812,   817,   821,
     826,   833,   834,   837,   840,   843,   846,   849,   852,   854,
     856,   860,   864,   866,   868,   870,   874,   878,   879,   881,
     883,   887,   891,   895,   899,   901,   903,   905,   907,   909,
     911,   913,   915,   918,   923,   928,   933,   939,   942,   945,
     952,   959,   964,   974,   984,   992,   999,  1006,  1011,  1021,
    1031,  1039,  1044,  1051,  1058,  1068,  1078,  1085,  1087,  1089,
    1091,  1093,  1095,  1097,  1099,  1101,  1105,  1106,  1111,  1116,
    1118,  1122,  1125,  1127,  1131,  1133,  1137,  1140,  1145,  1147,
    1149,  1151,  1153,  1155,  1157,  1159,  1161,  1166,  1170,  1174,
    1177,  1180,  1182,  1184,  1186,  1188,  1190,  1192,  1194,  1199,
    1204,  1209,  1213,  1217,  1221,  1225,  1230,  1234,  1236,  1238,
    1240,  1242,  1246,  1250,  1254,  1258,  1264,  1268,  1272,  1276,
    1280,  1284,  1288,  1292,  1296,  1300,  1304,  1308,  1312,  1316,
    1320,  1324,  1328,  1332,  1336,  1340,  1344,  1348,  1352,  1356,
    1359,  1362,  1365,  1368,  1371,  1374,  1378,  1382,  1386,  1390,
    1394,  1398,  1402,  1406,  1408,  1410,  1412,  1414,  1416,  1418
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     133,     0,    -1,   134,    -1,    -1,   134,   135,    -1,   137,
      -1,   136,   137,    -1,    50,     7,    -1,   136,    50,     7,
      -1,   138,    -1,   140,    -1,   142,    -1,   148,    -1,   143,
      -1,   149,    -1,   152,    -1,   150,    -1,   153,    -1,   156,
      -1,   147,    -1,   216,   117,    -1,    10,   137,    -1,    11,
     217,   137,    -1,    12,   144,   117,    -1,    15,   217,   140,
      -1,    19,   144,   117,    -1,    20,   224,   117,    -1,    36,
     145,   137,    -1,    39,   137,    -1,    45,   224,   146,    -1,
      59,   224,   146,    -1,    59,   146,    -1,    63,   137,    -1,
      74,   224,   117,    -1,     1,    -1,   139,   145,   123,   124,
      -1,   139,   145,   123,   141,   124,    -1,    41,    -1,   123,
     124,    -1,   123,   141,   124,    -1,   135,    -1,   141,   135,
      -1,    68,   205,   117,    -1,   225,    79,   224,   117,    -1,
     225,    90,   224,   117,    -1,   225,    87,   224,   117,    -1,
     225,    89,   224,   117,    -1,   225,    83,   224,   117,    -1,
     225,    88,   224,   117,    -1,   225,    84,   224,   117,    -1,
     225,    80,   224,   117,    -1,   225,    81,   224,   117,    -1,
     225,    82,   224,   117,    -1,   225,    92,   224,   117,    -1,
     225,    91,   224,   117,    -1,   225,   121,   224,   117,    -1,
     225,    85,   224,   117,    -1,   225,    86,   224,   117,    -1,
      -1,   145,    -1,     3,    -1,    22,   137,    -1,   140,    -1,
      56,   117,    -1,    56,   224,   117,    -1,   117,    -1,   169,
      -1,   157,    -1,   162,    -1,   189,    -1,   192,    -1,    27,
       8,    -1,    22,   137,    72,   224,   117,    -1,    72,   224,
     146,    -1,    16,   224,    31,   224,   217,   146,    -1,    16,
     224,    31,   151,   217,   146,    -1,    16,   224,   217,   146,
      -1,    28,   224,    31,   224,   146,    -1,    28,   224,    31,
     151,   146,    -1,    28,   224,   146,    -1,    28,   151,   146,
      -1,    28,    48,   145,    31,   224,   146,    -1,    29,   224,
      31,   224,   146,    -1,    29,   224,    31,   151,   146,    -1,
      29,   224,   146,    -1,   127,   205,    31,   224,   128,   137,
      -1,   127,   205,    31,   151,   128,   137,    -1,   127,   205,
     128,   137,    -1,    75,   125,   205,   126,    -1,    30,   224,
      64,   137,    -1,    30,   224,   140,    -1,    30,   224,    64,
     137,    24,   137,    -1,    30,   224,   140,    24,   137,    -1,
      58,   224,   123,   154,   124,    -1,    -1,   154,   155,    -1,
      70,   205,   146,    -1,    46,   137,    -1,    65,    58,   205,
     123,   154,   124,    -1,   158,   159,   145,   160,   123,   161,
     124,    -1,    -1,    27,    -1,    14,    -1,    53,    -1,    67,
      -1,    -1,    97,   205,    -1,    -1,   161,   148,    -1,   161,
     136,   148,    -1,    25,   145,   123,   163,   124,    -1,   164,
      -1,   163,    98,    -1,   163,    98,   164,    -1,   145,    -1,
     145,    79,   224,    -1,    -1,    -1,    37,   166,   176,   167,
     182,   200,   188,   184,    -1,    -1,    33,    -1,    26,    -1,
      26,   145,    -1,    27,    -1,    27,   145,    -1,    -1,    -1,
     168,   181,   170,   172,   171,   182,   200,   188,   183,    -1,
     180,   173,   175,    -1,   180,   174,   175,    -1,   180,   145,
     100,   173,   175,    -1,   180,   145,   100,   174,   175,    -1,
     145,    -1,    94,   145,    -1,    93,    -1,    95,    -1,    96,
      -1,    94,    -1,   103,    -1,   113,    -1,   109,    -1,   106,
      -1,   108,    -1,   105,    -1,   115,    -1,   110,    -1,   120,
      -1,    99,    -1,   118,    -1,   119,    -1,   111,    -1,   104,
      -1,   112,    -1,    13,    -1,   107,    -1,     9,    -1,   121,
      -1,   122,    -1,    79,    -1,    90,    -1,    87,    -1,    89,
      -1,    83,    -1,    88,    -1,    84,    -1,    80,    -1,    81,
      -1,    82,    -1,    92,    -1,    91,    -1,    -1,   125,   177,
     126,    -1,   125,   177,   126,    -1,    -1,   178,    -1,   177,
      98,   178,    -1,   179,   145,   204,   198,    -1,   179,   145,
     204,   187,    -1,   179,   125,   197,   126,   204,   198,    -1,
     179,   125,   197,   126,   204,   187,    -1,    -1,    31,    -1,
      34,    -1,    47,    -1,    18,    -1,    18,    31,    -1,    18,
      55,    -1,    48,    -1,    55,    -1,    65,    -1,    -1,    48,
      -1,    55,    -1,    52,    -1,    35,    -1,    -1,    18,    -1,
      69,    -1,    55,    -1,    48,    -1,    65,    -1,   117,    -1,
     184,    -1,   140,    -1,   147,    -1,   116,   145,    -1,   116,
      -1,    -1,   185,    -1,   102,   224,    -1,   102,   186,    -1,
      -1,    71,   224,    -1,    65,   190,   117,    -1,    17,    65,
     190,   117,    -1,    27,    65,   190,   117,    -1,   145,   191,
      -1,   145,   191,    98,   190,    -1,    -1,    79,   212,    -1,
      79,   201,    -1,   193,    48,   194,   117,    -1,   193,    18,
     194,   117,    -1,   193,    76,   194,   117,    -1,   193,    18,
      76,   194,   117,    -1,   193,    69,   194,   117,    -1,    -1,
      17,    -1,    27,    -1,   195,    -1,   194,    98,   195,    -1,
     145,   200,   198,    -1,   145,   199,    77,   224,    -1,   125,
     197,   126,   200,   198,    -1,    66,    -1,   145,    -1,   125,
     197,   126,    -1,   196,    -1,   196,    98,    -1,   196,    98,
     197,    -1,    -1,    79,    44,    -1,    79,   224,    -1,    -1,
      97,   127,   205,   128,    -1,    -1,    97,   212,    -1,    97,
     201,    -1,     1,    -1,   127,   205,   128,   212,    -1,   127,
     205,   128,   201,    -1,   127,   205,    31,   224,   128,   212,
      -1,     1,    -1,    -1,   212,    -1,   185,    -1,   127,   128,
     202,    -1,   127,   205,   128,   202,    -1,   127,   128,   203,
      -1,   127,   205,   128,   203,    -1,   127,   205,    31,   224,
     128,   202,    -1,    -1,    97,   212,    -1,    97,   185,    -1,
      97,    23,    -1,    97,    60,    -1,    97,    63,    -1,    97,
     203,    -1,   224,    -1,   185,    -1,   205,    98,   224,    -1,
     205,    98,   185,    -1,    66,    -1,   224,    -1,   185,    -1,
     206,    98,   206,    -1,   207,    98,   206,    -1,    -1,   209,
      -1,   210,    -1,   209,    98,   210,    -1,   145,    79,   185,
      -1,   145,    79,   224,    -1,   145,    77,   224,    -1,   185,
      -1,   224,    -1,   145,    -1,   215,    -1,   225,    -1,   214,
      -1,   233,    -1,   232,    -1,    60,   224,    -1,    32,   125,
     208,   126,    -1,    23,   125,   208,   126,    -1,    62,   125,
     208,   126,    -1,    61,    62,   125,   208,   126,    -1,    10,
     224,    -1,    63,   224,    -1,    28,   224,    31,   224,    22,
     224,    -1,    28,   224,    31,   151,    22,   224,    -1,    28,
     224,    22,   224,    -1,    28,   224,    31,   224,    22,    30,
     224,    64,   224,    -1,    28,   224,    31,   151,    22,    30,
     224,    64,   224,    -1,    28,   224,    22,    30,   224,    64,
     224,    -1,    29,   224,    31,   224,    22,   224,    -1,    29,
     224,    31,   151,    22,   224,    -1,    29,   224,    22,   224,
      -1,    29,   224,    31,   224,    22,    30,   224,    64,   224,
      -1,    29,   224,    31,   151,    22,    30,   224,    64,   224,
      -1,    29,   224,    22,    30,   224,    64,   224,    -1,   127,
     205,   128,   224,    -1,   127,   205,    31,   224,   128,   224,
      -1,   127,   205,    31,   151,   128,   224,    -1,   127,   205,
      31,   224,   128,    30,   224,    64,   224,    -1,   127,   205,
      31,   151,   128,    30,   224,    64,   224,    -1,    30,   224,
      64,   224,    24,   224,    -1,    43,    -1,   215,    -1,   211,
      -1,   228,    -1,   227,    -1,   165,    -1,   222,    -1,   223,
      -1,   221,   122,   224,    -1,    -1,    55,   125,   220,   126,
      -1,    73,   125,   218,   126,    -1,   219,    -1,   218,    98,
     219,    -1,   179,   211,    -1,   211,    -1,   220,    98,   211,
      -1,   225,    -1,   221,   122,   224,    -1,    42,   224,    -1,
      38,   194,    31,   224,    -1,   230,    -1,   212,    -1,   213,
      -1,   234,    -1,   235,    -1,   165,    -1,   222,    -1,   223,
      -1,   125,   102,   224,   126,    -1,   224,    97,   224,    -1,
     224,   101,   224,    -1,   224,   101,    -1,   101,   224,    -1,
     101,    -1,   211,    -1,   227,    -1,   228,    -1,   229,    -1,
     225,    -1,   165,    -1,   226,   125,   208,   126,    -1,   226,
     127,   208,   128,    -1,    51,   125,   208,   126,    -1,   224,
     100,   145,    -1,   224,   100,    65,    -1,   224,   100,    23,
      -1,   125,   206,   126,    -1,   125,   206,    98,   126,    -1,
     125,   207,   126,    -1,     5,    -1,     6,    -1,     4,    -1,
       7,    -1,   123,   205,   124,    -1,   127,   205,   128,    -1,
     127,   231,   128,    -1,   224,    77,   224,    -1,   231,    98,
     224,    77,   224,    -1,   224,   115,   224,    -1,   224,   110,
     224,    -1,   224,   120,   224,    -1,   224,    99,   224,    -1,
     224,   118,   224,    -1,   224,   119,   224,    -1,   224,   111,
     224,    -1,   224,   103,   224,    -1,   224,   113,   224,    -1,
     224,   109,   224,    -1,   224,   106,   224,    -1,   224,   108,
     224,    -1,   224,   105,   224,    -1,   224,    93,   224,    -1,
     224,    95,   224,    -1,   224,    96,   224,    -1,   224,    78,
     224,    -1,   224,   114,   224,    -1,   224,   104,   224,    -1,
     224,    13,   224,    -1,   224,     9,   224,    -1,   224,   107,
     224,    -1,   224,    21,   224,    -1,   115,   224,    -1,   110,
     224,    -1,    40,   224,    -1,    49,   224,    -1,   112,   224,
      -1,    94,   224,    -1,   224,    54,   224,    -1,   224,    54,
     151,    -1,   236,    54,   224,    -1,   236,    54,   151,    -1,
     224,    57,   224,    -1,   224,    57,   151,    -1,   236,    57,
     224,    -1,   236,    57,   151,    -1,   115,    -1,   120,    -1,
      78,    -1,   114,    -1,    93,    -1,    95,    -1,    96,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   219,   219,   224,   225,   231,   232,   237,   238,   243,
     244,   245,   246,   247,   248,   249,   250,   251,   252,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   272,   274,   279,   294,   295,
     300,   301,   306,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   328,   329,
     333,   337,   338,   342,   343,   347,   348,   349,   350,   351,
     352,   356,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   383,   384,   390,   396,   406,   410,
     411,   412,   413,   417,   422,   423,   427,   429,   434,   441,
     447,   448,   452,   454,   456,   461,   462,   467,   468,   470,
     475,   485,   492,   496,   504,   505,   510,   515,   509,   540,
     541,   543,   546,   549,   552,   558,   564,   557,   594,   598,
     603,   607,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,   644,   645,
     646,   647,   648,   649,   650,   651,   652,   653,   654,   655,
     659,   660,   664,   668,   669,   670,   674,   676,   678,   680,
     685,   686,   687,   688,   689,   690,   691,   692,   693,   694,
     698,   699,   700,   704,   705,   709,   710,   711,   718,   719,
     720,   724,   725,   729,   730,   734,   736,   741,   742,   746,
     747,   751,   752,   756,   758,   760,   765,   772,   783,   784,
     786,   791,   795,   799,   803,   807,   814,   815,   816,   820,
     821,   829,   831,   837,   842,   844,   846,   851,   853,   855,
     862,   863,   864,   868,   869,   874,   875,   876,   877,   897,
     901,   905,   913,   917,   918,   919,   923,   925,   931,   933,
     935,   940,   941,   942,   943,   944,   945,   946,   952,   953,
     954,   955,   959,   960,   961,   965,   966,   970,   971,   975,
     976,   980,   981,   982,   983,   984,   988,   999,  1000,  1001,
    1002,  1003,  1004,  1006,  1008,  1010,  1012,  1014,  1016,  1021,
    1023,  1025,  1027,  1029,  1031,  1033,  1035,  1037,  1039,  1041,
    1043,  1045,  1052,  1058,  1064,  1070,  1079,  1084,  1092,  1093,
    1094,  1095,  1096,  1097,  1098,  1099,  1104,  1105,  1111,  1115,
    1116,  1120,  1131,  1132,  1136,  1137,  1142,  1147,  1155,  1156,
    1157,  1158,  1159,  1160,  1161,  1162,  1163,  1165,  1167,  1169,
    1171,  1173,  1178,  1179,  1180,  1181,  1192,  1193,  1197,  1198,
    1199,  1203,  1204,  1205,  1213,  1214,  1215,  1219,  1220,  1221,
    1222,  1223,  1224,  1225,  1232,  1233,  1237,  1238,  1239,  1240,
    1241,  1242,  1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,
    1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,  1259,  1263,
    1264,  1265,  1266,  1267,  1268,  1272,  1273,  1274,  1275,  1279,
    1280,  1281,  1282,  1287,  1288,  1289,  1290,  1291,  1292,  1293
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TIDENT", "IMAGLITERAL", "INTLITERAL",
  "REALLITERAL", "STRINGLITERAL", "EXTERNCODE", "TALIGN", "TATOMIC",
  "TBEGIN", "TBREAK", "TBY", "TCLASS", "TCOBEGIN", "TCOFORALL", "TCONFIG",
  "TCONST", "TCONTINUE", "TDELETE", "TDMAPPED", "TDO", "TDOMAIN", "TELSE",
  "TENUM", "TEXPORT", "TEXTERN", "TFOR", "TFORALL", "TIF", "TIN", "TINDEX",
  "TINLINE", "TINOUT", "TITER", "TLABEL", "TLAMBDA", "TLET", "TLOCAL",
  "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TON", "TOTHERWISE",
  "TOUT", "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPROC",
  "TRECORD", "TREDUCE", "TREF", "TRETURN", "TSCAN", "TSELECT", "TSERIAL",
  "TSINGLE", "TSPARSE", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTYPE",
  "TUNDERSCORE", "TUNION", "TUSE", "TVAR", "TWHEN", "TWHERE", "TWHILE",
  "TWITH", "TYIELD", "TZIP", "TREFVAR", "TALIAS", "TAND", "TASSIGN",
  "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNSL", "TASSIGNSR", "TBAND",
  "TBNOT", "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT",
  "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL",
  "THASH", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOT", "TNOTEQUAL",
  "TOR", "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT",
  "TSTAR", "TSWAP", "TIO", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR",
  "TRSBR", "TNOELSE", "TUMINUS", "TUPLUS", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt",
  "module_decl_stmt", "module", "block_stmt", "stmt_ls", "use_stmt",
  "assignment_stmt", "opt_ident", "ident", "do_stmt", "return_stmt",
  "class_level_stmt", "extern_block_stmt", "loop_stmt",
  "zippered_iterator", "if_stmt", "select_stmt", "when_stmt_ls",
  "when_stmt", "type_select_stmt", "class_decl_stmt", "opt_extern",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_ls", "enum_item", "lambda_decl_expr", "$@1", "$@2", "linkage_spec",
  "fn_decl_stmt", "$@3", "$@4", "fn_decl_stmt_inner", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls",
  "formal", "opt_intent_tag", "opt_this_intent_tag", "proc_or_iter",
  "opt_ret_tag", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "opt_query_expr", "var_arg_expr", "opt_where_part",
  "type_alias_decl_stmt", "type_alias_decl_stmt_inner", "opt_init_type",
  "var_decl_stmt", "opt_config", "var_decl_stmt_inner_ls",
  "var_decl_stmt_inner", "tuple_var_decl_component",
  "tuple_var_decl_stmt_inner_ls", "opt_init_expr", "opt_reindex_expr",
  "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "opt_formal_type", "expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "for_expr", "cond_expr", "nil_expr",
  "stmt_level_expr", "opt_task_intent_ls", "task_intent_ls", "intent_expr",
  "ref_var_ls", "io_expr", "new_expr", "let_expr", "expr", "lhs_expr",
  "fun_expr", "call_expr", "dot_expr", "parenthesized_expr",
  "literal_expr", "assoc_expr_ls", "binary_op_expr", "unary_op_expr",
  "reduce_expr", "scan_expr", "reduce_scan_op_expr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   132,   133,   134,   134,   135,   135,   136,   136,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   138,   138,   139,   140,   140,
     141,   141,   142,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   143,   143,   143,   143,   143,   143,   144,   144,
     145,   146,   146,   147,   147,   148,   148,   148,   148,   148,
     148,   149,   150,   150,   150,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   150,   150,   150,   151,   152,
     152,   152,   152,   153,   154,   154,   155,   155,   156,   157,
     158,   158,   159,   159,   159,   160,   160,   161,   161,   161,
     162,   163,   163,   163,   164,   164,   166,   167,   165,   168,
     168,   168,   168,   168,   168,   170,   171,   169,   172,   172,
     172,   172,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   173,   173,
     173,   173,   173,   173,   173,   173,   173,   173,   174,   174,
     174,   174,   174,   174,   174,   174,   174,   174,   174,   174,
     175,   175,   176,   177,   177,   177,   178,   178,   178,   178,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     180,   180,   180,   181,   181,   182,   182,   182,   182,   182,
     182,   183,   183,   184,   184,   185,   185,   186,   186,   187,
     187,   188,   188,   189,   189,   189,   190,   190,   191,   191,
     191,   192,   192,   192,   192,   192,   193,   193,   193,   194,
     194,   195,   195,   195,   196,   196,   196,   197,   197,   197,
     198,   198,   198,   199,   199,   200,   200,   200,   200,   201,
     201,   201,   201,   202,   202,   202,   203,   203,   203,   203,
     203,   204,   204,   204,   204,   204,   204,   204,   205,   205,
     205,   205,   206,   206,   206,   207,   207,   208,   208,   209,
     209,   210,   210,   210,   210,   210,   211,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   214,   215,   216,   216,
     216,   216,   216,   216,   216,   216,   217,   217,   217,   218,
     218,   219,   220,   220,   221,   221,   222,   223,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   225,   225,   225,   225,   226,   226,   227,   227,
     227,   228,   228,   228,   229,   229,   229,   230,   230,   230,
     230,   230,   230,   230,   231,   231,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   233,
     233,   233,   233,   233,   233,   234,   234,   234,   234,   235,
     235,   235,   235,   236,   236,   236,   236,   236,   236,   236
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     3,     3,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     3,     1,     4,     5,     1,     2,     3,
       1,     2,     3,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     0,     1,
       1,     2,     1,     2,     3,     1,     1,     1,     1,     1,
       1,     2,     5,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     5,     3,     6,     6,     4,     4,     4,
       3,     6,     5,     5,     0,     2,     3,     2,     6,     7,
       0,     1,     1,     1,     1,     0,     2,     0,     2,     3,
       5,     1,     2,     3,     1,     3,     0,     0,     8,     0,
       1,     1,     2,     1,     2,     0,     0,     9,     3,     3,
       5,     5,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     3,     3,     0,     1,     3,     4,     4,     6,     6,
       0,     1,     1,     1,     1,     2,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     0,     1,     2,
       2,     0,     2,     3,     4,     4,     2,     4,     0,     2,
       2,     4,     4,     4,     5,     4,     0,     1,     1,     1,
       3,     3,     4,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     0,     4,     0,     2,     2,     1,     4,
       4,     6,     1,     0,     1,     1,     3,     4,     3,     4,
       6,     0,     2,     2,     2,     2,     2,     2,     1,     1,
       3,     3,     1,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     4,     4,     4,     5,     2,     2,     6,
       6,     4,     9,     9,     7,     6,     6,     4,     9,     9,
       7,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     0,     4,     4,     1,
       3,     2,     1,     3,     1,     3,     2,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     3,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     4,     4,
       4,     3,     3,     3,     3,     4,     3,     1,     1,     1,
       1,     3,     3,     3,     3,     5,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       3,     0,     0,     1,    34,    60,   369,   367,   368,   370,
       0,   326,    58,   326,     0,   227,    58,     0,     0,     0,
       0,   121,   228,     0,     0,     0,     0,   120,     0,   116,
       0,     0,     0,    37,     0,   317,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   415,   417,     0,   418,   419,   351,     0,     0,   416,
     413,    65,   414,     0,     0,     0,     4,     0,     5,     9,
       0,    10,    11,    13,   286,    19,    12,    14,    16,    15,
      17,    18,    67,     0,    68,   343,     0,    66,    69,    70,
       0,   352,   339,   340,   289,   287,     0,     0,   344,   345,
       0,   288,     0,   353,   354,   355,   338,   291,   290,   341,
     342,     0,    21,   297,     0,     0,     0,     0,    59,     0,
       0,     0,     0,     0,     0,     0,     0,   343,   352,   287,
     344,   345,   326,   288,   353,   354,     0,     0,     0,     0,
     277,     0,   122,    71,     0,   124,     0,     0,     0,     0,
       0,     0,   277,     0,     0,     0,     0,     0,   229,    28,
     401,   336,     0,   402,     7,   277,    63,     0,     0,     0,
      62,    31,     0,   292,     0,   277,    32,   298,     0,   218,
       0,   206,   269,     0,   268,     0,     0,   404,   350,   400,
     403,   399,    38,    40,     0,     0,   272,     0,   274,     0,
       0,   273,     0,   268,     0,     0,     6,     0,   102,   103,
     104,     0,   194,   193,   125,     0,     0,     0,     0,    20,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   349,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   277,   277,     0,     0,     0,   180,
      22,    23,     0,    24,     0,     0,     0,     0,     0,     0,
       0,    25,    26,     0,   286,   284,     0,   278,   279,   285,
       0,     0,     0,     0,    80,     0,     0,    79,     0,     0,
      84,     0,    90,     0,    27,   173,   117,   234,     0,   235,
     237,     0,   248,     0,     0,   240,     0,     0,    29,     0,
      64,    94,    61,    30,   277,     0,     0,     0,   216,   213,
     205,     0,    42,    73,    33,    39,    41,   371,     0,     0,
     364,     0,   366,     0,     0,     0,     0,   373,     8,     0,
     105,   190,     0,     0,     0,     0,     0,   325,   396,   395,
     398,   406,   405,   410,   409,   392,   389,   390,   391,   347,
     379,   363,   362,   361,   348,   383,   394,   388,   386,   397,
     387,   385,   377,   382,   384,   393,   376,   380,   381,   378,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   408,   407,   412,
     411,   332,     0,   184,   181,   182,   183,   187,   188,   189,
       0,     0,   329,     0,     0,     0,     0,     0,     0,   372,
     326,   326,    76,   214,     0,     0,     0,   294,     0,   114,
       0,   111,   215,     0,     0,     0,   301,     0,     0,     0,
     307,     0,     0,    89,     0,     0,   293,     0,   174,     0,
     195,     0,   238,     0,   252,     0,   247,   339,     0,     0,
     231,   337,   230,   360,     0,     0,   295,    94,     0,   220,
     339,     0,   271,   270,   346,   365,   275,   276,     0,     0,
      87,   311,   374,     0,    35,     0,     0,     0,   191,   192,
     126,     0,     0,   222,   221,   225,   223,    43,    50,    51,
      52,    47,    49,    56,    57,    45,    48,    46,    44,    54,
      53,    55,   358,   359,     0,   327,   185,   186,   331,   180,
     328,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    72,   283,   281,   282,   280,     0,   112,   110,     0,
      88,     0,     0,    78,     0,    77,     0,     0,    83,     0,
      82,     0,     0,    92,   180,   172,     0,   261,   196,   199,
     198,   200,   197,     0,   236,   239,     0,   240,     0,   232,
     241,   242,     0,     0,    93,    95,   296,     0,     0,   217,
       0,     0,     0,    36,   106,   107,   195,   155,   153,   158,
     165,   166,   167,   162,   164,   160,   163,   161,   159,   169,
     168,   134,   137,   135,   136,   147,   138,   151,   143,   141,
     154,   142,   140,   145,   150,   152,   139,   144,   148,   149,
     146,   156,   157,   132,   170,   170,   224,   333,   330,     0,
       0,     0,     0,     0,     0,     0,     0,    75,    74,   115,
     113,    81,     0,     0,   300,     0,   299,     0,     0,   306,
       0,   305,    91,   316,   175,     0,     0,   240,   211,   233,
       0,     0,    97,     0,    98,     0,     0,    86,   313,     0,
      85,   312,   375,   226,     0,   133,     0,   173,   128,   129,
       0,     0,     0,     0,     0,     0,     0,     0,   304,     0,
       0,   310,     0,     0,   261,   264,   265,   266,     0,   263,
     267,   339,   207,   177,   176,     0,     0,     0,   250,   339,
      96,     0,     0,   228,     0,    99,   226,   108,   211,   132,
     170,   170,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   240,   253,     0,   208,   210,   209,   212,
     203,   204,   118,     0,     0,     0,   109,     0,   130,   131,
     171,     0,     0,     0,     0,     0,     0,   303,   302,   309,
     308,   179,   178,   255,   256,   258,   339,     0,   372,   339,
     315,   314,   201,   127,   202,     0,   257,   259,   253,   260
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,   193,    67,   453,    69,    70,    71,   194,
      72,    73,   117,    74,   171,    75,    76,    77,    78,   537,
      79,    80,   474,   585,    81,    82,    83,   211,   497,   683,
      84,   440,   441,   127,   154,   460,    86,    87,   351,   596,
     500,   634,   635,   688,   306,   457,   458,   459,   501,   214,
     573,   783,   752,   182,   747,   713,   716,    88,   180,   328,
      89,    90,   157,   158,   310,   311,   470,   314,   315,   466,
     774,   710,   667,   195,   199,   200,   286,   287,   288,   128,
      92,    93,    94,   129,    96,   116,   421,   422,   412,    97,
     130,   131,   100,   133,   102,   134,   135,   105,   106,   204,
     107,   108,   109,   110,   111
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -661
static const yytype_int16 yypact[] =
{
    -661,   149,  1937,  -661,  -661,  -661,  -661,  -661,  -661,  -661,
    2992,   214,   166,   214,  5796,   108,   166,  5796,  2992,    59,
     166,   166,   287,  4828,  5796,  5796,    70,  -661,   166,  -661,
      18,  2992,  5796,  -661,  5796,  -661,  5796,  5796,   215,   171,
    5008,  5796,  5133,  5796,   230,   200,  2992,    32,  5212,  5796,
    5796,  -661,  -661,  5796,  -661,  -661,  6507,  5796,  5796,  -661,
    5796,  -661,  -661,  2099,  4626,  5212,  -661,  2867,  -661,  -661,
     166,  -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,
    -661,  -661,  -661,   245,  -661,   153,    27,  -661,  -661,  -661,
     133,   220,  -661,  -661,  -661,   221,   226,   227,   238,   242,
   10453,  6572,   177,   259,   275,  -661,  -661,  -661,  -661,  -661,
    -661,   263,  -661, 10453,   268,   273,  2992,   282,  -661,   297,
    5796,  5796,  5796,  5796,  5796,  5212,  5212,   183,  -661,  -661,
    -661,  -661,  7974,   255,  -661,  -661,   166,   307,  8242,   355,
    5212,   315,  -661,  -661,   166,  -661,   166,   317,    12,  6898,
    6965,  7032,  5212,  2992,   318,     7,    60,    69,  -661,  -661,
     825,   340,  7099,   825,  -661,  5212,  -661,  8309,  7836,  2992,
    -661,  -661,  7099, 10453,   320,  5212,  -661, 10453,  5212,   367,
     332,   166,  -661,    68, 10453,  7099,  8376,   350,   507,   825,
     350,   825,  -661,  -661,  2367,    73,  -661,  5796,  -661,   -46,
     -41, 10453,    33,  8443,   -69,   444,  -661,   330,  -661,  -661,
    -661,   166,  -661,  -661,  -661,     8,    18,    18,    18,  -661,
    5796,  5796,  5796,  5796,  5313,  5313,  5796,  5796,  5796,  5796,
    5796,  5796,    75,  6507,  5796,  5796,  5796,  5796,  5796,  5796,
    5796,  5796,  5796,  5796,  5796,  5796,  5796,  5796,  5796,  5796,
    5796,  5796,  5796,  5796,  5796,  5796,  5796,  5796,  5796,  5796,
    5796,  5796,  5796,  5796,  5212,  5212,  5313,  5313,   166,   483,
    -661,  -661,  2492,  -661,  8041,  8108,  8510,    46,  5313,    12,
     339,  -661,  -661,  5796,   352,  -661,   331,   371,  -661, 10453,
     166,   348,   440,  5212,  -661,  3117,  5313,  -661,  3242,  5313,
    -661,  2992,   448,   354,  -661,    71,  -661,  -661,     7,  -661,
     376,   361,  -661,  4367,   404,   413,  5796,    18,  -661,   369,
    -661,  -661,  -661,  -661,  5212,   370,   -22,  4446,   395,  -661,
    -661,  5212,  -661,  -661,  -661,  -661,  -661,  -661,  6831,  4727,
    -661,  4907,  -661,  5313,  2242,  5796,  5796,  -661,  -661,  2617,
     401,   219,    18,    84,    88,   162,   164,  7903, 10487, 10487,
     266,  -661,   266,  -661,   266, 10555,   994,   885,   725,   340,
     350,  -661,  -661,  -661,   507, 10589,   266,   290,   290, 10487,
     290,   290,   617,   350, 10589, 10521,   617,   825,   825,   350,
    8577,  8644,  8711,  8778,  8845,  8912,  8979,  9046,  9113,  9180,
    9247,  9314,  9381,  9448,  9515,   373,   372,  -661,   266,  -661,
     266,  -661,   -38,    10,  -661,  -661,  -661,  -661,  -661,  -661,
     166,    65,  -661,  5875,  5313,  5954,  5313,  5796,  5313,  1655,
     214,  8175,  -661,  -661,  9582,  5796,  5212,  -661,  5212,   423,
      74,  -661,  -661,  5796,    78,  5796, 10453,    23,  7166,  5796,
   10453,    29,  7233,   482,  9649,  2992,  -661,   167,  -661,    20,
     309,   381,     7,    91,  -661,  5212,  -661,   244,  5796,  5414,
    -661, 10453,  -661,  -661,    17,   384,  -661,  -661,  5212,  -661,
     186,   166,  -661, 10453,  -661,  -661,  -661,  -661,   383,  1700,
    -661, 10453, 10453,  9716,  -661,  2742,  5212,   389,  -661,  -661,
    -661,  1770,   211,  -661,  -661,  -661,  -661,  -661,  -661,  -661,
    -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,
    -661,  -661,  -661,  -661,   166,  -661,  -661,  -661,  -661,   483,
    -661,  5796,   491,  9783,  5796,   493,  9850,   388,  6629,    12,
      12,  -661, 10453,  -661, 10453,  -661,  5796,   166,  -661,  7099,
    -661,  7300,  3367,  -661,  3492,  -661,  7367,  3617,  -661,  3742,
    -661,  2992,  5796,  -661,   483,  -661,     7,   421,  -661,  -661,
    -661,  -661,  -661,    67,  -661,  -661,  4446,   413,    50, 10453,
    -661, 10453,  2992,  5212,  -661,  -661,  -661,    26,    55,  -661,
    3867,  3992,  5796,  -661,   422,  -661,   309,  -661,  -661,  -661,
    -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,
    -661,  -661,   166,  -661,  -661,  -661,  -661,  -661,  -661,  -661,
    -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,  -661,
    -661,  -661,  -661,   425,   397,   397,  -661,  -661,  -661,  9917,
    6033,  6112,  9984,  6191,  6270,  6349,  6428,  -661,  -661, 10453,
    -661,  -661,  2992,  5796, 10453,  5796, 10453,  2992,  5796, 10453,
    5796, 10453,  -661, 10453,  -661,   400,  5493,   101,   461,  -661,
    5313,  4117,  -661,     6,  -661,  4242,  5796,  -661, 10453,  5796,
    -661, 10453, 10453,  5027,    11,  -661,  1770,    71,  -661,  -661,
    5796,  5796,  5796,  5796,  5796,  5796,  5796,  5796,  9649,  7434,
    7501,  9649,  7568,  7635,   421,    59,  5796,  5796,  4525,  -661,
    -661,   277,  5212,  -661,  -661,  5796,   138,  6697,  -661,   365,
    -661,  7702,  7769,   438,   166,  -661,  4645,  -661,   461,  -661,
     397,   397,   173, 10051, 10118, 10185, 10252, 10319, 10386,  2992,
    2992,  2992,  2992,   101,  5594,    64,  -661,  -661, 10453, 10453,
    -661,  -661,  -661,  6428,  2992,  2992,  -661,   -14,  -661,  -661,
    -661,  5796,  5796,  5796,  5796,  5796,  5796,  9649,  9649,  9649,
    9649,  -661,  -661,  -661,  -661,  -661,   286,  5313,  6586,   406,
    9649,  9649,  -661,  -661,  -661,  6764,  -661,  -661,  5695,  -661
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -661,  -661,  -661,    -1,  -150,  1120,  -661,  -661,   860,   185,
    -661,  -661,   521,  1606,   -79,  -660,  -527,  -661,  -661,    52,
    -661,  -661,    62,  -661,  -661,  -661,  -661,  -661,  -661,  -661,
    -661,  -661,    -7,   314,  -661,  -661,  -661,  -661,  -661,  -661,
    -661,  -145,  -141,  -610,  -661,  -137,   -18,  -266,  -661,  -661,
     -45,  -661,  -204,   636,  -661,  -189,  -173,  -661,  -128,  -661,
    -661,  -661,  -198,   240,  -661,  -302,  -568,  -661,  -426,  -325,
    -506,  -619,  -146,     1,    93,  -661,   -58,  -661,   121,   295,
    -269,  -661,  -661,   525,  -661,    -8,  -661,    31,  -661,  -661,
     775,   830,   -10,   849,  -661,   909,  1178,  -661,  -661,  -661,
    -661,  -661,  -661,  -661,  -661
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -373
static const yytype_int16 yytable[] =
{
     113,    66,   479,   420,   132,   119,   461,   138,   280,   669,
       5,     5,   312,   149,   150,   151,   291,   353,   354,   355,
     356,     5,   160,     5,   161,   689,   162,   163,   169,   346,
     167,   168,   172,   173,   169,     5,   177,   577,   184,   185,
     186,   526,    40,   187,   467,   552,   188,   189,   190,   183,
     191,   557,   339,   184,   201,   203,   751,   341,   480,   347,
     524,   312,   212,   582,   343,   527,   202,  -245,   312,   294,
     297,   300,   582,   307,  -180,   148,   331,   428,     5,   213,
     340,   670,  -245,   318,   352,   342,   670,   583,   525,   413,
     178,  -245,   312,   323,   303,   777,   583,   751,   371,   714,
     316,   477,   414,   782,   331,   415,   333,   319,   576,   272,
     113,   274,   275,   276,   177,   184,   203,   325,   416,   417,
     758,   759,  -245,  -245,   279,   775,   418,   277,  -245,   272,
     289,   331,   308,   155,  -245,   272,   419,  -243,  -245,  -245,
     372,   584,   289,   155,   331,   566,   272,   668,   331,     3,
     674,   215,   272,   331,   502,   289,   727,   313,  -245,   787,
     575,   344,   331,   529,   576,   289,   331,   317,   184,     5,
    -245,   331,   547,   136,   429,   772,   331,  -245,   671,   326,
     469,   216,   317,   675,   140,   332,   317,   338,   576,  -245,
    -245,   530,   778,   336,    40,   152,  -180,   337,   548,   756,
     432,   503,   217,   712,   550,   504,   405,   406,  -245,   218,
     357,   358,   359,   360,   362,   364,   365,   366,   367,   368,
     369,   370,   164,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   289,   289,   408,   410,   728,   208,
     317,   272,   317,   420,   665,   564,   475,   498,   431,   114,
    -322,   564,   786,   434,   499,  -246,   361,   363,  -357,   505,
    -357,   506,   789,   184,  -219,   446,   448,   115,   450,   452,
       5,   454,   174,   565,   444,   143,   165,    91,   209,   760,
    -246,  -101,   264,  -219,   265,    91,   471,   467,  -357,   317,
    -357,   223,   210,    91,   289,  -246,    85,   266,   407,   409,
     267,   483,  -123,  -246,    85,   175,    91,   568,   636,   201,
     430,   201,    85,   489,   491,   492,   493,  -319,  -318,  -123,
    -101,    91,  -246,   219,   224,    85,   718,   225,   447,   220,
     718,   451,   144,   589,  -101,  -323,  -262,   569,    91,  -324,
      85,  -246,    91,   230,   570,  -254,   232,  -246,   553,   555,
     235,   223,   558,   560,   571,  -262,  -321,    85,   572,  -262,
    -356,    85,  -356,   227,  -254,   228,   229,   230,  -254,   231,
     232,   233,  -320,   268,   235,   488,  -249,   711,   269,   271,
     241,   242,   719,  -262,   224,   245,   719,   225,   246,   247,
     248,    91,  -254,   446,   533,   450,   536,   454,   538,   491,
     272,  -249,   539,   540,   281,   542,   544,   283,   289,   435,
      85,   436,   486,   549,   487,   551,  -249,  -251,   290,   556,
     232,     5,   293,   305,  -249,   324,   327,   230,    91,   329,
     232,   348,  -101,   349,   235,   203,   433,   437,   579,   581,
     647,   648,  -251,  -249,    91,   442,   578,    85,   203,   438,
     651,   443,   455,  -123,   462,   776,   532,  -251,   535,   588,
     456,   468,  -249,    85,   779,  -251,   184,   463,  -249,    91,
    -123,  -101,   469,   481,   336,   473,   476,   594,   496,   522,
     523,   413,   546,   144,  -251,  -101,   561,   574,    85,   776,
     586,   590,   595,   640,   414,   643,   645,   415,   666,   776,
     331,   639,   687,  -251,   642,   686,   704,    95,   223,  -251,
     416,   417,   715,   726,   495,    95,   649,   137,   418,   587,
     650,   730,   654,    95,   656,   731,   664,   659,   419,   661,
     732,   684,   663,   784,   771,   757,    95,   472,   743,   545,
     638,   224,     0,   411,   225,     0,     0,    91,     0,     0,
       0,    95,     0,   184,     0,     0,     0,     0,     0,     0,
     678,   681,   682,     0,   673,     0,    85,     0,    95,     0,
      91,     0,    95,    91,   720,     0,    91,     0,     0,     0,
     227,     0,   228,   229,   230,     0,   231,   232,     0,    85,
       0,   235,    85,     0,     0,    85,     0,   241,   242,     0,
       0,     0,   245,     0,     0,   246,   247,   248,     0,     0,
     654,   656,     0,   659,   661,   678,   681,     0,   223,    91,
       0,    95,   698,   699,    91,   700,     0,   701,   702,     0,
     703,     0,     0,     0,     0,     0,     0,     0,    85,     0,
     717,   491,     0,    85,     0,   491,   721,     0,     0,   722,
       0,   224,     0,     0,   225,     0,     0,     0,    95,     0,
     698,   733,   734,   701,   735,   736,   737,   738,     0,     0,
       0,     0,     0,     0,    95,     0,   173,   177,   203,     0,
     198,     0,   748,     0,     0,   749,     0,     0,     0,   745,
     227,     0,   228,   229,   230,   528,   231,   232,     0,    95,
       0,   235,     0,     0,     0,     0,     0,     0,   242,   767,
     768,   769,   770,     0,     0,   246,   247,   248,     0,     0,
       0,     0,     0,   681,   780,   781,   223,     0,     0,     0,
      91,   767,   768,   769,   770,   780,   781,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   785,   491,    85,
       0,     0,     0,     0,     0,     0,   285,    98,   681,   224,
       0,     0,   225,     0,     0,    98,     0,     0,   285,     0,
      91,     0,     0,    98,     0,     0,     0,    95,     0,     0,
       0,   285,     0,     0,     0,     0,    98,     0,     0,    85,
       0,   285,     0,     0,     0,     0,     0,     0,   227,   637,
      95,    98,   230,    95,   231,   232,    95,     0,     0,   235,
       0,     0,    99,     0,     0,     0,   242,     0,    98,     0,
      99,     0,    98,   246,   247,   248,   223,    91,    99,    91,
       0,   101,    91,     0,    91,     0,    91,     0,     0,   101,
       0,    99,     0,     0,     0,     0,    85,   101,    85,    95,
       0,    85,     0,    85,    95,    85,    99,    91,     0,   224,
     101,     0,   225,     0,     0,    91,    91,     0,     0,     0,
       0,    98,     0,    99,     0,   101,    85,    99,     0,     0,
     285,   285,   170,     0,    85,    85,   223,     0,     0,     0,
       0,   103,   101,     0,     0,     0,   101,     0,     0,   103,
       0,     0,   230,     0,   231,   232,     0,   103,    98,   235,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   224,
     103,     0,   225,     0,    98,   248,    99,    91,     0,     0,
       0,     0,    91,     0,     0,   103,     0,     0,     0,     0,
     285,     0,     0,     0,     0,   101,    85,   482,     0,    98,
       0,    85,   103,     0,     0,   198,   103,   198,   227,   273,
      95,   229,   230,    99,   231,   232,     0,     0,     0,   235,
       0,     0,     0,     0,     0,     0,   242,     0,     0,    99,
       0,     0,   101,   246,   247,   248,     0,     0,   170,   170,
     170,   302,     0,     0,     0,   223,     0,     0,   101,     0,
      95,     0,   170,     0,    99,   103,     0,     0,     0,     0,
       0,     0,   170,     0,    91,    91,    91,    91,     0,     0,
       0,     0,     0,   101,     0,   170,     0,    98,   224,    91,
      91,   225,     0,    85,    85,    85,    85,     0,     0,     0,
       0,     0,   103,     0,     0,     0,     0,     0,    85,    85,
      98,     0,   543,    98,   285,     0,    98,    95,   103,    95,
       0,     0,    95,     0,    95,     0,    95,     0,     0,     0,
       0,   230,     0,   231,   232,     0,     0,     0,   235,     0,
       0,     0,    99,   103,     0,   242,     0,    95,     0,     0,
       0,     0,   246,   247,   248,    95,    95,     0,     0,    98,
       0,   101,    68,     0,    98,    99,     0,     0,    99,     0,
     112,    99,     0,     0,     0,     0,     0,     0,   139,   170,
       0,     0,     0,     0,   101,     0,     0,   101,     0,     0,
     101,   159,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   176,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,    95,     0,    99,
     104,   103,    95,    68,     0,     0,     0,   206,   104,     0,
       0,     0,     0,   101,     0,     0,   104,     0,   101,     0,
       0,     0,     0,     0,   103,     0,     0,   103,     0,   104,
     103,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,   270,     0,     0,     0,
       0,   104,     0,     0,     0,   104,     0,     0,     0,     0,
       0,     0,     0,   103,     0,     0,     0,     0,   103,     0,
       0,     0,     0,     0,    95,    95,    95,    95,     0,     0,
      98,     0,     0,   304,     0,     0,     0,     0,     0,    95,
      95,     0,     0,     0,     0,    99,     0,     0,     0,   322,
       0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
       0,     0,   709,     0,   101,     0,     0,   170,   170,     0,
       0,   170,   170,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,    98,     0,    98,
       0,   104,    98,     0,    98,     0,    98,     0,     0,     0,
       0,     0,     0,     0,   101,     0,     0,   104,   746,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,   103,    98,    98,     0,     0,     0,
       0,     0,   104,     0,     0,     0,     0,     0,     0,     0,
     773,     0,    99,     0,    99,     0,     0,    99,     0,    99,
       0,    99,    68,     0,     0,     0,     0,     0,     0,   170,
     170,   101,     0,   101,   103,     0,   101,     0,   101,   170,
     101,   302,    99,     0,   773,   322,   302,     0,   322,     0,
      99,    99,     0,     0,   773,     0,     0,    98,     0,     0,
       0,   101,    98,     0,     0,     0,     0,     0,     0,   101,
     101,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   103,     0,   103,   490,     0,   103,     0,   103,    68,
     103,     0,     0,   104,     0,     0,   104,     0,     0,   104,
       0,     0,    99,     0,     0,     0,     0,    99,     0,     0,
       0,   103,     0,     0,     0,     0,     0,     0,     0,   103,
     103,   101,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,    98,    98,    98,    98,     0,     0,
       0,     0,   104,     0,     0,     0,     0,   104,     0,    98,
      98,     0,     0,   170,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   302,
     302,   103,   302,   302,     0,     0,   103,     0,     0,    99,
      99,    99,    99,     0,     0,   563,   750,     0,     0,     0,
       0,   302,   302,     0,    99,    99,     0,     0,   101,   101,
     101,   101,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   101,   101,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    68,     0,   750,   118,     0,
       0,     0,   118,     0,     0,     0,   141,   142,   145,     0,
       0,     0,     0,   104,   153,     0,   156,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   103,   103,
     103,   103,     0,   179,     0,     0,     0,     0,     5,     6,
       7,     8,     9,   103,   103,   120,     0,     0,     0,     0,
       0,     0,   322,   104,   322,     0,   207,   322,    19,   322,
       0,   662,     0,   121,   122,   123,     0,    26,     0,     0,
       0,     0,    29,    30,     0,    32,     0,    34,    35,     0,
       0,     0,   672,     0,    37,     0,    39,     0,     0,   221,
     677,   680,     0,   222,     0,    43,    44,    45,   124,     0,
       0,   223,     0,     0,     0,     0,     0,     0,     0,     0,
     104,     0,   104,     0,     0,   104,     0,   104,     0,   104,
       0,     0,   179,     0,     0,     0,   284,     0,     0,    53,
     179,     0,   292,     0,   224,     0,     0,   225,   284,     0,
     104,   309,     0,     0,     0,     0,     0,    58,   104,   104,
       0,   284,     0,     5,     0,     0,     0,     0,   226,   597,
      64,   284,   126,   598,     0,     0,     0,   330,     0,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,   350,   246,   247,
     248,   156,   156,   156,   156,     0,     0,     0,   591,     0,
     104,     0,     0,     0,     0,   104,     0,     0,   373,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   599,
     600,   601,   602,   603,   604,     0,     0,   605,   606,   607,
     608,   609,   610,   611,   612,   613,   614,     0,     0,   615,
     284,   284,     0,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,     0,   627,     0,     0,   628,   629,
     630,   631,   632,     0,     0,     0,   439,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,     0,     0,   104,   104,   104,
     104,     0,     0,   156,     0,     0,     0,     0,     0,     0,
     284,     0,   104,   104,     0,     0,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -100,    13,    14,    15,  -226,    16,    17,   156,    18,
      19,     0,    20,    21,    22,    23,    24,    25,     0,    26,
      27,     0,  -119,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -226,    37,    38,    39,  -119,
    -100,     0,     0,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -100,    48,  -226,     0,     0,    49,
       0,    50,     0,  -226,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,   284,     0,     0,    57,     0,    58,
       0,    59,    60,     0,    61,     0,     0,    62,     0,     0,
      63,     0,    64,     0,    65,   567,     0,     0,   309,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   179,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       4,     0,     5,     6,     7,     8,     9,   633,     0,    10,
      11,    12,     0,  -100,    13,    14,    15,  -226,    16,    17,
       0,    18,    19,     0,    20,    21,    22,    23,    24,    25,
       0,    26,    27,     0,  -119,    28,    29,    30,    31,    32,
      33,    34,    35,     0,    36,     0,     0,  -226,    37,    38,
      39,  -119,  -100,   439,     0,    40,     0,    41,    42,    43,
      44,    45,    46,     0,    47,     0,  -100,    48,  -226,     0,
       0,    49,   309,    50,     0,  -226,     0,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    52,    53,    54,    55,     0,     0,     0,     0,
      56,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    58,     0,    59,    60,   181,    61,     0,   685,    62,
       0,     0,    63,   192,    64,     0,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     4,     0,     5,     6,     7,     8,     9,
       0,  -372,    10,    11,    12,  -372,  -100,    13,    14,    15,
    -226,    16,    17,  -372,    18,    19,  -372,    20,    21,    22,
      23,    24,    25,     0,    26,    27,     0,  -119,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,     0,     0,
    -226,    37,   729,    39,  -119,  -100,  -372,     0,    40,  -372,
      41,    42,    43,    44,    45,    46,     0,    47,     0,  -100,
      48,  -226,     0,     0,    49,     0,    50,     0,  -226,     0,
    -372,     0,     0,     0,     0,     0,     0,     0,     0,   145,
     179,     0,     0,     0,     0,  -372,    53,  -372,  -372,  -372,
    -372,  -372,  -372,  -372,     0,  -372,  -372,  -372,  -372,  -372,
    -372,  -372,  -372,  -372,    58,  -372,  -372,  -372,     0,    61,
    -372,  -372,  -372,     0,     0,    63,  -372,    64,     4,    65,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -100,    13,    14,    15,  -226,    16,    17,     0,    18,
      19,     0,    20,    21,    22,    23,    24,    25,     0,    26,
      27,     0,  -119,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -226,    37,    38,    39,  -119,
    -100,     0,     0,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -100,    48,  -226,     0,     0,    49,
       0,    50,     0,  -226,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    58,
       0,    59,    60,     0,    61,     0,     0,    62,     0,     0,
      63,   335,    64,     4,    65,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,  -100,    13,    14,    15,
    -226,    16,    17,     0,    18,    19,     0,    20,    21,    22,
      23,    24,    25,     0,    26,    27,     0,  -119,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,     0,     0,
    -226,    37,    38,    39,  -119,  -100,     0,     0,    40,     0,
      41,    42,    43,    44,    45,    46,     0,    47,     0,  -100,
      48,  -226,     0,     0,    49,     0,    50,     0,  -226,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    58,     0,    59,    60,     0,    61,
       0,     0,    62,     0,     0,    63,   192,    64,     4,    65,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -100,    13,    14,    15,  -226,    16,    17,     0,    18,
      19,     0,    20,    21,    22,    23,    24,    25,     0,    26,
      27,     0,  -119,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -226,    37,    38,    39,  -119,
    -100,     0,     0,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -100,    48,  -226,     0,     0,    49,
       0,    50,     0,  -226,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    58,
       0,    59,    60,     0,    61,     0,     0,    62,     0,     0,
      63,   494,    64,     4,    65,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,  -100,    13,    14,    15,
    -226,    16,    17,     0,    18,    19,     0,    20,    21,    22,
      23,    24,    25,     0,    26,    27,     0,  -119,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,     0,     0,
    -226,    37,    38,    39,  -119,  -100,     0,     0,    40,     0,
      41,    42,    43,    44,    45,    46,     0,    47,     0,  -100,
      48,  -226,     0,     0,    49,     0,    50,     0,  -226,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    58,     0,    59,    60,     0,    61,
       0,     0,    62,     0,     0,    63,   593,    64,     4,    65,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -100,    13,    14,    15,  -226,    16,    17,     0,    18,
      19,     0,    20,    21,    22,    23,    24,    25,     0,    26,
      27,     0,  -119,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -226,    37,   205,    39,  -119,
    -100,     0,     0,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -100,    48,  -226,     0,     0,    49,
       0,    50,     0,  -226,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    58,
       0,    59,    60,     0,    61,     0,     0,    62,     0,     0,
      63,     0,    64,     4,    65,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,  -100,    13,    14,    15,
    -226,    16,    17,     0,    18,    19,     0,    20,    21,    22,
      23,    24,    25,     0,    26,    27,     0,  -119,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,     0,     0,
    -226,    37,     0,    39,  -119,  -100,     0,     0,    40,     0,
      41,    42,    43,    44,    45,    46,     0,    47,     0,  -100,
      48,  -226,     0,     0,    49,     0,    50,     0,  -226,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    58,     0,    59,    60,     0,    61,
       0,     0,    62,     0,     0,    63,     0,    64,     4,    65,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -100,    13,    14,    15,  -226,    16,    17,     0,    18,
      19,     0,    20,    21,    22,    23,    24,   445,     0,    26,
      27,     0,  -119,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -226,    37,     0,    39,  -119,
    -100,     0,     0,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -100,    48,  -226,     0,     0,    49,
       0,    50,     0,  -226,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    58,
       0,    59,    60,     0,    61,     0,     0,    62,     0,     0,
      63,     0,    64,     4,    65,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,  -100,    13,    14,    15,
    -226,    16,    17,     0,    18,    19,     0,    20,    21,    22,
      23,    24,   449,     0,    26,    27,     0,  -119,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,     0,     0,
    -226,    37,     0,    39,  -119,  -100,     0,     0,    40,     0,
      41,    42,    43,    44,    45,    46,     0,    47,     0,  -100,
      48,  -226,     0,     0,    49,     0,    50,     0,  -226,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    58,     0,    59,    60,     0,    61,
       0,     0,    62,     0,     0,    63,     0,    64,     4,    65,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -100,    13,    14,    15,  -226,    16,    17,     0,    18,
      19,     0,    20,    21,    22,    23,    24,   653,     0,    26,
      27,     0,  -119,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -226,    37,     0,    39,  -119,
    -100,     0,     0,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -100,    48,  -226,     0,     0,    49,
       0,    50,     0,  -226,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    58,
       0,    59,    60,     0,    61,     0,     0,    62,     0,     0,
      63,     0,    64,     4,    65,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,  -100,    13,    14,    15,
    -226,    16,    17,     0,    18,    19,     0,    20,    21,    22,
      23,    24,   655,     0,    26,    27,     0,  -119,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,     0,     0,
    -226,    37,     0,    39,  -119,  -100,     0,     0,    40,     0,
      41,    42,    43,    44,    45,    46,     0,    47,     0,  -100,
      48,  -226,     0,     0,    49,     0,    50,     0,  -226,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    58,     0,    59,    60,     0,    61,
       0,     0,    62,     0,     0,    63,     0,    64,     4,    65,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -100,    13,    14,    15,  -226,    16,    17,     0,    18,
      19,     0,    20,    21,    22,    23,    24,   658,     0,    26,
      27,     0,  -119,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -226,    37,     0,    39,  -119,
    -100,     0,     0,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -100,    48,  -226,     0,     0,    49,
       0,    50,     0,  -226,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    58,
       0,    59,    60,     0,    61,     0,     0,    62,     0,     0,
      63,     0,    64,     4,    65,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,  -100,    13,    14,    15,
    -226,    16,    17,     0,    18,    19,     0,    20,    21,    22,
      23,    24,   660,     0,    26,    27,     0,  -119,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,     0,     0,
    -226,    37,     0,    39,  -119,  -100,     0,     0,    40,     0,
      41,    42,    43,    44,    45,    46,     0,    47,     0,  -100,
      48,  -226,     0,     0,    49,     0,    50,     0,  -226,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    58,     0,    59,    60,     0,    61,
       0,     0,    62,     0,     0,    63,     0,    64,     4,    65,
       5,     6,     7,     8,     9,     0,     0,    10,    11,    12,
       0,  -100,    13,    14,    15,  -226,    16,    17,     0,    18,
      19,     0,    20,    21,    22,    23,    24,   676,     0,    26,
      27,     0,  -119,    28,    29,    30,    31,    32,    33,    34,
      35,     0,    36,     0,     0,  -226,    37,     0,    39,  -119,
    -100,     0,     0,    40,     0,    41,    42,    43,    44,    45,
      46,     0,    47,     0,  -100,    48,  -226,     0,     0,    49,
       0,    50,     0,  -226,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    58,
       0,    59,    60,     0,    61,     0,     0,    62,     0,     0,
      63,     0,    64,     4,    65,     5,     6,     7,     8,     9,
       0,     0,    10,    11,    12,     0,  -100,    13,    14,    15,
    -226,    16,    17,     0,    18,    19,     0,    20,    21,    22,
      23,    24,   679,     0,    26,    27,     0,  -119,    28,    29,
      30,    31,    32,    33,    34,    35,     0,    36,     0,     0,
    -226,    37,     0,    39,  -119,  -100,     0,     0,    40,     0,
      41,    42,    43,    44,    45,    46,     0,    47,     0,  -100,
      48,  -226,     0,     0,    49,     0,    50,     0,  -226,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    58,     0,    59,    60,     0,    61,
       0,     0,    62,     0,     0,    63,     0,    64,   464,    65,
       5,     6,     7,     8,     9,     0,  -372,   120,     0,     0,
    -372,     0,     0,     0,     0,     0,     0,     0,  -372,     0,
      19,     0,     0,     0,     0,   121,   122,   123,     0,    26,
       0,     0,     0,     0,    29,    30,     0,    32,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,  -372,     0,     0,  -372,     0,     0,    43,    44,    45,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -244,  -372,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -372,    53,  -372,  -372,  -372,     0,  -372,  -372,  -372,     0,
    -372,  -372,  -372,  -372,  -372,  -372,  -372,  -372,  -372,    58,
    -372,  -372,  -372,     0,     0,  -372,  -372,  -372,     0,     0,
     125,     0,    64,   464,   478,     5,     6,     7,     8,     9,
       0,  -372,   120,     0,     0,  -372,     0,     0,     0,     0,
       0,     0,     0,  -372,     0,    19,     0,     0,     0,     0,
     121,   122,   123,     0,    26,     0,     0,     0,     0,    29,
      30,     0,    32,     0,    34,    35,     0,     0,     0,     0,
       0,    37,     0,    39,     0,     0,  -372,     0,     0,  -372,
       0,     0,    43,    44,    45,   124,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -372,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -372,    53,  -372,  -372,  -372,
       0,  -372,  -372,  -372,     0,  -372,  -372,  -372,  -372,  -372,
    -372,  -372,  -372,  -372,    58,  -372,  -372,  -372,     0,     0,
    -372,  -372,  -372,     0,     0,   125,     0,    64,   464,   478,
       5,     6,     7,     8,     9,     0,     0,   120,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,   121,   122,   123,     0,    26,
       0,     0,     0,     0,    29,    30,     0,    32,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,    45,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,   464,     0,     5,
       6,     7,     8,     9,     0,     0,   120,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,    19,
       0,     0,     0,     0,   121,   122,   123,    57,    26,    58,
       0,    59,    60,    29,    30,     0,    32,    62,    34,    35,
     125,     0,    64,     0,   465,    37,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,    43,    44,    45,   124,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   120,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,    19,     0,
       0,     0,     0,   121,   122,   123,    57,    26,    58,     0,
      59,    60,    29,    30,     0,    32,    62,    34,    35,   125,
       0,    64,     0,   478,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,    45,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,    53,
      54,    55,     0,     0,     0,     0,    56,     0,     0,     5,
       6,     7,     8,     9,     0,    57,   120,    58,     0,    59,
      60,   181,     0,     0,     0,    62,     0,     0,   125,    19,
      64,     0,   126,   744,   121,   122,   123,     0,    26,  -100,
       0,     0,    15,    29,    30,     0,    32,     0,    34,    35,
      20,    21,   723,     0,     0,    37,     0,    39,    27,     0,
    -119,     0,     0,     0,     0,     0,    43,    44,    45,   124,
       0,     0,   196,     0,     0,   205,     0,  -119,  -100,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     0,     0,
     724,     0,  -100,     0,     0,     0,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,   197,     0,
       5,     6,     7,     8,     9,     0,    57,   120,    58,     0,
      59,    60,   181,     0,     0,     0,    62,     0,     0,   125,
      19,    64,     0,   126,     0,   121,   122,   123,     0,    26,
       0,     0,    61,     0,    29,    30,     0,    32,     0,    34,
      35,     0,     0,     0,     0,     0,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,    45,
     124,     0,     0,   196,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,     0,
       0,     5,     6,     7,     8,     9,     0,    57,   120,    58,
       0,    59,    60,   181,     0,     0,     0,    62,     0,     0,
     125,    19,    64,   485,   126,     0,   121,   122,   123,     0,
      26,     0,     0,     0,     0,    29,    30,     0,    32,     0,
      34,    35,     0,     0,     0,     0,   146,    37,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,    43,    44,
      45,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   147,     0,     0,    51,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   120,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
      19,     0,     0,     0,     0,   121,   122,   123,    57,    26,
      58,     0,    59,    60,    29,    30,     0,    32,    62,    34,
      35,   125,     0,    64,     0,   126,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,    45,
     124,     0,     0,   196,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,    56,     0,
       0,     5,     6,     7,     8,     9,     0,    57,   120,    58,
       0,    59,    60,   181,     0,     0,     0,    62,     0,     0,
     125,    19,    64,     0,   126,     0,   121,   122,   123,     0,
      26,  -100,     0,     0,    15,    29,    30,     0,    32,     0,
      34,    35,    20,    21,   723,     0,     0,    37,     0,    39,
      27,     0,  -119,     0,     0,     0,     0,     0,    43,    44,
      45,   124,     0,     0,     0,     0,     0,    38,     0,  -119,
    -100,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       0,     0,   724,     0,  -100,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
       0,     0,     0,     0,     0,     0,     0,     0,    57,     0,
      58,     0,    59,    60,     0,   166,     0,     0,    62,     0,
       0,   125,     0,    64,     0,   126,     5,     6,     7,     8,
       9,     0,     0,   120,    61,     0,     0,     0,     0,     0,
       0,   725,     0,     0,     0,   169,    19,     0,     0,     0,
       0,   121,   122,   123,     0,    26,     0,     0,     0,     0,
      29,    30,     0,    32,     0,    34,    35,     0,     0,     0,
       0,     0,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,    43,    44,    45,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     5,     6,     7,     8,     9,
       0,     0,   120,     0,     0,     0,    52,    53,    54,    55,
       0,     0,     0,     0,    56,    19,     0,     0,     0,     0,
     121,   122,   123,    57,    26,    58,     0,    59,    60,    29,
      30,     0,    32,    62,    34,    35,    63,     0,    64,     0,
     126,    37,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,    43,    44,    45,   124,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,     0,     0,     5,     6,     7,     8,
       9,     0,    57,   120,    58,     0,    59,    60,   181,     0,
       0,     0,    62,     0,     0,   125,    19,    64,     0,   126,
       0,   121,   122,   123,     0,    26,     0,     0,     0,     0,
      29,    30,     0,    32,     0,    34,    35,     0,     0,     0,
       0,     0,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,    43,    44,    45,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   147,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    53,    54,    55,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,     0,    57,   120,    58,     0,    59,    60,     0,
       0,     0,     0,    62,     0,     0,   125,    19,    64,     0,
     126,     0,   121,   122,   123,     0,    26,     0,     0,     0,
       0,    29,    30,     0,    32,     0,    34,    35,   580,     0,
       0,     0,     0,    37,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,    43,    44,    45,   124,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     5,     6,     7,     8,
       9,     0,     0,   120,     0,     0,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    56,   705,     0,     0,     0,
       0,   121,   122,   123,    57,    26,    58,     0,    59,    60,
      29,    30,     0,    32,    62,    34,    35,   125,     0,    64,
       0,   126,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,   706,    44,    45,   707,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    53,    54,    55,
       0,     0,     0,     0,    56,     0,     0,     5,     6,     7,
       8,     9,     0,    57,   120,    58,     0,    59,    60,   181,
       0,     0,     0,    62,     0,     0,   125,    19,    64,     0,
     708,     0,   121,   122,   123,     0,    26,     0,     0,     0,
       0,    29,    30,     0,    32,     0,    34,    35,     0,     0,
       0,     0,     0,    37,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,    43,    44,    45,   124,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    56,     0,     0,     5,     6,
       7,     8,     9,     0,    57,   120,    58,     0,    59,    60,
     181,     0,     0,     0,    62,     0,     0,   125,    19,    64,
       0,   708,     0,   121,   122,   697,     0,    26,     0,     0,
       0,     0,    29,    30,     0,    32,     0,    34,    35,     0,
       0,     0,     0,     0,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,    45,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    52,    53,
      54,    55,     0,     0,     0,     0,    56,     0,     0,     5,
       6,     7,     8,     9,     0,    57,   120,    58,     0,    59,
      60,   181,     0,     0,     0,    62,     0,     0,   125,    19,
      64,     0,   126,     0,   121,   122,   123,     0,    26,     0,
       0,     0,     0,    29,    30,     0,    32,     0,    34,    35,
       0,     0,     0,     0,     0,    37,     0,    39,     0,     0,
       0,     0,     0,     0,     0,     0,    43,    44,    45,   124,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,     0,     0,     0,     5,     6,
       7,     8,     9,     0,     0,   120,     0,     0,     0,    52,
      53,    54,    55,     0,     0,     0,     0,    56,    19,     0,
       0,     0,     0,   121,   122,   531,    57,    26,    58,     0,
      59,    60,    29,    30,     0,    32,    62,    34,    35,   125,
       0,    64,     0,   126,    37,     0,    39,     0,     0,     0,
       0,     0,     0,     0,     0,    43,    44,    45,   124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     5,     6,     7,
       8,     9,     0,     0,   120,     0,     0,     0,    52,    53,
      54,    55,     0,     0,     0,     0,    56,    19,     0,     0,
       0,     0,   121,   122,   534,    57,    26,    58,     0,    59,
      60,    29,    30,     0,    32,    62,    34,    35,   125,     0,
      64,     0,   126,    37,     0,    39,     0,     0,     0,     0,
       0,     0,     0,     0,    43,    44,    45,   124,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    51,     0,     0,     0,     5,     6,     7,     8,
       9,     0,     0,   120,     0,     0,     0,    52,    53,    54,
      55,     0,     0,     0,     0,    56,    19,     0,     0,     0,
       0,   121,   122,   691,    57,    26,    58,     0,    59,    60,
      29,    30,     0,    32,    62,    34,    35,   125,     0,    64,
       0,   126,    37,     0,    39,     0,     0,     0,     0,     0,
       0,     0,     0,    43,    44,    45,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,     0,     0,     0,     5,     6,     7,     8,     9,
       0,     0,   120,     0,     0,     0,    52,    53,    54,    55,
       0,     0,     0,     0,    56,    19,     0,     0,     0,     0,
     121,   122,   692,    57,    26,    58,     0,    59,    60,    29,
      30,     0,    32,    62,    34,    35,   125,     0,    64,     0,
     126,    37,     0,    39,     0,     0,     0,     0,     0,     0,
       0,     0,    43,    44,    45,   124,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     5,     6,     7,     8,     9,     0,
       0,   120,     0,     0,     0,    52,    53,    54,    55,     0,
       0,     0,     0,    56,    19,     0,     0,     0,     0,   121,
     122,   694,    57,    26,    58,     0,    59,    60,    29,    30,
       0,    32,    62,    34,    35,   125,     0,    64,     0,   126,
      37,     0,    39,     0,     0,     0,     0,     0,     0,     0,
       0,    43,    44,    45,   124,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     5,     6,     7,     8,     9,     0,     0,
     120,     0,     0,     0,    52,    53,    54,    55,     0,     0,
       0,     0,    56,    19,     0,     0,     0,     0,   121,   122,
     695,    57,    26,    58,     0,    59,    60,    29,    30,     0,
      32,    62,    34,    35,   125,     0,    64,     0,   126,    37,
       0,    39,     0,     0,     0,     0,     0,     0,     0,     0,
      43,    44,    45,   124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    51,     0,
       0,     0,     5,     6,     7,     8,     9,     0,     0,   120,
       0,     0,     0,    52,    53,    54,    55,     0,     0,     0,
       0,    56,    19,     0,     0,     0,     0,   121,   122,   696,
      57,    26,    58,     0,    59,    60,    29,    30,     0,    32,
      62,    34,    35,   125,     0,    64,     0,   126,    37,     0,
      39,     0,     0,     0,     0,     0,     0,     0,     0,    43,
      44,    45,   124,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    51,     0,     0,
       0,     5,     6,     7,     8,     9,     0,     0,   120,     0,
       0,     0,    52,    53,    54,    55,     0,     0,     0,     0,
      56,    19,     0,     0,     0,     0,   121,   122,   697,    57,
      26,    58,     0,    59,    60,    29,    30,     0,    32,    62,
      34,    35,   125,     0,    64,     0,   126,    37,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,    43,    44,
      45,   124,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    51,     0,     0,     0,
       5,     6,     7,     8,     9,     0,     0,   120,     0,     0,
       0,    52,    53,    54,    55,     0,     0,     0,     0,    56,
      19,     0,     0,     0,     0,   121,   122,   123,    57,    26,
      58,     0,    59,    60,    29,    30,     0,    32,    62,    34,
      35,   125,     0,    64,     0,   126,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,    43,    44,    45,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       6,     7,     8,     9,     0,     0,   120,     0,     0,     0,
      52,    53,    54,    55,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,   121,   122,   123,    57,    26,    58,
       0,     0,    60,    29,    30,     0,    32,    62,    34,    35,
     125,     0,    64,     0,   126,    37,     0,    39,   221,     0,
       0,     0,   222,     0,     0,     0,    43,    44,    45,   124,
     223,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,  -253,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,     0,     0,   224,  -253,     0,   225,     0,  -253,     0,
       0,     0,     0,   263,  -334,     0,     0,  -356,    58,  -356,
       0,     0,   181,     0,     0,     0,   221,   226,     0,   125,
     222,    64,  -253,   708,     0,     0,     0,     0,   223,     0,
       0,     0,   227,     0,   228,   229,   230,     0,   231,   232,
     233,     0,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,   243,   244,   245,     0,     0,   246,   247,   248,
       0,   224,     0,     0,   225,     0,     0,   646,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   221,     0,   226,     0,   222,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
     227,     0,   228,   229,   230,     0,   231,   232,   233,     0,
     234,   235,   236,   237,   238,   239,   240,   241,   242,     0,
     243,   244,   245,     0,     0,   246,   247,   248,   224,     0,
       0,   225,     0,     0,     0,   753,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,   788,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   221,     0,   226,
       0,   222,     0,     0,     0,     0,     0,     0,     0,   223,
     295,     0,     0,     0,   227,     0,   228,   229,   230,   296,
     231,   232,   233,     0,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,   243,   244,   245,     0,     0,   246,
     247,   248,   224,     0,     0,   225,     0,   484,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,   298,     0,     0,
       0,   227,     0,   228,   229,   230,   299,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,     0,   246,   247,   248,   224,
       0,   272,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   221,     0,   226,     0,   222,     0,     0,     0,     0,
       0,     0,     0,   223,     0,     0,     0,     0,   227,     0,
     228,   229,   230,     0,   231,   232,   233,     0,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,   243,   244,
     245,     0,     0,   246,   247,   248,   224,     0,   272,   225,
       0,     0,     0,     0,     0,     0,   301,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,   169,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,     0,
     246,   247,   248,   224,     0,   272,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   221,     0,   226,     0,   222,
       0,     0,     0,     0,     0,     0,     0,   223,   554,     0,
       0,     0,   227,     0,   228,   229,   230,     0,   231,   232,
     233,     0,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,   243,   244,   245,     0,     0,   246,   247,   248,
     224,     0,   272,   225,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,   559,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,     0,   246,   247,   248,   224,     0,   272,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   221,
       0,   226,     0,   222,     0,     0,     0,     0,     0,     0,
       0,   223,     0,     0,     0,     0,   227,     0,   228,   229,
     230,     0,   231,   232,   233,     0,   234,   235,   236,   237,
     238,   239,   240,   241,   242,     0,   243,   244,   245,     0,
       0,   246,   247,   248,   224,     0,   272,   225,     0,     0,
       0,     0,     0,     0,   652,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248,   224,     0,   272,   225,     0,     0,     0,     0,     0,
       0,   657,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   221,     0,   226,     0,   222,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
     227,     0,   228,   229,   230,     0,   231,   232,   233,     0,
     234,   235,   236,   237,   238,   239,   240,   241,   242,     0,
     243,   244,   245,     0,     0,   246,   247,   248,   224,     0,
     272,   225,     0,     0,     0,     0,     0,     0,   739,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,   272,   225,     0,
       0,     0,     0,     0,     0,   740,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   221,     0,   226,
       0,   222,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,     0,   227,     0,   228,   229,   230,     0,
     231,   232,   233,     0,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,   243,   244,   245,     0,     0,   246,
     247,   248,   224,     0,   272,   225,     0,     0,     0,     0,
       0,     0,   741,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,     0,   246,   247,   248,   224,
       0,   272,   225,     0,     0,     0,     0,     0,     0,   742,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   221,     0,   226,     0,   222,     0,     0,     0,     0,
       0,     0,     0,   223,     0,     0,     0,     0,   227,     0,
     228,   229,   230,     0,   231,   232,   233,     0,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,   243,   244,
     245,     0,     0,   246,   247,   248,   224,     0,   272,   225,
       0,     0,     0,     0,     0,     0,   754,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,     0,
     246,   247,   248,   224,     0,   272,   225,     0,     0,     0,
       0,     0,     0,   755,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   221,     0,   226,     0,   222,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,   227,     0,   228,   229,   230,     0,   231,   232,
     233,     0,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,   243,   244,   245,     0,     0,   246,   247,   248,
     224,     0,   272,   225,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,     0,   246,   247,   248,   224,     0,   321,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   226,     0,   221,     0,     0,     0,   222,     0,     0,
       0,     0,     0,     0,     0,   223,   227,     0,   228,   229,
     230,     0,   231,   232,   233,   278,   234,   235,   236,   237,
     238,   239,   240,   241,   242,     0,   243,   244,   245,     0,
       0,   246,   247,   248,     0,  -335,     0,     0,   224,   114,
       0,   225,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   115,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,   423,     0,     0,     0,   227,     0,   228,
     229,   230,   424,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   221,     0,   226,
       0,   222,     0,     0,     0,     0,     0,     0,     0,   223,
     425,     0,     0,     0,   227,     0,   228,   229,   230,   426,
     231,   232,   233,     0,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,   243,   244,   245,     0,     0,   246,
     247,   248,   224,     0,     0,   225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,     0,   246,   247,   248,   224,
     114,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   115,     0,
       0,   221,     0,   226,     0,   222,     0,     0,     0,     0,
       0,     0,     0,   223,     0,     0,     0,     0,   227,     0,
     228,   229,   230,     0,   231,   232,   233,     0,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,   243,   244,
     245,     0,     0,   246,   247,   248,   224,     0,     0,   225,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,   282,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   221,     0,   226,     0,   222,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,   227,     0,   228,   229,   230,     0,   231,   232,
     233,     0,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,   243,   244,   245,     0,   320,   246,   247,   248,
     224,     0,     0,   225,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,   334,   246,   247,   248,   224,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   221,
     345,   226,     0,   222,     0,     0,     0,     0,     0,     0,
       0,   223,     0,     0,     0,     0,   227,     0,   228,   229,
     230,     0,   231,   232,   233,     0,   234,   235,   236,   237,
     238,   239,   240,   241,   242,     0,   243,   244,   245,     0,
       0,   246,   247,   248,   224,     0,     0,   225,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   221,     0,   226,     0,   222,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
     227,     0,   228,   229,   230,     0,   231,   232,   233,     0,
     234,   235,   236,   237,   238,   239,   240,   241,   242,     0,
     243,   244,   245,     0,   507,   246,   247,   248,   224,     0,
       0,   225,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,   508,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   221,     0,   226,
       0,   222,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,     0,   227,     0,   228,   229,   230,     0,
     231,   232,   233,     0,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,   243,   244,   245,     0,   509,   246,
     247,   248,   224,     0,     0,   225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,   510,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   221,     0,   226,     0,   222,     0,     0,     0,     0,
       0,     0,     0,   223,     0,     0,     0,     0,   227,     0,
     228,   229,   230,     0,   231,   232,   233,     0,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,   243,   244,
     245,     0,   511,   246,   247,   248,   224,     0,     0,   225,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,   512,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   221,     0,   226,     0,   222,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,   227,     0,   228,   229,   230,     0,   231,   232,
     233,     0,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,   243,   244,   245,     0,   513,   246,   247,   248,
     224,     0,     0,   225,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,   514,   246,   247,   248,   224,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   221,
       0,   226,     0,   222,     0,     0,     0,     0,     0,     0,
       0,   223,     0,     0,     0,     0,   227,     0,   228,   229,
     230,     0,   231,   232,   233,     0,   234,   235,   236,   237,
     238,   239,   240,   241,   242,     0,   243,   244,   245,     0,
     515,   246,   247,   248,   224,     0,     0,   225,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,   516,   246,   247,
     248,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   221,     0,   226,     0,   222,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
     227,     0,   228,   229,   230,     0,   231,   232,   233,     0,
     234,   235,   236,   237,   238,   239,   240,   241,   242,     0,
     243,   244,   245,     0,   517,   246,   247,   248,   224,     0,
       0,   225,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,   518,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   221,     0,   226,
       0,   222,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,     0,   227,     0,   228,   229,   230,     0,
     231,   232,   233,     0,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,   243,   244,   245,     0,   519,   246,
     247,   248,   224,     0,     0,   225,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,   520,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   221,     0,   226,     0,   222,     0,     0,     0,     0,
       0,     0,     0,   223,     0,     0,     0,     0,   227,     0,
     228,   229,   230,     0,   231,   232,   233,     0,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,   243,   244,
     245,     0,   521,   246,   247,   248,   224,     0,     0,   225,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,   562,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,   541,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   221,     0,   226,     0,   222,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,   227,     0,   228,   229,   230,     0,   231,   232,
     233,     0,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,   243,   244,   245,     0,     0,   246,   247,   248,
     224,     0,     0,   225,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,   592,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,   641,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,     0,   246,   247,   248,   224,     0,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   221,
       0,   226,     0,   222,     0,     0,     0,     0,     0,     0,
       0,   223,   644,     0,     0,     0,   227,     0,   228,   229,
     230,     0,   231,   232,   233,     0,   234,   235,   236,   237,
     238,   239,   240,   241,   242,     0,   243,   244,   245,     0,
       0,   246,   247,   248,   224,     0,     0,   225,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   221,     0,   226,     0,
     222,     0,     0,     0,     0,     0,     0,     0,   223,     0,
       0,     0,     0,   227,     0,   228,   229,   230,     0,   231,
     232,   233,     0,   234,   235,   236,   237,   238,   239,   240,
     241,   242,     0,   243,   244,   245,     0,     0,   246,   247,
     248,   224,     0,     0,   225,     0,     0,     0,     0,     0,
       0,   690,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   221,     0,   226,     0,   222,     0,     0,
       0,     0,     0,     0,     0,   223,     0,     0,     0,     0,
     227,     0,   228,   229,   230,     0,   231,   232,   233,     0,
     234,   235,   236,   237,   238,   239,   240,   241,   242,     0,
     243,   244,   245,     0,     0,   246,   247,   248,   224,     0,
       0,   225,     0,     0,     0,     0,     0,     0,   693,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     221,     0,   226,     0,   222,     0,     0,     0,     0,     0,
       0,     0,   223,     0,     0,     0,     0,   227,     0,   228,
     229,   230,     0,   231,   232,   233,     0,   234,   235,   236,
     237,   238,   239,   240,   241,   242,     0,   243,   244,   245,
       0,     0,   246,   247,   248,   224,     0,     0,   225,     0,
       0,     0,     0,     0,     0,   761,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   221,     0,   226,
       0,   222,     0,     0,     0,     0,     0,     0,     0,   223,
       0,     0,     0,     0,   227,     0,   228,   229,   230,     0,
     231,   232,   233,     0,   234,   235,   236,   237,   238,   239,
     240,   241,   242,     0,   243,   244,   245,     0,     0,   246,
     247,   248,   224,     0,     0,   225,     0,     0,     0,     0,
       0,     0,   762,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   221,     0,   226,     0,   222,     0,
       0,     0,     0,     0,     0,     0,   223,     0,     0,     0,
       0,   227,     0,   228,   229,   230,     0,   231,   232,   233,
       0,   234,   235,   236,   237,   238,   239,   240,   241,   242,
       0,   243,   244,   245,     0,     0,   246,   247,   248,   224,
       0,     0,   225,     0,     0,     0,     0,     0,     0,   763,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   221,     0,   226,     0,   222,     0,     0,     0,     0,
       0,     0,     0,   223,     0,     0,     0,     0,   227,     0,
     228,   229,   230,     0,   231,   232,   233,     0,   234,   235,
     236,   237,   238,   239,   240,   241,   242,     0,   243,   244,
     245,     0,     0,   246,   247,   248,   224,     0,     0,   225,
       0,     0,     0,     0,     0,     0,   764,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   221,     0,
     226,     0,   222,     0,     0,     0,     0,     0,     0,     0,
     223,     0,     0,     0,     0,   227,     0,   228,   229,   230,
       0,   231,   232,   233,     0,   234,   235,   236,   237,   238,
     239,   240,   241,   242,     0,   243,   244,   245,     0,     0,
     246,   247,   248,   224,     0,     0,   225,     0,     0,     0,
       0,     0,     0,   765,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   221,     0,   226,     0,   222,
       0,     0,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,   227,     0,   228,   229,   230,     0,   231,   232,
     233,     0,   234,   235,   236,   237,   238,   239,   240,   241,
     242,     0,   243,   244,   245,     0,     0,   246,   247,   248,
     224,     0,     0,   225,     0,     0,     0,     0,     0,     0,
     766,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   221,     0,   226,     0,   222,     0,     0,     0,
       0,     0,     0,     0,   223,     0,     0,     0,     0,   227,
       0,   228,   229,   230,     0,   231,   232,   233,     0,   234,
     235,   236,   237,   238,   239,   240,   241,   242,     0,   243,
     244,   245,     0,     0,   246,   247,   248,   224,   223,     0,
     225,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   226,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   224,   223,     0,   225,     0,   227,     0,   228,   229,
     230,     0,   231,   232,   233,     0,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   226,   243,   244,   245,     0,
       0,   246,   247,   248,     0,   224,   223,     0,   225,     0,
     227,     0,   228,   229,   230,     0,   231,   232,   233,     0,
     234,   235,   236,   237,     0,   239,   240,   241,   242,   226,
     243,   244,   245,     0,     0,   246,   247,   248,     0,   224,
     223,     0,   225,     0,   227,     0,   228,   229,   230,     0,
     231,   232,   233,     0,   234,   235,   236,   237,     0,   239,
     240,   241,   242,     0,   243,     0,   245,     0,     0,   246,
     247,   248,     0,   224,     0,     0,   225,     0,   227,     0,
     228,   229,   230,     0,   231,   232,   233,     0,   234,   235,
     236,   237,     0,   239,   240,   241,   242,     0,   243,     0,
     245,     0,     0,   246,   247,   248,     0,     0,     0,     0,
       0,     0,   227,     0,   228,   229,   230,     0,   231,   232,
     233,     0,     0,   235,   236,   237,     0,   239,   240,   241,
     242,     0,     0,     0,   245,     0,     0,   246,   247,   248
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-661))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      10,     2,   327,   269,    14,    13,   308,    17,   136,   577,
       3,     3,     1,    23,    24,    25,   144,   215,   216,   217,
     218,     3,    32,     3,    34,   635,    36,    37,    22,    98,
      40,    41,    42,    43,    22,     3,    46,   463,    48,    49,
      50,    31,    56,    53,   313,    22,    56,    57,    58,    48,
      60,    22,    98,    63,    64,    65,   716,    98,   327,   128,
      98,     1,    35,    46,    31,    55,    65,    56,     1,   148,
     149,   150,    46,    66,     3,    23,    98,    31,     3,    52,
     126,    31,    71,   162,    76,   126,    31,    70,   126,    18,
      58,    31,     1,   172,   152,    31,    70,   757,    23,   667,
      31,   123,    31,   117,    98,    34,   185,   165,    97,   123,
     120,   121,   122,   123,   124,   125,   126,   175,    47,    48,
     730,   731,    31,    56,   132,   744,    55,   126,   117,   123,
     140,    98,   125,   125,   123,   123,    65,    77,    71,    79,
      65,   124,   152,   125,    98,   125,   123,   573,    98,     0,
     124,    18,   123,    98,   352,   165,   683,    97,    98,   778,
     462,   128,    98,    98,    97,   175,    98,    98,   178,     3,
      79,    98,    98,    65,   128,   743,    98,   117,   128,   178,
      79,    48,    98,   128,   125,   117,    98,   197,    97,    98,
     123,   126,   128,   194,    56,   125,   125,   124,   124,   726,
     279,   117,    69,   102,   126,   117,   264,   265,   117,    76,
     220,   221,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,     7,   233,   234,   235,   236,   237,   238,   239,
     240,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   684,    14,
      98,   123,    98,   529,   566,    98,   324,    48,   278,    55,
     117,    98,   778,   283,    55,    31,   224,   225,   125,   117,
     127,   117,   788,   293,    98,   295,   296,    73,   298,   299,
       3,   301,    62,   126,   293,     8,   125,     2,    53,   126,
      56,    14,   125,   117,   127,    10,   316,   576,   125,    98,
     127,    21,    67,    18,   324,    71,     2,    54,   266,   267,
      57,   331,    35,    79,    10,   125,    31,    18,   117,   339,
     278,   341,    18,   343,   344,   345,   346,   117,   117,    52,
      53,    46,    98,   117,    54,    31,   671,    57,   296,   122,
     675,   299,    65,   481,    67,   117,    79,    48,    63,   117,
      46,   117,    67,    97,    55,    79,   100,   123,   447,   448,
     104,    21,   451,   452,    65,    98,   117,    63,    69,   102,
     125,    67,   127,    93,    98,    95,    96,    97,   102,    99,
     100,   101,   117,   125,   104,   343,    31,   666,   125,   117,
     110,   111,   671,   126,    54,   115,   675,    57,   118,   119,
     120,   116,   126,   423,   424,   425,   426,   427,   428,   429,
     123,    56,   430,   431,   117,   435,   436,    72,   438,    77,
     116,    79,   339,   443,   341,   445,    71,    31,   123,   449,
     100,     3,   125,   125,    79,   125,    79,    97,   153,   117,
     100,     7,    14,   123,   104,   465,   117,   126,   468,   469,
     539,   540,    56,    98,   169,   117,   465,   153,   478,    98,
     549,    31,    24,    35,    98,   744,   424,    71,   426,   478,
     126,    77,   117,   169,   753,    79,   496,   126,   123,   194,
      52,    53,    79,    98,   495,   126,   126,   496,    97,   126,
     128,    18,    79,    65,    98,    67,    24,   126,   194,   778,
     126,   128,   123,    22,    31,    22,   128,    34,    97,   788,
      98,   531,   125,   117,   534,   100,   126,     2,    21,   123,
      47,    48,    71,   683,   349,    10,   546,    16,    55,   477,
     547,   686,   552,    18,   554,   686,   564,   557,    65,   559,
     687,   596,   562,   757,   743,   728,    31,   317,   704,   438,
     529,    54,    -1,   268,    57,    -1,    -1,   272,    -1,    -1,
      -1,    46,    -1,   583,    -1,    -1,    -1,    -1,    -1,    -1,
     590,   591,   592,    -1,   583,    -1,   272,    -1,    63,    -1,
     295,    -1,    67,   298,   673,    -1,   301,    -1,    -1,    -1,
      93,    -1,    95,    96,    97,    -1,    99,   100,    -1,   295,
      -1,   104,   298,    -1,    -1,   301,    -1,   110,   111,    -1,
      -1,    -1,   115,    -1,    -1,   118,   119,   120,    -1,    -1,
     640,   641,    -1,   643,   644,   645,   646,    -1,    21,   344,
      -1,   116,   652,   653,   349,   655,    -1,   657,   658,    -1,
     660,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   344,    -1,
     670,   671,    -1,   349,    -1,   675,   676,    -1,    -1,   679,
      -1,    54,    -1,    -1,    57,    -1,    -1,    -1,   153,    -1,
     690,   691,   692,   693,   694,   695,   696,   697,    -1,    -1,
      -1,    -1,    -1,    -1,   169,    -1,   706,   707,   708,    -1,
      64,    -1,   712,    -1,    -1,   715,    -1,    -1,    -1,   708,
      93,    -1,    95,    96,    97,   420,    99,   100,    -1,   194,
      -1,   104,    -1,    -1,    -1,    -1,    -1,    -1,   111,   739,
     740,   741,   742,    -1,    -1,   118,   119,   120,    -1,    -1,
      -1,    -1,    -1,   753,   754,   755,    21,    -1,    -1,    -1,
     455,   761,   762,   763,   764,   765,   766,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   777,   778,   455,
      -1,    -1,    -1,    -1,    -1,    -1,   140,     2,   788,    54,
      -1,    -1,    57,    -1,    -1,    10,    -1,    -1,   152,    -1,
     495,    -1,    -1,    18,    -1,    -1,    -1,   272,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    31,    -1,    -1,   495,
      -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    93,   524,
     295,    46,    97,   298,    99,   100,   301,    -1,    -1,   104,
      -1,    -1,     2,    -1,    -1,    -1,   111,    -1,    63,    -1,
      10,    -1,    67,   118,   119,   120,    21,   552,    18,   554,
      -1,     2,   557,    -1,   559,    -1,   561,    -1,    -1,    10,
      -1,    31,    -1,    -1,    -1,    -1,   552,    18,   554,   344,
      -1,   557,    -1,   559,   349,   561,    46,   582,    -1,    54,
      31,    -1,    57,    -1,    -1,   590,   591,    -1,    -1,    -1,
      -1,   116,    -1,    63,    -1,    46,   582,    67,    -1,    -1,
     264,   265,    42,    -1,   590,   591,    21,    -1,    -1,    -1,
      -1,     2,    63,    -1,    -1,    -1,    67,    -1,    -1,    10,
      -1,    -1,    97,    -1,    99,   100,    -1,    18,   153,   104,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,    54,
      31,    -1,    57,    -1,   169,   120,   116,   652,    -1,    -1,
      -1,    -1,   657,    -1,    -1,    46,    -1,    -1,    -1,    -1,
     324,    -1,    -1,    -1,    -1,   116,   652,   331,    -1,   194,
      -1,   657,    63,    -1,    -1,   339,    67,   341,    93,   119,
     455,    96,    97,   153,    99,   100,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,    -1,   111,    -1,    -1,   169,
      -1,    -1,   153,   118,   119,   120,    -1,    -1,   148,   149,
     150,   151,    -1,    -1,    -1,    21,    -1,    -1,   169,    -1,
     495,    -1,   162,    -1,   194,   116,    -1,    -1,    -1,    -1,
      -1,    -1,   172,    -1,   739,   740,   741,   742,    -1,    -1,
      -1,    -1,    -1,   194,    -1,   185,    -1,   272,    54,   754,
     755,    57,    -1,   739,   740,   741,   742,    -1,    -1,    -1,
      -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,   754,   755,
     295,    -1,   436,   298,   438,    -1,   301,   552,   169,   554,
      -1,    -1,   557,    -1,   559,    -1,   561,    -1,    -1,    -1,
      -1,    97,    -1,    99,   100,    -1,    -1,    -1,   104,    -1,
      -1,    -1,   272,   194,    -1,   111,    -1,   582,    -1,    -1,
      -1,    -1,   118,   119,   120,   590,   591,    -1,    -1,   344,
      -1,   272,     2,    -1,   349,   295,    -1,    -1,   298,    -1,
      10,   301,    -1,    -1,    -1,    -1,    -1,    -1,    18,   279,
      -1,    -1,    -1,    -1,   295,    -1,    -1,   298,    -1,    -1,
     301,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   344,    -1,    -1,   652,    -1,   349,
       2,   272,   657,    63,    -1,    -1,    -1,    67,    10,    -1,
      -1,    -1,    -1,   344,    -1,    -1,    18,    -1,   349,    -1,
      -1,    -1,    -1,    -1,   295,    -1,    -1,   298,    -1,    31,
     301,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,    -1,
     455,    -1,    -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    67,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   344,    -1,    -1,    -1,    -1,   349,    -1,
      -1,    -1,    -1,    -1,   739,   740,   741,   742,    -1,    -1,
     495,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,   754,
     755,    -1,    -1,    -1,    -1,   455,    -1,    -1,    -1,   169,
      -1,    -1,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   666,    -1,   455,    -1,    -1,   447,   448,    -1,
      -1,   451,   452,    -1,   194,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   495,    -1,   552,    -1,   554,
      -1,   153,   557,    -1,   559,    -1,   561,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   495,    -1,    -1,   169,   712,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   582,    -1,    -1,
      -1,    -1,    -1,    -1,   455,   590,   591,    -1,    -1,    -1,
      -1,    -1,   194,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     744,    -1,   552,    -1,   554,    -1,    -1,   557,    -1,   559,
      -1,   561,   272,    -1,    -1,    -1,    -1,    -1,    -1,   539,
     540,   552,    -1,   554,   495,    -1,   557,    -1,   559,   549,
     561,   551,   582,    -1,   778,   295,   556,    -1,   298,    -1,
     590,   591,    -1,    -1,   788,    -1,    -1,   652,    -1,    -1,
      -1,   582,   657,    -1,    -1,    -1,    -1,    -1,    -1,   590,
     591,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     272,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   552,    -1,   554,   344,    -1,   557,    -1,   559,   349,
     561,    -1,    -1,   295,    -1,    -1,   298,    -1,    -1,   301,
      -1,    -1,   652,    -1,    -1,    -1,    -1,   657,    -1,    -1,
      -1,   582,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   590,
     591,   652,    -1,    -1,    -1,    -1,   657,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   739,   740,   741,   742,    -1,    -1,
      -1,    -1,   344,    -1,    -1,    -1,    -1,   349,    -1,   754,
     755,    -1,    -1,   673,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   699,
     700,   652,   702,   703,    -1,    -1,   657,    -1,    -1,   739,
     740,   741,   742,    -1,    -1,   455,   716,    -1,    -1,    -1,
      -1,   721,   722,    -1,   754,   755,    -1,    -1,   739,   740,
     741,   742,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   754,   755,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   495,    -1,   757,    12,    -1,
      -1,    -1,    16,    -1,    -1,    -1,    20,    21,    22,    -1,
      -1,    -1,    -1,   455,    28,    -1,    30,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   739,   740,
     741,   742,    -1,    47,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,     7,   754,   755,    10,    -1,    -1,    -1,    -1,
      -1,    -1,   552,   495,   554,    -1,    70,   557,    23,   559,
      -1,   561,    -1,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,   582,    -1,    49,    -1,    51,    -1,    -1,     9,
     590,   591,    -1,    13,    -1,    60,    61,    62,    63,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     552,    -1,   554,    -1,    -1,   557,    -1,   559,    -1,   561,
      -1,    -1,   136,    -1,    -1,    -1,   140,    -1,    -1,    94,
     144,    -1,   146,    -1,    54,    -1,    -1,    57,   152,    -1,
     582,   155,    -1,    -1,    -1,    -1,    -1,   112,   590,   591,
      -1,   165,    -1,     3,    -1,    -1,    -1,    -1,    78,     9,
     125,   175,   127,    13,    -1,    -1,    -1,   181,    -1,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,   211,   118,   119,
     120,   215,   216,   217,   218,    -1,    -1,    -1,   128,    -1,
     652,    -1,    -1,    -1,    -1,   657,    -1,    -1,   232,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    79,
      80,    81,    82,    83,    84,    -1,    -1,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    -1,    -1,    99,
     264,   265,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,    -1,   115,    -1,    -1,   118,   119,
     120,   121,   122,    -1,    -1,    -1,   290,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   308,    -1,    -1,   739,   740,   741,
     742,    -1,    -1,   317,    -1,    -1,    -1,    -1,    -1,    -1,
     324,    -1,   754,   755,    -1,    -1,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,   352,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    50,    51,    52,
      53,    -1,    -1,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,   438,    -1,    -1,   110,    -1,   112,
      -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
     123,    -1,   125,    -1,   127,   459,    -1,    -1,   462,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   481,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,     3,     4,     5,     6,     7,   501,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    -1,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    -1,    45,    -1,    -1,    48,    49,    50,
      51,    52,    53,   547,    -1,    56,    -1,    58,    59,    60,
      61,    62,    63,    -1,    65,    -1,    67,    68,    69,    -1,
      -1,    72,   566,    74,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,   112,    -1,   114,   115,   116,   117,    -1,   612,   120,
      -1,    -1,   123,   124,   125,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,     4,     5,     6,     7,
      -1,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,   686,    51,    52,    53,    54,    -1,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    65,    -1,    67,
      68,    69,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   723,
     724,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,   117,
     118,   119,   120,    -1,    -1,   123,   124,   125,     1,   127,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    50,    51,    52,
      53,    -1,    -1,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
      -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
     123,   124,   125,     1,   127,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,    50,    51,    52,    53,    -1,    -1,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    -1,    67,
      68,    69,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,   123,   124,   125,     1,   127,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    50,    51,    52,
      53,    -1,    -1,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
      -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
     123,   124,   125,     1,   127,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,    50,    51,    52,    53,    -1,    -1,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    -1,    67,
      68,    69,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,   123,   124,   125,     1,   127,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    50,    51,    52,
      53,    -1,    -1,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
      -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
     123,    -1,   125,     1,   127,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    53,    -1,    -1,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    -1,    67,
      68,    69,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,   123,    -1,   125,     1,   127,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    -1,    -1,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
      -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
     123,    -1,   125,     1,   127,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    53,    -1,    -1,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    -1,    67,
      68,    69,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,   123,    -1,   125,     1,   127,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    -1,    -1,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
      -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
     123,    -1,   125,     1,   127,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    53,    -1,    -1,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    -1,    67,
      68,    69,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,   123,    -1,   125,     1,   127,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    -1,    -1,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
      -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
     123,    -1,   125,     1,   127,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    53,    -1,    -1,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    -1,    67,
      68,    69,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,   123,    -1,   125,     1,   127,
       3,     4,     5,     6,     7,    -1,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    -1,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    -1,    45,    -1,    -1,    48,    49,    -1,    51,    52,
      53,    -1,    -1,    56,    -1,    58,    59,    60,    61,    62,
      63,    -1,    65,    -1,    67,    68,    69,    -1,    -1,    72,
      -1,    74,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,   112,
      -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,
     123,    -1,   125,     1,   127,     3,     4,     5,     6,     7,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    -1,    25,    26,    27,
      28,    29,    30,    -1,    32,    33,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    -1,    45,    -1,    -1,
      48,    49,    -1,    51,    52,    53,    -1,    -1,    56,    -1,
      58,    59,    60,    61,    62,    63,    -1,    65,    -1,    67,
      68,    69,    -1,    -1,    72,    -1,    74,    -1,    76,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,   112,    -1,   114,   115,    -1,   117,
      -1,    -1,   120,    -1,    -1,   123,    -1,   125,     1,   127,
       3,     4,     5,     6,     7,    -1,     9,    10,    -1,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    54,    -1,    -1,    57,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    97,    -1,    99,   100,   101,    -1,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,   119,   120,    -1,    -1,
     123,    -1,   125,     1,   127,     3,     4,     5,     6,     7,
      -1,     9,    10,    -1,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    23,    -1,    -1,    -1,    -1,
      28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,    37,
      38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,    -1,
      -1,    49,    -1,    51,    -1,    -1,    54,    -1,    -1,    57,
      -1,    -1,    60,    61,    62,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,    -1,
     118,   119,   120,    -1,    -1,   123,    -1,   125,     1,   127,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,     1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    23,
      -1,    -1,    -1,    -1,    28,    29,    30,   110,    32,   112,
      -1,   114,   115,    37,    38,    -1,    40,   120,    42,    43,
     123,    -1,   125,    -1,   127,    49,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   110,    32,   112,    -1,
     114,   115,    37,    38,    -1,    40,   120,    42,    43,   123,
      -1,   125,    -1,   127,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    -1,    -1,   101,    -1,    -1,     3,
       4,     5,     6,     7,    -1,   110,    10,   112,    -1,   114,
     115,   116,    -1,    -1,    -1,   120,    -1,    -1,   123,    23,
     125,    -1,   127,   128,    28,    29,    30,    -1,    32,    14,
      -1,    -1,    17,    37,    38,    -1,    40,    -1,    42,    43,
      25,    26,    27,    -1,    -1,    49,    -1,    51,    33,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    -1,    66,    -1,    -1,    50,    -1,    52,    53,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      65,    -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,   102,    -1,
       3,     4,     5,     6,     7,    -1,   110,    10,   112,    -1,
     114,   115,   116,    -1,    -1,    -1,   120,    -1,    -1,   123,
      23,   125,    -1,   127,    -1,    28,    29,    30,    -1,    32,
      -1,    -1,   117,    -1,    37,    38,    -1,    40,    -1,    42,
      43,    -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,     3,     4,     5,     6,     7,    -1,   110,    10,   112,
      -1,   114,   115,   116,    -1,    -1,    -1,   120,    -1,    -1,
     123,    23,   125,   126,   127,    -1,    28,    29,    30,    -1,
      32,    -1,    -1,    -1,    -1,    37,    38,    -1,    40,    -1,
      42,    43,    -1,    -1,    -1,    -1,    48,    49,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    75,    -1,    -1,    78,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   110,    32,
     112,    -1,   114,   115,    37,    38,    -1,    40,   120,    42,
      43,   123,    -1,   125,    -1,   127,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,    -1,
      -1,     3,     4,     5,     6,     7,    -1,   110,    10,   112,
      -1,   114,   115,   116,    -1,    -1,    -1,   120,    -1,    -1,
     123,    23,   125,    -1,   127,    -1,    28,    29,    30,    -1,
      32,    14,    -1,    -1,    17,    37,    38,    -1,    40,    -1,
      42,    43,    25,    26,    27,    -1,    -1,    49,    -1,    51,
      33,    -1,    35,    -1,    -1,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,    -1,    -1,    -1,    -1,    50,    -1,    52,
      53,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    65,    -1,    67,    -1,    -1,    -1,    -1,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
     112,    -1,   114,   115,    -1,   117,    -1,    -1,   120,    -1,
      -1,   123,    -1,   125,    -1,   127,     3,     4,     5,     6,
       7,    -1,    -1,    10,   117,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    22,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    10,    -1,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,   101,    23,    -1,    -1,    -1,    -1,
      28,    29,    30,   110,    32,   112,    -1,   114,   115,    37,
      38,    -1,    40,   120,    42,    43,   123,    -1,   125,    -1,
     127,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,    -1,    -1,     3,     4,     5,     6,
       7,    -1,   110,    10,   112,    -1,   114,   115,   116,    -1,
      -1,    -1,   120,    -1,    -1,   123,    23,   125,    -1,   127,
      -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,    -1,
      37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,   101,    -1,    -1,     3,     4,     5,
       6,     7,    -1,   110,    10,   112,    -1,   114,   115,    -1,
      -1,    -1,    -1,   120,    -1,    -1,   123,    23,   125,    -1,
     127,    -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    -1,    42,    43,    44,    -1,
      -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,   110,    32,   112,    -1,   114,   115,
      37,    38,    -1,    40,   120,    42,    43,   123,    -1,   125,
      -1,   127,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,   101,    -1,    -1,     3,     4,     5,
       6,     7,    -1,   110,    10,   112,    -1,   114,   115,   116,
      -1,    -1,    -1,   120,    -1,    -1,   123,    23,   125,    -1,
     127,    -1,    28,    29,    30,    -1,    32,    -1,    -1,    -1,
      -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,    -1,
      -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    -1,    -1,     3,     4,
       5,     6,     7,    -1,   110,    10,   112,    -1,   114,   115,
     116,    -1,    -1,    -1,   120,    -1,    -1,   123,    23,   125,
      -1,   127,    -1,    28,    29,    30,    -1,    32,    -1,    -1,
      -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,    -1,
      -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    -1,    -1,   101,    -1,    -1,     3,
       4,     5,     6,     7,    -1,   110,    10,   112,    -1,   114,
     115,   116,    -1,    -1,    -1,   120,    -1,    -1,   123,    23,
     125,    -1,   127,    -1,    28,    29,    30,    -1,    32,    -1,
      -1,    -1,    -1,    37,    38,    -1,    40,    -1,    42,    43,
      -1,    -1,    -1,    -1,    -1,    49,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,    93,
      94,    95,    96,    -1,    -1,    -1,    -1,   101,    23,    -1,
      -1,    -1,    -1,    28,    29,    30,   110,    32,   112,    -1,
     114,   115,    37,    38,    -1,    40,   120,    42,    43,   123,
      -1,   125,    -1,   127,    49,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,     3,     4,     5,
       6,     7,    -1,    -1,    10,    -1,    -1,    -1,    93,    94,
      95,    96,    -1,    -1,    -1,    -1,   101,    23,    -1,    -1,
      -1,    -1,    28,    29,    30,   110,    32,   112,    -1,   114,
     115,    37,    38,    -1,    40,   120,    42,    43,   123,    -1,
     125,    -1,   127,    49,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    10,    -1,    -1,    -1,    93,    94,    95,
      96,    -1,    -1,    -1,    -1,   101,    23,    -1,    -1,    -1,
      -1,    28,    29,    30,   110,    32,   112,    -1,   114,   115,
      37,    38,    -1,    40,   120,    42,    43,   123,    -1,   125,
      -1,   127,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    61,    62,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    10,    -1,    -1,    -1,    93,    94,    95,    96,
      -1,    -1,    -1,    -1,   101,    23,    -1,    -1,    -1,    -1,
      28,    29,    30,   110,    32,   112,    -1,   114,   115,    37,
      38,    -1,    40,   120,    42,    43,   123,    -1,   125,    -1,
     127,    49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    62,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,
      -1,    10,    -1,    -1,    -1,    93,    94,    95,    96,    -1,
      -1,    -1,    -1,   101,    23,    -1,    -1,    -1,    -1,    28,
      29,    30,   110,    32,   112,    -1,   114,   115,    37,    38,
      -1,    40,   120,    42,    43,   123,    -1,   125,    -1,   127,
      49,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      10,    -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,
      -1,    -1,   101,    23,    -1,    -1,    -1,    -1,    28,    29,
      30,   110,    32,   112,    -1,   114,   115,    37,    38,    -1,
      40,   120,    42,    43,   123,    -1,   125,    -1,   127,    49,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      60,    61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    10,
      -1,    -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,
      -1,   101,    23,    -1,    -1,    -1,    -1,    28,    29,    30,
     110,    32,   112,    -1,   114,   115,    37,    38,    -1,    40,
     120,    42,    43,   123,    -1,   125,    -1,   127,    49,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      61,    62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,     3,     4,     5,     6,     7,    -1,    -1,    10,    -1,
      -1,    -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,
     101,    23,    -1,    -1,    -1,    -1,    28,    29,    30,   110,
      32,   112,    -1,   114,   115,    37,    38,    -1,    40,   120,
      42,    43,   123,    -1,   125,    -1,   127,    49,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,
      62,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
       3,     4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,
      -1,    93,    94,    95,    96,    -1,    -1,    -1,    -1,   101,
      23,    -1,    -1,    -1,    -1,    28,    29,    30,   110,    32,
     112,    -1,   114,   115,    37,    38,    -1,    40,   120,    42,
      43,   123,    -1,   125,    -1,   127,    49,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,    62,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    10,    -1,    -1,    -1,
      93,    94,    95,    96,    -1,    -1,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    28,    29,    30,   110,    32,   112,
      -1,    -1,   115,    37,    38,    -1,    40,   120,    42,    43,
     123,    -1,   125,    -1,   127,    49,    -1,    51,     9,    -1,
      -1,    -1,    13,    -1,    -1,    -1,    60,    61,    62,    63,
      21,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    -1,    -1,    54,    98,    -1,    57,    -1,   102,    -1,
      -1,    -1,    -1,   121,   122,    -1,    -1,   125,   112,   127,
      -1,    -1,   116,    -1,    -1,    -1,     9,    78,    -1,   123,
      13,   125,   126,   127,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,    -1,    -1,   118,   119,   120,
      -1,    54,    -1,    -1,    57,    -1,    -1,   128,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      93,    -1,    95,    96,    97,    -1,    99,   100,   101,    -1,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,    -1,    -1,   118,   119,   120,    54,    -1,
      -1,    57,    -1,    -1,    -1,   128,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    54,    -1,    -1,    57,    -1,
      -1,    -1,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    93,    -1,    95,    96,    97,    31,
      99,   100,   101,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,    -1,    -1,   118,
     119,   120,    54,    -1,    -1,    57,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    31,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    -1,   118,   119,   120,    54,
      -1,   123,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,
      95,    96,    97,    -1,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,    -1,    -1,   118,   119,   120,    54,    -1,   123,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      78,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    22,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,    -1,
     118,   119,   120,    54,    -1,   123,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,    -1,    -1,   118,   119,   120,
      54,    -1,   123,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    54,    -1,   123,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    78,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,
      97,    -1,    99,   100,   101,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,    -1,
      -1,   118,   119,   120,    54,    -1,   123,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    54,    -1,   123,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      93,    -1,    95,    96,    97,    -1,    99,   100,   101,    -1,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,    -1,    -1,   118,   119,   120,    54,    -1,
     123,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    54,    -1,   123,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,
      99,   100,   101,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,    -1,    -1,   118,
     119,   120,    54,    -1,   123,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    -1,   118,   119,   120,    54,
      -1,   123,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,
      95,    96,    97,    -1,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,    -1,    -1,   118,   119,   120,    54,    -1,   123,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      78,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,    -1,
     118,   119,   120,    54,    -1,   123,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,    -1,    -1,   118,   119,   120,
      54,    -1,   123,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    54,    -1,   123,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,     9,    -1,    -1,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    93,    -1,    95,    96,
      97,    -1,    99,   100,   101,    31,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,    -1,
      -1,   118,   119,   120,    -1,   122,    -1,    -1,    54,    55,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,
       9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    22,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    31,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    93,    -1,    95,    96,    97,    31,
      99,   100,   101,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,    -1,    -1,   118,
     119,   120,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    -1,   118,   119,   120,    54,
      55,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,
      95,    96,    97,    -1,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,    -1,    -1,   118,   119,   120,    54,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      78,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
      54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,    54,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      77,    78,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,
      97,    -1,    99,   100,   101,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,    -1,
      -1,   118,   119,   120,    54,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      93,    -1,    95,    96,    97,    -1,    99,   100,   101,    -1,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,    54,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,
      99,   100,   101,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,
      95,    96,    97,    -1,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,    54,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      78,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,    -1,   117,   118,   119,   120,
      54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,   117,   118,   119,   120,    54,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    78,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,
      97,    -1,    99,   100,   101,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,    -1,
     117,   118,   119,   120,    54,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,   117,   118,   119,
     120,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      93,    -1,    95,    96,    97,    -1,    99,   100,   101,    -1,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,    -1,   117,   118,   119,   120,    54,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,   117,   118,   119,   120,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,
      99,   100,   101,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,    -1,   117,   118,
     119,   120,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,   117,   118,   119,   120,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,
      95,    96,    97,    -1,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,    -1,   117,   118,   119,   120,    54,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      78,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    24,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,   117,
     118,   119,   120,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,    -1,    -1,   118,   119,   120,
      54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    77,    78,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    22,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    54,    -1,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,
      -1,    78,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    21,    22,    -1,    -1,    -1,    93,    -1,    95,    96,
      97,    -1,    99,   100,   101,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,    -1,
      -1,   118,   119,   120,    54,    -1,    -1,    57,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,
      -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,
     100,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,    -1,    -1,   118,   119,
     120,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,
      93,    -1,    95,    96,    97,    -1,    99,   100,   101,    -1,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,    -1,    -1,   118,   119,   120,    54,    -1,
      -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,    95,
      96,    97,    -1,    99,   100,   101,    -1,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
      -1,    -1,   118,   119,   120,    54,    -1,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,    78,
      -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,
      -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,    -1,
      99,   100,   101,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,    -1,    -1,   118,
     119,   120,    54,    -1,    -1,    57,    -1,    -1,    -1,    -1,
      -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    93,    -1,    95,    96,    97,    -1,    99,   100,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,    -1,    -1,   118,   119,   120,    54,
      -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    93,    -1,
      95,    96,    97,    -1,    99,   100,   101,    -1,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,    -1,    -1,   118,   119,   120,    54,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     9,    -1,
      78,    -1,    13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      21,    -1,    -1,    -1,    -1,    93,    -1,    95,    96,    97,
      -1,    99,   100,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,    -1,    -1,
     118,   119,   120,    54,    -1,    -1,    57,    -1,    -1,    -1,
      -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     9,    -1,    78,    -1,    13,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,   100,
     101,    -1,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,    -1,    -1,   118,   119,   120,
      54,    -1,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     9,    -1,    78,    -1,    13,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,    -1,    93,
      -1,    95,    96,    97,    -1,    99,   100,   101,    -1,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,    -1,    -1,   118,   119,   120,    54,    21,    -1,
      57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    54,    21,    -1,    57,    -1,    93,    -1,    95,    96,
      97,    -1,    99,   100,   101,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    78,   113,   114,   115,    -1,
      -1,   118,   119,   120,    -1,    54,    21,    -1,    57,    -1,
      93,    -1,    95,    96,    97,    -1,    99,   100,   101,    -1,
     103,   104,   105,   106,    -1,   108,   109,   110,   111,    78,
     113,   114,   115,    -1,    -1,   118,   119,   120,    -1,    54,
      21,    -1,    57,    -1,    93,    -1,    95,    96,    97,    -1,
      99,   100,   101,    -1,   103,   104,   105,   106,    -1,   108,
     109,   110,   111,    -1,   113,    -1,   115,    -1,    -1,   118,
     119,   120,    -1,    54,    -1,    -1,    57,    -1,    93,    -1,
      95,    96,    97,    -1,    99,   100,   101,    -1,   103,   104,
     105,   106,    -1,   108,   109,   110,   111,    -1,   113,    -1,
     115,    -1,    -1,   118,   119,   120,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    -1,    99,   100,
     101,    -1,    -1,   104,   105,   106,    -1,   108,   109,   110,
     111,    -1,    -1,    -1,   115,    -1,    -1,   118,   119,   120
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   133,   134,     0,     1,     3,     4,     5,     6,     7,
      10,    11,    12,    15,    16,    17,    19,    20,    22,    23,
      25,    26,    27,    28,    29,    30,    32,    33,    36,    37,
      38,    39,    40,    41,    42,    43,    45,    49,    50,    51,
      56,    58,    59,    60,    61,    62,    63,    65,    68,    72,
      74,    78,    93,    94,    95,    96,   101,   110,   112,   114,
     115,   117,   120,   123,   125,   127,   135,   136,   137,   138,
     139,   140,   142,   143,   145,   147,   148,   149,   150,   152,
     153,   156,   157,   158,   162,   165,   168,   169,   189,   192,
     193,   211,   212,   213,   214,   215,   216,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   232,   233,   234,
     235,   236,   137,   224,    55,    73,   217,   144,   145,   217,
      10,    28,    29,    30,    63,   123,   127,   165,   211,   215,
     222,   223,   224,   225,   227,   228,    65,   144,   224,   137,
     125,   145,   145,     8,    65,   145,    48,    75,   151,   224,
     224,   224,   125,   145,   166,   125,   145,   194,   195,   137,
     224,   224,   224,   224,     7,   125,   117,   224,   224,    22,
     140,   146,   224,   224,    62,   125,   137,   224,    58,   145,
     190,   116,   185,   205,   224,   224,   224,   224,   224,   224,
     224,   224,   124,   135,   141,   205,    66,   102,   185,   206,
     207,   224,   205,   224,   231,    50,   137,   145,    14,    53,
      67,   159,    35,    52,   181,    18,    48,    69,    76,   117,
     122,     9,    13,    21,    54,    57,    78,    93,    95,    96,
      97,    99,   100,   101,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   113,   114,   115,   118,   119,   120,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,   121,   125,   127,    54,    57,   125,   125,
     137,   117,   123,   140,   224,   224,   224,   205,    31,   217,
     190,   117,   117,    72,   145,   185,   208,   209,   210,   224,
     123,   190,   145,   125,   146,    22,    31,   146,    22,    31,
     146,    64,   140,   208,   137,   125,   176,    66,   125,   145,
     196,   197,     1,    97,   199,   200,    31,    98,   146,   208,
     117,   123,   137,   146,   125,   208,   205,    79,   191,   117,
     145,    98,   117,   146,   117,   124,   135,   124,   224,    98,
     126,    98,   126,    31,   128,    77,    98,   128,     7,   123,
     145,   170,    76,   194,   194,   194,   194,   224,   224,   224,
     224,   151,   224,   151,   224,   224,   224,   224,   224,   224,
     224,    23,    65,   145,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   208,   208,   151,   224,   151,
     224,   211,   220,    18,    31,    34,    47,    48,    55,    65,
     179,   218,   219,    22,    31,    22,    31,    64,    31,   128,
     151,   224,   146,   117,   224,    77,    79,   126,    98,   145,
     163,   164,   117,    31,   205,    30,   224,   151,   224,    30,
     224,   151,   224,   137,   224,    24,   126,   177,   178,   179,
     167,   197,    98,   126,     1,   127,   201,   212,    77,    79,
     198,   224,   195,   126,   154,   208,   126,   123,   127,   201,
     212,    98,   185,   224,   126,   126,   206,   206,   151,   224,
     137,   224,   224,   224,   124,   141,    97,   160,    48,    55,
     172,   180,   194,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     117,   117,   126,   128,    98,   126,    31,    55,   211,    98,
     126,    30,   151,   224,    30,   151,   224,   151,   224,   217,
     217,   117,   224,   185,   224,   210,    79,    98,   124,   224,
     126,   224,    22,   146,    22,   146,   224,    22,   146,    22,
     146,    24,    24,   137,    98,   126,   125,   145,    18,    48,
      55,    65,    69,   182,   126,   197,    97,   200,   205,   224,
      44,   224,    46,    70,   124,   155,   126,   154,   205,   190,
     128,   128,    77,   124,   205,   123,   171,     9,    13,    79,
      80,    81,    82,    83,    84,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    99,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   115,   118,   119,
     120,   121,   122,   145,   173,   174,   117,   211,   219,   224,
      22,    22,   224,    22,    22,   128,   128,   146,   146,   224,
     164,   146,    64,    30,   224,    30,   224,    64,    30,   224,
      30,   224,   137,   224,   178,   197,    97,   204,   200,   198,
      31,   128,   137,   205,   124,   128,    30,   137,   224,    30,
     137,   224,   224,   161,   182,   145,   100,   125,   175,   175,
      64,    30,    30,    64,    30,    30,    30,    30,   224,   224,
     224,   224,   224,   224,   126,    23,    60,    63,   127,   185,
     203,   212,   102,   187,   198,    71,   188,   224,   201,   212,
     146,   224,   224,    27,    65,   124,   136,   148,   200,   145,
     173,   174,   177,   224,   224,   224,   224,   224,   224,    64,
      64,    64,    64,   204,   128,   205,   185,   186,   224,   224,
     140,   147,   184,   128,    64,    64,   148,   188,   175,   175,
     126,    64,    64,    64,    64,    64,    64,   224,   224,   224,
     224,   187,   198,   185,   202,   203,   212,    31,   128,   212,
     224,   224,   117,   183,   184,   224,   202,   203,   128,   202
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
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
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
      int yyn = yypact[*yyssp];
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
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

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
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
      yychar = YYLEX;
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 219 "chapel.ypp"
    { (void)(yylsp[(1) - (1)]).first_line; yyblock = (yyval.pblockstmt); }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 224 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt();     resetTempID(); }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 225 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); resetTempID(); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 232 "chapel.ypp"
    { (yyval.pblockstmt) = buildPragmaStmt( (yyvsp[(1) - (2)].vpch), (yyvsp[(2) - (2)].pblockstmt) ); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 237 "chapel.ypp"
    { (yyval.vpch) = new Vec<const char*>(); (yyval.vpch)->add(astr((yyvsp[(2) - (2)].pch))); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 238 "chapel.ypp"
    { (yyvsp[(1) - (3)].vpch)->add(astr((yyvsp[(3) - (3)].pch))); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 254 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 255 "chapel.ypp"
    { (yyval.pblockstmt) = buildAtomicStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 256 "chapel.ypp"
    { (yyval.pblockstmt) = buildBeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 257 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_BREAK, (yyvsp[(2) - (3)].pch)); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 258 "chapel.ypp"
    { (yyval.pblockstmt) = buildCobeginStmt((yyvsp[(2) - (3)].pcallexpr), (yyvsp[(3) - (3)].pblockstmt));  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 259 "chapel.ypp"
    { (yyval.pblockstmt) = buildGotoStmt(GOTO_CONTINUE, (yyvsp[(2) - (3)].pch)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 260 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_DELETE, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 261 "chapel.ypp"
    { (yyval.pblockstmt) = buildLabelStmt((yyvsp[(2) - (3)].pch), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 262 "chapel.ypp"
    { (yyval.pblockstmt) = buildLocalStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 263 "chapel.ypp"
    { (yyval.pblockstmt) = buildOnStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 264 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 265 "chapel.ypp"
    { (yyval.pblockstmt) = buildSerialStmt(new SymExpr(gTrue), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 266 "chapel.ypp"
    { (yyval.pblockstmt) = buildSyncStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 267 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_YIELD, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 268 "chapel.ypp"
    { printf("syntax error"); clean_exit(1); }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 273 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(2) - (4)].pch), new BlockStmt(), yyfilename, (yylsp[(1) - (4)]).comment))); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 275 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(buildModule((yyvsp[(2) - (5)].pch), (yyvsp[(4) - (5)].pblockstmt), yyfilename, (yylsp[(1) - (5)]).comment))); }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 279 "chapel.ypp"
    { (yyloc).comment = yylloc.comment; yylloc.comment = NULL;}
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 294 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 295 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt);              }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 300 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); (yyval.pblockstmt)->appendChapelStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 301 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->appendChapelStmt((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 306 "chapel.ypp"
    { (yyval.pblockstmt) = buildUseStmt((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 310 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "=");   }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 311 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "+=");  }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 312 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "-=");  }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 313 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "*=");  }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 314 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "/=");  }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 315 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "%=");  }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 316 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "**="); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 317 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "&=");  }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 318 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "|=");  }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 319 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "^=");  }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 320 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), ">>="); }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 321 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<<="); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 322 "chapel.ypp"
    { (yyval.pblockstmt) = buildAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr), "<=>"); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 323 "chapel.ypp"
    { (yyval.pblockstmt) = buildLAndAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));    }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 324 "chapel.ypp"
    { (yyval.pblockstmt) = buildLOrAssignment((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pexpr));     }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 328 "chapel.ypp"
    { (yyval.pch) = NULL; }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 333 "chapel.ypp"
    { (yyval.pch) = astr(yytext); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 337 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (2)].pblockstmt); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 338 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(1) - (1)].pblockstmt); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 342 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, new SymExpr(gVoid)); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 343 "chapel.ypp"
    { (yyval.pblockstmt) = buildPrimitiveStmt(PRIM_RETURN, (yyvsp[(2) - (3)].pexpr)); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 347 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new BlockStmt()); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 357 "chapel.ypp"
    {
#ifdef HAVE_LLVM
      if (externC) {
        (yyval.pblockstmt) = buildExternBlockStmt(astr((yyvsp[(2) - (2)].pch)));
      } else {
        USR_FATAL(new BlockStmt(), "extern block syntax is turned off. Use --extern-c flag to turn on.");
      }
#else
      USR_FATAL(new BlockStmt(), "Chapel must be built with llvm in order to use the extern block syntax");
#endif
    }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 371 "chapel.ypp"
    { (yyval.pblockstmt) = buildDoWhileLoopStmt((yyvsp[(4) - (5)].pexpr), (yyvsp[(2) - (5)].pblockstmt)); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 372 "chapel.ypp"
    { (yyval.pblockstmt) = buildWhileDoLoopStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 373 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 374 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(5) - (6)].pcallexpr), (yyvsp[(6) - (6)].pblockstmt), true); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 375 "chapel.ypp"
    { (yyval.pblockstmt) = buildCoforallLoopStmt(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 376 "chapel.ypp"
    { (yyval.pblockstmt) = buildForLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 377 "chapel.ypp"
    { (yyval.pblockstmt) = buildForLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), false, true); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 378 "chapel.ypp"
    { (yyval.pblockstmt) = buildForLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 379 "chapel.ypp"
    { (yyval.pblockstmt) = buildForLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt), false, true); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 380 "chapel.ypp"
    { (yyval.pblockstmt) = buildParamForLoopStmt((yyvsp[(3) - (6)].pch), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 381 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 382 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pblockstmt), true); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 383 "chapel.ypp"
    { (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 385 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), new BlockStmt((yyvsp[(6) - (6)].pblockstmt)));
    }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 391 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pblockstmt) = buildForallLoopStmt((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), new BlockStmt((yyvsp[(6) - (6)].pblockstmt)), true);
    }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 397 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
      else
        (yyval.pblockstmt) = buildForallLoopStmt(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), new BlockStmt((yyvsp[(4) - (4)].pblockstmt)));
    }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 406 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_tuple", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 410 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pblockstmt)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 411 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (3)].pexpr), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 412 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pblockstmt), (yyvsp[(6) - (6)].pblockstmt)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 413 "chapel.ypp"
    { (yyval.pblockstmt) = buildIfStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(3) - (5)].pblockstmt), (yyvsp[(5) - (5)].pblockstmt)); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 418 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildSelectStmt((yyvsp[(2) - (5)].pexpr), (yyvsp[(4) - (5)].pblockstmt))); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 422 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 423 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pexpr)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 428 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN, (yyvsp[(2) - (3)].pcallexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 430 "chapel.ypp"
    { (yyval.pexpr) = new CondStmt(new CallExpr(PRIM_WHEN), (yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 435 "chapel.ypp"
    { (yyval.pblockstmt) = buildTypeSelectStmt((yyvsp[(3) - (6)].pcallexpr), (yyvsp[(5) - (6)].pblockstmt)); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 442 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(buildClassDefExpr((yyvsp[(3) - (7)].pch), (yyvsp[(2) - (7)].ptype), (yyvsp[(4) - (7)].pcallexpr), (yyvsp[(6) - (7)].pblockstmt), (yyvsp[(1) - (7)].flag), (yylsp[(1) - (7)]).comment));
      yylloc.comment = NULL; }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 447 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 448 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN; (yyloc).comment = yylloc.comment; yylloc.comment = NULL;}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 452 "chapel.ypp"
    { (yyval.ptype) = new AggregateType(AGGREGATE_CLASS); (yyloc).comment = yylloc.comment; 
             yylloc.comment = NULL;}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 454 "chapel.ypp"
    { (yyval.ptype) = new AggregateType(AGGREGATE_RECORD); (yyloc).comment = yylloc.comment; 
             yylloc.comment = NULL;}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 456 "chapel.ypp"
    { (yyval.ptype) = new AggregateType(AGGREGATE_UNION); (yyloc).comment = yylloc.comment;
             yylloc.comment = NULL;}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 461 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 462 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 467 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt(); }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 469 "chapel.ypp"
    { (yyvsp[(1) - (2)].pblockstmt)->insertAtTail((yyvsp[(2) - (2)].pblockstmt)); }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 471 "chapel.ypp"
    { (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(buildPragmaStmt((yyvsp[(2) - (3)].vpch), (yyvsp[(3) - (3)].pblockstmt))); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 476 "chapel.ypp"
    {
      EnumType* pdt = (yyvsp[(4) - (5)].penumtype);
      TypeSymbol* pst = new TypeSymbol((yyvsp[(2) - (5)].pch), pdt);
      (yyvsp[(4) - (5)].penumtype)->symbol = pst;
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(pst));
    }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 486 "chapel.ypp"
    {
      (yyval.penumtype) = new EnumType();
      (yyvsp[(1) - (1)].pdefexpr)->sym->type = (yyval.penumtype);
      (yyval.penumtype)->constants.insertAtTail((yyvsp[(1) - (1)].pdefexpr));
      (yyval.penumtype)->defaultValue = (yyvsp[(1) - (1)].pdefexpr)->sym;
    }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 493 "chapel.ypp"
    {
      (yyval.penumtype) = (yyvsp[(1) - (2)].penumtype);
    }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 497 "chapel.ypp"
    {
      (yyvsp[(1) - (3)].penumtype)->constants.insertAtTail((yyvsp[(3) - (3)].pdefexpr));
      (yyvsp[(3) - (3)].pdefexpr)->sym->type = (yyvsp[(1) - (3)].penumtype);
    }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 504 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 505 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new EnumSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 510 "chapel.ypp"
    {
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 515 "chapel.ypp"
    {
      captureTokens = 0;
      (yyvsp[(3) - (3)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 520 "chapel.ypp"
    {
      (yyvsp[(3) - (8)].pfnsymbol)->retTag = (yyvsp[(5) - (8)].retTag);
      if ((yyvsp[(5) - (8)].retTag) == RET_REF)
        USR_FATAL("'ref' return types are not allowed in lambdas");
      if ((yyvsp[(5) - (8)].retTag) == RET_PARAM)
        USR_FATAL("'param' return types are not allowed in lambdas");
      if ((yyvsp[(5) - (8)].retTag) == RET_TYPE)
        USR_FATAL("'type' return types are not allowed in lambdas");
      if ((yyvsp[(6) - (8)].pexpr))
        (yyvsp[(3) - (8)].pfnsymbol)->retExprType = new BlockStmt((yyvsp[(6) - (8)].pexpr), BLOCK_SCOPELESS);
      if ((yyvsp[(7) - (8)].pexpr))
        (yyvsp[(3) - (8)].pfnsymbol)->where = new BlockStmt((yyvsp[(7) - (8)].pexpr));
      (yyvsp[(3) - (8)].pfnsymbol)->insertAtTail((yyvsp[(8) - (8)].pblockstmt));
      (yyval.pexpr) = new DefExpr(buildLambda((yyvsp[(3) - (8)].pfnsymbol)));
    }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 540 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol(""); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 541 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol(""); (yyval.pfnsymbol)->addFlag(FLAG_INLINE); 
                  (yyloc).comment = yylloc.comment; yylloc.comment = NULL;}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 543 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol(""); (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);  
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);
                  (yyloc).comment = yylloc.comment; yylloc.comment = NULL;}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 546 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch)); (yyval.pfnsymbol)->addFlag(FLAG_EXPORT);  
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);
                  (yyloc).comment = yylloc.comment; yylloc.comment = NULL;}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 549 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol(""); (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);
                  (yyloc).comment = yylloc.comment; yylloc.comment = NULL;}
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 552 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol((yyvsp[(2) - (2)].pch)); (yyval.pfnsymbol)->addFlag(FLAG_EXTERN);  
                  (yyval.pfnsymbol)->addFlag(FLAG_LOCAL_ARGS);
                  (yyloc).comment = yylloc.comment; yylloc.comment = NULL;}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 558 "chapel.ypp"
    {
      // Sets up to capture tokens while parsing the next grammar nonterminal.
      captureTokens = 1;
      captureString[0] = '\0';
    }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 564 "chapel.ypp"
    {
      // Stop capturing and save the result.
      captureTokens = 0;

      (yyvsp[(4) - (4)].pfnsymbol)->userString = astr(captureString);
    }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 571 "chapel.ypp"
    {
      FnSymbol* fn = (yyvsp[(4) - (9)].pfnsymbol);

      fn->copyFlags((yyvsp[(1) - (9)].pfnsymbol));
      // The user explicitly named this function (controls mangling).
      if ((yyvsp[(1) - (9)].pfnsymbol)->hasFlag(FLAG_EXPORT) || (yyvsp[(1) - (9)].pfnsymbol)->hasFlag(FLAG_EXTERN))
        fn->addFlag(FLAG_USER_NAMED);
      if (*(yyvsp[(1) - (9)].pfnsymbol)->name)
        fn->cname = (yyvsp[(1) - (9)].pfnsymbol)->name;

      if ((yyvsp[(2) - (9)].procIter) == ProcIter_ITER)
      {
        if (fn->hasFlag(FLAG_EXTERN))
          USR_FATAL_CONT(fn, "'iter' is not legal with 'extern'");
        fn->addFlag(FLAG_ITERATOR_FN);
      }

      (yyval.pblockstmt) = buildFunctionDecl((yyvsp[(4) - (9)].pfnsymbol), (yyvsp[(6) - (9)].retTag), (yyvsp[(7) - (9)].pexpr), (yyvsp[(8) - (9)].pexpr), (yyvsp[(9) - (9)].pblockstmt), (yylsp[(1) - (9)]).comment);
      yylloc.comment = NULL;
    }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 595 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
    }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 599 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(3) - (3)].pfnsymbol), (yyvsp[(2) - (3)].pch), (yyvsp[(1) - (3)].pt), NULL);
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 604 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
    }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 608 "chapel.ypp"
    {
      (yyval.pfnsymbol) = buildFunctionSymbol((yyvsp[(5) - (5)].pfnsymbol), (yyvsp[(4) - (5)].pch), (yyvsp[(1) - (5)].pt), (yyvsp[(2) - (5)].pch));
      (yyval.pfnsymbol)->addFlag(FLAG_ASSIGNOP);
    }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 616 "chapel.ypp"
    { (yyval.pch) = astr("~", (yyvsp[(2) - (2)].pch)); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 617 "chapel.ypp"
    { (yyval.pch) = "&"; }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 618 "chapel.ypp"
    { (yyval.pch) = "|"; }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 619 "chapel.ypp"
    { (yyval.pch) = "^"; }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 620 "chapel.ypp"
    { (yyval.pch) = "~"; }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 621 "chapel.ypp"
    { (yyval.pch) = "=="; }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 622 "chapel.ypp"
    { (yyval.pch) = "!="; }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 623 "chapel.ypp"
    { (yyval.pch) = "<="; }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 624 "chapel.ypp"
    { (yyval.pch) = ">="; }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 625 "chapel.ypp"
    { (yyval.pch) = "<"; }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 626 "chapel.ypp"
    { (yyval.pch) = ">"; }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 627 "chapel.ypp"
    { (yyval.pch) = "+"; }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 628 "chapel.ypp"
    { (yyval.pch) = "-"; }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 629 "chapel.ypp"
    { (yyval.pch) = "*"; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 630 "chapel.ypp"
    { (yyval.pch) = "/"; }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 631 "chapel.ypp"
    { (yyval.pch) = "<<"; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 632 "chapel.ypp"
    { (yyval.pch) = ">>"; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 633 "chapel.ypp"
    { (yyval.pch) = "%"; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 634 "chapel.ypp"
    { (yyval.pch) = "**"; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 635 "chapel.ypp"
    { (yyval.pch) = "!"; }
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 636 "chapel.ypp"
    { (yyval.pch) = "by"; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 637 "chapel.ypp"
    { (yyval.pch) = "#"; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 638 "chapel.ypp"
    { (yyval.pch) = "align"; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 639 "chapel.ypp"
    { (yyval.pch) = "<=>"; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 640 "chapel.ypp"
    { (yyval.pch) = "<~>"; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 644 "chapel.ypp"
    { (yyval.pch) = "="; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 645 "chapel.ypp"
    { (yyval.pch) = "+="; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 646 "chapel.ypp"
    { (yyval.pch) = "-="; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 647 "chapel.ypp"
    { (yyval.pch) = "*="; }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 648 "chapel.ypp"
    { (yyval.pch) = "/="; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 649 "chapel.ypp"
    { (yyval.pch) = "%="; }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 650 "chapel.ypp"
    { (yyval.pch) = "**="; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 651 "chapel.ypp"
    { (yyval.pch) = "&="; }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 652 "chapel.ypp"
    { (yyval.pch) = "|="; }
    break;

  case 167:

/* Line 1806 of yacc.c  */
#line 653 "chapel.ypp"
    { (yyval.pch) = "^="; }
    break;

  case 168:

/* Line 1806 of yacc.c  */
#line 654 "chapel.ypp"
    { (yyval.pch) = ">>="; }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 655 "chapel.ypp"
    { (yyval.pch) = "<<="; }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 659 "chapel.ypp"
    { (yyval.pfnsymbol) = new FnSymbol("_"); (yyval.pfnsymbol)->addFlag(FLAG_NO_PARENS); }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 660 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 664 "chapel.ypp"
    { (yyval.pfnsymbol) = (yyvsp[(2) - (3)].pfnsymbol); }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 668 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, NULL); }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 669 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal(NULL, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 670 "chapel.ypp"
    { (yyval.pfnsymbol) = buildFunctionFormal((yyvsp[(1) - (3)].pfnsymbol), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 675 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr), NULL); }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 677 "chapel.ypp"
    { (yyval.pdefexpr) = buildArgDefExpr((yyvsp[(1) - (4)].pt), (yyvsp[(2) - (4)].pch), (yyvsp[(3) - (4)].pexpr), NULL, (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 679 "chapel.ypp"
    { (yyval.pdefexpr) = buildTupleArgDefExpr((yyvsp[(1) - (6)].pt), (yyvsp[(3) - (6)].pblockstmt), (yyvsp[(5) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 681 "chapel.ypp"
    { USR_FATAL("variable-length argument may not be grouped in a tuple"); }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 685 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 686 "chapel.ypp"
    { (yyval.pt) = INTENT_IN; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 687 "chapel.ypp"
    { (yyval.pt) = INTENT_INOUT; }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 688 "chapel.ypp"
    { (yyval.pt) = INTENT_OUT; }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 689 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST; }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 690 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_IN; }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 691 "chapel.ypp"
    { (yyval.pt) = INTENT_CONST_REF; }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 692 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 693 "chapel.ypp"
    { (yyval.pt) = INTENT_REF; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 694 "chapel.ypp"
    { (yyval.pt) = INTENT_TYPE; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 698 "chapel.ypp"
    { (yyval.pt) = INTENT_BLANK; }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 699 "chapel.ypp"
    { (yyval.pt) = INTENT_PARAM; }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 700 "chapel.ypp"
    { (yyval.pt) = INTENT_REF;   }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 704 "chapel.ypp"
    { (yyval.procIter) = ProcIter_PROC; }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 705 "chapel.ypp"
    { (yyval.procIter) = ProcIter_ITER; }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 709 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 710 "chapel.ypp"
    { (yyval.retTag) = RET_VALUE; }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 711 "chapel.ypp"
    {
    (yyval.retTag) = RET_REF;
    // create a new dummy AST node to get a location into USR_WARN
    USR_WARN(new BlockStmt(), "In the next release, 'var' return intents will "
                              "no longer be supported. Please use the "
                              "equivalent 'ref' return intent.");
            }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 718 "chapel.ypp"
    { (yyval.retTag) = RET_REF; }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 719 "chapel.ypp"
    { (yyval.retTag) = RET_PARAM; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 720 "chapel.ypp"
    { (yyval.retTag) = RET_TYPE; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 724 "chapel.ypp"
    { (yyval.pblockstmt) = NULL; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 730 "chapel.ypp"
    { (yyval.pblockstmt) = new BlockStmt((yyvsp[(1) - (1)].pblockstmt)); }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 735 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol((yyvsp[(2) - (2)].pch))); }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 737 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 741 "chapel.ypp"
    { (yyval.pdefexpr) = new DefExpr(new VarSymbol(astr("chpl__query", istr(query_uid++)))); }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 746 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 747 "chapel.ypp"
    { (yyvsp[(2) - (2)].pdefexpr)->sym->addFlag(FLAG_PARAM); (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 751 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 752 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 757 "chapel.ypp"
    { (yyval.pblockstmt) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 759 "chapel.ypp"
    { (yyval.pblockstmt) = handleConfigTypes((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 761 "chapel.ypp"
    { (yyval.pblockstmt) = convertTypesToExtern((yyvsp[(3) - (4)].pblockstmt)); }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 766 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (2)].pch));
      var->addFlag(FLAG_TYPE_VARIABLE);
      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (2)].pexpr));
      (yyval.pblockstmt) = buildChapelStmt(def);
    }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 773 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_TYPE_VARIABLE);
      DefExpr* def = new DefExpr(var, (yyvsp[(2) - (4)].pexpr));
      (yyvsp[(4) - (4)].pblockstmt)->insertAtHead(def);
      (yyval.pblockstmt) = buildChapelStmt((yyvsp[(4) - (4)].pblockstmt));
    }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 783 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 785 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 787 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExprFromArrayType((yyvsp[(2) - (2)].pcallexpr)); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 792 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), (yyvsp[(1) - (4)].flag), FLAG_PARAM, FLAG_UNKNOWN, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 796 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), (yyvsp[(1) - (4)].flag), FLAG_CONST, FLAG_UNKNOWN, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 800 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), (yyvsp[(1) - (4)].flag), FLAG_UNKNOWN, FLAG_REF_VAR, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 804 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(4) - (5)].pblockstmt), (yyvsp[(1) - (5)].flag), FLAG_CONST, FLAG_REF_VAR, (yylsp[(1) - (5)]).comment);
    }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 808 "chapel.ypp"
    {
      (yyval.pblockstmt) = buildVarDecls((yyvsp[(3) - (4)].pblockstmt), (yyvsp[(1) - (4)].flag), FLAG_UNKNOWN, FLAG_UNKNOWN, (yylsp[(1) - (4)]).comment);
    }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 814 "chapel.ypp"
    { (yyval.flag) = FLAG_UNKNOWN; (yyloc).comment = yylloc.comment; yylloc.comment = NULL;}
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 815 "chapel.ypp"
    { (yyval.flag) = FLAG_CONFIG; (yyloc).comment = yylloc.comment; yylloc.comment = NULL;}
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 816 "chapel.ypp"
    { (yyval.flag) = FLAG_EXTERN; (yyloc).comment = yylloc.comment; yylloc.comment = NULL;}
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 822 "chapel.ypp"
    {
      for_alist(expr, (yyvsp[(3) - (3)].pblockstmt)->body)
        (yyvsp[(1) - (3)].pblockstmt)->insertAtTail(expr->remove());
    }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 830 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt(new DefExpr(new VarSymbol((yyvsp[(1) - (3)].pch)), (yyvsp[(3) - (3)].pexpr), (yyvsp[(2) - (3)].pexpr))); }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 832 "chapel.ypp"
    {
      VarSymbol* var = new VarSymbol((yyvsp[(1) - (4)].pch));
      var->addFlag(FLAG_ARRAY_ALIAS);
      (yyval.pblockstmt) = buildChapelStmt(new DefExpr(var, (yyvsp[(4) - (4)].pexpr), (yyvsp[(2) - (4)].pexpr)));
    }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 838 "chapel.ypp"
    { (yyval.pblockstmt) = buildTupleVarDeclStmt((yyvsp[(2) - (5)].pblockstmt), (yyvsp[(4) - (5)].pexpr), (yyvsp[(5) - (5)].pexpr)); }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 843 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol("chpl__tuple_blank")); }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 845 "chapel.ypp"
    { (yyval.pexpr) = new DefExpr(new VarSymbol((yyvsp[(1) - (1)].pch))); }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 847 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pblockstmt); }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 852 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (1)].pexpr)); }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 854 "chapel.ypp"
    { (yyval.pblockstmt) = buildChapelStmt((yyvsp[(1) - (2)].pexpr)); }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 856 "chapel.ypp"
    { (yyval.pblockstmt) = ((yyvsp[(3) - (3)].pblockstmt)->insertAtHead((yyvsp[(1) - (3)].pexpr)), (yyvsp[(3) - (3)].pblockstmt)); }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 862 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 863 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNoInit); }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 864 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 868 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 870 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__ensureDomainExpr", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 874 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 246:

/* Line 1806 of yacc.c  */
#line 875 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 247:

/* Line 1806 of yacc.c  */
#line 876 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pcallexpr); }
    break;

  case 248:

/* Line 1806 of yacc.c  */
#line 877 "chapel.ypp"
    {printf("bad type specification"); }
    break;

  case 249:

/* Line 1806 of yacc.c  */
#line 898 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr)); 
    }
    break;

  case 250:

/* Line 1806 of yacc.c  */
#line 902 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pcallexpr)); 
    }
    break;

  case 251:

/* Line 1806 of yacc.c  */
#line 906 "chapel.ypp"
    { 
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pcallexpr) = new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", (yyvsp[(4) - (6)].pexpr)->copy()));
    }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 913 "chapel.ypp"
    {printf("bad array type specification"); clean_exit(1); }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 917 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 918 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 919 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 924 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 926 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 932 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayRuntimeType", gNil, (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 934 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(2) - (4)].pcallexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 936 "chapel.ypp"
    { (yyval.pexpr) = buildFormalArrayType((yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), (yyvsp[(2) - (6)].pcallexpr)); }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 940 "chapel.ypp"
    { (yyval.pexpr) = NULL; }
    break;

  case 262:

/* Line 1806 of yacc.c  */
#line 941 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 263:

/* Line 1806 of yacc.c  */
#line 942 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pdefexpr); }
    break;

  case 264:

/* Line 1806 of yacc.c  */
#line 943 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("_domain"); }
    break;

  case 265:

/* Line 1806 of yacc.c  */
#line 944 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_singlevar"); }
    break;

  case 266:

/* Line 1806 of yacc.c  */
#line 945 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr( "_syncvar"); }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 946 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 952 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 953 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pdefexpr)); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 954 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 955 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 959 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("chpl__tuple_blank"); }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 960 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pexpr); }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 961 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 275:

/* Line 1806 of yacc.c  */
#line 965 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 276:

/* Line 1806 of yacc.c  */
#line 966 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 277:

/* Line 1806 of yacc.c  */
#line 970 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST); }
    break;

  case 279:

/* Line 1806 of yacc.c  */
#line 975 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 280:

/* Line 1806 of yacc.c  */
#line 976 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 281:

/* Line 1806 of yacc.c  */
#line 980 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pdefexpr)); }
    break;

  case 282:

/* Line 1806 of yacc.c  */
#line 981 "chapel.ypp"
    { (yyval.pexpr) = buildNamedActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 982 "chapel.ypp"
    { (yyval.pexpr) = buildNamedAliasActual((yyvsp[(1) - (3)].pch), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 983 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(1) - (1)].pdefexpr); }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 988 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr((yyvsp[(1) - (1)].pch)); }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 1005 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_singlevar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 1007 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildIndexType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 1009 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 1011 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSubDomainType", (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 1013 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildSparseDomainRuntimeType", new UnresolvedSymExpr("defaultDist"), (yyvsp[(4) - (5)].pcallexpr)); }
    break;

  case 297:

/* Line 1806 of yacc.c  */
#line 1015 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__atomicType", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 1017 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr( "_syncvar", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 1022 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 1024 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 301:

/* Line 1806 of yacc.c  */
#line 1026 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 302:

/* Line 1806 of yacc.c  */
#line 1028 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 303:

/* Line 1806 of yacc.c  */
#line 1030 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 304:

/* Line 1806 of yacc.c  */
#line 1032 "chapel.ypp"
    { (yyval.pexpr) = buildForLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 305:

/* Line 1806 of yacc.c  */
#line 1034 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)); }
    break;

  case 306:

/* Line 1806 of yacc.c  */
#line 1036 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true); }
    break;

  case 307:

/* Line 1806 of yacc.c  */
#line 1038 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pexpr), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 308:

/* Line 1806 of yacc.c  */
#line 1040 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr)); }
    break;

  case 309:

/* Line 1806 of yacc.c  */
#line 1042 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pexpr), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true); }
    break;

  case 310:

/* Line 1806 of yacc.c  */
#line 1044 "chapel.ypp"
    { (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (7)].pexpr), (yyvsp[(7) - (7)].pexpr), (yyvsp[(5) - (7)].pexpr)); }
    break;

  case 311:

/* Line 1806 of yacc.c  */
#line 1046 "chapel.ypp"
    {
      if ((yyvsp[(2) - (4)].pcallexpr)->argList.length > 1)
        (yyval.pexpr) = buildForallLoopExpr(NULL, new CallExpr("chpl__ensureDomainExpr", (yyvsp[(2) - (4)].pcallexpr)), (yyvsp[(4) - (4)].pexpr), NULL, true);
      else
        (yyval.pexpr) = buildForallLoopExpr(NULL, (yyvsp[(2) - (4)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (4)].pexpr), NULL, true);
    }
    break;

  case 312:

/* Line 1806 of yacc.c  */
#line 1053 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, true);
    }
    break;

  case 313:

/* Line 1806 of yacc.c  */
#line 1059 "chapel.ypp"
    {
      if ((yyvsp[(2) - (6)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (6)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (6)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr), NULL, false, true);
    }
    break;

  case 314:

/* Line 1806 of yacc.c  */
#line 1065 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr));
    }
    break;

  case 315:

/* Line 1806 of yacc.c  */
#line 1071 "chapel.ypp"
    {
      if ((yyvsp[(2) - (9)].pcallexpr)->argList.length != 1)
        USR_FATAL((yyvsp[(4) - (9)].pexpr), "invalid index expression");
      (yyval.pexpr) = buildForallLoopExpr((yyvsp[(2) - (9)].pcallexpr)->get(1)->remove(), (yyvsp[(4) - (9)].pexpr), (yyvsp[(9) - (9)].pexpr), (yyvsp[(7) - (9)].pexpr), false, true);
    }
    break;

  case 316:

/* Line 1806 of yacc.c  */
#line 1080 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(new DefExpr(buildIfExpr((yyvsp[(2) - (6)].pexpr), (yyvsp[(4) - (6)].pexpr), (yyvsp[(6) - (6)].pexpr)))); }
    break;

  case 317:

/* Line 1806 of yacc.c  */
#line 1084 "chapel.ypp"
    { (yyval.pexpr) = new SymExpr(gNil); }
    break;

  case 325:

/* Line 1806 of yacc.c  */
#line 1100 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 326:

/* Line 1806 of yacc.c  */
#line 1104 "chapel.ypp"
    { (yyval.pcallexpr) = NULL; }
    break;

  case 327:

/* Line 1806 of yacc.c  */
#line 1105 "chapel.ypp"
    {
    (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr);
    USR_WARN((yyvsp[(3) - (4)].pcallexpr), "In the next release, ref(x, y, ...) will no longer be valid. "
                 "You will need to convert any task intents to the new form "
                 "of `with (ref x, ref y, ...)`");
                                }
    break;

  case 328:

/* Line 1806 of yacc.c  */
#line 1111 "chapel.ypp"
    { (yyval.pcallexpr) = (yyvsp[(3) - (4)].pcallexpr); }
    break;

  case 329:

/* Line 1806 of yacc.c  */
#line 1115 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 330:

/* Line 1806 of yacc.c  */
#line 1116 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 331:

/* Line 1806 of yacc.c  */
#line 1121 "chapel.ypp"
    {
      if ((yyvsp[(1) - (2)].pt) != INTENT_REF) {
        USR_FATAL((yyvsp[(2) - (2)].pexpr), "Only 'ref' intents are currently supported in with clauses");
      } else {
        (yyval.pexpr) = (yyvsp[(2) - (2)].pexpr);
      }
    }
    break;

  case 332:

/* Line 1806 of yacc.c  */
#line 1131 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (1)].pexpr)); }
    break;

  case 333:

/* Line 1806 of yacc.c  */
#line 1132 "chapel.ypp"
    { (yyvsp[(1) - (3)].pcallexpr)->insertAtTail((yyvsp[(3) - (3)].pexpr)); }
    break;

  case 335:

/* Line 1806 of yacc.c  */
#line 1138 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<~>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 336:

/* Line 1806 of yacc.c  */
#line 1143 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_NEW, (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 337:

/* Line 1806 of yacc.c  */
#line 1148 "chapel.ypp"
    { (yyval.pexpr) = buildLetExpr((yyvsp[(2) - (4)].pblockstmt), (yyvsp[(4) - (4)].pexpr)); }
    break;

  case 346:

/* Line 1806 of yacc.c  */
#line 1164 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TUPLE_EXPAND, (yyvsp[(3) - (4)].pexpr)); }
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1166 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_cast", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1168 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_range", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1170 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_range", buildDotExpr("BoundedRangeType", "boundedLow"), (yyvsp[(1) - (2)].pexpr)); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1172 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_range", buildDotExpr("BoundedRangeType", "boundedHigh"), (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1174 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("_build_range", buildDotExpr("BoundedRangeType", "boundedNone")); }
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1197 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1198 "chapel.ypp"
    { (yyval.pexpr) = buildSquareCallExpr((yyvsp[(1) - (4)].pexpr), (yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1199 "chapel.ypp"
    { (yyval.pexpr) = buildPrimitiveExpr((yyvsp[(3) - (4)].pcallexpr)); }
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1203 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pch)); }
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1204 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(PRIM_TYPEOF, (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1205 "chapel.ypp"
    { (yyval.pexpr) = buildDotExpr((yyvsp[(1) - (3)].pexpr), "_dom"); }
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1213 "chapel.ypp"
    { (yyval.pexpr) = (yyvsp[(2) - (3)].pexpr); }
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1214 "chapel.ypp"
    { (yyval.pexpr) = buildOneTuple((yyvsp[(2) - (4)].pexpr)); }
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1215 "chapel.ypp"
    { (yyval.pexpr) = buildTuple((yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1219 "chapel.ypp"
    { (yyval.pexpr) = buildIntLiteral(yytext); }
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1220 "chapel.ypp"
    { (yyval.pexpr) = buildRealLiteral(yytext); }
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1221 "chapel.ypp"
    { (yyval.pexpr) = buildImagLiteral(yytext); }
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1222 "chapel.ypp"
    { (yyval.pexpr) = buildStringLiteral((yyvsp[(1) - (1)].pch)); }
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1223 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildDomainExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1224 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__buildArrayExpr", (yyvsp[(2) - (3)].pcallexpr)); }
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1226 "chapel.ypp"
    {
      (yyval.pexpr) = new CallExpr("chpl__buildAssociativeArrayExpr", (yyvsp[(2) - (3)].pcallexpr));
    }
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1232 "chapel.ypp"
    { (yyval.pcallexpr) = new CallExpr(PRIM_ACTUALS_LIST, (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1233 "chapel.ypp"
    { (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(3) - (5)].pexpr)); (yyvsp[(1) - (5)].pcallexpr)->insertAtTail((yyvsp[(5) - (5)].pexpr)); }
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1237 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1238 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1239 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("*", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1240 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("/", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1241 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1242 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">>", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1243 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("%", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1244 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("==", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1245 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1246 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1247 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">=", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1248 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("<", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1249 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr(">", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1250 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("&", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1251 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("|", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1252 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("^", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1253 "chapel.ypp"
    { (yyval.pexpr) = buildLogicalAndExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1254 "chapel.ypp"
    { (yyval.pexpr) = buildLogicalOrExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1255 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("**", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1256 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("by", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1257 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("align", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1258 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("#", (yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1259 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("chpl__distributed", (yyvsp[(3) - (3)].pexpr), (yyvsp[(1) - (3)].pexpr)); }
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1263 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("+", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1264 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("-", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1265 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '-'); }
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1266 "chapel.ypp"
    { (yyval.pexpr) = buildPreDecIncWarning((yyvsp[(2) - (2)].pexpr), '+'); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1267 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("!", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1268 "chapel.ypp"
    { (yyval.pexpr) = new CallExpr("~", (yyvsp[(2) - (2)].pexpr)); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1272 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1273 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1274 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1275 "chapel.ypp"
    { (yyval.pexpr) = buildReduceExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1279 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1280 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1281 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr)); }
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1282 "chapel.ypp"
    { (yyval.pexpr) = buildScanExpr((yyvsp[(1) - (3)].pexpr), (yyvsp[(3) - (3)].pexpr), true); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1287 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("SumReduceScanOp"); }
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1288 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("ProductReduceScanOp"); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1289 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalAndReduceScanOp"); }
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1290 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("LogicalOrReduceScanOp"); }
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1291 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseAndReduceScanOp"); }
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1292 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseOrReduceScanOp"); }
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1293 "chapel.ypp"
    { (yyval.pexpr) = new UnresolvedSymExpr("BitwiseXorReduceScanOp"); }
    break;



/* Line 1806 of yacc.c  */
#line 7086 "chapel.tab.cpp"
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
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  yyerror_range[1] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 1296 "chapel.ypp"


