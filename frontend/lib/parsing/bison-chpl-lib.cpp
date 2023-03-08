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
#define YYLAST   18599

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  206
/* YYNRULES -- Number of rules.  */
#define YYNRULES  731
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1285

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
    2652,  2653,  2657,  2658,  2661,  2662,  2666,  2667,  2668,  2672,
    2677,  2678,  2680,  2682,  2684,  2689,  2691,  2696,  2698,  2700,
    2702,  2704,  2706,  2708,  2713,  2714,  2718,  2727,  2731,  2739,
    2743,  2750,  2771,  2772,  2777,  2778,  2779,  2780,  2781,  2786,
    2785,  2794,  2802,  2806,  2813,  2829,  2846,  2850,  2854,  2861,
    2863,  2865,  2872,  2873,  2874,  2878,  2882,  2883,  2884,  2888,
    2889,  2893,  2894,  2898,  2899,  2900,  2901,  2905,  2906,  2910,
    2914,  2915,  2921,  2922,  2926,  2927,  2931,  2935,  2942,  2943,
    2947,  2952,  2961,  2962,  2966,  2967,  2974,  2975,  2976,  2977,
    2978,  2979,  2981,  2983,  2985,  2987,  2993,  2995,  2998,  3000,
    3002,  3004,  3006,  3008,  3010,  3012,  3014,  3016,  3021,  3030,
    3039,  3047,  3061,  3075,  3089,  3098,  3107,  3115,  3129,  3143,
    3160,  3164,  3168,  3172,  3176,  3180,  3184,  3191,  3209,  3217,
    3218,  3219,  3220,  3221,  3222,  3223,  3227,  3228,  3232,  3241,
    3242,  3246,  3255,  3256,  3260,  3274,  3278,  3285,  3286,  3287,
    3288,  3289,  3290,  3294,  3296,  3298,  3300,  3302,  3308,  3315,
    3327,  3339,  3352,  3369,  3376,  3381,  3386,  3391,  3397,  3403,
    3433,  3440,  3447,  3448,  3452,  3454,  3455,  3457,  3459,  3460,
    3461,  3462,  3465,  3466,  3467,  3468,  3469,  3470,  3471,  3472,
    3473,  3477,  3478,  3482,  3483,  3484,  3488,  3489,  3490,  3491,
    3500,  3501,  3504,  3505,  3506,  3510,  3522,  3534,  3541,  3543,
    3545,  3547,  3549,  3555,  3568,  3569,  3573,  3577,  3584,  3585,
    3589,  3590,  3594,  3595,  3596,  3597,  3598,  3599,  3600,  3601,
    3606,  3611,  3615,  3619,  3623,  3631,  3636,  3645,  3646,  3647,
    3648,  3649,  3650,  3651,  3652,  3653,  3654,  3655,  3656,  3657,
    3658,  3659,  3660,  3661,  3662,  3663,  3664,  3665,  3666,  3667,
    3671,  3672,  3673,  3674,  3675,  3676,  3679,  3683,  3687,  3691,
    3695,  3702,  3706,  3710,  3714,  3722,  3723,  3724,  3725,  3726,
    3727,  3728
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

