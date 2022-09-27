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
  YYSYMBOL_TLESS = 157,                    /* TLESS  */
  YYSYMBOL_TLESSEQUAL = 158,               /* TLESSEQUAL  */
  YYSYMBOL_TMINUS = 159,                   /* TMINUS  */
  YYSYMBOL_TMOD = 160,                     /* TMOD  */
  YYSYMBOL_TNOTEQUAL = 161,                /* TNOTEQUAL  */
  YYSYMBOL_TOR = 162,                      /* TOR  */
  YYSYMBOL_TPLUS = 163,                    /* TPLUS  */
  YYSYMBOL_TQUESTION = 164,                /* TQUESTION  */
  YYSYMBOL_TSEMI = 165,                    /* TSEMI  */
  YYSYMBOL_TSHIFTLEFT = 166,               /* TSHIFTLEFT  */
  YYSYMBOL_TSHIFTRIGHT = 167,              /* TSHIFTRIGHT  */
  YYSYMBOL_TSTAR = 168,                    /* TSTAR  */
  YYSYMBOL_TSWAP = 169,                    /* TSWAP  */
  YYSYMBOL_TLCBR = 170,                    /* TLCBR  */
  YYSYMBOL_TRCBR = 171,                    /* TRCBR  */
  YYSYMBOL_TLP = 172,                      /* TLP  */
  YYSYMBOL_TRP = 173,                      /* TRP  */
  YYSYMBOL_TLSBR = 174,                    /* TLSBR  */
  YYSYMBOL_TRSBR = 175,                    /* TRSBR  */
  YYSYMBOL_TNOELSE = 176,                  /* TNOELSE  */
  YYSYMBOL_TDOTDOTOPENHIGH = 177,          /* TDOTDOTOPENHIGH  */
  YYSYMBOL_TUPLUS = 178,                   /* TUPLUS  */
  YYSYMBOL_TUMINUS = 179,                  /* TUMINUS  */
  YYSYMBOL_TLNOT = 180,                    /* TLNOT  */
  YYSYMBOL_YYACCEPT = 181,                 /* $accept  */
  YYSYMBOL_program = 182,                  /* program  */
  YYSYMBOL_toplevel_stmt_ls = 183,         /* toplevel_stmt_ls  */
  YYSYMBOL_toplevel_stmt = 184,            /* toplevel_stmt  */
  YYSYMBOL_pragma_ls = 185,                /* pragma_ls  */
  YYSYMBOL_stmt = 186,                     /* stmt  */
  YYSYMBOL_tryable_stmt = 187,             /* tryable_stmt  */
  YYSYMBOL_deprecated_decl_stmt = 188,     /* deprecated_decl_stmt  */
  YYSYMBOL_189_1 = 189,                    /* $@1  */
  YYSYMBOL_190_2 = 190,                    /* $@2  */
  YYSYMBOL_decl_base = 191,                /* decl_base  */
  YYSYMBOL_unstable_decl_stmt = 192,       /* unstable_decl_stmt  */
  YYSYMBOL_193_3 = 193,                    /* $@3  */
  YYSYMBOL_194_4 = 194,                    /* $@4  */
  YYSYMBOL_module_decl_start = 195,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 196,         /* module_decl_stmt  */
  YYSYMBOL_opt_access_control = 197,       /* opt_access_control  */
  YYSYMBOL_opt_prototype = 198,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 199,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 200,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 201,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 202,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 203,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 204,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 205,              /* opt_only_ls  */
  YYSYMBOL_use_stmt = 206,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 207,              /* import_stmt  */
  YYSYMBOL_import_expr = 208,              /* import_expr  */
  YYSYMBOL_import_ls = 209,                /* import_ls  */
  YYSYMBOL_require_stmt = 210,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 211,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 212,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 213,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 214,                /* ident_def  */
  YYSYMBOL_ident_use = 215,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 216,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 217,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 218,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 219,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 220,              /* return_stmt  */
  YYSYMBOL_deprecated_class_level_stmt = 221, /* deprecated_class_level_stmt  */
  YYSYMBOL_222_5 = 222,                    /* $@5  */
  YYSYMBOL_223_6 = 223,                    /* $@6  */
  YYSYMBOL_unstable_class_level_stmt = 224, /* unstable_class_level_stmt  */
  YYSYMBOL_225_7 = 225,                    /* $@7  */
  YYSYMBOL_226_8 = 226,                    /* $@8  */
  YYSYMBOL_class_level_stmt = 227,         /* class_level_stmt  */
  YYSYMBOL_228_9 = 228,                    /* $@9  */
  YYSYMBOL_229_10 = 229,                   /* $@10  */
  YYSYMBOL_inner_class_level_stmt = 230,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 231,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 232,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 233,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 234, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 235,  /* extern_export_decl_stmt  */
  YYSYMBOL_236_11 = 236,                   /* $@11  */
  YYSYMBOL_237_12 = 237,                   /* $@12  */
  YYSYMBOL_extern_block_stmt = 238,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 239,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 240,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 241,                  /* if_stmt  */
  YYSYMBOL_ifvar = 242,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 243,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 244,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 245,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 246,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 247, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 248,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 249,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 250,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 251,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 252,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 253,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 254,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 255,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 256,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 257,                /* when_stmt  */
  YYSYMBOL_manager_expr = 258,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 259,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 260,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 261,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 262,              /* class_start  */
  YYSYMBOL_class_tag = 263,                /* class_tag  */
  YYSYMBOL_opt_inherit = 264,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 265,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 266,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 267,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 268,                  /* enum_ls  */
  YYSYMBOL_269_13 = 269,                   /* $@13  */
  YYSYMBOL_270_14 = 270,                   /* $@14  */
  YYSYMBOL_deprecated_enum_item = 271,     /* deprecated_enum_item  */
  YYSYMBOL_272_15 = 272,                   /* $@15  */
  YYSYMBOL_273_16 = 273,                   /* $@16  */
  YYSYMBOL_unstable_enum_item = 274,       /* unstable_enum_item  */
  YYSYMBOL_275_17 = 275,                   /* $@17  */
  YYSYMBOL_276_18 = 276,                   /* $@18  */
  YYSYMBOL_enum_item = 277,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 278,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 279,         /* lambda_decl_expr  */
  YYSYMBOL_280_19 = 280,                   /* $@19  */
  YYSYMBOL_linkage_spec_empty = 281,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 282,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt_complete = 283,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 284,             /* fn_decl_stmt  */
  YYSYMBOL_285_20 = 285,                   /* $@20  */
  YYSYMBOL_fn_decl_stmt_inner = 286,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 287,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 288,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 289,                 /* fn_ident  */
  YYSYMBOL_op_ident = 290,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 291,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 292,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 293,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 294,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 295,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 296,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 297,                /* formal_ls  */
  YYSYMBOL_formal = 298,                   /* formal  */
  YYSYMBOL_opt_formal_intent_tag = 299,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 300,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 301,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 302,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 303,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 304,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 305,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 306,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 307,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 308,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 309, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 310,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 311,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 312,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 313, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 314, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 315, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 316,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 317,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 318,            /* var_decl_stmt  */
  YYSYMBOL_319_21 = 319,                   /* $@21  */
  YYSYMBOL_var_decl_stmt_inner_ls = 320,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 321,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 322, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 323, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 324,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 325,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 326,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 327,                 /* opt_type  */
  YYSYMBOL_array_type = 328,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 329, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 330,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 331,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 332,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 333,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 334,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 335,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 336,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 337,                /* actual_ls  */
  YYSYMBOL_actual_expr = 338,              /* actual_expr  */
  YYSYMBOL_ident_expr = 339,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 340,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 341,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 342,                 /* for_expr  */
  YYSYMBOL_cond_expr = 343,                /* cond_expr  */
  YYSYMBOL_nil_expr = 344,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 345,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 346,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 347,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 348,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 349,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 350,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 351,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 352,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 353,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 354,                 /* new_expr  */
  YYSYMBOL_let_expr = 355,                 /* let_expr  */
  YYSYMBOL_expr = 356,                     /* expr  */
  YYSYMBOL_opt_expr = 357,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 358,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 359,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 360,           /* call_base_expr  */
  YYSYMBOL_call_expr = 361,                /* call_expr  */
  YYSYMBOL_dot_expr = 362,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 363,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 364,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 365,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 366,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 367,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 368,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 369,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 370,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 371,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 372       /* reduce_scan_op_expr  */
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

#line 496 "bison-chpl-lib.cpp"

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
#define YYLAST   20393

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  192
/* YYNRULES -- Number of rules.  */
#define YYNRULES  734
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1295

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   435


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
     175,   176,   177,   178,   179,   180
};

#if YYCHPL_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   612,   612,   616,   617,   622,   623,   631,   635,   643,
     644,   645,   646,   647,   648,   649,   650,   651,   652,   653,
     654,   667,   668,   669,   678,   687,   691,   698,   706,   707,
     708,   709,   710,   711,   712,   713,   727,   742,   750,   762,
     773,   785,   797,   808,   824,   826,   825,   834,   833,   844,
     845,   850,   849,   858,   857,   868,   888,   903,   918,   934,
     935,   937,   942,   943,   948,   966,   970,   974,   985,  1004,
    1005,  1009,  1013,  1017,  1022,  1026,  1030,  1040,  1045,  1051,
    1058,  1063,  1070,  1081,  1082,  1086,  1091,  1099,  1108,  1118,
    1126,  1135,  1148,  1156,  1160,  1165,  1171,  1180,  1181,  1185,
    1194,  1198,  1202,  1206,  1210,  1214,  1223,  1224,  1228,  1229,
    1230,  1231,  1232,  1233,  1239,  1240,  1241,  1242,  1243,  1244,
    1259,  1260,  1271,  1272,  1273,  1274,  1275,  1276,  1277,  1278,
    1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,
    1289,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,
    1302,  1303,  1304,  1311,  1312,  1313,  1314,  1318,  1319,  1323,
    1330,  1340,  1342,  1341,  1350,  1349,  1361,  1360,  1369,  1368,
    1379,  1383,  1389,  1389,  1395,  1395,  1404,  1405,  1406,  1407,
    1408,  1409,  1410,  1414,  1419,  1424,  1429,  1436,  1444,  1445,
    1449,  1458,  1464,  1472,  1491,  1490,  1500,  1499,  1512,  1519,
    1537,  1550,  1563,  1567,  1571,  1575,  1579,  1583,  1587,  1591,
    1609,  1613,  1617,  1621,  1625,  1629,  1633,  1637,  1641,  1645,
    1649,  1653,  1657,  1661,  1665,  1669,  1673,  1677,  1681,  1685,
    1689,  1693,  1697,  1702,  1710,  1719,  1723,  1727,  1731,  1735,
    1739,  1743,  1747,  1751,  1756,  1761,  1766,  1774,  1789,  1807,
    1811,  1818,  1819,  1824,  1829,  1830,  1831,  1832,  1833,  1834,
    1835,  1836,  1837,  1838,  1839,  1840,  1841,  1853,  1854,  1855,
    1856,  1865,  1866,  1870,  1874,  1878,  1885,  1889,  1893,  1900,
    1904,  1908,  1912,  1919,  1920,  1924,  1928,  1932,  1939,  1952,
    1968,  1976,  1980,  1989,  1990,  1994,  1998,  2003,  2011,  2016,
    2020,  2027,  2028,  2032,  2041,  2046,  2057,  2064,  2065,  2066,
    2070,  2071,  2075,  2079,  2083,  2087,  2091,  2098,  2115,  2128,
    2135,  2140,  2147,  2146,  2157,  2163,  2162,  2176,  2178,  2177,
    2186,  2185,  2197,  2196,  2205,  2204,  2215,  2222,  2234,  2251,
    2249,  2276,  2280,  2281,  2283,  2288,  2298,  2295,  2329,  2337,
    2345,  2356,  2367,  2376,  2391,  2392,  2396,  2397,  2398,  2407,
    2408,  2409,  2410,  2411,  2412,  2413,  2414,  2415,  2416,  2417,
    2418,  2419,  2420,  2421,  2422,  2423,  2424,  2425,  2426,  2427,
    2428,  2429,  2430,  2431,  2435,  2436,  2437,  2438,  2439,  2440,
    2441,  2442,  2443,  2444,  2445,  2446,  2451,  2452,  2456,  2457,
    2458,  2462,  2463,  2467,  2471,  2472,  2476,  2477,  2481,  2490,
    2501,  2511,  2523,  2535,  2543,  2547,  2555,  2556,  2557,  2558,
    2559,  2560,  2561,  2562,  2563,  2567,  2568,  2569,  2570,  2571,
    2572,  2576,  2577,  2578,  2582,  2583,  2584,  2585,  2586,  2587,
    2591,  2592,  2595,  2596,  2600,  2601,  2605,  2610,  2611,  2613,
    2615,  2617,  2622,  2624,  2629,  2631,  2633,  2635,  2637,  2639,
    2641,  2646,  2647,  2651,  2660,  2664,  2672,  2676,  2683,  2704,
    2705,  2707,  2715,  2716,  2717,  2718,  2719,  2724,  2723,  2732,
    2740,  2744,  2751,  2767,  2784,  2788,  2792,  2799,  2801,  2803,
    2810,  2811,  2812,  2817,  2821,  2825,  2829,  2833,  2837,  2841,
    2848,  2849,  2850,  2851,  2852,  2857,  2858,  2859,  2860,  2861,
    2881,  2885,  2889,  2893,  2900,  2901,  2902,  2906,  2911,  2919,
    2924,  2928,  2935,  2936,  2937,  2938,  2939,  2945,  2946,  2947,
    2948,  2952,  2953,  2957,  2958,  2959,  2963,  2967,  2974,  2975,
    2979,  2984,  2993,  2994,  2995,  2996,  3000,  3001,  3012,  3014,
    3016,  3022,  3023,  3024,  3025,  3026,  3027,  3029,  3031,  3033,
    3035,  3041,  3043,  3046,  3048,  3050,  3052,  3054,  3056,  3058,
    3060,  3063,  3065,  3070,  3079,  3088,  3096,  3110,  3124,  3138,
    3147,  3156,  3164,  3178,  3192,  3206,  3223,  3232,  3241,  3256,
    3274,  3292,  3300,  3301,  3302,  3303,  3304,  3305,  3306,  3310,
    3311,  3315,  3324,  3325,  3329,  3338,  3339,  3343,  3358,  3362,
    3369,  3370,  3371,  3372,  3373,  3374,  3378,  3380,  3382,  3384,
    3386,  3392,  3399,  3411,  3423,  3436,  3453,  3463,  3464,  3465,
    3466,  3467,  3468,  3469,  3470,  3471,  3472,  3476,  3480,  3485,
    3490,  3495,  3501,  3507,  3539,  3540,  3544,  3545,  3546,  3550,
    3551,  3552,  3553,  3562,  3563,  3566,  3567,  3568,  3572,  3584,
    3596,  3603,  3605,  3607,  3609,  3611,  3617,  3630,  3631,  3635,
    3639,  3646,  3647,  3651,  3652,  3656,  3657,  3658,  3659,  3660,
    3661,  3662,  3663,  3668,  3673,  3677,  3682,  3686,  3695,  3700,
    3709,  3710,  3711,  3712,  3713,  3714,  3715,  3716,  3717,  3718,
    3719,  3720,  3721,  3722,  3723,  3724,  3725,  3726,  3727,  3728,
    3729,  3730,  3731,  3735,  3736,  3737,  3738,  3739,  3740,  3743,
    3747,  3751,  3755,  3759,  3766,  3770,  3774,  3778,  3786,  3787,
    3788,  3789,  3790,  3791,  3792
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
  "forall_intent_ls", "intent_expr", "task_var_prefix",
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

