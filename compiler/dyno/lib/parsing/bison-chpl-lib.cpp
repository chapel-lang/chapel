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
  YYSYMBOL_TPROCLP = 87,                   /* TPROCLP  */
  YYSYMBOL_TREAL = 88,                     /* TREAL  */
  YYSYMBOL_TRECORD = 89,                   /* TRECORD  */
  YYSYMBOL_TREDUCE = 90,                   /* TREDUCE  */
  YYSYMBOL_TREF = 91,                      /* TREF  */
  YYSYMBOL_TREQUIRE = 92,                  /* TREQUIRE  */
  YYSYMBOL_TRETURN = 93,                   /* TRETURN  */
  YYSYMBOL_TSCAN = 94,                     /* TSCAN  */
  YYSYMBOL_TSELECT = 95,                   /* TSELECT  */
  YYSYMBOL_TSERIAL = 96,                   /* TSERIAL  */
  YYSYMBOL_TSHARED = 97,                   /* TSHARED  */
  YYSYMBOL_TSINGLE = 98,                   /* TSINGLE  */
  YYSYMBOL_TSPARSE = 99,                   /* TSPARSE  */
  YYSYMBOL_TSTRING = 100,                  /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 101,               /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 102,                    /* TSYNC  */
  YYSYMBOL_TTHEN = 103,                    /* TTHEN  */
  YYSYMBOL_TTHIS = 104,                    /* TTHIS  */
  YYSYMBOL_TTHROW = 105,                   /* TTHROW  */
  YYSYMBOL_TTHROWS = 106,                  /* TTHROWS  */
  YYSYMBOL_TTRUE = 107,                    /* TTRUE  */
  YYSYMBOL_TTRY = 108,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 109,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 110,                    /* TTYPE  */
  YYSYMBOL_TUINT = 111,                    /* TUINT  */
  YYSYMBOL_TUNDERSCORE = 112,              /* TUNDERSCORE  */
  YYSYMBOL_TUNION = 113,                   /* TUNION  */
  YYSYMBOL_TUNMANAGED = 114,               /* TUNMANAGED  */
  YYSYMBOL_TUNSTABLE = 115,                /* TUNSTABLE  */
  YYSYMBOL_TUSE = 116,                     /* TUSE  */
  YYSYMBOL_TVAR = 117,                     /* TVAR  */
  YYSYMBOL_TVOID = 118,                    /* TVOID  */
  YYSYMBOL_TWHEN = 119,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 120,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 121,                   /* TWHILE  */
  YYSYMBOL_TWITH = 122,                    /* TWITH  */
  YYSYMBOL_TYIELD = 123,                   /* TYIELD  */
  YYSYMBOL_TZIP = 124,                     /* TZIP  */
  YYSYMBOL_TALIAS = 125,                   /* TALIAS  */
  YYSYMBOL_TAND = 126,                     /* TAND  */
  YYSYMBOL_TASSIGN = 127,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 128,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 129,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 130,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 131,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 132,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 133,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 134,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 135,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 136,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 137,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 138,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 139,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 140,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 141,                /* TASSIGNSR  */
  YYSYMBOL_TBANG = 142,                    /* TBANG  */
  YYSYMBOL_TBAND = 143,                    /* TBAND  */
  YYSYMBOL_TBNOT = 144,                    /* TBNOT  */
  YYSYMBOL_TBOR = 145,                     /* TBOR  */
  YYSYMBOL_TBXOR = 146,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 147,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 148,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 149,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 150,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 151,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 152,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 153,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 154,                     /* TEXP  */
  YYSYMBOL_TGREATER = 155,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 156,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 157,                    /* THASH  */
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
  YYSYMBOL_reserved_word_ident = 214,      /* reserved_word_ident  */
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
  YYSYMBOL_opt_fn_type_formal_ls = 284,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 285,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 286,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 287,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 288,                  /* fn_type  */
  YYSYMBOL_fn_expr = 289,                  /* fn_expr  */
  YYSYMBOL_290_20 = 290,                   /* $@20  */
  YYSYMBOL_291_21 = 291,                   /* $@21  */
  YYSYMBOL_fn_decl_stmt_complete = 292,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 293,             /* fn_decl_stmt  */
  YYSYMBOL_294_22 = 294,                   /* $@22  */
  YYSYMBOL_fn_decl_stmt_inner = 295,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 296,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 297,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 298,                 /* fn_ident  */
  YYSYMBOL_op_ident = 299,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 300,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 301,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 302,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 303,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 304,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 305,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 306,                /* formal_ls  */
  YYSYMBOL_formal = 307,                   /* formal  */
  YYSYMBOL_named_formal = 308,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 309,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 310,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 311,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 312,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 313,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 314,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 315,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 316,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 317,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 318,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 319, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 320,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 321,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 322,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 323, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 324, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 325, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 326,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 327,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 328,            /* var_decl_stmt  */
  YYSYMBOL_329_23 = 329,                   /* $@23  */
  YYSYMBOL_var_decl_stmt_inner_ls = 330,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 331,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 332, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 333, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 334,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 335,           /* ret_array_type  */
  YYSYMBOL_ret_type = 336,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 337,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 338,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 339,                 /* opt_type  */
  YYSYMBOL_array_type = 340,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 341, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 342,        /* formal_array_type  */
  YYSYMBOL_formal_type = 343,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 344,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 345,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 346,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 347,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 348,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 349,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 350,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 351,                /* actual_ls  */
  YYSYMBOL_actual_expr = 352,              /* actual_expr  */
  YYSYMBOL_ident_expr = 353,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 354,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 355,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 356,                 /* for_expr  */
  YYSYMBOL_cond_expr = 357,                /* cond_expr  */
  YYSYMBOL_nil_expr = 358,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 359,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 360,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 361,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 362,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 363,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 364,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 365,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 366,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 367,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 368,                 /* new_expr  */
  YYSYMBOL_let_expr = 369,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 370,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 371,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 372,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 373,               /* super_expr  */
  YYSYMBOL_expr = 374,                     /* expr  */
  YYSYMBOL_opt_expr = 375,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 376,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 377,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 378,           /* call_base_expr  */
  YYSYMBOL_call_expr = 379,                /* call_expr  */
  YYSYMBOL_dot_expr = 380,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 381,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 382,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 383,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 384,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 385,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 386,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 387,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 388,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 389,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 390       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 327 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include helper macros for use in writing productions
  #include "parser-help.h"
  // include implementation of error logging hook
  #include "parser-yyerror.h"

#line 514 "bison-chpl-lib.cpp"

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
#define YYLAST   20769

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  209
/* YYNRULES -- Number of rules.  */
#define YYNRULES  756
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1325

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
       0,   630,   630,   634,   635,   640,   641,   649,   653,   661,
     662,   663,   664,   665,   666,   667,   668,   669,   670,   671,
     672,   685,   686,   687,   696,   705,   709,   716,   724,   725,
     726,   727,   728,   729,   730,   731,   745,   760,   768,   780,
     791,   803,   815,   826,   842,   844,   843,   852,   851,   862,
     863,   868,   867,   876,   875,   886,   906,   921,   936,   952,
     953,   955,   960,   961,   966,   984,   988,   992,  1003,  1022,
    1023,  1027,  1031,  1035,  1040,  1044,  1048,  1058,  1063,  1069,
    1076,  1081,  1088,  1099,  1100,  1104,  1109,  1117,  1126,  1136,
    1144,  1153,  1166,  1174,  1178,  1183,  1189,  1198,  1199,  1203,
    1212,  1216,  1220,  1224,  1228,  1232,  1241,  1242,  1246,  1247,
    1248,  1249,  1250,  1254,  1255,  1267,  1268,  1273,  1274,  1275,
    1276,  1277,  1278,  1279,  1280,  1281,  1282,  1283,  1284,  1285,
    1286,  1287,  1288,  1289,  1290,  1291,  1295,  1296,  1297,  1298,
    1299,  1300,  1301,  1302,  1303,  1304,  1305,  1306,  1313,  1314,
    1315,  1316,  1320,  1321,  1325,  1332,  1342,  1344,  1343,  1352,
    1351,  1363,  1362,  1371,  1370,  1381,  1385,  1391,  1391,  1397,
    1397,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1416,  1421,
    1426,  1431,  1438,  1446,  1447,  1451,  1460,  1466,  1474,  1493,
    1492,  1502,  1501,  1514,  1521,  1539,  1552,  1565,  1569,  1573,
    1577,  1581,  1585,  1589,  1593,  1611,  1615,  1619,  1623,  1627,
    1631,  1635,  1639,  1643,  1647,  1651,  1655,  1659,  1663,  1667,
    1671,  1675,  1679,  1683,  1687,  1691,  1695,  1699,  1704,  1712,
    1721,  1725,  1729,  1733,  1737,  1741,  1745,  1749,  1753,  1758,
    1763,  1768,  1776,  1791,  1809,  1813,  1820,  1821,  1826,  1831,
    1832,  1833,  1834,  1835,  1836,  1837,  1838,  1839,  1840,  1841,
    1842,  1843,  1855,  1856,  1857,  1858,  1867,  1868,  1872,  1876,
    1880,  1887,  1891,  1895,  1902,  1906,  1910,  1914,  1921,  1922,
    1926,  1930,  1934,  1941,  1954,  1970,  1978,  1982,  1991,  1992,
    1996,  2000,  2005,  2013,  2018,  2022,  2029,  2030,  2034,  2043,
    2048,  2059,  2066,  2067,  2068,  2072,  2073,  2077,  2081,  2085,
    2089,  2093,  2100,  2117,  2130,  2137,  2142,  2149,  2148,  2159,
    2165,  2164,  2178,  2180,  2179,  2188,  2187,  2199,  2198,  2207,
    2206,  2217,  2224,  2236,  2253,  2251,  2278,  2282,  2283,  2285,
    2290,  2291,  2295,  2296,  2300,  2303,  2305,  2312,  2313,  2325,
    2346,  2345,  2361,  2360,  2378,  2388,  2385,  2419,  2427,  2435,
    2446,  2457,  2466,  2481,  2482,  2486,  2487,  2488,  2497,  2498,
    2499,  2500,  2501,  2502,  2503,  2504,  2505,  2506,  2507,  2508,
    2509,  2510,  2511,  2512,  2513,  2514,  2515,  2516,  2517,  2518,
    2519,  2520,  2521,  2525,  2526,  2527,  2528,  2529,  2530,  2531,
    2532,  2533,  2534,  2535,  2536,  2541,  2542,  2546,  2547,  2548,
    2552,  2553,  2557,  2558,  2562,  2563,  2567,  2568,  2572,  2576,
    2580,  2584,  2588,  2592,  2597,  2605,  2609,  2617,  2618,  2619,
    2620,  2621,  2622,  2623,  2624,  2625,  2629,  2630,  2631,  2632,
    2633,  2634,  2638,  2639,  2640,  2644,  2645,  2646,  2647,  2648,
    2649,  2653,  2654,  2657,  2658,  2662,  2663,  2667,  2672,  2673,
    2675,  2677,  2679,  2684,  2686,  2691,  2693,  2695,  2697,  2699,
    2701,  2703,  2708,  2709,  2713,  2722,  2726,  2734,  2738,  2745,
    2766,  2767,  2769,  2777,  2778,  2779,  2780,  2781,  2786,  2785,
    2794,  2802,  2806,  2813,  2829,  2846,  2850,  2854,  2861,  2863,
    2865,  2872,  2873,  2874,  2878,  2882,  2886,  2890,  2894,  2898,
    2902,  2909,  2910,  2911,  2912,  2916,  2917,  2921,  2922,  2926,
    2927,  2928,  2929,  2930,  2950,  2954,  2958,  2962,  2969,  2970,
    2971,  2975,  2980,  2988,  2993,  2997,  3004,  3005,  3006,  3007,
    3011,  3015,  3016,  3022,  3023,  3024,  3025,  3029,  3030,  3034,
    3035,  3036,  3040,  3044,  3051,  3052,  3056,  3061,  3070,  3071,
    3072,  3073,  3077,  3078,  3089,  3091,  3093,  3095,  3102,  3103,
    3104,  3105,  3106,  3107,  3109,  3111,  3113,  3115,  3121,  3123,
    3126,  3128,  3130,  3132,  3134,  3136,  3138,  3140,  3142,  3144,
    3149,  3158,  3167,  3175,  3189,  3203,  3217,  3226,  3235,  3243,
    3257,  3271,  3285,  3302,  3311,  3320,  3335,  3353,  3371,  3379,
    3380,  3381,  3382,  3383,  3384,  3385,  3389,  3390,  3394,  3403,
    3404,  3408,  3417,  3418,  3422,  3437,  3441,  3448,  3449,  3450,
    3451,  3452,  3453,  3457,  3459,  3461,  3463,  3465,  3471,  3478,
    3490,  3502,  3515,  3532,  3539,  3544,  3549,  3554,  3560,  3566,
    3596,  3603,  3610,  3611,  3615,  3616,  3617,  3618,  3619,  3620,
    3621,  3622,  3623,  3624,  3625,  3626,  3630,  3631,  3635,  3636,
    3637,  3641,  3642,  3643,  3644,  3653,  3654,  3657,  3658,  3659,
    3663,  3675,  3687,  3694,  3696,  3698,  3700,  3702,  3708,  3721,
    3722,  3726,  3730,  3737,  3738,  3742,  3743,  3747,  3748,  3749,
    3750,  3751,  3752,  3753,  3754,  3759,  3764,  3768,  3773,  3777,
    3786,  3791,  3800,  3801,  3802,  3803,  3804,  3805,  3806,  3807,
    3808,  3809,  3810,  3811,  3812,  3813,  3814,  3815,  3816,  3817,
    3818,  3819,  3820,  3821,  3822,  3826,  3827,  3828,  3829,  3830,
    3831,  3834,  3838,  3842,  3846,  3850,  3857,  3861,  3865,  3869,
    3877,  3878,  3879,  3880,  3881,  3882,  3883
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
  "TPROTOTYPE", "TPUBLIC", "TPROCLP", "TREAL", "TRECORD", "TREDUCE",
  "TREF", "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSHARED",
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
  "TLESS", "TLESSEQUAL", "TMINUS", "TMOD", "TNOTEQUAL", "TOR", "TPLUS",
  "TQUESTION", "TSEMI", "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP",
  "TLCBR", "TRCBR", "TLP", "TRP", "TLSBR", "TRSBR", "TNOELSE",
  "TDOTDOTOPENHIGH", "TUPLUS", "TUMINUS", "TLNOT", "$accept", "program",
  "toplevel_stmt_ls", "toplevel_stmt", "pragma_ls", "stmt", "tryable_stmt",
  "deprecated_decl_stmt", "$@1", "$@2", "decl_base", "unstable_decl_stmt",
  "$@3", "$@4", "module_decl_start", "module_decl_stmt",
  "opt_access_control", "opt_prototype", "include_module_stmt",
  "block_stmt_body", "block_stmt", "stmt_ls", "renames_ls",
  "use_renames_ls", "opt_only_ls", "use_stmt", "import_stmt",
  "import_expr", "import_ls", "require_stmt", "assignment_stmt",
  "opt_label_ident", "reserved_word_ident", "ident_def", "ident_use",
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
  "linkage_spec", "opt_fn_type_formal_ls", "fn_type_formal_ls",
  "fn_type_formal", "opt_fn_type_ret_type", "fn_type", "fn_expr", "$@20",
  "$@21", "fn_decl_stmt_complete", "fn_decl_stmt", "$@22",
  "fn_decl_stmt_inner", "fn_decl_stmt_start", "fn_decl_receiver_expr",
  "fn_ident", "op_ident", "assignop_ident", "all_op_name",
  "formal_var_arg_expr", "opt_formal_ls", "req_formal_ls",
  "formal_ls_inner", "formal_ls", "formal", "named_formal",
  "opt_formal_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_iter_or_op", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "opt_lifetime_where", "lifetime_components_expr", "lifetime_expr",
  "lifetime_ident", "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner_ls", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt", "$@23",
  "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "ret_array_type", "ret_type", "colon_ret_type",
  "opt_ret_type", "opt_type", "array_type", "opt_formal_array_elt_type",
  "formal_array_type", "formal_type", "colon_formal_type",
  "opt_colon_formal_type", "expr_ls", "simple_expr_ls", "tuple_component",
  "tuple_expr_ls", "opt_actual_ls", "actual_ls", "actual_expr",
  "ident_expr", "type_level_expr", "sub_type_level_expr", "for_expr",
  "cond_expr", "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "task_var_prefix",
  "new_maybe_decorated", "new_expr", "let_expr", "range_literal_expr",
  "cast_expr", "tuple_expand_expr", "super_expr", "expr", "opt_expr",
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

