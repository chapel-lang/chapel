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
  YYSYMBOL_formal_ident_def = 308,         /* formal_ident_def  */
  YYSYMBOL_named_formal = 309,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 310,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 311,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 312,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 313,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 314,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 315,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 316,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 317,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 318,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 319,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 320, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 321,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 322,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 323,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 324, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 325, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 326, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 327,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 328,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 329,            /* var_decl_stmt  */
  YYSYMBOL_330_23 = 330,                   /* $@23  */
  YYSYMBOL_var_decl_stmt_inner_ls = 331,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 332,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 333, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 334, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 335,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 336,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 337,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 338,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 339,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 340,                 /* opt_type  */
  YYSYMBOL_formal_type = 341,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 342,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 343,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 344,                  /* expr_ls  */
  YYSYMBOL_tuple_component = 345,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 346,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 347,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 348,                /* actual_ls  */
  YYSYMBOL_actual_expr = 349,              /* actual_expr  */
  YYSYMBOL_ident_expr = 350,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 351,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 352,                 /* for_expr  */
  YYSYMBOL_bracket_loop_expr = 353,        /* bracket_loop_expr  */
  YYSYMBOL_cond_expr = 354,                /* cond_expr  */
  YYSYMBOL_nil_expr = 355,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 356,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 357,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 358,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 359,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 360,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 361,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 362,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 363,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 364,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 365,                 /* new_expr  */
  YYSYMBOL_let_expr = 366,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 367,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 368,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 369,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 370,               /* super_expr  */
  YYSYMBOL_expr = 371,                     /* expr  */
  YYSYMBOL_opt_expr = 372,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 373,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 374,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 375,           /* call_base_expr  */
  YYSYMBOL_call_expr = 376,                /* call_expr  */
  YYSYMBOL_dot_expr = 377,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 378,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 379,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 380,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 381,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 382,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 383,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 384,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 385,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 386,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 387       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 328 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include helper macros for use in writing productions
  #include "parser-help.h"
  // include implementation of error logging hook
  #include "parser-yyerror.h"

#line 511 "bison-chpl-lib.cpp"

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
#define YYLAST   18176

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  206
/* YYNRULES -- Number of rules.  */
#define YYNRULES  730
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1283

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
       0,   633,   633,   637,   638,   643,   644,   652,   656,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,   674,
     675,   688,   689,   690,   699,   708,   712,   719,   727,   728,
     729,   730,   731,   732,   733,   734,   748,   763,   771,   783,
     794,   806,   818,   829,   845,   847,   846,   855,   854,   865,
     866,   871,   870,   879,   878,   889,   909,   924,   939,   955,
     956,   958,   963,   964,   969,   987,   991,   995,  1006,  1025,
    1026,  1030,  1034,  1038,  1043,  1047,  1051,  1061,  1066,  1072,
    1079,  1084,  1091,  1102,  1103,  1107,  1112,  1120,  1129,  1139,
    1147,  1156,  1169,  1177,  1181,  1186,  1192,  1201,  1202,  1206,
    1215,  1219,  1223,  1227,  1231,  1235,  1244,  1245,  1249,  1250,
    1251,  1252,  1253,  1257,  1258,  1270,  1271,  1276,  1277,  1278,
    1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,
    1289,  1290,  1291,  1292,  1293,  1294,  1298,  1299,  1300,  1301,
    1302,  1303,  1304,  1305,  1306,  1307,  1308,  1309,  1316,  1317,
    1318,  1319,  1323,  1324,  1328,  1335,  1345,  1347,  1346,  1355,
    1354,  1366,  1365,  1374,  1373,  1384,  1388,  1394,  1394,  1400,
    1400,  1409,  1410,  1411,  1412,  1413,  1414,  1415,  1419,  1424,
    1429,  1434,  1441,  1449,  1450,  1454,  1463,  1469,  1477,  1496,
    1495,  1505,  1504,  1517,  1524,  1542,  1555,  1568,  1572,  1576,
    1580,  1584,  1588,  1592,  1596,  1614,  1618,  1622,  1626,  1630,
    1634,  1638,  1642,  1646,  1650,  1654,  1658,  1662,  1666,  1670,
    1674,  1678,  1682,  1686,  1690,  1694,  1698,  1702,  1707,  1715,
    1724,  1728,  1732,  1736,  1740,  1744,  1748,  1752,  1756,  1761,
    1766,  1771,  1779,  1794,  1812,  1816,  1823,  1824,  1829,  1834,
    1835,  1836,  1837,  1838,  1839,  1840,  1841,  1842,  1843,  1844,
    1845,  1846,  1855,  1856,  1857,  1858,  1867,  1868,  1872,  1876,
    1880,  1887,  1891,  1895,  1902,  1906,  1910,  1914,  1921,  1922,
    1926,  1930,  1934,  1941,  1954,  1970,  1978,  1982,  1991,  1992,
    1996,  2000,  2005,  2013,  2018,  2022,  2029,  2030,  2034,  2043,
    2048,  2059,  2066,  2067,  2068,  2072,  2073,  2077,  2081,  2085,
    2089,  2093,  2100,  2117,  2130,  2137,  2142,  2149,  2148,  2159,
    2165,  2164,  2178,  2180,  2179,  2188,  2187,  2199,  2198,  2207,
    2206,  2217,  2224,  2236,  2254,  2251,  2279,  2283,  2284,  2286,
    2291,  2292,  2296,  2297,  2301,  2304,  2306,  2311,  2312,  2323,
    2343,  2342,  2358,  2357,  2375,  2385,  2382,  2416,  2424,  2432,
    2443,  2454,  2463,  2478,  2479,  2483,  2484,  2485,  2494,  2495,
    2496,  2497,  2498,  2499,  2500,  2501,  2502,  2503,  2504,  2505,
    2506,  2507,  2508,  2509,  2510,  2511,  2512,  2513,  2514,  2515,
    2516,  2517,  2518,  2522,  2523,  2524,  2525,  2526,  2527,  2528,
    2529,  2530,  2531,  2532,  2533,  2538,  2539,  2543,  2544,  2548,
    2549,  2553,  2554,  2558,  2559,  2563,  2564,  2568,  2572,  2573,
    2577,  2581,  2586,  2591,  2596,  2601,  2609,  2613,  2621,  2622,
    2623,  2624,  2625,  2626,  2627,  2628,  2629,  2633,  2634,  2635,
    2636,  2637,  2638,  2642,  2643,  2644,  2648,  2649,  2650,  2651,
    2652,  2653,  2657,  2658,  2661,  2662,  2666,  2667,  2671,  2676,
    2677,  2679,  2681,  2683,  2688,  2690,  2695,  2697,  2699,  2701,
    2703,  2705,  2707,  2712,  2713,  2717,  2726,  2730,  2738,  2742,
    2749,  2770,  2771,  2776,  2777,  2778,  2779,  2780,  2785,  2784,
    2793,  2801,  2805,  2812,  2828,  2845,  2849,  2853,  2860,  2862,
    2864,  2871,  2872,  2873,  2877,  2881,  2882,  2883,  2887,  2888,
    2892,  2893,  2897,  2898,  2899,  2900,  2904,  2905,  2909,  2913,
    2914,  2920,  2921,  2925,  2926,  2930,  2934,  2941,  2942,  2946,
    2951,  2960,  2961,  2965,  2966,  2973,  2974,  2975,  2976,  2977,
    2978,  2980,  2982,  2984,  2986,  2992,  2994,  2997,  2999,  3001,
    3003,  3005,  3007,  3009,  3011,  3013,  3015,  3020,  3029,  3038,
    3046,  3060,  3074,  3088,  3097,  3106,  3114,  3128,  3142,  3159,
    3163,  3167,  3171,  3175,  3179,  3183,  3190,  3208,  3216,  3217,
    3218,  3219,  3220,  3221,  3222,  3226,  3227,  3231,  3240,  3241,
    3245,  3254,  3255,  3259,  3273,  3277,  3284,  3285,  3286,  3287,
    3288,  3289,  3293,  3295,  3297,  3299,  3301,  3307,  3314,  3326,
    3338,  3351,  3368,  3375,  3380,  3385,  3390,  3396,  3402,  3432,
    3439,  3446,  3447,  3451,  3453,  3454,  3456,  3458,  3459,  3460,
    3461,  3464,  3465,  3466,  3467,  3468,  3469,  3470,  3471,  3472,
    3476,  3477,  3481,  3482,  3483,  3487,  3488,  3489,  3490,  3499,
    3500,  3503,  3504,  3505,  3509,  3521,  3533,  3540,  3542,  3544,
    3546,  3548,  3554,  3567,  3568,  3572,  3576,  3583,  3584,  3588,
    3589,  3593,  3594,  3595,  3596,  3597,  3598,  3599,  3600,  3605,
    3610,  3614,  3618,  3622,  3630,  3635,  3644,  3645,  3646,  3647,
    3648,  3649,  3650,  3651,  3652,  3653,  3654,  3655,  3656,  3657,
    3658,  3659,  3660,  3661,  3662,  3663,  3664,  3665,  3666,  3670,
    3671,  3672,  3673,  3674,  3675,  3678,  3682,  3686,  3690,  3694,
    3701,  3705,  3709,  3713,  3721,  3722,  3723,  3724,  3725,  3726,
    3727
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
  "formal_ls_inner", "formal_ls", "formal", "formal_ident_def",
  "named_formal", "opt_formal_intent_tag", "required_intent_tag",
  "opt_this_intent_tag", "proc_iter_or_op", "opt_ret_tag",
  "opt_throws_error", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "opt_lifetime_where", "lifetime_components_expr",
  "lifetime_expr", "lifetime_ident", "type_alias_decl_stmt",
  "type_alias_decl_stmt_start", "type_alias_decl_stmt_inner_ls",
  "type_alias_decl_stmt_inner", "opt_init_type", "var_decl_type",
  "var_decl_stmt", "$@23", "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "formal_or_ret_type_expr", "ret_type", "colon_ret_type",
  "opt_ret_type", "opt_type", "formal_type", "colon_formal_type",
  "opt_colon_formal_type", "expr_ls", "tuple_component", "tuple_expr_ls",
  "opt_actual_ls", "actual_ls", "actual_expr", "ident_expr",
  "sub_type_level_expr", "for_expr", "bracket_loop_expr", "cond_expr",
  "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
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