#define YYPACT_NINF (-1156)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-735)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1156,    86,  3755, -1156,   -57,    85, -1156, -1156, -1156, -1156,
   -1156, -1156, 12121,    31,   167,   170, 14983,   258, 20102,    31,
   12121,   267,   266,   288,   167,  5155, 12121,   437,  5155,   254,
   20189, -1156,   442,   427,  8991, 10383, 10383, -1156,  9164,   468,
     194,   365, -1156,   487, 20189, 20189, 20189, -1156,  3189, 10556,
     490, 12121, 12121,   157, -1156,   517,   526, 12121, -1156, 14983,
   -1156, 12121,   559,   430,   202,   420,   554, 20276, -1156, 10731,
    8470, 12121, 10556, 14983, 12121,   518,   576,   465,  5155,   586,
   12121,   591,  7426,  7426, -1156,   597, -1156, 14983,   648, -1156,
     616,  9164,  9337, -1156, 12121, -1156, 12121, -1156, -1156, 14500,
   12121, -1156, 12121, -1156, -1156, -1156,  4105,  7601,  9512, 12121,
   -1156,  4980, -1156, -1156, -1156, -1156, -1156,   505, -1156,   218,
   -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156,
   -1156, -1156, -1156,  7774, -1156, 10904, -1156, -1156, -1156, -1156,
   -1156,   623, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156,
     536, 20189, -1156, 19823,   521,   196, -1156,   283, -1156, -1156,
     236,   429, -1156, 20189,  3189, -1156,   532, -1156,   537, -1156,
   -1156,   541,   542, 12121,   543,   545, 19388,  1085,   308,   558,
     561, -1156, -1156,   435, -1156, -1156, -1156, -1156, -1156,   269,
   -1156, -1156, -1156, 12121, 12121, 12121, 20189, -1156, 12121, 10731,
   10731,   640,   461, -1156, -1156, -1156, -1156, 19388,   464, -1156,
   -1156,   547,  5155, -1156, -1156,   563,    72,   533, 16674, -1156,
     231, -1156,   564, -1156,   -36, 19388, -1156, 19726,   604,  8645,
   -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156,
   -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156,
   -1156, -1156, -1156, -1156,   560, -1156, -1156, 20189,   568,    39,
    3300,    34,  2641,    34, 16264, 20189, 20189,   -19, 15580, -1156,
   -1156,   646,  8645,   570,   473,  5155,  1954,   101,    16, -1156,
    5155, -1156, -1156, 16750, -1156,    37, 17303,   660, -1156,   574,
     587, -1156, 16750,    72,   660, -1156,  8645,  1791,  1791, -1156,
   -1156,     9, 19388, 12121, 12121, -1156, 19388,   572, 17383, -1156,
   16750,    72, 19388,   588,  8645, -1156, 19388, 17422, -1156, -1156,
   -1156, -1156,    72, -1156, 19726,    39, 16750,   596,   223,   223,
    1040,   660,   660,   200, -1156, -1156,  4280,   -29, -1156, 12121,
   -1156,    94,   109, -1156,   -30,    45, 17469,   -40,  1040,   727,
   -1156,  4455, 12121, -1156, 12121,   697, -1156, -1156, 16355,    56,
     536, 19388,   763, 20189, 10731,   594, -1156,   600,   757,   759,
     647,   115, -1156, -1156, -1156,   414,   236, -1156, -1156, -1156,
   -1156,   682, -1156, -1156, -1156,    54,   688, -1156, -1156, -1156,
   14332,   653,   212, -1156,   216,   492, -1156,   633, 12121, 12121,
   12121, 10383, 10383, 12121,   511, 12121, 12121, 12121, 12121, 12121,
     451, 14500, 12121, 12121, 12121, 12121, 12121, 12121, 12121, 12121,
   12121, 12121, 12121, 12121, 12121, 12121, 12121, 12121,   715, -1156,
   -1156, -1156, -1156, -1156,  9337,  9337, -1156, -1156, -1156, -1156,
    9337, -1156, -1156,  9337,  9337,  8645,  8645, 10383, 10383, 16826,
   16902, 17584,   613,    15, 20189,  8297, -1156, -1156,  4630, -1156,
   10383,    39,  3189, -1156, 12121, -1156, 19726, -1156, 20189, -1156,
   -1156, -1156,   646, 12121,   663, -1156,   617,   644, -1156, -1156,
   -1156,   752, 10731, -1156,  5330, 10383, -1156,   631, -1156,    39,
    5505, 10383, -1156,    39, -1156,    39, 10383, -1156,    39,   678,
     681,  5155,   776,  5155, -1156,   777, 12121,   750,   645,  8645,
   20189, -1156, -1156, -1156,  1954, -1156, -1156,   124, -1156, 12294,
     695, 12121,  3189, -1156, -1156, 12121, -1156, 19914, 12121, 12121,
   -1156,   651, -1156, -1156, 10731, -1156, 19388, 19388, -1156,    51,
   -1156,  8645,   665, -1156,   818,   818, 19726, -1156, -1156, -1156,
   -1156, -1156, -1156, -1156,  9687, -1156, 17628,  7949, -1156,  8124,
   -1156,  5155,   666, 10383,  9862,  3930,   670, 12121, 11077, -1156,
   -1156,   361, -1156,  4805, -1156,   293, 17675,   303, 16446, 20189,
    7253,  7253, -1156,   536,   676, -1156,   231, -1156,   -45,   705,
    1261, -1156, -1156, 20189, -1156, 20189, 12121,   128, -1156,    53,
   -1156, -1156, -1156, -1156, -1156, -1156,   549,   708,   683, -1156,
    3594, -1156,    99, -1156, -1156, 12467,   753, -1156,   689,   132,
   -1156,   711, -1156,   713,   717,   728,   722,   725, -1156,   726,
     735,   730,   731,   732,   189,   736,   733,   734, -1156, -1156,
   -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156,
   -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156,
   12121, -1156,   745,   747,   740,   689, -1156,   689, -1156, 12640,
   -1156, 20189, -1156, -1156,  2930,  2930,   516, -1156,   516, -1156,
     516, 19583,   855,   183,   602,    72,   223, -1156,   512, -1156,
   -1156, -1156, -1156, -1156,  1040, 17263,   516,  1478,  1478,  2930,
    1478,  1478,  1628,   223, 17263, 19546,  1628,   660,   660,   223,
    1040,   729,   739,   741,   742,   748,   751,   719,   720, -1156,
     516, -1156,   516, 12813, 10383, 12986, 10383, 12121,  8645, 10383,
   15265,   718,    57, -1156, -1156, -1156,   136, -1156,  1293, 19507,
     438,    31, 17780, -1156,   322, 19388, -1156, 17827,  8645, -1156,
    8645, 12121,   146,  9164, 19388,    41, 16978,  8297, -1156,  9164,
   19388,    36, 16522, -1156, -1156,    34, 16598, -1156, 12121, 12121,
     862,  5155,   864, 17871,  5155, 17054, 20189, -1156,   173, -1156,
     219, -1156,   251,  1954,   101,    21,    42, 12121, 12121,  7080,
   -1156, -1156,   415,  8818, -1156, 19388, -1156, -1156, -1156, 20189,
   17979, 18023, -1156, -1156, 19388,   754,     1,   737, -1156,  2395,
   -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156,  5155,
     -22, 17130, -1156, -1156, 19388,  5155, 19388, -1156, 18068, -1156,
   -1156, 12121, -1156,   130, 15338, 12121, -1156, 11250,  7253,  7253,
   -1156, 12121,   486, 12121,   510,   531, 12121, 10035,   538,   327,
   -1156, -1156, -1156, 18175,   765,   761,   758, -1156, 20189,  3189,
   -1156,  8645,   764,  1438, 20189, -1156, 20189, -1156, 19388, 20015,
     786, -1156, -1156, 20189,   565, -1156,  1954,   755,    20,  6905,
   -1156, -1156,   169, -1156,    20,   414, -1156, 18220, -1156, 15497,
   -1156, -1156, -1156,   374, -1156,   766,   749, -1156, -1156, -1156,
   -1156, -1156, -1156, -1156, -1156, 12121,   882, 18260, 12121,   897,
   18412,   273,   762, 18451,  8645, -1156, -1156,  8297, -1156, -1156,
   -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156,
      50, 10383, 10383,    39,    39, -1156, -1156, -1156, -1156, -1156,
   16750, -1156, 15656,  5680, -1156,  5855, -1156,   274, -1156, 15732,
    6030, -1156,    39,  6205, -1156,    39, -1156,    39, -1156,    39,
   19388, 19388,  5155, -1156,  5155, 12121, -1156,  5155,   901,   767,
     775, 20189,   533, -1156, -1156,   695,   770,    69, -1156, -1156,
   -1156,   778,   780, -1156,  6380, 10731, -1156, -1156, -1156, 20189,
   -1156,   801,   533, -1156,  6555,   779,  6730,   781, -1156, 12121,
   -1156, -1156,  7253, -1156, 18495,    55, 17221,   383,   790,  7253,
   -1156, 12121, -1156, -1156,  1875,   393,   416,   278, -1156,   949,
   -1156, -1156,   950, -1156, 18416, -1156, -1156, -1156, -1156, -1156,
   -1156, -1156,   755, -1156,   292, 11425,    -1,    62, 12121, -1156,
     784, 14197,    82, -1156,   787, -1156,   820,   821,   689,   689,
   -1156, -1156, 18603, 13159, 13332, 18648, 13505, 13678, -1156, 13851,
   14024,   302, -1156,   695,   330,   352, -1156, -1156, -1156,  5155,
    9164, 19388,  9164, 19388,  8297, -1156,  5155,  9164, 19388, -1156,
    9164, 19388, -1156, -1156, -1156, -1156, -1156, 19388,   928,  5155,
   -1156, -1156, -1156, -1156, -1156, -1156, 10383, 15124,  8645,  8645,
    5155, -1156,    38,   791, 12121, -1156,  9164, -1156, 19388,  5155,
    9164, -1156, 19388,  5155, 19388,   151, 11598,  7253,  7253,  7253,
    7253, -1156, -1156, -1156, 18688, 19388,  2271, -1156, -1156, -1156,
     800, -1156, 19746, -1156, 19746, -1156, -1156,    -1,   755, 10210,
   -1156, -1156, -1156,   355, 10731, -1156, -1156, -1156,   131, -1156,
     -10, -1156,   696, 18807,    -3, -1156, -1156,   396, 14670,   148,
   -1156, -1156, -1156, 12121, 12121, 12121, 12121, 12121, 12121, 12121,
   12121, -1156, -1156, 17871, 15808, 15884, -1156, 17871, 15960, 16036,
    5155, -1156, 18847, -1156,   738,   794,   795,   604, -1156,   533,
   19388, 16112, -1156, 16188, -1156, -1156, -1156, 19388,   418,   804,
     424,   805, 12121, -1156, -1156, 19746, -1156, 19746, -1156, -1156,
   -1156,    -1, 11773,    84, -1156, 19388, -1156, 12121,    62,   131,
     131,   131,   131,   131,   131,    62, -1156, -1156, -1156, -1156,
     503, -1156, -1156, -1156, 18892, 18999, 19044, 19151, 19196, 19303,
    5155,  5155,  5155,  5155, -1156, 14024,   807,   808, -1156,  5155,
    5155, -1156, -1156, -1156, -1156, 19388, -1156, -1156, -1156, -1156,
   -1156, -1156, -1156,   423, 10383, 14811, 19388, -1156, -1156, -1156,
   -1156, -1156, -1156, -1156,   828, 12121, 12121, 12121, 12121, 12121,
   12121, 17871, 17871, 17871, 17871,   744, -1156, -1156, 17871, 17871,
   19348, -1156, -1156, 11948, -1156
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   120,   677,   678,   679,   673,
     674,   680,     0,   599,   106,   141,   569,   148,   571,   599,
       0,   147,   477,   475,   106,     0,     0,    47,     0,   271,
     142,   189,   188,   671,     0,     0,     0,   187,     0,   146,
      59,   272,   343,   143,     0,     0,     0,   338,     0,     0,
     150,     0,     0,   616,   591,   681,   151,     0,   344,   563,
     472,     0,     0,     0,   174,   172,   145,   572,   474,     0,
       0,     0,     0,   567,     0,     0,   149,     0,     0,   121,
       0,   672,     0,     0,   464,   144,   309,   565,    53,   476,
     152,     0,     0,   730,     0,   732,     0,   733,   734,   643,
       0,   731,   728,   550,   170,   729,     0,     0,     0,     0,
       4,     0,     5,     9,    10,    44,    11,     0,    49,    62,
      12,    68,    13,    14,    15,    16,    28,   546,   547,    22,
      50,   171,   181,     0,   190,   644,   182,    17,    30,    29,
      19,     0,   266,    18,   635,    21,    34,    31,    32,   180,
     310,     0,   178,     0,     0,   632,   342,     0,   176,   345,
     434,   425,   179,     0,     0,   177,   649,   628,   548,   629,
     553,   551,     0,     0,   633,   634,     0,   552,     0,   650,
     651,   652,   675,   676,   627,   555,   554,   630,   631,     0,
      27,   571,   142,     0,     0,     0,     0,   572,     0,     0,
       0,     0,   632,   649,   551,   633,   634,   561,   552,   650,
     651,     0,     0,   600,   107,     0,   570,     0,   599,   465,
       0,   473,     0,    20,     0,   531,    45,   341,     0,   538,
     114,   122,   134,   128,   127,   136,   117,   126,   137,   123,
     138,   115,   139,   132,   125,   133,   131,   129,   130,   116,
     118,   124,   135,   140,     0,   119,   198,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    61,
      60,    62,   538,     0,     0,     0,     0,     0,     0,   480,
       0,   158,    39,     0,   301,     0,   300,   715,   620,   617,
     618,   619,     0,   564,   716,     7,   538,   341,   341,   446,
     528,     0,   527,     0,     0,   159,   648,     0,     0,    42,
       0,   568,   556,     0,   538,    43,   562,     0,   279,   283,
     280,   283,   566,    51,   341,     0,     0,     0,   717,   719,
     641,   714,   713,     0,    65,    69,     0,     0,   533,     0,
     535,     0,     0,   534,     0,     0,   527,     0,   642,     0,
       6,     0,     0,    63,     0,     0,   477,   186,     0,   673,
     310,   645,   194,     0,     0,     0,   306,     0,   330,   334,
     336,     0,   320,   324,   327,   414,   434,   432,   433,   431,
     353,   435,   438,   437,   439,     0,   429,   426,   427,   430,
       0,   469,     0,   466,     0,   549,    33,   621,     0,     0,
       0,     0,     0,     0,   718,     0,     0,     0,     0,     0,
       0,   640,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   384,   391,
     392,   393,   388,   390,     0,     0,   386,   389,   387,   385,
       0,   395,   394,     0,     0,   538,   538,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    35,    23,     0,    36,
       0,     0,     0,    24,     0,    37,   341,   307,     0,   188,
     308,    48,    62,     0,   546,   544,     0,   539,   540,   545,
     319,     0,     0,   208,     0,     0,   207,     0,   216,     0,
       0,     0,   214,     0,   224,     0,     0,   222,     0,     0,
       0,     0,   240,     0,   384,   236,     0,     0,     0,     0,
       0,   250,    25,   484,     0,   485,   487,     0,   509,     0,
     490,     0,     0,   157,    38,     0,   303,     0,     0,     0,
      40,     0,   175,   173,     0,    99,   646,   647,   160,     0,
      41,     0,     0,   290,   281,   282,   341,    54,   201,   200,
      26,    67,    66,    70,     0,   682,     0,     0,   667,     0,
     669,     0,     0,     0,     0,     0,     0,     0,     0,   686,
       8,     0,    56,     0,    97,     0,    93,     0,    77,     0,
       0,    83,   183,   310,     0,   196,     0,   193,   277,   311,
       0,   318,   328,     0,   332,     0,     0,   322,   317,   421,
     416,   417,   418,   422,   423,   424,   414,   407,     0,   404,
       0,   415,     0,   436,   504,     0,   440,   428,   401,   108,
     380,   122,   378,   128,   127,   111,   126,   123,   382,   138,
     109,   139,   125,   129,   110,   112,   124,   140,   377,   359,
     362,   360,   361,   383,   372,   363,   376,   368,   366,   379,
     367,   365,   370,   375,   364,   369,   373,   374,   371,   381,
       0,   356,     0,   113,     0,   401,   357,   401,   354,     0,
     468,     0,   463,   479,   710,   709,   712,   721,   720,   725,
     724,   706,   703,   704,   705,   637,   693,   120,     0,   663,
     664,   121,   662,   661,   638,   697,   708,   702,   700,   711,
     701,   699,   691,   696,   698,   707,   690,   694,   695,   692,
     639,     0,     0,     0,     0,     0,     0,     0,     0,   723,
     722,   727,   726,     0,     0,     0,     0,     0,     0,     0,
     684,   277,   610,   611,   613,   615,     0,   602,     0,     0,
       0,   599,   599,   204,     0,   532,    46,     0,     0,   558,
       0,     0,     0,     0,   575,     0,     0,     0,   217,     0,
     581,     0,     0,   215,   225,     0,     0,   223,     0,     0,
     239,     0,   235,     0,     0,     0,     0,   557,     0,   253,
       0,   251,     0,   488,     0,   155,   156,   154,   153,     0,
     508,   507,   628,     0,   482,   626,   481,   302,   299,     0,
       0,     0,   660,   530,   529,     0,     0,     0,   559,     0,
     284,    52,   683,   636,   668,   536,   670,   537,   232,     0,
       0,     0,   685,   230,   585,     0,   688,   687,     0,    58,
      57,     0,    92,     0,     0,     0,    85,     0,     0,    83,
      55,   377,   359,   362,   360,   361,   370,   369,   371,     0,
     396,   397,    72,    71,    84,     0,     0,   312,     0,     0,
     274,     0,     0,   341,     0,   331,     0,   335,   337,     0,
       0,   419,   420,     0,   414,   403,     0,   522,   447,     0,
     503,   502,   628,   441,   447,   414,   352,     0,   358,     0,
     348,   349,   471,   628,   467,     0,     0,   105,   103,   104,
     102,   101,   100,   658,   659,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   612,   614,     0,   601,   141,
     148,   147,   146,   143,   150,   151,   145,   149,   144,   152,
       0,     0,     0,     0,     0,   478,   199,   542,   543,   541,
       0,   234,     0,     0,   206,     0,   205,     0,   605,     0,
       0,   212,     0,     0,   210,     0,   220,     0,   218,     0,
     248,   247,     0,   242,     0,     0,   238,     0,   244,     0,
     276,     0,     0,   486,   489,   490,     0,     0,   491,   492,
     298,     0,     0,   292,     0,     0,   291,   294,   560,     0,
     285,   288,     0,   233,     0,     0,     0,     0,   231,     0,
      98,    95,     0,    94,    80,    79,    78,     0,     0,     0,
     184,     0,   185,   312,   341,     0,     0,     0,   305,   164,
     174,   172,   168,   304,   341,   313,   314,   161,   329,   333,
     323,   326,   522,   405,     0,     0,   490,     0,     0,   339,
       0,   494,     0,   346,     0,   355,   108,   110,   401,   401,
     665,   666,     0,     0,     0,     0,     0,     0,   276,     0,
       0,     0,   603,   490,   649,   649,   203,   202,   209,     0,
       0,   574,     0,   573,     0,   604,     0,     0,   580,   213,
       0,   579,   211,   221,   219,   241,   237,   590,   243,     0,
      64,   273,   252,   249,   483,   513,     0,   684,   538,   538,
       0,   296,     0,     0,     0,   286,     0,   228,   587,     0,
       0,   226,   586,     0,   689,     0,     0,     0,    83,     0,
      83,    86,    89,    75,    74,    73,   341,   191,   197,   195,
     278,   162,   341,   166,   341,   315,   316,   490,   522,     0,
     525,   524,   526,   628,   398,   410,   408,   461,     0,   462,
     449,   452,     0,   448,     0,   499,   497,   628,   684,     0,
     402,   350,   351,     0,     0,     0,     0,     0,     0,     0,
       0,   278,   607,   578,     0,     0,   606,   584,     0,     0,
       0,   246,     0,   511,   628,     0,     0,   297,   295,     0,
     289,     0,   229,     0,   227,    96,    82,    81,     0,     0,
       0,     0,     0,   192,   275,   341,   165,   341,   169,   411,
     409,   490,   514,     0,   400,   399,   460,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,   445,   340,   498,
     628,   442,   347,   443,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   245,     0,   622,   623,   287,     0,
       0,    88,    91,    87,    90,    76,   163,   167,   413,   412,
     516,   517,   519,   628,     0,   684,   451,   453,   454,   457,
     458,   459,   455,   456,   450,     0,     0,     0,     0,     0,
       0,   577,   576,   583,   582,   628,   624,   625,   589,   588,
       0,   518,   520,   514,   521
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1156, -1156, -1156,     5,  -374,  2375,   578, -1156, -1156, -1156,
    -202, -1156, -1156, -1156, -1156, -1156,   -28,   706, -1156,  -699,
    2561,   625,  -560, -1156,  -812, -1156, -1156,   147, -1156, -1156,
   -1156,   955, -1156,  2731,  -156,  -377, -1156,  -605,  2643,  -561,
     -44, -1156, -1156,   -43, -1156, -1156,  -778, -1156, -1156,   392,
   -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156,   203,
   -1156,   892, -1156, -1156,    14,  2180, -1156, -1156, -1156, -1156,
     667, -1156,     0, -1156, -1156, -1156, -1156,   466, -1156, -1156,
   -1156,  -116, -1156,  -331,  -801, -1156, -1156, -1156, -1156, -1156,
     117, -1156, -1156,   123, -1156, -1156,  -559, -1156,   598, -1156,
   -1156, -1156, -1156,   632, -1156, -1156, -1156, -1156,   106,  -369,
    -162,  -784, -1034,  -632, -1156, -1156,   120,   126,   400, -1156,
   -1156, -1156,   626, -1156, -1156,  -152,    95,   134,  -215,  -204,
    -426, -1156, -1156,   161,   349, -1156,  -167,   891, -1156,  -161,
     506,   242,  -496,  -932,  -987, -1156,  -595,  -653, -1155,  -980,
    -990,   -64, -1156,   143, -1156,  -266,  -477,  -500,   508,  -491,
   -1156, -1156, -1156,   840, -1156,    -8, -1156, -1156,  -150, -1156,
    -719, -1156, -1156,  1167,  1374,   -12, -1156,   -61,  1608, -1156,
    1743,  1948, -1156, -1156, -1156, -1156, -1156, -1156, -1156, -1156,
   -1156,  -453
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   335,   111,   772,   113,   114,   466,   227,
     115,   116,   546,   324,   117,   118,   119,   355,   120,   121,
     122,   336,   854,   577,   855,   123,   124,   574,   575,   125,
     126,   215,   661,   370,   127,   255,   128,   790,   282,   129,
    1025,  1205,  1132,  1026,  1207,  1134,   130,   298,   297,   131,
     132,   133,   134,   135,   136,   586,   858,   137,   138,   912,
     139,   267,   140,   780,   781,   201,   142,   143,   144,   145,
     544,   810,   992,   146,   147,   806,   987,   284,   285,   148,
     149,   150,   151,   365,   863,   152,   153,   371,   869,   870,
     372,   864,   593,   373,   866,   595,   374,   154,   202,  1154,
     156,   157,   158,   159,  1159,   160,   161,   664,   665,   850,
     851,   852,  1145,   886,   376,   607,   608,   609,   610,   611,
     390,   380,   385,   884,  1232,  1228,   475,  1039,  1150,  1151,
    1152,   162,   163,   392,   393,   670,   164,   165,   220,   278,
     279,   516,   517,   794,   881,   616,   520,   791,  1261,  1142,
    1036,   337,   224,   341,   342,   476,   477,   478,   203,   167,
     168,   169,   170,   204,   172,   212,   213,   736,   489,   947,
     737,   738,   173,   205,   206,   176,   362,   479,   208,   178,
     209,   210,   181,   182,   183,   184,   347,   185,   186,   187,
     188,   189
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     207,   606,   740,   394,   216,   301,   508,   110,   218,   307,
     880,   217,   271,   663,   225,   444,   892,   878,   782,   360,
     849,   666,   260,   262,   264,   471,   268,  1008,   792,   584,
     531,   327,   778,   890,   865,   891,   867,   283,   948,   286,
     287,   807,  1137,  1094,   345,   292,   343,   293,   542,   294,
    1003,   518,   805,   462,  1156,   614,  1014,   302,   306,   308,
     310,   311,   312,   729,   521,  1147,   316,   280,   317,   950,
     280,   280,   280,   474,   943,   322,   984,  -271,   467,   326,
     306,  1037,   328,   501,   329,  1027,     3,   330,   331,    70,
     332,   487,  1117,   563,   302,   306,   346,   348,  -272,   487,
     518,   871,   518,  1209,  1146,   915,   506,   568,   190,  1217,
    1291,   464,   493,   495,   498,  -500,   474,  1096,   554,   985,
     860,   358,   547,   361,   882,   793,  -293,   861,  1118,   465,
     729,  1172,  1264,   687,  1147,   569,   453,  1218,  1294,  1038,
     474,  -254,   555,   872,   470,   561,  -500,   916,  1211,  -505,
    1144,   458,   211,   994,  1148,   487,   534,   487,   474,  -500,
    -505,   397,   564,   522,   300,  1149,   487,   458,    86,  -293,
     214,  1229,   986,  -500,   535,   288,  -505,  1258,   893,   717,
     718,   449,   450,   451,   525,   534,   316,   302,   346,   975,
     730,  -505,   564,   229,   562,   566,   519,  -505,  1062,   472,
     615,   300,   340,   300,   458,  1210,   458,   458,   458,   458,
     461,   458,  1126,   404,   272,   400,   564,   306,  -505,  -500,
     565,   410,  -293,  -505,  -500,  1123,  -255,  -505,   667,   564,
    -501,   564,  1262,   691,  1149,   289,  1027,   259,   261,   263,
      70,   557,  -325,   583,  1097,   519,  1027,   519,  -505,   -61,
     939,   911,   856,   290,   693,   400,   559,  1158,    23,  1265,
     306,  -501,   597,   381,   746,   352,  -505,   558,   -61,  -505,
     291,   783,   401,  -114,  -501,  -321,   402,   269,  1007,  1259,
     270,  -120,   560,   917,   306,  1292,   598,   -61,  -501,   474,
     474,   536,   537,   534,   300,   300,   472,   784,  1009,  -321,
     589,   744,   306,   353,   740,  1028,  1199,  1029,  1201,   918,
      60,   344,   401,  1231,  -261,   382,   402,   -61,   458,   941,
     750,    68,  1195,  -260,   404,   405,   383,   556,   407,   408,
    -116,   409,   410,   354,  -501,  1063,   413,   377,  -121,  -501,
     576,   553,   578,   420,   811,   384,   970,    89,  1027,   424,
     425,   426,   302,   474,  1206,  1176,  1208,   378,   447,   671,
     799,  -596,   448,   522,   404,   190,   971,   379,  -656,   408,
    -656,   551,   410,   712,   713,   219,   413,   672,   221,   714,
    1034,   673,   715,   716,  1017,   474,   674,   675,   676,   678,
     680,   681,   972,   682,   683,   684,   685,   686,   783,   694,
     695,   696,   697,   698,   699,   700,   701,   702,   703,   704,
     705,   706,   707,   708,   709,   710,  1161,  1162,   752,   859,
     750,  1074,   306,   306,   973,   750,   229,  1256,   306,  1257,
    1140,   306,   306,   306,   306,   720,   722,  1061,   472,   783,
     831,   599,  1115,   739,  1183,   226,  1058,  1075,   742,   750,
     835,  1130,   745,   256,   687,  1226,   386,  -493,   832,   300,
    1226,   747,   600,  -506,   740,  1138,   601,   -60,   836,   522,
     302,   688,   754,   756,  1009,  1171,  -506,  -609,   760,   762,
     445,  -523,   446,  -269,   766,   689,   -60,   935,  -493,  1024,
     602,   773,  1010,   603,   775,    62,   343,   306,   343,  -608,
     606,  -493,  -523,  -609,   604,   -60,  -523,  -506,   387,   795,
     166,   606,   663,   286,   690,  -493,   800,   801,   472,   388,
     666,  -470,   804,   605,  -259,  -608,   190,   932,  -523,   306,
    1009,   448,   829,   166,  -506,   -60,   166,   272,   389,  -470,
     671,  -506,   804,  -256,  1143,   306,  -263,   306,  1121,  -515,
    1157,   821,   804,   824,   691,   826,   828,  1198,  1128,  1200,
     692,  -493,  -506,   522,  -495,  1009,  -493,   295,   853,   853,
    -515,  1009,   474,  -267,  -515,  -732,   599,   300,   553,  -732,
    -506,  1129,  -264,  1251,   868,  -506,   166,  -406,  -506,  1253,
     166,   166,   599,  1227,   474,  -495,  -515,   600,  1227,  -733,
     155,   601,   296,  -733,   677,   679,  1184,  -657,  -495,  -657,
    -258,   952,   955,   600,   166,   957,   959,   601,   313,   166,
    -734,   740,  -495,   155,  -734,   602,   155,  -729,   603,   803,
     349,  -729,  -262,  -656,   400,  -656,  -653,   314,  -653,   604,
    1024,   602,  -268,   458,   603,   510,    62,  -270,   887,   803,
     719,   721,   340,  -257,   340,   604,   323,   404,   605,   803,
     318,   320,   408,   741,  -655,   410,  -655,  1230,  -495,   413,
     995,   997,  -265,  -495,   605,   351,   155,  1001,   693,   363,
     155,   155,   364,  -654,   895,  -654,   896,   938,   755,   532,
     533,   401,   400,   375,   761,   402,   454,  -593,  1016,   765,
     815,   395,   817,   458,   155,   474,  -592,   396,  -597,   155,
    -598,   754,   907,   760,   910,   773,   306,   913,   824,   455,
     166,  1263,  1216,  -595,   473,   977,  -594,  1049,   457,   463,
     480,   353,   979,   933,   934,   570,   306,   538,   306,   940,
     482,   942,   509,   404,   405,   739,   528,   949,   408,   401,
     409,   410,  1024,   402,  1285,   413,   960,   961,   474,   529,
     541,   550,   420,   579,   590,   592,   820,   594,   424,   425,
     426,   591,   613,   596,  1263,   312,   316,   346,   617,   669,
     506,   306,   410,   166,   711,   728,  -510,   506,   166,   748,
     749,   750,  -512,  1268,  1269,  1270,  1271,  1272,  1273,  -510,
     751,   404,  1263,   757,   768,  -512,   408,   769,   409,   410,
     155,   771,   774,   413,    42,  1042,   776,  -341,   777,   576,
     420,   793,  1219,  1004,   802,  1006,   853,   853,   426,   328,
    -510,   329,  1185,  1186,   331,   332,  -512,  -341,   808,   809,
      58,   819,   171,   937,   166,   825,   857,  -341,  1220,   306,
    1221,  1222,   534,  1223,  1224,   874,   875,  -510,   883,   166,
    -141,   885,  -148,  -512,  -510,   171,  -147,   346,   171,  -117,
    -512,  -146,   585,   155,  -143,  -150,  -115,  -118,   155,  -151,
    -145,  -149,  -144,  -152,   300,  -510,   888,   400,  -119,   889,
     914,  -512,   903,  1052,   897,   904,  1055,   962,   668,   964,
     369,  1035,   306,  -510,   898,   739,   899,   900,  -510,  -512,
     988,  -510,  1009,   901,  -512,  1053,   902,  -512,   171,   678,
     720,  1102,   171,   171,  1051,   983,  1012,   906,  1013,   909,
    1056,  1071,  1090,  1073,   155,  1018,  1089,  1059,  1078,  1050,
    1091,  1081,   474,   474,   401,  1095,   171,  1104,   402,   155,
    1098,   171,  1099,  1087,  1109,  1122,  1113,  1131,  1133,  1155,
    1160,  -114,  -116,  1180,  1189,  1204,   166,  1246,  1247,  1252,
    1254,  1286,  1287,   302,   300,  1218,   573,   507,  1000,   222,
    1135,  1136,  1108,   325,  1112,  1092,  1030,  1114,   545,  1103,
     853,   797,   166,  1031,   587,  1048,   404,  1124,   166,  1125,
    1033,   408,   612,   409,   410,  1044,   873,  1233,   413,   166,
    1274,   166,   506,   506,  1267,   420,   506,   506,  1043,  1015,
     894,   424,   425,   426,   357,   974,  1153,     0,   796,   506,
       0,   506,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1071,  1073,     0,  1078,  1081,     0,  1108,  1112,     0,
       0,     0,   171,     0,     0,     0,   155,  1173,  1174,     0,
    1175,     0,   739,     0,  1177,  1178,     0,     0,  1179,   166,
       0,     0,   400,   166,     0,  1213,     0,     0,     0,     0,
     300,   166,   155,     0,  1182,   824,   306,   306,   155,     0,
       0,     0,  1190,     0,  1191,     0,     0,     0,  1193,   155,
       0,   155,     0,     0,  1197,   853,   853,   853,   853,     0,
       0,     0,     0,     0,     0,   171,     0,     0,     0,     0,
     171,     0,     0,     0,     0,     0,     0,   346,     0,   401,
    1141,     0,  1215,   402,   677,   719,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   824,     0,     0,     0,
       0,  1173,  1234,  1235,  1177,  1236,  1237,  1238,  1239,   155,
       0,     0,     0,   155,     0,     0,     0,     0,     0,   174,
       0,   155,     0,     0,     0,     0,   171,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
    1255,   171,   174,   413,     0,   174,     0,     0,     0,   419,
     420,     0,     0,   423,     0,  1266,   424,   425,   426,     0,
       0,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,     0,     0,  1281,  1282,
    1283,  1284,     0,  1112,   300,     0,     0,  1288,  1289,  1214,
       0,     0,     0,     0,     0,   174,   930,     0,     0,   174,
     174,     0,  1290,   824,   443,     0,     0,  -653,     0,  -653,
       0,     0,   862,  1281,  1282,  1283,  1284,  1288,  1289,     0,
       0,     0,     0,   174,     0,     0,     0,     0,   174,   166,
       0,  1112,   166,  -312,     0,     0,     0,  -312,  -312,     0,
       0,     0,  -312,     0,     0,     0,   687,  -312,   171,  -312,
    -312,     0,     0,     0,     0,  -312,     0,  1260,     0,     0,
     919,     0,  -312,   920,     0,  -312,     0,     0,   921,     0,
       0,     0,     0,     0,   171,     0,     0,   166,     0,   192,
     171,     0,     0,   166,     0,  -312,     0,     0,  -312,   922,
    -312,   171,  -312,   171,  -312,  -312,   923,  -312,     0,  -312,
       0,  -312,     0,     0,     0,     0,   924,     0,     0,     0,
    1260,     0,     0,     0,   925,     0,     0,     0,     0,   155,
    -312,     0,   155,  -312,     0,  -312,   175,  -312,     0,   174,
     926,     0,     0,     0,     0,     0,     0,     0,  1260,     0,
       0,     0,   927,     0,     0,     0,   691,     0,     0,   175,
       0,   171,   175,   928,     0,   171,     0,     0,     0,     0,
     929,     0,     0,   171,     0,     0,     0,   155,     0,     0,
       0,     0,     0,   155,     0,     0,  -312,     0,     0,     0,
       0,     0,  -312,     0,     0,     0,     0,     0,     0,  1064,
    1065,     0,   174,     0,     0,     0,     0,   174,     0,     0,
       0,   166,   175,   166,     0,     0,   175,   175,   166,     0,
     467,   166,     0,     0,    22,    23,     0,     0,     0,  1019,
     166,     0,   166,     0,   468,   166,    31,   469,     0,     0,
     175,     0,    37,     0,     0,   175,     0,     0,     0,    42,
       0,     0,   166,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   166,   174,   166,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,    58,     0,    60,   174,    62,
       0,  1020,     0,     0,  1021,     0,   470,     0,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   155,     0,   155,     0,     0,     0,    84,   155,     0,
      86,   155,  1022,     0,    89,     0,     0,     0,     0,     0,
     155,     0,   155,     0,     0,   155,     0,   401,     0,     0,
       0,   402,     0,     0,     0,     0,     0,   166,     0,     0,
       0,     0,   155,     0,   166,     0,   175,     0,     0,     0,
       0,     0,   155,     0,   155,     0,     0,   166,     0,     0,
       0,     0,     0,   104,     0,     0,     0,     0,   166,  1023,
     177,   171,     0,     0,   171,     0,     0,   166,     0,   404,
     405,   166,   406,   407,   408,   174,   409,   410,   411,     0,
       0,   413,     0,   177,     0,     0,   177,   419,   420,     0,
       0,   423,     0,     0,   424,   425,   426,     0,     0,   175,
       0,   174,     0,     0,   175,   427,     0,   174,     0,   171,
     400,     0,     0,     0,     0,   171,     0,   155,   174,     0,
     174,     0,     0,     0,   155,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   177,   155,   166,     0,
     177,   177,     0,     0,     0,     0,     0,     0,   155,     0,
       0,     0,     0,     0,     0,     0,     0,   155,     0,     0,
     175,   155,     0,     0,   177,     0,     0,   401,     0,   177,
       0,   402,     0,     0,     0,   175,     0,     0,   174,     0,
       0,     0,   174,     0,     0,     0,     0,     0,     0,     0,
     174,     0,     0,     0,     0,   179,     0,     0,   166,   166,
     166,   166,     0,     0,     0,     0,     0,   166,   166,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   179,   404,
     405,   179,   406,   407,   408,     0,   409,   410,   155,     0,
       0,   413,     0,   171,     0,   171,     0,     0,   420,     0,
     171,     0,     0,   171,   424,   425,   426,     0,     0,     0,
       0,     0,   171,     0,   171,     0,     0,   171,     0,     0,
       0,     0,     0,   467,     0,     0,     0,    22,    23,     0,
     177,   179,     0,     0,   171,   179,   179,   468,     0,    31,
     469,     0,   175,     0,   171,    37,   171,     0,   155,   155,
     155,   155,    42,     0,     0,     0,     0,   155,   155,   179,
       0,     0,     0,     0,   179,     0,     0,     0,   175,     0,
       0,     0,     0,     0,   175,     0,     0,     0,    58,     0,
      60,     0,     0,     0,     0,   175,     0,   175,     0,   470,
       0,    68,     0,   177,     0,     0,     0,     0,   177,     0,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
      84,    22,    23,    86,     0,     0,  1019,    89,     0,   171,
       0,   468,     0,    31,   469,     0,   171,     0,     0,    37,
       0,     0,     0,     0,     0,     0,    42,     0,     0,   171,
       0,     0,     0,     0,     0,   175,     0,     0,   174,   175,
     171,   174,     0,     0,   177,     0,     0,   175,     0,   171,
     180,     0,    58,   171,    60,   179,    62,   230,  1020,   177,
       0,  1021,     0,   470,     0,    68,     0,     0,     0,     0,
       0,   231,   232,   180,   233,     0,   180,     0,     0,   234,
       0,     0,     0,     0,    84,     0,   174,    86,   235,  1022,
       0,    89,   174,     0,   236,     0,     0,     0,     0,     0,
     237,     0,     0,     0,   238,     0,     0,   239,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   240,   179,     0,
     171,     0,     0,   179,   241,   242,   180,     0,     0,     0,
     180,   180,   243,     0,     0,     0,     0,     0,     0,     0,
     104,   244,     0,     0,     0,     0,  1127,     0,     0,     0,
     245,   246,     0,   247,   180,   248,     0,   249,     0,   180,
     250,     0,     0,     0,   251,   513,   177,   252,     0,     0,
       0,   253,     0,     0,     0,     0,     0,     0,     0,   179,
     171,   171,   171,   171,     0,     0,     0,     0,     0,   171,
     171,     0,   177,     0,   179,     0,     0,     0,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   177,
     174,   177,   174,     0,     0,     0,     0,   174,     0,     0,
     174,     0,     0,     0,     0,     0,   514,     0,     0,   174,
       0,   174,     0,     0,   174,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,     0,     0,   175,     0,
       0,   174,     0,     0,     0,     0,     0,     0,     0,     0,
     180,   174,     0,   174,     0,     0,     0,     0,     0,   177,
       0,     0,     0,   177,     0,     0,     0,     0,     0,     0,
       0,   177,   141,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   175,     0,     0,     0,     0,     0,   175,
       0,   179,     0,     0,     0,   141,     0,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,     0,     0,     0,   179,   180,     0,
       0,     0,     0,   179,     0,     0,   174,     0,     0,     0,
       0,     0,     0,   174,   179,     0,   179,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   174,     0,   141,     0,
       0,     0,     0,     0,     0,     0,     0,   174,     0,     0,
       0,     0,     0,     0,     0,     0,   174,     0,     0,     0,
     174,     0,     0,     0,   180,     0,   141,     0,     0,     0,
       0,   141,     0,   467,     0,     0,     0,    22,    23,   180,
       0,     0,  1019,     0,   179,     0,     0,   468,   179,    31,
     469,     0,     0,     0,     0,    37,   179,   175,     0,   175,
       0,     0,    42,     0,   175,     0,     0,   175,     0,     0,
       0,     0,     0,     0,     0,     0,   175,     0,   175,     0,
       0,   175,     0,     0,     0,     0,     0,   174,    58,     0,
      60,     0,    62,     0,  1020,     0,     0,  1021,   175,   470,
       0,    68,     0,     0,     0,     0,     0,     0,   175,     0,
     175,     0,     0,     0,     0,     0,     0,   112,     0,   177,
      84,     0,   177,    86,     0,  1022,     0,    89,     0,     0,
       0,     0,   141,     0,     0,     0,     0,     0,   230,     0,
     223,     0,     0,   228,     0,     0,   180,   174,   174,   174,
     174,     0,   231,   232,     0,   233,   174,   174,     0,     0,
     234,     0,     0,     0,     0,     0,     0,   177,     0,   235,
       0,     0,   180,   177,     0,   236,   104,     0,   180,     0,
       0,   237,  1203,   175,     0,   238,     0,     0,   239,   180,
     175,   180,     0,   315,     0,   141,     0,     0,   240,     0,
     141,     0,     0,   175,     0,   241,   242,     0,     0,     0,
       0,     0,     0,   243,   175,     0,     0,     0,     0,     0,
       0,   112,   244,   175,     0,     0,   350,   175,     0,     0,
       0,   245,   246,     0,   247,     0,   248,     0,   249,     0,
       0,   250,     0,     0,     0,   251,     0,     0,   252,   180,
       0,     0,   253,   180,   179,     0,   141,   179,     0,     0,
       0,   180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   141,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   177,     0,   177,   175,     0,     0,     0,   177,     0,
       0,   177,   179,     0,     0,   458,     0,   989,   179,     0,
     177,     0,   177,     0,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   456,     0,     0,
       0,     0,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   177,     0,   177,     0,     0,     0,     0,     0,
     281,     0,     0,     0,   175,   175,   175,   175,     0,     0,
       0,     0,     0,   175,   175,     0,     0,     0,     0,     0,
       0,     0,     0,   281,     0,     0,     0,     0,   141,     0,
       0,     0,     0,   319,   321,     0,     0,     0,     0,     0,
     512,     0,     0,   398,     0,   523,     0,     0,     0,     0,
     399,     0,     0,     0,   141,     0,     0,     0,     0,     0,
     141,     0,     0,   400,   490,     0,     0,   177,     0,     0,
       0,   141,     0,   141,   177,     0,   179,     0,   179,   491,
       0,     0,     0,   179,     0,     0,   179,   177,     0,     0,
       0,     0,     0,     0,     0,   179,     0,   179,   177,     0,
     179,   112,     0,     0,     0,   309,     0,   177,     0,   180,
       0,   177,   180,     0,     0,     0,   112,   179,     0,     0,
     401,     0,     0,     0,   402,     0,     0,   179,     0,   179,
       0,   141,     0,     0,     0,   141,     0,     0,     0,     0,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
       0,   254,   487,     0,     0,     0,   403,   180,     0,     0,
       0,     0,     0,   180,     0,   273,   274,   275,   459,   277,
       0,     0,   404,   405,     0,   406,   407,   408,   177,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,   458,   179,     0,     0,     0,     0,     0,   427,   179,
     281,   281,   281,   281,   281,   281,     0,     0,   502,   505,
       0,     0,   179,   112,     0,   511,     0,     0,     0,     0,
       0,     0,     0,   179,   281,     0,   281,     0,   177,   177,
     177,   177,   179,   281,     0,     0,   179,   177,   177,   523,
       0,     0,     0,     0,     0,   523,     0,     0,     0,     0,
       0,   281,     0,     0,     0,     0,   770,     0,     0,     0,
       0,     0,   366,     0,     0,     0,   281,   281,     0,     0,
       0,   180,     0,   180,   391,   277,     0,     0,   180,     0,
       0,   180,   483,   486,   488,   492,   494,   497,     0,     0,
     180,     0,   180,     0,     0,   180,     0,     0,     0,     0,
       0,     0,     0,   179,     0,     0,   524,   452,   526,     0,
       0,     0,   180,     0,     0,   530,   818,     0,     0,     0,
     823,     0,   180,     0,   180,     0,     0,     0,   112,     0,
       0,   141,     0,   540,   141,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,   548,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   179,   179,   179,   179,     0,   481,     0,
       0,     0,   179,   179,     0,     0,   499,   500,     0,   141,
       0,     0,     0,     0,     0,   141,     0,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,     0,   401,
       0,     0,   281,   402,   180,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   180,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   180,     0,
     281,     0,     0,     0,   281,   403,   281,   180,     0,   281,
       0,   180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,     0,   417,   418,   419,
     420,   421,   422,   423,   588,     0,   424,   425,   426,     0,
       0,     0,     0,     0,   743,     0,     0,   427,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   662,     0,   141,     0,   141,     0,     0,   180,     0,
     141,     0,   758,   141,     0,     0,   763,     0,   764,     0,
       0,   767,   141,     0,   141,     0,   963,   141,     0,   966,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   141,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   141,     0,   141,     0,     0,     0,
       0,     0,     0,     0,     0,   731,     0,     0,   180,   180,
     180,   180,   230,   277,   993,     0,     0,   180,   180,   254,
     998,     0,     0,     0,     0,     0,   231,   232,     0,   233,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
       0,     0,     0,   235,     0,     0,     0,     0,     0,   236,
       0,     0,     0,     0,     0,   237,     0,     0,     0,   238,
       0,   779,   239,     0,     0,   515,     0,     0,     0,   141,
       0,     0,   240,   277,     0,     0,   141,     0,   798,   241,
     242,     0,     0,     0,     0,     0,     0,   243,     0,   141,
       0,     0,     0,     0,     0,     0,   244,     0,     0,     0,
     141,     0,     0,     0,     0,   245,   246,     0,   247,   141,
     248,     0,   249,   141,     0,   250,     0,     0,     0,   251,
       0,     0,   252,     0,     0,     0,   253,     0,     0,     0,
     840,     0,   398,     0,     0,     0,   281,   281,   523,   399,
     523,     0,   281,   281,     0,   523,   281,   281,   523,     0,
       0,     0,   400,   484,     0,     0,   968,  1085,     0,  1086,
       0,   877,  1088,     0,     0,     0,     0,     0,   485,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1101,
     141,   276,     0,     0,     0,     0,     0,     0,     0,  1107,
     990,  1111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,     0,     0,   402,     0,     0,     0,     0,   944,   946,
       0,     0,   391,     0,   951,   954,     0,     0,   956,   958,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     141,   141,   141,   141,     0,   403,     0,     0,     0,   141,
     141,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,  1181,     0,   424,   425,   426,     0,
     458,     0,     0,     0,     0,  1187,     0,   427,     0,     0,
       0,     0,     0,     0,  1192,     0,     0,     0,  1194,     0,
       0,     0,     0,     0,   281,   281,     0,     0,     0,     0,
       0,   281,     0,   505,     0,     0,     0,   969,     0,     0,
     505,     0,     0,   281,   515,     0,   281,     0,   281,     0,
     281,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     980,     0,     0,  1093,     0,     0,     0,     0,     0,     0,
     991,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1105,     0,  1244,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1066,  1067,     0,     0,
       0,     0,     0,  1068,     0,     0,     0,     0,     0,   391,
     277,     0,     0,     0,     0,  1079,     0,   230,  1082,     0,
    1083,     0,  1084,     0,  1032,     0,     0,   515,     0,     0,
       0,   231,   232,     0,   233,     0,     0,     0,     0,   234,
     662,     0,     0,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,     0,     0,
     237,     0,     0,     0,   238,     0,     0,   239,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   240,     0,     0,
       0,     0,     0,   281,   241,   242,     0,     0,     0,     0,
       0,     0,   243,     0,     0,     0,     0,     0,     0,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,     0,   247,     0,   248,     0,   249,     0,     0,
     250,     0,   779,     0,   251,     0,     0,   252,     0,     0,
       0,   253,     0,     0,     0,     0,     0,     0,     0,     0,
     991,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   505,   505,     0,     0,   505,
     505,     0,     0,     0,     0,  1188,     0,     0,     0,     0,
    1248,     0,   505,     0,   505,    -2,     4,     0,     5,     0,
       6,     7,     8,     9,    10,    11,   876,     0,     0,    12,
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
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
     104,     0,     0,   105,     0,   106,     0,   107,     0,   108,
       0,     4,   109,     5,     0,     6,     7,     8,     9,    10,
      11,     0,  -684,     0,    12,    13,    14,    15,    16,  -684,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,  -684,    28,    29,  -684,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -59,     0,    40,
      41,    42,     0,    43,  -341,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -59,    53,    54,     0,
      55,    56,    57,     0,  -341,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -341,   -59,    65,    66,    67,  -684,
      68,    69,    70,  -684,    71,    72,    73,    74,    75,    76,
      77,    78,     0,    79,    80,     0,    81,    82,    83,    84,
      85,     0,    86,    87,    88,   -59,    89,    90,     0,     0,
      91,     0,    92,     0,     0,  -684,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -684,  -684,    96,  -684,  -684,  -684,  -684,  -684,  -684,
    -684,     0,  -684,  -684,  -684,  -684,  -684,  -684,  -684,  -684,
    -684,  -684,  -684,  -684,  -684,   104,  -684,  -684,  -684,     0,
     106,  -684,   107,     0,   108,     0,   333,  -684,     5,   299,
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
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
     104,     0,     0,   105,     0,   106,   334,   107,     0,   108,
       0,     4,   109,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -59,     0,    40,
      41,    42,     0,    43,  -341,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -59,    53,    54,     0,
      55,    56,    57,     0,  -341,     0,     0,    58,    59,    60,
      61,    62,    63,    64,  -341,   -59,    65,    66,    67,     0,
      68,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,    78,     0,    79,    80,     0,    81,    82,    83,    84,
      85,     0,    86,    87,    88,   -59,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,   552,   107,     0,   108,     0,   571,   109,     5,     0,
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
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
     104,     0,     0,   105,     0,   106,   572,   107,     0,   108,
       0,   333,   109,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -59,     0,    40,
      41,    42,     0,    43,  -341,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -59,    53,    54,     0,
      55,    56,    57,     0,  -341,     0,     0,    58,    59,    60,
      61,    62,    63,    64,  -341,   -59,    65,    66,    67,     0,
      68,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,    78,     0,    79,    80,     0,    81,    82,    83,    84,
      85,     0,    86,    87,    88,   -59,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,   334,   107,     0,   108,     0,     4,   109,     5,     0,
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
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
     104,     0,     0,   105,     0,   106,   830,   107,     0,   108,
       0,     4,   109,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -59,     0,    40,
      41,    42,     0,    43,  -341,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -59,    53,    54,     0,
      55,    56,    57,     0,  -341,     0,     0,    58,    59,    60,
      61,   349,    63,    64,  -341,   -59,    65,    66,    67,     0,
      68,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,    78,     0,    79,    80,     0,    81,    82,    83,    84,
      85,     0,    86,    87,    88,   -59,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,     0,   107,     0,   108,     0,     4,   109,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -59,     0,    40,    41,    42,     0,    43,  -341,
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
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
     104,     0,     0,   105,     0,   106,     0,   107,     0,   108,
       0,     4,   109,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,   753,    39,   -59,     0,    40,
      41,    42,     0,    43,  -341,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -59,    53,    54,     0,
      55,    56,    57,     0,  -341,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -341,   -59,    65,    66,    67,     0,
      68,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,    78,     0,    79,    80,     0,    81,    82,    83,    84,
      85,     0,    86,    87,    88,   -59,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,     0,   107,     0,   108,     0,     4,   109,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
     759,    39,   -59,     0,    40,    41,    42,     0,    43,  -341,
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
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
     104,     0,     0,   105,     0,   106,     0,   107,     0,   108,
       0,     4,   109,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,  1070,    39,   -59,     0,    40,
      41,    42,     0,    43,  -341,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -59,    53,    54,     0,
      55,    56,    57,     0,  -341,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -341,   -59,    65,    66,    67,     0,
      68,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,    78,     0,    79,    80,     0,    81,    82,    83,    84,
      85,     0,    86,    87,    88,   -59,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,     0,   107,     0,   108,     0,     4,   109,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1072,    39,   -59,     0,    40,    41,    42,     0,    43,  -341,
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
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
     104,     0,     0,   105,     0,   106,     0,   107,     0,   108,
       0,     4,   109,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,  1077,    39,   -59,     0,    40,
      41,    42,     0,    43,  -341,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -59,    53,    54,     0,
      55,    56,    57,     0,  -341,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -341,   -59,    65,    66,    67,     0,
      68,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,    78,     0,    79,    80,     0,    81,    82,    83,    84,
      85,     0,    86,    87,    88,   -59,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,     0,   107,     0,   108,     0,     4,   109,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1080,    39,   -59,     0,    40,    41,    42,     0,    43,  -341,
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
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
     104,     0,     0,   105,     0,   106,     0,   107,     0,   108,
       0,     4,   109,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,  1100,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -59,     0,    40,
      41,    42,     0,    43,  -341,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -59,    53,    54,     0,
      55,    56,    57,     0,  -341,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -341,   -59,    65,    66,    67,     0,
      68,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,    78,     0,    79,    80,     0,    81,    82,    83,    84,
      85,     0,    86,    87,    88,   -59,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,     0,   107,     0,   108,     0,     4,   109,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1106,    39,   -59,     0,    40,    41,    42,     0,    43,  -341,
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
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
     104,     0,     0,   105,     0,   106,     0,   107,     0,   108,
       0,     4,   109,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,  1110,    39,   -59,     0,    40,
      41,    42,     0,    43,  -341,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -59,    53,    54,     0,
      55,    56,    57,     0,  -341,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -341,   -59,    65,    66,    67,     0,
      68,    69,    70,     0,    71,    72,    73,    74,    75,    76,
      77,    78,     0,    79,    80,     0,    81,    82,    83,    84,
      85,     0,    86,    87,    88,   -59,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   104,     0,     0,   105,     0,
     106,     0,   107,     0,   108,     0,  1040,   109,     5,   299,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   191,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   192,     0,     0,     0,    33,   193,   194,     0,     0,
     195,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   196,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   197,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   198,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
       0,     0,     0,   105,     0,   199,     0,   107,     0,   200,
    1041,   976,   109,     5,   299,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   191,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   192,     0,     0,     0,
      33,   193,   194,     0,     0,   195,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   196,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   197,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   198,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     199,     0,   107,     0,   200,     0,     5,   109,     6,     7,
       8,     9,    10,    11,     0,   620,     0,    12,     0,     0,
      15,    16,   622,    17,     0,   191,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   192,
       0,     0,     0,    33,   193,   194,     0,     0,   195,    39,
       0,     0,     0,    41,     0,     0,    43,     0,   628,   196,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   197,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   198,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,   504,
     429,   430,   431,   432,   433,     0,     0,   436,   437,   438,
     439,     0,   441,   442,   841,   842,   843,   844,   845,   643,
       0,   644,     0,    99,     0,   645,   646,   647,   648,   649,
     650,   651,   846,   653,   654,   101,   847,   103,     0,   656,
     657,   848,   659,   199,     0,   107,     0,   200,     0,     5,
     109,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,     0,    15,    16,     0,    17,     0,   191,    19,
      20,    21,     0,     0,     0,     0,    26,     0,     0,    28,
      29,     0,   192,     0,     0,     0,    33,    34,    35,    36,
       0,    38,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   196,     0,     0,    47,    48,     0,    49,    50,
      51,    52,     0,    53,    54,     0,    55,    56,    57,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   197,     0,     0,     0,     0,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,     0,    90,     0,     0,    91,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,     0,     0,     0,     0,    99,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,     0,     0,     0,   105,     0,   106,     0,   107,     0,
     108,     0,     0,   109,     5,   299,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   191,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   192,     0,     0,
       0,    33,   193,   194,     0,     0,   195,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   196,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   197,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   198,     0,    79,     0,     0,    81,   303,   304,
       0,    85,   338,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,   339,     0,     0,     0,     0,     0,     0,     0,
     100,     0,     0,   101,   102,   103,     0,     0,     0,   105,
       0,   199,     0,   107,     0,   200,     0,     5,   109,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   191,     0,     0,    21,
     356,    23,     0,     0,     0,     0,     0,     0,    29,     0,
     192,     0,     0,     0,    33,   193,   194,     0,     0,   195,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     196,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,    60,    61,     0,    63,     0,     0,     0,
       0,    66,   197,     0,    68,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   198,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,    90,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,    95,    96,    97,    98,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   199,     0,   107,     0,   200,     0,
       0,   109,     5,   299,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   191,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   192,     0,     0,     0,    33,
     193,   194,     0,     0,   195,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   196,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   197,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     198,     0,    79,     0,     0,    81,   303,   304,     0,    85,
     338,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,   100,     0,
       0,   101,   102,   103,     0,     0,     0,   105,     0,   199,
       0,   107,   814,   200,     0,     0,   109,     5,   299,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   191,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     192,     0,     0,     0,    33,   193,   194,     0,     0,   195,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     196,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   197,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   198,     0,    79,     0,     0,
      81,   303,   304,     0,    85,   338,     0,    87,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,    95,    96,    97,    98,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   199,     0,   107,   816,   200,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   191,
       0,     0,    21,     0,   732,     0,     0,     0,     0,     0,
       0,    29,     0,   192,     0,     0,     0,    33,   193,   194,
       0,     0,   195,    39,     0,   733,     0,    41,     0,     0,
      43,     0,     0,   196,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   197,     0,   734,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   198,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,   735,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,   100,     0,     0,   101,
     102,   103,     0,     0,     0,   105,     0,   199,     0,   107,
       0,   200,     0,     5,   109,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   191,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   192,     0,     0,     0,
      33,   193,   194,     0,     0,   195,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   196,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   197,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   198,     0,    79,     0,     0,    81,   303,   304,     0,
      85,     0,     0,    87,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,   305,     0,     0,   105,     0,
     199,     0,   107,     0,   200,     0,     0,   109,     5,   299,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   191,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   192,     0,     0,     0,    33,   193,   194,     0,     0,
     195,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   196,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   197,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   198,     0,    79,     0,
       0,    81,   303,   304,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
       0,     0,     0,   105,     0,   199,     0,   107,     0,   200,
       0,     5,   109,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     191,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   192,     0,     0,     0,    33,   193,
     194,     0,     0,   195,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   196,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,   978,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   197,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   198,
       0,    79,     0,     0,    81,   303,   304,     0,    85,     0,
       0,    87,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,     0,     0,     0,     0,    99,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   199,     0,
     107,     0,   200,     0,     5,   109,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   191,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   192,     0,     0,
       0,    33,   193,   194,     0,     0,   195,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   196,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
     257,    61,     0,    63,     0,     0,     0,     0,    66,   197,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   198,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,   258,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
     100,     0,     0,   101,   102,   103,     0,     0,     0,   105,
       0,   199,     0,   107,     0,   200,     0,     5,   109,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   191,     0,     0,    21,
       0,   265,     0,     0,     0,     0,     0,     0,    29,     0,
     192,     0,     0,     0,    33,   193,   194,     0,     0,   195,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     196,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   197,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   198,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
     266,    90,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,    95,    96,    97,    98,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   199,     0,   107,     0,   200,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   191,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   192,     0,     0,     0,    33,   193,   194,
       0,     0,   195,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   196,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   197,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   198,     0,
      79,     0,     0,    81,   303,   304,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,   100,     0,     0,   101,
     102,   103,     0,     0,     0,   105,     0,   199,     0,   107,
       0,   200,     0,     0,   109,     5,   299,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   191,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   192,     0,
       0,     0,    33,   193,   194,     0,     0,   195,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   196,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     197,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   198,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,     0,    90,
       0,     0,     0,     0,     0,   258,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,     0,     0,
       0,     0,    99,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,     0,     0,     0,
     105,     0,   199,     0,   107,     0,   200,     0,     0,   109,
       5,   299,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   191,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   192,     0,     0,     0,    33,   193,   194,
       0,     0,   195,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   196,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   197,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   198,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,   100,     0,     0,   101,
     102,   103,     0,     0,     0,   105,     0,   199,   812,   107,
       0,   200,     0,     0,   109,     5,   299,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   191,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   192,     0,
       0,     0,    33,   193,   194,     0,     0,   195,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   196,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     197,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   198,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,     0,     0,
       0,     0,    99,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,     0,     0,     0,
     105,     0,   199,     0,   107,     0,   200,   822,     5,   109,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   191,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   192,     0,     0,     0,    33,   193,   194,     0,     0,
     195,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   196,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   197,  -728,     0,     0,     0,  -728,     0,
       0,    73,    74,    75,    76,    77,   198,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
       0,     0,     0,   105,     0,   199,     0,   107,     0,   200,
       0,     0,   109,     5,   299,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   191,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   192,     0,     0,     0,
      33,   193,   194,     0,     0,   195,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   196,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   197,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   198,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     199,     0,   107,     0,   200,  1212,     5,   109,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   191,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   192,
       0,     0,     0,    33,   193,   194,     0,     0,   195,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   196,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   197,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   198,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,   258,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,   100,     0,     0,   101,   102,   103,     0,     0,
       0,   105,     0,   199,     0,   107,     0,   200,     0,     5,
     109,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   191,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   280,
      29,     0,   192,     0,     0,     0,    33,   193,   194,     0,
       0,   195,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   196,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   197,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   198,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,     0,     0,     0,     0,    99,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,     0,     0,     0,   105,     0,   106,     0,   107,     0,
     200,     0,     0,   109,     5,   299,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   191,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   192,     0,     0,
       0,    33,   193,   194,     0,     0,   195,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   196,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   197,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   198,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
     100,     0,     0,   101,   102,   103,     0,     0,     0,   105,
       0,   199,     0,   107,     0,   200,     0,     5,   109,     6,
       7,     8,   359,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,    18,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     192,     0,     0,     0,    33,   193,   194,     0,     0,   195,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     196,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,    67,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   198,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,    86,    87,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,    95,    96,    97,    98,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   199,     0,   107,     0,   200,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   191,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   192,     0,     0,     0,    33,   193,   194,
       0,     0,   195,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   196,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   197,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   198,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,   100,     0,     0,   101,
     102,   103,     0,     0,     0,   105,     0,   199,     0,   107,
       0,   200,   827,     5,   109,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   191,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   192,     0,     0,     0,
      33,   193,   194,     0,     0,   195,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   196,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   197,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   198,     0,    79,     0,     0,    81,     0,     0,     0,
      85,  1005,     0,    87,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     199,     0,   107,     0,   200,     0,     0,   109,     5,   299,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   191,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,   785,
       0,   192,     0,     0,     0,    33,   193,   194,     0,     0,
     195,    39,     0,     0,     0,   786,     0,     0,    43,     0,
       0,   196,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   197,     0,     0,     0,     0,     0,     0,
       0,    73,   787,    75,    76,    77,   788,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
       0,     0,     0,   105,     0,   199,     0,   107,     0,  1139,
       0,     5,   109,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     191,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   192,     0,     0,     0,    33,   193,
     194,     0,     0,   195,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   196,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   197,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   198,
       0,    79,     0,     0,    81,     0,     0,     0,    85,  1196,
       0,    87,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,     0,     0,     0,     0,    99,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   199,     0,
     107,     0,   200,     0,     0,   109,     5,   299,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   191,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   192,
       0,     0,     0,    33,   193,   194,     0,     0,   195,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   196,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   197,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   198,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,   100,     0,     0,   101,   102,   103,     0,     0,
       0,   105,     0,   199,     0,   107,     0,  1139,     0,     0,
     109,     5,   299,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     191,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   192,     0,     0,     0,    33,   193,
     194,     0,     0,  1170,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   196,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   197,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   198,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,     0,     0,     0,     0,    99,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   199,     0,
     107,     0,   200,     0,     5,   109,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   191,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   192,     0,     0,
       0,    33,   193,   194,     0,     0,   195,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   196,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   197,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   198,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
     100,     0,     0,   101,   102,   103,     0,     0,     0,   105,
       0,   199,     0,   107,     0,   200,     0,     5,   109,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   191,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,   785,     0,
     192,     0,     0,     0,    33,   193,   194,     0,     0,   195,
      39,     0,     0,     0,   786,     0,     0,    43,     0,     0,
     196,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   197,     0,     0,     0,     0,     0,     0,     0,
      73,   787,    75,    76,    77,   788,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,    95,    96,    97,    98,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   199,     0,   107,     0,   789,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   191,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,   785,     0,   192,     0,     0,     0,    33,   193,   194,
       0,     0,   195,    39,     0,     0,     0,   786,     0,     0,
      43,     0,     0,   196,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   197,     0,     0,     0,     0,
       0,     0,     0,    73,   787,    75,    76,    77,   788,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,    95,
      96,    97,    98,     0,     0,     0,     0,    99,     0,     0,
       0,     0,     0,     0,     0,     0,   100,     0,     0,   101,
     102,   103,     0,     0,     0,   105,     0,   199,     0,   107,
       0,   879,     0,     5,   109,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   191,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   192,     0,     0,     0,
      33,   193,   194,     0,     0,   195,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   196,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   197,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   198,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,   101,   102,   103,     0,     0,     0,   105,     0,
     199,     0,   107,     0,   789,     0,     5,   109,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   191,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   192,
       0,     0,     0,    33,   193,   194,     0,     0,   905,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   196,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   197,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   198,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,    95,    96,    97,    98,     0,
       0,     0,     0,    99,     0,     0,     0,     0,     0,     0,
       0,     0,   100,     0,     0,   101,   102,   103,     0,     0,
       0,   105,     0,   199,     0,   107,     0,   200,     0,     5,
     109,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   191,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   192,     0,     0,     0,    33,   193,   194,     0,
       0,   908,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   196,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   197,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   198,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,     0,    90,     0,     0,     0,     0,     0,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,     0,     0,     0,     0,    99,     0,     0,     0,
       0,     0,     0,     0,     0,   100,     0,     0,   101,   102,
     103,     0,     0,     0,   105,     0,   199,     0,   107,     0,
     200,     0,     5,   109,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   191,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   192,     0,     0,     0,    33,
     193,   194,     0,     0,  1164,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   196,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   197,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     198,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,     0,    90,     0,     0,     0,
       0,     0,     0,     0,    93,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
       0,     0,     0,     0,     0,     0,     0,     0,   100,     0,
       0,   101,   102,   103,     0,     0,     0,   105,     0,   199,
       0,   107,     0,   200,     0,     5,   109,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   191,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   192,     0,
       0,     0,    33,   193,   194,     0,     0,  1165,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   196,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     197,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   198,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,    95,    96,    97,    98,     0,     0,
       0,     0,    99,     0,     0,     0,     0,     0,     0,     0,
       0,   100,     0,     0,   101,   102,   103,     0,     0,     0,
     105,     0,   199,     0,   107,     0,   200,     0,     5,   109,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   191,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   192,     0,     0,     0,    33,   193,   194,     0,     0,
    1167,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   196,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   197,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   198,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,     0,     0,     0,     0,
       0,     0,     0,     0,   100,     0,     0,   101,   102,   103,
       0,     0,     0,   105,     0,   199,     0,   107,     0,   200,
       0,     5,   109,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     191,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   192,     0,     0,     0,    33,   193,
     194,     0,     0,  1168,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   196,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   197,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   198,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,     0,    90,     0,     0,     0,     0,
       0,     0,     0,    93,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
      95,    96,    97,    98,     0,     0,     0,     0,    99,     0,
       0,     0,     0,     0,     0,     0,     0,   100,     0,     0,
     101,   102,   103,     0,     0,     0,   105,     0,   199,     0,
     107,     0,   200,     0,     5,   109,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   191,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   192,     0,     0,
       0,    33,   193,   194,     0,     0,  1169,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   196,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   197,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   198,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,    93,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    95,    96,    97,    98,     0,     0,     0,
       0,    99,     0,     0,     0,     0,     0,     0,     0,     0,
     100,     0,     0,   101,   102,   103,     0,     0,     0,   105,
       0,   199,     0,   107,     0,   200,     0,     5,   109,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   191,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     192,     0,     0,     0,    33,   193,   194,     0,     0,  1170,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     196,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   197,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   198,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,    95,    96,    97,    98,
       0,     0,     0,     0,    99,     0,     0,     0,     0,     0,
       0,     0,     0,   100,     0,     0,   101,   102,   103,     0,
       0,     0,   105,     0,   199,     0,   107,     0,   200,     0,
       5,   109,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   191,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   192,     0,     0,     0,    33,   193,   194,
       0,     0,   195,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   196,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   197,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   198,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,     0,    90,     0,     0,     0,     0,     0,
       0,     0,    93,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   618,     0,   619,     0,     0,    94,    95,
      96,    97,    98,     0,   620,     0,     0,    99,     0,   621,
     232,   622,   623,     0,     0,     0,   100,   624,     0,   101,
     102,   103,     0,     0,     0,   105,   235,     0,   192,   107,
       0,   879,   625,     0,   109,     0,     0,     0,   626,     0,
       0,     0,   238,     0,     0,   627,     0,   628,     0,     0,
       0,     0,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,   630,   631,     0,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,     0,     0,     0,   632,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   246,
       0,   633,     0,   248,     0,   634,     0,     0,   635,     0,
       0,     0,   636,     0,     0,   252,     0,     0,     0,   637,
       0,     0,     0,     0,     0,     0,     0,     0,   504,   429,
     430,   431,   432,   433,     0,     0,   436,   437,   438,   439,
       0,   441,   442,   638,   639,   640,   641,   642,   643,     0,
     644,     0,     0,     0,   645,   646,   647,   648,   649,   650,
     651,   652,   653,   654,     0,   655,     0,     0,   656,   657,
     658,   659,     0,     5,   660,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   191,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   192,     0,     0,     0,
      33,   193,   194,     0,     0,   195,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   196,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   197,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   198,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,     0,   102,   103,     0,     0,     0,   105,     0,
     199,     0,   107,     5,   200,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   191,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   192,     0,     0,     0,
      33,   193,   194,     0,     0,   195,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   196,     0,     0,    47,
      48,  -496,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   197,     0,
       0,     0,  -496,     0,     0,     0,    73,    74,    75,    76,
      77,   198,     0,    79,     0,  -496,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,     0,    90,     0,  -496,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    96,     5,   299,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   191,   103,  -496,    21,     0,     0,     0,
    -496,     0,   107,     0,   879,    29,     0,   192,     0,     0,
       0,    33,   193,   194,     0,     0,   195,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   196,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   197,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   198,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,     0,    90,     0,
       0,     0,     0,     0,     0,     0,     0,  -514,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    96,     0,     0,     0,  -514,     0,
       0,     0,  -514,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   103,     0,     0,     0,     0,
       0,   199,     0,   107,  -514,  1139,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   191,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   192,
       0,     0,     0,    33,   193,   194,     0,     0,   195,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   196,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   197,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   198,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,     0,
      90,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,    96,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   191,   103,     0,    21,
       0,     0,     0,   199,     0,   107,     0,   200,    29,     0,
     192,     0,     0,     0,    33,   193,   194,     0,     0,   195,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     196,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   197,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   198,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
       0,    90,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    96,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   191,   103,     0,
      21,     0,     0,     0,   199,     0,   107,     0,   789,    29,
       0,   192,     0,     0,     0,    33,   193,   194,     0,     0,
     195,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   196,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,   687,     0,    59,     0,    61,     0,    63,     0,     0,
     620,     0,    66,   197,     0,     0,     0,   622,   688,     0,
       0,    73,    74,    75,    76,    77,   198,     0,    79,     0,
       0,    81,   689,     0,     0,    85,     0,     0,    87,     0,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   628,     0,     0,     0,     0,     0,     0,
       0,   690,     0,     0,     0,     0,     0,     0,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107,     0,   200,
       0,   691,     0,     0,     0,     0,     0,   692,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   504,   429,   430,   431,   432,   433,
       0,     0,   436,   437,   438,   439,     0,   441,   442,   638,
     639,   640,   641,   642,   643,     0,   644,     0,     0,     0,
     645,   646,   647,   648,   649,   650,   651,   652,   653,   654,
    1046,   655,     0,     0,   656,   657,   658,   659,  1002,   620,
       0,     0,     0,     0,   231,   232,   622,   233,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,     0,     0,
       0,   235,     0,     0,     0,     0,     0,   625,     0,     0,
       0,     0,     0,   237,     0,     0,     0,   238,     0,     0,
     239,     0,   628,     0,     0,     0,     0,     0,     0,     0,
     240,     0,     0,     0,     0,     0,     0,   630,   242,     0,
       0,     0,     0,     0,     0,   243,     0,     0,     0,     0,
       0,     0,     0,     0,   244,     0,     0,     0,     0,     0,
       0,     0,   398,   245,   246,     0,   247,     0,   248,   399,
    1047,     0,     0,   635,     0,     0,     0,   251,     0,     0,
     252,     0,   400,     0,   253,     0,     0,     0,     0,     0,
       0,     0,     0,   504,   429,   430,   431,   432,   433,     0,
       0,   436,   437,   438,   439,     0,   441,   442,   638,   639,
     640,   641,   642,   643,     0,   644,     0,     0,     0,   645,
     646,   647,   648,   649,   650,   651,   652,   653,   654,     0,
     655,     0,     0,   656,   657,   658,   659,     0,   398,   401,
       0,     0,     0,   402,     0,   399,     0,     0,     0,     0,
       0,     0,   503,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,   504,   429,   430,   431,
     432,   433,     0,     0,   436,   437,   438,   439,     0,   441,
     442,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   398,   401,   424,   425,   426,   402,
     458,   399,     0,     0,     0,     0,     0,   427,  1069,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,   504,   429,   430,   431,   432,   433,     0,     0,
     436,   437,   438,   439,     0,   441,   442,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     398,   401,   424,   425,   426,   402,   458,   399,     0,     0,
       0,     0,     0,   427,  1076,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,   504,   429,
     430,   431,   432,   433,     0,     0,   436,   437,   438,   439,
       0,   441,   442,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   398,   401,   424,   425,
     426,   402,   458,   399,     0,     0,     0,     0,     0,   427,
    1240,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,   504,   429,   430,   431,   432,   433,
       0,     0,   436,   437,   438,   439,     0,   441,   442,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   398,   401,   424,   425,   426,   402,   458,   399,
       0,     0,     0,     0,     0,   427,  1241,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
     504,   429,   430,   431,   432,   433,     0,     0,   436,   437,
     438,   439,     0,   441,   442,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   398,   401,
     424,   425,   426,   402,   458,   399,     0,     0,     0,     0,
       0,   427,  1242,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,   504,   429,   430,   431,
     432,   433,     0,     0,   436,   437,   438,   439,     0,   441,
     442,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   398,   401,   424,   425,   426,   402,
     458,   399,     0,     0,     0,     0,     0,   427,  1243,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,   504,   429,   430,   431,   432,   433,     0,     0,
     436,   437,   438,   439,     0,   441,   442,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     398,   401,   424,   425,   426,   402,   458,   399,     0,     0,
       0,     0,     0,   427,  1249,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,   504,   429,
     430,   431,   432,   433,     0,     0,   436,   437,   438,   439,
       0,   441,   442,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   398,   401,   424,   425,
     426,   402,   458,   399,     0,     0,     0,     0,     0,   427,
    1250,     0,     0,     0,     0,     0,   400,   280,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   496,   403,   504,   429,   430,   431,   432,   433,
       0,     0,   436,   437,   438,   439,     0,   441,   442,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,     0,   401,   424,   425,   426,   402,   458,     0,
       0,     0,     0,     0,     0,   427,     0,   398,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   487,     0,   400,     0,   403,
       0,     0,   580,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   581,     0,
     424,   425,   426,     0,   458,     0,     0,     0,     0,     0,
       0,   427,     0,     0,   401,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   398,   837,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
     403,     0,     0,   838,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   839,
     582,   424,   425,   426,     0,     0,     0,     0,     0,     0,
       0,     0,   427,     0,   398,   401,     0,     0,     0,   402,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   953,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     398,   401,   424,   425,   426,   402,     0,   399,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
     400,   280,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   487,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   398,   401,   424,   425,
     426,   402,   458,   399,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   487,
       0,     0,   460,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   398,   401,   424,   425,   426,   402,   458,   399,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,   400,   280,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   211,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   398,   401,
     424,   425,   426,   402,     0,   399,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,   400,   723,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   724,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   398,   401,   424,   425,   426,   402,
     458,   399,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,     0,     0,   400,   725,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     726,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     398,   401,   424,   425,   426,   402,     0,   399,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
     400,   945,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   398,   401,   424,   425,
     426,   402,     0,   399,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   398,   401,   424,   425,   426,   402,   458,   399,
       0,     0,     0,     0,     0,   427,   967,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,     0,   401,
     424,   425,   426,   402,   458,     0,     0,     0,     0,     0,
       0,   427,     0,   398,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   487,     0,   400,     0,   403,     0,     0,  1119,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,  1120,   400,   424,   425,   426,     0,
       0,     0,     0,     0,     0,   996,     0,   427,     0,     0,
     401,     0,     0,     0,   402,   398,   527,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,     0,   401,     0,     0,     0,   402,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,     0,   401,     0,     0,   398,   402,     0,   427,     0,
       0,     0,   399,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   400,   413,   414,   415,     0,
     417,   418,   419,   420,     0,     0,   423,     0,   403,   424,
     425,   426,     0,     0,   398,     0,     0,     0,     0,     0,
     427,   399,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,   400,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,   401,     0,     0,     0,   402,     0,     0,     0,
     427,   398,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,   403,     0,
       0,   401,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   403,     0,   424,
     425,   426,     0,   539,     0,     0,     0,     0,   401,     0,
     427,     0,   402,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,     0,   543,   424,   425,
     426,     0,     0,   567,   403,     0,   398,     0,     0,   427,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
     404,   405,     0,   406,   407,   408,   400,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,     0,
     398,     0,     0,     0,     0,     0,   427,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,     0,   727,   398,   833,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,   403,
       0,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,   402,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,   403,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,   401,     0,     0,     0,   402,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   398,     0,   424,   425,   426,     0,     0,   399,
     403,   813,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   834,   411,     0,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   398,
       0,   424,   425,   426,     0,     0,   399,     0,     0,     0,
       0,     0,   427,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   211,     0,   400,     0,   403,   965,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,     0,     0,     0,
     402,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   424,   425,   426,     0,
       0,     0,   403,     0,     0,     0,     0,   427,     0,     0,
     401,     0,     0,     0,   402,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   398,   936,   424,   425,   426,   403,     0,   399,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
       0,   400,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   398,     0,   424,   425,   426,
       0,     0,   399,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   401,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,   401,     0,     0,     0,   402,     0,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,   403,     0,
       0,     0,   981,     0,     0,     0,   427,   401,     0,     0,
       0,   402,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,   398,  1011,   424,
     425,   426,   999,   403,   399,     0,   982,     0,     0,     0,
     427,     0,     0,     0,     0,     0,     0,   400,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   398,     0,   424,   425,   426,     0,     0,   399,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,     0,   402,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,  1054,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,     0,     0,   402,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   403,     0,     0,     0,   401,
       0,     0,   427,   402,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,     0,   403,   424,   425,   426,     0,
       0,     0,     0,  1045,     0,     0,     0,   427,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   398,     0,   424,   425,   426,     0,
       0,   399,     0,     0,     0,     0,     0,   427,   467,     0,
       0,     0,    22,    23,   400,  1057,     0,  1019,     0,     0,
       0,     0,   468,     0,    31,   469,     0,     0,     0,     0,
      37,     0,     0,   398,     0,     0,     0,    42,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,   349,     0,  1020,
       0,   401,  1021,     0,   470,   402,    68,   398,  1116,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    84,     0,   400,    86,     0,
    1022,     0,    89,     0,     0,     0,     0,   403,     0,     0,
     401,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   403,     0,   424,   425,
     426,   104,     0,     0,   401,     0,     0,     0,   402,   427,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   398,     0,   424,   425,   426,
     403,     0,   399,     0,     0,     0,  1060,     0,   427,     0,
       0,     0,     0,     0,     0,   400,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,     0,
     398,   424,   425,   426,     0,     0,     0,   399,     0,     0,
       0,     0,   427,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,     0,     0,   402,     0,     0,     0,
     398,  1202,     0,     0,     0,  1163,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,   402,     0,     0,   404,   405,     0,   406,   407,   408,
    1166,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,     0,   403,     0,     0,     0,   401,     0,     0,
     427,   402,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,     0,   403,   424,   425,   426,     0,     0,   398,
       0,     0,     0,     0,     0,   427,   399,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,   400,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,     0,   398,
       0,     0,     0,     0,     0,   427,   399,     0,  1225,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,     0,     0,     0,
     402,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,   401,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,     0,   403,   424,   425,   426,     0,     0,     0,     0,
       0,   401,     0,     0,   427,   402,     0,     0,   404,   405,
       0,   406,   407,   408,  1275,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   398,     0,   424,   425,   426,     0,   403,   399,     0,
       0,     0,  1245,     0,   427,     0,     0,     0,     0,     0,
       0,   400,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,   419,   420,   421,   422,   423,   398,     0,   424,   425,
     426,     0,     0,   399,     0,     0,     0,     0,     0,   427,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   401,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,  1276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,     0,     0,   402,     0,     0,
     404,   405,     0,   406,   407,   408,  1277,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   398,     0,   424,   425,   426,     0,   403,
     399,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,   400,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   398,     0,
     424,   425,   426,     0,     0,   399,     0,     0,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     401,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,  1278,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,     0,     0,   402,
       0,     0,   404,   405,     0,   406,   407,   408,  1279,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
     419,   420,   421,   422,   423,   398,     0,   424,   425,   426,
       0,   403,   399,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,     0,   400,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,   419,   420,   421,   422,   423,
     398,     0,   424,   425,   426,     0,     0,   399,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,     0,     0,   402,     0,     0,     0,
     398,     0,     0,     0,     0,  1280,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,   402,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,     0,   403,     0,     0,     0,   401,     0,     0,
     427,   402,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,     0,   403,   424,   425,   426,     0,     0,   398,
       0,     0,     0,  1293,     0,   427,   399,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,   400,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   931,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,   401,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   403,   401,   424,   425,   426,   402,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,     0,   417,   418,   419,   420,   421,     0,   423,
       0,     0,   424,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,   427,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,     0,
     417,   418,   419,   420,   421,     0,   423,     0,   467,   424,
     425,   426,    22,    23,     0,     0,     0,     0,     0,     0,
     427,     0,   468,     0,    31,   469,     0,     0,   467,     0,
      37,     0,    22,    23,     0,     0,     0,    42,     0,     0,
       0,     0,   468,     0,    31,   469,     0,     0,     0,     0,
      37,     0,   -59,     0,     0,     0,     0,    42,     0,     0,
       0,     0,     0,    58,     0,    60,     0,     0,     0,    64,
       0,   -59,    65,     0,   470,     0,    68,     0,     0,     0,
       0,     0,     0,    58,   367,    60,   230,     0,     0,  1020,
       0,     0,  1021,     0,   470,    84,    68,     0,    86,     0,
     231,   232,    89,   233,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,   368,    84,     0,   235,    86,     0,
       0,     0,    89,   236,     0,     0,     0,     0,     0,   237,
       0,     0,     0,   238,     0,     0,   239,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   240,     0,     0,     0,
       0,   104,     0,   241,   242,     0,     0,     0,     0,     0,
       0,   243,     0,     0,     0,     0,     0,     0,     0,     0,
     244,   104,     0,     0,     0,     0,     0,   230,     0,   245,
     246,     0,   247,     0,   248,     0,   249,     0,     0,   250,
       0,   231,   232,   251,   233,     0,   252,   369,     0,   234,
     253,    23,     0,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,     0,     0,
     237,     0,     0,     0,   238,     0,     0,   239,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   240,     0,     0,
       0,     0,     0,     0,   241,   242,     0,     0,     0,     0,
       0,     0,   243,    60,     0,     0,     0,     0,     0,     0,
       0,   244,     0,     0,    68,     0,     0,     0,     0,     0,
     245,   246,     0,   247,     0,   248,     0,   249,   230,     0,
     250,     0,     0,     0,   251,     0,     0,   252,     0,     0,
      89,   253,   231,   232,     0,   233,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,   368,     0,     0,   235,
       0,     0,     0,     0,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,     0,     0,   239,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   240,     0,
       0,     0,     0,     0,     0,   241,   242,     0,     0,     0,
       0,     0,     0,   243,     0,     0,     0,     0,     0,     0,
       0,     0,   244,     0,     0,  -307,     0,     0,     0,     0,
       0,   245,   246,     0,   247,     0,   248,     0,   249,  -307,
    -307,   250,  -307,     0,     0,   251,     0,  -307,   252,     0,
       0,     0,   253,     0,     0,     0,  -307,     0,     0,     0,
       0,     0,  -307,     0,     0,     0,     0,     0,  -307,     0,
       0,     0,  -307,     0,     0,  -307,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -307,     0,     0,     0,     0,
       0,     0,  -307,  -307,     0,     0,     0,     0,     0,     0,
    -307,     0,     0,     0,     0,     0,     0,     0,     0,  -307,
       0,     0,   230,     0,     0,     0,     0,     0,  -307,  -307,
       0,  -307,     0,  -307,     0,  -307,   231,   232,  -307,   233,
       0,     0,  -307,     0,   234,  -307,     0,     0,     0,  -307,
       0,     0,     0,   235,     0,     0,     0,     0,     0,   236,
       0,     0,     0,     0,     0,   237,     0,     0,     0,   238,
       0,     0,   239,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   240,     0,     0,     0,     0,     0,     0,   241,
     242,     0,     0,     0,     0,     0,     0,   243,     0,     0,
       0,     0,     0,     0,     0,     0,   244,     0,     0,  -308,
       0,     0,     0,     0,     0,   245,   246,     0,   247,     0,
     248,     0,   249,  -308,  -308,   250,  -308,     0,     0,   251,
       0,  -308,   252,     0,     0,     0,   253,     0,     0,     0,
    -308,     0,     0,     0,     0,     0,  -308,     0,     0,     0,
       0,     0,  -308,     0,     0,     0,  -308,     0,     0,  -308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -308,
       0,     0,     0,     0,     0,     0,  -308,  -308,     0,     0,
       0,     0,     0,     0,  -308,     0,     0,     0,     0,     0,
       0,     0,     0,  -308,     0,     0,     0,     0,     0,     0,
       0,     0,  -308,  -308,     0,  -308,     0,  -308,     0,  -308,
       0,     0,  -308,     0,     0,     0,  -308,     0,     0,  -308,
       0,     0,     0,  -308
};

