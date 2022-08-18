/* A Bison parser, made by GNU Bison 3.8.2.  */

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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 1

/* Pull parsers.  */
#define YYPULL 0

/* Substitute the type names.  */
#define YYSTYPE         YYCHPL_STYPE
#define YYLTYPE         YYCHPL_LTYPE
/* Substitute the variable and function names.  */
#define yypush_parse    yychpl_push_parse
#define yypstate_new    yychpl_pstate_new
#define yypstate_clear  yychpl_pstate_clear
#define yypstate_delete yychpl_pstate_delete
#define yypstate        yychpl_pstate
#define yylex           yychpl_lex
#define yyerror         yychpl_error
#define yydebug         yychpl_debug
#define yynerrs         yychpl_nerrs


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

#include "bison-chpl-lib.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TIDENT = 3,                     /* TIDENT  */
  YYSYMBOL_TQUERIEDIDENT = 4,              /* TQUERIEDIDENT  */
  YYSYMBOL_INTLITERAL = 5,                 /* INTLITERAL  */
  YYSYMBOL_REALLITERAL = 6,                /* REALLITERAL  */
  YYSYMBOL_IMAGLITERAL = 7,                /* IMAGLITERAL  */
  YYSYMBOL_STRINGLITERAL = 8,              /* STRINGLITERAL  */
  YYSYMBOL_BYTESLITERAL = 9,               /* BYTESLITERAL  */
  YYSYMBOL_CSTRINGLITERAL = 10,            /* CSTRINGLITERAL  */
  YYSYMBOL_EXTERNCODE = 11,                /* EXTERNCODE  */
  YYSYMBOL_TALIGN = 12,                    /* TALIGN  */
  YYSYMBOL_TAS = 13,                       /* TAS  */
  YYSYMBOL_TATOMIC = 14,                   /* TATOMIC  */
  YYSYMBOL_TBEGIN = 15,                    /* TBEGIN  */
  YYSYMBOL_TBREAK = 16,                    /* TBREAK  */
  YYSYMBOL_TBOOL = 17,                     /* TBOOL  */
  YYSYMBOL_TBORROWED = 18,                 /* TBORROWED  */
  YYSYMBOL_TBY = 19,                       /* TBY  */
  YYSYMBOL_TBYTES = 20,                    /* TBYTES  */
  YYSYMBOL_TCATCH = 21,                    /* TCATCH  */
  YYSYMBOL_TCLASS = 22,                    /* TCLASS  */
  YYSYMBOL_TCOBEGIN = 23,                  /* TCOBEGIN  */
  YYSYMBOL_TCOFORALL = 24,                 /* TCOFORALL  */
  YYSYMBOL_TCOMPLEX = 25,                  /* TCOMPLEX  */
  YYSYMBOL_TCONFIG = 26,                   /* TCONFIG  */
  YYSYMBOL_TCONST = 27,                    /* TCONST  */
  YYSYMBOL_TCONTINUE = 28,                 /* TCONTINUE  */
  YYSYMBOL_TDEFER = 29,                    /* TDEFER  */
  YYSYMBOL_TDELETE = 30,                   /* TDELETE  */
  YYSYMBOL_TDEPRECATED = 31,               /* TDEPRECATED  */
  YYSYMBOL_TDMAPPED = 32,                  /* TDMAPPED  */
  YYSYMBOL_TDO = 33,                       /* TDO  */
  YYSYMBOL_TDOMAIN = 34,                   /* TDOMAIN  */
  YYSYMBOL_TELSE = 35,                     /* TELSE  */
  YYSYMBOL_TENUM = 36,                     /* TENUM  */
  YYSYMBOL_TEXCEPT = 37,                   /* TEXCEPT  */
  YYSYMBOL_TEXPORT = 38,                   /* TEXPORT  */
  YYSYMBOL_TEXTERN = 39,                   /* TEXTERN  */
  YYSYMBOL_TFALSE = 40,                    /* TFALSE  */
  YYSYMBOL_TFOR = 41,                      /* TFOR  */
  YYSYMBOL_TFORALL = 42,                   /* TFORALL  */
  YYSYMBOL_TFOREACH = 43,                  /* TFOREACH  */
  YYSYMBOL_TFORWARDING = 44,               /* TFORWARDING  */
  YYSYMBOL_TIF = 45,                       /* TIF  */
  YYSYMBOL_TIMAG = 46,                     /* TIMAG  */
  YYSYMBOL_TIMPORT = 47,                   /* TIMPORT  */
  YYSYMBOL_TIN = 48,                       /* TIN  */
  YYSYMBOL_TINCLUDE = 49,                  /* TINCLUDE  */
  YYSYMBOL_TINDEX = 50,                    /* TINDEX  */
  YYSYMBOL_TINLINE = 51,                   /* TINLINE  */
  YYSYMBOL_TINOUT = 52,                    /* TINOUT  */
  YYSYMBOL_TINT = 53,                      /* TINT  */
  YYSYMBOL_TITER = 54,                     /* TITER  */
  YYSYMBOL_TINITEQUALS = 55,               /* TINITEQUALS  */
  YYSYMBOL_TIMPLEMENTS = 56,               /* TIMPLEMENTS  */
  YYSYMBOL_TINTERFACE = 57,                /* TINTERFACE  */
  YYSYMBOL_TLABEL = 58,                    /* TLABEL  */
  YYSYMBOL_TLAMBDA = 59,                   /* TLAMBDA  */
  YYSYMBOL_TLET = 60,                      /* TLET  */
  YYSYMBOL_TLIFETIME = 61,                 /* TLIFETIME  */
  YYSYMBOL_TLOCAL = 62,                    /* TLOCAL  */
  YYSYMBOL_TLOCALE = 63,                   /* TLOCALE  */
  YYSYMBOL_TMANAGE = 64,                   /* TMANAGE  */
  YYSYMBOL_TMINUSMINUS = 65,               /* TMINUSMINUS  */
  YYSYMBOL_TMODULE = 66,                   /* TMODULE  */
  YYSYMBOL_TNEW = 67,                      /* TNEW  */
  YYSYMBOL_TNIL = 68,                      /* TNIL  */
  YYSYMBOL_TNOINIT = 69,                   /* TNOINIT  */
  YYSYMBOL_TNONE = 70,                     /* TNONE  */
  YYSYMBOL_TNOTHING = 71,                  /* TNOTHING  */
  YYSYMBOL_TON = 72,                       /* TON  */
  YYSYMBOL_TONLY = 73,                     /* TONLY  */
  YYSYMBOL_TOPERATOR = 74,                 /* TOPERATOR  */
  YYSYMBOL_TOTHERWISE = 75,                /* TOTHERWISE  */
  YYSYMBOL_TOUT = 76,                      /* TOUT  */
  YYSYMBOL_TOVERRIDE = 77,                 /* TOVERRIDE  */
  YYSYMBOL_TOWNED = 78,                    /* TOWNED  */
  YYSYMBOL_TPARAM = 79,                    /* TPARAM  */
  YYSYMBOL_TPLUSPLUS = 80,                 /* TPLUSPLUS  */
  YYSYMBOL_TPRAGMA = 81,                   /* TPRAGMA  */
  YYSYMBOL_TPRIMITIVE = 82,                /* TPRIMITIVE  */
  YYSYMBOL_TPRIVATE = 83,                  /* TPRIVATE  */
  YYSYMBOL_TPROC = 84,                     /* TPROC  */
  YYSYMBOL_TPROTOTYPE = 85,                /* TPROTOTYPE  */
  YYSYMBOL_TPUBLIC = 86,                   /* TPUBLIC  */
  YYSYMBOL_TREAL = 87,                     /* TREAL  */
  YYSYMBOL_TRECORD = 88,                   /* TRECORD  */
  YYSYMBOL_TREDUCE = 89,                   /* TREDUCE  */
  YYSYMBOL_TREF = 90,                      /* TREF  */
  YYSYMBOL_TREQUIRE = 91,                  /* TREQUIRE  */
  YYSYMBOL_TRETURN = 92,                   /* TRETURN  */
  YYSYMBOL_TSCAN = 93,                     /* TSCAN  */
  YYSYMBOL_TSELECT = 94,                   /* TSELECT  */
  YYSYMBOL_TSERIAL = 95,                   /* TSERIAL  */
  YYSYMBOL_TSHARED = 96,                   /* TSHARED  */
  YYSYMBOL_TSINGLE = 97,                   /* TSINGLE  */
  YYSYMBOL_TSPARSE = 98,                   /* TSPARSE  */
  YYSYMBOL_TSTRING = 99,                   /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 100,               /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 101,                    /* TSYNC  */
  YYSYMBOL_TTHEN = 102,                    /* TTHEN  */
  YYSYMBOL_TTHIS = 103,                    /* TTHIS  */
  YYSYMBOL_TTHROW = 104,                   /* TTHROW  */
  YYSYMBOL_TTHROWS = 105,                  /* TTHROWS  */
  YYSYMBOL_TTRUE = 106,                    /* TTRUE  */
  YYSYMBOL_TTRY = 107,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 108,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 109,                    /* TTYPE  */
  YYSYMBOL_TUINT = 110,                    /* TUINT  */
  YYSYMBOL_TUNDERSCORE = 111,              /* TUNDERSCORE  */
  YYSYMBOL_TUNION = 112,                   /* TUNION  */
  YYSYMBOL_TUNMANAGED = 113,               /* TUNMANAGED  */
  YYSYMBOL_TUNSTABLE = 114,                /* TUNSTABLE  */
  YYSYMBOL_TUSE = 115,                     /* TUSE  */
  YYSYMBOL_TVAR = 116,                     /* TVAR  */
  YYSYMBOL_TVOID = 117,                    /* TVOID  */
  YYSYMBOL_TWHEN = 118,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 119,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 120,                   /* TWHILE  */
  YYSYMBOL_TWITH = 121,                    /* TWITH  */
  YYSYMBOL_TYIELD = 122,                   /* TYIELD  */
  YYSYMBOL_TZIP = 123,                     /* TZIP  */
  YYSYMBOL_TALIAS = 124,                   /* TALIAS  */
  YYSYMBOL_TAND = 125,                     /* TAND  */
  YYSYMBOL_TASSIGN = 126,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 127,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 128,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 129,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 130,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 131,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 132,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 133,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 134,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 135,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 136,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 137,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 138,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 139,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 140,                /* TASSIGNSR  */
  YYSYMBOL_TBANG = 141,                    /* TBANG  */
  YYSYMBOL_TBAND = 142,                    /* TBAND  */
  YYSYMBOL_TBNOT = 143,                    /* TBNOT  */
  YYSYMBOL_TBOR = 144,                     /* TBOR  */
  YYSYMBOL_TBXOR = 145,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 146,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 147,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 148,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 149,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 150,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 151,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 152,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 153,                     /* TEXP  */
  YYSYMBOL_TGREATER = 154,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 155,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 156,                    /* THASH  */
  YYSYMBOL_TIO = 157,                      /* TIO  */
  YYSYMBOL_TLESS = 158,                    /* TLESS  */
  YYSYMBOL_TLESSEQUAL = 159,               /* TLESSEQUAL  */
  YYSYMBOL_TMINUS = 160,                   /* TMINUS  */
  YYSYMBOL_TMOD = 161,                     /* TMOD  */
  YYSYMBOL_TNOTEQUAL = 162,                /* TNOTEQUAL  */
  YYSYMBOL_TOR = 163,                      /* TOR  */
  YYSYMBOL_TPLUS = 164,                    /* TPLUS  */
  YYSYMBOL_TQUESTION = 165,                /* TQUESTION  */
  YYSYMBOL_TSEMI = 166,                    /* TSEMI  */
  YYSYMBOL_TSHIFTLEFT = 167,               /* TSHIFTLEFT  */
  YYSYMBOL_TSHIFTRIGHT = 168,              /* TSHIFTRIGHT  */
  YYSYMBOL_TSTAR = 169,                    /* TSTAR  */
  YYSYMBOL_TSWAP = 170,                    /* TSWAP  */
  YYSYMBOL_TLCBR = 171,                    /* TLCBR  */
  YYSYMBOL_TRCBR = 172,                    /* TRCBR  */
  YYSYMBOL_TLP = 173,                      /* TLP  */
  YYSYMBOL_TRP = 174,                      /* TRP  */
  YYSYMBOL_TLSBR = 175,                    /* TLSBR  */
  YYSYMBOL_TRSBR = 176,                    /* TRSBR  */
  YYSYMBOL_TNOELSE = 177,                  /* TNOELSE  */
  YYSYMBOL_TDOTDOTOPENHIGH = 178,          /* TDOTDOTOPENHIGH  */
  YYSYMBOL_TUPLUS = 179,                   /* TUPLUS  */
  YYSYMBOL_TUMINUS = 180,                  /* TUMINUS  */
  YYSYMBOL_TLNOT = 181,                    /* TLNOT  */
  YYSYMBOL_YYACCEPT = 182,                 /* $accept  */
  YYSYMBOL_program = 183,                  /* program  */
  YYSYMBOL_toplevel_stmt_ls = 184,         /* toplevel_stmt_ls  */
  YYSYMBOL_toplevel_stmt = 185,            /* toplevel_stmt  */
  YYSYMBOL_pragma_ls = 186,                /* pragma_ls  */
  YYSYMBOL_stmt = 187,                     /* stmt  */
  YYSYMBOL_tryable_stmt = 188,             /* tryable_stmt  */
  YYSYMBOL_deprecated_decl_stmt = 189,     /* deprecated_decl_stmt  */
  YYSYMBOL_190_1 = 190,                    /* $@1  */
  YYSYMBOL_191_2 = 191,                    /* $@2  */
  YYSYMBOL_decl_base = 192,                /* decl_base  */
  YYSYMBOL_unstable_decl_stmt = 193,       /* unstable_decl_stmt  */
  YYSYMBOL_194_3 = 194,                    /* $@3  */
  YYSYMBOL_195_4 = 195,                    /* $@4  */
  YYSYMBOL_module_decl_start = 196,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 197,         /* module_decl_stmt  */
  YYSYMBOL_opt_access_control = 198,       /* opt_access_control  */
  YYSYMBOL_opt_prototype = 199,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 200,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 201,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 202,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 203,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 204,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 205,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 206,              /* opt_only_ls  */
  YYSYMBOL_use_stmt = 207,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 208,              /* import_stmt  */
  YYSYMBOL_import_expr = 209,              /* import_expr  */
  YYSYMBOL_import_ls = 210,                /* import_ls  */
  YYSYMBOL_require_stmt = 211,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 212,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 213,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 214,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 215,                /* ident_def  */
  YYSYMBOL_ident_use = 216,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 217,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 218,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 219,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 220,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 221,              /* return_stmt  */
  YYSYMBOL_deprecated_class_level_stmt = 222, /* deprecated_class_level_stmt  */
  YYSYMBOL_223_5 = 223,                    /* $@5  */
  YYSYMBOL_224_6 = 224,                    /* $@6  */
  YYSYMBOL_unstable_class_level_stmt = 225, /* unstable_class_level_stmt  */
  YYSYMBOL_226_7 = 226,                    /* $@7  */
  YYSYMBOL_227_8 = 227,                    /* $@8  */
  YYSYMBOL_class_level_stmt = 228,         /* class_level_stmt  */
  YYSYMBOL_229_9 = 229,                    /* $@9  */
  YYSYMBOL_230_10 = 230,                   /* $@10  */
  YYSYMBOL_inner_class_level_stmt = 231,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 232,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 233,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 234,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 235, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 236,  /* extern_export_decl_stmt  */
  YYSYMBOL_237_11 = 237,                   /* $@11  */
  YYSYMBOL_238_12 = 238,                   /* $@12  */
  YYSYMBOL_extern_block_stmt = 239,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 240,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 241,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 242,                  /* if_stmt  */
  YYSYMBOL_ifvar = 243,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 244,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 245,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 246,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 247,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 248, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 249,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 250,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 251,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 252,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 253,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 254,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 255,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 256,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 257,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 258,                /* when_stmt  */
  YYSYMBOL_manager_expr = 259,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 260,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 261,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 262,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 263,              /* class_start  */
  YYSYMBOL_class_tag = 264,                /* class_tag  */
  YYSYMBOL_opt_inherit = 265,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 266,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 267,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 268,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 269,                  /* enum_ls  */
  YYSYMBOL_270_13 = 270,                   /* $@13  */
  YYSYMBOL_271_14 = 271,                   /* $@14  */
  YYSYMBOL_deprecated_enum_item = 272,     /* deprecated_enum_item  */
  YYSYMBOL_273_15 = 273,                   /* $@15  */
  YYSYMBOL_274_16 = 274,                   /* $@16  */
  YYSYMBOL_unstable_enum_item = 275,       /* unstable_enum_item  */
  YYSYMBOL_276_17 = 276,                   /* $@17  */
  YYSYMBOL_277_18 = 277,                   /* $@18  */
  YYSYMBOL_enum_item = 278,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 279,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 280,         /* lambda_decl_expr  */
  YYSYMBOL_281_19 = 281,                   /* $@19  */
  YYSYMBOL_linkage_spec_empty = 282,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 283,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt_complete = 284,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 285,             /* fn_decl_stmt  */
  YYSYMBOL_286_20 = 286,                   /* $@20  */
  YYSYMBOL_fn_decl_stmt_inner = 287,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 288,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 289,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 290,                 /* fn_ident  */
  YYSYMBOL_op_ident = 291,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 292,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 293,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 294,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 295,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 296,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 297,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 298,                /* formal_ls  */
  YYSYMBOL_formal = 299,                   /* formal  */
  YYSYMBOL_opt_formal_intent_tag = 300,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 301,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 302,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 303,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 304,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 305,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 306,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 307,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 308,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 309,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 310, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 311,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 312,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 313,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 314, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 315, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 316, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 317,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 318,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 319,            /* var_decl_stmt  */
  YYSYMBOL_320_21 = 320,                   /* $@21  */
  YYSYMBOL_var_decl_stmt_inner_ls = 321,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 322,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 323, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 324, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 325,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 326,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 327,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 328,                 /* opt_type  */
  YYSYMBOL_array_type = 329,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 330, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 331,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 332,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 333,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 334,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 335,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 336,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 337,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 338,                /* actual_ls  */
  YYSYMBOL_actual_expr = 339,              /* actual_expr  */
  YYSYMBOL_ident_expr = 340,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 341,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 342,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 343,                 /* for_expr  */
  YYSYMBOL_cond_expr = 344,                /* cond_expr  */
  YYSYMBOL_nil_expr = 345,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 346,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 347,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 348,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 349,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 350,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 351,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 352,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 353,          /* task_var_prefix  */
  YYSYMBOL_io_expr = 354,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 355,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 356,                 /* new_expr  */
  YYSYMBOL_let_expr = 357,                 /* let_expr  */
  YYSYMBOL_expr = 358,                     /* expr  */
  YYSYMBOL_opt_expr = 359,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 360,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 361,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 362,           /* call_base_expr  */
  YYSYMBOL_call_expr = 363,                /* call_expr  */
  YYSYMBOL_dot_expr = 364,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 365,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 366,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 367,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 368,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 369,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 370,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 371,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 372,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 373,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 374       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 326 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include helper macros for use in writing productions
  #include "parser-help.h"
  // include implementation of error logging hook
  #include "parser-yyerror.h"

#line 498 "bison-chpl-lib.cpp"

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
typedef yytype_int16 yy_state_t;

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

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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
         || (defined YYCHPL_LTYPE_IS_TRIVIAL && YYCHPL_LTYPE_IS_TRIVIAL \
             && defined YYCHPL_STYPE_IS_TRIVIAL && YYCHPL_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   21141

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  193
/* YYNRULES -- Number of rules.  */
#define YYNRULES  738
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1299

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   436


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
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
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181
};