#define YYPACT_NINF (-1039)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-757)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1039,   128,  3747, -1039,    33,    94, -1039, -1039, -1039, -1039,
   -1039, -1039, 12337,   189,   313,   267, 15219,   358, 20533,   189,
   12337,   397,   353,   379,   313,  5155, 12337,   474,  5155,   320,
   20592, -1039,   479,   445,  9189, 10589, 10589, -1039,  9363,   481,
     288,   352, -1039,   505, 20592, 20592, 20592, -1039,  3028, 10763,
     514, 12337, 12337,   394, -1039,   517,   527, 12337, -1039, 15219,
   -1039, 12337,   601,   472,   413,   431,  3466,   594, 20651, -1039,
   10939,  8665, 12337, 10763, 15219, 12337,   552,   610,   496,  5155,
     619, 12337,   623,  7615,  7615, -1039,   631, -1039, 15219,   682,
   -1039,   639,  9363,  9537, -1039, 12337, -1039, 12337, -1039, -1039,
   14731, 12337, -1039, 12337, -1039, -1039, -1039,  4099,  7791,  9713,
   12337, -1039,  4979, -1039, -1039, -1039, -1039, -1039,   533, -1039,
     129, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039,
   -1039, -1039, -1039, -1039,  7965, -1039, 11113, -1039, -1039, -1039,
   -1039, -1039,   652, -1039, -1039, -1039, -1039, -1039, -1039, -1039,
   -1039,   568, 20592, -1039, 20296,   544,   378, -1039,   209, -1039,
   -1039, -1039,   240,   291, -1039, 20592,  3028, -1039,   553, -1039,
     555, -1039, -1039,   557,   560, 12337,   564,   565, -1039, -1039,
   -1039, 19706,  1987,   340,   570,   586, -1039, -1039,   411, -1039,
   -1039, -1039, -1039, -1039,   317, -1039, -1039, -1039, 12337, 12337,
   12337, 20592, -1039, 12337, 10939, 10939,   662,   458, -1039, -1039,
   -1039, -1039, 19706,   465, -1039, -1039,   561,  5155, -1039, -1039,
     590,   219,   588, 17094, -1039,   210, -1039,   596, -1039,   -27,
   19706, -1039,  3164,   642,  8841, -1039, -1039, -1039, -1039, -1039,
   -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039,
   -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039,
     589, -1039, -1039, 20592,   592,    72, 16673,    32,  3278,    32,
   16588, 20592, 20592,   -16, 15868, -1039, -1039,   688,  8841,   597,
     233,  5155,  1917,    53,    18, -1039,  5155, -1039, -1039, 17179,
   -1039,    60, 17745,  1282, -1039,   602,   608, -1039, 17179,   219,
    1282, -1039,  8841,  2432,  2432, -1039,   157,     8, -1039,    54,
   -1039, -1039, -1039, -1039, 12337, 12337, -1039,  9889,   685, -1039,
     611,   634, -1039, -1039, 15679,   640, -1039, -1039, -1039,   251,
   -1039,   162, 19706, 12337, 12337, -1039,   -31, -1039, -1039, 19706,
     620, 17785, -1039, 17179,   219, 19706,   615,  8841, -1039, 19706,
   17831, -1039, -1039, -1039, -1039,   219, -1039,  3164,    72, 17179,
     625,   242,   242,   803,  1282,  1282,   192, -1039, -1039,  4275,
     238, -1039, 12337, -1039,   -23,   -13, -1039,   -44,    31, 17939,
     -19,   803,   784, -1039,  4451, 12337, -1039, 12337,   727, -1039,
   -1039, 16765,   168,   568, 19706,   303, 20592, 10939,   633, -1039,
     630,   801,   805,   684,   311, -1039, -1039, -1039,    55,   240,
   -1039, -1039, -1039, -1039,   723, -1039, -1039, -1039,    69,   724,
   -1039, -1039, -1039, 14562,   689,   375, -1039,   386,   511, -1039,
     667, 12337, 12337, 12337, 10589, 10589, 12337,   523, 12337, 12337,
   12337, 12337, 12337,   468, 14731, 12337, 12337, 12337, 12337, 12337,
   12337, 12337, 12337, 12337, 12337, 12337, 12337, 12337, 12337, 12337,
   12337,   750, -1039, -1039, -1039, -1039, -1039,  9537,  9537, -1039,
   -1039, -1039, -1039,  9537, -1039, -1039,  9537,  9537,  8841,  8841,
   10589, 10589, 17255, 17340, 17984,   647,    85, 20592,  8491, -1039,
   -1039,  4627, -1039, 10589,    72,  3028, -1039, 12337, -1039,  3164,
   -1039, 20592, -1039, -1039, -1039,   688, 12337,   703, -1039,   653,
     686, -1039, -1039, -1039,   785, 10939, -1039,  5331, 10589, -1039,
     659, -1039,    72,  5507, 10589, -1039,    72, -1039,    72, 10589,
   -1039,    72,   710,   711,  5155,   804,  5155, -1039,   811, 12337,
     781,   675,  8841, 20592, -1039, -1039, -1039,  1917, -1039, -1039,
     -10, -1039, 12511,   725, 12337,  3028, -1039, -1039, 12337, -1039,
   20372, 12337, 12337, -1039,   676, -1039, -1039, -1039, -1039, 11289,
      93, 20592, -1039,   240, 14426,  1917,   706, 11465, 10939, -1039,
   19706, 19706, -1039,   588, -1039,    51, -1039,  8841,   680, -1039,
     834,   834,  3164, -1039, -1039, -1039, -1039, -1039, -1039, -1039,
   10065, -1039, 18024,  8141, -1039,  8317, -1039,  5155,   683, 10589,
   10241,  3923,   690, 12337, 11639, -1039, -1039,   392, -1039,  4803,
   -1039,   390, 18100,   439, 16857, 20592,  7441,  7441, -1039,   568,
     694, -1039,   210, -1039,   140,   712,  1361, -1039, -1039, 20592,
   -1039, 20592, 12337,    -1, -1039, -1039,   102, -1039, -1039,    91,
   -1039, -1039,  7267, -1039,   755, -1039,   696,   728, -1039,   731,
   -1039,   732,   733,   734,   735, -1039,   739,   741,   742,   746,
     748,   749,   751, -1039, -1039, -1039, -1039, -1039, -1039, -1039,
   -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039,
   -1039, -1039, -1039, -1039, -1039, 12337,   758,   753,   696, -1039,
     696, -1039, 12685, -1039, 20592, -1039, -1039, 19983, 19983,   547,
   -1039,   547, -1039,   547, 20099,  1299,  1096,   198,   219,   242,
   -1039,   530, -1039, -1039, -1039, -1039, -1039,   803,   773,   547,
     513,   513, 19983,   513,   513,   248,   242,   773, 20059,   248,
    1282,  1282,   242,   803,   707,   709,   722,   744,   745,   747,
     740,   701, -1039,   547, -1039,   547, 12859, 10589, 13033, 10589,
   12337,  8841, 10589, 15503,   752,   191, -1039, -1039, -1039,   117,
   -1039,  1991, 19746,   495,   189, 18144, -1039,   442, 19706, -1039,
   18189,  8841, -1039,  8841, 12337,   120,  9363, 19706,    73, 17416,
    8491, -1039,  9363, 19706,    38, 16933, -1039, -1039,    32, 17018,
   -1039, 12337, 12337,   870,  5155,   877, 18298,  5155, 17501, 20592,
   -1039,   124, -1039,   159, -1039,   181,  1917,    53,  7091, -1039,
   -1039,   558,  9015, -1039, 19706, -1039, -1039, -1039, 20592, 18342,
   18381, -1039, -1039, -1039, -1039,   362, 10589, 14904,   706,   779,
   -1039,   182, 11465, -1039,    -7, -1039, -1039, 19706, 12337, -1039,
     764,    40,   756, -1039, 15591, -1039, -1039, -1039, -1039, -1039,
   -1039, -1039, -1039, -1039,  5155,    -3, 17577, -1039, -1039, 19706,
    5155, 19706, -1039, 18462, -1039, -1039, 12337, -1039,    74,  3565,
   12337, -1039, 11813,  7441,  7441, -1039, 12337,   506, 12337,   535,
     571, 12337, 10415,   582,   444, -1039, -1039, -1039, 18545,   791,
     777,   783, -1039, 20592,  3028, -1039,  8841,   786, 19890, 20592,
   -1039, 20592, -1039, 19706, 20474,   840,   939, -1039,    23, -1039,
    6915, -1039, -1039, -1039,   461, -1039,    23,   194, -1039, 18625,
   -1039, 15784, -1039, -1039, -1039,   446, -1039,   782,   789, -1039,
   -1039, -1039, -1039, -1039, -1039, -1039, -1039, 12337,   926, 18706,
   12337,   941, 18786,   214,   806, 18826,  8841, -1039, -1039,  8491,
   -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039,
   -1039, -1039,    50, 10589, 10589,    72,    72, -1039, -1039, -1039,
   -1039, -1039, 17179, -1039, 15944,  5683, -1039,  5859, -1039,   231,
   -1039, 16029,  6035, -1039,    72,  6211, -1039,    72, -1039,    72,
   -1039,    72, 19706, 19706,  5155, -1039,  5155, 12337, -1039,  5155,
     942,   813,   818, 20592,   588, -1039, -1039,   725,   809,   101,
   -1039, -1039, -1039,   815,   816, 18866, -1039, -1039,    -7, 12337,
     755,   706, -1039, 10939, -1039, -1039, 19706, -1039,  6387, 10939,
   -1039, -1039, -1039, 20592, -1039,   843,   588, -1039,  6563,   820,
    6739,   821, -1039, 12337, -1039, -1039,  7441, -1039, 18906,    71,
   17669,   447,   826,  7441, -1039, 12337, -1039, -1039, 19913,   469,
     488,   283, -1039,   986, -1039, -1039,   991, -1039, 20243, -1039,
   -1039, -1039, -1039, -1039, -1039, -1039, -1039,    65, 12337, -1039,
     824, 14251,   108, -1039,   853,   828, -1039,   696,   696, -1039,
   -1039, 18986, 13207, 13381, 19066, 13555, 13729, -1039, 13903, 14077,
     301, -1039,   725,   310,   359, -1039, -1039, -1039,  5155,  9363,
   19706,  9363, 19706,  8491, -1039,  5155,  9363, 19706, -1039,  9363,
   19706, -1039, -1039, -1039, -1039, -1039, 19706,   969,  5155, -1039,
   -1039, -1039, -1039, -1039, -1039, 10589, 15361,  8841,  8841, 11989,
   -1039, -1039,   219, -1039,    -7, -1039, 19706,  5155, -1039,    80,
     831, 12337, -1039,  9363, -1039, 19706,  5155,  9363, -1039, 19706,
    5155, 19706,   332, 12163,  7441,  7441,  7441,  7441, -1039, -1039,
   -1039, 19106, 19706, 20007, -1039, -1039, -1039,   841, -1039,  1533,
   -1039,  1533, -1039, -1039, -1039,   106, -1039,   139, -1039,   421,
   19146,    11, -1039, -1039,   736, 15077,    81, -1039, -1039, -1039,
   12337, 12337, 12337, 12337, 12337, 12337, 12337, 12337, -1039, -1039,
   18298, 16105, 16190, -1039, 18298, 16266, 16351,  5155, -1039, 19186,
   -1039,   674,   832,   835, -1039, -1039, -1039,   642, -1039,   588,
   19706, 16427, -1039, 16512, -1039, -1039, -1039, 19706,   491,   842,
     504,   844, 12337, -1039, -1039,  1533, -1039,  1533, -1039, -1039,
   12337,    65,   106,   106,   106,   106,   106,   106,    65, -1039,
   -1039, -1039, -1039,   738, -1039, -1039, -1039, 19266, 19346, 19426,
   19506, 19586, 19666,  5155,  5155,  5155,  5155, -1039, 14077,   849,
     851, -1039,  5155,  5155, -1039, -1039, -1039, -1039, 19706, -1039,
   -1039, 19706, -1039, -1039, -1039, -1039, -1039, -1039, -1039,   871,
   12337, 12337, 12337, 12337, 12337, 12337, 18298, 18298, 18298, 18298,
     950, -1039, -1039, 18298, 18298
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   115,   699,   700,   701,   695,
     696,   702,     0,   616,   106,   136,   586,   143,   588,   616,
       0,   142,   488,   486,   106,     0,     0,    47,     0,   266,
     137,   184,   183,   693,     0,     0,     0,   182,     0,   141,
      59,   267,   338,   138,     0,     0,     0,   333,     0,     0,
     145,     0,     0,   633,   608,   703,   146,     0,   339,   580,
     483,     0,     0,     0,   169,   167,   425,   140,   589,   485,
       0,     0,     0,     0,   584,     0,     0,   144,     0,     0,
     116,     0,   694,     0,     0,   475,   139,   304,   582,    53,
     487,   147,     0,     0,   752,     0,   754,     0,   755,   756,
     649,     0,   753,   750,   566,   165,   751,     0,     0,     0,
       0,     4,     0,     5,     9,    10,    44,    11,     0,    49,
      62,    12,    68,    13,    14,    15,    16,    28,   562,   563,
      22,    50,   166,   176,     0,   185,   666,   177,    17,    30,
      29,    19,     0,   261,    18,   662,    21,    34,    31,    32,
     175,   305,     0,   173,     0,     0,   659,   337,     0,   567,
     171,   354,   445,   436,   174,     0,     0,   172,   671,   656,
     564,   655,   570,   568,     0,     0,   660,   661,   665,   664,
     663,     0,   569,     0,   672,   673,   674,   697,   698,   654,
     572,   571,   657,   658,     0,    27,   588,   137,     0,     0,
       0,     0,   589,     0,     0,     0,     0,   659,   671,   568,
     660,   661,   578,   569,   672,   673,     0,     0,   617,   107,
       0,   587,     0,   616,   476,     0,   484,     0,    20,     0,
     547,    45,   336,     0,   554,   113,   117,   129,   123,   122,
     131,   110,   121,   132,   118,   133,   108,   134,   127,   120,
     128,   126,   124,   125,   109,   111,   119,   130,   135,   114,
       0,   112,   193,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    60,    62,   554,     0,
       0,     0,     0,     0,     0,   491,     0,   153,    39,     0,
     296,     0,   295,   737,   637,   634,   635,   636,     0,   581,
     738,     7,   554,   336,   336,   457,   432,   150,   427,   151,
     428,   429,   433,   434,   149,   148,   435,     0,   425,   538,
       0,   341,   342,   344,     0,   426,   537,   539,   346,   656,
     544,     0,   543,     0,     0,   154,   567,   652,   670,   653,
       0,     0,    42,     0,   585,   573,     0,   554,    43,   579,
       0,   274,   278,   275,   278,   583,    51,   336,     0,     0,
       0,   739,   741,   647,   736,   735,     0,    65,    69,     0,
       0,   549,     0,   551,     0,     0,   550,     0,     0,   543,
       0,   648,     0,     6,     0,     0,    63,     0,     0,   488,
     181,     0,   695,   305,   667,   189,     0,     0,     0,   301,
       0,   325,   329,   331,     0,   315,   319,   322,   425,   445,
     443,   444,   442,   362,   446,   449,   448,   450,     0,   440,
     437,   438,   441,     0,   480,     0,   477,     0,   565,    33,
     638,     0,     0,     0,     0,     0,     0,   740,     0,     0,
       0,     0,     0,     0,   646,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   393,   400,   401,   402,   397,   399,     0,     0,   395,
     398,   396,   394,     0,   404,   403,     0,     0,   554,   554,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    35,
      23,     0,    36,     0,     0,     0,    24,     0,    37,   336,
     302,     0,   183,   303,    48,    62,     0,   562,   560,     0,
     555,   556,   561,   314,     0,     0,   203,     0,     0,   202,
       0,   211,     0,     0,     0,   209,     0,   219,     0,     0,
     217,     0,     0,     0,     0,   235,     0,   393,   231,     0,
       0,     0,     0,     0,   245,    25,   495,     0,   496,   498,
       0,   523,     0,   501,     0,     0,   152,    38,     0,   298,
       0,     0,     0,    40,     0,   170,   168,   430,   431,   528,
       0,     0,   426,   445,   425,     0,   541,     0,     0,    99,
     668,   669,   352,     0,   155,     0,    41,     0,     0,   285,
     276,   277,   336,    54,   196,   195,    26,    67,    66,    70,
       0,   704,     0,     0,   689,     0,   691,     0,     0,     0,
       0,     0,     0,     0,     0,   708,     8,     0,    56,     0,
      97,     0,    93,     0,    77,     0,     0,    83,   178,   305,
       0,   191,     0,   188,   272,   306,     0,   313,   323,     0,
     327,     0,     0,   317,   312,   412,     0,   414,   418,     0,
     447,   516,     0,   518,   451,   439,   410,   113,   389,   117,
     387,   123,   122,   121,   118,   391,   133,   134,   120,   124,
     109,   119,   135,   386,   368,   371,   369,   370,   392,   381,
     372,   385,   377,   375,   388,   376,   374,   379,   384,   373,
     378,   382,   383,   380,   390,     0,   365,     0,   410,   366,
     410,   363,     0,   479,     0,   474,   490,   732,   731,   734,
     743,   742,   747,   746,   728,   725,   726,   727,   650,   715,
     115,     0,   685,   686,   116,   684,   683,   644,   719,   730,
     724,   722,   733,   723,   721,   713,   718,   720,   729,   712,
     716,   717,   714,   645,     0,     0,     0,     0,     0,     0,
       0,     0,   745,   744,   749,   748,     0,     0,     0,     0,
       0,     0,     0,   706,   272,   627,   628,   630,   632,     0,
     619,     0,     0,     0,   616,   616,   199,     0,   548,    46,
       0,     0,   575,     0,     0,     0,     0,   592,     0,     0,
       0,   212,     0,   598,     0,     0,   210,   220,     0,     0,
     218,     0,     0,   234,     0,   230,     0,     0,     0,     0,
     574,     0,   248,     0,   246,     0,   499,     0,     0,   522,
     521,   656,     0,   493,   643,   492,   297,   294,     0,     0,
       0,   682,   530,   531,   533,   656,     0,   706,   541,   347,
     343,     0,     0,   542,   501,   345,   546,   545,     0,   351,
       0,     0,     0,   576,     0,   279,    52,   705,   651,   690,
     552,   692,   553,   227,     0,     0,     0,   707,   225,   602,
       0,   710,   709,     0,    58,    57,     0,    92,     0,     0,
       0,    85,     0,     0,    83,    55,   386,   368,   371,   369,
     370,   379,   378,   380,     0,   405,   406,    72,    71,    84,
       0,     0,   307,     0,     0,   269,     0,     0,   336,     0,
     326,     0,   330,   332,     0,     0,   425,   413,   458,   514,
       0,   513,   512,   515,   656,   452,   458,   425,   361,     0,
     367,     0,   357,   358,   482,   656,   478,     0,     0,   105,
     103,   104,   102,   101,   100,   680,   681,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   629,   631,     0,
     618,   136,   143,   142,   141,   138,   145,   146,   140,   144,
     139,   147,     0,     0,     0,     0,     0,   489,   194,   558,
     559,   557,     0,   229,     0,     0,   201,     0,   200,     0,
     622,     0,     0,   207,     0,     0,   205,     0,   215,     0,
     213,     0,   243,   242,     0,   237,     0,     0,   233,     0,
     239,     0,   271,     0,     0,   497,   500,   501,     0,     0,
     502,   503,   293,     0,     0,     0,   532,   534,   501,     0,
     451,   541,   540,   407,   421,   419,   353,   287,     0,     0,
     286,   289,   577,     0,   280,   283,     0,   228,     0,     0,
       0,     0,   226,     0,    98,    95,     0,    94,    80,    79,
      78,     0,     0,     0,   179,     0,   180,   307,   336,     0,
       0,     0,   300,   159,   169,   167,   163,   299,   336,   308,
     309,   156,   324,   328,   318,   321,   415,     0,     0,   334,
       0,   505,     0,   355,   417,     0,   364,   410,   410,   687,
     688,     0,     0,     0,     0,     0,     0,   271,     0,     0,
       0,   620,   501,   671,   671,   198,   197,   204,     0,     0,
     591,     0,   590,     0,   621,     0,     0,   597,   208,     0,
     596,   206,   216,   214,   236,   232,   607,   238,     0,    64,
     268,   247,   244,   494,   527,     0,   706,   554,   554,   528,
     422,   420,   348,   349,   501,   409,   408,     0,   291,     0,
       0,     0,   281,     0,   223,   604,     0,     0,   221,   603,
       0,   711,     0,     0,     0,    83,     0,    83,    86,    89,
      75,    74,    73,   336,   186,   192,   190,   273,   157,   336,
     161,   336,   310,   311,   472,     0,   473,   460,   463,     0,
     459,     0,   510,   508,   656,   706,     0,   411,   359,   360,
       0,     0,     0,     0,     0,     0,     0,     0,   273,   624,
     595,     0,     0,   623,   601,     0,     0,     0,   241,     0,
     525,   656,     0,     0,   535,   424,   423,   292,   290,     0,
     284,     0,   224,     0,   222,    96,    82,    81,     0,     0,
       0,     0,     0,   187,   270,   336,   160,   336,   164,   471,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   455,
     456,   335,   509,   656,   453,   356,   454,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   240,     0,   639,
     640,   282,     0,     0,    88,    91,    87,    90,    76,   158,
     162,   462,   464,   465,   468,   469,   470,   466,   467,   461,
       0,     0,     0,     0,     0,     0,   594,   593,   600,   599,
     656,   641,   642,   606,   605
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1039, -1039, -1039,     7,   -65,  2149,   476, -1039, -1039, -1039,
    -220, -1039, -1039, -1039, -1039, -1039,   -37,   754, -1039,  -578,
    2380,   638,  -605, -1039,  -851, -1039, -1039,   147, -1039, -1039,
   -1039,  1000, -1039,  2837,  -179, -1039, -1039,  -480,  2484,  -948,
     -53, -1039, -1039,   -52, -1039, -1039,  -874, -1039, -1039,   407,
   -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039,    39,
   -1039,   937, -1039, -1039,    20,  1803, -1039, -1039, -1039, -1039,
     678, -1039,    -9, -1039, -1039, -1039, -1039,   477, -1039, -1039,
   -1039,  -106, -1039,  -358,  -852, -1039, -1039, -1039, -1039, -1039,
     123, -1039, -1039,   125, -1039, -1039,  -597, -1039,   609, -1039,
   -1039, -1039, -1039, -1039,   471, -1039,  2446, -1039, -1039, -1039,
   -1039,   644, -1039, -1039, -1039, -1039,   111,  -410,  -163,  -831,
    -971,  -598, -1039,   119, -1039,   131,   -48,   730,   -46, -1039,
   -1039,  -353,    22, -1039,  -156,  1311,   130,  -213,  -200,  -524,
   -1039, -1039,   161,   361, -1039,  -214,   932, -1039,  -151,   512,
     252,  -222,  -815, -1038, -1039, -1039, -1039,  -621,  -692,  -812,
    -542,  -546, -1039,  -795,   -63, -1039,   104, -1039,  -261,  -506,
    -549,   537,   -34, -1039, -1039, -1039,   700, -1039,   -17, -1039,
   -1039,  -152, -1039,  -774, -1039, -1039,  1205,  1242, -1039, -1039,
   -1039, -1039,   -12, -1039,    19,  1267, -1039,  1577,  1772, -1039,
   -1039, -1039, -1039, -1039, -1039, -1039, -1039, -1039,  -482
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   368,   112,   805,   114,   115,   499,   232,
     116,   117,   592,   357,   118,   119,   120,   388,   121,   122,
     123,   369,   899,   623,   900,   124,   125,   620,   621,   126,
     127,   220,   259,   403,   128,   261,   129,   319,   288,   130,
    1079,  1255,  1189,  1080,  1257,  1191,   131,   304,   303,   132,
     133,   134,   135,   136,   137,   632,   903,   138,   139,   954,
     140,   273,   141,   813,   814,   206,   143,   144,   145,   146,
     590,   855,  1046,   147,   148,   851,  1041,   290,   291,   149,
     150,   151,   152,   398,   908,   153,   154,   404,   914,   915,
     405,   909,   639,   406,   911,   641,   407,   155,   207,  1201,
     157,   158,   320,   321,   322,  1030,   159,   337,   583,   848,
     160,   161,  1206,   162,   163,   697,   698,   895,   896,   897,
    1034,   928,   409,   646,  1095,   647,   648,   324,   572,   423,
     413,   418,   926,  1275,  1271,   508,  1089,  1197,  1198,  1199,
     164,   165,   425,   426,   703,   166,   167,   225,   284,   285,
     549,   550,   823,   922,   923,   653,   654,   553,   820,   833,
     327,   328,   843,   844,   370,   229,   374,   375,   509,   510,
     511,   208,   169,   170,   171,   172,   209,   174,   217,   218,
     769,   522,   989,   770,   771,   175,   210,   211,   178,   179,
     180,   338,   181,   395,   512,   213,   183,   214,   215,   186,
     187,   188,   189,   380,   190,   191,   192,   193,   194
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     212,   318,   222,   277,   221,   849,   773,   331,   223,   111,
     934,   495,   504,   699,   230,   427,   990,   541,   323,   477,
     325,   894,   266,   268,   270,  1026,   274,   834,   918,  1035,
     393,   845,   329,  1062,  1081,   630,   811,   289,   852,   292,
     293,   564,   910,  1028,   912,   298,   378,   299,  1057,   300,
    1068,   551,   850,  1203,   551,   507,   649,  1150,   332,   339,
     341,   343,   344,   345,  -266,   286,   554,   349,  1194,   350,
     651,   992,   819,   265,   267,   269,   355,   720,   520,   609,
     359,   339,   306,   361,  1087,   362,   588,   534,   363,   364,
     340,   365,   551,   286,   582,   332,   339,   379,   381,   507,
     932,  -519,   933,   308,    71,   286,   985,   310,  1174,  1194,
    -267,   539,   360,   286,  -320,  1038,   526,   528,   531,   520,
     822,   497,   391,   507,   394,   603,  -288,   376,     3,   614,
    -517,   311,   607,   762,   312,   605,    62,   593,   816,   498,
    -350,   836,   486,  1088,  1175,  1033,   313,  -316,   377,  1145,
    -249,   604,  -519,   520,   520,   491,   762,   615,  1195,  1039,
     520,   606,  -517,   430,   817,   316,   555,  1272,   507,  1196,
    -288,  -316,   921,  1048,    71,  -517,   385,  -519,   724,   610,
    -519,   234,   491,  1235,  -519,  1111,   482,   483,   484,  -517,
     500,   349,   332,   379,  1081,   505,  1017,   552,  -519,   195,
     552,  -519,  1143,   491,  1081,   567,   494,   611,   558,   491,
    1196,  -519,  1040,  1151,   386,  1183,   652,   750,   751,  -519,
     839,   306,   339,  -288,  -519,   608,   612,   278,   578,   645,
     433,   491,  1180,   610,   981,  -517,  1154,    23,   552,   957,
    -517,   610,   308,   491,   491,   387,   310,  1274,   568,   610,
     916,   491,   491,  1270,   570,   953,   610,   503,  1270,  1260,
     700,   763,  -519,   410,   726,   959,   339,   414,   578,   837,
     311,   901,   783,   312,   433,    62,   917,  1146,  1061,   779,
     433,    87,   958,   411,  1205,   313,   629,  1261,   434,    60,
     339,   960,   435,   412,   983,  1027,  1032,  1219,  1012,   507,
     507,    69,   345,   349,   316,   379,   905,  1013,   773,  1081,
     578,   216,  1082,   906,  1083,  1256,   219,  1258,   419,   415,
     505,   580,   581,  -250,  1249,   815,  1251,    90,   579,   816,
     816,   416,   434,  1014,   635,   339,   435,  1234,   434,  1236,
     437,   438,   435,   318,   777,   441,   828,   442,   443,  1223,
     417,  1112,   446,   841,    42,  1015,  1031,  -336,   195,   453,
     602,   437,   783,   507,   597,   457,   458,   459,  -416,   443,
     420,   275,   856,   622,   276,   624,   599,  -336,  -536,  1123,
      58,  1299,   421,  1300,   437,   332,   600,  -336,  1107,   441,
     437,   438,   443,   439,   440,   441,   446,   442,   443,  -536,
    1071,   422,   446,  -536,   491,  1124,   543,   480,   507,   453,
     601,   481,   294,   631,  -256,   457,   458,   459,   904,   707,
     708,   709,   711,   713,   714,  -536,   715,   716,   717,   718,
     719,   783,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,   737,   738,   739,   740,   741,   742,   743,   783,
    1110,  1172,   785,  -255,  1230,   339,   339,  1187,  -626,   643,
     -61,   339,   505,   224,   339,   339,   339,   339,   753,   755,
     226,   720,   295,   710,   712,  1218,   772,   773,   -60,   -61,
    1063,   775,   231,   644,  -626,   778,   745,   746,   721,  -529,
     262,   296,   747,   234,   780,   748,   749,   -60,   -61,  1208,
    1209,  -264,   722,   332,  1245,   787,   789,  -625,   297,   318,
    -529,   793,   795,   478,  -529,   479,   -60,   799,   821,   752,
     754,   699,  -511,   704,   806,   278,   323,   808,   325,   -61,
     339,   723,   774,  -625,   555,   835,  -529,  -254,   876,   168,
     329,   705,   824,   329,  -613,   433,   292,   -60,  1262,   829,
     830,  -678,   706,  -678,  -511,   505,   877,   788,   195,   351,
     353,  -251,   168,   794,   874,   168,   847,  -511,   798,  1248,
    -258,  1250,   724,  -262,  1263,   339,  1264,  1265,   725,  1266,
    1267,  -511,   507,  -259,  -679,   974,  -679,   880,   847,   481,
     555,   339,  1063,   339,  -481,  1063,  -754,   866,   847,   869,
    -754,   871,   873,   434,   507,   881,  -520,   435,   977,   301,
    1064,   156,  -481,  1178,   898,   898,   168,   704,   924,  -520,
     168,   168,   376,  1269,   376,  -755,   599,  -511,  1269,  -755,
     913,  -678,  -511,  -678,   156,  1185,   555,   156,  -675,  1063,
    -675,   773,   994,   997,   168,   302,   999,  1001,   865,   168,
    -253,  -520,  1063,   346,  1186,   437,   438,  1294,   439,   440,
     441,  -756,   442,   443,   444,  -756,  -257,   446,   935,   347,
    1296,  1259,  -751,   452,   453,  -263,  -751,   456,  -520,  -265,
     457,   458,   459,   929,  -677,  -520,  -677,  -252,   156,   437,
     356,   460,   156,   156,   441,  -260,  -676,   443,  -676,  1055,
     726,   446,   173,   937,   384,   938,  -520,   860,   396,   862,
     565,   566,   306,  1049,  1051,   397,   156,   408,   487,  -610,
     428,   156,  -524,  -609,  -520,   173,   429,   507,   173,  -520,
    -614,  -615,  -520,   308,   488,  -524,  -612,   310,  1303,  1304,
    1305,  1306,  1307,  1308,   787,   949,   793,   952,   806,   339,
     955,   869,  -611,  1070,   168,  1019,   490,   975,   976,   491,
     513,   311,   496,   506,   312,   515,   382,  -524,  1098,   339,
     542,   339,   982,   386,   984,   561,   313,   507,   772,   173,
     991,   562,   574,   173,   173,   573,   584,   577,   587,  1002,
    1003,   596,   616,   625,  -524,   316,   948,  -504,   951,  -506,
     980,  -524,   637,   835,   636,   433,   379,   173,   329,   638,
     339,   642,   173,   640,   650,   655,   702,   443,   168,   744,
     761,   539,  -524,   168,  1025,   869,   156,   782,   539,  -504,
     781,  -506,   790,   784,   783,   433,  1036,   801,   802,   804,
    -524,  1021,  -504,  1078,  -506,  -524,   807,   809,  -524,   810,
     831,   318,   822,   842,   853,   854,  -504,  1092,  -506,   864,
     578,   925,   318,   434,   622,   902,   870,   435,  1058,   927,
    1060,   898,   898,   939,   361,   940,   362,   946,  -115,   364,
     365,  -136,  -143,  -142,  -141,  -138,  1232,  1233,   941,  -145,
     156,  -146,  -140,   434,   339,   156,  -144,   435,  -116,  -139,
     930,  -147,  -504,   931,  -506,  1004,   168,  -504,   379,  -506,
     942,   943,  1006,   944,   945,   437,   438,   173,   439,   440,
     441,   168,   442,   443,   444,   956,  1029,   446,   447,   448,
    1042,   450,   451,   452,   453,  1101,  1037,   456,  1104,  1063,
     457,   458,   459,  1066,   339,   437,   438,   772,   439,   440,
     441,   460,   442,   443,  1067,   402,  1099,   446,  1072,  1102,
     701,   711,   753,   452,   453,  1100,   306,   456,   507,   507,
     457,   458,   459,  1120,  1105,  1122,  1159,  1138,   156,  1139,
    1127,   173,  1108,  1130,  1140,  1144,   173,   308,  1147,  1148,
    1161,   310,  1179,   156,  1188,  1136,  1166,  1170,  -526,  1190,
    1202,   916,  1207,  1078,  1227,  1239,  1289,  1254,  1295,  1290,
    1297,  -526,   710,   752,  1321,   311,  1322,  1152,   312,  1261,
      62,  1156,   619,  1054,   227,  1192,  1193,   332,   168,   358,
     313,   540,   591,  1141,  1160,   826,  1165,  1084,  1169,   633,
    1085,  1171,  1097,  -526,   898,   840,  1094,  1086,   571,   316,
    1276,  1181,  1153,  1182,   168,  1309,  1093,  1204,   539,   539,
     168,  1302,   539,   539,  1069,   936,   390,   825,  1016,   173,
    -526,   168,     0,   168,     0,     0,  1200,  -526,   539,     0,
     539,     0,     0,     0,   173,     0,     0,     0,     0,     0,
    1120,  1122,     0,  1127,  1130,     0,  1165,  1169,  -526,     0,
     156,     0,     0,     0,     0,     0,  1220,  1221,     0,  1222,
       0,   772,  1231,  1224,  1225,   835,  -526,  1226,  1078,     0,
       0,  -526,     0,     0,  -526,     0,   156,     0,   433,     0,
       0,     0,   156,  1229,   869,   339,   339,  1169,     0,     0,
       0,     0,     0,   156,   168,   156,     0,     0,   168,  1240,
       0,  1241,     0,     0,     0,  1243,   168,     0,     0,     0,
       0,  1247,   898,   898,   898,   898,     0,     0,     0,     0,
       0,  1273,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,     0,     0,
     435,   173,     0,   869,     0,     0,     0,     0,  1220,  1277,
    1278,  1224,  1279,  1280,  1281,  1282,     0,   176,     0,     0,
       0,     0,     0,     0,     0,     0,   156,   173,     0,     0,
     156,     0,     0,   173,     0,     0,     0,     0,   156,     0,
     176,     0,     0,   176,   173,     0,   173,     0,   437,   438,
    1298,     0,   440,   441,   177,   442,   443,     0,  1301,     0,
     446,     0,     0,     0,  1320,     0,     0,   453,     0,     0,
       0,     0,     0,   457,   458,   459,     0,   177,     0,   182,
     177,  1316,  1317,  1318,  1319,     0,  1169,     0,     0,     0,
    1323,  1324,     0,     0,   176,     0,     0,     0,   176,   176,
       0,     0,   182,     0,     0,   182,     0,     0,  1316,  1317,
    1318,  1319,  1323,  1324,     0,     0,     0,   173,   972,     0,
       0,   173,   176,     0,   433,     0,     0,   176,     0,   173,
       0,   177,     0,     0,     0,   177,   177,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   168,     0,     0,   168,     0,   182,     0,     0,   177,
     182,   182,     0,     0,   177,     0,     0,     0,     0,     0,
       0,     0,   907,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,   182,     0,   435,   326,     0,   182,
       0,   330,     0,  -307,     0,     0,     0,  -307,  -307,   434,
       0,     0,  -307,   435,     0,     0,     0,  -307,     0,  -307,
    -307,   168,     0,     0,     0,  -307,     0,   168,     0,     0,
       0,     0,  -307,   156,     0,  -307,   156,     0,   330,   373,
     330,     0,   176,     0,   437,     0,     0,     0,     0,   441,
       0,   442,   443,     0,     0,  -307,   446,     0,  -307,     0,
    -307,   437,  -307,   453,  -307,  -307,   441,  -307,   442,   443,
    -307,   459,  -307,   446,     0,     0,     0,     0,     0,   177,
     453,     0,     0,     0,     0,     0,   457,   458,   459,     0,
       0,  -307,     0,   156,  -307,     0,  -307,     0,  -307,   156,
       0,     0,     0,     0,   182,     0,   176,     0,     0,     0,
       0,   176,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   173,     0,     0,   173,     0,     0,
    1113,  1114,     0,     0,     0,   330,   330,     0,     0,     0,
       0,     0,   168,   177,   168,     0,     0,  -307,   177,   168,
       0,     0,   168,  -307,     0,     0,     0,     0,     0,     0,
       0,   168,     0,   168,     0,     0,   168,     0,   182,     0,
       0,     0,     0,   182,     0,   500,     0,     0,     0,    22,
      23,     0,     0,     0,   173,     0,     0,     0,     0,   501,
     173,    31,   502,     0,   176,   168,     0,    37,     0,   184,
       0,     0,     0,     0,    42,   168,     0,   168,     0,   176,
       0,     0,     0,     0,   156,     0,   156,     0,     0,     0,
       0,   156,   184,     0,   156,   184,     0,     0,     0,     0,
      58,   177,    60,   156,     0,   156,  1074,     0,   156,  1075,
       0,     0,   503,     0,    69,     0,   177,     0,   330,     0,
       0,     0,     0,     0,     0,     0,   182,     0,     0,     0,
       0,     0,     0,    85,     0,     0,    87,   156,     0,     0,
      90,   182,     0,     0,     0,   168,   184,   156,     0,   156,
     184,   184,   168,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   168,     0,     0,     0,     0,
       0,     0,     0,     0,   184,   173,     0,   173,     0,   184,
       0,     0,   173,     0,   168,   173,   176,     0,     0,   105,
       0,     0,     0,   168,   173,     0,   173,   168,   330,   173,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   176,     0,     0,     0,     0,   156,   176,     0,
       0,     0,     0,   177,   156,     0,     0,     0,   173,   176,
       0,   176,     0,     0,     0,     0,     0,   156,   173,     0,
     173,     0,     0,     0,     0,     0,     0,     0,   182,   177,
       0,     0,     0,     0,   168,   177,   156,     0,     0,     0,
       0,     0,     0,     0,   185,   156,   177,     0,   177,   156,
       0,     0,     0,     0,   182,     0,     0,     0,     0,     0,
     182,     0,     0,     0,   184,     0,     0,   185,     0,     0,
     185,   182,     0,   182,     0,   142,     0,     0,     0,     0,
       0,     0,   176,     0,     0,     0,   176,     0,   173,     0,
     168,   168,   168,   168,   176,   173,   330,     0,   142,   168,
     168,   142,     0,     0,     0,     0,   156,     0,   173,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   177,
       0,   185,     0,   177,     0,   185,   185,   173,   184,     0,
       0,   177,     0,   184,     0,     0,   173,     0,     0,     0,
     173,     0,     0,     0,   182,     0,     0,     0,   182,   185,
     832,     0,   142,     0,   185,   326,   182,     0,   326,   846,
       0,     0,   156,   156,   156,   156,     0,     0,     0,     0,
       0,   156,   156,     0,     0,     0,     0,     0,     0,     0,
     142,   846,     0,     0,   373,   142,   373,     0,     0,     0,
     235,   846,     0,     0,     0,     0,     0,   173,     0,     0,
       0,     0,     0,     0,   236,   237,     0,   238,     0,     0,
       0,     0,   239,     0,     0,     0,   184,     0,     0,     0,
       0,   240,     0,     0,     0,     0,     0,   241,     0,     0,
       0,   184,     0,   242,     0,     0,     0,   243,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,   173,   173,   173,   173,   246,   247,   185,
       0,     0,   173,   173,   720,   248,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   249,     0,     0,   961,   176,
       0,   962,   176,     0,   250,   251,   963,   252,     0,   253,
     142,   254,     0,     0,   255,     0,     0,   197,   256,   546,
       0,   257,     0,     0,     0,   258,     0,   964,     0,     0,
       0,     0,     0,     0,   965,     0,   177,     0,     0,   177,
       0,     0,     0,   185,   966,     0,     0,     0,   185,     0,
       0,     0,   967,     0,     0,     0,     0,     0,   184,   176,
       0,   182,     0,     0,   182,   176,     0,     0,     0,   968,
       0,     0,     0,     0,   142,     0,     0,     0,     0,   142,
     547,   969,   979,     0,   184,   724,     0,     0,     0,     0,
     184,     0,   970,     0,     0,     0,   177,     0,     0,   971,
       0,   184,   177,   184,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   330,
       0,   182,     0,     0,     0,     0,     0,   182,     0,     0,
       0,   185,     0,     0,     0,     0,     0,     0,   832,     0,
       0,   113,     0,   326,     0,     0,   185,   476,     0,     0,
    -675,     0,  -675,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   142,     0,   228,     0,     0,   233,     0,     0,
       0,     0,     0,     0,   184,     0,     0,   142,   184,     0,
     176,     0,   176,     0,     0,     0,   184,   176,     0,     0,
     176,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,   176,     0,     0,   176,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   177,   348,   177,
       0,   330,     0,     0,   177,     0,     0,   177,     0,     0,
       0,     0,     0,   176,     0,     0,   177,     0,   177,     0,
       0,   177,   182,   176,   182,   176,   113,     0,     0,   182,
       0,   383,   182,   185,     0,     0,     0,     0,     0,     0,
       0,   182,     0,   182,     0,     0,   182,     0,     0,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,   185,
     177,     0,   177,     0,   142,   185,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   182,   185,     0,   185,     0,
       0,     0,     0,     0,     0,   182,     0,   182,     0,     0,
     142,     0,     0,   176,     0,     0,   142,     0,     0,     0,
     176,     0,     0,     0,     0,     0,     0,   142,     0,   142,
       0,     0,     0,   176,  1155,     0,     0,     0,     0,     0,
     330,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     177,     0,   176,     0,     0,     0,   489,   177,     0,     0,
       0,   176,     0,     0,     0,   176,     0,     0,     0,   185,
     177,   184,     0,   185,   184,   182,     0,     0,     0,     0,
       0,   185,   182,     0,     0,     0,     0,     0,     0,   177,
       0,     0,     0,     0,     0,   182,     0,     0,   177,     0,
     142,     0,   177,     0,   142,     0,     0,     0,     0,     0,
       0,     0,   142,     0,   182,     0,     0,     0,     0,   287,
     545,     0,   176,   182,     0,   556,     0,   182,     0,     0,
       0,   184,     0,     0,     0,     0,     0,   184,     0,     0,
       0,     0,     0,   287,   500,     0,     0,     0,    22,    23,
     832,     0,     0,   352,   354,     0,     0,     0,   501,   177,
      31,   502,     0,     0,     0,     0,    37,     0,     0,     0,
       0,     0,     0,    42,     0,     0,     0,     0,   176,   176,
     176,   176,     0,     0,   182,     0,     0,   176,   176,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,     0,     0,     0,     0,   336,   113,     0,
       0,   503,     0,    69,     0,   177,   177,   177,   177,     0,
       0,     0,     0,   113,   177,   177,     0,     0,     0,   336,
       0,     0,    85,     0,     0,    87,     0,     0,     0,    90,
     182,   182,   182,   182,   336,     0,     0,   342,     0,   182,
     182,     0,   184,     0,   184,     0,     0,     0,     0,   184,
       0,     0,   184,     0,     0,     0,   185,     0,     0,   185,
       0,   184,     0,   184,     0,     0,   184,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   492,     0,     0,     0,     0,   142,     0,     0,
     142,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,   184,     0,     0,
       0,     0,     0,     0,     0,     0,   185,     0,     0,     0,
     113,     0,   185,     0,     0,   287,   287,   287,   287,   287,
     287,     0,     0,   535,   538,     0,     0,     0,     0,     0,
     544,     0,     0,     0,     0,     0,   556,   142,     0,   287,
       0,   287,   556,   142,     0,     0,     0,     0,   287,     0,
     336,     0,     0,   803,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,     0,   184,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
       0,     0,     0,   287,   336,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,   287,   287,
       0,     0,     0,   184,     0,     0,     0,   184,   336,   516,
     519,   521,   525,   527,   530,     0,   863,   185,     0,   185,
     868,     0,     0,     0,   185,     0,     0,   185,   113,     0,
       0,     0,     0,   557,     0,   559,   185,     0,   185,     0,
       0,   185,   563,     0,     0,     0,     0,     0,   142,     0,
     142,     0,     0,   336,     0,   142,     0,     0,   142,     0,
       0,     0,     0,     0,   184,     0,     0,   142,     0,   142,
     185,     0,   142,     0,     0,     0,     0,     0,     0,     0,
     185,     0,   185,     0,     0,     0,     0,   586,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   142,   594,   595,     0,     0,     0,     0,     0,     0,
       0,   142,     0,   142,     0,     0,     0,     0,     0,     0,
     184,   184,   184,   184,     0,     0,     0,   260,     0,   184,
     184,     0,     0,     0,   287,     0,     0,     0,     0,     0,
       0,   279,   280,   281,     0,   283,     0,     0,     0,     0,
     185,     0,     0,     0,     0,     0,     0,   185,     0,     0,
       0,     0,   287,     0,     0,     0,   287,     0,   287,     0,
     185,   287,     0,   336,   336,     0,     0,     0,     0,   336,
       0,   142,   336,   336,   336,   336,     0,     0,   142,   185,
       0,     0,     0,     0,     0,     0,     0,     0,   185,     0,
       0,   142,   185,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1005,     0,     0,  1008,     0,     0,     0,
     142,     0,     0,     0,     0,     0,     0,     0,     0,   142,
       0,     0,     0,   142,     0,     0,     0,     0,   776,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   336,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   185,
       0,     0,   424,   283,     0,     0,   791,     0,     0,     0,
     796,     0,   797,  1047,     0,   800,     0,     0,     0,  1052,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     142,   235,     0,   336,     0,     0,     0,     0,   485,     0,
       0,     0,     0,     0,     0,   236,   237,     0,   238,   336,
       0,   336,     0,   239,     0,   185,   185,   185,   185,     0,
       0,     0,   240,     0,   185,   185,     0,     0,   241,     0,
       0,     0,     0,     0,   242,     0,     0,     0,   243,     0,
       0,   244,     0,     0,     0,     0,   142,   142,   142,   142,
       0,   245,     0,     0,     0,   142,   142,     0,   246,   247,
     514,     0,     0,     0,     0,     0,   248,     0,   532,   533,
       0,     0,     0,     0,     0,     0,   249,     0,     0,   548,
       0,     0,     0,     0,     0,   250,   251,     0,   252,     0,
     253,     0,   254,     0,   556,   255,   556,     0,     0,   256,
       0,   556,   257,     0,   556,     0,   258,     0,     0,     0,
       0,     0,     0,  1134,     0,  1135,     0,     0,  1137,     0,
       0,   576,     0,     0,     0,     0,     0,     0,   287,   287,
       0,     0,     0,     0,   287,   287,     0,     0,   287,   287,
       0,     0,     0,     0,     0,     0,   500,  1158,  1010,     0,
      22,    23,     0,     0,     0,     0,     0,  1164,     0,  1168,
     501,   282,    31,   502,     0,     0,     0,   336,    37,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   336,     0,   336,
     -59,     0,     0,   634,  1044,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,     0,     0,    64,     0,   -59,
      65,     0,     0,   503,     0,    69,     0,     0,     0,     0,
     696,     0,     0,     0,     0,     0,     0,     0,   336,     0,
       0,     0,   986,   988,    85,     0,     0,    87,   993,   996,
       0,    90,   998,  1000,     0,     0,     0,  1228,     0,     0,
     431,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,  1237,     0,     0,     0,
     433,   523,     0,     0,     0,  1242,     0,     0,     0,  1244,
       0,     0,     0,     0,   764,     0,   524,     0,     0,     0,
     105,     0,   283,     0,     0,     0,     0,     0,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   336,     0,     0,   287,   287,     0,     0,     0,
       0,     0,   287,     0,   538,     0,     0,     0,   434,     0,
       0,   538,   435,     0,   287,     0,  1287,   287,     0,   287,
     812,   287,     0,     0,   548,     0,     0,     0,     0,     0,
       0,     0,   283,     0,  1142,     0,     0,   827,     0,     0,
     520,     0,   336,     0,   436,     0,     0,     0,   838,     0,
       0,     0,   548,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,  1162,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,   491,
       0,     0,     0,     0,     0,     0,   460,     0,     0,  1115,
    1116,     0,   885,     0,     0,     0,  1117,     0,     0,     0,
     305,     6,     7,     8,     9,    10,    11,     0,  1128,     0,
      12,  1131,     0,  1132,    16,  1133,     0,     0,   196,     0,
       0,     0,     0,   306,     0,     0,     0,     0,     0,     0,
       0,     0,   197,     0,     0,     0,     0,   198,   199,     0,
       0,   200,     0,     0,   308,     0,     0,     0,   310,     0,
       0,     0,   201,     0,     0,    47,    48,     0,     0,     0,
       0,    52,     0,    53,    54,     0,     0,     0,     0,   287,
       0,   424,   311,     0,    59,   312,    61,    62,    63,     0,
       0,     0,     0,    66,     0,   202,     0,   313,     0,     0,
       0,     0,     0,    74,     0,    76,     0,    78,   720,     0,
       0,     0,     0,     0,     0,     0,   316,   658,     0,     0,
      88,     0,     0,     0,   660,   721,     0,     0,     0,     0,
       0,     0,    94,   336,   336,     0,     0,     0,     0,   722,
       0,   538,   538,     0,     0,   538,   538,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,  1291,
     665,   538,     0,   538,     0,     0,   101,     0,   723,   102,
     103,   104,     0,     0,     0,   106,     0,   204,     0,   108,
    -340,   317,     0,  1238,   110,     0,  1011,     0,     0,     0,
       0,     0,     0,   548,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1022,     0,     0,     0,   724,
       0,     0,     0,     0,     0,   725,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1045,   537,   462,   463,   464,   465,   466,     0,     0,
     469,   470,   471,   472,     0,   474,   475,   673,   674,   675,
     676,   677,   678,     0,   679,     0,     0,     0,   680,   681,
     682,   683,   684,   685,   686,   687,   688,   689,     0,   690,
       0,     0,   691,   692,   693,   694,  1056,     0,     0,     0,
     424,   283,     0,     0,     0,     0,     0,    -2,     4,     0,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,   696,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -59,     0,    40,    41,    42,     0,
      43,  -336,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -59,    53,    54,     0,    55,    56,    57,
       0,  -336,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -336,   -59,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
     812,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,    89,   -59,    90,    91,     0,     0,    92,     0,
      93,     0,     0,    94,     0,     0,     0,     0,     0,     0,
    1045,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     0,     6,     7,
       8,     9,    10,    11,     0,  -706,     0,    12,    13,    14,
      15,    16,  -706,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,  -706,    28,    29,  -706,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -59,     0,    40,    41,    42,     0,    43,  -336,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -59,
      53,    54,     0,    55,    56,    57,     0,  -336,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -336,   -59,    65,
      66,    67,    68,  -706,    69,    70,    71,  -706,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,    89,   -59,
      90,    91,     0,     0,    92,     0,    93,     0,     0,  -706,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -706,  -706,    97,  -706,  -706,
    -706,  -706,  -706,  -706,  -706,     0,  -706,  -706,  -706,  -706,
    -706,  -706,  -706,  -706,  -706,  -706,  -706,  -706,  -706,   105,
    -706,  -706,  -706,     0,   107,  -706,   108,     0,   109,     0,
     366,  -706,     5,   305,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -59,     0,    40,    41,
      42,     0,    43,  -336,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -59,    53,    54,     0,    55,
      56,    57,     0,  -336,     0,     0,    58,    59,    60,    61,
      62,    63,    64,  -336,   -59,    65,    66,    67,    68,     0,
      69,    70,    71,     0,    72,    73,    74,    75,    76,    77,
      78,    79,     0,    80,    81,     0,    82,    83,    84,    85,
      86,     0,    87,    88,    89,   -59,    90,    91,     0,     0,
      92,     0,    93,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,   105,     0,     0,   106,     0,
     107,   367,   108,     0,   109,     0,     4,   110,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -59,     0,    40,    41,    42,     0,    43,  -336,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -59,    53,    54,     0,    55,    56,    57,     0,  -336,
       0,     0,    58,    59,    60,    61,    62,    63,    64,  -336,
     -59,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
      89,   -59,    90,    91,     0,     0,    92,     0,    93,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,   598,   108,     0,
     109,     0,   617,   110,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -59,     0,
      40,    41,    42,     0,    43,  -336,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -59,    53,    54,
       0,    55,    56,    57,     0,  -336,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -336,   -59,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,    89,   -59,    90,    91,
       0,     0,    92,     0,    93,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,   618,   108,     0,   109,     0,   366,   110,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -59,     0,    40,    41,    42,     0,
      43,  -336,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -59,    53,    54,     0,    55,    56,    57,
       0,  -336,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -336,   -59,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,    89,   -59,    90,    91,     0,     0,    92,     0,
      93,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,   367,
     108,     0,   109,     0,     4,   110,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -59,     0,    40,    41,    42,     0,    43,  -336,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -59,
      53,    54,     0,    55,    56,    57,     0,  -336,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -336,   -59,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,    89,   -59,
      90,    91,     0,     0,    92,     0,    93,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,   875,   108,     0,   109,     0,
       4,   110,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -59,     0,    40,    41,
      42,     0,    43,  -336,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -59,    53,    54,     0,    55,
      56,    57,     0,  -336,     0,     0,    58,    59,    60,    61,
     382,    63,    64,  -336,   -59,    65,    66,    67,    68,     0,
      69,    70,    71,     0,    72,    73,    74,    75,    76,    77,
      78,    79,     0,    80,    81,     0,    82,    83,    84,    85,
      86,     0,    87,    88,    89,   -59,    90,    91,     0,     0,
      92,     0,    93,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,   105,     0,     0,   106,     0,
     107,     0,   108,     0,   109,     0,     4,   110,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -59,     0,    40,    41,    42,     0,    43,  -336,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -59,    53,    54,     0,    55,    56,    57,     0,  -336,
       0,     0,    58,    59,    60,    61,     0,    63,    64,  -336,
     -59,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
      89,   -59,    90,    91,     0,     0,    92,     0,    93,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,     0,   108,     0,
     109,     0,     4,   110,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,   786,    39,   -59,     0,
      40,    41,    42,     0,    43,  -336,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -59,    53,    54,
       0,    55,    56,    57,     0,  -336,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -336,   -59,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,    89,   -59,    90,    91,
       0,     0,    92,     0,    93,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,     0,   108,     0,   109,     0,     4,   110,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,   792,    39,   -59,     0,    40,    41,    42,     0,
      43,  -336,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -59,    53,    54,     0,    55,    56,    57,
       0,  -336,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -336,   -59,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,    89,   -59,    90,    91,     0,     0,    92,     0,
      93,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1119,    39,
     -59,     0,    40,    41,    42,     0,    43,  -336,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -59,
      53,    54,     0,    55,    56,    57,     0,  -336,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -336,   -59,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,    89,   -59,
      90,    91,     0,     0,    92,     0,    93,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,     0,   108,     0,   109,     0,
       4,   110,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,  1121,    39,   -59,     0,    40,    41,
      42,     0,    43,  -336,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -59,    53,    54,     0,    55,
      56,    57,     0,  -336,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -336,   -59,    65,    66,    67,    68,     0,
      69,    70,    71,     0,    72,    73,    74,    75,    76,    77,
      78,    79,     0,    80,    81,     0,    82,    83,    84,    85,
      86,     0,    87,    88,    89,   -59,    90,    91,     0,     0,
      92,     0,    93,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,   105,     0,     0,   106,     0,
     107,     0,   108,     0,   109,     0,     4,   110,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1126,    39,   -59,     0,    40,    41,    42,     0,    43,  -336,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -59,    53,    54,     0,    55,    56,    57,     0,  -336,
       0,     0,    58,    59,    60,    61,     0,    63,    64,  -336,
     -59,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
      89,   -59,    90,    91,     0,     0,    92,     0,    93,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,     0,   108,     0,
     109,     0,     4,   110,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1129,    39,   -59,     0,
      40,    41,    42,     0,    43,  -336,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -59,    53,    54,
       0,    55,    56,    57,     0,  -336,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -336,   -59,    65,    66,    67,
      68,     0,    69,    70,    71,     0,    72,    73,    74,    75,
      76,    77,    78,    79,     0,    80,    81,     0,    82,    83,
      84,    85,    86,     0,    87,    88,    89,   -59,    90,    91,
       0,     0,    92,     0,    93,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,   105,     0,     0,
     106,     0,   107,     0,   108,     0,   109,     0,     4,   110,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
    1157,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -59,     0,    40,    41,    42,     0,
      43,  -336,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -59,    53,    54,     0,    55,    56,    57,
       0,  -336,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -336,   -59,    65,    66,    67,    68,     0,    69,    70,
      71,     0,    72,    73,    74,    75,    76,    77,    78,    79,
       0,    80,    81,     0,    82,    83,    84,    85,    86,     0,
      87,    88,    89,   -59,    90,    91,     0,     0,    92,     0,
      93,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,   105,     0,     0,   106,     0,   107,     0,
     108,     0,   109,     0,     4,   110,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1163,    39,
     -59,     0,    40,    41,    42,     0,    43,  -336,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -59,
      53,    54,     0,    55,    56,    57,     0,  -336,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -336,   -59,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,     0,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,    89,   -59,
      90,    91,     0,     0,    92,     0,    93,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,     0,   108,     0,   109,     0,
       4,   110,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,  1167,    39,   -59,     0,    40,    41,
      42,     0,    43,  -336,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -59,    53,    54,     0,    55,
      56,    57,     0,  -336,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -336,   -59,    65,    66,    67,    68,     0,
      69,    70,    71,     0,    72,    73,    74,    75,    76,    77,
      78,    79,     0,    80,    81,     0,    82,    83,    84,    85,
      86,     0,    87,    88,    89,   -59,    90,    91,     0,     0,
      92,     0,    93,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,   105,     0,     0,   106,     0,
     107,     0,   108,     0,   109,     0,  1090,   110,     5,   305,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   196,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   197,     0,     0,     0,    33,   198,   199,     0,     0,
     200,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   203,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   204,     0,   108,     0,
     205,  1091,  1018,   110,     5,   305,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   204,     0,   108,     0,   205,     0,   919,   110,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   196,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,   307,     0,   197,     0,     0,     0,    33,   198,   199,
       0,     0,   200,    39,     0,     0,     0,   309,     0,     0,
      43,     0,     0,   201,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   202,     0,     0,     0,
       0,     0,     0,     0,    74,   314,    76,    77,    78,   315,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   204,     0,
     108,     0,   920,     0,     5,   110,     6,     7,     8,     9,
      10,    11,     0,   658,     0,    12,     0,     0,    15,    16,
     660,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,    41,     0,     0,    43,     0,   665,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,   537,   462,
     463,   464,   465,   466,     0,     0,   469,   470,   471,   472,
       0,   474,   475,   886,   887,   888,   889,   890,   678,     0,
     679,     0,   100,     0,   680,   681,   682,   683,   684,   685,
     686,   891,   688,   689,   102,   892,   104,     0,   691,   692,
     893,   694,   204,     0,   108,     0,   205,     0,     5,   110,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,     0,    15,    16,     0,    17,     0,   196,    19,    20,
      21,     0,     0,     0,     0,    26,     0,     0,    28,    29,
       0,   197,     0,     0,     0,    33,    34,    35,    36,     0,
      38,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,    49,    50,    51,
      52,     0,    53,    54,     0,    55,    56,    57,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,     0,    91,     0,     0,    92,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   107,     0,   108,     0,
     109,     0,     0,   110,     5,   305,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,   333,
     334,     0,    86,   371,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,   372,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   204,     0,   108,     0,   205,     0,     5,   110,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   196,     0,     0,
      21,   389,    23,     0,     0,     0,     0,     0,     0,    29,
       0,   197,     0,     0,     0,    33,   198,   199,     0,     0,
     200,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,    60,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,     0,    69,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   203,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,    91,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   204,     0,   108,     0,
     205,     0,     0,   110,     5,   305,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,   333,
     334,     0,    86,   371,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   204,     0,   108,   859,   205,     0,     0,   110,
       5,   305,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   196,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   197,     0,     0,     0,    33,   198,   199,
       0,     0,   200,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   201,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   202,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   203,
       0,    80,     0,     0,    82,   333,   334,     0,    86,   371,
       0,    88,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   204,     0,
     108,   861,   205,     0,     5,   110,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,   765,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,   766,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,   767,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,   768,    91,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   204,     0,   108,     0,   205,     0,     5,   110,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   196,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   197,     0,     0,     0,    33,   198,   199,     0,     0,
     200,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   203,     0,    80,
       0,     0,    82,   333,   334,     0,    86,     0,     0,    88,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   335,     0,     0,   106,     0,   204,     0,   108,     0,
     205,     0,     0,   110,     5,   305,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,   333,
     334,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   204,     0,   108,     0,   205,     0,     5,   110,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   196,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   197,     0,     0,     0,    33,   198,   199,     0,     0,
     200,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,  1020,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   203,     0,    80,
       0,     0,    82,   333,   334,     0,    86,     0,     0,    88,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   204,     0,   108,     0,
     205,     0,     5,   110,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,   200,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   201,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,   263,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   202,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   203,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,     0,    91,     0,     0,
       0,     0,     0,   264,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,     0,     0,     0,   106,     0,
     204,     0,   108,     0,   205,     0,     5,   110,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   196,     0,     0,    21,     0,
     271,     0,     0,     0,     0,     0,     0,    29,     0,   197,
       0,     0,     0,    33,   198,   199,     0,     0,   200,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   201,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   202,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   203,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
     272,    91,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   204,     0,   108,     0,   205,     0,
       5,   110,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   196,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   197,     0,     0,     0,    33,   198,   199,
       0,     0,   200,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   201,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   202,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   203,
       0,    80,     0,     0,    82,   333,   334,     0,    86,     0,
       0,    88,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   204,     0,
     108,     0,   205,     0,     0,   110,     5,   305,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   196,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   197,
       0,     0,     0,    33,   198,   199,     0,     0,   200,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   201,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   202,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   203,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
       0,    91,     0,     0,     0,     0,     0,   264,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   204,     0,   108,     0,   205,     0,
       0,   110,     5,   305,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,   200,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   201,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   202,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   203,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,     0,     0,     0,   106,     0,
     204,     0,   108,     0,   205,   569,     0,   110,     5,   305,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   196,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   197,     0,     0,     0,    33,   198,   199,     0,     0,
     200,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   203,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   204,   857,   108,     0,
     205,     0,     0,   110,     5,   305,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   204,     0,   108,     0,   205,   867,     5,   110,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   196,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   197,     0,     0,     0,    33,   198,   199,     0,     0,
     200,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,  -750,     0,     0,     0,  -750,
       0,     0,    74,    75,    76,    77,    78,   203,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   204,     0,   108,     0,
     205,     0,     5,   110,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,   200,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   201,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   202,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   203,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,     0,    91,     0,     0,
       0,     0,     0,   264,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,     0,     0,     0,   106,     0,
     204,     0,   108,     0,   205,     0,     5,   110,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   196,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,   286,    29,     0,   197,
       0,     0,     0,    33,   198,   199,     0,     0,   200,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   201,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   202,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   203,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   107,     0,   108,     0,   205,     0,
       0,   110,     5,   305,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,   200,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   201,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   202,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   203,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,     0,     0,     0,   106,     0,
     204,     0,   108,     0,   205,     0,     5,   110,     6,     7,
       8,   392,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,    18,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   197,
       0,     0,     0,    33,   198,   199,     0,     0,   200,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   201,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,    68,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   203,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,    87,    88,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   204,     0,   108,     0,   205,     0,
       0,   110,     5,   305,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,   200,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   201,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   202,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   203,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,     0,     0,     0,   106,     0,
     204,     0,   108,     0,   317,     0,     0,   110,     5,   305,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   196,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,   307,
       0,   197,     0,     0,     0,    33,   198,   199,     0,     0,
     200,    39,     0,     0,     0,   309,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,     0,     0,     0,     0,     0,
       0,     0,    74,   314,    76,    77,    78,   315,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   204,     0,   108,     0,
     317,     0,     5,   110,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,   200,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   201,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   202,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   203,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,     0,     0,     0,   106,     0,
     204,     0,   108,     0,   205,   872,     5,   110,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   196,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   197,
       0,     0,     0,    33,   198,   199,     0,     0,   200,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   201,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   202,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   203,     0,    80,     0,     0,
      82,     0,     0,     0,    86,  1059,     0,    88,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   204,     0,   108,     0,   205,     0,
       0,   110,     5,   305,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,  1217,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   201,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   202,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   203,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,     0,     0,     0,   106,     0,
     204,     0,   108,     0,   205,     0,     5,   110,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   196,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   197,
       0,     0,     0,    33,   198,   199,     0,     0,   200,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   201,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   202,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   203,     0,    80,     0,     0,
      82,     0,     0,     0,    86,  1246,     0,    88,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   204,     0,   108,     0,   205,     0,
       5,   110,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   196,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   197,     0,     0,     0,    33,   198,   199,
       0,     0,   200,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   201,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   202,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   203,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   204,     0,
     108,     0,   205,     0,     5,   110,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   307,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,   309,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,   314,
      76,    77,    78,   315,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   204,     0,   108,     0,   818,     0,     5,   110,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   196,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   197,     0,     0,     0,    33,   198,   199,     0,     0,
     200,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   203,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   204,     0,   108,     0,
     818,     0,     5,   110,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,   947,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   201,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   202,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   203,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,     0,     0,     0,   106,     0,
     204,     0,   108,     0,   205,     0,     5,   110,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   196,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   197,
       0,     0,     0,    33,   198,   199,     0,     0,   950,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   201,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   202,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   203,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   204,     0,   108,     0,   205,     0,
       5,   110,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   196,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   197,     0,     0,     0,    33,   198,   199,
       0,     0,  1211,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   201,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   202,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   203,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   204,     0,
     108,     0,   205,     0,     5,   110,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,  1212,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,   204,     0,   108,     0,   205,     0,     5,   110,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   196,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   197,     0,     0,     0,    33,   198,   199,     0,     0,
    1214,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   203,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   204,     0,   108,     0,
     205,     0,     5,   110,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,  1215,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   201,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   202,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   203,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,    96,    97,    98,    99,     0,     0,     0,     0,
     100,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,   102,   103,   104,     0,     0,     0,   106,     0,
     204,     0,   108,     0,   205,     0,     5,   110,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   196,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   197,
       0,     0,     0,    33,   198,   199,     0,     0,  1216,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   201,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   202,     0,     0,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   203,     0,    80,     0,     0,
      82,     0,     0,     0,    86,     0,     0,    88,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,     0,
       0,     0,   106,     0,   204,     0,   108,     0,   205,     0,
       5,   110,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   196,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   197,     0,     0,     0,    33,   198,   199,
       0,     0,  1217,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   201,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   202,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,    77,    78,   203,
       0,    80,     0,     0,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    95,
      96,    97,    98,    99,     0,     0,     0,     0,   100,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
     102,   103,   104,     0,     0,     0,   106,     0,   204,     0,
     108,     0,   205,     0,     5,   110,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,     0,     0,   108,     0,   920,     0,     0,   110,
     305,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,     0,    16,     0,     0,     0,   196,     0,
       0,     0,     0,   306,     0,     0,     0,     0,     0,     0,
       0,     0,   197,     0,     0,     0,     0,   198,   199,     0,
       0,   200,     0,     0,   308,     0,     0,     0,   310,     0,
       0,     0,   201,     0,     0,    47,    48,     0,     0,     0,
       0,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,   311,     0,    59,   312,    61,    62,    63,     0,
       0,     0,     0,    66,     0,   202,     0,   313,     0,     0,
       0,     0,     0,    74,     0,    76,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,   316,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   656,     0,   657,     0,     0,    95,    96,
      97,    98,    99,     0,   658,     0,     0,   100,     0,   659,
     237,   660,   661,     0,     0,     0,   101,   662,     0,   102,
     103,   104,     0,     0,     0,   106,   240,   204,   197,   108,
       0,   317,   241,     0,   110,     0,     0,     0,   663,     0,
       0,     0,   243,     0,     0,   664,     0,   665,     0,     0,
       0,     0,     0,     0,     0,   666,     0,     0,     0,     0,
       0,     0,   246,   667,     0,     0,     0,     0,     0,     0,
     248,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     668,     0,     0,     0,     0,     0,     0,     0,     0,   250,
     251,     0,   669,     0,   253,     0,   670,     0,     0,   255,
       0,     0,     0,   671,     0,     0,   257,     0,     0,     0,
     672,     0,     0,     0,     0,     0,     0,     0,     0,   537,
     462,   463,   464,   465,   466,     0,     0,   469,   470,   471,
     472,     0,   474,   475,   673,   674,   675,   676,   677,   678,
       0,   679,     0,     0,     0,   680,   681,   682,   683,   684,
     685,   686,   687,   688,   689,     0,   690,     0,     0,   691,
     692,   693,   694,     0,     5,   695,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,     0,   103,   104,     0,     0,     0,
     106,     0,   204,     0,   108,     0,   205,     5,   305,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     197,     0,     0,     0,    33,   198,   199,     0,     0,   200,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     201,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,    67,   202,     0,     0,     0,     0,     0,     0,
       0,    74,    75,    76,    77,    78,   203,     0,    80,     0,
       0,    82,     0,     0,     0,    86,     0,     0,    88,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,  -528,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    97,     0,
       0,     0,  -528,     0,     0,     0,  -528,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   104,
       0,     0,     0,     0,     0,   204,     0,   108,  -528,   317,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   196,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   197,     0,     0,     0,    33,   198,   199,
       0,     0,   200,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   201,     0,     0,    47,    48,  -507,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,   202,     0,     0,     0,
    -507,     0,     0,     0,    74,    75,    76,    77,    78,   203,
       0,    80,     0,  -507,    82,     0,     0,     0,    86,     0,
       0,    88,     0,     0,     0,    91,     0,  -507,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    97,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,   104,  -507,    21,     0,     0,     0,  -507,     0,
     108,     0,   920,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,   200,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   201,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,    67,   202,     0,
       0,     0,     0,     0,     0,     0,    74,    75,    76,    77,
      78,   203,     0,    80,     0,     0,    82,     0,     0,     0,
      86,     0,     0,    88,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,    97,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,   104,     0,    21,     0,     0,     0,
     204,     0,   108,     0,   205,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    97,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   196,   104,     0,    21,     0,
       0,     0,   204,     0,   108,     0,   818,    29,     0,   197,
       0,     0,     0,    33,   198,   199,     0,     0,   200,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   201,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,    67,   202,     0,   235,     0,     0,     0,     0,     0,
      74,    75,    76,    77,    78,   203,     0,    80,   236,   237,
      82,   238,     0,     0,    86,     0,   239,    88,     0,     0,
       0,    91,     0,     0,     0,   240,     0,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,   242,     0,     0,
       0,   243,     0,     0,   244,     0,     0,    97,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,     0,
       0,   246,   247,     0,     0,     0,     0,     0,     0,   248,
       0,     0,     0,     0,     0,     0,   108,     0,   205,   249,
       0,     0,   235,     0,     0,     0,     0,     0,   250,   251,
       0,   252,     0,   253,     0,   254,   236,   237,   255,   238,
       0,     0,   256,     0,   239,   257,     0,     0,     0,   258,
       0,     0,     0,   240,     0,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,   242,     0,     0,     0,   243,
       0,     0,   244,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   245,     0,     0,     0,     0,     0,     0,   246,
     247,     0,     0,     0,     0,     0,     0,   248,     0,     0,
       0,     0,   491,     0,  1043,     0,     0,   249,     0,     0,
       0,     0,     0,     0,     0,     0,   250,   251,     0,   252,
       0,   253,     0,   254,     0,     0,   255,   235,     0,     0,
     256,     0,     0,   257,     0,     0,   658,   258,     0,     0,
       0,   236,   237,   660,   238,     0,     0,     0,     0,   239,
       0,     0,     0,     0,     0,     0,     0,     0,   240,     0,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
     242,     0,     0,     0,   243,     0,     0,   244,     0,   665,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,   575,     0,   246,   247,     0,     0,     0,     0,
       0,     0,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   249,     0,     0,     0,     0,     0,     0,     0,
     431,   250,   251,     0,   252,     0,   253,   432,   254,     0,
       0,   255,     0,     0,     0,   256,     0,     0,   257,     0,
     433,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   537,   462,   463,   464,   465,   466,     0,     0,   469,
     470,   471,   472,     0,   474,   475,   673,   674,   675,   676,
     677,   678,     0,   679,     0,     0,     0,   680,   681,   682,
     683,   684,   685,   686,   687,   688,   689,     0,   690,     0,
       0,   691,   692,   693,   694,     0,   431,     0,   434,     0,
       0,     0,   435,   432,     0,     0,     0,     0,     0,     0,
       0,   536,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,   537,   462,   463,   464,   465,
     466,     0,     0,   469,   470,   471,   472,     0,   474,   475,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,   434,   457,   458,   459,   435,   491,
       0,   431,     0,     0,     0,     0,   460,  1118,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
     436,   537,   462,   463,   464,   465,   466,     0,     0,   469,
     470,   471,   472,     0,   474,   475,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
       0,   457,   458,   459,     0,   491,     0,   431,     0,   434,
       0,     0,   460,   435,   432,     0,     0,     0,     0,     0,
       0,     0,  1125,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   537,   462,   463,   464,
     465,   466,     0,     0,   469,   470,   471,   472,     0,   474,
     475,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,   434,   457,   458,   459,   435,
     491,     0,   431,     0,     0,     0,     0,   460,  1283,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   537,   462,   463,   464,   465,   466,     0,     0,
     469,   470,   471,   472,     0,   474,   475,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,     0,   457,   458,   459,     0,   491,     0,   431,     0,
     434,     0,     0,   460,   435,   432,     0,     0,     0,     0,
       0,     0,     0,  1284,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,   537,   462,   463,
     464,   465,   466,     0,     0,   469,   470,   471,   472,     0,
     474,   475,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,   434,   457,   458,   459,
     435,   491,     0,   431,     0,     0,     0,     0,   460,  1285,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   537,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,     0,   491,     0,   431,
       0,   434,     0,     0,   460,   435,   432,     0,     0,     0,
       0,     0,     0,     0,  1286,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   537,   462,
     463,   464,   465,   466,     0,     0,   469,   470,   471,   472,
       0,   474,   475,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,     0,   434,   457,   458,
     459,   435,   491,     0,   431,     0,     0,     0,     0,   460,
    1292,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   537,   462,   463,   464,   465,   466,
       0,     0,   469,   470,   471,   472,     0,   474,   475,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,     0,   491,     0,
     431,     0,   434,     0,     0,   460,   435,   432,     0,     0,
       0,     0,     0,     0,     0,  1293,     0,     0,     0,     0,
     433,   286,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   529,     0,   436,   537,
     462,   463,   464,   465,   466,     0,     0,   469,   470,   471,
     472,     0,   474,   475,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,   434,   457,
     458,   459,   435,   491,     0,   431,     0,     0,     0,     0,
     460,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   517,     0,     0,     0,
     520,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,   518,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,   491,
       0,     0,     0,   434,     0,     0,   460,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,   436,
       0,     0,   626,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   627,     0,
     457,   458,   459,     0,   491,     0,     0,     0,     0,     0,
       0,   460,     0,     0,     0,   434,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     882,     0,     0,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,   436,     0,     0,   883,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     884,   628,   457,   458,   459,     0,     0,     0,     0,     0,
       0,     0,     0,   460,     0,   431,     0,   434,     0,     0,
       0,   435,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   995,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,   434,   457,   458,   459,   435,     0,     0,
     431,     0,     0,     0,     0,   460,     0,   432,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   286,     0,     0,     0,   520,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,     0,
     457,   458,   459,     0,   491,     0,   431,     0,   434,     0,
       0,   460,   435,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,     0,   493,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,   434,   457,   458,   459,   435,   491,
       0,   431,     0,     0,     0,     0,   460,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,   286,     0,     0,     0,   216,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
       0,   457,   458,   459,     0,     0,     0,   431,     0,   434,
       0,     0,   460,   435,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,   756,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   757,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,   434,   457,   458,   459,   435,
     491,     0,   431,     0,     0,     0,     0,   460,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   758,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,   759,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,     0,   457,   458,   459,     0,     0,     0,   431,     0,
     434,     0,     0,   460,   435,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   987,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,   434,   457,   458,   459,
     435,     0,     0,   431,     0,     0,     0,     0,   460,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,     0,   491,     0,   431,
       0,   434,     0,     0,   460,   435,   432,     0,     0,     0,
       0,     0,     0,     0,  1009,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,     0,   434,   457,   458,
     459,   435,   491,     0,     0,     0,     0,     0,     0,   460,
       0,   431,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   520,
       0,   433,     0,   436,     0,     0,  1176,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,  1177,     0,   457,   458,   459,     0,     0,     0,
       0,     0,     0,  1050,     0,   460,     0,   431,   560,   434,
       0,     0,     0,   435,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,   431,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,   433,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,   434,   457,   458,   459,   435,
       0,     0,     0,   431,     0,     0,     0,   460,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,   434,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   436,   457,   458,   459,     0,     0,     0,     0,     0,
       0,   434,     0,   460,     0,   435,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   431,   457,   458,   459,     0,   585,   436,   432,     0,
       0,     0,     0,   460,     0,     0,     0,     0,     0,     0,
       0,   433,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   431,   589,   457,   458,
     459,     0,     0,   432,     0,     0,     0,     0,     0,   460,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
       0,     0,     0,   435,     0,     0,   431,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,   613,   436,     0,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,   435,     0,
       0,   437,   438,     0,   439,   440,   441,   760,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,     0,   457,   458,   459,     0,
     436,     0,   431,   878,   434,     0,     0,   460,   435,   432,
       0,     0,     0,     0,     0,     0,   437,   438,     0,   439,
     440,   441,   433,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
     436,   457,   458,   459,     0,     0,   431,     0,     0,     0,
       0,     0,   460,   432,     0,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   433,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
     434,   457,   458,   459,   435,     0,     0,     0,   858,     0,
       0,   431,   460,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,   435,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     879,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,   216,   457,   458,   459,
     436,     0,     0,     0,     0,     0,     0,     0,   460,   434,
       0,     0,     0,   435,     0,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
     431,   457,   458,   459,     0,   436,     0,   432,     0,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,     0,
     433,   437,   438,  1007,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   431,   978,   457,   458,   459,     0,
       0,   432,     0,     0,     0,     0,     0,   460,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   435,   431,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,     0,   435,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,   436,     0,
       0,   434,     0,     0,   431,   435,   460,     0,     0,     0,
       0,   432,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   433,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   436,     0,   457,
     458,   459,     0,     0,     0,     0,  1023,     0,     0,     0,
     460,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,     0,     0,   457,   458,
     459,     0,   434,     0,     0,  1024,   435,   431,  1065,   460,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1053,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,     0,   457,
     458,   459,     0,     0,     0,   434,     0,   431,     0,   435,
     460,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,     0,   457,   458,   459,   434,     0,     0,   431,   435,
       0,     0,     0,   460,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,  1103,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,     0,   457,   458,   459,     0,   434,     0,   431,  1096,
     435,     0,     0,   460,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,  1106,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,   432,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   433,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,   434,     0,   431,     0,
     435,     0,     0,     0,   460,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,   434,     0,   431,  1173,
     435,     0,     0,     0,     0,   432,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   433,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   436,   457,   458,   459,   434,     0,     0,     0,
     435,     0,     0,     0,   460,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   436,   457,   458,   459,   434,     0,   431,     0,
     435,     0,  1109,     0,   460,   432,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   433,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   436,   457,   458,   459,     0,     0,     0,     0,
       0,     0,  1149,     0,   460,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,   434,     0,   431,     0,
     435,     0,     0,     0,   460,   432,     0,     0,     0,  1210,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,   431,  1252,
       0,     0,     0,     0,     0,   432,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   433,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,   434,     0,   431,     0,
     435,     0,     0,     0,   460,   432,     0,     0,     0,  1213,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,   434,     0,   431,     0,
     435,     0,     0,     0,     0,   432,     0,  1268,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   433,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   436,   457,   458,   459,   434,     0,     0,     0,
     435,     0,     0,     0,   460,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   436,   457,   458,   459,   434,     0,   431,     0,
     435,     0,     0,     0,   460,   432,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   433,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   436,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,   460,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,   434,     0,   431,     0,
     435,     0,  1288,     0,   460,   432,     0,     0,     0,  1310,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,   434,     0,   431,     0,
     435,     0,     0,     0,   460,   432,     0,     0,     0,  1311,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,   434,     0,   431,     0,
     435,     0,     0,     0,   460,   432,     0,     0,     0,  1312,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,   434,     0,   431,     0,
     435,     0,     0,     0,   460,   432,     0,     0,     0,  1313,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,   434,     0,   431,     0,
     435,     0,     0,     0,   460,   432,     0,     0,     0,  1314,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,   431,     0,
       0,     0,     0,     0,     0,   432,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   433,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,   434,     0,   431,     0,
     435,     0,     0,     0,   460,   432,     0,     0,     0,  1315,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,   434,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   436,   457,   458,   459,   973,     0,     0,     0,
     435,     0,     0,     0,   460,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   436,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,   460,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   500,   457,   458,   459,    22,    23,     0,     0,
       0,  1073,     0,     0,   460,     0,   501,     0,    31,   502,
       0,     0,     0,     0,    37,   500,     0,     0,     0,    22,
      23,    42,     0,     0,  1073,     0,     0,     0,     0,   501,
       0,    31,   502,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,    42,     0,     0,    58,     0,    60,
       0,    62,     0,  1074,     0,     0,  1075,     0,     0,   503,
       0,    69,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,  1074,     0,     0,  1075,
      85,     0,   503,    87,    69,  1076,     0,    90,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,    85,     0,     0,    87,     0,  1076,   500,
      90,     0,     0,    22,    23,     0,     0,     0,  1073,     0,
       0,     0,     0,   501,     0,    31,   502,     0,     0,     0,
       0,    37,     0,     0,     0,     0,   105,     0,    42,     0,
       0,     0,  1077,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,     0,     0,     0,   435,     0,   105,
       0,     0,     0,     0,    58,  1184,    60,     0,    62,     0,
    1074,   433,     0,  1075,     0,     0,   503,     0,    69,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,    85,     0,     0,
      87,     0,  1076,     0,    90,   437,   438,     0,   439,   440,
     441,   433,   442,   443,   444,     0,   445,   446,   447,   448,
       0,   450,   451,   452,   453,   454,   455,   456,     0,   434,
     457,   458,   459,   435,     0,     0,     0,     0,     0,     0,
       0,   460,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   105,     0,     0,     0,     0,     0,  1253,
       0,     0,     0,     0,     0,   436,     0,     0,     0,   434,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,     0,   450,   451,   452,
     453,   454,     0,   456,     0,     0,   457,   458,   459,     0,
       0,     0,     0,     0,     0,     0,     0,   460,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,     0,   450,   451,   452,
     453,   454,     0,   456,     0,   500,   457,   458,   459,    22,
      23,     0,     0,     0,  1073,     0,     0,   460,     0,   501,
       0,    31,   502,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,    42,     0,     0,   400,     0,   235,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   236,   237,     0,   238,     0,     0,     0,
      58,   239,    60,     0,   382,     0,  1074,   401,     0,  1075,
     240,     0,   503,     0,    69,     0,   241,     0,     0,     0,
       0,     0,   242,     0,     0,     0,   243,     0,     0,   244,
       0,     0,     0,    85,     0,     0,    87,     0,  1076,   245,
      90,     0,     0,     0,     0,     0,   246,   247,     0,     0,
       0,     0,     0,     0,   248,   235,     0,     0,     0,     0,
       0,     0,     0,     0,   249,     0,     0,     0,     0,   236,
     237,     0,   238,   250,   251,     0,   252,   239,   253,    23,
     254,     0,     0,   255,     0,     0,   240,   256,     0,   105,
     257,   402,   241,     0,   258,     0,     0,     0,   242,     0,
       0,     0,   243,     0,     0,   244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   245,     0,     0,     0,     0,
       0,     0,   246,   247,     0,     0,     0,     0,     0,     0,
     248,    60,     0,     0,     0,     0,     0,     0,     0,     0,
     249,     0,     0,    69,     0,     0,     0,     0,     0,   250,
     251,     0,   252,     0,   253,     0,   254,   235,     0,   255,
       0,     0,     0,   256,     0,     0,   257,     0,     0,    90,
     258,   236,   237,     0,   238,     0,     0,     0,     0,   239,
       0,     0,     0,     0,     0,   401,     0,     0,   240,     0,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
     242,     0,     0,     0,   243,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,     0,  -302,   245,     0,     0,
       0,     0,     0,     0,   246,   247,     0,     0,     0,     0,
    -302,  -302,   248,  -302,     0,     0,     0,     0,  -302,     0,
       0,     0,   249,     0,     0,     0,     0,  -302,     0,     0,
       0,   250,   251,  -302,   252,     0,   253,     0,   254,  -302,
       0,   255,     0,  -302,     0,   256,  -302,     0,   257,     0,
       0,     0,   258,     0,     0,   235,  -302,     0,     0,     0,
       0,     0,     0,  -302,  -302,     0,     0,     0,     0,   236,
     237,  -302,   238,     0,     0,     0,     0,   239,     0,     0,
       0,  -302,     0,     0,     0,     0,   240,     0,     0,     0,
    -302,  -302,   241,  -302,     0,  -302,     0,  -302,   242,     0,
    -302,     0,   243,     0,  -302,   244,     0,  -302,     0,     0,
       0,  -302,     0,     0,  -303,   245,     0,     0,     0,     0,
       0,     0,   246,   247,     0,     0,     0,     0,  -303,  -303,
     248,  -303,     0,     0,     0,     0,  -303,     0,     0,     0,
     249,     0,     0,     0,     0,  -303,     0,     0,     0,   250,
     251,  -303,   252,     0,   253,     0,   254,  -303,     0,   255,
       0,  -303,     0,   256,  -303,     0,   257,     0,     0,     0,
     258,     0,     0,     0,  -303,     0,     0,     0,     0,     0,
       0,  -303,  -303,     0,     0,     0,     0,     0,     0,  -303,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -303,
       0,     0,     0,     0,     0,     0,     0,     0,  -303,  -303,
       0,  -303,     0,  -303,     0,  -303,     0,     0,  -303,     0,
       0,     0,  -303,     0,     0,  -303,     0,     0,     0,  -303
};