static const yytype_int16 yycheck[] =
{
      12,   375,   455,   164,    16,    69,   272,     2,    20,    70,
     615,    19,    40,   390,    26,   177,   669,   612,   514,   135,
     580,   390,    34,    35,    36,   227,    38,   839,   519,   360,
     296,    92,   509,   665,   593,   667,   595,    49,   757,    51,
      52,   541,  1032,   975,   108,    57,   107,    59,   314,    61,
     834,     1,     1,   220,  1041,     1,   857,    69,    70,    71,
      72,    73,    74,    48,    48,     3,    78,    33,    80,    33,
      33,    33,    33,   229,    33,    87,    75,    56,    22,    91,
      92,    61,    94,   102,    96,   863,     0,    99,   100,    92,
     102,   121,    37,    48,   106,   107,   108,   109,    56,   121,
       1,    48,     1,  1137,  1036,    48,   268,   147,   165,   119,
    1265,   147,   262,   263,   264,    61,   272,    48,   147,   118,
     165,   133,   324,   135,   615,   126,    75,   172,    73,   165,
      48,  1063,    48,     3,     3,   175,   200,   147,  1293,   119,
     296,    56,   171,    90,    88,   175,    92,    90,  1138,    48,
     151,   170,   121,   175,    92,   121,   147,   121,   314,   105,
      61,   173,   147,   147,    69,   103,   121,   170,   112,   118,
       3,  1158,   171,   119,   165,    18,   126,  1211,   669,   445,
     446,   193,   194,   195,   147,   147,   198,   199,   200,   784,
     175,    92,   147,   172,   344,   345,   146,   147,   917,   227,
     146,   106,   107,   108,   170,  1137,   170,   170,   170,   170,
     218,   170,  1013,   141,   172,    32,   147,   229,   119,   165,
     175,   149,   171,   173,   170,  1009,    56,   126,   390,   147,
      61,   147,  1212,   103,   103,    78,  1014,    34,    35,    36,
      92,   147,   114,   359,   175,   146,  1024,   146,   147,    47,
     750,   728,   583,    96,   410,    32,   147,   175,    27,   175,
     272,    92,   147,    27,   466,    47,   165,   173,    66,   170,
     113,   147,    89,   141,   105,   147,    93,    83,   838,  1211,
      86,   149,   173,   147,   296,  1265,   171,    85,   119,   445,
     446,   303,   304,   147,   199,   200,   324,   173,   147,   171,
     364,   462,   314,    85,   757,   864,  1118,   866,  1120,   173,
      79,   108,    89,   165,    56,    79,    93,   115,   170,   173,
     147,    90,   171,    56,   141,   142,    90,   339,   145,   146,
     141,   148,   149,   115,   165,   930,   153,    54,   149,   170,
     352,   336,   354,   160,   546,   109,   173,   116,  1126,   166,
     167,   168,   364,   509,  1132,  1074,  1134,    74,    89,   147,
     527,   165,    93,   147,   141,   165,   147,    84,   172,   146,
     174,   171,   149,   434,   435,   109,   153,   165,    90,   440,
     876,   165,   443,   444,   861,   541,   398,   399,   400,   401,
     402,   403,   173,   405,   406,   407,   408,   409,   147,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,  1048,  1049,   482,   586,
     147,   147,   434,   435,   173,   147,   172,  1205,   440,  1207,
    1035,   443,   444,   445,   446,   447,   448,   914,   466,   147,
     147,    27,  1002,   455,  1097,     8,   173,   173,   460,   147,
     147,   173,   464,    11,     3,  1154,    27,    61,   165,   364,
    1159,   473,    48,    48,   917,   173,    52,    47,   165,   147,
     482,    20,   484,   485,   147,   173,    61,   147,   490,   491,
     172,   126,   174,    56,   496,    34,    66,   165,    92,   863,
      76,   503,   165,    79,   506,    81,   557,   509,   559,   147,
     874,   105,   147,   173,    90,    85,   151,    92,    79,   521,
       2,   885,   889,   525,    63,   119,   528,   529,   546,    90,
     889,   147,   534,   109,    56,   173,   165,    89,   173,   541,
     147,    93,   171,    25,   119,   115,    28,   172,   109,   165,
     147,   126,   554,    56,  1035,   557,    56,   559,   165,   126,
    1041,   563,   564,   565,   103,   567,   568,  1117,   165,  1119,
     109,   165,   147,   147,    61,   147,   170,     8,   580,   581,
     147,   147,   728,    56,   151,    89,    27,   482,   573,    93,
     165,   165,    56,   165,   596,   170,    78,   173,   173,   165,
      82,    83,    27,  1154,   750,    92,   173,    48,  1159,    89,
       2,    52,   172,    93,   401,   402,  1097,   172,   105,   174,
      56,   761,   762,    48,   106,   765,   766,    52,   100,   111,
      89,  1074,   119,    25,    93,    76,    28,    89,    79,   534,
      81,    93,    56,   172,    32,   174,   172,   172,   174,    90,
    1014,    76,    56,   170,    79,   172,    81,    56,   660,   554,
     447,   448,   557,    56,   559,    90,     8,   141,   109,   564,
      82,    83,   146,   460,   172,   149,   174,  1158,   165,   153,
     820,   821,    56,   170,   109,   170,    78,   833,   834,    56,
      82,    83,   146,   172,   172,   174,   174,   748,   485,   297,
     298,    89,    32,   172,   491,    93,    56,   165,   859,   496,
     557,   164,   559,   170,   106,   861,   165,   165,   165,   111,
     165,   723,   724,   725,   726,   727,   728,   729,   730,   172,
     212,  1212,  1148,   165,   120,   789,   165,   889,   165,   165,
     170,    85,   793,   741,   742,     8,   748,   165,   750,   751,
     172,   753,   172,   141,   142,   757,   172,   759,   146,    89,
     148,   149,  1126,    93,  1245,   153,   768,   769,   914,   172,
     172,   165,   160,    66,   170,     8,   563,     8,   166,   167,
     168,   171,    90,   126,  1265,   787,   788,   789,    90,   126,
     942,   793,   149,   275,    69,   172,    48,   949,   280,   126,
     173,   147,    48,  1219,  1220,  1221,  1222,  1223,  1224,    61,
      48,   141,  1293,   172,   126,    61,   146,   126,   148,   149,
     212,    35,    35,   153,    51,   879,    66,    54,   173,   831,
     160,   126,   126,   835,   173,   837,   838,   839,   168,   841,
      92,   843,  1098,  1099,   846,   847,    92,    74,   173,    21,
      77,   175,     2,   748,   336,   175,   170,    84,   152,   861,
     154,   155,   147,   157,   158,   147,   173,   119,   105,   351,
     149,   172,   149,   119,   126,    25,   149,   879,    28,   141,
     126,   149,   109,   275,   149,   149,   141,   141,   280,   149,
     149,   149,   149,   149,   789,   147,   141,    32,   141,   149,
     172,   147,   173,   905,   165,   175,   908,    35,   390,    35,
     114,   146,   914,   165,   165,   917,   165,   165,   170,   165,
     173,   173,   147,   165,   170,    33,   165,   173,    78,   931,
     932,   985,    82,    83,   175,   171,   165,   724,   170,   726,
      33,   943,   165,   945,   336,   171,    35,   175,   950,   173,
     165,   953,  1098,  1099,    89,   175,   106,   146,    93,   351,
     172,   111,   172,   965,   175,   165,   175,     8,     8,   175,
     173,   141,   141,    35,   173,   165,   458,   173,   173,   165,
     165,   164,   164,   985,   879,   147,   351,   271,   831,    24,
    1024,  1024,   994,    91,   996,   971,   869,   999,   321,   989,
    1002,   525,   484,   870,   362,   889,   141,  1009,   490,  1011,
     874,   146,   376,   148,   149,   885,   606,  1159,   153,   501,
    1225,   503,  1174,  1175,  1218,   160,  1178,  1179,   884,   858,
     671,   166,   167,   168,   133,   783,  1038,    -1,   522,  1191,
      -1,  1193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1053,  1054,    -1,  1056,  1057,    -1,  1059,  1060,    -1,
      -1,    -1,   212,    -1,    -1,    -1,   458,  1069,  1070,    -1,
    1072,    -1,  1074,    -1,  1076,  1077,    -1,    -1,  1080,   561,
      -1,    -1,    32,   565,    -1,  1139,    -1,    -1,    -1,    -1,
     985,   573,   484,    -1,  1096,  1097,  1098,  1099,   490,    -1,
      -1,    -1,  1104,    -1,  1106,    -1,    -1,    -1,  1110,   501,
      -1,   503,    -1,    -1,  1116,  1117,  1118,  1119,  1120,    -1,
      -1,    -1,    -1,    -1,    -1,   275,    -1,    -1,    -1,    -1,
     280,    -1,    -1,    -1,    -1,    -1,    -1,  1139,    -1,    89,
    1035,    -1,  1144,    93,   931,   932,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1158,    -1,    -1,    -1,
      -1,  1163,  1164,  1165,  1166,  1167,  1168,  1169,  1170,   561,
      -1,    -1,    -1,   565,    -1,    -1,    -1,    -1,    -1,     2,
      -1,   573,    -1,    -1,    -1,    -1,   336,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
    1202,   351,    25,   153,    -1,    28,    -1,    -1,    -1,   159,
     160,    -1,    -1,   163,    -1,  1217,   166,   167,   168,    -1,
      -1,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,    -1,    -1,  1240,  1241,
    1242,  1243,    -1,  1245,  1139,    -1,    -1,  1249,  1250,  1144,
      -1,    -1,    -1,    -1,    -1,    78,   738,    -1,    -1,    82,
      83,    -1,  1264,  1265,   169,    -1,    -1,   172,    -1,   174,
      -1,    -1,     1,  1275,  1276,  1277,  1278,  1279,  1280,    -1,
      -1,    -1,    -1,   106,    -1,    -1,    -1,    -1,   111,   771,
      -1,  1293,   774,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,     3,    36,   458,    38,
      39,    -1,    -1,    -1,    -1,    44,    -1,  1212,    -1,    -1,
      17,    -1,    51,    20,    -1,    54,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,   484,    -1,    -1,   819,    -1,    36,
     490,    -1,    -1,   825,    -1,    74,    -1,    -1,    77,    46,
      79,   501,    81,   503,    83,    84,    53,    86,    -1,    88,
      -1,    90,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
    1265,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,   771,
     109,    -1,   774,   112,    -1,   114,     2,   116,    -1,   212,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1293,    -1,
      -1,    -1,    99,    -1,    -1,    -1,   103,    -1,    -1,    25,
      -1,   561,    28,   110,    -1,   565,    -1,    -1,    -1,    -1,
     117,    -1,    -1,   573,    -1,    -1,    -1,   819,    -1,    -1,
      -1,    -1,    -1,   825,    -1,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    -1,   931,
     932,    -1,   275,    -1,    -1,    -1,    -1,   280,    -1,    -1,
      -1,   943,    78,   945,    -1,    -1,    82,    83,   950,    -1,
      22,   953,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,
     962,    -1,   964,    -1,    36,   967,    38,    39,    -1,    -1,
     106,    -1,    44,    -1,    -1,   111,    -1,    -1,    -1,    51,
      -1,    -1,   984,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   994,   336,   996,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    77,    -1,    79,   351,    81,
      -1,    83,    -1,    -1,    86,    -1,    88,    -1,    90,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   943,    -1,   945,    -1,    -1,    -1,   109,   950,    -1,
     112,   953,   114,    -1,   116,    -1,    -1,    -1,    -1,    -1,
     962,    -1,   964,    -1,    -1,   967,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,  1069,    -1,    -1,
      -1,    -1,   984,    -1,  1076,    -1,   212,    -1,    -1,    -1,
      -1,    -1,   994,    -1,   996,    -1,    -1,  1089,    -1,    -1,
      -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,  1100,   171,
       2,   771,    -1,    -1,   774,    -1,    -1,  1109,    -1,   141,
     142,  1113,   144,   145,   146,   458,   148,   149,   150,    -1,
      -1,   153,    -1,    25,    -1,    -1,    28,   159,   160,    -1,
      -1,   163,    -1,    -1,   166,   167,   168,    -1,    -1,   275,
      -1,   484,    -1,    -1,   280,   177,    -1,   490,    -1,   819,
      32,    -1,    -1,    -1,    -1,   825,    -1,  1069,   501,    -1,
     503,    -1,    -1,    -1,  1076,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    78,  1089,  1180,    -1,
      82,    83,    -1,    -1,    -1,    -1,    -1,    -1,  1100,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1109,    -1,    -1,
     336,  1113,    -1,    -1,   106,    -1,    -1,    89,    -1,   111,
      -1,    93,    -1,    -1,    -1,   351,    -1,    -1,   561,    -1,
      -1,    -1,   565,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     573,    -1,    -1,    -1,    -1,     2,    -1,    -1,  1240,  1241,
    1242,  1243,    -1,    -1,    -1,    -1,    -1,  1249,  1250,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,   141,
     142,    28,   144,   145,   146,    -1,   148,   149,  1180,    -1,
      -1,   153,    -1,   943,    -1,   945,    -1,    -1,   160,    -1,
     950,    -1,    -1,   953,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,   962,    -1,   964,    -1,    -1,   967,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
     212,    78,    -1,    -1,   984,    82,    83,    36,    -1,    38,
      39,    -1,   458,    -1,   994,    44,   996,    -1,  1240,  1241,
    1242,  1243,    51,    -1,    -1,    -1,    -1,  1249,  1250,   106,
      -1,    -1,    -1,    -1,   111,    -1,    -1,    -1,   484,    -1,
      -1,    -1,    -1,    -1,   490,    -1,    -1,    -1,    77,    -1,
      79,    -1,    -1,    -1,    -1,   501,    -1,   503,    -1,    88,
      -1,    90,    -1,   275,    -1,    -1,    -1,    -1,   280,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
     109,    26,    27,   112,    -1,    -1,    31,   116,    -1,  1069,
      -1,    36,    -1,    38,    39,    -1,  1076,    -1,    -1,    44,
      -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,  1089,
      -1,    -1,    -1,    -1,    -1,   561,    -1,    -1,   771,   565,
    1100,   774,    -1,    -1,   336,    -1,    -1,   573,    -1,  1109,
       2,    -1,    77,  1113,    79,   212,    81,     3,    83,   351,
      -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    17,    18,    25,    20,    -1,    28,    -1,    -1,    25,
      -1,    -1,    -1,    -1,   109,    -1,   819,   112,    34,   114,
      -1,   116,   825,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,   275,    -1,
    1180,    -1,    -1,   280,    70,    71,    78,    -1,    -1,    -1,
      82,    83,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     165,    87,    -1,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      96,    97,    -1,    99,   106,   101,    -1,   103,    -1,   111,
     106,    -1,    -1,    -1,   110,   111,   458,   113,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   336,
    1240,  1241,  1242,  1243,    -1,    -1,    -1,    -1,    -1,  1249,
    1250,    -1,   484,    -1,   351,    -1,    -1,    -1,   490,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   501,
     943,   503,   945,    -1,    -1,    -1,    -1,   950,    -1,    -1,
     953,    -1,    -1,    -1,    -1,    -1,   172,    -1,    -1,   962,
      -1,   964,    -1,    -1,   967,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   771,    -1,    -1,   774,    -1,
      -1,   984,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     212,   994,    -1,   996,    -1,    -1,    -1,    -1,    -1,   561,
      -1,    -1,    -1,   565,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   573,     2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   819,    -1,    -1,    -1,    -1,    -1,   825,
      -1,   458,    -1,    -1,    -1,    25,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   275,    -1,    -1,    -1,   484,   280,    -1,
      -1,    -1,    -1,   490,    -1,    -1,  1069,    -1,    -1,    -1,
      -1,    -1,    -1,  1076,   501,    -1,   503,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1089,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1100,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1109,    -1,    -1,    -1,
    1113,    -1,    -1,    -1,   336,    -1,   106,    -1,    -1,    -1,
      -1,   111,    -1,    22,    -1,    -1,    -1,    26,    27,   351,
      -1,    -1,    31,    -1,   561,    -1,    -1,    36,   565,    38,
      39,    -1,    -1,    -1,    -1,    44,   573,   943,    -1,   945,
      -1,    -1,    51,    -1,   950,    -1,    -1,   953,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   962,    -1,   964,    -1,
      -1,   967,    -1,    -1,    -1,    -1,    -1,  1180,    77,    -1,
      79,    -1,    81,    -1,    83,    -1,    -1,    86,   984,    88,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,   994,    -1,
     996,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,   771,
     109,    -1,   774,   112,    -1,   114,    -1,   116,    -1,    -1,
      -1,    -1,   212,    -1,    -1,    -1,    -1,    -1,     3,    -1,
      25,    -1,    -1,    28,    -1,    -1,   458,  1240,  1241,  1242,
    1243,    -1,    17,    18,    -1,    20,  1249,  1250,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,   819,    -1,    34,
      -1,    -1,   484,   825,    -1,    40,   165,    -1,   490,    -1,
      -1,    46,   171,  1069,    -1,    50,    -1,    -1,    53,   501,
    1076,   503,    -1,    78,    -1,   275,    -1,    -1,    63,    -1,
     280,    -1,    -1,  1089,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,  1100,    -1,    -1,    -1,    -1,    -1,
      -1,   106,    87,  1109,    -1,    -1,   111,  1113,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,   561,
      -1,    -1,   117,   565,   771,    -1,   336,   774,    -1,    -1,
      -1,   573,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   351,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   943,    -1,   945,  1180,    -1,    -1,    -1,   950,    -1,
      -1,   953,   819,    -1,    -1,   170,    -1,   172,   825,    -1,
     962,    -1,   964,    -1,    -1,   967,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   212,    -1,    -1,
      -1,    -1,   984,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   994,    -1,   996,    -1,    -1,    -1,    -1,    -1,
      49,    -1,    -1,    -1,  1240,  1241,  1242,  1243,    -1,    -1,
      -1,    -1,    -1,  1249,  1250,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    72,    -1,    -1,    -1,    -1,   458,    -1,
      -1,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
     275,    -1,    -1,    12,    -1,   280,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,   484,    -1,    -1,    -1,    -1,    -1,
     490,    -1,    -1,    32,    33,    -1,    -1,  1069,    -1,    -1,
      -1,   501,    -1,   503,  1076,    -1,   943,    -1,   945,    48,
      -1,    -1,    -1,   950,    -1,    -1,   953,  1089,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   962,    -1,   964,  1100,    -1,
     967,   336,    -1,    -1,    -1,    72,    -1,  1109,    -1,   771,
      -1,  1113,   774,    -1,    -1,    -1,   351,   984,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,   994,    -1,   996,
      -1,   561,    -1,    -1,    -1,   565,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   573,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    30,   121,    -1,    -1,    -1,   125,   819,    -1,    -1,
      -1,    -1,    -1,   825,    -1,    44,    45,    46,   217,    48,
      -1,    -1,   141,   142,    -1,   144,   145,   146,  1180,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   170,  1069,    -1,    -1,    -1,    -1,    -1,   177,  1076,
     259,   260,   261,   262,   263,   264,    -1,    -1,   267,   268,
      -1,    -1,  1089,   458,    -1,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1100,   283,    -1,   285,    -1,  1240,  1241,
    1242,  1243,  1109,   292,    -1,    -1,  1113,  1249,  1250,   484,
      -1,    -1,    -1,    -1,    -1,   490,    -1,    -1,    -1,    -1,
      -1,   310,    -1,    -1,    -1,    -1,   501,    -1,    -1,    -1,
      -1,    -1,   151,    -1,    -1,    -1,   325,   326,    -1,    -1,
      -1,   943,    -1,   945,   163,   164,    -1,    -1,   950,    -1,
      -1,   953,   259,   260,   261,   262,   263,   264,    -1,    -1,
     962,    -1,   964,    -1,    -1,   967,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1180,    -1,    -1,   283,   196,   285,    -1,
      -1,    -1,   984,    -1,    -1,   292,   561,    -1,    -1,    -1,
     565,    -1,   994,    -1,   996,    -1,    -1,    -1,   573,    -1,
      -1,   771,    -1,   310,   774,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,   325,   326,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1240,  1241,  1242,  1243,    -1,   257,    -1,
      -1,    -1,  1249,  1250,    -1,    -1,   265,   266,    -1,   819,
      -1,    -1,    -1,    -1,    -1,   825,    -1,   276,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1069,    -1,    89,
      -1,    -1,   461,    93,  1076,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1089,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1100,    -1,
     489,    -1,    -1,    -1,   493,   125,   495,  1109,    -1,   498,
      -1,  1113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   363,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,   461,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   390,    -1,   943,    -1,   945,    -1,    -1,  1180,    -1,
     950,    -1,   489,   953,    -1,    -1,   493,    -1,   495,    -1,
      -1,   498,   962,    -1,   964,    -1,   771,   967,    -1,   774,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   984,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   994,    -1,   996,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   454,    -1,    -1,  1240,  1241,
    1242,  1243,     3,   462,   819,    -1,    -1,  1249,  1250,   468,
     825,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,   510,    53,    -1,    -1,   514,    -1,    -1,    -1,  1069,
      -1,    -1,    63,   522,    -1,    -1,  1076,    -1,   527,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,  1089,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
    1100,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,  1109,
     101,    -1,   103,  1113,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
     579,    -1,    12,    -1,    -1,    -1,   755,   756,   943,    19,
     945,    -1,   761,   762,    -1,   950,   765,   766,   953,    -1,
      -1,    -1,    32,    33,    -1,    -1,   775,   962,    -1,   964,
      -1,   610,   967,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   984,
    1180,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   994,
     809,   996,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,   755,   756,
      -1,    -1,   671,    -1,   761,   762,    -1,    -1,   765,   766,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1240,  1241,  1242,  1243,    -1,   125,    -1,    -1,    -1,  1249,
    1250,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,  1089,    -1,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,    -1,  1100,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,  1109,    -1,    -1,    -1,  1113,    -1,
      -1,    -1,    -1,    -1,   933,   934,    -1,    -1,    -1,    -1,
      -1,   940,    -1,   942,    -1,    -1,    -1,   776,    -1,    -1,
     949,    -1,    -1,   952,   783,    -1,   955,    -1,   957,    -1,
     959,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     799,    -1,    -1,   972,    -1,    -1,    -1,    -1,    -1,    -1,
     809,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   992,    -1,  1180,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   933,   934,    -1,    -1,
      -1,    -1,    -1,   940,    -1,    -1,    -1,    -1,    -1,   858,
     859,    -1,    -1,    -1,    -1,   952,    -1,     3,   955,    -1,
     957,    -1,   959,    -1,   873,    -1,    -1,   876,    -1,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
     889,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,  1102,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,   971,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     989,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1174,  1175,    -1,    -1,  1178,
    1179,    -1,    -1,    -1,    -1,  1102,    -1,    -1,    -1,    -1,
    1189,    -1,  1191,    -1,  1193,     0,     1,    -1,     3,    -1,
       5,     6,     7,     8,     9,    10,   172,    -1,    -1,    14,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
     170,   171,   172,    -1,   174,    -1,     1,   177,     3,     4,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,
      -1,     1,   177,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,   171,   172,    -1,   174,    -1,     1,   177,     3,    -1,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,
      -1,     1,   177,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,   171,   172,    -1,   174,    -1,     1,   177,     3,    -1,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,
      -1,     1,   177,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,    -1,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     1,   177,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    68,    -1,
      70,    71,    72,    -1,    74,    -1,    -1,    77,    78,    79,
      80,    -1,    82,    83,    84,    85,    86,    87,    88,    -1,
      90,    91,    92,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,    -1,   103,   104,    -1,   106,   107,   108,   109,
     110,    -1,   112,   113,   114,   115,   116,   117,    -1,    -1,
     120,    -1,   122,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     1,   177,     3,     4,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
     175,     1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     3,   177,     5,     6,
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
     157,   158,   159,   160,   161,   162,   163,   164,    -1,   166,
     167,   168,   169,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    -1,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    -1,    -1,    -1,    -1,    30,    -1,    -1,    33,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    43,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    62,    63,
      64,    65,    -1,    67,    68,    -1,    70,    71,    72,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,   117,    -1,    -1,   120,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
      -1,   110,   111,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      26,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,   173,   174,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,   107,   108,    -1,   110,   111,    -1,   113,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,   173,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    48,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     3,   177,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,
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
      -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
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
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,
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
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,   171,   172,
      -1,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,   175,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    89,    -1,    -1,    -1,    93,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,    -1,   177,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,   175,     3,   177,     5,     6,
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
     117,    -1,    -1,    -1,    -1,    -1,   123,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,
      -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
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
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,   112,   113,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,   175,     3,   177,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,   111,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,   111,
      -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,    -1,   177,     3,     4,     5,     6,
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
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,    -1,
     177,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
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
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     3,   177,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
     150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,    -1,   174,    -1,     3,   177,     5,     6,
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
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     3,   177,     5,     6,     7,     8,     9,    10,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,
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
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
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
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
      -1,    -1,    -1,     1,    -1,     3,    -1,    -1,   141,   142,
     143,   144,   145,    -1,    12,    -1,    -1,   150,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,   159,    25,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    34,    -1,    36,   172,
      -1,   174,    40,    -1,   177,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,   141,   142,   143,   144,   145,   146,    -1,
     148,    -1,    -1,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
     168,   169,    -1,     3,   172,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,    -1,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,    -1,   172,     3,   174,     5,     6,     7,     8,     9,
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
      -1,    -1,    -1,   143,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,   164,   165,    25,    -1,    -1,    -1,
     170,    -1,   172,    -1,   174,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,    -1,   117,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   126,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   143,    -1,    -1,    -1,   147,    -1,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,
      -1,   170,    -1,   172,   173,   174,     3,    -1,     5,     6,
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
     117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,   143,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   164,    -1,    25,
      -1,    -1,    -1,   170,    -1,   172,    -1,   174,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   143,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,   164,    -1,
      25,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,     3,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      12,    -1,    87,    88,    -1,    -1,    -1,    19,    20,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    34,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,    -1,   174,
      -1,   103,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   126,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,   136,   137,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,    -1,   148,    -1,    -1,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
       3,   163,    -1,    -1,   166,   167,   168,   169,   170,    12,
      -1,    -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    96,    97,    -1,    99,    -1,   101,    19,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    32,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   126,   127,   128,   129,   130,   131,    -1,
      -1,   134,   135,   136,   137,    -1,   139,   140,   141,   142,
     143,   144,   145,   146,    -1,   148,    -1,    -1,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
     163,    -1,    -1,   166,   167,   168,   169,    -1,    12,    89,
      -1,    -1,    -1,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,    -1,   139,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,    -1,   139,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,
     102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,   136,   137,    -1,   139,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
     126,   127,   128,   129,   130,   131,    -1,    -1,   134,   135,
     136,   137,    -1,   139,   140,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,    -1,    -1,   134,   135,   136,   137,    -1,   139,
     140,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,   131,    -1,    -1,
     134,   135,   136,   137,    -1,   139,   140,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,   131,    -1,    -1,   134,   135,   136,   137,
      -1,   139,   140,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,
     102,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,   125,   126,   127,   128,   129,   130,   131,
      -1,    -1,   134,   135,   136,   137,    -1,   139,   140,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    89,   166,   167,   168,    93,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    32,    -1,   125,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    73,    -1,
     166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
     125,    -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    73,
     165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    12,    89,    -1,    -1,    -1,    93,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   121,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   121,
      -1,    -1,    48,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   121,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,   125,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,   125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    89,
     166,   167,   168,    93,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    32,    -1,   125,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    73,    32,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,    -1,    -1,
      89,    -1,    -1,    -1,    93,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,    -1,    89,    -1,    -1,    12,    93,    -1,   177,    -1,
      -1,    -1,    19,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,    32,   153,   154,   155,    -1,
     157,   158,   159,   160,    -1,    -1,   163,    -1,   125,   166,
     167,   168,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     177,    19,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    32,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
     177,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   125,    -1,   166,
     167,   168,    -1,   170,    -1,    -1,    -1,    -1,    89,    -1,
     177,    -1,    93,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,   165,   166,   167,
     168,    -1,    -1,   124,   125,    -1,    12,    -1,    -1,   177,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    32,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   177,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   102,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   125,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    -1,
     166,   167,   168,   125,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    89,    -1,    -1,    -1,    93,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
     125,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   121,    -1,    32,    -1,   125,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,   165,   166,   167,   168,   125,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,   125,    -1,
      -1,    -1,   173,    -1,    -1,    -1,   177,    89,    -1,    -1,
      -1,    93,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    13,   166,
     167,   168,   124,   125,    19,    -1,   173,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
     125,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,   125,    -1,    -1,    -1,    89,
      -1,    -1,   177,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   125,   166,   167,   168,    -1,
      -1,    -1,    -1,   173,    -1,    -1,    -1,   177,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    22,    -1,
      -1,    -1,    26,    27,    32,    33,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    12,    -1,    -1,    -1,    51,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,
      -1,    89,    86,    -1,    88,    93,    90,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    32,   112,    -1,
     114,    -1,   116,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   125,    -1,   166,   167,
     168,   165,    -1,    -1,    89,    -1,    -1,    -1,    93,   177,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
     125,    -1,    19,    -1,    -1,    -1,   175,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      12,   166,   167,   168,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    13,    -1,    -1,    -1,   102,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,   141,   142,    -1,   144,   145,   146,
     102,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,   125,    -1,    -1,    -1,    89,    -1,    -1,
     177,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   125,   166,   167,   168,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   177,    19,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    32,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   177,    19,    -1,    61,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,   125,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,   177,    93,    -1,    -1,   141,   142,
      -1,   144,   145,   146,   102,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    -1,   166,   167,   168,    -1,   125,    19,    -1,
      -1,    -1,   175,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
     141,   142,    -1,   144,   145,   146,   102,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    -1,   166,   167,   168,    -1,   125,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,   141,   142,    -1,   144,   145,   146,   102,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
      -1,   125,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,   102,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,   125,    -1,    -1,    -1,    89,    -1,    -1,
     177,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,   125,   166,   167,   168,    -1,    -1,    12,
      -1,    -1,    -1,   175,    -1,   177,    19,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    32,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   125,    89,   166,   167,   168,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,    -1,   163,
      -1,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,    -1,   163,    -1,    22,   166,
     167,   168,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    36,    -1,    38,    39,    -1,    -1,    22,    -1,
      44,    -1,    26,    27,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    66,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,    83,
      -1,    85,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    77,     1,    79,     3,    -1,    -1,    83,
      -1,    -1,    86,    -1,    88,   109,    90,    -1,   112,    -1,
      17,    18,   116,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    31,   109,    -1,    34,   112,    -1,
      -1,    -1,   116,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,   165,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,   165,    -1,    -1,    -1,    -1,    -1,     3,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    17,    18,   110,    20,    -1,   113,   114,    -1,    25,
     117,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    -1,   103,     3,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,   117,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    17,
      18,   106,    20,    -1,    -1,   110,    -1,    25,   113,    -1,
      -1,    -1,   117,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    17,    18,   106,    20,
      -1,    -1,   110,    -1,    25,   113,    -1,    -1,    -1,   117,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    17,    18,   106,    20,    -1,    -1,   110,
      -1,    25,   113,    -1,    -1,    -1,   117,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,    -1,   117
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   182,   183,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    33,    34,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      49,    50,    51,    53,    56,    57,    58,    59,    60,    62,
      63,    64,    65,    67,    68,    70,    71,    72,    77,    78,
      79,    80,    81,    82,    83,    86,    87,    88,    90,    91,
      92,    94,    95,    96,    97,    98,    99,   100,   101,   103,
     104,   106,   107,   108,   109,   110,   112,   113,   114,   116,
     117,   120,   122,   125,   141,   142,   143,   144,   145,   150,
     159,   162,   163,   164,   165,   168,   170,   172,   174,   177,
     184,   185,   186,   187,   188,   191,   192,   195,   196,   197,
     199,   200,   201,   206,   207,   210,   211,   215,   217,   220,
     227,   230,   231,   232,   233,   234,   235,   238,   239,   241,
     243,   246,   247,   248,   249,   250,   254,   255,   260,   261,
     262,   263,   266,   267,   278,   279,   281,   282,   283,   284,
     286,   287,   312,   313,   317,   318,   339,   340,   341,   342,
     343,   344,   345,   353,   354,   355,   356,   359,   360,   361,
     362,   363,   364,   365,   366,   368,   369,   370,   371,   372,
     165,    22,    36,    41,    42,    45,    56,    88,   101,   170,
     174,   246,   279,   339,   344,   354,   355,   356,   359,   361,
     362,   121,   346,   347,     3,   212,   356,   346,   356,   109,
     319,    90,   212,   186,   333,   356,     8,   190,   186,   172,
       3,    17,    18,    20,    25,    34,    40,    46,    50,    53,
      63,    70,    71,    78,    87,    96,    97,    99,   101,   103,
     106,   110,   113,   117,   214,   216,    11,    79,   123,   240,
     356,   240,   356,   240,   356,    27,   116,   242,   356,    83,
      86,   197,   172,   214,   214,   214,   172,   214,   320,   321,
      33,   201,   219,   356,   258,   259,   356,   356,    18,    78,
      96,   113,   356,   356,   356,     8,   172,   229,   228,     4,
     307,   332,   356,   107,   108,   165,   356,   358,   356,   219,
     356,   356,   356,   100,   172,   186,   356,   356,   187,   201,
     187,   201,   356,     8,   194,   242,   356,   358,   356,   356,
     356,   356,   356,     1,   171,   184,   202,   332,   111,   151,
     307,   334,   335,   358,   240,   332,   356,   367,   356,    81,
     186,   170,    47,    85,   115,   198,    26,   318,   356,     8,
     262,   356,   357,    56,   146,   264,   214,     1,    31,   114,
     214,   268,   271,   274,   277,   172,   295,    54,    74,    84,
     302,    27,    79,    90,   109,   303,    27,    79,    90,   109,
     301,   214,   314,   315,   320,   164,   165,   356,    12,    19,
      32,    89,    93,   125,   141,   142,   144,   145,   146,   148,
     149,   150,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   166,   167,   168,   177,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   169,   291,   172,   174,    89,    93,   356,
     356,   356,   214,   332,    56,   172,   186,   165,   170,   201,
      48,   346,   317,   165,   147,   165,   189,    22,    36,    39,
      88,   191,   197,   120,   215,   307,   336,   337,   338,   358,
     170,   214,   172,   219,    33,    48,   219,   121,   219,   349,
      33,    48,   219,   349,   219,   349,    48,   219,   349,   214,
     214,   102,   201,   102,   126,   201,   291,   198,   336,   172,
     172,   201,   186,   111,   172,   214,   322,   323,     1,   146,
     327,    48,   147,   186,   219,   147,   219,    13,   172,   172,
     219,   336,   230,   230,   147,   165,   356,   356,   165,   170,
     219,   172,   336,   165,   251,   251,   193,   191,   219,   219,
     165,   171,   171,   184,   147,   171,   356,   147,   173,   147,
     173,   175,   349,    48,   147,   175,   349,   124,   147,   175,
       8,     1,   171,   202,   208,   209,   356,   204,   356,    66,
      37,    73,   165,   262,   264,   109,   236,   284,   214,   332,
     170,   171,     8,   273,     8,   276,   126,   147,   171,    27,
      48,    52,    76,    79,    90,   109,   185,   296,   297,   298,
     299,   300,   303,    90,     1,   146,   326,    90,     1,     3,
      12,    17,    19,    20,    25,    40,    46,    53,    55,    63,
      70,    71,    87,    99,   103,   106,   110,   117,   141,   142,
     143,   144,   145,   146,   148,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   163,   166,   167,   168,   169,
     172,   213,   214,   216,   288,   289,   290,   291,   339,   126,
     316,   147,   165,   165,   356,   356,   356,   240,   356,   240,
     356,   356,   356,   356,   356,   356,   356,     3,    20,    34,
      63,   103,   109,   215,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,    69,   358,   358,   358,   358,   358,   336,   336,   240,
     356,   240,   356,    33,    48,    33,    48,   102,   172,    48,
     175,   214,    27,    48,    90,   116,   348,   351,   352,   356,
     372,   240,   356,   219,   320,   356,   191,   356,   126,   173,
     147,    48,   332,    45,   356,   240,   356,   172,   219,    45,
     356,   240,   356,   219,   219,   240,   356,   219,   126,   126,
     186,    35,   186,   356,    35,   356,    66,   173,   337,   214,
     244,   245,   323,   147,   173,    34,    50,    97,   101,   174,
     218,   328,   340,   126,   324,   356,   321,   258,   214,   317,
     356,   356,   173,   307,   356,     1,   256,   338,   173,    21,
     252,   191,   171,   173,   173,   334,   173,   334,   186,   175,
     240,   356,   175,   186,   356,   175,   356,   175,   356,   171,
     171,   147,   165,    13,   149,   147,   165,    13,    37,    73,
     214,   141,   142,   143,   144,   145,   159,   163,   168,   203,
     290,   291,   292,   356,   203,   205,   264,   170,   237,   317,
     165,   172,     1,   265,   272,   277,   275,   277,   356,   269,
     270,    48,    90,   299,   147,   173,   172,   214,   327,   174,
     218,   325,   340,   105,   304,   172,   294,   356,   141,   149,
     294,   294,   328,   340,   315,   172,   174,   165,   165,   165,
     165,   165,   165,   173,   175,    45,   240,   356,    45,   240,
     356,   337,   240,   356,   172,    48,    90,   147,   173,    17,
      20,    25,    46,    53,    63,    71,    87,    99,   110,   117,
     339,    89,    89,   346,   346,   165,   165,   307,   358,   338,
     356,   173,   356,    33,   219,    33,   219,   350,   351,   356,
      33,   219,   349,    33,   219,   349,   219,   349,   219,   349,
     356,   356,    35,   186,    35,    35,   186,   102,   201,   214,
     173,   147,   173,   173,   322,   327,     1,   332,    69,   358,
     214,   173,   173,   171,    75,   118,   171,   257,   173,   172,
     201,   214,   253,   186,   175,   349,   175,   349,   186,   124,
     208,   215,   170,   292,   356,   111,   356,   203,   205,   147,
     165,    13,   165,   170,   265,   314,   320,   337,   171,    31,
      83,    86,   114,   171,   185,   221,   224,   227,   277,   277,
     271,   274,   214,   298,   323,   146,   331,    61,   119,   308,
       1,   175,   332,   308,   297,   173,     3,   103,   289,   291,
     173,   175,   356,    33,    33,   356,    33,    33,   173,   175,
     175,   337,   351,   327,   339,   339,   219,   219,   219,   102,
      45,   356,    45,   356,   147,   173,   102,    45,   356,   219,
      45,   356,   219,   219,   219,   186,   186,   356,   186,    35,
     165,   165,   245,   201,   324,   175,    48,   175,   172,   172,
      33,   186,   332,   253,   146,   201,    45,   186,   356,   175,
      45,   186,   356,   175,   356,   203,    13,    37,    73,    37,
      73,   165,   165,   292,   356,   356,   265,   171,   165,   165,
     173,     8,   223,     8,   226,   221,   224,   331,   173,   174,
     218,   307,   330,   340,   151,   293,   324,     3,    92,   103,
     309,   310,   311,   356,   280,   175,   325,   340,   175,   285,
     173,   294,   294,   102,    45,    45,   102,    45,    45,    45,
      45,   173,   324,   356,   356,   356,   351,   356,   356,   356,
      35,   186,   356,   328,   340,   336,   336,   186,   219,   173,
     356,   356,   186,   356,   186,   171,   111,   356,   203,   205,
     203,   205,    13,   171,   165,   222,   227,   225,   227,   293,
     324,   331,   175,   332,   307,   356,   311,   119,   147,   126,
     152,   154,   155,   157,   158,    61,   200,   220,   306,   325,
     340,   165,   305,   306,   356,   356,   356,   356,   356,   356,
     102,   102,   102,   102,   186,   175,   173,   173,   201,   102,
     102,   165,   165,   165,   165,   356,   227,   227,   293,   324,
     307,   329,   330,   340,    48,   175,   356,   310,   311,   311,
     311,   311,   311,   311,   309,   102,   102,   102,   102,   102,
     102,   356,   356,   356,   356,   340,   164,   164,   356,   356,
     356,   329,   330,   175,   329
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   188,   189,   188,   190,   188,   191,
     191,   193,   192,   194,   192,   195,   196,   196,   196,   197,
     197,   197,   198,   198,   199,   200,   200,   200,   201,   202,
     202,   203,   203,   203,   203,   203,   203,   204,   204,   204,
     204,   204,   204,   205,   205,   206,   206,   206,   206,   206,
     206,   206,   207,   208,   208,   208,   208,   209,   209,   210,
     211,   211,   211,   211,   211,   211,   212,   212,   213,   213,
     213,   213,   213,   213,   214,   214,   214,   214,   214,   214,
     215,   215,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   218,   218,   218,   218,   219,   219,   220,
     220,   221,   222,   221,   223,   221,   225,   224,   226,   224,
     227,   227,   228,   227,   229,   227,   230,   230,   230,   230,
     230,   230,   230,   231,   231,   231,   231,   232,   233,   233,
     234,   235,   235,   235,   236,   235,   237,   235,   238,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   240,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   242,   242,   243,
     243,   244,   244,   245,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   246,   246,   246,   247,   247,   247,
     247,   247,   247,   248,   248,   248,   249,   249,   249,   250,
     250,   250,   250,   251,   251,   252,   252,   252,   253,   253,
     254,   255,   255,   256,   256,   257,   257,   257,   258,   258,
     258,   259,   259,   260,   261,   261,   262,   263,   263,   263,
     264,   264,   265,   265,   265,   265,   265,   266,   266,   267,
     268,   268,   269,   268,   268,   270,   268,   271,   272,   271,
     273,   271,   275,   274,   276,   274,   277,   277,   278,   280,
     279,   281,   282,   282,   282,   283,   285,   284,   286,   286,
     286,   286,   286,   287,   288,   288,   289,   289,   289,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   290,   290,
     290,   290,   290,   290,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   292,   292,   293,   293,
     293,   294,   294,   295,   296,   296,   297,   297,   298,   298,
     298,   298,   298,   298,   299,   299,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   301,   301,   301,   301,   301,
     301,   302,   302,   302,   303,   303,   303,   303,   303,   303,
     304,   304,   305,   305,   306,   306,   307,   308,   308,   308,
     308,   308,   309,   309,   310,   310,   310,   310,   310,   310,
     310,   311,   311,   312,   313,   313,   314,   314,   315,   316,
     316,   316,   317,   317,   317,   317,   317,   319,   318,   318,
     320,   320,   321,   321,   322,   322,   322,   323,   323,   323,
     324,   324,   324,   325,   325,   325,   325,   325,   325,   325,
     326,   326,   326,   326,   326,   327,   327,   327,   327,   327,
     328,   328,   328,   328,   329,   329,   329,   330,   330,   330,
     330,   330,   331,   331,   331,   331,   331,   332,   332,   332,
     332,   333,   333,   334,   334,   334,   335,   335,   336,   336,
     337,   337,   338,   338,   338,   338,   339,   339,   340,   340,
     340,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     343,   344,   345,   345,   345,   345,   345,   345,   345,   346,
     346,   347,   348,   348,   349,   350,   350,   351,   351,   351,
     352,   352,   352,   352,   352,   352,   353,   353,   353,   353,
     353,   354,   354,   354,   354,   354,   355,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   357,   357,   358,   358,   358,   359,
     359,   359,   359,   360,   360,   360,   360,   360,   361,   361,
     361,   362,   362,   362,   362,   362,   362,   363,   363,   363,
     363,   364,   364,   365,   365,   366,   366,   366,   366,   366,
     366,   366,   366,   366,   366,   366,   366,   366,   367,   367,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   368,   368,
     368,   368,   368,   369,   369,   369,   369,   369,   369,   369,
     370,   370,   370,   370,   371,   371,   371,   371,   372,   372,
     372,   372,   372,   372,   372
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     0,     3,     3,     1,     3,     0,     1,     4,     5,
       4,     5,     6,     6,     0,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     0,     2,     2,
       4,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     3,     1,     2,     1,     3,     2,     0,
       2,     2,     1,     2,     1,     1,     1,     0,     5,     3,
       1,     3,     3,     5,     1,     1,     3,     1,     2,     3,
       0,     2,     2,     3,     2,     4,     3,     3,     4,     3,
       0,     2,     2,     2,     1,     0,     2,     2,     2,     1,
       4,     4,     6,     3,     0,     1,     1,     3,     4,     3,
       4,     6,     0,     2,     2,     2,     2,     1,     1,     3,
       3,     1,     3,     1,     1,     1,     3,     3,     0,     1,
       1,     3,     3,     3,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     2,     4,     4,     4,
       5,     2,     2,     1,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     6,     6,     4,     9,     9,     7,     6,
       6,     4,     9,     9,     7,     4,     6,     6,     9,     9,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     4,     1,     3,     4,     1,     3,     4,     3,     3,
       1,     1,     2,     1,     2,     1,     1,     2,     2,     2,
       2,     2,     8,     8,     9,     9,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     3,     3,     3,
       2,     2,     2,     1,     0,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     4,     4,
       4,     3,     3,     3,     3,     5,     5,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     3,     4,     3,     4,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1
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
#line 612 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6455 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 616 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6461 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 617 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6467 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 624 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6475 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 632 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6483 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 636 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6492 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 644 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6498 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: unstable_decl_stmt  */
#line 645 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6504 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: include_module_stmt  */
#line 646 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6510 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: block_stmt  */
#line 647 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6516 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: use_stmt  */
#line 648 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6522 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: import_stmt  */
#line 649 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6528 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: require_stmt  */
#line 650 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6534 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: extern_block_stmt  */
#line 651 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6540 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 652 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6546 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 653 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6552 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: TDEFER stmt  */
#line 655 "chpl.ypp"
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
#line 6569 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: try_stmt  */
#line 667 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6575 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: return_stmt  */
#line 668 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6581 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 670 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6594 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 679 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6607 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TLABEL ident_def stmt  */
#line 688 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6615 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: TYIELD opt_try_expr TSEMI  */
#line 692 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6626 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: error TSEMI  */
#line 699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6635 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: assignment_stmt  */
#line 706 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6641 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: if_stmt  */
#line 707 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: loop_stmt  */
#line 708 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6653 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: select_stmt  */
#line 709 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6659 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: manage_stmt  */
#line 710 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6665 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 711 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6671 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: throw_stmt  */
#line 712 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6677 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 714 "chpl.ypp"
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
#line 6695 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 728 "chpl.ypp"
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
#line 6714 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 743 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6726 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 751 "chpl.ypp"
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
#line 6742 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL do_stmt  */
#line 763 "chpl.ypp"
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
#line 6757 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TON expr do_stmt  */
#line 774 "chpl.ypp"
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
#line 6773 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 786 "chpl.ypp"
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
#line 6789 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL do_stmt  */
#line 798 "chpl.ypp"
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
#line 6804 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSYNC stmt  */
#line 809 "chpl.ypp"
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
#line 6821 "bison-chpl-lib.cpp"
    break;

  case 45: /* $@1: %empty  */
#line 826 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 46: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 decl_base  */
#line 830 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6837 "bison-chpl-lib.cpp"
    break;

  case 47: /* $@2: %empty  */
#line 834 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6845 "bison-chpl-lib.cpp"
    break;

  case 48: /* deprecated_decl_stmt: TDEPRECATED $@2 decl_base  */
#line 838 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6853 "bison-chpl-lib.cpp"
    break;

  case 51: /* $@3: %empty  */
#line 850 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 6861 "bison-chpl-lib.cpp"
    break;

  case 52: /* unstable_decl_stmt: TUNSTABLE STRINGLITERAL $@3 decl_base  */
#line 854 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6869 "bison-chpl-lib.cpp"
    break;

  case 53: /* $@4: %empty  */
#line 858 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 6877 "bison-chpl-lib.cpp"
    break;

  case 54: /* unstable_decl_stmt: TUNSTABLE $@4 decl_base  */
#line 862 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6885 "bison-chpl-lib.cpp"
    break;

  case 55: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 869 "chpl.ypp"
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
#line 6906 "bison-chpl-lib.cpp"
    break;

  case 56: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 889 "chpl.ypp"
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
#line 6925 "bison-chpl-lib.cpp"
    break;

  case 57: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 904 "chpl.ypp"
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
#line 6944 "bison-chpl-lib.cpp"
    break;

  case 58: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 919 "chpl.ypp"
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
#line 6961 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_access_control: %empty  */
#line 934 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6967 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_access_control: TPUBLIC  */
#line 935 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6974 "bison-chpl-lib.cpp"
    break;

  case 61: /* opt_access_control: TPRIVATE  */
#line 937 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6981 "bison-chpl-lib.cpp"
    break;

  case 62: /* opt_prototype: %empty  */
#line 942 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 63: /* opt_prototype: TPROTOTYPE  */
#line 943 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6994 "bison-chpl-lib.cpp"
    break;

  case 64: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 949 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7004 "bison-chpl-lib.cpp"
    break;

  case 65: /* block_stmt_body: TLCBR TRCBR  */
#line 967 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7012 "bison-chpl-lib.cpp"
    break;

  case 66: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 971 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7020 "bison-chpl-lib.cpp"
    break;

  case 67: /* block_stmt_body: TLCBR error TRCBR  */
#line 975 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7032 "bison-chpl-lib.cpp"
    break;

  case 68: /* block_stmt: block_stmt_body  */
#line 986 "chpl.ypp"
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
#line 7051 "bison-chpl-lib.cpp"
    break;

  case 69: /* stmt_ls: toplevel_stmt  */
#line 1004 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7057 "bison-chpl-lib.cpp"
    break;

  case 70: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1005 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7063 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: expr  */
#line 1010 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7071 "bison-chpl-lib.cpp"
    break;

  case 72: /* renames_ls: all_op_name  */
#line 1014 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7079 "bison-chpl-lib.cpp"
    break;

  case 73: /* renames_ls: expr TAS expr  */
#line 1018 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7088 "bison-chpl-lib.cpp"
    break;

  case 74: /* renames_ls: renames_ls TCOMMA expr  */
#line 1023 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7096 "bison-chpl-lib.cpp"
    break;

  case 75: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1027 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7104 "bison-chpl-lib.cpp"
    break;

  case 76: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1031 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: expr  */
#line 1041 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7122 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_renames_ls: expr TAS expr  */
#line 1046 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7132 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1052 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7143 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1059 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7152 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1064 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7163 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1071 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7175 "bison-chpl-lib.cpp"
    break;

  case 83: /* opt_only_ls: %empty  */
#line 1081 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7181 "bison-chpl-lib.cpp"
    break;

  case 84: /* opt_only_ls: renames_ls  */
#line 1082 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7187 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1087 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7196 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1092 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7208 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1100 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7221 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1109 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7235 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1119 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7247 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1127 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7260 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1136 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7274 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1149 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7283 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_expr: expr  */
#line 1157 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7291 "bison-chpl-lib.cpp"
    break;

  case 94: /* import_expr: expr TDOT all_op_name  */
#line 1161 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7300 "bison-chpl-lib.cpp"
    break;

  case 95: /* import_expr: expr TAS ident_use  */
#line 1166 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7310 "bison-chpl-lib.cpp"
    break;

  case 96: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1172 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7320 "bison-chpl-lib.cpp"
    break;

  case 97: /* import_ls: import_expr  */
#line 1180 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7326 "bison-chpl-lib.cpp"
    break;

  case 98: /* import_ls: import_ls TCOMMA import_expr  */
#line 1181 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7332 "bison-chpl-lib.cpp"
    break;

  case 99: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1186 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7342 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1195 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7350 "bison-chpl-lib.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1199 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7358 "bison-chpl-lib.cpp"
    break;

  case 102: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1203 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7366 "bison-chpl-lib.cpp"
    break;

  case 103: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1207 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7374 "bison-chpl-lib.cpp"
    break;

  case 104: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1211 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7382 "bison-chpl-lib.cpp"
    break;

  case 105: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1215 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7390 "bison-chpl-lib.cpp"
    break;

  case 106: /* opt_label_ident: %empty  */
#line 1223 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7396 "bison-chpl-lib.cpp"
    break;

  case 107: /* opt_label_ident: TIDENT  */
#line 1224 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7402 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_fn_def: TIDENT  */
#line 1228 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7408 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_fn_def: TNONE  */
#line 1229 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'none'"); }
#line 7414 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_fn_def: TTHIS  */
#line 1230 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_fn_def: TFALSE  */
#line 1231 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'false'"); }
#line 7426 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_fn_def: TTRUE  */
#line 1232 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'true'"); }
#line 7432 "bison-chpl-lib.cpp"
    break;

  case 113: /* ident_fn_def: internal_type_ident_def  */
#line 1233 "chpl.ypp"
                           { std::string msg = "attempt to redefine reserved type '";
                             msg += (yyvsp[0].uniqueStr).c_str();
                             msg += "'";
                             (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), msg); }