#if YYCHPL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   613,   613,   617,   618,   623,   624,   632,   636,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
     655,   668,   669,   670,   679,   688,   692,   699,   707,   708,
     709,   710,   711,   712,   713,   714,   728,   743,   751,   763,
     774,   786,   798,   809,   825,   827,   826,   835,   834,   845,
     846,   851,   850,   859,   858,   869,   889,   904,   919,   935,
     936,   938,   943,   944,   949,   967,   971,   975,   986,  1005,
    1006,  1010,  1014,  1018,  1023,  1027,  1031,  1041,  1046,  1052,
    1059,  1064,  1071,  1082,  1083,  1087,  1092,  1100,  1109,  1119,
    1127,  1136,  1149,  1157,  1161,  1166,  1172,  1181,  1182,  1186,
    1195,  1199,  1203,  1207,  1211,  1215,  1224,  1225,  1229,  1230,
    1231,  1232,  1233,  1234,  1240,  1241,  1242,  1243,  1244,  1245,
    1260,  1261,  1272,  1273,  1274,  1275,  1276,  1277,  1278,  1279,
    1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,  1289,
    1290,  1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,  1302,
    1303,  1304,  1305,  1312,  1313,  1314,  1315,  1319,  1320,  1324,
    1331,  1341,  1343,  1342,  1351,  1350,  1362,  1361,  1370,  1369,
    1380,  1384,  1390,  1390,  1396,  1396,  1405,  1406,  1407,  1408,
    1409,  1410,  1411,  1415,  1420,  1425,  1430,  1437,  1445,  1446,
    1450,  1459,  1465,  1473,  1492,  1491,  1501,  1500,  1513,  1520,
    1538,  1551,  1564,  1568,  1572,  1576,  1580,  1584,  1588,  1592,
    1610,  1614,  1618,  1622,  1626,  1630,  1634,  1638,  1642,  1646,
    1650,  1654,  1658,  1662,  1666,  1670,  1674,  1678,  1682,  1686,
    1690,  1694,  1698,  1703,  1711,  1720,  1724,  1728,  1732,  1736,
    1740,  1744,  1748,  1752,  1757,  1762,  1767,  1775,  1790,  1808,
    1812,  1819,  1820,  1825,  1830,  1831,  1832,  1833,  1834,  1835,
    1836,  1837,  1838,  1839,  1840,  1841,  1842,  1854,  1855,  1856,
    1857,  1866,  1867,  1871,  1875,  1879,  1886,  1890,  1894,  1901,
    1905,  1909,  1913,  1920,  1921,  1925,  1929,  1933,  1940,  1953,
    1969,  1977,  1981,  1990,  1991,  1995,  1999,  2004,  2012,  2017,
    2021,  2028,  2029,  2033,  2042,  2047,  2058,  2065,  2066,  2067,
    2071,  2072,  2076,  2080,  2084,  2088,  2092,  2099,  2116,  2129,
    2136,  2141,  2148,  2147,  2158,  2164,  2163,  2177,  2179,  2178,
    2187,  2186,  2198,  2197,  2206,  2205,  2216,  2223,  2235,  2252,
    2250,  2277,  2281,  2282,  2284,  2289,  2299,  2296,  2330,  2338,
    2346,  2357,  2368,  2377,  2392,  2393,  2397,  2398,  2399,  2408,
    2409,  2410,  2411,  2412,  2413,  2414,  2415,  2416,  2417,  2418,
    2419,  2420,  2421,  2422,  2423,  2424,  2425,  2426,  2427,  2428,
    2429,  2430,  2431,  2432,  2433,  2437,  2438,  2439,  2440,  2441,
    2442,  2443,  2444,  2445,  2446,  2447,  2448,  2453,  2454,  2458,
    2459,  2460,  2464,  2465,  2469,  2473,  2474,  2478,  2479,  2483,
    2492,  2503,  2513,  2525,  2537,  2545,  2549,  2557,  2558,  2559,
    2560,  2561,  2562,  2563,  2564,  2565,  2569,  2570,  2571,  2572,
    2573,  2574,  2578,  2579,  2580,  2584,  2585,  2586,  2587,  2588,
    2589,  2593,  2594,  2597,  2598,  2602,  2603,  2607,  2612,  2613,
    2615,  2617,  2619,  2624,  2626,  2631,  2633,  2635,  2637,  2639,
    2641,  2643,  2648,  2649,  2653,  2662,  2666,  2674,  2678,  2685,
    2706,  2707,  2709,  2717,  2718,  2719,  2720,  2721,  2726,  2725,
    2734,  2742,  2746,  2753,  2769,  2786,  2790,  2794,  2801,  2803,
    2805,  2812,  2813,  2814,  2819,  2823,  2827,  2831,  2835,  2839,
    2843,  2850,  2851,  2852,  2853,  2854,  2859,  2860,  2861,  2862,
    2863,  2883,  2887,  2891,  2895,  2902,  2903,  2904,  2908,  2913,
    2921,  2926,  2930,  2937,  2938,  2939,  2940,  2941,  2947,  2948,
    2949,  2950,  2954,  2955,  2959,  2960,  2961,  2965,  2969,  2976,
    2977,  2981,  2986,  2995,  2996,  2997,  2998,  3002,  3003,  3014,
    3016,  3018,  3024,  3025,  3026,  3027,  3028,  3029,  3031,  3033,
    3035,  3037,  3043,  3045,  3048,  3050,  3052,  3054,  3056,  3058,
    3060,  3062,  3065,  3067,  3072,  3081,  3090,  3098,  3112,  3126,
    3140,  3149,  3158,  3166,  3180,  3194,  3208,  3225,  3234,  3243,
    3258,  3276,  3294,  3302,  3303,  3304,  3305,  3306,  3307,  3308,
    3309,  3314,  3315,  3319,  3328,  3329,  3333,  3342,  3343,  3347,
    3362,  3366,  3373,  3374,  3375,  3376,  3377,  3378,  3382,  3383,
    3388,  3390,  3392,  3394,  3396,  3402,  3409,  3421,  3433,  3446,
    3463,  3473,  3474,  3475,  3476,  3477,  3478,  3479,  3480,  3481,
    3482,  3486,  3490,  3495,  3500,  3505,  3511,  3517,  3549,  3550,
    3554,  3555,  3556,  3560,  3561,  3562,  3563,  3572,  3573,  3576,
    3577,  3578,  3582,  3594,  3606,  3613,  3615,  3617,  3619,  3621,
    3627,  3640,  3641,  3645,  3649,  3656,  3657,  3661,  3662,  3666,
    3667,  3668,  3669,  3670,  3671,  3672,  3673,  3678,  3683,  3687,
    3692,  3696,  3705,  3710,  3719,  3720,  3721,  3722,  3723,  3724,
    3725,  3726,  3727,  3728,  3729,  3730,  3731,  3732,  3733,  3734,
    3735,  3736,  3737,  3738,  3739,  3740,  3741,  3745,  3746,  3747,
    3748,  3749,  3750,  3753,  3757,  3761,  3765,  3769,  3776,  3780,
    3784,  3788,  3796,  3797,  3798,  3799,  3800,  3801,  3802
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYCHPL_DEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TIDENT",
  "TQUERIEDIDENT", "INTLITERAL", "REALLITERAL", "IMAGLITERAL",
  "STRINGLITERAL", "BYTESLITERAL", "CSTRINGLITERAL", "EXTERNCODE",
  "TALIGN", "TAS", "TATOMIC", "TBEGIN", "TBREAK", "TBOOL", "TBORROWED",
  "TBY", "TBYTES", "TCATCH", "TCLASS", "TCOBEGIN", "TCOFORALL", "TCOMPLEX",
  "TCONFIG", "TCONST", "TCONTINUE", "TDEFER", "TDELETE", "TDEPRECATED",
  "TDMAPPED", "TDO", "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT",
  "TEXTERN", "TFALSE", "TFOR", "TFORALL", "TFOREACH", "TFORWARDING", "TIF",
  "TIMAG", "TIMPORT", "TIN", "TINCLUDE", "TINDEX", "TINLINE", "TINOUT",
  "TINT", "TITER", "TINITEQUALS", "TIMPLEMENTS", "TINTERFACE", "TLABEL",
  "TLAMBDA", "TLET", "TLIFETIME", "TLOCAL", "TLOCALE", "TMANAGE",
  "TMINUSMINUS", "TMODULE", "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING",
  "TON", "TONLY", "TOPERATOR", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED",
  "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC",
  "TPROTOTYPE", "TPUBLIC", "TREAL", "TRECORD", "TREDUCE", "TREF",
  "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSHARED",
  "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS",
  "TTHROW", "TTHROWS", "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT",
  "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUNSTABLE", "TUSE", "TVAR",
  "TVOID", "TWHEN", "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP",
  "TALIAS", "TAND", "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR",
  "TASSIGNDIVIDE", "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR",
  "TASSIGNMINUS", "TASSIGNMOD", "TASSIGNMULTIPLY", "TASSIGNPLUS",
  "TASSIGNREDUCE", "TASSIGNSL", "TASSIGNSR", "TBANG", "TBAND", "TBNOT",
  "TBOR", "TBXOR", "TCOLON", "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT",
  "TDOTDOTDOT", "TEQUAL", "TEXP", "TGREATER", "TGREATEREQUAL", "THASH",
  "TIO", "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOTEQUAL", "TOR",
  "TPLUS", "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR",
  "TSWAP", "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE",
  "TDOTDOTOPENHIGH", "TUPLUS", "TUMINUS", "TLNOT", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt", "tryable_stmt",
  "deprecated_decl_stmt", "$@1", "$@2", "decl_base", "unstable_decl_stmt",
  "$@3", "$@4", "module_decl_start", "module_decl_stmt",
  "opt_access_control", "opt_prototype", "include_module_stmt",
  "block_stmt_body", "block_stmt", "stmt_ls", "renames_ls",
  "use_renames_ls", "opt_only_ls", "use_stmt", "import_stmt",
  "import_expr", "import_ls", "require_stmt", "assignment_stmt",
  "opt_label_ident", "ident_fn_def", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "do_stmt", "return_stmt", "deprecated_class_level_stmt", "$@5", "$@6",
  "unstable_class_level_stmt", "$@7", "$@8", "class_level_stmt", "$@9",
  "$@10", "inner_class_level_stmt", "forwarding_decl_stmt",
  "forwarding_decl_start", "extern_or_export",
  "extern_export_decl_stmt_start", "extern_export_decl_stmt", "$@11",
  "$@12", "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
  "catch_expr", "catch_expr_inner", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "manager_expr", "manager_expr_ls",
  "manage_stmt", "class_decl_stmt", "class_start", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header_lcbr", "enum_ls", "$@13", "$@14", "deprecated_enum_item",
  "$@15", "$@16", "unstable_enum_item", "$@17", "$@18", "enum_item",
  "lambda_decl_start", "lambda_decl_expr", "$@19", "linkage_spec_empty",
  "linkage_spec", "fn_decl_stmt_complete", "fn_decl_stmt", "$@20",
  "fn_decl_stmt_inner", "fn_decl_stmt_start", "fn_decl_receiver_expr",
  "fn_ident", "op_ident", "assignop_ident", "all_op_name",
  "formal_var_arg_expr", "opt_formal_ls", "req_formal_ls",
  "formal_ls_inner", "formal_ls", "formal", "opt_formal_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_iter_or_op",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner_ls", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt", "$@21",
  "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "ret_array_type", "opt_ret_type", "opt_type",
  "array_type", "opt_formal_array_elt_type", "formal_array_type",
  "opt_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "sub_type_level_expr", "for_expr",
  "cond_expr", "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "task_var_prefix", "io_expr",
  "new_maybe_decorated", "new_expr", "let_expr", "expr", "opt_expr",
  "opt_try_expr", "lhs_expr", "call_base_expr", "call_expr", "dot_expr",
  "parenthesized_expr", "bool_literal", "str_bytes_literal",
  "literal_expr", "assoc_expr_ls", "binary_op_expr", "unary_op_expr",
  "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1159)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-739)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1159,   176,  3951, -1159,   -62,    86, -1159, -1159, -1159, -1159,
   -1159, -1159, 12189,   118,   253,   207, 14895,   239, 20850,   118,
   12189,   260,   270,   280,   253,  5359, 12189,   454,  5359,   304,
   20937, -1159,   467,   436,  9041, 10441, 10441, -1159,  9215,   456,
     421,   333, -1159,   457, 20937, 20937, 20937, -1159,  2387, 10615,
     466, 12189, 12189,   142, -1159,   489,   495, 12189, -1159, 14895,
   -1159, 12189,   522,   379,   205,   206,   520, 21024, -1159, 10791,
    8517, 12189, 10615, 14895, 12189,   468,   525,   428,  5359,   549,
   12189,   558,  7467,  7467, -1159,   564, -1159, 14895,   603, -1159,
     565,  9215,  9389, -1159, 12189, -1159, 12189, -1159, -1159,  3769,
   12189, -1159, 12189, -1159, -1159, -1159,  4303,  7643,  9565, 12189,
   -1159,  5183, -1159, -1159, -1159, -1159, -1159,   476, -1159,   261,
   -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159,
   -1159, -1159, -1159,  7817, -1159, 10965, -1159, -1159, -1159, -1159,
   -1159,   571, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159,
     492, 20937, -1159, 20571,   484,   449, -1159,   193, -1159, -1159,
     235,   342, -1159, 20937,  2387, -1159,   497, -1159,   510, -1159,
   -1159,   516,   532,   542, 12189,   536,   538, 20050,  3315,   462,
     539,   540, -1159, -1159,   477, -1159, -1159, -1159, -1159, -1159,
      63, -1159, -1159, -1159, 12189, 12189, 12189, 20937, -1159, 12189,
   10791, 10791,   651,   485, -1159, -1159, -1159, -1159, 20050,   501,
   -1159, -1159,   541,  5359, -1159, -1159,   543,   155,   552, 16750,
   -1159,   243, -1159,   544, -1159,   -38, 20050, -1159, 20454,   591,
    8693, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159,
   -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159,
   -1159, -1159, -1159, -1159, -1159,   556, -1159, -1159, 20937,   553,
      22, 16332,    38, 16165,    38, 16248, 20937, 20937,   -10,  2967,
   -1159, -1159,   627,  8693,   561,   507,  5359,  1473,    66,    89,
   -1159,  5359, -1159, -1159, 16832, -1159,    44, 17420,   524, -1159,
     568,   573, -1159, 16832,   155,   524, -1159,  8693,  1697,  1697,
   -1159, -1159,   152, 20050, 12189, 12189, -1159, 20050,   547, 17502,
   -1159, 16832,   155, 20050,   574,  8693, -1159, 20050, 17585, -1159,
   -1159, -1159, -1159,   155, -1159, 20454,    22, 16832,   562,   185,
     185,  1814,   524,   524,   -42, -1159, -1159,  4479,     3, -1159,
   12189, -1159,   139,   190, -1159,   -36,    64, 17667,   107,  1814,
     721, -1159,  4655, 12189, -1159, 12189,   671, -1159, -1159, 16416,
      34,   492, 20050,   555, 20937, 10791,   567, -1159,   572,   740,
     744,   628,   147, -1159, -1159, -1159,   275,   235, -1159, -1159,
   -1159, -1159,   663, -1159, -1159, -1159,   102,   665, -1159, -1159,
   -1159, 14411,   630,   221, -1159,   276,   506, -1159, 12189,   609,
   12189, 12189, 12189, 10441, 10441, 12189,   511, 12189, 12189, 12189,
   12189, 12189,   337,  3769, 12189, 12189, 12189, 12189, 12189, 12189,
   12189, 12189, 12189, 12189, 12189, 12189, 12189, 12189, 12189, 12189,
     690, -1159, -1159, -1159, -1159, -1159,  9389,  9389, -1159, -1159,
   -1159, -1159,  9389, -1159, -1159,  9389,  9389,  8693,  8693, 10441,
   10441, 16915, 16997, 17749,   589,    53, 20937,  8343, -1159, -1159,
    4831, -1159, 10441,    22,  2387, -1159, 12189, -1159, 20454, -1159,
   20937, -1159, -1159, -1159,   627, 12189,   637, -1159,   590,   618,
   -1159, -1159, -1159,   719, 10791, -1159,  5535, 10441, -1159,   595,
   -1159,    22,  5711, 10441, -1159,    22, -1159,    22, 10441, -1159,
      22,   643,   644,  5359,   736,  5359, -1159,   737, 12189,   708,
     602,  8693, 20937, -1159, -1159, -1159,  1473, -1159, -1159,   192,
   -1159, 12363,   656, 12189,  2387, -1159, -1159, 12189, -1159, 20662,
   12189, 12189, -1159,   613, -1159, -1159, 10791, -1159, 20050, 20050,
   -1159,    52, -1159,  8693,   615, -1159,   762,   762, 20454, -1159,
   -1159, -1159, -1159, -1159, -1159, -1159,  9741, -1159, 17831,  7993,
   -1159,  8169, -1159,  5359,   616, 10441,  9917,  4127,   620, 12189,
   11139, -1159, -1159,   208, -1159,  5007, -1159,   310, 17913,   382,
   16500, 20937,  7293,  7293, -1159,   492,   626, -1159,   243, -1159,
     -48,   652,  1301, -1159, -1159, 20937, -1159, 20937, 12189,    -6,
   -1159,    58, -1159, -1159, -1159, -1159, -1159, -1159,   916,   654,
     624, -1159,  3295, -1159,   104, -1159, -1159, 12537,   699, -1159,
     632,   157, -1159,   657, -1159,   658,   660,   669,   662,   667,
   -1159,   668,   673,   670,   675,   677,   312,   679,   685,   688,
   -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159,
   -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159,
   -1159, -1159, -1159, 12189, -1159,   681,   687,   691,   632, -1159,
     632, -1159, 12711, -1159, 20937, -1159, -1159, 17995,  2378,  2378,
     513, -1159,   513, -1159,   513, 20291,   812,   743,   500,   155,
     185, -1159,   514, -1159, -1159, -1159, -1159, -1159,  1814,  3110,
     513,  2483,  2483,  2378,  2483,  2483,  2132,   185,  3110, 20210,
    2132,   524,   524,   185,  1814,   646,   676,   682,   689,   692,
     693,   686,   694, -1159,   513, -1159,   513, 12885, 10441, 13059,
   10441, 12189,  8693, 10441, 15179,   678,   189, -1159, -1159, -1159,
     201, -1159,  1418, 20133,   474,   118, 18077, -1159,   396, 20050,
   -1159, 18159,  8693, -1159,  8693, 12189,   203,  9215, 20050,    56,
   17079,  8343, -1159,  9215, 20050,    43, 16583, -1159, -1159,    38,
   16667, -1159, 12189, 12189,   826,  5359,   827, 18241,  5359, 17162,
   20937, -1159,   281, -1159,   284, -1159,   305,  1473,    66,     7,
      60, 12189, 12189,  7119, -1159, -1159,   647,  8867, -1159, 20050,
   -1159, -1159, -1159, 20937, 18323, 18405, -1159, -1159, 20050,   695,
     170,   700, -1159,  1969, -1159, -1159, -1159, -1159, -1159, -1159,
   -1159, -1159, -1159,  5359,    -8, 17246, -1159, -1159, 20050,  5359,
   20050, -1159, 18487, -1159, -1159, 12189, -1159,    83, 15250, 12189,
   -1159, 11313,  7293,  7293, -1159, 12189,   509, 12189,   515,   537,
   12189, 10091,   551,   419, -1159, -1159, -1159, 18569,   716,   709,
     706, -1159, 20937,  2387, -1159,  8693,   707,   764, 20937, -1159,
   20937, -1159, 20050, 20763,   751, -1159, -1159, 20937,  1089, -1159,
    1473,   735,    32,  3583, -1159, -1159,   368, -1159,    32,   275,
   -1159, 18651, -1159, 15410, -1159, -1159, -1159,   426, -1159,   712,
     714, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, 12189,
     849, 18733, 12189,   850, 18815,   317,   717, 18897,  8693, -1159,
   -1159,  8343, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159,
   -1159, -1159, -1159, -1159,    51, 10441, 10441,    22,    22, -1159,
   -1159, -1159, -1159, -1159, 16832, -1159, 15493,  5887, -1159,  6063,
   -1159,   336, -1159, 15577,  6239, -1159,    22,  6415, -1159,    22,
   -1159,    22, -1159,    22, 20050, 20050,  5359, -1159,  5359, 12189,
   -1159,  5359,   859,   729,   732, 20937,   552, -1159, -1159,   656,
     723,    81, -1159, -1159, -1159,   734,   741, -1159,  6591, 10791,
   -1159, -1159, -1159, 20937, -1159,   756,   552, -1159,  6767,   739,
    6943,   742, -1159, 12189, -1159, -1159,  7293, -1159, 18979,    65,
   17338,   430,   747,  7293, -1159, 12189, -1159, -1159,  2812,   433,
     437,   351, -1159,   900, -1159, -1159,   908, -1159, 20387, -1159,
   -1159, -1159, -1159, -1159, -1159, -1159,   735, -1159,   354, 11489,
     125,    75, 12189, -1159,   749, 14277,    84, -1159,   745, -1159,
     776,   788,   632,   632, -1159, -1159, 19062, 13233, 13407, 19144,
   13581, 13755, -1159, 13929, 14103,   361, -1159,   656,   362,   391,
   -1159, -1159, -1159,  5359,  9215, 20050,  9215, 20050,  8343, -1159,
    5359,  9215, 20050, -1159,  9215, 20050, -1159, -1159, -1159, -1159,
   -1159, 20050,   897,  5359, -1159, -1159, -1159, -1159, -1159, -1159,
   10441, 15037,  8693,  8693,  5359, -1159,    48,   759, 12189, -1159,
    9215, -1159, 20050,  5359,  9215, -1159, 20050,  5359, 20050,   183,
   11663,  7293,  7293,  7293,  7293, -1159, -1159, -1159, 19226, 20050,
   20229, -1159, -1159, -1159,   768, -1159, 20485, -1159, 20485, -1159,
   -1159,   125,   735, 10267, -1159, -1159, -1159,   -16, 10791, -1159,
   -1159, -1159,    96, -1159,    -2, -1159,   330, 19308,    55, -1159,
   -1159,   398, 14580,   -22, -1159, -1159, -1159, 12189, 12189, 12189,
   12189, 12189, 12189, 12189, 12189, -1159, -1159, 18241, 15661, 15745,
   -1159, 18241, 15829, 15913,  5359, -1159, 19390, -1159,  1052,   765,
     766,   591, -1159,   552, 20050, 15997, -1159, 16081, -1159, -1159,
   -1159, 20050,   444,   772,   450,   775, 12189, -1159, -1159, 20485,
   -1159, 20485, -1159, -1159, -1159,   125, 11839,    88, -1159, 20050,
   -1159, 12189,    75,    96,    96,    96,    96,    96,    96,    75,
   -1159, -1159, -1159, -1159,   441, -1159, -1159, -1159, 19472, 19558,
   19640, 19722, 19804, 19886,  5359,  5359,  5359,  5359, -1159, 14103,
     779,   781, -1159,  5359,  5359, -1159, -1159, -1159, -1159, 20050,
   -1159, -1159, -1159, -1159, -1159, -1159, -1159,   158, 10441, 14722,
   20050, -1159, -1159, -1159, -1159, -1159, -1159, -1159,   800, 12189,
   12189, 12189, 12189, 12189, 12189, 18241, 18241, 18241, 18241,  1074,
   -1159, -1159, 18241, 18241, 19968, -1159, -1159, 12015, -1159
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   120,   681,   682,   683,   677,
     678,   684,     0,   601,   106,   141,   570,   148,   572,   601,
       0,   147,   478,   476,   106,     0,     0,    47,     0,   271,
     142,   189,   188,   675,     0,     0,     0,   187,     0,   146,
      59,   272,   343,   143,     0,     0,     0,   338,     0,     0,
     150,     0,     0,   620,   592,   685,   151,     0,   344,   564,
     473,     0,     0,     0,   174,   172,   145,   573,   475,     0,
       0,     0,     0,   568,     0,     0,   149,     0,     0,   121,
       0,   676,     0,     0,   465,   144,   309,   566,    53,   477,
     152,     0,     0,   734,     0,   736,     0,   737,   738,   647,
       0,   735,   732,   551,   170,   733,     0,     0,     0,     0,
       4,     0,     5,     9,    10,    44,    11,     0,    49,    62,
      12,    68,    13,    14,    15,    16,    28,   547,   548,    22,
      50,   171,   181,     0,   190,   648,   182,    17,    30,    29,
      19,     0,   266,    18,   639,    21,    34,    31,    32,   180,
     310,     0,   178,     0,     0,   636,   342,     0,   176,   345,
     435,   426,   179,     0,     0,   177,   653,   632,   549,   633,
     554,   552,     0,     0,     0,   637,   638,     0,   553,     0,
     654,   655,   656,   679,   680,   631,   556,   555,   634,   635,
       0,    27,   572,   142,     0,     0,     0,     0,   573,     0,
       0,     0,     0,   636,   653,   552,   637,   638,   562,   553,
     654,   655,     0,     0,   602,   107,     0,   571,     0,   601,
     466,     0,   474,     0,    20,     0,   532,    45,   341,     0,
     539,   114,   122,   134,   128,   127,   136,   117,   126,   137,
     123,   138,   115,   139,   132,   125,   133,   131,   129,   130,
     116,   118,   124,   135,   140,     0,   119,   198,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      61,    60,    62,   539,     0,     0,     0,     0,     0,     0,
     481,     0,   158,    39,     0,   301,     0,   300,   719,   624,
     621,   622,   623,     0,   565,   720,     7,   539,   341,   341,
     447,   529,     0,   528,     0,     0,   159,   652,     0,     0,
      42,     0,   569,   557,     0,   539,    43,   563,     0,   279,
     283,   280,   283,   567,    51,   341,     0,     0,     0,   721,
     723,   645,   718,   717,     0,    65,    69,     0,     0,   534,
       0,   536,     0,     0,   535,     0,     0,   528,     0,   646,
       0,     6,     0,     0,    63,     0,     0,   478,   186,     0,
     677,   310,   649,   194,     0,     0,     0,   306,     0,   330,
     334,   336,     0,   320,   324,   327,   415,   435,   433,   434,
     432,   353,   436,   439,   438,   440,     0,   430,   427,   428,
     431,     0,   470,     0,   467,     0,   550,    33,     0,   625,
       0,     0,     0,     0,     0,     0,   722,     0,     0,     0,
       0,     0,     0,   644,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     385,   392,   393,   394,   389,   391,     0,     0,   387,   390,
     388,   386,     0,   396,   395,     0,     0,   539,   539,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    35,    23,
       0,    36,     0,     0,     0,    24,     0,    37,   341,   307,
       0,   188,   308,    48,    62,     0,   547,   545,     0,   540,
     541,   546,   319,     0,     0,   208,     0,     0,   207,     0,
     216,     0,     0,     0,   214,     0,   224,     0,     0,   222,
       0,     0,     0,     0,   240,     0,   385,   236,     0,     0,
       0,     0,     0,   250,    25,   485,     0,   486,   488,     0,
     510,     0,   491,     0,     0,   157,    38,     0,   303,     0,
       0,     0,    40,     0,   175,   173,     0,    99,   650,   651,
     160,     0,    41,     0,     0,   290,   281,   282,   341,    54,
     201,   200,    26,    67,    66,    70,     0,   686,     0,     0,
     671,     0,   673,     0,     0,     0,     0,     0,     0,     0,
       0,   690,     8,     0,    56,     0,    97,     0,    93,     0,
      77,     0,     0,    83,   183,   310,     0,   196,     0,   193,
     277,   311,     0,   318,   328,     0,   332,     0,     0,   322,
     317,   422,   417,   418,   419,   423,   424,   425,   415,   408,
       0,   405,     0,   416,     0,   437,   505,     0,   441,   429,
     402,   108,   380,   122,   378,   128,   127,   111,   126,   123,
     383,   138,   109,   139,   125,   129,   110,   112,   124,   140,
     377,   359,   362,   360,   361,   384,   372,   363,   376,   368,
     366,   379,   382,   367,   365,   370,   375,   364,   369,   373,
     374,   371,   381,     0,   356,     0,   113,     0,   402,   357,
     402,   354,     0,   469,     0,   464,   480,   600,   714,   713,
     716,   725,   724,   729,   728,   710,   707,   708,   709,   641,
     697,   120,     0,   667,   668,   121,   666,   665,   642,   701,
     712,   706,   704,   715,   705,   703,   695,   700,   702,   711,
     694,   698,   699,   696,   643,     0,     0,     0,     0,     0,
       0,     0,     0,   727,   726,   731,   730,     0,     0,     0,
       0,     0,     0,     0,   688,   277,   612,   613,   615,   617,
       0,   604,     0,     0,     0,   601,   601,   204,     0,   533,
      46,     0,     0,   559,     0,     0,     0,     0,   576,     0,
       0,     0,   217,     0,   582,     0,     0,   215,   225,     0,
       0,   223,     0,     0,   239,     0,   235,     0,     0,     0,
       0,   558,     0,   253,     0,   251,     0,   489,     0,   155,
     156,   154,   153,     0,   509,   508,   632,     0,   483,   630,
     482,   302,   299,     0,     0,     0,   664,   531,   530,     0,
       0,     0,   560,     0,   284,    52,   687,   640,   672,   537,
     674,   538,   232,     0,     0,     0,   689,   230,   586,     0,
     692,   691,     0,    58,    57,     0,    92,     0,     0,     0,
      85,     0,     0,    83,    55,   377,   359,   362,   360,   361,
     370,   369,   371,     0,   397,   398,    72,    71,    84,     0,
       0,   312,     0,     0,   274,     0,     0,   341,     0,   331,
       0,   335,   337,     0,     0,   420,   421,     0,   415,   404,
       0,   523,   448,     0,   504,   503,   632,   442,   448,   415,
     352,     0,   358,     0,   348,   349,   472,   632,   468,     0,
       0,   105,   103,   104,   102,   101,   100,   662,   663,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   614,
     616,     0,   603,   141,   148,   147,   146,   143,   150,   151,
     145,   149,   144,   152,     0,     0,     0,     0,     0,   479,
     199,   543,   544,   542,     0,   234,     0,     0,   206,     0,
     205,     0,   607,     0,     0,   212,     0,     0,   210,     0,
     220,     0,   218,     0,   248,   247,     0,   242,     0,     0,
     238,     0,   244,     0,   276,     0,     0,   487,   490,   491,
       0,     0,   492,   493,   298,     0,     0,   292,     0,     0,
     291,   294,   561,     0,   285,   288,     0,   233,     0,     0,
       0,     0,   231,     0,    98,    95,     0,    94,    80,    79,
      78,     0,     0,     0,   184,     0,   185,   312,   341,     0,
       0,     0,   305,   164,   174,   172,   168,   304,   341,   313,
     314,   161,   329,   333,   323,   326,   523,   406,     0,     0,
     491,     0,     0,   339,     0,   495,     0,   346,     0,   355,
     108,   110,   402,   402,   669,   670,     0,     0,     0,     0,
       0,     0,   276,     0,     0,     0,   605,   491,   653,   653,
     203,   202,   209,     0,     0,   575,     0,   574,     0,   606,
       0,     0,   581,   213,     0,   580,   211,   221,   219,   241,
     237,   591,   243,     0,    64,   273,   252,   249,   484,   514,
       0,   688,   539,   539,     0,   296,     0,     0,     0,   286,
       0,   228,   588,     0,     0,   226,   587,     0,   693,     0,
       0,     0,    83,     0,    83,    86,    89,    75,    74,    73,
     341,   191,   197,   195,   278,   162,   341,   166,   341,   315,
     316,   491,   523,     0,   526,   525,   527,   632,   399,   411,
     409,   462,     0,   463,   450,   453,     0,   449,     0,   500,
     498,   632,   688,     0,   403,   350,   351,     0,     0,     0,
       0,     0,     0,     0,     0,   278,   609,   579,     0,     0,
     608,   585,     0,     0,     0,   246,     0,   512,   632,     0,
       0,   297,   295,     0,   289,     0,   229,     0,   227,    96,
      82,    81,     0,     0,     0,     0,     0,   192,   275,   341,
     165,   341,   169,   412,   410,   491,   515,     0,   401,   400,
     461,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     445,   446,   340,   499,   632,   443,   347,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   245,     0,
     626,   627,   287,     0,     0,    88,    91,    87,    90,    76,
     163,   167,   414,   413,   517,   518,   520,   632,     0,   688,
     452,   454,   455,   458,   459,   460,   456,   457,   451,     0,
       0,     0,     0,     0,     0,   578,   577,   584,   583,   632,
     628,   629,   590,   589,     0,   519,   521,   515,   522
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1159, -1159, -1159,     8,  -373,  2581,    37, -1159, -1159, -1159,
    -199, -1159, -1159, -1159, -1159, -1159,   -25,   680, -1159,  -800,
    2753,   596,  -562, -1159,  -812, -1159, -1159,   114, -1159, -1159,
   -1159,   927, -1159,  2908,  -182,  -372, -1159,  -600,  1423,  -736,
     -74, -1159, -1159,   -72, -1159, -1159,  -849, -1159, -1159,   149,
   -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159, -1159,   138,
   -1159,   868, -1159, -1159,   -13,  2229, -1159, -1159, -1159, -1159,
     641, -1159,   -27, -1159, -1159, -1159, -1159,   440, -1159, -1159,
   -1159,  -102, -1159,  -326,  -807, -1159, -1159, -1159, -1159, -1159,
      97, -1159, -1159,    95, -1159, -1159,  -553, -1159,   844, -1159,
   -1159, -1159, -1159,   608, -1159, -1159, -1159, -1159,    82,  -366,
    -169,  -797, -1058,  -634, -1159, -1159,    85,    98,   370, -1159,
   -1159, -1159,   605, -1159, -1159,  -180,   135,    99,  -245,  -237,
    -758, -1159, -1159,   127,   316, -1159,  -205,   860, -1159,  -163,
     472,   212,  -495,  -933,  -981, -1159,  -582,  -660, -1158,  -972,
    -971,   -58, -1159,   131, -1159,  -246,  -498,  -505,   512,  -519,
   -1159, -1159, -1159,  1105, -1159,   -14, -1159, -1159,  -191, -1159,
    -731, -1159, -1159, -1159,  1147,  1368,   -12, -1159,   -64,  1669,
   -1159,  1779,  2213, -1159, -1159, -1159, -1159, -1159, -1159, -1159,
   -1159, -1159,  -450
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   336,   111,   776,   113,   114,   468,   228,
     115,   116,   548,   325,   117,   118,   119,   356,   120,   121,
     122,   337,   858,   579,   859,   123,   124,   576,   577,   125,
     126,   216,   664,   371,   127,   256,   128,   794,   283,   129,
    1029,  1209,  1136,  1030,  1211,  1138,   130,   299,   298,   131,
     132,   133,   134,   135,   136,   588,   862,   137,   138,   916,
     139,   268,   140,   784,   785,   202,   142,   143,   144,   145,
     546,   814,   996,   146,   147,   810,   991,   285,   286,   148,
     149,   150,   151,   366,   867,   152,   153,   372,   873,   874,
     373,   868,   595,   374,   870,   597,   375,   154,   203,  1158,
     156,   157,   158,   159,  1163,   160,   161,   667,   668,   854,
     855,   856,  1149,   890,   377,   609,   610,   611,   612,   613,
     391,   381,   386,   888,  1236,  1232,   477,  1043,  1154,  1155,
    1156,   162,   163,   393,   394,   673,   164,   165,   221,   279,
     280,   518,   519,   798,   885,   618,   522,   795,  1265,  1146,
    1040,   338,   225,   342,   343,   478,   479,   480,   204,   167,
     168,   169,   170,   205,   172,   213,   214,   740,   491,   951,
     741,   742,   173,   174,   206,   207,   177,   363,   481,   209,
     179,   210,   211,   182,   183,   184,   185,   348,   186,   187,
     188,   189,   190
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     208,   395,   796,   608,   217,   218,   308,   744,   219,   446,
     110,   302,   896,   782,   226,   272,   464,   884,  1031,   666,
     853,   786,   261,   263,   265,   669,   269,   510,   328,   473,
     952,  1012,   882,   361,   894,   586,   895,   284,   811,   287,
     288,  1007,   869,   344,   871,   293,  1098,   294,   476,   295,
     346,   533,   520,   809,  1018,   281,   469,   303,   307,   309,
     311,   312,   313,  -271,  1160,  1141,   317,   520,   318,   544,
      70,   281,   495,   497,   500,   323,   954,   281,  1151,   327,
     307,   281,   329,  1213,   330,   489,   691,   331,   332,   947,
     333,   476,   503,  1041,   303,   307,   347,   349,   886,  1151,
     508,   733,  1121,   616,   191,   520,   875,  1150,  -325,   466,
    -524,  1295,   565,   489,  -506,   476,  -272,  1221,   864,   319,
     321,   359,   472,   362,   191,   865,   549,  -293,   467,  1100,
     553,  -524,   733,   476,  1176,  -524,  1268,   523,  1122,  1298,
     563,  -321,  -254,   455,  1235,  1222,    86,    70,   876,   460,
     556,  1042,   449,   897,   564,   568,   450,  1262,  -524,   489,
     289,   460,   399,  -501,   489,  -506,  -321,  1152,   998,  1031,
    -293,  1215,   260,   262,   264,   557,     3,  -506,  1153,  1031,
     230,  1233,   451,   452,   453,   489,   695,   317,   303,   347,
    1066,   527,  -506,   460,  -501,   536,  -506,   521,  -506,  1153,
     566,   721,   722,   474,   301,   463,   979,  -501,  1214,   460,
    1130,   566,   521,  -506,   460,   460,  1127,   402,   307,   460,
     290,  -501,   670,  -506,  -293,  -506,   460,   460,   566,   734,
     697,   566,  -506,   273,   915,   566,   524,   919,   291,   212,
     567,   301,   341,   301,  1266,   988,   345,   378,   617,   943,
     521,   797,   -61,   -60,   570,   292,   215,  1101,   585,   860,
    1162,   307,   382,  -255,  1269,   476,   476,   379,  -501,   750,
      23,   -61,   -60,  -501,   403,  -506,  1148,   380,   404,   920,
    1011,  1031,  1263,   571,  -516,   307,   559,  1210,   989,  1212,
     -61,   -60,   538,   539,   599,  -261,   406,  1296,  -114,   536,
     474,   748,   601,   307,   412,  -516,  -120,   591,   353,  -516,
    1203,   744,  1205,   560,   383,  1032,  -260,  1033,   537,   600,
     -61,   -60,    60,   602,   803,   384,   406,   603,   558,   476,
    1013,   410,  -516,    68,   412,   301,   301,   561,   415,   787,
     691,   578,   990,   580,   385,   555,   354,  1180,   921,   815,
     536,   604,  1067,   303,   605,  1199,    62,   692,  1230,    89,
    1260,   476,  1261,  1230,   562,   606,   788,  1021,   674,   387,
     222,   693,   716,   717,   191,   922,   355,   945,   718,   220,
     833,   719,   720,   863,   607,  1038,   677,   675,   678,   679,
     680,   682,   684,   685,  1220,   686,   687,   688,   689,   690,
     694,   698,   699,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,   710,   711,   712,   713,   714,  1165,  1166,
    1065,   388,  1231,   524,   307,   307,   756,  1231,   754,  -502,
     307,   975,   389,   307,   307,   307,   307,   724,   726,  1144,
     695,  1187,   676,   474,  1119,   743,   696,   534,   535,  -407,
     746,   390,   787,  -116,   749,   974,  1223,   835,   976,  -494,
    -502,  -121,   227,   751,   754,  1272,  1273,  1274,  1275,  1276,
    1277,   744,   303,  -502,   758,   760,   836,   230,   257,   977,
     764,   766,  1224,  1078,  1225,  1226,   770,  -502,  1227,  1228,
    -494,  1062,  -269,   777,  1028,   344,   779,   344,   754,   307,
     301,   787,  -496,  -494,   270,   608,   273,   271,   754,  -611,
    1079,   799,  -259,  -256,   166,   287,   608,  -494,   804,   805,
    1147,   666,  -263,   474,   808,  1134,  1161,   669,  1142,   839,
     296,   307,   402,  -496,  -502,  1175,  -611,   166,  -610,  -502,
     166,   681,   683,   524,   808,  -267,  -496,   307,   840,   307,
     476,  -264,   297,   825,   808,   828,   402,   830,   832,  1202,
    -496,  1204,   939,   936,  -494,  -610,  1013,   450,   314,  -494,
     857,   857,   476,  -471,   956,   959,  -258,  1013,   961,   963,
     674,  -262,  1188,   555,   524,  1014,   872,   723,   725,   403,
     166,  1013,  -471,   404,   166,   166,  1125,  1013,  -736,  1132,
     745,   315,  -736,  1133,  -737,  -268,    42,  -496,  -737,  -341,
    1255,   324,  -496,   403,  -270,  -597,  1257,   404,   166,   301,
    -257,  -265,  -660,   166,  -660,   759,  -738,   364,   744,  -341,
    -738,   765,    58,   999,  1001,   447,   769,   448,   365,  -341,
    -733,   406,   407,  1234,  -733,  1028,   410,   352,   411,   412,
    -661,   891,  -661,   415,   406,  1005,   697,   376,  -660,   410,
    -660,   422,   412,  -594,   587,   406,   415,   426,   427,   428,
     410,   807,   411,   412,  -657,   396,  -657,   415,   460,  -659,
     512,  -659,  -593,   476,  -658,   422,  -658,   899,   942,   900,
     819,   807,   821,   428,   341,  -507,   341,  1267,   397,   398,
    1020,   807,  -598,   824,  -599,  -596,  -595,   456,  -507,   459,
     465,   475,   354,   540,   457,   758,   911,   764,   914,   777,
     307,   917,   828,   460,  1053,   166,   484,   482,   552,   572,
    1289,   937,   938,   983,   511,   981,   476,   581,   592,  -507,
     307,   530,   307,   944,   593,   946,   531,   543,   594,   743,
    1267,   953,   596,   615,   598,   619,   672,  1028,   412,   715,
     964,   965,   732,   752,   753,   754,  -507,   755,   761,   772,
     773,   775,   778,  -507,   780,   402,   781,   508,  1267,   313,
     317,   347,   797,   813,   508,   307,   469,   806,   166,   812,
      22,    23,   823,   166,  -507,  1023,   829,   861,   879,   536,
     470,   878,    31,   471,   887,   889,  -141,  -148,    37,  -147,
    -117,  -146,   901,  -507,  -115,    42,  -143,  -150,  -507,  -151,
    -118,  -507,   892,   578,  -145,  1046,  -149,  1008,  -119,  1010,
     857,   857,   403,   329,  -144,   330,   404,  -152,   332,   333,
     893,    58,   902,    60,   402,    62,   155,  1024,   903,   166,
    1025,   918,   472,   307,    68,   904,  1189,  1190,   905,   906,
     907,   966,   968,  1013,   166,   370,   910,   987,   913,   155,
     908,   347,   155,    84,   992,  1016,    86,  1017,  1026,  1022,
      89,  1039,  1057,  1060,   406,   407,  1054,   941,   409,   410,
    1055,   411,   412,  1063,  1093,  1094,   415,  1056,  1095,  1099,
    1059,   403,  1108,   671,   422,   404,   307,  1102,  1135,   743,
     426,   427,   428,  1126,  1103,  1113,  1137,  -114,  1117,  1164,
     476,   476,   155,   682,   724,  1159,   155,   155,   301,  -116,
     104,  1106,  1184,  1193,  1208,  1075,  1027,  1077,  1256,  1250,
    1251,  1258,  1082,   601,  1290,  1085,  1291,  1222,   575,  1004,
     155,   223,   509,   406,  1139,   155,  1140,  1091,   410,   326,
     411,   412,  1096,   547,   602,   415,  1107,   801,   603,  1035,
    1034,   589,   166,   422,  1048,  1052,  1037,   303,   877,   426,
     427,   428,   614,  1237,  1278,  1271,  1112,  1047,  1116,  1019,
     898,  1118,   604,   358,   857,   605,   800,   350,   166,   978,
       0,  1128,     0,  1129,   166,     0,   606,     0,     0,   508,
     508,     0,     0,   508,   508,   166,     0,   166,   301,     0,
       0,     0,     0,     0,     0,   607,   508,     0,   508,     0,
    1157,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1075,  1077,     0,  1082,  1085,
       0,  1112,  1116,     0,     0,     0,     0,   155,     0,     0,
       0,  1177,  1178,     0,  1179,     0,   743,     0,  1181,  1182,
       0,     0,  1183,   681,   723,   166,     0,     0,     0,   166,
       0,     0,     0,     0,     0,  1217,     0,   166,  1186,   828,
     307,   307,     0,     0,     0,     0,  1194,     0,  1195,     0,
    -511,     0,  1197,     0,     0,     0,     0,   171,  1201,   857,
     857,   857,   857,  -511,     0,     0,   601,     0,     0,     0,
     155,     0,  -513,     0,   301,   155,     0,     0,     0,     0,
     171,   347,     0,   171,     0,  -513,  1219,   602,     0,     0,
       0,   603,     0,     0,  -511,     0,     0,     0,     0,   175,
     828,     0,     0,     0,     0,  1177,  1238,  1239,  1181,  1240,
    1241,  1242,  1243,     0,     0,   604,  -513,     0,   605,     0,
      62,  -511,   175,     0,  1145,   175,     0,     0,  -511,   606,
       0,   155,     0,   171,     0,     0,     0,   171,   171,     0,
       0,     0,     0,  -513,  1259,     0,   155,     0,   607,  -511,
    -513,     0,     0,     0,     0,     0,     0,     0,     0,  1270,
       0,   171,     0,     0,     0,     0,   171,     0,  -511,     0,
       0,  -513,     0,  -511,     0,   175,  -511,     0,     0,   175,
     175,     0,  1285,  1286,  1287,  1288,     0,  1116,     0,     0,
    -513,  1292,  1293,     0,     0,  -513,     0,     0,  -513,     0,
       0,     0,     0,   175,   934,     0,  1294,   828,   175,     0,
       0,     0,     0,     0,     0,     0,     0,  1285,  1286,  1287,
    1288,  1292,  1293,     0,     0,     0,     0,     0,   301,     0,
       0,     0,     0,  1218,     0,  1116,     0,   166,     0,     0,
     166,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   866,     0,   155,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   171,     0,
       0,     0,     0,  -312,     0,     0,     0,  -312,  -312,     0,
     155,     0,  -312,     0,     0,   166,   155,  -312,     0,  -312,
    -312,   166,     0,     0,     0,  -312,     0,   155,     0,   155,
       0,  1264,  -312,     0,     0,  -312,     0,     0,     0,     0,
     175,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     176,     0,     0,     0,     0,  -312,     0,     0,  -312,     0,
    -312,   171,  -312,     0,  -312,  -312,   171,  -312,     0,  -312,
       0,  -312,     0,   176,     0,     0,   176,     0,     0,     0,
       0,     0,     0,     0,  1264,     0,     0,   155,     0,     0,
    -312,   155,     0,  -312,     0,  -312,     0,  -312,     0,   155,
       0,   691,     0,   175,     0,     0,     0,     0,   175,     0,
       0,     0,  1264,     0,     0,   923,     0,     0,   924,     0,
       0,     0,   171,   925,     0,     0,   176,  1068,  1069,     0,
     176,   176,     0,     0,   193,     0,     0,   171,     0,   166,
       0,   166,     0,     0,   926,     0,   166,  -312,     0,   166,
       0,   927,     0,  -312,   176,     0,   231,     0,   166,   176,
     166,   928,     0,   166,   175,     0,     0,     0,     0,   929,
     232,   233,     0,   234,     0,   310,     0,     0,   235,   175,
     166,     0,     0,     0,     0,   930,     0,   236,     0,     0,
     166,     0,   166,   237,     0,     0,     0,   931,     0,   238,
       0,   695,     0,   239,     0,     0,   240,     0,   932,     0,
       0,     0,     0,     0,     0,   933,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,     0,     0,   171,     0,     0,     0,   246,
     247,     0,   248,     0,   249,     0,   250,     0,     0,   251,
       0,   176,     0,   252,   515,   166,   253,     0,     0,     0,
     254,   171,   166,     0,     0,     0,     0,   171,     0,     0,
       0,     0,     0,     0,     0,   166,     0,   175,   171,     0,
     171,     0,     0,     0,     0,     0,   166,     0,     0,   155,
       0,     0,   155,     0,     0,   166,     0,     0,     0,   166,
       0,     0,     0,   175,     0,     0,     0,     0,     0,   175,
       0,     0,     0,     0,   176,     0,   516,     0,     0,   176,
     175,     0,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   155,   171,     0,
       0,   178,   171,   155,     0,     0,     0,     0,     0,     0,
     171,     0,     0,   485,   488,   490,   494,   496,   499,     0,
       0,     0,     0,     0,   178,     0,   166,   178,     0,     0,
       0,     0,     0,     0,     0,   176,     0,   526,     0,   528,
     175,     0,     0,     0,   175,     0,   532,     0,     0,   469,
     176,     0,   175,    22,    23,     0,     0,     0,     0,     0,
       0,     0,     0,   470,   542,    31,   471,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,   178,    42,   550,
     551,   178,   178,     0,     0,     0,   166,   166,   166,   166,
       0,     0,     0,     0,     0,   166,   166,     0,     0,     0,
       0,     0,     0,     0,    58,   178,    60,     0,     0,     0,
     178,   180,     0,     0,     0,   472,     0,    68,     0,     0,
       0,   155,     0,   155,     0,     0,     0,     0,   155,     0,
       0,   155,     0,     0,   180,     0,    84,   180,     0,    86,
     155,     0,   155,    89,     0,   155,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   176,     0,
       0,     0,   155,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   155,     0,   155,     0,   402,     0,     0,     0,
       0,     0,     0,     0,   176,     0,     0,   180,     0,     0,
     176,   180,   180,     0,     0,     0,     0,     0,     0,     0,
       0,   176,     0,   176,     0,     0,     0,     0,     0,     0,
     171,     0,   178,   171,     0,   180,   747,     0,     0,     0,
     180,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,   404,     0,     0,
       0,     0,     0,     0,   762,     0,     0,   155,   767,     0,
     768,     0,   175,   771,   155,   175,     0,     0,   171,     0,
       0,   176,     0,     0,   171,   176,     0,   155,     0,     0,
       0,     0,     0,   176,     0,   178,     0,     0,   155,     0,
     178,     0,     0,     0,     0,   406,   407,   155,   408,   409,
     410,   155,   411,   412,     0,     0,     0,   415,     0,     0,
     175,     0,   231,     0,   421,   422,   175,     0,   425,     0,
       0,   426,   427,   428,     0,     0,   232,   233,     0,   234,
       0,     0,   180,     0,   235,     0,     0,     0,     0,     0,
       0,     0,     0,   236,     0,     0,   178,     0,     0,   237,
       0,     0,     0,     0,     0,   238,     0,     0,     0,   239,
       0,   178,   240,     0,     0,     0,     0,     0,   155,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,   242,
     243,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,   171,     0,   171,   180,   245,     0,     0,   171,
     180,     0,   171,     0,     0,   246,   247,     0,   248,     0,
     249,   171,   250,   171,     0,   251,   171,     0,     0,   252,
       0,     0,   253,     0,     0,     0,   254,     0,   155,   155,
     155,   155,     0,   171,   175,     0,   175,   155,   155,     0,
       0,   175,     0,   171,   175,   171,     0,     0,     0,     0,
       0,     0,     0,   175,     0,   175,   180,     0,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
       0,   180,     0,     0,     0,   175,     0,     0,     0,     0,
     460,     0,   993,   176,     0,   175,   176,   175,     0,     0,
       0,     0,     0,     0,     0,   178,     0,     0,     0,     0,
       0,   178,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,   178,     0,   178,     0,     0,     0,   171,     0,
       0,     0,   948,   950,     0,   171,     0,     0,   955,   958,
       0,   176,   960,   962,     0,     0,     0,   176,   171,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   171,
       0,     0,     0,     0,     0,   181,     0,     0,   171,     0,
     175,   403,   171,     0,     0,   404,     0,   175,     0,     0,
       0,   141,   178,     0,     0,     0,   178,     0,   181,   180,
     175,   181,     0,     0,   178,     0,     0,     0,     0,     0,
       0,   175,     0,     0,   141,     0,     0,   141,     0,     0,
     175,     0,     0,     0,   175,   180,     0,     0,     0,     0,
       0,   180,     0,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   180,     0,   180,   415,     0,     0,     0,   171,
       0,   181,     0,   422,     0,   181,   181,     0,     0,   426,
     427,   428,     0,     0,     0,     0,     0,   141,     0,     0,
       0,     0,     0,     0,     0,   176,     0,   176,     0,   181,
       0,     0,   176,     0,   181,   176,     0,     0,     0,     0,
       0,   175,     0,     0,   176,   141,   176,     0,     0,   176,
     141,     0,   180,     0,     0,     0,   180,     0,     0,   171,
     171,   171,   171,     0,   180,     0,   176,     0,   171,   171,
    1070,  1071,     0,     0,     0,     0,   176,  1072,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1083,
       0,     0,  1086,     0,  1087,     0,  1088,     0,     0,     0,
     231,   175,   175,   175,   175,     0,     0,     0,     0,     0,
     175,   175,     0,     0,   232,   233,     0,   234,     0,     0,
     402,     0,   235,     0,     0,     0,     0,     0,     0,     0,
       0,   236,     0,     0,     0,     0,   181,   237,     0,     0,
       0,     0,     0,   238,     0,     0,     0,   239,     0,     0,
     240,   176,   141,     0,   178,     0,     0,   178,   176,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
       0,   176,     0,     0,     0,   244,     0,   403,     0,     0,
       0,   404,   176,     0,   245,     0,     0,     0,     0,     0,
       0,   176,     0,   246,   247,   176,   248,     0,   249,   181,
     250,     0,   178,   251,   181,     0,     0,   252,   178,     0,
     253,     0,     0,   405,   254,   141,     0,     0,     0,     0,
     141,     0,     0,     0,     0,   402,     0,     0,     0,   406,
     407,     0,   408,   409,   410,     0,   411,   412,   413,  1192,
     414,   415,   416,   417,     0,     0,   419,   420,   421,   422,
     423,   424,   425,     0,     0,   426,   427,   428,     0,     0,
     181,     0,   176,     0,   180,     0,   429,   180,     0,     0,
     277,     0,     0,     0,     0,   181,   141,     0,     0,     0,
       0,     0,   403,     0,     0,     0,   404,     0,     0,     0,
       0,   141,     0,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   180,     0,     0,     0,   224,     0,   180,   229,
       0,     0,   176,   176,   176,   176,   178,     0,   178,     0,
       0,   176,   176,   178,   406,   407,   178,   408,   409,   410,
       0,   411,   412,   413,     0,   178,   415,   178,     0,     0,
     178,     0,     0,   421,   422,     0,     0,   425,     0,     0,
     426,   427,   428,     0,     0,     0,     0,   178,     0,   316,
       0,   429,     0,     0,     0,     0,     0,   178,     0,   178,
       0,     0,     0,   181,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   112,     0,   141,
       0,     0,   351,     0,     0,     0,     0,     0,     0,   181,
       0,     0,     0,     0,     0,   181,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   141,   181,     0,   181,     0,
       0,   141,     0,     0,     0,     0,   180,     0,   180,     0,
       0,     0,   141,   180,   141,     0,   180,     0,     0,     0,
       0,     0,   178,     0,     0,   180,     0,   180,     0,   178,
     180,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   178,     0,     0,     0,     0,   180,     0,     0,
       0,     0,     0,   178,     0,     0,   181,   180,     0,   180,
     181,     0,   178,     0,     0,     0,   178,     0,   181,     0,
       0,     0,   141,     0,   458,     0,   141,     0,     0,     0,
       0,     0,   282,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   282,     0,     0,     0,     0,
       0,     0,     0,     0,   469,   320,   322,     0,    22,    23,
       0,     0,     0,  1023,     0,     0,     0,     0,   470,     0,
      31,   471,   180,   178,     0,     0,    37,   514,     0,   180,
       0,     0,   525,    42,     0,     0,     0,     0,     0,     0,
       0,     0,   180,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,     0,     0,     0,     0,     0,    58,
       0,    60,   180,    62,     0,  1024,   180,     0,  1025,     0,
     472,     0,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   178,   178,   178,   178,     0,   112,     0,
       0,    84,   178,   178,    86,     0,  1026,     0,    89,     0,
       0,     0,     0,   112,     0,     0,     0,     0,   255,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   274,   275,   276,     0,   278,     0,     0,     0,
       0,     0,     0,   180,     0,     0,     0,     0,     0,     0,
       0,   461,     0,     0,     0,     0,     0,     0,   104,   400,
       0,     0,     0,     0,  1131,     0,   401,     0,   181,     0,
       0,   181,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,   141,     0,     0,   141,     0,     0,
       0,     0,     0,   282,   282,   282,   282,   282,   282,     0,
       0,   504,   507,   180,   180,   180,   180,     0,   513,     0,
       0,     0,   180,   180,     0,     0,   181,   282,     0,   282,
       0,   112,   181,     0,     0,     0,   282,     0,     0,     0,
       0,     0,   141,     0,     0,     0,   403,     0,   141,   367,
     404,     0,     0,     0,   282,     0,     0,   525,     0,   505,
       0,   392,   278,   525,     0,     0,     0,     0,     0,   282,
     282,     0,     0,     0,   774,     0,     0,     0,     0,     0,
       0,     0,   405,   506,   431,   432,   433,   434,   435,     0,
       0,   438,   439,   440,   441,   454,   443,   444,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,     0,   414,
     415,   416,   417,   418,     0,   419,   420,   421,   422,   423,
     424,   425,     0,     0,   426,   427,   428,     0,   460,     0,
       0,     0,   402,     0,   822,   429,     0,     0,   827,     0,
       0,     0,     0,     0,     0,     0,   112,     0,     0,     0,
     181,     0,   181,     0,     0,     0,   483,   181,     0,     0,
     181,     0,     0,     0,   501,   502,   141,     0,   141,   181,
       0,   181,     0,   141,   181,   517,   141,     0,     0,     0,
       0,     0,     0,     0,     0,   141,     0,   141,     0,   403,
     141,   181,     0,   404,     0,     0,     0,     0,     0,     0,
       0,   181,     0,   181,     0,     0,   282,   141,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   141,     0,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   282,     0,     0,     0,   282,     0,
     282,   406,   407,   282,   408,   409,   410,     0,   411,   412,
     413,     0,     0,   415,   416,   417,     0,     0,   419,   420,
     421,   422,   590,     0,   425,     0,     0,   426,   427,   428,
       0,     0,     0,     0,     0,     0,   181,     0,   429,     0,
       0,     0,     0,   181,     0,     0,     0,     0,   231,   665,
       0,     0,   141,     0,     0,     0,   181,     0,     0,   141,
       0,     0,   232,   233,     0,   234,     0,   181,     0,     0,
     235,     0,   141,     0,     0,     0,   181,     0,     0,   236,
     181,     0,     0,   141,     0,   237,     0,     0,     0,     0,
       0,   238,   141,     0,     0,   239,   141,     0,   240,     0,
       0,     0,     0,     0,     0,     0,   967,     0,   241,   970,
       0,     0,     0,     0,   735,   242,   243,     0,     0,     0,
       0,     0,   278,   244,     0,     0,     0,     0,   255,     0,
       0,     0,   245,     0,     0,     0,     0,     0,     0,     0,
       0,   246,   247,     0,   248,     0,   249,   181,   250,     0,
       0,   251,     0,     0,   997,   252,     0,     0,   253,     0,
    1002,     0,   254,   141,     0,     0,     0,     0,     0,     0,
     783,     0,     0,     0,   517,     0,     0,     0,     0,     0,
       0,     0,   278,     0,     0,     0,     0,   802,     0,     0,
       0,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   443,   444,     0,   181,   181,   181,
     181,     0,     0,     0,     0,     0,   181,   181,   880,     0,
       0,     0,  -618,   141,   141,   141,   141,     0,     0,     0,
       0,     0,   141,   141,     0,   445,     0,     0,  -657,   844,
    -657,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   282,   282,     0,     0,     0,     0,   282,   282,
     881,     0,   282,   282,     0,     0,     0,     0,   525,     0,
     525,     0,   972,     0,     0,   525,     0,     0,   525,     0,
       0,     0,     0,     0,     0,     0,     0,  1089,     0,  1090,
       0,     0,  1092,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   994,     0,     0,  1105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1111,
       0,  1115,   392,     0,  1044,     0,     5,   300,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,   196,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,     0,  1185,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   199,  1191,    79,     0,   973,    81,
     282,   282,     0,    85,  1196,   517,    87,   282,  1198,   507,
      90,     0,     0,     0,     0,     0,   507,     0,    93,   282,
       0,   984,   282,     0,   282,     0,   282,     0,     0,     0,
       0,   995,     0,     0,    94,    95,    96,    97,    98,  1097,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,  1109,
       0,     0,   105,     0,   200,     0,   107,     0,   201,  1045,
       0,   109,     0,     0,     0,  1248,     0,     0,     0,     0,
     392,   278,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,  1036,    15,    16,   517,    17,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,   665,     0,    29,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,   196,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   198,     0,   282,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     199,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,   783,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   995,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,   507,   507,   102,   103,   507,   507,     0,   105,     0,
     200,     0,   107,     0,   201,     0,  1252,     0,   507,     0,
     507,    -2,     4,     0,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -59,     0,
      40,    41,    42,     0,    43,  -341,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -59,    53,    54,
       0,    55,    56,    57,     0,  -341,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -341,   -59,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,    88,   -59,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,   104,     0,     0,
     105,     0,   106,     0,   107,     0,   108,     0,     4,   109,
       5,     0,     6,     7,     8,     9,    10,    11,     0,  -688,
       0,    12,    13,    14,    15,    16,  -688,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,  -688,
      28,    29,  -688,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -59,     0,    40,    41,    42,     0,
      43,  -341,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -59,    53,    54,     0,    55,    56,    57,
       0,  -341,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -341,   -59,    65,    66,    67,  -688,    68,    69,    70,
    -688,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,    88,   -59,    89,    90,     0,     0,    91,     0,    92,
       0,     0,  -688,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -688,  -688,
      96,  -688,  -688,  -688,  -688,  -688,  -688,  -688,     0,  -688,
    -688,  -688,  -688,  -688,     0,  -688,  -688,  -688,  -688,  -688,
    -688,  -688,  -688,   104,  -688,  -688,  -688,     0,   106,  -688,
     107,     0,   108,     0,   334,  -688,     5,   300,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -59,     0,    40,    41,    42,     0,    43,  -341,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -59,
      53,    54,     0,    55,    56,    57,     0,  -341,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -341,   -59,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,    88,   -59,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,   104,
       0,     0,   105,     0,   106,   335,   107,     0,   108,     0,
       4,   109,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -59,     0,    40,    41,
      42,     0,    43,  -341,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -59,    53,    54,     0,    55,
      56,    57,     0,  -341,     0,     0,    58,    59,    60,    61,
      62,    63,    64,  -341,   -59,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,    88,   -59,    89,    90,     0,     0,    91,
       0,    92,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,   554,   107,     0,   108,     0,   573,   109,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -59,     0,    40,    41,    42,     0,    43,  -341,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -59,    53,    54,     0,    55,    56,    57,     0,  -341,
       0,     0,    58,    59,    60,    61,    62,    63,    64,  -341,
     -59,    65,    66,    67,     0,    68,    69,    70,     0,    71,
      72,    73,    74,    75,    76,    77,    78,     0,    79,    80,
       0,    81,    82,    83,    84,    85,     0,    86,    87,    88,
     -59,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,   104,     0,     0,   105,     0,   106,   574,   107,     0,
     108,     0,   334,   109,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -59,     0,
      40,    41,    42,     0,    43,  -341,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -59,    53,    54,
       0,    55,    56,    57,     0,  -341,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -341,   -59,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,    88,   -59,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,   104,     0,     0,
     105,     0,   106,   335,   107,     0,   108,     0,     4,   109,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -59,     0,    40,    41,    42,     0,
      43,  -341,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -59,    53,    54,     0,    55,    56,    57,
       0,  -341,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -341,   -59,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,    88,   -59,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,   104,     0,     0,   105,     0,   106,   834,
     107,     0,   108,     0,     4,   109,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -59,     0,    40,    41,    42,     0,    43,  -341,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -59,
      53,    54,     0,    55,    56,    57,     0,  -341,     0,     0,
      58,    59,    60,    61,   350,    63,    64,  -341,   -59,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,    88,   -59,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,   104,
       0,     0,   105,     0,   106,     0,   107,     0,   108,     0,
       4,   109,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -59,     0,    40,    41,
      42,     0,    43,  -341,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -59,    53,    54,     0,    55,
      56,    57,     0,  -341,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -341,   -59,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,    88,   -59,    89,    90,     0,     0,    91,
       0,    92,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,     0,   107,     0,   108,     0,     4,   109,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
     757,    39,   -59,     0,    40,    41,    42,     0,    43,  -341,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -59,    53,    54,     0,    55,    56,    57,     0,  -341,
       0,     0,    58,    59,    60,    61,     0,    63,    64,  -341,
     -59,    65,    66,    67,     0,    68,    69,    70,     0,    71,
      72,    73,    74,    75,    76,    77,    78,     0,    79,    80,
       0,    81,    82,    83,    84,    85,     0,    86,    87,    88,
     -59,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,   104,     0,     0,   105,     0,   106,     0,   107,     0,
     108,     0,     4,   109,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,   763,    39,   -59,     0,
      40,    41,    42,     0,    43,  -341,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -59,    53,    54,
       0,    55,    56,    57,     0,  -341,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -341,   -59,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,    88,   -59,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,   104,     0,     0,
     105,     0,   106,     0,   107,     0,   108,     0,     4,   109,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1074,    39,   -59,     0,    40,    41,    42,     0,
      43,  -341,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -59,    53,    54,     0,    55,    56,    57,
       0,  -341,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -341,   -59,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,    88,   -59,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,   104,     0,     0,   105,     0,   106,     0,
     107,     0,   108,     0,     4,   109,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1076,    39,
     -59,     0,    40,    41,    42,     0,    43,  -341,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -59,
      53,    54,     0,    55,    56,    57,     0,  -341,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -341,   -59,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,    88,   -59,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,   104,
       0,     0,   105,     0,   106,     0,   107,     0,   108,     0,
       4,   109,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,  1081,    39,   -59,     0,    40,    41,
      42,     0,    43,  -341,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -59,    53,    54,     0,    55,
      56,    57,     0,  -341,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -341,   -59,    65,    66,    67,     0,    68,
      69,    70,     0,    71,    72,    73,    74,    75,    76,    77,
      78,     0,    79,    80,     0,    81,    82,    83,    84,    85,
       0,    86,    87,    88,   -59,    89,    90,     0,     0,    91,
       0,    92,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,     0,   107,     0,   108,     0,     4,   109,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1084,    39,   -59,     0,    40,    41,    42,     0,    43,  -341,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -59,    53,    54,     0,    55,    56,    57,     0,  -341,
       0,     0,    58,    59,    60,    61,     0,    63,    64,  -341,
     -59,    65,    66,    67,     0,    68,    69,    70,     0,    71,
      72,    73,    74,    75,    76,    77,    78,     0,    79,    80,
       0,    81,    82,    83,    84,    85,     0,    86,    87,    88,
     -59,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,   104,     0,     0,   105,     0,   106,     0,   107,     0,
     108,     0,     4,   109,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,  1104,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -59,     0,
      40,    41,    42,     0,    43,  -341,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -59,    53,    54,
       0,    55,    56,    57,     0,  -341,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -341,   -59,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,    88,   -59,    89,    90,     0,
       0,    91,     0,    92,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,   104,     0,     0,
     105,     0,   106,     0,   107,     0,   108,     0,     4,   109,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1110,    39,   -59,     0,    40,    41,    42,     0,
      43,  -341,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -59,    53,    54,     0,    55,    56,    57,
       0,  -341,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -341,   -59,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,    88,   -59,    89,    90,     0,     0,    91,     0,    92,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,   104,     0,     0,   105,     0,   106,     0,
     107,     0,   108,     0,     4,   109,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1114,    39,
     -59,     0,    40,    41,    42,     0,    43,  -341,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -59,
      53,    54,     0,    55,    56,    57,     0,  -341,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -341,   -59,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,    88,   -59,    89,
      90,     0,     0,    91,     0,    92,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,   104,
       0,     0,   105,     0,   106,     0,   107,     0,   108,     0,
     980,   109,     5,   300,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,   196,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   198,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     199,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     200,     0,   107,     0,   201,     0,     5,   109,     6,     7,
       8,     9,    10,    11,     0,   622,     0,    12,     0,     0,
      15,    16,   624,    17,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,   196,    39,
       0,     0,     0,    41,     0,     0,    43,     0,   630,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   199,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,   506,
     431,   432,   433,   434,   435,     0,     0,   438,   439,   440,
     441,     0,   443,   444,   845,   846,   847,   848,   849,   645,
       0,   646,     0,    99,     0,   647,   648,   649,   650,   651,
     652,   653,   654,   850,   656,   657,   101,   851,   103,     0,
     659,   660,   852,   662,   200,     0,   107,     0,   201,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,     0,    15,    16,     0,    17,     0,   192,
      19,    20,    21,     0,     0,     0,     0,    26,     0,     0,
      28,    29,     0,   193,     0,     0,     0,    33,    34,    35,
      36,     0,    38,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   197,     0,     0,    47,    48,     0,    49,
      50,    51,    52,     0,    53,    54,     0,    55,    56,    57,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   198,     0,     0,     0,     0,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,    91,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   106,     0,
     107,     0,   108,     0,     0,   109,     5,   300,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,   196,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   199,     0,    79,     0,     0,    81,
     304,   305,     0,    85,   339,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,   340,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   200,     0,   107,     0,   201,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   192,
       0,     0,    21,   357,    23,     0,     0,     0,     0,     0,
       0,    29,     0,   193,     0,     0,     0,    33,   194,   195,
       0,     0,   196,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   197,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,    60,    61,     0,    63,
       0,     0,     0,     0,    66,   198,     0,    68,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   199,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   200,     0,
     107,     0,   201,     0,     0,   109,     5,   300,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,   196,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   199,     0,    79,     0,     0,    81,
     304,   305,     0,    85,   339,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   200,     0,   107,   818,   201,     0,
       0,   109,     5,   300,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,   196,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   198,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     199,     0,    79,     0,     0,    81,   304,   305,     0,    85,
     339,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     200,     0,   107,   820,   201,     0,     5,   109,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   192,     0,     0,    21,     0,
     736,     0,     0,     0,     0,     0,     0,    29,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,   196,    39,
       0,   737,     0,    41,     0,     0,    43,     0,     0,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,   738,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   199,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,   739,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   200,     0,   107,     0,   201,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   193,     0,     0,     0,    33,   194,   195,
       0,     0,   196,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   197,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   198,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   199,     0,
      79,     0,     0,    81,   304,   305,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,   306,     0,     0,   105,     0,   200,     0,
     107,     0,   201,     0,     0,   109,     5,   300,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,   196,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   199,     0,    79,     0,     0,    81,
     304,   305,     0,    85,     0,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   200,     0,   107,     0,   201,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   193,     0,     0,     0,    33,   194,   195,
       0,     0,   196,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   197,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,   982,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   198,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   199,     0,
      79,     0,     0,    81,   304,   305,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   200,     0,
     107,     0,   201,     0,     5,   109,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   193,     0,     0,
       0,    33,   194,   195,     0,     0,   196,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   197,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
     258,    61,     0,    63,     0,     0,     0,     0,    66,   198,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   199,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,   259,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,     0,     0,     0,
     105,     0,   200,     0,   107,     0,   201,     0,     5,   109,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   192,     0,     0,
      21,     0,   266,     0,     0,     0,     0,     0,     0,    29,
       0,   193,     0,     0,     0,    33,   194,   195,     0,     0,
     196,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   197,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   198,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   199,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,   267,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,     0,     0,     0,   105,     0,   200,     0,   107,     0,
     201,     0,     5,   109,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,   196,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   198,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     199,     0,    79,     0,     0,    81,   304,   305,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     200,     0,   107,     0,   201,     0,     0,   109,     5,   300,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   193,     0,     0,     0,    33,   194,   195,     0,     0,
     196,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   197,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   198,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   199,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,   259,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,     0,     0,     0,   105,     0,   200,     0,   107,     0,
     201,     0,     0,   109,     5,   300,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   193,     0,     0,
       0,    33,   194,   195,     0,     0,   196,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   197,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   198,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   199,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,     0,     0,     0,
     105,     0,   200,   816,   107,     0,   201,     0,     0,   109,
       5,   300,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   193,     0,     0,     0,    33,   194,   195,
       0,     0,   196,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   197,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   198,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   199,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   200,     0,
     107,     0,   201,   826,     5,   109,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   193,     0,     0,
       0,    33,   194,   195,     0,     0,   196,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   197,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   198,
    -732,     0,     0,     0,  -732,     0,     0,    73,    74,    75,
      76,    77,   199,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,     0,     0,     0,
     105,     0,   200,     0,   107,     0,   201,     0,     0,   109,
       5,   300,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   193,     0,     0,     0,    33,   194,   195,
       0,     0,   196,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   197,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   198,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   199,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   200,     0,
     107,     0,   201,  1216,     5,   109,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   193,     0,     0,
       0,    33,   194,   195,     0,     0,   196,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   197,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   198,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   199,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,   259,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,     0,     0,     0,
     105,     0,   200,     0,   107,     0,   201,     0,     5,   109,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,   281,    29,
       0,   193,     0,     0,     0,    33,   194,   195,     0,     0,
     196,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   197,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   198,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   199,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,     0,     0,     0,   105,     0,   106,     0,   107,     0,
     201,     0,     0,   109,     5,   300,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   193,     0,     0,
       0,    33,   194,   195,     0,     0,   196,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   197,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   198,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   199,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,     0,     0,     0,
     105,     0,   200,     0,   107,     0,   201,     0,     5,   109,
       6,     7,     8,   360,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,    18,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   193,     0,     0,     0,    33,   194,   195,     0,     0,
     196,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   197,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   199,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,    86,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,     0,     0,     0,   105,     0,   200,     0,   107,     0,
     201,     0,     5,   109,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,   196,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   198,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     199,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     200,     0,   107,     0,   201,   831,     5,   109,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,   196,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   199,     0,    79,     0,     0,    81,
       0,     0,     0,    85,  1009,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   200,     0,   107,     0,   201,     0,
       0,   109,     5,   300,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,   789,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,   196,    39,     0,     0,     0,   790,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   198,     0,     0,
       0,     0,     0,     0,     0,    73,   791,    75,    76,    77,
     792,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     200,     0,   107,     0,  1143,     0,     5,   109,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,   196,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   199,     0,    79,     0,     0,    81,
       0,     0,     0,    85,  1200,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   200,     0,   107,     0,   201,     0,
       0,   109,     5,   300,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,   196,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   198,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     199,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     200,     0,   107,     0,  1143,     0,     0,   109,     5,   300,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   193,     0,     0,     0,    33,   194,   195,     0,     0,
    1174,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   197,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   198,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   199,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,     0,     0,     0,   105,     0,   200,     0,   107,     0,
     201,     0,     5,   109,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,   196,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   198,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     199,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     200,     0,   107,     0,   201,     0,     5,   109,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,   789,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,   196,    39,
       0,     0,     0,   790,     0,     0,    43,     0,     0,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,     0,     0,     0,     0,     0,     0,    73,
     791,    75,    76,    77,   792,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   200,     0,   107,     0,   793,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,   789,     0,   193,     0,     0,     0,    33,   194,   195,
       0,     0,   196,    39,     0,     0,     0,   790,     0,     0,
      43,     0,     0,   197,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   198,     0,     0,     0,     0,
       0,     0,     0,    73,   791,    75,    76,    77,   792,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   200,     0,
     107,     0,   883,     0,     5,   109,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   193,     0,     0,
       0,    33,   194,   195,     0,     0,   196,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   197,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   198,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   199,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,     0,     0,     0,
     105,     0,   200,     0,   107,     0,   793,     0,     5,   109,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   193,     0,     0,     0,    33,   194,   195,     0,     0,
     909,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   197,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   198,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   199,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,     0,     0,     0,   105,     0,   200,     0,   107,     0,
     201,     0,     5,   109,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,   912,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   198,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     199,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     200,     0,   107,     0,   201,     0,     5,   109,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,  1168,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   199,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   200,     0,   107,     0,   201,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   193,     0,     0,     0,    33,   194,   195,
       0,     0,  1169,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   197,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   198,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   199,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   200,     0,
     107,     0,   201,     0,     5,   109,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   192,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   193,     0,     0,
       0,    33,   194,   195,     0,     0,  1171,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   197,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   198,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   199,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,     0,     0,     0,
     105,     0,   200,     0,   107,     0,   201,     0,     5,   109,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   193,     0,     0,     0,    33,   194,   195,     0,     0,
    1172,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   197,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   198,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   199,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,     0,     0,     0,   105,     0,   200,     0,   107,     0,
     201,     0,     5,   109,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   192,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,  1173,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   198,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     199,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     200,     0,   107,     0,   201,     0,     5,   109,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   192,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   193,
       0,     0,     0,    33,   194,   195,     0,     0,  1174,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   197,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   198,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   199,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   200,     0,   107,     0,   201,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   192,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   193,     0,     0,     0,    33,   194,   195,
       0,     0,   196,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   197,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   198,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   199,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   620,     0,   621,     0,     0,     0,    94,    95,
      96,    97,    98,   622,     0,     0,     0,    99,   623,   233,
     624,   625,     0,     0,     0,     0,   626,   100,     0,     0,
     101,   102,   103,     0,     0,   236,   105,   193,     0,     0,
     107,   627,   883,     0,     0,   109,     0,   628,     0,     0,
       0,   239,     0,     0,   629,     0,   630,     0,     0,     0,
       0,     0,     0,     0,   631,     0,     0,     0,     0,     0,
       0,   632,   633,     0,     0,     0,     0,     0,     0,   244,
       0,     0,     0,     0,     0,     0,     0,     0,   634,     0,
       0,     0,     0,     0,     0,     0,     0,   246,   247,     0,
     635,     0,   249,     0,   636,     0,     0,   637,     0,     0,
       0,   638,     0,     0,   253,     0,     0,     0,   639,     0,
       0,     0,     0,     0,     0,     0,     0,   506,   431,   432,
     433,   434,   435,     0,     0,   438,   439,   440,   441,     0,
     443,   444,   640,   641,   642,   643,   644,   645,     0,   646,
       0,     0,     0,   647,   648,   649,   650,   651,   652,   653,
     654,   655,   656,   657,     0,   658,     0,     0,   659,   660,
     661,   662,     0,     5,   663,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   192,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   193,     0,     0,     0,
      33,   194,   195,     0,     0,   196,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   197,     0,     0,    47,
      48,  -497,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   198,     0,
       0,     0,  -497,     0,     0,     0,    73,    74,    75,    76,
      77,   199,     0,    79,     0,  -497,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,     0,    90,     0,  -497,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     0,     5,   300,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   192,   103,  -497,    21,     0,     0,
       0,  -497,     0,   107,     0,   883,    29,     0,   193,     0,
       0,     0,    33,   194,   195,     0,     0,   196,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   197,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     198,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   199,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,  -515,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,     0,     0,     0,  -515,
       0,     0,     0,  -515,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   103,     0,     0,
       0,     0,     0,   200,     0,   107,  -515,  1143,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   192,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   193,     0,     0,     0,    33,   194,   195,     0,     0,
     196,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   197,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   198,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   199,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,     0,    96,     0,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   192,
     103,     0,    21,     0,     0,     0,   200,     0,   107,     0,
     201,    29,     0,   193,     0,     0,     0,    33,   194,   195,
       0,     0,   196,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   197,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   198,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   199,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   192,   103,     0,    21,     0,     0,     0,   200,     0,
     107,     0,   793,    29,     0,   193,     0,     0,     0,    33,
     194,   195,     0,     0,   196,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   197,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,   691,     0,     0,     0,    59,     0,    61,
       0,    63,   622,     0,     0,     0,    66,   198,     0,   624,
     692,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     199,     0,    79,     0,   693,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,     0,   630,     0,     0,     0,     0,
       0,     0,     0,   694,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,   695,   201,     0,     0,     0,     0,   696,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   431,   432,   433,
     434,   435,     0,     0,   438,   439,   440,   441,     0,   443,
     444,   640,   641,   642,   643,   644,   645,     0,   646,     0,
       0,     0,   647,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,  1050,   658,     0,     0,   659,   660,   661,
     662,  1006,   622,     0,     0,     0,     0,   232,   233,   624,
     234,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,     0,     0,     0,   236,     0,     0,     0,     0,     0,
     627,     0,     0,     0,     0,     0,   238,     0,     0,     0,
     239,     0,     0,   240,     0,   630,     0,     0,     0,     0,
       0,     0,     0,   241,     0,     0,     0,     0,     0,     0,
     632,   243,     0,     0,     0,     0,     0,     0,   244,     0,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,   400,   246,   247,     0,   248,
       0,   249,   401,  1051,     0,     0,   637,     0,     0,     0,
     252,     0,     0,   253,     0,   402,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,   506,   431,   432,   433,
     434,   435,     0,     0,   438,   439,   440,   441,     0,   443,
     444,   640,   641,   642,   643,   644,   645,     0,   646,     0,
       0,     0,   647,   648,   649,   650,   651,   652,   653,   654,
     655,   656,   657,     0,   658,     0,     0,   659,   660,   661,
     662,     0,   403,     0,     0,     0,   404,     0,     0,   400,
       0,     0,     0,     0,     0,  1073,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,   405,   506,
     431,   432,   433,   434,   435,     0,     0,   438,   439,   440,
     441,     0,   443,   444,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,     0,   414,   415,   416,   417,   418,
       0,   419,   420,   421,   422,   423,   424,   425,     0,     0,
     426,   427,   428,     0,   460,     0,   403,     0,     0,     0,
     404,   429,     0,   400,     0,     0,     0,     0,     0,  1080,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
       0,     0,   405,   506,   431,   432,   433,   434,   435,     0,
       0,   438,   439,   440,   441,     0,   443,   444,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,     0,   414,
     415,   416,   417,   418,     0,   419,   420,   421,   422,   423,
     424,   425,     0,     0,   426,   427,   428,     0,   460,     0,
     403,     0,     0,     0,   404,   429,     0,   400,     0,     0,
       0,     0,     0,  1244,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,     0,   405,   506,   431,   432,
     433,   434,   435,     0,     0,   438,   439,   440,   441,     0,
     443,   444,   406,   407,     0,   408,   409,   410,     0,   411,
     412,   413,     0,   414,   415,   416,   417,   418,     0,   419,
     420,   421,   422,   423,   424,   425,     0,     0,   426,   427,
     428,     0,   460,     0,   403,     0,     0,     0,   404,   429,
       0,   400,     0,     0,     0,     0,     0,  1245,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,     0,     0,
     405,   506,   431,   432,   433,   434,   435,     0,     0,   438,
     439,   440,   441,     0,   443,   444,   406,   407,     0,   408,
     409,   410,     0,   411,   412,   413,     0,   414,   415,   416,
     417,   418,     0,   419,   420,   421,   422,   423,   424,   425,
       0,     0,   426,   427,   428,     0,   460,     0,   403,     0,
       0,     0,   404,   429,     0,   400,     0,     0,     0,     0,
       0,  1246,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,   405,   506,   431,   432,   433,   434,
     435,     0,     0,   438,   439,   440,   441,     0,   443,   444,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
       0,   414,   415,   416,   417,   418,     0,   419,   420,   421,
     422,   423,   424,   425,     0,     0,   426,   427,   428,     0,
     460,     0,   403,     0,     0,     0,   404,   429,     0,   400,
       0,     0,     0,     0,     0,  1247,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,   405,   506,
     431,   432,   433,   434,   435,     0,     0,   438,   439,   440,
     441,     0,   443,   444,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,     0,   414,   415,   416,   417,   418,
       0,   419,   420,   421,   422,   423,   424,   425,     0,     0,
     426,   427,   428,     0,   460,     0,   403,     0,     0,     0,
     404,   429,     0,   400,     0,     0,     0,     0,     0,  1253,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
       0,     0,   405,   506,   431,   432,   433,   434,   435,     0,
       0,   438,   439,   440,   441,     0,   443,   444,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,     0,   414,
     415,   416,   417,   418,     0,   419,   420,   421,   422,   423,
     424,   425,     0,     0,   426,   427,   428,     0,   460,     0,
     403,     0,     0,     0,   404,   429,     0,   400,     0,     0,
       0,     0,     0,  1254,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,   492,     0,
       0,     0,     0,     0,     0,     0,   405,   506,   431,   432,
     433,   434,   435,   493,     0,   438,   439,   440,   441,     0,
     443,   444,   406,   407,     0,   408,   409,   410,     0,   411,
     412,   413,     0,   414,   415,   416,   417,   418,     0,   419,
     420,   421,   422,   423,   424,   425,     0,     0,   426,   427,
     428,     0,   460,     0,   403,     0,     0,     0,   404,   429,
     400,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,   281,     0,     0,     0,     0,   489,     0,     0,     0,
     405,     0,     0,     0,     0,     0,   498,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   406,   407,     0,   408,
     409,   410,     0,   411,   412,   413,     0,   414,   415,   416,
     417,   418,     0,   419,   420,   421,   422,   423,   424,   425,
       0,     0,   426,   427,   428,     0,   460,   403,     0,     0,
       0,   404,     0,   429,   400,     0,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,   486,     0,     0,     0,   489,
       0,     0,     0,   405,     0,     0,     0,     0,     0,     0,
     487,     0,     0,     0,     0,     0,     0,     0,     0,   406,
     407,     0,   408,   409,   410,     0,   411,   412,   413,     0,
     414,   415,   416,   417,   418,     0,   419,   420,   421,   422,
     423,   424,   425,     0,     0,   426,   427,   428,     0,   460,
       0,   403,     0,     0,     0,   404,   429,     0,   400,     0,
       0,     0,     0,     0,     0,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,     0,     0,   582,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,     0,   414,   415,   416,   417,   418,   583,
     419,   420,   421,   422,   423,   424,   425,     0,     0,   426,
     427,   428,     0,   460,     0,   403,     0,     0,     0,   404,
     429,     0,   400,   841,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,   842,     0,     0,
       0,   405,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   406,   407,     0,
     408,   409,   410,     0,   411,   412,   413,     0,   414,   415,
     416,   417,   418,   843,   419,   420,   421,   422,   423,   424,
     425,     0,   584,   426,   427,   428,     0,     0,     0,   403,
       0,     0,     0,   404,   429,   400,     0,     0,     0,     0,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,   957,     0,     0,     0,
       0,     0,     0,     0,     0,   405,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,     0,   414,   415,   416,   417,   418,     0,   419,   420,
     421,   422,   423,   424,   425,     0,     0,   426,   427,   428,
       0,     0,   403,     0,     0,     0,   404,     0,   429,   400,
       0,     0,     0,     0,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     281,     0,     0,     0,   489,     0,     0,     0,   405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,     0,   414,   415,   416,   417,   418,
       0,   419,   420,   421,   422,   423,   424,   425,     0,     0,
     426,   427,   428,     0,   460,     0,   403,     0,     0,     0,
     404,   429,   400,     0,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,   489,     0,
       0,     0,   405,     0,     0,     0,     0,     0,   462,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,     0,   414,
     415,   416,   417,   418,     0,   419,   420,   421,   422,   423,
     424,   425,     0,     0,   426,   427,   428,     0,   460,   403,
       0,     0,     0,   404,   400,   429,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,   281,     0,     0,     0,     0,
       0,   212,     0,     0,     0,   405,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,     0,   414,   415,   416,   417,   418,     0,   419,   420,
     421,   422,   423,   424,   425,     0,     0,   426,   427,   428,
       0,   403,     0,     0,     0,   404,     0,   400,   429,     0,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,   727,     0,
       0,     0,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,   728,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,     0,   414,   415,   416,   417,   418,     0,
     419,   420,   421,   422,   423,   424,   425,     0,     0,   426,
     427,   428,     0,   460,   403,     0,     0,     0,   404,   400,
     429,     0,     0,     0,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     729,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     405,     0,     0,     0,     0,   730,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   406,   407,     0,   408,
     409,   410,     0,   411,   412,   413,     0,   414,   415,   416,
     417,   418,     0,   419,   420,   421,   422,   423,   424,   425,
       0,     0,   426,   427,   428,     0,   403,     0,     0,     0,
     404,   400,     0,   429,     0,     0,     0,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   949,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   405,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,     0,   414,
     415,   416,   417,   418,     0,   419,   420,   421,   422,   423,
     424,   425,     0,     0,   426,   427,   428,     0,   403,     0,
       0,     0,   404,     0,   400,   429,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
       0,   414,   415,   416,   417,   418,     0,   419,   420,   421,
     422,   423,   424,   425,     0,     0,   426,   427,   428,     0,
     460,   403,     0,     0,     0,   404,     0,   429,   400,     0,
       0,     0,     0,     0,   971,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,     0,   414,   415,   416,   417,   418,     0,
     419,   420,   421,   422,   423,   424,   425,     0,     0,   426,
     427,   428,     0,   460,     0,   403,     0,     0,     0,   404,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   489,     0,     0,
     402,   405,     0,     0,     0,  1123,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   406,   407,     0,
     408,   409,   410,     0,   411,   412,   413,     0,   414,   415,
     416,   417,   418,     0,   419,   420,   421,   422,   423,   424,
     425,  1124,     0,   426,   427,   428,     0,     0,     0,     0,
       0,     0,  1000,     0,   429,     0,     0,   403,     0,     0,
       0,   404,   400,   529,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   405,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   406,
     407,     0,   408,   409,   410,     0,   411,   412,   413,     0,
     414,   415,   416,   417,   418,     0,   419,   420,   421,   422,
     423,   424,   425,     0,     0,   426,   427,   428,     0,   403,
       0,     0,     0,   404,   400,     0,   429,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   405,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,     0,   414,   415,   416,   417,   418,     0,   419,   420,
     421,   422,   423,   424,   425,     0,     0,   426,   427,   428,
       0,   403,     0,     0,     0,   404,     0,   400,   429,     0,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,     0,   414,   415,   416,   417,   418,     0,
     419,   420,   421,   422,   423,   424,   425,     0,     0,   426,
     427,   428,     0,   541,   403,     0,     0,     0,   404,   400,
     429,     0,     0,     0,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     405,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   406,   407,     0,   408,
     409,   410,     0,   411,   412,   413,     0,   414,   415,   416,
     417,   418,     0,   419,   420,   421,   422,   423,   424,   425,
       0,   545,   426,   427,   428,     0,   403,     0,     0,     0,
     404,   400,     0,   429,     0,     0,     0,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   569,   405,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,     0,   414,
     415,   416,   417,   418,     0,   419,   420,   421,   422,   423,
     424,   425,     0,     0,   426,   427,   428,     0,   403,     0,
       0,     0,   404,   400,     0,   429,     0,     0,     0,     0,
     401,   731,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
       0,   414,   415,   416,   417,   418,     0,   419,   420,   421,
     422,   423,   424,   425,     0,     0,   426,   427,   428,     0,
     403,     0,     0,     0,   404,   400,   837,   429,     0,     0,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   405,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   406,   407,     0,   408,   409,   410,     0,   411,
     412,   413,     0,   414,   415,   416,   417,   418,     0,   419,
     420,   421,   422,   423,   424,   425,     0,     0,   426,   427,
     428,     0,   403,     0,     0,   817,   404,   400,     0,   429,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   406,   407,     0,   408,   409,   410,
       0,   411,   838,   413,     0,   414,   415,   416,   417,   418,
       0,   419,   420,   421,   422,   423,   424,   425,     0,     0,
     426,   427,   428,     0,   403,     0,     0,     0,   404,   400,
       0,   429,     0,     0,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     405,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   406,   407,     0,   408,
     409,   410,     0,   411,   412,   413,     0,   414,   415,   416,
     417,   418,  -619,   419,   420,   421,   422,   423,   424,   425,
       0,     0,   426,   427,   428,     0,   403,     0,     0,     0,
     404,   400,     0,   429,     0,     0,     0,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,   212,     0,
       0,     0,   405,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,     0,   414,
     415,   416,   417,   418,     0,   419,   420,   421,   422,   423,
     424,   425,     0,     0,   426,   427,   428,     0,   403,     0,
       0,     0,   404,   400,     0,   429,     0,     0,     0,     0,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,   969,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
       0,   414,   415,   416,   417,   418,     0,   419,   420,   421,
     422,   423,   424,   425,     0,   940,   426,   427,   428,     0,
     403,     0,     0,     0,   404,   400,     0,   429,     0,     0,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   405,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   406,   407,     0,   408,   409,   410,     0,   411,
     412,   413,     0,   414,   415,   416,   417,   418,     0,   419,
     420,   421,   422,   423,   424,   425,     0,     0,   426,   427,
     428,     0,   403,     0,     0,     0,   404,   400,     0,   429,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,     0,   414,   415,   416,   417,   418,
       0,   419,   420,   421,   422,   423,   424,   425,     0,     0,
     426,   427,   428,     0,   403,     0,     0,   985,   404,   400,
       0,   429,     0,     0,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     405,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   406,   407,     0,   408,
     409,   410,     0,   411,   412,   413,     0,   414,   415,   416,
     417,   418,     0,   419,   420,   421,   422,   423,   424,   425,
       0,     0,   426,   427,   428,     0,   403,     0,     0,   986,
     404,   400,  1015,   429,     0,     0,     0,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1003,   405,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,     0,   414,
     415,   416,   417,   418,     0,   419,   420,   421,   422,   423,
     424,   425,     0,     0,   426,   427,   428,     0,   403,     0,
       0,     0,   404,   400,     0,   429,     0,     0,     0,     0,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
       0,   414,   415,   416,   417,   418,     0,   419,   420,   421,
     422,   423,   424,   425,     0,     0,   426,   427,   428,     0,
     403,     0,     0,     0,   404,   400,     0,   429,     0,     0,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,  1058,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   405,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   406,   407,     0,   408,   409,   410,     0,   411,
     412,   413,     0,   414,   415,   416,   417,   418,     0,   419,
     420,   421,   422,   423,   424,   425,     0,     0,   426,   427,
     428,     0,   403,     0,     0,  1049,   404,   400,     0,   429,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,  1061,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   405,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,     0,   414,   415,   416,   417,   418,
       0,   419,   420,   421,   422,   423,   424,   425,     0,     0,
     426,   427,   428,     0,   403,     0,     0,     0,   404,   400,
       0,   429,     0,     0,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     405,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   406,   407,     0,   408,
     409,   410,     0,   411,   412,   413,     0,   414,   415,   416,
     417,   418,     0,   419,   420,   421,   422,   423,   424,   425,
       0,     0,   426,   427,   428,     0,   403,     0,     0,     0,
     404,   400,  1120,   429,     0,     0,     0,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   405,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,     0,   414,
     415,   416,   417,   418,     0,   419,   420,   421,   422,   423,
     424,   425,     0,     0,   426,   427,   428,     0,   403,     0,
       0,     0,   404,  1064,   400,   429,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   405,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
       0,   414,   415,   416,   417,   418,     0,   419,   420,   421,
     422,   423,   424,   425,     0,     0,   426,   427,   428,     0,
       0,   403,     0,     0,     0,   404,   400,   429,     0,     0,
       0,     0,     0,   401,  1167,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,     0,   414,   415,   416,   417,   418,     0,
     419,   420,   421,   422,   423,   424,   425,     0,     0,   426,
     427,   428,     0,   403,     0,     0,     0,   404,   400,  1206,
     429,     0,     0,     0,     0,   401,  1170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   405,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   406,   407,     0,   408,   409,
     410,     0,   411,   412,   413,     0,   414,   415,   416,   417,
     418,     0,   419,   420,   421,   422,   423,   424,   425,     0,
       0,   426,   427,   428,     0,   403,     0,     0,     0,   404,
     400,     0,   429,     0,     0,     0,     0,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   405,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   406,   407,  1229,
     408,   409,   410,     0,   411,   412,   413,     0,   414,   415,
     416,   417,   418,     0,   419,   420,   421,   422,   423,   424,
     425,     0,     0,   426,   427,   428,     0,   403,     0,     0,
       0,   404,   400,     0,   429,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   405,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   406,
     407,     0,   408,   409,   410,     0,   411,   412,   413,     0,
     414,   415,   416,   417,   418,     0,   419,   420,   421,   422,
     423,   424,   425,     0,     0,   426,   427,   428,     0,   403,
       0,     0,     0,   404,   400,     0,   429,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   405,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,     0,   414,   415,   416,   417,   418,     0,   419,   420,
     421,   422,   423,   424,   425,     0,     0,   426,   427,   428,
       0,   403,     0,     0,     0,   404,  1249,     0,   429,     0,
     400,     0,     0,     0,  1279,     0,     0,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,     0,   414,   415,   416,   417,   418,     0,
     419,   420,   421,   422,   423,   424,   425,     0,     0,   426,
     427,   428,     0,     0,     0,     0,     0,   403,     0,     0,
     429,   404,   400,     0,     0,     0,     0,     0,     0,   401,
    1280,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   405,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   406,
     407,     0,   408,   409,   410,     0,   411,   412,   413,     0,
     414,   415,   416,   417,   418,     0,   419,   420,   421,   422,
     423,   424,   425,     0,     0,   426,   427,   428,     0,   403,
       0,     0,     0,   404,   400,     0,   429,     0,     0,     0,
       0,   401,  1281,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   405,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,     0,   414,   415,   416,   417,   418,     0,   419,   420,
     421,   422,   423,   424,   425,     0,     0,   426,   427,   428,
       0,   403,     0,     0,     0,   404,   400,     0,   429,     0,
       0,     0,     0,   401,  1282,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   405,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,     0,   414,   415,   416,   417,   418,     0,
     419,   420,   421,   422,   423,   424,   425,     0,     0,   426,
     427,   428,     0,   403,     0,     0,     0,   404,   400,     0,
     429,     0,     0,     0,     0,   401,  1283,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   405,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   406,   407,     0,   408,   409,
     410,     0,   411,   412,   413,     0,   414,   415,   416,   417,
     418,     0,   419,   420,   421,   422,   423,   424,   425,     0,
       0,   426,   427,   428,     0,   403,     0,     0,     0,   404,
     400,     0,   429,     0,     0,     0,     0,   401,  1284,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   405,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   406,   407,     0,
     408,   409,   410,     0,   411,   412,   413,     0,   414,   415,
     416,   417,   418,     0,   419,   420,   421,   422,   423,   424,
     425,     0,     0,   426,   427,   428,     0,   403,     0,     0,
       0,   404,   400,     0,   429,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   405,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   406,
     407,     0,   408,   409,   410,     0,   411,   412,   413,     0,
     414,   415,   416,   417,   418,     0,   419,   420,   421,   422,
     423,   424,   425,     0,     0,   426,   427,   428,     0,   403,
       0,     0,     0,   404,  1297,   400,   429,     0,     0,     0,
       0,     0,   401,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   405,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,     0,   414,   415,   416,   417,   418,     0,   419,   420,
     421,   422,   423,   424,   425,     0,     0,   426,   427,   428,
       0,     0,   935,     0,     0,     0,   404,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,   469,     0,     0,     0,    22,    23,     0,   405,     0,
    1023,     0,     0,     0,     0,   470,     0,    31,   471,     0,
       0,     0,     0,    37,   406,   407,     0,   408,   409,   410,
      42,   411,   412,   413,     0,   414,   415,   416,   417,   418,
       0,   419,   420,   421,   422,   423,   424,   425,     0,   403,
     426,   427,   428,   404,     0,     0,    58,     0,    60,     0,
      62,   429,  1024,     0,     0,  1025,     0,   472,     0,    68,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   405,     0,     0,    84,     0,
       0,    86,     0,  1026,     0,    89,     0,     0,     0,     0,
       0,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,     0,   414,   415,   416,   417,     0,     0,   419,   420,
     421,   422,   423,     0,   425,     0,     0,   426,   427,   428,
     403,     0,     0,     0,   404,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,   104,     0,     0,     0,     0,
       0,  1207,     0,     0,     0,     0,     0,     0,     0,   469,
       0,     0,     0,    22,    23,     0,     0,     0,  1023,     0,
       0,     0,     0,   470,     0,    31,   471,     0,     0,     0,
       0,    37,   406,   407,     0,   408,   409,   410,    42,   411,
     412,   413,     0,   414,   415,   416,   417,     0,     0,   419,
     420,   421,   422,   423,     0,   425,     0,     0,   426,   427,
     428,     0,     0,     0,    58,     0,    60,     0,   350,   429,
    1024,     0,     0,  1025,     0,   472,   469,    68,     0,     0,
      22,    23,     0,     0,     0,     0,     0,     0,     0,     0,
     470,     0,    31,   471,     0,     0,    84,     0,    37,    86,
       0,  1026,     0,    89,     0,    42,     0,   469,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
     -59,   470,     0,    31,   471,     0,     0,     0,     0,    37,
       0,    58,     0,    60,     0,     0,    42,    64,     0,   -59,
      65,     0,   472,     0,    68,     0,     0,     0,     0,     0,
       0,     0,     0,   104,     0,     0,     0,     0,     0,     0,
       0,     0,    58,    84,    60,     0,    86,     0,  1024,     0,
      89,  1025,   368,   472,   231,    68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   232,   233,
       0,   234,     0,     0,    84,     0,   235,    86,     0,     0,
       0,    89,   369,     0,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,     0,     0,   238,     0,     0,
     104,   239,     0,     0,   240,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
       0,   242,   243,     0,     0,     0,     0,     0,     0,   244,
       0,   104,     0,     0,     0,     0,     0,     0,   245,     0,
       0,     0,     0,     0,     0,   231,     0,   246,   247,     0,
     248,     0,   249,     0,   250,     0,     0,   251,     0,   232,
     233,   252,   234,     0,   253,   370,     0,   235,   254,    23,
       0,     0,     0,     0,     0,     0,   236,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,   238,     0,
       0,     0,   239,     0,     0,   240,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   242,   243,     0,     0,     0,     0,     0,     0,
     244,    60,     0,     0,     0,     0,     0,     0,     0,   245,
       0,     0,    68,     0,     0,     0,     0,     0,   246,   247,
       0,   248,     0,   249,     0,   250,   231,     0,   251,     0,
       0,     0,   252,     0,     0,   253,     0,     0,    89,   254,
     232,   233,     0,   234,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   369,     0,     0,   236,     0,     0,
       0,     0,     0,   237,     0,     0,     0,     0,     0,   238,
       0,     0,     0,   239,     0,     0,   240,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,   242,   243,     0,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,  -307,     0,     0,     0,     0,     0,   246,
     247,     0,   248,     0,   249,     0,   250,  -307,  -307,   251,
    -307,     0,     0,   252,     0,  -307,   253,     0,     0,     0,
     254,     0,     0,     0,  -307,     0,     0,     0,     0,     0,
    -307,     0,     0,     0,     0,     0,  -307,     0,     0,     0,
    -307,     0,     0,  -307,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -307,     0,     0,     0,     0,     0,     0,
    -307,  -307,     0,     0,     0,     0,     0,     0,  -307,     0,
       0,     0,     0,     0,     0,     0,     0,  -307,     0,     0,
     231,     0,     0,     0,     0,     0,  -307,  -307,     0,  -307,
       0,  -307,     0,  -307,   232,   233,  -307,   234,     0,     0,
    -307,     0,   235,  -307,     0,     0,     0,  -307,     0,     0,
       0,   236,     0,     0,     0,     0,     0,   237,     0,     0,
       0,     0,     0,   238,     0,     0,     0,   239,     0,     0,
     240,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,   242,   243,     0,
       0,     0,     0,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,  -308,     0,     0,
       0,     0,     0,   246,   247,     0,   248,     0,   249,     0,
     250,  -308,  -308,   251,  -308,     0,     0,   252,     0,  -308,
     253,     0,     0,     0,   254,     0,     0,     0,  -308,     0,
       0,     0,     0,     0,  -308,     0,     0,     0,     0,     0,
    -308,     0,     0,     0,  -308,     0,     0,  -308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -308,     0,     0,
       0,     0,     0,     0,  -308,  -308,     0,     0,     0,     0,
       0,     0,  -308,     0,     0,     0,     0,     0,     0,     0,
       0,  -308,     0,     0,     0,     0,     0,     0,     0,     0,
    -308,  -308,     0,  -308,     0,  -308,     0,  -308,     0,     0,
    -308,     0,     0,     0,  -308,     0,     0,  -308,     0,     0,
       0,  -308
};

