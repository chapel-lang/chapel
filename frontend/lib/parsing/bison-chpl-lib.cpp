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
  YYSYMBOL_ret_array_type = 336,           /* ret_array_type  */
  YYSYMBOL_ret_type = 337,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 338,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 339,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 340,                 /* opt_type  */
  YYSYMBOL_array_type = 341,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 342, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 343,        /* formal_array_type  */
  YYSYMBOL_formal_type = 344,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 345,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 346,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 347,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 348,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 349,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 350,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 351,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 352,                /* actual_ls  */
  YYSYMBOL_actual_expr = 353,              /* actual_expr  */
  YYSYMBOL_ident_expr = 354,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 355,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 356,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 357,                 /* for_expr  */
  YYSYMBOL_cond_expr = 358,                /* cond_expr  */
  YYSYMBOL_nil_expr = 359,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 360,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 361,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 362,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 363,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 364,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 365,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 366,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 367,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 368,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 369,                 /* new_expr  */
  YYSYMBOL_let_expr = 370,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 371,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 372,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 373,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 374,               /* super_expr  */
  YYSYMBOL_expr = 375,                     /* expr  */
  YYSYMBOL_opt_expr = 376,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 377,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 378,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 379,           /* call_base_expr  */
  YYSYMBOL_call_expr = 380,                /* call_expr  */
  YYSYMBOL_dot_expr = 381,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 382,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 383,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 384,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 385,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 386,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 387,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 388,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 389,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 390,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 391       /* reduce_scan_op_expr  */
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

#line 515 "bison-chpl-lib.cpp"

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
#define YYLAST   20583

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  182
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  210
/* YYNRULES -- Number of rules.  */
#define YYNRULES  758
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1328

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
       0,   631,   631,   635,   636,   641,   642,   650,   654,   662,
     663,   664,   665,   666,   667,   668,   669,   670,   671,   672,
     673,   686,   687,   688,   697,   706,   710,   717,   725,   726,
     727,   728,   729,   730,   731,   732,   746,   761,   769,   781,
     792,   804,   816,   827,   843,   845,   844,   853,   852,   863,
     864,   869,   868,   877,   876,   887,   907,   922,   937,   953,
     954,   956,   961,   962,   967,   985,   989,   993,  1004,  1023,
    1024,  1028,  1032,  1036,  1041,  1045,  1049,  1059,  1064,  1070,
    1077,  1082,  1089,  1100,  1101,  1105,  1110,  1118,  1127,  1137,
    1145,  1154,  1167,  1175,  1179,  1184,  1190,  1199,  1200,  1204,
    1213,  1217,  1221,  1225,  1229,  1233,  1242,  1243,  1247,  1248,
    1249,  1250,  1251,  1255,  1256,  1268,  1269,  1274,  1275,  1276,
    1277,  1278,  1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,
    1287,  1288,  1289,  1290,  1291,  1292,  1296,  1297,  1298,  1299,
    1300,  1301,  1302,  1303,  1304,  1305,  1306,  1307,  1314,  1315,
    1316,  1317,  1321,  1322,  1326,  1333,  1343,  1345,  1344,  1353,
    1352,  1364,  1363,  1372,  1371,  1382,  1386,  1392,  1392,  1398,
    1398,  1407,  1408,  1409,  1410,  1411,  1412,  1413,  1417,  1422,
    1427,  1432,  1439,  1447,  1448,  1452,  1461,  1467,  1475,  1494,
    1493,  1503,  1502,  1515,  1522,  1540,  1553,  1566,  1570,  1574,
    1578,  1582,  1586,  1590,  1594,  1612,  1616,  1620,  1624,  1628,
    1632,  1636,  1640,  1644,  1648,  1652,  1656,  1660,  1664,  1668,
    1672,  1676,  1680,  1684,  1688,  1692,  1696,  1700,  1705,  1713,
    1722,  1726,  1730,  1734,  1738,  1742,  1746,  1750,  1754,  1759,
    1764,  1769,  1777,  1792,  1810,  1814,  1821,  1822,  1827,  1832,
    1833,  1834,  1835,  1836,  1837,  1838,  1839,  1840,  1841,  1842,
    1843,  1844,  1854,  1855,  1856,  1857,  1866,  1867,  1871,  1875,
    1879,  1886,  1890,  1894,  1901,  1905,  1909,  1913,  1920,  1921,
    1925,  1929,  1933,  1940,  1953,  1969,  1977,  1981,  1990,  1991,
    1995,  1999,  2004,  2012,  2017,  2021,  2028,  2029,  2033,  2042,
    2047,  2058,  2065,  2066,  2067,  2071,  2072,  2076,  2080,  2084,
    2088,  2092,  2099,  2116,  2129,  2136,  2141,  2148,  2147,  2158,
    2164,  2163,  2177,  2179,  2178,  2187,  2186,  2198,  2197,  2206,
    2205,  2216,  2223,  2235,  2253,  2250,  2278,  2282,  2283,  2285,
    2290,  2291,  2295,  2296,  2300,  2303,  2305,  2312,  2313,  2325,
    2346,  2345,  2361,  2360,  2378,  2388,  2385,  2419,  2427,  2435,
    2446,  2457,  2466,  2481,  2482,  2486,  2487,  2488,  2497,  2498,
    2499,  2500,  2501,  2502,  2503,  2504,  2505,  2506,  2507,  2508,
    2509,  2510,  2511,  2512,  2513,  2514,  2515,  2516,  2517,  2518,
    2519,  2520,  2521,  2525,  2526,  2527,  2528,  2529,  2530,  2531,
    2532,  2533,  2534,  2535,  2536,  2541,  2542,  2546,  2547,  2548,
    2552,  2553,  2557,  2558,  2562,  2563,  2567,  2568,  2572,  2576,
    2577,  2581,  2585,  2590,  2595,  2600,  2605,  2614,  2618,  2626,
    2627,  2628,  2629,  2630,  2631,  2632,  2633,  2634,  2638,  2639,
    2640,  2641,  2642,  2643,  2647,  2648,  2649,  2653,  2654,  2655,
    2656,  2657,  2658,  2662,  2663,  2666,  2667,  2671,  2672,  2676,
    2681,  2682,  2684,  2686,  2688,  2693,  2695,  2700,  2702,  2704,
    2706,  2708,  2710,  2712,  2717,  2718,  2722,  2731,  2735,  2743,
    2747,  2754,  2775,  2776,  2778,  2786,  2787,  2788,  2789,  2790,
    2795,  2794,  2803,  2811,  2815,  2822,  2838,  2855,  2859,  2863,
    2870,  2872,  2874,  2881,  2882,  2883,  2887,  2891,  2895,  2899,
    2903,  2907,  2911,  2919,  2920,  2921,  2922,  2926,  2927,  2931,
    2932,  2936,  2937,  2938,  2939,  2940,  2960,  2964,  2968,  2972,
    2979,  2980,  2981,  2985,  2990,  2998,  3003,  3007,  3014,  3015,
    3016,  3017,  3021,  3025,  3026,  3032,  3033,  3034,  3035,  3039,
    3040,  3044,  3045,  3046,  3050,  3054,  3061,  3062,  3066,  3071,
    3080,  3081,  3082,  3083,  3087,  3088,  3099,  3101,  3103,  3105,
    3112,  3113,  3114,  3115,  3116,  3117,  3119,  3121,  3123,  3125,
    3131,  3133,  3136,  3138,  3140,  3142,  3144,  3146,  3148,  3150,
    3152,  3154,  3159,  3168,  3177,  3185,  3199,  3213,  3227,  3236,
    3245,  3253,  3267,  3281,  3295,  3312,  3321,  3330,  3345,  3363,
    3381,  3389,  3390,  3391,  3392,  3393,  3394,  3395,  3399,  3400,
    3404,  3413,  3414,  3418,  3427,  3428,  3432,  3447,  3451,  3458,
    3459,  3460,  3461,  3462,  3463,  3467,  3469,  3471,  3473,  3475,
    3481,  3488,  3500,  3512,  3525,  3542,  3549,  3554,  3559,  3564,
    3570,  3576,  3606,  3613,  3620,  3621,  3625,  3626,  3627,  3628,
    3629,  3630,  3631,  3632,  3633,  3634,  3635,  3636,  3640,  3641,
    3645,  3646,  3647,  3651,  3652,  3653,  3654,  3663,  3664,  3667,
    3668,  3669,  3673,  3685,  3697,  3704,  3706,  3708,  3710,  3712,
    3718,  3731,  3732,  3736,  3740,  3747,  3748,  3752,  3753,  3757,
    3758,  3759,  3760,  3761,  3762,  3763,  3764,  3769,  3774,  3778,
    3783,  3787,  3796,  3801,  3810,  3811,  3812,  3813,  3814,  3815,
    3816,  3817,  3818,  3819,  3820,  3821,  3822,  3823,  3824,  3825,
    3826,  3827,  3828,  3829,  3830,  3831,  3832,  3836,  3837,  3838,
    3839,  3840,  3841,  3844,  3848,  3852,  3856,  3860,  3867,  3871,
    3875,  3879,  3887,  3888,  3889,  3890,  3891,  3892,  3893
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