static const yytype_int16 yycheck[] =
{
      12,    66,    19,    40,    16,   583,   488,    70,    20,     2,
     702,   225,   232,   423,    26,   166,   790,   278,    66,   182,
      66,   626,    34,    35,    36,   837,    38,   569,   649,   844,
     136,   577,    66,   884,   908,   393,   542,    49,   587,    51,
      52,   302,   639,   838,   641,    57,   109,    59,   879,    61,
     902,     1,     1,  1091,     1,   234,   409,  1028,    70,    71,
      72,    73,    74,    75,    56,    33,    48,    79,     3,    81,
       1,    33,   552,    34,    35,    36,    88,     3,   122,    48,
      92,    93,    27,    95,    61,    97,   347,   103,   100,   101,
      71,   103,     1,    33,   125,   107,   108,   109,   110,   278,
     698,    48,   700,    48,    93,    33,    33,    52,    37,     3,
      56,   274,    93,    33,   115,    75,   268,   269,   270,   122,
     127,   148,   134,   302,   136,   148,    75,   108,     0,   148,
      61,    76,   176,    48,    79,   148,    81,   357,   148,   166,
     171,    48,   205,   120,    73,   152,    91,   148,   109,    48,
      56,   174,    61,   122,   122,   171,    48,   176,    93,   119,
     122,   174,    93,   175,   174,   110,   148,  1205,   347,   104,
     119,   172,   652,   176,    93,   106,    47,   127,   104,   148,
     127,   173,   171,  1154,    93,   959,   198,   199,   200,   120,
      22,   203,   204,   205,  1068,   232,   817,   147,   148,   166,
     147,   148,  1017,   171,  1078,    48,   223,   176,   148,   171,
     104,   120,   172,  1028,    85,  1067,   147,   478,   479,   166,
     573,    27,   234,   172,   174,   377,   378,   173,   148,   174,
      32,   171,  1063,   148,   783,   166,  1031,    27,   147,    48,
     171,   148,    48,   171,   171,   116,    52,   166,    91,   148,
     148,   171,   171,  1201,   317,   761,   148,    89,  1206,   120,
     423,   176,   171,    54,   443,   148,   278,    27,   148,   176,
      76,   629,   148,    79,    32,    81,   174,   176,   883,   499,
      32,   113,    91,    74,   176,    91,   392,   148,    90,    79,
     302,   174,    94,    84,   174,   837,   842,  1112,   174,   478,
     479,    91,   314,   315,   110,   317,   166,   148,   790,  1183,
     148,   122,   909,   173,   911,  1189,     3,  1191,    27,    79,
     357,   333,   334,    56,  1175,   547,  1177,   117,   166,   148,
     148,    91,    90,   174,   397,   347,    94,  1149,    90,  1154,
     142,   143,    94,   408,   495,   147,   560,   149,   150,  1123,
     110,   972,   154,   575,    51,   174,   174,    54,   166,   161,
     372,   142,   148,   542,   172,   167,   168,   169,   174,   150,
      79,    83,   592,   385,    86,   387,   369,    74,   127,   148,
      77,  1255,    91,  1257,   142,   397,   148,    84,   174,   147,
     142,   143,   150,   145,   146,   147,   154,   149,   150,   148,
     906,   110,   154,   152,   171,   174,   173,    90,   587,   161,
     172,    94,    18,   110,    56,   167,   168,   169,   632,   431,
     432,   433,   434,   435,   436,   174,   438,   439,   440,   441,
     442,   148,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,   148,
     956,  1056,   515,    56,  1146,   467,   468,   174,   148,   148,
      47,   473,   499,   110,   476,   477,   478,   479,   480,   481,
      91,     3,    78,   434,   435,   174,   488,   959,    47,    66,
     148,   493,     8,   172,   174,   497,   467,   468,    20,   127,
      11,    97,   473,   173,   506,   476,   477,    66,    85,  1097,
    1098,    56,    34,   515,   172,   517,   518,   148,   114,   574,
     148,   523,   524,   173,   152,   175,    85,   529,   552,   480,
     481,   931,    61,   148,   536,   173,   574,   539,   574,   116,
     542,    63,   493,   174,   148,   569,   174,    56,   148,     2,
     574,   166,   554,   577,   166,    32,   558,   116,   127,   561,
     562,   173,   166,   175,    93,   592,   166,   518,   166,    83,
      84,    56,    25,   524,   172,    28,   578,   106,   529,  1174,
      56,  1176,   104,    56,   153,   587,   155,   156,   110,   158,
     159,   120,   761,    56,   173,    90,   175,   148,   600,    94,
     148,   603,   148,   605,   148,   148,    90,   609,   610,   611,
      94,   613,   614,    90,   783,   166,    48,    94,   166,     8,
     166,     2,   166,   166,   626,   627,    79,   148,   652,    61,
      83,    84,   603,  1201,   605,    90,   619,   166,  1206,    94,
     642,   173,   171,   175,    25,   166,   148,    28,   173,   148,
     175,  1123,   794,   795,   107,   173,   798,   799,   609,   112,
      56,    93,   148,   101,   166,   142,   143,   166,   145,   146,
     147,    90,   149,   150,   151,    94,    56,   154,   702,   173,
     166,  1195,    90,   160,   161,    56,    94,   164,   120,    56,
     167,   168,   169,   695,   173,   127,   175,    56,    79,   142,
       8,   178,    83,    84,   147,    56,   173,   150,   175,   878,
     879,   154,     2,   173,   171,   175,   148,   603,    56,   605,
     303,   304,    27,   865,   866,   147,   107,   173,    56,   166,
     165,   112,    48,   166,   166,    25,   166,   906,    28,   171,
     166,   166,   174,    48,   173,    61,   166,    52,  1262,  1263,
    1264,  1265,  1266,  1267,   756,   757,   758,   759,   760,   761,
     762,   763,   166,   904,   217,   818,   166,   774,   775,   171,
     171,    76,   166,   121,    79,   173,    81,    93,   931,   781,
     173,   783,   784,    85,   786,   173,    91,   956,   790,    79,
     792,   173,   148,    83,    84,   174,   166,   147,   173,   801,
     802,   166,     8,    66,   120,   110,   757,    61,   759,    61,
     781,   127,   172,   837,   171,    32,   818,   107,   842,     8,
     822,   127,   112,     8,    91,    91,   127,   150,   281,    69,
     173,   984,   148,   286,   836,   837,   217,   174,   991,    93,
     127,    93,   173,    48,   148,    32,   848,   127,   127,    35,
     166,   822,   106,   908,   106,   171,    35,    66,   174,   174,
     174,   916,   127,   147,   174,    21,   120,   920,   120,   176,
     148,   106,   927,    90,   876,   171,   176,    94,   880,   173,
     882,   883,   884,   166,   886,   166,   888,   176,   150,   891,
     892,   150,   150,   150,   150,   150,  1147,  1148,   166,   150,
     281,   150,   150,    90,   906,   286,   150,    94,   150,   150,
     142,   150,   166,   150,   166,    35,   369,   171,   920,   171,
     166,   166,    35,   166,   174,   142,   143,   217,   145,   146,
     147,   384,   149,   150,   151,   173,   147,   154,   155,   156,
     174,   158,   159,   160,   161,   947,   172,   164,   950,   148,
     167,   168,   169,   166,   956,   142,   143,   959,   145,   146,
     147,   178,   149,   150,   171,   115,   174,   154,   172,    33,
     423,   973,   974,   160,   161,   176,    27,   164,  1147,  1148,
     167,   168,   169,   985,    33,   987,  1039,    35,   369,   166,
     992,   281,   176,   995,   166,   176,   286,    48,   173,   173,
     147,    52,   166,   384,     8,  1007,   176,   176,    48,     8,
     176,   148,   174,  1068,    35,   174,   174,   166,   166,   174,
     166,    61,   973,   974,   165,    76,   165,  1029,    79,   148,
      81,  1033,   384,   876,    24,  1078,  1078,  1039,   491,    92,
      91,   277,   354,  1013,  1043,   558,  1048,   914,  1050,   395,
     915,  1053,   931,    93,  1056,   574,   927,   916,   318,   110,
    1206,  1063,  1030,  1065,   517,  1268,   926,  1091,  1221,  1222,
     523,  1261,  1225,  1226,   903,   704,   134,   555,   816,   369,
     120,   534,    -1,   536,    -1,    -1,  1088,   127,  1241,    -1,
    1243,    -1,    -1,    -1,   384,    -1,    -1,    -1,    -1,    -1,
    1102,  1103,    -1,  1105,  1106,    -1,  1108,  1109,   148,    -1,
     491,    -1,    -1,    -1,    -1,    -1,  1118,  1119,    -1,  1121,
      -1,  1123,  1146,  1125,  1126,  1149,   166,  1129,  1183,    -1,
      -1,   171,    -1,    -1,   174,    -1,   517,    -1,    32,    -1,
      -1,    -1,   523,  1145,  1146,  1147,  1148,  1149,    -1,    -1,
      -1,    -1,    -1,   534,   607,   536,    -1,    -1,   611,  1161,
      -1,  1163,    -1,    -1,    -1,  1167,   619,    -1,    -1,    -1,
      -1,  1173,  1174,  1175,  1176,  1177,    -1,    -1,    -1,    -1,
      -1,  1205,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,   491,    -1,  1205,    -1,    -1,    -1,    -1,  1210,  1211,
    1212,  1213,  1214,  1215,  1216,  1217,    -1,     2,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   607,   517,    -1,    -1,
     611,    -1,    -1,   523,    -1,    -1,    -1,    -1,   619,    -1,
      25,    -1,    -1,    28,   534,    -1,   536,    -1,   142,   143,
    1252,    -1,   146,   147,     2,   149,   150,    -1,  1260,    -1,
     154,    -1,    -1,    -1,  1288,    -1,    -1,   161,    -1,    -1,
      -1,    -1,    -1,   167,   168,   169,    -1,    25,    -1,     2,
      28,  1283,  1284,  1285,  1286,    -1,  1288,    -1,    -1,    -1,
    1292,  1293,    -1,    -1,    79,    -1,    -1,    -1,    83,    84,
      -1,    -1,    25,    -1,    -1,    28,    -1,    -1,  1310,  1311,
    1312,  1313,  1314,  1315,    -1,    -1,    -1,   607,   771,    -1,
      -1,   611,   107,    -1,    32,    -1,    -1,   112,    -1,   619,
      -1,    79,    -1,    -1,    -1,    83,    84,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   804,    -1,    -1,   807,    -1,    79,    -1,    -1,   107,
      83,    84,    -1,    -1,   112,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,   107,    -1,    94,    66,    -1,   112,
      -1,    70,    -1,    22,    -1,    -1,    -1,    26,    27,    90,
      -1,    -1,    31,    94,    -1,    -1,    -1,    36,    -1,    38,
      39,   864,    -1,    -1,    -1,    44,    -1,   870,    -1,    -1,
      -1,    -1,    51,   804,    -1,    54,   807,    -1,   107,   108,
     109,    -1,   217,    -1,   142,    -1,    -1,    -1,    -1,   147,
      -1,   149,   150,    -1,    -1,    74,   154,    -1,    77,    -1,
      79,   142,    81,   161,    83,    84,   147,    86,   149,   150,
      89,   169,    91,   154,    -1,    -1,    -1,    -1,    -1,   217,
     161,    -1,    -1,    -1,    -1,    -1,   167,   168,   169,    -1,
      -1,   110,    -1,   864,   113,    -1,   115,    -1,   117,   870,
      -1,    -1,    -1,    -1,   217,    -1,   281,    -1,    -1,    -1,
      -1,   286,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   804,    -1,    -1,   807,    -1,    -1,
     973,   974,    -1,    -1,    -1,   204,   205,    -1,    -1,    -1,
      -1,    -1,   985,   281,   987,    -1,    -1,   166,   286,   992,
      -1,    -1,   995,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1004,    -1,  1006,    -1,    -1,  1009,    -1,   281,    -1,
      -1,    -1,    -1,   286,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,   864,    -1,    -1,    -1,    -1,    36,
     870,    38,    39,    -1,   369,  1038,    -1,    44,    -1,     2,
      -1,    -1,    -1,    -1,    51,  1048,    -1,  1050,    -1,   384,
      -1,    -1,    -1,    -1,   985,    -1,   987,    -1,    -1,    -1,
      -1,   992,    25,    -1,   995,    28,    -1,    -1,    -1,    -1,
      77,   369,    79,  1004,    -1,  1006,    83,    -1,  1009,    86,
      -1,    -1,    89,    -1,    91,    -1,   384,    -1,   317,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   369,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,   113,  1038,    -1,    -1,
     117,   384,    -1,    -1,    -1,  1118,    79,  1048,    -1,  1050,
      83,    84,  1125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   107,   985,    -1,   987,    -1,   112,
      -1,    -1,   992,    -1,  1157,   995,   491,    -1,    -1,   166,
      -1,    -1,    -1,  1166,  1004,    -1,  1006,  1170,   397,  1009,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   517,    -1,    -1,    -1,    -1,  1118,   523,    -1,
      -1,    -1,    -1,   491,  1125,    -1,    -1,    -1,  1038,   534,
      -1,   536,    -1,    -1,    -1,    -1,    -1,  1138,  1048,    -1,
    1050,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   491,   517,
      -1,    -1,    -1,    -1,  1227,   523,  1157,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     2,  1166,   534,    -1,   536,  1170,
      -1,    -1,    -1,    -1,   517,    -1,    -1,    -1,    -1,    -1,
     523,    -1,    -1,    -1,   217,    -1,    -1,    25,    -1,    -1,
      28,   534,    -1,   536,    -1,     2,    -1,    -1,    -1,    -1,
      -1,    -1,   607,    -1,    -1,    -1,   611,    -1,  1118,    -1,
    1283,  1284,  1285,  1286,   619,  1125,   515,    -1,    25,  1292,
    1293,    28,    -1,    -1,    -1,    -1,  1227,    -1,  1138,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   607,
      -1,    79,    -1,   611,    -1,    83,    84,  1157,   281,    -1,
      -1,   619,    -1,   286,    -1,    -1,  1166,    -1,    -1,    -1,
    1170,    -1,    -1,    -1,   607,    -1,    -1,    -1,   611,   107,
     569,    -1,    79,    -1,   112,   574,   619,    -1,   577,   578,
      -1,    -1,  1283,  1284,  1285,  1286,    -1,    -1,    -1,    -1,
      -1,  1292,  1293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     107,   600,    -1,    -1,   603,   112,   605,    -1,    -1,    -1,
       3,   610,    -1,    -1,    -1,    -1,    -1,  1227,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,   369,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,   384,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,  1283,  1284,  1285,  1286,    70,    71,   217,
      -1,    -1,  1292,  1293,     3,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    17,   804,
      -1,    20,   807,    -1,    97,    98,    25,   100,    -1,   102,
     217,   104,    -1,    -1,   107,    -1,    -1,    36,   111,   112,
      -1,   114,    -1,    -1,    -1,   118,    -1,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    53,    -1,   804,    -1,    -1,   807,
      -1,    -1,    -1,   281,    63,    -1,    -1,    -1,   286,    -1,
      -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,   491,   864,
      -1,   804,    -1,    -1,   807,   870,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,   281,    -1,    -1,    -1,    -1,   286,
     173,   100,   781,    -1,   517,   104,    -1,    -1,    -1,    -1,
     523,    -1,   111,    -1,    -1,    -1,   864,    -1,    -1,   118,
      -1,   534,   870,   536,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   818,
      -1,   864,    -1,    -1,    -1,    -1,    -1,   870,    -1,    -1,
      -1,   369,    -1,    -1,    -1,    -1,    -1,    -1,   837,    -1,
      -1,     2,    -1,   842,    -1,    -1,   384,   170,    -1,    -1,
     173,    -1,   175,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   369,    -1,    25,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,   607,    -1,    -1,   384,   611,    -1,
     985,    -1,   987,    -1,    -1,    -1,   619,   992,    -1,    -1,
     995,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1004,
      -1,  1006,    -1,    -1,  1009,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   985,    79,   987,
      -1,   920,    -1,    -1,   992,    -1,    -1,   995,    -1,    -1,
      -1,    -1,    -1,  1038,    -1,    -1,  1004,    -1,  1006,    -1,
      -1,  1009,   985,  1048,   987,  1050,   107,    -1,    -1,   992,
      -1,   112,   995,   491,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1004,    -1,  1006,    -1,    -1,  1009,    -1,    -1,    -1,
    1038,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   517,
    1048,    -1,  1050,    -1,   491,   523,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1038,   534,    -1,   536,    -1,
      -1,    -1,    -1,    -1,    -1,  1048,    -1,  1050,    -1,    -1,
     517,    -1,    -1,  1118,    -1,    -1,   523,    -1,    -1,    -1,
    1125,    -1,    -1,    -1,    -1,    -1,    -1,   534,    -1,   536,
      -1,    -1,    -1,  1138,  1033,    -1,    -1,    -1,    -1,    -1,
    1039,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1118,    -1,  1157,    -1,    -1,    -1,   217,  1125,    -1,    -1,
      -1,  1166,    -1,    -1,    -1,  1170,    -1,    -1,    -1,   607,
    1138,   804,    -1,   611,   807,  1118,    -1,    -1,    -1,    -1,
      -1,   619,  1125,    -1,    -1,    -1,    -1,    -1,    -1,  1157,
      -1,    -1,    -1,    -1,    -1,  1138,    -1,    -1,  1166,    -1,
     607,    -1,  1170,    -1,   611,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   619,    -1,  1157,    -1,    -1,    -1,    -1,    49,
     281,    -1,  1227,  1166,    -1,   286,    -1,  1170,    -1,    -1,
      -1,   864,    -1,    -1,    -1,    -1,    -1,   870,    -1,    -1,
      -1,    -1,    -1,    73,    22,    -1,    -1,    -1,    26,    27,
    1149,    -1,    -1,    83,    84,    -1,    -1,    -1,    36,  1227,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,  1283,  1284,
    1285,  1286,    -1,    -1,  1227,    -1,    -1,  1292,  1293,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    71,   369,    -1,
      -1,    89,    -1,    91,    -1,  1283,  1284,  1285,  1286,    -1,
      -1,    -1,    -1,   384,  1292,  1293,    -1,    -1,    -1,    93,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
    1283,  1284,  1285,  1286,   108,    -1,    -1,    73,    -1,  1292,
    1293,    -1,   985,    -1,   987,    -1,    -1,    -1,    -1,   992,
      -1,    -1,   995,    -1,    -1,    -1,   804,    -1,    -1,   807,
      -1,  1004,    -1,  1006,    -1,    -1,  1009,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   222,    -1,    -1,    -1,    -1,   804,    -1,    -1,
     807,    -1,    -1,    -1,    -1,  1038,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1048,    -1,  1050,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   864,    -1,    -1,    -1,
     491,    -1,   870,    -1,    -1,   265,   266,   267,   268,   269,
     270,    -1,    -1,   273,   274,    -1,    -1,    -1,    -1,    -1,
     280,    -1,    -1,    -1,    -1,    -1,   517,   864,    -1,   289,
      -1,   291,   523,   870,    -1,    -1,    -1,    -1,   298,    -1,
     234,    -1,    -1,   534,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1118,    -1,    -1,    -1,    -1,
      -1,    -1,  1125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   343,   278,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1157,    -1,    -1,    -1,   358,   359,
      -1,    -1,    -1,  1166,    -1,    -1,    -1,  1170,   302,   265,
     266,   267,   268,   269,   270,    -1,   607,   985,    -1,   987,
     611,    -1,    -1,    -1,   992,    -1,    -1,   995,   619,    -1,
      -1,    -1,    -1,   289,    -1,   291,  1004,    -1,  1006,    -1,
      -1,  1009,   298,    -1,    -1,    -1,    -1,    -1,   985,    -1,
     987,    -1,    -1,   347,    -1,   992,    -1,    -1,   995,    -1,
      -1,    -1,    -1,    -1,  1227,    -1,    -1,  1004,    -1,  1006,
    1038,    -1,  1009,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1048,    -1,  1050,    -1,    -1,    -1,    -1,   343,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1038,   358,   359,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1048,    -1,  1050,    -1,    -1,    -1,    -1,    -1,    -1,
    1283,  1284,  1285,  1286,    -1,    -1,    -1,    30,    -1,  1292,
    1293,    -1,    -1,    -1,   494,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,    -1,    48,    -1,    -1,    -1,    -1,
    1118,    -1,    -1,    -1,    -1,    -1,    -1,  1125,    -1,    -1,
      -1,    -1,   522,    -1,    -1,    -1,   526,    -1,   528,    -1,
    1138,   531,    -1,   467,   468,    -1,    -1,    -1,    -1,   473,
      -1,  1118,   476,   477,   478,   479,    -1,    -1,  1125,  1157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1166,    -1,
      -1,  1138,  1170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   804,    -1,    -1,   807,    -1,    -1,    -1,
    1157,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1166,
      -1,    -1,    -1,  1170,    -1,    -1,    -1,    -1,   494,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   542,   152,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1227,
      -1,    -1,   165,   166,    -1,    -1,   522,    -1,    -1,    -1,
     526,    -1,   528,   864,    -1,   531,    -1,    -1,    -1,   870,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1227,     3,    -1,   587,    -1,    -1,    -1,    -1,   201,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,   603,
      -1,   605,    -1,    25,    -1,  1283,  1284,  1285,  1286,    -1,
      -1,    -1,    34,    -1,  1292,  1293,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,  1283,  1284,  1285,  1286,
      -1,    63,    -1,    -1,    -1,  1292,  1293,    -1,    70,    71,
     263,    -1,    -1,    -1,    -1,    -1,    78,    -1,   271,   272,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,   282,
      -1,    -1,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,
     102,    -1,   104,    -1,   985,   107,   987,    -1,    -1,   111,
      -1,   992,   114,    -1,   995,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,  1004,    -1,  1006,    -1,    -1,  1009,    -1,
      -1,   324,    -1,    -1,    -1,    -1,    -1,    -1,   788,   789,
      -1,    -1,    -1,    -1,   794,   795,    -1,    -1,   798,   799,
      -1,    -1,    -1,    -1,    -1,    -1,    22,  1038,   808,    -1,
      26,    27,    -1,    -1,    -1,    -1,    -1,  1048,    -1,  1050,
      36,   173,    38,    39,    -1,    -1,    -1,   761,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   781,    -1,   783,
      66,    -1,    -1,   396,   854,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    -1,    -1,    83,    -1,    85,
      86,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,
     423,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   822,    -1,
      -1,    -1,   788,   789,   110,    -1,    -1,   113,   794,   795,
      -1,   117,   798,   799,    -1,    -1,    -1,  1138,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1157,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,  1166,    -1,    -1,    -1,  1170,
      -1,    -1,    -1,    -1,   487,    -1,    48,    -1,    -1,    -1,
     166,    -1,   495,    -1,    -1,    -1,    -1,    -1,   501,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   906,    -1,    -1,   975,   976,    -1,    -1,    -1,
      -1,    -1,   982,    -1,   984,    -1,    -1,    -1,    90,    -1,
      -1,   991,    94,    -1,   994,    -1,  1227,   997,    -1,   999,
     543,  1001,    -1,    -1,   547,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   555,    -1,  1014,    -1,    -1,   560,    -1,    -1,
     122,    -1,   956,    -1,   126,    -1,    -1,    -1,   571,    -1,
      -1,    -1,   575,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,  1046,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,   975,
     976,    -1,   625,    -1,    -1,    -1,   982,    -1,    -1,    -1,
       4,     5,     6,     7,     8,     9,    10,    -1,   994,    -1,
      14,   997,    -1,   999,    18,  1001,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,  1159,
      -1,   704,    76,    -1,    78,    79,    80,    81,    82,    -1,
      -1,    -1,    -1,    87,    -1,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    99,    -1,   101,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    12,    -1,    -1,
     114,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,
      -1,    -1,   126,  1147,  1148,    -1,    -1,    -1,    -1,    34,
      -1,  1221,  1222,    -1,    -1,  1225,  1226,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,  1239,
      55,  1241,    -1,  1243,    -1,    -1,   160,    -1,    63,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
     174,   175,    -1,  1159,   178,    -1,   809,    -1,    -1,    -1,
      -1,    -1,    -1,   816,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   828,    -1,    -1,    -1,   104,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   854,   127,   128,   129,   130,   131,   132,    -1,    -1,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,   144,
     145,   146,   147,    -1,   149,    -1,    -1,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
      -1,    -1,   167,   168,   169,   170,   171,    -1,    -1,    -1,
     903,   904,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,   931,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
    1013,   104,   105,    -1,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
    1043,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,     1,   178,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,   105,    -1,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,   173,    -1,   175,    -1,
       1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,    -1,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
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
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
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
      89,    -1,    91,    92,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
      -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
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
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,   105,    -1,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      81,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,    -1,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
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
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
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
      89,    -1,    91,    92,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
      -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
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
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,   105,    -1,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,    -1,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
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
      85,    86,    87,    88,    89,    -1,    91,    92,    93,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,   108,   109,   110,   111,    -1,   113,   114,
     115,   116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
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
      89,    -1,    91,    92,    93,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
      -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      83,    84,    85,    86,    87,    88,    89,    -1,    91,    92,
      93,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
      -1,   104,   105,    -1,   107,   108,   109,   110,   111,    -1,
     113,   114,   115,   116,   117,   118,    -1,    -1,   121,    -1,
     123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
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
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,    -1,   104,   105,    -1,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    -1,
      91,    92,    93,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,    -1,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,   176,     1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,     3,   178,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,
      19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,   132,    -1,    -1,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,   144,   145,   146,   147,    -1,
     149,    -1,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    -1,   167,   168,
     169,   170,   171,    -1,   173,    -1,   175,    -1,     3,   178,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    -1,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    62,    63,    64,
      65,    -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,    -1,   104,
     105,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,   121,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
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
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,   108,
     109,    -1,   111,   112,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     3,   178,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
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
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,   108,
     109,    -1,   111,   112,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,   174,   175,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,   112,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,   174,   175,    -1,     3,   178,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    48,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,   108,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,   108,
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     3,   178,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    69,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,   108,   109,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     3,   178,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     3,   178,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
     117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
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
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,   176,    -1,   178,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,   172,   173,    -1,
     175,    -1,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,   176,     3,   178,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    90,    -1,    -1,    -1,    94,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
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
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,   124,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,     3,   178,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
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
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,   113,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
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
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
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
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
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
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,   112,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
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
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,   112,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
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
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
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
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
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
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
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
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
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
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
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
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
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
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,   178,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    18,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    79,    80,    81,    82,    -1,
      -1,    -1,    -1,    87,    -1,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    12,    -1,    -1,   151,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,   160,    25,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    34,   171,    36,   173,
      -1,   175,    40,    -1,   178,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    -1,   100,    -1,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,
     128,   129,   130,   131,   132,    -1,    -1,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
      -1,   149,    -1,    -1,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,   164,    -1,    -1,   167,
     168,   169,   170,    -1,     3,   173,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,    -1,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   144,    -1,
      -1,    -1,   148,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,   171,    -1,   173,   174,   175,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    61,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,   106,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,   120,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   144,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,   165,   166,    25,    -1,    -1,    -1,   171,    -1,
     173,    -1,   175,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,   144,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,   165,    -1,    25,    -1,    -1,    -1,
     171,    -1,   173,    -1,   175,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   144,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,   165,    -1,    25,    -1,
      -1,    -1,   171,    -1,   173,    -1,   175,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    17,    18,
     107,    20,    -1,    -1,   111,    -1,    25,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    88,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,    -1,   104,    17,    18,   107,    20,
      -1,    -1,   111,    -1,    25,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,   171,    -1,   173,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,   100,
      -1,   102,    -1,   104,    -1,    -1,   107,     3,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    12,   118,    -1,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,   173,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    97,    98,    -1,   100,    -1,   102,    19,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      32,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,   131,   132,    -1,    -1,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,    -1,   149,    -1,    -1,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,   164,    -1,
      -1,   167,   168,   169,   170,    -1,    12,    -1,    90,    -1,
      -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    90,   167,   168,   169,    94,   171,
      -1,    12,    -1,    -1,    -1,    -1,   178,   103,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,   131,   132,    -1,    -1,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,    90,
      -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
     131,   132,    -1,    -1,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
     171,    -1,    12,    -1,    -1,    -1,    -1,   178,   103,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,
      90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,   171,    -1,    12,    -1,    -1,    -1,    -1,   178,   103,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,   130,   131,   132,    -1,
      -1,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,    12,
      -1,    90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,   132,    -1,    -1,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    90,   167,   168,
     169,    94,   171,    -1,    12,    -1,    -1,    -1,    -1,   178,
     103,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,   132,
      -1,    -1,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,
      12,    -1,    90,    -1,    -1,   178,    94,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    90,   167,
     168,   169,    94,   171,    -1,    12,    -1,    -1,    -1,    -1,
     178,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
     122,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   171,
      -1,    -1,    -1,    90,    -1,    -1,   178,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   126,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    73,    -1,
     167,   168,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,   126,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      73,   166,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    12,    -1,    90,    -1,    -1,
      -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    90,   167,   168,   169,    94,    -1,    -1,
      12,    -1,    -1,    -1,    -1,   178,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,   122,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    12,    -1,    90,    -1,
      -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    48,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    90,   167,   168,   169,    94,   171,
      -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,   122,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    -1,    -1,    12,    -1,    90,
      -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
     171,    -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    12,    -1,
      90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,    -1,    -1,    12,    -1,    -1,    -1,    -1,   178,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,    12,
      -1,    90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    90,   167,   168,
     169,    94,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,
      -1,    32,    -1,   126,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    73,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,   178,    -1,    12,    13,    90,
      -1,    -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    32,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
      -1,    -1,    -1,    12,    -1,    -1,    -1,   178,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   126,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,    90,    -1,   178,    -1,    94,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    12,   167,   168,   169,    -1,   171,   126,    19,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    12,   166,   167,   168,
     169,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,   142,   143,    -1,   145,   146,   147,   103,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
     126,    -1,    12,    13,    90,    -1,    -1,   178,    94,    19,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    32,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
     126,   167,   168,   169,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   178,    19,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    32,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      90,   167,   168,   169,    94,    -1,    -1,    -1,   174,    -1,
      -1,    12,   178,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,   122,   167,   168,   169,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    90,
      -1,    -1,    -1,    94,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      12,   167,   168,   169,    -1,   126,    -1,    19,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,   142,   143,    35,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    12,   166,   167,   168,   169,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,   126,    -1,
      -1,    90,    -1,    -1,    12,    94,   178,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    32,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   126,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,   174,    -1,    -1,    -1,
     178,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,    90,    -1,    -1,   174,    94,    12,    13,   178,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,    90,    -1,    12,    -1,    94,
     178,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    90,    -1,    -1,    12,    94,
      -1,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    90,    -1,    12,   174,
      94,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    32,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    90,    -1,    12,    13,
      94,    -1,    -1,    -1,    -1,    19,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    32,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   126,   167,   168,   169,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   126,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,   176,    -1,   178,    19,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    32,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   126,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,   178,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    32,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,    -1,    19,    -1,    61,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    32,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   126,   167,   168,   169,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   126,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    32,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   126,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,   176,    -1,   178,    19,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    32,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    12,    -1,
      94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   126,   167,   168,   169,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   126,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    22,   167,   168,   169,    26,    27,    -1,    -1,
      -1,    31,    -1,    -1,   178,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,    22,    -1,    -1,    -1,    26,
      27,    51,    -1,    -1,    31,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    83,    -1,    -1,    86,    -1,    -1,    89,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    86,
     110,    -1,    89,   113,    91,   115,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,    22,
     117,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,   166,    -1,    51,    -1,
      -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,   166,
      -1,    -1,    -1,    -1,    77,   172,    79,    -1,    81,    -1,
      83,    32,    -1,    86,    -1,    -1,    89,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,    -1,   117,   142,   143,    -1,   145,   146,
     147,    32,   149,   150,   151,    -1,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    90,
     167,   168,   169,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   166,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,   162,    -1,   164,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,   162,    -1,   164,    -1,    22,   167,   168,   169,    26,
      27,    -1,    -1,    -1,    31,    -1,    -1,   178,    -1,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,     1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      77,    25,    79,    -1,    81,    -1,    83,    31,    -1,    86,
      34,    -1,    89,    -1,    91,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,    63,
     117,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    97,    98,    -1,   100,    25,   102,    27,
     104,    -1,    -1,   107,    -1,    -1,    34,   111,    -1,   166,
     114,   115,    40,    -1,   118,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    91,    -1,    -1,    -1,    -1,    -1,    97,
      98,    -1,   100,    -1,   102,    -1,   104,     3,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
     118,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      17,    18,    78,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    97,    98,    40,   100,    -1,   102,    -1,   104,    46,
      -1,   107,    -1,    50,    -1,   111,    53,    -1,   114,    -1,
      -1,    -1,   118,    -1,    -1,     3,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    17,
      18,    78,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      97,    98,    40,   100,    -1,   102,    -1,   104,    46,    -1,
     107,    -1,    50,    -1,   111,    53,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,     3,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    17,    18,
      78,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    97,
      98,    40,   100,    -1,   102,    -1,   104,    46,    -1,   107,
      -1,    50,    -1,   111,    53,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118
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
      79,    80,    81,    82,    83,    86,    87,    88,    89,    91,
      92,    93,    95,    96,    97,    98,    99,   100,   101,   102,
     104,   105,   107,   108,   109,   110,   111,   113,   114,   115,
     117,   118,   121,   123,   126,   142,   143,   144,   145,   146,
     151,   160,   163,   164,   165,   166,   169,   171,   173,   175,
     178,   185,   186,   187,   188,   189,   192,   193,   196,   197,
     198,   200,   201,   202,   207,   208,   211,   212,   216,   218,
     221,   228,   231,   232,   233,   234,   235,   236,   239,   240,
     242,   244,   247,   248,   249,   250,   251,   255,   256,   261,
     262,   263,   264,   267,   268,   279,   280,   282,   283,   288,
     292,   293,   295,   296,   322,   323,   327,   328,   353,   354,
     355,   356,   357,   358,   359,   367,   368,   369,   370,   371,
     372,   374,   377,   378,   379,   380,   381,   382,   383,   384,
     386,   387,   388,   389,   390,   166,    22,    36,    41,    42,
      45,    56,    89,   102,   171,   175,   247,   280,   353,   358,
     368,   369,   374,   377,   379,   380,   122,   360,   361,     3,
     213,   374,   360,   374,   110,   329,    91,   213,   187,   347,
     374,     8,   191,   187,   173,     3,    17,    18,    20,    25,
      34,    40,    46,    50,    53,    63,    70,    71,    78,    88,
      97,    98,   100,   102,   104,   107,   111,   114,   118,   214,
     215,   217,    11,    79,   124,   241,   374,   241,   374,   241,
     374,    27,   117,   243,   374,    83,    86,   198,   173,   215,
     215,   215,   173,   215,   330,   331,    33,   202,   220,   374,
     259,   260,   374,   374,    18,    78,    97,   114,   374,   374,
     374,     8,   173,   230,   229,     4,    27,    34,    48,    50,
      52,    76,    79,    91,    98,   102,   110,   175,   186,   219,
     284,   285,   286,   308,   309,   310,   317,   342,   343,   354,
     317,   346,   374,   108,   109,   166,   288,   289,   373,   374,
     376,   374,   220,   374,   374,   374,   101,   173,   187,   374,
     374,   188,   202,   188,   202,   374,     8,   195,   243,   374,
     376,   374,   374,   374,   374,   374,     1,   172,   185,   203,
     346,   112,   152,   317,   348,   349,   376,   241,   346,   374,
     385,   374,    81,   187,   171,    47,    85,   116,   199,    26,
     328,   374,     8,   263,   374,   375,    56,   147,   265,   215,
       1,    31,   115,   215,   269,   272,   275,   278,   173,   304,
      54,    74,    84,   312,    27,    79,    91,   110,   313,    27,
      79,    91,   110,   311,   215,   324,   325,   330,   165,   166,
     374,    12,    19,    32,    90,    94,   126,   142,   143,   145,
     146,   147,   149,   150,   151,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   167,   168,   169,
     178,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   170,   300,   173,   175,
      90,    94,   374,   374,   374,   215,   346,    56,   173,   187,
     166,   171,   202,    48,   360,   327,   166,   148,   166,   190,
      22,    36,    39,    89,   192,   198,   121,   216,   317,   350,
     351,   352,   376,   171,   215,   173,   220,    33,    48,   220,
     122,   220,   363,    33,    48,   220,   363,   220,   363,    48,
     220,   363,   215,   215,   103,   202,   103,   127,   202,   300,
     199,   350,   173,   173,   202,   187,   112,   173,   215,   332,
     333,     1,   147,   339,    48,   148,   187,   220,   148,   220,
      13,   173,   173,   220,   350,   231,   231,    48,    91,   176,
     346,   309,   310,   174,   148,   173,   215,   147,   148,   166,
     374,   374,   125,   290,   166,   171,   220,   173,   350,   166,
     252,   252,   194,   192,   220,   220,   166,   172,   172,   185,
     148,   172,   374,   148,   174,   148,   174,   176,   363,    48,
     148,   176,   363,   125,   148,   176,     8,     1,   172,   203,
     209,   210,   374,   205,   374,    66,    37,    73,   166,   263,
     265,   110,   237,   293,   215,   346,   171,   172,     8,   274,
       8,   277,   127,   148,   172,   174,   305,   307,   308,   313,
      91,     1,   147,   337,   338,    91,     1,     3,    12,    17,
      19,    20,    25,    46,    53,    55,    63,    71,    88,   100,
     104,   111,   118,   142,   143,   144,   145,   146,   147,   149,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     164,   167,   168,   169,   170,   173,   215,   297,   298,   299,
     300,   353,   127,   326,   148,   166,   166,   374,   374,   374,
     241,   374,   241,   374,   374,   374,   374,   374,   374,   374,
       3,    20,    34,    63,   104,   110,   216,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,    69,   376,   376,   376,   376,   376,
     350,   350,   241,   374,   241,   374,    33,    48,    33,    48,
     103,   173,    48,   176,   215,    27,    48,    91,   117,   362,
     365,   366,   374,   390,   241,   374,   220,   330,   374,   192,
     374,   127,   174,   148,    48,   346,    45,   374,   241,   374,
     173,   220,    45,   374,   241,   374,   220,   220,   241,   374,
     220,   127,   127,   187,    35,   187,   374,    35,   374,    66,
     174,   351,   215,   245,   246,   333,   148,   174,   175,   219,
     340,   354,   127,   334,   374,   331,   259,   215,   327,   374,
     374,   174,   317,   341,   342,   354,    48,   176,   215,   313,
     286,   333,   147,   344,   345,   343,   317,   374,   291,   201,
       1,   257,   352,   174,    21,   253,   192,   172,   174,   174,
     348,   174,   348,   187,   176,   241,   374,   176,   187,   374,
     176,   374,   176,   374,   172,   172,   148,   166,    13,   150,
     148,   166,    13,    37,    73,   215,   142,   143,   144,   145,
     146,   160,   164,   169,   204,   299,   300,   301,   374,   204,
     206,   265,   171,   238,   327,   166,   173,     1,   266,   273,
     278,   276,   278,   374,   270,   271,   148,   174,   339,     1,
     175,   219,   335,   336,   354,   106,   314,   173,   303,   374,
     142,   150,   303,   303,   340,   354,   325,   173,   175,   166,
     166,   166,   166,   166,   166,   174,   176,    45,   241,   374,
      45,   241,   374,   351,   241,   374,   173,    48,    91,   148,
     174,    17,    20,    25,    46,    53,    63,    71,    88,   100,
     111,   118,   353,    90,    90,   360,   360,   166,   166,   317,
     376,   352,   374,   174,   374,    33,   220,    33,   220,   364,
     365,   374,    33,   220,   363,    33,   220,   363,   220,   363,
     220,   363,   374,   374,    35,   187,    35,    35,   187,   103,
     202,   215,   174,   148,   174,   174,   332,   339,     1,   346,
      69,   376,   215,   174,   174,   374,   341,   342,   345,   147,
     287,   174,   343,   152,   302,   334,   374,   172,    75,   119,
     172,   258,   174,   173,   202,   215,   254,   187,   176,   363,
     176,   363,   187,   125,   209,   216,   171,   301,   374,   112,
     374,   204,   206,   148,   166,    13,   166,   171,   266,   324,
     330,   351,   172,    31,    83,    86,   115,   172,   186,   222,
     225,   228,   278,   278,   272,   275,   307,    61,   120,   318,
       1,   176,   346,   318,   305,   306,   174,   298,   300,   174,
     176,   374,    33,    33,   374,    33,    33,   174,   176,   176,
     351,   365,   339,   353,   353,   220,   220,   220,   103,    45,
     374,    45,   374,   148,   174,   103,    45,   374,   220,    45,
     374,   220,   220,   220,   187,   187,   374,   187,    35,   166,
     166,   246,   202,   334,   176,    48,   176,   173,   173,   176,
     302,   334,   374,   314,   345,   317,   374,    33,   187,   346,
     254,   147,   202,    45,   187,   374,   176,    45,   187,   374,
     176,   374,   204,    13,    37,    73,    37,    73,   166,   166,
     301,   374,   374,   266,   172,   166,   166,   174,     8,   224,
       8,   227,   222,   225,     3,    93,   104,   319,   320,   321,
     374,   281,   176,   335,   354,   176,   294,   174,   303,   303,
     103,    45,    45,   103,    45,    45,    45,    45,   174,   334,
     374,   374,   374,   365,   374,   374,   374,    35,   187,   374,
     340,   354,   350,   350,   341,   302,   334,   187,   220,   174,
     374,   374,   187,   374,   187,   172,   112,   374,   204,   206,
     204,   206,    13,   172,   166,   223,   228,   226,   228,   321,
     120,   148,   127,   153,   155,   156,   158,   159,    61,   201,
     221,   316,   335,   354,   166,   315,   316,   374,   374,   374,
     374,   374,   374,   103,   103,   103,   103,   187,   176,   174,
     174,   202,   103,   103,   166,   166,   166,   166,   374,   228,
     228,   374,   320,   321,   321,   321,   321,   321,   321,   319,
     103,   103,   103,   103,   103,   103,   374,   374,   374,   374,
     354,   165,   165,   374,   374
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
     214,   214,   214,   215,   215,   216,   216,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   219,   219,
     219,   219,   220,   220,   221,   221,   222,   223,   222,   224,
     222,   226,   225,   227,   225,   228,   228,   229,   228,   230,
     228,   231,   231,   231,   231,   231,   231,   231,   232,   232,
     232,   232,   233,   234,   234,   235,   236,   236,   236,   237,
     236,   238,   236,   239,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   241,
     242,   242,   242,   242,   242,   242,   242,   242,   242,   242,
     242,   242,   243,   243,   244,   244,   245,   245,   246,   247,
     247,   247,   247,   247,   247,   247,   247,   247,   247,   247,
     247,   247,   248,   248,   248,   248,   248,   248,   249,   249,
     249,   250,   250,   250,   251,   251,   251,   251,   252,   252,
     253,   253,   253,   254,   254,   255,   256,   256,   257,   257,
     258,   258,   258,   259,   259,   259,   260,   260,   261,   262,
     262,   263,   264,   264,   264,   265,   265,   266,   266,   266,
     266,   266,   267,   267,   268,   269,   269,   270,   269,   269,
     271,   269,   272,   273,   272,   274,   272,   276,   275,   277,
     275,   278,   278,   279,   281,   280,   282,   283,   283,   283,
     284,   284,   285,   285,   286,   286,   286,   287,   287,   288,
     290,   289,   291,   289,   292,   294,   293,   295,   295,   295,
     295,   295,   296,   297,   297,   298,   298,   298,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   299,   299,
     299,   299,   299,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   301,   301,   302,   302,   302,
     303,   303,   304,   304,   305,   305,   306,   306,   307,   308,
     308,   308,   308,   308,   308,   309,   309,   310,   310,   310,
     310,   310,   310,   310,   310,   310,   311,   311,   311,   311,
     311,   311,   312,   312,   312,   313,   313,   313,   313,   313,
     313,   314,   314,   315,   315,   316,   316,   317,   318,   318,
     318,   318,   318,   319,   319,   320,   320,   320,   320,   320,
     320,   320,   321,   321,   322,   323,   323,   324,   324,   325,
     326,   326,   326,   327,   327,   327,   327,   327,   329,   328,
     328,   330,   330,   331,   331,   332,   332,   332,   333,   333,
     333,   334,   334,   334,   335,   335,   335,   335,   335,   335,
     335,   336,   336,   336,   336,   337,   337,   338,   338,   339,
     339,   339,   339,   339,   340,   340,   340,   340,   341,   341,
     341,   342,   342,   342,   342,   342,   343,   343,   343,   343,
     344,   345,   345,   346,   346,   346,   346,   347,   347,   348,
     348,   348,   349,   349,   350,   350,   351,   351,   352,   352,
     352,   352,   353,   353,   354,   354,   354,   354,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     355,   355,   355,   355,   355,   355,   355,   355,   355,   355,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   357,   358,   359,
     359,   359,   359,   359,   359,   359,   360,   360,   361,   362,
     362,   363,   364,   364,   365,   365,   365,   366,   366,   366,
     366,   366,   366,   367,   367,   367,   367,   367,   368,   368,
     368,   368,   368,   369,   370,   370,   370,   370,   370,   370,
     371,   372,   373,   373,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   375,   375,   376,   376,
     376,   377,   377,   377,   377,   378,   378,   378,   378,   378,
     379,   379,   379,   380,   380,   380,   380,   380,   380,   381,
     381,   381,   381,   382,   382,   383,   383,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   384,   384,
     385,   385,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   386,   386,   387,   387,   387,   387,   387,
     387,   387,   388,   388,   388,   388,   389,   389,   389,   389,
     390,   390,   390,   390,   390,   390,   390
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
       1,     1,     2,     1,     2,     3,     1,     0,     4,     0,
       3,     0,     4,     0,     3,     1,     1,     0,     3,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     5,
       5,     2,     1,     1,     1,     1,     6,     7,     3,     0,
       6,     0,     6,     2,     5,     3,     3,     6,     6,     4,
       5,     5,     3,     3,     6,     5,     6,     5,     6,     3,
       4,     3,     4,     5,     6,     5,     6,     3,     4,     3,
       4,     6,     7,     6,     7,     4,     5,     4,     5,     4,
       4,     3,     6,     5,     4,     3,     6,     5,     6,     5,
       8,     7,     4,     4,     6,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     6,     4,
       7,     5,     3,     6,     2,     2,     3,     3,     0,     2,
       2,     3,     5,     1,     3,     3,     5,     5,     0,     2,
       3,     2,     3,     4,     3,     1,     1,     3,     3,     5,
       5,     2,     1,     1,     1,     0,     2,     0,     2,     2,
       3,     3,     3,     3,     3,     1,     2,     0,     4,     1,
       0,     4,     1,     0,     4,     0,     3,     0,     4,     0,
       3,     1,     3,     1,     0,     7,     0,     1,     1,     1,
       0,     1,     1,     3,     1,     3,     1,     0,     2,     6,
       0,     3,     0,     4,     1,     0,     7,     4,     4,     6,
       6,     4,     2,     1,     3,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       0,     3,     2,     3,     1,     3,     0,     1,     1,     4,
       5,     4,     5,     6,     6,     0,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     1,     0,     1,     2,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     1,     0,     2,
       2,     4,     4,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     3,     1,     2,     1,     3,     2,
       0,     2,     2,     1,     2,     1,     1,     1,     0,     5,
       3,     1,     3,     3,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     3,     2,     4,     3,     3,     4,
       3,     1,     1,     1,     1,     2,     1,     0,     1,     0,
       2,     2,     2,     1,     4,     4,     6,     3,     0,     1,
       1,     3,     4,     3,     4,     6,     1,     1,     1,     1,
       2,     0,     1,     1,     1,     3,     3,     1,     3,     1,
       1,     1,     3,     3,     0,     1,     1,     3,     3,     3,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     4,     4,     4,     5,     2,     2,
       1,     2,     1,     2,     1,     2,     1,     2,     1,     1,
       6,     6,     4,     9,     9,     7,     6,     6,     4,     9,
       9,     7,     4,     6,     6,     9,     9,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     4,     1,
       3,     4,     1,     3,     4,     3,     3,     1,     1,     2,
       1,     2,     1,     1,     2,     2,     2,     2,     2,     8,
       8,     9,     9,     4,     3,     3,     2,     2,     2,     1,
       3,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     2,     2,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     1,
       4,     4,     4,     3,     3,     3,     3,     5,     5,     3,
       4,     3,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     3,     4,     3,     4,
       3,     5,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       1,     1,     1,     1,     1,     1,     1
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
#line 630 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6568 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 634 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6574 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 635 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6580 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 642 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6588 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 650 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6596 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 654 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 662 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6611 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: unstable_decl_stmt  */
#line 663 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6617 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: include_module_stmt  */
#line 664 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6623 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: block_stmt  */
#line 665 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6629 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: use_stmt  */
#line 666 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6635 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: import_stmt  */
#line 667 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6641 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: require_stmt  */
#line 668 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: extern_block_stmt  */
#line 669 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6653 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 670 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6659 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 671 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6665 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: TDEFER stmt  */
#line 673 "chpl.ypp"
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
#line 6682 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: try_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6688 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: return_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6694 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 688 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6707 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 697 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6720 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TLABEL ident_def stmt  */
#line 706 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6728 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: TYIELD opt_try_expr TSEMI  */
#line 710 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6739 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: error TSEMI  */
#line 717 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6748 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: assignment_stmt  */
#line 724 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6754 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: if_stmt  */
#line 725 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6760 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: loop_stmt  */
#line 726 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6766 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: select_stmt  */
#line 727 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6772 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: manage_stmt  */
#line 728 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6778 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 729 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6784 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: throw_stmt  */
#line 730 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6790 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 732 "chpl.ypp"
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
#line 6808 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 746 "chpl.ypp"
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
#line 6827 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 761 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6839 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 769 "chpl.ypp"
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
#line 6855 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL do_stmt  */
#line 781 "chpl.ypp"
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
#line 6870 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TON expr do_stmt  */
#line 792 "chpl.ypp"
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
#line 6886 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 804 "chpl.ypp"
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
#line 6902 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL do_stmt  */
#line 816 "chpl.ypp"
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
#line 6917 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSYNC stmt  */
#line 827 "chpl.ypp"
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
#line 6934 "bison-chpl-lib.cpp"
    break;

  case 45: /* $@1: %empty  */
#line 844 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6942 "bison-chpl-lib.cpp"
    break;

  case 46: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 decl_base  */
#line 848 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6950 "bison-chpl-lib.cpp"
    break;

  case 47: /* $@2: %empty  */
#line 852 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6958 "bison-chpl-lib.cpp"
    break;

  case 48: /* deprecated_decl_stmt: TDEPRECATED $@2 decl_base  */
#line 856 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6966 "bison-chpl-lib.cpp"
    break;

  case 51: /* $@3: %empty  */
#line 868 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 6974 "bison-chpl-lib.cpp"
    break;

  case 52: /* unstable_decl_stmt: TUNSTABLE STRINGLITERAL $@3 decl_base  */
#line 872 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6982 "bison-chpl-lib.cpp"
    break;

  case 53: /* $@4: %empty  */
#line 876 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 6990 "bison-chpl-lib.cpp"
    break;

  case 54: /* unstable_decl_stmt: TUNSTABLE $@4 decl_base  */
#line 880 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6998 "bison-chpl-lib.cpp"
    break;

  case 55: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 887 "chpl.ypp"
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
#line 7019 "bison-chpl-lib.cpp"
    break;

  case 56: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 907 "chpl.ypp"
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
#line 7038 "bison-chpl-lib.cpp"
    break;

  case 57: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 922 "chpl.ypp"
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
#line 7057 "bison-chpl-lib.cpp"
    break;

  case 58: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 937 "chpl.ypp"
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
#line 7074 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_access_control: %empty  */
#line 952 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 7080 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_access_control: TPUBLIC  */
#line 953 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 7087 "bison-chpl-lib.cpp"
    break;

  case 61: /* opt_access_control: TPRIVATE  */
#line 955 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7094 "bison-chpl-lib.cpp"
    break;

  case 62: /* opt_prototype: %empty  */
#line 960 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7100 "bison-chpl-lib.cpp"
    break;

  case 63: /* opt_prototype: TPROTOTYPE  */
#line 961 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7107 "bison-chpl-lib.cpp"
    break;

  case 64: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 967 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7117 "bison-chpl-lib.cpp"
    break;

  case 65: /* block_stmt_body: TLCBR TRCBR  */
#line 985 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7125 "bison-chpl-lib.cpp"
    break;

  case 66: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 989 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7133 "bison-chpl-lib.cpp"
    break;

  case 67: /* block_stmt_body: TLCBR error TRCBR  */
#line 993 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7145 "bison-chpl-lib.cpp"
    break;

  case 68: /* block_stmt: block_stmt_body  */
#line 1004 "chpl.ypp"
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
#line 7164 "bison-chpl-lib.cpp"
    break;

  case 69: /* stmt_ls: toplevel_stmt  */
#line 1022 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7170 "bison-chpl-lib.cpp"
    break;

  case 70: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1023 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7176 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: expr  */
#line 1028 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7184 "bison-chpl-lib.cpp"
    break;

  case 72: /* renames_ls: all_op_name  */
#line 1032 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7192 "bison-chpl-lib.cpp"
    break;

  case 73: /* renames_ls: expr TAS expr  */
#line 1036 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7201 "bison-chpl-lib.cpp"
    break;

  case 74: /* renames_ls: renames_ls TCOMMA expr  */
#line 1041 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7209 "bison-chpl-lib.cpp"
    break;

  case 75: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1045 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7217 "bison-chpl-lib.cpp"
    break;

  case 76: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1049 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7226 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: expr  */
#line 1059 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7235 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_renames_ls: expr TAS expr  */
#line 1064 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7245 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1070 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7256 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1077 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7265 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1082 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7276 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1089 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7288 "bison-chpl-lib.cpp"
    break;

  case 83: /* opt_only_ls: %empty  */
#line 1099 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7294 "bison-chpl-lib.cpp"
    break;

  case 84: /* opt_only_ls: renames_ls  */
#line 1100 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7300 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1105 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7309 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1110 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7321 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1118 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7334 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1127 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7348 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1137 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7360 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1145 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7373 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1154 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7387 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1167 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7396 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_expr: expr  */
#line 1175 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7404 "bison-chpl-lib.cpp"
    break;

  case 94: /* import_expr: expr TDOT all_op_name  */
#line 1179 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7413 "bison-chpl-lib.cpp"
    break;

  case 95: /* import_expr: expr TAS ident_use  */
#line 1184 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7423 "bison-chpl-lib.cpp"
    break;

  case 96: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1190 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7433 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_ls: import_expr  */
#line 1198 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7439 "bison-chpl-lib.cpp"
    break;

  case 98: /* import_ls: import_ls TCOMMA import_expr  */
#line 1199 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7445 "bison-chpl-lib.cpp"
    break;

  case 99: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1204 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7455 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1213 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7463 "bison-chpl-lib.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1217 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7471 "bison-chpl-lib.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1221 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7479 "bison-chpl-lib.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1225 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 104: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1229 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 105: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1233 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7503 "bison-chpl-lib.cpp"
    break;

  case 106: /* opt_label_ident: %empty  */
#line 1241 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7509 "bison-chpl-lib.cpp"
    break;

  case 107: /* opt_label_ident: TIDENT  */
#line 1242 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7515 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_use: TIDENT  */
#line 1267 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7521 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_use: TTHIS  */
#line 1268 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7527 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TBOOL  */
#line 1295 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7533 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TENUM  */
#line 1296 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7539 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TINT  */
#line 1297 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7545 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TUINT  */
#line 1298 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7551 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TREAL  */
#line 1299 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7557 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TIMAG  */
#line 1300 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7563 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TCOMPLEX  */
#line 1301 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7569 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TBYTES  */
#line 1302 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7575 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TSTRING  */
#line 1303 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7581 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TLOCALE  */
#line 1304 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7587 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TNOTHING  */
#line 1305 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7593 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TVOID  */
#line 1306 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7599 "bison-chpl-lib.cpp"
    break;

  case 152: /* do_stmt: TDO stmt  */
#line 1320 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7605 "bison-chpl-lib.cpp"
    break;

  case 153: /* do_stmt: block_stmt  */
#line 1321 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7611 "bison-chpl-lib.cpp"
    break;

  case 154: /* return_stmt: TRETURN TSEMI  */
#line 1326 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7622 "bison-chpl-lib.cpp"
    break;

  case 155: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1333 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7633 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@5: %empty  */
#line 1344 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7641 "bison-chpl-lib.cpp"
    break;

  case 158: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@5 class_level_stmt  */
#line 1348 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7649 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@6: %empty  */
#line 1352 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7657 "bison-chpl-lib.cpp"
    break;

  case 160: /* deprecated_class_level_stmt: TDEPRECATED $@6 class_level_stmt  */
#line 1356 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7665 "bison-chpl-lib.cpp"
    break;

  case 161: /* $@7: %empty  */
#line 1363 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 7673 "bison-chpl-lib.cpp"
    break;

  case 162: /* unstable_class_level_stmt: TUNSTABLE STRINGLITERAL $@7 class_level_stmt  */
#line 1367 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7681 "bison-chpl-lib.cpp"
    break;

  case 163: /* $@8: %empty  */
#line 1371 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 7689 "bison-chpl-lib.cpp"
    break;

  case 164: /* unstable_class_level_stmt: TUNSTABLE $@8 class_level_stmt  */
#line 1375 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7697 "bison-chpl-lib.cpp"
    break;

  case 165: /* class_level_stmt: TSEMI  */
#line 1382 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7705 "bison-chpl-lib.cpp"
    break;

  case 166: /* class_level_stmt: inner_class_level_stmt  */
#line 1386 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7715 "bison-chpl-lib.cpp"
    break;

  case 167: /* $@9: %empty  */
#line 1391 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7722 "bison-chpl-lib.cpp"
    break;

  case 168: /* class_level_stmt: TPUBLIC $@9 inner_class_level_stmt  */
#line 1393 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7731 "bison-chpl-lib.cpp"
    break;

  case 169: /* $@10: %empty  */
#line 1397 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7738 "bison-chpl-lib.cpp"
    break;

  case 170: /* class_level_stmt: TPRIVATE $@10 inner_class_level_stmt  */
#line 1399 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7747 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1417 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 179: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1422 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7765 "bison-chpl-lib.cpp"
    break;

  case 180: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1427 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7774 "bison-chpl-lib.cpp"
    break;

  case 181: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1432 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7782 "bison-chpl-lib.cpp"
    break;

  case 182: /* forwarding_decl_start: TFORWARDING  */
#line 1439 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7791 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_or_export: TEXTERN  */
#line 1446 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7797 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_or_export: TEXPORT  */
#line 1447 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7803 "bison-chpl-lib.cpp"
    break;

  case 185: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1452 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7813 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1462 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7822 "bison-chpl-lib.cpp"
    break;

  case 187: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1468 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7833 "bison-chpl-lib.cpp"
    break;

  case 188: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1475 "chpl.ypp"
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
#line 7855 "bison-chpl-lib.cpp"
    break;

  case 189: /* $@11: %empty  */
#line 1493 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7863 "bison-chpl-lib.cpp"
    break;

  case 190: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1497 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7872 "bison-chpl-lib.cpp"
    break;

  case 191: /* $@12: %empty  */
#line 1502 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7881 "bison-chpl-lib.cpp"
    break;

  case 192: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@12 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1507 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7890 "bison-chpl-lib.cpp"
    break;

  case 193: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1515 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7898 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1522 "chpl.ypp"
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
#line 7920 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TWHILE expr do_stmt  */
#line 1540 "chpl.ypp"
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
#line 7937 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1553 "chpl.ypp"
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
#line 7954 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1566 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7962 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1570 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7970 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1574 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7978 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1578 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7986 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1582 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7994 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOR expr do_stmt  */
#line 1586 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8002 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1590 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8010 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1594 "chpl.ypp"
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
#line 8032 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1612 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8040 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1616 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8048 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1620 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8056 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1624 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8064 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFORALL expr do_stmt  */
#line 1628 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8072 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1632 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8080 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1636 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8088 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1640 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8096 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1644 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8104 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1648 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8112 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1652 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8120 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1656 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8128 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1660 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8136 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1664 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8144 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1668 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8152 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1672 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8160 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1676 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8168 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1680 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8176 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1684 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8184 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1688 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8192 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1692 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8200 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1696 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8208 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1700 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8217 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1705 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8226 "bison-chpl-lib.cpp"
    break;

  case 229: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1713 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8236 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF expr TTHEN stmt  */
#line 1722 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8244 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF expr block_stmt  */
#line 1726 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8252 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1730 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8260 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1734 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8268 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1738 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8276 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF ifvar block_stmt  */
#line 1742 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8284 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1746 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8292 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1750 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8300 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1754 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8309 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1759 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8318 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1764 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8327 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1769 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8336 "bison-chpl-lib.cpp"
    break;

  case 242: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1777 "chpl.ypp"
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
#line 8355 "bison-chpl-lib.cpp"
    break;

  case 243: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1792 "chpl.ypp"
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
#line 8374 "bison-chpl-lib.cpp"
    break;

  case 244: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1810 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8382 "bison-chpl-lib.cpp"
    break;

  case 245: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1814 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8390 "bison-chpl-lib.cpp"
    break;

  case 246: /* ifc_formal_ls: ifc_formal  */
#line 1820 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8396 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1821 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8402 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifc_formal: ident_def  */
#line 1826 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8408 "bison-chpl-lib.cpp"
    break;

  case 261: /* implements_type_ident: implements_type_error_ident  */
#line 1844 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8420 "bison-chpl-lib.cpp"
    break;

  case 268: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1873 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8428 "bison-chpl-lib.cpp"
    break;

  case 269: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1877 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8436 "bison-chpl-lib.cpp"
    break;

  case 270: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1881 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8444 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1888 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8452 "bison-chpl-lib.cpp"
    break;

  case 272: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1892 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8460 "bison-chpl-lib.cpp"
    break;

  case 273: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1896 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8468 "bison-chpl-lib.cpp"
    break;

  case 274: /* try_stmt: TTRY tryable_stmt  */
#line 1903 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8476 "bison-chpl-lib.cpp"
    break;

  case 275: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1907 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8484 "bison-chpl-lib.cpp"
    break;

  case 276: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1911 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8492 "bison-chpl-lib.cpp"
    break;

  case 277: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1915 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8500 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_ls: %empty  */
#line 1921 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8506 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1922 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8512 "bison-chpl-lib.cpp"
    break;

  case 280: /* catch_expr: TCATCH block_stmt  */
#line 1927 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8520 "bison-chpl-lib.cpp"
    break;

  case 281: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1931 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8528 "bison-chpl-lib.cpp"
    break;

  case 282: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1935 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8536 "bison-chpl-lib.cpp"
    break;

  case 283: /* catch_expr_inner: ident_def  */
#line 1942 "chpl.ypp"
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
#line 8553 "bison-chpl-lib.cpp"
    break;

  case 284: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1955 "chpl.ypp"
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
#line 8570 "bison-chpl-lib.cpp"
    break;

  case 285: /* throw_stmt: TTHROW expr TSEMI  */
#line 1971 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8580 "bison-chpl-lib.cpp"
    break;

  case 286: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1979 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8588 "bison-chpl-lib.cpp"
    break;

  case 287: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1983 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8598 "bison-chpl-lib.cpp"
    break;

  case 288: /* when_stmt_ls: %empty  */
#line 1991 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8604 "bison-chpl-lib.cpp"
    break;

  case 289: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1992 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8610 "bison-chpl-lib.cpp"
    break;

  case 290: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1997 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8618 "bison-chpl-lib.cpp"
    break;

  case 291: /* when_stmt: TOTHERWISE stmt  */
#line 2001 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8627 "bison-chpl-lib.cpp"
    break;

  case 292: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2006 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8636 "bison-chpl-lib.cpp"
    break;

  case 293: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2014 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8645 "bison-chpl-lib.cpp"
    break;

  case 294: /* manager_expr: expr TAS ident_def  */
#line 2019 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8653 "bison-chpl-lib.cpp"
    break;

  case 295: /* manager_expr: expr  */
#line 2023 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8661 "bison-chpl-lib.cpp"
    break;

  case 296: /* manager_expr_ls: manager_expr  */
#line 2029 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8667 "bison-chpl-lib.cpp"
    break;

  case 297: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2030 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8673 "bison-chpl-lib.cpp"
    break;

  case 298: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2035 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8681 "bison-chpl-lib.cpp"
    break;

  case 299: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2044 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8690 "bison-chpl-lib.cpp"
    break;

  case 300: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2049 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8701 "bison-chpl-lib.cpp"
    break;

  case 301: /* class_start: class_tag ident_def  */
#line 2060 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8709 "bison-chpl-lib.cpp"
    break;

  case 302: /* class_tag: TCLASS  */
#line 2066 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8715 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_tag: TRECORD  */
#line 2067 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8721 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_tag: TUNION  */
#line 2068 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8727 "bison-chpl-lib.cpp"
    break;

  case 305: /* opt_inherit: %empty  */
#line 2072 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8733 "bison-chpl-lib.cpp"
    break;

  case 306: /* opt_inherit: TCOLON expr_ls  */
#line 2073 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8739 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_level_stmt_ls: %empty  */
#line 2077 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8748 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2082 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8756 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_level_stmt_ls: class_level_stmt_ls unstable_class_level_stmt  */
#line 2086 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2090 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8772 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls unstable_class_level_stmt  */
#line 2094 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8780 "bison-chpl-lib.cpp"
    break;

  case 312: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2101 "chpl.ypp"
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
#line 8801 "bison-chpl-lib.cpp"
    break;

  case 313: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2118 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8815 "bison-chpl-lib.cpp"
    break;

  case 314: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2131 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8823 "bison-chpl-lib.cpp"
    break;

  case 315: /* enum_ls: deprecated_enum_item  */
#line 2138 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 316: /* enum_ls: enum_ls TCOMMA  */
#line 2143 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8842 "bison-chpl-lib.cpp"
    break;

  case 317: /* $@13: %empty  */
#line 2149 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8851 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_ls: enum_ls TCOMMA $@13 deprecated_enum_item  */
#line 2154 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_ls: unstable_enum_item  */
#line 2160 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8869 "bison-chpl-lib.cpp"
    break;

  case 320: /* $@14: %empty  */
#line 2165 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8878 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_ls TCOMMA $@14 unstable_enum_item  */
#line 2170 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8887 "bison-chpl-lib.cpp"
    break;

  case 323: /* $@15: %empty  */
#line 2180 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8895 "bison-chpl-lib.cpp"
    break;

  case 324: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@15 enum_item  */
#line 2184 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8903 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@16: %empty  */
#line 2188 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8911 "bison-chpl-lib.cpp"
    break;

  case 326: /* deprecated_enum_item: TDEPRECATED $@16 enum_item  */
#line 2192 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8919 "bison-chpl-lib.cpp"
    break;

  case 327: /* $@17: %empty  */
#line 2199 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 8927 "bison-chpl-lib.cpp"
    break;

  case 328: /* unstable_enum_item: TUNSTABLE STRINGLITERAL $@17 enum_item  */
#line 2203 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8935 "bison-chpl-lib.cpp"
    break;

  case 329: /* $@18: %empty  */
#line 2207 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 8943 "bison-chpl-lib.cpp"
    break;

  case 330: /* unstable_enum_item: TUNSTABLE $@18 enum_item  */
#line 2211 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8951 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_item: ident_def  */
#line 2218 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8962 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_item: ident_def TASSIGN expr  */
#line 2225 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8975 "bison-chpl-lib.cpp"
    break;

  case 333: /* lambda_decl_start: TLAMBDA  */
#line 2237 "chpl.ypp"
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
#line 8991 "bison-chpl-lib.cpp"
    break;

  case 334: /* $@19: %empty  */
#line 2253 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 9001 "bison-chpl-lib.cpp"
    break;

  case 335: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_lifetime_where $@19 function_body_stmt  */
#line 2259 "chpl.ypp"
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
#line 9023 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: linkage_spec_empty  */
#line 2282 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 9029 "bison-chpl-lib.cpp"
    break;

  case 338: /* linkage_spec: TINLINE  */
#line 2283 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 339: /* linkage_spec: TOVERRIDE  */
#line 2285 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 9043 "bison-chpl-lib.cpp"
    break;

  case 340: /* opt_fn_type_formal_ls: %empty  */
#line 2290 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 9049 "bison-chpl-lib.cpp"
    break;

  case 341: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2291 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9055 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal_ls: fn_type_formal  */
#line 2295 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9061 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2296 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9067 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: named_formal  */
#line 2301 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9073 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2304 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 9079 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type_formal: formal_type  */
#line 2306 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 9085 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_ret_type: %empty  */
#line 2312 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 9091 "bison-chpl-lib.cpp"
    break;

  case 348: /* opt_fn_type_ret_type: TCOLON expr  */
#line 2313 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9097 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2327 "chpl.ypp"
  {
    FunctionParts fp = context->makeFunctionParts(false, false);
    fp.kind = Function::PROC;
    fp.name = context->buildIdent((yylsp[-5]), STR("proc"));
    fp.formals = (yyvsp[-4].exprList);
    fp.returnIntent = (yyvsp[-2].returnTag);
    fp.returnType = (yyvsp[-1].expr);
    fp.throws = ((yyvsp[0].throwsTag) == ThrowsTag_THROWS) ? true : false;
    fp.body = nullptr;
    fp.comments = nullptr;
    fp.attributes = nullptr;
    fp.visibility = context->visibility;
    (yyval.functionParts) = fp;
  }
#line 9116 "bison-chpl-lib.cpp"
    break;

  case 350: /* $@20: %empty  */
#line 2346 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_expr: fn_type $@20 block_stmt_body  */
#line 2352 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9139 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@21: %empty  */
#line 2361 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9149 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_expr: fn_type TALIAS $@21 expr  */
#line 2367 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2379 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9170 "bison-chpl-lib.cpp"
    break;

  case 355: /* $@22: %empty  */
#line 2388 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 9184 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@22 opt_function_body_stmt  */
#line 2398 "chpl.ypp"
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
#line 9207 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2420 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9219 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2428 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9231 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2436 "chpl.ypp"
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
#line 9246 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2447 "chpl.ypp"
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
#line 9261 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2458 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9271 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2467 "chpl.ypp"
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
#line 9287 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2482 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9293 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_ident: ident_def TBANG  */
#line 2489 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9303 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2546 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9309 "bison-chpl-lib.cpp"
    break;

  case 408: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2547 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9315 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2548 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9321 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_formal_ls: %empty  */
#line 2552 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9327 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2553 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9333 "bison-chpl-lib.cpp"
    break;

  case 412: /* req_formal_ls: TLP TRP  */
#line 2557 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 9339 "bison-chpl-lib.cpp"
    break;

  case 413: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2558 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9345 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls_inner: formal  */
#line 2562 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9351 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2563 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9357 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_ls: %empty  */
#line 2567 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9363 "bison-chpl-lib.cpp"
    break;

  case 417: /* formal_ls: formal_ls_inner  */
#line 2568 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9369 "bison-chpl-lib.cpp"
    break;

  case 419: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2577 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type opt_init_expr  */
#line 2581 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9385 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2585 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9393 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: pragma_ls opt_formal_intent_tag ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2589 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2594 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9409 "bison-chpl-lib.cpp"
    break;

  case 424: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2599 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
  }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_formal_intent_tag: %empty  */