#define YYPACT_NINF (-1068)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-731)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1068,   119,  3595, -1068,   129,   248, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, 10811,   183,   358,   302, 13050,   311, 17940,
     183, 10811,   312,   294,   316,   358,  4827, 10811,   437,  4827,
     275, 17999, -1068,   468,   424,  8347,  9227,  9227, -1068,  8523,
     430,   269,   323, -1068,   433, 17999, 17999, 17999, -1068, 13280,
    9403,   462, 10811, 10811,   236, -1068,   492,   495, 10811, -1068,
   13050, -1068, 10811,   491,   351,   203,   213,  3390,   501, 18058,
   -1068, 10811,  7995, 10811,  9403, 13050, 10811,   408,   511,   399,
    4827,   517, 10811,   520,  6939,  6939, -1068,   524, -1068, 13050,
     573, -1068,   544,  8523,  8699, -1068, 10811, -1068, 10811, -1068,
   -1068, 12877, 10811, -1068, 10811, -1068, -1068, -1068,  3947,  7291,
    8875, 10811, -1068,  4651, -1068, -1068, -1068, -1068, -1068,   435,
   -1068,   204, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068,  7115, -1068,  9579, -1068, -1068,
   -1068, -1068, -1068,   552, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068,   465, 17999, -1068, 17665,   447,   224, -1068,   280,
   -1068, -1068, -1068,   256,   368, -1068, -1068, 17999, 13280, -1068,
     460,   464, -1068, -1068, -1068,   461,   470, 10811,   471,   475,
   -1068, -1068, -1068, 17168,  2238,   225,   476,   479, -1068, -1068,
     365, -1068, -1068, -1068, -1068, -1068,   320, -1068, -1068, -1068,
   10811, 10811, 10811, 17999, -1068, 10811, 10811,  9755,   574,   381,
   -1068, -1068, -1068, -1068, 17168,   404, -1068, -1068,   458,  4827,
   -1068, -1068,   481,     7,   469,  3183, -1068,   221, -1068,   486,
   -1068,   -38, 17168, -1068,  2433,   533,  8699, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068,   484, -1068, -1068, 17999,   485,    60, 14445,    45,
   14284,    45, 14369, 17999, 17999,   140, 13564, -1068, -1068,   572,
    8699,   487,   413,  4827,  1386,    78,    22, -1068,  4827, -1068,
   -1068, 14866, -1068,    53, 15286,   747, -1068,   489,   506, -1068,
   14866,     7,   747, -1068,  8699,  2237,  2237,    75,     1, -1068,
       9, -1068, -1068, -1068, -1068, 10811, 10811, -1068,   523, -1068,
     507,   535, -1068, -1068,  1587,   512, -1068, -1068, 17168,   -37,
   10811, 10811, -1068,   -19, -1068, -1068, 17168,   518, 15461, -1068,
   14866,     7, 17168,   513,  8699, -1068, 17168, 15501, -1068, -1068,
   -1068, -1068,     7, -1068,  2433,    60, 14866,   521,   242,   242,
    2031,   747,   747,    90, -1068, -1068,  4123,   -30, -1068, 10811,
     -31,   101, -1068, 13192,   -35,     8, 15546,   -32,  2031,   685,
   -1068,  4299, 10811, -1068, 10811,   633, -1068, -1068, 14537,   157,
     465, 17168,   443, 17999, 10811,   529, -1068,   530,   696,   697,
     579,   120, -1068, -1068, -1068,   156,   256, -1068, -1068, -1068,
   -1068,   616, -1068, -1068, -1068,    70,   617, -1068, -1068, -1068,
   12706,   582,   197, -1068,   307,   415, -1068,   560, 10811, 10811,
   10811,  9227,  9227, 10811,   416, 10811, 10811, 10811, 10811, 10811,
     117, 12877, 10811, 10811, 10811, 10811, 10811, 10811, 10811, 10811,
   10811, 10811, 10811, 10811, 10811, 10811, 10811, 10811,   643, -1068,
   -1068, -1068, -1068, -1068,  8699,  8699, -1068, -1068, -1068, -1068,
    8699, -1068, -1068,  8699,  8699,  8699,  8699,  9227,  9227, 14951,
   15027, 15654,   540,     2, 17999,  7467, -1068, -1068,  3947, -1068,
    9227,    60, 13280, -1068, 10811, -1068,  2433, -1068, 17999, -1068,
   -1068, -1068,   572, 10811,   587,   542,   569, -1068, -1068, -1068,
     671, 10811, -1068,  5003,  9227, -1068,   547, -1068,    60,  5179,
    9227, -1068,    60, -1068,    60,  9227, -1068,    60,   597,   599,
    4827,   693,  4827, -1068,   695, 10811,   661,   559,  8699, 17999,
   -1068, -1068, -1068,  1386, -1068, -1068,   116, -1068, 10987,   607,
   10811, 13280, -1068, -1068, 10811, -1068, 17724, 10811, 10811, -1068,
     561, -1068, -1068, -1068, -1068, 17793, -1068,   256, 12570, -1068,
    1386, -1068,   589, 10987, -1068, 17168, 17168, -1068,   469, -1068,
      50, -1068,  8699,   564, -1068,   726,   726,  2433, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068,  9931, -1068, 15699,  7643, -1068,
    7819, -1068, 17168,  4827,   575,  9227, 10107,  3771,   578, 10811,
   10283, -1068, -1068,   143, -1068,  4475, -1068,   308, 15739,   310,
   14629, 17999,  6763,  6763, -1068,   465,   577, -1068,   221, -1068,
     178,   602,  3202, -1068, -1068, 17999, -1068, 17999, 10811,   -14,
   -1068, -1068,   139, -1068, -1068,   114, -1068, -1068,  6587, -1068,
     651, -1068,   576,   608, -1068,   611, -1068,   613,   615,   618,
     620, -1068,   621,   624,   630,   631,   632,   637,   638, -1068,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068, 10811,   648,   642,   576, -1068,   576, -1068, 10811, -1068,
   17999, -1068, -1068, 17453, 17453,   392, -1068,   392, -1068,   392,
    1419,  1603,  1999,   840,     7,   242, -1068,   432, -1068, -1068,
   -1068, -1068, -1068,  2031,  2643,   392,  1760,  1760, 17453,  1760,
    1760,  1181,   242,  2643, 17503,  1181,   747,   747,   242,  2031,
     628,   635,   636,   641,   644,   646,   623,   622, -1068,   392,
   -1068,   392, 11163,  9227, 11339,  9227, 10811,  8699,  9227, 13192,
     626,   185, -1068, -1068, -1068,   175, -1068,  2191, 17244,   356,
     183, 15815, -1068,   319, 17168, -1068, 15859,  8699, -1068,  8699,
   10811,   179,  8523, 17168,    67, 15112,  7467, -1068,  8523, 17168,
      59, 14705, -1068, -1068,    45, 14790, -1068, 10811, 10811,   769,
    4827,   774, 15904,  4827, 15188, 17999, -1068,   198, -1068,   200,
   -1068,   214,  1386,    78, -1068, 17168,  8171, -1068, 17168, -1068,
   -1068, -1068, 17999, 16012, 16057, -1068,   589,   666, -1068,   217,
   10987, -1068,   128, -1068, 10811, -1068,   647,   167,   649, -1068,
    2950, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
    4827,   -17, 15232, -1068, -1068, 17168,  4827, 17168, -1068, 16097,
   -1068, -1068, 10811, -1068,    96, 13320, 10811, -1068, 10459,  6763,
    6763, -1068, 10811,   436, 10811,   442,   453, 10811,  9051,   474,
     321, -1068, -1068, -1068, 16217,   667,   650,   653, -1068, 17999,
   13280, -1068,  8699,   654, 17337, 17999, -1068, 17999, -1068, 17168,
   17881,   705,   601, -1068,   651, -1068, -1068, -1068, -1068, -1068,
      41,   522, -1068, 16173, -1068, 13480, -1068, -1068, 17168, -1068,
     655,   652, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   10811,   789, 16262, 10811,   798, 16370,   228,   656, 16415,  8699,
   -1068, -1068,  7467, -1068, -1068, -1068, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068,    71,  9227,  9227,    60,    60,
   -1068, -1068, -1068, -1068, 14866, -1068, 13640,  5355, -1068,  5531,
   -1068,   235, -1068, 13725,  5707, -1068,    60,  5883, -1068,    60,
   -1068,    60, -1068,    60, 17168, 17168,  4827, -1068,  4827, 10811,
   -1068,  4827,   800,   670,   673, 17999,   469, -1068, -1068,   607,
   -1068, -1068, -1068,   660,   669,   128, 10811,   651,   589, -1068,
   10811, -1068, -1068, 17168, -1068,  6059, 10811, -1068, -1068, -1068,
   17999, -1068,   699,   469, -1068,  6235,   672,  6411,   674, -1068,
   10811, -1068, -1068,  6763, -1068, 16455,    95, 15385,   327,   677,
    6763, -1068, 10811, -1068, -1068, 17405,   336,   355,   237, -1068,
     843, -1068, -1068,   844, -1068,  3073, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068,    41,    61, 10811, -1068,   710,   686,
   -1068,   576,   576, -1068, -1068, 16531, 11515, 11691, 16616, 11867,
   12043, -1068, 12219, 12395,   238, -1068,   607,   239,   253, -1068,
   -1068, -1068,  4827,  8523, 17168,  8523, 17168,  7467, -1068,  4827,
    8523, 17168, -1068,  8523, 17168, -1068, -1068, -1068, -1068, -1068,
   17168,   827,  4827, -1068, -1068, -1068, -1068, -1068,  8699,  8699,
   -1068, -1068, -1068, -1068,   128, 17168,  4827, -1068,    81,   689,
   10811, -1068,  8523, -1068, 17168,  4827,  8523, -1068, 17168,  4827,
   17168,   168, 10635,  6763,  6763,  6763,  6763, -1068, -1068, -1068,
   16692, 17168, 17475, -1068, -1068, -1068,   700, -1068,   612, -1068,
     612, -1068, -1068, -1068, -1068,   110, -1068,   133, -1068,   -20,
   16732,   -11, -1068, -1068, -1068, 10811, 10811, 10811, 10811, 10811,
   10811, 10811, 10811, -1068, -1068, 15904, 13801, 13886, -1068, 15904,
   13962, 14047,  4827, -1068,   701,   702, -1068, -1068,   533, -1068,
     469, 17168, 14123, -1068, 14208, -1068, -1068, -1068, 17168,   357,
     703,   363,   704, 10811, -1068, -1068,   612, -1068,   612, -1068,
      -9, -1068, 10811,    61,   110,   110,   110,   110,   110,   110,
      61, -1068, -1068, -1068, -1068, -1068, 16777, 16885, 16930, 16975,
   17083, 17128,  4827,  4827,  4827,  4827, -1068,   706,   709, -1068,
    4827,  4827, -1068, -1068, -1068, -1068, 17168, -1068, -1068, -1068,
   17168, -1068, -1068, -1068, -1068, -1068, -1068, -1068,   719, 10811,
   10811, 10811, 10811, 10811, 10811, 15904, 15904, 15904, 15904, -1068,
   -1068, 15904, 15904
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   115,   458,   673,   674,   675,
     669,   670,   676,     0,   585,   106,   136,   553,   143,   555,
     585,     0,   142,   488,   486,   106,     0,     0,    47,     0,
     266,   137,   184,   183,   667,     0,     0,     0,   182,     0,
     141,    59,   267,   338,   138,     0,     0,     0,   333,     0,
       0,   145,     0,     0,   602,   577,   677,   146,     0,   339,
     547,   483,     0,     0,     0,   169,   167,   426,   140,   556,
     485,     0,     0,     0,     0,   551,     0,     0,   144,     0,
       0,   116,     0,   668,     0,     0,   476,   139,   304,   549,
      53,   487,   147,     0,     0,   726,     0,   728,     0,   729,
     730,   618,     0,   727,   724,   626,   165,   725,     0,     0,
       0,     0,     4,     0,     5,     9,    10,    44,    11,     0,
      49,    62,    12,    68,    13,    14,    15,    16,    28,   533,
     534,    22,    50,   166,   176,     0,   185,   640,   177,    17,
      30,    29,    19,     0,   261,    18,   636,    21,    34,    31,
      32,   175,   305,     0,   173,     0,     0,   633,   337,     0,
     630,   171,   354,   446,   437,   628,   174,     0,     0,   172,
     645,   624,   623,   627,   537,   535,     0,     0,   634,   635,
     639,   638,   637,     0,   536,     0,   646,   647,   648,   671,
     672,   629,   539,   538,   631,   632,     0,    27,   555,   137,
       0,     0,     0,     0,   556,     0,     0,     0,     0,   633,
     645,   535,   634,   635,   545,   536,   646,   647,     0,     0,
     586,   107,     0,   554,     0,   585,   477,     0,   484,     0,
      20,     0,   521,    45,   336,     0,   527,   113,   117,   129,
     123,   122,   131,   110,   121,   132,   118,   133,   108,   134,
     127,   120,   128,   126,   124,   125,   109,   111,   119,   130,
     135,   114,     0,   112,   193,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    60,    62,
     527,     0,     0,     0,     0,     0,     0,   491,     0,   153,
      39,     0,   296,     0,   295,   711,   606,   603,   604,   605,
       0,   548,   712,     7,   527,   336,   336,   433,   150,   428,
     151,   429,   430,   434,   435,   149,   148,   436,   426,   517,
       0,   341,   342,   344,     0,   427,   516,   346,   504,     0,
       0,     0,   154,   630,   621,   644,   622,     0,     0,    42,
       0,   552,   540,     0,   527,    43,   546,     0,   274,   278,
     275,   278,   550,    51,   336,     0,     0,     0,   713,   715,
     616,   710,   709,     0,    65,    69,     0,     0,   523,     0,
       0,     0,   524,   569,     0,     0,   521,     0,   617,     0,
       6,     0,     0,    63,     0,     0,   488,   181,     0,   669,
     305,   641,   189,     0,     0,     0,   301,     0,   325,   329,
     331,     0,   315,   319,   322,   426,   446,   444,   445,   443,
     362,   447,   450,   449,   451,     0,   441,   438,   439,   442,
       0,   481,     0,   478,     0,   625,    33,   607,     0,     0,
       0,     0,     0,     0,   714,     0,     0,     0,     0,     0,
       0,   615,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   393,   400,
     401,   402,   397,   399,     0,     0,   395,   398,   396,   394,
       0,   404,   403,     0,     0,   527,   527,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,    23,     0,    36,
       0,     0,     0,    24,     0,    37,   336,   302,     0,   183,
     303,    48,    62,     0,   533,     0,   528,   529,   532,   314,
       0,     0,   203,     0,     0,   202,     0,   211,     0,     0,
       0,   209,     0,   219,     0,     0,   217,     0,     0,     0,
       0,   235,     0,   393,   231,     0,     0,     0,     0,     0,
     245,    25,   495,     0,   496,   498,     0,   515,     0,   501,
       0,     0,   152,    38,     0,   298,     0,     0,     0,    40,
       0,   170,   168,   431,   432,     0,   427,   446,   426,   419,
       0,   418,   519,     0,    99,   642,   643,   352,     0,   155,
       0,    41,     0,     0,   285,   276,   277,   336,    54,   196,
     195,    26,    67,    66,    70,     0,   678,     0,     0,   663,
       0,   665,   570,     0,     0,     0,     0,     0,     0,     0,
       0,   682,     8,     0,    56,     0,    97,     0,    93,     0,
      77,     0,     0,    83,   178,   305,     0,   191,     0,   188,
     272,   306,     0,   313,   323,     0,   327,     0,     0,   317,
     312,   411,     0,   413,   417,     0,   448,   509,     0,   511,
     452,   440,   409,   113,   389,   117,   387,   123,   122,   121,
     118,   391,   133,   134,   120,   124,   109,   119,   135,   386,
     368,   371,   369,   370,   392,   381,   372,   385,   377,   375,
     388,   376,   374,   379,   384,   373,   378,   382,   383,   380,
     390,     0,   365,     0,   409,   366,   409,   363,     0,   480,
       0,   475,   490,   706,   705,   708,   717,   716,   721,   720,
     702,   699,   700,   701,   619,   689,   115,     0,   659,   660,
     116,   658,   657,   613,   693,   704,   698,   696,   707,   697,
     695,   687,   692,   694,   703,   686,   690,   691,   688,   614,
       0,     0,     0,     0,     0,     0,     0,     0,   719,   718,
     723,   722,     0,     0,     0,     0,     0,     0,     0,   680,
     272,   596,   597,   599,   601,     0,   588,     0,     0,     0,
     585,   585,   199,     0,   522,    46,     0,     0,   542,     0,
       0,     0,     0,   559,     0,     0,     0,   212,     0,   565,
       0,     0,   210,   220,     0,     0,   218,     0,     0,   234,
       0,   230,     0,     0,     0,     0,   541,     0,   248,     0,
     246,     0,   499,     0,   514,   513,     0,   493,   612,   492,
     297,   294,     0,     0,     0,   656,   519,   347,   343,     0,
       0,   520,   501,   345,     0,   351,     0,     0,     0,   543,
       0,   279,    52,   679,   620,   664,   525,   666,   526,   227,
       0,     0,     0,   681,   225,   571,     0,   684,   683,     0,
      58,    57,     0,    92,     0,     0,     0,    85,     0,     0,
      83,    55,   386,   368,   371,   369,   370,   379,   378,   380,
       0,   405,   406,    72,    71,    84,     0,     0,   307,     0,
       0,   269,     0,     0,   336,     0,   326,     0,   330,   332,
       0,     0,   426,   412,   452,   507,   506,   505,   508,   453,
     459,   426,   361,     0,   367,     0,   357,   358,   482,   479,
       0,     0,   105,   103,   104,   102,   101,   100,   654,   655,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     598,   600,     0,   587,   136,   143,   142,   141,   138,   145,
     146,   140,   144,   139,   147,     0,     0,     0,     0,     0,
     489,   194,   531,   530,     0,   229,     0,     0,   201,     0,
     200,     0,   591,     0,     0,   207,     0,     0,   205,     0,
     215,     0,   213,     0,   243,   242,     0,   237,     0,     0,
     233,     0,   239,     0,   271,     0,     0,   497,   500,   501,
     502,   503,   293,     0,     0,   501,     0,   452,   519,   518,
     407,   422,   420,   353,   287,     0,     0,   286,   289,   544,
       0,   280,   283,     0,   228,     0,     0,     0,     0,   226,
       0,    98,    95,     0,    94,    80,    79,    78,     0,     0,
       0,   179,     0,   180,   307,   336,     0,     0,     0,   300,
     159,   169,   167,   163,   299,   336,   308,   309,   156,   324,
     328,   318,   321,   414,   459,     0,     0,   355,   416,     0,
     364,   409,   409,   661,   662,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,   589,   501,   645,   645,   198,
     197,   204,     0,     0,   558,     0,   557,     0,   590,     0,
       0,   564,   208,     0,   563,   206,   216,   214,   236,   232,
     576,   238,     0,    64,   268,   247,   244,   494,   527,   527,
     423,   421,   348,   349,   501,   408,     0,   291,     0,     0,
       0,   281,     0,   223,   573,     0,     0,   221,   572,     0,
     685,     0,     0,     0,    83,     0,    83,    86,    89,    75,
      74,    73,   336,   186,   192,   190,   273,   157,   336,   161,
     336,   310,   311,   334,   473,     0,   474,   461,   464,     0,
     460,     0,   410,   359,   360,     0,     0,     0,     0,     0,
       0,     0,     0,   273,   593,   562,     0,     0,   592,   568,
       0,     0,     0,   241,     0,     0,   425,   424,   292,   290,
       0,   284,     0,   224,     0,   222,    96,    82,    81,     0,
       0,     0,     0,     0,   187,   270,   336,   160,   336,   164,
       0,   472,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   454,   456,   457,   356,   455,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   240,   608,   609,   282,
       0,     0,    88,    91,    87,    90,    76,   158,   162,   335,
     463,   465,   466,   469,   470,   471,   467,   468,   462,     0,
       0,     0,     0,     0,     0,   561,   560,   567,   566,   610,
     611,   575,   574
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1068, -1068, -1068,     4,   -64,  2110,   174, -1068, -1068, -1068,
    -214, -1068, -1068, -1068, -1068, -1068,   -28,   598, -1068,  -576,
    2390,   499,  -612, -1068,  -838, -1068, -1068,    23, -1068, -1068,
   -1068,   859, -1068,  2571,  -133, -1068, -1068,  -471,   397, -1067,
    -169, -1068, -1068,  -168, -1068, -1068,  -859, -1068, -1068,   -33,
   -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068, -1068,    38,
   -1068,   797, -1068, -1068,  -104,  1968, -1068, -1068, -1068, -1068,
     541, -1068,  -127, -1068, -1068, -1068, -1068,   341, -1068, -1068,
   -1068,  -112, -1068,  -362,  -842, -1068, -1068, -1068, -1068, -1068,
      -1, -1068, -1068,     3, -1068, -1068,  -601, -1068,   692, -1068,
   -1068, -1068, -1068, -1068,   332, -1068,  1927, -1068, -1068, -1068,
   -1068,   514, -1068, -1068, -1068, -1068,   -12,  -405,  -155,  -827,
    -939,  -678, -1068,    -6, -1068,     5,   344,   -60,   594,   -55,
   -1068, -1068,  -358,  -795, -1068,  -307, -1068,  -150,  -312,  -301,
    -554, -1068, -1068,    34,   227, -1068,  -174,   790, -1068,  -167,
     377,   121,  -371,  -802,  -629, -1068, -1068, -1068,  -618,  -552,
   -1068,  -793,   -16,    17, -1068,  -236,  -486,  -551,   295, -1068,
   -1068, -1068, -1068,   760, -1068,   -15, -1068, -1068,  -229, -1068,
    -769, -1068, -1068,   852,  1113, -1068, -1068, -1068, -1068,   -13,
   -1068,    18,  1315, -1068,  1443,  1781, -1068, -1068, -1068, -1068,
   -1068, -1068, -1068, -1068, -1068,  -476
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   365,   113,   801,   115,   116,   496,   234,
     117,   118,   587,   354,   119,   120,   121,   385,   122,   123,
     124,   366,   885,   619,   886,   125,   126,   616,   617,   127,
     128,   222,   261,   400,   129,   263,   130,   319,   290,   131,
    1056,  1216,  1158,  1057,  1218,  1160,   132,   306,   305,   133,
     134,   135,   136,   137,   138,   628,   889,   139,   140,   706,
     141,   275,   142,   809,   810,   208,   144,   145,   146,   147,
     585,   841,  1023,   148,   149,   837,  1018,   292,   293,   150,
     151,   152,   153,   395,   894,   154,   155,   401,   900,   901,
     402,   895,   635,   403,   897,   637,   404,   156,   209,  1220,
     158,   159,   320,   321,   322,  1007,   160,   334,   578,   834,
     161,   162,  1171,   163,   164,   693,   694,   881,   882,   883,
    1011,   912,   406,   642,  1069,   643,   572,   644,   324,   566,
     420,   410,   415,   910,  1234,  1235,   165,  1067,  1167,  1168,
    1169,   166,   167,   422,   423,   699,   168,   169,   227,   286,
     287,   545,   546,   817,   326,   908,   649,   650,   549,   327,
     831,   832,   367,   370,   371,   505,   506,   507,   210,   171,
     172,   173,   174,   211,   176,   219,   220,   765,   518,   971,
     766,   767,   177,   212,   213,   180,   181,   182,   335,   183,
     392,   508,   215,   185,   216,   217,   188,   189,   190,   191,
     377,   192,   193,   194,   195,   196
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     214,   424,   835,   318,   223,   224,   112,   323,   225,   769,
     880,   231,   325,   279,   232,   695,   916,   972,   917,   907,
     501,   833,   268,   270,   272,   390,   276,   904,   626,   474,
    1012,   838,  1039,  1005,   896,  1058,   898,   291,  1034,   294,
     295,   522,   524,   527,   537,   300,  1045,   301,   645,   302,
     758,   836,   807,   492,   328,   329,   605,  -266,   232,   336,
     338,   340,   341,   342,  1164,  -267,  1120,   346,   560,   347,
     550,   647,   547,   267,   269,   271,   352,   814,   288,   547,
     356,   336,    72,   358,    72,   359,   288,   516,   360,   361,
     337,   362,   974,   288,   375,   232,   336,   376,   378,   716,
     967,  -320,  1065,   504,  1233,   516,   577,  1224,   583,  1064,
     494,   494,   357,  1164,   288,   547,   610,   598,   595,     3,
     716,   535,   388,   563,   391,  -288,  -512,   372,   495,   574,
     516,  -510,  1143,  1225,  -316,  1226,  1227,   717,  1228,  1229,
     588,   603,   596,   599,   611,   604,   608,   504,   374,   434,
     606,   718,  -350,  1233,  1165,  1231,   606,   440,  -316,  1025,
     488,  1066,   488,  -510,   427,  1166,   564,   516,  1144,  -288,
     551,   504,   811,  1085,   236,  -512,  -510,   906,   759,   497,
     719,   516,   280,   307,   607,  1196,  1058,   479,   480,   481,
    -510,   483,   346,   232,   376,   999,  1058,  1117,  -512,   829,
     720,   554,  1152,  1121,   309,  -512,   502,  -512,   311,   827,
     491,   504,  1123,  1149,  1166,  1124,   488,   648,   548,  -512,
    -512,   720,  -288,   336,   488,   548,  -512,   721,   963,   494,
     488,   488,   312,   940,  -512,   313,  -510,    63,   488,   746,
     747,  -510,  1015,   530,  -512,  -512,   500,   314,    24,   600,
     -61,   382,   488,  1222,   296,   816,   197,  1038,   348,   350,
     -60,   548,   592,   887,   812,   696,   317,   336,   639,   -61,
      88,   936,   561,   562,   430,   601,   941,   625,  1009,   -60,
    1010,  1223,   775,   411,  1184,  -512,  1016,   902,   -61,   383,
     813,   336,   640,  1058,  1059,   197,  1060,   170,   -60,  1217,
      61,  1219,   342,   346,  -249,   218,  1210,   722,  1212,   197,
     769,   488,    70,   903,   297,   860,  1040,   575,   576,   -61,
     384,   170,  1197,   942,   170,   773,   502,   494,  1188,   -60,
     641,   336,   431,   298,   407,   412,   432,  1086,    91,  1017,
    1206,   318,   504,   504,   891,   700,   779,   413,   995,   943,
     299,   892,   277,   965,   408,   278,   597,  1257,  -250,  1258,
     602,   221,   812,   701,   409,   812,   414,  -256,  -255,   618,
     594,   620,   994,   842,   996,   170,   779,  1122,   631,   170,
     170,   232,   822,  1097,   434,   779,   779,  -595,   997,   438,
    -582,  1008,   440,  1173,  1174,   416,   443,  -652,   475,  -652,
     476,  -594,  1081,   170,   226,   504,  1048,   228,   170,  1098,
     477,  1156,  1183,  -595,   478,   703,   704,   705,   707,   709,
     710,  1141,   711,   712,   713,   714,   715,  -594,   723,   724,
     725,   726,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,   737,   738,   739,   233,   957,   417,   236,   504,
     478,   336,   336,  1084,   890,   551,   862,   336,   866,   418,
     336,   336,   336,   336,   749,   751,   769,   551,   502,  1040,
     708,   339,   768,   702,   863,  1040,   867,   771,   419,   264,
    -264,   774,   741,   742,   700,   960,  -254,  1041,   743,  -251,
     776,   744,   745,  1147,    43,   781,   280,  -336,   232,   303,
     783,   785,  1154,   551,   318,  1040,   789,   791,   323,   343,
     695,  1040,   795,   325,   170,   748,   750,  -336,  -258,   802,
      59,  1155,   804,  1252,   304,   336,  -728,  -336,   770,  1254,
    -728,  1209,  -729,  1211,   434,   815,  -729,   818,  -653,   438,
    -653,   294,   440,  -730,   823,   824,   443,  -730,  -262,   307,
     307,  -259,   784,   627,  -652,   328,  -652,  -253,   790,   502,
     328,   976,   979,   794,  -725,   981,   983,  -257,  -725,   336,
     309,   309,   344,  -263,   311,   311,  -265,  -649,   170,  -649,
    -252,   353,   774,   170,   488,   336,   539,   336,  -651,  -650,
    -651,  -650,   852,   774,   855,  1232,   857,   859,   312,   312,
    -260,   313,   313,    63,   379,   920,   381,   921,   393,   884,
     884,  1221,   394,   314,   314,   846,   372,   848,   372,   594,
     405,   769,  1026,  1028,   504,   899,  -579,  -578,   307,   425,
     484,   485,   317,   317,   497,   328,   426,  -583,    23,    24,
     488,  -584,  -581,   851,  1232,  -580,   504,   487,   498,   309,
      32,   499,   493,   311,   503,   509,    38,   383,   511,   573,
     538,   170,   557,    43,   512,   515,   517,   521,   523,   526,
    1262,  1263,  1264,  1265,  1266,  1267,   170,   312,   913,   558,
     313,   567,    63,   568,   579,   918,   582,   591,   553,    59,
     555,    61,   314,   612,   157,  1051,  -415,   559,  1052,   621,
     632,   500,   633,    70,   634,   636,   638,   646,   651,   698,
     440,   317,   740,   757,   777,   697,   778,   779,   157,   780,
     786,   157,    86,  1047,   797,    88,   798,   805,   800,    91,
     803,  1032,   722,   806,   816,   825,   830,   581,   839,   783,
     932,   789,   935,   802,   336,   938,   855,   840,   888,   911,
     494,   850,   589,   590,   856,   958,   959,   909,  -115,   504,
    1072,  -136,   175,  -143,   336,  -142,   336,   964,  -141,   966,
    -138,  -145,   157,   768,  -146,   973,   157,   157,   106,   430,
    -140,  -144,  -116,   170,   984,   985,   175,  -139,  -147,   175,
     914,   931,   915,   934,   922,   962,   937,   928,   929,   939,
     157,   923,   924,   336,   986,   157,   504,   925,   170,   988,
     926,   535,   927,  1006,   170,  1040,  1043,   328,   535,  1014,
     399,  1013,  1076,  1019,  1044,   170,  1049,   170,  1074,  1073,
    1055,  1079,  1082,  1118,  1001,  1112,  1113,   431,   318,  1114,
     175,   432,  1119,  1148,   175,   175,  1130,   318,  1135,   618,
    1139,  1157,  1159,  1035,   178,  1037,   884,   884,   902,   358,
    1172,   359,  1192,  1200,   361,   362,  1215,  1223,   175,  1253,
    1255,  1279,   430,   175,  1280,  1247,  1248,   536,   178,   336,
     615,   178,  1194,  1195,   229,  1031,  1161,  1162,   772,   434,
     355,  1115,   586,  1129,   438,   820,   439,   440,   170,  1061,
     828,   443,   170,  1071,  1062,  1068,   629,  1063,   450,   826,
     170,   157,   565,  1259,  1163,   787,   456,  1075,  1268,   792,
    1078,   793,  1261,  1046,   796,   387,   336,   919,   819,   768,
     431,     0,   178,   998,   432,     0,   178,   178,     0,     0,
       0,     0,     0,   707,   749,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1094,     0,  1096,     0,     0,     0,
     178,  1101,     0,     0,  1104,   178,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   157,  1110,     0,     0,   175,
     157,  1055,   434,   435,     0,   504,   504,   438,     0,   439,
     440,     0,     0,   328,   443,   748,     0,  1125,     0,     0,
    1128,   450,     0,   232,     0,     0,     0,   454,   455,   456,
       0,     0,  1134,     0,  1138,     0,     0,  1140,     0,     0,
     884,     0,     0,     0,     0,     0,     0,  1150,     0,  1151,
       0,   535,   535,     0,     0,   535,   535,     0,     0,     0,
       0,     0,     0,   175,     0,     0,     0,   535,   175,   535,
       0,     0,     0,  1170,     0,     0,     0,     0,   157,     0,
       0,     0,   955,  1094,  1096,     0,  1101,  1104,     0,  1134,
    1138,   178,     0,   157,     0,     0,     0,     0,     0,  1185,
    1186,     0,  1187,     0,   768,     0,  1189,  1190,  1055,     0,
    1191,     0,     0,     0,     0,   170,     0,     0,   170,     0,
       0,     0,     0,     0,     0,   336,   336,     0,     0,     0,
       0,     0,     0,     0,     0,   179,     0,  1201,     0,  1202,
       0,     0,     0,  1204,     0,     0,   175,     0,     0,  1208,
     884,   884,   884,   884,     0,   178,     0,     0,     0,   179,
     178,   175,   179,     0,     0,   170,     0,     0,     0,     0,
       0,   170,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1185,  1236,  1237,  1189,  1238,  1239,  1240,  1241,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     157,   968,   970,     0,     0,     0,     0,   975,   978,     0,
       0,   980,   982,   179,     0,     0,     0,   179,   179,     0,
    1256,     0,     0,     0,     0,   157,     0,     0,     0,  1260,
       0,   157,     0,   430,     0,     0,     0,     0,   178,     0,
       0,   179,   157,     0,   157,     0,   179,     0,     0,  1275,
    1276,  1277,  1278,   178,     0,     0,     0,  1281,  1282,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   175,     0,
       0,  1087,  1088,     0,     0,     0,  1275,  1276,  1277,  1278,
    1281,  1282,   170,     0,   170,     0,     0,     0,     0,   170,
       0,   431,   170,   175,     0,   432,     0,     0,     0,   175,
       0,   170,     0,   170,     0,     0,   170,     0,     0,     0,
     175,     0,   175,     0,     0,   157,     0,     0,     0,   157,
       0,     0,     0,     0,     0,     0,     0,   157,     0,     0,
     170,     0,     0,     0,     0,     0,     0,   184,     0,     0,
     170,     0,   170,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   179,     0,     0,   443,     0,     0,     0,     0,
     178,   184,   450,     0,   184,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,  1089,  1090,     0,     0,     0,
       0,  1091,     0,   175,     0,   178,     0,   175,     0,     0,
       0,   178,     0,  1102,     0,   175,  1105,     0,  1106,     0,
    1107,     0,   178,     0,   178,     0,     0,   170,     0,   237,
       0,     0,     0,     0,   170,   184,   179,     0,     0,   184,
     184,   179,     0,   238,   239,     0,   240,   170,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
     242,   170,     0,   184,     0,     0,   243,     0,   184,     0,
     170,     0,   244,     0,   170,     0,   245,     0,     0,   246,
       0,     0,     0,     0,     0,   186,     0,     0,     0,   247,
       0,   430,     0,     0,     0,   178,   248,   249,     0,   178,
       0,     0,     0,     0,   250,     0,     0,   178,     0,   186,
       0,     0,   186,     0,   251,     0,     0,     0,     0,   179,
       0,     0,     0,   252,   253,     0,   254,   170,   255,     0,
     256,     0,   157,   257,   179,   157,     0,   258,   542,     0,
     259,     0,     0,     0,   260,     0,     0,     0,     0,   431,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   186,     0,  1199,     0,   186,   186,     0,
       0,     0,     0,     0,   184,     0,     0,   170,   170,   170,
     170,     0,   157,     0,     0,   170,   170,     0,   157,     0,
       0,   186,     0,     0,     0,     0,   186,     0,     0,   543,
     175,   434,   435,   175,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,     0,   447,   448,   449,
     450,   451,     0,   453,     0,     0,   454,   455,   456,     0,
     237,     0,     0,     0,     0,     0,     0,   457,   184,     0,
       0,   179,     0,   184,   238,   239,     0,   240,     0,     0,
     175,     0,   241,     0,     0,     0,   175,     0,     0,     0,
       0,   242,     0,     0,     0,     0,   179,   243,     0,     0,
       0,     0,   179,   244,     0,   430,     0,   245,     0,     0,
     246,     0,     0,   179,     0,   179,     0,     0,     0,     0,
     247,     0,   178,     0,     0,   178,     0,   248,   249,   157,
       0,   157,   186,     0,     0,   250,   157,     0,     0,   157,
       0,     0,     0,     0,     0,   251,     0,     0,   157,     0,
     157,   184,     0,   157,   252,   253,     0,   254,     0,   255,
       0,   256,     0,   431,   257,     0,   184,   432,   258,   569,
       0,   259,   178,     0,     0,   260,     0,   157,   178,     0,
       0,     0,     0,     0,     0,     0,   179,   157,     0,   157,
     179,     0,     0,     0,     0,     0,   186,   175,   179,   175,
       0,   186,     0,     0,   175,     0,     0,   175,     0,     0,
       0,     0,     0,     0,     0,   434,   175,     0,   175,     0,
     438,   175,   439,   440,     0,     0,     0,   443,     0,     0,
     570,     0,     0,     0,   450,     0,     0,     0,     0,     0,
     454,   455,   456,     0,     0,   175,     0,     0,     0,     0,
       0,     0,     0,   187,   157,   175,     0,   175,     0,     0,
       0,   157,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   184,   157,     0,     0,   187,     0,   186,
     187,     0,     0,     0,     0,     0,     0,     0,   157,   178,
       0,   178,     0,     0,   186,     0,   178,   157,   184,   178,
       0,   157,     0,     0,   184,     0,     0,     0,   178,     0,
     178,     0,     0,   178,     0,   184,     0,   184,     0,     0,
     431,     0,   175,     0,   432,     0,     0,     0,     0,   175,
       0,   187,     0,     0,     0,   187,   187,   178,     0,     0,
       0,     0,   175,     0,     0,     0,     0,   178,     0,   178,
       0,     0,     0,     0,   157,     0,   175,     0,     0,   187,
       0,     0,     0,     0,   187,   175,     0,     0,     0,   175,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   179,   443,     0,   179,     0,   184,     0,
     449,   450,   184,     0,   453,     0,     0,   454,   455,   456,
     184,   186,     0,     0,   157,   157,   157,   157,   457,     0,
       0,     0,   157,   157,   178,     0,     0,     0,     0,     0,
       0,   178,   175,     0,     0,     0,   186,     0,     0,     0,
       0,     0,   186,   179,   178,     0,     0,     0,     0,   179,
     143,     0,     0,   186,     0,   186,     0,     0,   178,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,     0,
       0,   178,     0,     0,   143,     0,     0,   143,     0,   333,
     187,     0,   175,   175,   175,   175,     0,     0,     0,     0,
     175,   175,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   333,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,   333,     0,     0,     0,
       0,     0,     0,     0,   178,     0,   186,     0,   143,     0,
     186,     0,     0,     0,     0,     0,     0,     0,   186,     0,
       0,     0,     0,   430,   187,     0,     0,     0,     0,   187,
       0,     0,     0,     0,     0,     0,   143,     0,     0,     0,
     179,   143,   179,     0,     0,     0,     0,   179,     0,   431,
     179,     0,     0,   432,   178,   178,   178,   178,     0,   179,
       0,   179,   178,   178,   179,     0,     0,     0,     0,     0,
       0,     0,   114,     0,     0,   184,     0,     0,   184,     0,
       0,   431,     0,     0,     0,   432,     0,     0,   179,     0,
       0,     0,     0,     0,     0,     0,   230,     0,   179,   235,
     179,   434,   435,     0,     0,   437,   438,   187,   439,   440,
       0,     0,     0,   443,     0,     0,     0,     0,     0,     0,
     450,     0,   187,   333,     0,   184,   454,   455,   456,     0,
       0,   184,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,     0,     0,     0,   443,     0,   143,     0,     0,
     345,   449,   450,     0,   716,   453,     0,     0,   454,   455,
     456,     0,     0,     0,     0,   179,     0,   333,   944,     0,
       0,   945,   179,     0,     0,     0,   946,     0,   114,     0,
       0,     0,     0,   380,     0,   179,     0,   199,     0,     0,
       0,   333,     0,     0,     0,     0,     0,   947,     0,   179,
       0,     0,     0,   186,   948,     0,   186,     0,   179,     0,
       0,   143,   179,     0,   949,     0,   143,     0,     0,   497,
       0,     0,   950,    23,    24,     0,     0,     0,     0,   187,
       0,   333,     0,   498,     0,    32,   499,     0,     0,   951,
       0,    38,   184,     0,   184,     0,     0,     0,    43,   184,
       0,   952,   184,   186,   187,   720,     0,     0,     0,   186,
     187,   184,   953,   184,     0,   179,   184,     0,     0,   954,
       0,   187,     0,   187,    59,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   500,     0,    70,   486,
     184,     0,     0,     0,   143,     0,     0,     0,     0,     0,
     184,     0,   184,     0,     0,     0,     0,    86,     0,   143,
      88,     0,     0,     0,    91,   179,   179,   179,   179,     0,
       0,     0,     0,   179,   179,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
       0,     0,     0,     0,   187,     0,     0,     0,   187,     0,
       0,   333,   333,   541,     0,     0,   187,   333,   552,     0,
     333,   333,   333,   333,     0,     0,     0,   184,   473,     0,
     186,  -649,   186,  -649,   184,     0,     0,   186,     0,     0,
     186,     0,     0,     0,     0,     0,     0,   184,     0,   186,
       0,   186,     0,     0,   186,     0,     0,     0,     0,     0,
     289,   184,     0,     0,     0,     0,     0,     0,     0,     0,
     184,     0,     0,     0,   184,   497,   143,     0,   186,    23,
      24,     0,     0,     0,   289,   333,     0,     0,   186,   498,
     186,    32,   499,     0,   349,   351,   114,    38,     0,     0,
       0,   143,     0,     0,    43,     0,     0,   143,     0,     0,
       0,   114,     0,     0,     0,     0,     0,     0,   143,   -59,
     143,     0,     0,     0,     0,     0,     0,   184,     0,   333,
      59,     0,    61,     0,     0,     0,    65,     0,   -59,    66,
       0,     0,   500,     0,    70,   333,     0,   333,     0,     0,
       0,     0,     0,     0,     0,   186,     0,     0,     0,     0,
       0,     0,   186,    86,     0,     0,    88,     0,     0,     0,
      91,     0,     0,     0,     0,   186,     0,   184,   184,   184,
     184,     0,     0,     0,     0,   184,   184,     0,     0,   186,
       0,   143,     0,     0,     0,   143,     0,     0,   186,     0,
       0,   187,   186,   143,   187,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,   106,
       0,     0,   262,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   489,     0,   281,   282,   283,     0,
     285,     0,     0,   552,     0,     0,     0,     0,     0,   552,
       0,   187,     0,     0,     0,   186,     0,   187,     0,     0,
     799,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   289,   289,   289,
     289,   289,   289,     0,     0,   531,   534,     0,     0,     0,
       0,     0,   540,     0,     0,   430,     0,     0,     0,     0,
       0,   289,     0,   289,   333,   186,   186,   186,   186,     0,
     289,     0,     0,   186,   186,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   333,     0,   333,     0,     0,     0,
       0,     0,     0,   849,     0,     0,     0,   854,     0,     0,
       0,     0,     0,     0,   396,   114,     0,     0,     0,     0,
     289,     0,     0,   431,     0,     0,     0,   432,   421,   285,
       0,     0,     0,   333,     0,   289,   289,     0,   187,     0,
     187,     0,     0,     0,     0,   187,     0,     0,   187,     0,
       0,     0,     0,     0,     0,     0,     0,   187,   143,   187,
       0,   143,   187,     0,   482,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   187,   443,   444,   445,
       0,   447,   448,   449,   450,     0,   187,   453,   187,     0,
     454,   455,   456,     0,     0,     0,     0,     0,   143,   333,
       0,   457,     0,     0,   143,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   510,     0,     0,     0,
       0,     0,     0,     0,   528,   529,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   544,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   333,     0,     0,     0,
       0,     0,     0,   187,     0,     0,     0,     0,     0,     0,
     187,   289,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   187,     0,   571,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   187,   289,     0,
     987,     0,   289,   990,   289,     0,   187,   289,     0,     0,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   143,     0,   143,     0,     0,
       0,     0,   143,     0,     0,   143,     0,     0,     0,     0,
       0,     0,     0,   237,   143,     0,   143,     0,     0,   143,
    1024,     0,     0,     0,   630,     0,  1029,   238,   239,     0,
     240,     0,     0,   187,     0,   241,     0,     0,     0,     0,
       0,     0,     0,   143,   242,     0,     0,     0,     0,     0,
     243,   692,     0,   143,     0,   143,   244,     0,     0,     0,
     245,     0,     0,   246,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   247,     0,     0,     0,     0,     0,     0,
     248,   249,     0,   187,   187,   187,   187,     0,   250,     0,
       0,   187,   187,     0,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,   333,   333,   252,   253,     0,
     254,     0,   255,     0,   256,   760,     0,   257,     0,     0,
     143,   258,     0,   285,   259,     0,     0,   143,   260,   262,
       0,     0,     0,     0,     0,     0,     0,   552,     0,   552,
     143,     0,     0,     0,   552,     0,     0,   552,     0,     0,
       0,     0,     0,     0,   143,   497,  1108,     0,  1109,    23,
      24,  1111,     0,   143,  1050,     0,     0,   143,     0,   498,
     808,    32,   499,     0,   544,     0,     0,    38,     0,     0,
       0,   488,   285,  1020,    43,  1127,     0,   821,     0,     0,
       0,     0,     0,     0,     0,  1133,   571,  1137,     0,     0,
       0,   544,     0,     0,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,   379,     0,  1051,     0,     0,  1052,
     143,     0,   500,     0,    70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   289,   289,     0,     0,     0,     0,
     289,   289,     0,    86,   289,   289,    88,     0,  1053,     0,
      91,     0,   871,     0,   992,   428,     0,     0,     0,     0,
       0,     0,   429,   893,     0,     0,     0,     0,     0,     0,
     143,   143,   143,   143,     0,   430,     0,     0,   143,   143,
       0,     0,  1193,     0,  -307,     0,     0,     0,  -307,  -307,
    1021,   490,     0,  -307,     0,     0,  1198,     0,  -307,   106,
    -307,  -307,     0,     0,     0,  1203,  -307,     0,     0,  1205,
       0,     0,     0,  -307,     0,     0,  -307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   421,     0,   431,     0,     0,  -307,   432,     0,  -307,
       0,  -307,     0,  -307,     0,  -307,  -307,     0,  -307,     0,
       0,  -307,     0,  -307,     0,     0,     0,     0,     0,     0,
       0,     0,  1246,     0,     0,   218,     0,     0,     0,   433,
       0,     0,  -307,     0,     0,  -307,     0,  -307,     0,  -307,
       0,     0,     0,     0,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   289,   289,
     454,   455,   456,     0,   289,     0,   534,     0,     0,     0,
       0,   457,     0,   534,     0,     0,   289,     0,  -307,   289,
       0,   289,     0,   289,  -307,     0,   993,     0,     0,     0,
       0,     0,     0,   544,     0,     0,  1116,     0,     0,     0,
       0,     0,     0,  1002,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,     0,    17,     0,
       0,  1022,   198,  1131,     0,     0,     0,   307,     0,     0,
       0,     0,     0,     0,     0,     0,   199,     0,     0,     0,
       0,   200,   201,     0,     0,   202,     0,     0,   309,     0,
       0,     0,   311,     0,     0,     0,   203,     0,     0,    48,
      49,     0,     0,     0,     0,    53,     0,    54,    55,     0,
     421,   285,     0,     0,     0,     0,   312,     0,    60,   313,
      62,    63,    64,     0,     0,     0,     0,    67,     0,   204,
       0,   314,     0,     0,     0,     0,   692,    75,     0,    77,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
     317,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,   289,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   103,   104,   105,     0,     0,     0,   107,
       0,   206,     0,   109,  -340,   207,   808,     0,   111,     0,
       0,     0,     0,     0,     0,     0,   534,   534,     0,     0,
     534,   534,     0,     0,     0,     0,     0,     0,     0,     0,
    1249,  1022,   534,     0,   534,    -2,     4,     0,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,     0,    29,    30,
       0,    31,     0,    32,    33,    34,    35,    36,    37,    38,
      39,    40,   -59,     0,    41,    42,    43,     0,    44,  -336,
       0,    45,    46,    47,    48,    49,     0,    50,    51,    52,
      53,   -59,    54,    55,     0,    56,    57,    58,     0,  -336,
       0,     0,    59,    60,    61,    62,    63,    64,    65,  -336,
     -59,    66,    67,    68,    69,     0,    70,    71,    72,     0,
      73,    74,    75,    76,    77,    78,    79,    80,     0,    81,
      82,     0,    83,    84,    85,    86,    87,     0,    88,    89,
      90,   -59,    91,    92,     0,     0,    93,     0,    94,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,   106,     0,     0,   107,     0,   108,     0,   109,     0,
     110,     0,     4,   111,     5,     6,     7,     8,     9,    10,
      11,    12,     0,  -680,     0,    13,    14,    15,    16,    17,
    -680,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,  -680,    29,    30,  -680,    31,     0,    32,
      33,    34,    35,    36,    37,    38,    39,    40,   -59,     0,
      41,    42,    43,     0,    44,  -336,     0,    45,    46,    47,
      48,    49,     0,    50,    51,    52,    53,   -59,    54,    55,
       0,    56,    57,    58,     0,  -336,     0,     0,    59,    60,
      61,    62,     0,    64,    65,  -336,   -59,    66,    67,    68,
      69,  -680,    70,    71,    72,  -680,    73,    74,    75,    76,
      77,    78,    79,    80,     0,    81,    82,     0,    83,    84,
      85,    86,    87,     0,    88,    89,    90,   -59,    91,    92,
       0,     0,    93,     0,    94,     0,     0,  -680,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,  -680,    98,  -680,  -680,  -680,  -680,
    -680,  -680,  -680,     0,  -680,  -680,  -680,  -680,  -680,  -680,
    -680,  -680,  -680,  -680,  -680,  -680,   105,   106,  -680,  -680,
    -680,     0,   108,  -680,   109,     0,   110,     0,   363,  -680,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,     0,
      29,    30,     0,    31,     0,    32,    33,    34,    35,    36,
      37,    38,    39,    40,   -59,     0,    41,    42,    43,     0,
      44,  -336,     0,    45,    46,    47,    48,    49,     0,    50,
      51,    52,    53,   -59,    54,    55,     0,    56,    57,    58,
       0,  -336,     0,     0,    59,    60,    61,    62,    63,    64,
      65,  -336,   -59,    66,    67,    68,    69,     0,    70,    71,
      72,     0,    73,    74,    75,    76,    77,    78,    79,    80,
       0,    81,    82,     0,    83,    84,    85,    86,    87,     0,
      88,    89,    90,   -59,    91,    92,     0,     0,    93,     0,
      94,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,   106,     0,     0,   107,     0,   108,   364,
     109,     0,   110,     0,     4,   111,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,     0,    29,    30,     0,    31,
       0,    32,    33,    34,    35,    36,    37,    38,    39,    40,
     -59,     0,    41,    42,    43,     0,    44,  -336,     0,    45,
      46,    47,    48,    49,     0,    50,    51,    52,    53,   -59,
      54,    55,     0,    56,    57,    58,     0,  -336,     0,     0,
      59,    60,    61,    62,    63,    64,    65,  -336,   -59,    66,
      67,    68,    69,     0,    70,    71,    72,     0,    73,    74,
      75,    76,    77,    78,    79,    80,     0,    81,    82,     0,
      83,    84,    85,    86,    87,     0,    88,    89,    90,   -59,
      91,    92,     0,     0,    93,     0,    94,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   103,   104,   105,   106,
       0,     0,   107,     0,   108,   593,   109,     0,   110,     0,
     613,   111,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,    29,    30,     0,    31,     0,    32,    33,    34,
      35,    36,    37,    38,    39,    40,   -59,     0,    41,    42,
      43,     0,    44,  -336,     0,    45,    46,    47,    48,    49,
       0,    50,    51,    52,    53,   -59,    54,    55,     0,    56,
      57,    58,     0,  -336,     0,     0,    59,    60,    61,    62,
      63,    64,    65,  -336,   -59,    66,    67,    68,    69,     0,
      70,    71,    72,     0,    73,    74,    75,    76,    77,    78,
      79,    80,     0,    81,    82,     0,    83,    84,    85,    86,
      87,     0,    88,    89,    90,   -59,    91,    92,     0,     0,
      93,     0,    94,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   104,   105,   106,     0,     0,   107,     0,
     108,   614,   109,     0,   110,     0,     4,   111,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,     0,    29,    30,
       0,    31,     0,    32,    33,    34,    35,    36,    37,    38,
      39,    40,   -59,     0,    41,    42,    43,     0,    44,  -336,
       0,    45,    46,    47,    48,    49,     0,    50,    51,    52,
      53,   -59,    54,    55,     0,    56,    57,    58,     0,  -336,
       0,     0,    59,    60,    61,    62,    63,    64,    65,  -336,
     -59,    66,    67,    68,    69,     0,    70,    71,    72,     0,
      73,    74,    75,    76,    77,    78,    79,    80,     0,    81,
      82,     0,    83,    84,    85,    86,    87,     0,    88,    89,
      90,   -59,    91,    92,     0,     0,    93,     0,    94,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,   106,     0,     0,   107,     0,   108,   861,   109,     0,
     110,     0,     4,   111,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,     0,    29,    30,     0,    31,     0,    32,
      33,    34,    35,    36,    37,    38,    39,    40,   -59,     0,
      41,    42,    43,     0,    44,  -336,     0,    45,    46,    47,
      48,    49,     0,    50,    51,    52,    53,   -59,    54,    55,
       0,    56,    57,    58,     0,  -336,     0,     0,    59,    60,
      61,    62,   379,    64,    65,  -336,   -59,    66,    67,    68,
      69,     0,    70,    71,    72,     0,    73,    74,    75,    76,
      77,    78,    79,    80,     0,    81,    82,     0,    83,    84,
      85,    86,    87,     0,    88,    89,    90,   -59,    91,    92,
       0,     0,    93,     0,    94,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,    97,    98,    99,   100,     0,     0,
       0,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   103,   104,   105,   106,     0,     0,
     107,     0,   108,     0,   109,     0,   110,     0,     4,   111,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,     0,
      29,    30,     0,    31,     0,    32,    33,    34,    35,    36,
      37,    38,    39,    40,   -59,     0,    41,    42,    43,     0,
      44,  -336,     0,    45,    46,    47,    48,    49,     0,    50,
      51,    52,    53,   -59,    54,    55,     0,    56,    57,    58,
       0,  -336,     0,     0,    59,    60,    61,    62,     0,    64,
      65,  -336,   -59,    66,    67,    68,    69,     0,    70,    71,
      72,     0,    73,    74,    75,    76,    77,    78,    79,    80,
       0,    81,    82,     0,    83,    84,    85,    86,    87,     0,
      88,    89,    90,   -59,    91,    92,     0,     0,    93,     0,
      94,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,   106,     0,     0,   107,     0,   108,     0,
     109,     0,   110,     0,     4,   111,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,     0,    29,    30,     0,    31,
       0,    32,    33,    34,    35,    36,    37,    38,   782,    40,
     -59,     0,    41,    42,    43,     0,    44,  -336,     0,    45,
      46,    47,    48,    49,     0,    50,    51,    52,    53,   -59,
      54,    55,     0,    56,    57,    58,     0,  -336,     0,     0,
      59,    60,    61,    62,     0,    64,    65,  -336,   -59,    66,
      67,    68,    69,     0,    70,    71,    72,     0,    73,    74,
      75,    76,    77,    78,    79,    80,     0,    81,    82,     0,
      83,    84,    85,    86,    87,     0,    88,    89,    90,   -59,
      91,    92,     0,     0,    93,     0,    94,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   103,   104,   105,   106,
       0,     0,   107,     0,   108,     0,   109,     0,   110,     0,
       4,   111,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,    29,    30,     0,    31,     0,    32,    33,    34,
      35,    36,    37,    38,   788,    40,   -59,     0,    41,    42,
      43,     0,    44,  -336,     0,    45,    46,    47,    48,    49,
       0,    50,    51,    52,    53,   -59,    54,    55,     0,    56,
      57,    58,     0,  -336,     0,     0,    59,    60,    61,    62,
       0,    64,    65,  -336,   -59,    66,    67,    68,    69,     0,
      70,    71,    72,     0,    73,    74,    75,    76,    77,    78,
      79,    80,     0,    81,    82,     0,    83,    84,    85,    86,
      87,     0,    88,    89,    90,   -59,    91,    92,     0,     0,
      93,     0,    94,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   104,   105,   106,     0,     0,   107,     0,
     108,     0,   109,     0,   110,     0,     4,   111,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,     0,    29,    30,
       0,    31,     0,    32,    33,    34,    35,    36,    37,    38,
    1093,    40,   -59,     0,    41,    42,    43,     0,    44,  -336,
       0,    45,    46,    47,    48,    49,     0,    50,    51,    52,
      53,   -59,    54,    55,     0,    56,    57,    58,     0,  -336,
       0,     0,    59,    60,    61,    62,     0,    64,    65,  -336,
     -59,    66,    67,    68,    69,     0,    70,    71,    72,     0,
      73,    74,    75,    76,    77,    78,    79,    80,     0,    81,
      82,     0,    83,    84,    85,    86,    87,     0,    88,    89,
      90,   -59,    91,    92,     0,     0,    93,     0,    94,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,   106,     0,     0,   107,     0,   108,     0,   109,     0,
     110,     0,     4,   111,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,     0,    29,    30,     0,    31,     0,    32,
      33,    34,    35,    36,    37,    38,  1095,    40,   -59,     0,
      41,    42,    43,     0,    44,  -336,     0,    45,    46,    47,
      48,    49,     0,    50,    51,    52,    53,   -59,    54,    55,
       0,    56,    57,    58,     0,  -336,     0,     0,    59,    60,
      61,    62,     0,    64,    65,  -336,   -59,    66,    67,    68,
      69,     0,    70,    71,    72,     0,    73,    74,    75,    76,
      77,    78,    79,    80,     0,    81,    82,     0,    83,    84,
      85,    86,    87,     0,    88,    89,    90,   -59,    91,    92,
       0,     0,    93,     0,    94,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,    97,    98,    99,   100,     0,     0,
       0,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   103,   104,   105,   106,     0,     0,
     107,     0,   108,     0,   109,     0,   110,     0,     4,   111,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,     0,
      29,    30,     0,    31,     0,    32,    33,    34,    35,    36,
      37,    38,  1100,    40,   -59,     0,    41,    42,    43,     0,
      44,  -336,     0,    45,    46,    47,    48,    49,     0,    50,
      51,    52,    53,   -59,    54,    55,     0,    56,    57,    58,
       0,  -336,     0,     0,    59,    60,    61,    62,     0,    64,
      65,  -336,   -59,    66,    67,    68,    69,     0,    70,    71,
      72,     0,    73,    74,    75,    76,    77,    78,    79,    80,
       0,    81,    82,     0,    83,    84,    85,    86,    87,     0,
      88,    89,    90,   -59,    91,    92,     0,     0,    93,     0,
      94,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,   106,     0,     0,   107,     0,   108,     0,
     109,     0,   110,     0,     4,   111,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,    14,    15,
      16,    17,     0,    18,     0,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,     0,    29,    30,     0,    31,
       0,    32,    33,    34,    35,    36,    37,    38,  1103,    40,
     -59,     0,    41,    42,    43,     0,    44,  -336,     0,    45,
      46,    47,    48,    49,     0,    50,    51,    52,    53,   -59,
      54,    55,     0,    56,    57,    58,     0,  -336,     0,     0,
      59,    60,    61,    62,     0,    64,    65,  -336,   -59,    66,
      67,    68,    69,     0,    70,    71,    72,     0,    73,    74,
      75,    76,    77,    78,    79,    80,     0,    81,    82,     0,
      83,    84,    85,    86,    87,     0,    88,    89,    90,   -59,
      91,    92,     0,     0,    93,     0,    94,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   103,   104,   105,   106,
       0,     0,   107,     0,   108,     0,   109,     0,   110,     0,
       4,   111,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,     0,  1126,    30,     0,    31,     0,    32,    33,    34,
      35,    36,    37,    38,    39,    40,   -59,     0,    41,    42,
      43,     0,    44,  -336,     0,    45,    46,    47,    48,    49,
       0,    50,    51,    52,    53,   -59,    54,    55,     0,    56,
      57,    58,     0,  -336,     0,     0,    59,    60,    61,    62,
       0,    64,    65,  -336,   -59,    66,    67,    68,    69,     0,
      70,    71,    72,     0,    73,    74,    75,    76,    77,    78,
      79,    80,     0,    81,    82,     0,    83,    84,    85,    86,
      87,     0,    88,    89,    90,   -59,    91,    92,     0,     0,
      93,     0,    94,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   104,   105,   106,     0,     0,   107,     0,
     108,     0,   109,     0,   110,     0,     4,   111,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
      14,    15,    16,    17,     0,    18,     0,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,     0,    29,    30,
       0,    31,     0,    32,    33,    34,    35,    36,    37,    38,
    1132,    40,   -59,     0,    41,    42,    43,     0,    44,  -336,
       0,    45,    46,    47,    48,    49,     0,    50,    51,    52,
      53,   -59,    54,    55,     0,    56,    57,    58,     0,  -336,
       0,     0,    59,    60,    61,    62,     0,    64,    65,  -336,
     -59,    66,    67,    68,    69,     0,    70,    71,    72,     0,
      73,    74,    75,    76,    77,    78,    79,    80,     0,    81,
      82,     0,    83,    84,    85,    86,    87,     0,    88,    89,
      90,   -59,    91,    92,     0,     0,    93,     0,    94,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,   106,     0,     0,   107,     0,   108,     0,   109,     0,
     110,     0,     4,   111,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,     0,    29,    30,     0,    31,     0,    32,
      33,    34,    35,    36,    37,    38,  1136,    40,   -59,     0,
      41,    42,    43,     0,    44,  -336,     0,    45,    46,    47,
      48,    49,     0,    50,    51,    52,    53,   -59,    54,    55,
       0,    56,    57,    58,     0,  -336,     0,     0,    59,    60,
      61,    62,     0,    64,    65,  -336,   -59,    66,    67,    68,
      69,     0,    70,    71,    72,     0,    73,    74,    75,    76,
      77,    78,    79,    80,     0,    81,    82,     0,    83,    84,
      85,    86,    87,     0,    88,    89,    90,   -59,    91,    92,
       0,     0,    93,     0,    94,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,    97,    98,    99,   100,     0,     0,
       0,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   103,   104,   105,   106,     0,     0,
     107,     0,   108,     0,   109,     0,   110,     0,   905,   111,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   198,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,   308,     0,   199,     0,     0,     0,    34,   200,   201,
       0,     0,   202,    40,     0,     0,     0,   310,     0,     0,
      44,     0,     0,   203,     0,     0,    48,    49,     0,     0,
      51,     0,    53,     0,    54,    55,     0,    56,    57,     0,
       0,     0,     0,     0,     0,    60,     0,    62,     0,    64,
       0,     0,     0,     0,    67,    68,   204,     0,     0,     0,
       0,     0,     0,     0,    75,   315,    77,    78,    79,   316,
       0,    81,     0,     0,    83,     0,     0,     0,    87,     0,
       0,    89,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,     0,     0,     0,   107,     0,   206,     0,
     109,     0,   207,     0,     0,   111,     5,     6,     7,     8,
       9,    10,    11,    12,     0,   654,     0,    13,     0,     0,
      16,    17,   656,    18,     0,   198,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,    30,     0,   199,
       0,     0,     0,    34,   200,   201,     0,     0,   202,    40,
       0,     0,     0,    42,     0,     0,    44,     0,   661,   203,
       0,     0,    48,    49,     0,     0,    51,     0,    53,     0,
      54,    55,     0,    56,    57,     0,     0,     0,     0,     0,
       0,    60,     0,    62,     0,    64,     0,     0,     0,     0,
      67,    68,   204,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,   205,     0,    81,     0,     0,
      83,     0,     0,     0,    87,     0,     0,    89,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,    95,
     533,   459,   460,   461,   462,   463,     0,     0,   466,   467,
     468,   469,     0,   471,   472,   872,   873,   874,   875,   876,
     674,     0,   675,     0,   101,     0,   676,   677,   678,   679,
     680,   681,   682,   877,   684,   685,   103,   878,   105,     0,
     687,   688,   879,   690,   206,     0,   109,     0,   207,     0,
       0,   111,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,     0,    16,    17,     0,    18,
       0,   198,    20,    21,    22,     0,     0,     0,     0,    27,
       0,     0,    29,    30,     0,   199,     0,     0,     0,    34,
      35,    36,    37,     0,    39,    40,     0,     0,     0,    42,
       0,     0,    44,     0,     0,   203,     0,     0,    48,    49,
       0,    50,    51,    52,    53,     0,    54,    55,     0,    56,
      57,    58,     0,     0,     0,     0,     0,    60,     0,    62,
       0,    64,     0,     0,     0,     0,    67,    68,   204,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,    78,
      79,    80,     0,    81,    82,     0,    83,     0,     0,     0,
      87,     0,     0,    89,     0,     0,     0,    92,     0,     0,
      93,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   104,   105,     0,     0,     0,   107,     0,
     108,     0,   109,     0,   110,     0,     0,   111,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   198,     0,     0,
      22,   386,    24,     0,     0,     0,     0,     0,     0,    30,
       0,   199,     0,     0,     0,    34,   200,   201,     0,     0,
     202,    40,     0,     0,     0,    42,     0,     0,    44,     0,
       0,   203,     0,     0,    48,    49,     0,     0,    51,     0,
      53,     0,    54,    55,     0,    56,    57,     0,     0,     0,
       0,     0,     0,    60,    61,    62,     0,    64,     0,     0,
       0,     0,    67,    68,   204,     0,    70,     0,     0,     0,
       0,     0,    75,    76,    77,    78,    79,   205,     0,    81,
       0,     0,    83,     0,     0,     0,    87,     0,     0,    89,
       0,     0,    91,    92,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,     0,     0,     0,   107,     0,   206,     0,   109,     0,
     207,     0,     0,   111,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   198,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,   199,     0,     0,
       0,    34,   200,   201,     0,     0,   202,    40,     0,     0,
       0,    42,     0,     0,    44,     0,     0,   203,     0,     0,
      48,    49,     0,     0,    51,     0,    53,     0,    54,    55,
       0,    56,    57,     0,     0,     0,     0,     0,     0,    60,
       0,    62,     0,    64,     0,     0,     0,     0,    67,    68,
     204,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,    79,   205,     0,    81,     0,     0,    83,   330,
     331,     0,    87,   368,     0,    89,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,    97,    98,    99,   100,     0,     0,
       0,     0,   101,   369,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   103,   104,   105,     0,     0,     0,
     107,     0,   206,     0,   109,     0,   207,     0,     0,   111,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   198,
       0,     0,    22,     0,   761,     0,     0,     0,     0,     0,
       0,    30,     0,   199,     0,     0,     0,    34,   200,   201,
       0,     0,   202,    40,     0,   762,     0,    42,     0,     0,
      44,     0,     0,   203,     0,     0,    48,    49,     0,     0,
      51,     0,    53,     0,    54,    55,     0,    56,    57,     0,
       0,     0,     0,     0,     0,    60,     0,    62,     0,    64,
       0,     0,     0,     0,    67,    68,   204,     0,   763,     0,
       0,     0,     0,     0,    75,    76,    77,    78,    79,   205,
       0,    81,     0,     0,    83,     0,     0,     0,    87,     0,
       0,    89,     0,     0,   764,    92,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,     0,     0,     0,   107,     0,   206,     0,
     109,     0,   207,     0,     0,   111,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   198,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,    30,     0,   199,
       0,     0,     0,    34,   200,   201,     0,     0,   202,    40,
       0,     0,     0,    42,     0,     0,    44,     0,     0,   203,
       0,     0,    48,    49,     0,     0,    51,     0,    53,     0,
      54,    55,     0,    56,    57,     0,     0,     0,     0,     0,
       0,    60,     0,    62,     0,    64,     0,     0,     0,     0,
      67,    68,   204,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,   205,     0,    81,     0,     0,
      83,   330,   331,     0,    87,   368,     0,    89,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   103,   104,   105,     0,
       0,     0,   107,     0,   206,     0,   109,   845,   207,     0,
       0,   111,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   198,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,     0,    30,     0,   199,     0,     0,     0,    34,
     200,   201,     0,     0,   202,    40,     0,     0,     0,    42,
       0,     0,    44,     0,     0,   203,     0,     0,    48,    49,
       0,     0,    51,     0,    53,     0,    54,    55,     0,    56,
      57,     0,     0,     0,     0,     0,     0,    60,     0,    62,
       0,    64,     0,     0,     0,     0,    67,    68,   204,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,   205,     0,    81,     0,     0,    83,   330,   331,     0,
      87,   368,     0,    89,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   104,   105,     0,     0,     0,   107,     0,
     206,     0,   109,   847,   207,     0,     0,   111,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   198,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,    30,
       0,   199,     0,     0,     0,    34,   200,   201,     0,     0,
     202,    40,     0,     0,     0,    42,     0,     0,    44,     0,
       0,   203,     0,     0,    48,    49,     0,     0,    51,     0,
      53,     0,    54,    55,     0,    56,    57,     0,     0,     0,
       0,     0,     0,    60,     0,    62,     0,    64,     0,     0,
       0,     0,    67,    68,   204,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,    79,   205,     0,    81,
       0,     0,    83,   330,   331,     0,    87,     0,     0,    89,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,   332,     0,     0,   107,     0,   206,     0,   109,     0,
     207,     0,     0,   111,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   198,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,   199,     0,     0,
       0,    34,   200,   201,     0,     0,   202,    40,     0,     0,
       0,    42,     0,     0,    44,     0,     0,   203,     0,     0,
      48,    49,     0,     0,    51,     0,    53,     0,    54,    55,
    1000,    56,    57,     0,     0,     0,     0,     0,     0,    60,
       0,    62,     0,    64,     0,     0,     0,     0,    67,    68,
     204,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,    79,   205,     0,    81,     0,     0,    83,   330,
     331,     0,    87,     0,     0,    89,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,    97,    98,    99,   100,     0,     0,
       0,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   103,   104,   105,     0,     0,     0,
     107,     0,   206,     0,   109,     0,   207,     0,     0,   111,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   198,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,    30,     0,   199,     0,     0,     0,    34,   200,   201,
       0,     0,   202,    40,     0,     0,     0,    42,     0,     0,
      44,     0,     0,   203,     0,     0,    48,    49,     0,     0,
      51,     0,    53,     0,    54,    55,     0,    56,    57,     0,
       0,     0,     0,     0,     0,    60,   265,    62,     0,    64,
       0,     0,     0,     0,    67,    68,   204,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,    79,   205,
       0,    81,     0,     0,    83,     0,     0,     0,    87,     0,
       0,    89,     0,     0,     0,    92,     0,     0,     0,     0,
       0,   266,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,     0,     0,     0,   107,     0,   206,     0,
     109,     0,   207,     0,     0,   111,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   198,     0,     0,    22,     0,
     273,     0,     0,     0,     0,     0,     0,    30,     0,   199,
       0,     0,     0,    34,   200,   201,     0,     0,   202,    40,
       0,     0,     0,    42,     0,     0,    44,     0,     0,   203,
       0,     0,    48,    49,     0,     0,    51,     0,    53,     0,
      54,    55,     0,    56,    57,     0,     0,     0,     0,     0,
       0,    60,     0,    62,     0,    64,     0,     0,     0,     0,
      67,    68,   204,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,   205,     0,    81,     0,     0,
      83,     0,     0,     0,    87,     0,     0,    89,     0,     0,
     274,    92,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   103,   104,   105,     0,
       0,     0,   107,     0,   206,     0,   109,     0,   207,     0,
       0,   111,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   198,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,     0,    30,     0,   199,     0,     0,     0,    34,
     200,   201,     0,     0,   202,    40,     0,     0,     0,    42,
       0,     0,    44,     0,     0,   203,     0,     0,    48,    49,
       0,     0,    51,     0,    53,     0,    54,    55,     0,    56,
      57,     0,     0,     0,     0,     0,     0,    60,     0,    62,
       0,    64,     0,     0,     0,     0,    67,    68,   204,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,   205,     0,    81,     0,     0,    83,   330,   331,     0,
      87,     0,     0,    89,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   104,   105,     0,     0,     0,   107,     0,
     206,     0,   109,     0,   207,     0,     0,   111,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   198,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,    30,
       0,   199,     0,     0,     0,    34,   200,   201,     0,     0,
     202,    40,     0,     0,     0,    42,     0,     0,    44,     0,
       0,   203,     0,     0,    48,    49,     0,     0,    51,     0,
      53,     0,    54,    55,     0,    56,    57,     0,     0,     0,
       0,     0,     0,    60,     0,    62,     0,    64,     0,     0,
       0,     0,    67,    68,   204,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,    79,   205,     0,    81,
       0,     0,    83,     0,     0,     0,    87,     0,     0,    89,
       0,     0,     0,    92,     0,     0,     0,     0,     0,   266,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,     0,     0,     0,   107,     0,   206,     0,   109,     0,
     207,   373,     0,   111,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   198,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,   199,     0,     0,
       0,    34,   200,   201,     0,     0,   202,    40,     0,     0,
       0,    42,     0,     0,    44,     0,     0,   203,     0,     0,
      48,    49,     0,     0,    51,     0,    53,     0,    54,    55,
       0,    56,    57,     0,     0,     0,     0,     0,     0,    60,
       0,    62,     0,    64,     0,     0,     0,     0,    67,    68,
     204,  -724,     0,     0,     0,  -724,     0,     0,    75,    76,
      77,    78,    79,   205,     0,    81,     0,     0,    83,     0,
       0,     0,    87,     0,     0,    89,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,    97,    98,    99,   100,     0,     0,
       0,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   103,   104,   105,     0,     0,     0,
     107,     0,   206,     0,   109,     0,   207,     0,     0,   111,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   198,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,    30,     0,   199,     0,     0,     0,    34,   200,   201,
       0,     0,   202,    40,     0,     0,     0,    42,     0,     0,
      44,     0,     0,   203,     0,     0,    48,    49,     0,     0,
      51,     0,    53,     0,    54,    55,     0,    56,    57,     0,
       0,     0,     0,     0,     0,    60,     0,    62,     0,    64,
       0,     0,     0,     0,    67,    68,   204,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,    79,   205,
       0,    81,     0,     0,    83,     0,     0,     0,    87,     0,
       0,    89,     0,     0,     0,    92,     0,     0,     0,     0,
       0,   266,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,     0,     0,     0,   107,     0,   206,     0,
     109,     0,   207,     0,     0,   111,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   198,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,   288,    30,     0,   199,
       0,     0,     0,    34,   200,   201,     0,     0,   202,    40,
       0,     0,     0,    42,     0,     0,    44,     0,     0,   203,
       0,     0,    48,    49,     0,     0,    51,     0,    53,     0,
      54,    55,     0,    56,    57,     0,     0,     0,     0,     0,
       0,    60,     0,    62,     0,    64,     0,     0,     0,     0,
      67,    68,   204,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,   205,     0,    81,     0,     0,
      83,     0,     0,     0,    87,     0,     0,    89,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   103,   104,   105,     0,
       0,     0,   107,     0,   108,     0,   109,     0,   207,     0,
       0,   111,     5,     6,     7,     8,     9,   389,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,    19,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,     0,    30,     0,   199,     0,     0,     0,    34,
     200,   201,     0,     0,   202,    40,     0,     0,     0,    42,
       0,     0,    44,     0,     0,   203,     0,     0,    48,    49,
       0,     0,    51,     0,    53,     0,    54,    55,     0,    56,
      57,     0,     0,     0,     0,     0,     0,    60,     0,    62,
       0,    64,     0,     0,     0,     0,    67,    68,    69,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,   205,     0,    81,     0,     0,    83,     0,     0,     0,
      87,     0,    88,    89,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   104,   105,     0,     0,     0,   107,     0,
     206,     0,   109,     0,   207,     0,     0,   111,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   198,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,    30,
       0,   199,     0,     0,     0,    34,   200,   201,     0,     0,
     202,    40,     0,     0,     0,    42,     0,     0,    44,     0,
       0,   203,     0,     0,    48,    49,     0,     0,    51,     0,
      53,     0,    54,    55,     0,    56,    57,     0,     0,     0,
       0,     0,     0,    60,     0,    62,     0,    64,     0,     0,
       0,     0,    67,    68,   204,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,    79,   205,     0,    81,
       0,     0,    83,     0,     0,     0,    87,     0,     0,    89,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,     0,     0,     0,   107,     0,   206,     0,   109,     0,
     207,   373,     0,   111,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   198,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,   199,     0,     0,
       0,    34,   200,   201,     0,     0,   202,    40,     0,     0,
       0,    42,     0,     0,    44,     0,     0,   203,     0,     0,
      48,    49,     0,     0,    51,     0,    53,     0,    54,    55,
       0,    56,    57,     0,     0,     0,     0,     0,     0,    60,
       0,    62,     0,    64,     0,     0,     0,     0,    67,    68,
     204,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,    79,   205,     0,    81,     0,     0,    83,     0,
       0,     0,    87,     0,     0,    89,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,    97,    98,    99,   100,     0,     0,
       0,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   103,   104,   105,     0,     0,     0,
     107,     0,   206,   843,   109,     0,   207,     0,     0,   111,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   198,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,    30,     0,   199,     0,     0,     0,    34,   200,   201,
       0,     0,   202,    40,     0,     0,     0,    42,     0,     0,
      44,     0,     0,   203,     0,     0,    48,    49,     0,     0,
      51,     0,    53,     0,    54,    55,     0,    56,    57,     0,
       0,     0,     0,     0,     0,    60,     0,    62,     0,    64,
       0,     0,     0,     0,    67,    68,   204,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,    79,   205,
       0,    81,     0,     0,    83,     0,     0,     0,    87,     0,
       0,    89,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,     0,     0,     0,   107,     0,   206,     0,
     109,     0,   207,   853,     0,   111,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   198,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,    30,     0,   199,
       0,     0,     0,    34,   200,   201,     0,     0,   202,    40,
       0,     0,     0,    42,     0,     0,    44,     0,     0,   203,
       0,     0,    48,    49,     0,     0,    51,     0,    53,     0,
      54,    55,     0,    56,    57,     0,     0,     0,     0,     0,
       0,    60,     0,    62,     0,    64,     0,     0,     0,     0,
      67,    68,   204,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,   205,     0,    81,     0,     0,
      83,     0,     0,     0,    87,     0,     0,    89,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   103,   104,   105,     0,
       0,     0,   107,     0,   206,     0,   109,     0,   207,   858,
       0,   111,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   198,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,     0,    30,     0,   199,     0,     0,     0,    34,
     200,   201,     0,     0,   202,    40,     0,     0,     0,    42,
       0,     0,    44,     0,     0,   203,     0,     0,    48,    49,
       0,     0,    51,     0,    53,     0,    54,    55,     0,    56,
      57,     0,     0,     0,     0,     0,     0,    60,     0,    62,
       0,    64,     0,     0,     0,     0,    67,    68,   204,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,   205,     0,    81,     0,     0,    83,     0,     0,     0,
      87,  1036,     0,    89,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   104,   105,     0,     0,     0,   107,     0,
     206,     0,   109,     0,   207,     0,     0,   111,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   198,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,    30,
       0,   199,     0,     0,     0,    34,   200,   201,     0,     0,
     202,    40,     0,     0,     0,    42,     0,     0,    44,     0,
       0,   203,     0,     0,    48,    49,     0,     0,    51,     0,
      53,     0,    54,    55,     0,    56,    57,     0,     0,     0,
       0,     0,     0,    60,     0,    62,     0,    64,     0,     0,
       0,     0,    67,    68,   204,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,    79,   205,     0,    81,
       0,     0,    83,     0,     0,     0,    87,  1207,     0,    89,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,     0,     0,     0,   107,     0,   206,     0,   109,     0,
     207,     0,     0,   111,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   198,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,   199,     0,     0,
       0,    34,   200,   201,     0,     0,   202,    40,     0,     0,
       0,    42,     0,     0,    44,     0,     0,   203,     0,     0,
      48,    49,     0,     0,    51,     0,    53,     0,    54,    55,
       0,    56,    57,     0,     0,     0,     0,     0,     0,    60,
       0,    62,     0,    64,     0,     0,     0,     0,    67,    68,
     204,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,    79,   205,     0,    81,     0,     0,    83,     0,
       0,     0,    87,     0,     0,    89,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,    97,    98,    99,   100,     0,     0,
       0,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   103,   104,   105,     0,     0,     0,
     107,     0,   206,     0,   109,     0,   207,     0,     0,   111,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   198,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,   308,     0,   199,     0,     0,     0,    34,   200,   201,
       0,     0,   202,    40,     0,     0,     0,   310,     0,     0,
      44,     0,     0,   203,     0,     0,    48,    49,     0,     0,
      51,     0,    53,     0,    54,    55,     0,    56,    57,     0,
       0,     0,     0,     0,     0,    60,     0,    62,     0,    64,
       0,     0,     0,     0,    67,    68,   204,     0,     0,     0,
       0,     0,     0,     0,    75,   315,    77,    78,    79,   316,
       0,    81,     0,     0,    83,     0,     0,     0,    87,     0,
       0,    89,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,     0,     0,     0,   107,     0,   206,     0,
     109,     0,   207,     0,     0,   111,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   198,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,    30,     0,   199,
       0,     0,     0,    34,   200,   201,     0,     0,   930,    40,
       0,     0,     0,    42,     0,     0,    44,     0,     0,   203,
       0,     0,    48,    49,     0,     0,    51,     0,    53,     0,
      54,    55,     0,    56,    57,     0,     0,     0,     0,     0,
       0,    60,     0,    62,     0,    64,     0,     0,     0,     0,
      67,    68,   204,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,   205,     0,    81,     0,     0,
      83,     0,     0,     0,    87,     0,     0,    89,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   103,   104,   105,     0,
       0,     0,   107,     0,   206,     0,   109,     0,   207,     0,
       0,   111,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   198,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,     0,    30,     0,   199,     0,     0,     0,    34,
     200,   201,     0,     0,   933,    40,     0,     0,     0,    42,
       0,     0,    44,     0,     0,   203,     0,     0,    48,    49,
       0,     0,    51,     0,    53,     0,    54,    55,     0,    56,
      57,     0,     0,     0,     0,     0,     0,    60,     0,    62,
       0,    64,     0,     0,     0,     0,    67,    68,   204,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,   205,     0,    81,     0,     0,    83,     0,     0,     0,
      87,     0,     0,    89,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   104,   105,     0,     0,     0,   107,     0,
     206,     0,   109,     0,   207,     0,     0,   111,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   198,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,    30,
       0,   199,     0,     0,     0,    34,   200,   201,     0,     0,
    1176,    40,     0,     0,     0,    42,     0,     0,    44,     0,
       0,   203,     0,     0,    48,    49,     0,     0,    51,     0,
      53,     0,    54,    55,     0,    56,    57,     0,     0,     0,
       0,     0,     0,    60,     0,    62,     0,    64,     0,     0,
       0,     0,    67,    68,   204,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,    79,   205,     0,    81,
       0,     0,    83,     0,     0,     0,    87,     0,     0,    89,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,     0,     0,     0,   107,     0,   206,     0,   109,     0,
     207,     0,     0,   111,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   198,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,     0,    30,     0,   199,     0,     0,
       0,    34,   200,   201,     0,     0,  1177,    40,     0,     0,
       0,    42,     0,     0,    44,     0,     0,   203,     0,     0,
      48,    49,     0,     0,    51,     0,    53,     0,    54,    55,
       0,    56,    57,     0,     0,     0,     0,     0,     0,    60,
       0,    62,     0,    64,     0,     0,     0,     0,    67,    68,
     204,     0,     0,     0,     0,     0,     0,     0,    75,    76,
      77,    78,    79,   205,     0,    81,     0,     0,    83,     0,
       0,     0,    87,     0,     0,    89,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,    97,    98,    99,   100,     0,     0,
       0,     0,   101,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   103,   104,   105,     0,     0,     0,
     107,     0,   206,     0,   109,     0,   207,     0,     0,   111,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   198,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,    30,     0,   199,     0,     0,     0,    34,   200,   201,
       0,     0,  1179,    40,     0,     0,     0,    42,     0,     0,
      44,     0,     0,   203,     0,     0,    48,    49,     0,     0,
      51,     0,    53,     0,    54,    55,     0,    56,    57,     0,
       0,     0,     0,     0,     0,    60,     0,    62,     0,    64,
       0,     0,     0,     0,    67,    68,   204,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,    79,   205,
       0,    81,     0,     0,    83,     0,     0,     0,    87,     0,
       0,    89,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,     0,     0,     0,   107,     0,   206,     0,
     109,     0,   207,     0,     0,   111,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   198,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,     0,    30,     0,   199,
       0,     0,     0,    34,   200,   201,     0,     0,  1180,    40,
       0,     0,     0,    42,     0,     0,    44,     0,     0,   203,
       0,     0,    48,    49,     0,     0,    51,     0,    53,     0,
      54,    55,     0,    56,    57,     0,     0,     0,     0,     0,
       0,    60,     0,    62,     0,    64,     0,     0,     0,     0,
      67,    68,   204,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,   205,     0,    81,     0,     0,
      83,     0,     0,     0,    87,     0,     0,    89,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,    95,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    96,    97,    98,    99,   100,
       0,     0,     0,     0,   101,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,   103,   104,   105,     0,
       0,     0,   107,     0,   206,     0,   109,     0,   207,     0,
       0,   111,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   198,     0,     0,    22,     0,     0,     0,     0,     0,
       0,     0,     0,    30,     0,   199,     0,     0,     0,    34,
     200,   201,     0,     0,  1181,    40,     0,     0,     0,    42,
       0,     0,    44,     0,     0,   203,     0,     0,    48,    49,
       0,     0,    51,     0,    53,     0,    54,    55,     0,    56,
      57,     0,     0,     0,     0,     0,     0,    60,     0,    62,
       0,    64,     0,     0,     0,     0,    67,    68,   204,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,   205,     0,    81,     0,     0,    83,     0,     0,     0,
      87,     0,     0,    89,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,   103,   104,   105,     0,     0,     0,   107,     0,
     206,     0,   109,     0,   207,     0,     0,   111,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   198,     0,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,    30,
       0,   199,     0,     0,     0,    34,   200,   201,     0,     0,
    1182,    40,     0,     0,     0,    42,     0,     0,    44,     0,
       0,   203,     0,     0,    48,    49,     0,     0,    51,     0,
      53,     0,    54,    55,     0,    56,    57,     0,     0,     0,
       0,     0,     0,    60,     0,    62,     0,    64,     0,     0,
       0,     0,    67,    68,   204,     0,     0,     0,     0,     0,
       0,     0,    75,    76,    77,    78,    79,   205,     0,    81,
       0,     0,    83,     0,     0,     0,    87,     0,     0,    89,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,    97,    98,
      99,   100,     0,     0,     0,     0,   101,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,     0,     0,     0,   107,     0,   206,     0,   109,     0,
     207,     0,     0,   111,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,     0,    17,     0,
       0,     0,   198,     0,     0,     0,     0,   307,     0,     0,
       0,     0,     0,     0,     0,     0,   199,     0,     0,     0,
       0,   200,   201,     0,     0,   202,     0,     0,   309,     0,
       0,     0,   311,     0,     0,     0,   203,     0,     0,    48,
      49,     0,     0,     0,     0,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,     0,   312,     0,    60,   313,
      62,    63,    64,     0,     0,     0,     0,    67,     0,   204,
       0,   314,     0,     0,     0,     0,     0,    75,     0,    77,
       0,    79,     0,     0,     0,     0,     0,     0,     0,     0,
     317,     0,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   652,     0,   653,
       0,     0,    96,    97,    98,    99,   100,     0,   654,     0,
       0,   101,     0,   655,   239,   656,   657,     0,     0,     0,
     102,   658,     0,   103,   104,   105,     0,     0,     0,   107,
     242,   206,   199,   109,     0,   207,   243,     0,   111,     0,
       0,     0,   659,     0,     0,     0,   245,     0,     0,   660,
       0,   661,     0,     0,     0,     0,     0,     0,     0,   662,
       0,     0,     0,     0,     0,     0,   248,   663,     0,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   664,     0,     0,     0,     0,     0,
       0,     0,     0,   252,   253,     0,   665,     0,   255,     0,
     666,     0,     0,   257,     0,     0,     0,   667,     0,     0,
     259,     0,     0,     0,   668,     0,     0,     0,     0,     0,
       0,     0,     0,   533,   459,   460,   461,   462,   463,     0,
       0,   466,   467,   468,   469,     0,   471,   472,   669,   670,
     671,   672,   673,   674,     0,   675,     0,     0,     0,   676,
     677,   678,   679,   680,   681,   682,   683,   684,   685,     0,
     686,     0,     0,   687,   688,   689,   690,     0,     0,   691,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,    16,    17,     0,    18,     0,   198,
       0,     0,    22,     0,     0,     0,     0,     0,     0,     0,
       0,    30,     0,   199,     0,     0,     0,    34,   200,   201,
       0,     0,   202,    40,     0,     0,     0,    42,     0,     0,
      44,     0,     0,   203,     0,     0,    48,    49,     0,     0,
      51,     0,    53,     0,    54,    55,     0,    56,    57,     0,
       0,     0,     0,     0,     0,    60,     0,    62,     0,    64,
       0,     0,     0,     0,    67,    68,   204,     0,     0,     0,
       0,     0,     0,     0,    75,    76,    77,    78,    79,   205,
       0,    81,     0,     0,    83,     0,     0,     0,    87,     0,
       0,    89,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
       0,   104,   105,     0,     0,     0,   107,     0,   206,     0,
     109,     0,   207,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   198,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,    30,     0,   199,     0,     0,     0,
      34,   200,   201,     0,     0,   202,    40,     0,     0,     0,
      42,     0,     0,    44,     0,     0,   203,     0,     0,    48,
      49,     0,     0,    51,     0,    53,     0,    54,    55,     0,
      56,    57,     0,     0,     0,     0,     0,     0,    60,     0,
      62,     0,    64,     0,     0,     0,     0,    67,    68,   204,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,   205,     0,    81,     0,     0,    83,     0,     0,
       0,    87,     0,     0,    89,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,     0,    98,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   198,   105,     0,    22,     0,     0,
       0,   206,     0,   109,     0,   207,    30,     0,   199,     0,
       0,     0,    34,   200,   201,     0,     0,   202,    40,     0,
       0,     0,    42,     0,     0,    44,     0,     0,   203,     0,
       0,    48,    49,     0,     0,    51,     0,    53,     0,    54,
      55,     0,    56,    57,     0,     0,     0,     0,     0,     0,
      60,     0,    62,     0,    64,     0,     0,     0,     0,    67,
      68,   204,     0,   237,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,   205,     0,    81,   238,   239,    83,
     240,     0,     0,    87,     0,   241,    89,     0,     0,     0,
      92,     0,     0,     0,   242,     0,     0,     0,     0,     0,
     243,     0,     0,   716,     0,     0,   244,     0,     0,     0,
     245,     0,   654,   246,    96,     0,    98,     0,     0,   656,
     717,     0,     0,   247,     0,     0,     0,     0,     0,     0,
     248,   249,     0,     0,   718,     0,     0,   105,   250,     0,
       0,     0,     0,     0,     0,   109,     0,   207,   251,     0,
       0,     0,     0,     0,     0,   661,     0,   252,   253,     0,
     254,     0,   255,   719,   256,     0,     0,   257,     0,     0,
       0,   258,     0,     0,   259,     0,     0,     0,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   720,     0,     0,     0,     0,     0,
     721,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   533,   459,   460,
     461,   462,   463,   284,     0,   466,   467,   468,   469,     0,
     471,   472,   669,   670,   671,   672,   673,   674,     0,   675,
       0,     0,     0,   676,   677,   678,   679,   680,   681,   682,
     683,   684,   685,   237,   686,     0,     0,   687,   688,   689,
     690,  1033,   654,     0,     0,     0,     0,   238,   239,   656,
     240,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,   244,     0,     0,     0,
     245,     0,     0,   246,     0,   661,     0,     0,     0,     0,
       0,     0,     0,   247,     0,     0,     0,     0,     0,     0,
     248,   249,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,     0,     0,   428,   252,   253,     0,
     254,     0,   255,   429,   256,     0,     0,   257,     0,     0,
       0,   258,     0,     0,   259,     0,   430,     0,   260,     0,
       0,     0,     0,     0,     0,     0,     0,   533,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,   669,   670,   671,   672,   673,   674,     0,   675,
       0,     0,     0,   676,   677,   678,   679,   680,   681,   682,
     683,   684,   685,     0,   686,     0,     0,   687,   688,   689,
     690,     0,   428,     0,   431,     0,     0,     0,   432,   429,
       0,     0,     0,     0,     0,     0,     0,   532,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,   533,   459,   460,   461,   462,   463,     0,     0,   466,
     467,   468,   469,     0,   471,   472,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
     431,   454,   455,   456,   432,   488,     0,   428,     0,     0,
       0,     0,   457,  1092,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   533,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
       0,   488,     0,   428,     0,   431,     0,     0,   457,   432,
     429,     0,     0,     0,     0,     0,     0,     0,  1099,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,   533,   459,   460,   461,   462,   463,     0,     0,
     466,   467,   468,   469,     0,   471,   472,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   431,   454,   455,   456,   432,   488,     0,   428,     0,
       0,     0,     0,   457,  1242,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,   433,   533,   459,
     460,   461,   462,   463,     0,     0,   466,   467,   468,   469,
       0,   471,   472,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,     0,   488,     0,   428,     0,   431,     0,     0,   457,
     432,   429,     0,     0,     0,     0,     0,     0,     0,  1243,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   533,   459,   460,   461,   462,   463,     0,
       0,   466,   467,   468,   469,     0,   471,   472,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,   431,   454,   455,   456,   432,   488,     0,   428,
       0,     0,     0,     0,   457,  1244,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   533,
     459,   460,   461,   462,   463,     0,     0,   466,   467,   468,
     469,     0,   471,   472,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,     0,   488,     0,   428,     0,   431,     0,     0,
     457,   432,   429,     0,     0,     0,     0,     0,     0,     0,
    1245,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,   533,   459,   460,   461,   462,   463,
       0,     0,   466,   467,   468,   469,     0,   471,   472,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   431,   454,   455,   456,   432,   488,     0,
     428,     0,     0,     0,     0,   457,  1250,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     533,   459,   460,   461,   462,   463,     0,     0,   466,   467,
     468,   469,     0,   471,   472,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,     0,
     454,   455,   456,     0,   488,     0,   428,     0,   431,     0,
       0,   457,   432,   429,     0,     0,     0,     0,     0,     0,
       0,  1251,     0,     0,     0,     0,   430,   519,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   520,     0,   433,   533,   459,   460,   461,   462,
     463,     0,     0,   466,   467,   468,   469,     0,   471,   472,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,   431,   454,   455,   456,   432,   488,
       0,   428,     0,     0,     0,     0,   457,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,   288,     0,     0,     0,   516,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,   525,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,     0,   488,     0,   428,     0,   431,
       0,     0,   457,   432,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,   513,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,     0,   514,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,   431,   454,   455,   456,   432,
     488,     0,     0,     0,     0,     0,     0,   457,     0,   428,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,   433,     0,     0,   622,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     623,     0,   454,   455,   456,     0,   488,     0,     0,     0,
       0,     0,     0,   457,     0,     0,     0,   431,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   428,   868,     0,     0,     0,     0,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,   433,     0,     0,   869,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   870,   624,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,   428,     0,   431,
       0,     0,     0,   432,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,   977,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,   431,   454,   455,   456,   432,
       0,     0,   428,     0,     0,     0,     0,   457,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,   288,     0,     0,     0,   516,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,     0,   454,   455,   456,     0,   488,     0,   428,     0,
     431,     0,     0,   457,   432,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,   288,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   516,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,   431,   454,   455,   456,
     432,   488,     0,   428,     0,     0,     0,     0,   457,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,   752,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,   753,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,     0,   488,     0,   428,
       0,   431,     0,     0,   457,   432,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
     754,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   755,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   431,   454,   455,
     456,   432,     0,     0,   428,     0,     0,     0,     0,   457,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,   969,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,     0,     0,     0,
     428,     0,   431,     0,     0,   457,   432,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,   428,     0,     0,     0,     0,     0,
       0,   429,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,   430,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,   431,   454,
     455,   456,   432,   488,     0,     0,     0,     0,     0,     0,
     457,   991,     0,     0,     0,     0,     0,     0,   428,   556,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,   430,     0,
       0,     0,   431,     0,     0,     0,   432,     0,     0,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,   516,   454,   455,   456,   433,   488,
       0,     0,     0,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,   434,   435,   431,   436,   437,   438,
     432,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   428,     0,   454,
     455,   456,     0,     0,   429,     0,     0,     0,  1027,     0,
     457,     0,   433,     0,     0,     0,     0,   430,     0,     0,
       0,     0,  1145,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,     0,     0,  1146,     0,
       0,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,     0,   428,     0,   431,     0,     0,     0,   432,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,   428,     0,     0,     0,     0,     0,     0,
     429,     0,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,   430,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   431,   454,   455,   456,   432,     0,     0,   428,     0,
       0,     0,     0,   457,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,   431,     0,     0,     0,   432,     0,     0,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   433,   454,   455,
     456,     0,   580,     0,     0,     0,   431,     0,     0,   457,
     432,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   428,   584,   454,   455,
     456,   609,   433,   429,     0,     0,     0,     0,     0,   457,
       0,     0,     0,     0,     0,     0,   430,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   428,     0,   454,   455,   456,     0,     0,   429,     0,
       0,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,     0,     0,     0,   432,     0,
       0,   428,   864,     0,     0,     0,     0,   756,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,   432,     0,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,     0,   433,     0,   428,     0,   431,
       0,     0,   457,   432,   429,     0,     0,     0,     0,     0,
       0,   434,   435,     0,   436,   437,   438,   430,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,   433,   454,   455,   456,     0,
       0,   428,     0,   844,     0,     0,     0,   457,   429,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   865,
     441,   430,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,   431,   454,   455,   456,   432,
       0,     0,     0,     0,     0,     0,   428,   457,     0,     0,
       0,     0,     0,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,   218,     0,   989,
       0,   433,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,   432,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,     0,   454,   455,   456,   433,     0,     0,     0,     0,
       0,     0,     0,   457,   431,     0,     0,     0,   432,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   428,   961,   454,   455,   456,     0,
     433,   429,     0,     0,     0,     0,     0,   457,     0,     0,
       0,     0,     0,     0,   430,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   428,
       0,   454,   455,   456,     0,     0,   429,     0,     0,     0,
       0,     0,   457,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   431,     0,     0,     0,   432,     0,     0,   428,
       0,     0,     0,     0,     0,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,   432,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,     0,   433,     0,   428,  1003,   431,     0,     0,
     457,   432,   429,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,   430,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,  1030,   433,   454,   455,   456,     0,     0,   428,
    1042,  1004,     0,     0,     0,   457,   429,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   430,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   431,   454,   455,   456,   432,     0,     0,
       0,     0,     0,     0,   428,   457,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,  1077,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,   432,     0,     0,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,     0,
     454,   455,   456,   433,     0,     0,     0,  1070,     0,     0,
       0,   457,   431,     0,     0,     0,   432,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   428,     0,   454,   455,   456,     0,   433,   429,
       0,     0,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,   430,  1080,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   428,     0,   454,
     455,   456,     0,     0,   429,     0,     0,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,   432,     0,     0,   428,  1142,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,   432,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
       0,   433,     0,   428,     0,   431,     0,     0,   457,   432,
     429,     0,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,   430,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   433,   454,   455,   456,     0,     0,     0,     0,     0,
       0,  1083,     0,   457,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   431,   454,   455,   456,   432,     0,     0,   428,     0,
       0,     0,     0,   457,  1175,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,     0,   454,   455,
     456,     0,     0,     0,   428,  1213,   431,     0,     0,   457,
     432,   429,     0,     0,     0,     0,     0,     0,     0,  1178,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,   428,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,   430,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,   431,   454,   455,   456,   432,     0,     0,   428,
       0,     0,     0,  1230,   457,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,   431,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,   433,   454,
     455,   456,     0,     0,     0,     0,     0,   431,     0,     0,
     457,   432,     0,     0,   434,   435,     0,   436,   437,   438,
    1269,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   428,     0,   454,
     455,   456,     0,   433,   429,     0,     0,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,   430,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   428,     0,   454,   455,   456,     0,     0,   429,
       0,     0,     0,     0,     0,   457,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,   428,  1270,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,   432,     0,     0,   434,   435,     0,
     436,   437,   438,  1271,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,     0,   454,   455,   456,     0,   433,     0,     0,     0,
       0,     0,     0,   457,     0,   431,     0,     0,     0,   432,
       0,     0,   434,   435,     0,   436,   437,   438,  1272,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   428,     0,   454,   455,   456,
       0,   433,   429,     0,     0,     0,     0,     0,   457,     0,
       0,     0,     0,     0,     0,   430,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     428,     0,   454,   455,   456,     0,     0,   429,     0,     0,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,   432,     0,     0,
     428,     0,     0,     0,     0,     0,  1273,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,   432,     0,     0,   434,   435,     0,   436,   437,
     438,  1274,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,     0,
     454,   455,   456,     0,   433,     0,   428,     0,   431,     0,
       0,   457,   432,   429,     0,     0,     0,     0,     0,     0,
     434,   435,     0,   436,   437,   438,   430,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,   433,   454,   455,   456,     0,     0,
       0,     0,     0,     0,     0,     0,   457,     0,     0,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,   956,   454,   455,   456,   432,     0,
       0,     0,     0,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   497,
       0,     0,     0,    23,    24,     0,     0,     0,  1050,     0,
     433,     0,     0,   498,     0,    32,   499,     0,     0,     0,
       0,    38,     0,     0,     0,     0,   434,   435,    43,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,    59,     0,    61,     0,    63,     0,
    1051,     0,   457,  1052,     0,     0,   500,   497,    70,     0,
       0,    23,    24,     0,     0,     0,  1050,     0,     0,     0,
       0,   498,     0,    32,   499,     0,     0,    86,     0,    38,
      88,     0,  1053,     0,    91,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    59,     0,    61,   430,    63,     0,  1051,     0,
       0,  1052,     0,     0,   500,     0,    70,   497,     0,     0,
       0,    23,    24,   106,     0,     0,  1050,     0,     0,  1054,
       0,   498,     0,    32,   499,    86,     0,     0,    88,    38,
    1053,     0,    91,     0,     0,     0,    43,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,   432,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,  1051,     0,
       0,  1052,     0,     0,   500,     0,    70,     0,     0,     0,
       0,   106,     0,     0,     0,     0,     0,  1153,     0,   433,
       0,     0,     0,     0,     0,    86,     0,     0,    88,     0,
    1053,     0,    91,   431,     0,   434,   435,   432,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
       0,   447,   448,   449,   450,   451,   452,   453,     0,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,   433,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   106,     0,     0,     0,   434,   435,  1214,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
       0,   447,   448,   449,   450,   451,   397,   453,   237,     0,
     454,   455,   456,     0,     0,     0,     0,     0,     0,     0,
       0,   457,   238,   239,     0,   240,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,   398,     0,     0,   242,
       0,     0,     0,     0,     0,   243,     0,     0,     0,     0,
       0,   244,     0,     0,     0,   245,     0,     0,   246,     0,
       0,     0,     0,     0,     0,     0,     0,   237,   247,     0,
       0,     0,     0,     0,     0,   248,   249,     0,     0,     0,
       0,   238,   239,   250,   240,     0,     0,     0,     0,   241,
       0,    24,     0,   251,     0,     0,     0,     0,   242,     0,
       0,     0,   252,   253,   243,   254,     0,   255,     0,   256,
     244,     0,   257,     0,   245,     0,   258,   246,     0,   259,
     399,     0,     0,   260,     0,     0,     0,   247,     0,     0,
       0,     0,     0,     0,   248,   249,   237,     0,     0,     0,
       0,     0,   250,    61,     0,     0,     0,     0,     0,     0,
     238,   239,   251,   240,     0,    70,     0,     0,   241,     0,
       0,   252,   253,     0,   254,     0,   255,   242,   256,     0,
       0,   257,     0,   243,     0,   258,     0,     0,   259,   244,
       0,    91,   260,   245,     0,     0,   246,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   247,     0,     0,     0,
       0,     0,     0,   248,   249,     0,     0,     0,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,     0,     0,   237,     0,     0,     0,     0,     0,
     252,   253,     0,   254,     0,   255,     0,   256,   238,   239,
     257,   240,     0,     0,   258,   569,   241,   259,     0,     0,
       0,   260,   398,     0,     0,   242,     0,     0,     0,     0,
       0,   243,     0,     0,     0,     0,     0,   244,     0,     0,
       0,   245,     0,     0,   246,     0,     0,     0,     0,     0,
       0,     0,     0,  -302,   247,     0,     0,     0,     0,     0,
       0,   248,   249,     0,     0,     0,     0,  -302,  -302,   250,
    -302,     0,     0,     0,     0,  -302,     0,     0,     0,   251,
       0,     0,     0,     0,  -302,     0,     0,     0,   252,   253,
    -302,   254,     0,   255,     0,   256,  -302,     0,   257,     0,
    -302,     0,   258,  -302,     0,   259,     0,     0,     0,   260,
       0,     0,   237,  -302,     0,     0,     0,     0,     0,     0,
    -302,  -302,     0,     0,     0,     0,   238,   239,  -302,   240,
       0,     0,     0,     0,   241,     0,     0,     0,  -302,     0,
       0,     0,     0,   242,     0,     0,     0,  -302,  -302,   243,
    -302,     0,  -302,     0,  -302,   244,     0,  -302,     0,   245,
       0,  -302,   246,     0,  -302,     0,     0,     0,  -302,     0,
       0,  -303,   247,     0,     0,     0,     0,     0,     0,   248,
     249,     0,     0,     0,     0,  -303,  -303,   250,  -303,     0,
       0,     0,     0,  -303,     0,     0,     0,   251,     0,     0,
       0,     0,  -303,     0,     0,     0,   252,   253,  -303,   254,
       0,   255,     0,   256,  -303,     0,   257,     0,  -303,     0,
     258,  -303,     0,   259,     0,     0,     0,   260,     0,     0,
       0,  -303,     0,     0,     0,     0,     0,     0,  -303,  -303,
       0,     0,     0,     0,     0,     0,  -303,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -303,     0,     0,     0,
       0,     0,     0,     0,     0,  -303,  -303,     0,  -303,     0,
    -303,     0,  -303,     0,     0,  -303,     0,     0,     0,  -303,
       0,     0,  -303,     0,     0,     0,  -303
};