#define YYPACT_NINF (-1107)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-759)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1107,   128,  3673, -1107,   -33,   206, -1107, -1107, -1107, -1107,
   -1107, -1107, 12087,   159,   279,   291, 15144,   303, 20347,   159,
   12087,   310,   184,   305,   279,  5081, 12087,   402,  5081,   252,
   20406, -1107,   407,   393,  8939, 10339, 10339, -1107,  9113,   445,
     490,   378, -1107,   483, 20406, 20406, 20406, -1107, 15604, 10513,
     510, 12087, 12087,   300, -1107,   533,   551, 12087, -1107, 15144,
   -1107, 12087,   566,   462,    60,   441, 14176,   580, 20465, -1107,
   10689,  8415, 12087, 10513, 15144, 12087,   548,   605,   491,  5081,
     612, 12087,   617,  7365,  7365, -1107,   618, -1107, 15144,   671,
   -1107,   630,  9113,  9287, -1107, 12087, -1107, 12087, -1107, -1107,
   14656, 12087, -1107, 12087, -1107, -1107, -1107,  4025,  7541,  9463,
   12087, -1107,  4905, -1107, -1107, -1107, -1107, -1107,   524, -1107,
     224, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107,
   -1107, -1107, -1107, -1107,  7715, -1107, 10863, -1107, -1107, -1107,
   -1107, -1107,   640, -1107, -1107, -1107, -1107, -1107, -1107, -1107,
   -1107,   550, 20406, -1107, 20055,   532,   318, -1107,    55, -1107,
   -1107, -1107,   257,   379, -1107, 20406, 15604, -1107,   540, -1107,
     542, -1107, -1107,   543,   544, 12087,   546,   549, -1107, -1107,
   -1107, 19571,  1467,   -15,   552,   554, -1107, -1107,   144, -1107,
   -1107, -1107, -1107, -1107,   311, -1107, -1107, -1107, 12087, 12087,
   12087, 20406, -1107, 12087, 10689, 10689,   652,   356, -1107, -1107,
   -1107, -1107, 19571,   484, -1107, -1107,   555,  5081, -1107, -1107,
     556,   123,   558, 15583, -1107,   266, -1107,   565, -1107,   198,
   19571, -1107,  2629,   611,  8591, -1107, -1107, -1107, -1107, -1107,
   -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107,
   -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107,
     564, -1107, -1107, 20406,   560,    41,  3018,    32, 16608,    32,
   16693, 20406, 20406,    -3, 15888, -1107, -1107,   651,  8591,   568,
     498,  5081,  2471,    92,    79, -1107,  5081, -1107, -1107, 17114,
   -1107,    33, 17534,   752, -1107,   569,   571, -1107, 17114,   123,
     752, -1107,  8591,  3009,  3009, -1107,   205,    28, -1107,    38,
   -1107, -1107, -1107, -1107, 12087, 12087, -1107,  9639,   444, -1107,
     563,   590, -1107, -1107,  3227,   607, -1107, -1107, -1107,   410,
   -1107,   245, 19571, 12087, 12087, -1107,   142, -1107, -1107, 19571,
     589, 17709, -1107, 17114,   123, 19571,   585,  8591, -1107, 19571,
   17749, -1107, -1107, -1107, -1107,   123, -1107,  2629,    41, 17114,
     595,   707,   707, 14046,   752,   752,   372, -1107, -1107,  4201,
     167, -1107, 12087, -1107,   109,   115, -1107,   -19,    29, 17794,
     -35, 14046,   754, -1107,  4377, 12087, -1107, 12087,   697, -1107,
   -1107, 16785,    70,   550, 19571,    69, 20406, 10689,   594, -1107,
     596,   762,   764,   648,   180, -1107, -1107, -1107,   216,   257,
   -1107, -1107, -1107, -1107,   686, -1107, -1107, -1107,   114,   687,
   -1107, -1107, -1107, 14487,   656,   246, -1107,   329,   505, -1107,
     638, 12087, 12087, 12087, 10339, 10339, 12087,   509, 12087, 12087,
   12087, 12087, 12087,   368, 14656, 12087, 12087, 12087, 12087, 12087,
   12087, 12087, 12087, 12087, 12087, 12087, 12087, 12087, 12087, 12087,
   12087,   726, -1107, -1107, -1107, -1107, -1107,  9287,  9287, -1107,
   -1107, -1107, -1107,  9287, -1107, -1107,  9287,  9287,  8591,  8591,
   10339, 10339, 17199, 17275, 17902,   623,    22, 20406,  8241, -1107,
   -1107,  4553, -1107, 10339,    41, 15604, -1107, 12087, -1107,  2629,
   -1107, 20406, -1107, -1107, -1107,   651, 12087,   672, -1107,   624,
     654, -1107, -1107, -1107,   755, 10689, -1107,  5257, 10339, -1107,
     631, -1107,    41,  5433, 10339, -1107,    41, -1107,    41, 10339,
   -1107,    41,   678,   680,  5081,   775,  5081, -1107,   776, 12087,
     751,   649,  8591, 20406, -1107, -1107, -1107,  2471, -1107, -1107,
     124, -1107, 12261,   701, 12087, 15604, -1107, -1107, 12087, -1107,
   20131, 12087, 12087, -1107,   650, -1107, -1107, -1107, -1107, 11039,
      68, 20200, -1107,   257, 14351, -1107,  2471, -1107,   683, 11215,
   10689, -1107, 19571, 19571, -1107,   558, -1107,    63, -1107,  8591,
     657, -1107,   811,   811,  2629, -1107, -1107, -1107, -1107, -1107,
   -1107, -1107,  9815, -1107, 17947,  7891, -1107,  8067, -1107,  5081,
     658, 10339,  9991,  3849,   659, 12087, 11389, -1107, -1107,   439,
   -1107,  4729, -1107,   332, 17987,   338, 16877, 20406,  7191,  7191,
   -1107,   550,   662, -1107,   266, -1107,   190,   688,  1365, -1107,
   -1107, 20406, -1107, 20406, 12087,    -1, -1107, -1107,   181, -1107,
   -1107,   103, -1107, -1107,  7017, -1107,   731, -1107,   667,   691,
   -1107,   693, -1107,   700,   705,   706,   708, -1107,   710,   712,
     713,   715,   717,   718,   719, -1107, -1107, -1107, -1107, -1107,
   -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107,
   -1107, -1107, -1107, -1107, -1107, -1107, -1107, 12087,   709,   721,
     667, -1107,   667, -1107, 12435, -1107, 20406, -1107, -1107, 19858,
   19858,   500, -1107,   500, -1107,   500,  2851,  1130,  2024,  2212,
     123,   707, -1107,   518, -1107, -1107, -1107, -1107, -1107, 14046,
   17315,   500,  1759,  1759, 19858,  1759,  1759,  1320,   707, 17315,
    1857,  1320,   752,   752,   707, 14046,   714,   720,   724,   725,
     727,   729,   733,   676, -1107,   500, -1107,   500, 12609, 10339,
   12783, 10339, 12087,  8591, 10339, 15428,   711,   301, -1107, -1107,
   -1107,   202, -1107,   880, 19724,   519,   159, 18063, -1107,   349,
   19571, -1107, 18107,  8591, -1107,  8591, 12087,   213,  9113, 19571,
      57, 17360,  8241, -1107,  9113, 19571,    39, 16953, -1107, -1107,
      32, 17038, -1107, 12087, 12087,   840,  5081,   844, 18152,  5081,
   17436, 20406, -1107,   221, -1107,   229, -1107,   309,  2471,    92,
    6841, -1107, -1107,   492,  8765, -1107, 19571, -1107, -1107, -1107,
   20406, 18260, 18305, -1107, -1107, -1107, -1107,   434, 10339, 14829,
     683,   740, -1107,   325, 11215, -1107,    -2, -1107, -1107, 19571,
   12087, -1107,   739,   158,   738, -1107, 15516, -1107, -1107, -1107,
   -1107, -1107, -1107, -1107, -1107, -1107,  5081,   -14, 17480, -1107,
   -1107, 19571,  5081, 19571, -1107, 18345, -1107, -1107, 12087, -1107,
      96, 15644, 12087, -1107, 11563,  7191,  7191, -1107, 12087,   520,
   12087,   531,   547, 12087, 10165,   562,   366, -1107, -1107, -1107,
   18465,   750,   737,   743, -1107, 20406, 15604, -1107,  8591,   745,
     935, 20406, -1107, 20406, -1107, 19571, 20288,   800,   635, -1107,
     731, -1107,  3438, -1107, -1107, -1107,   218, -1107,    45,   289,
   -1107, 18421, -1107, 15804, -1107, -1107, -1107,   386, -1107,   744,
     746, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107, 12087,
     887, 18510, 12087,   891, 18618,   326,   753, 18663,  8591, -1107,
   -1107,  8241, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107,
   -1107, -1107, -1107, -1107,    67, 10339, 10339,    41,    41, -1107,
   -1107, -1107, -1107, -1107, 17114, -1107, 15964,  5609, -1107,  5785,
   -1107,   334, -1107, 16049,  5961, -1107,    41,  6137, -1107,    41,
   -1107,    41, -1107,    41, 19571, 19571,  5081, -1107,  5081, 12087,
   -1107,  5081,   892,   766,   768, 20406,   558, -1107, -1107,   701,
     759,    73, -1107, -1107, -1107,   763,   765, 18703, -1107, -1107,
      -2, 12087,   731,   683, -1107, 10689, -1107, -1107, 19571, -1107,
    6313, 10689, -1107, -1107, -1107, 20406, -1107,   781,   558, -1107,
    6489,   769,  6665,   771, -1107, 12087, -1107, -1107,  7191, -1107,
   18779,    93, 17633,   415,   773,  7191, -1107, 12087, -1107, -1107,
    3320,   417,   430,   339, -1107,   933, -1107, -1107,   934, -1107,
   20002, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107,    45,
     772, 14001,    84,    76, 12087, -1107,   796,   778, -1107,   667,
     667, -1107, -1107, 18864, 12957, 13131, 18940, 13305, 13479, -1107,
   13653, 13827,   342, -1107,   701,   354,   374, -1107, -1107, -1107,
    5081,  9113, 19571,  9113, 19571,  8241, -1107,  5081,  9113, 19571,
   -1107,  9113, 19571, -1107, -1107, -1107, -1107, -1107, 19571,   915,
    5081, -1107, -1107, -1107, -1107, -1107, -1107, 10339, 15286,  8591,
    8591, 11739, -1107, -1107,   123, -1107,    -2, -1107, 19571,  5081,
   -1107,    42,   779, 12087, -1107,  9113, -1107, 19571,  5081,  9113,
   -1107, 19571,  5081, 19571,   243, 11913,  7191,  7191,  7191,  7191,
   -1107, -1107, -1107, 18980, 19571, 19817, -1107, -1107, -1107,   789,
   -1107, 19885, -1107, 19885, -1107, -1107, -1107, -1107, -1107,   238,
   15002, -1107,    98, -1107,   127, -1107,   660, 19027,    85, -1107,
   -1107, -1107, 12087, 12087, 12087, 12087, 12087, 12087, 12087, 12087,
   -1107, -1107, 18152, 16125, 16210, -1107, 18152, 16286, 16371,  5081,
   -1107, 19133, -1107,   673,   786,   791, -1107, -1107, -1107,   611,
   -1107,   558, 19571, 16447, -1107, 16532, -1107, -1107, -1107, 19571,
     431,   801,   458,   803, 12087, -1107, -1107, 19885, -1107, 19885,
   -1107,    74, -1107,   482, -1107, 12087,    76,    98,    98,    98,
      98,    98,    98,    76, -1107, -1107, -1107, -1107, -1107, 19209,
   19294, 19370, 19455, 19531, 19616,  5081,  5081,  5081,  5081, -1107,
   13827,   805,   807, -1107,  5081,  5081, -1107, -1107, -1107, -1107,
   19571, -1107, -1107, -1107, 19571, -1107, -1107, -1107, -1107, -1107,
   -1107, -1107,   828, 12087, 12087, 12087, 12087, 12087, 12087, 18152,
   18152, 18152, 18152,  1201, -1107, -1107, 18152, 18152
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   115,   701,   702,   703,   697,
     698,   704,     0,   618,   106,   136,   588,   143,   590,   618,
       0,   142,   490,   488,   106,     0,     0,    47,     0,   266,
     137,   184,   183,   695,     0,     0,     0,   182,     0,   141,
      59,   267,   338,   138,     0,     0,     0,   333,     0,     0,
     145,     0,     0,   635,   610,   705,   146,     0,   339,   582,
     485,     0,     0,     0,   169,   167,   427,   140,   591,   487,
       0,     0,     0,     0,   586,     0,     0,   144,     0,     0,
     116,     0,   696,     0,     0,   477,   139,   304,   584,    53,
     489,   147,     0,     0,   754,     0,   756,     0,   757,   758,
     651,     0,   755,   752,   568,   165,   753,     0,     0,     0,
       0,     4,     0,     5,     9,    10,    44,    11,     0,    49,
      62,    12,    68,    13,    14,    15,    16,    28,   564,   565,
      22,    50,   166,   176,     0,   185,   668,   177,    17,    30,
      29,    19,     0,   261,    18,   664,    21,    34,    31,    32,
     175,   305,     0,   173,     0,     0,   661,   337,     0,   569,
     171,   354,   447,   438,   174,     0,     0,   172,   673,   658,
     566,   657,   572,   570,     0,     0,   662,   663,   667,   666,
     665,     0,   571,     0,   674,   675,   676,   699,   700,   656,
     574,   573,   659,   660,     0,    27,   590,   137,     0,     0,
       0,     0,   591,     0,     0,     0,     0,   661,   673,   570,
     662,   663,   580,   571,   674,   675,     0,     0,   619,   107,
       0,   589,     0,   618,   478,     0,   486,     0,    20,     0,
     549,    45,   336,     0,   556,   113,   117,   129,   123,   122,
     131,   110,   121,   132,   118,   133,   108,   134,   127,   120,
     128,   126,   124,   125,   109,   111,   119,   130,   135,   114,
       0,   112,   193,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    60,    62,   556,     0,
       0,     0,     0,     0,     0,   493,     0,   153,    39,     0,
     296,     0,   295,   739,   639,   636,   637,   638,     0,   583,
     740,     7,   556,   336,   336,   459,   434,   150,   429,   151,
     430,   431,   435,   436,   149,   148,   437,     0,   427,   540,
       0,   341,   342,   344,     0,   428,   539,   541,   346,   658,
     546,     0,   545,     0,     0,   154,   569,   654,   672,   655,
       0,     0,    42,     0,   587,   575,     0,   556,    43,   581,
       0,   274,   278,   275,   278,   585,    51,   336,     0,     0,
       0,   741,   743,   649,   738,   737,     0,    65,    69,     0,
       0,   551,     0,   553,     0,     0,   552,     0,     0,   545,
       0,   650,     0,     6,     0,     0,    63,     0,     0,   490,
     181,     0,   697,   305,   669,   189,     0,     0,     0,   301,
       0,   325,   329,   331,     0,   315,   319,   322,   427,   447,
     445,   446,   444,   362,   448,   451,   450,   452,     0,   442,
     439,   440,   443,     0,   482,     0,   479,     0,   567,    33,
     640,     0,     0,     0,     0,     0,     0,   742,     0,     0,
       0,     0,     0,     0,   648,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   393,   400,   401,   402,   397,   399,     0,     0,   395,
     398,   396,   394,     0,   404,   403,     0,     0,   556,   556,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    35,
      23,     0,    36,     0,     0,     0,    24,     0,    37,   336,
     302,     0,   183,   303,    48,    62,     0,   564,   562,     0,
     557,   558,   563,   314,     0,     0,   203,     0,     0,   202,
       0,   211,     0,     0,     0,   209,     0,   219,     0,     0,
     217,     0,     0,     0,     0,   235,     0,   393,   231,     0,
       0,     0,     0,     0,   245,    25,   497,     0,   498,   500,
       0,   525,     0,   503,     0,     0,   152,    38,     0,   298,
       0,     0,     0,    40,     0,   170,   168,   432,   433,   530,
       0,     0,   428,   447,   427,   420,     0,   419,   543,     0,
       0,    99,   670,   671,   352,     0,   155,     0,    41,     0,
       0,   285,   276,   277,   336,    54,   196,   195,    26,    67,
      66,    70,     0,   706,     0,     0,   691,     0,   693,     0,
       0,     0,     0,     0,     0,     0,     0,   710,     8,     0,
      56,     0,    97,     0,    93,     0,    77,     0,     0,    83,
     178,   305,     0,   191,     0,   188,   272,   306,     0,   313,
     323,     0,   327,     0,     0,   317,   312,   412,     0,   414,
     418,     0,   449,   518,     0,   520,   453,   441,   410,   113,
     389,   117,   387,   123,   122,   121,   118,   391,   133,   134,
     120,   124,   109,   119,   135,   386,   368,   371,   369,   370,
     392,   381,   372,   385,   377,   375,   388,   376,   374,   379,
     384,   373,   378,   382,   383,   380,   390,     0,   365,     0,
     410,   366,   410,   363,     0,   481,     0,   476,   492,   734,
     733,   736,   745,   744,   749,   748,   730,   727,   728,   729,
     652,   717,   115,     0,   687,   688,   116,   686,   685,   646,
     721,   732,   726,   724,   735,   725,   723,   715,   720,   722,
     731,   714,   718,   719,   716,   647,     0,     0,     0,     0,
       0,     0,     0,     0,   747,   746,   751,   750,     0,     0,
       0,     0,     0,     0,     0,   708,   272,   629,   630,   632,
     634,     0,   621,     0,     0,     0,   618,   618,   199,     0,
     550,    46,     0,     0,   577,     0,     0,     0,     0,   594,
       0,     0,     0,   212,     0,   600,     0,     0,   210,   220,
       0,     0,   218,     0,     0,   234,     0,   230,     0,     0,
       0,     0,   576,     0,   248,     0,   246,     0,   501,     0,
       0,   524,   523,   658,     0,   495,   645,   494,   297,   294,
       0,     0,     0,   684,   532,   533,   535,   658,     0,   708,
     543,   347,   343,     0,     0,   544,   503,   345,   548,   547,
       0,   351,     0,     0,     0,   578,     0,   279,    52,   707,
     653,   692,   554,   694,   555,   227,     0,     0,     0,   709,
     225,   604,     0,   712,   711,     0,    58,    57,     0,    92,
       0,     0,     0,    85,     0,     0,    83,    55,   386,   368,
     371,   369,   370,   379,   378,   380,     0,   405,   406,    72,
      71,    84,     0,     0,   307,     0,     0,   269,     0,     0,
     336,     0,   326,     0,   330,   332,     0,     0,   427,   413,
     453,   516,     0,   515,   514,   517,   658,   454,   460,   427,
     361,     0,   367,     0,   357,   358,   484,   658,   480,     0,
       0,   105,   103,   104,   102,   101,   100,   682,   683,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   631,
     633,     0,   620,   136,   143,   142,   141,   138,   145,   146,
     140,   144,   139,   147,     0,     0,     0,     0,     0,   491,
     194,   560,   561,   559,     0,   229,     0,     0,   201,     0,
     200,     0,   624,     0,     0,   207,     0,     0,   205,     0,
     215,     0,   213,     0,   243,   242,     0,   237,     0,     0,
     233,     0,   239,     0,   271,     0,     0,   499,   502,   503,
       0,     0,   504,   505,   293,     0,     0,     0,   534,   536,
     503,     0,   453,   543,   542,   407,   423,   421,   353,   287,
       0,     0,   286,   289,   579,     0,   280,   283,     0,   228,
       0,     0,     0,     0,   226,     0,    98,    95,     0,    94,
      80,    79,    78,     0,     0,     0,   179,     0,   180,   307,
     336,     0,     0,     0,   300,   159,   169,   167,   163,   299,
     336,   308,   309,   156,   324,   328,   318,   321,   415,   460,
       0,   507,     0,     0,     0,   355,   417,     0,   364,   410,
     410,   689,   690,     0,     0,     0,     0,     0,     0,   271,
       0,     0,     0,   622,   503,   673,   673,   198,   197,   204,
       0,     0,   593,     0,   592,     0,   623,     0,     0,   599,
     208,     0,   598,   206,   216,   214,   236,   232,   609,   238,
       0,    64,   268,   247,   244,   496,   529,     0,   708,   556,
     556,   530,   424,   422,   348,   349,   503,   409,   408,     0,
     291,     0,     0,     0,   281,     0,   223,   606,     0,     0,
     221,   605,     0,   713,     0,     0,     0,    83,     0,    83,
      86,    89,    75,    74,    73,   336,   186,   192,   190,   273,
     157,   336,   161,   336,   310,   311,   334,   512,   510,   658,
     708,   474,     0,   475,   462,   465,     0,   461,     0,   411,
     359,   360,     0,     0,     0,     0,     0,     0,     0,     0,
     273,   626,   597,     0,     0,   625,   603,     0,     0,     0,
     241,     0,   527,   658,     0,     0,   537,   426,   425,   292,
     290,     0,   284,     0,   224,     0,   222,    96,    82,    81,
       0,     0,     0,     0,     0,   187,   270,   336,   160,   336,
     164,     0,   511,   658,   473,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   455,   457,   458,   356,   456,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   240,
       0,   641,   642,   282,     0,     0,    88,    91,    87,    90,
      76,   158,   162,   335,   464,   466,   467,   470,   471,   472,
     468,   469,   463,     0,     0,     0,     0,     0,     0,   596,
     595,   602,   601,   658,   643,   644,   608,   607
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1107, -1107, -1107,    10,   -65,  2083,    35, -1107, -1107, -1107,
    -213, -1107, -1107, -1107, -1107, -1107,   -24,   704, -1107,  -580,
    2409,   606,  -607, -1107,  -854, -1107, -1107,   111, -1107, -1107,
   -1107,   968, -1107,  2761,  -191, -1107, -1107,  -470,  1286, -1106,
     -87, -1107, -1107,   -86, -1107, -1107,  -885, -1107, -1107,  -155,
   -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107,   195,
   -1107,   903, -1107, -1107,   -13,  1788, -1107, -1107, -1107, -1107,
     642, -1107,   -45, -1107, -1107, -1107, -1107,   443, -1107, -1107,
   -1107,  -101, -1107,  -355,  -831, -1107, -1107, -1107, -1107, -1107,
      88, -1107, -1107,    89, -1107, -1107,  -599, -1107,   539, -1107,
   -1107, -1107, -1107, -1107,   429, -1107,  1654, -1107, -1107, -1107,
   -1107,   613, -1107, -1107, -1107, -1107,    77,  -414,  -164,  -828,
    -959,  -646, -1107,    78, -1107,    91,   440,   -56,   695,   -53,
   -1107, -1107,  -331,  -808, -1107,  -246,    65,   -72,  -253,  -244,
    -569, -1107, -1107,   122,   322, -1107,  -174,   898, -1107,  -163,
     478,   217,  -442,  -794,  -974, -1107, -1107, -1107,  -620,  -689,
    -809,  -512,  -543, -1107,  -785,   -63, -1107,    87, -1107,  -261,
    -508,  -539,   508,    86, -1107, -1107, -1107,   702, -1107,   -17,
   -1107, -1107,  -241, -1107,  -772, -1107, -1107,  1049,  1148, -1107,
   -1107, -1107, -1107,   -12, -1107,   -60,  1243, -1107,  1590,  1855,
   -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107, -1107,  -482
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   368,   112,   807,   114,   115,   499,   232,
     116,   117,   594,   357,   118,   119,   120,   388,   121,   122,
     123,   369,   901,   625,   902,   124,   125,   622,   623,   126,
     127,   220,   259,   403,   128,   261,   129,   319,   288,   130,
    1081,  1257,  1191,  1082,  1259,  1193,   131,   304,   303,   132,
     133,   134,   135,   136,   137,   634,   905,   138,   139,   956,
     140,   273,   141,   815,   816,   206,   143,   144,   145,   146,
     592,   857,  1048,   147,   148,   853,  1043,   290,   291,   149,
     150,   151,   152,   398,   910,   153,   154,   404,   916,   917,
     405,   911,   641,   406,   913,   643,   407,   155,   207,  1261,
     157,   158,   320,   321,   322,  1032,   159,   337,   585,   850,
     160,   161,  1208,   162,   163,   699,   700,   897,   898,   899,
    1036,   930,   409,   648,  1097,   649,   578,   650,   324,   572,
     423,   413,   418,   928,  1277,  1278,   508,  1095,  1204,  1205,
    1206,   164,   165,   425,   426,   705,   166,   167,   225,   284,
     285,   549,   550,   825,   924,   925,   655,   656,   553,   822,
     835,   327,   328,   845,   846,   370,   229,   374,   375,   509,
     510,   511,   208,   169,   170,   171,   172,   209,   174,   217,
     218,   771,   522,   991,   772,   773,   175,   210,   211,   178,
     179,   180,   338,   181,   395,   512,   213,   183,   214,   215,
     186,   187,   188,   189,   380,   190,   191,   192,   193,   194
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     212,   318,   222,   427,   221,   851,   775,   331,   223,   701,
     323,   340,   111,   325,   230,   936,   277,   541,   477,   504,
     992,   896,   266,   268,   270,  1083,   274,   526,   528,   531,
    1028,   920,  1064,   360,   813,   393,   847,   289,   632,   292,
     293,   564,   912,   507,   914,   298,   378,   299,   376,   300,
     854,   495,  1037,  1059,   934,  1030,   935,   836,   332,   339,
     341,   343,   344,   345,   852,   286,   286,   349,   551,   350,
     764,  1152,   994,  1070,   286,   286,   355,   611,   651,  1201,
     359,   339,   821,   361,  -266,   362,   590,   507,   363,   364,
     987,   365,   500,   551,  -267,   332,   339,   379,   381,   722,
     534,  1201,  1276,   520,   551,   817,  1093,   -61,   520,   410,
     539,   507,  1089,   616,  -320,   653,   838,  1198,   351,   353,
      42,  1147,   391,  -336,   394,   824,   -61,   554,     3,   411,
    1176,   326,   764,   195,   843,   330,   610,   614,  -288,   412,
    -521,   617,   486,  -336,   595,   -61,    58,  -316,   565,   566,
    1035,   520,   329,  -336,   520,  1276,   507,   609,   478,   503,
     479,   520,  1050,   430,  -521,  1094,  1177,    71,   491,  1202,
     612,  -316,   330,   373,   330,  -519,   -61,   612,    71,   633,
    1203,   558,  -288,    87,   923,  1083,   482,   483,   484,  1113,
     580,   349,   332,   379,  -521,  1083,  -521,  1237,   765,  1019,
     726,   234,  1203,   491,   491,   613,   494,  -519,   505,  -521,
     491,   278,   491,   491,   552,  -521,   612,   752,   753,  -521,
    -519,   612,   339,  -521,  1155,  1145,  1262,   555,   491,   265,
     267,   269,   612,  1040,  -519,  -288,  1153,  1182,  1185,   552,
    -521,  -521,   841,   306,   839,   491,   983,  1265,  1156,  1148,
     552,  1274,   728,   567,   570,   955,   491,   605,  -521,   702,
    1200,   654,  -249,   607,   308,   437,   339,   584,   310,   330,
     330,   385,   818,   443,  -521,  1266,   903,  1041,  1063,  -513,
    -519,   216,   219,   606,   414,  -519,   781,   507,   507,   608,
     339,   631,   311,    23,   224,   312,   568,    62,   819,  -506,
    1083,  1034,   345,   349,   377,   379,  1258,   313,  1260,   386,
     775,  -513,  1084,  -350,  1085,   602,   306,  -681,   294,  -681,
    1221,   582,   583,  1251,  -513,  1253,   316,  1029,   645,   918,
    1042,  -506,   779,   505,   637,   339,   415,   308,  -513,   603,
     387,   310,  1236,   318,  -506,    60,   497,  -250,   416,   959,
     961,   507,   646,  1225,  1114,   919,   907,    69,  -506,  -256,
     604,   580,  1238,   908,   498,   311,  -255,   417,   312,   785,
      62,   722,  1301,   624,  1302,   626,   962,  1015,   295,   601,
     313,   858,   330,    90,  -513,   332,   830,   985,   723,  -513,
     647,  1065,   960,   580,   706,  1014,   226,   296,   507,   316,
    1073,   480,   724,  1016,  -506,   481,   419,   747,   748,  -506,
     231,   581,   707,   749,   297,  1247,   750,   751,   262,   709,
     710,   711,   713,   715,   716,   234,   717,   718,   719,   720,
     721,   725,   729,   730,   731,   732,   733,   734,   735,   736,
     737,   738,   739,   740,   741,   742,   743,   744,   745,  -264,
    1112,  1174,   787,  1210,  1211,   339,   339,   818,   420,  1232,
     906,   339,   330,  -416,   339,   339,   339,   339,   755,   757,
     421,   306,   726,   818,   785,   505,   774,   555,   727,   775,
     878,   777,  1125,  1017,  -615,   780,   882,   785,   -60,   422,
     785,  -680,   308,  -680,   782,   708,   310,   555,   879,  1033,
    1109,  -254,  -628,   332,   883,   789,   791,   -60,  1126,   318,
     168,   795,   797,  1189,  1065,   979,  1220,   801,   323,   701,
     311,   325,  -627,   312,   808,   382,   -60,   810,  -628,  -680,
     339,  -680,  1066,   168,  -483,   313,   168,  -538,   195,  -251,
    -522,   156,   826,  -508,   599,   376,   292,   376,  -627,   831,
     832,   278,  -483,  -522,   316,   996,   999,   -60,  -538,  1001,
    1003,  -531,  -538,  1065,   156,   706,  -258,   156,   849,  1250,
     505,  1252,   507,   275,   301,  -508,   276,   339,   555,  1065,
     330,  1180,  -531,  1187,  -538,  -522,  -531,   168,  -508,  -262,
     849,   168,   168,   339,   507,   339,  1188,  1296,  -522,   868,
     849,   871,  -508,   873,   875,   195,  1065,  -259,  -531,   976,
    -756,   876,  -522,   481,  -756,   168,   900,   900,   156,  -522,
     168,  -757,   156,   156,  1298,  -757,  1051,  1053,  1275,   712,
     714,   601,   915,  1264,   834,   302,  -253,  -758,   823,   326,
    -522,  -758,   437,   775,   326,   848,   156,   441,  -508,   346,
     443,   156,  -753,  -508,   446,   837,  -753,  -677,  -522,  -677,
     329,  -257,   306,  -522,   347,   329,  -522,   848,  -263,   491,
     373,   543,   373,  -265,  -252,   754,   756,   848,  -679,   356,
    -679,  1275,  -678,   308,  -678,   931,  -260,   310,   776,  1057,
     728,   939,   862,   940,   864,   384,   396,   397,  1306,  1307,
    1308,  1309,  1310,  1311,   173,   408,  -612,   428,   487,  -611,
     429,   311,  -616,   790,   312,  -617,    62,   507,  -614,   796,
    -613,  -526,   490,   982,   800,   168,   313,   173,   488,   491,
     173,   496,   506,   515,  -526,   513,   386,   573,   574,   433,
     926,   542,   561,  1072,   562,   316,   789,   951,   795,   954,
     808,   339,   957,   871,   579,   586,   156,  1021,   589,   977,
     978,   598,   618,   627,  1023,   638,  -526,   507,   639,  1100,
     640,   339,   642,   339,   984,   644,   986,   652,   657,  -526,
     774,   173,   993,   704,   433,   173,   173,  1267,   443,   168,
     937,  1004,  1005,  -526,   168,   746,   763,   434,   784,   783,
    -526,   435,   785,   786,   792,   803,   867,   804,   379,   173,
     806,   809,   339,  1268,   173,  1269,  1270,   811,  1271,  1272,
     156,  -526,   539,   812,   833,   156,  1027,   871,   824,   539,
     844,   855,   856,   904,   866,   872,   580,   927,  1038,  -526,
     929,  -115,   434,  -136,  -526,  1080,   435,  -526,   981,   437,
    -143,   932,   948,   318,   441,  -142,  -141,   443,  -138,  1092,
    -145,   446,  -146,  -140,   318,  -144,   624,  -116,  -139,  -147,
    1060,   933,  1062,   900,   900,  1006,   361,   168,   362,  1008,
     941,   364,   365,   722,   958,   330,   942,  1031,  1234,  1235,
     943,   944,   168,   945,   437,   946,   339,   963,  1065,   441,
     964,   442,   443,  1068,   834,   965,   446,   947,   156,   326,
     379,  1039,  1044,   453,  1069,   402,   197,  1074,  1101,   173,
    1104,   459,  1102,   156,  1107,   837,   966,  1140,  1163,  1110,
     329,   703,  1141,   967,  1142,  1146,  1149,  1103,  1150,  1181,
    1106,  1190,  1192,   968,   918,  1168,   339,  1172,  1197,   774,
    1229,   969,  1209,  1241,   950,  1256,   953,   500,   507,   507,
    1291,    22,    23,   713,   755,  1292,  1075,  1297,   970,  1299,
    1324,   501,  1325,    31,   502,  1122,  1266,  1124,  1161,    37,
     971,   540,  1129,   173,   726,  1132,    42,   330,   173,  1056,
     621,   972,   227,  1194,  1195,   358,   593,  1138,   973,   168,
    1162,   828,  1143,   842,  1086,  1080,  1087,  1096,   635,  1088,
    1099,   840,    58,   571,    60,  1303,    62,  1196,  1076,  1154,
    1312,  1077,  1305,  1158,   503,   168,    69,  1071,   938,   332,
     156,   168,   390,   827,     0,  1018,     0,     0,  1167,     0,
    1171,     0,   168,  1173,   168,    85,   900,     0,    87,     0,
    1078,   176,    90,  1183,     0,  1184,   156,     0,     0,   539,
     539,     0,   156,   539,   539,     0,     0,     0,     0,     0,
       0,   173,     0,   156,   176,   156,     0,   176,     0,   539,
       0,   539,  1207,     0,     0,     0,   173,     0,     0,     0,
       0,     0,  1122,  1124,     0,  1129,  1132,     0,  1167,  1171,
    1157,   105,     0,     0,     0,     0,   330,  1079,  1222,  1223,
       0,  1224,     0,   774,     0,  1226,  1227,   168,     0,  1228,
    1080,   168,     0,     0,     0,     0,     0,     0,   176,   168,
       0,     0,   176,   176,     0,  1231,   871,   339,   339,  1171,
       0,     0,     0,     0,     0,     0,     0,     0,   156,     0,
     177,  1242,   156,  1243,     0,     0,   176,  1245,     0,     0,
     156,   176,   433,  1249,   900,   900,   900,   900,     0,     0,
     712,   754,     0,   177,     0,     0,   177,  1199,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   871,     0,
       0,     0,     0,   173,     0,     0,     0,     0,     0,     0,
    1222,  1279,  1280,  1226,  1281,  1282,  1283,  1284,     0,     0,
       0,     0,     0,     0,     0,     0,   834,     0,     0,   173,
     434,     0,     0,     0,   435,   173,     0,   177,     0,     0,
       0,   177,   177,     0,  1233,     0,   173,   837,   173,     0,
       0,     0,  1300,     0,     0,   182,     0,     0,     0,  -528,
       0,     0,     0,  1304,     0,   177,     0,     0,     0,     0,
     177,     0,  -528,     0,     0,     0,   176,     0,   182,     0,
       0,   182,   437,  1319,  1320,  1321,  1322,   441,  1171,   442,
     443,   974,  1326,  1327,   446,     0,  1263,     0,     0,     0,
       0,   453,     0,     0,  -528,     0,     0,   457,   458,   459,
       0,  1319,  1320,  1321,  1322,  1326,  1327,  -528,     0,     0,
       0,   173,     0,     0,   168,   173,     0,   168,     0,     0,
       0,  -528,   182,   173,     0,     0,   182,   182,  -528,     0,
     176,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   156,     0,     0,   156,  -528,
     182,     0,   433,     0,     0,   182,     0,     0,     0,   342,
       0,     0,     0,     0,     0,   177,   909,  -528,     0,     0,
       0,     0,  -528,     0,   168,  -528,  1323,     0,     0,     0,
     168,     0,     0,     0,     0,     0,     0,  -307,     0,     0,
       0,  -307,  -307,     0,     0,     0,  -307,     0,     0,     0,
       0,  -307,     0,  -307,  -307,   156,     0,     0,     0,  -307,
     434,   156,     0,     0,   435,     0,  -307,     0,   176,  -307,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   177,
       0,     0,     0,   176,   177,     0,     0,     0,     0,  -307,
       0,     0,  -307,     0,  -307,     0,  -307,     0,  -307,  -307,
       0,  -307,     0,     0,  -307,     0,  -307,     0,     0,     0,
     182,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,     0,     0,     0,   446,  -307,     0,     0,  -307,     0,
    -307,   453,  -307,  1115,  1116,     0,     0,   457,   458,   459,
       0,     0,     0,     0,     0,   168,     0,   168,     0,     0,
       0,     0,   168,     0,     0,   168,     0,     0,   173,     0,
       0,   173,     0,     0,   168,     0,   168,   177,     0,   168,
       0,     0,     0,     0,   182,     0,   156,     0,   156,   182,
       0,  -307,   177,   156,     0,     0,   156,  -307,     0,     0,
     176,     0,     0,     0,     0,   156,     0,   156,   168,     0,
     156,   516,   519,   521,   525,   527,   530,     0,   168,     0,
     168,     0,     0,     0,     0,     0,   176,     0,   173,     0,
       0,     0,   176,     0,   173,   557,     0,   559,     0,   156,
       0,     0,     0,   176,   563,   176,     0,     0,     0,   156,
       0,   156,   184,     0,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,     0,
       0,     0,   182,     0,     0,   184,     0,     0,   184,     0,
       0,     0,     0,     0,     0,     0,     0,   182,   168,   588,
       0,     0,     0,     0,     0,   168,     0,   476,     0,   177,
    -677,     0,  -677,     0,   596,   597,     0,     0,   168,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   176,   156,
       0,     0,   176,     0,     0,   177,   156,   168,     0,   184,
     176,   177,     0,   184,   184,     0,   168,     0,     0,   156,
     168,     0,   177,     0,   177,     0,     0,     0,     0,   173,
       0,   173,     0,     0,     0,     0,   173,   184,   156,   173,
       0,     0,   184,     0,     0,     0,     0,   156,   173,     0,
     173,   156,     0,   173,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   336,     0,     0,     0,     0,
       0,     0,     0,     0,   182,     0,     0,   168,     0,     0,
       0,     0,   173,     0,     0,     0,     0,   336,     0,     0,
       0,     0,   173,     0,   173,     0,     0,   177,     0,     0,
     182,   177,   336,     0,     0,     0,   182,     0,   156,   177,
       0,     0,     0,     0,     0,     0,     0,   182,     0,   182,
     778,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     142,   433,     0,   168,   168,   168,   168,     0,     0,     0,
       0,     0,   168,   168,     0,     0,     0,   184,   793,     0,
       0,     0,   798,   142,   799,     0,   142,   802,     0,     0,
       0,     0,   173,     0,   156,   156,   156,   156,     0,   173,
       0,     0,     0,   156,   156,     0,     0,     0,     0,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,   434,
       0,     0,   182,   435,     0,   176,   182,   185,   176,     0,
       0,   173,     0,     0,   182,     0,     0,   142,     0,     0,
     173,   184,     0,     0,   173,     0,   184,     0,     0,     0,
     185,     0,     0,   185,     0,     0,     0,     0,   336,   433,
       0,     0,     0,     0,     0,   142,     0,     0,     0,     0,
     142,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,     0,   446,     0,   176,     0,     0,     0,   452,
     453,   176,     0,   456,     0,     0,   457,   458,   459,     0,
       0,   173,   336,     0,   185,     0,     0,   460,   185,   185,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,   435,     0,     0,   177,     0,   336,   177,     0,   184,
       0,     0,   185,     0,     0,     0,     0,   185,     0,     0,
       0,     0,     0,     0,   184,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,   173,   173,   173,
     173,     0,     0,     0,     0,     0,   173,   173,     0,   437,
     438,   336,   439,   440,   441,   142,   442,   443,   444,     0,
     445,   446,   447,   448,   177,   450,   451,   452,   453,   454,
     177,   456,     0,     0,   457,   458,   459,     0,     0,     0,
       0,     0,     0,     0,     0,   460,   176,     0,   176,     0,
       0,     0,     0,   176,     0,     0,   176,     0,     0,   182,
       0,     0,   182,     0,     0,   176,   433,   176,     0,     0,
     176,     0,     0,     0,     0,     0,     0,     0,     0,   142,
       0,     0,   185,     0,   142,     0,   988,   990,     0,     0,
       0,   184,   995,   998,     0,   113,  1000,  1002,     0,   176,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,   176,     0,     0,     0,     0,     0,   184,   228,   182,
       0,   233,     0,   184,   434,   182,     0,     0,   435,     0,
       0,   336,   336,     0,   184,     0,   184,   336,     0,     0,
     336,   336,   336,   336,     0,   177,   185,   177,     0,     0,
       0,   185,   177,     0,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,   177,     0,   177,   142,     0,   177,
       0,     0,   348,     0,     0,     0,   437,   438,     0,   176,
     440,   441,   142,   442,   443,     0,   176,     0,   446,     0,
       0,     0,     0,     0,     0,   453,     0,     0,   177,   176,
     113,   457,   458,   459,     0,   383,   336,     0,   177,   184,
     177,     0,     0,   184,     0,     0,     0,     0,   176,     0,
       0,   184,     0,     0,     0,     0,     0,   176,     0,     0,
       0,   176,     0,     0,   185,     0,     0,     0,     0,     0,
     182,     0,   182,     0,     0,     0,     0,   182,     0,   185,
     182,     0,     0,   336,   433,     0,     0,     0,     0,   182,
       0,   182,     0,     0,   182,     0,     0,     0,     0,   336,
       0,   336,     0,  1117,  1118,     0,     0,     0,   177,     0,
    1119,     0,     0,     0,     0,   177,     0,     0,   176,   142,
       0,     0,  1130,   182,     0,  1133,     0,  1134,   177,  1135,
       0,     0,     0,   182,     0,   182,     0,     0,     0,     0,
     489,     0,   434,     0,     0,   142,   435,   177,     0,     0,
       0,   142,     0,     0,     0,     0,   177,     0,     0,     0,
     177,     0,   142,     0,   142,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   176,   176,   176,   176,     0,     0,
       0,     0,     0,   176,   176,     0,   185,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,     0,     0,   441,
       0,   442,   443,   182,   545,     0,   446,     0,     0,   556,
     182,     0,   185,   453,     0,     0,     0,   177,   185,   457,
     458,   459,     0,   182,     0,     0,     0,     0,     0,   185,
       0,   185,     0,     0,     0,     0,   184,   142,     0,   184,
       0,   142,   182,     0,     0,     0,     0,     0,     0,   142,
       0,   182,     0,     0,     0,   182,     0,   336,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,   177,   177,   177,   336,     0,   336,
       0,     0,   177,   177,     0,     0,     0,  1240,     0,     0,
       0,     0,   113,     0,     0,     0,   184,     0,   287,     0,
       0,     0,   184,     0,   185,     0,     0,   113,   185,     0,
       0,     0,   182,     0,   235,     0,   185,     0,   336,     0,
       0,     0,   287,     0,     0,     0,     0,     0,   236,   237,
       0,   238,   352,   354,     0,     0,   239,     0,     0,     0,
       0,     0,     0,     0,     0,   240,     0,     0,     0,     0,
       0,   241,     0,     0,     0,     0,     0,   242,     0,     0,
       0,   243,     0,     0,   244,     0,     0,     0,   182,   182,
     182,   182,     0,     0,   245,     0,     0,   182,   182,     0,
       0,   246,   247,     0,     0,     0,     0,     0,     0,   248,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   249,
       0,     0,   336,     0,     0,     0,     0,     0,   250,   251,
       0,   252,     0,   253,   113,   254,     0,   184,   255,   184,
       0,     0,   256,   546,   184,   257,     0,   184,     0,   258,
       0,     0,     0,     0,   142,     0,   184,   142,   184,     0,
     556,   184,     0,     0,     0,     0,   556,     0,     0,     0,
       0,     0,   336,     0,     0,     0,     0,   805,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     184,   492,     0,     0,     0,     0,     0,     0,     0,     0,
     184,     0,   184,     0,   547,     0,     0,     0,     0,     0,
       0,   500,     0,     0,   142,    22,    23,     0,     0,     0,
     142,   185,     0,     0,   185,   501,     0,    31,   502,     0,
       0,     0,     0,    37,   287,   287,   287,   287,   287,   287,
      42,     0,   535,   538,     0,     0,     0,     0,     0,   544,
       0,     0,   865,     0,     0,   -59,   870,     0,   287,     0,
     287,     0,     0,     0,   113,     0,    58,   287,    60,     0,
     184,     0,    64,     0,   -59,    65,     0,   184,   503,     0,
      69,   185,     0,     0,     0,     0,     0,   185,     0,     0,
     184,     0,     0,     0,     0,     0,     0,     0,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,   184,
       0,     0,   287,     0,     0,     0,     0,     0,   184,     0,
       0,     0,   184,     0,     0,     0,     0,   287,   287,     0,
       0,     0,     0,     0,     0,   142,     0,   142,     0,     0,
       0,     0,   142,     0,     0,   142,     0,     0,     0,     0,
       0,   260,     0,     0,   142,   105,   142,     0,     0,   142,
       0,     0,     0,   336,   336,   279,   280,   281,     0,   283,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   184,
       0,     0,     0,     0,     0,     0,     0,     0,   142,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   142,     0,
     142,     0,   185,     0,   185,     0,     0,     0,     0,   185,
       0,     0,   185,     0,     0,     0,     0,     0,     0,     0,
       0,   185,     0,   185,     0,     0,   185,     0,     0,     0,
       0,     0,     0,     0,     0,   184,   184,   184,   184,     0,
       0,     0,     0,   433,   184,   184,     0,     0,     0,  1007,
       0,     0,  1010,     0,     0,   185,     0,     0,     0,     0,
       0,     0,     0,   287,     0,   185,     0,   185,   142,     0,
       0,     0,     0,   399,     0,   142,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   424,   283,   142,     0,
       0,   287,     0,     0,     0,   287,     0,   287,     0,     0,
     287,   434,     0,     0,     0,   435,     0,   142,     0,  1049,
       0,     0,     0,     0,     0,  1054,   142,     0,     0,     0,
     142,     0,   485,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   185,     0,     0,     0,     0,
       0,     0,   185,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,   438,   185,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,     0,   450,
     451,   452,   453,   454,   185,   456,     0,   142,   457,   458,
     459,     0,     0,   185,   514,     0,     0,   185,     0,   460,
     431,   500,   532,   533,     0,    22,    23,   432,     0,     0,
       0,     0,     0,   548,     0,   501,     0,    31,   502,     0,
     433,   517,     0,    37,     0,     0,     0,     0,     0,     0,
      42,     0,     0,     0,     0,     0,   518,     0,     0,     0,
     556,     0,   556,   142,   142,   142,   142,   556,     0,     0,
     556,     0,   142,   142,   185,   577,    58,     0,    60,  1136,
       0,  1137,     0,     0,  1139,     0,     0,     0,   503,     0,
      69,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,    85,
       0,     0,    87,  1160,     0,     0,    90,     0,     0,     0,
       0,     0,     0,  1166,     0,  1170,     0,     0,     0,     0,
     185,   185,   185,   185,   436,     0,     0,     0,     0,   185,
     185,     0,     0,     0,     0,     0,     0,   636,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,   698,   457,   458,   459,     0,   491,
       0,     0,     0,     0,     0,     0,   460,     0,     0,   287,
     287,     0,     0,     0,     0,   287,   287,     0,     0,   287,
     287,     0,     0,     0,     0,     0,     0,     0,     0,  1012,
       0,     0,     0,  1230,     0,     0,     0,     0,     0,     0,
     235,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1239,     0,   236,   237,     0,   238,   766,     0,
       0,  1244,   239,     0,     0,  1246,   283,     0,     0,     0,
       0,   240,   260,     0,     0,  1046,     0,   241,     0,     0,
       0,     0,     0,   242,     0,     0,     0,   243,     0,     0,
     244,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     245,     0,     0,     0,     0,     0,     0,   246,   247,     0,
       0,     0,     0,     0,   814,   248,     0,     0,   548,     0,
       0,     0,  1289,     0,     0,   249,   283,     0,     0,     0,
       0,   829,     0,     0,   250,   251,     0,   252,     0,   253,
       0,   254,   577,     0,   255,     0,     0,   548,   256,   575,
       0,   257,   500,     0,     0,   258,    22,    23,     0,     0,
       0,  1075,     0,     0,     0,     0,   501,     0,    31,   502,
       0,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   287,   287,   887,     0,
       0,     0,     0,   287,     0,   538,     0,    58,     0,    60,
     576,    62,   538,  1076,     0,   287,  1077,     0,   287,   503,
     287,    69,   287,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1144,     0,     0,     0,     0,
      85,     0,     0,    87,     0,  1078,     0,    90,     0,  1090,
       0,     5,   305,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,  1164,    17,     0,
     196,     0,     0,    21,     0,     0,     0,   424,     0,     0,
       0,     0,    29,     0,   197,     0,     0,     0,    33,   198,
     199,     0,     0,   200,    39,     0,   105,     0,    41,     0,
       0,    43,  1186,     0,   201,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   202,     0,     0,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     203,     0,    80,     0,     0,    82,     0,     0,     0,    86,
       0,     0,    88,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
     287,     0,  1013,     0,     0,     0,     0,     0,     0,   548,
      95,    96,    97,    98,    99,     0,     0,     0,     0,   100,
       0,  1024,     0,     0,     0,     0,     0,     0,   101,     0,
       0,   102,   103,   104,     0,     0,     0,   106,     0,   204,
       0,   108,     0,   205,  1091,     0,   110,  1047,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   538,   538,     0,     0,   538,   538,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1293,     0,   538,     0,   538,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   424,   283,     0,     0,
       0,     0,     0,    -2,     4,     0,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,   698,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -59,     0,    40,    41,    42,     0,    43,  -336,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -59,
      53,    54,     0,    55,    56,    57,     0,  -336,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -336,   -59,    65,
      66,    67,    68,     0,    69,    70,    71,     0,    72,    73,
      74,    75,    76,    77,    78,    79,   814,    80,    81,     0,
      82,    83,    84,    85,    86,     0,    87,    88,    89,   -59,
      90,    91,     0,     0,    92,     0,    93,     0,     0,    94,
       0,     0,     0,     0,     0,     0,  1047,     0,     0,     0,
       0,     0,     0,     0,     0,    95,    96,    97,    98,    99,
       0,     0,     0,     0,   100,     0,     0,     0,     0,     0,
       0,     0,     0,   101,     0,     0,   102,   103,   104,   105,
       0,     0,   106,     0,   107,     0,   108,     0,   109,     0,
       4,   110,     5,     0,     6,     7,     8,     9,    10,    11,
       0,  -708,     0,    12,    13,    14,    15,    16,  -708,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,  -708,    28,    29,  -708,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -59,     0,    40,    41,
      42,     0,    43,  -336,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,    52,   -59,    53,    54,     0,    55,
      56,    57,     0,  -336,     0,     0,    58,    59,    60,    61,
       0,    63,    64,  -336,   -59,    65,    66,    67,    68,  -708,
      69,    70,    71,  -708,    72,    73,    74,    75,    76,    77,
      78,    79,     0,    80,    81,     0,    82,    83,    84,    85,
      86,     0,    87,    88,    89,   -59,    90,    91,     0,     0,
      92,     0,    93,     0,     0,  -708,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -708,  -708,    97,  -708,  -708,  -708,  -708,  -708,  -708,
    -708,     0,  -708,  -708,  -708,  -708,  -708,  -708,  -708,  -708,
    -708,  -708,  -708,  -708,  -708,   105,  -708,  -708,  -708,     0,
     107,  -708,   108,     0,   109,     0,   366,  -708,     5,   305,
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
     104,   105,     0,     0,   106,     0,   107,   367,   108,     0,
     109,     0,     4,   110,     5,     0,     6,     7,     8,     9,
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
     106,     0,   107,   600,   108,     0,   109,     0,   619,   110,
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
     102,   103,   104,   105,     0,     0,   106,     0,   107,   620,
     108,     0,   109,     0,   366,   110,     5,     0,     6,     7,
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
       0,     0,   106,     0,   107,   367,   108,     0,   109,     0,
       4,   110,     5,     0,     6,     7,     8,     9,    10,    11,
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
     107,   877,   108,     0,   109,     0,     4,   110,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -59,     0,    40,    41,    42,     0,    43,  -336,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -59,    53,    54,     0,    55,    56,    57,     0,  -336,
       0,     0,    58,    59,    60,    61,   382,    63,    64,  -336,
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
      32,    33,    34,    35,    36,    37,    38,    39,   -59,     0,
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
      36,    37,   788,    39,   -59,     0,    40,    41,    42,     0,
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
       0,    31,    32,    33,    34,    35,    36,    37,   794,    39,
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
    1123,    39,   -59,     0,    40,    41,    42,     0,    43,  -336,
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
      32,    33,    34,    35,    36,    37,  1128,    39,   -59,     0,
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
      36,    37,  1131,    39,   -59,     0,    40,    41,    42,     0,
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
      23,    24,    25,    26,    27,     0,  1159,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
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
      34,    35,    36,    37,  1165,    39,   -59,     0,    40,    41,
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
    1169,    39,   -59,     0,    40,    41,    42,     0,    43,  -336,
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
     109,     0,  1020,   110,     5,   305,     6,     7,     8,     9,
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
     106,     0,   204,     0,   108,     0,   205,     0,   921,   110,
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
     108,     0,   922,     0,     5,   110,     6,     7,     8,     9,
      10,    11,     0,   660,     0,    12,     0,     0,    15,    16,
     662,    17,     0,   196,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,     0,
       0,    41,     0,     0,    43,     0,   667,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,   537,   462,
     463,   464,   465,   466,     0,     0,   469,   470,   471,   472,
       0,   474,   475,   888,   889,   890,   891,   892,   680,     0,
     681,     0,   100,     0,   682,   683,   684,   685,   686,   687,
     688,   893,   690,   691,   102,   894,   104,     0,   693,   694,
     895,   696,   204,     0,   108,     0,   205,     0,     5,   110,
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
     106,     0,   204,     0,   108,   861,   205,     0,     0,   110,
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
     108,   863,   205,     0,     5,   110,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   196,     0,     0,    21,     0,   767,     0,
       0,     0,     0,     0,     0,    29,     0,   197,     0,     0,
       0,    33,   198,   199,     0,     0,   200,    39,     0,   768,
       0,    41,     0,     0,    43,     0,     0,   201,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,   769,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,   770,    91,
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
      52,     0,    53,    54,  1022,    55,    56,     0,     0,     0,
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
     104,     0,     0,     0,   106,     0,   204,   859,   108,     0,
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
     106,     0,   204,     0,   108,     0,   205,   869,     5,   110,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   196,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   197,     0,     0,     0,    33,   198,   199,     0,     0,
     200,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   201,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   202,  -752,     0,     0,     0,  -752,
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
     204,     0,   108,     0,   205,   874,     5,   110,     6,     7,
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
      82,     0,     0,     0,    86,  1061,     0,    88,     0,     0,
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
     198,   199,     0,     0,  1219,    39,     0,     0,     0,    41,
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
      82,     0,     0,     0,    86,  1248,     0,    88,     0,     0,
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
     106,     0,   204,     0,   108,     0,   820,     0,     5,   110,
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
     820,     0,     5,   110,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   197,     0,     0,     0,    33,
     198,   199,     0,     0,   949,    39,     0,     0,     0,    41,
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
       0,     0,     0,    33,   198,   199,     0,     0,   952,    39,
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
       0,     0,  1213,    39,     0,     0,     0,    41,     0,     0,
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
       0,    33,   198,   199,     0,     0,  1214,    39,     0,     0,
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
    1216,    39,     0,     0,     0,    41,     0,     0,    43,     0,
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
       0,     0,     0,    33,   198,   199,     0,     0,  1218,    39,
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
       0,     0,  1219,    39,     0,     0,     0,    41,     0,     0,
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
       0,    55,    56,     0,     0,     0,     0,     0,   433,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,    67,
     202,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      76,    77,    78,   203,     0,    80,     0,     0,    82,     0,
       0,     0,    86,     0,     0,    88,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,   434,     0,     0,     0,
     435,     0,     0,    95,    96,    97,    98,    99,     0,     0,
       0,     0,   100,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,   102,   103,   104,     0,     0,     0,
     106,     0,     0,     0,   108,     0,   922,     0,     0,   110,
     305,     6,     7,     8,     9,    10,    11,     0,   437,   438,
      12,   439,   440,   441,    16,   442,   443,     0,   196,     0,
     446,     0,     0,   306,     0,     0,   452,   453,     0,     0,
     456,     0,   197,   457,   458,   459,     0,   198,   199,     0,
       0,   200,     0,     0,   308,     0,     0,     0,   310,     0,
       0,     0,   201,     0,     0,    47,    48,     0,     0,     0,
       0,    52,     0,    53,    54,     0,     0,     0,     0,     0,
       0,     0,   311,     0,    59,   312,    61,    62,    63,     0,
       0,     0,     0,    66,     0,   202,     0,   313,     0,     0,
       0,     0,     0,    74,     0,    76,     0,    78,     0,     0,
       0,     0,     0,     0,     0,     0,   316,     0,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,     0,     0,     0,   106,     0,   204,     0,   108,
    -340,   317,     0,     0,   110,   305,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,     0,    16,
       0,     0,     0,   196,     0,     0,     0,     0,   306,     0,
       0,     0,     0,     0,     0,     0,     0,   197,     0,     0,
       0,     0,   198,   199,     0,     0,   200,     0,     0,   308,
       0,     0,     0,   310,     0,     0,     0,   201,     0,     0,
      47,    48,     0,     0,     0,     0,    52,     0,    53,    54,
       0,     0,     0,     0,     0,     0,     0,   311,     0,    59,
     312,    61,    62,    63,     0,     0,     0,     0,    66,     0,
     202,     0,   313,     0,     0,     0,     0,     0,    74,     0,
      76,     0,    78,     0,     0,     0,     0,     0,     0,     0,
       0,   316,     0,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   658,     0,
     659,     0,     0,    95,    96,    97,    98,    99,     0,   660,
       0,     0,   100,     0,   661,   237,   662,   663,     0,     0,
       0,   101,   664,     0,   102,   103,   104,     0,     0,     0,
     106,   240,   204,   197,   108,     0,   317,   241,     0,   110,
       0,     0,     0,   665,     0,     0,     0,   243,     0,     0,
     666,     0,   667,     0,     0,     0,     0,     0,     0,     0,
     668,     0,     0,     0,     0,     0,     0,   246,   669,     0,
       0,     0,     0,     0,     0,   248,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   670,     0,     0,     0,     0,
       0,     0,     0,     0,   250,   251,     0,   671,     0,   253,
       0,   672,     0,     0,   255,     0,     0,     0,   673,     0,
       0,   257,     0,     0,     0,   674,     0,     0,     0,     0,
       0,     0,     0,     0,   537,   462,   463,   464,   465,   466,
       0,     0,   469,   470,   471,   472,     0,   474,   475,   675,
     676,   677,   678,   679,   680,     0,   681,     0,     0,     0,
     682,   683,   684,   685,   686,   687,   688,   689,   690,   691,
       0,   692,     0,     0,   693,   694,   695,   696,     0,     5,
     697,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   196,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   197,     0,     0,     0,    33,   198,   199,     0,
       0,   200,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   201,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   202,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   203,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,    96,
      97,    98,    99,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,     0,
     103,   104,     0,     0,     0,   106,     0,   204,     0,   108,
       0,   205,     5,   305,     6,     7,     8,     9,    10,    11,
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
       0,     0,     0,     0,     0,     0,  -530,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    97,     0,     0,     0,  -530,     0,     0,
       0,  -530,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   104,     0,     0,     0,     0,     0,
     204,     0,   108,  -530,   317,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   196,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   197,     0,
       0,     0,    33,   198,   199,     0,     0,   200,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   201,     0,
       0,    47,    48,  -509,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   202,     0,     0,     0,  -509,     0,     0,     0,    74,
      75,    76,    77,    78,   203,     0,    80,     0,  -509,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,     0,
      91,     0,  -509,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    97,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   196,   104,  -509,    21,
       0,     0,     0,  -509,     0,   108,     0,   922,    29,     0,
     197,     0,     0,     0,    33,   198,   199,     0,     0,   200,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     201,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,    67,   202,     0,     0,     0,     0,     0,     0,
       0,    74,    75,    76,    77,    78,   203,     0,    80,     0,
       0,    82,     0,     0,     0,    86,     0,     0,    88,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,    97,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   196,   104,
       0,    21,     0,     0,     0,   204,     0,   108,     0,   205,
      29,     0,   197,     0,     0,     0,    33,   198,   199,     0,
       0,   200,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   201,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,   202,     0,     0,     0,     0,
       0,     0,     0,    74,    75,    76,    77,    78,   203,     0,
      80,     0,     0,    82,     0,     0,     0,    86,     0,     0,
      88,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      97,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     196,   104,     0,    21,     0,     0,     0,   204,     0,   108,
       0,   820,    29,     0,   197,     0,     0,     0,    33,   198,
     199,     0,     0,   200,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   201,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,    67,   202,     0,   235,
       0,     0,     0,     0,     0,    74,    75,    76,    77,    78,
     203,     0,    80,   236,   237,    82,   238,     0,     0,    86,
       0,   239,    88,     0,     0,     0,    91,     0,     0,     0,
     240,     0,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,   242,     0,     0,     0,   243,     0,     0,   244,
       0,     0,    97,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,     0,   246,   247,     0,     0,
       0,     0,     0,     0,   248,   431,     0,     0,     0,     0,
       0,   108,   432,   205,   249,     0,     0,   235,     0,     0,
       0,     0,     0,   250,   251,   433,   252,     0,   253,     0,
     254,   236,   237,   255,   238,     0,     0,   256,     0,   239,
     257,   493,     0,     0,   258,     0,     0,     0,   240,     0,
       0,     0,     0,     0,   241,     0,     0,   722,     0,     0,
     242,     0,     0,     0,   243,     0,   660,   244,     0,     0,
       0,     0,     0,   662,   723,     0,     0,   245,     0,     0,
       0,     0,     0,   434,   246,   247,     0,   435,   724,     0,
       0,     0,   248,     0,     0,     0,     0,   491,     0,  1045,
       0,     0,   249,     0,     0,     0,     0,     0,     0,   667,
       0,   250,   251,     0,   252,   216,   253,   725,   254,   436,
       0,   255,     0,     0,     0,   256,     0,     0,   257,     0,
       0,     0,   258,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   726,     0,
     457,   458,   459,     0,   727,     0,     0,     0,     0,     0,
       0,   460,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   537,   462,   463,   464,   465,   466,   282,     0,   469,
     470,   471,   472,     0,   474,   475,   675,   676,   677,   678,
     679,   680,     0,   681,     0,     0,     0,   682,   683,   684,
     685,   686,   687,   688,   689,   690,   691,   235,   692,     0,
       0,   693,   694,   695,   696,  1058,   660,     0,     0,     0,
       0,   236,   237,   662,   238,     0,     0,     0,     0,   239,
       0,     0,     0,     0,     0,     0,     0,     0,   240,     0,
       0,     0,     0,     0,   241,     0,     0,     0,     0,     0,
     242,     0,     0,     0,   243,     0,     0,   244,     0,   667,
       0,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,   246,   247,     0,     0,     0,     0,
       0,     0,   248,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   249,     0,     0,     0,     0,     0,     0,     0,
     431,   250,   251,     0,   252,     0,   253,   432,   254,     0,
       0,   255,     0,     0,     0,   256,     0,     0,   257,     0,
     433,     0,   258,     0,     0,     0,     0,     0,     0,     0,
       0,   537,   462,   463,   464,   465,   466,     0,     0,   469,
     470,   471,   472,     0,   474,   475,   675,   676,   677,   678,
     679,   680,     0,   681,     0,     0,     0,   682,   683,   684,
     685,   686,   687,   688,   689,   690,   691,     0,   692,     0,
       0,   693,   694,   695,   696,     0,   431,     0,   434,     0,
       0,     0,   435,   432,     0,     0,     0,     0,     0,     0,
       0,   536,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,   537,   462,   463,   464,   465,
     466,     0,     0,   469,   470,   471,   472,     0,   474,   475,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,   434,   457,   458,   459,   435,   491,
       0,   431,     0,     0,     0,     0,   460,  1120,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
     436,   537,   462,   463,   464,   465,   466,     0,     0,   469,
     470,   471,   472,     0,   474,   475,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
       0,   457,   458,   459,     0,   491,     0,   431,     0,   434,
       0,     0,   460,   435,   432,     0,     0,     0,     0,     0,
       0,     0,  1127,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,   537,   462,   463,   464,
     465,   466,     0,     0,   469,   470,   471,   472,     0,   474,
     475,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,   434,   457,   458,   459,   435,
     491,     0,   431,     0,     0,     0,     0,   460,  1285,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,   436,   537,   462,   463,   464,   465,   466,     0,     0,
     469,   470,   471,   472,     0,   474,   475,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,     0,   457,   458,   459,     0,   491,     0,   431,     0,
     434,     0,     0,   460,   435,   432,     0,     0,     0,     0,
       0,     0,     0,  1286,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   436,   537,   462,   463,
     464,   465,   466,     0,     0,   469,   470,   471,   472,     0,
     474,   475,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,   434,   457,   458,   459,
     435,   491,     0,   431,     0,     0,     0,     0,   460,  1287,
     432,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   537,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,     0,   491,     0,   431,
       0,   434,     0,     0,   460,   435,   432,     0,     0,     0,
       0,     0,     0,     0,  1288,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,   537,   462,
     463,   464,   465,   466,     0,     0,   469,   470,   471,   472,
       0,   474,   475,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,     0,   434,   457,   458,
     459,   435,   491,     0,   431,     0,     0,     0,     0,   460,
    1294,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   537,   462,   463,   464,   465,   466,
       0,     0,   469,   470,   471,   472,     0,   474,   475,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,     0,   491,     0,
     431,     0,   434,     0,     0,   460,   435,   432,     0,     0,
       0,     0,     0,     0,     0,  1295,     0,     0,     0,     0,
     433,   523,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   524,     0,   436,   537,
     462,   463,   464,   465,   466,     0,     0,   469,   470,   471,
     472,     0,   474,   475,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,   434,   457,
     458,   459,   435,   491,     0,   431,     0,     0,     0,     0,
     460,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   286,     0,     0,     0,
     520,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,   491,
       0,     0,     0,   434,     0,     0,   460,   435,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   520,     0,   433,     0,   436,
       0,     0,   628,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   629,     0,
     457,   458,   459,     0,   491,     0,     0,     0,     0,     0,
       0,   460,     0,     0,     0,   434,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   431,
     884,     0,     0,     0,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,   436,     0,     0,   885,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     886,   630,   457,   458,   459,     0,     0,     0,     0,     0,
       0,     0,     0,   460,     0,   431,     0,   434,     0,     0,
       0,   435,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   997,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,   433,   286,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     520,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,   434,   457,   458,   459,   435,   491,
       0,   431,     0,     0,     0,     0,   460,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,   758,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,   759,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
       0,   457,   458,   459,     0,   491,     0,   431,     0,   434,
       0,     0,   460,   435,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,   760,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   761,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,   433,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,   434,   457,   458,   459,   435,
       0,     0,   431,     0,     0,     0,     0,   460,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,   989,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,   434,     0,     0,     0,   435,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,     0,   457,   458,   459,     0,     0,     0,   431,     0,
     434,     0,     0,   460,   435,   432,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   433,   446,
     447,   448,     0,   450,   451,   452,   453,     0,     0,   456,
       0,     0,   457,   458,   459,     0,   436,     0,     0,     0,
       0,     0,   431,   460,     0,     0,     0,     0,     0,   432,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,   433,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,   434,   457,   458,   459,
     435,   491,     0,     0,     0,     0,     0,     0,   460,  1011,
       0,     0,     0,     0,     0,     0,   431,   560,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,   433,     0,     0,     0,
     434,     0,     0,     0,   435,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,   520,   457,   458,   459,   436,   491,     0,     0,
       0,     0,     0,     0,   460,     0,     0,     0,     0,     0,
       0,     0,   437,   438,   434,   439,   440,   441,   435,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   431,     0,   457,   458,   459,
       0,     0,   432,     0,     0,     0,  1052,     0,   460,     0,
     436,     0,     0,     0,     0,   433,     0,     0,     0,     0,
    1178,     0,     0,     0,     0,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
       0,   457,   458,   459,     0,     0,  1179,     0,     0,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,   434,     0,     0,     0,   435,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,   431,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,   437,   438,     0,   439,   440,
     441,   433,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,   434,
     457,   458,   459,   435,     0,     0,   431,     0,     0,     0,
       0,   460,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,   434,
       0,     0,     0,   435,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,   436,   457,   458,   459,     0,
     587,     0,     0,     0,   434,     0,     0,   460,   435,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   431,   591,   457,   458,   459,   615,
     436,   432,     0,     0,     0,     0,     0,   460,     0,     0,
       0,     0,     0,     0,   433,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   431,
       0,   457,   458,   459,     0,     0,   432,     0,     0,     0,
       0,     0,   460,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   434,     0,     0,     0,   435,     0,     0,   431,
     880,     0,     0,     0,     0,   762,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,   435,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,     0,   457,
     458,   459,     0,   436,     0,   431,     0,   434,     0,     0,
     460,   435,   432,     0,     0,     0,     0,     0,     0,   437,
     438,     0,   439,   440,   441,   433,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,   436,   457,   458,   459,     0,     0,   431,
       0,   860,     0,     0,     0,   460,   432,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   881,   444,   433,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,   434,   457,   458,   459,   435,     0,     0,
       0,     0,     0,     0,   431,   460,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,   216,     0,  1009,     0,   436,
       0,     0,     0,     0,     0,     0,     0,   434,     0,     0,
       0,   435,     0,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,     0,
     457,   458,   459,   436,     0,     0,     0,     0,     0,     0,
       0,   460,   434,     0,     0,     0,   435,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   431,   980,   457,   458,   459,     0,   436,   432,
       0,     0,     0,     0,     0,   460,     0,     0,     0,     0,
       0,     0,   433,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   431,     0,   457,
     458,   459,     0,     0,   432,     0,     0,     0,     0,     0,
     460,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,   435,     0,     0,   431,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,   435,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,     0,   457,   458,   459,
       0,   436,     0,   431,  1025,   434,     0,     0,   460,   435,
     432,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,   433,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
    1055,   436,   457,   458,   459,     0,     0,   431,  1067,  1026,
       0,     0,     0,   460,   432,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,   433,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   434,   457,   458,   459,   435,     0,     0,     0,     0,
       0,     0,   431,   460,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   433,  1105,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,   434,     0,     0,     0,   435,
       0,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,     0,     0,   457,   458,
     459,   436,     0,     0,     0,  1098,     0,     0,     0,   460,
     434,     0,     0,     0,   435,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     431,     0,   457,   458,   459,     0,   436,   432,     0,     0,
       0,     0,     0,   460,     0,     0,     0,     0,     0,     0,
     433,  1108,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   431,     0,   457,   458,   459,
       0,     0,   432,     0,     0,     0,     0,     0,   460,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,   431,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,   434,     0,     0,     0,   435,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,   436,
       0,   431,  1175,   434,     0,     0,   460,   435,   432,     0,
       0,     0,     0,     0,     0,   437,   438,     0,   439,   440,
     441,   433,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,   436,
     457,   458,   459,     0,     0,     0,     0,     0,     0,  1111,
       0,   460,     0,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,   434,
     457,   458,   459,   435,     0,     0,   431,     0,     0,  1151,
       0,   460,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,     0,   457,   458,   459,     0,
       0,     0,   431,     0,   434,     0,     0,   460,   435,   432,
       0,     0,     0,     0,     0,     0,     0,  1212,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,   431,  1254,     0,     0,     0,     0,     0,   432,
       0,     0,     0,     0,     0,     0,   437,   438,     0,   439,
     440,   441,   433,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
     434,   457,   458,   459,   435,     0,     0,     0,     0,   431,
       0,     0,   460,  1215,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
     434,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,  1273,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,   436,   457,   458,   459,
       0,     0,     0,     0,     0,     0,     0,   434,   460,     0,
       0,   435,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   431,     0,   457,   458,   459,
       0,     0,   432,   436,     0,     0,     0,     0,   460,     0,
       0,     0,     0,     0,     0,   433,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,     0,     0,     0,
       0,     0,     0,     0,     0,   460,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   431,     0,   434,     0,     0,     0,   435,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,   434,
     457,   458,   459,   435,     0,     0,   431,     0,     0,  1290,
       0,   460,  1313,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,     0,     0,   457,   458,   459,     0,
       0,     0,   431,     0,   434,     0,     0,   460,   435,   432,
       0,     0,     0,     0,     0,     0,     0,  1314,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
     434,   457,   458,   459,   435,     0,     0,   431,     0,     0,
       0,     0,   460,  1315,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,     0,   457,   458,   459,
       0,     0,     0,   431,     0,   434,     0,     0,   460,   435,
     432,     0,     0,     0,     0,     0,     0,     0,  1316,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,   431,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,   433,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   434,   457,   458,   459,   435,     0,     0,   431,     0,
       0,     0,     0,   460,  1317,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,   434,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,     0,   436,   457,   458,
     459,     0,     0,     0,     0,     0,   434,     0,     0,   460,
     435,     0,     0,   437,   438,     0,   439,   440,   441,  1318,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   431,     0,   457,   458,
     459,     0,   436,   432,     0,     0,     0,     0,     0,   460,
       0,     0,     0,     0,     0,     0,   433,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,     0,     0,   457,   458,   459,     0,     0,     0,     0,
       0,     0,     0,     0,   460,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   975,     0,     0,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   500,
       0,     0,     0,    22,    23,     0,     0,     0,  1075,     0,
     436,     0,     0,   501,     0,    31,   502,     0,     0,     0,
       0,    37,     0,     0,     0,     0,   437,   438,    42,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
     433,   457,   458,   459,    58,     0,    60,     0,    62,     0,
    1076,     0,   460,  1077,     0,     0,   503,   500,    69,     0,
       0,    22,    23,     0,     0,     0,     0,     0,     0,     0,
       0,   501,     0,    31,   502,     0,     0,    85,     0,    37,
      87,     0,  1078,     0,    90,     0,    42,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,     0,     0,  1076,     0,
       0,  1077,     0,     0,   503,     0,    69,     0,     0,     0,
       0,     0,     0,   105,   436,     0,     0,     0,     0,  1255,
       0,     0,     0,     0,     0,    85,     0,     0,    87,     0,
     437,   438,    90,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,     0,   450,   451,   452,   453,
     454,   455,   456,     0,   500,   457,   458,   459,    22,    23,
       0,     0,     0,  1075,     0,     0,   460,     0,   501,     0,
      31,   502,     0,     0,     0,     0,    37,     0,     0,     0,
       0,   105,     0,    42,     0,     0,   400,     0,   235,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   236,   237,     0,   238,     0,     0,     0,    58,
     239,    60,     0,   382,     0,  1076,   401,     0,  1077,   240,
       0,   503,     0,    69,     0,   241,     0,     0,     0,     0,
       0,   242,     0,     0,     0,   243,     0,     0,   244,     0,
       0,     0,    85,     0,     0,    87,     0,  1078,   245,    90,
       0,     0,     0,     0,     0,   246,   247,     0,     0,     0,
       0,     0,     0,   248,   235,     0,     0,     0,     0,     0,
       0,     0,     0,   249,     0,     0,     0,     0,   236,   237,
       0,   238,   250,   251,     0,   252,   239,   253,    23,   254,
       0,     0,   255,     0,     0,   240,   256,     0,   105,   257,
     402,   241,     0,   258,     0,     0,     0,   242,     0,     0,
       0,   243,     0,     0,   244,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,     0,     0,     0,     0,     0,
       0,   246,   247,   235,     0,     0,     0,     0,     0,   248,
      60,     0,     0,     0,     0,     0,     0,   236,   237,   249,
     238,     0,    69,     0,     0,   239,     0,     0,   250,   251,
       0,   252,     0,   253,   240,   254,     0,     0,   255,     0,
     241,     0,   256,     0,     0,   257,   242,     0,    90,   258,
     243,     0,     0,   244,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   245,     0,     0,     0,     0,     0,     0,
     246,   247,     0,     0,     0,     0,     0,     0,   248,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   249,     0,
       0,   235,     0,     0,     0,     0,     0,   250,   251,     0,
     252,     0,   253,     0,   254,   236,   237,   255,   238,     0,
       0,   256,   575,   239,   257,     0,     0,     0,   258,   401,
       0,     0,   240,     0,     0,     0,     0,     0,   241,     0,
       0,     0,     0,     0,   242,     0,     0,     0,   243,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,     0,
    -302,   245,     0,     0,     0,     0,     0,     0,   246,   247,
       0,     0,     0,     0,  -302,  -302,   248,  -302,     0,     0,
       0,     0,  -302,     0,     0,     0,   249,     0,     0,     0,
       0,  -302,     0,     0,     0,   250,   251,  -302,   252,     0,
     253,     0,   254,  -302,     0,   255,     0,  -302,     0,   256,
    -302,     0,   257,     0,     0,     0,   258,     0,     0,   235,
    -302,     0,     0,     0,     0,     0,     0,  -302,  -302,     0,
       0,     0,     0,   236,   237,  -302,   238,     0,     0,     0,
       0,   239,     0,     0,     0,  -302,     0,     0,     0,     0,
     240,     0,     0,     0,  -302,  -302,   241,  -302,     0,  -302,
       0,  -302,   242,     0,  -302,     0,   243,     0,  -302,   244,
       0,  -302,     0,     0,     0,  -302,     0,     0,  -303,   245,
       0,     0,     0,     0,     0,     0,   246,   247,     0,     0,
       0,     0,  -303,  -303,   248,  -303,     0,     0,     0,     0,
    -303,     0,     0,     0,   249,     0,     0,     0,     0,  -303,
       0,     0,     0,   250,   251,  -303,   252,     0,   253,     0,
     254,  -303,     0,   255,     0,  -303,     0,   256,  -303,     0,
     257,     0,     0,     0,   258,     0,     0,     0,  -303,     0,
       0,     0,     0,     0,     0,  -303,  -303,     0,     0,     0,
       0,     0,     0,  -303,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -303,     0,     0,     0,     0,     0,     0,
       0,     0,  -303,  -303,     0,  -303,     0,  -303,     0,  -303,
       0,     0,  -303,     0,     0,     0,  -303,     0,     0,  -303,
       0,     0,     0,  -303
};