#line 2605 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9426 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_formal_intent_tag: required_intent_tag  */
#line 2610 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 427: /* required_intent_tag: TIN  */
#line 2617 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TINOUT  */
#line 2618 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TOUT  */
#line 2619 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TCONST TIN  */
#line 2620 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TCONST TREF  */
#line 2621 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TCONST  */
#line 2622 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TPARAM  */
#line 2623 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TREF  */
#line 2624 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9483 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TTYPE  */
#line 2625 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9489 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_this_intent_tag: %empty  */
#line 2629 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9495 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: TPARAM  */
#line 2630 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9501 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TREF  */
#line 2631 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9507 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TCONST TREF  */
#line 2632 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9513 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TCONST  */
#line 2633 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9519 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TTYPE  */
#line 2634 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9525 "bison-chpl-lib.cpp"
    break;

  case 442: /* proc_iter_or_op: TPROC  */
#line 2638 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9531 "bison-chpl-lib.cpp"
    break;

  case 443: /* proc_iter_or_op: TITER  */
#line 2639 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9537 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TOPERATOR  */
#line 2640 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9543 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_ret_tag: %empty  */
#line 2644 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9549 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_ret_tag: TCONST  */
#line 2645 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9555 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: TCONST TREF  */
#line 2646 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TREF  */
#line 2647 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9567 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TPARAM  */
#line 2648 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9573 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TTYPE  */
#line 2649 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9579 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_throws_error: %empty  */
#line 2653 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9585 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_throws_error: TTHROWS  */
#line 2654 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9591 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_function_body_stmt: TSEMI  */
#line 2657 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9597 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_function_body_stmt: function_body_stmt  */
#line 2658 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9603 "bison-chpl-lib.cpp"
    break;

  case 455: /* function_body_stmt: block_stmt_body  */