static const yytype_int16 yycheck[] =
{
      12,   164,   521,   376,    16,    19,    70,   457,    20,   178,
       2,    69,   672,   511,    26,    40,   221,   617,   867,   391,
     582,   516,    34,    35,    36,   391,    38,   273,    92,   228,
     761,   843,   614,   135,   668,   361,   670,    49,   543,    51,
      52,   838,   595,   107,   597,    57,   979,    59,   230,    61,
     108,   297,     1,     1,   861,    33,    22,    69,    70,    71,
      72,    73,    74,    56,  1045,  1036,    78,     1,    80,   315,
      92,    33,   263,   264,   265,    87,    33,    33,     3,    91,
      92,    33,    94,  1141,    96,   121,     3,    99,   100,    33,
     102,   273,   102,    61,   106,   107,   108,   109,   617,     3,
     269,    48,    37,     1,   166,     1,    48,  1040,   114,   147,
     126,  1269,    48,   121,    48,   297,    56,   119,   166,    82,
      83,   133,    88,   135,   166,   173,   325,    75,   166,    48,
     172,   147,    48,   315,  1067,   151,    48,    48,    73,  1297,
     176,   147,    56,   201,   166,   147,   112,    92,    90,   171,
     147,   119,    89,   672,   345,   346,    93,  1215,   174,   121,
      18,   171,   174,    61,   121,    61,   172,    92,   176,  1018,
     118,  1142,    34,    35,    36,   172,     0,   126,   103,  1028,
     173,  1162,   194,   195,   196,   121,   103,   199,   200,   201,
     921,   147,   126,   171,    92,   147,    92,   146,   147,   103,
     147,   447,   448,   228,    69,   219,   788,   105,  1141,   171,
    1017,   147,   146,   147,   171,   171,  1013,    32,   230,   171,
      78,   119,   391,   119,   172,   174,   171,   171,   147,   176,
     412,   147,   166,   173,   732,   147,   147,    48,    96,   121,
     176,   106,   107,   108,  1216,    75,   108,    54,   146,   754,
     146,   126,    47,    47,   147,   113,     3,   176,   360,   585,
     176,   273,    27,    56,   176,   447,   448,    74,   166,   468,
      27,    66,    66,   171,    89,   171,   151,    84,    93,    90,
     842,  1130,  1215,   176,   126,   297,   147,  1136,   118,  1138,
      85,    85,   304,   305,   147,    56,   141,  1269,   141,   147,
     325,   464,    27,   315,   149,   147,   149,   365,    47,   151,
    1122,   761,  1124,   174,    79,   868,    56,   870,   166,   172,
     115,   115,    79,    48,   529,    90,   141,    52,   340,   511,
     147,   146,   174,    90,   149,   200,   201,   147,   153,   147,
       3,   353,   172,   355,   109,   337,    85,  1078,   147,   548,
     147,    76,   934,   365,    79,   172,    81,    20,  1158,   116,
    1209,   543,  1211,  1163,   174,    90,   174,   865,   147,    27,
      90,    34,   436,   437,   166,   174,   115,   174,   442,   109,
     172,   445,   446,   588,   109,   880,   398,   166,   400,   401,
     402,   403,   404,   405,  1152,   407,   408,   409,   410,   411,
      63,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,  1052,  1053,
     918,    79,  1158,   147,   436,   437,   484,  1163,   147,    61,
     442,   147,    90,   445,   446,   447,   448,   449,   450,  1039,
     103,  1101,   166,   468,  1006,   457,   109,   298,   299,   174,
     462,   109,   147,   141,   466,   174,   126,   147,   174,    61,
      92,   149,     8,   475,   147,  1223,  1224,  1225,  1226,  1227,
    1228,   921,   484,   105,   486,   487,   166,   173,    11,   174,
     492,   493,   152,   147,   154,   155,   498,   119,   158,   159,
      92,   174,    56,   505,   867,   559,   508,   561,   147,   511,
     365,   147,    61,   105,    83,   878,   173,    86,   147,   147,
     174,   523,    56,    56,     2,   527,   889,   119,   530,   531,
    1039,   893,    56,   548,   536,   174,  1045,   893,   174,   147,
       8,   543,    32,    92,   166,   174,   174,    25,   147,   171,
      28,   403,   404,   147,   556,    56,   105,   559,   166,   561,
     732,    56,   173,   565,   566,   567,    32,   569,   570,  1121,
     119,  1123,   166,    89,   166,   174,   147,    93,   100,   171,
     582,   583,   754,   147,   765,   766,    56,   147,   769,   770,
     147,    56,  1101,   575,   147,   166,   598,   449,   450,    89,
      78,   147,   166,    93,    82,    83,   166,   147,    89,   166,
     462,   173,    93,   166,    89,    56,    51,   166,    93,    54,
     166,     8,   171,    89,    56,   166,   166,    93,   106,   484,
      56,    56,   173,   111,   175,   487,    89,    56,  1078,    74,
      93,   493,    77,   824,   825,   173,   498,   175,   146,    84,
      89,   141,   142,  1162,    93,  1018,   146,   171,   148,   149,
     173,   663,   175,   153,   141,   837,   838,   173,   173,   146,
     175,   161,   149,   166,   109,   141,   153,   167,   168,   169,
     146,   536,   148,   149,   173,   165,   175,   153,   171,   173,
     173,   175,   166,   865,   173,   161,   175,   173,   752,   175,
     559,   556,   561,   169,   559,    48,   561,  1216,   166,   157,
     863,   566,   166,   565,   166,   166,   166,    56,    61,   166,
     166,   120,    85,   166,   173,   727,   728,   729,   730,   731,
     732,   733,   734,   171,   893,   213,   173,   171,   166,     8,
    1249,   745,   746,   797,   173,   793,   918,    66,   171,    92,
     752,   173,   754,   755,   172,   757,   173,   173,     8,   761,
    1269,   763,     8,    90,   126,    90,   126,  1130,   149,    69,
     772,   773,   173,   126,   174,   147,   119,    48,   173,   126,
     126,    35,    35,   126,    66,    32,   174,   946,  1297,   791,
     792,   793,   126,    21,   953,   797,    22,   174,   276,   174,
      26,    27,   176,   281,   147,    31,   176,   171,   174,   147,
      36,   147,    38,    39,   105,   173,   149,   149,    44,   149,
     141,   149,   166,   166,   141,    51,   149,   149,   171,   149,
     141,   174,   141,   835,   149,   883,   149,   839,   141,   841,
     842,   843,    89,   845,   149,   847,    93,   149,   850,   851,
     149,    77,   166,    79,    32,    81,     2,    83,   166,   337,
      86,   173,    88,   865,    90,   166,  1102,  1103,   166,   166,
     174,    35,    35,   147,   352,   114,   728,   172,   730,    25,
     176,   883,    28,   109,   174,   166,   112,   171,   114,   172,
     116,   146,    33,    33,   141,   142,   174,   752,   145,   146,
     176,   148,   149,   176,    35,   166,   153,   909,   166,   176,
     912,    89,   146,   391,   161,    93,   918,   173,     8,   921,
     167,   168,   169,   166,   173,   176,     8,   141,   176,   174,
    1102,  1103,    78,   935,   936,   176,    82,    83,   793,   141,
     166,   989,    35,   174,   166,   947,   172,   949,   166,   174,
     174,   166,   954,    27,   165,   957,   165,   147,   352,   835,
     106,    24,   272,   141,  1028,   111,  1028,   969,   146,    91,
     148,   149,   975,   322,    48,   153,   993,   527,    52,   874,
     873,   363,   460,   161,   889,   893,   878,   989,   608,   167,
     168,   169,   377,  1163,  1229,  1222,   998,   888,  1000,   862,
     674,  1003,    76,   133,  1006,    79,   524,    81,   486,   787,
      -1,  1013,    -1,  1015,   492,    -1,    90,    -1,    -1,  1178,
    1179,    -1,    -1,  1182,  1183,   503,    -1,   505,   883,    -1,
      -1,    -1,    -1,    -1,    -1,   109,  1195,    -1,  1197,    -1,
    1042,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1057,  1058,    -1,  1060,  1061,
      -1,  1063,  1064,    -1,    -1,    -1,    -1,   213,    -1,    -1,
      -1,  1073,  1074,    -1,  1076,    -1,  1078,    -1,  1080,  1081,
      -1,    -1,  1084,   935,   936,   563,    -1,    -1,    -1,   567,
      -1,    -1,    -1,    -1,    -1,  1143,    -1,   575,  1100,  1101,
    1102,  1103,    -1,    -1,    -1,    -1,  1108,    -1,  1110,    -1,
      48,    -1,  1114,    -1,    -1,    -1,    -1,     2,  1120,  1121,
    1122,  1123,  1124,    61,    -1,    -1,    27,    -1,    -1,    -1,
     276,    -1,    48,    -1,   989,   281,    -1,    -1,    -1,    -1,
      25,  1143,    -1,    28,    -1,    61,  1148,    48,    -1,    -1,
      -1,    52,    -1,    -1,    92,    -1,    -1,    -1,    -1,     2,
    1162,    -1,    -1,    -1,    -1,  1167,  1168,  1169,  1170,  1171,
    1172,  1173,  1174,    -1,    -1,    76,    92,    -1,    79,    -1,
      81,   119,    25,    -1,  1039,    28,    -1,    -1,   126,    90,
      -1,   337,    -1,    78,    -1,    -1,    -1,    82,    83,    -1,
      -1,    -1,    -1,   119,  1206,    -1,   352,    -1,   109,   147,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1221,
      -1,   106,    -1,    -1,    -1,    -1,   111,    -1,   166,    -1,
      -1,   147,    -1,   171,    -1,    78,   174,    -1,    -1,    82,
      83,    -1,  1244,  1245,  1246,  1247,    -1,  1249,    -1,    -1,
     166,  1253,  1254,    -1,    -1,   171,    -1,    -1,   174,    -1,
      -1,    -1,    -1,   106,   742,    -1,  1268,  1269,   111,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1279,  1280,  1281,
    1282,  1283,  1284,    -1,    -1,    -1,    -1,    -1,  1143,    -1,
      -1,    -1,    -1,  1148,    -1,  1297,    -1,   775,    -1,    -1,
     778,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,   460,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   213,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
     486,    -1,    31,    -1,    -1,   823,   492,    36,    -1,    38,
      39,   829,    -1,    -1,    -1,    44,    -1,   503,    -1,   505,
      -1,  1216,    51,    -1,    -1,    54,    -1,    -1,    -1,    -1,
     213,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       2,    -1,    -1,    -1,    -1,    74,    -1,    -1,    77,    -1,
      79,   276,    81,    -1,    83,    84,   281,    86,    -1,    88,
      -1,    90,    -1,    25,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1269,    -1,    -1,   563,    -1,    -1,
     109,   567,    -1,   112,    -1,   114,    -1,   116,    -1,   575,
      -1,     3,    -1,   276,    -1,    -1,    -1,    -1,   281,    -1,
      -1,    -1,  1297,    -1,    -1,    17,    -1,    -1,    20,    -1,
      -1,    -1,   337,    25,    -1,    -1,    78,   935,   936,    -1,
      82,    83,    -1,    -1,    36,    -1,    -1,   352,    -1,   947,
      -1,   949,    -1,    -1,    46,    -1,   954,   166,    -1,   957,
      -1,    53,    -1,   172,   106,    -1,     3,    -1,   966,   111,
     968,    63,    -1,   971,   337,    -1,    -1,    -1,    -1,    71,
      17,    18,    -1,    20,    -1,    72,    -1,    -1,    25,   352,
     988,    -1,    -1,    -1,    -1,    87,    -1,    34,    -1,    -1,
     998,    -1,  1000,    40,    -1,    -1,    -1,    99,    -1,    46,
      -1,   103,    -1,    50,    -1,    -1,    53,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,   117,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,   460,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,   213,    -1,   110,   111,  1073,   113,    -1,    -1,    -1,
     117,   486,  1080,    -1,    -1,    -1,    -1,   492,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1093,    -1,   460,   503,    -1,
     505,    -1,    -1,    -1,    -1,    -1,  1104,    -1,    -1,   775,
      -1,    -1,   778,    -1,    -1,  1113,    -1,    -1,    -1,  1117,
      -1,    -1,    -1,   486,    -1,    -1,    -1,    -1,    -1,   492,
      -1,    -1,    -1,    -1,   276,    -1,   173,    -1,    -1,   281,
     503,    -1,   505,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   823,   563,    -1,
      -1,     2,   567,   829,    -1,    -1,    -1,    -1,    -1,    -1,
     575,    -1,    -1,   260,   261,   262,   263,   264,   265,    -1,
      -1,    -1,    -1,    -1,    25,    -1,  1184,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   337,    -1,   284,    -1,   286,
     563,    -1,    -1,    -1,   567,    -1,   293,    -1,    -1,    22,
     352,    -1,   575,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,   311,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    78,    51,   326,
     327,    82,    83,    -1,    -1,    -1,  1244,  1245,  1246,  1247,
      -1,    -1,    -1,    -1,    -1,  1253,  1254,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,   106,    79,    -1,    -1,    -1,
     111,     2,    -1,    -1,    -1,    88,    -1,    90,    -1,    -1,
      -1,   947,    -1,   949,    -1,    -1,    -1,    -1,   954,    -1,
      -1,   957,    -1,    -1,    25,    -1,   109,    28,    -1,   112,
     966,    -1,   968,   116,    -1,   971,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   460,    -1,
      -1,    -1,   988,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   998,    -1,  1000,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   486,    -1,    -1,    78,    -1,    -1,
     492,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   503,    -1,   505,    -1,    -1,    -1,    -1,    -1,    -1,
     775,    -1,   213,   778,    -1,   106,   463,    -1,    -1,    -1,
     111,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   491,    -1,    -1,  1073,   495,    -1,
     497,    -1,   775,   500,  1080,   778,    -1,    -1,   823,    -1,
      -1,   563,    -1,    -1,   829,   567,    -1,  1093,    -1,    -1,
      -1,    -1,    -1,   575,    -1,   276,    -1,    -1,  1104,    -1,
     281,    -1,    -1,    -1,    -1,   141,   142,  1113,   144,   145,
     146,  1117,   148,   149,    -1,    -1,    -1,   153,    -1,    -1,
     823,    -1,     3,    -1,   160,   161,   829,    -1,   164,    -1,
      -1,   167,   168,   169,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,   213,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,   337,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,   352,    53,    -1,    -1,    -1,    -1,    -1,  1184,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,   947,    -1,   949,   276,    87,    -1,    -1,   954,
     281,    -1,   957,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,   966,   103,   968,    -1,   106,   971,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,  1244,  1245,
    1246,  1247,    -1,   988,   947,    -1,   949,  1253,  1254,    -1,
      -1,   954,    -1,   998,   957,  1000,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   966,    -1,   968,   337,    -1,   971,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   460,
      -1,   352,    -1,    -1,    -1,   988,    -1,    -1,    -1,    -1,
     171,    -1,   173,   775,    -1,   998,   778,  1000,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   486,    -1,    -1,    -1,    -1,
      -1,   492,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   503,    -1,   505,    -1,    -1,    -1,  1073,    -1,
      -1,    -1,   759,   760,    -1,  1080,    -1,    -1,   765,   766,
      -1,   823,   769,   770,    -1,    -1,    -1,   829,  1093,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1104,
      -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,  1113,    -1,
    1073,    89,  1117,    -1,    -1,    93,    -1,  1080,    -1,    -1,
      -1,     2,   563,    -1,    -1,    -1,   567,    -1,    25,   460,
    1093,    28,    -1,    -1,   575,    -1,    -1,    -1,    -1,    -1,
      -1,  1104,    -1,    -1,    25,    -1,    -1,    28,    -1,    -1,
    1113,    -1,    -1,    -1,  1117,   486,    -1,    -1,    -1,    -1,
      -1,   492,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   503,    -1,   505,   153,    -1,    -1,    -1,  1184,
      -1,    78,    -1,   161,    -1,    82,    83,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   947,    -1,   949,    -1,   106,
      -1,    -1,   954,    -1,   111,   957,    -1,    -1,    -1,    -1,
      -1,  1184,    -1,    -1,   966,   106,   968,    -1,    -1,   971,
     111,    -1,   563,    -1,    -1,    -1,   567,    -1,    -1,  1244,
    1245,  1246,  1247,    -1,   575,    -1,   988,    -1,  1253,  1254,
     937,   938,    -1,    -1,    -1,    -1,   998,   944,  1000,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   956,
      -1,    -1,   959,    -1,   961,    -1,   963,    -1,    -1,    -1,
       3,  1244,  1245,  1246,  1247,    -1,    -1,    -1,    -1,    -1,
    1253,  1254,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      32,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,   213,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,  1073,   213,    -1,   775,    -1,    -1,   778,  1080,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,  1093,    -1,    -1,    -1,    78,    -1,    89,    -1,    -1,
      -1,    93,  1104,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,  1113,    -1,    96,    97,  1117,    99,    -1,   101,   276,
     103,    -1,   823,   106,   281,    -1,    -1,   110,   829,    -1,
     113,    -1,    -1,   125,   117,   276,    -1,    -1,    -1,    -1,
     281,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,  1106,
     152,   153,   154,   155,    -1,    -1,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,    -1,
     337,    -1,  1184,    -1,   775,    -1,   178,   778,    -1,    -1,
     173,    -1,    -1,    -1,    -1,   352,   337,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,   352,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   823,    -1,    -1,    -1,    25,    -1,   829,    28,
      -1,    -1,  1244,  1245,  1246,  1247,   947,    -1,   949,    -1,
      -1,  1253,  1254,   954,   141,   142,   957,   144,   145,   146,
      -1,   148,   149,   150,    -1,   966,   153,   968,    -1,    -1,
     971,    -1,    -1,   160,   161,    -1,    -1,   164,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,   988,    -1,    78,
      -1,   178,    -1,    -1,    -1,    -1,    -1,   998,    -1,  1000,
      -1,    -1,    -1,   460,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   106,    -1,   460,
      -1,    -1,   111,    -1,    -1,    -1,    -1,    -1,    -1,   486,
      -1,    -1,    -1,    -1,    -1,   492,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   486,   503,    -1,   505,    -1,
      -1,   492,    -1,    -1,    -1,    -1,   947,    -1,   949,    -1,
      -1,    -1,   503,   954,   505,    -1,   957,    -1,    -1,    -1,
      -1,    -1,  1073,    -1,    -1,   966,    -1,   968,    -1,  1080,
     971,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1093,    -1,    -1,    -1,    -1,   988,    -1,    -1,
      -1,    -1,    -1,  1104,    -1,    -1,   563,   998,    -1,  1000,
     567,    -1,  1113,    -1,    -1,    -1,  1117,    -1,   575,    -1,
      -1,    -1,   563,    -1,   213,    -1,   567,    -1,    -1,    -1,
      -1,    -1,    49,    -1,   575,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    22,    82,    83,    -1,    26,    27,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,
      38,    39,  1073,  1184,    -1,    -1,    44,   276,    -1,  1080,
      -1,    -1,   281,    51,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1093,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1104,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,  1113,    81,    -1,    83,  1117,    -1,    86,    -1,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1244,  1245,  1246,  1247,    -1,   337,    -1,
      -1,   109,  1253,  1254,   112,    -1,   114,    -1,   116,    -1,
      -1,    -1,    -1,   352,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    44,    45,    46,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,  1184,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   218,    -1,    -1,    -1,    -1,    -1,    -1,   166,    12,
      -1,    -1,    -1,    -1,   172,    -1,    19,    -1,   775,    -1,
      -1,   778,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,   775,    -1,    -1,   778,    -1,    -1,
      -1,    -1,    -1,   260,   261,   262,   263,   264,   265,    -1,
      -1,   268,   269,  1244,  1245,  1246,  1247,    -1,   275,    -1,
      -1,    -1,  1253,  1254,    -1,    -1,   823,   284,    -1,   286,
      -1,   460,   829,    -1,    -1,    -1,   293,    -1,    -1,    -1,
      -1,    -1,   823,    -1,    -1,    -1,    89,    -1,   829,   151,
      93,    -1,    -1,    -1,   311,    -1,    -1,   486,    -1,   102,
      -1,   163,   164,   492,    -1,    -1,    -1,    -1,    -1,   326,
     327,    -1,    -1,    -1,   503,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,   197,   139,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,
      -1,    -1,    32,    -1,   563,   178,    -1,    -1,   567,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   575,    -1,    -1,    -1,
     947,    -1,   949,    -1,    -1,    -1,   258,   954,    -1,    -1,
     957,    -1,    -1,    -1,   266,   267,   947,    -1,   949,   966,
      -1,   968,    -1,   954,   971,   277,   957,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   966,    -1,   968,    -1,    89,
     971,   988,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   998,    -1,  1000,    -1,    -1,   463,   988,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   998,    -1,  1000,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   491,    -1,    -1,    -1,   495,    -1,
     497,   141,   142,   500,   144,   145,   146,    -1,   148,   149,
     150,    -1,    -1,   153,   154,   155,    -1,    -1,   158,   159,
     160,   161,   364,    -1,   164,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,  1073,    -1,   178,    -1,
      -1,    -1,    -1,  1080,    -1,    -1,    -1,    -1,     3,   391,
      -1,    -1,  1073,    -1,    -1,    -1,  1093,    -1,    -1,  1080,
      -1,    -1,    17,    18,    -1,    20,    -1,  1104,    -1,    -1,
      25,    -1,  1093,    -1,    -1,    -1,  1113,    -1,    -1,    34,
    1117,    -1,    -1,  1104,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,  1113,    -1,    -1,    50,  1117,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   775,    -1,    63,   778,
      -1,    -1,    -1,    -1,   456,    70,    71,    -1,    -1,    -1,
      -1,    -1,   464,    78,    -1,    -1,    -1,    -1,   470,    -1,
      -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,  1184,   103,    -1,
      -1,   106,    -1,    -1,   823,   110,    -1,    -1,   113,    -1,
     829,    -1,   117,  1184,    -1,    -1,    -1,    -1,    -1,    -1,
     512,    -1,    -1,    -1,   516,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   524,    -1,    -1,    -1,    -1,   529,    -1,    -1,
      -1,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,    -1,  1244,  1245,  1246,
    1247,    -1,    -1,    -1,    -1,    -1,  1253,  1254,   173,    -1,
      -1,    -1,   157,  1244,  1245,  1246,  1247,    -1,    -1,    -1,
      -1,    -1,  1253,  1254,    -1,   170,    -1,    -1,   173,   581,
     175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   759,   760,    -1,    -1,    -1,    -1,   765,   766,
     612,    -1,   769,   770,    -1,    -1,    -1,    -1,   947,    -1,
     949,    -1,   779,    -1,    -1,   954,    -1,    -1,   957,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   966,    -1,   968,
      -1,    -1,   971,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   813,    -1,    -1,   988,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   998,
      -1,  1000,   674,    -1,     1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,  1093,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,  1104,   103,    -1,   780,   106,
     937,   938,    -1,   110,  1113,   787,   113,   944,  1117,   946,
     117,    -1,    -1,    -1,    -1,    -1,   953,    -1,   125,   956,
      -1,   803,   959,    -1,   961,    -1,   963,    -1,    -1,    -1,
      -1,   813,    -1,    -1,   141,   142,   143,   144,   145,   976,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   996,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,   176,
      -1,   178,    -1,    -1,    -1,  1184,    -1,    -1,    -1,    -1,
     862,   863,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,   877,    17,    18,   880,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,   893,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,  1106,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,   975,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   993,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,  1178,  1179,   164,   165,  1182,  1183,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,  1193,    -1,  1195,    -1,
    1197,     0,     1,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    12,
      -1,    14,    15,    16,    17,    18,    19,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,   171,   172,
     173,    -1,   175,    -1,     1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,    -1,   171,   172,   173,    -1,   175,    -1,
       1,   178,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,
     171,   172,   173,    -1,   175,    -1,     1,   178,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,   172,   173,    -1,
     175,    -1,     1,   178,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
     169,    -1,   171,   172,   173,    -1,   175,    -1,     1,   178,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,   172,
     173,    -1,   175,    -1,     1,   178,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       1,   178,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     1,   178,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    79,    80,    -1,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     1,   178,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,     1,   178,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       1,   178,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,   117,    -1,    -1,   120,
      -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     1,   178,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    68,    -1,    70,    71,    72,    -1,    74,
      -1,    -1,    77,    78,    79,    80,    -1,    82,    83,    84,
      85,    86,    87,    88,    -1,    90,    91,    92,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,    -1,   103,   104,
      -1,   106,   107,   108,   109,   110,    -1,   112,   113,   114,
     115,   116,   117,    -1,    -1,   120,    -1,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     1,   178,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,   117,    -1,
      -1,   120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,   117,    -1,    -1,   120,    -1,   122,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,     1,   178,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
     117,    -1,    -1,   120,    -1,   122,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     3,   178,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,
      17,    18,    19,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,   142,   143,   144,   145,   146,
      -1,   148,    -1,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
     167,   168,   169,   170,   171,    -1,   173,    -1,   175,    -1,
       3,   178,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    -1,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,
      33,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    62,
      63,    64,    65,    -1,    67,    68,    -1,    70,    71,    72,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,   120,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
     107,   108,    -1,   110,   111,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       3,   178,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
     107,   108,    -1,   110,   111,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,   174,   175,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
     111,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,   174,   175,    -1,     3,   178,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       3,   178,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
     107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       3,   178,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    69,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,     3,   178,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      79,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     3,   178,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     3,   178,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,   123,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,   172,   173,    -1,   175,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,   176,     3,   178,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    93,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,   176,     3,   178,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     3,   178,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     3,   178,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,   112,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     3,   178,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,   176,     3,   178,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,   111,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     3,   178,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,   111,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     3,   178,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     3,   178,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       3,   178,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,     3,   178,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     3,   178,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     3,   178,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     3,   178,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       3,   178,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,     3,   178,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     3,   178,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     3,   178,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     3,   178,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       3,   178,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    12,    -1,    -1,    -1,   150,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    34,   169,    36,    -1,    -1,
     173,    40,   175,    -1,    -1,   178,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,   141,   142,   143,   144,   145,   146,    -1,   148,
      -1,    -1,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,   164,    -1,    -1,   167,   168,
     169,   170,    -1,     3,   173,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    61,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,   105,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   143,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,   165,   166,    25,    -1,    -1,
      -1,   171,    -1,   173,    -1,   175,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,   147,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,    -1,    -1,
      -1,    -1,    -1,   171,    -1,   173,   174,   175,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,   143,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
     165,    -1,    25,    -1,    -1,    -1,   171,    -1,   173,    -1,
     175,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     143,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,   165,    -1,    25,    -1,    -1,    -1,   171,    -1,
     173,    -1,   175,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,     3,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    12,    -1,    -1,    -1,    87,    88,    -1,    19,
      20,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    34,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   173,   103,   175,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,   148,    -1,
      -1,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     3,   164,    -1,    -1,   167,   168,   169,
     170,   171,    12,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    96,    97,    -1,    99,
      -1,   101,    19,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    32,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,    -1,   148,    -1,
      -1,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,    -1,    -1,   167,   168,   169,
     170,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   102,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    89,    -1,    -1,    -1,
      93,   178,    -1,    12,    -1,    -1,    -1,    -1,    -1,   102,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,
      89,    -1,    -1,    -1,    93,   178,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   102,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    -1,    -1,   134,   135,   136,   137,    -1,
     139,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,   171,    -1,    89,    -1,    -1,    -1,    93,   178,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   102,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,    -1,    -1,   134,
     135,   136,   137,    -1,   139,   140,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,   171,    -1,    89,    -1,
      -1,    -1,    93,   178,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   102,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,    -1,    -1,   134,   135,   136,   137,    -1,   139,   140,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
     171,    -1,    89,    -1,    -1,    -1,    93,   178,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   102,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,    -1,    -1,   134,   135,   136,
     137,    -1,   139,   140,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    89,    -1,    -1,    -1,
      93,   178,    -1,    12,    -1,    -1,    -1,    -1,    -1,   102,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,
      89,    -1,    -1,    -1,    93,   178,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   102,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,    48,    -1,   134,   135,   136,   137,    -1,
     139,   140,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,   171,    -1,    89,    -1,    -1,    -1,    93,   178,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,   171,    89,    -1,    -1,
      -1,    93,    -1,   178,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,   121,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   171,
      -1,    89,    -1,    -1,    -1,    93,   178,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    37,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,    73,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    -1,    89,    -1,    -1,    -1,    93,
     178,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,    73,   158,   159,   160,   161,   162,   163,
     164,    -1,   166,   167,   168,   169,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,   178,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,   178,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    89,    -1,    -1,    -1,
      93,   178,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   171,    89,
      -1,    -1,    -1,    93,    12,   178,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,    89,    -1,    -1,    -1,    93,    -1,    12,   178,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    89,    -1,    -1,    -1,    93,    12,
     178,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    89,    -1,    -1,    -1,
      93,    12,    -1,   178,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    89,    -1,
      -1,    -1,    93,    -1,    12,   178,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
     171,    89,    -1,    -1,    -1,    93,    -1,   178,    12,    -1,
      -1,    -1,    -1,    -1,   102,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    -1,    89,    -1,    -1,    -1,    93,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,
      32,   125,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,    73,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,   178,    -1,    -1,    89,    -1,    -1,
      -1,    93,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,    89,
      -1,    -1,    -1,    93,    12,    -1,   178,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,    89,    -1,    -1,    -1,    93,    -1,    12,   178,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    89,    -1,    -1,    -1,    93,    12,
     178,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
      -1,   166,   167,   168,   169,    -1,    89,    -1,    -1,    -1,
      93,    12,    -1,   178,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    89,    -1,
      -1,    -1,    93,    12,    -1,   178,    -1,    -1,    -1,    -1,
      19,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
      89,    -1,    -1,    -1,    93,    12,    13,   178,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,    89,    -1,    -1,   174,    93,    12,    -1,   178,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,    89,    -1,    -1,    -1,    93,    12,
      -1,   178,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    89,    -1,    -1,    -1,
      93,    12,    -1,   178,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    89,    -1,
      -1,    -1,    93,    12,    -1,   178,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,    -1,   166,   167,   168,   169,    -1,
      89,    -1,    -1,    -1,    93,    12,    -1,   178,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,    89,    -1,    -1,    -1,    93,    12,    -1,   178,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,    89,    -1,    -1,   174,    93,    12,
      -1,   178,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    89,    -1,    -1,   174,
      93,    12,    13,   178,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    89,    -1,
      -1,    -1,    93,    12,    -1,   178,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
      89,    -1,    -1,    -1,    93,    12,    -1,   178,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,    -1,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,    89,    -1,    -1,   174,    93,    12,    -1,   178,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,    89,    -1,    -1,    -1,    93,    12,
      -1,   178,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,    -1,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    89,    -1,    -1,    -1,
      93,    12,    13,   178,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,    -1,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    89,    -1,
      -1,    -1,    93,   176,    12,   178,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
      -1,    89,    -1,    -1,    -1,    93,    12,   178,    -1,    -1,
      -1,    -1,    -1,    19,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    89,    -1,    -1,    -1,    93,    12,    13,
     178,    -1,    -1,    -1,    -1,    19,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    89,    -1,    -1,    -1,    93,
      12,    -1,   178,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    61,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,    89,    -1,    -1,
      -1,    93,    12,    -1,   178,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,    89,
      -1,    -1,    -1,    93,    12,    -1,   178,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,    89,    -1,    -1,    -1,    93,   176,    -1,   178,    -1,
      12,    -1,    -1,    -1,   102,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
     178,    93,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,    89,
      -1,    -1,    -1,    93,    12,    -1,   178,    -1,    -1,    -1,
      -1,    19,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,    89,    -1,    -1,    -1,    93,    12,    -1,   178,    -1,
      -1,    -1,    -1,    19,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    89,    -1,    -1,    -1,    93,    12,    -1,
     178,    -1,    -1,    -1,    -1,    19,   102,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,    -1,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    89,    -1,    -1,    -1,    93,
      12,    -1,   178,    -1,    -1,    -1,    -1,    19,   102,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,    -1,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,    89,    -1,    -1,
      -1,    93,    12,    -1,   178,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,    89,
      -1,    -1,    -1,    93,   176,    12,   178,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,    -1,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,   125,    -1,
      31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
      -1,    -1,    -1,    44,   141,   142,    -1,   144,   145,   146,
      51,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    89,
     167,   168,   169,    93,    -1,    -1,    77,    -1,    79,    -1,
      81,   178,    83,    -1,    -1,    86,    -1,    88,    -1,    90,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,   109,    -1,
      -1,   112,    -1,   114,    -1,   116,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,    -1,    -1,   158,   159,
     160,   161,   162,    -1,   164,    -1,    -1,   167,   168,   169,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,
      -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,   141,   142,    -1,   144,   145,   146,    51,   148,
     149,   150,    -1,   152,   153,   154,   155,    -1,    -1,   158,
     159,   160,   161,   162,    -1,   164,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,   178,
      83,    -1,    -1,    86,    -1,    88,    22,    90,    -1,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,   109,    -1,    44,   112,
      -1,   114,    -1,   116,    -1,    51,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      66,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,    77,    -1,    79,    -1,    -1,    51,    83,    -1,    85,
      86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,   109,    79,    -1,   112,    -1,    83,    -1,
     116,    86,     1,    88,     3,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,   109,    -1,    25,   112,    -1,    -1,
      -1,   116,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
     166,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,   166,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    17,
      18,   110,    20,    -1,   113,   114,    -1,    25,   117,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,     3,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,   117,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    17,    18,   106,
      20,    -1,    -1,   110,    -1,    25,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,
      -1,   101,    -1,   103,    17,    18,   106,    20,    -1,    -1,
     110,    -1,    25,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,    17,    18,   106,    20,    -1,    -1,   110,    -1,    25,
     113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,   117
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   183,   184,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    33,    34,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      49,    50,    51,    53,    56,    57,    58,    59,    60,    62,
      63,    64,    65,    67,    68,    70,    71,    72,    77,    78,
      79,    80,    81,    82,    83,    86,    87,    88,    90,    91,
      92,    94,    95,    96,    97,    98,    99,   100,   101,   103,
     104,   106,   107,   108,   109,   110,   112,   113,   114,   116,
     117,   120,   122,   125,   141,   142,   143,   144,   145,   150,
     160,   163,   164,   165,   166,   169,   171,   173,   175,   178,
     185,   186,   187,   188,   189,   192,   193,   196,   197,   198,
     200,   201,   202,   207,   208,   211,   212,   216,   218,   221,
     228,   231,   232,   233,   234,   235,   236,   239,   240,   242,
     244,   247,   248,   249,   250,   251,   255,   256,   261,   262,
     263,   264,   267,   268,   279,   280,   282,   283,   284,   285,
     287,   288,   313,   314,   318,   319,   340,   341,   342,   343,
     344,   345,   346,   354,   355,   356,   357,   358,   361,   362,
     363,   364,   365,   366,   367,   368,   370,   371,   372,   373,
     374,   166,    22,    36,    41,    42,    45,    56,    88,   101,
     171,   175,   247,   280,   340,   345,   356,   357,   358,   361,
     363,   364,   121,   347,   348,     3,   213,   358,   347,   358,
     109,   320,    90,   213,   187,   334,   358,     8,   191,   187,
     173,     3,    17,    18,    20,    25,    34,    40,    46,    50,
      53,    63,    70,    71,    78,    87,    96,    97,    99,   101,
     103,   106,   110,   113,   117,   215,   217,    11,    79,   123,
     241,   358,   241,   358,   241,   358,    27,   116,   243,   358,
      83,    86,   198,   173,   215,   215,   215,   173,   215,   321,
     322,    33,   202,   220,   358,   259,   260,   358,   358,    18,
      78,    96,   113,   358,   358,   358,     8,   173,   230,   229,
       4,   308,   333,   358,   107,   108,   166,   358,   360,   358,
     220,   358,   358,   358,   100,   173,   187,   358,   358,   188,
     202,   188,   202,   358,     8,   195,   243,   358,   360,   358,
     358,   358,   358,   358,     1,   172,   185,   203,   333,   111,
     151,   308,   335,   336,   360,   241,   333,   358,   369,   358,
      81,   187,   171,    47,    85,   115,   199,    26,   319,   358,
       8,   263,   358,   359,    56,   146,   265,   215,     1,    31,
     114,   215,   269,   272,   275,   278,   173,   296,    54,    74,
      84,   303,    27,    79,    90,   109,   304,    27,    79,    90,
     109,   302,   215,   315,   316,   321,   165,   166,   157,   358,
      12,    19,    32,    89,    93,   125,   141,   142,   144,   145,
     146,   148,   149,   150,   152,   153,   154,   155,   156,   158,
     159,   160,   161,   162,   163,   164,   167,   168,   169,   178,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   170,   292,   173,   175,    89,
      93,   358,   358,   358,   215,   333,    56,   173,   187,   166,
     171,   202,    48,   347,   318,   166,   147,   166,   190,    22,
      36,    39,    88,   192,   198,   120,   216,   308,   337,   338,
     339,   360,   171,   215,   173,   220,    33,    48,   220,   121,
     220,   350,    33,    48,   220,   350,   220,   350,    48,   220,
     350,   215,   215,   102,   202,   102,   126,   202,   292,   199,
     337,   173,   173,   202,   187,   111,   173,   215,   323,   324,
       1,   146,   328,    48,   147,   187,   220,   147,   220,    13,
     173,   173,   220,   337,   231,   231,   147,   166,   358,   358,
     166,   171,   220,   173,   337,   166,   252,   252,   194,   192,
     220,   220,   166,   172,   172,   185,   147,   172,   358,   147,
     174,   147,   174,   176,   350,    48,   147,   176,   350,   124,
     147,   176,     8,     1,   172,   203,   209,   210,   358,   205,
     358,    66,    37,    73,   166,   263,   265,   109,   237,   285,
     215,   333,   171,   172,     8,   274,     8,   277,   126,   147,
     172,    27,    48,    52,    76,    79,    90,   109,   186,   297,
     298,   299,   300,   301,   304,    90,     1,   146,   327,    90,
       1,     3,    12,    17,    19,    20,    25,    40,    46,    53,
      55,    63,    70,    71,    87,    99,   103,   106,   110,   117,
     141,   142,   143,   144,   145,   146,   148,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   164,   167,
     168,   169,   170,   173,   214,   215,   217,   289,   290,   291,
     292,   340,   126,   317,   147,   166,   166,   358,   358,   358,
     358,   241,   358,   241,   358,   358,   358,   358,   358,   358,
     358,     3,    20,    34,    63,   103,   109,   216,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,    69,   360,   360,   360,   360,
     360,   337,   337,   241,   358,   241,   358,    33,    48,    33,
      48,   102,   173,    48,   176,   215,    27,    48,    90,   116,
     349,   352,   353,   358,   374,   241,   358,   220,   321,   358,
     192,   358,   126,   174,   147,    48,   333,    45,   358,   241,
     358,   173,   220,    45,   358,   241,   358,   220,   220,   241,
     358,   220,   126,   126,   187,    35,   187,   358,    35,   358,
      66,   174,   338,   215,   245,   246,   324,   147,   174,    34,
      50,    97,   101,   175,   219,   329,   341,   126,   325,   358,
     322,   259,   215,   318,   358,   358,   174,   308,   358,     1,
     257,   339,   174,    21,   253,   192,   172,   174,   174,   335,
     174,   335,   187,   176,   241,   358,   176,   187,   358,   176,
     358,   176,   358,   172,   172,   147,   166,    13,   149,   147,
     166,    13,    37,    73,   215,   141,   142,   143,   144,   145,
     160,   164,   169,   204,   291,   292,   293,   358,   204,   206,
     265,   171,   238,   318,   166,   173,     1,   266,   273,   278,
     276,   278,   358,   270,   271,    48,    90,   300,   147,   174,
     173,   215,   328,   175,   219,   326,   341,   105,   305,   173,
     295,   358,   141,   149,   295,   295,   329,   341,   316,   173,
     175,   166,   166,   166,   166,   166,   166,   174,   176,    45,
     241,   358,    45,   241,   358,   338,   241,   358,   173,    48,
      90,   147,   174,    17,    20,    25,    46,    53,    63,    71,
      87,    99,   110,   117,   340,    89,    89,   347,   347,   166,
     166,   308,   360,   339,   358,   174,   358,    33,   220,    33,
     220,   351,   352,   358,    33,   220,   350,    33,   220,   350,
     220,   350,   220,   350,   358,   358,    35,   187,    35,    35,
     187,   102,   202,   215,   174,   147,   174,   174,   323,   328,
       1,   333,    69,   360,   215,   174,   174,   172,    75,   118,
     172,   258,   174,   173,   202,   215,   254,   187,   176,   350,
     176,   350,   187,   124,   209,   216,   171,   293,   358,   111,
     358,   204,   206,   147,   166,    13,   166,   171,   266,   315,
     321,   338,   172,    31,    83,    86,   114,   172,   186,   222,
     225,   228,   278,   278,   272,   275,   215,   299,   324,   146,
     332,    61,   119,   309,     1,   176,   333,   309,   298,   174,
       3,   103,   290,   292,   174,   176,   358,    33,    33,   358,
      33,    33,   174,   176,   176,   338,   352,   328,   340,   340,
     220,   220,   220,   102,    45,   358,    45,   358,   147,   174,
     102,    45,   358,   220,    45,   358,   220,   220,   220,   187,
     187,   358,   187,    35,   166,   166,   246,   202,   325,   176,
      48,   176,   173,   173,    33,   187,   333,   254,   146,   202,
      45,   187,   358,   176,    45,   187,   358,   176,   358,   204,
      13,    37,    73,    37,    73,   166,   166,   293,   358,   358,
     266,   172,   166,   166,   174,     8,   224,     8,   227,   222,
     225,   332,   174,   175,   219,   308,   331,   341,   151,   294,
     325,     3,    92,   103,   310,   311,   312,   358,   281,   176,
     326,   341,   176,   286,   174,   295,   295,   102,    45,    45,
     102,    45,    45,    45,    45,   174,   325,   358,   358,   358,
     352,   358,   358,   358,    35,   187,   358,   329,   341,   337,
     337,   187,   220,   174,   358,   358,   187,   358,   187,   172,
     111,   358,   204,   206,   204,   206,    13,   172,   166,   223,
     228,   226,   228,   294,   325,   332,   176,   333,   308,   358,
     312,   119,   147,   126,   152,   154,   155,   158,   159,    61,
     201,   221,   307,   326,   341,   166,   306,   307,   358,   358,
     358,   358,   358,   358,   102,   102,   102,   102,   187,   176,
     174,   174,   202,   102,   102,   166,   166,   166,   166,   358,
     228,   228,   294,   325,   308,   330,   331,   341,    48,   176,
     358,   311,   312,   312,   312,   312,   312,   312,   310,   102,
     102,   102,   102,   102,   102,   358,   358,   358,   358,   341,
     165,   165,   358,   358,   358,   330,   331,   176,   330
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   182,   183,   184,   184,   185,   185,   186,   186,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   188,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   189,   190,   189,   191,   189,   192,
     192,   194,   193,   195,   193,   196,   197,   197,   197,   198,
     198,   198,   199,   199,   200,   201,   201,   201,   202,   203,
     203,   204,   204,   204,   204,   204,   204,   205,   205,   205,
     205,   205,   205,   206,   206,   207,   207,   207,   207,   207,
     207,   207,   208,   209,   209,   209,   209,   210,   210,   211,
     212,   212,   212,   212,   212,   212,   213,   213,   214,   214,
     214,   214,   214,   214,   215,   215,   215,   215,   215,   215,
     216,   216,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   219,   219,   219,   219,   220,   220,   221,
     221,   222,   223,   222,   224,   222,   226,   225,   227,   225,
     228,   228,   229,   228,   230,   228,   231,   231,   231,   231,
     231,   231,   231,   232,   232,   232,   232,   233,   234,   234,
     235,   236,   236,   236,   237,   236,   238,   236,   239,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   241,   242,   242,   242,   242,   242,
     242,   242,   242,   242,   242,   242,   242,   243,   243,   244,
     244,   245,   245,   246,   247,   247,   247,   247,   247,   247,
     247,   247,   247,   247,   247,   247,   247,   248,   248,   248,
     248,   248,   248,   249,   249,   249,   250,   250,   250,   251,
     251,   251,   251,   252,   252,   253,   253,   253,   254,   254,
     255,   256,   256,   257,   257,   258,   258,   258,   259,   259,
     259,   260,   260,   261,   262,   262,   263,   264,   264,   264,
     265,   265,   266,   266,   266,   266,   266,   267,   267,   268,
     269,   269,   270,   269,   269,   271,   269,   272,   273,   272,
     274,   272,   276,   275,   277,   275,   278,   278,   279,   281,
     280,   282,   283,   283,   283,   284,   286,   285,   287,   287,
     287,   287,   287,   288,   289,   289,   290,   290,   290,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   292,   292,   292,   293,   293,   294,
     294,   294,   295,   295,   296,   297,   297,   298,   298,   299,
     299,   299,   299,   299,   299,   300,   300,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   302,   302,   302,   302,
     302,   302,   303,   303,   303,   304,   304,   304,   304,   304,
     304,   305,   305,   306,   306,   307,   307,   308,   309,   309,
     309,   309,   309,   310,   310,   311,   311,   311,   311,   311,
     311,   311,   312,   312,   313,   314,   314,   315,   315,   316,
     317,   317,   317,   318,   318,   318,   318,   318,   320,   319,
     319,   321,   321,   322,   322,   323,   323,   323,   324,   324,
     324,   325,   325,   325,   326,   326,   326,   326,   326,   326,
     326,   327,   327,   327,   327,   327,   328,   328,   328,   328,
     328,   329,   329,   329,   329,   330,   330,   330,   331,   331,
     331,   331,   331,   332,   332,   332,   332,   332,   333,   333,
     333,   333,   334,   334,   335,   335,   335,   336,   336,   337,
     337,   338,   338,   339,   339,   339,   339,   340,   340,   341,
     341,   341,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   344,   345,   346,   346,   346,   346,   346,   346,   346,
     346,   347,   347,   348,   349,   349,   350,   351,   351,   352,
     352,   352,   353,   353,   353,   353,   353,   353,   354,   354,
     355,   355,   355,   355,   355,   356,   356,   356,   356,   356,
     357,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   359,   359,
     360,   360,   360,   361,   361,   361,   361,   362,   362,   362,
     362,   362,   363,   363,   363,   364,   364,   364,   364,   364,
     364,   365,   365,   365,   365,   366,   366,   367,   367,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   369,   369,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   370,   370,   370,
     370,   370,   370,   370,   370,   370,   370,   371,   371,   371,
     371,   371,   371,   371,   372,   372,   372,   372,   373,   373,
     373,   373,   374,   374,   374,   374,   374,   374,   374
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     3,     3,     3,     3,     2,     1,     1,
       1,     1,     1,     2,     1,     3,     3,     3,     3,     2,
       3,     3,     2,     2,     1,     0,     4,     0,     3,     1,
       1,     0,     4,     0,     3,     4,     3,     4,     4,     0,
       1,     1,     0,     1,     6,     2,     3,     3,     1,     1,
       2,     1,     1,     3,     3,     3,     5,     1,     3,     3,
       3,     5,     5,     0,     1,     4,     6,     8,     8,     6,
       8,     8,     4,     1,     3,     3,     5,     1,     3,     3,
       4,     4,     4,     4,     4,     4,     0,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       3,     1,     0,     4,     0,     3,     0,     4,     0,     3,
       1,     1,     0,     3,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     3,     5,     5,     2,     1,     1,     1,
       1,     6,     7,     3,     0,     6,     0,     6,     2,     5,
       3,     3,     6,     6,     4,     5,     5,     3,     3,     6,
       5,     6,     5,     6,     3,     4,     3,     4,     5,     6,
       5,     6,     3,     4,     3,     4,     6,     7,     6,     7,
       4,     5,     4,     5,     4,     4,     3,     6,     5,     4,
       3,     6,     5,     6,     5,     8,     7,     4,     4,     6,
       3,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     6,     4,     7,     5,     3,     6,     2,
       2,     3,     3,     0,     2,     2,     3,     5,     1,     3,
       3,     5,     5,     0,     2,     3,     2,     3,     4,     3,
       1,     1,     3,     3,     5,     5,     2,     1,     1,     1,
       0,     2,     0,     2,     2,     3,     3,     3,     3,     3,
       1,     2,     0,     4,     1,     0,     4,     1,     0,     4,
       0,     3,     0,     4,     0,     3,     1,     3,     1,     0,
       7,     0,     1,     1,     1,     1,     0,     7,     4,     4,
       6,     6,     4,     2,     1,     3,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     0,     3,     3,     1,     3,     0,     1,     4,
       5,     4,     5,     6,     6,     0,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     1,     0,     1,     2,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     1,     0,     2,
       2,     4,     4,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     3,     1,     2,     1,     3,     2,
       0,     2,     2,     1,     2,     1,     1,     1,     0,     5,
       3,     1,     3,     3,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     3,     2,     4,     3,     3,     4,
       3,     0,     2,     2,     2,     1,     0,     2,     2,     2,
       1,     4,     4,     6,     3,     0,     1,     1,     3,     4,
       3,     4,     6,     0,     2,     2,     2,     2,     1,     1,
       3,     3,     1,     3,     1,     1,     1,     3,     3,     0,
       1,     1,     3,     3,     3,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     2,     4,     4,
       4,     5,     2,     2,     1,     2,     1,     2,     1,     2,
       1,     2,     1,     1,     6,     6,     4,     9,     9,     7,
       6,     6,     4,     9,     9,     7,     4,     6,     6,     9,
       9,     6,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     0,     1,     4,     1,     3,     4,     1,     3,     4,
       3,     3,     1,     1,     2,     1,     2,     1,     1,     3,
       1,     2,     2,     2,     2,     2,     8,     8,     9,     9,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     3,     3,     3,     2,     2,     2,     1,     0,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     4,     4,     4,     3,     3,     3,     3,     5,
       5,     3,     4,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     3,     4,
       3,     4,     3,     5,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYCHPL_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYCHPL_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, context, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYCHPL_error or YYCHPL_UNDEF. */
#define YYERRCODE YYCHPL_UNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYCHPL_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYCHPL_LTYPE_IS_TRIVIAL && YYCHPL_LTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, context); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (context);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, context);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, ParserContext* context)
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), context);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, context); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYCHPL_DEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYCHPL_DEBUG */


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
/* Parser data structure.  */
struct yypstate
  {
    /* Number of syntax errors so far.  */
    int yynerrs;

    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;
    /* Whether this instance has not started parsing yet.
     * If 2, it corresponds to a finished parsing.  */
    int yynew;
  };






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, ParserContext* context)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (context);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}