static const yytype_int16 yycheck[] =
{
      13,   168,   578,    67,    17,    20,     2,    67,    21,   485,
     622,    27,    67,    41,    27,   420,   694,   786,   696,   648,
     234,   573,    35,    36,    37,   137,    39,   645,   390,   184,
     832,   582,   870,   826,   635,   894,   637,    50,   865,    52,
      53,   270,   271,   272,   280,    58,   888,    60,   406,    62,
      48,     1,   538,   227,    67,    71,    48,    56,    71,    72,
      73,    74,    75,    76,     3,    56,  1005,    80,   304,    82,
      48,     1,     1,    35,    36,    37,    89,   548,    33,     1,
      93,    94,    93,    96,    93,    98,    33,   122,   101,   102,
      72,   104,    33,    33,   110,   108,   109,   110,   111,     3,
      33,   115,    61,   236,  1171,   122,   125,   127,   344,   904,
     148,   148,    94,     3,    33,     1,   148,   148,   148,     0,
       3,   276,   135,    48,   137,    75,    48,   109,   166,   166,
     122,    61,    37,   153,   148,   155,   156,    20,   158,   159,
     354,   176,   172,   174,   176,   374,   375,   280,   110,   142,
     148,    34,   171,  1220,    93,   166,   148,   150,   172,   176,
     171,   120,   171,    93,   177,   104,    91,   122,    73,   119,
     148,   304,   543,   942,   173,    61,   106,   648,   176,    22,
      63,   122,   173,    27,   176,  1124,  1045,   200,   201,   202,
     120,   207,   205,   206,   207,   813,  1055,   999,   127,   570,
     104,   148,  1044,  1005,    48,   127,   234,    93,    52,   567,
     225,   344,  1007,  1040,   104,  1008,   171,   147,   147,   148,
     106,   104,   172,   236,   171,   147,   148,   110,   779,   148,
     171,   171,    76,    48,   120,    79,   166,    81,   171,   475,
     476,   171,    75,   103,   166,   174,    89,    91,    27,   148,
      47,    47,   171,   120,    18,   127,   166,   869,    84,    85,
      47,   147,   172,   625,   148,   420,   110,   280,   148,    66,
     113,   757,   305,   306,    32,   174,    91,   389,   830,    66,
     152,   148,   496,    27,  1086,   171,   119,   148,    85,    85,
     174,   304,   172,  1152,   895,   166,   897,     2,    85,  1158,
      79,  1160,   315,   316,    56,   122,  1144,   440,  1146,   166,
     786,   171,    91,   174,    78,   172,   148,   330,   331,   116,
     116,    26,  1124,   148,    29,   492,   354,   148,  1097,   116,
     174,   344,    90,    97,    54,    79,    94,   955,   117,   172,
     172,   405,   475,   476,   166,   148,   148,    91,   148,   174,
     114,   173,    83,   174,    74,    86,   369,  1216,    56,  1218,
     373,     3,   148,   166,    84,   148,   110,    56,    56,   382,
     366,   384,   174,   587,   174,    80,   148,  1006,   394,    84,
      85,   394,   556,   148,   142,   148,   148,   148,   174,   147,
     166,   174,   150,  1071,  1072,    27,   154,   173,   173,   175,
     175,   148,   174,   108,   110,   538,   892,    91,   113,   174,
      90,   174,   174,   174,    94,   428,   429,   430,   431,   432,
     433,  1033,   435,   436,   437,   438,   439,   174,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,     8,    90,    79,   173,   582,
      94,   464,   465,   939,   628,   148,   148,   470,   148,    91,
     473,   474,   475,   476,   477,   478,   942,   148,   496,   148,
     432,    74,   485,   166,   166,   148,   166,   490,   110,    11,
      56,   494,   464,   465,   148,   166,    56,   166,   470,    56,
     503,   473,   474,   166,    51,   511,   173,    54,   511,     8,
     513,   514,   166,   148,   568,   148,   519,   520,   568,   101,
     915,   148,   525,   568,   219,   477,   478,    74,    56,   532,
      77,   166,   535,   166,   173,   538,    90,    84,   490,   166,
      94,  1143,    90,  1145,   142,   548,    94,   550,   173,   147,
     175,   554,   150,    90,   557,   558,   154,    94,    56,    27,
      27,    56,   514,   110,   173,   568,   175,    56,   520,   587,
     573,   790,   791,   525,    90,   794,   795,    56,    94,   582,
      48,    48,   173,    56,    52,    52,    56,   173,   283,   175,
      56,     8,   595,   288,   171,   598,   173,   600,   173,   173,
     175,   175,   605,   606,   607,  1171,   609,   610,    76,    76,
      56,    79,    79,    81,    81,   173,   171,   175,    56,   622,
     623,  1165,   147,    91,    91,   598,   598,   600,   600,   615,
     173,  1097,   851,   852,   757,   638,   166,   166,    27,   165,
      56,   173,   110,   110,    22,   648,   166,   166,    26,    27,
     171,   166,   166,   605,  1220,   166,   779,   166,    36,    48,
      38,    39,   166,    52,   121,   171,    44,    85,   173,   147,
     173,   366,   173,    51,   267,   268,   269,   270,   271,   272,
    1224,  1225,  1226,  1227,  1228,  1229,   381,    76,   691,   173,
      79,   174,    81,   148,   166,   698,   173,   166,   291,    77,
     293,    79,    91,     8,     2,    83,   174,   300,    86,    66,
     171,    89,   172,    91,     8,     8,   127,    91,    91,   127,
     150,   110,    69,   173,   127,   420,   174,   148,    26,    48,
     173,    29,   110,   890,   127,   113,   127,    66,    35,   117,
      35,   864,   865,   174,   127,   174,   147,   340,   174,   752,
     753,   754,   755,   756,   757,   758,   759,    21,   171,   173,
     148,   176,   355,   356,   176,   770,   771,   106,   150,   892,
     915,   150,     2,   150,   777,   150,   779,   780,   150,   782,
     150,   150,    80,   786,   150,   788,    84,    85,   166,    32,
     150,   150,   150,   488,   797,   798,    26,   150,   150,    29,
     142,   753,   150,   755,   166,   777,   758,   174,   176,   173,
     108,   166,   166,   816,    35,   113,   939,   166,   513,    35,
     166,   966,   166,   147,   519,   148,   166,   830,   973,   172,
     115,   834,    33,   174,   171,   530,   172,   532,   176,   174,
     894,    33,   176,   173,   816,    35,   166,    90,   902,   166,
      80,    94,   173,   166,    84,    85,   147,   911,   176,   862,
     176,     8,     8,   866,     2,   868,   869,   870,   148,   872,
     174,   874,    35,   174,   877,   878,   166,   148,   108,   166,
     166,   165,    32,   113,   165,   174,   174,   279,    26,   892,
     381,    29,  1118,  1119,    25,   862,  1055,  1055,   491,   142,
      93,   995,   351,  1020,   147,   554,   149,   150,   603,   900,
     568,   154,   607,   915,   901,   911,   392,   902,   161,   565,
     615,   219,   318,  1220,  1064,   518,   169,   930,  1230,   522,
     933,   524,  1223,   889,   527,   135,   939,   700,   551,   942,
      90,    -1,    80,   812,    94,    -1,    84,    85,    -1,    -1,
      -1,    -1,    -1,   956,   957,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   967,    -1,   969,    -1,    -1,    -1,
     108,   974,    -1,    -1,   977,   113,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   283,   989,    -1,    -1,   219,
     288,  1045,   142,   143,    -1,  1118,  1119,   147,    -1,   149,
     150,    -1,    -1,  1006,   154,   957,    -1,  1010,    -1,    -1,
    1016,   161,    -1,  1016,    -1,    -1,    -1,   167,   168,   169,
      -1,    -1,  1025,    -1,  1027,    -1,    -1,  1030,    -1,    -1,
    1033,    -1,    -1,    -1,    -1,    -1,    -1,  1040,    -1,  1042,
      -1,  1186,  1187,    -1,    -1,  1190,  1191,    -1,    -1,    -1,
      -1,    -1,    -1,   283,    -1,    -1,    -1,  1202,   288,  1204,
      -1,    -1,    -1,  1066,    -1,    -1,    -1,    -1,   366,    -1,
      -1,    -1,   767,  1076,  1077,    -1,  1079,  1080,    -1,  1082,
    1083,   219,    -1,   381,    -1,    -1,    -1,    -1,    -1,  1092,
    1093,    -1,  1095,    -1,  1097,    -1,  1099,  1100,  1152,    -1,
    1103,    -1,    -1,    -1,    -1,   800,    -1,    -1,   803,    -1,
      -1,    -1,    -1,    -1,    -1,  1118,  1119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     2,    -1,  1130,    -1,  1132,
      -1,    -1,    -1,  1136,    -1,    -1,   366,    -1,    -1,  1142,
    1143,  1144,  1145,  1146,    -1,   283,    -1,    -1,    -1,    26,
     288,   381,    29,    -1,    -1,   850,    -1,    -1,    -1,    -1,
      -1,   856,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1175,  1176,  1177,  1178,  1179,  1180,  1181,  1182,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     488,   784,   785,    -1,    -1,    -1,    -1,   790,   791,    -1,
      -1,   794,   795,    80,    -1,    -1,    -1,    84,    85,    -1,
    1213,    -1,    -1,    -1,    -1,   513,    -1,    -1,    -1,  1222,
      -1,   519,    -1,    32,    -1,    -1,    -1,    -1,   366,    -1,
      -1,   108,   530,    -1,   532,    -1,   113,    -1,    -1,  1242,
    1243,  1244,  1245,   381,    -1,    -1,    -1,  1250,  1251,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   488,    -1,
      -1,   956,   957,    -1,    -1,    -1,  1269,  1270,  1271,  1272,
    1273,  1274,   967,    -1,   969,    -1,    -1,    -1,    -1,   974,
      -1,    90,   977,   513,    -1,    94,    -1,    -1,    -1,   519,
      -1,   986,    -1,   988,    -1,    -1,   991,    -1,    -1,    -1,
     530,    -1,   532,    -1,    -1,   603,    -1,    -1,    -1,   607,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   615,    -1,    -1,
    1015,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,
    1025,    -1,  1027,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   219,    -1,    -1,   154,    -1,    -1,    -1,    -1,
     488,    26,   161,    -1,    29,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,   958,   959,    -1,    -1,    -1,
      -1,   964,    -1,   603,    -1,   513,    -1,   607,    -1,    -1,
      -1,   519,    -1,   976,    -1,   615,   979,    -1,   981,    -1,
     983,    -1,   530,    -1,   532,    -1,    -1,  1092,    -1,     3,
      -1,    -1,    -1,    -1,  1099,    80,   283,    -1,    -1,    84,
      85,   288,    -1,    17,    18,    -1,    20,  1112,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,  1126,    -1,   108,    -1,    -1,    40,    -1,   113,    -1,
    1135,    -1,    46,    -1,  1139,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,    63,
      -1,    32,    -1,    -1,    -1,   603,    70,    71,    -1,   607,
      -1,    -1,    -1,    -1,    78,    -1,    -1,   615,    -1,    26,
      -1,    -1,    29,    -1,    88,    -1,    -1,    -1,    -1,   366,
      -1,    -1,    -1,    97,    98,    -1,   100,  1192,   102,    -1,
     104,    -1,   800,   107,   381,   803,    -1,   111,   112,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    80,    -1,  1128,    -1,    84,    85,    -1,
      -1,    -1,    -1,    -1,   219,    -1,    -1,  1242,  1243,  1244,
    1245,    -1,   850,    -1,    -1,  1250,  1251,    -1,   856,    -1,
      -1,   108,    -1,    -1,    -1,    -1,   113,    -1,    -1,   173,
     800,   142,   143,   803,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,    -1,   158,   159,   160,
     161,   162,    -1,   164,    -1,    -1,   167,   168,   169,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,   178,   283,    -1,
      -1,   488,    -1,   288,    17,    18,    -1,    20,    -1,    -1,
     850,    -1,    25,    -1,    -1,    -1,   856,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,   513,    40,    -1,    -1,
      -1,    -1,   519,    46,    -1,    32,    -1,    50,    -1,    -1,
      53,    -1,    -1,   530,    -1,   532,    -1,    -1,    -1,    -1,
      63,    -1,   800,    -1,    -1,   803,    -1,    70,    71,   967,
      -1,   969,   219,    -1,    -1,    78,   974,    -1,    -1,   977,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,   986,    -1,
     988,   366,    -1,   991,    97,    98,    -1,   100,    -1,   102,
      -1,   104,    -1,    90,   107,    -1,   381,    94,   111,   112,
      -1,   114,   850,    -1,    -1,   118,    -1,  1015,   856,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   603,  1025,    -1,  1027,
     607,    -1,    -1,    -1,    -1,    -1,   283,   967,   615,   969,
      -1,   288,    -1,    -1,   974,    -1,    -1,   977,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   986,    -1,   988,    -1,
     147,   991,   149,   150,    -1,    -1,    -1,   154,    -1,    -1,
     173,    -1,    -1,    -1,   161,    -1,    -1,    -1,    -1,    -1,
     167,   168,   169,    -1,    -1,  1015,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     2,  1092,  1025,    -1,  1027,    -1,    -1,
      -1,  1099,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   488,  1112,    -1,    -1,    26,    -1,   366,
      29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1126,   967,
      -1,   969,    -1,    -1,   381,    -1,   974,  1135,   513,   977,
      -1,  1139,    -1,    -1,   519,    -1,    -1,    -1,   986,    -1,
     988,    -1,    -1,   991,    -1,   530,    -1,   532,    -1,    -1,
      90,    -1,  1092,    -1,    94,    -1,    -1,    -1,    -1,  1099,
      -1,    80,    -1,    -1,    -1,    84,    85,  1015,    -1,    -1,
      -1,    -1,  1112,    -1,    -1,    -1,    -1,  1025,    -1,  1027,
      -1,    -1,    -1,    -1,  1192,    -1,  1126,    -1,    -1,   108,
      -1,    -1,    -1,    -1,   113,  1135,    -1,    -1,    -1,  1139,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   800,   154,    -1,   803,    -1,   603,    -1,
     160,   161,   607,    -1,   164,    -1,    -1,   167,   168,   169,
     615,   488,    -1,    -1,  1242,  1243,  1244,  1245,   178,    -1,
      -1,    -1,  1250,  1251,  1092,    -1,    -1,    -1,    -1,    -1,
      -1,  1099,  1192,    -1,    -1,    -1,   513,    -1,    -1,    -1,
      -1,    -1,   519,   850,  1112,    -1,    -1,    -1,    -1,   856,
       2,    -1,    -1,   530,    -1,   532,    -1,    -1,  1126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1135,    -1,    -1,
      -1,  1139,    -1,    -1,    26,    -1,    -1,    29,    -1,    72,
     219,    -1,  1242,  1243,  1244,  1245,    -1,    -1,    -1,    -1,
    1250,  1251,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1192,    -1,   603,    -1,    80,    -1,
     607,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   615,    -1,
      -1,    -1,    -1,    32,   283,    -1,    -1,    -1,    -1,   288,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,    -1,
     967,   113,   969,    -1,    -1,    -1,    -1,   974,    -1,    90,
     977,    -1,    -1,    94,  1242,  1243,  1244,  1245,    -1,   986,
      -1,   988,  1250,  1251,   991,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     2,    -1,    -1,   800,    -1,    -1,   803,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,  1015,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    -1,  1025,    29,
    1027,   142,   143,    -1,    -1,   146,   147,   366,   149,   150,
      -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,    -1,    -1,
     161,    -1,   381,   236,    -1,   850,   167,   168,   169,    -1,
      -1,   856,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,    -1,    -1,    -1,   154,    -1,   219,    -1,    -1,
      80,   160,   161,    -1,     3,   164,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,  1092,    -1,   280,    17,    -1,
      -1,    20,  1099,    -1,    -1,    -1,    25,    -1,   108,    -1,
      -1,    -1,    -1,   113,    -1,  1112,    -1,    36,    -1,    -1,
      -1,   304,    -1,    -1,    -1,    -1,    -1,    46,    -1,  1126,
      -1,    -1,    -1,   800,    53,    -1,   803,    -1,  1135,    -1,
      -1,   283,  1139,    -1,    63,    -1,   288,    -1,    -1,    22,
      -1,    -1,    71,    26,    27,    -1,    -1,    -1,    -1,   488,
      -1,   344,    -1,    36,    -1,    38,    39,    -1,    -1,    88,
      -1,    44,   967,    -1,   969,    -1,    -1,    -1,    51,   974,
      -1,   100,   977,   850,   513,   104,    -1,    -1,    -1,   856,
     519,   986,   111,   988,    -1,  1192,   991,    -1,    -1,   118,
      -1,   530,    -1,   532,    77,    -1,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    91,   219,
    1015,    -1,    -1,    -1,   366,    -1,    -1,    -1,    -1,    -1,
    1025,    -1,  1027,    -1,    -1,    -1,    -1,   110,    -1,   381,
     113,    -1,    -1,    -1,   117,  1242,  1243,  1244,  1245,    -1,
      -1,    -1,    -1,  1250,  1251,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
      -1,    -1,    -1,    -1,   603,    -1,    -1,    -1,   607,    -1,
      -1,   464,   465,   283,    -1,    -1,   615,   470,   288,    -1,
     473,   474,   475,   476,    -1,    -1,    -1,  1092,   170,    -1,
     967,   173,   969,   175,  1099,    -1,    -1,   974,    -1,    -1,
     977,    -1,    -1,    -1,    -1,    -1,    -1,  1112,    -1,   986,
      -1,   988,    -1,    -1,   991,    -1,    -1,    -1,    -1,    -1,
      50,  1126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1135,    -1,    -1,    -1,  1139,    22,   488,    -1,  1015,    26,
      27,    -1,    -1,    -1,    74,   538,    -1,    -1,  1025,    36,
    1027,    38,    39,    -1,    84,    85,   366,    44,    -1,    -1,
      -1,   513,    -1,    -1,    51,    -1,    -1,   519,    -1,    -1,
      -1,   381,    -1,    -1,    -1,    -1,    -1,    -1,   530,    66,
     532,    -1,    -1,    -1,    -1,    -1,    -1,  1192,    -1,   582,
      77,    -1,    79,    -1,    -1,    -1,    83,    -1,    85,    86,
      -1,    -1,    89,    -1,    91,   598,    -1,   600,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1092,    -1,    -1,    -1,    -1,
      -1,    -1,  1099,   110,    -1,    -1,   113,    -1,    -1,    -1,
     117,    -1,    -1,    -1,    -1,  1112,    -1,  1242,  1243,  1244,
    1245,    -1,    -1,    -1,    -1,  1250,  1251,    -1,    -1,  1126,
      -1,   603,    -1,    -1,    -1,   607,    -1,    -1,  1135,    -1,
      -1,   800,  1139,   615,   803,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   488,   166,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   224,    -1,    45,    46,    47,    -1,
      49,    -1,    -1,   513,    -1,    -1,    -1,    -1,    -1,   519,
      -1,   850,    -1,    -1,    -1,  1192,    -1,   856,    -1,    -1,
     530,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   267,   268,   269,
     270,   271,   272,    -1,    -1,   275,   276,    -1,    -1,    -1,
      -1,    -1,   282,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,   291,    -1,   293,   757,  1242,  1243,  1244,  1245,    -1,
     300,    -1,    -1,  1250,  1251,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   777,    -1,   779,    -1,    -1,    -1,
      -1,    -1,    -1,   603,    -1,    -1,    -1,   607,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   615,    -1,    -1,    -1,    -1,
     340,    -1,    -1,    90,    -1,    -1,    -1,    94,   167,   168,
      -1,    -1,    -1,   816,    -1,   355,   356,    -1,   967,    -1,
     969,    -1,    -1,    -1,    -1,   974,    -1,    -1,   977,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   986,   800,   988,
      -1,   803,   991,    -1,   203,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,  1015,   154,   155,   156,
      -1,   158,   159,   160,   161,    -1,  1025,   164,  1027,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,   850,   892,
      -1,   178,    -1,    -1,   856,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   265,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   273,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   939,    -1,    -1,    -1,
      -1,    -1,    -1,  1092,    -1,    -1,    -1,    -1,    -1,    -1,
    1099,   491,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1112,    -1,   324,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1126,   518,    -1,
     800,    -1,   522,   803,   524,    -1,  1135,   527,    -1,    -1,
    1139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   967,    -1,   969,    -1,    -1,
      -1,    -1,   974,    -1,    -1,   977,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,   986,    -1,   988,    -1,    -1,   991,
     850,    -1,    -1,    -1,   393,    -1,   856,    17,    18,    -1,
      20,    -1,    -1,  1192,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1015,    34,    -1,    -1,    -1,    -1,    -1,
      40,   420,    -1,  1025,    -1,  1027,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,  1242,  1243,  1244,  1245,    -1,    78,    -1,
      -1,  1250,  1251,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,  1118,  1119,    97,    98,    -1,
     100,    -1,   102,    -1,   104,   484,    -1,   107,    -1,    -1,
    1092,   111,    -1,   492,   114,    -1,    -1,  1099,   118,   498,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   967,    -1,   969,
    1112,    -1,    -1,    -1,   974,    -1,    -1,   977,    -1,    -1,
      -1,    -1,    -1,    -1,  1126,    22,   986,    -1,   988,    26,
      27,   991,    -1,  1135,    31,    -1,    -1,  1139,    -1,    36,
     539,    38,    39,    -1,   543,    -1,    -1,    44,    -1,    -1,
      -1,   171,   551,   173,    51,  1015,    -1,   556,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1025,   565,  1027,    -1,    -1,
      -1,   570,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,    86,
    1192,    -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   784,   785,    -1,    -1,    -1,    -1,
     790,   791,    -1,   110,   794,   795,   113,    -1,   115,    -1,
     117,    -1,   621,    -1,   804,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,     1,    -1,    -1,    -1,    -1,    -1,    -1,
    1242,  1243,  1244,  1245,    -1,    32,    -1,    -1,  1250,  1251,
      -1,    -1,  1112,    -1,    22,    -1,    -1,    -1,    26,    27,
     840,    48,    -1,    31,    -1,    -1,  1126,    -1,    36,   166,
      38,    39,    -1,    -1,    -1,  1135,    44,    -1,    -1,  1139,
      -1,    -1,    -1,    51,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   700,    -1,    90,    -1,    -1,    74,    94,    -1,    77,
      -1,    79,    -1,    81,    -1,    83,    84,    -1,    86,    -1,
      -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1192,    -1,    -1,   122,    -1,    -1,    -1,   126,
      -1,    -1,   110,    -1,    -1,   113,    -1,   115,    -1,   117,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   958,   959,
     167,   168,   169,    -1,   964,    -1,   966,    -1,    -1,    -1,
      -1,   178,    -1,   973,    -1,    -1,   976,    -1,   166,   979,
      -1,   981,    -1,   983,   172,    -1,   805,    -1,    -1,    -1,
      -1,    -1,    -1,   812,    -1,    -1,   996,    -1,    -1,    -1,
      -1,    -1,    -1,   822,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    18,    -1,
      -1,   840,    22,  1023,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    41,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    65,    -1,    67,    68,    -1,
     889,   890,    -1,    -1,    -1,    -1,    76,    -1,    78,    79,
      80,    81,    82,    -1,    -1,    -1,    -1,    87,    -1,    89,
      -1,    91,    -1,    -1,    -1,    -1,   915,    97,    -1,    99,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,  1128,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,   174,   175,   995,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1186,  1187,    -1,    -1,
    1190,  1191,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1200,  1020,  1202,    -1,  1204,     0,     1,    -1,     3,     4,
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
     165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,     1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    15,    16,    17,    18,
      19,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,    -1,   104,   105,    -1,   107,   108,
     109,   110,   111,    -1,   113,   114,   115,   116,   117,   118,
      -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,   171,   172,   173,    -1,   175,    -1,     1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     173,    -1,   175,    -1,     1,   178,     3,     4,     5,     6,
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
     171,   172,   173,    -1,   175,    -1,     1,   178,     3,     4,
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
     175,    -1,     1,   178,     3,     4,     5,     6,     7,     8,
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
     169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     173,    -1,   175,    -1,     1,   178,     3,     4,     5,     6,
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
       1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
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
     175,    -1,     1,   178,     3,     4,     5,     6,     7,     8,
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
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     173,    -1,   175,    -1,     1,   178,     3,     4,     5,     6,
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
       1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
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
     175,    -1,     1,   178,     3,     4,     5,     6,     7,     8,
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
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,    -1,   178,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,
      17,    18,    19,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    99,   100,   101,   102,    -1,   104,    -1,    -1,
     107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,   144,   145,   146,
     147,    -1,   149,    -1,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,    -1,
     167,   168,   169,   170,   171,    -1,   173,    -1,   175,    -1,
      -1,   178,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    -1,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    43,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    62,    63,    64,    65,    -1,    67,    68,    -1,    70,
      71,    72,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,    -1,   107,    -1,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
     121,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,     4,
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
      -1,    -1,   151,   152,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    48,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,   117,   118,    -1,    -1,    -1,    -1,
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
     107,   108,   109,    -1,   111,   112,    -1,   114,    -1,    -1,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
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
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,
     101,   102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,
     111,   112,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,   174,   175,    -1,    -1,   178,     3,     4,
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
      69,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,   108,
     109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,    -1,   178,     3,     4,     5,     6,
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
     101,   102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,
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
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,   124,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,
     145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,   176,    -1,   178,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    90,    -1,    -1,    -1,    94,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,   124,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,    -1,    -1,   178,     3,     4,     5,     6,
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
     111,    -1,   113,   114,    -1,    -1,    -1,   118,    -1,    -1,
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
     175,   176,    -1,   178,     3,     4,     5,     6,     7,     8,
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
     169,    -1,   171,   172,   173,    -1,   175,    -1,    -1,   178,
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
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,   176,    -1,   178,     3,     4,     5,     6,
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
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,   176,
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
     111,   112,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
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
      -1,    -1,   107,    -1,    -1,    -1,   111,   112,    -1,   114,
      -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,
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
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
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
     169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,
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
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
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
     175,    -1,    -1,   178,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    18,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    41,    42,    -1,    -1,    45,    -1,    -1,    48,    -1,
      -1,    -1,    52,    -1,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    -1,    -1,    65,    -1,    67,    68,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    79,
      80,    81,    82,    -1,    -1,    -1,    -1,    87,    -1,    89,
      -1,    91,    -1,    -1,    -1,    -1,    -1,    97,    -1,    99,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    12,    -1,
      -1,   151,    -1,    17,    18,    19,    20,    -1,    -1,    -1,
     160,    25,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      34,   171,    36,   173,    -1,   175,    40,    -1,   178,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    -1,   100,    -1,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,    -1,
      -1,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,    -1,   149,    -1,    -1,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,    -1,    -1,   167,   168,   169,   170,    -1,    -1,   173,
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
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
      -1,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    -1,   175,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,   144,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,   165,    -1,    25,    -1,    -1,
      -1,   171,    -1,   173,    -1,   175,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,     3,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    17,    18,   107,
      20,    -1,    -1,   111,    -1,    25,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,     3,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    12,    53,   142,    -1,   144,    -1,    -1,    19,
      20,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    34,    -1,    -1,   165,    78,    -1,
      -1,    -1,    -1,    -1,    -1,   173,    -1,   175,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    55,    -1,    97,    98,    -1,
     100,    -1,   102,    63,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,    -1,
     110,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,   132,   173,    -1,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,   149,
      -1,    -1,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,     3,   164,    -1,    -1,   167,   168,   169,
     170,   171,    12,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    97,    98,    -1,
     100,    -1,   102,    19,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    32,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   127,   128,   129,
     130,   131,   132,    -1,    -1,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,   149,
      -1,    -1,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,    -1,    -1,   167,   168,   169,
     170,    -1,    12,    -1,    90,    -1,    -1,    -1,    94,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,   127,   128,   129,   130,   131,   132,    -1,    -1,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      90,   167,   168,   169,    94,   171,    -1,    12,    -1,    -1,
      -1,    -1,   178,   103,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   171,    -1,    12,    -1,    90,    -1,    -1,   178,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,   127,   128,   129,   130,   131,   132,    -1,    -1,
     135,   136,   137,   138,    -1,   140,   141,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    90,   167,   168,   169,    94,   171,    -1,    12,    -1,
      -1,    -1,    -1,   178,   103,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,
     129,   130,   131,   132,    -1,    -1,   135,   136,   137,   138,
      -1,   140,   141,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,   171,    -1,    12,    -1,    90,    -1,    -1,   178,
      94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,   127,   128,   129,   130,   131,   132,    -1,
      -1,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    90,   167,   168,   169,    94,   171,    -1,    12,
      -1,    -1,    -1,    -1,   178,   103,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   171,    -1,    12,    -1,    90,    -1,    -1,
     178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,   132,
      -1,    -1,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    90,   167,   168,   169,    94,   171,    -1,
      12,    -1,    -1,    -1,    -1,   178,   103,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    12,    -1,    90,    -1,
      -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    90,   167,   168,   169,    94,   171,
      -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,   122,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,    90,
      -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    48,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
     171,    -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,   126,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      73,    -1,   167,   168,   169,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,   126,    -1,    -1,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    73,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    12,    -1,    90,
      -1,    -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
      -1,    -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,   122,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,
      90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,   171,    -1,    12,    -1,    -1,    -1,    -1,   178,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,    12,
      -1,    90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    90,   167,   168,
     169,    94,    -1,    -1,    12,    -1,    -1,    -1,    -1,   178,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      12,    -1,    90,    -1,    -1,   178,    94,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    32,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    90,   167,
     168,   169,    94,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,   103,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,   122,   167,   168,   169,   126,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    90,   145,   146,   147,
      94,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    12,    -1,   167,
     168,   169,    -1,    -1,    19,    -1,    -1,    -1,   176,    -1,
     178,    -1,   126,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    90,    -1,    -1,    -1,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    32,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    90,   167,   168,   169,    94,    -1,    -1,    12,    -1,
      -1,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   126,   167,   168,
     169,    -1,   171,    -1,    -1,    -1,    90,    -1,    -1,   178,
      94,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    12,   166,   167,   168,
     169,   125,   126,    19,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    12,    -1,   167,   168,   169,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,   103,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   126,    -1,    12,    -1,    90,
      -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    32,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   126,   167,   168,   169,    -1,
      -1,    12,    -1,   174,    -1,    -1,    -1,   178,    19,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    32,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    12,   178,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,   122,    -1,    35,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    90,    -1,    -1,    -1,    94,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    12,   166,   167,   168,   169,    -1,
     126,    19,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    12,
      -1,   167,   168,   169,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   126,    -1,    12,   174,    90,    -1,    -1,
     178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    32,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   125,   126,   167,   168,   169,    -1,    -1,    12,
      13,   174,    -1,    -1,    -1,   178,    19,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    90,   167,   168,   169,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    12,   178,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,   126,    -1,    -1,    -1,   174,    -1,    -1,
      -1,   178,    90,    -1,    -1,    -1,    94,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    12,    -1,   167,   168,   169,    -1,   126,    19,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    12,    -1,   167,
     168,   169,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   126,    -1,    12,    -1,    90,    -1,    -1,   178,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    32,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   126,   167,   168,   169,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,   178,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    90,   167,   168,   169,    94,    -1,    -1,    12,    -1,
      -1,    -1,    -1,   178,   103,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    12,    13,    90,    -1,    -1,   178,
      94,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    32,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    90,   167,   168,   169,    94,    -1,    -1,    12,
      -1,    -1,    -1,    61,   178,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,   126,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
     178,    94,    -1,    -1,   142,   143,    -1,   145,   146,   147,
     103,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    12,    -1,   167,
     168,   169,    -1,   126,    19,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    12,    -1,   167,   168,   169,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,   103,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,   142,   143,    -1,
     145,   146,   147,   103,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,   142,   143,    -1,   145,   146,   147,   103,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    12,    -1,   167,   168,   169,
      -1,   126,    19,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      12,    -1,   167,   168,   169,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   103,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,   142,   143,    -1,   145,   146,
     147,   103,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   126,    -1,    12,    -1,    90,    -1,
      -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    32,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,   126,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    90,   167,   168,   169,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,
     126,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,   142,   143,    51,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    77,    -1,    79,    -1,    81,    -1,
      83,    -1,   178,    86,    -1,    -1,    89,    22,    91,    -1,
      -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,   110,    -1,    44,
     113,    -1,   115,    -1,   117,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    32,    81,    -1,    83,    -1,
      -1,    86,    -1,    -1,    89,    -1,    91,    22,    -1,    -1,
      -1,    26,    27,   166,    -1,    -1,    31,    -1,    -1,   172,
      -1,    36,    -1,    38,    39,   110,    -1,    -1,   113,    44,
     115,    -1,   117,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,    -1,
      -1,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,    -1,    -1,   172,    -1,   126,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
     115,    -1,   117,    90,    -1,   142,   143,    94,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   166,    -1,    -1,    -1,   142,   143,   172,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
      -1,   158,   159,   160,   161,   162,     1,   164,     3,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    17,    18,    78,    20,    -1,    -1,    -1,    -1,    25,
      -1,    27,    -1,    88,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    97,    98,    40,   100,    -1,   102,    -1,   104,
      46,    -1,   107,    -1,    50,    -1,   111,    53,    -1,   114,
     115,    -1,    -1,   118,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,     3,    -1,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    88,    20,    -1,    91,    -1,    -1,    25,    -1,
      -1,    97,    98,    -1,   100,    -1,   102,    34,   104,    -1,
      -1,   107,    -1,    40,    -1,   111,    -1,    -1,   114,    46,
      -1,   117,   118,    50,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,   100,    -1,   102,    -1,   104,    17,    18,
     107,    20,    -1,    -1,   111,   112,    25,   114,    -1,    -1,
      -1,   118,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     3,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    17,    18,    78,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    97,    98,
      40,   100,    -1,   102,    -1,   104,    46,    -1,   107,    -1,
      50,    -1,   111,    53,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,     3,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    17,    18,    78,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    97,    98,    40,
     100,    -1,   102,    -1,   104,    46,    -1,   107,    -1,    50,
      -1,   111,    53,    -1,   114,    -1,    -1,    -1,   118,    -1,
      -1,     3,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    17,    18,    78,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    97,    98,    40,   100,
      -1,   102,    -1,   104,    46,    -1,   107,    -1,    50,    -1,
     111,    53,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,   118
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   183,   184,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    10,    14,    15,    16,    17,    18,    20,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    33,
      34,    36,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    49,    50,    51,    53,    56,    57,    58,    59,    60,
      62,    63,    64,    65,    67,    68,    70,    71,    72,    77,
      78,    79,    80,    81,    82,    83,    86,    87,    88,    89,
      91,    92,    93,    95,    96,    97,    98,    99,   100,   101,
     102,   104,   105,   107,   108,   109,   110,   111,   113,   114,
     115,   117,   118,   121,   123,   126,   142,   143,   144,   145,
     146,   151,   160,   163,   164,   165,   166,   169,   171,   173,
     175,   178,   185,   186,   187,   188,   189,   192,   193,   196,
     197,   198,   200,   201,   202,   207,   208,   211,   212,   216,
     218,   221,   228,   231,   232,   233,   234,   235,   236,   239,
     240,   242,   244,   247,   248,   249,   250,   251,   255,   256,
     261,   262,   263,   264,   267,   268,   279,   280,   282,   283,
     288,   292,   293,   295,   296,   318,   323,   324,   328,   329,
     350,   351,   352,   353,   354,   355,   356,   364,   365,   366,
     367,   368,   369,   371,   374,   375,   376,   377,   378,   379,
     380,   381,   383,   384,   385,   386,   387,   166,    22,    36,
      41,    42,    45,    56,    89,   102,   171,   175,   247,   280,
     350,   355,   365,   366,   371,   374,   376,   377,   122,   357,
     358,     3,   213,   371,   357,   371,   110,   330,    91,   213,
     187,   344,   371,     8,   191,   187,   173,     3,    17,    18,
      20,    25,    34,    40,    46,    50,    53,    63,    70,    71,
      78,    88,    97,    98,   100,   102,   104,   107,   111,   114,
     118,   214,   215,   217,    11,    79,   124,   241,   371,   241,
     371,   241,   371,    27,   117,   243,   371,    83,    86,   198,
     173,   215,   215,   215,   173,   215,   331,   332,    33,   202,
     220,   371,   259,   260,   371,   371,    18,    78,    97,   114,
     371,   371,   371,     8,   173,   230,   229,    27,    34,    48,
      50,    52,    76,    79,    91,    98,   102,   110,   186,   219,
     284,   285,   286,   309,   310,   311,   336,   341,   371,   344,
     108,   109,   166,   288,   289,   370,   371,   373,   371,   220,
     371,   371,   371,   101,   173,   187,   371,   371,   188,   202,
     188,   202,   371,     8,   195,   243,   371,   373,   371,   371,
     371,   371,   371,     1,   172,   185,   203,   344,   112,   152,
     345,   346,   373,   176,   241,   344,   371,   382,   371,    81,
     187,   171,    47,    85,   116,   199,    26,   329,   371,     8,
     263,   371,   372,    56,   147,   265,   215,     1,    31,   115,
     215,   269,   272,   275,   278,   173,   304,    54,    74,    84,
     313,    27,    79,    91,   110,   314,    27,    79,    91,   110,
     312,   215,   325,   326,   331,   165,   166,   371,    12,    19,
      32,    90,    94,   126,   142,   143,   145,   146,   147,   149,
     150,   151,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   167,   168,   169,   178,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   170,   300,   173,   175,    90,    94,   371,
     371,   371,   215,   344,    56,   173,   187,   166,   171,   202,
      48,   357,   328,   166,   148,   166,   190,    22,    36,    39,
      89,   192,   198,   121,   216,   347,   348,   349,   373,   171,
     215,   173,   220,    33,    48,   220,   122,   220,   360,    33,
      48,   220,   360,   220,   360,    48,   220,   360,   215,   215,
     103,   202,   103,   127,   202,   300,   199,   347,   173,   173,
     202,   187,   112,   173,   215,   333,   334,     1,   147,   340,
      48,   148,   187,   220,   148,   220,    13,   173,   173,   220,
     347,   231,   231,    48,    91,   310,   311,   174,   148,   112,
     173,   215,   308,   147,   166,   371,   371,   125,   290,   166,
     171,   220,   173,   347,   166,   252,   252,   194,   192,   220,
     220,   166,   172,   172,   185,   148,   172,   371,   148,   174,
     148,   174,   371,   176,   360,    48,   148,   176,   360,   125,
     148,   176,     8,     1,   172,   203,   209,   210,   371,   205,
     371,    66,    37,    73,   166,   263,   265,   110,   237,   293,
     215,   344,   171,   172,     8,   274,     8,   277,   127,   148,
     172,   174,   305,   307,   309,   314,    91,     1,   147,   338,
     339,    91,     1,     3,    12,    17,    19,    20,    25,    46,
      53,    55,    63,    71,    88,   100,   104,   111,   118,   142,
     143,   144,   145,   146,   147,   149,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   164,   167,   168,   169,
     170,   173,   215,   297,   298,   299,   300,   350,   127,   327,
     148,   166,   166,   371,   371,   371,   241,   371,   241,   371,
     371,   371,   371,   371,   371,   371,     3,    20,    34,    63,
     104,   110,   216,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
      69,   373,   373,   373,   373,   373,   347,   347,   241,   371,
     241,   371,    33,    48,    33,    48,   103,   173,    48,   176,
     215,    27,    48,    91,   117,   359,   362,   363,   371,   387,
     241,   371,   220,   331,   371,   192,   371,   127,   174,   148,
      48,   344,    45,   371,   241,   371,   173,   220,    45,   371,
     241,   371,   220,   220,   241,   371,   220,   127,   127,   187,
      35,   187,   371,    35,   371,    66,   174,   348,   215,   245,
     246,   334,   148,   174,   219,   371,   127,   335,   371,   332,
     259,   215,   328,   371,   371,   174,   308,   314,   286,   334,
     147,   342,   343,   341,   291,   201,     1,   257,   349,   174,
      21,   253,   192,   172,   174,   174,   345,   174,   345,   187,
     176,   241,   371,   176,   187,   371,   176,   371,   176,   371,
     172,   172,   148,   166,    13,   150,   148,   166,    13,    37,
      73,   215,   142,   143,   144,   145,   146,   160,   164,   169,
     204,   299,   300,   301,   371,   204,   206,   265,   171,   238,
     328,   166,   173,     1,   266,   273,   278,   276,   278,   371,
     270,   271,   148,   174,   340,     1,   219,   336,   337,   106,
     315,   173,   303,   371,   142,   150,   303,   303,   371,   326,
     173,   175,   166,   166,   166,   166,   166,   166,   174,   176,
      45,   241,   371,    45,   241,   371,   348,   241,   371,   173,
      48,    91,   148,   174,    17,    20,    25,    46,    53,    63,
      71,    88,   100,   111,   118,   350,    90,    90,   357,   357,
     166,   166,   373,   349,   371,   174,   371,    33,   220,    33,
     220,   361,   362,   371,    33,   220,   360,    33,   220,   360,
     220,   360,   220,   360,   371,   371,    35,   187,    35,    35,
     187,   103,   202,   215,   174,   148,   174,   174,   333,   340,
      69,   373,   215,   174,   174,   343,   147,   287,   174,   341,
     152,   302,   335,   371,   172,    75,   119,   172,   258,   174,
     173,   202,   215,   254,   187,   176,   360,   176,   360,   187,
     125,   209,   216,   171,   301,   371,   112,   371,   204,   206,
     148,   166,    13,   166,   171,   266,   325,   331,   348,   172,
      31,    83,    86,   115,   172,   186,   222,   225,   228,   278,
     278,   272,   275,   307,   315,    61,   120,   319,   305,   306,
     174,   298,   300,   174,   176,   371,    33,    33,   371,    33,
      33,   174,   176,   176,   348,   362,   340,   350,   350,   220,
     220,   220,   103,    45,   371,    45,   371,   148,   174,   103,
      45,   371,   220,    45,   371,   220,   220,   220,   187,   187,
     371,   187,    35,   166,   166,   246,   202,   335,   173,   173,
     302,   335,   336,   315,   343,   371,    33,   187,   344,   254,
     147,   202,    45,   187,   371,   176,    45,   187,   371,   176,
     371,   204,    13,    37,    73,    37,    73,   166,   166,   301,
     371,   371,   266,   172,   166,   166,   174,     8,   224,     8,
     227,   222,   225,   319,     3,    93,   104,   320,   321,   322,
     371,   294,   174,   303,   303,   103,    45,    45,   103,    45,
      45,    45,    45,   174,   335,   371,   371,   371,   362,   371,
     371,   371,    35,   187,   347,   347,   302,   335,   187,   220,
     174,   371,   371,   187,   371,   187,   172,   112,   371,   204,
     206,   204,   206,    13,   172,   166,   223,   228,   226,   228,
     281,   322,   120,   148,   127,   153,   155,   156,   158,   159,
      61,   166,   201,   221,   316,   317,   371,   371,   371,   371,
     371,   371,   103,   103,   103,   103,   187,   174,   174,   202,
     103,   103,   166,   166,   166,   166,   371,   228,   228,   317,
     371,   321,   322,   322,   322,   322,   322,   322,   320,   103,
     103,   103,   103,   103,   103,   371,   371,   371,   371,   165,
     165,   371,   371
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
     300,   300,   300,   300,   300,   301,   301,   302,   302,   303,
     303,   304,   304,   305,   305,   306,   306,   307,   308,   308,
     309,   309,   309,   309,   309,   309,   310,   310,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   312,   312,   312,
     312,   312,   312,   313,   313,   313,   314,   314,   314,   314,
     314,   314,   315,   315,   316,   316,   317,   317,   318,   319,
     319,   319,   319,   319,   320,   320,   321,   321,   321,   321,
     321,   321,   321,   322,   322,   323,   324,   324,   325,   325,
     326,   327,   327,   328,   328,   328,   328,   328,   330,   329,
     329,   331,   331,   332,   332,   333,   333,   333,   334,   334,
     334,   335,   335,   335,   336,   337,   337,   337,   338,   338,
     339,   339,   340,   340,   340,   340,   341,   341,   342,   343,
     343,   344,   344,   345,   345,   346,   346,   347,   347,   348,
     348,   349,   349,   350,   350,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   353,
     353,   353,   353,   353,   353,   353,   354,   355,   356,   356,
     356,   356,   356,   356,   356,   357,   357,   358,   359,   359,
     360,   361,   361,   362,   362,   362,   363,   363,   363,   363,
     363,   363,   364,   364,   364,   364,   364,   365,   365,   365,
     365,   365,   366,   367,   367,   367,   367,   367,   367,   368,
     369,   370,   370,   371,   371,   371,   371,   371,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     372,   372,   373,   373,   373,   374,   374,   374,   374,   375,
     375,   375,   375,   375,   376,   376,   376,   377,   377,   377,
     377,   377,   377,   378,   378,   378,   378,   379,   379,   380,
     380,   381,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   382,   382,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   384,
     384,   384,   384,   384,   384,   384,   385,   385,   385,   385,
     386,   386,   386,   386,   387,   387,   387,   387,   387,   387,
     387
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
       3,     1,     3,     1,     0,     8,     0,     1,     1,     1,
       0,     1,     1,     3,     1,     3,     1,     0,     2,     6,
       0,     3,     0,     4,     1,     0,     7,     4,     4,     6,
       6,     4,     2,     1,     3,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     0,
       3,     2,     3,     1,     3,     0,     1,     1,     1,     1,
       4,     5,     4,     5,     6,     6,     0,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     1,     0,
       2,     2,     4,     4,     1,     3,     3,     3,     3,     3,
       3,     3,     2,     1,     1,     3,     1,     2,     1,     3,
       2,     0,     2,     1,     2,     1,     1,     1,     0,     5,
       3,     1,     3,     3,     5,     1,     1,     3,     1,     2,
       3,     0,     2,     2,     1,     1,     1,     1,     2,     1,
       0,     1,     0,     2,     2,     1,     1,     1,     2,     0,
       1,     1,     3,     1,     1,     3,     3,     0,     1,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     4,     4,     4,     5,     2,     2,     1,     2,     1,
       2,     1,     2,     1,     2,     1,     1,     6,     6,     4,
       9,     9,     7,     6,     6,     4,     9,     9,     7,     2,
       3,     4,     6,     6,     9,     9,     6,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     4,     1,     3,
       4,     1,     3,     4,     3,     3,     1,     1,     2,     1,
       2,     1,     1,     2,     2,     2,     2,     2,     8,     8,
       9,     9,     4,     3,     3,     2,     2,     2,     1,     3,
       4,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     4,     4,     4,     3,     3,     3,
       3,     5,     5,     3,     4,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       3,     4,     3,     4,     3,     5,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1
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
#line 633 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6028 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 637 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6034 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 638 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6040 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 645 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6048 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 653 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6056 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 657 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6065 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 665 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6071 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: unstable_decl_stmt  */
#line 666 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6077 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: include_module_stmt  */
#line 667 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6083 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: block_stmt  */
#line 668 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6089 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: use_stmt  */
#line 669 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6095 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: import_stmt  */
#line 670 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6101 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: require_stmt  */
#line 671 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6107 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: extern_block_stmt  */
#line 672 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6113 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 673 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6119 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 674 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6125 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: TDEFER stmt  */
#line 676 "chpl.ypp"
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
#line 6142 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: try_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6148 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: return_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6154 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 691 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6167 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 700 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6180 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TLABEL ident_def stmt  */
#line 709 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6188 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: TYIELD opt_try_expr TSEMI  */
#line 713 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6199 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: error TSEMI  */
#line 720 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6208 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: assignment_stmt  */
#line 727 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6214 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: if_stmt  */
#line 728 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6220 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: loop_stmt  */
#line 729 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6226 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: select_stmt  */
#line 730 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6232 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: manage_stmt  */
#line 731 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6238 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 732 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6244 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: throw_stmt  */
#line 733 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6250 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 735 "chpl.ypp"
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
#line 6268 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 749 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    YYLTYPE locBodyAnchor = context->makeLocationAtLast((yylsp[-1]));
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]),
                               false, locBodyAnchor, (yyvsp[0].commentsAndStmt));
    CHPL_ASSERT(blockStyle == BlockStyle::EXPLICIT);
    auto taskBodies = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Cobegin::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].withClause)),
                               std::move(taskBodies));
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6287 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 764 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6299 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 772 "chpl.ypp"
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
#line 6315 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL do_stmt  */
#line 784 "chpl.ypp"
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
#line 6330 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TON expr do_stmt  */
#line 795 "chpl.ypp"
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
#line 6346 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 807 "chpl.ypp"
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
#line 6362 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL do_stmt  */
#line 819 "chpl.ypp"
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
#line 6377 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSYNC stmt  */
#line 830 "chpl.ypp"
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
#line 6394 "bison-chpl-lib.cpp"
    break;

  case 45: /* $@1: %empty  */