#line 2662 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9609 "bison-chpl-lib.cpp"
    break;

  case 456: /* function_body_stmt: return_stmt  */
#line 2663 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9615 "bison-chpl-lib.cpp"
    break;

  case 457: /* query_expr: TQUERIEDIDENT  */
#line 2667 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9621 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_lifetime_where: %empty  */
#line 2672 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9627 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_lifetime_where: TWHERE expr  */
#line 2674 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9633 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2676 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9639 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2678 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2680 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9651 "bison-chpl-lib.cpp"
    break;

  case 463: /* lifetime_components_expr: lifetime_expr  */
#line 2685 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 464: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2687 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9663 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2692 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 466: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2694 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9675 "bison-chpl-lib.cpp"
    break;

  case 467: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2696 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9681 "bison-chpl-lib.cpp"
    break;

  case 468: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2698 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9687 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2700 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9693 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2702 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9699 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2704 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9705 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_ident: TIDENT  */
#line 2708 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9711 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_ident: TTHIS  */
#line 2709 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9717 "bison-chpl-lib.cpp"
    break;

  case 474: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2714 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9726 "bison-chpl-lib.cpp"
    break;

  case 475: /* type_alias_decl_stmt_start: TTYPE  */
#line 2723 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9734 "bison-chpl-lib.cpp"
    break;

  case 476: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2727 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9743 "bison-chpl-lib.cpp"
    break;

  case 477: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2735 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9751 "bison-chpl-lib.cpp"
    break;

  case 478: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2739 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9759 "bison-chpl-lib.cpp"
    break;

  case 479: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2746 "chpl.ypp"
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
#line 9781 "bison-chpl-lib.cpp"
    break;

  case 480: /* opt_init_type: %empty  */