#define yychpl_nerrs yyps->yychpl_nerrs
#define yystate yyps->yystate
#define yyerrstatus yyps->yyerrstatus
#define yyssa yyps->yyssa
#define yyss yyps->yyss
#define yyssp yyps->yyssp
#define yyvsa yyps->yyvsa
#define yyvs yyps->yyvs
#define yyvsp yyps->yyvsp
#define yylsa yyps->yylsa
#define yyls yyps->yyls
#define yylsp yyps->yylsp
#define yystacksize yyps->yystacksize

/* Initialize the parser data structure.  */
static void
yypstate_clear (yypstate *yyps)
{
  yynerrs = 0;
  yystate = 0;
  yyerrstatus = 0;

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

  /* Initialize the state stack, in case yypcontext_expected_tokens is
     called before the first call to yyparse. */
  *yyssp = 0;
  yyps->yynew = 1;
}

/* Initialize the parser data structure.  */
yypstate *
yypstate_new (void)
{
  yypstate *yyps;
  yyps = YY_CAST (yypstate *, YYMALLOC (sizeof *yyps));
  if (!yyps)
    return YY_NULLPTR;
  yystacksize = YYINITDEPTH;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yypstate_clear (yyps);
  return yyps;
}

void
yypstate_delete (yypstate *yyps)
{
  if (yyps)
    {
#ifndef yyoverflow
      /* If the stack was reallocated but the parse did not complete, then the
         stack still needs to be freed.  */
      if (yyss != yyssa)
        YYSTACK_FREE (yyss);
#endif
      YYFREE (yyps);
    }
}