#line 7441 "bison-chpl-lib.cpp"
    break;

  case 114: /* ident_def: TIDENT  */
#line 1239 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7447 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_def: TNONE  */
#line 1240 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'none'"); }
#line 7453 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_def: TTHIS  */
#line 1241 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'this'"); }
#line 7459 "bison-chpl-lib.cpp"
    break;

  case 117: /* ident_def: TFALSE  */
#line 1242 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'false'"); }
#line 7465 "bison-chpl-lib.cpp"
    break;

  case 118: /* ident_def: TTRUE  */
#line 1243 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'true'"); }
#line 7471 "bison-chpl-lib.cpp"
    break;

  case 119: /* ident_def: internal_type_ident_def  */
#line 1244 "chpl.ypp"
                           { std::string msg = "attempt to redefine reserved type '";
                             msg += (yyvsp[0].uniqueStr).c_str();
                             msg += "'";
                             (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), msg); }
#line 7480 "bison-chpl-lib.cpp"
    break;

  case 120: /* ident_use: TIDENT  */
#line 1259 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7486 "bison-chpl-lib.cpp"
    break;

  case 121: /* ident_use: TTHIS  */
#line 1260 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7492 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TBOOL  */
#line 1293 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7498 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TENUM  */
#line 1294 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7504 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TINT  */
#line 1295 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TUINT  */
#line 1296 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7516 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TREAL  */
#line 1297 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7522 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TIMAG  */
#line 1298 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7528 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TCOMPLEX  */
#line 1299 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 148: /* scalar_type: TBYTES  */
#line 1300 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7540 "bison-chpl-lib.cpp"
    break;

  case 149: /* scalar_type: TSTRING  */