#line 847 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6402 "bison-chpl-lib.cpp"
    break;

  case 46: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 decl_base  */
#line 851 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6410 "bison-chpl-lib.cpp"
    break;

  case 47: /* $@2: %empty  */
#line 855 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6418 "bison-chpl-lib.cpp"
    break;

  case 48: /* deprecated_decl_stmt: TDEPRECATED $@2 decl_base  */
#line 859 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6426 "bison-chpl-lib.cpp"
    break;

  case 51: /* $@3: %empty  */
#line 871 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 6434 "bison-chpl-lib.cpp"
    break;

  case 52: /* unstable_decl_stmt: TUNSTABLE STRINGLITERAL $@3 decl_base  */
#line 875 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6442 "bison-chpl-lib.cpp"
    break;

  case 53: /* $@4: %empty  */
#line 879 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 6450 "bison-chpl-lib.cpp"
    break;

  case 54: /* unstable_decl_stmt: TUNSTABLE $@4 decl_base  */
#line 883 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6458 "bison-chpl-lib.cpp"
    break;

  case 55: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 890 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));

      ModuleParts parts = {
        .comments=context->gatherComments(loc),
        .attributeGroup=context->buildAttributeGroup((yyloc)).release(),
        .visibility=(yyvsp[-3].visibilityTag),
        .kind=(yyvsp[-2].moduleKind),
        .name=(yyvsp[0].uniqueStr)
      };

      (yyval.moduleParts) = parts;
      context->resetDeclState();
      context->clearComments();
    }