/*---------------.
| yypush_parse.  |
`---------------*/

int
yypush_parse (yypstate *yyps,
              int yypushed_char, YYSTYPE const *yypushed_val, YYLTYPE *yypushed_loc, ParserContext* context)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYCHPL_LTYPE_IS_TRIVIAL && YYCHPL_LTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  switch (yyps->yynew)
    {
    case 0:
      yyn = yypact[yystate];
      goto yyread_pushed_token;

    case 2:
      yypstate_clear (yyps);
      break;

    default:
      break;
    }

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYCHPL_EMPTY; /* Cause a token to be read.  */

  yylsp[0] = *yypushed_loc;
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
    YYNOMEM;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  if (yychar == YYCHPL_EMPTY)
    {
      if (!yyps->yynew)
        {
          YYDPRINTF ((stderr, "Return for a new token:\n"));
          yyresult = YYPUSH_MORE;
          goto yypushreturn;
        }
      yyps->yynew = 0;
yyread_pushed_token:
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yypushed_char;
      if (yypushed_val)
        yylval = *yypushed_val;
      if (yypushed_loc)
        yylloc = *yypushed_loc;
    }

  if (yychar <= YYCHPL_EOF)
    {
      yychar = YYCHPL_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYCHPL_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYCHPL_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYCHPL_EMPTY;
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: toplevel_stmt_ls  */
#line 613 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6607 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 617 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6613 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 618 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6619 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 625 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6627 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 633 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6635 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 637 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6644 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 645 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6650 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: unstable_decl_stmt  */
#line 646 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6656 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: include_module_stmt  */
#line 647 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6662 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: block_stmt  */
#line 648 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6668 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: use_stmt  */
#line 649 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6674 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: import_stmt  */
#line 650 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6680 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: require_stmt  */
#line 651 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6686 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: extern_block_stmt  */
#line 652 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6692 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 653 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6698 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 654 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6704 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: TDEFER stmt  */
#line 656 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]),
                               false, (yylsp[-1]), (yyvsp[0].commentsAndStmt));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Defer::build(BUILDER, LOC((yyloc)), blockStyle,
                             std::move(stmts));
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6721 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: try_stmt  */
#line 668 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6727 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: return_stmt  */
#line 669 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6733 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 671 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6746 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 680 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6759 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TLABEL ident_def stmt  */
#line 689 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6767 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: TYIELD opt_try_expr TSEMI  */
#line 693 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6778 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: error TSEMI  */
#line 700 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6787 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: assignment_stmt  */
#line 707 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6793 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: if_stmt  */
#line 708 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6799 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: loop_stmt  */
#line 709 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6805 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: select_stmt  */
#line 710 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6811 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: manage_stmt  */
#line 711 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6817 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 712 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6823 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: throw_stmt  */
#line 713 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 715 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    YYLTYPE locBodyAnchor = context->makeLocationAtLast((yylsp[-1]));
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]),
                               false, locBodyAnchor, (yyvsp[0].commentsAndStmt));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Begin::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].withClause)), blockStyle,
                             std::move(stmts));
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6847 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 729 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    YYLTYPE locBodyAnchor = context->makeLocationAtLast((yylsp[-1]));
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]),
                               false, locBodyAnchor, (yyvsp[0].commentsAndStmt));
    assert(blockStyle == BlockStyle::EXPLICIT);
    auto taskBodies = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Cobegin::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].withClause)),
                               std::move(taskBodies));
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6866 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 744 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6878 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 752 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Local::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                             std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6894 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL do_stmt  */
#line 764 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Local::build(BUILDER, LOC((yyloc)), blockStyle, std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TON expr do_stmt  */
#line 775 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = On::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                          std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6925 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 787 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Serial::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)), blockStyle,
                              std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6941 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL do_stmt  */