#line 1301 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 150: /* scalar_type: TLOCALE  */
#line 1302 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7552 "bison-chpl-lib.cpp"
    break;

  case 151: /* scalar_type: TNOTHING  */
#line 1303 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 152: /* scalar_type: TVOID  */
#line 1304 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7564 "bison-chpl-lib.cpp"
    break;

  case 157: /* do_stmt: TDO stmt  */
#line 1318 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 158: /* do_stmt: block_stmt  */
#line 1319 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 159: /* return_stmt: TRETURN TSEMI  */
#line 1324 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7587 "bison-chpl-lib.cpp"
    break;

  case 160: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1331 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7598 "bison-chpl-lib.cpp"
    break;

  case 162: /* $@5: %empty  */
#line 1342 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7606 "bison-chpl-lib.cpp"
    break;

  case 163: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@5 class_level_stmt  */
#line 1346 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7614 "bison-chpl-lib.cpp"
    break;

  case 164: /* $@6: %empty  */
#line 1350 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7622 "bison-chpl-lib.cpp"
    break;

  case 165: /* deprecated_class_level_stmt: TDEPRECATED $@6 class_level_stmt  */
#line 1354 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7630 "bison-chpl-lib.cpp"
    break;

  case 166: /* $@7: %empty  */
#line 1361 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 7638 "bison-chpl-lib.cpp"
    break;

  case 167: /* unstable_class_level_stmt: TUNSTABLE STRINGLITERAL $@7 class_level_stmt  */