#line 2766 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9787 "bison-chpl-lib.cpp"
    break;

  case 481: /* opt_init_type: TASSIGN type_level_expr  */
#line 2768 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9793 "bison-chpl-lib.cpp"
    break;

  case 482: /* opt_init_type: TASSIGN array_type  */
#line 2770 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9802 "bison-chpl-lib.cpp"
    break;

  case 483: /* var_decl_type: TPARAM  */
#line 2777 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9808 "bison-chpl-lib.cpp"
    break;

  case 484: /* var_decl_type: TCONST TREF  */
#line 2778 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9814 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_type: TREF  */
#line 2779 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9820 "bison-chpl-lib.cpp"
    break;

  case 486: /* var_decl_type: TCONST  */
#line 2780 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9826 "bison-chpl-lib.cpp"
    break;

  case 487: /* var_decl_type: TVAR  */
#line 2781 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9832 "bison-chpl-lib.cpp"
    break;

  case 488: /* $@23: %empty  */
#line 2786 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9841 "bison-chpl-lib.cpp"
    break;

  case 489: /* var_decl_stmt: TCONFIG $@23 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2790 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9850 "bison-chpl-lib.cpp"
    break;

  case 490: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2795 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9859 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2803 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9867 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2807 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9875 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2814 "chpl.ypp"
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
#line 9895 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2830 "chpl.ypp"
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
#line 9913 "bison-chpl-lib.cpp"
    break;

  case 495: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2847 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9921 "bison-chpl-lib.cpp"
    break;

  case 496: /* tuple_var_decl_component: ident_def  */