#line 799 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Serial::build(BUILDER, LOC((yyloc)), blockStyle, std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6956 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSYNC stmt  */
#line 810 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-1]),
                               false, (yylsp[-1]), (yyvsp[0].commentsAndStmt));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Sync::build(BUILDER, LOC((yyloc)), blockStyle,
                            std::move(stmts));
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6973 "bison-chpl-lib.cpp"
    break;

  case 45: /* $@1: %empty  */
#line 827 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6981 "bison-chpl-lib.cpp"
    break;

  case 46: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 decl_base  */
#line 831 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6989 "bison-chpl-lib.cpp"
    break;

  case 47: /* $@2: %empty  */
#line 835 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6997 "bison-chpl-lib.cpp"
    break;

  case 48: /* deprecated_decl_stmt: TDEPRECATED $@2 decl_base  */
#line 839 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 51: /* $@3: %empty  */
#line 851 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 7013 "bison-chpl-lib.cpp"
    break;

  case 52: /* unstable_decl_stmt: TUNSTABLE STRINGLITERAL $@3 decl_base  */
#line 855 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7021 "bison-chpl-lib.cpp"
    break;

  case 53: /* $@4: %empty  */
#line 859 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 7029 "bison-chpl-lib.cpp"
    break;

  case 54: /* unstable_decl_stmt: TUNSTABLE $@4 decl_base  */