#line 1365 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7646 "bison-chpl-lib.cpp"
    break;

  case 168: /* $@8: %empty  */
#line 1369 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 7654 "bison-chpl-lib.cpp"
    break;

  case 169: /* unstable_class_level_stmt: TUNSTABLE $@8 class_level_stmt  */
#line 1373 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7662 "bison-chpl-lib.cpp"
    break;

  case 170: /* class_level_stmt: TSEMI  */
#line 1380 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7670 "bison-chpl-lib.cpp"
    break;

  case 171: /* class_level_stmt: inner_class_level_stmt  */
#line 1384 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7680 "bison-chpl-lib.cpp"
    break;

  case 172: /* $@9: %empty  */
#line 1389 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7687 "bison-chpl-lib.cpp"
    break;

  case 173: /* class_level_stmt: TPUBLIC $@9 inner_class_level_stmt  */
#line 1391 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7696 "bison-chpl-lib.cpp"
    break;

  case 174: /* $@10: %empty  */
#line 1395 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7703 "bison-chpl-lib.cpp"
    break;

  case 175: /* class_level_stmt: TPRIVATE $@10 inner_class_level_stmt  */
#line 1397 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7712 "bison-chpl-lib.cpp"
    break;

  case 183: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1415 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7721 "bison-chpl-lib.cpp"
    break;

  case 184: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1420 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7730 "bison-chpl-lib.cpp"
    break;

  case 185: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1425 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7739 "bison-chpl-lib.cpp"
    break;

  case 186: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1430 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7747 "bison-chpl-lib.cpp"
    break;

  case 187: /* forwarding_decl_start: TFORWARDING  */