#line 2851 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9929 "bison-chpl-lib.cpp"
    break;

  case 497: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2855 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9937 "bison-chpl-lib.cpp"
    break;

  case 498: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2862 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9943 "bison-chpl-lib.cpp"
    break;

  case 499: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2864 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9949 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2866 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9955 "bison-chpl-lib.cpp"
    break;

  case 501: /* opt_init_expr: %empty  */
#line 2872 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9961 "bison-chpl-lib.cpp"
    break;

  case 502: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2873 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9967 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2874 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9973 "bison-chpl-lib.cpp"
    break;

  case 504: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2879 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9981 "bison-chpl-lib.cpp"
    break;

  case 505: /* ret_array_type: TLSBR TRSBR  */
#line 2883 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 9989 "bison-chpl-lib.cpp"
    break;

  case 506: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2887 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9997 "bison-chpl-lib.cpp"
    break;

  case 507: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2891 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 10005 "bison-chpl-lib.cpp"
    break;

  case 508: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2895 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10013 "bison-chpl-lib.cpp"
    break;

  case 509: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2899 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10021 "bison-chpl-lib.cpp"
    break;

  case 510: /* ret_array_type: TLSBR error TRSBR  */
#line 2903 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "invalid expression for domain of array return type");
  }
#line 10029 "bison-chpl-lib.cpp"
    break;

  case 511: /* ret_type: type_level_expr  */