#line 6479 "bison-chpl-lib.cpp"
    break;

  case 56: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 910 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-1]));

      ModuleParts parts = (yyvsp[-2].moduleParts);
      ParserExprList* body = context->makeList();
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-2])), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6498 "bison-chpl-lib.cpp"
    break;

  case 57: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 925 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-2]));

      ModuleParts parts = (yyvsp[-3].moduleParts);
      ParserExprList* body = (yyvsp[-1].exprList);
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6517 "bison-chpl-lib.cpp"
    break;

  case 58: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 940 "chpl.ypp"
    {
      ModuleParts parts = (yyvsp[-3].moduleParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      ParserExprList* body = context->makeList(std::move(err));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6534 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_access_control: %empty  */
#line 955 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6540 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_access_control: TPUBLIC  */
#line 956 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6547 "bison-chpl-lib.cpp"
    break;

  case 61: /* opt_access_control: TPRIVATE  */
#line 958 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6554 "bison-chpl-lib.cpp"
    break;

  case 62: /* opt_prototype: %empty  */
#line 963 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6560 "bison-chpl-lib.cpp"
    break;

  case 63: /* opt_prototype: TPROTOTYPE  */
#line 964 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6567 "bison-chpl-lib.cpp"
    break;

  case 64: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 970 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6577 "bison-chpl-lib.cpp"
    break;

  case 65: /* block_stmt_body: TLCBR TRCBR  */
#line 988 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6585 "bison-chpl-lib.cpp"
    break;

  case 66: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 992 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6593 "bison-chpl-lib.cpp"
    break;

  case 67: /* block_stmt_body: TLCBR error TRCBR  */
#line 996 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 68: /* block_stmt: block_stmt_body  */
#line 1007 "chpl.ypp"
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
#line 6624 "bison-chpl-lib.cpp"
    break;

  case 69: /* stmt_ls: toplevel_stmt  */
#line 1025 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6630 "bison-chpl-lib.cpp"
    break;

  case 70: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1026 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6636 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: expr  */
#line 1031 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6644 "bison-chpl-lib.cpp"
    break;

  case 72: /* renames_ls: all_op_name  */
#line 1035 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 73: /* renames_ls: expr TAS expr  */
#line 1039 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6661 "bison-chpl-lib.cpp"
    break;

  case 74: /* renames_ls: renames_ls TCOMMA expr  */
#line 1044 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6669 "bison-chpl-lib.cpp"
    break;

  case 75: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1048 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6677 "bison-chpl-lib.cpp"
    break;

  case 76: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1052 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6686 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: expr  */
#line 1062 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6695 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_renames_ls: expr TAS expr  */
#line 1067 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6705 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1073 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6716 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1080 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6725 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1085 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6736 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1092 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6748 "bison-chpl-lib.cpp"
    break;

  case 83: /* opt_only_ls: %empty  */
#line 1102 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6754 "bison-chpl-lib.cpp"
    break;

  case 84: /* opt_only_ls: renames_ls  */
#line 1103 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6760 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1108 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6769 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1113 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6781 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1121 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6794 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1130 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6808 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1140 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6820 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1148 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6833 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1157 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6847 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1170 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6856 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_expr: expr  */
#line 1178 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6864 "bison-chpl-lib.cpp"
    break;

  case 94: /* import_expr: expr TDOT all_op_name  */
#line 1182 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6873 "bison-chpl-lib.cpp"
    break;

  case 95: /* import_expr: expr TAS ident_use  */
#line 1187 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6883 "bison-chpl-lib.cpp"
    break;

  case 96: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1193 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6893 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_ls: import_expr  */
#line 1201 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6899 "bison-chpl-lib.cpp"
    break;

  case 98: /* import_ls: import_ls TCOMMA import_expr  */
#line 1202 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6905 "bison-chpl-lib.cpp"
    break;

  case 99: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1207 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6915 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1216 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6923 "bison-chpl-lib.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1220 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6931 "bison-chpl-lib.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1224 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6939 "bison-chpl-lib.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1228 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6947 "bison-chpl-lib.cpp"
    break;

  case 104: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1232 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 6955 "bison-chpl-lib.cpp"
    break;

  case 105: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1236 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 6963 "bison-chpl-lib.cpp"
    break;

  case 106: /* opt_label_ident: %empty  */
#line 1244 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 6969 "bison-chpl-lib.cpp"
    break;

  case 107: /* opt_label_ident: TIDENT  */
#line 1245 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6975 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_use: TIDENT  */
#line 1270 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6981 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_use: TTHIS  */
#line 1271 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TBOOL  */
#line 1298 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6993 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TENUM  */
#line 1299 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TINT  */
#line 1300 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TUINT  */
#line 1301 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TREAL  */
#line 1302 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7017 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TIMAG  */
#line 1303 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7023 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TCOMPLEX  */
#line 1304 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7029 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TBYTES  */
#line 1305 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7035 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TSTRING  */
#line 1306 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7041 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TLOCALE  */
#line 1307 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TNOTHING  */
#line 1308 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TVOID  */
#line 1309 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 152: /* do_stmt: TDO stmt  */
#line 1323 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 153: /* do_stmt: block_stmt  */
#line 1324 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 154: /* return_stmt: TRETURN TSEMI  */
#line 1329 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7082 "bison-chpl-lib.cpp"
    break;

  case 155: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1336 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7093 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@5: %empty  */
#line 1347 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 158: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@5 class_level_stmt  */
#line 1351 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@6: %empty  */
#line 1355 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7117 "bison-chpl-lib.cpp"
    break;

  case 160: /* deprecated_class_level_stmt: TDEPRECATED $@6 class_level_stmt  */
#line 1359 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7125 "bison-chpl-lib.cpp"
    break;

  case 161: /* $@7: %empty  */
#line 1366 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 7133 "bison-chpl-lib.cpp"
    break;

  case 162: /* unstable_class_level_stmt: TUNSTABLE STRINGLITERAL $@7 class_level_stmt  */
#line 1370 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7141 "bison-chpl-lib.cpp"
    break;

  case 163: /* $@8: %empty  */
#line 1374 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 7149 "bison-chpl-lib.cpp"
    break;

  case 164: /* unstable_class_level_stmt: TUNSTABLE $@8 class_level_stmt  */
#line 1378 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7157 "bison-chpl-lib.cpp"
    break;

  case 165: /* class_level_stmt: TSEMI  */
#line 1385 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7165 "bison-chpl-lib.cpp"
    break;

  case 166: /* class_level_stmt: inner_class_level_stmt  */
#line 1389 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7175 "bison-chpl-lib.cpp"
    break;

  case 167: /* $@9: %empty  */
#line 1394 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7182 "bison-chpl-lib.cpp"
    break;

  case 168: /* class_level_stmt: TPUBLIC $@9 inner_class_level_stmt  */
#line 1396 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 169: /* $@10: %empty  */
#line 1400 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7198 "bison-chpl-lib.cpp"
    break;

  case 170: /* class_level_stmt: TPRIVATE $@10 inner_class_level_stmt  */
#line 1402 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1420 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7216 "bison-chpl-lib.cpp"
    break;

  case 179: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1425 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7225 "bison-chpl-lib.cpp"
    break;

  case 180: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1430 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7234 "bison-chpl-lib.cpp"
    break;

  case 181: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1435 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7242 "bison-chpl-lib.cpp"
    break;

  case 182: /* forwarding_decl_start: TFORWARDING  */
#line 1442 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7251 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_or_export: TEXTERN  */
#line 1449 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7257 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_or_export: TEXPORT  */
#line 1450 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7263 "bison-chpl-lib.cpp"
    break;

  case 185: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1455 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7273 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1465 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7282 "bison-chpl-lib.cpp"
    break;

  case 187: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1471 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7293 "bison-chpl-lib.cpp"
    break;

  case 188: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1478 "chpl.ypp"
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
#line 7315 "bison-chpl-lib.cpp"
    break;

  case 189: /* $@11: %empty  */
#line 1496 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7323 "bison-chpl-lib.cpp"
    break;

  case 190: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1500 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7332 "bison-chpl-lib.cpp"
    break;

  case 191: /* $@12: %empty  */
#line 1505 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7341 "bison-chpl-lib.cpp"
    break;

  case 192: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@12 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1510 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7350 "bison-chpl-lib.cpp"
    break;

  case 193: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1518 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7358 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1525 "chpl.ypp"
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
#line 7380 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TWHILE expr do_stmt  */
#line 1543 "chpl.ypp"
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
#line 7397 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1556 "chpl.ypp"
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
#line 7414 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1569 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7422 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1573 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7430 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1577 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7438 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1581 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7446 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1585 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7454 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOR expr do_stmt  */
#line 1589 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7462 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1593 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7470 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1597 "chpl.ypp"
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
#line 7492 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1615 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7500 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1619 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7508 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1623 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7516 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1627 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7524 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFORALL expr do_stmt  */
#line 1631 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7532 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1635 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7540 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7548 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1643 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7556 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7564 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7572 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7580 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7588 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7596 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7604 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7612 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7620 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1679 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7628 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1683 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1687 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7644 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1691 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7652 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1695 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7660 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7668 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1703 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7677 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1708 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7686 "bison-chpl-lib.cpp"
    break;

  case 229: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1716 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7696 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF expr TTHEN stmt  */
#line 1725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7704 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF expr block_stmt  */
#line 1729 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7712 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1733 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7720 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1737 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7728 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1741 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7736 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF ifvar block_stmt  */
#line 1745 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7744 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1749 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7752 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1753 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7760 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1757 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7769 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1762 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1767 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7787 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1772 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 242: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1780 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                   /*attributeGroup*/ nullptr,
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
#line 7815 "bison-chpl-lib.cpp"
    break;

  case 243: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1795 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                   /*attributeGroup*/ nullptr,
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
#line 7834 "bison-chpl-lib.cpp"
    break;

  case 244: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1813 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7842 "bison-chpl-lib.cpp"
    break;

  case 245: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1817 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7850 "bison-chpl-lib.cpp"
    break;

  case 246: /* ifc_formal_ls: ifc_formal  */
#line 1823 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7856 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1824 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7862 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifc_formal: ident_def  */
#line 1829 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7868 "bison-chpl-lib.cpp"
    break;

  case 261: /* implements_type_ident: implements_type_error_ident  */
#line 1847 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 7877 "bison-chpl-lib.cpp"
    break;

  case 268: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1873 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7885 "bison-chpl-lib.cpp"
    break;

  case 269: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1877 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7893 "bison-chpl-lib.cpp"
    break;

  case 270: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1881 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7901 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1888 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7909 "bison-chpl-lib.cpp"
    break;

  case 272: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1892 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 7917 "bison-chpl-lib.cpp"
    break;

  case 273: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1896 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 274: /* try_stmt: TTRY tryable_stmt  */
#line 1903 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 7933 "bison-chpl-lib.cpp"
    break;

  case 275: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1907 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 7941 "bison-chpl-lib.cpp"
    break;

  case 276: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1911 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 7949 "bison-chpl-lib.cpp"
    break;

  case 277: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1915 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 7957 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_ls: %empty  */
#line 1921 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 7963 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1922 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 7969 "bison-chpl-lib.cpp"
    break;

  case 280: /* catch_expr: TCATCH block_stmt  */
#line 1927 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 7977 "bison-chpl-lib.cpp"
    break;

  case 281: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1931 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 7985 "bison-chpl-lib.cpp"
    break;

  case 282: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1935 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 7993 "bison-chpl-lib.cpp"
    break;

  case 283: /* catch_expr_inner: ident_def  */
#line 1942 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
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
#line 8010 "bison-chpl-lib.cpp"
    break;

  case 284: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1955 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
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
#line 8027 "bison-chpl-lib.cpp"
    break;

  case 285: /* throw_stmt: TTHROW expr TSEMI  */
#line 1971 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 286: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1979 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8045 "bison-chpl-lib.cpp"
    break;

  case 287: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1983 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 288: /* when_stmt_ls: %empty  */
#line 1991 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8061 "bison-chpl-lib.cpp"
    break;

  case 289: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1992 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8067 "bison-chpl-lib.cpp"
    break;

  case 290: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1997 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 291: /* when_stmt: TOTHERWISE stmt  */
#line 2001 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8084 "bison-chpl-lib.cpp"
    break;

  case 292: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2006 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8093 "bison-chpl-lib.cpp"
    break;

  case 293: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2014 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8102 "bison-chpl-lib.cpp"
    break;

  case 294: /* manager_expr: expr TAS ident_def  */
#line 2019 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8110 "bison-chpl-lib.cpp"
    break;

  case 295: /* manager_expr: expr  */
#line 2023 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8118 "bison-chpl-lib.cpp"
    break;

  case 296: /* manager_expr_ls: manager_expr  */
#line 2029 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8124 "bison-chpl-lib.cpp"
    break;

  case 297: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2030 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8130 "bison-chpl-lib.cpp"
    break;

  case 298: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2035 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8138 "bison-chpl-lib.cpp"
    break;

  case 299: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2044 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8147 "bison-chpl-lib.cpp"
    break;

  case 300: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2049 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8158 "bison-chpl-lib.cpp"
    break;

  case 301: /* class_start: class_tag ident_def  */
#line 2060 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8166 "bison-chpl-lib.cpp"
    break;

  case 302: /* class_tag: TCLASS  */
#line 2066 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8172 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_tag: TRECORD  */
#line 2067 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8178 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_tag: TUNION  */
#line 2068 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8184 "bison-chpl-lib.cpp"
    break;

  case 305: /* opt_inherit: %empty  */
#line 2072 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8190 "bison-chpl-lib.cpp"
    break;

  case 306: /* opt_inherit: TCOLON expr_ls  */
#line 2073 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8196 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_level_stmt_ls: %empty  */
#line 2077 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8205 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2082 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_level_stmt_ls: class_level_stmt_ls unstable_class_level_stmt  */
#line 2086 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8221 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2090 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8229 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls unstable_class_level_stmt  */
#line 2094 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8237 "bison-chpl-lib.cpp"
    break;

  case 312: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2101 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      ParserExprList* list = (yyvsp[-1].exprList);
      // get any comments after the last element but before the closing brace
      context->appendList(list, context->gatherComments((yylsp[0])));

      auto decl = Enum::build(BUILDER, LOC((yyloc)), toOwned(parts.attributeGroup),
                              parts.visibility,
                              parts.name,
                              context->consumeList(list));
      CommentsAndStmt cs = {parts.comments, decl.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8258 "bison-chpl-lib.cpp"
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
#line 8272 "bison-chpl-lib.cpp"
    break;

  case 314: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2131 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8280 "bison-chpl-lib.cpp"
    break;

  case 315: /* enum_ls: deprecated_enum_item  */
#line 2138 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8289 "bison-chpl-lib.cpp"
    break;

  case 316: /* enum_ls: enum_ls TCOMMA  */
#line 2143 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8299 "bison-chpl-lib.cpp"
    break;

  case 317: /* $@13: %empty  */
#line 2149 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8308 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_ls: enum_ls TCOMMA $@13 deprecated_enum_item  */
#line 2154 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8317 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_ls: unstable_enum_item  */
#line 2160 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8326 "bison-chpl-lib.cpp"
    break;

  case 320: /* $@14: %empty  */
#line 2165 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8335 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_ls TCOMMA $@14 unstable_enum_item  */
#line 2170 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8344 "bison-chpl-lib.cpp"
    break;

  case 323: /* $@15: %empty  */
#line 2180 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8352 "bison-chpl-lib.cpp"
    break;

  case 324: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@15 enum_item  */
#line 2184 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8360 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@16: %empty  */
#line 2188 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8368 "bison-chpl-lib.cpp"
    break;

  case 326: /* deprecated_enum_item: TDEPRECATED $@16 enum_item  */
#line 2192 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8376 "bison-chpl-lib.cpp"
    break;

  case 327: /* $@17: %empty  */
#line 2199 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 8384 "bison-chpl-lib.cpp"
    break;

  case 328: /* unstable_enum_item: TUNSTABLE STRINGLITERAL $@17 enum_item  */
#line 2203 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8392 "bison-chpl-lib.cpp"
    break;

  case 329: /* $@18: %empty  */
#line 2207 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 330: /* unstable_enum_item: TUNSTABLE $@18 enum_item  */
#line 2211 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8408 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_item: ident_def  */
#line 2218 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8419 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_item: ident_def TASSIGN expr  */
#line 2225 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 333: /* lambda_decl_start: TLAMBDA  */
#line 2237 "chpl.ypp"
    {
      FunctionParts fp = context->makeFunctionParts(false, false);
      context->noteDeclStartLoc((yylsp[0]));
      auto loc = context->declStartLoc((yyloc));
      fp.comments = context->gatherComments(loc);
      fp.attributeGroup = context->buildAttributeGroup((yyloc)).release();
      fp.visibility = context->visibility;
      context->resetAttributeGroupPartsState();
      fp.kind = Function::PROC;
      (yyval.functionParts) = fp;
    }
#line 8448 "bison-chpl-lib.cpp"
    break;

  case 334: /* $@19: %empty  */
#line 2254 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8458 "bison-chpl-lib.cpp"
    break;

  case 335: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@19 function_body_stmt  */
#line 2260 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-7].functionParts);
      fp.name = context->buildIdent((yylsp[-7]), STR("lambda"));
      fp.formals = (yyvsp[-6].exprList);
      fp.returnIntent = (yyvsp[-5].returnTag);
      fp.returnType = (yyvsp[-4].expr);
      fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS);
      WhereAndLifetime wl = (yyvsp[-2].lifetimeAndWhere);
      fp.lifetime = wl.lifetime;
      fp.where = wl.where;
      fp.body = (yyvsp[0].exprList);

      context->clearComments();
      context->exitScope(asttags::Function, STR("lambda"));

      (yyval.expr) = context->buildLambda((yyloc), fp);
    }
#line 8480 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: linkage_spec_empty  */
#line 2283 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8486 "bison-chpl-lib.cpp"
    break;

  case 338: /* linkage_spec: TINLINE  */
#line 2284 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8493 "bison-chpl-lib.cpp"
    break;

  case 339: /* linkage_spec: TOVERRIDE  */
#line 2286 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8500 "bison-chpl-lib.cpp"
    break;

  case 340: /* opt_fn_type_formal_ls: %empty  */
#line 2291 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8506 "bison-chpl-lib.cpp"
    break;

  case 341: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2292 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8512 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal_ls: fn_type_formal  */
#line 2296 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8518 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2297 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8524 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: named_formal  */
#line 2302 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8530 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2305 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8536 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type_formal: formal_type  */
#line 2307 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8542 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_ret_type: %empty  */
#line 2311 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8548 "bison-chpl-lib.cpp"
    break;

  case 348: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2312 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8554 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2325 "chpl.ypp"
  {
    FunctionParts fp = context->makeFunctionParts(false, false);
    fp.kind = Function::PROC;
    fp.name = context->buildIdent((yylsp[-5]), STR("proc"));
    fp.formals = (yyvsp[-4].exprList);
    fp.returnIntent = (yyvsp[-2].returnTag);
    fp.returnType = (yyvsp[-1].expr);
    fp.throws = ((yyvsp[0].throwsTag) == ThrowsTag_THROWS);
    fp.body = nullptr;
    fp.comments = nullptr;
    fp.attributeGroup = nullptr;
    fp.visibility = context->visibility;
    (yyval.functionParts) = fp;
  }
#line 8573 "bison-chpl-lib.cpp"
    break;

  case 350: /* $@20: %empty  */
#line 2343 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_expr: fn_type $@20 block_stmt_body  */
#line 2349 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@21: %empty  */
#line 2358 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8606 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_expr: fn_type TALIAS $@21 expr  */
#line 2364 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8619 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2376 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8627 "bison-chpl-lib.cpp"
    break;

  case 355: /* $@22: %empty  */
#line 2385 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 8641 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@22 opt_function_body_stmt  */
#line 2395 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-6].functionParts);
    fp.returnIntent = (yyvsp[-5].returnTag);
    fp.returnType = (yyvsp[-4].expr);
    fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS);
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
#line 8664 "bison-chpl-lib.cpp"
    break;

  case 357: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2417 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8676 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2425 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8688 "bison-chpl-lib.cpp"
    break;

  case 359: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2433 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])), /*attributeGroup*/ nullptr,
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8703 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2444 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])), /*attributeGroup*/ nullptr,
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8718 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2455 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2464 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-1].functionParts);
      context->noteDeclStartLoc((yylsp[0]));
      auto loc = context->declStartLoc((yyloc));
      fp.comments = context->gatherComments(loc);
      fp.attributeGroup = context->buildAttributeGroup((yyloc)).release();
      fp.visibility = context->visibility;
      context->resetAttributeGroupPartsState();
      fp.kind = (yyvsp[0].functionKind);
      (yyval.functionParts) = fp;
    }