#define YYPACT_NINF (-1067)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-732)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1067,   106,  3822, -1067,   -65,    60, -1067, -1067, -1067, -1067,
   -1067, -1067, -1067, 11128,    15,   159,   158, 13542,   210, 18363,
      15, 11128,   240,   252,   221,   159,  5320, 11128,   372,  5320,
     215, 18422, -1067,   458,   346,  8664,  9544,  9544, -1067,  8840,
     424,   318,   316, -1067,   464, 18422, 18422, 18422, -1067,  3143,
    9720,   474, 11128, 11128,   238, -1067,   493,   496, 11128, -1067,
   13542, -1067, 11128,   549,   418,    63,   402, 12887,   543, 18481,
   -1067, 11128,  8312, 11128,  9720, 13542, 11128,   505,   558,   450,
    5320,   572, 11128,   580,  7256,  7256, -1067,   584, -1067, 13542,
     637, -1067,   590,  8840,  9016, -1067, 11128, -1067, 11128, -1067,
   -1067, 13369, 11128, -1067, 11128, -1067, -1067, -1067,  4264,  7608,
    9192, 11128, -1067,  4968, -1067, -1067, -1067, -1067, -1067,   477,
   -1067,    43, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067,
   -1067, -1067, -1067, -1067, -1067,  7432, -1067,  9896, -1067, -1067,
   -1067, -1067, -1067,   593, -1067, -1067, -1067, -1067, -1067, -1067,
   -1067, -1067,   509, 18422, -1067, 18071,   479,   -15, -1067,   305,
   -1067, -1067, -1067,   230,   253, -1067, -1067, 18422,  3143, -1067,
     498,   497, -1067, -1067, -1067,   499,   500, 11128,   502,   503,
   -1067, -1067, -1067, 17597,  2111,   180,   511,   513, -1067, -1067,
     218, -1067, -1067, -1067, -1067, -1067,   442, -1067, -1067, -1067,
   11128, 11128, 11128, 18422, -1067, 11128, 11128, 10072,   615,   313,
   -1067, -1067, -1067, -1067, 17597,   321, -1067, -1067,   508,  5320,
   -1067, -1067,   518,   122,   504, 15065, -1067,   251, -1067,   520,
   -1067,   229, 17597, -1067,  2052,   567,  9016, -1067, -1067, -1067,
   -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067,
   -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067,
   -1067, -1067,   521, -1067, -1067, 18422,   527,    24, 14644,    33,
    1123,    33,  3240, 18422, 18422,   -30,  3588, -1067, -1067,   606,
    9016,   529,   356,  5320,  1637,   200,    22, -1067,  5320, -1067,
   -1067, 15150, -1067,    69, 15716,   434, -1067,   531,   532, -1067,
   15150,   122,   434, -1067,  9016,  1853,  1853,    56,    37, -1067,
      70, -1067, -1067, -1067, -1067, 11128, 11128, -1067,   553, -1067,
     533,   547, -1067, -1067,  2904,   559, -1067, -1067, 17597,   234,
   11128, 11128, -1067,   -46, -1067, -1067, 17597,   530, 15756, -1067,
   15150,   122, 17597,   536,  9016, -1067, 17597, 15802, -1067, -1067,
   -1067, -1067,   122, -1067,  2052,    24, 15150,   545,   204,   204,
    1357,   434,   434,    99, -1067, -1067,  4440,   -27, -1067, 11128,
     110,   114, -1067, 13684,   -23,     8, 15910,   -56,  1357,   705,
   -1067,  4616, 11128, -1067, 11128,   654, -1067, -1067, 14736,   224,
     509, 17597,   573, 18422, 11128,   551, -1067,   552,   717,   718,
     600,   101, -1067, -1067, -1067,    55,   230, -1067, -1067, -1067,
   -1067,   638, -1067, -1067, -1067,   148,   639, -1067, -1067, -1067,
   13198,   601,   239, -1067,   246,   367, -1067,   581, 11128, 11128,
   11128,  9544,  9544, 11128,   391, 11128, 11128, 11128, 11128, 11128,
     570, 13369, 11128, 11128, 11128, 11128, 11128, 11128, 11128, 11128,
   11128, 11128, 11128, 11128, 11128, 11128, 11128, 11128,   663, -1067,
   -1067, -1067, -1067, -1067,  9016,  9016, -1067, -1067, -1067, -1067,
    9016, -1067, -1067,  9016,  9016,  9016,  9016,  9544,  9544, 15226,
   15311, 15955,   560,    94, 18422,  7784, -1067, -1067,  4264, -1067,
    9544,    24,  3143, -1067, 11128, -1067,  2052, -1067, 18422, -1067,
   -1067, -1067,   606, 11128,   608,   563,   599, -1067, -1067, -1067,
     700, 11128, -1067,  5496,  9544, -1067,   582, -1067,    24,  5672,
    9544, -1067,    24, -1067,    24,  9544, -1067,    24,   627,   629,
    5320,   722,  5320, -1067,   723, 11128,   693,   586,  9016, 18422,
   -1067, -1067, -1067,  1637, -1067, -1067,   115, -1067, 11304,   634,
   11128,  3143, -1067, -1067, 11128, -1067, 18147, 11128, 11128, -1067,
     588, -1067, -1067, -1067, -1067, 18216, -1067,   230, 13062, -1067,
    1637, -1067,   616, 11304, -1067, 17597, 17597, -1067,   504, -1067,
      52, -1067,  9016,   596, -1067,   750,   750,  2052, -1067, -1067,
   -1067, -1067, -1067, -1067, -1067, 10248, -1067, 15995,  7960, -1067,
    8136, -1067, 17597,  5320,   598,  9544, 10424,  4088,   602, 11128,
   10600, -1067, -1067,   220, -1067,  4792, -1067,   327, 16071,   331,
   14828, 18422,  7080,  7080, -1067,   509,   605, -1067,   251, -1067,
     149,   624,  1692, -1067, -1067, 18422, -1067, 18422, 11128,   185,
   -1067, -1067,   162, -1067, -1067,   214, -1067, -1067,  3485, -1067,
     671, -1067,   607,   631, -1067,   632, -1067,   633,   642,   645,
     646, -1067,   647,   649,   650,   652,   656,   657,   659, -1067,
   -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067,
   -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067,
   -1067, 11128,   662,   660,   607, -1067,   607, -1067, 11128, -1067,
   18422, -1067, -1067,  3912,  3912,   396, -1067,   396, -1067,   396,
    3865,  1316,  1441,   644,   122,   204, -1067,   404, -1067, -1067,
   -1067, -1067, -1067,  1357,  1881,   396,   841,   841,  3912,   841,
     841,  1671,   204,  1881, 17874,  1671,   434,   434,   204,  1357,
     613,   623,   648,   653,   658,   661,   641,   640, -1067,   396,
   -1067,   396, 11480,  9544, 11656,  9544, 11128,  9016,  9544, 13684,
     655,   235, -1067, -1067, -1067,   175, -1067,   851, 17637,   457,
      15, 16115, -1067,   343, 17597, -1067, 16160,  9016, -1067,  9016,
   11128,   191,  8840, 17597,    67, 15387,  7784, -1067,  8840, 17597,
      35, 14904, -1067, -1067,    33, 14989, -1067, 11128, 11128,   783,
    5320,   785, 16269,  5320, 15472, 18422, -1067,   202, -1067,   216,
   -1067,   236,  1637,   200, -1067, 17597,  8488, -1067, 17597, -1067,
   -1067, -1067, 18422, 16313, 16352, -1067,   616,   675, -1067,   279,
   11304, -1067,   108, -1067, 11128, -1067,   651,   125,   665, -1067,
    2592, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067,
    5320,    10, 15548, -1067, -1067, 17597,  5320, 17597, -1067, 16433,
   -1067, -1067, 11128, -1067,    71, 13756, 11128, -1067, 10776,  7080,
    7080, -1067, 11128,   484, 11128,   517,   522, 11128,  9368,   523,
     344, -1067, -1067, -1067, 16516,   678,   666,   664, -1067, 18422,
    3143, -1067,  9016,   668, 17781, 18422, -1067, 18422, -1067, 17597,
   18304,   716,   591, -1067,   671, -1067, -1067, -1067, -1067, -1067,
      23,    87, -1067, 16596, -1067, 13916, -1067, -1067, 17597, -1067,
     667,   672, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067,
   11128,   800, 16677, 11128,   803, 16757,   299,   674, 16797,  9016,
   -1067, -1067,  7784, -1067, -1067, -1067, -1067, -1067, -1067, -1067,
   -1067, -1067, -1067, -1067, -1067,    85,  9544,  9544,    24,    24,
   -1067, -1067, -1067, -1067, 15150, -1067, 14000,  5848, -1067,  6024,
   -1067,   300, -1067, 14076,  6200, -1067,    24,  6376, -1067,    24,
   -1067,    24, -1067,    24, 17597, 17597,  5320, -1067,  5320, 11128,
   -1067,  5320,   802,   676,   677, 18422,   504, -1067, -1067,   634,
   -1067, -1067, -1067,   679,   687,   108, 11128,   671,   616, -1067,
   11128, -1067, -1067, 17597, -1067,  6552, 11128, -1067, -1067, -1067,
   18422, -1067,   697,   504, -1067,  6728,   686,  6904,   691, -1067,
   11128, -1067, -1067,  7080, -1067, 16837,    50, 15640,   351,   685,
    7080, -1067, 11128, -1067, -1067, 17804,   355,   357,   308, -1067,
     855, -1067, -1067,   861, -1067, 18018, -1067, -1067, -1067, -1067,
   -1067, -1067, -1067, -1067,    23,   141, 11128, -1067,   726,   696,
   -1067,   607,   607, -1067, -1067, 16917, 11832, 12008, 16997, 12184,
   12360, -1067, 12536, 12712,   310, -1067,   634,   311,   328, -1067,
   -1067, -1067,  5320,  8840, 17597,  8840, 17597,  7784, -1067,  5320,
    8840, 17597, -1067,  8840, 17597, -1067, -1067, -1067, -1067, -1067,
   17597,   840,  5320, -1067, -1067, -1067, -1067, -1067,  9016,  9016,
   -1067, -1067, -1067, -1067,   108, 17597,  5320, -1067,    82,   703,
   11128, -1067,  8840, -1067, 17597,  5320,  8840, -1067, 17597,  5320,
   17597,   102, 10952,  7080,  7080,  7080,  7080, -1067, -1067, -1067,
   17037, 17597, 17898, -1067, -1067, -1067,   712, -1067,  1328, -1067,
    1328, -1067, -1067, -1067, -1067,    72, -1067,    32, -1067,   412,
   17077,    19, -1067, -1067, -1067, 11128, 11128, 11128, 11128, 11128,
   11128, 11128, 11128, -1067, -1067, 16269, 14161, 14237, -1067, 16269,
   14322, 14398,  5320, -1067,   706,   707, -1067, -1067,   567, -1067,
     504, 17597, 14483, -1067, 14559, -1067, -1067, -1067, 17597,   360,
     719,   368,   720, 11128, -1067, -1067,  1328, -1067,  1328, -1067,
      45, -1067, 11128,   141,    72,    72,    72,    72,    72,    72,
     141,  5144, -1067, -1067, -1067, -1067, -1067, 17157, 17237, 17317,
   17397, 17477, 17557,  5320,  5320,  5320,  5320, -1067,   725,   727,
   -1067,  5320,  5320, -1067, -1067, -1067, -1067, 17597, -1067, -1067,
   -1067, 17597, -1067, -1067, -1067, -1067, -1067, -1067, -1067,   734,
   -1067, 11128, 11128, 11128, 11128, 11128, 11128, 16269, 16269, 16269,
   16269, -1067, -1067, 16269, 16269
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   115,   459,   674,   675,   676,
     670,   671,   677,     0,   586,   106,   136,   554,   143,   556,
     586,     0,   142,   489,   487,   106,     0,     0,    47,     0,
     266,   137,   184,   183,   668,     0,     0,     0,   182,     0,
     141,    59,   267,   338,   138,     0,     0,     0,   333,     0,
       0,   145,     0,     0,   603,   578,   678,   146,     0,   339,
     548,   484,     0,     0,     0,   169,   167,   426,   140,   557,
     486,     0,     0,     0,     0,   552,     0,     0,   144,     0,
       0,   116,     0,   669,     0,     0,   477,   139,   304,   550,
      53,   488,   147,     0,     0,   727,     0,   729,     0,   730,
     731,   619,     0,   728,   725,   627,   165,   726,     0,     0,
       0,     0,     4,     0,     5,     9,    10,    44,    11,     0,
      49,    62,    12,    68,    13,    14,    15,    16,    28,   534,
     535,    22,    50,   166,   176,     0,   185,   641,   177,    17,
      30,    29,    19,     0,   261,    18,   637,    21,    34,    31,
      32,   175,   305,     0,   173,     0,     0,   634,   337,     0,
     631,   171,   354,   446,   437,   629,   174,     0,     0,   172,
     646,   625,   624,   628,   538,   536,     0,     0,   635,   636,
     640,   639,   638,     0,   537,     0,   647,   648,   649,   672,
     673,   630,   540,   539,   632,   633,     0,    27,   556,   137,
       0,     0,     0,     0,   557,     0,     0,     0,     0,   634,
     646,   536,   635,   636,   546,   537,   647,   648,     0,     0,
     587,   107,     0,   555,     0,   586,   478,     0,   485,     0,
      20,     0,   522,    45,   336,     0,   528,   113,   117,   129,
     123,   122,   131,   110,   121,   132,   118,   133,   108,   134,
     127,   120,   128,   126,   124,   125,   109,   111,   119,   130,
     135,   114,     0,   112,   193,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    61,    60,    62,
     528,     0,     0,     0,     0,     0,     0,   492,     0,   153,
      39,     0,   296,     0,   295,   712,   607,   604,   605,   606,
       0,   549,   713,     7,   528,   336,   336,   433,   150,   428,
     151,   429,   430,   434,   435,   149,   148,   436,   426,   518,
       0,   341,   342,   344,     0,   427,   517,   346,   505,     0,
       0,     0,   154,   631,   622,   645,   623,     0,     0,    42,
       0,   553,   541,     0,   528,    43,   547,     0,   274,   278,
     275,   278,   551,    51,   336,     0,     0,     0,   714,   716,
     617,   711,   710,     0,    65,    69,     0,     0,   524,     0,
       0,     0,   525,   570,     0,     0,   522,     0,   618,     0,
       6,     0,     0,    63,     0,     0,   489,   181,     0,   670,
     305,   642,   189,     0,     0,     0,   301,     0,   325,   329,
     331,     0,   315,   319,   322,   426,   446,   444,   445,   443,
     362,   447,   450,   449,   451,     0,   441,   438,   439,   442,
       0,   482,     0,   479,     0,   626,    33,   608,     0,     0,
       0,     0,     0,     0,   715,     0,     0,     0,     0,     0,
       0,   616,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   393,   400,
     401,   402,   397,   399,     0,     0,   395,   398,   396,   394,
       0,   404,   403,     0,     0,   528,   528,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,    23,     0,    36,
       0,     0,     0,    24,     0,    37,   336,   302,     0,   183,
     303,    48,    62,     0,   534,     0,   529,   530,   533,   314,
       0,     0,   203,     0,     0,   202,     0,   211,     0,     0,
       0,   209,     0,   219,     0,     0,   217,     0,     0,     0,
       0,   235,     0,   393,   231,     0,     0,     0,     0,     0,
     245,    25,   496,     0,   497,   499,     0,   516,     0,   502,
       0,     0,   152,    38,     0,   298,     0,     0,     0,    40,
       0,   170,   168,   431,   432,     0,   427,   446,   426,   419,
       0,   418,   520,     0,    99,   643,   644,   352,     0,   155,
       0,    41,     0,     0,   285,   276,   277,   336,    54,   196,
     195,    26,    67,    66,    70,     0,   679,     0,     0,   664,
       0,   666,   571,     0,     0,     0,     0,     0,     0,     0,
       0,   683,     8,     0,    56,     0,    97,     0,    93,     0,
      77,     0,     0,    83,   178,   305,     0,   191,     0,   188,
     272,   306,     0,   313,   323,     0,   327,     0,     0,   317,
     312,   411,     0,   413,   417,     0,   448,   510,     0,   512,
     452,   440,   409,   113,   389,   117,   387,   123,   122,   121,
     118,   391,   133,   134,   120,   124,   109,   119,   135,   386,
     368,   371,   369,   370,   392,   381,   372,   385,   377,   375,
     388,   376,   374,   379,   384,   373,   378,   382,   383,   380,
     390,     0,   365,     0,   409,   366,   409,   363,     0,   481,
       0,   476,   491,   707,   706,   709,   718,   717,   722,   721,
     703,   700,   701,   702,   620,   690,   115,     0,   660,   661,
     116,   659,   658,   614,   694,   705,   699,   697,   708,   698,
     696,   688,   693,   695,   704,   687,   691,   692,   689,   615,
       0,     0,     0,     0,     0,     0,     0,     0,   720,   719,
     724,   723,     0,     0,     0,     0,     0,     0,     0,   681,
     272,   597,   598,   600,   602,     0,   589,     0,     0,     0,
     586,   586,   199,     0,   523,    46,     0,     0,   543,     0,
       0,     0,     0,   560,     0,     0,     0,   212,     0,   566,
       0,     0,   210,   220,     0,     0,   218,     0,     0,   234,
       0,   230,     0,     0,     0,     0,   542,     0,   248,     0,
     246,     0,   500,     0,   515,   514,     0,   494,   613,   493,
     297,   294,     0,     0,     0,   657,   520,   347,   343,     0,
       0,   521,   502,   345,     0,   351,     0,     0,     0,   544,
       0,   279,    52,   680,   621,   665,   526,   667,   527,   227,
       0,     0,     0,   682,   225,   572,     0,   685,   684,     0,
      58,    57,     0,    92,     0,     0,     0,    85,     0,     0,
      83,    55,   386,   368,   371,   369,   370,   379,   378,   380,
       0,   405,   406,    72,    71,    84,     0,     0,   307,     0,
       0,   269,     0,     0,   336,     0,   326,     0,   330,   332,
       0,     0,   426,   412,   452,   508,   507,   506,   509,   453,
     460,   426,   361,     0,   367,     0,   357,   358,   483,   480,
       0,     0,   105,   103,   104,   102,   101,   100,   655,   656,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     599,   601,     0,   588,   136,   143,   142,   141,   138,   145,
     146,   140,   144,   139,   147,     0,     0,     0,     0,     0,
     490,   194,   532,   531,     0,   229,     0,     0,   201,     0,
     200,     0,   592,     0,     0,   207,     0,     0,   205,     0,
     215,     0,   213,     0,   243,   242,     0,   237,     0,     0,
     233,     0,   239,     0,   271,     0,     0,   498,   501,   502,
     503,   504,   293,     0,     0,   502,     0,   452,   520,   519,
     407,   422,   420,   353,   287,     0,     0,   286,   289,   545,
       0,   280,   283,     0,   228,     0,     0,     0,     0,   226,
       0,    98,    95,     0,    94,    80,    79,    78,     0,     0,
       0,   179,     0,   180,   307,   336,     0,     0,     0,   300,
     159,   169,   167,   163,   299,   336,   308,   309,   156,   324,
     328,   318,   321,   414,   460,     0,     0,   355,   416,     0,
     364,   409,   409,   662,   663,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,   590,   502,   646,   646,   198,
     197,   204,     0,     0,   559,     0,   558,     0,   591,     0,
       0,   565,   208,     0,   564,   206,   216,   214,   236,   232,
     577,   238,     0,    64,   268,   247,   244,   495,   528,   528,
     423,   421,   348,   349,   502,   408,     0,   291,     0,     0,
       0,   281,     0,   223,   574,     0,     0,   221,   573,     0,
     686,     0,     0,     0,    83,     0,    83,    86,    89,    75,
      74,    73,   336,   186,   192,   190,   273,   157,   336,   161,
     336,   310,   311,   334,   474,     0,   475,   462,   465,     0,
     461,     0,   410,   359,   360,     0,     0,     0,     0,     0,
       0,     0,     0,   273,   594,   563,     0,     0,   593,   569,
       0,     0,     0,   241,     0,     0,   425,   424,   292,   290,
       0,   284,     0,   224,     0,   222,    96,    82,    81,     0,
       0,     0,     0,     0,   187,   270,   336,   160,   336,   164,
       0,   473,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   454,   456,   458,   356,   455,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   240,   609,   610,
     282,     0,     0,    88,    91,    87,    90,    76,   158,   162,
     335,   464,   466,   467,   470,   471,   472,   468,   469,   463,
     457,     0,     0,     0,     0,     0,     0,   562,   561,   568,
     567,   611,   612,   576,   575
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1067, -1067, -1067,     4,   -64,  2345,   167, -1067, -1067, -1067,
    -204, -1067, -1067, -1067, -1067, -1067,   -26,   609, -1067,  -569,
    2485,   510,  -587, -1067,  -845, -1067, -1067,    31, -1067, -1067,
   -1067,   859, -1067,  3141,  -171, -1067, -1067,  -471,  2824, -1066,
    -161, -1067, -1067,  -160, -1067, -1067,  -873, -1067, -1067,   315,
   -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067, -1067,   183,
   -1067,   805, -1067, -1067,   -99,  2185, -1067, -1067, -1067, -1067,
     548, -1067,  -119, -1067, -1067, -1067, -1067,   349, -1067, -1067,
   -1067,  -108, -1067,  -349,  -816, -1067, -1067, -1067, -1067, -1067,
       6, -1067, -1067,     7, -1067, -1067,  -589, -1067,   546, -1067,
   -1067, -1067, -1067, -1067,   339, -1067,   -67, -1067, -1067, -1067,
   -1067,   519, -1067, -1067, -1067, -1067,    -6,  -404,  -165,  -814,
    -897,  -658, -1067,    -1, -1067,    11,   350,   -55,   603,   -54,
   -1067, -1067,  -356,  -840, -1067,  -304, -1067,  -146,  -311,  -300,
    -510, -1067, -1067,    36,   228, -1067,  -183,   789, -1067,  -167,
     379,   120,  -430,  -800,  -631, -1067, -1067, -1067,  -617,  -540,
   -1067,  -783,   -16,     0, -1067,  -273,  -518,  -548,   370, -1067,
   -1067, -1067, -1067,  1028, -1067,   -18, -1067, -1067,  -153, -1067,
    -768, -1067, -1067,  1098,  1336, -1067, -1067, -1067, -1067,   -13,
   -1067,    89,  1406, -1067,  1827,  1888, -1067, -1067, -1067, -1067,
   -1067, -1067, -1067, -1067, -1067,  -475
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
     420,   410,   415,   910,  1235,  1236,   165,  1067,  1167,  1168,
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
     214,   424,   224,   318,   223,   333,   112,   537,   225,   835,
     769,   231,   323,   325,   232,   279,   695,   907,   972,   474,
     807,  1058,   268,   270,   272,  1039,   276,   333,   904,   390,
     501,   560,  1012,   833,   838,   880,   916,   291,   917,   294,
     295,   626,   333,  1005,   492,   300,   896,   301,   898,   302,
     645,  1034,  1231,   836,   328,   329,   605,   288,   232,   336,
     338,   340,   341,   342,  1064,   504,   288,   346,   974,   347,
     550,   583,  1045,   530,   716,  1164,   352,   814,  1231,   577,
     356,   336,   307,   358,  1065,   359,   547,  1143,   360,   361,
     382,   362,   610,  -266,   375,   232,   336,   376,   378,   516,
     967,   197,   288,   309,   563,  1234,     3,   311,  1120,   504,
     -61,   535,    72,   811,   307,   288,  -249,   522,   524,   527,
     611,   595,   388,  1144,   391,  -350,  -267,  -288,   383,   -61,
     516,   312,   516,   504,   313,   309,    63,   218,    72,   311,
     829,   488,   758,  1066,  1164,   596,   314,   564,   -61,   647,
     588,  -583,  1222,   603,  1234,   516,   606,   516,  -653,   384,
    -653,   337,   221,   312,   427,   317,   313,  1123,    63,   333,
     551,  -288,  1058,   504,  1085,   720,  1166,   906,   314,   -61,
    1223,  -511,  1058,   357,   607,  1232,  1025,   479,   480,   481,
     488,   483,   346,   232,   376,   488,   999,   317,   372,  1117,
    1015,   547,   746,   747,   488,  1121,   488,   491,   502,  -511,
     236,   827,  -513,   333,  -250,   547,   488,   554,   267,   269,
     271,   604,   608,   336,  -288,  1124,  1149,  1196,  1152,   641,
     494,   963,   548,  -513,  1165,   816,   430,   333,   488,   936,
     488,  -511,   606,   280,  1016,  1166,   497,  -513,  -513,   639,
    1040,   348,   350,   488,  -511,   696,   296,   411,   598,  -513,
    1010,  -415,   600,   812,   434,   197,  -256,   336,  -511,   722,
     759,   592,   440,   640,  1206,  -513,   887,   333,    24,  1058,
     416,   625,  1038,   940,   599,  1217,  1184,  1219,   601,   813,
    1009,   336,   775,   374,   431,   648,  -255,  1017,   432,  1210,
    -320,  1212,   342,   346,   504,   504,  1059,  -513,  1060,   412,
     902,   769,   228,   500,  -511,   891,   297,   575,   576,  -511,
    -513,   413,   892,   942,  1197,   773,   941,  -513,   502,  1188,
      61,   336,   417,  -316,  -513,   298,   903,    88,  1086,   494,
     414,   318,    70,  1258,   418,  1259,   434,   548,  -513,   943,
     779,   438,   299,   475,   440,   476,   597,  -316,   443,   407,
     602,   548,   226,   419,   995,   965,  -513,   504,    91,   618,
     594,   620,   170,   822,  1048,  1122,   994,   494,   631,   408,
     233,   232,   494,   842,   812,  -513,   197,   700,   236,   409,
     996,  -654,   860,  -654,   551,   495,   170,   333,   333,   170,
     574,   277,  -264,   333,   278,   701,   333,   333,   333,   333,
     997,   504,   702,  1173,  1174,   703,   704,   705,   707,   709,
     710,  1084,   711,   712,   713,   714,   715,   812,   723,   724,
     725,   726,   727,   728,   729,   730,   731,   732,   733,   734,
     735,   736,   737,   738,   739,   890,  1141,   779,  1097,   -60,
     170,   336,   336,  1008,   170,   170,   779,   336,   779,  -596,
     336,   336,   336,   336,   749,   751,   430,   769,   -60,   264,
     502,   333,   768,  1081,  1098,   862,  -595,   771,   170,   866,
    -254,   774,  1156,   170,  1183,  -596,  -653,   -60,  -653,   280,
     776,   551,  1040,   863,  -650,   781,  -650,   867,   232,  1040,
     783,   785,  -595,   700,   318,   551,   789,   791,  1040,   960,
    1041,   695,   795,   323,   325,   333,  1040,  1147,   -60,   802,
    -251,  1154,   804,  1155,   431,   336,  1253,   488,   432,   539,
    -258,   333,   477,   333,  1255,   815,   478,   818,   434,  1224,
    -652,   294,  -652,   438,   823,   824,   440,   957,   157,  -262,
     443,   478,  -259,   741,   742,   328,  1209,   303,  1211,   743,
     328,   502,   744,   745,  -651,  1225,  -651,  1226,  1227,   336,
    1228,  1229,   157,   716,  -729,   157,   434,   920,  -729,   921,
     307,   438,   774,   439,   440,   336,   504,   336,   443,   170,
     717,   304,   852,   774,   855,   450,   857,   859,   846,  -253,
     848,   309,  1233,   456,   718,   311,   343,  -730,   504,   884,
     884,  -730,  -731,  -726,  -257,   708,  -731,  -726,   307,   594,
     561,   562,   769,   344,    43,   899,   157,  -336,  -263,   312,
     157,   157,   313,   719,   379,   328,  -265,   976,   979,   309,
    -252,   981,   983,   311,   314,   353,  -260,  -336,   381,   393,
      59,  1233,   405,   170,   157,  1221,   394,  -336,   170,   157,
     748,   750,   425,   317,  -580,  -579,   426,   312,  -584,  -585,
     313,   484,    63,   770,   720,   488,   430,  -582,   913,  -581,
     721,   485,   314,   627,   487,   918,   493,   372,   503,   372,
     333,   383,   509,  1032,   722,   568,   579,   784,  1026,  1028,
     511,   317,   538,   790,   557,   558,   573,   567,   794,   582,
     333,   591,   333,   612,  1263,  1264,  1265,  1266,  1267,  1268,
     621,   504,   632,  1047,   633,   634,   636,   638,   698,   646,
     651,   440,   740,   757,   431,   777,   170,   778,   432,   783,
     932,   789,   935,   802,   336,   938,   855,   779,   780,   333,
    1072,   170,   958,   959,   797,   786,   798,   800,   803,   805,
     806,   816,   825,   830,   336,   157,   336,   964,   504,   966,
     839,   840,   494,   768,   850,   973,   888,   909,   856,   922,
     911,  -115,  -136,  -143,   984,   985,   434,   435,   851,   923,
     697,   438,  -142,   439,   440,  -141,  -138,  -145,   443,  -146,
    -140,   535,  -144,   336,   914,   450,  -116,  -139,   535,  -147,
     915,   454,   455,   456,   924,   928,   929,   328,   986,   925,
     988,  1013,  1006,  1014,   926,   333,  1040,   927,   939,   157,
    1055,   399,  1043,  1076,   157,  1044,  1079,  1112,   318,  1019,
    1049,  1073,  1113,  1114,  1130,  1194,  1195,   318,  1074,   618,
    1082,  1148,  1118,  1035,   716,  1037,   884,   884,   170,   358,
    1119,   359,  1135,  1157,   361,   362,   962,  1139,   944,  1159,
    1172,   945,   333,   430,   902,  1192,   946,  1200,  1215,   336,
    1248,  1249,  1223,   170,   229,  1254,  1256,   199,   536,   170,
    1281,   615,  1282,  1031,  1161,  1162,  1115,   947,   355,   586,
     170,  1129,   170,   820,   948,  1001,  1061,   828,  1062,  1071,
    1068,   629,   157,  1063,   949,   826,  1260,  1075,  1163,  1269,
    1078,   565,   950,  1262,   387,  1046,   336,   157,   919,   768,
     819,   431,   998,     0,     0,   432,   931,     0,   934,   951,
       0,   937,     0,   707,   749,     0,     0,   504,   504,     0,
       0,   952,     0,     0,  1094,   720,  1096,     0,     0,     0,
       0,  1101,   953,     0,  1104,     0,     0,     0,     0,   954,
       0,     0,     0,   170,     0,     0,  1110,   170,     0,     0,
       0,  1055,     0,   434,   435,   170,   436,   437,   438,     0,
     439,   440,   441,   328,     0,   443,     0,  1125,     0,     0,
    1128,   449,   450,   232,     0,   453,     0,     0,   454,   455,
     456,     0,  1134,     0,  1138,     0,     0,  1140,     0,   457,
     884,   535,   535,     0,     0,   535,   535,  1150,     0,  1151,
     175,     0,     0,     0,   157,     0,     0,   535,     0,   535,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   333,   333,  1170,   175,     0,     0,   175,     0,   157,
       0,     0,     0,  1094,  1096,   157,  1101,  1104,     0,  1134,
    1138,     0,     0,     0,     0,     0,   157,     0,   157,  1185,
    1186,     0,  1187,     0,   768,     0,  1189,  1190,  1055,     0,
    1191,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     178,     0,     0,     0,     0,   336,   336,     0,   175,     0,
       0,     0,   175,   175,     0,     0,     0,  1201,     0,  1202,
       0,     0,     0,  1204,   178,     0,     0,   178,     0,  1208,
     884,   884,   884,   884,     0,   428,   175,   955,     0,     0,
     748,   175,   429,     0,     0,     0,     0,     0,     0,   157,
       0,     0,     0,   157,     0,   430,   519,     0,     0,     0,
       0,   157,  1185,  1237,  1238,  1189,  1239,  1240,  1241,  1242,
     170,   520,     0,   170,     0,     0,     0,     0,   178,     0,
       0,     0,   178,   178,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1257,     0,     0,     0,     0,     0,   178,     0,     0,  1261,
       0,   178,     0,   431,     0,     0,     0,   432,     0,     0,
     170,     0,     0,     0,     0,     0,   170,     0,     0,     0,
    1277,  1278,  1279,  1280,     0,  1270,     0,     0,  1283,  1284,
       0,     0,     0,     0,     0,   516,     0,   175,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,  1277,  1278,
    1279,  1280,  1283,  1284,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,     0,
     454,   455,   456,     0,   488,     0,     0,     0,     0,     0,
       0,   457,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,     0,     0,     0,     0,   175,   178,     0,     0,
       0,     0,     0,     0,     0,     0,  1087,  1088,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   170,   179,   170,
       0,     0,     0,     0,   170,     0,   157,   170,   430,   157,
     497,     0,     0,     0,    23,    24,   170,     0,   170,     0,
       0,   170,   179,     0,   498,   179,    32,   499,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     0,     0,    43,
       0,   178,     0,     0,     0,   170,   178,     0,     0,   430,
       0,     0,     0,     0,   175,   170,   157,   170,     0,     0,
       0,     0,   157,     0,     0,    59,   431,    61,   184,   175,
     432,  1051,     0,     0,  1052,     0,   179,   500,     0,    70,
     179,   179,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   184,     0,     0,   184,     0,     0,    86,     0,
       0,    88,     0,     0,   179,    91,     0,   431,     0,   179,
       0,   432,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   170,   438,   178,   439,   440,     0,     0,   170,
     443,     0,     0,   430,     0,     0,     0,   450,     0,   178,
       0,     0,   170,   454,   455,   456,   184,     0,     0,     0,
     184,   184,     0,     0,   106,     0,   170,     0,     0,   434,
     435,     0,   436,   437,   438,   170,   439,   440,     0,   170,
       0,   443,     0,   157,   184,   157,   175,   449,   450,   184,
     157,   453,     0,   157,   454,   455,   456,     0,     0,     0,
       0,   431,   157,     0,   157,   432,     0,   157,     0,     0,
       0,   175,     0,     0,     0,     0,     0,   175,     0,     0,
       0,     0,     0,     0,     0,   179,     0,     0,   175,     0,
     175,   157,   170,     0,     0,     0,     0,     0,     0,     0,
       0,   157,     0,   157,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,   435,     0,   178,   437,   438,     0,
     439,   440,     0,     0,     0,   443,     0,     0,     0,     0,
       0,   170,   450,     0,     0,     0,     0,     0,   454,   455,
     456,   178,     0,   170,   170,   170,   170,   178,     0,   179,
       0,   170,   170,     0,   179,   184,     0,     0,   178,     0,
     178,   175,     0,     0,     0,   175,     0,     0,   157,     0,
     237,     0,     0,   175,     0,   157,     0,     0,     0,     0,
       0,     0,     0,     0,   238,   239,     0,   240,   157,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,   242,   157,     0,     0,     0,     0,   243,     0,     0,
       0,   157,     0,   244,     0,   157,     0,   245,     0,   184,
     246,     0,     0,   893,   184,     0,     0,     0,     0,     0,
     247,   178,   179,   430,     0,   178,     0,   248,   249,     0,
       0,     0,     0,   178,  -307,   250,     0,   179,  -307,  -307,
       0,     0,     0,  -307,     0,   251,     0,     0,  -307,     0,
    -307,  -307,     0,     0,   252,   253,  -307,   254,   157,   255,
       0,   256,     0,  -307,   257,     0,  -307,     0,   258,   542,
       0,   259,     0,     0,     0,   260,     0,     0,     0,     0,
       0,   431,     0,     0,     0,   432,  -307,     0,     0,  -307,
       0,  -307,   184,  -307,     0,  -307,  -307,   157,  -307,     0,
       0,  -307,     0,  -307,     0,     0,     0,   184,     0,   157,
     157,   157,   157,     0,     0,     0,     0,   157,   157,     0,
       0,     0,  -307,     0,     0,  -307,     0,  -307,     0,  -307,
     543,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,     0,     0,   179,   443,     0,     0,   175,   186,
       0,   175,   450,     0,     0,     0,     0,     0,   454,   455,
     456,     0,     0,     0,     0,     0,     0,     0,     0,   179,
       0,     0,     0,   186,     0,   179,   186,     0,  -307,     0,
       0,     0,     0,     0,  -307,     0,   179,     0,   179,     0,
       0,     0,     0,     0,     0,   497,     0,     0,   175,    23,
      24,     0,     0,     0,   175,     0,     0,     0,     0,   498,
     187,    32,   499,     0,   184,     0,     0,    38,   178,     0,
       0,   178,     0,     0,    43,     0,     0,   186,     0,     0,
       0,   186,   186,   430,   187,     0,     0,   187,     0,   184,
       0,     0,     0,     0,     0,   184,     0,     0,     0,     0,
      59,     0,    61,     0,     0,   186,   184,     0,   184,   179,
     186,     0,   500,   179,    70,     0,     0,     0,   178,     0,
       0,   179,     0,     0,   178,     0,     0,     0,     0,     0,
       0,     0,     0,    86,     0,     0,    88,     0,   187,     0,
      91,   431,   187,   187,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,   187,   175,     0,     0,
       0,   187,   175,     0,     0,   175,     0,     0,     0,   184,
       0,     0,     0,   184,   175,     0,   175,     0,     0,   175,
       0,   184,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,     0,   443,   444,   445,     0,   447,
     448,   449,   450,   175,     0,   453,   186,     0,   454,   455,
     456,     0,     0,   175,     0,   175,     0,     0,     0,   457,
       0,     0,     0,     0,     0,   178,     0,   178,     0,     0,
       0,     0,   178,     0,   497,   178,     0,     0,    23,    24,
       0,     0,     0,     0,   178,     0,   178,     0,   498,   178,
      32,   499,     0,     0,     0,     0,    38,     0,     0,     0,
       0,     0,     0,    43,     0,     0,     0,   187,     0,     0,
     186,     0,     0,   178,     0,   186,     0,     0,   -59,     0,
     175,     0,     0,   178,     0,   178,     0,   175,     0,    59,
       0,    61,     0,     0,     0,    65,   179,   -59,    66,   179,
     175,   500,     0,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   175,     0,     0,     0,     0,     0,
       0,     0,    86,   175,     0,    88,     0,   175,     0,    91,
       0,   187,     0,     0,     0,     0,   187,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   179,   143,     0,     0,
     178,     0,   179,   186,     0,     0,     0,   178,     0,     0,
       0,     0,     0,     0,     0,     0,   184,     0,   186,   184,
     178,   143,     0,     0,   143,     0,     0,     0,   106,     0,
     175,     0,     0,     0,   178,     0,     0,     0,     0,     0,
       0,     0,     0,   178,     0,     0,     0,   178,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,     0,   187,     0,   184,     0,     0,   175,
       0,     0,   184,     0,     0,   143,     0,     0,     0,   187,
       0,   175,   175,   175,   175,     0,     0,     0,     0,   175,
     175,   473,     0,     0,  -650,     0,  -650,     0,     0,     0,
     178,     0,     0,   143,     0,     0,     0,     0,   143,     0,
       0,     0,     0,   179,     0,   179,     0,     0,     0,     0,
     179,     0,     0,   179,     0,   186,     0,     0,     0,     0,
       0,     0,   179,     0,   179,     0,     0,   179,     0,   178,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     186,   178,   178,   178,   178,     0,   186,   114,     0,   178,
     178,   179,     0,     0,     0,     0,     0,   186,     0,   186,
       0,   179,     0,   179,     0,     0,     0,     0,     0,     0,
       0,   230,     0,   184,   235,   184,   187,     0,     0,     0,
     184,     0,     0,   184,     0,     0,     0,     0,     0,     0,
       0,     0,   184,     0,   184,     0,     0,   184,     0,     0,
       0,   187,     0,     0,   143,     0,     0,   187,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
     187,   184,     0,     0,     0,   345,     0,     0,   179,     0,
     186,   184,     0,   184,   186,   179,     0,     0,     0,     0,
       0,     0,   186,     0,     0,     0,     0,     0,   179,     0,
       0,     0,     0,   114,     0,     0,     0,     0,   380,     0,
       0,     0,   179,     0,     0,     0,     0,     0,   143,     0,
       0,   179,     0,   143,     0,   179,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   187,     0,     0,     0,   187,     0,     0,   184,     0,
       0,     0,     0,   187,     0,   184,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   179,     0,
       0,     0,   184,     0,     0,   289,     0,     0,     0,     0,
       0,   184,     0,     0,     0,   184,     0,     0,     0,     0,
       0,   143,     0,     0,     0,     0,     0,     0,     0,   289,
       0,     0,     0,     0,   486,     0,   143,   179,     0,   349,
     351,     0,     0,     0,     0,     0,     0,     0,     0,   179,
     179,   179,   179,     0,     0,     0,     0,   179,   179,     0,
       0,     0,     0,     0,     0,   237,     0,     0,   184,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   238,
     239,     0,   240,     0,     0,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,   242,   186,   541,     0,
     186,     0,   243,   552,     0,     0,     0,   184,   244,     0,
       0,     0,   245,     0,     0,   246,     0,     0,     0,   184,
     184,   184,   184,     0,     0,   247,     0,   184,   184,     0,
       0,     0,   248,   249,     0,     0,     0,     0,     0,     0,
     250,     0,     0,   143,     0,     0,     0,   186,     0,     0,
     251,     0,     0,   186,     0,     0,     0,     0,   187,   252,
     253,   187,   254,     0,   255,     0,   256,     0,   143,   257,
       0,     0,     0,   258,   143,     0,   259,     0,     0,   489,
     260,   114,     0,     0,     0,   143,     0,   143,     0,     0,
       0,     0,     0,     0,     0,     0,   114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   187,     0,
       0,     0,     0,     0,   187,     0,     0,     0,     0,     0,
       0,     0,   289,   289,   289,   289,   289,   289,     0,     0,
     531,   534,     0,   488,     0,  1020,     0,   540,     0,     0,
       0,     0,     0,     0,     0,     0,   289,     0,   289,     0,
       0,     0,     0,     0,     0,   289,     0,     0,   143,     0,
       0,     0,   143,     0,   186,     0,   186,     0,     0,     0,
     143,   186,     0,     0,   186,     0,     0,     0,     0,     0,
       0,     0,     0,   186,     0,   186,     0,     0,   186,     0,
       0,     0,     0,     0,     0,   289,     0,     0,     0,     0,
       0,     0,     0,   114,     0,     0,     0,     0,     0,     0,
     289,   289,   186,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   186,     0,   186,   187,     0,   187,   552,     0,
       0,     0,   187,     0,   552,   187,     0,     0,     0,     0,
       0,     0,     0,     0,   187,   799,   187,     0,     0,   187,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   339,     0,
       0,     0,     0,   187,     0,     0,     0,   237,     0,     0,
       0,     0,     0,   187,     0,   187,     0,     0,     0,   186,
       0,   238,   239,     0,   240,     0,   186,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,     0,   242,   186,
       0,     0,     0,     0,   243,     0,     0,     0,   849,     0,
     244,     0,   854,   186,   245,     0,     0,   246,     0,     0,
     114,     0,   186,     0,     0,     0,   186,   247,     0,     0,
       0,     0,     0,     0,   248,   249,   289,     0,     0,     0,
     187,     0,   250,     0,     0,   143,     0,   187,   143,     0,
       0,     0,   251,     0,     0,     0,     0,     0,     0,     0,
     187,   252,   253,   289,   254,     0,   255,   289,   256,   289,
       0,   257,   289,     0,   187,   258,   569,     0,   259,   186,
       0,     0,   260,   187,     0,     0,     0,   187,     0,     0,
       0,     0,     0,     0,     0,   143,     0,     0,     0,     0,
       0,   143,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   186,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     186,   186,   186,   186,     0,     0,     0,   570,   186,   186,
     187,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   512,   515,   517,   521,   523,   526,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   553,     0,   555,     0,   187,
       0,     0,     0,     0,   559,     0,     0,     0,     0,     0,
       0,   187,   187,   187,   187,     0,     0,     0,     0,   187,
     187,     0,     0,     0,     0,   987,   237,     0,   990,     0,
       0,     0,   143,     0,   143,     0,     0,     0,     0,   143,
     238,   239,   143,   240,   581,     0,     0,     0,   241,     0,
       0,   143,   262,   143,     0,     0,   143,   242,     0,   589,
     590,     0,     0,   243,     0,     0,   281,   282,   283,   244,
     285,     0,     0,   245,     0,  1024,   246,     0,     0,     0,
     143,  1029,     0,     0,     0,     0,   247,     0,     0,     0,
     143,     0,   143,   248,   249,     0,     0,     0,     0,     0,
       0,   250,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   251,     0,     0,     0,     0,     0,     0,     0,     0,
     252,   253,     0,   254,     0,   255,     0,   256,     0,     0,
     257,     0,   428,     0,   258,     0,     0,   259,     0,   429,
       0,   260,     0,     0,     0,     0,     0,     0,     0,   289,
     289,     0,   430,   288,     0,   289,   289,   143,     0,   289,
     289,     0,     0,     0,   143,     0,     0,     0,   525,   992,
       0,     0,     0,     0,   396,     0,     0,   143,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   421,   285,
       0,   143,   552,     0,   552,   772,   284,     0,     0,   552,
     143,     0,   552,     0,   143,  1021,     0,     0,     0,     0,
     431,  1108,     0,  1109,   432,     0,  1111,     0,     0,     0,
       0,     0,   787,     0,   482,     0,   792,     0,   793,     0,
       0,   796,     0,     0,     0,     0,     0,     0,     0,     0,
    1127,     0,   516,     0,     0,     0,   433,     0,     0,     0,
    1133,     0,  1137,     0,     0,     0,     0,   143,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,   510,   454,   455,   456,
       0,   488,     0,     0,   528,   529,   143,     0,   457,     0,
       0,     0,     0,     0,     0,   544,     0,     0,   143,   143,
     143,   143,     0,     0,     0,     0,   143,   143,     0,     0,
       0,     0,     0,   289,   289,     0,     0,     0,     0,   289,
       0,   534,     0,     0,     0,     0,     0,  1193,   534,     0,
       0,   289,     0,     0,   289,   571,   289,     0,   289,     0,
       0,  1198,     0,     0,     0,     0,     0,     0,     0,     0,
    1203,  1116,     0,     0,  1205,     0,   905,     0,     5,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,    16,    17,     0,    18,     0,   198,  1131,     0,
      22,     0,     0,     0,     0,     0,     0,     0,     0,   308,
       0,   199,     0,     0,     0,    34,   200,   201,     0,     0,
     202,    40,     0,     0,   630,   310,     0,  1247,    44,     0,
       0,   203,     0,     0,    48,    49,     0,     0,    51,     0,
      53,     0,    54,    55,     0,    56,    57,     0,     0,     0,
       0,   692,     0,    60,     0,    62,     0,    64,     0,     0,
       0,     0,    67,    68,   204,     0,   114,     0,     0,     0,
       0,     0,    75,   315,    77,    78,    79,   316,     0,    81,
       0,     0,    83,     0,     0,     0,    87,     0,     0,    89,
     428,     0,     0,    92,     0,     0,     0,   429,   968,   970,
       0,    95,     0,   289,   975,   978,     0,     0,   980,   982,
     430,     0,     0,     0,     0,   760,     0,    96,    97,    98,
      99,   100,     0,   285,     0,     0,   101,     0,     0,   262,
       0,     0,     0,     0,     0,   102,     0,     0,   103,   104,
     105,     0,     0,     0,   107,     0,   206,     0,   109,     0,
     207,     0,     0,   111,     0,     0,     0,     0,     0,     0,
       0,   534,   534,     0,     0,   534,   534,     0,   431,     0,
     808,     0,   432,     0,   544,  1250,     0,   534,     0,   534,
       0,   532,   285,     0,     0,     0,     0,   821,     0,     0,
       0,     0,     0,     0,     0,     0,   571,     0,     0,     0,
       0,   544,     0,     0,   433,   533,   459,   460,   461,   462,
     463,     0,     0,   466,   467,   468,   469,     0,   471,   472,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,     0,   488,
       0,     0,   871,     0,     0,     0,   457,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1089,  1090,     0,     0,     0,     0,  1091,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1102,     0,     0,  1105,     0,  1106,     0,  1107,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    -2,     4,     0,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,   421,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,     0,    29,    30,     0,    31,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,   -59,
       0,    41,    42,    43,     0,    44,  -336,     0,    45,    46,
      47,    48,    49,     0,    50,    51,    52,    53,   -59,    54,
      55,     0,    56,    57,    58,     0,  -336,   430,     0,    59,
      60,    61,    62,    63,    64,    65,  -336,   -59,    66,    67,
      68,    69,     0,    70,    71,    72,     0,    73,    74,    75,
      76,    77,    78,    79,    80,     0,    81,    82,     0,    83,
      84,    85,    86,    87,     0,    88,    89,    90,   -59,    91,
      92,     0,     0,    93,   430,    94,   993,     0,    95,     0,
       0,     0,  1199,   544,     0,   431,     0,     0,     0,   432,
       0,     0,     0,  1002,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,  1022,   102,     0,     0,   103,   104,   105,   106,     0,
       0,   107,     0,   108,     0,   109,     0,   110,     0,     0,
     111,     0,   431,     0,     0,     0,   432,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,     0,   447,   448,   449,   450,   451,     0,   453,
     421,   285,   454,   455,   456,     0,     0,     0,   433,     0,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,   692,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,     0,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,     0,     0,     0,     0,     0,     0,     0,     4,
     457,     5,     6,     7,     8,     9,    10,    11,    12,     0,
    -681,     0,    13,    14,    15,    16,    17,  -681,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
    -681,    29,    30,  -681,    31,     0,    32,    33,    34,    35,
      36,    37,    38,    39,    40,   -59,   808,    41,    42,    43,
       0,    44,  -336,     0,    45,    46,    47,    48,    49,     0,
      50,    51,    52,    53,   -59,    54,    55,     0,    56,    57,
      58,  1022,  -336,     0,     0,    59,    60,    61,    62,     0,
      64,    65,  -336,   -59,    66,    67,    68,    69,  -681,    70,
      71,    72,  -681,    73,    74,    75,    76,    77,    78,    79,
      80,     0,    81,    82,     0,    83,    84,    85,    86,    87,
       0,    88,    89,    90,   -59,    91,    92,     0,     0,    93,
       0,    94,     0,     0,  -681,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,  -681,    98,  -681,  -681,  -681,  -681,  -681,  -681,  -681,
       0,  -681,  -681,  -681,  -681,  -681,  -681,  -681,  -681,  -681,
    -681,  -681,  -681,   105,   106,  -681,  -681,  -681,     0,   108,
    -681,   109,     0,   110,     0,   363,  -681,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     0,    29,    30,     0,
      31,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,   -59,     0,    41,    42,    43,     0,    44,  -336,     0,
      45,    46,    47,    48,    49,     0,    50,    51,    52,    53,
     -59,    54,    55,     0,    56,    57,    58,     0,  -336,     0,
       0,    59,    60,    61,    62,    63,    64,    65,  -336,   -59,
      66,    67,    68,    69,     0,    70,    71,    72,     0,    73,
      74,    75,    76,    77,    78,    79,    80,     0,    81,    82,
       0,    83,    84,    85,    86,    87,     0,    88,    89,    90,
     -59,    91,    92,     0,     0,    93,     0,    94,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     100,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   103,   104,   105,
     106,     0,     0,   107,     0,   108,   364,   109,     0,   110,
       0,     4,   111,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,     0,    29,    30,     0,    31,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    40,   -59,     0,    41,
      42,    43,     0,    44,  -336,     0,    45,    46,    47,    48,
      49,     0,    50,    51,    52,    53,   -59,    54,    55,     0,
      56,    57,    58,     0,  -336,     0,     0,    59,    60,    61,
      62,    63,    64,    65,  -336,   -59,    66,    67,    68,    69,
       0,    70,    71,    72,     0,    73,    74,    75,    76,    77,
      78,    79,    80,     0,    81,    82,     0,    83,    84,    85,
      86,    87,     0,    88,    89,    90,   -59,    91,    92,     0,
       0,    93,     0,    94,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   103,   104,   105,   106,     0,     0,   107,
       0,   108,   593,   109,     0,   110,     0,   613,   111,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,     0,    29,
      30,     0,    31,     0,    32,    33,    34,    35,    36,    37,
      38,    39,    40,   -59,     0,    41,    42,    43,     0,    44,
    -336,     0,    45,    46,    47,    48,    49,     0,    50,    51,
      52,    53,   -59,    54,    55,     0,    56,    57,    58,     0,
    -336,     0,     0,    59,    60,    61,    62,    63,    64,    65,
    -336,   -59,    66,    67,    68,    69,     0,    70,    71,    72,
       0,    73,    74,    75,    76,    77,    78,    79,    80,     0,
      81,    82,     0,    83,    84,    85,    86,    87,     0,    88,
      89,    90,   -59,    91,    92,     0,     0,    93,     0,    94,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   103,
     104,   105,   106,     0,     0,   107,     0,   108,   614,   109,
       0,   110,     0,     4,   111,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,     0,    29,    30,     0,    31,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,   -59,
       0,    41,    42,    43,     0,    44,  -336,     0,    45,    46,
      47,    48,    49,     0,    50,    51,    52,    53,   -59,    54,
      55,     0,    56,    57,    58,     0,  -336,     0,     0,    59,
      60,    61,    62,    63,    64,    65,  -336,   -59,    66,    67,
      68,    69,     0,    70,    71,    72,     0,    73,    74,    75,
      76,    77,    78,    79,    80,     0,    81,    82,     0,    83,
      84,    85,    86,    87,     0,    88,    89,    90,   -59,    91,
      92,     0,     0,    93,     0,    94,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,   103,   104,   105,   106,     0,
       0,   107,     0,   108,   861,   109,     0,   110,     0,     4,
     111,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
       0,    29,    30,     0,    31,     0,    32,    33,    34,    35,
      36,    37,    38,    39,    40,   -59,     0,    41,    42,    43,
       0,    44,  -336,     0,    45,    46,    47,    48,    49,     0,
      50,    51,    52,    53,   -59,    54,    55,     0,    56,    57,
      58,     0,  -336,     0,     0,    59,    60,    61,    62,   379,
      64,    65,  -336,   -59,    66,    67,    68,    69,     0,    70,
      71,    72,     0,    73,    74,    75,    76,    77,    78,    79,
      80,     0,    81,    82,     0,    83,    84,    85,    86,    87,
       0,    88,    89,    90,   -59,    91,    92,     0,     0,    93,
       0,    94,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     0,     0,   102,     0,
       0,   103,   104,   105,   106,     0,     0,   107,     0,   108,
       0,   109,     0,   110,     0,     4,   111,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     0,    29,    30,     0,
      31,     0,    32,    33,    34,    35,    36,    37,    38,    39,
      40,   -59,     0,    41,    42,    43,     0,    44,  -336,     0,
      45,    46,    47,    48,    49,     0,    50,    51,    52,    53,
     -59,    54,    55,     0,    56,    57,    58,     0,  -336,     0,
       0,    59,    60,    61,    62,    63,    64,    65,  -336,   -59,
      66,    67,    68,    69,     0,    70,    71,    72,     0,    73,
      74,    75,    76,    77,    78,    79,    80,     0,    81,    82,
       0,    83,    84,    85,    86,    87,     0,    88,    89,    90,
     -59,    91,    92,     0,     0,    93,     0,    94,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     100,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   103,   104,   105,
     106,     0,     0,   107,     0,   108,     0,   109,     0,   110,
       0,     4,   111,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,     0,    29,    30,     0,    31,     0,    32,    33,
      34,    35,    36,    37,    38,    39,    40,   -59,     0,    41,
      42,    43,     0,    44,  -336,     0,    45,    46,    47,    48,
      49,     0,    50,    51,    52,    53,   -59,    54,    55,     0,
      56,    57,    58,     0,  -336,     0,     0,    59,    60,    61,
      62,     0,    64,    65,  -336,   -59,    66,    67,    68,    69,
       0,    70,    71,    72,     0,    73,    74,    75,    76,    77,
      78,    79,    80,     0,    81,    82,     0,    83,    84,    85,
      86,    87,     0,    88,    89,    90,   -59,    91,    92,     0,
       0,    93,     0,    94,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   103,   104,   105,   106,     0,     0,   107,
       0,   108,     0,   109,     0,   110,     0,     4,   111,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,     0,    29,
      30,     0,    31,     0,    32,    33,    34,    35,    36,    37,
      38,   782,    40,   -59,     0,    41,    42,    43,     0,    44,
    -336,     0,    45,    46,    47,    48,    49,     0,    50,    51,
      52,    53,   -59,    54,    55,     0,    56,    57,    58,     0,
    -336,     0,     0,    59,    60,    61,    62,     0,    64,    65,
    -336,   -59,    66,    67,    68,    69,     0,    70,    71,    72,
       0,    73,    74,    75,    76,    77,    78,    79,    80,     0,
      81,    82,     0,    83,    84,    85,    86,    87,     0,    88,
      89,    90,   -59,    91,    92,     0,     0,    93,     0,    94,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   103,
     104,   105,   106,     0,     0,   107,     0,   108,     0,   109,
       0,   110,     0,     4,   111,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,     0,    29,    30,     0,    31,     0,
      32,    33,    34,    35,    36,    37,    38,   788,    40,   -59,
       0,    41,    42,    43,     0,    44,  -336,     0,    45,    46,
      47,    48,    49,     0,    50,    51,    52,    53,   -59,    54,
      55,     0,    56,    57,    58,     0,  -336,     0,     0,    59,
      60,    61,    62,     0,    64,    65,  -336,   -59,    66,    67,
      68,    69,     0,    70,    71,    72,     0,    73,    74,    75,
      76,    77,    78,    79,    80,     0,    81,    82,     0,    83,
      84,    85,    86,    87,     0,    88,    89,    90,   -59,    91,
      92,     0,     0,    93,     0,    94,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,   103,   104,   105,   106,     0,
       0,   107,     0,   108,     0,   109,     0,   110,     0,     4,
     111,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
       0,    29,    30,     0,    31,     0,    32,    33,    34,    35,
      36,    37,    38,  1093,    40,   -59,     0,    41,    42,    43,
       0,    44,  -336,     0,    45,    46,    47,    48,    49,     0,
      50,    51,    52,    53,   -59,    54,    55,     0,    56,    57,
      58,     0,  -336,     0,     0,    59,    60,    61,    62,     0,
      64,    65,  -336,   -59,    66,    67,    68,    69,     0,    70,
      71,    72,     0,    73,    74,    75,    76,    77,    78,    79,
      80,     0,    81,    82,     0,    83,    84,    85,    86,    87,
       0,    88,    89,    90,   -59,    91,    92,     0,     0,    93,
       0,    94,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     0,     0,   102,     0,
       0,   103,   104,   105,   106,     0,     0,   107,     0,   108,
       0,   109,     0,   110,     0,     4,   111,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     0,    29,    30,     0,
      31,     0,    32,    33,    34,    35,    36,    37,    38,  1095,
      40,   -59,     0,    41,    42,    43,     0,    44,  -336,     0,
      45,    46,    47,    48,    49,     0,    50,    51,    52,    53,
     -59,    54,    55,     0,    56,    57,    58,     0,  -336,     0,
       0,    59,    60,    61,    62,     0,    64,    65,  -336,   -59,
      66,    67,    68,    69,     0,    70,    71,    72,     0,    73,
      74,    75,    76,    77,    78,    79,    80,     0,    81,    82,
       0,    83,    84,    85,    86,    87,     0,    88,    89,    90,
     -59,    91,    92,     0,     0,    93,     0,    94,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     100,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   103,   104,   105,
     106,     0,     0,   107,     0,   108,     0,   109,     0,   110,
       0,     4,   111,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,    14,    15,    16,    17,     0,
      18,     0,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,     0,    29,    30,     0,    31,     0,    32,    33,
      34,    35,    36,    37,    38,  1100,    40,   -59,     0,    41,
      42,    43,     0,    44,  -336,     0,    45,    46,    47,    48,
      49,     0,    50,    51,    52,    53,   -59,    54,    55,     0,
      56,    57,    58,     0,  -336,     0,     0,    59,    60,    61,
      62,     0,    64,    65,  -336,   -59,    66,    67,    68,    69,
       0,    70,    71,    72,     0,    73,    74,    75,    76,    77,
      78,    79,    80,     0,    81,    82,     0,    83,    84,    85,
      86,    87,     0,    88,    89,    90,   -59,    91,    92,     0,
       0,    93,     0,    94,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   103,   104,   105,   106,     0,     0,   107,
       0,   108,     0,   109,     0,   110,     0,     4,   111,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,     0,    29,
      30,     0,    31,     0,    32,    33,    34,    35,    36,    37,
      38,  1103,    40,   -59,     0,    41,    42,    43,     0,    44,
    -336,     0,    45,    46,    47,    48,    49,     0,    50,    51,
      52,    53,   -59,    54,    55,     0,    56,    57,    58,     0,
    -336,     0,     0,    59,    60,    61,    62,     0,    64,    65,
    -336,   -59,    66,    67,    68,    69,     0,    70,    71,    72,
       0,    73,    74,    75,    76,    77,    78,    79,    80,     0,
      81,    82,     0,    83,    84,    85,    86,    87,     0,    88,
      89,    90,   -59,    91,    92,     0,     0,    93,     0,    94,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   103,
     104,   105,   106,     0,     0,   107,     0,   108,     0,   109,
       0,   110,     0,     4,   111,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,    14,    15,    16,
      17,     0,    18,     0,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,     0,  1126,    30,     0,    31,     0,
      32,    33,    34,    35,    36,    37,    38,    39,    40,   -59,
       0,    41,    42,    43,     0,    44,  -336,     0,    45,    46,
      47,    48,    49,     0,    50,    51,    52,    53,   -59,    54,
      55,     0,    56,    57,    58,     0,  -336,     0,     0,    59,
      60,    61,    62,     0,    64,    65,  -336,   -59,    66,    67,
      68,    69,     0,    70,    71,    72,     0,    73,    74,    75,
      76,    77,    78,    79,    80,     0,    81,    82,     0,    83,
      84,    85,    86,    87,     0,    88,    89,    90,   -59,    91,
      92,     0,     0,    93,     0,    94,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,   103,   104,   105,   106,     0,
       0,   107,     0,   108,     0,   109,     0,   110,     0,     4,
     111,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,    14,    15,    16,    17,     0,    18,     0,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
       0,    29,    30,     0,    31,     0,    32,    33,    34,    35,
      36,    37,    38,  1132,    40,   -59,     0,    41,    42,    43,
       0,    44,  -336,     0,    45,    46,    47,    48,    49,     0,
      50,    51,    52,    53,   -59,    54,    55,     0,    56,    57,
      58,     0,  -336,     0,     0,    59,    60,    61,    62,     0,
      64,    65,  -336,   -59,    66,    67,    68,    69,     0,    70,
      71,    72,     0,    73,    74,    75,    76,    77,    78,    79,
      80,     0,    81,    82,     0,    83,    84,    85,    86,    87,
       0,    88,    89,    90,   -59,    91,    92,     0,     0,    93,
       0,    94,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     0,     0,   102,     0,
       0,   103,   104,   105,   106,     0,     0,   107,     0,   108,
       0,   109,     0,   110,     0,     4,   111,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,     0,    29,    30,     0,
      31,     0,    32,    33,    34,    35,    36,    37,    38,  1136,
      40,   -59,     0,    41,    42,    43,     0,    44,  -336,     0,
      45,    46,    47,    48,    49,     0,    50,    51,    52,    53,
     -59,    54,    55,     0,    56,    57,    58,     0,  -336,     0,
       0,    59,    60,    61,    62,     0,    64,    65,  -336,   -59,
      66,    67,    68,    69,     0,    70,    71,    72,     0,    73,
      74,    75,    76,    77,    78,    79,    80,     0,    81,    82,
       0,    83,    84,    85,    86,    87,     0,    88,    89,    90,
     -59,    91,    92,     0,     0,    93,     0,    94,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     100,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   103,   104,   105,
     106,     0,     0,   107,     0,   108,     0,   109,     0,   110,
       0,     0,   111,     5,     6,     7,     8,     9,    10,    11,
      12,     0,   654,     0,    13,     0,     0,    16,    17,   656,
      18,     0,   198,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,    30,     0,   199,     0,     0,     0,
      34,   200,   201,     0,     0,   202,    40,     0,     0,     0,
      42,     0,     0,    44,     0,   661,   203,     0,     0,    48,
      49,     0,     0,    51,     0,    53,     0,    54,    55,     0,
      56,    57,     0,     0,     0,     0,     0,     0,    60,     0,
      62,     0,    64,     0,     0,     0,     0,    67,    68,   204,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,   205,     0,    81,     0,     0,    83,     0,     0,
       0,    87,     0,     0,    89,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,    95,   533,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,   872,   873,   874,   875,   876,   674,     0,   675,
       0,   101,     0,   676,   677,   678,   679,   680,   681,   682,
     877,   684,   685,   103,   878,   105,     0,   687,   688,   879,
     690,   206,     0,   109,     0,   207,     0,     0,   111,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,     0,    16,    17,     0,    18,     0,   198,    20,
      21,    22,     0,     0,     0,     0,    27,     0,     0,    29,
      30,     0,   199,     0,     0,     0,    34,    35,    36,    37,
       0,    39,    40,     0,     0,     0,    42,     0,     0,    44,
       0,     0,   203,     0,     0,    48,    49,     0,    50,    51,
      52,    53,     0,    54,    55,     0,    56,    57,    58,     0,
       0,     0,     0,     0,    60,     0,    62,     0,    64,     0,
       0,     0,     0,    67,    68,   204,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,    78,    79,    80,     0,
      81,    82,     0,    83,     0,     0,     0,    87,     0,     0,
      89,     0,     0,     0,    92,     0,     0,    93,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   103,
     104,   105,     0,     0,     0,   107,     0,   108,     0,   109,
       0,   110,     0,     0,   111,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   198,     0,     0,    22,   386,    24,
       0,     0,     0,     0,     0,     0,    30,     0,   199,     0,
       0,     0,    34,   200,   201,     0,     0,   202,    40,     0,
       0,     0,    42,     0,     0,    44,     0,     0,   203,     0,
       0,    48,    49,     0,     0,    51,     0,    53,     0,    54,
      55,     0,    56,    57,     0,     0,     0,     0,     0,     0,
      60,    61,    62,     0,    64,     0,     0,     0,     0,    67,
      68,   204,     0,    70,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,   205,     0,    81,     0,     0,    83,
       0,     0,     0,    87,     0,     0,    89,     0,     0,    91,
      92,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,   103,   104,   105,     0,     0,
       0,   107,     0,   206,     0,   109,     0,   207,     0,     0,
     111,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     198,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,   199,     0,     0,     0,    34,   200,
     201,     0,     0,   202,    40,     0,     0,     0,    42,     0,
       0,    44,     0,     0,   203,     0,     0,    48,    49,     0,
       0,    51,     0,    53,     0,    54,    55,     0,    56,    57,
       0,     0,     0,     0,     0,     0,    60,     0,    62,     0,
      64,     0,     0,     0,     0,    67,    68,   204,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,    79,
     205,     0,    81,     0,     0,    83,   330,   331,     0,    87,
     368,     0,    89,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   101,
     369,     0,     0,     0,     0,     0,     0,     0,   102,     0,
       0,   103,   104,   105,     0,     0,     0,   107,     0,   206,
       0,   109,     0,   207,     0,     0,   111,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   198,     0,     0,    22,
       0,   761,     0,     0,     0,     0,     0,     0,    30,     0,
     199,     0,     0,     0,    34,   200,   201,     0,     0,   202,
      40,     0,   762,     0,    42,     0,     0,    44,     0,     0,
     203,     0,     0,    48,    49,     0,     0,    51,     0,    53,
       0,    54,    55,     0,    56,    57,     0,     0,     0,     0,
       0,     0,    60,     0,    62,     0,    64,     0,     0,     0,
       0,    67,    68,   204,     0,   763,     0,     0,     0,     0,
       0,    75,    76,    77,    78,    79,   205,     0,    81,     0,
       0,    83,     0,     0,     0,    87,     0,     0,    89,     0,
       0,   764,    92,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     100,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   103,   104,   105,
       0,     0,     0,   107,     0,   206,     0,   109,     0,   207,
       0,     0,   111,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   198,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,    30,     0,   199,     0,     0,     0,
      34,   200,   201,     0,     0,   202,    40,     0,     0,     0,
      42,     0,     0,    44,     0,     0,   203,     0,     0,    48,
      49,     0,     0,    51,     0,    53,     0,    54,    55,     0,
      56,    57,     0,     0,     0,     0,     0,     0,    60,     0,
      62,     0,    64,     0,     0,     0,     0,    67,    68,   204,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,   205,     0,    81,     0,     0,    83,   330,   331,
       0,    87,   368,     0,    89,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   103,   104,   105,     0,     0,     0,   107,
       0,   206,     0,   109,   845,   207,     0,     0,   111,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   198,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
      30,     0,   199,     0,     0,     0,    34,   200,   201,     0,
       0,   202,    40,     0,     0,     0,    42,     0,     0,    44,
       0,     0,   203,     0,     0,    48,    49,     0,     0,    51,
       0,    53,     0,    54,    55,     0,    56,    57,     0,     0,
       0,     0,     0,     0,    60,     0,    62,     0,    64,     0,
       0,     0,     0,    67,    68,   204,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,    79,   205,     0,
      81,     0,     0,    83,   330,   331,     0,    87,   368,     0,
      89,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   103,
     104,   105,     0,     0,     0,   107,     0,   206,     0,   109,
     847,   207,     0,     0,   111,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   198,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,    30,     0,   199,     0,
       0,     0,    34,   200,   201,     0,     0,   202,    40,     0,
       0,     0,    42,     0,     0,    44,     0,     0,   203,     0,
       0,    48,    49,     0,     0,    51,     0,    53,     0,    54,
      55,     0,    56,    57,     0,     0,     0,     0,     0,     0,
      60,     0,    62,     0,    64,     0,     0,     0,     0,    67,
      68,   204,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,   205,     0,    81,     0,     0,    83,
     330,   331,     0,    87,     0,     0,    89,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,   103,   104,   105,   332,     0,
       0,   107,     0,   206,     0,   109,     0,   207,     0,     0,
     111,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     198,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,   199,     0,     0,     0,    34,   200,
     201,     0,     0,   202,    40,     0,     0,     0,    42,     0,
       0,    44,     0,     0,   203,     0,     0,    48,    49,     0,
       0,    51,     0,    53,     0,    54,    55,  1000,    56,    57,
       0,     0,     0,     0,     0,     0,    60,     0,    62,     0,
      64,     0,     0,     0,     0,    67,    68,   204,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,    79,
     205,     0,    81,     0,     0,    83,   330,   331,     0,    87,
       0,     0,    89,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     0,     0,   102,     0,
       0,   103,   104,   105,     0,     0,     0,   107,     0,   206,
       0,   109,     0,   207,     0,     0,   111,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   198,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,    30,     0,
     199,     0,     0,     0,    34,   200,   201,     0,     0,   202,
      40,     0,     0,     0,    42,     0,     0,    44,     0,     0,
     203,     0,     0,    48,    49,     0,     0,    51,     0,    53,
       0,    54,    55,     0,    56,    57,     0,     0,     0,     0,
       0,     0,    60,   265,    62,     0,    64,     0,     0,     0,
       0,    67,    68,   204,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,    79,   205,     0,    81,     0,
       0,    83,     0,     0,     0,    87,     0,     0,    89,     0,
       0,     0,    92,     0,     0,     0,     0,     0,   266,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     100,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   103,   104,   105,
       0,     0,     0,   107,     0,   206,     0,   109,     0,   207,
       0,     0,   111,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   198,     0,     0,    22,     0,   273,     0,     0,
       0,     0,     0,     0,    30,     0,   199,     0,     0,     0,
      34,   200,   201,     0,     0,   202,    40,     0,     0,     0,
      42,     0,     0,    44,     0,     0,   203,     0,     0,    48,
      49,     0,     0,    51,     0,    53,     0,    54,    55,     0,
      56,    57,     0,     0,     0,     0,     0,     0,    60,     0,
      62,     0,    64,     0,     0,     0,     0,    67,    68,   204,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,   205,     0,    81,     0,     0,    83,     0,     0,
       0,    87,     0,     0,    89,     0,     0,   274,    92,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   103,   104,   105,     0,     0,     0,   107,
       0,   206,     0,   109,     0,   207,     0,     0,   111,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   198,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
      30,     0,   199,     0,     0,     0,    34,   200,   201,     0,
       0,   202,    40,     0,     0,     0,    42,     0,     0,    44,
       0,     0,   203,     0,     0,    48,    49,     0,     0,    51,
       0,    53,     0,    54,    55,     0,    56,    57,     0,     0,
       0,     0,     0,     0,    60,     0,    62,     0,    64,     0,
       0,     0,     0,    67,    68,   204,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,    79,   205,     0,
      81,     0,     0,    83,   330,   331,     0,    87,     0,     0,
      89,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   103,
     104,   105,     0,     0,     0,   107,     0,   206,     0,   109,
       0,   207,     0,     0,   111,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   198,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,    30,     0,   199,     0,
       0,     0,    34,   200,   201,     0,     0,   202,    40,     0,
       0,     0,    42,     0,     0,    44,     0,     0,   203,     0,
       0,    48,    49,     0,     0,    51,     0,    53,     0,    54,
      55,     0,    56,    57,     0,     0,     0,     0,     0,     0,
      60,     0,    62,     0,    64,     0,     0,     0,     0,    67,
      68,   204,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,   205,     0,    81,     0,     0,    83,
       0,     0,     0,    87,     0,     0,    89,     0,     0,     0,
      92,     0,     0,     0,     0,     0,   266,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,   103,   104,   105,     0,     0,
       0,   107,     0,   206,     0,   109,     0,   207,   373,     0,
     111,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     198,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,   199,     0,     0,     0,    34,   200,
     201,     0,     0,   202,    40,     0,     0,     0,    42,     0,
       0,    44,     0,     0,   203,     0,     0,    48,    49,     0,
       0,    51,     0,    53,     0,    54,    55,     0,    56,    57,
       0,     0,     0,     0,     0,     0,    60,     0,    62,     0,
      64,     0,     0,     0,     0,    67,    68,   204,  -725,     0,
       0,     0,  -725,     0,     0,    75,    76,    77,    78,    79,
     205,     0,    81,     0,     0,    83,     0,     0,     0,    87,
       0,     0,    89,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     0,     0,   102,     0,
       0,   103,   104,   105,     0,     0,     0,   107,     0,   206,
       0,   109,     0,   207,     0,     0,   111,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   198,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,    30,     0,
     199,     0,     0,     0,    34,   200,   201,     0,     0,   202,
      40,     0,     0,     0,    42,     0,     0,    44,     0,     0,
     203,     0,     0,    48,    49,     0,     0,    51,     0,    53,
       0,    54,    55,     0,    56,    57,     0,     0,     0,     0,
       0,     0,    60,     0,    62,     0,    64,     0,     0,     0,
       0,    67,    68,   204,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,    79,   205,     0,    81,     0,
       0,    83,     0,     0,     0,    87,     0,     0,    89,     0,
       0,     0,    92,     0,     0,     0,     0,     0,   266,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     100,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   103,   104,   105,
       0,     0,     0,   107,     0,   206,     0,   109,     0,   207,
       0,     0,   111,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   198,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,   288,    30,     0,   199,     0,     0,     0,
      34,   200,   201,     0,     0,   202,    40,     0,     0,     0,
      42,     0,     0,    44,     0,     0,   203,     0,     0,    48,
      49,     0,     0,    51,     0,    53,     0,    54,    55,     0,
      56,    57,     0,     0,     0,     0,     0,     0,    60,     0,
      62,     0,    64,     0,     0,     0,     0,    67,    68,   204,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,   205,     0,    81,     0,     0,    83,     0,     0,
       0,    87,     0,     0,    89,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   103,   104,   105,     0,     0,     0,   107,
       0,   108,     0,   109,     0,   207,     0,     0,   111,     5,
       6,     7,     8,     9,   389,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,    19,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
      30,     0,   199,     0,     0,     0,    34,   200,   201,     0,
       0,   202,    40,     0,     0,     0,    42,     0,     0,    44,
       0,     0,   203,     0,     0,    48,    49,     0,     0,    51,
       0,    53,     0,    54,    55,     0,    56,    57,     0,     0,
       0,     0,     0,     0,    60,     0,    62,     0,    64,     0,
       0,     0,     0,    67,    68,    69,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,    79,   205,     0,
      81,     0,     0,    83,     0,     0,     0,    87,     0,    88,
      89,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   103,
     104,   105,     0,     0,     0,   107,     0,   206,     0,   109,
       0,   207,     0,     0,   111,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   198,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,    30,     0,   199,     0,
       0,     0,    34,   200,   201,     0,     0,   202,    40,     0,
       0,     0,    42,     0,     0,    44,     0,     0,   203,     0,
       0,    48,    49,     0,     0,    51,     0,    53,     0,    54,
      55,     0,    56,    57,     0,     0,     0,     0,     0,     0,
      60,     0,    62,     0,    64,     0,     0,     0,     0,    67,
      68,   204,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,   205,     0,    81,     0,     0,    83,
       0,     0,     0,    87,     0,     0,    89,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,   103,   104,   105,     0,     0,
       0,   107,     0,   206,     0,   109,     0,   207,   373,     0,
     111,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     198,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,   199,     0,     0,     0,    34,   200,
     201,     0,     0,   202,    40,     0,     0,     0,    42,     0,
       0,    44,     0,     0,   203,     0,     0,    48,    49,     0,
       0,    51,     0,    53,     0,    54,    55,     0,    56,    57,
       0,     0,     0,     0,     0,     0,    60,     0,    62,     0,
      64,     0,     0,     0,     0,    67,    68,   204,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,    79,
     205,     0,    81,     0,     0,    83,     0,     0,     0,    87,
       0,     0,    89,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     0,     0,   102,     0,
       0,   103,   104,   105,     0,     0,     0,   107,     0,   206,
     843,   109,     0,   207,     0,     0,   111,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   198,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,    30,     0,
     199,     0,     0,     0,    34,   200,   201,     0,     0,   202,
      40,     0,     0,     0,    42,     0,     0,    44,     0,     0,
     203,     0,     0,    48,    49,     0,     0,    51,     0,    53,
       0,    54,    55,     0,    56,    57,     0,     0,     0,     0,
       0,     0,    60,     0,    62,     0,    64,     0,     0,     0,
       0,    67,    68,   204,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,    79,   205,     0,    81,     0,
       0,    83,     0,     0,     0,    87,     0,     0,    89,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     100,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   103,   104,   105,
       0,     0,     0,   107,     0,   206,     0,   109,     0,   207,
     853,     0,   111,     5,     6,     7,     8,     9,    10,    11,
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
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   103,   104,   105,     0,     0,     0,   107,
       0,   206,     0,   109,     0,   207,   858,     0,   111,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   198,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
      30,     0,   199,     0,     0,     0,    34,   200,   201,     0,
       0,   202,    40,     0,     0,     0,    42,     0,     0,    44,
       0,     0,   203,     0,     0,    48,    49,     0,     0,    51,
       0,    53,     0,    54,    55,     0,    56,    57,     0,     0,
       0,     0,     0,     0,    60,     0,    62,     0,    64,     0,
       0,     0,     0,    67,    68,   204,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,    79,   205,     0,
      81,     0,     0,    83,     0,     0,     0,    87,  1036,     0,
      89,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   103,
     104,   105,     0,     0,     0,   107,     0,   206,     0,   109,
       0,   207,     0,     0,   111,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   198,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,    30,     0,   199,     0,
       0,     0,    34,   200,   201,     0,     0,   202,    40,     0,
       0,     0,    42,     0,     0,    44,     0,     0,   203,     0,
       0,    48,    49,     0,     0,    51,     0,    53,     0,    54,
      55,     0,    56,    57,     0,     0,     0,     0,     0,     0,
      60,     0,    62,     0,    64,     0,     0,     0,     0,    67,
      68,   204,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,   205,     0,    81,     0,     0,    83,
       0,     0,     0,    87,  1207,     0,    89,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,   103,   104,   105,     0,     0,
       0,   107,     0,   206,     0,   109,     0,   207,     0,     0,
     111,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     198,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,   199,     0,     0,     0,    34,   200,
     201,     0,     0,   202,    40,     0,     0,     0,    42,     0,
       0,    44,     0,     0,   203,     0,     0,    48,    49,     0,
       0,    51,     0,    53,     0,    54,    55,     0,    56,    57,
       0,     0,     0,     0,     0,     0,    60,     0,    62,     0,
      64,     0,     0,     0,     0,    67,    68,   204,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,    79,
     205,     0,    81,     0,     0,    83,     0,     0,     0,    87,
       0,     0,    89,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     0,     0,   102,     0,
       0,   103,   104,   105,     0,     0,     0,   107,     0,   206,
       0,   109,     0,   207,     0,     0,   111,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   198,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,   308,     0,
     199,     0,     0,     0,    34,   200,   201,     0,     0,   202,
      40,     0,     0,     0,   310,     0,     0,    44,     0,     0,
     203,     0,     0,    48,    49,     0,     0,    51,     0,    53,
       0,    54,    55,     0,    56,    57,     0,     0,     0,     0,
       0,     0,    60,     0,    62,     0,    64,     0,     0,     0,
       0,    67,    68,   204,     0,     0,     0,     0,     0,     0,
       0,    75,   315,    77,    78,    79,   316,     0,    81,     0,
       0,    83,     0,     0,     0,    87,     0,     0,    89,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     100,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   103,   104,   105,
       0,     0,     0,   107,     0,   206,     0,   109,     0,   207,
       0,     0,   111,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   198,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,    30,     0,   199,     0,     0,     0,
      34,   200,   201,     0,     0,   930,    40,     0,     0,     0,
      42,     0,     0,    44,     0,     0,   203,     0,     0,    48,
      49,     0,     0,    51,     0,    53,     0,    54,    55,     0,
      56,    57,     0,     0,     0,     0,     0,     0,    60,     0,
      62,     0,    64,     0,     0,     0,     0,    67,    68,   204,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,   205,     0,    81,     0,     0,    83,     0,     0,
       0,    87,     0,     0,    89,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   103,   104,   105,     0,     0,     0,   107,
       0,   206,     0,   109,     0,   207,     0,     0,   111,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   198,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
      30,     0,   199,     0,     0,     0,    34,   200,   201,     0,
       0,   933,    40,     0,     0,     0,    42,     0,     0,    44,
       0,     0,   203,     0,     0,    48,    49,     0,     0,    51,
       0,    53,     0,    54,    55,     0,    56,    57,     0,     0,
       0,     0,     0,     0,    60,     0,    62,     0,    64,     0,
       0,     0,     0,    67,    68,   204,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,    79,   205,     0,
      81,     0,     0,    83,     0,     0,     0,    87,     0,     0,
      89,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   103,
     104,   105,     0,     0,     0,   107,     0,   206,     0,   109,
       0,   207,     0,     0,   111,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   198,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,    30,     0,   199,     0,
       0,     0,    34,   200,   201,     0,     0,  1176,    40,     0,
       0,     0,    42,     0,     0,    44,     0,     0,   203,     0,
       0,    48,    49,     0,     0,    51,     0,    53,     0,    54,
      55,     0,    56,    57,     0,     0,     0,     0,     0,     0,
      60,     0,    62,     0,    64,     0,     0,     0,     0,    67,
      68,   204,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,   205,     0,    81,     0,     0,    83,
       0,     0,     0,    87,     0,     0,    89,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,   103,   104,   105,     0,     0,
       0,   107,     0,   206,     0,   109,     0,   207,     0,     0,
     111,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     198,     0,     0,    22,     0,     0,     0,     0,     0,     0,
       0,     0,    30,     0,   199,     0,     0,     0,    34,   200,
     201,     0,     0,  1177,    40,     0,     0,     0,    42,     0,
       0,    44,     0,     0,   203,     0,     0,    48,    49,     0,
       0,    51,     0,    53,     0,    54,    55,     0,    56,    57,
       0,     0,     0,     0,     0,     0,    60,     0,    62,     0,
      64,     0,     0,     0,     0,    67,    68,   204,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,    79,
     205,     0,    81,     0,     0,    83,     0,     0,     0,    87,
       0,     0,    89,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      96,    97,    98,    99,   100,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,     0,     0,     0,   102,     0,
       0,   103,   104,   105,     0,     0,     0,   107,     0,   206,
       0,   109,     0,   207,     0,     0,   111,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   198,     0,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,    30,     0,
     199,     0,     0,     0,    34,   200,   201,     0,     0,  1179,
      40,     0,     0,     0,    42,     0,     0,    44,     0,     0,
     203,     0,     0,    48,    49,     0,     0,    51,     0,    53,
       0,    54,    55,     0,    56,    57,     0,     0,     0,     0,
       0,     0,    60,     0,    62,     0,    64,     0,     0,     0,
       0,    67,    68,   204,     0,     0,     0,     0,     0,     0,
       0,    75,    76,    77,    78,    79,   205,     0,    81,     0,
       0,    83,     0,     0,     0,    87,     0,     0,    89,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    96,    97,    98,    99,
     100,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,     0,     0,     0,   102,     0,     0,   103,   104,   105,
       0,     0,     0,   107,     0,   206,     0,   109,     0,   207,
       0,     0,   111,     5,     6,     7,     8,     9,    10,    11,
      12,     0,     0,     0,    13,     0,     0,    16,    17,     0,
      18,     0,   198,     0,     0,    22,     0,     0,     0,     0,
       0,     0,     0,     0,    30,     0,   199,     0,     0,     0,
      34,   200,   201,     0,     0,  1180,    40,     0,     0,     0,
      42,     0,     0,    44,     0,     0,   203,     0,     0,    48,
      49,     0,     0,    51,     0,    53,     0,    54,    55,     0,
      56,    57,     0,     0,     0,     0,     0,     0,    60,     0,
      62,     0,    64,     0,     0,     0,     0,    67,    68,   204,
       0,     0,     0,     0,     0,     0,     0,    75,    76,    77,
      78,    79,   205,     0,    81,     0,     0,    83,     0,     0,
       0,    87,     0,     0,    89,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    96,    97,    98,    99,   100,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,     0,     0,     0,
     102,     0,     0,   103,   104,   105,     0,     0,     0,   107,
       0,   206,     0,   109,     0,   207,     0,     0,   111,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   198,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
      30,     0,   199,     0,     0,     0,    34,   200,   201,     0,
       0,  1181,    40,     0,     0,     0,    42,     0,     0,    44,
       0,     0,   203,     0,     0,    48,    49,     0,     0,    51,
       0,    53,     0,    54,    55,     0,    56,    57,     0,     0,
       0,     0,     0,     0,    60,     0,    62,     0,    64,     0,
       0,     0,     0,    67,    68,   204,     0,     0,     0,     0,
       0,     0,     0,    75,    76,    77,    78,    79,   205,     0,
      81,     0,     0,    83,     0,     0,     0,    87,     0,     0,
      89,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    96,    97,
      98,    99,   100,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,   103,
     104,   105,     0,     0,     0,   107,     0,   206,     0,   109,
       0,   207,     0,     0,   111,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   198,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,    30,     0,   199,     0,
       0,     0,    34,   200,   201,     0,     0,  1182,    40,     0,
       0,     0,    42,     0,     0,    44,     0,     0,   203,     0,
       0,    48,    49,     0,     0,    51,     0,    53,     0,    54,
      55,     0,    56,    57,     0,     0,     0,     0,     0,     0,
      60,     0,    62,     0,    64,     0,     0,     0,     0,    67,
      68,   204,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,   205,     0,    81,     0,     0,    83,
       0,     0,     0,    87,     0,     0,    89,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,    97,    98,    99,   100,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,   102,     0,     0,   103,   104,   105,     0,     0,
       0,   107,     0,   206,     0,   109,     0,   207,     0,     0,
     111,     6,     7,     8,     9,    10,    11,    12,     0,     0,
       0,    13,     0,     0,     0,    17,     0,     0,     0,   198,
       0,     0,     0,     0,   307,     0,     0,     0,     0,     0,
       0,     0,     0,   199,     0,     0,     0,     0,   200,   201,
       0,     0,   202,     0,     0,   309,     0,     0,     0,   311,
       0,     0,     0,   203,     0,     0,    48,    49,     0,     0,
       0,     0,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,     0,   312,     0,    60,   313,    62,    63,    64,
       0,     0,     0,     0,    67,     0,   204,     0,   314,     0,
       0,     0,     0,     0,    75,     0,    77,     0,    79,     0,
       0,     0,     0,     0,     0,     0,     0,   317,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    96,
      97,    98,    99,   100,     0,     0,     0,     0,   101,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
     103,   104,   105,     0,     0,     0,   107,     0,   206,     0,
     109,  -340,   207,     0,     0,   111,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,     0,
      17,     0,     0,     0,   198,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,   199,     0,
       0,     0,     0,   200,   201,     0,     0,   202,     0,     0,
     309,     0,     0,     0,   311,     0,     0,     0,   203,     0,
       0,    48,    49,     0,     0,     0,     0,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,     0,   312,     0,
      60,   313,    62,    63,    64,     0,     0,     0,     0,    67,
       0,   204,     0,   314,     0,     0,     0,     0,     0,    75,
       0,    77,     0,    79,     0,     0,     0,     0,     0,     0,
       0,     0,   317,     0,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   652,
       0,   653,     0,     0,    96,    97,    98,    99,   100,     0,
     654,     0,     0,   101,     0,   655,   239,   656,   657,     0,
       0,     0,   102,   658,     0,   103,   104,   105,     0,     0,
       0,   107,   242,   206,   199,   109,     0,   207,   243,     0,
     111,     0,     0,     0,   659,     0,     0,     0,   245,     0,
       0,   660,     0,   661,     0,     0,     0,     0,     0,     0,
       0,   662,     0,     0,     0,     0,     0,     0,   248,   663,
       0,     0,     0,     0,     0,     0,   250,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   664,     0,     0,     0,
       0,     0,     0,     0,     0,   252,   253,     0,   665,     0,
     255,     0,   666,     0,     0,   257,     0,     0,     0,   667,
       0,     0,   259,     0,     0,     0,   668,     0,     0,     0,
       0,     0,     0,     0,     0,   533,   459,   460,   461,   462,
     463,     0,     0,   466,   467,   468,   469,     0,   471,   472,
     669,   670,   671,   672,   673,   674,     0,   675,     0,     0,
       0,   676,   677,   678,   679,   680,   681,   682,   683,   684,
     685,     0,   686,     0,     0,   687,   688,   689,   690,     0,
       0,   691,     5,     6,     7,     8,     9,    10,    11,    12,
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
      87,     0,     0,    89,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    96,    97,    98,    99,   100,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   102,
       0,     0,     0,   104,   105,     0,     0,     0,   107,     0,
     206,     0,   109,     0,   207,     5,     6,     7,     8,     9,
      10,    11,    12,     0,     0,     0,    13,     0,     0,    16,
      17,     0,    18,     0,   198,     0,     0,    22,     0,     0,
       0,     0,     0,     0,     0,     0,    30,     0,   199,     0,
       0,     0,    34,   200,   201,     0,     0,   202,    40,     0,
       0,     0,    42,     0,     0,    44,     0,     0,   203,     0,
       0,    48,    49,     0,     0,    51,     0,    53,     0,    54,
      55,     0,    56,    57,     0,     0,     0,     0,     0,     0,
      60,     0,    62,     0,    64,     0,     0,     0,     0,    67,
      68,   204,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,   205,     0,    81,     0,     0,    83,
       0,     0,     0,    87,     0,     0,    89,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,    98,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   198,   105,     0,    22,
       0,     0,     0,   206,     0,   109,     0,   207,    30,     0,
     199,     0,     0,     0,    34,   200,   201,     0,     0,   202,
      40,     0,     0,     0,    42,     0,     0,    44,     0,     0,
     203,     0,     0,    48,    49,     0,     0,    51,     0,    53,
       0,    54,    55,     0,    56,    57,     0,     0,     0,   716,
       0,     0,    60,     0,    62,     0,    64,     0,   654,     0,
       0,    67,    68,   204,     0,   656,   717,     0,     0,     0,
       0,    75,    76,    77,    78,    79,   205,     0,    81,     0,
     718,    83,     0,     0,     0,    87,     0,     0,    89,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,   661,     0,     0,     0,     0,     0,     0,     0,   719,
       0,     0,     0,     0,     0,     0,    96,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   105,
       0,     0,     0,     0,     0,     0,     0,   109,     0,   207,
     720,     0,     0,     0,     0,     0,   721,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   533,   459,   460,   461,   462,   463,     0,
       0,   466,   467,   468,   469,     0,   471,   472,   669,   670,
     671,   672,   673,   674,     0,   675,     0,     0,     0,   676,
     677,   678,   679,   680,   681,   682,   683,   684,   685,   237,
     686,     0,     0,   687,   688,   689,   690,  1033,   654,     0,
       0,     0,     0,   238,   239,   656,   240,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,   243,     0,     0,     0,
       0,     0,   244,     0,     0,     0,   245,     0,     0,   246,
       0,   661,     0,     0,     0,     0,     0,     0,     0,   247,
       0,     0,     0,     0,     0,     0,   248,   249,     0,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,   428,   252,   253,     0,   254,     0,   255,   429,
     256,     0,     0,   257,     0,     0,     0,   258,     0,     0,
     259,     0,   430,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,   533,   459,   460,   461,   462,   463,     0,
       0,   466,   467,   468,   469,     0,   471,   472,   669,   670,
     671,   672,   673,   674,     0,   675,     0,     0,     0,   676,
     677,   678,   679,   680,   681,   682,   683,   684,   685,     0,
     686,     0,     0,   687,   688,   689,   690,     0,   428,     0,
     431,     0,     0,     0,   432,   429,     0,     0,     0,     0,
       0,     0,     0,  1092,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,   533,   459,   460,
     461,   462,   463,     0,     0,   466,   467,   468,   469,     0,
     471,   472,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,   431,   454,   455,   456,
     432,   488,     0,   428,     0,     0,     0,     0,   457,  1099,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   533,   459,   460,   461,   462,   463,     0,
       0,   466,   467,   468,   469,     0,   471,   472,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,     0,   488,     0,   428,
       0,   431,     0,     0,   457,   432,   429,     0,     0,     0,
       0,     0,     0,     0,  1243,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,   533,   459,
     460,   461,   462,   463,     0,     0,   466,   467,   468,   469,
       0,   471,   472,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   431,   454,   455,
     456,   432,   488,     0,   428,     0,     0,     0,     0,   457,
    1244,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,   433,   533,   459,   460,   461,   462,   463,
       0,     0,   466,   467,   468,   469,     0,   471,   472,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,     0,   488,     0,
     428,     0,   431,     0,     0,   457,   432,   429,     0,     0,
       0,     0,     0,     0,     0,  1245,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,   533,
     459,   460,   461,   462,   463,     0,     0,   466,   467,   468,
     469,     0,   471,   472,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,   431,   454,
     455,   456,   432,   488,     0,   428,     0,     0,     0,     0,
     457,  1246,   429,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   533,   459,   460,   461,   462,
     463,     0,     0,   466,   467,   468,   469,     0,   471,   472,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,     0,   454,   455,   456,     0,   488,
       0,   428,     0,   431,     0,     0,   457,   432,   429,     0,
       0,     0,     0,     0,     0,     0,  1251,     0,     0,     0,
       0,   430,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
     533,   459,   460,   461,   462,   463,     0,     0,   466,   467,
     468,   469,     0,   471,   472,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,   431,
     454,   455,   456,   432,   488,     0,   428,     0,     0,     0,
       0,   457,  1252,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,   513,     0,     0,
       0,     0,     0,     0,     0,   433,   533,   459,   460,   461,
     462,   463,   514,     0,   466,   467,   468,   469,     0,   471,
     472,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,     0,   454,   455,   456,     0,
     488,     0,     0,     0,   431,     0,     0,   457,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   428,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
     433,     0,     0,   622,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   623,
       0,   454,   455,   456,     0,   488,     0,     0,     0,     0,
       0,     0,   457,     0,     0,     0,   431,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     428,   868,     0,     0,     0,     0,     0,   429,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     430,     0,   433,     0,     0,   869,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   870,   624,   454,   455,   456,     0,     0,     0,     0,
       0,     0,     0,     0,   457,     0,   428,     0,   431,     0,
       0,     0,   432,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   430,   977,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,     0,   431,   454,   455,   456,   432,     0,
       0,   428,     0,     0,     0,     0,   457,     0,   429,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   430,   288,     0,     0,     0,   516,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,     0,
       0,   454,   455,   456,     0,   488,     0,   428,     0,   431,
       0,     0,   457,   432,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   516,     0,   490,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,   431,   454,   455,   456,   432,
     488,     0,   428,     0,     0,     0,     0,   457,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,   288,     0,     0,     0,   218,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,     0,   454,   455,   456,     0,     0,     0,   428,     0,
     431,     0,     0,   457,   432,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,   752,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   753,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,   431,   454,   455,   456,
     432,   488,     0,   428,     0,     0,     0,     0,   457,     0,
     429,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   430,   754,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,   755,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,     0,     0,     0,   428,
       0,   431,     0,     0,   457,   432,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
     969,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   431,   454,   455,
     456,   432,     0,     0,   428,     0,     0,     0,     0,   457,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,     0,   488,     0,
     428,     0,   431,     0,     0,   457,   432,   429,     0,     0,
       0,     0,     0,     0,     0,   991,     0,     0,     0,     0,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,   431,   454,
     455,   456,   432,   488,     0,     0,     0,     0,     0,     0,
     457,     0,   428,     0,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     516,     0,   430,     0,   433,     0,     0,  1145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,   435,     0,   436,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,  1146,     0,   454,   455,   456,     0,     0,
       0,     0,     0,     0,  1027,     0,   457,     0,   428,   556,
     431,     0,     0,     0,   432,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,   428,     0,
       0,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,   434,   435,     0,   436,   437,   438,   430,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,   431,   454,   455,   456,
     432,     0,     0,     0,   428,     0,     0,     0,   457,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,   431,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,   433,   454,   455,   456,     0,     0,     0,     0,
       0,     0,   431,     0,   457,     0,   432,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,   428,   454,   455,   456,     0,   580,   433,   429,
       0,     0,     0,     0,   457,     0,     0,     0,     0,     0,
       0,     0,   430,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   428,   584,   454,
     455,   456,     0,     0,   429,     0,     0,     0,     0,     0,
     457,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,   432,     0,     0,   428,     0,     0,
       0,     0,     0,     0,   429,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   430,     0,     0,
       0,     0,     0,     0,     0,   609,   433,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,   432,
       0,     0,   434,   435,     0,   436,   437,   438,   756,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,     0,     0,   454,   455,   456,
       0,   433,     0,   428,   864,   431,     0,     0,   457,   432,
     429,     0,     0,     0,     0,     0,     0,   434,   435,     0,
     436,   437,   438,   430,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   433,   454,   455,   456,     0,     0,   428,     0,     0,
       0,     0,     0,   457,   429,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,   430,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   431,   454,   455,   456,   432,     0,     0,     0,   844,
       0,     0,   428,   457,     0,     0,     0,     0,     0,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   430,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,   432,
       0,     0,     0,   434,   435,     0,   436,   437,   438,     0,
     439,   865,   441,     0,   442,   443,   444,   445,   446,   447,
     448,   449,   450,   451,   452,   453,     0,   218,   454,   455,
     456,   433,     0,     0,     0,     0,     0,     0,     0,   457,
     431,     0,     0,     0,   432,     0,     0,   434,   435,     0,
     436,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,   445,   446,   447,   448,   449,   450,   451,   452,   453,
       0,   428,   454,   455,   456,     0,   433,     0,   429,     0,
       0,     0,     0,   457,     0,     0,     0,     0,     0,     0,
       0,   430,   434,   435,   989,   436,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   428,   961,   454,   455,   456,
       0,     0,   429,     0,     0,     0,     0,     0,   457,     0,
       0,     0,     0,     0,     0,   430,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
       0,     0,     0,   432,   428,     0,     0,     0,     0,     0,
       0,   429,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,   431,     0,     0,     0,   432,     0,     0,
       0,   434,   435,     0,   436,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,   445,   446,   447,   448,   449,
     450,   451,   452,   453,     0,     0,   454,   455,   456,   433,
       0,     0,   431,     0,     0,   428,   432,   457,     0,     0,
       0,     0,   429,     0,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,   430,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   433,     0,
     454,   455,   456,     0,     0,     0,     0,  1003,     0,     0,
       0,   457,     0,     0,   434,   435,     0,   436,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,   445,   446,
     447,   448,   449,   450,   451,   452,   453,     0,     0,   454,
     455,   456,     0,   431,     0,     0,  1004,   432,   428,  1042,
     457,     0,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1030,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   434,   435,     0,   436,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,     0,     0,
     454,   455,   456,     0,     0,     0,   431,     0,   428,     0,
     432,   457,     0,     0,     0,   429,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   430,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,   431,     0,     0,   428,
     432,     0,     0,     0,   457,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
    1077,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,   435,
       0,   436,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,     0,     0,   454,   455,   456,     0,   431,     0,   428,
    1070,   432,     0,     0,   457,     0,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
    1080,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,   429,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   430,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   431,     0,   428,
    1142,   432,     0,     0,     0,   457,   429,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,   431,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   433,   454,   455,   456,   431,     0,   428,
       0,   432,     0,     0,     0,   457,   429,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   430,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   433,   454,   455,   456,     0,     0,     0,
       0,     0,     0,  1083,     0,   457,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   431,     0,   428,
       0,   432,     0,     0,     0,   457,   429,     0,     0,     0,
    1175,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,   428,
    1213,     0,     0,     0,     0,     0,   429,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   430,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   431,     0,   428,
       0,   432,     0,     0,     0,   457,   429,     0,     0,     0,
    1178,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,   431,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,  1230,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   433,   454,   455,   456,   431,     0,   428,
       0,   432,     0,     0,     0,   457,   429,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   430,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   433,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   431,     0,   428,
       0,   432,     0,     0,     0,   457,   429,     0,     0,     0,
    1271,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   431,     0,   428,
       0,   432,     0,     0,     0,   457,   429,     0,     0,     0,
    1272,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   431,     0,   428,
       0,   432,     0,     0,     0,   457,   429,     0,     0,     0,
    1273,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   431,     0,   428,
       0,   432,     0,     0,     0,   457,   429,     0,     0,     0,
    1274,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   431,     0,   428,
       0,   432,     0,     0,     0,   457,   429,     0,     0,     0,
    1275,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,   428,
       0,     0,     0,     0,     0,     0,   429,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,   430,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,     0,   454,   455,   456,   431,     0,   428,
       0,   432,     0,     0,     0,   457,   429,     0,     0,     0,
    1276,     0,     0,     0,     0,     0,     0,     0,     0,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,   431,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   433,   454,   455,   456,   956,     0,     0,
       0,   432,     0,     0,     0,   457,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   433,   454,   455,   456,     0,     0,     0,
       0,     0,     0,     0,     0,   457,     0,     0,     0,   434,
     435,     0,   436,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,     0,   497,   454,   455,   456,    23,    24,     0,
       0,     0,  1050,     0,     0,   457,     0,   498,     0,    32,
     499,     0,     0,     0,     0,    38,   497,     0,     0,     0,
      23,    24,    43,     0,     0,  1050,     0,     0,     0,     0,
     498,     0,    32,   499,     0,     0,     0,     0,    38,     0,
       0,     0,     0,     0,     0,    43,     0,     0,    59,     0,
      61,     0,    63,     0,  1051,     0,     0,  1052,     0,     0,
     500,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,  1051,     0,     0,
    1052,    86,     0,   500,    88,    70,  1053,     0,    91,     0,
       0,     0,     0,     0,     0,     0,   430,     0,     0,     0,
       0,     0,     0,     0,    86,     0,     0,    88,     0,  1053,
     497,    91,     0,     0,    23,    24,     0,     0,     0,  1050,
       0,     0,     0,     0,   498,     0,    32,   499,     0,     0,
       0,     0,    38,     0,     0,     0,     0,   106,     0,    43,
       0,     0,     0,  1054,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   431,     0,     0,     0,   432,     0,
     106,     0,     0,     0,     0,    59,  1153,    61,     0,    63,
       0,  1051,     0,     0,  1052,     0,     0,   500,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,    86,     0,
       0,    88,     0,  1053,     0,    91,   434,   435,     0,   436,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     445,     0,   447,   448,   449,   450,   451,     0,   453,     0,
     497,   454,   455,   456,    23,    24,     0,     0,     0,  1050,
       0,     0,   457,     0,   498,     0,    32,   499,     0,     0,
       0,     0,    38,     0,   106,     0,     0,     0,     0,    43,
    1214,     0,   397,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   238,   239,
       0,   240,     0,     0,     0,    59,   241,    61,     0,   379,
       0,  1051,   398,     0,  1052,   242,     0,   500,     0,    70,
       0,   243,     0,     0,     0,     0,     0,   244,     0,     0,
       0,   245,     0,     0,   246,     0,     0,     0,    86,     0,
       0,    88,     0,  1053,   247,    91,     0,     0,     0,     0,
       0,   248,   249,     0,     0,     0,     0,     0,     0,   250,
     237,     0,     0,     0,     0,     0,     0,     0,     0,   251,
       0,     0,     0,     0,   238,   239,     0,   240,   252,   253,
       0,   254,   241,   255,    24,   256,     0,     0,   257,     0,
       0,   242,   258,     0,   106,   259,   399,   243,     0,   260,
       0,     0,     0,   244,     0,     0,     0,   245,     0,     0,
     246,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     247,     0,     0,     0,     0,     0,     0,   248,   249,   237,
       0,     0,     0,     0,     0,   250,    61,     0,     0,     0,
       0,     0,     0,   238,   239,   251,   240,     0,    70,     0,
       0,   241,     0,     0,   252,   253,     0,   254,     0,   255,
     242,   256,     0,     0,   257,     0,   243,     0,   258,     0,
       0,   259,   244,     0,    91,   260,   245,     0,     0,   246,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   247,
       0,     0,     0,     0,     0,     0,   248,   249,     0,     0,
       0,     0,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,     0,     0,   237,     0,     0,
       0,     0,     0,   252,   253,     0,   254,     0,   255,     0,
     256,   238,   239,   257,   240,     0,     0,   258,   569,   241,
     259,     0,     0,     0,   260,   398,     0,     0,   242,     0,
       0,     0,     0,     0,   243,     0,     0,     0,     0,     0,
     244,     0,     0,     0,   245,     0,     0,   246,     0,     0,
       0,     0,     0,     0,     0,     0,  -302,   247,     0,     0,
       0,     0,     0,     0,   248,   249,     0,     0,     0,     0,
    -302,  -302,   250,  -302,     0,     0,     0,     0,  -302,     0,
       0,     0,   251,     0,     0,     0,     0,  -302,     0,     0,
       0,   252,   253,  -302,   254,     0,   255,     0,   256,  -302,
       0,   257,     0,  -302,     0,   258,  -302,     0,   259,     0,
       0,     0,   260,     0,     0,   237,  -302,     0,     0,     0,
       0,     0,     0,  -302,  -302,     0,     0,     0,     0,   238,
     239,  -302,   240,     0,     0,     0,     0,   241,     0,     0,
       0,  -302,     0,     0,     0,     0,   242,     0,     0,     0,
    -302,  -302,   243,  -302,     0,  -302,     0,  -302,   244,     0,
    -302,     0,   245,     0,  -302,   246,     0,  -302,     0,     0,
       0,  -302,     0,     0,  -303,   247,     0,     0,     0,     0,
       0,     0,   248,   249,     0,     0,     0,     0,  -303,  -303,
     250,  -303,     0,     0,     0,     0,  -303,     0,     0,     0,
     251,     0,     0,     0,     0,  -303,     0,     0,     0,   252,
     253,  -303,   254,     0,   255,     0,   256,  -303,     0,   257,
       0,  -303,     0,   258,  -303,     0,   259,     0,     0,     0,
     260,     0,     0,     0,  -303,     0,     0,     0,     0,     0,
       0,  -303,  -303,     0,     0,     0,     0,     0,     0,  -303,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -303,
       0,     0,     0,     0,     0,     0,     0,     0,  -303,  -303,
       0,  -303,     0,  -303,     0,  -303,     0,     0,  -303,     0,
       0,     0,  -303,     0,     0,  -303,     0,     0,     0,  -303
};