#line 863 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7037 "bison-chpl-lib.cpp"
    break;

  case 55: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 870 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));

      ModuleParts parts = {
        .comments=context->gatherComments(loc),
        .attributes=context->buildAttributes((yyloc)).release(),
        .visibility=(yyvsp[-3].visibilityTag),
        .kind=(yyvsp[-2].moduleKind),
        .name=(yyvsp[0].uniqueStr)
      };

      (yyval.moduleParts) = parts;
      context->resetDeclState();
      context->clearComments();
    }
#line 7058 "bison-chpl-lib.cpp"
    break;

  case 56: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 890 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-1]));

      ModuleParts parts = (yyvsp[-2].moduleParts);
      ParserExprList* body = context->makeList();
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-2])), toOwned(parts.attributes),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 7077 "bison-chpl-lib.cpp"
    break;

  case 57: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 905 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-2]));

      ModuleParts parts = (yyvsp[-3].moduleParts);
      ParserExprList* body = (yyvsp[-1].exprList);
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])), toOwned(parts.attributes),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 7096 "bison-chpl-lib.cpp"
    break;

  case 58: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 920 "chpl.ypp"
    {
      ModuleParts parts = (yyvsp[-3].moduleParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      ParserExprList* body = context->makeList(std::move(err));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])), toOwned(parts.attributes),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_access_control: %empty  */
#line 935 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_access_control: TPUBLIC  */
#line 936 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 7126 "bison-chpl-lib.cpp"
    break;

  case 61: /* opt_access_control: TPRIVATE  */
#line 938 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7133 "bison-chpl-lib.cpp"
    break;

  case 62: /* opt_prototype: %empty  */
#line 943 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7139 "bison-chpl-lib.cpp"
    break;

  case 63: /* opt_prototype: TPROTOTYPE  */
#line 944 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7146 "bison-chpl-lib.cpp"
    break;

  case 64: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 950 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7156 "bison-chpl-lib.cpp"
    break;

  case 65: /* block_stmt_body: TLCBR TRCBR  */
#line 968 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7164 "bison-chpl-lib.cpp"
    break;

  case 66: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 972 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7172 "bison-chpl-lib.cpp"
    break;

  case 67: /* block_stmt_body: TLCBR error TRCBR  */
#line 976 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7184 "bison-chpl-lib.cpp"
    break;

  case 68: /* block_stmt: block_stmt_body  */
#line 987 "chpl.ypp"
  {
    // comments from before the opening bracket will have been
    // gathered into stmt_ls when that was parsed, so pull out any
    // comments that occur before the opening bracket.
    // (This would be simpler to handle with midrule actions but those
    //  lead to parser conflicts).
    CommentsAndStmt cs = {0};
    cs.comments = context->gatherCommentsFromList((yyvsp[0].exprList), (yylsp[0]));

    // Create the BlockStmt
    auto blockstmt = Block::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[0].exprList)));
    cs.stmt = blockstmt.release();
    (yyval.commentsAndStmt) = cs;
  }
#line 7203 "bison-chpl-lib.cpp"
    break;

  case 69: /* stmt_ls: toplevel_stmt  */
#line 1005 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7209 "bison-chpl-lib.cpp"
    break;

  case 70: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1006 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7215 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: expr  */
#line 1011 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7223 "bison-chpl-lib.cpp"
    break;

  case 72: /* renames_ls: all_op_name  */
#line 1015 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7231 "bison-chpl-lib.cpp"
    break;

  case 73: /* renames_ls: expr TAS expr  */
#line 1019 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7240 "bison-chpl-lib.cpp"
    break;

  case 74: /* renames_ls: renames_ls TCOMMA expr  */
#line 1024 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7248 "bison-chpl-lib.cpp"
    break;

  case 75: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1028 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7256 "bison-chpl-lib.cpp"
    break;

  case 76: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1032 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7265 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: expr  */
#line 1042 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7274 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_renames_ls: expr TAS expr  */
#line 1047 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7284 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1053 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7295 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1060 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7304 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1065 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7315 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1072 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7327 "bison-chpl-lib.cpp"
    break;

  case 83: /* opt_only_ls: %empty  */
#line 1082 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7333 "bison-chpl-lib.cpp"
    break;

  case 84: /* opt_only_ls: renames_ls  */
#line 1083 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7339 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1088 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7348 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1093 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7360 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1101 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7373 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1110 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7387 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1120 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7399 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1128 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7412 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1137 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7426 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1150 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7435 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_expr: expr  */
#line 1158 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7443 "bison-chpl-lib.cpp"
    break;

  case 94: /* import_expr: expr TDOT all_op_name  */
#line 1162 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7452 "bison-chpl-lib.cpp"
    break;

  case 95: /* import_expr: expr TAS ident_use  */
#line 1167 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7462 "bison-chpl-lib.cpp"
    break;

  case 96: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1173 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7472 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_ls: import_expr  */
#line 1181 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7478 "bison-chpl-lib.cpp"
    break;

  case 98: /* import_ls: import_ls TCOMMA import_expr  */
#line 1182 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7484 "bison-chpl-lib.cpp"
    break;

  case 99: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1187 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7494 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1196 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7502 "bison-chpl-lib.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1200 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1204 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7518 "bison-chpl-lib.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1208 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7526 "bison-chpl-lib.cpp"
    break;

  case 104: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1212 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 105: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1216 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7542 "bison-chpl-lib.cpp"
    break;

  case 106: /* opt_label_ident: %empty  */
#line 1224 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7548 "bison-chpl-lib.cpp"
    break;

  case 107: /* opt_label_ident: TIDENT  */
#line 1225 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7554 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_fn_def: TIDENT  */
#line 1229 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7560 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_fn_def: TNONE  */
#line 1230 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'none'"); }
#line 7566 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_fn_def: TTHIS  */
#line 1231 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7572 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_fn_def: TFALSE  */
#line 1232 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'false'"); }
#line 7578 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_fn_def: TTRUE  */
#line 1233 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'true'"); }
#line 7584 "bison-chpl-lib.cpp"
    break;

  case 113: /* ident_fn_def: internal_type_ident_def  */
#line 1234 "chpl.ypp"
                           { std::string msg = "attempt to redefine reserved type '";
                             msg += (yyvsp[0].uniqueStr).c_str();
                             msg += "'";
                             (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), msg); }
#line 7593 "bison-chpl-lib.cpp"
    break;

  case 114: /* ident_def: TIDENT  */
#line 1240 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7599 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_def: TNONE  */
#line 1241 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'none'"); }
#line 7605 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_def: TTHIS  */
#line 1242 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'this'"); }
#line 7611 "bison-chpl-lib.cpp"
    break;

  case 117: /* ident_def: TFALSE  */
#line 1243 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'false'"); }
#line 7617 "bison-chpl-lib.cpp"
    break;

  case 118: /* ident_def: TTRUE  */
#line 1244 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'true'"); }
#line 7623 "bison-chpl-lib.cpp"
    break;

  case 119: /* ident_def: internal_type_ident_def  */
#line 1245 "chpl.ypp"
                           { std::string msg = "attempt to redefine reserved type '";
                             msg += (yyvsp[0].uniqueStr).c_str();
                             msg += "'";
                             (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), msg); }
#line 7632 "bison-chpl-lib.cpp"
    break;

  case 120: /* ident_use: TIDENT  */
#line 1260 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7638 "bison-chpl-lib.cpp"
    break;

  case 121: /* ident_use: TTHIS  */
#line 1261 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7644 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TBOOL  */
#line 1294 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7650 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TENUM  */
#line 1295 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7656 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TINT  */
#line 1296 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7662 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TUINT  */
#line 1297 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7668 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TREAL  */
#line 1298 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7674 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TIMAG  */
#line 1299 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7680 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TCOMPLEX  */
#line 1300 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7686 "bison-chpl-lib.cpp"
    break;

  case 148: /* scalar_type: TBYTES  */
#line 1301 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7692 "bison-chpl-lib.cpp"
    break;

  case 149: /* scalar_type: TSTRING  */
#line 1302 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7698 "bison-chpl-lib.cpp"
    break;

  case 150: /* scalar_type: TLOCALE  */
#line 1303 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7704 "bison-chpl-lib.cpp"
    break;

  case 151: /* scalar_type: TNOTHING  */
#line 1304 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7710 "bison-chpl-lib.cpp"
    break;

  case 152: /* scalar_type: TVOID  */
#line 1305 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7716 "bison-chpl-lib.cpp"
    break;

  case 157: /* do_stmt: TDO stmt  */
#line 1319 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7722 "bison-chpl-lib.cpp"
    break;

  case 158: /* do_stmt: block_stmt  */
#line 1320 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7728 "bison-chpl-lib.cpp"
    break;

  case 159: /* return_stmt: TRETURN TSEMI  */
#line 1325 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7739 "bison-chpl-lib.cpp"
    break;

  case 160: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1332 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7750 "bison-chpl-lib.cpp"
    break;

  case 162: /* $@5: %empty  */
#line 1343 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 163: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@5 class_level_stmt  */
#line 1347 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7766 "bison-chpl-lib.cpp"
    break;

  case 164: /* $@6: %empty  */
#line 1351 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7774 "bison-chpl-lib.cpp"
    break;

  case 165: /* deprecated_class_level_stmt: TDEPRECATED $@6 class_level_stmt  */
#line 1355 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7782 "bison-chpl-lib.cpp"
    break;

  case 166: /* $@7: %empty  */
#line 1362 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 7790 "bison-chpl-lib.cpp"
    break;

  case 167: /* unstable_class_level_stmt: TUNSTABLE STRINGLITERAL $@7 class_level_stmt  */
#line 1366 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7798 "bison-chpl-lib.cpp"
    break;

  case 168: /* $@8: %empty  */
#line 1370 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 7806 "bison-chpl-lib.cpp"
    break;

  case 169: /* unstable_class_level_stmt: TUNSTABLE $@8 class_level_stmt  */
#line 1374 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7814 "bison-chpl-lib.cpp"
    break;

  case 170: /* class_level_stmt: TSEMI  */
#line 1381 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7822 "bison-chpl-lib.cpp"
    break;

  case 171: /* class_level_stmt: inner_class_level_stmt  */
#line 1385 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7832 "bison-chpl-lib.cpp"
    break;

  case 172: /* $@9: %empty  */
#line 1390 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7839 "bison-chpl-lib.cpp"
    break;

  case 173: /* class_level_stmt: TPUBLIC $@9 inner_class_level_stmt  */
#line 1392 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7848 "bison-chpl-lib.cpp"
    break;

  case 174: /* $@10: %empty  */
#line 1396 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7855 "bison-chpl-lib.cpp"
    break;

  case 175: /* class_level_stmt: TPRIVATE $@10 inner_class_level_stmt  */
#line 1398 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7864 "bison-chpl-lib.cpp"
    break;

  case 183: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1416 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7873 "bison-chpl-lib.cpp"
    break;

  case 184: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1421 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7882 "bison-chpl-lib.cpp"
    break;

  case 185: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1426 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7891 "bison-chpl-lib.cpp"
    break;

  case 186: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1431 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7899 "bison-chpl-lib.cpp"
    break;

  case 187: /* forwarding_decl_start: TFORWARDING  */
#line 1438 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7908 "bison-chpl-lib.cpp"
    break;

  case 188: /* extern_or_export: TEXTERN  */
#line 1445 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7914 "bison-chpl-lib.cpp"
    break;

  case 189: /* extern_or_export: TEXPORT  */
#line 1446 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7920 "bison-chpl-lib.cpp"
    break;

  case 190: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1451 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7930 "bison-chpl-lib.cpp"
    break;

  case 191: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1461 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7939 "bison-chpl-lib.cpp"
    break;

  case 192: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1467 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7950 "bison-chpl-lib.cpp"
    break;

  case 193: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1474 "chpl.ypp"
  {
    auto loc = context->declStartLoc((yylsp[-2]));

    AstNode* optExpr = (yyvsp[-1].expr);
    FunctionParts fp = (yyvsp[0].functionParts);

    auto commentList = context->appendList(context->makeList(),
                                           fp.comments);

    auto comments = context->gatherCommentsFromList(commentList, loc);
    delete commentList;

    fp.linkage = (yyvsp[-2].linkageTag);
    fp.comments = comments;
    fp.linkageNameExpr = optExpr;
    (yyval.commentsAndStmt) = context->buildExternExportFunctionDecl((yyloc), fp);
  }
#line 7972 "bison-chpl-lib.cpp"
    break;

  case 194: /* $@11: %empty  */
#line 1492 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7980 "bison-chpl-lib.cpp"
    break;

  case 195: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1496 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7989 "bison-chpl-lib.cpp"
    break;

  case 196: /* $@12: %empty  */
#line 1501 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7998 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@12 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1506 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 8007 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1514 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 8015 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1521 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-4]),
                               false, (yylsp[-4]), (yyvsp[-3].commentsAndStmt));
    // If the body is not a block, snag any comments before the 'while'.
    if (blockStyle == BlockStyle::IMPLICIT) {
      exprLst = context->appendList(exprLst, context->gatherComments((yylsp[-2])));
    }
    auto body = context->consumeToBlock((yylsp[-4]), exprLst);
    auto node = DoWhile::build(BUILDER, LOC((yyloc)), blockStyle,
                               std::move(body),
                               toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TWHILE expr do_stmt  */
#line 1539 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(body));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 8054 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1552 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(body));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 8071 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1565 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8079 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1569 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8087 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1573 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8095 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1577 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8103 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1581 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8111 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFOR expr do_stmt  */
#line 1585 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8119 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1589 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8127 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1593 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-5]), (yylsp[0]), (yyvsp[0].blockOrDo));
    AstNode* ident = context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr));
    auto index = context->buildLoopIndexDecl((yylsp[-3]), toOwned(ident));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = For::build(BUILDER, LOC((yylsp[-5])), std::move(index),
                           toOwned((yyvsp[-1].expr)),
                           blockStyle,
                           std::move(body),
                           /*isExpressionLevel*/ false,
                           /*isParam*/ true);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 8149 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1611 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8157 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1615 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8165 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1619 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8173 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1623 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8181 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFORALL expr do_stmt  */
#line 1627 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8189 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1631 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8197 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1635 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8205 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1643 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8221 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8229 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8237 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8245 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8253 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8261 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8269 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8277 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8285 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1679 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8293 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1683 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8301 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1687 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8309 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1691 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8317 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1695 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8325 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1699 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8334 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1704 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8343 "bison-chpl-lib.cpp"
    break;

  case 234: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1712 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8353 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF expr TTHEN stmt  */
#line 1721 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8361 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr block_stmt  */
#line 1725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8369 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1729 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8377 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1733 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8385 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1737 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8393 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar block_stmt  */
#line 1741 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8401 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1745 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8409 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1749 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8417 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1753 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8426 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1758 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8435 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1763 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8444 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1768 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8453 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1776 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                   /*attributes*/ nullptr,
                                   Decl::DEFAULT_VISIBILITY,
                                   Decl::DEFAULT_LINKAGE,
                                   /*linkageName*/ nullptr,
                                   /*name*/ (yyvsp[-2].uniqueStr),
                                   Variable::VAR,
                                   /*isConfig*/ false,
                                   context->currentScopeIsAggregate(),
                                   /*typeExpression*/ nullptr,
                                   toOwned((yyvsp[0].expr)));
    (yyval.expr) = varDecl.release();
  }
#line 8472 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1791 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                   /*attributes*/ nullptr,
                                   Decl::DEFAULT_VISIBILITY,
                                   Decl::DEFAULT_LINKAGE,
                                   /*linkageName*/ nullptr,
                                   /*name*/ (yyvsp[-2].uniqueStr),
                                   Variable::CONST,
                                   /*isConfig*/ false,
                                   context->currentScopeIsAggregate(),
                                   /*typeExpression*/ nullptr,
                                   toOwned((yyvsp[0].expr)));
    (yyval.expr) = varDecl.release();
  }
#line 8491 "bison-chpl-lib.cpp"
    break;

  case 249: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1809 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8499 "bison-chpl-lib.cpp"
    break;

  case 250: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1813 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8507 "bison-chpl-lib.cpp"
    break;

  case 251: /* ifc_formal_ls: ifc_formal  */
#line 1819 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8513 "bison-chpl-lib.cpp"
    break;

  case 252: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1820 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8519 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifc_formal: ident_def  */
#line 1825 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8525 "bison-chpl-lib.cpp"
    break;

  case 266: /* implements_type_ident: implements_type_error_ident  */
#line 1843 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8537 "bison-chpl-lib.cpp"
    break;

  case 273: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1872 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8545 "bison-chpl-lib.cpp"
    break;

  case 274: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1876 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8553 "bison-chpl-lib.cpp"
    break;

  case 275: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1880 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8561 "bison-chpl-lib.cpp"
    break;

  case 276: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1887 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8569 "bison-chpl-lib.cpp"
    break;

  case 277: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1891 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8577 "bison-chpl-lib.cpp"
    break;

  case 278: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1895 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8585 "bison-chpl-lib.cpp"
    break;

  case 279: /* try_stmt: TTRY tryable_stmt  */
#line 1902 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8593 "bison-chpl-lib.cpp"
    break;

  case 280: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1906 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8601 "bison-chpl-lib.cpp"
    break;

  case 281: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1910 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8609 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1914 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8617 "bison-chpl-lib.cpp"
    break;

  case 283: /* catch_expr_ls: %empty  */
#line 1920 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8623 "bison-chpl-lib.cpp"
    break;

  case 284: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1921 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8629 "bison-chpl-lib.cpp"
    break;

  case 285: /* catch_expr: TCATCH block_stmt  */
#line 1926 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8637 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1930 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8645 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1934 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8653 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr_inner: ident_def  */
#line 1941 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                         Decl::DEFAULT_VISIBILITY,
                         Decl::DEFAULT_LINKAGE,
                         /*linkageName*/ nullptr,
                         /*name*/ (yyvsp[0].uniqueStr),
                         Variable::VAR,
                         /*isConfig*/ false,
                         /*isField*/ false,
                         /*typeExpression*/ nullptr,
                         /*initExpression*/ nullptr).release();
  }
#line 8670 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1954 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                         Decl::DEFAULT_VISIBILITY,
                         Decl::DEFAULT_LINKAGE,
                         /*linkageName*/ nullptr,
                         /*name*/ (yyvsp[-2].uniqueStr),
                         Variable::VAR,
                         /*isConfig*/ false,
                         /*isField*/ false,
                         /*typeExpression*/ toOwned((yyvsp[0].expr)),
                         /*initExpression*/ nullptr).release();
  }
#line 8687 "bison-chpl-lib.cpp"
    break;

  case 290: /* throw_stmt: TTHROW expr TSEMI  */
#line 1970 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8697 "bison-chpl-lib.cpp"
    break;

  case 291: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1978 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8705 "bison-chpl-lib.cpp"
    break;

  case 292: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1982 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8715 "bison-chpl-lib.cpp"
    break;

  case 293: /* when_stmt_ls: %empty  */
#line 1990 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8721 "bison-chpl-lib.cpp"
    break;

  case 294: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1991 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8727 "bison-chpl-lib.cpp"
    break;

  case 295: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1996 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8735 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt: TOTHERWISE stmt  */
#line 2000 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8744 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2005 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8753 "bison-chpl-lib.cpp"
    break;

  case 298: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2013 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8762 "bison-chpl-lib.cpp"
    break;

  case 299: /* manager_expr: expr TAS ident_def  */
#line 2018 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 300: /* manager_expr: expr  */
#line 2022 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8778 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr_ls: manager_expr  */
#line 2028 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8784 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2029 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8790 "bison-chpl-lib.cpp"
    break;

  case 303: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2034 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8798 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2043 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8807 "bison-chpl-lib.cpp"
    break;

  case 305: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2048 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 306: /* class_start: class_tag ident_def  */
#line 2059 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8826 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_tag: TCLASS  */
#line 2065 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_tag: TRECORD  */
#line 2066 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8838 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_tag: TUNION  */
#line 2067 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8844 "bison-chpl-lib.cpp"
    break;

  case 310: /* opt_inherit: %empty  */
#line 2071 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 311: /* opt_inherit: TCOLON expr_ls  */
#line 2072 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8856 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_level_stmt_ls: %empty  */
#line 2076 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8865 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2081 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8873 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_level_stmt_ls: class_level_stmt_ls unstable_class_level_stmt  */
#line 2085 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8881 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2089 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8889 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls unstable_class_level_stmt  */
#line 2093 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8897 "bison-chpl-lib.cpp"
    break;

  case 317: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2100 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      ParserExprList* list = (yyvsp[-1].exprList);
      // get any comments after the last element but before the closing brace
      context->appendList(list, context->gatherComments((yylsp[0])));

      auto decl = Enum::build(BUILDER, LOC((yyloc)), toOwned(parts.attributes),
                              parts.visibility,
                              parts.name,
                              context->consumeList(list));
      CommentsAndStmt cs = {parts.comments, decl.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8918 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2117 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2130 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8940 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_ls: deprecated_enum_item  */
#line 2137 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8949 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_ls TCOMMA  */
#line 2142 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8959 "bison-chpl-lib.cpp"
    break;

  case 322: /* $@13: %empty  */
#line 2148 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8968 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_ls: enum_ls TCOMMA $@13 deprecated_enum_item  */
#line 2153 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8977 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: unstable_enum_item  */
#line 2159 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8986 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@14: %empty  */
#line 2164 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8995 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_ls TCOMMA $@14 unstable_enum_item  */
#line 2169 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 9004 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@15: %empty  */
#line 2179 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 329: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@15 enum_item  */
#line 2183 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 9020 "bison-chpl-lib.cpp"
    break;

  case 330: /* $@16: %empty  */
#line 2187 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 9028 "bison-chpl-lib.cpp"
    break;

  case 331: /* deprecated_enum_item: TDEPRECATED $@16 enum_item  */
#line 2191 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 332: /* $@17: %empty  */
#line 2198 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 9044 "bison-chpl-lib.cpp"
    break;

  case 333: /* unstable_enum_item: TUNSTABLE STRINGLITERAL $@17 enum_item  */
#line 2202 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 9052 "bison-chpl-lib.cpp"
    break;

  case 334: /* $@18: %empty  */
#line 2206 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 335: /* unstable_enum_item: TUNSTABLE $@18 enum_item  */
#line 2210 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 9068 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_item: ident_def  */
#line 2217 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 9079 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_item: ident_def TASSIGN expr  */
#line 2224 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 9092 "bison-chpl-lib.cpp"
    break;

  case 338: /* lambda_decl_start: TLAMBDA  */
#line 2236 "chpl.ypp"
    {
      FunctionParts fp = context->makeFunctionParts(false, false);
      context->noteDeclStartLoc((yylsp[0]));
      auto loc = context->declStartLoc((yyloc));
      fp.comments = context->gatherComments(loc);
      fp.attributes = context->buildAttributes((yyloc)).release();
      fp.visibility = context->visibility;
      context->resetAttributePartsState();
      fp.kind = Function::PROC;
      (yyval.functionParts) = fp;
    }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 339: /* $@19: %empty  */
#line 2252 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 9118 "bison-chpl-lib.cpp"
    break;

  case 340: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_lifetime_where $@19 function_body_stmt  */
#line 2258 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-6].functionParts);
      fp.name = context->buildIdent((yylsp[-6]), STR("lambda"));
      fp.formals = (yyvsp[-5].exprList);
      fp.returnIntent = (yyvsp[-4].returnTag);
      fp.returnType = (yyvsp[-3].expr);
      WhereAndLifetime wl = (yyvsp[-2].lifetimeAndWhere);
      fp.body = (yyvsp[0].exprList);

      fp.lifetime = wl.lifetime;
      fp.where = wl.where;

      context->clearComments();
      context->exitScope(asttags::Function, STR("lambda"));

      (yyval.expr) = context->buildLambda((yyloc), fp);
    }
#line 9140 "bison-chpl-lib.cpp"
    break;

  case 342: /* linkage_spec: linkage_spec_empty  */
#line 2281 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 9146 "bison-chpl-lib.cpp"
    break;

  case 343: /* linkage_spec: TINLINE  */
#line 2282 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 9153 "bison-chpl-lib.cpp"
    break;

  case 344: /* linkage_spec: TOVERRIDE  */
#line 2284 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 9160 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2290 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 346: /* $@20: %empty  */
#line 2299 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 9182 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@20 opt_function_body_stmt  */
#line 2309 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-6].functionParts);
    fp.returnIntent = (yyvsp[-5].returnTag);
    fp.returnType = (yyvsp[-4].expr);
    fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS) ? true : false;
    WhereAndLifetime wl = (yyvsp[-2].lifetimeAndWhere);
    fp.body = (yyvsp[0].exprList);
    fp.lifetime = wl.lifetime;
    fp.where = wl.where;
    context->clearComments();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-6].functionParts).errorExpr == nullptr) {
      context->exitScope(asttags::Function, (yyvsp[-6].functionParts).name->name());
    }

    (yyval.functionParts) = fp;
  }
#line 9205 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2331 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9217 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2339 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9229 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2347 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])), /*attributes*/ nullptr,
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9244 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2358 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])), /*attributes*/ nullptr,
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9259 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2369 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9269 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2378 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-1].functionParts);
      context->noteDeclStartLoc((yylsp[0]));
      auto loc = context->declStartLoc((yyloc));
      fp.comments = context->gatherComments(loc);
      fp.attributes = context->buildAttributes((yyloc)).release();
      fp.visibility = context->visibility;
      context->resetAttributePartsState();
      fp.kind = (yyvsp[0].functionKind);
      (yyval.functionParts) = fp;
    }
#line 9285 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2393 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9291 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_ident: ident_def TBANG  */
#line 2400 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 399: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2458 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9307 "bison-chpl-lib.cpp"
    break;

  case 400: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2459 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9313 "bison-chpl-lib.cpp"
    break;

  case 401: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2460 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9319 "bison-chpl-lib.cpp"
    break;

  case 402: /* opt_formal_ls: %empty  */
#line 2464 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 403: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2465 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 404: /* req_formal_ls: TLP formal_ls TRP  */
#line 2469 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9337 "bison-chpl-lib.cpp"
    break;

  case 405: /* formal_ls_inner: formal  */
#line 2473 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9343 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2474 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9349 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_ls: %empty  */
#line 2478 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9355 "bison-chpl-lib.cpp"
    break;

  case 408: /* formal_ls: formal_ls_inner  */
#line 2479 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9361 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal: opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2484 "chpl.ypp"
  {
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9374 "bison-chpl-lib.cpp"
    break;

  case 410: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2493 "chpl.ypp"
  {
    auto attributes = context->buildAttributes((yyloc));
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), std::move(attributes),
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
    context->resetAttributePartsState();
  }