#line 8744 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2479 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8750 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_ident: ident_def TBANG  */
#line 2486 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8760 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2543 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8766 "bison-chpl-lib.cpp"
    break;

  case 408: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2544 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8772 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_formal_ls: %empty  */
#line 2548 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8778 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2549 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8784 "bison-chpl-lib.cpp"
    break;

  case 411: /* req_formal_ls: TLP TRP  */
#line 2553 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8790 "bison-chpl-lib.cpp"
    break;

  case 412: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2554 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8796 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_ls_inner: formal  */
#line 2558 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8802 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2559 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8808 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls: %empty  */
#line 2563 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8814 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_ls: formal_ls_inner  */
#line 2564 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8820 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2578 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8828 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2583 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8836 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2588 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8844 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2593 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8852 "bison-chpl-lib.cpp"
    break;

  case 424: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2598 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 425: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2603 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8868 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_formal_intent_tag: %empty  */
#line 2609 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8877 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_formal_intent_tag: required_intent_tag  */
#line 2614 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8886 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TIN  */
#line 2621 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TINOUT  */
#line 2622 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8898 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TOUT  */
#line 2623 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8904 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TCONST TIN  */
#line 2624 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8910 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TCONST TREF  */
#line 2625 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8916 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TCONST  */
#line 2626 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8922 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TPARAM  */
#line 2627 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8928 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TREF  */
#line 2628 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8934 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TTYPE  */
#line 2629 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8940 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: %empty  */
#line 2633 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8946 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TPARAM  */
#line 2634 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8952 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TREF  */
#line 2635 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8958 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TCONST TREF  */
#line 2636 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8964 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TCONST  */
#line 2637 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8970 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TTYPE  */
#line 2638 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8976 "bison-chpl-lib.cpp"
    break;

  case 443: /* proc_iter_or_op: TPROC  */