static const yytype_int16 yycheck[] =
{
      13,   168,    20,    67,    17,    72,     2,   280,    21,   578,
     485,    27,    67,    67,    27,    41,   420,   648,   786,   184,
     538,   894,    35,    36,    37,   870,    39,    94,   645,   137,
     234,   304,   832,   573,   582,   622,   694,    50,   696,    52,
      53,   390,   109,   826,   227,    58,   635,    60,   637,    62,
     406,   865,    33,     1,    67,    71,    48,    33,    71,    72,
      73,    74,    75,    76,   904,   236,    33,    80,    33,    82,
      48,   344,   888,   103,     3,     3,    89,   548,    33,   125,
      93,    94,    27,    96,    61,    98,     1,    37,   101,   102,
      47,   104,   148,    56,   110,   108,   109,   110,   111,   122,
      33,   166,    33,    48,    48,  1171,     0,    52,  1005,   280,
      47,   276,    93,   543,    27,    33,    56,   270,   271,   272,
     176,   148,   135,    73,   137,   171,    56,    75,    85,    66,
     122,    76,   122,   304,    79,    48,    81,   122,    93,    52,
     570,   171,    48,   120,     3,   172,    91,    91,    85,     1,
     354,   166,   120,   176,  1220,   122,   148,   122,   173,   116,
     175,    72,     3,    76,   177,   110,    79,  1007,    81,   236,
     148,   119,  1045,   344,   942,   104,   104,   648,    91,   116,
     148,    33,  1055,    94,   176,   166,   176,   200,   201,   202,
     171,   207,   205,   206,   207,   171,   813,   110,   109,   999,
      75,     1,   475,   476,   171,  1005,   171,   225,   234,    61,
     173,   567,   127,   280,    56,     1,   171,   148,    35,    36,
      37,   374,   375,   236,   172,  1008,  1040,  1124,  1044,   174,
     148,   779,   147,   148,    93,   127,    32,   304,   171,   757,
     171,    93,   148,   173,   119,   104,    22,    33,    48,   148,
     148,    84,    85,   171,   106,   420,    18,    27,   148,   174,
     152,   174,   148,   148,   142,   166,    56,   280,   120,   440,
     176,   172,   150,   172,   172,    61,   625,   344,    27,  1152,
      27,   389,   869,    48,   174,  1158,  1086,  1160,   174,   174,
     830,   304,   496,   110,    90,   147,    56,   172,    94,  1144,
     115,  1146,   315,   316,   475,   476,   895,    93,   897,    79,
     148,   786,    91,    89,   166,   166,    78,   330,   331,   171,
     106,    91,   173,   148,  1124,   492,    91,   127,   354,  1097,
      79,   344,    79,   148,   120,    97,   174,   113,   955,   148,
     110,   405,    91,  1216,    91,  1218,   142,   147,   148,   174,
     148,   147,   114,   173,   150,   175,   369,   172,   154,    54,
     373,   147,   110,   110,   148,   174,   166,   538,   117,   382,
     366,   384,     2,   556,   892,  1006,   174,   148,   394,    74,
       8,   394,   148,   587,   148,   171,   166,   148,   173,    84,
     174,   173,   172,   175,   148,   166,    26,   464,   465,    29,
     166,    83,    56,   470,    86,   166,   473,   474,   475,   476,
     174,   582,   166,  1071,  1072,   428,   429,   430,   431,   432,
     433,   939,   435,   436,   437,   438,   439,   148,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   628,  1033,   148,   148,    47,
      80,   464,   465,   174,    84,    85,   148,   470,   148,   148,
     473,   474,   475,   476,   477,   478,    32,   942,    66,    11,
     496,   538,   485,   174,   174,   148,   148,   490,   108,   148,
      56,   494,   174,   113,   174,   174,   173,    85,   175,   173,
     503,   148,   148,   166,   173,   511,   175,   166,   511,   148,
     513,   514,   174,   148,   568,   148,   519,   520,   148,   166,
     166,   915,   525,   568,   568,   582,   148,   166,   116,   532,
      56,   166,   535,   166,    90,   538,   166,   171,    94,   173,
      56,   598,    90,   600,   166,   548,    94,   550,   142,   127,
     173,   554,   175,   147,   557,   558,   150,    90,     2,    56,
     154,    94,    56,   464,   465,   568,  1143,     8,  1145,   470,
     573,   587,   473,   474,   173,   153,   175,   155,   156,   582,
     158,   159,    26,     3,    90,    29,   142,   173,    94,   175,
      27,   147,   595,   149,   150,   598,   757,   600,   154,   219,
      20,   173,   605,   606,   607,   161,   609,   610,   598,    56,
     600,    48,  1171,   169,    34,    52,   101,    90,   779,   622,
     623,    94,    90,    90,    56,   432,    94,    94,    27,   615,
     305,   306,  1097,   173,    51,   638,    80,    54,    56,    76,
      84,    85,    79,    63,    81,   648,    56,   790,   791,    48,
      56,   794,   795,    52,    91,     8,    56,    74,   171,    56,
      77,  1220,   173,   283,   108,  1165,   147,    84,   288,   113,
     477,   478,   165,   110,   166,   166,   166,    76,   166,   166,
      79,    56,    81,   490,   104,   171,    32,   166,   691,   166,
     110,   173,    91,   110,   166,   698,   166,   598,   121,   600,
     757,    85,   171,   864,   865,   148,   166,   514,   851,   852,
     173,   110,   173,   520,   173,   173,   147,   174,   525,   173,
     777,   166,   779,     8,  1224,  1225,  1226,  1227,  1228,  1229,
      66,   892,   171,   890,   172,     8,     8,   127,   127,    91,
      91,   150,    69,   173,    90,   127,   366,   174,    94,   752,
     753,   754,   755,   756,   757,   758,   759,   148,    48,   816,
     915,   381,   770,   771,   127,   173,   127,    35,    35,    66,
     174,   127,   174,   147,   777,   219,   779,   780,   939,   782,
     174,    21,   148,   786,   176,   788,   171,   106,   176,   166,
     173,   150,   150,   150,   797,   798,   142,   143,   605,   166,
     420,   147,   150,   149,   150,   150,   150,   150,   154,   150,
     150,   966,   150,   816,   142,   161,   150,   150,   973,   150,
     150,   167,   168,   169,   166,   174,   176,   830,    35,   166,
      35,   834,   147,   172,   166,   892,   148,   166,   173,   283,
     894,   115,   166,    33,   288,   171,    33,    35,   902,   174,
     172,   174,   166,   166,   147,  1118,  1119,   911,   176,   862,
     176,   166,   173,   866,     3,   868,   869,   870,   488,   872,
     173,   874,   176,     8,   877,   878,   777,   176,    17,     8,
     174,    20,   939,    32,   148,    35,    25,   174,   166,   892,
     174,   174,   148,   513,    25,   166,   166,    36,   279,   519,
     165,   381,   165,   862,  1055,  1055,   995,    46,    93,   351,
     530,  1020,   532,   554,    53,   816,   900,   568,   901,   915,
     911,   392,   366,   902,    63,   565,  1220,   930,  1064,  1230,
     933,   318,    71,  1223,   135,   889,   939,   381,   700,   942,
     551,    90,   812,    -1,    -1,    94,   753,    -1,   755,    88,
      -1,   758,    -1,   956,   957,    -1,    -1,  1118,  1119,    -1,
      -1,   100,    -1,    -1,   967,   104,   969,    -1,    -1,    -1,
      -1,   974,   111,    -1,   977,    -1,    -1,    -1,    -1,   118,
      -1,    -1,    -1,   603,    -1,    -1,   989,   607,    -1,    -1,
      -1,  1045,    -1,   142,   143,   615,   145,   146,   147,    -1,
     149,   150,   151,  1006,    -1,   154,    -1,  1010,    -1,    -1,
    1016,   160,   161,  1016,    -1,   164,    -1,    -1,   167,   168,
     169,    -1,  1025,    -1,  1027,    -1,    -1,  1030,    -1,   178,
    1033,  1186,  1187,    -1,    -1,  1190,  1191,  1040,    -1,  1042,
       2,    -1,    -1,    -1,   488,    -1,    -1,  1202,    -1,  1204,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1118,  1119,  1066,    26,    -1,    -1,    29,    -1,   513,
      -1,    -1,    -1,  1076,  1077,   519,  1079,  1080,    -1,  1082,
    1083,    -1,    -1,    -1,    -1,    -1,   530,    -1,   532,  1092,
    1093,    -1,  1095,    -1,  1097,    -1,  1099,  1100,  1152,    -1,
    1103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       2,    -1,    -1,    -1,    -1,  1118,  1119,    -1,    80,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,  1130,    -1,  1132,
      -1,    -1,    -1,  1136,    26,    -1,    -1,    29,    -1,  1142,
    1143,  1144,  1145,  1146,    -1,    12,   108,   767,    -1,    -1,
     957,   113,    19,    -1,    -1,    -1,    -1,    -1,    -1,   603,
      -1,    -1,    -1,   607,    -1,    32,    33,    -1,    -1,    -1,
      -1,   615,  1175,  1176,  1177,  1178,  1179,  1180,  1181,  1182,
     800,    48,    -1,   803,    -1,    -1,    -1,    -1,    80,    -1,
      -1,    -1,    84,    85,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1213,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,  1222,
      -1,   113,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
     850,    -1,    -1,    -1,    -1,    -1,   856,    -1,    -1,    -1,
    1243,  1244,  1245,  1246,    -1,  1231,    -1,    -1,  1251,  1252,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   219,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1271,  1272,
    1273,  1274,  1275,  1276,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   283,    -1,    -1,    -1,    -1,   288,   219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   956,   957,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   967,     2,   969,
      -1,    -1,    -1,    -1,   974,    -1,   800,   977,    32,   803,
      22,    -1,    -1,    -1,    26,    27,   986,    -1,   988,    -1,
      -1,   991,    26,    -1,    36,    29,    38,    39,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,   283,    -1,    -1,    -1,  1015,   288,    -1,    -1,    32,
      -1,    -1,    -1,    -1,   366,  1025,   850,  1027,    -1,    -1,
      -1,    -1,   856,    -1,    -1,    77,    90,    79,     2,   381,
      94,    83,    -1,    -1,    86,    -1,    80,    89,    -1,    91,
      84,    85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,    -1,    -1,    29,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   108,   117,    -1,    90,    -1,   113,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,
      -1,    -1,  1092,   147,   366,   149,   150,    -1,    -1,  1099,
     154,    -1,    -1,    32,    -1,    -1,    -1,   161,    -1,   381,
      -1,    -1,  1112,   167,   168,   169,    80,    -1,    -1,    -1,
      84,    85,    -1,    -1,   166,    -1,  1126,    -1,    -1,   142,
     143,    -1,   145,   146,   147,  1135,   149,   150,    -1,  1139,
      -1,   154,    -1,   967,   108,   969,   488,   160,   161,   113,
     974,   164,    -1,   977,   167,   168,   169,    -1,    -1,    -1,
      -1,    90,   986,    -1,   988,    94,    -1,   991,    -1,    -1,
      -1,   513,    -1,    -1,    -1,    -1,    -1,   519,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   219,    -1,    -1,   530,    -1,
     532,  1015,  1192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1025,    -1,  1027,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   488,   146,   147,    -1,
     149,   150,    -1,    -1,    -1,   154,    -1,    -1,    -1,    -1,
      -1,  1231,   161,    -1,    -1,    -1,    -1,    -1,   167,   168,
     169,   513,    -1,  1243,  1244,  1245,  1246,   519,    -1,   283,
      -1,  1251,  1252,    -1,   288,   219,    -1,    -1,   530,    -1,
     532,   603,    -1,    -1,    -1,   607,    -1,    -1,  1092,    -1,
       3,    -1,    -1,   615,    -1,  1099,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,  1112,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,  1126,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,  1135,    -1,    46,    -1,  1139,    -1,    50,    -1,   283,
      53,    -1,    -1,     1,   288,    -1,    -1,    -1,    -1,    -1,
      63,   603,   366,    32,    -1,   607,    -1,    70,    71,    -1,
      -1,    -1,    -1,   615,    22,    78,    -1,   381,    26,    27,
      -1,    -1,    -1,    31,    -1,    88,    -1,    -1,    36,    -1,
      38,    39,    -1,    -1,    97,    98,    44,   100,  1192,   102,
      -1,   104,    -1,    51,   107,    -1,    54,    -1,   111,   112,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    74,    -1,    -1,    77,
      -1,    79,   366,    81,    -1,    83,    84,  1231,    86,    -1,
      -1,    89,    -1,    91,    -1,    -1,    -1,   381,    -1,  1243,
    1244,  1245,  1246,    -1,    -1,    -1,    -1,  1251,  1252,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,   115,    -1,   117,
     173,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,    -1,    -1,   488,   154,    -1,    -1,   800,     2,
      -1,   803,   161,    -1,    -1,    -1,    -1,    -1,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   513,
      -1,    -1,    -1,    26,    -1,   519,    29,    -1,   166,    -1,
      -1,    -1,    -1,    -1,   172,    -1,   530,    -1,   532,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,   850,    26,
      27,    -1,    -1,    -1,   856,    -1,    -1,    -1,    -1,    36,
       2,    38,    39,    -1,   488,    -1,    -1,    44,   800,    -1,
      -1,   803,    -1,    -1,    51,    -1,    -1,    80,    -1,    -1,
      -1,    84,    85,    32,    26,    -1,    -1,    29,    -1,   513,
      -1,    -1,    -1,    -1,    -1,   519,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    -1,   108,   530,    -1,   532,   603,
     113,    -1,    89,   607,    91,    -1,    -1,    -1,   850,    -1,
      -1,   615,    -1,    -1,   856,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    80,    -1,
     117,    90,    84,    85,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   967,   108,   969,    -1,    -1,
      -1,   113,   974,    -1,    -1,   977,    -1,    -1,    -1,   603,
      -1,    -1,    -1,   607,   986,    -1,   988,    -1,    -1,   991,
      -1,   615,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,    -1,   154,   155,   156,    -1,   158,
     159,   160,   161,  1015,    -1,   164,   219,    -1,   167,   168,
     169,    -1,    -1,  1025,    -1,  1027,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,   967,    -1,   969,    -1,    -1,
      -1,    -1,   974,    -1,    22,   977,    -1,    -1,    26,    27,
      -1,    -1,    -1,    -1,   986,    -1,   988,    -1,    36,   991,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,    -1,    -1,    51,    -1,    -1,    -1,   219,    -1,    -1,
     283,    -1,    -1,  1015,    -1,   288,    -1,    -1,    66,    -1,
    1092,    -1,    -1,  1025,    -1,  1027,    -1,  1099,    -1,    77,
      -1,    79,    -1,    -1,    -1,    83,   800,    85,    86,   803,
    1112,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,  1135,    -1,   113,    -1,  1139,    -1,   117,
      -1,   283,    -1,    -1,    -1,    -1,   288,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   850,     2,    -1,    -1,
    1092,    -1,   856,   366,    -1,    -1,    -1,  1099,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   800,    -1,   381,   803,
    1112,    26,    -1,    -1,    29,    -1,    -1,    -1,   166,    -1,
    1192,    -1,    -1,    -1,  1126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1135,    -1,    -1,    -1,  1139,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,    -1,   366,    -1,   850,    -1,    -1,  1231,
      -1,    -1,   856,    -1,    -1,    80,    -1,    -1,    -1,   381,
      -1,  1243,  1244,  1245,  1246,    -1,    -1,    -1,    -1,  1251,
    1252,   170,    -1,    -1,   173,    -1,   175,    -1,    -1,    -1,
    1192,    -1,    -1,   108,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,   967,    -1,   969,    -1,    -1,    -1,    -1,
     974,    -1,    -1,   977,    -1,   488,    -1,    -1,    -1,    -1,
      -1,    -1,   986,    -1,   988,    -1,    -1,   991,    -1,  1231,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     513,  1243,  1244,  1245,  1246,    -1,   519,     2,    -1,  1251,
    1252,  1015,    -1,    -1,    -1,    -1,    -1,   530,    -1,   532,
      -1,  1025,    -1,  1027,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,   967,    29,   969,   488,    -1,    -1,    -1,
     974,    -1,    -1,   977,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   986,    -1,   988,    -1,    -1,   991,    -1,    -1,
      -1,   513,    -1,    -1,   219,    -1,    -1,   519,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   530,    -1,
     532,  1015,    -1,    -1,    -1,    80,    -1,    -1,  1092,    -1,
     603,  1025,    -1,  1027,   607,  1099,    -1,    -1,    -1,    -1,
      -1,    -1,   615,    -1,    -1,    -1,    -1,    -1,  1112,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,   113,    -1,
      -1,    -1,  1126,    -1,    -1,    -1,    -1,    -1,   283,    -1,
      -1,  1135,    -1,   288,    -1,  1139,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   603,    -1,    -1,    -1,   607,    -1,    -1,  1092,    -1,
      -1,    -1,    -1,   615,    -1,  1099,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1192,    -1,
      -1,    -1,  1126,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      -1,  1135,    -1,    -1,    -1,  1139,    -1,    -1,    -1,    -1,
      -1,   366,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    -1,    -1,   219,    -1,   381,  1231,    -1,    84,
      85,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1243,
    1244,  1245,  1246,    -1,    -1,    -1,    -1,  1251,  1252,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,  1192,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,   800,   283,    -1,
     803,    -1,    40,   288,    -1,    -1,    -1,  1231,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,  1243,
    1244,  1245,  1246,    -1,    -1,    63,    -1,  1251,  1252,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,   488,    -1,    -1,    -1,   850,    -1,    -1,
      88,    -1,    -1,   856,    -1,    -1,    -1,    -1,   800,    97,
      98,   803,   100,    -1,   102,    -1,   104,    -1,   513,   107,
      -1,    -1,    -1,   111,   519,    -1,   114,    -1,    -1,   224,
     118,   366,    -1,    -1,    -1,   530,    -1,   532,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   381,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   850,    -1,
      -1,    -1,    -1,    -1,   856,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   267,   268,   269,   270,   271,   272,    -1,    -1,
     275,   276,    -1,   171,    -1,   173,    -1,   282,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   291,    -1,   293,    -1,
      -1,    -1,    -1,    -1,    -1,   300,    -1,    -1,   603,    -1,
      -1,    -1,   607,    -1,   967,    -1,   969,    -1,    -1,    -1,
     615,   974,    -1,    -1,   977,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   986,    -1,   988,    -1,    -1,   991,    -1,
      -1,    -1,    -1,    -1,    -1,   340,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   488,    -1,    -1,    -1,    -1,    -1,    -1,
     355,   356,  1015,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1025,    -1,  1027,   967,    -1,   969,   513,    -1,
      -1,    -1,   974,    -1,   519,   977,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   986,   530,   988,    -1,    -1,   991,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    74,    -1,
      -1,    -1,    -1,  1015,    -1,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,  1025,    -1,  1027,    -1,    -1,    -1,  1092,
      -1,    17,    18,    -1,    20,    -1,  1099,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,  1112,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,   603,    -1,
      46,    -1,   607,  1126,    50,    -1,    -1,    53,    -1,    -1,
     615,    -1,  1135,    -1,    -1,    -1,  1139,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,   491,    -1,    -1,    -1,
    1092,    -1,    78,    -1,    -1,   800,    -1,  1099,   803,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1112,    97,    98,   518,   100,    -1,   102,   522,   104,   524,
      -1,   107,   527,    -1,  1126,   111,   112,    -1,   114,  1192,
      -1,    -1,   118,  1135,    -1,    -1,    -1,  1139,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   850,    -1,    -1,    -1,    -1,
      -1,   856,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1231,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1243,  1244,  1245,  1246,    -1,    -1,    -1,   173,  1251,  1252,
    1192,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   267,   268,   269,   270,   271,   272,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   291,    -1,   293,    -1,  1231,
      -1,    -1,    -1,    -1,   300,    -1,    -1,    -1,    -1,    -1,
      -1,  1243,  1244,  1245,  1246,    -1,    -1,    -1,    -1,  1251,
    1252,    -1,    -1,    -1,    -1,   800,     3,    -1,   803,    -1,
      -1,    -1,   967,    -1,   969,    -1,    -1,    -1,    -1,   974,
      17,    18,   977,    20,   340,    -1,    -1,    -1,    25,    -1,
      -1,   986,    31,   988,    -1,    -1,   991,    34,    -1,   355,
     356,    -1,    -1,    40,    -1,    -1,    45,    46,    47,    46,
      49,    -1,    -1,    50,    -1,   850,    53,    -1,    -1,    -1,
    1015,   856,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
    1025,    -1,  1027,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      97,    98,    -1,   100,    -1,   102,    -1,   104,    -1,    -1,
     107,    -1,    12,    -1,   111,    -1,    -1,   114,    -1,    19,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   784,
     785,    -1,    32,    33,    -1,   790,   791,  1092,    -1,   794,
     795,    -1,    -1,    -1,  1099,    -1,    -1,    -1,    48,   804,
      -1,    -1,    -1,    -1,   153,    -1,    -1,  1112,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,   168,
      -1,  1126,   967,    -1,   969,   491,   173,    -1,    -1,   974,
    1135,    -1,   977,    -1,  1139,   840,    -1,    -1,    -1,    -1,
      90,   986,    -1,   988,    94,    -1,   991,    -1,    -1,    -1,
      -1,    -1,   518,    -1,   203,    -1,   522,    -1,   524,    -1,
      -1,   527,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1015,    -1,   122,    -1,    -1,    -1,   126,    -1,    -1,    -1,
    1025,    -1,  1027,    -1,    -1,    -1,    -1,  1192,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,   265,   167,   168,   169,
      -1,   171,    -1,    -1,   273,   274,  1231,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,   284,    -1,    -1,  1243,  1244,
    1245,  1246,    -1,    -1,    -1,    -1,  1251,  1252,    -1,    -1,
      -1,    -1,    -1,   958,   959,    -1,    -1,    -1,    -1,   964,
      -1,   966,    -1,    -1,    -1,    -1,    -1,  1112,   973,    -1,
      -1,   976,    -1,    -1,   979,   324,   981,    -1,   983,    -1,
      -1,  1126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1135,   996,    -1,    -1,  1139,    -1,     1,    -1,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,  1023,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,   393,    50,    -1,  1192,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,   420,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,  1231,    -1,    -1,    -1,
      -1,    -1,    97,    98,    99,   100,   101,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      12,    -1,    -1,   118,    -1,    -1,    -1,    19,   784,   785,
      -1,   126,    -1,  1128,   790,   791,    -1,    -1,   794,   795,
      32,    -1,    -1,    -1,    -1,   484,    -1,   142,   143,   144,
     145,   146,    -1,   492,    -1,    -1,   151,    -1,    -1,   498,
      -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,
     165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,
     175,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1186,  1187,    -1,    -1,  1190,  1191,    -1,    90,    -1,
     539,    -1,    94,    -1,   543,  1200,    -1,  1202,    -1,  1204,
      -1,   103,   551,    -1,    -1,    -1,    -1,   556,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   565,    -1,    -1,    -1,
      -1,   570,    -1,    -1,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   171,
      -1,    -1,   621,    -1,    -1,    -1,   178,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   958,   959,    -1,    -1,    -1,    -1,   964,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     976,    -1,    -1,   979,    -1,   981,    -1,   983,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     0,     1,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,   700,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    32,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,    -1,    -1,   121,    32,   123,   805,    -1,   126,    -1,
      -1,    -1,  1128,   812,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,   822,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   840,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,
     178,    -1,    90,    -1,    -1,    -1,    94,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,    -1,   158,   159,   160,   161,   162,    -1,   164,
     889,   890,   167,   168,   169,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   915,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,    -1,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,   995,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,  1020,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,    -1,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,    -1,    -1,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
     172,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
      -1,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,   172,   173,    -1,   175,
      -1,     1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,   172,   173,    -1,   175,    -1,     1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,    -1,    -1,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,   172,   173,
      -1,   175,    -1,     1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,   172,   173,    -1,   175,    -1,     1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,    -1,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,    -1,    -1,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
      -1,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,     1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,    -1,    -1,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,     1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,    -1,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,    -1,    -1,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
      -1,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,     1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      -1,    91,    92,    93,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,    -1,   104,   105,    -1,   107,   108,   109,
     110,   111,    -1,   113,   114,   115,   116,   117,   118,    -1,
      -1,   121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,   166,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,    -1,     1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    -1,    91,    92,    93,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,   108,   109,   110,   111,    -1,   113,
     114,   115,   116,   117,   118,    -1,    -1,   121,    -1,   123,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,   166,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,     1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    -1,    91,    92,    93,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,    -1,   104,   105,    -1,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,   117,
     118,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,     1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    -1,    91,
      92,    93,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,    -1,   104,   105,    -1,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,   117,   118,    -1,    -1,   121,
      -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,   166,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,     1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    68,    -1,    70,    71,    72,    -1,    74,    -1,
      -1,    77,    78,    79,    80,    -1,    82,    83,    84,    85,
      86,    87,    88,    89,    -1,    91,    92,    93,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,    -1,   104,   105,
      -1,   107,   108,   109,   110,   111,    -1,   113,   114,   115,
     116,   117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
     166,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,   135,   136,   137,   138,    -1,
     140,   141,   142,   143,   144,   145,   146,   147,    -1,   149,
      -1,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    -1,   167,   168,   169,
     170,   171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    -1,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      64,    65,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,    -1,
     104,   105,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,   121,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,
     112,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
     152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    48,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    91,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,   117,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,   108,   109,
      -1,   111,   112,    -1,   114,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,   174,   175,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,   108,   109,    -1,   111,   112,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
     174,   175,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
     108,   109,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,   108,   109,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,    -1,   178,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,   117,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,   108,   109,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,   124,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,   176,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    90,    -1,
      -1,    -1,    94,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,    -1,   178,     3,     4,     5,
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
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,   124,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,    99,
     100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,   113,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,   176,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
     172,   173,    -1,   175,    -1,    -1,   178,     3,     4,     5,
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
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
     176,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
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
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,   176,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,   112,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,   112,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,    -1,   178,     3,     4,     5,
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
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
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
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,
     178,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,    -1,    -1,   178,     3,     4,     5,
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
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,
     146,    -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,
      -1,    -1,   178,     3,     4,     5,     6,     7,     8,     9,
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
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,
      -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,   169,
      -1,   171,    -1,   173,    -1,   175,    -1,    -1,   178,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,    -1,    -1,   178,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,    -1,
     178,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    -1,    18,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    41,    42,
      -1,    -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      -1,    -1,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    79,    80,    81,    82,
      -1,    -1,    -1,    -1,    87,    -1,    89,    -1,    91,    -1,
      -1,    -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,
     163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,   174,   175,    -1,    -1,   178,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,
      18,    -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    41,    42,    -1,    -1,    45,    -1,    -1,
      48,    -1,    -1,    -1,    52,    -1,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    -1,    -1,    65,    -1,    67,
      68,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,
      78,    79,    80,    81,    82,    -1,    -1,    -1,    -1,    87,
      -1,    89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    99,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   110,    -1,    -1,    -1,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,    -1,   142,   143,   144,   145,   146,    -1,
      12,    -1,    -1,   151,    -1,    17,    18,    19,    20,    -1,
      -1,    -1,   160,    25,    -1,   163,   164,   165,    -1,    -1,
      -1,   169,    34,   171,    36,   173,    -1,   175,    40,    -1,
     178,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,
     132,    -1,    -1,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,    -1,   149,    -1,    -1,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,    -1,    -1,   167,   168,   169,   170,    -1,
      -1,   173,     3,     4,     5,     6,     7,     8,     9,    10,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   160,
      -1,    -1,    -1,   164,   165,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    -1,   175,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,    -1,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,   144,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   165,    -1,    25,
      -1,    -1,    -1,   171,    -1,   173,    -1,   175,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,     3,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    12,    -1,
      -1,    87,    88,    89,    -1,    19,    20,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      34,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   165,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   173,    -1,   175,
     104,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,    -1,
      -1,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,    -1,   149,    -1,    -1,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,     3,
     164,    -1,    -1,   167,   168,   169,   170,   171,    12,    -1,
      -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    97,    98,    -1,   100,    -1,   102,    19,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    32,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   127,   128,   129,   130,   131,   132,    -1,
      -1,   135,   136,   137,   138,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,    -1,   149,    -1,    -1,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,    -1,    -1,   167,   168,   169,   170,    -1,    12,    -1,
      90,    -1,    -1,    -1,    94,    19,    -1,    -1,    -1,    -1,
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
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,    -1,    -1,   135,   136,   137,
     138,    -1,   140,   141,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    90,   167,
     168,   169,    94,   171,    -1,    12,    -1,    -1,    -1,    -1,
     178,   103,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,   131,
     132,    -1,    -1,   135,   136,   137,   138,    -1,   140,   141,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   171,
      -1,    12,    -1,    90,    -1,    -1,   178,    94,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
     127,   128,   129,   130,   131,   132,    -1,    -1,   135,   136,
     137,   138,    -1,   140,   141,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    90,
     167,   168,   169,    94,   171,    -1,    12,    -1,    -1,    -1,
      -1,   178,   103,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,
     131,   132,    48,    -1,   135,   136,   137,   138,    -1,   140,
     141,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
     171,    -1,    -1,    -1,    90,    -1,    -1,   178,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
     126,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    73,
      -1,   167,   168,   169,    -1,   171,    -1,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,   126,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    73,   166,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    12,    -1,    90,    -1,
      -1,    -1,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    90,   167,   168,   169,    94,    -1,
      -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,   122,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,    90,
      -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,    -1,    48,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
     171,    -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,   122,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    12,    -1,
      90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,   126,    -1,    -1,    -1,
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
     164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,    12,
      -1,    90,    -1,    -1,   178,    94,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    90,   167,   168,
     169,    94,    -1,    -1,    12,    -1,    -1,    -1,    -1,   178,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,   171,    -1,
      12,    -1,    90,    -1,    -1,   178,    94,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    90,   167,
     168,   169,    94,   171,    -1,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    32,    -1,   126,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    73,    -1,   167,   168,   169,    -1,    -1,
      -1,    -1,    -1,    -1,   176,    -1,   178,    -1,    12,    13,
      90,    -1,    -1,    -1,    94,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    32,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,    -1,    -1,    -1,    12,    -1,    -1,    -1,   178,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   126,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,   178,    -1,    94,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    12,   167,   168,   169,    -1,   171,   126,    19,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    12,   166,   167,
     168,   169,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,   142,   143,    -1,   145,   146,   147,   103,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   126,    -1,    12,    13,    90,    -1,    -1,   178,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    32,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   126,   167,   168,   169,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   178,    19,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    32,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    90,   167,   168,   169,    94,    -1,    -1,    -1,   174,
      -1,    -1,    12,   178,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   122,   167,   168,
     169,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   178,
      90,    -1,    -1,    -1,    94,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    12,   167,   168,   169,    -1,   126,    -1,    19,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,   142,   143,    35,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    12,   166,   167,   168,   169,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,   126,
      -1,    -1,    90,    -1,    -1,    12,    94,   178,    -1,    -1,
      -1,    -1,    19,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    32,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   126,    -1,
     167,   168,   169,    -1,    -1,    -1,    -1,   174,    -1,    -1,
      -1,   178,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,    90,    -1,    -1,   174,    94,    12,    13,
     178,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,    -1,    -1,    -1,    90,    -1,    12,    -1,
      94,   178,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    90,    -1,    -1,    12,
      94,    -1,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,    90,    -1,    12,
     174,    94,    -1,    -1,   178,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,    -1,    12,
      13,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   126,   167,   168,   169,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   126,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,   178,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    61,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   126,   167,   168,   169,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   126,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    90,    -1,    12,
      -1,    94,    -1,    -1,    -1,   178,    19,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   126,   167,   168,   169,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   126,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    22,   167,   168,   169,    26,    27,    -1,
      -1,    -1,    31,    -1,    -1,   178,    -1,    36,    -1,    38,
      39,    -1,    -1,    -1,    -1,    44,    22,    -1,    -1,    -1,
      26,    27,    51,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    86,    -1,    -1,
      89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      86,   110,    -1,    89,   113,    91,   115,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,
      22,   117,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    -1,   166,    -1,    51,
      -1,    -1,    -1,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
     166,    -1,    -1,    -1,    -1,    77,   172,    79,    -1,    81,
      -1,    83,    -1,    -1,    86,    -1,    -1,    89,    -1,    91,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,   115,    -1,   117,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,    -1,   158,   159,   160,   161,   162,    -1,   164,    -1,
      22,   167,   168,   169,    26,    27,    -1,    -1,    -1,    31,
      -1,    -1,   178,    -1,    36,    -1,    38,    39,    -1,    -1,
      -1,    -1,    44,    -1,   166,    -1,    -1,    -1,    -1,    51,
     172,    -1,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    77,    25,    79,    -1,    81,
      -1,    83,    31,    -1,    86,    34,    -1,    89,    -1,    91,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,   115,    63,   117,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    97,    98,
      -1,   100,    25,   102,    27,   104,    -1,    -1,   107,    -1,
      -1,    34,   111,    -1,   166,   114,   115,    40,    -1,   118,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,     3,
      -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    88,    20,    -1,    91,    -1,
      -1,    25,    -1,    -1,    97,    98,    -1,   100,    -1,   102,
      34,   104,    -1,    -1,   107,    -1,    40,    -1,   111,    -1,
      -1,   114,    46,    -1,   117,   118,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    97,    98,    -1,   100,    -1,   102,    -1,
     104,    17,    18,   107,    20,    -1,    -1,   111,   112,    25,
     114,    -1,    -1,    -1,   118,    31,    -1,    -1,    34,    -1,
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
      61,    33,   166,   201,   221,   316,   317,   371,   371,   371,
     371,   371,   371,   103,   103,   103,   103,   187,   174,   174,
     202,   103,   103,   166,   166,   166,   166,   371,   228,   228,
     317,   371,   321,   322,   322,   322,   322,   322,   322,   320,
     185,   103,   103,   103,   103,   103,   103,   371,   371,   371,
     371,   165,   165,   371,   371
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
     314,   314,   315,   315,   316,   316,   317,   317,   317,   318,
     319,   319,   319,   319,   319,   320,   320,   321,   321,   321,
     321,   321,   321,   321,   322,   322,   323,   324,   324,   325,
     325,   326,   327,   327,   328,   328,   328,   328,   328,   330,
     329,   329,   331,   331,   332,   332,   333,   333,   333,   334,
     334,   334,   335,   335,   335,   336,   337,   337,   337,   338,
     338,   339,   339,   340,   340,   340,   340,   341,   341,   342,
     343,   343,   344,   344,   345,   345,   346,   346,   347,   347,
     348,   348,   349,   349,   350,   350,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     353,   353,   353,   353,   353,   353,   353,   354,   355,   356,
     356,   356,   356,   356,   356,   356,   357,   357,   358,   359,
     359,   360,   361,   361,   362,   362,   362,   363,   363,   363,
     363,   363,   363,   364,   364,   364,   364,   364,   365,   365,
     365,   365,   365,   366,   367,   367,   367,   367,   367,   367,
     368,   369,   370,   370,   371,   371,   371,   371,   371,   371,
     371,   371,   371,   371,   371,   371,   371,   371,   371,   371,
     371,   372,   372,   373,   373,   373,   374,   374,   374,   374,
     375,   375,   375,   375,   375,   376,   376,   376,   377,   377,
     377,   377,   377,   377,   378,   378,   378,   378,   379,   379,
     380,   380,   381,   381,   381,   381,   381,   381,   381,   381,
     381,   381,   381,   381,   381,   382,   382,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     384,   384,   384,   384,   384,   384,   384,   385,   385,   385,
     385,   386,   386,   386,   386,   387,   387,   387,   387,   387,
     387,   387
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
       1,     1,     0,     1,     1,     1,     1,     2,     1,     1,
       0,     2,     2,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     3,     1,     2,     1,
       3,     2,     0,     2,     1,     2,     1,     1,     1,     0,
       5,     3,     1,     3,     3,     5,     1,     1,     3,     1,
       2,     3,     0,     2,     2,     1,     1,     1,     1,     2,
       1,     0,     1,     0,     2,     2,     1,     1,     1,     2,
       0,     1,     1,     3,     1,     1,     3,     3,     0,     1,
       1,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     5,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     1,     1,     6,     6,
       4,     9,     9,     7,     6,     6,     4,     9,     9,     7,
       2,     3,     4,     6,     6,     9,     9,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     4,     1,
       3,     4,     1,     3,     4,     3,     3,     1,     1,     2,
       1,     2,     1,     1,     2,     2,     2,     2,     2,     8,
       8,     9,     9,     4,     3,     3,     2,     2,     2,     1,
       3,     4,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     2,     2,     1,     1,     4,     4,     4,     3,     3,
       3,     3,     5,     5,     3,     4,     3,     4,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       4,     3,     4,     3,     4,     3,     5,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     2,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     1,     1,     1,     1,     1,
       1,     1
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
#line 6112 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 637 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6118 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 638 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6124 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 645 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6132 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 653 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6140 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 657 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6149 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 665 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6155 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: unstable_decl_stmt  */
#line 666 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6161 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: include_module_stmt  */
#line 667 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6167 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: block_stmt  */
#line 668 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6173 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: use_stmt  */
#line 669 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6179 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: import_stmt  */
#line 670 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6185 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: require_stmt  */
#line 671 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6191 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: extern_block_stmt  */
#line 672 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6197 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 673 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6203 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 674 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6209 "bison-chpl-lib.cpp"
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
#line 6226 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: try_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6232 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: return_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6238 "bison-chpl-lib.cpp"
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
#line 6251 "bison-chpl-lib.cpp"
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
#line 6264 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TLABEL ident_def stmt  */
#line 709 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6272 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: TYIELD opt_try_expr TSEMI  */
#line 713 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6283 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: error TSEMI  */
#line 720 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6292 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: assignment_stmt  */
#line 727 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6298 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: if_stmt  */
#line 728 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6304 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: loop_stmt  */
#line 729 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6310 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: select_stmt  */
#line 730 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6316 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: manage_stmt  */
#line 731 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6322 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 732 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6328 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: throw_stmt  */
#line 733 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6334 "bison-chpl-lib.cpp"
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
#line 6352 "bison-chpl-lib.cpp"
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
#line 6371 "bison-chpl-lib.cpp"
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
#line 6383 "bison-chpl-lib.cpp"
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
#line 6399 "bison-chpl-lib.cpp"
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
#line 6414 "bison-chpl-lib.cpp"
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
#line 6430 "bison-chpl-lib.cpp"
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
#line 6446 "bison-chpl-lib.cpp"
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
#line 6461 "bison-chpl-lib.cpp"
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
#line 6478 "bison-chpl-lib.cpp"
    break;

  case 45: /* $@1: %empty  */
#line 847 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6486 "bison-chpl-lib.cpp"
    break;

  case 46: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 decl_base  */
#line 851 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6494 "bison-chpl-lib.cpp"
    break;

  case 47: /* $@2: %empty  */
#line 855 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6502 "bison-chpl-lib.cpp"
    break;

  case 48: /* deprecated_decl_stmt: TDEPRECATED $@2 decl_base  */
#line 859 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6510 "bison-chpl-lib.cpp"
    break;

  case 51: /* $@3: %empty  */
#line 871 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 6518 "bison-chpl-lib.cpp"
    break;

  case 52: /* unstable_decl_stmt: TUNSTABLE STRINGLITERAL $@3 decl_base  */
#line 875 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6526 "bison-chpl-lib.cpp"
    break;

  case 53: /* $@4: %empty  */
#line 879 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 6534 "bison-chpl-lib.cpp"
    break;

  case 54: /* unstable_decl_stmt: TUNSTABLE $@4 decl_base  */
#line 883 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6542 "bison-chpl-lib.cpp"
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
#line 6563 "bison-chpl-lib.cpp"
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
#line 6582 "bison-chpl-lib.cpp"
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
#line 6601 "bison-chpl-lib.cpp"
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
#line 6618 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_access_control: %empty  */
#line 955 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6624 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_access_control: TPUBLIC  */
#line 956 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6631 "bison-chpl-lib.cpp"
    break;

  case 61: /* opt_access_control: TPRIVATE  */
#line 958 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6638 "bison-chpl-lib.cpp"
    break;

  case 62: /* opt_prototype: %empty  */
#line 963 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6644 "bison-chpl-lib.cpp"
    break;

  case 63: /* opt_prototype: TPROTOTYPE  */
#line 964 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6651 "bison-chpl-lib.cpp"
    break;

  case 64: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 970 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6661 "bison-chpl-lib.cpp"
    break;

  case 65: /* block_stmt_body: TLCBR TRCBR  */
#line 988 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6669 "bison-chpl-lib.cpp"
    break;

  case 66: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 992 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6677 "bison-chpl-lib.cpp"
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
#line 6689 "bison-chpl-lib.cpp"
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
#line 6708 "bison-chpl-lib.cpp"
    break;

  case 69: /* stmt_ls: toplevel_stmt  */
#line 1025 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6714 "bison-chpl-lib.cpp"
    break;

  case 70: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1026 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6720 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: expr  */
#line 1031 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6728 "bison-chpl-lib.cpp"
    break;

  case 72: /* renames_ls: all_op_name  */
#line 1035 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6736 "bison-chpl-lib.cpp"
    break;

  case 73: /* renames_ls: expr TAS expr  */
#line 1039 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6745 "bison-chpl-lib.cpp"
    break;

  case 74: /* renames_ls: renames_ls TCOMMA expr  */
#line 1044 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6753 "bison-chpl-lib.cpp"
    break;

  case 75: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1048 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6761 "bison-chpl-lib.cpp"
    break;

  case 76: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1052 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6770 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: expr  */
#line 1062 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6779 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_renames_ls: expr TAS expr  */
#line 1067 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6789 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1073 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6800 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1080 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6809 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1085 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6820 "bison-chpl-lib.cpp"
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
#line 6832 "bison-chpl-lib.cpp"
    break;

  case 83: /* opt_only_ls: %empty  */
#line 1102 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6838 "bison-chpl-lib.cpp"
    break;

  case 84: /* opt_only_ls: renames_ls  */
#line 1103 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6844 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1108 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6853 "bison-chpl-lib.cpp"
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
#line 6865 "bison-chpl-lib.cpp"
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
#line 6878 "bison-chpl-lib.cpp"
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
#line 6892 "bison-chpl-lib.cpp"
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
#line 6904 "bison-chpl-lib.cpp"
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
#line 6917 "bison-chpl-lib.cpp"
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
#line 6931 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1170 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6940 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_expr: expr  */
#line 1178 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 6948 "bison-chpl-lib.cpp"
    break;

  case 94: /* import_expr: expr TDOT all_op_name  */
#line 1182 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 6957 "bison-chpl-lib.cpp"
    break;

  case 95: /* import_expr: expr TAS ident_use  */
#line 1187 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 6967 "bison-chpl-lib.cpp"
    break;

  case 96: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1193 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 6977 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_ls: import_expr  */
#line 1201 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 6983 "bison-chpl-lib.cpp"
    break;

  case 98: /* import_ls: import_ls TCOMMA import_expr  */
#line 1202 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 6989 "bison-chpl-lib.cpp"
    break;

  case 99: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1207 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1216 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7007 "bison-chpl-lib.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1220 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7015 "bison-chpl-lib.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1224 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7023 "bison-chpl-lib.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1228 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7031 "bison-chpl-lib.cpp"
    break;

  case 104: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1232 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7039 "bison-chpl-lib.cpp"
    break;

  case 105: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1236 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 106: /* opt_label_ident: %empty  */
#line 1244 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 107: /* opt_label_ident: TIDENT  */
#line 1245 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_use: TIDENT  */
#line 1270 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_use: TTHIS  */
#line 1271 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TBOOL  */
#line 1298 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7077 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TENUM  */
#line 1299 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TINT  */
#line 1300 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7089 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TUINT  */
#line 1301 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TREAL  */
#line 1302 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TIMAG  */
#line 1303 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7107 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TCOMPLEX  */
#line 1304 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TBYTES  */
#line 1305 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TSTRING  */
#line 1306 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7125 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TLOCALE  */
#line 1307 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7131 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TNOTHING  */
#line 1308 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7137 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TVOID  */
#line 1309 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7143 "bison-chpl-lib.cpp"
    break;

  case 152: /* do_stmt: TDO stmt  */
#line 1323 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7149 "bison-chpl-lib.cpp"
    break;

  case 153: /* do_stmt: block_stmt  */
#line 1324 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7155 "bison-chpl-lib.cpp"
    break;

  case 154: /* return_stmt: TRETURN TSEMI  */
#line 1329 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7166 "bison-chpl-lib.cpp"
    break;

  case 155: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1336 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7177 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@5: %empty  */
#line 1347 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7185 "bison-chpl-lib.cpp"
    break;

  case 158: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@5 class_level_stmt  */
#line 1351 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7193 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@6: %empty  */
#line 1355 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7201 "bison-chpl-lib.cpp"
    break;

  case 160: /* deprecated_class_level_stmt: TDEPRECATED $@6 class_level_stmt  */
#line 1359 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7209 "bison-chpl-lib.cpp"
    break;

  case 161: /* $@7: %empty  */
#line 1366 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 7217 "bison-chpl-lib.cpp"
    break;

  case 162: /* unstable_class_level_stmt: TUNSTABLE STRINGLITERAL $@7 class_level_stmt  */
#line 1370 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7225 "bison-chpl-lib.cpp"
    break;

  case 163: /* $@8: %empty  */
#line 1374 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 7233 "bison-chpl-lib.cpp"
    break;

  case 164: /* unstable_class_level_stmt: TUNSTABLE $@8 class_level_stmt  */
#line 1378 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7241 "bison-chpl-lib.cpp"
    break;

  case 165: /* class_level_stmt: TSEMI  */
#line 1385 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7249 "bison-chpl-lib.cpp"
    break;

  case 166: /* class_level_stmt: inner_class_level_stmt  */
#line 1389 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7259 "bison-chpl-lib.cpp"
    break;

  case 167: /* $@9: %empty  */
#line 1394 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7266 "bison-chpl-lib.cpp"
    break;

  case 168: /* class_level_stmt: TPUBLIC $@9 inner_class_level_stmt  */
#line 1396 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7275 "bison-chpl-lib.cpp"
    break;

  case 169: /* $@10: %empty  */
#line 1400 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7282 "bison-chpl-lib.cpp"
    break;

  case 170: /* class_level_stmt: TPRIVATE $@10 inner_class_level_stmt  */
#line 1402 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7291 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1420 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7300 "bison-chpl-lib.cpp"
    break;

  case 179: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1425 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7309 "bison-chpl-lib.cpp"
    break;

  case 180: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1430 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7318 "bison-chpl-lib.cpp"
    break;

  case 181: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1435 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7326 "bison-chpl-lib.cpp"
    break;

  case 182: /* forwarding_decl_start: TFORWARDING  */
#line 1442 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7335 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_or_export: TEXTERN  */
#line 1449 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7341 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_or_export: TEXPORT  */
#line 1450 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7347 "bison-chpl-lib.cpp"
    break;

  case 185: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1455 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7357 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1465 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7366 "bison-chpl-lib.cpp"
    break;

  case 187: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1471 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7377 "bison-chpl-lib.cpp"
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
#line 7399 "bison-chpl-lib.cpp"
    break;

  case 189: /* $@11: %empty  */
#line 1496 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7407 "bison-chpl-lib.cpp"
    break;

  case 190: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1500 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7416 "bison-chpl-lib.cpp"
    break;

  case 191: /* $@12: %empty  */
#line 1505 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7425 "bison-chpl-lib.cpp"
    break;

  case 192: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@12 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1510 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7434 "bison-chpl-lib.cpp"
    break;

  case 193: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1518 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7442 "bison-chpl-lib.cpp"
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
#line 7464 "bison-chpl-lib.cpp"
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
#line 7481 "bison-chpl-lib.cpp"
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
#line 7498 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1569 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7506 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1573 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7514 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1577 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7522 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1581 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7530 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1585 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7538 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOR expr do_stmt  */
#line 1589 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1593 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7554 "bison-chpl-lib.cpp"
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
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1615 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7584 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1619 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7592 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1623 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7600 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1627 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7608 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFORALL expr do_stmt  */
#line 1631 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7616 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1635 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7624 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7632 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1643 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7640 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7656 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7664 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7672 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7680 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7688 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7696 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7704 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1679 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7712 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1683 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7720 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1687 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7728 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1691 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7736 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1695 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7744 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7752 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1703 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7761 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1708 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7770 "bison-chpl-lib.cpp"
    break;

  case 229: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1716 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7780 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF expr TTHEN stmt  */
#line 1725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7788 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF expr block_stmt  */
#line 1729 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1733 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7804 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1737 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7812 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1741 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7820 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF ifvar block_stmt  */
#line 1745 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7828 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1749 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7836 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1753 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7844 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1757 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7853 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1762 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7862 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1767 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7871 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1772 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7880 "bison-chpl-lib.cpp"
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
#line 7899 "bison-chpl-lib.cpp"
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
#line 7918 "bison-chpl-lib.cpp"
    break;

  case 244: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1813 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7926 "bison-chpl-lib.cpp"
    break;

  case 245: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1817 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 7934 "bison-chpl-lib.cpp"
    break;

  case 246: /* ifc_formal_ls: ifc_formal  */
#line 1823 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7940 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1824 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7946 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifc_formal: ident_def  */
#line 1829 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 7952 "bison-chpl-lib.cpp"
    break;

  case 261: /* implements_type_ident: implements_type_error_ident  */
#line 1847 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 7961 "bison-chpl-lib.cpp"
    break;

  case 268: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1873 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7969 "bison-chpl-lib.cpp"
    break;

  case 269: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1877 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 7977 "bison-chpl-lib.cpp"
    break;

  case 270: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1881 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 7985 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1888 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 7993 "bison-chpl-lib.cpp"
    break;

  case 272: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1892 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8001 "bison-chpl-lib.cpp"
    break;

  case 273: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1896 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8009 "bison-chpl-lib.cpp"
    break;

  case 274: /* try_stmt: TTRY tryable_stmt  */
#line 1903 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8017 "bison-chpl-lib.cpp"
    break;

  case 275: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1907 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8025 "bison-chpl-lib.cpp"
    break;

  case 276: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1911 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8033 "bison-chpl-lib.cpp"
    break;

  case 277: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1915 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8041 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_ls: %empty  */
#line 1921 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8047 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1922 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8053 "bison-chpl-lib.cpp"
    break;

  case 280: /* catch_expr: TCATCH block_stmt  */
#line 1927 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8061 "bison-chpl-lib.cpp"
    break;

  case 281: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1931 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8069 "bison-chpl-lib.cpp"
    break;

  case 282: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1935 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8077 "bison-chpl-lib.cpp"
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
#line 8094 "bison-chpl-lib.cpp"
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
#line 8111 "bison-chpl-lib.cpp"
    break;

  case 285: /* throw_stmt: TTHROW expr TSEMI  */
#line 1971 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8121 "bison-chpl-lib.cpp"
    break;

  case 286: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1979 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8129 "bison-chpl-lib.cpp"
    break;

  case 287: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1983 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8139 "bison-chpl-lib.cpp"
    break;

  case 288: /* when_stmt_ls: %empty  */
#line 1991 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8145 "bison-chpl-lib.cpp"
    break;

  case 289: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1992 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8151 "bison-chpl-lib.cpp"
    break;

  case 290: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1997 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8159 "bison-chpl-lib.cpp"
    break;

  case 291: /* when_stmt: TOTHERWISE stmt  */
#line 2001 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8168 "bison-chpl-lib.cpp"
    break;

  case 292: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2006 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8177 "bison-chpl-lib.cpp"
    break;

  case 293: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2014 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 294: /* manager_expr: expr TAS ident_def  */
#line 2019 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8194 "bison-chpl-lib.cpp"
    break;

  case 295: /* manager_expr: expr  */
#line 2023 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8202 "bison-chpl-lib.cpp"
    break;

  case 296: /* manager_expr_ls: manager_expr  */
#line 2029 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8208 "bison-chpl-lib.cpp"
    break;

  case 297: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2030 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8214 "bison-chpl-lib.cpp"
    break;

  case 298: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2035 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8222 "bison-chpl-lib.cpp"
    break;

  case 299: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2044 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 300: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2049 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8242 "bison-chpl-lib.cpp"
    break;

  case 301: /* class_start: class_tag ident_def  */
#line 2060 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8250 "bison-chpl-lib.cpp"
    break;

  case 302: /* class_tag: TCLASS  */
#line 2066 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8256 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_tag: TRECORD  */
#line 2067 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8262 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_tag: TUNION  */
#line 2068 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8268 "bison-chpl-lib.cpp"
    break;

  case 305: /* opt_inherit: %empty  */
#line 2072 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8274 "bison-chpl-lib.cpp"
    break;

  case 306: /* opt_inherit: TCOLON expr_ls  */
#line 2073 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8280 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_level_stmt_ls: %empty  */
#line 2077 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8289 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2082 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8297 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_level_stmt_ls: class_level_stmt_ls unstable_class_level_stmt  */
#line 2086 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8305 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2090 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8313 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls unstable_class_level_stmt  */
#line 2094 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8321 "bison-chpl-lib.cpp"
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
#line 8342 "bison-chpl-lib.cpp"
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
#line 8356 "bison-chpl-lib.cpp"
    break;

  case 314: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2131 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8364 "bison-chpl-lib.cpp"
    break;

  case 315: /* enum_ls: deprecated_enum_item  */
#line 2138 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8373 "bison-chpl-lib.cpp"
    break;

  case 316: /* enum_ls: enum_ls TCOMMA  */
#line 2143 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8383 "bison-chpl-lib.cpp"
    break;

  case 317: /* $@13: %empty  */
#line 2149 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8392 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_ls: enum_ls TCOMMA $@13 deprecated_enum_item  */
#line 2154 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8401 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_ls: unstable_enum_item  */
#line 2160 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8410 "bison-chpl-lib.cpp"
    break;

  case 320: /* $@14: %empty  */
#line 2165 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8419 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_ls TCOMMA $@14 unstable_enum_item  */
#line 2170 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8428 "bison-chpl-lib.cpp"
    break;

  case 323: /* $@15: %empty  */
#line 2180 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8436 "bison-chpl-lib.cpp"
    break;

  case 324: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@15 enum_item  */
#line 2184 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8444 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@16: %empty  */
#line 2188 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8452 "bison-chpl-lib.cpp"
    break;

  case 326: /* deprecated_enum_item: TDEPRECATED $@16 enum_item  */
#line 2192 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8460 "bison-chpl-lib.cpp"
    break;

  case 327: /* $@17: %empty  */
#line 2199 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 8468 "bison-chpl-lib.cpp"
    break;

  case 328: /* unstable_enum_item: TUNSTABLE STRINGLITERAL $@17 enum_item  */
#line 2203 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8476 "bison-chpl-lib.cpp"
    break;

  case 329: /* $@18: %empty  */
#line 2207 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 8484 "bison-chpl-lib.cpp"
    break;

  case 330: /* unstable_enum_item: TUNSTABLE $@18 enum_item  */
#line 2211 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8492 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_item: ident_def  */
#line 2218 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8503 "bison-chpl-lib.cpp"
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
#line 8516 "bison-chpl-lib.cpp"
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
#line 8532 "bison-chpl-lib.cpp"
    break;

  case 334: /* $@19: %empty  */
#line 2254 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8542 "bison-chpl-lib.cpp"
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
#line 8564 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: linkage_spec_empty  */
#line 2283 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8570 "bison-chpl-lib.cpp"
    break;

  case 338: /* linkage_spec: TINLINE  */
#line 2284 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8577 "bison-chpl-lib.cpp"
    break;

  case 339: /* linkage_spec: TOVERRIDE  */
#line 2286 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 340: /* opt_fn_type_formal_ls: %empty  */
#line 2291 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 341: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2292 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal_ls: fn_type_formal  */
#line 2296 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2297 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: named_formal  */
#line 2302 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8614 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2305 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type_formal: formal_type  */
#line 2307 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_ret_type: %empty  */
#line 2311 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8632 "bison-chpl-lib.cpp"
    break;

  case 348: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2312 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8638 "bison-chpl-lib.cpp"
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
#line 8657 "bison-chpl-lib.cpp"
    break;

  case 350: /* $@20: %empty  */
#line 2343 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8667 "bison-chpl-lib.cpp"
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
#line 8680 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@21: %empty  */
#line 2358 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8690 "bison-chpl-lib.cpp"
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
#line 8703 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2376 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8711 "bison-chpl-lib.cpp"
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
#line 8725 "bison-chpl-lib.cpp"
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
#line 8748 "bison-chpl-lib.cpp"
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
#line 8760 "bison-chpl-lib.cpp"
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
#line 8772 "bison-chpl-lib.cpp"
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
#line 8787 "bison-chpl-lib.cpp"
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
#line 8802 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2455 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8812 "bison-chpl-lib.cpp"
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
#line 8828 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2479 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8834 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_ident: ident_def TBANG  */
#line 2486 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8844 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2543 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 408: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2544 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8856 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_formal_ls: %empty  */
#line 2548 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8862 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2549 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8868 "bison-chpl-lib.cpp"
    break;

  case 411: /* req_formal_ls: TLP TRP  */
#line 2553 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8874 "bison-chpl-lib.cpp"
    break;

  case 412: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2554 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8880 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal_ls_inner: formal  */
#line 2558 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8886 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2559 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls: %empty  */
#line 2563 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8898 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_ls: formal_ls_inner  */
#line 2564 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8904 "bison-chpl-lib.cpp"
    break;

  case 420: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2578 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8912 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2583 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2588 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8928 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2593 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8936 "bison-chpl-lib.cpp"
    break;

  case 424: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2598 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 425: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2603 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8952 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_formal_intent_tag: %empty  */
#line 2609 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8961 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_formal_intent_tag: required_intent_tag  */
#line 2614 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8970 "bison-chpl-lib.cpp"
    break;

  case 428: /* required_intent_tag: TIN  */
#line 2621 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8976 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TINOUT  */
#line 2622 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8982 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TOUT  */
#line 2623 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TCONST TIN  */
#line 2624 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8994 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TCONST TREF  */
#line 2625 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9000 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TCONST  */
#line 2626 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TPARAM  */
#line 2627 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TREF  */
#line 2628 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TTYPE  */
#line 2629 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_this_intent_tag: %empty  */
#line 2633 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: TPARAM  */
#line 2634 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TREF  */
#line 2635 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TCONST TREF  */
#line 2636 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TCONST  */
#line 2637 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TTYPE  */
#line 2638 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 443: /* proc_iter_or_op: TPROC  */
#line 2642 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TITER  */
#line 2643 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 445: /* proc_iter_or_op: TOPERATOR  */
#line 2644 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_ret_tag: %empty  */
#line 2648 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: TCONST  */
#line 2649 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TCONST TREF  */
#line 2650 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TREF  */
#line 2651 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TPARAM  */
#line 2652 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TTYPE  */
#line 2653 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_throws_error: %empty  */
#line 2657 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_throws_error: TTHROWS  */
#line 2658 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_function_body_stmt: TSEMI  */
#line 2661 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_function_body_stmt: function_body_stmt  */
#line 2662 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 456: /* function_body_stmt: block_stmt_body  */
#line 2666 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 457: /* function_body_stmt: TDO toplevel_stmt  */
#line 2667 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 458: /* function_body_stmt: return_stmt  */
#line 2668 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9156 "bison-chpl-lib.cpp"
    break;

  case 459: /* query_expr: TQUERIEDIDENT  */
#line 2672 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_lifetime_where: %empty  */
#line 2677 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_lifetime_where: TWHERE expr  */
#line 2679 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2681 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9180 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2683 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9186 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2685 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_components_expr: lifetime_expr  */
#line 2690 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9198 "bison-chpl-lib.cpp"
    break;

  case 466: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2692 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 467: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2697 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 468: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2699 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9216 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2701 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2703 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9228 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2705 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9234 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2707 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9240 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2709 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9246 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_ident: TIDENT  */
#line 2713 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9252 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_ident: TTHIS  */
#line 2714 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9258 "bison-chpl-lib.cpp"
    break;

  case 476: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2719 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9267 "bison-chpl-lib.cpp"
    break;

  case 477: /* type_alias_decl_stmt_start: TTYPE  */
#line 2728 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9275 "bison-chpl-lib.cpp"
    break;

  case 478: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2732 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9284 "bison-chpl-lib.cpp"
    break;

  case 479: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2740 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9292 "bison-chpl-lib.cpp"
    break;

  case 480: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2744 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9300 "bison-chpl-lib.cpp"
    break;

  case 481: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2751 "chpl.ypp"
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
#line 9322 "bison-chpl-lib.cpp"
    break;

  case 482: /* opt_init_type: %empty  */
#line 2771 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9328 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_init_type: TASSIGN expr  */
#line 2773 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9334 "bison-chpl-lib.cpp"
    break;

  case 484: /* var_decl_type: TPARAM  */
#line 2777 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9340 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_type: TCONST TREF  */
#line 2778 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9346 "bison-chpl-lib.cpp"
    break;

  case 486: /* var_decl_type: TREF  */
#line 2779 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9352 "bison-chpl-lib.cpp"
    break;

  case 487: /* var_decl_type: TCONST  */
#line 2780 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9358 "bison-chpl-lib.cpp"
    break;

  case 488: /* var_decl_type: TVAR  */
#line 2781 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9364 "bison-chpl-lib.cpp"
    break;

  case 489: /* $@23: %empty  */
#line 2786 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9373 "bison-chpl-lib.cpp"
    break;

  case 490: /* var_decl_stmt: TCONFIG $@23 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2790 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9382 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2795 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9391 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2803 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2807 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2814 "chpl.ypp"
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
#line 9427 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2830 "chpl.ypp"
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
#line 9445 "bison-chpl-lib.cpp"
    break;

  case 496: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2847 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 497: /* tuple_var_decl_component: ident_def  */
#line 2851 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9461 "bison-chpl-lib.cpp"
    break;

  case 498: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2855 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9469 "bison-chpl-lib.cpp"
    break;

  case 499: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2862 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2864 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9481 "bison-chpl-lib.cpp"
    break;

  case 501: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2866 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9487 "bison-chpl-lib.cpp"
    break;

  case 502: /* opt_init_expr: %empty  */
#line 2872 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2873 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2874 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 505: /* formal_or_ret_type_expr: expr  */
#line 2878 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9511 "bison-chpl-lib.cpp"
    break;

  case 506: /* ret_type: formal_or_ret_type_expr  */
#line 2882 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 507: /* ret_type: reserved_type_ident_use  */
#line 2883 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 508: /* ret_type: error  */
#line 2884 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 509: /* colon_ret_type: TCOLON ret_type  */
#line 2888 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 510: /* colon_ret_type: error  */
#line 2889 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 511: /* opt_ret_type: %empty  */
#line 2893 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_type: %empty  */
#line 2898 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_type: TCOLON expr  */
#line 2899 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9559 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2900 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9565 "bison-chpl-lib.cpp"
    break;

  case 516: /* opt_type: error  */
#line 2901 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9571 "bison-chpl-lib.cpp"
    break;

  case 517: /* formal_type: formal_or_ret_type_expr  */
#line 2905 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 518: /* formal_type: reserved_type_ident_use  */
#line 2906 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9583 "bison-chpl-lib.cpp"
    break;

  case 519: /* colon_formal_type: TCOLON formal_type  */
#line 2910 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9589 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_colon_formal_type: %empty  */
#line 2914 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9595 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_colon_formal_type: colon_formal_type  */
#line 2915 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9601 "bison-chpl-lib.cpp"
    break;

  case 522: /* expr_ls: expr  */
#line 2921 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9607 "bison-chpl-lib.cpp"
    break;

  case 523: /* expr_ls: expr_ls TCOMMA expr  */
#line 2922 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9613 "bison-chpl-lib.cpp"
    break;

  case 524: /* tuple_component: TUNDERSCORE  */
#line 2926 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9619 "bison-chpl-lib.cpp"
    break;

  case 525: /* tuple_component: opt_try_expr  */
#line 2927 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9625 "bison-chpl-lib.cpp"
    break;

  case 526: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2932 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9633 "bison-chpl-lib.cpp"
    break;

  case 527: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2936 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9641 "bison-chpl-lib.cpp"
    break;

  case 528: /* opt_actual_ls: %empty  */
#line 2942 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9647 "bison-chpl-lib.cpp"
    break;

  case 529: /* opt_actual_ls: actual_ls  */
#line 2943 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9653 "bison-chpl-lib.cpp"
    break;

  case 530: /* actual_ls: actual_expr  */
#line 2948 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9662 "bison-chpl-lib.cpp"
    break;

  case 531: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2953 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9672 "bison-chpl-lib.cpp"
    break;

  case 532: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2961 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9678 "bison-chpl-lib.cpp"
    break;

  case 533: /* actual_expr: opt_try_expr  */
#line 2962 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9684 "bison-chpl-lib.cpp"
    break;

  case 534: /* ident_expr: ident_use  */
#line 2966 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9690 "bison-chpl-lib.cpp"
    break;

  case 535: /* ident_expr: scalar_type  */
#line 2967 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9696 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TSINGLE expr  */
#line 2980 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9702 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2982 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9708 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2984 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9714 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2986 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9720 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2988 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9730 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TATOMIC expr  */
#line 2994 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9736 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TSYNC expr  */
#line 2996 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9742 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TOWNED  */
#line 2999 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9748 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TOWNED expr  */
#line 3001 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9754 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TUNMANAGED  */
#line 3003 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9760 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3005 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9766 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TSHARED  */
#line 3007 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9772 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TSHARED expr  */
#line 3009 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9778 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TBORROWED  */
#line 3011 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9784 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TBORROWED expr  */
#line 3013 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9790 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TCLASS  */
#line 3015 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9796 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TRECORD  */
#line 3017 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9802 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 3022 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 9815 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3031 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 9828 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TFOR expr TDO expr  */
#line 3040 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 9840 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3048 "chpl.ypp"
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
#line 9858 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3062 "chpl.ypp"
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
#line 9876 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3076 "chpl.ypp"
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
#line 9894 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3090 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9907 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3099 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9920 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr: TFORALL expr TDO expr  */
#line 3108 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9932 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3116 "chpl.ypp"
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
#line 9950 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3130 "chpl.ypp"
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
#line 9968 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3144 "chpl.ypp"
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
#line 9986 "bison-chpl-lib.cpp"
    break;

  case 570: /* bracket_loop_expr: TLSBR TRSBR  */
#line 3161 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 9994 "bison-chpl-lib.cpp"
    break;

  case 571: /* bracket_loop_expr: TLSBR TRSBR expr  */
#line 3165 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10002 "bison-chpl-lib.cpp"
    break;

  case 572: /* bracket_loop_expr: TLSBR expr_ls TRSBR expr  */
#line 3169 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10010 "bison-chpl-lib.cpp"
    break;

  case 573: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3173 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10018 "bison-chpl-lib.cpp"
    break;

  case 574: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3177 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 575: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3181 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10034 "bison-chpl-lib.cpp"
    break;

  case 576: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3185 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10042 "bison-chpl-lib.cpp"
    break;

  case 577: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3192 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10056 "bison-chpl-lib.cpp"
    break;

  case 578: /* nil_expr: TNIL  */
#line 3209 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10062 "bison-chpl-lib.cpp"
    break;

  case 586: /* opt_task_intent_ls: %empty  */
#line 3227 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10068 "bison-chpl-lib.cpp"
    break;

  case 587: /* opt_task_intent_ls: task_intent_clause  */
#line 3228 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 588: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3233 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10084 "bison-chpl-lib.cpp"
    break;

  case 589: /* task_intent_ls: intent_expr  */
#line 3241 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 590: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3242 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10096 "bison-chpl-lib.cpp"
    break;

  case 591: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3247 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10106 "bison-chpl-lib.cpp"
    break;

  case 592: /* forall_intent_ls: intent_expr  */
#line 3255 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10112 "bison-chpl-lib.cpp"
    break;

  case 593: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3256 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10118 "bison-chpl-lib.cpp"
    break;

  case 594: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3261 "chpl.ypp"
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
#line 10136 "bison-chpl-lib.cpp"
    break;

  case 595: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3275 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10144 "bison-chpl-lib.cpp"
    break;

  case 596: /* intent_expr: expr TREDUCE ident_expr  */
#line 3279 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10152 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_var_prefix: TCONST  */
#line 3285 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10158 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_var_prefix: TIN  */
#line 3286 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10164 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_var_prefix: TCONST TIN  */
#line 3287 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10170 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_var_prefix: TREF  */
#line 3288 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10176 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_var_prefix: TCONST TREF  */
#line 3289 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10182 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_var_prefix: TVAR  */
#line 3290 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10188 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_maybe_decorated: TNEW  */
#line 3295 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10194 "bison-chpl-lib.cpp"
    break;

  case 604: /* new_maybe_decorated: TNEW TOWNED  */
#line 3297 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 605: /* new_maybe_decorated: TNEW TSHARED  */
#line 3299 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10206 "bison-chpl-lib.cpp"
    break;

  case 606: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3301 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10212 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3303 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_expr: new_maybe_decorated expr  */
#line 3309 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10226 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3316 "chpl.ypp"
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
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3328 "chpl.ypp"
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
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 611: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3340 "chpl.ypp"
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
#line 10275 "bison-chpl-lib.cpp"
    break;

  case 612: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3353 "chpl.ypp"
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
#line 10293 "bison-chpl-lib.cpp"
    break;

  case 613: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3370 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10301 "bison-chpl-lib.cpp"
    break;

  case 614: /* range_literal_expr: expr TDOTDOT expr  */
#line 3377 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10310 "bison-chpl-lib.cpp"
    break;

  case 615: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3382 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10319 "bison-chpl-lib.cpp"
    break;

  case 616: /* range_literal_expr: expr TDOTDOT  */
#line 3387 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10328 "bison-chpl-lib.cpp"
    break;

  case 617: /* range_literal_expr: TDOTDOT expr  */
#line 3392 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10338 "bison-chpl-lib.cpp"
    break;

  case 618: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3398 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10348 "bison-chpl-lib.cpp"
    break;

  case 619: /* range_literal_expr: TDOTDOT  */
#line 3404 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10358 "bison-chpl-lib.cpp"
    break;

  case 620: /* cast_expr: expr TCOLON expr  */
#line 3434 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10366 "bison-chpl-lib.cpp"
    break;

  case 621: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3441 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10374 "bison-chpl-lib.cpp"
    break;

  case 622: /* super_expr: fn_expr  */
#line 3447 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10380 "bison-chpl-lib.cpp"
    break;

  case 626: /* expr: sub_type_level_expr TQUESTION  */
#line 3456 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10386 "bison-chpl-lib.cpp"
    break;

  case 627: /* expr: TQUESTION  */
#line 3458 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10392 "bison-chpl-lib.cpp"
    break;

  case 631: /* expr: fn_type  */
#line 3463 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10398 "bison-chpl-lib.cpp"
    break;

  case 641: /* opt_expr: %empty  */
#line 3477 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10404 "bison-chpl-lib.cpp"
    break;

  case 642: /* opt_expr: expr  */
#line 3478 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10410 "bison-chpl-lib.cpp"
    break;

  case 643: /* opt_try_expr: TTRY expr  */
#line 3482 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10416 "bison-chpl-lib.cpp"
    break;

  case 644: /* opt_try_expr: TTRYBANG expr  */
#line 3483 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10422 "bison-chpl-lib.cpp"
    break;

  case 645: /* opt_try_expr: super_expr  */
#line 3484 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10428 "bison-chpl-lib.cpp"
    break;

  case 651: /* call_base_expr: expr TBANG  */
#line 3501 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10436 "bison-chpl-lib.cpp"
    break;

  case 652: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3504 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10442 "bison-chpl-lib.cpp"
    break;

  case 655: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3511 "chpl.ypp"
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
#line 10458 "bison-chpl-lib.cpp"
    break;

  case 656: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3523 "chpl.ypp"
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
#line 10474 "bison-chpl-lib.cpp"
    break;

  case 657: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3535 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10482 "bison-chpl-lib.cpp"
    break;

  case 658: /* dot_expr: expr TDOT ident_use  */
#line 3542 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10488 "bison-chpl-lib.cpp"
    break;

  case 659: /* dot_expr: expr TDOT TTYPE  */
#line 3544 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10494 "bison-chpl-lib.cpp"
    break;

  case 660: /* dot_expr: expr TDOT TDOMAIN  */
#line 3546 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10500 "bison-chpl-lib.cpp"
    break;

  case 661: /* dot_expr: expr TDOT TLOCALE  */
#line 3548 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10506 "bison-chpl-lib.cpp"
    break;

  case 662: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3550 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 663: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3556 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10526 "bison-chpl-lib.cpp"
    break;

  case 664: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3568 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10532 "bison-chpl-lib.cpp"
    break;

  case 665: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3570 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10540 "bison-chpl-lib.cpp"
    break;

  case 666: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3574 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10548 "bison-chpl-lib.cpp"
    break;

  case 667: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3578 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10556 "bison-chpl-lib.cpp"
    break;

  case 668: /* bool_literal: TFALSE  */
#line 3584 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10562 "bison-chpl-lib.cpp"
    break;

  case 669: /* bool_literal: TTRUE  */
#line 3585 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10568 "bison-chpl-lib.cpp"
    break;

  case 670: /* str_bytes_literal: STRINGLITERAL  */
#line 3589 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10574 "bison-chpl-lib.cpp"
    break;

  case 671: /* str_bytes_literal: BYTESLITERAL  */
#line 3590 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10580 "bison-chpl-lib.cpp"
    break;

  case 674: /* literal_expr: INTLITERAL  */
#line 3596 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10586 "bison-chpl-lib.cpp"
    break;

  case 675: /* literal_expr: REALLITERAL  */
#line 3597 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10592 "bison-chpl-lib.cpp"
    break;

  case 676: /* literal_expr: IMAGLITERAL  */
#line 3598 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10598 "bison-chpl-lib.cpp"
    break;

  case 677: /* literal_expr: CSTRINGLITERAL  */
#line 3599 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10604 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: TNONE  */
#line 3600 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3602 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10619 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3607 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3612 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10636 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3616 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
  }