#line 2909 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 10035 "bison-chpl-lib.cpp"
    break;

  case 512: /* ret_type: ret_array_type  */
#line 2910 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 10041 "bison-chpl-lib.cpp"
    break;

  case 513: /* ret_type: reserved_type_ident_use  */
#line 2911 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10047 "bison-chpl-lib.cpp"
    break;

  case 514: /* ret_type: error  */
#line 2912 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10053 "bison-chpl-lib.cpp"
    break;

  case 515: /* colon_ret_type: TCOLON ret_type  */
#line 2916 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10059 "bison-chpl-lib.cpp"
    break;

  case 516: /* colon_ret_type: error  */
#line 2917 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10065 "bison-chpl-lib.cpp"
    break;

  case 517: /* opt_ret_type: %empty  */
#line 2921 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 10071 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_type: %empty  */
#line 2926 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 10077 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_type: TCOLON type_level_expr  */
#line 2927 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10083 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_type: TCOLON array_type  */
#line 2928 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10089 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2929 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10095 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: error  */
#line 2930 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10101 "bison-chpl-lib.cpp"
    break;

  case 524: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2951 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10109 "bison-chpl-lib.cpp"
    break;

  case 525: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2955 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10117 "bison-chpl-lib.cpp"
    break;

  case 526: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2959 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10125 "bison-chpl-lib.cpp"
    break;

  case 527: /* array_type: TLSBR error TRSBR  */
#line 2963 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 10133 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_formal_array_elt_type: %empty  */
#line 2969 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 10139 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_formal_array_elt_type: type_level_expr  */
#line 2970 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10145 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_formal_array_elt_type: query_expr  */
#line 2971 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10151 "bison-chpl-lib.cpp"
    break;

  case 531: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2976 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10160 "bison-chpl-lib.cpp"
    break;

  case 532: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2981 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10168 "bison-chpl-lib.cpp"
    break;

  case 533: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2989 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10177 "bison-chpl-lib.cpp"
    break;

  case 534: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2994 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10185 "bison-chpl-lib.cpp"
    break;

  case 535: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2998 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10193 "bison-chpl-lib.cpp"
    break;

  case 538: /* formal_type: reserved_type_ident_use  */
#line 3006 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10199 "bison-chpl-lib.cpp"
    break;

  case 540: /* colon_formal_type: TCOLON formal_type  */
#line 3011 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10205 "bison-chpl-lib.cpp"
    break;

  case 541: /* opt_colon_formal_type: %empty  */
#line 3015 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 10211 "bison-chpl-lib.cpp"
    break;

  case 542: /* opt_colon_formal_type: colon_formal_type  */
#line 3016 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10217 "bison-chpl-lib.cpp"
    break;

  case 543: /* expr_ls: expr  */
#line 3022 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10223 "bison-chpl-lib.cpp"
    break;

  case 544: /* expr_ls: query_expr  */
#line 3023 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10229 "bison-chpl-lib.cpp"
    break;

  case 545: /* expr_ls: expr_ls TCOMMA expr  */
#line 3024 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10235 "bison-chpl-lib.cpp"
    break;

  case 546: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 3025 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10241 "bison-chpl-lib.cpp"
    break;

  case 547: /* simple_expr_ls: expr  */
#line 3029 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10247 "bison-chpl-lib.cpp"
    break;

  case 548: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 3030 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10253 "bison-chpl-lib.cpp"
    break;

  case 549: /* tuple_component: TUNDERSCORE  */
#line 3034 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10259 "bison-chpl-lib.cpp"
    break;

  case 550: /* tuple_component: opt_try_expr  */
#line 3035 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10265 "bison-chpl-lib.cpp"
    break;

  case 551: /* tuple_component: query_expr  */
#line 3036 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10271 "bison-chpl-lib.cpp"
    break;

  case 552: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3041 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10279 "bison-chpl-lib.cpp"
    break;

  case 553: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3045 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10287 "bison-chpl-lib.cpp"
    break;

  case 554: /* opt_actual_ls: %empty  */
#line 3051 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10293 "bison-chpl-lib.cpp"
    break;

  case 555: /* opt_actual_ls: actual_ls  */
#line 3052 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10299 "bison-chpl-lib.cpp"
    break;

  case 556: /* actual_ls: actual_expr  */
#line 3057 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10308 "bison-chpl-lib.cpp"
    break;

  case 557: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3062 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10318 "bison-chpl-lib.cpp"
    break;

  case 558: /* actual_expr: ident_use TASSIGN query_expr  */
#line 3070 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10324 "bison-chpl-lib.cpp"
    break;

  case 559: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3071 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10330 "bison-chpl-lib.cpp"
    break;

  case 560: /* actual_expr: query_expr  */
#line 3072 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10336 "bison-chpl-lib.cpp"
    break;

  case 561: /* actual_expr: opt_try_expr  */
#line 3073 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10342 "bison-chpl-lib.cpp"
    break;

  case 562: /* ident_expr: ident_use  */
#line 3077 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10348 "bison-chpl-lib.cpp"
    break;

  case 563: /* ident_expr: scalar_type  */
#line 3078 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10354 "bison-chpl-lib.cpp"
    break;

  case 564: /* type_level_expr: sub_type_level_expr  */
#line 3090 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 10360 "bison-chpl-lib.cpp"
    break;

  case 565: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 3092 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10366 "bison-chpl-lib.cpp"
    break;

  case 566: /* type_level_expr: TQUESTION  */
#line 3094 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10372 "bison-chpl-lib.cpp"
    break;

  case 567: /* type_level_expr: fn_type  */
#line 3096 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10378 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TSINGLE expr  */
#line 3108 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10384 "bison-chpl-lib.cpp"
    break;

  case 574: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3110 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10390 "bison-chpl-lib.cpp"
    break;

  case 575: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3112 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10396 "bison-chpl-lib.cpp"
    break;

  case 576: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3114 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10402 "bison-chpl-lib.cpp"
    break;

  case 577: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3116 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10412 "bison-chpl-lib.cpp"
    break;

  case 578: /* sub_type_level_expr: TATOMIC expr  */
#line 3122 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10418 "bison-chpl-lib.cpp"
    break;

  case 579: /* sub_type_level_expr: TSYNC expr  */
#line 3124 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10424 "bison-chpl-lib.cpp"
    break;

  case 580: /* sub_type_level_expr: TOWNED  */
#line 3127 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10430 "bison-chpl-lib.cpp"
    break;

  case 581: /* sub_type_level_expr: TOWNED expr  */
#line 3129 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10436 "bison-chpl-lib.cpp"
    break;

  case 582: /* sub_type_level_expr: TUNMANAGED  */
#line 3131 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10442 "bison-chpl-lib.cpp"
    break;

  case 583: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3133 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10448 "bison-chpl-lib.cpp"
    break;

  case 584: /* sub_type_level_expr: TSHARED  */
#line 3135 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10454 "bison-chpl-lib.cpp"
    break;

  case 585: /* sub_type_level_expr: TSHARED expr  */
#line 3137 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10460 "bison-chpl-lib.cpp"
    break;

  case 586: /* sub_type_level_expr: TBORROWED  */
#line 3139 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10466 "bison-chpl-lib.cpp"
    break;

  case 587: /* sub_type_level_expr: TBORROWED expr  */
#line 3141 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10472 "bison-chpl-lib.cpp"
    break;

  case 588: /* sub_type_level_expr: TCLASS  */
#line 3143 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10478 "bison-chpl-lib.cpp"
    break;

  case 589: /* sub_type_level_expr: TRECORD  */
#line 3145 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10484 "bison-chpl-lib.cpp"
    break;

  case 590: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 3150 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10497 "bison-chpl-lib.cpp"
    break;

  case 591: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3159 "chpl.ypp"
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

  case 592: /* for_expr: TFOR expr TDO expr  */
#line 3168 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10522 "bison-chpl-lib.cpp"
    break;

  case 593: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3176 "chpl.ypp"
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
#line 10540 "bison-chpl-lib.cpp"
    break;

  case 594: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3190 "chpl.ypp"
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
#line 10558 "bison-chpl-lib.cpp"
    break;

  case 595: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3204 "chpl.ypp"
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
#line 10576 "bison-chpl-lib.cpp"
    break;

  case 596: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3218 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10589 "bison-chpl-lib.cpp"
    break;

  case 597: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3227 "chpl.ypp"
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

  case 598: /* for_expr: TFORALL expr TDO expr  */
#line 3236 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10614 "bison-chpl-lib.cpp"
    break;

  case 599: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3244 "chpl.ypp"
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
#line 10632 "bison-chpl-lib.cpp"
    break;

  case 600: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3258 "chpl.ypp"
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
#line 10650 "bison-chpl-lib.cpp"
    break;

  case 601: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3272 "chpl.ypp"
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
#line 10668 "bison-chpl-lib.cpp"
    break;

  case 602: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 3286 "chpl.ypp"
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
#line 10689 "bison-chpl-lib.cpp"
    break;

  case 603: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3303 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10702 "bison-chpl-lib.cpp"
    break;

  case 604: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3312 "chpl.ypp"
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

  case 605: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3321 "chpl.ypp"
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
#line 10734 "bison-chpl-lib.cpp"
    break;

  case 606: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3336 "chpl.ypp"
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
#line 10753 "bison-chpl-lib.cpp"
    break;

  case 607: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3354 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10767 "bison-chpl-lib.cpp"
    break;

  case 608: /* nil_expr: TNIL  */
#line 3371 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10773 "bison-chpl-lib.cpp"
    break;

  case 616: /* opt_task_intent_ls: %empty  */
#line 3389 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10779 "bison-chpl-lib.cpp"
    break;

  case 617: /* opt_task_intent_ls: task_intent_clause  */
#line 3390 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10785 "bison-chpl-lib.cpp"
    break;

  case 618: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3395 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10795 "bison-chpl-lib.cpp"
    break;

  case 619: /* task_intent_ls: intent_expr  */
#line 3403 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10801 "bison-chpl-lib.cpp"
    break;

  case 620: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3404 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10807 "bison-chpl-lib.cpp"
    break;

  case 621: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3409 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10817 "bison-chpl-lib.cpp"
    break;

  case 622: /* forall_intent_ls: intent_expr  */
#line 3417 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10823 "bison-chpl-lib.cpp"
    break;

  case 623: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3418 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10829 "bison-chpl-lib.cpp"
    break;

  case 624: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3423 "chpl.ypp"
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
#line 10848 "bison-chpl-lib.cpp"
    break;

  case 625: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3438 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10856 "bison-chpl-lib.cpp"
    break;

  case 626: /* intent_expr: expr TREDUCE ident_expr  */
#line 3442 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10864 "bison-chpl-lib.cpp"
    break;

  case 627: /* task_var_prefix: TCONST  */
#line 3448 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10870 "bison-chpl-lib.cpp"
    break;

  case 628: /* task_var_prefix: TIN  */
#line 3449 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10876 "bison-chpl-lib.cpp"
    break;

  case 629: /* task_var_prefix: TCONST TIN  */
#line 3450 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10882 "bison-chpl-lib.cpp"
    break;

  case 630: /* task_var_prefix: TREF  */
#line 3451 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10888 "bison-chpl-lib.cpp"
    break;

  case 631: /* task_var_prefix: TCONST TREF  */
#line 3452 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10894 "bison-chpl-lib.cpp"
    break;

  case 632: /* task_var_prefix: TVAR  */
#line 3453 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10900 "bison-chpl-lib.cpp"
    break;

  case 633: /* new_maybe_decorated: TNEW  */
#line 3458 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10906 "bison-chpl-lib.cpp"
    break;

  case 634: /* new_maybe_decorated: TNEW TOWNED  */
#line 3460 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10912 "bison-chpl-lib.cpp"
    break;

  case 635: /* new_maybe_decorated: TNEW TSHARED  */
#line 3462 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10918 "bison-chpl-lib.cpp"
    break;

  case 636: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3464 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10924 "bison-chpl-lib.cpp"
    break;

  case 637: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3466 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10930 "bison-chpl-lib.cpp"
    break;

  case 638: /* new_expr: new_maybe_decorated expr  */
#line 3472 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10938 "bison-chpl-lib.cpp"
    break;

  case 639: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3479 "chpl.ypp"
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
#line 10954 "bison-chpl-lib.cpp"
    break;

  case 640: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3491 "chpl.ypp"
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
#line 10970 "bison-chpl-lib.cpp"
    break;

  case 641: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3503 "chpl.ypp"
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
#line 10987 "bison-chpl-lib.cpp"
    break;

  case 642: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3516 "chpl.ypp"
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
#line 11005 "bison-chpl-lib.cpp"
    break;

  case 643: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3533 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 11013 "bison-chpl-lib.cpp"
    break;

  case 644: /* range_literal_expr: expr TDOTDOT expr  */
#line 3540 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 11022 "bison-chpl-lib.cpp"
    break;

  case 645: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3545 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 11031 "bison-chpl-lib.cpp"
    break;

  case 646: /* range_literal_expr: expr TDOTDOT  */
#line 3550 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 11040 "bison-chpl-lib.cpp"
    break;

  case 647: /* range_literal_expr: TDOTDOT expr  */
#line 3555 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 11050 "bison-chpl-lib.cpp"
    break;

  case 648: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3561 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 11060 "bison-chpl-lib.cpp"
    break;

  case 649: /* range_literal_expr: TDOTDOT  */
#line 3567 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 11070 "bison-chpl-lib.cpp"
    break;

  case 650: /* cast_expr: expr TCOLON expr  */
#line 3597 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 11078 "bison-chpl-lib.cpp"
    break;

  case 651: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3604 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 11086 "bison-chpl-lib.cpp"
    break;

  case 652: /* super_expr: fn_expr  */
#line 3610 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 11092 "bison-chpl-lib.cpp"
    break;

  case 666: /* opt_expr: %empty  */
#line 3630 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 11098 "bison-chpl-lib.cpp"
    break;

  case 667: /* opt_expr: expr  */
#line 3631 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11104 "bison-chpl-lib.cpp"
    break;

  case 668: /* opt_try_expr: TTRY expr  */
#line 3635 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 11110 "bison-chpl-lib.cpp"
    break;

  case 669: /* opt_try_expr: TTRYBANG expr  */
#line 3636 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 11116 "bison-chpl-lib.cpp"
    break;

  case 670: /* opt_try_expr: super_expr  */
#line 3637 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11122 "bison-chpl-lib.cpp"
    break;

  case 676: /* call_base_expr: expr TBANG  */
#line 3654 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 11130 "bison-chpl-lib.cpp"
    break;

  case 677: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3657 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 11136 "bison-chpl-lib.cpp"
    break;

  case 680: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3664 "chpl.ypp"
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
#line 11152 "bison-chpl-lib.cpp"
    break;

  case 681: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3676 "chpl.ypp"
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
#line 11168 "bison-chpl-lib.cpp"
    break;

  case 682: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3688 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 11176 "bison-chpl-lib.cpp"
    break;

  case 683: /* dot_expr: expr TDOT ident_use  */
#line 3695 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11182 "bison-chpl-lib.cpp"
    break;

  case 684: /* dot_expr: expr TDOT TTYPE  */
#line 3697 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11188 "bison-chpl-lib.cpp"
    break;

  case 685: /* dot_expr: expr TDOT TDOMAIN  */
#line 3699 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11194 "bison-chpl-lib.cpp"
    break;

  case 686: /* dot_expr: expr TDOT TLOCALE  */
#line 3701 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11200 "bison-chpl-lib.cpp"
    break;

  case 687: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3703 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 11210 "bison-chpl-lib.cpp"
    break;

  case 688: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3709 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 11220 "bison-chpl-lib.cpp"
    break;

  case 689: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3721 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 11226 "bison-chpl-lib.cpp"
    break;

  case 690: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3723 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 11234 "bison-chpl-lib.cpp"
    break;

  case 691: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3727 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11242 "bison-chpl-lib.cpp"
    break;

  case 692: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3731 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11250 "bison-chpl-lib.cpp"
    break;

  case 693: /* bool_literal: TFALSE  */
#line 3737 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 11256 "bison-chpl-lib.cpp"
    break;

  case 694: /* bool_literal: TTRUE  */
#line 3738 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 11262 "bison-chpl-lib.cpp"
    break;

  case 695: /* str_bytes_literal: STRINGLITERAL  */
#line 3742 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11268 "bison-chpl-lib.cpp"
    break;

  case 696: /* str_bytes_literal: BYTESLITERAL  */
#line 3743 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11274 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: INTLITERAL  */
#line 3749 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 11280 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: REALLITERAL  */
#line 3750 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11286 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: IMAGLITERAL  */
#line 3751 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11292 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: CSTRINGLITERAL  */
#line 3752 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11298 "bison-chpl-lib.cpp"
    break;

  case 703: /* literal_expr: TNONE  */
#line 3753 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11304 "bison-chpl-lib.cpp"
    break;

  case 704: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3755 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11313 "bison-chpl-lib.cpp"
    break;

  case 705: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3760 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11322 "bison-chpl-lib.cpp"
    break;

  case 706: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3765 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11330 "bison-chpl-lib.cpp"
    break;

  case 707: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3769 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11339 "bison-chpl-lib.cpp"
    break;

  case 708: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3774 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11347 "bison-chpl-lib.cpp"
    break;

  case 709: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3778 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11356 "bison-chpl-lib.cpp"
    break;

  case 710: /* assoc_expr_ls: expr TALIAS expr  */
#line 3787 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11365 "bison-chpl-lib.cpp"
    break;

  case 711: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3792 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11375 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TPLUS expr  */
#line 3800 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11381 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TMINUS expr  */
#line 3801 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11387 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TSTAR expr  */
#line 3802 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11393 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TDIVIDE expr  */
#line 3803 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11399 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3804 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11405 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3805 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11411 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TMOD expr  */
#line 3806 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11417 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TEQUAL expr  */
#line 3807 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11423 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3808 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11429 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3809 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11435 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3810 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11441 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TLESS expr  */
#line 3811 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11447 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TGREATER expr  */
#line 3812 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11453 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TBAND expr  */
#line 3813 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11459 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TBOR expr  */
#line 3814 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11465 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr TBXOR expr  */
#line 3815 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11471 "bison-chpl-lib.cpp"
    break;

  case 728: /* binary_op_expr: expr TAND expr  */
#line 3816 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11477 "bison-chpl-lib.cpp"
    break;

  case 729: /* binary_op_expr: expr TOR expr  */
#line 3817 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11483 "bison-chpl-lib.cpp"
    break;

  case 730: /* binary_op_expr: expr TEXP expr  */
#line 3818 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11489 "bison-chpl-lib.cpp"
    break;

  case 731: /* binary_op_expr: expr TBY expr  */
#line 3819 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11495 "bison-chpl-lib.cpp"
    break;

  case 732: /* binary_op_expr: expr TALIGN expr  */
#line 3820 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11501 "bison-chpl-lib.cpp"
    break;

  case 733: /* binary_op_expr: expr THASH expr  */
#line 3821 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11507 "bison-chpl-lib.cpp"
    break;

  case 734: /* binary_op_expr: expr TDMAPPED expr  */
#line 3822 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11513 "bison-chpl-lib.cpp"
    break;

  case 735: /* unary_op_expr: TPLUS expr  */
#line 3826 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11519 "bison-chpl-lib.cpp"
    break;

  case 736: /* unary_op_expr: TMINUS expr  */
#line 3827 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11525 "bison-chpl-lib.cpp"
    break;

  case 737: /* unary_op_expr: TMINUSMINUS expr  */
#line 3828 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11531 "bison-chpl-lib.cpp"
    break;

  case 738: /* unary_op_expr: TPLUSPLUS expr  */
#line 3829 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11537 "bison-chpl-lib.cpp"
    break;

  case 739: /* unary_op_expr: TBANG expr  */
#line 3830 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11543 "bison-chpl-lib.cpp"
    break;

  case 740: /* unary_op_expr: expr TBANG  */
#line 3831 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11551 "bison-chpl-lib.cpp"
    break;

  case 741: /* unary_op_expr: TBNOT expr  */
#line 3834 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11557 "bison-chpl-lib.cpp"
    break;

  case 742: /* reduce_expr: expr TREDUCE expr  */
#line 3839 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11565 "bison-chpl-lib.cpp"
    break;

  case 743: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3843 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11573 "bison-chpl-lib.cpp"
    break;

  case 744: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3847 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11581 "bison-chpl-lib.cpp"
    break;

  case 745: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3851 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11589 "bison-chpl-lib.cpp"
    break;

  case 746: /* scan_expr: expr TSCAN expr  */
#line 3858 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11597 "bison-chpl-lib.cpp"
    break;

  case 747: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3862 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11605 "bison-chpl-lib.cpp"
    break;

  case 748: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3866 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11613 "bison-chpl-lib.cpp"
    break;

  case 749: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3870 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11621 "bison-chpl-lib.cpp"
    break;


#line 11625 "bison-chpl-lib.cpp"

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