#line 2642 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8982 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TITER  */
#line 2643 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 445: /* proc_iter_or_op: TOPERATOR  */
#line 2644 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8994 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_ret_tag: %empty  */
#line 2648 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9000 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: TCONST  */
#line 2649 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TCONST TREF  */
#line 2650 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TREF  */
#line 2651 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TPARAM  */
#line 2652 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TTYPE  */
#line 2653 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_throws_error: %empty  */
#line 2657 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_throws_error: TTHROWS  */
#line 2658 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_function_body_stmt: TSEMI  */
#line 2661 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_function_body_stmt: function_body_stmt  */
#line 2662 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 456: /* function_body_stmt: block_stmt_body  */
#line 2666 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 457: /* function_body_stmt: return_stmt  */
#line 2667 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 458: /* query_expr: TQUERIEDIDENT  */
#line 2671 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_lifetime_where: %empty  */
#line 2676 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_lifetime_where: TWHERE expr  */
#line 2678 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2680 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2682 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2684 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 464: /* lifetime_components_expr: lifetime_expr  */
#line 2689 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2691 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 466: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2696 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 467: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2698 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 468: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2700 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2702 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2704 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2706 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2708 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9156 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_ident: TIDENT  */
#line 2712 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_ident: TTHIS  */
#line 2713 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 475: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2718 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9177 "bison-chpl-lib.cpp"
    break;

  case 476: /* type_alias_decl_stmt_start: TTYPE  */