#line 10644 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3620 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
  }
#line 10652 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3624 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
  }
#line 10660 "bison-chpl-lib.cpp"
    break;

  case 685: /* assoc_expr_ls: expr TALIAS expr  */
#line 3632 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10669 "bison-chpl-lib.cpp"
    break;

  case 686: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3637 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10679 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TPLUS expr  */
#line 3645 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TMINUS expr  */
#line 3646 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10691 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TSTAR expr  */
#line 3647 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10697 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TDIVIDE expr  */
#line 3648 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10703 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3649 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10709 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3650 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10715 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TMOD expr  */
#line 3651 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10721 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TEQUAL expr  */
#line 3652 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10727 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3653 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10733 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3654 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10739 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3655 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10745 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TLESS expr  */
#line 3656 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10751 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TGREATER expr  */
#line 3657 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10757 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TBAND expr  */
#line 3658 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10763 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TBOR expr  */
#line 3659 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10769 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TBXOR expr  */
#line 3660 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10775 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TAND expr  */
#line 3661 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10781 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TOR expr  */
#line 3662 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10787 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TEXP expr  */
#line 3663 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10793 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TBY expr  */
#line 3664 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10799 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TALIGN expr  */
#line 3665 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10805 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr THASH expr  */
#line 3666 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10811 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TDMAPPED expr  */
#line 3667 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10817 "bison-chpl-lib.cpp"
    break;

  case 710: /* unary_op_expr: TPLUS expr  */