#line 9389 "bison-chpl-lib.cpp"
    break;

  case 411: /* formal: opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2504 "chpl.ypp"
  {
    (yyval.expr) = VarArgFormal::build(BUILDER, LOC((yyloc)),
                             /*attributes*/ nullptr,
                             /*name*/ (yyvsp[-2].uniqueStr),
                             /*intent*/ (yyvsp[-3].intentTag),
                             toOwned((yyvsp[-1].expr)),
                             toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9403 "bison-chpl-lib.cpp"
    break;

  case 412: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2514 "chpl.ypp"
  {
    auto attributes = context->buildAttributes((yyloc));
    (yyval.expr) = VarArgFormal::build(BUILDER, LOC((yyloc)),
                             std::move(attributes),
                             /*name*/ (yyvsp[-2].uniqueStr),
                             /*intent*/ (yyvsp[-3].intentTag),
                             toOwned((yyvsp[-1].expr)),
                             toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
    context->resetAttributePartsState();
  }
#line 9419 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2527 "chpl.ypp"
  {
    (yyval.expr) = TupleDecl::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                          context->visibility,
                          context->linkage,
                          ((TupleDecl::IntentOrKind) (yyvsp[-5].intentTag)),
                          context->consumeList((yyvsp[-3].exprList)),
                          toOwned((yyvsp[-1].expr)),
                          toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9434 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type formal_var_arg_expr  */
#line 2539 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
  }
#line 9442 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_formal_intent_tag: %empty  */
#line 2545 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9451 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_formal_intent_tag: required_intent_tag  */
#line 2550 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9460 "bison-chpl-lib.cpp"
    break;

  case 417: /* required_intent_tag: TIN  */
#line 2557 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9466 "bison-chpl-lib.cpp"
    break;

  case 418: /* required_intent_tag: TINOUT  */
#line 2558 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9472 "bison-chpl-lib.cpp"
    break;

  case 419: /* required_intent_tag: TOUT  */
#line 2559 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9478 "bison-chpl-lib.cpp"
    break;

  case 420: /* required_intent_tag: TCONST TIN  */
#line 2560 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9484 "bison-chpl-lib.cpp"
    break;

  case 421: /* required_intent_tag: TCONST TREF  */
#line 2561 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9490 "bison-chpl-lib.cpp"
    break;

  case 422: /* required_intent_tag: TCONST  */
#line 2562 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9496 "bison-chpl-lib.cpp"
    break;

  case 423: /* required_intent_tag: TPARAM  */
#line 2563 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9502 "bison-chpl-lib.cpp"
    break;

  case 424: /* required_intent_tag: TREF  */
#line 2564 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9508 "bison-chpl-lib.cpp"
    break;

  case 425: /* required_intent_tag: TTYPE  */
#line 2565 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9514 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_this_intent_tag: %empty  */
#line 2569 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9520 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_this_intent_tag: TPARAM  */
#line 2570 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9526 "bison-chpl-lib.cpp"
    break;

  case 428: /* opt_this_intent_tag: TREF  */
#line 2571 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9532 "bison-chpl-lib.cpp"
    break;

  case 429: /* opt_this_intent_tag: TCONST TREF  */
#line 2572 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9538 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_this_intent_tag: TCONST  */
#line 2573 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9544 "bison-chpl-lib.cpp"
    break;

  case 431: /* opt_this_intent_tag: TTYPE  */
#line 2574 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9550 "bison-chpl-lib.cpp"
    break;

  case 432: /* proc_iter_or_op: TPROC  */
#line 2578 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9556 "bison-chpl-lib.cpp"
    break;

  case 433: /* proc_iter_or_op: TITER  */
#line 2579 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9562 "bison-chpl-lib.cpp"
    break;

  case 434: /* proc_iter_or_op: TOPERATOR  */
#line 2580 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9568 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_ret_tag: %empty  */
#line 2584 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9574 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_ret_tag: TCONST  */
#line 2585 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 9580 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_ret_tag: TCONST TREF  */
#line 2586 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 9586 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_ret_tag: TREF  */
#line 2587 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 9592 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_ret_tag: TPARAM  */
#line 2588 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9598 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_ret_tag: TTYPE  */
#line 2589 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9604 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_throws_error: %empty  */
#line 2593 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9610 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_throws_error: TTHROWS  */
#line 2594 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_function_body_stmt: TSEMI  */
#line 2597 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9622 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_function_body_stmt: function_body_stmt  */
#line 2598 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9628 "bison-chpl-lib.cpp"
    break;

  case 445: /* function_body_stmt: block_stmt_body  */
#line 2602 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9634 "bison-chpl-lib.cpp"
    break;

  case 446: /* function_body_stmt: return_stmt  */
#line 2603 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9640 "bison-chpl-lib.cpp"
    break;

  case 447: /* query_expr: TQUERIEDIDENT  */
#line 2607 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9646 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_lifetime_where: %empty  */
#line 2612 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9652 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_lifetime_where: TWHERE expr  */
#line 2614 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9658 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2616 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9664 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2618 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9670 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2620 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9676 "bison-chpl-lib.cpp"
    break;

  case 453: /* lifetime_components_expr: lifetime_expr  */
#line 2625 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 454: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2627 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9688 "bison-chpl-lib.cpp"
    break;

  case 455: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2632 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9694 "bison-chpl-lib.cpp"
    break;

  case 456: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2634 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9700 "bison-chpl-lib.cpp"
    break;

  case 457: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2636 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 458: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2638 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9712 "bison-chpl-lib.cpp"
    break;

  case 459: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2640 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9718 "bison-chpl-lib.cpp"
    break;

  case 460: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2642 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9724 "bison-chpl-lib.cpp"
    break;

  case 461: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2644 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9730 "bison-chpl-lib.cpp"
    break;

  case 462: /* lifetime_ident: TIDENT  */
#line 2648 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9736 "bison-chpl-lib.cpp"
    break;

  case 463: /* lifetime_ident: TTHIS  */
#line 2649 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9742 "bison-chpl-lib.cpp"
    break;

  case 464: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2654 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9751 "bison-chpl-lib.cpp"
    break;

  case 465: /* type_alias_decl_stmt_start: TTYPE  */
#line 2663 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9759 "bison-chpl-lib.cpp"
    break;

  case 466: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2667 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9768 "bison-chpl-lib.cpp"
    break;

  case 467: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2675 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9776 "bison-chpl-lib.cpp"
    break;

  case 468: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2679 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9784 "bison-chpl-lib.cpp"
    break;

  case 469: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2686 "chpl.ypp"
  {
    // TODO (dlongnecke-cray): Add a helper to build this and var_decl_stmt.
    auto node = Variable::build(BUILDER, LOC((yyloc)),
                                context->buildAttributes((yyloc)),
                                context->visibility,
                                context->linkage,
                                context->consumeVarDeclLinkageName(),
                                /*name*/ (yyvsp[-1].uniqueStr),
                                context->varDeclKind,
                                context->isVarDeclConfig,
                                context->currentScopeIsAggregate(),
                                /*typeExpression*/ nullptr,
                                toOwned((yyvsp[0].expr)));
      // Gather the comments like a statement does.
      (yyval.commentsAndStmt) = STMT((yylsp[-1]), node.release());
      context->clearComments();
  }
#line 9806 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_init_type: %empty  */
#line 2706 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9812 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_init_type: TASSIGN type_level_expr  */
#line 2708 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9818 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_init_type: TASSIGN array_type  */
#line 2710 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9827 "bison-chpl-lib.cpp"
    break;

  case 473: /* var_decl_type: TPARAM  */
#line 2717 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9833 "bison-chpl-lib.cpp"
    break;

  case 474: /* var_decl_type: TCONST TREF  */
#line 2718 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9839 "bison-chpl-lib.cpp"
    break;

  case 475: /* var_decl_type: TREF  */
#line 2719 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9845 "bison-chpl-lib.cpp"
    break;

  case 476: /* var_decl_type: TCONST  */
#line 2720 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9851 "bison-chpl-lib.cpp"
    break;

  case 477: /* var_decl_type: TVAR  */
#line 2721 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9857 "bison-chpl-lib.cpp"
    break;

  case 478: /* $@21: %empty  */
#line 2726 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9866 "bison-chpl-lib.cpp"
    break;

  case 479: /* var_decl_stmt: TCONFIG $@21 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2730 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9875 "bison-chpl-lib.cpp"
    break;

  case 480: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2735 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9884 "bison-chpl-lib.cpp"
    break;

  case 481: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2743 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9892 "bison-chpl-lib.cpp"
    break;

  case 482: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2747 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9900 "bison-chpl-lib.cpp"
    break;

  case 483: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2754 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     context->visibility,
                                     context->linkage,
                                     context->consumeVarDeclLinkageName(),
                                     /*name*/ (yyvsp[-2].uniqueStr),
                                     context->varDeclKind,
                                     context->isVarDeclConfig,
                                     context->currentScopeIsAggregate(),
                                     toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr)));
      // gather the comments like a statement does
      (yyval.commentsAndStmt) = STMT((yylsp[-2]), varDecl.release());
      context->clearComments();
    }
#line 9920 "bison-chpl-lib.cpp"
    break;

  case 484: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2770 "chpl.ypp"
    {
      auto intentOrKind = (TupleDecl::IntentOrKind) context->varDeclKind;
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        context->buildAttributes((yyloc)),
                                        context->visibility,
                                        context->linkage,
                                        intentOrKind,
                                        context->consumeList((yyvsp[-3].exprList)),
                                        toOwned((yyvsp[-1].expr)),
                                        toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yylsp[-4]), tupleDecl.release());
      context->clearComments();
    }
#line 9938 "bison-chpl-lib.cpp"
    break;

  case 485: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2787 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9946 "bison-chpl-lib.cpp"
    break;

  case 486: /* tuple_var_decl_component: ident_def  */
#line 2791 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9954 "bison-chpl-lib.cpp"
    break;

  case 487: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2795 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9962 "bison-chpl-lib.cpp"
    break;

  case 488: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2802 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9968 "bison-chpl-lib.cpp"
    break;

  case 489: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2804 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9974 "bison-chpl-lib.cpp"
    break;

  case 490: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2806 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9980 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_init_expr: %empty  */
#line 2812 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9986 "bison-chpl-lib.cpp"
    break;

  case 492: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2813 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9992 "bison-chpl-lib.cpp"
    break;

  case 493: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2814 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9998 "bison-chpl-lib.cpp"
    break;

  case 494: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2820 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 495: /* ret_array_type: TLSBR TRSBR  */
#line 2824 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 10014 "bison-chpl-lib.cpp"
    break;

  case 496: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2828 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10022 "bison-chpl-lib.cpp"
    break;

  case 497: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2832 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 10030 "bison-chpl-lib.cpp"
    break;

  case 498: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2836 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10038 "bison-chpl-lib.cpp"
    break;

  case 499: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2840 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10046 "bison-chpl-lib.cpp"
    break;

  case 500: /* ret_array_type: TLSBR error TRSBR  */
#line 2844 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "invalid expression for domain of array return type");
  }
#line 10054 "bison-chpl-lib.cpp"
    break;

  case 501: /* opt_ret_type: %empty  */
#line 2850 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 10060 "bison-chpl-lib.cpp"
    break;

  case 502: /* opt_ret_type: TCOLON type_level_expr  */
#line 2851 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10066 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_ret_type: TCOLON ret_array_type  */
#line 2852 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10072 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2853 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10078 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_ret_type: error  */
#line 2854 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10084 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_type: %empty  */
#line 2859 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_type: TCOLON type_level_expr  */
#line 2860 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10096 "bison-chpl-lib.cpp"
    break;

  case 508: /* opt_type: TCOLON array_type  */
#line 2861 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 509: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2862 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_type: error  */
#line 2863 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10114 "bison-chpl-lib.cpp"
    break;

  case 511: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2884 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10122 "bison-chpl-lib.cpp"
    break;

  case 512: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2888 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10130 "bison-chpl-lib.cpp"
    break;

  case 513: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2892 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10138 "bison-chpl-lib.cpp"
    break;

  case 514: /* array_type: TLSBR error TRSBR  */
#line 2896 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 10146 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_formal_array_elt_type: %empty  */
#line 2902 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 10152 "bison-chpl-lib.cpp"
    break;

  case 516: /* opt_formal_array_elt_type: type_level_expr  */
#line 2903 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10158 "bison-chpl-lib.cpp"
    break;

  case 517: /* opt_formal_array_elt_type: query_expr  */
#line 2904 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10164 "bison-chpl-lib.cpp"
    break;

  case 518: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2909 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10173 "bison-chpl-lib.cpp"
    break;

  case 519: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2914 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10181 "bison-chpl-lib.cpp"
    break;

  case 520: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2922 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10190 "bison-chpl-lib.cpp"
    break;

  case 521: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2927 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10198 "bison-chpl-lib.cpp"
    break;

  case 522: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2931 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10206 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_formal_type: %empty  */
#line 2937 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 10212 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_formal_type: TCOLON type_level_expr  */
#line 2938 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_formal_type: TCOLON query_expr  */
#line 2939 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10224 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2940 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10230 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_formal_type: TCOLON formal_array_type  */
#line 2941 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10236 "bison-chpl-lib.cpp"
    break;

  case 528: /* expr_ls: expr  */
#line 2947 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 529: /* expr_ls: query_expr  */
#line 2948 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10248 "bison-chpl-lib.cpp"
    break;

  case 530: /* expr_ls: expr_ls TCOMMA expr  */
#line 2949 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10254 "bison-chpl-lib.cpp"
    break;

  case 531: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2950 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10260 "bison-chpl-lib.cpp"
    break;

  case 532: /* simple_expr_ls: expr  */
#line 2954 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10266 "bison-chpl-lib.cpp"
    break;

  case 533: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2955 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10272 "bison-chpl-lib.cpp"
    break;

  case 534: /* tuple_component: TUNDERSCORE  */
#line 2959 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10278 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_component: opt_try_expr  */
#line 2960 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10284 "bison-chpl-lib.cpp"
    break;

  case 536: /* tuple_component: query_expr  */
#line 2961 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10290 "bison-chpl-lib.cpp"
    break;

  case 537: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2966 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10298 "bison-chpl-lib.cpp"
    break;

  case 538: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2970 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10306 "bison-chpl-lib.cpp"
    break;

  case 539: /* opt_actual_ls: %empty  */
#line 2976 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10312 "bison-chpl-lib.cpp"
    break;

  case 540: /* opt_actual_ls: actual_ls  */
#line 2977 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10318 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_ls: actual_expr  */
#line 2982 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10327 "bison-chpl-lib.cpp"
    break;

  case 542: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2987 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10337 "bison-chpl-lib.cpp"
    break;

  case 543: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2995 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10343 "bison-chpl-lib.cpp"
    break;

  case 544: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2996 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10349 "bison-chpl-lib.cpp"
    break;

  case 545: /* actual_expr: query_expr  */
#line 2997 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10355 "bison-chpl-lib.cpp"
    break;

  case 546: /* actual_expr: opt_try_expr  */
#line 2998 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10361 "bison-chpl-lib.cpp"
    break;

  case 547: /* ident_expr: ident_use  */
#line 3002 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10367 "bison-chpl-lib.cpp"
    break;

  case 548: /* ident_expr: scalar_type  */
#line 3003 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10373 "bison-chpl-lib.cpp"
    break;

  case 549: /* type_level_expr: sub_type_level_expr  */
#line 3015 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 10379 "bison-chpl-lib.cpp"
    break;

  case 550: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 3017 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10385 "bison-chpl-lib.cpp"
    break;

  case 551: /* type_level_expr: TQUESTION  */
#line 3019 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10391 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TSINGLE expr  */
#line 3030 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10397 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3032 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10403 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3034 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10409 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3036 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10415 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3038 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10425 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TATOMIC expr  */
#line 3044 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10431 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSYNC expr  */
#line 3046 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10437 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TOWNED  */
#line 3049 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10443 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TOWNED expr  */
#line 3051 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10449 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TUNMANAGED  */
#line 3053 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10455 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3055 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10461 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TSHARED  */
#line 3057 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10467 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TSHARED expr  */
#line 3059 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10473 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TBORROWED  */
#line 3061 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10479 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TBORROWED expr  */
#line 3063 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TCLASS  */
#line 3066 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TRECORD  */
#line 3068 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10497 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 3073 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10510 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3082 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10523 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr: TFOR expr TDO expr  */
#line 3091 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10535 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3099 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].expr));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-5].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock(ifLoc, ifExpr.release()),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10553 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3113 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].expr));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-5].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock(ifLoc, ifExpr.release()),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10571 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3127 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr,
                    toOwned((yyvsp[-5].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock(ifLoc, ifExpr.release()),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10589 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3141 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10602 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3150 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10615 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr: TFORALL expr TDO expr  */
#line 3159 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10627 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3167 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].expr));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-5].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock(ifLoc, ifExpr.release()),
                       /*isExpressionLevel*/ true).release();
  }
#line 10645 "bison-chpl-lib.cpp"
    break;

  case 584: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3181 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].expr));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr)= Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-5].expr)),
                      /*withClause*/ nullptr,
                      BlockStyle::IMPLICIT,
                      context->consumeToBlock(ifLoc, ifExpr.release()),
                      /*isExpressionLevel*/ true).release();
  }
#line 10663 "bison-chpl-lib.cpp"
    break;

  case 585: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3195 "chpl.ypp"
  {
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr,
                       toOwned((yyvsp[-5].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock(ifLoc, ifExpr.release()),
                       /*isExpressionLevel*/ true).release();
  }
#line 10681 "bison-chpl-lib.cpp"
    break;

  case 586: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 3209 "chpl.ypp"
  {
    owned<AstNode> iterand = nullptr;
    auto iterExprs = context->consumeList((yyvsp[-2].exprList));
    if (iterExprs.size() > 1) {
      iterand = Domain::build(BUILDER, LOC((yylsp[-2])), false, std::move(iterExprs));
    } else {
      iterand = std::move(iterExprs[0]);
    }
    assert(iterand);
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), /*index*/ nullptr,
                            std::move(iterand),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10702 "bison-chpl-lib.cpp"
    break;

  case 587: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3226 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10715 "bison-chpl-lib.cpp"
    break;

  case 588: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3235 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10728 "bison-chpl-lib.cpp"
    break;

  case 589: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3244 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].exprList));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index),
                            toOwned((yyvsp[-5].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock(ifLoc, ifExpr.release()),
                            /*isExpressionLevel*/ true).release();
  }
#line 10747 "bison-chpl-lib.cpp"
    break;

  case 590: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3259 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-7]), (yyvsp[-7].exprList));
    auto ifExpr = Conditional::build(BUILDER, LOC2((yylsp[-3]), (yylsp[0])), toOwned((yyvsp[-2].expr)),
                                     BlockStyle::IMPLICIT,
                                     context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                     /*isExpressionLevel*/ true);
    auto ifLoc = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index),
                            toOwned((yyvsp[-5].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock(ifLoc, ifExpr.release()),
                            /*isExpressionLevel*/ true).release();
  }
#line 10766 "bison-chpl-lib.cpp"
    break;

  case 591: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3277 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10780 "bison-chpl-lib.cpp"
    break;

  case 592: /* nil_expr: TNIL  */
#line 3294 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10786 "bison-chpl-lib.cpp"
    break;

  case 600: /* stmt_level_expr: io_expr TIO expr  */
#line 3310 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10792 "bison-chpl-lib.cpp"
    break;

  case 601: /* opt_task_intent_ls: %empty  */
#line 3314 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10798 "bison-chpl-lib.cpp"
    break;

  case 602: /* opt_task_intent_ls: task_intent_clause  */
#line 3315 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10804 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3320 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10814 "bison-chpl-lib.cpp"
    break;

  case 604: /* task_intent_ls: intent_expr  */
#line 3328 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10820 "bison-chpl-lib.cpp"
    break;

  case 605: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3329 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10826 "bison-chpl-lib.cpp"
    break;

  case 606: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3334 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10836 "bison-chpl-lib.cpp"
    break;

  case 607: /* forall_intent_ls: intent_expr  */
#line 3342 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10842 "bison-chpl-lib.cpp"
    break;

  case 608: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3343 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10848 "bison-chpl-lib.cpp"
    break;

  case 609: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3348 "chpl.ypp"
  {
    if (auto ident = (yyvsp[-2].expr)->toIdentifier()) {
      auto name = ident->name();
      auto node = TaskVar::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                                 name,
                                 /*intent*/ (yyvsp[-3].taskIntent),
                                 toOwned((yyvsp[-1].expr)),
                                 toOwned((yyvsp[0].expr)));
      (yyval.expr) = node.release();
    } else {
      const char* msg = "Expected identifier for task variable name";
      (yyval.expr) = context->raiseError((yyloc), msg);
    }
  }
#line 10867 "bison-chpl-lib.cpp"
    break;

  case 610: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3363 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10875 "bison-chpl-lib.cpp"
    break;

  case 611: /* intent_expr: expr TREDUCE ident_expr  */
#line 3367 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10883 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TCONST  */
#line 3373 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10889 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TIN  */
#line 3374 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10895 "bison-chpl-lib.cpp"
    break;

  case 614: /* task_var_prefix: TCONST TIN  */
#line 3375 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10901 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TREF  */
#line 3376 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10907 "bison-chpl-lib.cpp"
    break;

  case 616: /* task_var_prefix: TCONST TREF  */
#line 3377 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10913 "bison-chpl-lib.cpp"
    break;

  case 617: /* task_var_prefix: TVAR  */
#line 3378 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10919 "bison-chpl-lib.cpp"
    break;

  case 619: /* io_expr: io_expr TIO expr  */
#line 3384 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10925 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_maybe_decorated: TNEW  */
#line 3389 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10931 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_maybe_decorated: TNEW TOWNED  */
#line 3391 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10937 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_maybe_decorated: TNEW TSHARED  */
#line 3393 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10943 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3395 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10949 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3397 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10955 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_expr: new_maybe_decorated expr  */
#line 3403 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10963 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3410 "chpl.ypp"
  {
    AstList actuals;
    std::vector<UniqueString> actualNames;
    context->consumeNamedActuals((yyvsp[-1].maybeNamedActualList), actuals, actualNames);
    auto call = FnCall::build(BUILDER, LOC((yyloc)),
                              toOwned((yyvsp[-4].expr)),
                              std::move(actuals),
                              std::move(actualNames),
                              /* square */ false);
    (yyval.expr) = context->buildNewExpr((yyloc), New::OWNED, call.release());
  }
#line 10979 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3422 "chpl.ypp"
  {
    AstList actuals;
    std::vector<UniqueString> actualNames;
    context->consumeNamedActuals((yyvsp[-1].maybeNamedActualList), actuals, actualNames);
    auto call = FnCall::build(BUILDER, LOC((yyloc)),
                              toOwned((yyvsp[-4].expr)),
                              std::move(actuals),
                              std::move(actualNames),
                              /* square */ false);
    (yyval.expr) = context->buildNewExpr((yyloc), New::SHARED, call.release());
  }
#line 10995 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3434 "chpl.ypp"
  {
    AstList actuals;
    std::vector<UniqueString> actualNames;
    context->consumeNamedActuals((yyvsp[-2].maybeNamedActualList), actuals, actualNames);
    auto base = context->buildUnaryOp((yylsp[-5]), (yyvsp[0].uniqueStr), (yyvsp[-5].expr));
    auto call = FnCall::build(BUILDER, LOC((yyloc)),
                              toOwned(base),
                              std::move(actuals),
                              std::move(actualNames),
                              /* square */ false);
    (yyval.expr) = context->buildNewExpr((yyloc), New::OWNED, call.release());
  }
#line 11012 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3447 "chpl.ypp"
  {
    AstList actuals;
    std::vector<UniqueString> actualNames;
    context->consumeNamedActuals((yyvsp[-2].maybeNamedActualList), actuals, actualNames);
    auto base = context->buildUnaryOp((yylsp[-5]), (yyvsp[0].uniqueStr), (yyvsp[-5].expr));
    auto call = FnCall::build(BUILDER, LOC((yyloc)),
                              toOwned(base),
                              std::move(actuals),
                              std::move(actualNames),
                              /* square */ false);
    (yyval.expr) = context->buildNewExpr((yyloc), New::SHARED, call.release());

  }
#line 11030 "bison-chpl-lib.cpp"
    break;

  case 630: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3464 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 11038 "bison-chpl-lib.cpp"
    break;

  case 640: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 3483 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 11046 "bison-chpl-lib.cpp"
    break;

  case 641: /* expr: expr TCOLON expr  */
#line 3487 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 11054 "bison-chpl-lib.cpp"
    break;

  case 642: /* expr: expr TDOTDOT expr  */
#line 3491 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 11063 "bison-chpl-lib.cpp"
    break;

  case 643: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 3496 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 11072 "bison-chpl-lib.cpp"
    break;

  case 644: /* expr: expr TDOTDOT  */
#line 3501 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 11081 "bison-chpl-lib.cpp"
    break;

  case 645: /* expr: TDOTDOT expr  */
#line 3506 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 11091 "bison-chpl-lib.cpp"
    break;

  case 646: /* expr: TDOTDOTOPENHIGH expr  */
#line 3512 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 11101 "bison-chpl-lib.cpp"
    break;

  case 647: /* expr: TDOTDOT  */
#line 3518 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 11111 "bison-chpl-lib.cpp"
    break;

  case 648: /* opt_expr: %empty  */
#line 3549 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 11117 "bison-chpl-lib.cpp"
    break;

  case 649: /* opt_expr: expr  */
#line 3550 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11123 "bison-chpl-lib.cpp"
    break;

  case 650: /* opt_try_expr: TTRY expr  */
#line 3554 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 11129 "bison-chpl-lib.cpp"
    break;

  case 651: /* opt_try_expr: TTRYBANG expr  */
#line 3555 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 11135 "bison-chpl-lib.cpp"
    break;

  case 652: /* opt_try_expr: expr  */
#line 3556 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11141 "bison-chpl-lib.cpp"
    break;

  case 658: /* call_base_expr: expr TBANG  */
#line 3573 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 11149 "bison-chpl-lib.cpp"
    break;

  case 659: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3576 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 11155 "bison-chpl-lib.cpp"
    break;

  case 662: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3583 "chpl.ypp"
    {
      AstList actuals;
      std::vector<UniqueString> actualNames;
      context->consumeNamedActuals((yyvsp[-1].maybeNamedActualList), actuals, actualNames);
      auto fnCall = FnCall::build(BUILDER, LOC((yyloc)),
                                  toOwned((yyvsp[-3].expr)),
                                  std::move(actuals),
                                  std::move(actualNames),
                                  /* square */ false);
      (yyval.expr) = fnCall.release();
    }
#line 11171 "bison-chpl-lib.cpp"
    break;

  case 663: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3595 "chpl.ypp"
    {
      AstList actuals;
      std::vector<UniqueString> actualNames;
      context->consumeNamedActuals((yyvsp[-1].maybeNamedActualList), actuals, actualNames);
      auto fnCall = FnCall::build(BUILDER, LOC((yyloc)),
                                  toOwned((yyvsp[-3].expr)),
                                  std::move(actuals),
                                  std::move(actualNames),
                                  /* square */ true);
      (yyval.expr) = fnCall.release();
    }
#line 11187 "bison-chpl-lib.cpp"
    break;

  case 664: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3607 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 11195 "bison-chpl-lib.cpp"
    break;

  case 665: /* dot_expr: expr TDOT ident_use  */
#line 3614 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11201 "bison-chpl-lib.cpp"
    break;

  case 666: /* dot_expr: expr TDOT TTYPE  */
#line 3616 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11207 "bison-chpl-lib.cpp"
    break;

  case 667: /* dot_expr: expr TDOT TDOMAIN  */
#line 3618 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11213 "bison-chpl-lib.cpp"
    break;

  case 668: /* dot_expr: expr TDOT TLOCALE  */
#line 3620 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11219 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3622 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 11229 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3628 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 11239 "bison-chpl-lib.cpp"
    break;

  case 671: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3640 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 11245 "bison-chpl-lib.cpp"
    break;

  case 672: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3642 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 11253 "bison-chpl-lib.cpp"
    break;

  case 673: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3646 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11261 "bison-chpl-lib.cpp"
    break;

  case 674: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3650 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11269 "bison-chpl-lib.cpp"
    break;

  case 675: /* bool_literal: TFALSE  */
#line 3656 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 11275 "bison-chpl-lib.cpp"
    break;

  case 676: /* bool_literal: TTRUE  */
#line 3657 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 11281 "bison-chpl-lib.cpp"
    break;

  case 677: /* str_bytes_literal: STRINGLITERAL  */
#line 3661 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11287 "bison-chpl-lib.cpp"
    break;

  case 678: /* str_bytes_literal: BYTESLITERAL  */
#line 3662 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11293 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: INTLITERAL  */
#line 3668 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 11299 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: REALLITERAL  */
#line 3669 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11305 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: IMAGLITERAL  */
#line 3670 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11311 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: CSTRINGLITERAL  */
#line 3671 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11317 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: TNONE  */
#line 3672 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11323 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3674 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11332 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3679 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11341 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3684 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11349 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3688 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11358 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3693 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11366 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3697 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11375 "bison-chpl-lib.cpp"
    break;

  case 692: /* assoc_expr_ls: expr TALIAS expr  */
#line 3706 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11384 "bison-chpl-lib.cpp"
    break;

  case 693: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3711 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11394 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TPLUS expr  */
#line 3719 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11400 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TMINUS expr  */
#line 3720 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11406 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TSTAR expr  */
#line 3721 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11412 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TDIVIDE expr  */
#line 3722 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11418 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3723 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11424 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3724 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11430 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TMOD expr  */
#line 3725 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11436 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TEQUAL expr  */
#line 3726 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11442 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3727 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11448 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3728 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11454 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3729 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11460 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TLESS expr  */
#line 3730 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11466 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TGREATER expr  */
#line 3731 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11472 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TBAND expr  */
#line 3732 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11478 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TBOR expr  */
#line 3733 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11484 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TBXOR expr  */
#line 3734 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11490 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TAND expr  */
#line 3735 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11496 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TOR expr  */
#line 3736 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11502 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TEXP expr  */
#line 3737 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11508 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TBY expr  */
#line 3738 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11514 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TALIGN expr  */
#line 3739 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11520 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr THASH expr  */
#line 3740 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11526 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TDMAPPED expr  */
#line 3741 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11532 "bison-chpl-lib.cpp"
    break;

  case 717: /* unary_op_expr: TPLUS expr  */
#line 3745 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11538 "bison-chpl-lib.cpp"
    break;

  case 718: /* unary_op_expr: TMINUS expr  */
#line 3746 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11544 "bison-chpl-lib.cpp"
    break;

  case 719: /* unary_op_expr: TMINUSMINUS expr  */
#line 3747 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11550 "bison-chpl-lib.cpp"
    break;

  case 720: /* unary_op_expr: TPLUSPLUS expr  */
#line 3748 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11556 "bison-chpl-lib.cpp"
    break;

  case 721: /* unary_op_expr: TBANG expr  */
#line 3749 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11562 "bison-chpl-lib.cpp"
    break;

  case 722: /* unary_op_expr: expr TBANG  */
#line 3750 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11570 "bison-chpl-lib.cpp"
    break;

  case 723: /* unary_op_expr: TBNOT expr  */
#line 3753 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11576 "bison-chpl-lib.cpp"
    break;

  case 724: /* reduce_expr: expr TREDUCE expr  */
#line 3758 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11584 "bison-chpl-lib.cpp"
    break;

  case 725: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3762 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11592 "bison-chpl-lib.cpp"
    break;

  case 726: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3766 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11600 "bison-chpl-lib.cpp"
    break;

  case 727: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3770 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11608 "bison-chpl-lib.cpp"
    break;

  case 728: /* scan_expr: expr TSCAN expr  */
#line 3777 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11616 "bison-chpl-lib.cpp"
    break;

  case 729: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3781 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11624 "bison-chpl-lib.cpp"
    break;

  case 730: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3785 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11632 "bison-chpl-lib.cpp"
    break;

  case 731: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3789 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11640 "bison-chpl-lib.cpp"
    break;


#line 11644 "bison-chpl-lib.cpp"

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
  *++yylsp = yyloc;

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
  yytoken = yychar == YYCHPL_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (&yylloc, context, YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYCHPL_EOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYCHPL_EOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, context);
          yychar = YYCHPL_EMPTY;
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
  ++yynerrs;

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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, context);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, context, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYCHPL_EMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, context);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, context);
      YYPOPSTACK (1);
    }
  yyps->yynew = 2;
  goto yypushreturn;


/*-------------------------.
| yypushreturn -- return.  |
`-------------------------*/
yypushreturn:

  return yyresult;
}
#undef yychpl_nerrs
#undef yystate
#undef yyerrstatus
#undef yyssa
#undef yyss
#undef yyssp
#undef yyvsa
#undef yyvs
#undef yyvsp
#undef yylsa
#undef yyls
#undef yylsp
#undef yystacksize