#line 2727 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9185 "bison-chpl-lib.cpp"
    break;

  case 477: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2731 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9194 "bison-chpl-lib.cpp"
    break;

  case 478: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2739 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9202 "bison-chpl-lib.cpp"
    break;

  case 479: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2743 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 480: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2750 "chpl.ypp"
  {
    // TODO (dlongnecke-cray): Add a helper to build this and var_decl_stmt.
    auto node = Variable::build(BUILDER, LOC((yyloc)),
                                context->buildAttributeGroup((yyloc)),
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
#line 9232 "bison-chpl-lib.cpp"
    break;

  case 481: /* opt_init_type: %empty  */
#line 2770 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9238 "bison-chpl-lib.cpp"
    break;

  case 482: /* opt_init_type: TASSIGN expr  */
#line 2772 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9244 "bison-chpl-lib.cpp"
    break;

  case 483: /* var_decl_type: TPARAM  */
#line 2776 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9250 "bison-chpl-lib.cpp"
    break;

  case 484: /* var_decl_type: TCONST TREF  */
#line 2777 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9256 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_type: TREF  */
#line 2778 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9262 "bison-chpl-lib.cpp"
    break;

  case 486: /* var_decl_type: TCONST  */
#line 2779 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9268 "bison-chpl-lib.cpp"
    break;

  case 487: /* var_decl_type: TVAR  */
#line 2780 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9274 "bison-chpl-lib.cpp"
    break;

  case 488: /* $@23: %empty  */
#line 2785 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9283 "bison-chpl-lib.cpp"
    break;

  case 489: /* var_decl_stmt: TCONFIG $@23 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2789 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9292 "bison-chpl-lib.cpp"
    break;

  case 490: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2794 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2802 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9309 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2806 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9317 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2813 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
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
#line 9337 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2829 "chpl.ypp"
    {
      auto intentOrKind = (TupleDecl::IntentOrKind) context->varDeclKind;
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        context->buildAttributeGroup((yyloc)),
                                        context->visibility,
                                        context->linkage,
                                        intentOrKind,
                                        context->consumeList((yyvsp[-3].exprList)),
                                        toOwned((yyvsp[-1].expr)),
                                        toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yylsp[-4]), tupleDecl.release());
      context->clearComments();
    }
#line 9355 "bison-chpl-lib.cpp"
    break;

  case 495: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2846 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9363 "bison-chpl-lib.cpp"
    break;

  case 496: /* tuple_var_decl_component: ident_def  */
#line 2850 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 497: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2854 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9379 "bison-chpl-lib.cpp"
    break;

  case 498: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2861 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9385 "bison-chpl-lib.cpp"
    break;

  case 499: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2863 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9391 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2865 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9397 "bison-chpl-lib.cpp"
    break;

  case 501: /* opt_init_expr: %empty  */
#line 2871 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9403 "bison-chpl-lib.cpp"
    break;

  case 502: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2872 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9409 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2873 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9415 "bison-chpl-lib.cpp"
    break;

  case 504: /* formal_or_ret_type_expr: expr  */
#line 2877 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9421 "bison-chpl-lib.cpp"
    break;

  case 505: /* ret_type: formal_or_ret_type_expr  */
#line 2881 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9427 "bison-chpl-lib.cpp"
    break;

  case 506: /* ret_type: reserved_type_ident_use  */
#line 2882 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9433 "bison-chpl-lib.cpp"
    break;

  case 507: /* ret_type: error  */
#line 2883 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 508: /* colon_ret_type: TCOLON ret_type  */
#line 2887 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9445 "bison-chpl-lib.cpp"
    break;

  case 509: /* colon_ret_type: error  */
#line 2888 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9451 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_ret_type: %empty  */
#line 2892 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_type: %empty  */
#line 2897 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9463 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_type: TCOLON expr  */
#line 2898 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9469 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2899 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_type: error  */
#line 2900 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9481 "bison-chpl-lib.cpp"
    break;

  case 516: /* formal_type: formal_or_ret_type_expr  */
#line 2904 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9487 "bison-chpl-lib.cpp"
    break;

  case 517: /* formal_type: reserved_type_ident_use  */
#line 2905 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 518: /* colon_formal_type: TCOLON formal_type  */
#line 2909 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_colon_formal_type: %empty  */
#line 2913 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_colon_formal_type: colon_formal_type  */
#line 2914 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9511 "bison-chpl-lib.cpp"
    break;

  case 521: /* expr_ls: expr  */
#line 2920 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 522: /* expr_ls: expr_ls TCOMMA expr  */
#line 2921 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 523: /* tuple_component: TUNDERSCORE  */
#line 2925 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 524: /* tuple_component: opt_try_expr  */
#line 2926 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 525: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2931 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9543 "bison-chpl-lib.cpp"
    break;

  case 526: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2935 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9551 "bison-chpl-lib.cpp"
    break;

  case 527: /* opt_actual_ls: %empty  */
#line 2941 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9557 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_actual_ls: actual_ls  */
#line 2942 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9563 "bison-chpl-lib.cpp"
    break;

  case 529: /* actual_ls: actual_expr  */
#line 2947 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 530: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2952 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9582 "bison-chpl-lib.cpp"
    break;

  case 531: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2960 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9588 "bison-chpl-lib.cpp"
    break;

  case 532: /* actual_expr: opt_try_expr  */
#line 2961 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9594 "bison-chpl-lib.cpp"
    break;

  case 533: /* ident_expr: ident_use  */
#line 2965 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9600 "bison-chpl-lib.cpp"
    break;

  case 534: /* ident_expr: scalar_type  */
#line 2966 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9606 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TSINGLE expr  */
#line 2979 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9612 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2981 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9618 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2983 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9624 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2985 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9630 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2987 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9640 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TATOMIC expr  */
#line 2993 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9646 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TSYNC expr  */
#line 2995 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9652 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TOWNED  */
#line 2998 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9658 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TOWNED expr  */
#line 3000 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9664 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TUNMANAGED  */
#line 3002 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9670 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3004 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9676 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TSHARED  */
#line 3006 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TSHARED expr  */
#line 3008 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9688 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TBORROWED  */
#line 3010 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9694 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TBORROWED expr  */
#line 3012 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9700 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TCLASS  */
#line 3014 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TRECORD  */
#line 3016 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9712 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 3021 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 9725 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3030 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 9738 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TFOR expr TDO expr  */
#line 3039 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 9750 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3047 "chpl.ypp"
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
#line 9768 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3061 "chpl.ypp"
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
#line 9786 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3075 "chpl.ypp"
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
#line 9804 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3089 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9817 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3098 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9830 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr: TFORALL expr TDO expr  */
#line 3107 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9842 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3115 "chpl.ypp"
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
#line 9860 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3129 "chpl.ypp"
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
#line 9878 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3143 "chpl.ypp"
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
#line 9896 "bison-chpl-lib.cpp"
    break;

  case 569: /* bracket_loop_expr: TLSBR TRSBR  */
#line 3160 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9904 "bison-chpl-lib.cpp"
    break;

  case 570: /* bracket_loop_expr: TLSBR TRSBR expr  */
#line 3164 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 9912 "bison-chpl-lib.cpp"
    break;

  case 571: /* bracket_loop_expr: TLSBR expr_ls TRSBR expr  */
#line 3168 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9920 "bison-chpl-lib.cpp"
    break;

  case 572: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3172 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3176 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9936 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3180 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9944 "bison-chpl-lib.cpp"
    break;

  case 575: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3184 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9952 "bison-chpl-lib.cpp"
    break;

  case 576: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3191 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 9966 "bison-chpl-lib.cpp"
    break;

  case 577: /* nil_expr: TNIL  */
#line 3208 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9972 "bison-chpl-lib.cpp"
    break;

  case 585: /* opt_task_intent_ls: %empty  */
#line 3226 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9978 "bison-chpl-lib.cpp"
    break;

  case 586: /* opt_task_intent_ls: task_intent_clause  */
#line 3227 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9984 "bison-chpl-lib.cpp"
    break;

  case 587: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3232 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9994 "bison-chpl-lib.cpp"
    break;

  case 588: /* task_intent_ls: intent_expr  */
#line 3240 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10000 "bison-chpl-lib.cpp"
    break;

  case 589: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3241 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 590: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3246 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10016 "bison-chpl-lib.cpp"
    break;

  case 591: /* forall_intent_ls: intent_expr  */
#line 3254 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10022 "bison-chpl-lib.cpp"
    break;

  case 592: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3255 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10028 "bison-chpl-lib.cpp"
    break;

  case 593: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3260 "chpl.ypp"
  {
    if (auto ident = (yyvsp[-2].expr)->toIdentifier()) {
      auto name = ident->name();
      auto node = TaskVar::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
                                 name,
                                 /*intent*/ (yyvsp[-3].taskIntent),
                                 toOwned((yyvsp[-1].expr)),
                                 toOwned((yyvsp[0].expr)));
      (yyval.expr) = node.release();
    } else {
      (yyval.expr) = context->syntax((yyloc), "expected identifier for task variable name.");
    }
  }
#line 10046 "bison-chpl-lib.cpp"
    break;

  case 594: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3274 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10054 "bison-chpl-lib.cpp"
    break;

  case 595: /* intent_expr: expr TREDUCE ident_expr  */
#line 3278 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10062 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_var_prefix: TCONST  */
#line 3284 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10068 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_var_prefix: TIN  */
#line 3285 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_var_prefix: TCONST TIN  */
#line 3286 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10080 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_var_prefix: TREF  */
#line 3287 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10086 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_var_prefix: TCONST TREF  */
#line 3288 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10092 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_var_prefix: TVAR  */
#line 3289 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10098 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW  */
#line 3294 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10104 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_maybe_decorated: TNEW TOWNED  */
#line 3296 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10110 "bison-chpl-lib.cpp"
    break;

  case 604: /* new_maybe_decorated: TNEW TSHARED  */
#line 3298 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10116 "bison-chpl-lib.cpp"
    break;

  case 605: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3300 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10122 "bison-chpl-lib.cpp"
    break;

  case 606: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3302 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10128 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_expr: new_maybe_decorated expr  */
#line 3308 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10136 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3315 "chpl.ypp"
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
#line 10152 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3327 "chpl.ypp"
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
#line 10168 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3339 "chpl.ypp"
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
#line 10185 "bison-chpl-lib.cpp"
    break;

  case 611: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3352 "chpl.ypp"
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
#line 10203 "bison-chpl-lib.cpp"
    break;

  case 612: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3369 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10211 "bison-chpl-lib.cpp"
    break;

  case 613: /* range_literal_expr: expr TDOTDOT expr  */
#line 3376 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10220 "bison-chpl-lib.cpp"
    break;

  case 614: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3381 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10229 "bison-chpl-lib.cpp"
    break;

  case 615: /* range_literal_expr: expr TDOTDOT  */
#line 3386 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10238 "bison-chpl-lib.cpp"
    break;

  case 616: /* range_literal_expr: TDOTDOT expr  */
#line 3391 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10248 "bison-chpl-lib.cpp"
    break;

  case 617: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3397 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 618: /* range_literal_expr: TDOTDOT  */
#line 3403 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10268 "bison-chpl-lib.cpp"
    break;

  case 619: /* cast_expr: expr TCOLON expr  */
#line 3433 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10276 "bison-chpl-lib.cpp"
    break;

  case 620: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3440 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10284 "bison-chpl-lib.cpp"
    break;

  case 621: /* super_expr: fn_expr  */
#line 3446 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10290 "bison-chpl-lib.cpp"
    break;

  case 625: /* expr: sub_type_level_expr TQUESTION  */
#line 3455 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10296 "bison-chpl-lib.cpp"
    break;

  case 626: /* expr: TQUESTION  */
#line 3457 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10302 "bison-chpl-lib.cpp"
    break;

  case 630: /* expr: fn_type  */
#line 3462 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10308 "bison-chpl-lib.cpp"
    break;

  case 640: /* opt_expr: %empty  */
#line 3476 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10314 "bison-chpl-lib.cpp"
    break;

  case 641: /* opt_expr: expr  */
#line 3477 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 642: /* opt_try_expr: TTRY expr  */
#line 3481 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10326 "bison-chpl-lib.cpp"
    break;

  case 643: /* opt_try_expr: TTRYBANG expr  */
#line 3482 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10332 "bison-chpl-lib.cpp"
    break;

  case 644: /* opt_try_expr: super_expr  */
#line 3483 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10338 "bison-chpl-lib.cpp"
    break;

  case 650: /* call_base_expr: expr TBANG  */
#line 3500 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 651: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3503 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10352 "bison-chpl-lib.cpp"
    break;

  case 654: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3510 "chpl.ypp"
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
#line 10368 "bison-chpl-lib.cpp"
    break;

  case 655: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3522 "chpl.ypp"
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
#line 10384 "bison-chpl-lib.cpp"
    break;

  case 656: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3534 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10392 "bison-chpl-lib.cpp"
    break;

  case 657: /* dot_expr: expr TDOT ident_use  */
#line 3541 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10398 "bison-chpl-lib.cpp"
    break;

  case 658: /* dot_expr: expr TDOT TTYPE  */
#line 3543 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10404 "bison-chpl-lib.cpp"
    break;

  case 659: /* dot_expr: expr TDOT TDOMAIN  */
#line 3545 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10410 "bison-chpl-lib.cpp"
    break;

  case 660: /* dot_expr: expr TDOT TLOCALE  */
#line 3547 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10416 "bison-chpl-lib.cpp"
    break;

  case 661: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3549 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10426 "bison-chpl-lib.cpp"
    break;

  case 662: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3555 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10436 "bison-chpl-lib.cpp"
    break;

  case 663: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3567 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10442 "bison-chpl-lib.cpp"
    break;

  case 664: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3569 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10450 "bison-chpl-lib.cpp"
    break;

  case 665: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3573 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10458 "bison-chpl-lib.cpp"
    break;

  case 666: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3577 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10466 "bison-chpl-lib.cpp"
    break;

  case 667: /* bool_literal: TFALSE  */
#line 3583 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10472 "bison-chpl-lib.cpp"
    break;

  case 668: /* bool_literal: TTRUE  */
#line 3584 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10478 "bison-chpl-lib.cpp"
    break;

  case 669: /* str_bytes_literal: STRINGLITERAL  */
#line 3588 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10484 "bison-chpl-lib.cpp"
    break;

  case 670: /* str_bytes_literal: BYTESLITERAL  */
#line 3589 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10490 "bison-chpl-lib.cpp"
    break;

  case 673: /* literal_expr: INTLITERAL  */
#line 3595 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10496 "bison-chpl-lib.cpp"
    break;

  case 674: /* literal_expr: REALLITERAL  */
#line 3596 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10502 "bison-chpl-lib.cpp"
    break;

  case 675: /* literal_expr: IMAGLITERAL  */
#line 3597 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10508 "bison-chpl-lib.cpp"
    break;

  case 676: /* literal_expr: CSTRINGLITERAL  */
#line 3598 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10514 "bison-chpl-lib.cpp"
    break;

  case 677: /* literal_expr: TNONE  */
#line 3599 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10520 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3601 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10529 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3606 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10538 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3611 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10546 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3615 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
  }
#line 10554 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3619 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
  }
#line 10562 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3623 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
  }
#line 10570 "bison-chpl-lib.cpp"
    break;

  case 684: /* assoc_expr_ls: expr TALIAS expr  */
#line 3631 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10579 "bison-chpl-lib.cpp"
    break;

  case 685: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3636 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10589 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TPLUS expr  */
#line 3644 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10595 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TMINUS expr  */
#line 3645 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10601 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TSTAR expr  */
#line 3646 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10607 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TDIVIDE expr  */
#line 3647 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10613 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3648 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10619 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3649 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10625 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TMOD expr  */
#line 3650 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10631 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TEQUAL expr  */
#line 3651 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10637 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3652 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10643 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3653 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10649 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3654 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10655 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TLESS expr  */
#line 3655 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10661 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TGREATER expr  */
#line 3656 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10667 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TBAND expr  */
#line 3657 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10673 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TBOR expr  */
#line 3658 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10679 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TBXOR expr  */
#line 3659 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TAND expr  */
#line 3660 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10691 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TOR expr  */
#line 3661 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10697 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TEXP expr  */
#line 3662 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10703 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TBY expr  */
#line 3663 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10709 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TALIGN expr  */
#line 3664 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10715 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr THASH expr  */
#line 3665 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10721 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TDMAPPED expr  */
#line 3666 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10727 "bison-chpl-lib.cpp"
    break;

  case 709: /* unary_op_expr: TPLUS expr  */
#line 3670 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10733 "bison-chpl-lib.cpp"
    break;

  case 710: /* unary_op_expr: TMINUS expr  */
#line 3671 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10739 "bison-chpl-lib.cpp"
    break;

  case 711: /* unary_op_expr: TMINUSMINUS expr  */
#line 3672 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10745 "bison-chpl-lib.cpp"
    break;

  case 712: /* unary_op_expr: TPLUSPLUS expr  */
#line 3673 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10751 "bison-chpl-lib.cpp"
    break;

  case 713: /* unary_op_expr: TBANG expr  */
#line 3674 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10757 "bison-chpl-lib.cpp"
    break;

  case 714: /* unary_op_expr: expr TBANG  */
#line 3675 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10765 "bison-chpl-lib.cpp"
    break;

  case 715: /* unary_op_expr: TBNOT expr  */
#line 3678 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10771 "bison-chpl-lib.cpp"
    break;

  case 716: /* reduce_expr: expr TREDUCE expr  */
#line 3683 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10779 "bison-chpl-lib.cpp"
    break;

  case 717: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3687 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10787 "bison-chpl-lib.cpp"
    break;

  case 718: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3691 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10795 "bison-chpl-lib.cpp"
    break;

  case 719: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3695 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10803 "bison-chpl-lib.cpp"
    break;

  case 720: /* scan_expr: expr TSCAN expr  */
#line 3702 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10811 "bison-chpl-lib.cpp"
    break;

  case 721: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3706 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10819 "bison-chpl-lib.cpp"
    break;

  case 722: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3710 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10827 "bison-chpl-lib.cpp"
    break;

  case 723: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3714 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10835 "bison-chpl-lib.cpp"
    break;


#line 10839 "bison-chpl-lib.cpp"

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