#line 3671 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10823 "bison-chpl-lib.cpp"
    break;

  case 711: /* unary_op_expr: TMINUS expr  */
#line 3672 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10829 "bison-chpl-lib.cpp"
    break;

  case 712: /* unary_op_expr: TMINUSMINUS expr  */
#line 3673 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10835 "bison-chpl-lib.cpp"
    break;

  case 713: /* unary_op_expr: TPLUSPLUS expr  */
#line 3674 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10841 "bison-chpl-lib.cpp"
    break;

  case 714: /* unary_op_expr: TBANG expr  */
#line 3675 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10847 "bison-chpl-lib.cpp"
    break;

  case 715: /* unary_op_expr: expr TBANG  */
#line 3676 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10855 "bison-chpl-lib.cpp"
    break;

  case 716: /* unary_op_expr: TBNOT expr  */
#line 3679 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10861 "bison-chpl-lib.cpp"
    break;

  case 717: /* reduce_expr: expr TREDUCE expr  */
#line 3684 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10869 "bison-chpl-lib.cpp"
    break;

  case 718: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3688 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10877 "bison-chpl-lib.cpp"
    break;

  case 719: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3692 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10885 "bison-chpl-lib.cpp"
    break;

  case 720: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3696 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10893 "bison-chpl-lib.cpp"
    break;

  case 721: /* scan_expr: expr TSCAN expr  */
#line 3703 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10901 "bison-chpl-lib.cpp"
    break;

  case 722: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3707 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10909 "bison-chpl-lib.cpp"
    break;

  case 723: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3711 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10917 "bison-chpl-lib.cpp"
    break;

  case 724: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3715 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10925 "bison-chpl-lib.cpp"
    break;


#line 10929 "bison-chpl-lib.cpp"

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