static const yytype_int16 yycheck[] =
{
      12,    66,    19,   166,    16,   585,   488,    70,    20,   423,
      66,    71,     2,    66,    26,   704,    40,   278,   182,   232,
     792,   628,    34,    35,    36,   910,    38,   268,   269,   270,
     839,   651,   886,    93,   542,   136,   579,    49,   393,    51,
      52,   302,   641,   234,   643,    57,   109,    59,   108,    61,
     589,   225,   846,   881,   700,   840,   702,   569,    70,    71,
      72,    73,    74,    75,     1,    33,    33,    79,     1,    81,
      48,  1030,    33,   904,    33,    33,    88,    48,   409,     3,
      92,    93,   552,    95,    56,    97,   347,   278,   100,   101,
      33,   103,    22,     1,    56,   107,   108,   109,   110,     3,
     103,     3,  1208,   122,     1,   547,    61,    47,   122,    54,
     274,   302,   920,   148,   115,     1,    48,  1091,    83,    84,
      51,    48,   134,    54,   136,   127,    66,    48,     0,    74,
      37,    66,    48,   166,   576,    70,   377,   378,    75,    84,
      48,   176,   205,    74,   357,    85,    77,   148,   303,   304,
     152,   122,    66,    84,   122,  1261,   347,   176,   173,    89,
     175,   122,   176,   175,    61,   120,    73,    93,   171,    93,
     148,   172,   107,   108,   109,    61,   116,   148,    93,   110,
     104,   148,   119,   113,   654,  1070,   198,   199,   200,   961,
     148,   203,   204,   205,   127,  1080,    93,  1156,   176,   819,
     104,   173,   104,   171,   171,   176,   223,    93,   232,   106,
     171,   173,   171,   171,   147,   148,   148,   478,   479,   127,
     106,   148,   234,   120,  1032,  1019,  1200,   148,   171,    34,
      35,    36,   148,    75,   120,   172,  1030,  1065,  1069,   147,
     148,   174,   573,    27,   176,   171,   785,   120,  1033,   176,
     147,   166,   443,    48,   317,   763,   171,   148,   166,   423,
     176,   147,    56,   148,    48,   142,   278,   125,    52,   204,
     205,    47,   148,   150,   171,   148,   631,   119,   885,    61,
     166,   122,     3,   174,    27,   171,   499,   478,   479,   174,
     302,   392,    76,    27,   110,    79,    91,    81,   174,    61,
    1185,   844,   314,   315,   109,   317,  1191,    91,  1193,    85,
     792,    93,   911,   171,   913,   148,    27,   173,    18,   175,
    1114,   333,   334,  1177,   106,  1179,   110,   839,   148,   148,
     172,    93,   495,   357,   397,   347,    79,    48,   120,   172,
     116,    52,  1151,   408,   106,    79,   148,    56,    91,    48,
     148,   542,   172,  1125,   974,   174,   166,    91,   120,    56,
     372,   148,  1156,   173,   166,    76,    56,   110,    79,   148,
      81,     3,  1257,   385,  1259,   387,   174,   148,    78,   369,
      91,   594,   317,   117,   166,   397,   560,   174,    20,   171,
     174,   148,    91,   148,   148,   174,    91,    97,   589,   110,
     908,    90,    34,   174,   166,    94,    27,   467,   468,   171,
       8,   166,   166,   473,   114,   172,   476,   477,    11,   431,
     432,   433,   434,   435,   436,   173,   438,   439,   440,   441,
     442,    63,   444,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   457,   458,   459,   460,    56,
     958,  1058,   515,  1099,  1100,   467,   468,   148,    79,  1148,
     634,   473,   397,   174,   476,   477,   478,   479,   480,   481,
      91,    27,   104,   148,   148,   499,   488,   148,   110,   961,
     148,   493,   148,   174,   166,   497,   148,   148,    47,   110,
     148,   173,    48,   175,   506,   166,    52,   148,   166,   174,
     174,    56,   148,   515,   166,   517,   518,    66,   174,   574,
       2,   523,   524,   174,   148,   166,   174,   529,   574,   933,
      76,   574,   148,    79,   536,    81,    85,   539,   174,   173,
     542,   175,   166,    25,   148,    91,    28,   127,   166,    56,
      48,     2,   554,    61,   172,   605,   558,   607,   174,   561,
     562,   173,   166,    61,   110,   796,   797,   116,   148,   800,
     801,   127,   152,   148,    25,   148,    56,    28,   580,  1176,
     594,  1178,   763,    83,     8,    93,    86,   589,   148,   148,
     515,   166,   148,   166,   174,    93,   152,    79,   106,    56,
     602,    83,    84,   605,   785,   607,   166,   166,   106,   611,
     612,   613,   120,   615,   616,   166,   148,    56,   174,    90,
      90,   172,   120,    94,    94,   107,   628,   629,    79,   127,
     112,    90,    83,    84,   166,    94,   867,   868,  1208,   434,
     435,   621,   644,  1202,   569,   173,    56,    90,   552,   574,
     148,    94,   142,  1125,   579,   580,   107,   147,   166,   101,
     150,   112,    90,   171,   154,   569,    94,   173,   166,   175,
     574,    56,    27,   171,   173,   579,   174,   602,    56,   171,
     605,   173,   607,    56,    56,   480,   481,   612,   173,     8,
     175,  1261,   173,    48,   175,   697,    56,    52,   493,   880,
     881,   173,   605,   175,   607,   171,    56,   147,  1267,  1268,
    1269,  1270,  1271,  1272,     2,   173,   166,   165,    56,   166,
     166,    76,   166,   518,    79,   166,    81,   908,   166,   524,
     166,    48,   166,   783,   529,   217,    91,    25,   173,   171,
      28,   166,   121,   173,    61,   171,    85,   174,   148,    32,
     654,   173,   173,   906,   173,   110,   758,   759,   760,   761,
     762,   763,   764,   765,   147,   166,   217,   820,   173,   776,
     777,   166,     8,    66,   824,   171,    93,   958,   172,   933,
       8,   783,     8,   785,   786,   127,   788,    91,    91,   106,
     792,    79,   794,   127,    32,    83,    84,   127,   150,   281,
     704,   803,   804,   120,   286,    69,   173,    90,   174,   127,
     127,    94,   148,    48,   173,   127,   611,   127,   820,   107,
      35,    35,   824,   153,   112,   155,   156,    66,   158,   159,
     281,   148,   986,   174,   174,   286,   838,   839,   127,   993,
     147,   174,    21,   171,   176,   176,   148,   106,   850,   166,
     173,   150,    90,   150,   171,   910,    94,   174,   783,   142,
     150,   142,   176,   918,   147,   150,   150,   150,   150,   922,
     150,   154,   150,   150,   929,   150,   878,   150,   150,   150,
     882,   150,   884,   885,   886,    35,   888,   369,   890,    35,
     166,   893,   894,     3,   173,   820,   166,   147,  1149,  1150,
     166,   166,   384,   166,   142,   166,   908,    17,   148,   147,
      20,   149,   150,   166,   839,    25,   154,   174,   369,   844,
     922,   172,   174,   161,   171,   115,    36,   172,   174,   217,
      33,   169,   176,   384,    33,   839,    46,    35,   147,   176,
     844,   423,   166,    53,   166,   176,   173,   949,   173,   166,
     952,     8,     8,    63,   148,   176,   958,   176,   176,   961,
      35,    71,   174,   174,   759,   166,   761,    22,  1149,  1150,
     174,    26,    27,   975,   976,   174,    31,   166,    88,   166,
     165,    36,   165,    38,    39,   987,   148,   989,  1041,    44,
     100,   277,   994,   281,   104,   997,    51,   922,   286,   878,
     384,   111,    24,  1080,  1080,    92,   354,  1009,   118,   491,
    1045,   558,  1015,   574,   916,  1070,   917,   929,   395,   918,
     933,   571,    77,   318,    79,  1261,    81,  1089,    83,  1031,
    1273,    86,  1266,  1035,    89,   517,    91,   905,   706,  1041,
     491,   523,   134,   555,    -1,   818,    -1,    -1,  1050,    -1,
    1052,    -1,   534,  1055,   536,   110,  1058,    -1,   113,    -1,
     115,     2,   117,  1065,    -1,  1067,   517,    -1,    -1,  1223,
    1224,    -1,   523,  1227,  1228,    -1,    -1,    -1,    -1,    -1,
      -1,   369,    -1,   534,    25,   536,    -1,    28,    -1,  1243,
      -1,  1245,  1094,    -1,    -1,    -1,   384,    -1,    -1,    -1,
      -1,    -1,  1104,  1105,    -1,  1107,  1108,    -1,  1110,  1111,
    1035,   166,    -1,    -1,    -1,    -1,  1041,   172,  1120,  1121,
      -1,  1123,    -1,  1125,    -1,  1127,  1128,   609,    -1,  1131,
    1185,   613,    -1,    -1,    -1,    -1,    -1,    -1,    79,   621,
      -1,    -1,    83,    84,    -1,  1147,  1148,  1149,  1150,  1151,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   609,    -1,
       2,  1163,   613,  1165,    -1,    -1,   107,  1169,    -1,    -1,
     621,   112,    32,  1175,  1176,  1177,  1178,  1179,    -1,    -1,
     975,   976,    -1,    25,    -1,    -1,    28,  1091,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1200,    -1,
      -1,    -1,    -1,   491,    -1,    -1,    -1,    -1,    -1,    -1,
    1212,  1213,  1214,  1215,  1216,  1217,  1218,  1219,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1151,    -1,    -1,   517,
      90,    -1,    -1,    -1,    94,   523,    -1,    79,    -1,    -1,
      -1,    83,    84,    -1,  1148,    -1,   534,  1151,   536,    -1,
      -1,    -1,  1254,    -1,    -1,     2,    -1,    -1,    -1,    48,
      -1,    -1,    -1,  1265,    -1,   107,    -1,    -1,    -1,    -1,
     112,    -1,    61,    -1,    -1,    -1,   217,    -1,    25,    -1,
      -1,    28,   142,  1285,  1286,  1287,  1288,   147,  1290,   149,
     150,   773,  1294,  1295,   154,    -1,  1200,    -1,    -1,    -1,
      -1,   161,    -1,    -1,    93,    -1,    -1,   167,   168,   169,
      -1,  1313,  1314,  1315,  1316,  1317,  1318,   106,    -1,    -1,
      -1,   609,    -1,    -1,   806,   613,    -1,   809,    -1,    -1,
      -1,   120,    79,   621,    -1,    -1,    83,    84,   127,    -1,
     281,    -1,    -1,    -1,    -1,   286,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   806,    -1,    -1,   809,   148,
     107,    -1,    32,    -1,    -1,   112,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,   217,     1,   166,    -1,    -1,
      -1,    -1,   171,    -1,   866,   174,  1290,    -1,    -1,    -1,
     872,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,   866,    -1,    -1,    -1,    44,
      90,   872,    -1,    -1,    94,    -1,    51,    -1,   369,    54,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   281,
      -1,    -1,    -1,   384,   286,    -1,    -1,    -1,    -1,    74,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,    84,
      -1,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,
     217,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,    -1,    -1,    -1,   154,   110,    -1,    -1,   113,    -1,
     115,   161,   117,   975,   976,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,   987,    -1,   989,    -1,    -1,
      -1,    -1,   994,    -1,    -1,   997,    -1,    -1,   806,    -1,
      -1,   809,    -1,    -1,  1006,    -1,  1008,   369,    -1,  1011,
      -1,    -1,    -1,    -1,   281,    -1,   987,    -1,   989,   286,
      -1,   166,   384,   994,    -1,    -1,   997,   172,    -1,    -1,
     491,    -1,    -1,    -1,    -1,  1006,    -1,  1008,  1040,    -1,
    1011,   265,   266,   267,   268,   269,   270,    -1,  1050,    -1,
    1052,    -1,    -1,    -1,    -1,    -1,   517,    -1,   866,    -1,
      -1,    -1,   523,    -1,   872,   289,    -1,   291,    -1,  1040,
      -1,    -1,    -1,   534,   298,   536,    -1,    -1,    -1,  1050,
      -1,  1052,     2,    -1,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,    -1,
      -1,    -1,   369,    -1,    -1,    25,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   384,  1120,   343,
      -1,    -1,    -1,    -1,    -1,  1127,    -1,   170,    -1,   491,
     173,    -1,   175,    -1,   358,   359,    -1,    -1,  1140,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   609,  1120,
      -1,    -1,   613,    -1,    -1,   517,  1127,  1159,    -1,    79,
     621,   523,    -1,    83,    84,    -1,  1168,    -1,    -1,  1140,
    1172,    -1,   534,    -1,   536,    -1,    -1,    -1,    -1,   987,
      -1,   989,    -1,    -1,    -1,    -1,   994,   107,  1159,   997,
      -1,    -1,   112,    -1,    -1,    -1,    -1,  1168,  1006,    -1,
    1008,  1172,    -1,  1011,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   491,    -1,    -1,  1229,    -1,    -1,
      -1,    -1,  1040,    -1,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,  1050,    -1,  1052,    -1,    -1,   609,    -1,    -1,
     517,   613,   108,    -1,    -1,    -1,   523,    -1,  1229,   621,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   534,    -1,   536,
     494,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       2,    32,    -1,  1285,  1286,  1287,  1288,    -1,    -1,    -1,
      -1,    -1,  1294,  1295,    -1,    -1,    -1,   217,   522,    -1,
      -1,    -1,   526,    25,   528,    -1,    28,   531,    -1,    -1,
      -1,    -1,  1120,    -1,  1285,  1286,  1287,  1288,    -1,  1127,
      -1,    -1,    -1,  1294,  1295,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1140,    -1,    -1,    -1,    -1,    -1,    -1,    90,
      -1,    -1,   609,    94,    -1,   806,   613,     2,   809,    -1,
      -1,  1159,    -1,    -1,   621,    -1,    -1,    79,    -1,    -1,
    1168,   281,    -1,    -1,  1172,    -1,   286,    -1,    -1,    -1,
      25,    -1,    -1,    28,    -1,    -1,    -1,    -1,   234,    32,
      -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,
     112,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,    -1,   154,    -1,   866,    -1,    -1,    -1,   160,
     161,   872,    -1,   164,    -1,    -1,   167,   168,   169,    -1,
      -1,  1229,   278,    -1,    79,    -1,    -1,   178,    83,    84,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,   806,    -1,   302,   809,    -1,   369,
      -1,    -1,   107,    -1,    -1,    -1,    -1,   112,    -1,    -1,
      -1,    -1,    -1,    -1,   384,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,    -1,    -1,    -1,  1285,  1286,  1287,
    1288,    -1,    -1,    -1,    -1,    -1,  1294,  1295,    -1,   142,
     143,   347,   145,   146,   147,   217,   149,   150,   151,    -1,
     153,   154,   155,   156,   866,   158,   159,   160,   161,   162,
     872,   164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,   987,    -1,   989,    -1,
      -1,    -1,    -1,   994,    -1,    -1,   997,    -1,    -1,   806,
      -1,    -1,   809,    -1,    -1,  1006,    32,  1008,    -1,    -1,
    1011,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   281,
      -1,    -1,   217,    -1,   286,    -1,   790,   791,    -1,    -1,
      -1,   491,   796,   797,    -1,     2,   800,   801,    -1,  1040,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1050,
      -1,  1052,    -1,    -1,    -1,    -1,    -1,   517,    25,   866,
      -1,    28,    -1,   523,    90,   872,    -1,    -1,    94,    -1,
      -1,   467,   468,    -1,   534,    -1,   536,   473,    -1,    -1,
     476,   477,   478,   479,    -1,   987,   281,   989,    -1,    -1,
      -1,   286,   994,    -1,    -1,   997,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1006,    -1,  1008,   369,    -1,  1011,
      -1,    -1,    79,    -1,    -1,    -1,   142,   143,    -1,  1120,
     146,   147,   384,   149,   150,    -1,  1127,    -1,   154,    -1,
      -1,    -1,    -1,    -1,    -1,   161,    -1,    -1,  1040,  1140,
     107,   167,   168,   169,    -1,   112,   542,    -1,  1050,   609,
    1052,    -1,    -1,   613,    -1,    -1,    -1,    -1,  1159,    -1,
      -1,   621,    -1,    -1,    -1,    -1,    -1,  1168,    -1,    -1,
      -1,  1172,    -1,    -1,   369,    -1,    -1,    -1,    -1,    -1,
     987,    -1,   989,    -1,    -1,    -1,    -1,   994,    -1,   384,
     997,    -1,    -1,   589,    32,    -1,    -1,    -1,    -1,  1006,
      -1,  1008,    -1,    -1,  1011,    -1,    -1,    -1,    -1,   605,
      -1,   607,    -1,   977,   978,    -1,    -1,    -1,  1120,    -1,
     984,    -1,    -1,    -1,    -1,  1127,    -1,    -1,  1229,   491,
      -1,    -1,   996,  1040,    -1,   999,    -1,  1001,  1140,  1003,
      -1,    -1,    -1,  1050,    -1,  1052,    -1,    -1,    -1,    -1,
     217,    -1,    90,    -1,    -1,   517,    94,  1159,    -1,    -1,
      -1,   523,    -1,    -1,    -1,    -1,  1168,    -1,    -1,    -1,
    1172,    -1,   534,    -1,   536,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1285,  1286,  1287,  1288,    -1,    -1,
      -1,    -1,    -1,  1294,  1295,    -1,   491,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,   143,    -1,    -1,    -1,   147,
      -1,   149,   150,  1120,   281,    -1,   154,    -1,    -1,   286,
    1127,    -1,   517,   161,    -1,    -1,    -1,  1229,   523,   167,
     168,   169,    -1,  1140,    -1,    -1,    -1,    -1,    -1,   534,
      -1,   536,    -1,    -1,    -1,    -1,   806,   609,    -1,   809,
      -1,   613,  1159,    -1,    -1,    -1,    -1,    -1,    -1,   621,
      -1,  1168,    -1,    -1,    -1,  1172,    -1,   763,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1285,  1286,  1287,  1288,   783,    -1,   785,
      -1,    -1,  1294,  1295,    -1,    -1,    -1,  1161,    -1,    -1,
      -1,    -1,   369,    -1,    -1,    -1,   866,    -1,    49,    -1,
      -1,    -1,   872,    -1,   609,    -1,    -1,   384,   613,    -1,
      -1,    -1,  1229,    -1,     3,    -1,   621,    -1,   824,    -1,
      -1,    -1,    73,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    83,    84,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,  1285,  1286,
    1287,  1288,    -1,    -1,    63,    -1,    -1,  1294,  1295,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,   908,    -1,    -1,    -1,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,   491,   104,    -1,   987,   107,   989,
      -1,    -1,   111,   112,   994,   114,    -1,   997,    -1,   118,
      -1,    -1,    -1,    -1,   806,    -1,  1006,   809,  1008,    -1,
     517,  1011,    -1,    -1,    -1,    -1,   523,    -1,    -1,    -1,
      -1,    -1,   958,    -1,    -1,    -1,    -1,   534,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1040,   222,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1050,    -1,  1052,    -1,   173,    -1,    -1,    -1,    -1,    -1,
      -1,    22,    -1,    -1,   866,    26,    27,    -1,    -1,    -1,
     872,   806,    -1,    -1,   809,    36,    -1,    38,    39,    -1,
      -1,    -1,    -1,    44,   265,   266,   267,   268,   269,   270,
      51,    -1,   273,   274,    -1,    -1,    -1,    -1,    -1,   280,
      -1,    -1,   609,    -1,    -1,    66,   613,    -1,   289,    -1,
     291,    -1,    -1,    -1,   621,    -1,    77,   298,    79,    -1,
    1120,    -1,    83,    -1,    85,    86,    -1,  1127,    89,    -1,
      91,   866,    -1,    -1,    -1,    -1,    -1,   872,    -1,    -1,
    1140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,  1159,
      -1,    -1,   343,    -1,    -1,    -1,    -1,    -1,  1168,    -1,
      -1,    -1,  1172,    -1,    -1,    -1,    -1,   358,   359,    -1,
      -1,    -1,    -1,    -1,    -1,   987,    -1,   989,    -1,    -1,
      -1,    -1,   994,    -1,    -1,   997,    -1,    -1,    -1,    -1,
      -1,    30,    -1,    -1,  1006,   166,  1008,    -1,    -1,  1011,
      -1,    -1,    -1,  1149,  1150,    44,    45,    46,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1229,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1040,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1050,    -1,
    1052,    -1,   987,    -1,   989,    -1,    -1,    -1,    -1,   994,
      -1,    -1,   997,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1006,    -1,  1008,    -1,    -1,  1011,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1285,  1286,  1287,  1288,    -1,
      -1,    -1,    -1,    32,  1294,  1295,    -1,    -1,    -1,   806,
      -1,    -1,   809,    -1,    -1,  1040,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   494,    -1,  1050,    -1,  1052,  1120,    -1,
      -1,    -1,    -1,   152,    -1,  1127,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   165,   166,  1140,    -1,
      -1,   522,    -1,    -1,    -1,   526,    -1,   528,    -1,    -1,
     531,    90,    -1,    -1,    -1,    94,    -1,  1159,    -1,   866,
      -1,    -1,    -1,    -1,    -1,   872,  1168,    -1,    -1,    -1,
    1172,    -1,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1120,    -1,    -1,    -1,    -1,
      -1,    -1,  1127,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,  1140,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,    -1,   158,
     159,   160,   161,   162,  1159,   164,    -1,  1229,   167,   168,
     169,    -1,    -1,  1168,   263,    -1,    -1,  1172,    -1,   178,
      12,    22,   271,   272,    -1,    26,    27,    19,    -1,    -1,
      -1,    -1,    -1,   282,    -1,    36,    -1,    38,    39,    -1,
      32,    33,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
     987,    -1,   989,  1285,  1286,  1287,  1288,   994,    -1,    -1,
     997,    -1,  1294,  1295,  1229,   324,    77,    -1,    79,  1006,
      -1,  1008,    -1,    -1,  1011,    -1,    -1,    -1,    89,    -1,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,   113,  1040,    -1,    -1,   117,    -1,    -1,    -1,
      -1,    -1,    -1,  1050,    -1,  1052,    -1,    -1,    -1,    -1,
    1285,  1286,  1287,  1288,   126,    -1,    -1,    -1,    -1,  1294,
    1295,    -1,    -1,    -1,    -1,    -1,    -1,   396,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,   423,   167,   168,   169,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,   790,
     791,    -1,    -1,    -1,    -1,   796,   797,    -1,    -1,   800,
     801,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   810,
      -1,    -1,    -1,  1140,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1159,    -1,    17,    18,    -1,    20,   487,    -1,
      -1,  1168,    25,    -1,    -1,  1172,   495,    -1,    -1,    -1,
      -1,    34,   501,    -1,    -1,   856,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,   543,    78,    -1,    -1,   547,    -1,
      -1,    -1,  1229,    -1,    -1,    88,   555,    -1,    -1,    -1,
      -1,   560,    -1,    -1,    97,    98,    -1,   100,    -1,   102,
      -1,   104,   571,    -1,   107,    -1,    -1,   576,   111,   112,
      -1,   114,    22,    -1,    -1,   118,    26,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   977,   978,   627,    -1,
      -1,    -1,    -1,   984,    -1,   986,    -1,    77,    -1,    79,
     173,    81,   993,    83,    -1,   996,    86,    -1,   999,    89,
    1001,    91,  1003,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1016,    -1,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,   115,    -1,   117,    -1,     1,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,  1048,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,   706,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,   166,    -1,    50,    -1,
      -1,    53,   172,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
    1161,    -1,   811,    -1,    -1,    -1,    -1,    -1,    -1,   818,
     142,   143,   144,   145,   146,    -1,    -1,    -1,    -1,   151,
      -1,   830,    -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,   163,   164,   165,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    -1,   175,   176,    -1,   178,   856,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1223,  1224,    -1,    -1,  1227,  1228,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1241,    -1,  1243,    -1,  1245,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   905,   906,    -1,    -1,
      -1,    -1,    -1,     0,     1,    -1,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,   933,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    -1,    91,    92,    93,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,  1015,   104,   105,    -1,
     107,   108,   109,   110,   111,    -1,   113,   114,   115,   116,
     117,   118,    -1,    -1,   121,    -1,   123,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,  1045,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,   144,   145,   146,
      -1,    -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   160,    -1,    -1,   163,   164,   165,   166,
      -1,    -1,   169,    -1,   171,    -1,   173,    -1,   175,    -1,
       1,   178,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    15,    16,    17,    18,    19,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    68,    -1,    70,
      71,    72,    -1,    74,    -1,    -1,    77,    78,    79,    80,
      -1,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,    -1,   104,   105,    -1,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,   117,   118,    -1,    -1,
     121,    -1,   123,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
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
     175,    -1,     1,   178,     3,     4,     5,     6,     7,     8,
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
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    32,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,    98,
      99,   100,   101,   102,    -1,   104,    -1,    -1,   107,    -1,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      94,    -1,    -1,   142,   143,   144,   145,   146,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    -1,    -1,    -1,   173,    -1,   175,    -1,    -1,   178,
       4,     5,     6,     7,     8,     9,    10,    -1,   142,   143,
      14,   145,   146,   147,    18,   149,   150,    -1,    22,    -1,
     154,    -1,    -1,    27,    -1,    -1,   160,   161,    -1,    -1,
     164,    -1,    36,   167,   168,   169,    -1,    41,    42,    -1,
      -1,    45,    -1,    -1,    48,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    -1,
      -1,    65,    -1,    67,    68,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    79,    80,    81,    82,    -1,
      -1,    -1,    -1,    87,    -1,    89,    -1,    91,    -1,    -1,
      -1,    -1,    -1,    97,    -1,    99,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,   151,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,   163,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
     174,   175,    -1,    -1,   178,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    -1,    18,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    41,    42,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    -1,    -1,    65,    -1,    67,    68,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      79,    80,    81,    82,    -1,    -1,    -1,    -1,    87,    -1,
      89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    97,    -1,
      99,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,    -1,    -1,   142,   143,   144,   145,   146,    -1,    12,
      -1,    -1,   151,    -1,    17,    18,    19,    20,    -1,    -1,
      -1,   160,    25,    -1,   163,   164,   165,    -1,    -1,    -1,
     169,    34,   171,    36,   173,    -1,   175,    40,    -1,   178,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    97,    98,    -1,   100,    -1,   102,
      -1,   104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   127,   128,   129,   130,   131,   132,
      -1,    -1,   135,   136,   137,   138,    -1,   140,   141,   142,
     143,   144,   145,   146,   147,    -1,   149,    -1,    -1,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,    -1,    -1,   167,   168,   169,   170,    -1,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,
     144,   145,   146,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,
     164,   165,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      -1,   175,     3,     4,     5,     6,     7,     8,     9,    10,
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
      -1,    -1,    -1,    -1,    -1,    -1,   127,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   144,    -1,    -1,    -1,   148,    -1,    -1,
      -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
     171,    -1,   173,   174,   175,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    61,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    97,
      98,    99,   100,   101,   102,    -1,   104,    -1,   106,   107,
      -1,    -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,
     118,    -1,   120,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   144,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   165,   166,    25,
      -1,    -1,    -1,   171,    -1,   173,    -1,   175,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    98,    99,   100,   101,   102,    -1,   104,    -1,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,   144,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,   165,
      -1,    25,    -1,    -1,    -1,   171,    -1,   173,    -1,   175,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    97,    98,    99,   100,   101,   102,    -1,
     104,    -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     144,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,   165,    -1,    25,    -1,    -1,    -1,   171,    -1,   173,
      -1,   175,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    97,    98,    99,   100,   101,
     102,    -1,   104,    17,    18,   107,    20,    -1,    -1,   111,
      -1,    25,   114,    -1,    -1,    -1,   118,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    12,    -1,    -1,    -1,    -1,
      -1,   173,    19,   175,    88,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    97,    98,    32,   100,    -1,   102,    -1,
     104,    17,    18,   107,    20,    -1,    -1,   111,    -1,    25,
     114,    48,    -1,    -1,   118,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,     3,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    12,    53,    -1,    -1,
      -1,    -1,    -1,    19,    20,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    90,    70,    71,    -1,    94,    34,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,   171,    -1,   173,
      -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    55,
      -1,    97,    98,    -1,   100,   122,   102,    63,   104,   126,
      -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,   118,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   104,    -1,
     167,   168,   169,    -1,   110,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   127,   128,   129,   130,   131,   132,   173,    -1,   135,
     136,   137,   138,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,    -1,   149,    -1,    -1,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,     3,   164,    -1,
      -1,   167,   168,   169,   170,   171,    12,    -1,    -1,    -1,
      -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   122,    -1,    32,    -1,   126,
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
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    90,   167,   168,   169,    94,   171,
      -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,   171,    -1,    12,    -1,    90,
      -1,    -1,   178,    94,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    32,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    90,   167,   168,   169,    94,
      -1,    -1,    12,    -1,    -1,    -1,    -1,   178,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    -1,   167,   168,   169,    -1,    -1,    -1,    12,    -1,
      90,    -1,    -1,   178,    94,    19,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,    32,   154,
     155,   156,    -1,   158,   159,   160,   161,    -1,    -1,   164,
      -1,    -1,   167,   168,   169,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    12,   178,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    32,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    90,   167,   168,   169,
      94,   171,    -1,    -1,    -1,    -1,    -1,    -1,   178,   103,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   126,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   122,   167,   168,   169,   126,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    90,   145,   146,   147,    94,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    12,    -1,   167,   168,   169,
      -1,    -1,    19,    -1,    -1,    -1,   176,    -1,   178,    -1,
     126,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      -1,   167,   168,   169,    -1,    -1,    73,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    90,    -1,    -1,    -1,    94,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    32,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    90,
     167,   168,   169,    94,    -1,    -1,    12,    -1,    -1,    -1,
      -1,   178,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    90,
      -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   126,   167,   168,   169,    -1,
     171,    -1,    -1,    -1,    90,    -1,    -1,   178,    94,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    12,   166,   167,   168,   169,   125,
     126,    19,    -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    12,
      -1,   167,   168,   169,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,   103,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    -1,    -1,   167,
     168,   169,    -1,   126,    -1,    12,    -1,    90,    -1,    -1,
     178,    94,    19,    -1,    -1,    -1,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    32,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,   126,   167,   168,   169,    -1,    -1,    12,
      -1,   174,    -1,    -1,    -1,   178,    19,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    32,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    90,   167,   168,   169,    94,    -1,    -1,
      -1,    -1,    -1,    -1,    12,   178,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,   122,    -1,    35,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
      -1,    94,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    -1,
     167,   168,   169,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   178,    90,    -1,    -1,    -1,    94,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    12,   166,   167,   168,   169,    -1,   126,    19,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,   142,   143,    -1,   145,   146,   147,
      -1,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,    12,    -1,   167,
     168,   169,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     178,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,   126,    -1,    12,   174,    90,    -1,    -1,   178,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    32,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     125,   126,   167,   168,   169,    -1,    -1,    12,    13,   174,
      -1,    -1,    -1,   178,    19,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    32,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    90,   167,   168,   169,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    12,   178,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,    -1,   167,   168,
     169,   126,    -1,    -1,    -1,   174,    -1,    -1,    -1,   178,
      90,    -1,    -1,    -1,    94,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    -1,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      12,    -1,   167,   168,   169,    -1,   126,    19,    -1,    -1,
      -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    12,    -1,   167,   168,   169,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,    -1,
     142,   143,    -1,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,    -1,    -1,   167,   168,   169,    -1,   126,
      -1,    12,    13,    90,    -1,    -1,   178,    94,    19,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    32,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,   126,
     167,   168,   169,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,   178,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    90,
     167,   168,   169,    94,    -1,    -1,    12,    -1,    -1,   176,
      -1,   178,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    12,    -1,    90,    -1,    -1,   178,    94,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    32,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      90,   167,   168,   169,    94,    -1,    -1,    -1,    -1,    12,
      -1,    -1,   178,   103,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    61,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,   126,   167,   168,   169,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,   178,    -1,
      -1,    94,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    12,    -1,   167,   168,   169,
      -1,    -1,    19,   126,    -1,    -1,    -1,    -1,   178,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   142,
     143,    -1,   145,   146,   147,    -1,   149,   150,   151,    -1,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    90,    -1,    -1,    -1,    94,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,   146,
     147,    -1,   149,   150,   151,    -1,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,    -1,    90,
     167,   168,   169,    94,    -1,    -1,    12,    -1,    -1,   176,
      -1,   178,   103,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,   143,    -1,   145,   146,   147,    -1,   149,   150,
     151,    -1,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,    -1,   167,   168,   169,    -1,
      -1,    -1,    12,    -1,    90,    -1,    -1,   178,    94,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     126,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      90,   167,   168,   169,    94,    -1,    -1,    12,    -1,    -1,
      -1,    -1,   178,   103,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,   149,
     150,   151,    -1,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,    -1,    -1,   167,   168,   169,
      -1,    -1,    -1,    12,    -1,    90,    -1,    -1,   178,    94,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   126,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   142,   143,    -1,
     145,   146,   147,    32,   149,   150,   151,    -1,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,    90,   167,   168,   169,    94,    -1,    -1,    12,    -1,
      -1,    -1,    -1,   178,   103,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    90,    -1,    -1,    -1,    94,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,   143,    -1,   145,   146,   147,    -1,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   126,   167,   168,
     169,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,   178,
      94,    -1,    -1,   142,   143,    -1,   145,   146,   147,   103,
     149,   150,   151,    -1,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    12,    -1,   167,   168,
     169,    -1,   126,    19,    -1,    -1,    -1,    -1,    -1,   178,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   142,   143,
      -1,   145,   146,   147,    -1,   149,   150,   151,    -1,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,    -1,   167,   168,   169,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   178,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,
     126,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,   142,   143,    51,   145,
     146,   147,    -1,   149,   150,   151,    -1,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
      32,   167,   168,   169,    77,    -1,    79,    -1,    81,    -1,
      83,    -1,   178,    86,    -1,    -1,    89,    22,    91,    -1,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,   110,    -1,    44,
     113,    -1,   115,    -1,   117,    -1,    51,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,
      -1,    -1,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    -1,    -1,    83,    -1,
      -1,    86,    -1,    -1,    89,    -1,    91,    -1,    -1,    -1,
      -1,    -1,    -1,   166,   126,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
     142,   143,   117,   145,   146,   147,    -1,   149,   150,   151,
      -1,   153,   154,   155,   156,    -1,   158,   159,   160,   161,
     162,   163,   164,    -1,    22,   167,   168,   169,    26,    27,
      -1,    -1,    -1,    31,    -1,    -1,   178,    -1,    36,    -1,
      38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,
      -1,   166,    -1,    51,    -1,    -1,     1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    77,
      25,    79,    -1,    81,    -1,    83,    31,    -1,    86,    34,
      -1,    89,    -1,    91,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,   115,    63,   117,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    97,    98,    -1,   100,    25,   102,    27,   104,
      -1,    -1,   107,    -1,    -1,    34,   111,    -1,   166,   114,
     115,    40,    -1,   118,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,     3,    -1,    -1,    -1,    -1,    -1,    78,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    88,
      20,    -1,    91,    -1,    -1,    25,    -1,    -1,    97,    98,
      -1,   100,    -1,   102,    34,   104,    -1,    -1,   107,    -1,
      40,    -1,   111,    -1,    -1,   114,    46,    -1,   117,   118,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    97,    98,    -1,
     100,    -1,   102,    -1,   104,    17,    18,   107,    20,    -1,
      -1,   111,   112,    25,   114,    -1,    -1,    -1,   118,    31,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       3,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    17,    18,    78,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    97,    98,    40,   100,    -1,
     102,    -1,   104,    46,    -1,   107,    -1,    50,    -1,   111,
      53,    -1,   114,    -1,    -1,    -1,   118,    -1,    -1,     3,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    17,    18,    78,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    97,    98,    40,   100,    -1,   102,
      -1,   104,    46,    -1,   107,    -1,    50,    -1,   111,    53,
      -1,   114,    -1,    -1,    -1,   118,    -1,    -1,     3,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    17,    18,    78,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    97,    98,    40,   100,    -1,   102,    -1,
     104,    46,    -1,   107,    -1,    50,    -1,   111,    53,    -1,
     114,    -1,    -1,    -1,   118,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    98,    -1,   100,    -1,   102,    -1,   104,
      -1,    -1,   107,    -1,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,   118
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
     292,   293,   295,   296,   323,   324,   328,   329,   354,   355,
     356,   357,   358,   359,   360,   368,   369,   370,   371,   372,
     373,   375,   378,   379,   380,   381,   382,   383,   384,   385,
     387,   388,   389,   390,   391,   166,    22,    36,    41,    42,
      45,    56,    89,   102,   171,   175,   247,   280,   354,   359,
     369,   370,   375,   378,   380,   381,   122,   361,   362,     3,
     213,   375,   361,   375,   110,   330,    91,   213,   187,   348,
     375,     8,   191,   187,   173,     3,    17,    18,    20,    25,
      34,    40,    46,    50,    53,    63,    70,    71,    78,    88,
      97,    98,   100,   102,   104,   107,   111,   114,   118,   214,
     215,   217,    11,    79,   124,   241,   375,   241,   375,   241,
     375,    27,   117,   243,   375,    83,    86,   198,   173,   215,
     215,   215,   173,   215,   331,   332,    33,   202,   220,   375,
     259,   260,   375,   375,    18,    78,    97,   114,   375,   375,
     375,     8,   173,   230,   229,     4,    27,    34,    48,    50,
      52,    76,    79,    91,    98,   102,   110,   175,   186,   219,
     284,   285,   286,   309,   310,   311,   318,   343,   344,   355,
     318,   347,   375,   108,   109,   166,   288,   289,   374,   375,
     377,   375,   220,   375,   375,   375,   101,   173,   187,   375,
     375,   188,   202,   188,   202,   375,     8,   195,   243,   375,
     377,   375,   375,   375,   375,   375,     1,   172,   185,   203,
     347,   112,   152,   318,   349,   350,   377,   241,   347,   375,
     386,   375,    81,   187,   171,    47,    85,   116,   199,    26,
     329,   375,     8,   263,   375,   376,    56,   147,   265,   215,
       1,    31,   115,   215,   269,   272,   275,   278,   173,   304,
      54,    74,    84,   313,    27,    79,    91,   110,   314,    27,
      79,    91,   110,   312,   215,   325,   326,   331,   165,   166,
     375,    12,    19,    32,    90,    94,   126,   142,   143,   145,
     146,   147,   149,   150,   151,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   167,   168,   169,
     178,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   170,   300,   173,   175,
      90,    94,   375,   375,   375,   215,   347,    56,   173,   187,
     166,   171,   202,    48,   361,   328,   166,   148,   166,   190,
      22,    36,    39,    89,   192,   198,   121,   216,   318,   351,
     352,   353,   377,   171,   215,   173,   220,    33,    48,   220,
     122,   220,   364,    33,    48,   220,   364,   220,   364,    48,
     220,   364,   215,   215,   103,   202,   103,   127,   202,   300,
     199,   351,   173,   173,   202,   187,   112,   173,   215,   333,
     334,     1,   147,   340,    48,   148,   187,   220,   148,   220,
      13,   173,   173,   220,   351,   231,   231,    48,    91,   176,
     347,   310,   311,   174,   148,   112,   173,   215,   308,   147,
     148,   166,   375,   375,   125,   290,   166,   171,   220,   173,
     351,   166,   252,   252,   194,   192,   220,   220,   166,   172,
     172,   185,   148,   172,   375,   148,   174,   148,   174,   176,
     364,    48,   148,   176,   364,   125,   148,   176,     8,     1,
     172,   203,   209,   210,   375,   205,   375,    66,    37,    73,
     166,   263,   265,   110,   237,   293,   215,   347,   171,   172,
       8,   274,     8,   277,   127,   148,   172,   174,   305,   307,
     309,   314,    91,     1,   147,   338,   339,    91,     1,     3,
      12,    17,    19,    20,    25,    46,    53,    55,    63,    71,
      88,   100,   104,   111,   118,   142,   143,   144,   145,   146,
     147,   149,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   164,   167,   168,   169,   170,   173,   215,   297,
     298,   299,   300,   354,   127,   327,   148,   166,   166,   375,
     375,   375,   241,   375,   241,   375,   375,   375,   375,   375,
     375,   375,     3,    20,    34,    63,   104,   110,   216,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,    69,   377,   377,   377,
     377,   377,   351,   351,   241,   375,   241,   375,    33,    48,
      33,    48,   103,   173,    48,   176,   215,    27,    48,    91,
     117,   363,   366,   367,   375,   391,   241,   375,   220,   331,
     375,   192,   375,   127,   174,   148,    48,   347,    45,   375,
     241,   375,   173,   220,    45,   375,   241,   375,   220,   220,
     241,   375,   220,   127,   127,   187,    35,   187,   375,    35,
     375,    66,   174,   352,   215,   245,   246,   334,   148,   174,
     175,   219,   341,   355,   127,   335,   375,   332,   259,   215,
     328,   375,   375,   174,   318,   342,   343,   355,    48,   176,
     308,   314,   286,   334,   147,   345,   346,   344,   318,   375,
     291,   201,     1,   257,   353,   174,    21,   253,   192,   172,
     174,   174,   349,   174,   349,   187,   176,   241,   375,   176,
     187,   375,   176,   375,   176,   375,   172,   172,   148,   166,
      13,   150,   148,   166,    13,    37,    73,   215,   142,   143,
     144,   145,   146,   160,   164,   169,   204,   299,   300,   301,
     375,   204,   206,   265,   171,   238,   328,   166,   173,     1,
     266,   273,   278,   276,   278,   375,   270,   271,   148,   174,
     340,     1,   175,   219,   336,   337,   355,   106,   315,   173,
     303,   375,   142,   150,   303,   303,   341,   355,   326,   173,
     175,   166,   166,   166,   166,   166,   166,   174,   176,    45,
     241,   375,    45,   241,   375,   352,   241,   375,   173,    48,
      91,   148,   174,    17,    20,    25,    46,    53,    63,    71,
      88,   100,   111,   118,   354,    90,    90,   361,   361,   166,
     166,   318,   377,   353,   375,   174,   375,    33,   220,    33,
     220,   365,   366,   375,    33,   220,   364,    33,   220,   364,
     220,   364,   220,   364,   375,   375,    35,   187,    35,    35,
     187,   103,   202,   215,   174,   148,   174,   174,   333,   340,
       1,   347,    69,   377,   215,   174,   174,   375,   342,   343,
     346,   147,   287,   174,   344,   152,   302,   335,   375,   172,
      75,   119,   172,   258,   174,   173,   202,   215,   254,   187,
     176,   364,   176,   364,   187,   125,   209,   216,   171,   301,
     375,   112,   375,   204,   206,   148,   166,    13,   166,   171,
     266,   325,   331,   352,   172,    31,    83,    86,   115,   172,
     186,   222,   225,   228,   278,   278,   272,   275,   307,   315,
       1,   176,   347,    61,   120,   319,   305,   306,   174,   298,
     300,   174,   176,   375,    33,    33,   375,    33,    33,   174,
     176,   176,   352,   366,   340,   354,   354,   220,   220,   220,
     103,    45,   375,    45,   375,   148,   174,   103,    45,   375,
     220,    45,   375,   220,   220,   220,   187,   187,   375,   187,
      35,   166,   166,   246,   202,   335,   176,    48,   176,   173,
     173,   176,   302,   335,   375,   315,   346,   318,   375,    33,
     187,   347,   254,   147,   202,    45,   187,   375,   176,    45,
     187,   375,   176,   375,   204,    13,    37,    73,    37,    73,
     166,   166,   301,   375,   375,   266,   172,   166,   166,   174,
       8,   224,     8,   227,   222,   225,   319,   176,   336,   355,
     176,     3,    93,   104,   320,   321,   322,   375,   294,   174,
     303,   303,   103,    45,    45,   103,    45,    45,    45,    45,
     174,   335,   375,   375,   375,   366,   375,   375,   375,    35,
     187,   375,   341,   355,   351,   351,   342,   302,   335,   187,
     220,   174,   375,   375,   187,   375,   187,   172,   112,   375,
     204,   206,   204,   206,    13,   172,   166,   223,   228,   226,
     228,   281,   336,   355,   322,   120,   148,   127,   153,   155,
     156,   158,   159,    61,   166,   201,   221,   316,   317,   375,
     375,   375,   375,   375,   375,   103,   103,   103,   103,   187,
     176,   174,   174,   202,   103,   103,   166,   166,   166,   166,
     375,   228,   228,   317,   375,   321,   322,   322,   322,   322,
     322,   322,   320,   103,   103,   103,   103,   103,   103,   375,
     375,   375,   375,   355,   165,   165,   375,   375
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
     308,   309,   309,   309,   309,   309,   309,   310,   310,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   312,   312,
     312,   312,   312,   312,   313,   313,   313,   314,   314,   314,
     314,   314,   314,   315,   315,   316,   316,   317,   317,   318,
     319,   319,   319,   319,   319,   320,   320,   321,   321,   321,
     321,   321,   321,   321,   322,   322,   323,   324,   324,   325,
     325,   326,   327,   327,   327,   328,   328,   328,   328,   328,
     330,   329,   329,   331,   331,   332,   332,   333,   333,   333,
     334,   334,   334,   335,   335,   335,   336,   336,   336,   336,
     336,   336,   336,   337,   337,   337,   337,   338,   338,   339,
     339,   340,   340,   340,   340,   340,   341,   341,   341,   341,
     342,   342,   342,   343,   343,   343,   343,   343,   344,   344,
     344,   344,   345,   346,   346,   347,   347,   347,   347,   348,
     348,   349,   349,   349,   350,   350,   351,   351,   352,   352,
     353,   353,   353,   353,   354,   354,   355,   355,   355,   355,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   358,
     359,   360,   360,   360,   360,   360,   360,   360,   361,   361,
     362,   363,   363,   364,   365,   365,   366,   366,   366,   367,
     367,   367,   367,   367,   367,   368,   368,   368,   368,   368,
     369,   369,   369,   369,   369,   370,   371,   371,   371,   371,
     371,   371,   372,   373,   374,   374,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   376,   376,
     377,   377,   377,   378,   378,   378,   378,   379,   379,   379,
     379,   379,   380,   380,   380,   381,   381,   381,   381,   381,
     381,   382,   382,   382,   382,   383,   383,   384,   384,   385,
     385,   385,   385,   385,   385,   385,   385,   385,   385,   385,
     385,   385,   386,   386,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   387,   387,   387,
     387,   387,   387,   387,   387,   387,   387,   388,   388,   388,
     388,   388,   388,   388,   389,   389,   389,   389,   390,   390,
     390,   390,   391,   391,   391,   391,   391,   391,   391
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
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       0,     3,     2,     3,     1,     3,     0,     1,     1,     1,
       1,     4,     5,     4,     5,     6,     6,     0,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     1,     0,     1,     2,
       1,     1,     1,     0,     1,     1,     1,     1,     1,     1,
       0,     2,     2,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     3,     1,     2,     1,
       3,     2,     0,     2,     2,     1,     2,     1,     1,     1,
       0,     5,     3,     1,     3,     3,     5,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     3,     2,     4,     3,
       3,     4,     3,     1,     1,     1,     1,     2,     1,     0,
       1,     0,     2,     2,     2,     1,     4,     4,     6,     3,
       0,     1,     1,     3,     4,     3,     4,     6,     1,     1,
       1,     1,     2,     0,     1,     1,     1,     3,     3,     1,
       3,     1,     1,     1,     3,     3,     0,     1,     1,     3,
       3,     3,     1,     1,     1,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     2,     4,     4,     4,     5,
       2,     2,     1,     2,     1,     2,     1,     2,     1,     2,
       1,     1,     6,     6,     4,     9,     9,     7,     6,     6,
       4,     9,     9,     7,     4,     6,     6,     9,     9,     6,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
       4,     1,     3,     4,     1,     3,     4,     3,     3,     1,
       1,     2,     1,     2,     1,     1,     2,     2,     2,     2,
       2,     8,     8,     9,     9,     4,     3,     3,     2,     2,
       2,     1,     3,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     1,
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
#line 631 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6533 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 635 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6539 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 636 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6545 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 643 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6553 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 651 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6561 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 655 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6570 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 663 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6576 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: unstable_decl_stmt  */
#line 664 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6582 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: include_module_stmt  */
#line 665 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6588 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: block_stmt  */
#line 666 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6594 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: use_stmt  */
#line 667 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6600 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: import_stmt  */
#line 668 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6606 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: require_stmt  */
#line 669 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6612 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: extern_block_stmt  */
#line 670 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6618 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 671 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6624 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 672 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6630 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: TDEFER stmt  */
#line 674 "chpl.ypp"
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
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: try_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6653 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: return_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6659 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 689 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6672 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 698 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6685 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TLABEL ident_def stmt  */
#line 707 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6693 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: TYIELD opt_try_expr TSEMI  */
#line 711 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6704 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: error TSEMI  */
#line 718 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6713 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: assignment_stmt  */
#line 725 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6719 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: if_stmt  */
#line 726 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6725 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: loop_stmt  */
#line 727 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6731 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: select_stmt  */
#line 728 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6737 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: manage_stmt  */
#line 729 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6743 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 730 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6749 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: throw_stmt  */
#line 731 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6755 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 733 "chpl.ypp"
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
#line 6773 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 747 "chpl.ypp"
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
#line 6792 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 762 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6804 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 770 "chpl.ypp"
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
#line 6820 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL do_stmt  */
#line 782 "chpl.ypp"
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
#line 6835 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TON expr do_stmt  */
#line 793 "chpl.ypp"
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
#line 6851 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 805 "chpl.ypp"
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
#line 6867 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL do_stmt  */
#line 817 "chpl.ypp"
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
#line 6882 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSYNC stmt  */
#line 828 "chpl.ypp"
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
#line 6899 "bison-chpl-lib.cpp"
    break;

  case 45: /* $@1: %empty  */
#line 845 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6907 "bison-chpl-lib.cpp"
    break;

  case 46: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 decl_base  */
#line 849 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6915 "bison-chpl-lib.cpp"
    break;

  case 47: /* $@2: %empty  */
#line 853 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6923 "bison-chpl-lib.cpp"
    break;

  case 48: /* deprecated_decl_stmt: TDEPRECATED $@2 decl_base  */
#line 857 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6931 "bison-chpl-lib.cpp"
    break;

  case 51: /* $@3: %empty  */
#line 869 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 6939 "bison-chpl-lib.cpp"
    break;

  case 52: /* unstable_decl_stmt: TUNSTABLE STRINGLITERAL $@3 decl_base  */
#line 873 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6947 "bison-chpl-lib.cpp"
    break;

  case 53: /* $@4: %empty  */
#line 877 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 6955 "bison-chpl-lib.cpp"
    break;

  case 54: /* unstable_decl_stmt: TUNSTABLE $@4 decl_base  */
#line 881 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6963 "bison-chpl-lib.cpp"
    break;

  case 55: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 888 "chpl.ypp"
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
#line 6984 "bison-chpl-lib.cpp"
    break;

  case 56: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 908 "chpl.ypp"
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
#line 7003 "bison-chpl-lib.cpp"
    break;

  case 57: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 923 "chpl.ypp"
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
#line 7022 "bison-chpl-lib.cpp"
    break;

  case 58: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 938 "chpl.ypp"
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
#line 7039 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_access_control: %empty  */
#line 953 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 7045 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_access_control: TPUBLIC  */
#line 954 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 7052 "bison-chpl-lib.cpp"
    break;

  case 61: /* opt_access_control: TPRIVATE  */
#line 956 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 62: /* opt_prototype: %empty  */
#line 961 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7065 "bison-chpl-lib.cpp"
    break;

  case 63: /* opt_prototype: TPROTOTYPE  */
#line 962 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7072 "bison-chpl-lib.cpp"
    break;

  case 64: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 968 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7082 "bison-chpl-lib.cpp"
    break;

  case 65: /* block_stmt_body: TLCBR TRCBR  */
#line 986 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7090 "bison-chpl-lib.cpp"
    break;

  case 66: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 990 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7098 "bison-chpl-lib.cpp"
    break;

  case 67: /* block_stmt_body: TLCBR error TRCBR  */
#line 994 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7110 "bison-chpl-lib.cpp"
    break;

  case 68: /* block_stmt: block_stmt_body  */
#line 1005 "chpl.ypp"
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
#line 7129 "bison-chpl-lib.cpp"
    break;

  case 69: /* stmt_ls: toplevel_stmt  */
#line 1023 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7135 "bison-chpl-lib.cpp"
    break;

  case 70: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1024 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7141 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: expr  */
#line 1029 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7149 "bison-chpl-lib.cpp"
    break;

  case 72: /* renames_ls: all_op_name  */
#line 1033 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7157 "bison-chpl-lib.cpp"
    break;

  case 73: /* renames_ls: expr TAS expr  */
#line 1037 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7166 "bison-chpl-lib.cpp"
    break;

  case 74: /* renames_ls: renames_ls TCOMMA expr  */
#line 1042 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7174 "bison-chpl-lib.cpp"
    break;

  case 75: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1046 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7182 "bison-chpl-lib.cpp"
    break;

  case 76: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1050 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: expr  */
#line 1060 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7200 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_renames_ls: expr TAS expr  */
#line 1065 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7210 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1071 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7221 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1078 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7230 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1083 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7241 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1090 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7253 "bison-chpl-lib.cpp"
    break;

  case 83: /* opt_only_ls: %empty  */
#line 1100 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7259 "bison-chpl-lib.cpp"
    break;

  case 84: /* opt_only_ls: renames_ls  */
#line 1101 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7265 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1106 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7274 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1111 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7286 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1119 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7299 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1128 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7313 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1138 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7325 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1146 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7338 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1155 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7352 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1168 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7361 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_expr: expr  */
#line 1176 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7369 "bison-chpl-lib.cpp"
    break;

  case 94: /* import_expr: expr TDOT all_op_name  */
#line 1180 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7378 "bison-chpl-lib.cpp"
    break;

  case 95: /* import_expr: expr TAS ident_use  */
#line 1185 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7388 "bison-chpl-lib.cpp"
    break;

  case 96: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1191 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7398 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_ls: import_expr  */
#line 1199 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7404 "bison-chpl-lib.cpp"
    break;

  case 98: /* import_ls: import_ls TCOMMA import_expr  */
#line 1200 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7410 "bison-chpl-lib.cpp"
    break;

  case 99: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1205 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1214 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7428 "bison-chpl-lib.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1218 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7436 "bison-chpl-lib.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1222 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7444 "bison-chpl-lib.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1226 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7452 "bison-chpl-lib.cpp"
    break;

  case 104: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1230 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7460 "bison-chpl-lib.cpp"
    break;

  case 105: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1234 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 106: /* opt_label_ident: %empty  */
#line 1242 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7474 "bison-chpl-lib.cpp"
    break;

  case 107: /* opt_label_ident: TIDENT  */
#line 1243 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7480 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_use: TIDENT  */
#line 1268 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7486 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_use: TTHIS  */
#line 1269 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7492 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TBOOL  */
#line 1296 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7498 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TENUM  */
#line 1297 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7504 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TINT  */
#line 1298 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TUINT  */
#line 1299 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7516 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TREAL  */
#line 1300 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7522 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TIMAG  */
#line 1301 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7528 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TCOMPLEX  */
#line 1302 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TBYTES  */
#line 1303 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7540 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TSTRING  */
#line 1304 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TLOCALE  */
#line 1305 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7552 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TNOTHING  */
#line 1306 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TVOID  */
#line 1307 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7564 "bison-chpl-lib.cpp"
    break;

  case 152: /* do_stmt: TDO stmt  */
#line 1321 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 153: /* do_stmt: block_stmt  */
#line 1322 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 154: /* return_stmt: TRETURN TSEMI  */
#line 1327 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7587 "bison-chpl-lib.cpp"
    break;

  case 155: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1334 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7598 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@5: %empty  */
#line 1345 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7606 "bison-chpl-lib.cpp"
    break;

  case 158: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@5 class_level_stmt  */
#line 1349 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7614 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@6: %empty  */
#line 1353 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7622 "bison-chpl-lib.cpp"
    break;

  case 160: /* deprecated_class_level_stmt: TDEPRECATED $@6 class_level_stmt  */
#line 1357 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7630 "bison-chpl-lib.cpp"
    break;

  case 161: /* $@7: %empty  */
#line 1364 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 7638 "bison-chpl-lib.cpp"
    break;

  case 162: /* unstable_class_level_stmt: TUNSTABLE STRINGLITERAL $@7 class_level_stmt  */
#line 1368 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7646 "bison-chpl-lib.cpp"
    break;

  case 163: /* $@8: %empty  */
#line 1372 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 7654 "bison-chpl-lib.cpp"
    break;

  case 164: /* unstable_class_level_stmt: TUNSTABLE $@8 class_level_stmt  */
#line 1376 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7662 "bison-chpl-lib.cpp"
    break;

  case 165: /* class_level_stmt: TSEMI  */
#line 1383 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7670 "bison-chpl-lib.cpp"
    break;

  case 166: /* class_level_stmt: inner_class_level_stmt  */
#line 1387 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7680 "bison-chpl-lib.cpp"
    break;

  case 167: /* $@9: %empty  */
#line 1392 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7687 "bison-chpl-lib.cpp"
    break;

  case 168: /* class_level_stmt: TPUBLIC $@9 inner_class_level_stmt  */
#line 1394 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7696 "bison-chpl-lib.cpp"
    break;

  case 169: /* $@10: %empty  */
#line 1398 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7703 "bison-chpl-lib.cpp"
    break;

  case 170: /* class_level_stmt: TPRIVATE $@10 inner_class_level_stmt  */
#line 1400 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7712 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1418 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7721 "bison-chpl-lib.cpp"
    break;

  case 179: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1423 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7730 "bison-chpl-lib.cpp"
    break;

  case 180: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1428 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7739 "bison-chpl-lib.cpp"
    break;

  case 181: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1433 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7747 "bison-chpl-lib.cpp"
    break;

  case 182: /* forwarding_decl_start: TFORWARDING  */
#line 1440 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_or_export: TEXTERN  */
#line 1447 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7762 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_or_export: TEXPORT  */
#line 1448 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7768 "bison-chpl-lib.cpp"
    break;

  case 185: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1453 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1463 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7787 "bison-chpl-lib.cpp"
    break;

  case 187: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1469 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7798 "bison-chpl-lib.cpp"
    break;

  case 188: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1476 "chpl.ypp"
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
#line 7820 "bison-chpl-lib.cpp"
    break;

  case 189: /* $@11: %empty  */
#line 1494 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7828 "bison-chpl-lib.cpp"
    break;

  case 190: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1498 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7837 "bison-chpl-lib.cpp"
    break;

  case 191: /* $@12: %empty  */
#line 1503 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7846 "bison-chpl-lib.cpp"
    break;

  case 192: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@12 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1508 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7855 "bison-chpl-lib.cpp"
    break;

  case 193: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1516 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7863 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1523 "chpl.ypp"
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
#line 7885 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TWHILE expr do_stmt  */
#line 1541 "chpl.ypp"
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
#line 7902 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1554 "chpl.ypp"
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
#line 7919 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1567 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7927 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1571 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7935 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1575 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7943 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1579 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7951 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1583 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7959 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOR expr do_stmt  */
#line 1587 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7967 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1591 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7975 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1595 "chpl.ypp"
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
#line 7997 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1613 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8005 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1617 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8013 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1621 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8021 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1625 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8029 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFORALL expr do_stmt  */
#line 1629 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1633 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8045 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1637 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8053 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1641 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8061 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1645 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8069 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1649 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8077 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1653 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8085 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1657 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8093 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1661 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8101 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1665 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8109 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1669 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8117 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1673 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8125 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1677 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8133 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1681 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8141 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1685 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8149 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1689 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8157 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1693 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8165 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1697 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8173 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1701 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8182 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1706 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8191 "bison-chpl-lib.cpp"
    break;

  case 229: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1714 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8201 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF expr TTHEN stmt  */
#line 1723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8209 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF expr block_stmt  */
#line 1727 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8217 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1731 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8225 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1735 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8233 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1739 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8241 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF ifvar block_stmt  */
#line 1743 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8249 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1747 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8257 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1751 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8265 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1755 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8274 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1760 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8283 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1765 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8292 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1770 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8301 "bison-chpl-lib.cpp"
    break;

  case 242: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1778 "chpl.ypp"
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
#line 8320 "bison-chpl-lib.cpp"
    break;

  case 243: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1793 "chpl.ypp"
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
#line 8339 "bison-chpl-lib.cpp"
    break;

  case 244: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1811 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8347 "bison-chpl-lib.cpp"
    break;

  case 245: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1815 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8355 "bison-chpl-lib.cpp"
    break;

  case 246: /* ifc_formal_ls: ifc_formal  */
#line 1821 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8361 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1822 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8367 "bison-chpl-lib.cpp"
    break;

  case 248: /* ifc_formal: ident_def  */
#line 1827 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8373 "bison-chpl-lib.cpp"
    break;

  case 261: /* implements_type_ident: implements_type_error_ident  */
#line 1845 "chpl.ypp"
  {
    CHPL_PARSER_REPORT_SYNTAX(
        context, (yyloc), "type '" + (yyvsp[0].uniqueStr).str() + "' cannot implement an interface.");
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8383 "bison-chpl-lib.cpp"
    break;

  case 268: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1872 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8391 "bison-chpl-lib.cpp"
    break;

  case 269: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1876 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8399 "bison-chpl-lib.cpp"
    break;

  case 270: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1880 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8407 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1887 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8415 "bison-chpl-lib.cpp"
    break;

  case 272: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1891 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8423 "bison-chpl-lib.cpp"
    break;

  case 273: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1895 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8431 "bison-chpl-lib.cpp"
    break;

  case 274: /* try_stmt: TTRY tryable_stmt  */
#line 1902 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8439 "bison-chpl-lib.cpp"
    break;

  case 275: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1906 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8447 "bison-chpl-lib.cpp"
    break;

  case 276: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1910 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8455 "bison-chpl-lib.cpp"
    break;

  case 277: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1914 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8463 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_ls: %empty  */
#line 1920 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8469 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1921 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8475 "bison-chpl-lib.cpp"
    break;

  case 280: /* catch_expr: TCATCH block_stmt  */
#line 1926 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8483 "bison-chpl-lib.cpp"
    break;

  case 281: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1930 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8491 "bison-chpl-lib.cpp"
    break;

  case 282: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1934 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8499 "bison-chpl-lib.cpp"
    break;

  case 283: /* catch_expr_inner: ident_def  */
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
#line 8516 "bison-chpl-lib.cpp"
    break;

  case 284: /* catch_expr_inner: ident_def TCOLON expr  */
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
#line 8533 "bison-chpl-lib.cpp"
    break;

  case 285: /* throw_stmt: TTHROW expr TSEMI  */
#line 1970 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8543 "bison-chpl-lib.cpp"
    break;

  case 286: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1978 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8551 "bison-chpl-lib.cpp"
    break;

  case 287: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1982 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8561 "bison-chpl-lib.cpp"
    break;

  case 288: /* when_stmt_ls: %empty  */
#line 1990 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8567 "bison-chpl-lib.cpp"
    break;

  case 289: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1991 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8573 "bison-chpl-lib.cpp"
    break;

  case 290: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1996 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8581 "bison-chpl-lib.cpp"
    break;

  case 291: /* when_stmt: TOTHERWISE stmt  */
#line 2000 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 292: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2005 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8599 "bison-chpl-lib.cpp"
    break;

  case 293: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2013 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 294: /* manager_expr: expr TAS ident_def  */
#line 2018 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8616 "bison-chpl-lib.cpp"
    break;

  case 295: /* manager_expr: expr  */
#line 2022 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8624 "bison-chpl-lib.cpp"
    break;

  case 296: /* manager_expr_ls: manager_expr  */
#line 2028 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8630 "bison-chpl-lib.cpp"
    break;

  case 297: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2029 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8636 "bison-chpl-lib.cpp"
    break;

  case 298: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2034 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8644 "bison-chpl-lib.cpp"
    break;

  case 299: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2043 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8653 "bison-chpl-lib.cpp"
    break;

  case 300: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2048 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8664 "bison-chpl-lib.cpp"
    break;

  case 301: /* class_start: class_tag ident_def  */
#line 2059 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8672 "bison-chpl-lib.cpp"
    break;

  case 302: /* class_tag: TCLASS  */
#line 2065 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8678 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_tag: TRECORD  */
#line 2066 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8684 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_tag: TUNION  */
#line 2067 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8690 "bison-chpl-lib.cpp"
    break;

  case 305: /* opt_inherit: %empty  */
#line 2071 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8696 "bison-chpl-lib.cpp"
    break;

  case 306: /* opt_inherit: TCOLON expr_ls  */
#line 2072 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8702 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_level_stmt_ls: %empty  */
#line 2076 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8711 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2081 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8719 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_level_stmt_ls: class_level_stmt_ls unstable_class_level_stmt  */
#line 2085 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8727 "bison-chpl-lib.cpp"
    break;

  case 310: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2089 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8735 "bison-chpl-lib.cpp"
    break;

  case 311: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls unstable_class_level_stmt  */
#line 2093 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8743 "bison-chpl-lib.cpp"
    break;

  case 312: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
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
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 313: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
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
#line 8778 "bison-chpl-lib.cpp"
    break;

  case 314: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2130 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8786 "bison-chpl-lib.cpp"
    break;

  case 315: /* enum_ls: deprecated_enum_item  */
#line 2137 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8795 "bison-chpl-lib.cpp"
    break;

  case 316: /* enum_ls: enum_ls TCOMMA  */
#line 2142 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8805 "bison-chpl-lib.cpp"
    break;

  case 317: /* $@13: %empty  */
#line 2148 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8814 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_ls: enum_ls TCOMMA $@13 deprecated_enum_item  */
#line 2153 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8823 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_ls: unstable_enum_item  */
#line 2159 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 320: /* $@14: %empty  */
#line 2164 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8841 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_ls TCOMMA $@14 unstable_enum_item  */
#line 2169 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 323: /* $@15: %empty  */
#line 2179 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8858 "bison-chpl-lib.cpp"
    break;

  case 324: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@15 enum_item  */
#line 2183 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@16: %empty  */
#line 2187 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8874 "bison-chpl-lib.cpp"
    break;

  case 326: /* deprecated_enum_item: TDEPRECATED $@16 enum_item  */
#line 2191 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8882 "bison-chpl-lib.cpp"
    break;

  case 327: /* $@17: %empty  */
#line 2198 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 8890 "bison-chpl-lib.cpp"
    break;

  case 328: /* unstable_enum_item: TUNSTABLE STRINGLITERAL $@17 enum_item  */
#line 2202 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8898 "bison-chpl-lib.cpp"
    break;

  case 329: /* $@18: %empty  */
#line 2206 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 8906 "bison-chpl-lib.cpp"
    break;

  case 330: /* unstable_enum_item: TUNSTABLE $@18 enum_item  */
#line 2210 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_item: ident_def  */
#line 2217 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8925 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_item: ident_def TASSIGN expr  */
#line 2224 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 333: /* lambda_decl_start: TLAMBDA  */
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
#line 8954 "bison-chpl-lib.cpp"
    break;

  case 334: /* $@19: %empty  */
#line 2253 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8964 "bison-chpl-lib.cpp"
    break;

  case 335: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@19 function_body_stmt  */
#line 2259 "chpl.ypp"
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
#line 8986 "bison-chpl-lib.cpp"
    break;

  case 337: /* linkage_spec: linkage_spec_empty  */
#line 2282 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8992 "bison-chpl-lib.cpp"
    break;

  case 338: /* linkage_spec: TINLINE  */
#line 2283 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8999 "bison-chpl-lib.cpp"
    break;

  case 339: /* linkage_spec: TOVERRIDE  */
#line 2285 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 340: /* opt_fn_type_formal_ls: %empty  */
#line 2290 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 341: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2291 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 342: /* fn_type_formal_ls: fn_type_formal  */
#line 2295 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 343: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2296 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 344: /* fn_type_formal: named_formal  */
#line 2301 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2304 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 346: /* fn_type_formal: formal_type  */
#line 2306 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 347: /* opt_fn_type_ret_type: %empty  */
#line 2312 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 348: /* opt_fn_type_ret_type: TCOLON expr  */
#line 2313 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9060 "bison-chpl-lib.cpp"
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
    fp.throws = ((yyvsp[0].throwsTag) == ThrowsTag_THROWS);
    fp.body = nullptr;
    fp.comments = nullptr;
    fp.attributes = nullptr;
    fp.visibility = context->visibility;
    (yyval.functionParts) = fp;
  }
#line 9079 "bison-chpl-lib.cpp"
    break;

  case 350: /* $@20: %empty  */
#line 2346 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9089 "bison-chpl-lib.cpp"
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
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 352: /* $@21: %empty  */
#line 2361 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9112 "bison-chpl-lib.cpp"
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
#line 9125 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2379 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9133 "bison-chpl-lib.cpp"
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
#line 9147 "bison-chpl-lib.cpp"
    break;

  case 356: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@22 opt_function_body_stmt  */
#line 2398 "chpl.ypp"
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
#line 9170 "bison-chpl-lib.cpp"
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
#line 9182 "bison-chpl-lib.cpp"
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
#line 9194 "bison-chpl-lib.cpp"
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
#line 9209 "bison-chpl-lib.cpp"
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
#line 9224 "bison-chpl-lib.cpp"
    break;

  case 361: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2458 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9234 "bison-chpl-lib.cpp"
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
#line 9250 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2482 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9256 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_ident: ident_def TBANG  */
#line 2489 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9266 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2546 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9272 "bison-chpl-lib.cpp"
    break;

  case 408: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2547 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9278 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2548 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9284 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_formal_ls: %empty  */
#line 2552 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9290 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2553 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9296 "bison-chpl-lib.cpp"
    break;

  case 412: /* req_formal_ls: TLP TRP  */
#line 2557 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 9302 "bison-chpl-lib.cpp"
    break;

  case 413: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2558 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9308 "bison-chpl-lib.cpp"
    break;

  case 414: /* formal_ls_inner: formal  */
#line 2562 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9314 "bison-chpl-lib.cpp"
    break;

  case 415: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2563 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9320 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_ls: %empty  */
#line 2567 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9326 "bison-chpl-lib.cpp"
    break;

  case 417: /* formal_ls: formal_ls_inner  */
#line 2568 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 421: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2582 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9340 "bison-chpl-lib.cpp"
    break;

  case 422: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2587 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9348 "bison-chpl-lib.cpp"
    break;

  case 423: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2592 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 424: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2597 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9364 "bison-chpl-lib.cpp"
    break;

  case 425: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2602 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9372 "bison-chpl-lib.cpp"
    break;

  case 426: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2607 "chpl.ypp"
  {
    (yyval.expr) = CHPL_PARSER_REPORT_SYNTAX(
        context, (yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 9381 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_formal_intent_tag: %empty  */
#line 2614 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9390 "bison-chpl-lib.cpp"
    break;

  case 428: /* opt_formal_intent_tag: required_intent_tag  */
#line 2619 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 429: /* required_intent_tag: TIN  */
#line 2626 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9405 "bison-chpl-lib.cpp"
    break;

  case 430: /* required_intent_tag: TINOUT  */
#line 2627 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 431: /* required_intent_tag: TOUT  */
#line 2628 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 432: /* required_intent_tag: TCONST TIN  */
#line 2629 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9423 "bison-chpl-lib.cpp"
    break;

  case 433: /* required_intent_tag: TCONST TREF  */
#line 2630 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 434: /* required_intent_tag: TCONST  */
#line 2631 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 435: /* required_intent_tag: TPARAM  */
#line 2632 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 436: /* required_intent_tag: TREF  */
#line 2633 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 437: /* required_intent_tag: TTYPE  */
#line 2634 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_this_intent_tag: %empty  */
#line 2638 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_this_intent_tag: TPARAM  */
#line 2639 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_this_intent_tag: TREF  */
#line 2640 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_this_intent_tag: TCONST TREF  */
#line 2641 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_this_intent_tag: TCONST  */
#line 2642 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9483 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_this_intent_tag: TTYPE  */
#line 2643 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9489 "bison-chpl-lib.cpp"
    break;

  case 444: /* proc_iter_or_op: TPROC  */
#line 2647 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9495 "bison-chpl-lib.cpp"
    break;

  case 445: /* proc_iter_or_op: TITER  */
#line 2648 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9501 "bison-chpl-lib.cpp"
    break;

  case 446: /* proc_iter_or_op: TOPERATOR  */
#line 2649 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9507 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_ret_tag: %empty  */
#line 2653 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9513 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_ret_tag: TCONST  */
#line 2654 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9519 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_ret_tag: TCONST TREF  */
#line 2655 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9525 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_ret_tag: TREF  */
#line 2656 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9531 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_ret_tag: TPARAM  */
#line 2657 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9537 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_ret_tag: TTYPE  */
#line 2658 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9543 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_throws_error: %empty  */
#line 2662 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9549 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_throws_error: TTHROWS  */
#line 2663 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9555 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_function_body_stmt: TSEMI  */
#line 2666 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_function_body_stmt: function_body_stmt  */
#line 2667 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9567 "bison-chpl-lib.cpp"
    break;

  case 457: /* function_body_stmt: block_stmt_body  */
#line 2671 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9573 "bison-chpl-lib.cpp"
    break;

  case 458: /* function_body_stmt: return_stmt  */
#line 2672 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9579 "bison-chpl-lib.cpp"
    break;

  case 459: /* query_expr: TQUERIEDIDENT  */
#line 2676 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9585 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_lifetime_where: %empty  */
#line 2681 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9591 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_lifetime_where: TWHERE expr  */
#line 2683 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9597 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2685 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9603 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2687 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9609 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2689 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9615 "bison-chpl-lib.cpp"
    break;

  case 465: /* lifetime_components_expr: lifetime_expr  */
#line 2694 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9621 "bison-chpl-lib.cpp"
    break;

  case 466: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2696 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9627 "bison-chpl-lib.cpp"
    break;

  case 467: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2701 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9633 "bison-chpl-lib.cpp"
    break;

  case 468: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2703 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9639 "bison-chpl-lib.cpp"
    break;

  case 469: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2705 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 470: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2707 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9651 "bison-chpl-lib.cpp"
    break;

  case 471: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2709 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 472: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2711 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9663 "bison-chpl-lib.cpp"
    break;

  case 473: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2713 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 474: /* lifetime_ident: TIDENT  */
#line 2717 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9675 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_ident: TTHIS  */
#line 2718 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9681 "bison-chpl-lib.cpp"
    break;

  case 476: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9690 "bison-chpl-lib.cpp"
    break;

  case 477: /* type_alias_decl_stmt_start: TTYPE  */
#line 2732 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9698 "bison-chpl-lib.cpp"
    break;

  case 478: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2736 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9707 "bison-chpl-lib.cpp"
    break;

  case 479: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2744 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9715 "bison-chpl-lib.cpp"
    break;

  case 480: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2748 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9723 "bison-chpl-lib.cpp"
    break;

  case 481: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2755 "chpl.ypp"
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
#line 9745 "bison-chpl-lib.cpp"
    break;

  case 482: /* opt_init_type: %empty  */
#line 2775 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9751 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_init_type: TASSIGN type_level_expr  */
#line 2777 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9757 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_init_type: TASSIGN array_type  */
#line 2779 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9766 "bison-chpl-lib.cpp"
    break;

  case 485: /* var_decl_type: TPARAM  */
#line 2786 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9772 "bison-chpl-lib.cpp"
    break;

  case 486: /* var_decl_type: TCONST TREF  */
#line 2787 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9778 "bison-chpl-lib.cpp"
    break;

  case 487: /* var_decl_type: TREF  */
#line 2788 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9784 "bison-chpl-lib.cpp"
    break;

  case 488: /* var_decl_type: TCONST  */
#line 2789 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9790 "bison-chpl-lib.cpp"
    break;

  case 489: /* var_decl_type: TVAR  */
#line 2790 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9796 "bison-chpl-lib.cpp"
    break;

  case 490: /* $@23: %empty  */
#line 2795 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9805 "bison-chpl-lib.cpp"
    break;

  case 491: /* var_decl_stmt: TCONFIG $@23 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2799 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9814 "bison-chpl-lib.cpp"
    break;

  case 492: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2804 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9823 "bison-chpl-lib.cpp"
    break;

  case 493: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2812 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2816 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9839 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2823 "chpl.ypp"
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
#line 9859 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2839 "chpl.ypp"
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
#line 9877 "bison-chpl-lib.cpp"
    break;

  case 497: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2856 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9885 "bison-chpl-lib.cpp"
    break;

  case 498: /* tuple_var_decl_component: ident_def  */
#line 2860 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9893 "bison-chpl-lib.cpp"
    break;

  case 499: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2864 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9901 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2871 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9907 "bison-chpl-lib.cpp"
    break;

  case 501: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2873 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9913 "bison-chpl-lib.cpp"
    break;

  case 502: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2875 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9919 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_init_expr: %empty  */
#line 2881 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9925 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2882 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9931 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2883 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9937 "bison-chpl-lib.cpp"
    break;

  case 506: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2888 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9945 "bison-chpl-lib.cpp"
    break;

  case 507: /* ret_array_type: TLSBR TRSBR  */
#line 2892 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 9953 "bison-chpl-lib.cpp"
    break;

  case 508: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2896 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9961 "bison-chpl-lib.cpp"
    break;

  case 509: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2900 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 9969 "bison-chpl-lib.cpp"
    break;

  case 510: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2904 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9977 "bison-chpl-lib.cpp"
    break;

  case 511: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2908 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9985 "bison-chpl-lib.cpp"
    break;

  case 512: /* ret_array_type: TLSBR error TRSBR  */
#line 2912 "chpl.ypp"
  {
    (yyval.expr) = CHPL_PARSER_REPORT_SYNTAX(
        context, (yyloc), "invalid expression for domain of array return type.");
  }
#line 9994 "bison-chpl-lib.cpp"
    break;

  case 513: /* ret_type: type_level_expr  */
#line 2919 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 10000 "bison-chpl-lib.cpp"
    break;

  case 514: /* ret_type: ret_array_type  */
#line 2920 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 515: /* ret_type: reserved_type_ident_use  */
#line 2921 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10012 "bison-chpl-lib.cpp"
    break;

  case 516: /* ret_type: error  */
#line 2922 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10018 "bison-chpl-lib.cpp"
    break;

  case 517: /* colon_ret_type: TCOLON ret_type  */
#line 2926 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10024 "bison-chpl-lib.cpp"
    break;

  case 518: /* colon_ret_type: error  */
#line 2927 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10030 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_ret_type: %empty  */
#line 2931 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 10036 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_type: %empty  */
#line 2936 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 10042 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_type: TCOLON type_level_expr  */
#line 2937 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10048 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: TCOLON array_type  */
#line 2938 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10054 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2939 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10060 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_type: error  */
#line 2940 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10066 "bison-chpl-lib.cpp"
    break;

  case 526: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2961 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 527: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2965 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10082 "bison-chpl-lib.cpp"
    break;

  case 528: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2969 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 529: /* array_type: TLSBR error TRSBR  */
#line 2973 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 10098 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_formal_array_elt_type: %empty  */
#line 2979 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 10104 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_formal_array_elt_type: type_level_expr  */
#line 2980 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10110 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_formal_array_elt_type: query_expr  */
#line 2981 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10116 "bison-chpl-lib.cpp"
    break;

  case 533: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2986 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10125 "bison-chpl-lib.cpp"
    break;

  case 534: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2991 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10133 "bison-chpl-lib.cpp"
    break;

  case 535: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2999 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10142 "bison-chpl-lib.cpp"
    break;

  case 536: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 3004 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10150 "bison-chpl-lib.cpp"
    break;

  case 537: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 3008 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10158 "bison-chpl-lib.cpp"
    break;

  case 540: /* formal_type: reserved_type_ident_use  */
#line 3016 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10164 "bison-chpl-lib.cpp"
    break;

  case 542: /* colon_formal_type: TCOLON formal_type  */
#line 3021 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10170 "bison-chpl-lib.cpp"
    break;

  case 543: /* opt_colon_formal_type: %empty  */
#line 3025 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 10176 "bison-chpl-lib.cpp"
    break;

  case 544: /* opt_colon_formal_type: colon_formal_type  */
#line 3026 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10182 "bison-chpl-lib.cpp"
    break;

  case 545: /* expr_ls: expr  */
#line 3032 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10188 "bison-chpl-lib.cpp"
    break;

  case 546: /* expr_ls: query_expr  */
#line 3033 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10194 "bison-chpl-lib.cpp"
    break;

  case 547: /* expr_ls: expr_ls TCOMMA expr  */
#line 3034 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 548: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 3035 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10206 "bison-chpl-lib.cpp"
    break;

  case 549: /* simple_expr_ls: expr  */
#line 3039 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10212 "bison-chpl-lib.cpp"
    break;

  case 550: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 3040 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 551: /* tuple_component: TUNDERSCORE  */
#line 3044 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10224 "bison-chpl-lib.cpp"
    break;

  case 552: /* tuple_component: opt_try_expr  */
#line 3045 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10230 "bison-chpl-lib.cpp"
    break;

  case 553: /* tuple_component: query_expr  */
#line 3046 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10236 "bison-chpl-lib.cpp"
    break;

  case 554: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3051 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10244 "bison-chpl-lib.cpp"
    break;

  case 555: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3055 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10252 "bison-chpl-lib.cpp"
    break;

  case 556: /* opt_actual_ls: %empty  */
#line 3061 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 557: /* opt_actual_ls: actual_ls  */
#line 3062 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10264 "bison-chpl-lib.cpp"
    break;

  case 558: /* actual_ls: actual_expr  */
#line 3067 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10273 "bison-chpl-lib.cpp"
    break;

  case 559: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3072 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10283 "bison-chpl-lib.cpp"
    break;

  case 560: /* actual_expr: ident_use TASSIGN query_expr  */
#line 3080 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10289 "bison-chpl-lib.cpp"
    break;

  case 561: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3081 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10295 "bison-chpl-lib.cpp"
    break;

  case 562: /* actual_expr: query_expr  */
#line 3082 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10301 "bison-chpl-lib.cpp"
    break;

  case 563: /* actual_expr: opt_try_expr  */
#line 3083 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10307 "bison-chpl-lib.cpp"
    break;

  case 564: /* ident_expr: ident_use  */
#line 3087 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10313 "bison-chpl-lib.cpp"
    break;

  case 565: /* ident_expr: scalar_type  */
#line 3088 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10319 "bison-chpl-lib.cpp"
    break;

  case 566: /* type_level_expr: sub_type_level_expr  */
#line 3100 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 10325 "bison-chpl-lib.cpp"
    break;

  case 567: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 3102 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10331 "bison-chpl-lib.cpp"
    break;

  case 568: /* type_level_expr: TQUESTION  */
#line 3104 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10337 "bison-chpl-lib.cpp"
    break;

  case 569: /* type_level_expr: fn_type  */
#line 3106 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10343 "bison-chpl-lib.cpp"
    break;

  case 575: /* sub_type_level_expr: TSINGLE expr  */
#line 3118 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10349 "bison-chpl-lib.cpp"
    break;

  case 576: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3120 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10355 "bison-chpl-lib.cpp"
    break;

  case 577: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3122 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10361 "bison-chpl-lib.cpp"
    break;

  case 578: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3124 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10367 "bison-chpl-lib.cpp"
    break;

  case 579: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3126 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10377 "bison-chpl-lib.cpp"
    break;

  case 580: /* sub_type_level_expr: TATOMIC expr  */
#line 3132 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10383 "bison-chpl-lib.cpp"
    break;

  case 581: /* sub_type_level_expr: TSYNC expr  */
#line 3134 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10389 "bison-chpl-lib.cpp"
    break;

  case 582: /* sub_type_level_expr: TOWNED  */
#line 3137 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10395 "bison-chpl-lib.cpp"
    break;

  case 583: /* sub_type_level_expr: TOWNED expr  */
#line 3139 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10401 "bison-chpl-lib.cpp"
    break;

  case 584: /* sub_type_level_expr: TUNMANAGED  */
#line 3141 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10407 "bison-chpl-lib.cpp"
    break;

  case 585: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3143 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10413 "bison-chpl-lib.cpp"
    break;

  case 586: /* sub_type_level_expr: TSHARED  */
#line 3145 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10419 "bison-chpl-lib.cpp"
    break;

  case 587: /* sub_type_level_expr: TSHARED expr  */
#line 3147 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10425 "bison-chpl-lib.cpp"
    break;

  case 588: /* sub_type_level_expr: TBORROWED  */
#line 3149 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10431 "bison-chpl-lib.cpp"
    break;

  case 589: /* sub_type_level_expr: TBORROWED expr  */
#line 3151 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10437 "bison-chpl-lib.cpp"
    break;

  case 590: /* sub_type_level_expr: TCLASS  */
#line 3153 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10443 "bison-chpl-lib.cpp"
    break;

  case 591: /* sub_type_level_expr: TRECORD  */
#line 3155 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10449 "bison-chpl-lib.cpp"
    break;

  case 592: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 3160 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10462 "bison-chpl-lib.cpp"
    break;

  case 593: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3169 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10475 "bison-chpl-lib.cpp"
    break;

  case 594: /* for_expr: TFOR expr TDO expr  */
#line 3178 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10487 "bison-chpl-lib.cpp"
    break;

  case 595: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3186 "chpl.ypp"
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
#line 10505 "bison-chpl-lib.cpp"
    break;

  case 596: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3200 "chpl.ypp"
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
#line 10523 "bison-chpl-lib.cpp"
    break;

  case 597: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3214 "chpl.ypp"
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
#line 10541 "bison-chpl-lib.cpp"
    break;

  case 598: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3228 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10554 "bison-chpl-lib.cpp"
    break;

  case 599: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3237 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10567 "bison-chpl-lib.cpp"
    break;

  case 600: /* for_expr: TFORALL expr TDO expr  */
#line 3246 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10579 "bison-chpl-lib.cpp"
    break;

  case 601: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3254 "chpl.ypp"
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
#line 10597 "bison-chpl-lib.cpp"
    break;

  case 602: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3268 "chpl.ypp"
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
#line 10615 "bison-chpl-lib.cpp"
    break;

  case 603: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3282 "chpl.ypp"
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
#line 10633 "bison-chpl-lib.cpp"
    break;

  case 604: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 3296 "chpl.ypp"
  {
    owned<AstNode> iterand = nullptr;
    auto iterExprs = context->consumeList((yyvsp[-2].exprList));
    if (iterExprs.size() > 1) {
      iterand = Domain::build(BUILDER, LOC((yylsp[-2])), false, std::move(iterExprs));
    } else {
      iterand = std::move(iterExprs[0]);
    }
    CHPL_ASSERT(iterand != nullptr);
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), /*index*/ nullptr,
                            std::move(iterand),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10654 "bison-chpl-lib.cpp"
    break;

  case 605: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3313 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10667 "bison-chpl-lib.cpp"
    break;

  case 606: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3322 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10680 "bison-chpl-lib.cpp"
    break;

  case 607: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3331 "chpl.ypp"
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
#line 10699 "bison-chpl-lib.cpp"
    break;

  case 608: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3346 "chpl.ypp"
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
#line 10718 "bison-chpl-lib.cpp"
    break;

  case 609: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3364 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10732 "bison-chpl-lib.cpp"
    break;

  case 610: /* nil_expr: TNIL  */
#line 3381 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10738 "bison-chpl-lib.cpp"
    break;

  case 618: /* opt_task_intent_ls: %empty  */
#line 3399 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10744 "bison-chpl-lib.cpp"
    break;

  case 619: /* opt_task_intent_ls: task_intent_clause  */
#line 3400 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10750 "bison-chpl-lib.cpp"
    break;

  case 620: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3405 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10760 "bison-chpl-lib.cpp"
    break;

  case 621: /* task_intent_ls: intent_expr  */
#line 3413 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10766 "bison-chpl-lib.cpp"
    break;

  case 622: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3414 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10772 "bison-chpl-lib.cpp"
    break;

  case 623: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3419 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10782 "bison-chpl-lib.cpp"
    break;

  case 624: /* forall_intent_ls: intent_expr  */
#line 3427 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10788 "bison-chpl-lib.cpp"
    break;

  case 625: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3428 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10794 "bison-chpl-lib.cpp"
    break;

  case 626: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3433 "chpl.ypp"
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
      (yyval.expr) = CHPL_PARSER_REPORT_SYNTAX(
          context, (yyloc), "expected identifier for task variable name.");
    }
  }
#line 10813 "bison-chpl-lib.cpp"
    break;

  case 627: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3448 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10821 "bison-chpl-lib.cpp"
    break;

  case 628: /* intent_expr: expr TREDUCE ident_expr  */
#line 3452 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10829 "bison-chpl-lib.cpp"
    break;

  case 629: /* task_var_prefix: TCONST  */
#line 3458 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10835 "bison-chpl-lib.cpp"
    break;

  case 630: /* task_var_prefix: TIN  */
#line 3459 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10841 "bison-chpl-lib.cpp"
    break;

  case 631: /* task_var_prefix: TCONST TIN  */
#line 3460 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10847 "bison-chpl-lib.cpp"
    break;

  case 632: /* task_var_prefix: TREF  */
#line 3461 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10853 "bison-chpl-lib.cpp"
    break;

  case 633: /* task_var_prefix: TCONST TREF  */
#line 3462 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10859 "bison-chpl-lib.cpp"
    break;

  case 634: /* task_var_prefix: TVAR  */
#line 3463 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10865 "bison-chpl-lib.cpp"
    break;

  case 635: /* new_maybe_decorated: TNEW  */
#line 3468 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10871 "bison-chpl-lib.cpp"
    break;

  case 636: /* new_maybe_decorated: TNEW TOWNED  */
#line 3470 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10877 "bison-chpl-lib.cpp"
    break;

  case 637: /* new_maybe_decorated: TNEW TSHARED  */
#line 3472 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10883 "bison-chpl-lib.cpp"
    break;

  case 638: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3474 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10889 "bison-chpl-lib.cpp"
    break;

  case 639: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3476 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10895 "bison-chpl-lib.cpp"
    break;

  case 640: /* new_expr: new_maybe_decorated expr  */
#line 3482 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10903 "bison-chpl-lib.cpp"
    break;

  case 641: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3489 "chpl.ypp"
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
#line 10919 "bison-chpl-lib.cpp"
    break;

  case 642: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3501 "chpl.ypp"
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
#line 10935 "bison-chpl-lib.cpp"
    break;

  case 643: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3513 "chpl.ypp"
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
#line 10952 "bison-chpl-lib.cpp"
    break;

  case 644: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3526 "chpl.ypp"
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
#line 10970 "bison-chpl-lib.cpp"
    break;

  case 645: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3543 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10978 "bison-chpl-lib.cpp"
    break;

  case 646: /* range_literal_expr: expr TDOTDOT expr  */
#line 3550 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10987 "bison-chpl-lib.cpp"
    break;

  case 647: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3555 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10996 "bison-chpl-lib.cpp"
    break;

  case 648: /* range_literal_expr: expr TDOTDOT  */
#line 3560 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 11005 "bison-chpl-lib.cpp"
    break;

  case 649: /* range_literal_expr: TDOTDOT expr  */
#line 3565 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 11015 "bison-chpl-lib.cpp"
    break;

  case 650: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3571 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 11025 "bison-chpl-lib.cpp"
    break;

  case 651: /* range_literal_expr: TDOTDOT  */
#line 3577 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 11035 "bison-chpl-lib.cpp"
    break;

  case 652: /* cast_expr: expr TCOLON expr  */
#line 3607 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 11043 "bison-chpl-lib.cpp"
    break;

  case 653: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3614 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 11051 "bison-chpl-lib.cpp"
    break;

  case 654: /* super_expr: fn_expr  */
#line 3620 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 11057 "bison-chpl-lib.cpp"
    break;

  case 668: /* opt_expr: %empty  */
#line 3640 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 11063 "bison-chpl-lib.cpp"
    break;

  case 669: /* opt_expr: expr  */
#line 3641 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11069 "bison-chpl-lib.cpp"
    break;

  case 670: /* opt_try_expr: TTRY expr  */
#line 3645 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 11075 "bison-chpl-lib.cpp"
    break;

  case 671: /* opt_try_expr: TTRYBANG expr  */
#line 3646 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 11081 "bison-chpl-lib.cpp"
    break;

  case 672: /* opt_try_expr: super_expr  */
#line 3647 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11087 "bison-chpl-lib.cpp"
    break;

  case 678: /* call_base_expr: expr TBANG  */
#line 3664 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 11095 "bison-chpl-lib.cpp"
    break;

  case 679: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3667 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 11101 "bison-chpl-lib.cpp"
    break;

  case 682: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3674 "chpl.ypp"
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
#line 11117 "bison-chpl-lib.cpp"
    break;

  case 683: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3686 "chpl.ypp"
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
#line 11133 "bison-chpl-lib.cpp"
    break;

  case 684: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3698 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 11141 "bison-chpl-lib.cpp"
    break;

  case 685: /* dot_expr: expr TDOT ident_use  */
#line 3705 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11147 "bison-chpl-lib.cpp"
    break;

  case 686: /* dot_expr: expr TDOT TTYPE  */
#line 3707 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11153 "bison-chpl-lib.cpp"
    break;

  case 687: /* dot_expr: expr TDOT TDOMAIN  */
#line 3709 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11159 "bison-chpl-lib.cpp"
    break;

  case 688: /* dot_expr: expr TDOT TLOCALE  */
#line 3711 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11165 "bison-chpl-lib.cpp"
    break;

  case 689: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3713 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 11175 "bison-chpl-lib.cpp"
    break;

  case 690: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3719 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 11185 "bison-chpl-lib.cpp"
    break;

  case 691: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3731 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 11191 "bison-chpl-lib.cpp"
    break;

  case 692: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3733 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 11199 "bison-chpl-lib.cpp"
    break;

  case 693: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3737 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11207 "bison-chpl-lib.cpp"
    break;

  case 694: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3741 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11215 "bison-chpl-lib.cpp"
    break;

  case 695: /* bool_literal: TFALSE  */
#line 3747 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 11221 "bison-chpl-lib.cpp"
    break;

  case 696: /* bool_literal: TTRUE  */
#line 3748 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 11227 "bison-chpl-lib.cpp"
    break;

  case 697: /* str_bytes_literal: STRINGLITERAL  */
#line 3752 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11233 "bison-chpl-lib.cpp"
    break;

  case 698: /* str_bytes_literal: BYTESLITERAL  */
#line 3753 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11239 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: INTLITERAL  */
#line 3759 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 11245 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: REALLITERAL  */
#line 3760 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11251 "bison-chpl-lib.cpp"
    break;

  case 703: /* literal_expr: IMAGLITERAL  */
#line 3761 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11257 "bison-chpl-lib.cpp"
    break;

  case 704: /* literal_expr: CSTRINGLITERAL  */
#line 3762 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11263 "bison-chpl-lib.cpp"
    break;

  case 705: /* literal_expr: TNONE  */
#line 3763 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11269 "bison-chpl-lib.cpp"
    break;

  case 706: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3765 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11278 "bison-chpl-lib.cpp"
    break;

  case 707: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3770 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11287 "bison-chpl-lib.cpp"
    break;

  case 708: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3775 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11295 "bison-chpl-lib.cpp"
    break;

  case 709: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3779 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11304 "bison-chpl-lib.cpp"
    break;

  case 710: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3784 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11312 "bison-chpl-lib.cpp"
    break;

  case 711: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3788 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11321 "bison-chpl-lib.cpp"
    break;

  case 712: /* assoc_expr_ls: expr TALIAS expr  */
#line 3797 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11330 "bison-chpl-lib.cpp"
    break;

  case 713: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3802 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11340 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TPLUS expr  */
#line 3810 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11346 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TMINUS expr  */
#line 3811 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11352 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TSTAR expr  */
#line 3812 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11358 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TDIVIDE expr  */
#line 3813 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11364 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3814 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11370 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3815 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11376 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TMOD expr  */
#line 3816 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11382 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TEQUAL expr  */
#line 3817 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11388 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3818 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11394 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3819 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11400 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3820 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11406 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TLESS expr  */
#line 3821 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11412 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TGREATER expr  */
#line 3822 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11418 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr TBAND expr  */
#line 3823 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11424 "bison-chpl-lib.cpp"
    break;

  case 728: /* binary_op_expr: expr TBOR expr  */
#line 3824 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11430 "bison-chpl-lib.cpp"
    break;

  case 729: /* binary_op_expr: expr TBXOR expr  */
#line 3825 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11436 "bison-chpl-lib.cpp"
    break;

  case 730: /* binary_op_expr: expr TAND expr  */
#line 3826 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11442 "bison-chpl-lib.cpp"
    break;

  case 731: /* binary_op_expr: expr TOR expr  */
#line 3827 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11448 "bison-chpl-lib.cpp"
    break;

  case 732: /* binary_op_expr: expr TEXP expr  */
#line 3828 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11454 "bison-chpl-lib.cpp"
    break;

  case 733: /* binary_op_expr: expr TBY expr  */
#line 3829 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11460 "bison-chpl-lib.cpp"
    break;

  case 734: /* binary_op_expr: expr TALIGN expr  */
#line 3830 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11466 "bison-chpl-lib.cpp"
    break;

  case 735: /* binary_op_expr: expr THASH expr  */
#line 3831 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11472 "bison-chpl-lib.cpp"
    break;

  case 736: /* binary_op_expr: expr TDMAPPED expr  */
#line 3832 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11478 "bison-chpl-lib.cpp"
    break;

  case 737: /* unary_op_expr: TPLUS expr  */
#line 3836 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11484 "bison-chpl-lib.cpp"
    break;

  case 738: /* unary_op_expr: TMINUS expr  */
#line 3837 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11490 "bison-chpl-lib.cpp"
    break;

  case 739: /* unary_op_expr: TMINUSMINUS expr  */
#line 3838 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11496 "bison-chpl-lib.cpp"
    break;

  case 740: /* unary_op_expr: TPLUSPLUS expr  */
#line 3839 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11502 "bison-chpl-lib.cpp"
    break;

  case 741: /* unary_op_expr: TBANG expr  */
#line 3840 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11508 "bison-chpl-lib.cpp"
    break;

  case 742: /* unary_op_expr: expr TBANG  */
#line 3841 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11516 "bison-chpl-lib.cpp"
    break;

  case 743: /* unary_op_expr: TBNOT expr  */
#line 3844 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11522 "bison-chpl-lib.cpp"
    break;

  case 744: /* reduce_expr: expr TREDUCE expr  */
#line 3849 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11530 "bison-chpl-lib.cpp"
    break;

  case 745: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3853 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11538 "bison-chpl-lib.cpp"
    break;

  case 746: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3857 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11546 "bison-chpl-lib.cpp"
    break;

  case 747: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3861 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11554 "bison-chpl-lib.cpp"
    break;

  case 748: /* scan_expr: expr TSCAN expr  */
#line 3868 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11562 "bison-chpl-lib.cpp"
    break;

  case 749: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3872 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11570 "bison-chpl-lib.cpp"
    break;

  case 750: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3876 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11578 "bison-chpl-lib.cpp"
    break;

  case 751: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3880 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11586 "bison-chpl-lib.cpp"
    break;


#line 11590 "bison-chpl-lib.cpp"

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