#line 1437 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 188: /* extern_or_export: TEXTERN  */
#line 1444 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7762 "bison-chpl-lib.cpp"
    break;

  case 189: /* extern_or_export: TEXPORT  */
#line 1445 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7768 "bison-chpl-lib.cpp"
    break;

  case 190: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1450 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 191: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1460 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7787 "bison-chpl-lib.cpp"
    break;

  case 192: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1466 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7798 "bison-chpl-lib.cpp"
    break;

  case 193: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1473 "chpl.ypp"
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

  case 194: /* $@11: %empty  */
#line 1491 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7828 "bison-chpl-lib.cpp"
    break;

  case 195: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1495 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7837 "bison-chpl-lib.cpp"
    break;

  case 196: /* $@12: %empty  */
#line 1500 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7846 "bison-chpl-lib.cpp"
    break;

  case 197: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@12 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1505 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7855 "bison-chpl-lib.cpp"
    break;

  case 198: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1513 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7863 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1520 "chpl.ypp"
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

  case 200: /* loop_stmt: TWHILE expr do_stmt  */
#line 1538 "chpl.ypp"
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

  case 201: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1551 "chpl.ypp"
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

  case 202: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1564 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7927 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1568 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7935 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1572 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7943 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1576 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7951 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1580 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7959 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFOR expr do_stmt  */
#line 1584 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7967 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1588 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7975 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1592 "chpl.ypp"
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

  case 210: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1610 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8005 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1614 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8013 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1618 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8021 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1622 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8029 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFORALL expr do_stmt  */
#line 1626 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1630 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8045 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1634 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8053 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1638 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8061 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1642 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8069 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1646 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8077 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1650 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8085 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1654 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8093 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1658 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8101 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1662 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8109 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1666 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8117 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1670 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8125 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8133 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1678 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8141 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1682 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8149 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1686 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8157 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1690 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8165 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1694 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8173 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1698 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8182 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1703 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8191 "bison-chpl-lib.cpp"
    break;

  case 234: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1711 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8201 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF expr TTHEN stmt  */
#line 1720 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8209 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr block_stmt  */
#line 1724 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8217 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1728 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8225 "bison-chpl-lib.cpp"
    break;

  case 238: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1732 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8233 "bison-chpl-lib.cpp"
    break;

  case 239: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1736 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8241 "bison-chpl-lib.cpp"
    break;

  case 240: /* if_stmt: TIF ifvar block_stmt  */
#line 1740 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8249 "bison-chpl-lib.cpp"
    break;

  case 241: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1744 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8257 "bison-chpl-lib.cpp"
    break;

  case 242: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1748 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8265 "bison-chpl-lib.cpp"
    break;

  case 243: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1752 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8274 "bison-chpl-lib.cpp"
    break;

  case 244: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1757 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8283 "bison-chpl-lib.cpp"
    break;

  case 245: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1762 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8292 "bison-chpl-lib.cpp"
    break;

  case 246: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1767 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8301 "bison-chpl-lib.cpp"
    break;

  case 247: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1775 "chpl.ypp"
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

  case 248: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1790 "chpl.ypp"
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

  case 249: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1808 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8347 "bison-chpl-lib.cpp"
    break;

  case 250: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1812 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8355 "bison-chpl-lib.cpp"
    break;

  case 251: /* ifc_formal_ls: ifc_formal  */
#line 1818 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8361 "bison-chpl-lib.cpp"
    break;

  case 252: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1819 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8367 "bison-chpl-lib.cpp"
    break;

  case 253: /* ifc_formal: ident_def  */
#line 1824 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8373 "bison-chpl-lib.cpp"
    break;

  case 266: /* implements_type_ident: implements_type_error_ident  */
#line 1842 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8385 "bison-chpl-lib.cpp"
    break;

  case 273: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1871 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8393 "bison-chpl-lib.cpp"
    break;

  case 274: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1875 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8401 "bison-chpl-lib.cpp"
    break;

  case 275: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1879 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8409 "bison-chpl-lib.cpp"
    break;

  case 276: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1886 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8417 "bison-chpl-lib.cpp"
    break;

  case 277: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1890 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8425 "bison-chpl-lib.cpp"
    break;

  case 278: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1894 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8433 "bison-chpl-lib.cpp"
    break;

  case 279: /* try_stmt: TTRY tryable_stmt  */
#line 1901 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8441 "bison-chpl-lib.cpp"
    break;

  case 280: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1905 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8449 "bison-chpl-lib.cpp"
    break;

  case 281: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1909 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8457 "bison-chpl-lib.cpp"
    break;

  case 282: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1913 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8465 "bison-chpl-lib.cpp"
    break;

  case 283: /* catch_expr_ls: %empty  */
#line 1919 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8471 "bison-chpl-lib.cpp"
    break;

  case 284: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1920 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8477 "bison-chpl-lib.cpp"
    break;

  case 285: /* catch_expr: TCATCH block_stmt  */
#line 1925 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8485 "bison-chpl-lib.cpp"
    break;

  case 286: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1929 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8493 "bison-chpl-lib.cpp"
    break;

  case 287: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1933 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8501 "bison-chpl-lib.cpp"
    break;

  case 288: /* catch_expr_inner: ident_def  */
#line 1940 "chpl.ypp"
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
#line 8518 "bison-chpl-lib.cpp"
    break;

  case 289: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1953 "chpl.ypp"
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
#line 8535 "bison-chpl-lib.cpp"
    break;

  case 290: /* throw_stmt: TTHROW expr TSEMI  */
#line 1969 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8545 "bison-chpl-lib.cpp"
    break;

  case 291: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1977 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8553 "bison-chpl-lib.cpp"
    break;

  case 292: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1981 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8563 "bison-chpl-lib.cpp"
    break;

  case 293: /* when_stmt_ls: %empty  */
#line 1989 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8569 "bison-chpl-lib.cpp"
    break;

  case 294: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1990 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8575 "bison-chpl-lib.cpp"
    break;

  case 295: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1995 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 296: /* when_stmt: TOTHERWISE stmt  */
#line 1999 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8592 "bison-chpl-lib.cpp"
    break;

  case 297: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2004 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8601 "bison-chpl-lib.cpp"
    break;

  case 298: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2012 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8610 "bison-chpl-lib.cpp"
    break;

  case 299: /* manager_expr: expr TAS ident_def  */
#line 2017 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8618 "bison-chpl-lib.cpp"
    break;

  case 300: /* manager_expr: expr  */
#line 2021 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 301: /* manager_expr_ls: manager_expr  */
#line 2027 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8632 "bison-chpl-lib.cpp"
    break;

  case 302: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2028 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8638 "bison-chpl-lib.cpp"
    break;

  case 303: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2033 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8646 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2042 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8655 "bison-chpl-lib.cpp"
    break;

  case 305: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2047 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8666 "bison-chpl-lib.cpp"
    break;

  case 306: /* class_start: class_tag ident_def  */
#line 2058 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8674 "bison-chpl-lib.cpp"
    break;

  case 307: /* class_tag: TCLASS  */
#line 2064 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8680 "bison-chpl-lib.cpp"
    break;

  case 308: /* class_tag: TRECORD  */
#line 2065 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8686 "bison-chpl-lib.cpp"
    break;

  case 309: /* class_tag: TUNION  */
#line 2066 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8692 "bison-chpl-lib.cpp"
    break;

  case 310: /* opt_inherit: %empty  */
#line 2070 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8698 "bison-chpl-lib.cpp"
    break;

  case 311: /* opt_inherit: TCOLON expr_ls  */
#line 2071 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8704 "bison-chpl-lib.cpp"
    break;

  case 312: /* class_level_stmt_ls: %empty  */
#line 2075 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8713 "bison-chpl-lib.cpp"
    break;

  case 313: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2080 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8721 "bison-chpl-lib.cpp"
    break;

  case 314: /* class_level_stmt_ls: class_level_stmt_ls unstable_class_level_stmt  */
#line 2084 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 315: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2088 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8737 "bison-chpl-lib.cpp"
    break;

  case 316: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls unstable_class_level_stmt  */
#line 2092 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8745 "bison-chpl-lib.cpp"
    break;

  case 317: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2099 "chpl.ypp"
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
#line 8766 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2116 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8780 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2129 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 320: /* enum_ls: deprecated_enum_item  */
#line 2136 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8797 "bison-chpl-lib.cpp"
    break;

  case 321: /* enum_ls: enum_ls TCOMMA  */
#line 2141 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8807 "bison-chpl-lib.cpp"
    break;

  case 322: /* $@13: %empty  */
#line 2147 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8816 "bison-chpl-lib.cpp"
    break;

  case 323: /* enum_ls: enum_ls TCOMMA $@13 deprecated_enum_item  */
#line 2152 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8825 "bison-chpl-lib.cpp"
    break;

  case 324: /* enum_ls: unstable_enum_item  */
#line 2158 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8834 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@14: %empty  */
#line 2163 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8843 "bison-chpl-lib.cpp"
    break;

  case 326: /* enum_ls: enum_ls TCOMMA $@14 unstable_enum_item  */
#line 2168 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8852 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@15: %empty  */
#line 2178 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 329: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@15 enum_item  */
#line 2182 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8868 "bison-chpl-lib.cpp"
    break;

  case 330: /* $@16: %empty  */
#line 2186 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8876 "bison-chpl-lib.cpp"
    break;

  case 331: /* deprecated_enum_item: TDEPRECATED $@16 enum_item  */
#line 2190 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8884 "bison-chpl-lib.cpp"
    break;

  case 332: /* $@17: %empty  */
#line 2197 "chpl.ypp"
  {
    context->noteUnstable((yyloc), (yyvsp[0].expr));
  }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 333: /* unstable_enum_item: TUNSTABLE STRINGLITERAL $@17 enum_item  */
#line 2201 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8900 "bison-chpl-lib.cpp"
    break;

  case 334: /* $@18: %empty  */
#line 2205 "chpl.ypp"
  {
    context->noteUnstable((yyloc), nullptr);
  }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 335: /* unstable_enum_item: TUNSTABLE $@18 enum_item  */
#line 2209 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8916 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_item: ident_def  */
#line 2216 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8927 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_item: ident_def TASSIGN expr  */
#line 2223 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8940 "bison-chpl-lib.cpp"
    break;

  case 338: /* lambda_decl_start: TLAMBDA  */
#line 2235 "chpl.ypp"
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
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 339: /* $@19: %empty  */
#line 2251 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8966 "bison-chpl-lib.cpp"
    break;

  case 340: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_lifetime_where $@19 function_body_stmt  */
#line 2257 "chpl.ypp"
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
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 342: /* linkage_spec: linkage_spec_empty  */
#line 2280 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8994 "bison-chpl-lib.cpp"
    break;

  case 343: /* linkage_spec: TINLINE  */
#line 2281 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 9001 "bison-chpl-lib.cpp"
    break;

  case 344: /* linkage_spec: TOVERRIDE  */
#line 2283 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 9008 "bison-chpl-lib.cpp"
    break;

  case 345: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2289 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9016 "bison-chpl-lib.cpp"
    break;

  case 346: /* $@20: %empty  */
#line 2298 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 347: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@20 opt_function_body_stmt  */
#line 2308 "chpl.ypp"
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
#line 9053 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2330 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9065 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2338 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9077 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2346 "chpl.ypp"
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
#line 9092 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2357 "chpl.ypp"
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
#line 9107 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2368 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9117 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2377 "chpl.ypp"
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
#line 9133 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2392 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9139 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_ident: ident_def TBANG  */
#line 2399 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9149 "bison-chpl-lib.cpp"
    break;

  case 398: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2456 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9155 "bison-chpl-lib.cpp"
    break;

  case 399: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2457 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9161 "bison-chpl-lib.cpp"
    break;

  case 400: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2458 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9167 "bison-chpl-lib.cpp"
    break;

  case 401: /* opt_formal_ls: %empty  */
#line 2462 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9173 "bison-chpl-lib.cpp"
    break;

  case 402: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2463 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9179 "bison-chpl-lib.cpp"
    break;

  case 403: /* req_formal_ls: TLP formal_ls TRP  */
#line 2467 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9185 "bison-chpl-lib.cpp"
    break;

  case 404: /* formal_ls_inner: formal  */
#line 2471 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9191 "bison-chpl-lib.cpp"
    break;

  case 405: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2472 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9197 "bison-chpl-lib.cpp"
    break;

  case 406: /* formal_ls: %empty  */
#line 2476 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9203 "bison-chpl-lib.cpp"
    break;

  case 407: /* formal_ls: formal_ls_inner  */
#line 2477 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9209 "bison-chpl-lib.cpp"
    break;

  case 408: /* formal: opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2482 "chpl.ypp"
  {
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 409: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2491 "chpl.ypp"
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
#line 9237 "bison-chpl-lib.cpp"
    break;

  case 410: /* formal: opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2502 "chpl.ypp"
  {
    (yyval.expr) = VarArgFormal::build(BUILDER, LOC((yyloc)),
                             /*attributes*/ nullptr,
                             /*name*/ (yyvsp[-2].uniqueStr),
                             /*intent*/ (yyvsp[-3].intentTag),
                             toOwned((yyvsp[-1].expr)),
                             toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9251 "bison-chpl-lib.cpp"
    break;

  case 411: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2512 "chpl.ypp"
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
#line 9267 "bison-chpl-lib.cpp"
    break;

  case 412: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2525 "chpl.ypp"
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
#line 9282 "bison-chpl-lib.cpp"
    break;

  case 413: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type formal_var_arg_expr  */
#line 2537 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
  }
#line 9290 "bison-chpl-lib.cpp"
    break;

  case 414: /* opt_formal_intent_tag: %empty  */
#line 2543 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9299 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_formal_intent_tag: required_intent_tag  */
#line 2548 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9308 "bison-chpl-lib.cpp"
    break;

  case 416: /* required_intent_tag: TIN  */
#line 2555 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9314 "bison-chpl-lib.cpp"
    break;

  case 417: /* required_intent_tag: TINOUT  */
#line 2556 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9320 "bison-chpl-lib.cpp"
    break;

  case 418: /* required_intent_tag: TOUT  */
#line 2557 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9326 "bison-chpl-lib.cpp"
    break;

  case 419: /* required_intent_tag: TCONST TIN  */
#line 2558 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 420: /* required_intent_tag: TCONST TREF  */
#line 2559 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9338 "bison-chpl-lib.cpp"
    break;

  case 421: /* required_intent_tag: TCONST  */
#line 2560 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9344 "bison-chpl-lib.cpp"
    break;

  case 422: /* required_intent_tag: TPARAM  */
#line 2561 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 423: /* required_intent_tag: TREF  */
#line 2562 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 424: /* required_intent_tag: TTYPE  */
#line 2563 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_this_intent_tag: %empty  */
#line 2567 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9368 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_this_intent_tag: TPARAM  */
#line 2568 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9374 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_this_intent_tag: TREF  */
#line 2569 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9380 "bison-chpl-lib.cpp"
    break;

  case 428: /* opt_this_intent_tag: TCONST TREF  */
#line 2570 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9386 "bison-chpl-lib.cpp"
    break;

  case 429: /* opt_this_intent_tag: TCONST  */
#line 2571 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9392 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_this_intent_tag: TTYPE  */
#line 2572 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9398 "bison-chpl-lib.cpp"
    break;

  case 431: /* proc_iter_or_op: TPROC  */
#line 2576 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9404 "bison-chpl-lib.cpp"
    break;

  case 432: /* proc_iter_or_op: TITER  */
#line 2577 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9410 "bison-chpl-lib.cpp"
    break;

  case 433: /* proc_iter_or_op: TOPERATOR  */
#line 2578 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9416 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_ret_tag: %empty  */
#line 2582 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9422 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_ret_tag: TCONST  */
#line 2583 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 9428 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_ret_tag: TCONST TREF  */
#line 2584 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 9434 "bison-chpl-lib.cpp"
    break;

  case 437: /* opt_ret_tag: TREF  */
#line 2585 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 9440 "bison-chpl-lib.cpp"
    break;

  case 438: /* opt_ret_tag: TPARAM  */
#line 2586 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9446 "bison-chpl-lib.cpp"
    break;

  case 439: /* opt_ret_tag: TTYPE  */
#line 2587 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9452 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_throws_error: %empty  */
#line 2591 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9458 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_throws_error: TTHROWS  */
#line 2592 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9464 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_function_body_stmt: TSEMI  */
#line 2595 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9470 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_function_body_stmt: function_body_stmt  */
#line 2596 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9476 "bison-chpl-lib.cpp"
    break;

  case 444: /* function_body_stmt: block_stmt_body  */
#line 2600 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9482 "bison-chpl-lib.cpp"
    break;

  case 445: /* function_body_stmt: return_stmt  */
#line 2601 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9488 "bison-chpl-lib.cpp"
    break;

  case 446: /* query_expr: TQUERIEDIDENT  */
#line 2605 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9494 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_lifetime_where: %empty  */
#line 2610 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9500 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_lifetime_where: TWHERE expr  */
#line 2612 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9506 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2614 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9512 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2616 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9518 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2618 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9524 "bison-chpl-lib.cpp"
    break;

  case 452: /* lifetime_components_expr: lifetime_expr  */
#line 2623 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9530 "bison-chpl-lib.cpp"
    break;

  case 453: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2625 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9536 "bison-chpl-lib.cpp"
    break;

  case 454: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2630 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9542 "bison-chpl-lib.cpp"
    break;

  case 455: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2632 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9548 "bison-chpl-lib.cpp"
    break;

  case 456: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2634 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9554 "bison-chpl-lib.cpp"
    break;

  case 457: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2636 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9560 "bison-chpl-lib.cpp"
    break;

  case 458: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2638 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9566 "bison-chpl-lib.cpp"
    break;

  case 459: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2640 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 460: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2642 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9578 "bison-chpl-lib.cpp"
    break;

  case 461: /* lifetime_ident: TIDENT  */
#line 2646 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9584 "bison-chpl-lib.cpp"
    break;

  case 462: /* lifetime_ident: TTHIS  */
#line 2647 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9590 "bison-chpl-lib.cpp"
    break;

  case 463: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2652 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9599 "bison-chpl-lib.cpp"
    break;

  case 464: /* type_alias_decl_stmt_start: TTYPE  */
#line 2661 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9607 "bison-chpl-lib.cpp"
    break;

  case 465: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2665 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 466: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2673 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9624 "bison-chpl-lib.cpp"
    break;

  case 467: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2677 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 468: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2684 "chpl.ypp"
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
#line 9654 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_init_type: %empty  */
#line 2704 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9660 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_init_type: TASSIGN type_level_expr  */
#line 2706 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9666 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_init_type: TASSIGN array_type  */
#line 2708 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9675 "bison-chpl-lib.cpp"
    break;

  case 472: /* var_decl_type: TPARAM  */
#line 2715 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9681 "bison-chpl-lib.cpp"
    break;

  case 473: /* var_decl_type: TCONST TREF  */
#line 2716 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9687 "bison-chpl-lib.cpp"
    break;

  case 474: /* var_decl_type: TREF  */
#line 2717 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9693 "bison-chpl-lib.cpp"
    break;

  case 475: /* var_decl_type: TCONST  */
#line 2718 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9699 "bison-chpl-lib.cpp"
    break;

  case 476: /* var_decl_type: TVAR  */
#line 2719 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9705 "bison-chpl-lib.cpp"
    break;

  case 477: /* $@21: %empty  */
#line 2724 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9714 "bison-chpl-lib.cpp"
    break;

  case 478: /* var_decl_stmt: TCONFIG $@21 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2728 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9723 "bison-chpl-lib.cpp"
    break;

  case 479: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2733 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9732 "bison-chpl-lib.cpp"
    break;

  case 480: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2741 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9740 "bison-chpl-lib.cpp"
    break;

  case 481: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2745 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9748 "bison-chpl-lib.cpp"
    break;

  case 482: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2752 "chpl.ypp"
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
#line 9768 "bison-chpl-lib.cpp"
    break;

  case 483: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2768 "chpl.ypp"
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
#line 9786 "bison-chpl-lib.cpp"
    break;

  case 484: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2785 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9794 "bison-chpl-lib.cpp"
    break;

  case 485: /* tuple_var_decl_component: ident_def  */
#line 2789 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9802 "bison-chpl-lib.cpp"
    break;

  case 486: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2793 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9810 "bison-chpl-lib.cpp"
    break;

  case 487: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2800 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9816 "bison-chpl-lib.cpp"
    break;

  case 488: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2802 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9822 "bison-chpl-lib.cpp"
    break;

  case 489: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2804 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9828 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_init_expr: %empty  */
#line 2810 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9834 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2811 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9840 "bison-chpl-lib.cpp"
    break;

  case 492: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2812 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9846 "bison-chpl-lib.cpp"
    break;

  case 493: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2818 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9854 "bison-chpl-lib.cpp"
    break;

  case 494: /* ret_array_type: TLSBR TRSBR  */
#line 2822 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 9862 "bison-chpl-lib.cpp"
    break;

  case 495: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2826 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9870 "bison-chpl-lib.cpp"
    break;

  case 496: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2830 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 9878 "bison-chpl-lib.cpp"
    break;

  case 497: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2834 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9886 "bison-chpl-lib.cpp"
    break;

  case 498: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2838 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9894 "bison-chpl-lib.cpp"
    break;

  case 499: /* ret_array_type: TLSBR error TRSBR  */
#line 2842 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "invalid expression for domain of array return type");
  }
#line 9902 "bison-chpl-lib.cpp"
    break;

  case 500: /* opt_ret_type: %empty  */
#line 2848 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9908 "bison-chpl-lib.cpp"
    break;

  case 501: /* opt_ret_type: TCOLON type_level_expr  */
#line 2849 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9914 "bison-chpl-lib.cpp"
    break;

  case 502: /* opt_ret_type: TCOLON ret_array_type  */
#line 2850 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9920 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2851 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9926 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_ret_type: error  */
#line 2852 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9932 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_type: %empty  */
#line 2857 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9938 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_type: TCOLON type_level_expr  */
#line 2858 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9944 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_type: TCOLON array_type  */
#line 2859 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9950 "bison-chpl-lib.cpp"
    break;

  case 508: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2860 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9956 "bison-chpl-lib.cpp"
    break;

  case 509: /* opt_type: error  */
#line 2861 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9962 "bison-chpl-lib.cpp"
    break;

  case 510: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2882 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 511: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2886 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9978 "bison-chpl-lib.cpp"
    break;

  case 512: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2890 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9986 "bison-chpl-lib.cpp"
    break;

  case 513: /* array_type: TLSBR error TRSBR  */
#line 2894 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 9994 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_formal_array_elt_type: %empty  */
#line 2900 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 10000 "bison-chpl-lib.cpp"
    break;

  case 515: /* opt_formal_array_elt_type: type_level_expr  */
#line 2901 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 516: /* opt_formal_array_elt_type: query_expr  */
#line 2902 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10012 "bison-chpl-lib.cpp"
    break;

  case 517: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2907 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10021 "bison-chpl-lib.cpp"
    break;

  case 518: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2912 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10029 "bison-chpl-lib.cpp"
    break;

  case 519: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2920 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 10038 "bison-chpl-lib.cpp"
    break;

  case 520: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2925 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10046 "bison-chpl-lib.cpp"
    break;

  case 521: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2929 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10054 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_formal_type: %empty  */
#line 2935 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 10060 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_formal_type: TCOLON type_level_expr  */
#line 2936 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10066 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_formal_type: TCOLON query_expr  */
#line 2937 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10072 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2938 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10078 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_formal_type: TCOLON formal_array_type  */
#line 2939 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10084 "bison-chpl-lib.cpp"
    break;

  case 527: /* expr_ls: expr  */
#line 2945 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 528: /* expr_ls: query_expr  */
#line 2946 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10096 "bison-chpl-lib.cpp"
    break;

  case 529: /* expr_ls: expr_ls TCOMMA expr  */
#line 2947 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 530: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2948 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 531: /* simple_expr_ls: expr  */
#line 2952 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10114 "bison-chpl-lib.cpp"
    break;

  case 532: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2953 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10120 "bison-chpl-lib.cpp"
    break;

  case 533: /* tuple_component: TUNDERSCORE  */
#line 2957 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10126 "bison-chpl-lib.cpp"
    break;

  case 534: /* tuple_component: opt_try_expr  */
#line 2958 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10132 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_component: query_expr  */
#line 2959 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10138 "bison-chpl-lib.cpp"
    break;

  case 536: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2964 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10146 "bison-chpl-lib.cpp"
    break;

  case 537: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2968 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10154 "bison-chpl-lib.cpp"
    break;

  case 538: /* opt_actual_ls: %empty  */
#line 2974 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10160 "bison-chpl-lib.cpp"
    break;

  case 539: /* opt_actual_ls: actual_ls  */
#line 2975 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10166 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_ls: actual_expr  */
#line 2980 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10175 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2985 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10185 "bison-chpl-lib.cpp"
    break;

  case 542: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2993 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10191 "bison-chpl-lib.cpp"
    break;

  case 543: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2994 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10197 "bison-chpl-lib.cpp"
    break;

  case 544: /* actual_expr: query_expr  */
#line 2995 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10203 "bison-chpl-lib.cpp"
    break;

  case 545: /* actual_expr: opt_try_expr  */
#line 2996 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10209 "bison-chpl-lib.cpp"
    break;

  case 546: /* ident_expr: ident_use  */
#line 3000 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10215 "bison-chpl-lib.cpp"
    break;

  case 547: /* ident_expr: scalar_type  */
#line 3001 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10221 "bison-chpl-lib.cpp"
    break;

  case 548: /* type_level_expr: sub_type_level_expr  */
#line 3013 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 10227 "bison-chpl-lib.cpp"
    break;

  case 549: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 3015 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10233 "bison-chpl-lib.cpp"
    break;

  case 550: /* type_level_expr: TQUESTION  */
#line 3017 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10239 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TSINGLE expr  */
#line 3028 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10245 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3030 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10251 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3032 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10257 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3034 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10263 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3036 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10273 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TATOMIC expr  */
#line 3042 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10279 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSYNC expr  */
#line 3044 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10285 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TOWNED  */
#line 3047 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10291 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TOWNED expr  */
#line 3049 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10297 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TUNMANAGED  */
#line 3051 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10303 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3053 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10309 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TSHARED  */
#line 3055 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10315 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TSHARED expr  */
#line 3057 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10321 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TBORROWED  */
#line 3059 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10327 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TBORROWED expr  */
#line 3061 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10333 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TCLASS  */
#line 3064 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10339 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TRECORD  */
#line 3066 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10345 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 3071 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10358 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3080 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10371 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr: TFOR expr TDO expr  */
#line 3089 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10383 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3097 "chpl.ypp"
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
#line 10401 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3111 "chpl.ypp"
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
#line 10419 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3125 "chpl.ypp"
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
#line 10437 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3139 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10450 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3148 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10463 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr: TFORALL expr TDO expr  */
#line 3157 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10475 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3165 "chpl.ypp"
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
#line 10493 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3179 "chpl.ypp"
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
#line 10511 "bison-chpl-lib.cpp"
    break;

  case 584: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3193 "chpl.ypp"
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
#line 10529 "bison-chpl-lib.cpp"
    break;

  case 585: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 3207 "chpl.ypp"
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
#line 10550 "bison-chpl-lib.cpp"
    break;

  case 586: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3224 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10563 "bison-chpl-lib.cpp"
    break;

  case 587: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3233 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10576 "bison-chpl-lib.cpp"
    break;

  case 588: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3242 "chpl.ypp"
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
#line 10595 "bison-chpl-lib.cpp"
    break;

  case 589: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3257 "chpl.ypp"
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
#line 10614 "bison-chpl-lib.cpp"
    break;

  case 590: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3275 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 591: /* nil_expr: TNIL  */
#line 3292 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10634 "bison-chpl-lib.cpp"
    break;

  case 599: /* opt_task_intent_ls: %empty  */
#line 3310 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10640 "bison-chpl-lib.cpp"
    break;

  case 600: /* opt_task_intent_ls: task_intent_clause  */
#line 3311 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10646 "bison-chpl-lib.cpp"
    break;

  case 601: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3316 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10656 "bison-chpl-lib.cpp"
    break;

  case 602: /* task_intent_ls: intent_expr  */
#line 3324 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10662 "bison-chpl-lib.cpp"
    break;

  case 603: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3325 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10668 "bison-chpl-lib.cpp"
    break;

  case 604: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3330 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10678 "bison-chpl-lib.cpp"
    break;

  case 605: /* forall_intent_ls: intent_expr  */
#line 3338 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10684 "bison-chpl-lib.cpp"
    break;

  case 606: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3339 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10690 "bison-chpl-lib.cpp"
    break;

  case 607: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3344 "chpl.ypp"
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
#line 10709 "bison-chpl-lib.cpp"
    break;

  case 608: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3359 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10717 "bison-chpl-lib.cpp"
    break;

  case 609: /* intent_expr: expr TREDUCE ident_expr  */
#line 3363 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10725 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TCONST  */
#line 3369 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10731 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TIN  */
#line 3370 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10737 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TCONST TIN  */
#line 3371 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10743 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_var_prefix: TREF  */
#line 3372 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10749 "bison-chpl-lib.cpp"
    break;

  case 614: /* task_var_prefix: TCONST TREF  */
#line 3373 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10755 "bison-chpl-lib.cpp"
    break;

  case 615: /* task_var_prefix: TVAR  */
#line 3374 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10761 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_maybe_decorated: TNEW  */
#line 3379 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10767 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_maybe_decorated: TNEW TOWNED  */
#line 3381 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10773 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_maybe_decorated: TNEW TSHARED  */
#line 3383 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10779 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3385 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10785 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3387 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10791 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_expr: new_maybe_decorated expr  */
#line 3393 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10799 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3400 "chpl.ypp"
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
#line 10815 "bison-chpl-lib.cpp"
    break;

  case 623: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3412 "chpl.ypp"
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
#line 10831 "bison-chpl-lib.cpp"
    break;

  case 624: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3424 "chpl.ypp"
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
#line 10848 "bison-chpl-lib.cpp"
    break;

  case 625: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3437 "chpl.ypp"
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
#line 10866 "bison-chpl-lib.cpp"
    break;

  case 626: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3454 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10874 "bison-chpl-lib.cpp"
    break;

  case 636: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 3473 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10882 "bison-chpl-lib.cpp"
    break;

  case 637: /* expr: expr TCOLON expr  */
#line 3477 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10890 "bison-chpl-lib.cpp"
    break;

  case 638: /* expr: expr TDOTDOT expr  */
#line 3481 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10899 "bison-chpl-lib.cpp"
    break;

  case 639: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 3486 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10908 "bison-chpl-lib.cpp"
    break;

  case 640: /* expr: expr TDOTDOT  */
#line 3491 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10917 "bison-chpl-lib.cpp"
    break;

  case 641: /* expr: TDOTDOT expr  */
#line 3496 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10927 "bison-chpl-lib.cpp"
    break;

  case 642: /* expr: TDOTDOTOPENHIGH expr  */
#line 3502 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10937 "bison-chpl-lib.cpp"
    break;

  case 643: /* expr: TDOTDOT  */
#line 3508 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10947 "bison-chpl-lib.cpp"
    break;

  case 644: /* opt_expr: %empty  */
#line 3539 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10953 "bison-chpl-lib.cpp"
    break;

  case 645: /* opt_expr: expr  */
#line 3540 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10959 "bison-chpl-lib.cpp"
    break;

  case 646: /* opt_try_expr: TTRY expr  */
#line 3544 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10965 "bison-chpl-lib.cpp"
    break;

  case 647: /* opt_try_expr: TTRYBANG expr  */
#line 3545 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10971 "bison-chpl-lib.cpp"
    break;

  case 648: /* opt_try_expr: expr  */
#line 3546 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10977 "bison-chpl-lib.cpp"
    break;

  case 654: /* call_base_expr: expr TBANG  */
#line 3563 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10985 "bison-chpl-lib.cpp"
    break;

  case 655: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3566 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10991 "bison-chpl-lib.cpp"
    break;

  case 658: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3573 "chpl.ypp"
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
#line 11007 "bison-chpl-lib.cpp"
    break;

  case 659: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3585 "chpl.ypp"
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
#line 11023 "bison-chpl-lib.cpp"
    break;

  case 660: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3597 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 11031 "bison-chpl-lib.cpp"
    break;

  case 661: /* dot_expr: expr TDOT ident_use  */
#line 3604 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11037 "bison-chpl-lib.cpp"
    break;

  case 662: /* dot_expr: expr TDOT TTYPE  */
#line 3606 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11043 "bison-chpl-lib.cpp"
    break;

  case 663: /* dot_expr: expr TDOT TDOMAIN  */
#line 3608 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11049 "bison-chpl-lib.cpp"
    break;

  case 664: /* dot_expr: expr TDOT TLOCALE  */
#line 3610 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11055 "bison-chpl-lib.cpp"
    break;

  case 665: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3612 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 11065 "bison-chpl-lib.cpp"
    break;

  case 666: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3618 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 11075 "bison-chpl-lib.cpp"
    break;

  case 667: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3630 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 11081 "bison-chpl-lib.cpp"
    break;

  case 668: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3632 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 11089 "bison-chpl-lib.cpp"
    break;

  case 669: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3636 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11097 "bison-chpl-lib.cpp"
    break;

  case 670: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3640 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11105 "bison-chpl-lib.cpp"
    break;

  case 671: /* bool_literal: TFALSE  */
#line 3646 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 11111 "bison-chpl-lib.cpp"
    break;

  case 672: /* bool_literal: TTRUE  */
#line 3647 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 11117 "bison-chpl-lib.cpp"
    break;

  case 673: /* str_bytes_literal: STRINGLITERAL  */
#line 3651 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11123 "bison-chpl-lib.cpp"
    break;

  case 674: /* str_bytes_literal: BYTESLITERAL  */
#line 3652 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11129 "bison-chpl-lib.cpp"
    break;

  case 677: /* literal_expr: INTLITERAL  */
#line 3658 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 11135 "bison-chpl-lib.cpp"
    break;

  case 678: /* literal_expr: REALLITERAL  */
#line 3659 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11141 "bison-chpl-lib.cpp"
    break;

  case 679: /* literal_expr: IMAGLITERAL  */
#line 3660 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11147 "bison-chpl-lib.cpp"
    break;

  case 680: /* literal_expr: CSTRINGLITERAL  */
#line 3661 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11153 "bison-chpl-lib.cpp"
    break;

  case 681: /* literal_expr: TNONE  */
#line 3662 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11159 "bison-chpl-lib.cpp"
    break;

  case 682: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3664 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11168 "bison-chpl-lib.cpp"
    break;

  case 683: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3669 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11177 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3674 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11185 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3678 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11194 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3683 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11202 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3687 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11211 "bison-chpl-lib.cpp"
    break;

  case 688: /* assoc_expr_ls: expr TALIAS expr  */
#line 3696 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11220 "bison-chpl-lib.cpp"
    break;

  case 689: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3701 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11230 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TPLUS expr  */
#line 3709 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11236 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TMINUS expr  */
#line 3710 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11242 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TSTAR expr  */
#line 3711 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11248 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TDIVIDE expr  */
#line 3712 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11254 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3713 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11260 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3714 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11266 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TMOD expr  */
#line 3715 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11272 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TEQUAL expr  */
#line 3716 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11278 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3717 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11284 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3718 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11290 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3719 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11296 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TLESS expr  */
#line 3720 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11302 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TGREATER expr  */
#line 3721 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11308 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TBAND expr  */
#line 3722 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11314 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TBOR expr  */
#line 3723 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11320 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TBXOR expr  */
#line 3724 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11326 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TAND expr  */
#line 3725 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11332 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TOR expr  */
#line 3726 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11338 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TEXP expr  */
#line 3727 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11344 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TBY expr  */
#line 3728 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11350 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TALIGN expr  */
#line 3729 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11356 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr THASH expr  */
#line 3730 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11362 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TDMAPPED expr  */
#line 3731 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11368 "bison-chpl-lib.cpp"
    break;

  case 713: /* unary_op_expr: TPLUS expr  */
#line 3735 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11374 "bison-chpl-lib.cpp"
    break;

  case 714: /* unary_op_expr: TMINUS expr  */
#line 3736 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11380 "bison-chpl-lib.cpp"
    break;

  case 715: /* unary_op_expr: TMINUSMINUS expr  */
#line 3737 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11386 "bison-chpl-lib.cpp"
    break;

  case 716: /* unary_op_expr: TPLUSPLUS expr  */
#line 3738 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11392 "bison-chpl-lib.cpp"
    break;

  case 717: /* unary_op_expr: TBANG expr  */
#line 3739 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11398 "bison-chpl-lib.cpp"
    break;

  case 718: /* unary_op_expr: expr TBANG  */
#line 3740 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11406 "bison-chpl-lib.cpp"
    break;

  case 719: /* unary_op_expr: TBNOT expr  */
#line 3743 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11412 "bison-chpl-lib.cpp"
    break;

  case 720: /* reduce_expr: expr TREDUCE expr  */
#line 3748 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11420 "bison-chpl-lib.cpp"
    break;

  case 721: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3752 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11428 "bison-chpl-lib.cpp"
    break;

  case 722: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3756 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11436 "bison-chpl-lib.cpp"
    break;

  case 723: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3760 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11444 "bison-chpl-lib.cpp"
    break;

  case 724: /* scan_expr: expr TSCAN expr  */
#line 3767 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11452 "bison-chpl-lib.cpp"
    break;

  case 725: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3771 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11460 "bison-chpl-lib.cpp"
    break;

  case 726: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3775 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11468 "bison-chpl-lib.cpp"
    break;

  case 727: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3779 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11476 "bison-chpl-lib.cpp"
    break;


#line 11480 "bison-chpl-lib.cpp"

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
