/* A Bison parser, made by GNU Bison 3.7.6.  */

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
#define YYBISON 30706

/* Bison version string.  */
#define YYBISON_VERSION "3.7.6"

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
  YYSYMBOL_TMINUSMINUS = 64,               /* TMINUSMINUS  */
  YYSYMBOL_TMODULE = 65,                   /* TMODULE  */
  YYSYMBOL_TNEW = 66,                      /* TNEW  */
  YYSYMBOL_TNIL = 67,                      /* TNIL  */
  YYSYMBOL_TNOINIT = 68,                   /* TNOINIT  */
  YYSYMBOL_TNONE = 69,                     /* TNONE  */
  YYSYMBOL_TNOTHING = 70,                  /* TNOTHING  */
  YYSYMBOL_TON = 71,                       /* TON  */
  YYSYMBOL_TONLY = 72,                     /* TONLY  */
  YYSYMBOL_TOPERATOR = 73,                 /* TOPERATOR  */
  YYSYMBOL_TOTHERWISE = 74,                /* TOTHERWISE  */
  YYSYMBOL_TOUT = 75,                      /* TOUT  */
  YYSYMBOL_TOVERRIDE = 76,                 /* TOVERRIDE  */
  YYSYMBOL_TOWNED = 77,                    /* TOWNED  */
  YYSYMBOL_TPARAM = 78,                    /* TPARAM  */
  YYSYMBOL_TPLUSPLUS = 79,                 /* TPLUSPLUS  */
  YYSYMBOL_TPRAGMA = 80,                   /* TPRAGMA  */
  YYSYMBOL_TPRIMITIVE = 81,                /* TPRIMITIVE  */
  YYSYMBOL_TPRIVATE = 82,                  /* TPRIVATE  */
  YYSYMBOL_TPROC = 83,                     /* TPROC  */
  YYSYMBOL_TPROTOTYPE = 84,                /* TPROTOTYPE  */
  YYSYMBOL_TPUBLIC = 85,                   /* TPUBLIC  */
  YYSYMBOL_TREAL = 86,                     /* TREAL  */
  YYSYMBOL_TRECORD = 87,                   /* TRECORD  */
  YYSYMBOL_TREDUCE = 88,                   /* TREDUCE  */
  YYSYMBOL_TREF = 89,                      /* TREF  */
  YYSYMBOL_TREQUIRE = 90,                  /* TREQUIRE  */
  YYSYMBOL_TRETURN = 91,                   /* TRETURN  */
  YYSYMBOL_TSCAN = 92,                     /* TSCAN  */
  YYSYMBOL_TSELECT = 93,                   /* TSELECT  */
  YYSYMBOL_TSERIAL = 94,                   /* TSERIAL  */
  YYSYMBOL_TSHARED = 95,                   /* TSHARED  */
  YYSYMBOL_TSINGLE = 96,                   /* TSINGLE  */
  YYSYMBOL_TSPARSE = 97,                   /* TSPARSE  */
  YYSYMBOL_TSTRING = 98,                   /* TSTRING  */
  YYSYMBOL_TSUBDOMAIN = 99,                /* TSUBDOMAIN  */
  YYSYMBOL_TSYNC = 100,                    /* TSYNC  */
  YYSYMBOL_TTHEN = 101,                    /* TTHEN  */
  YYSYMBOL_TTHIS = 102,                    /* TTHIS  */
  YYSYMBOL_TTHROW = 103,                   /* TTHROW  */
  YYSYMBOL_TTHROWS = 104,                  /* TTHROWS  */
  YYSYMBOL_TTRUE = 105,                    /* TTRUE  */
  YYSYMBOL_TTRY = 106,                     /* TTRY  */
  YYSYMBOL_TTRYBANG = 107,                 /* TTRYBANG  */
  YYSYMBOL_TTYPE = 108,                    /* TTYPE  */
  YYSYMBOL_TUINT = 109,                    /* TUINT  */
  YYSYMBOL_TUNDERSCORE = 110,              /* TUNDERSCORE  */
  YYSYMBOL_TUNION = 111,                   /* TUNION  */
  YYSYMBOL_TUNMANAGED = 112,               /* TUNMANAGED  */
  YYSYMBOL_TUSE = 113,                     /* TUSE  */
  YYSYMBOL_TVAR = 114,                     /* TVAR  */
  YYSYMBOL_TVOID = 115,                    /* TVOID  */
  YYSYMBOL_TWHEN = 116,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 117,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 118,                   /* TWHILE  */
  YYSYMBOL_TWITH = 119,                    /* TWITH  */
  YYSYMBOL_TYIELD = 120,                   /* TYIELD  */
  YYSYMBOL_TZIP = 121,                     /* TZIP  */
  YYSYMBOL_TALIAS = 122,                   /* TALIAS  */
  YYSYMBOL_TAND = 123,                     /* TAND  */
  YYSYMBOL_TASSIGN = 124,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 125,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 126,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 127,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 128,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 129,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 130,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 131,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 132,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 133,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 134,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 135,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 136,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 137,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 138,                /* TASSIGNSR  */
  YYSYMBOL_TBANG = 139,                    /* TBANG  */
  YYSYMBOL_TBAND = 140,                    /* TBAND  */
  YYSYMBOL_TBNOT = 141,                    /* TBNOT  */
  YYSYMBOL_TBOR = 142,                     /* TBOR  */
  YYSYMBOL_TBXOR = 143,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 144,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 145,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 146,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 147,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 148,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 149,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 150,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 151,                     /* TEXP  */
  YYSYMBOL_TGREATER = 152,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 153,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 154,                    /* THASH  */
  YYSYMBOL_TIO = 155,                      /* TIO  */
  YYSYMBOL_TLESS = 156,                    /* TLESS  */
  YYSYMBOL_TLESSEQUAL = 157,               /* TLESSEQUAL  */
  YYSYMBOL_TMINUS = 158,                   /* TMINUS  */
  YYSYMBOL_TMOD = 159,                     /* TMOD  */
  YYSYMBOL_TNOTEQUAL = 160,                /* TNOTEQUAL  */
  YYSYMBOL_TOR = 161,                      /* TOR  */
  YYSYMBOL_TPLUS = 162,                    /* TPLUS  */
  YYSYMBOL_TQUESTION = 163,                /* TQUESTION  */
  YYSYMBOL_TSEMI = 164,                    /* TSEMI  */
  YYSYMBOL_TSHIFTLEFT = 165,               /* TSHIFTLEFT  */
  YYSYMBOL_TSHIFTRIGHT = 166,              /* TSHIFTRIGHT  */
  YYSYMBOL_TSTAR = 167,                    /* TSTAR  */
  YYSYMBOL_TSWAP = 168,                    /* TSWAP  */
  YYSYMBOL_TLCBR = 169,                    /* TLCBR  */
  YYSYMBOL_TRCBR = 170,                    /* TRCBR  */
  YYSYMBOL_TLP = 171,                      /* TLP  */
  YYSYMBOL_TRP = 172,                      /* TRP  */
  YYSYMBOL_TLSBR = 173,                    /* TLSBR  */
  YYSYMBOL_TRSBR = 174,                    /* TRSBR  */
  YYSYMBOL_TNOELSE = 175,                  /* TNOELSE  */
  YYSYMBOL_TDOTDOTOPENHIGH = 176,          /* TDOTDOTOPENHIGH  */
  YYSYMBOL_TUPLUS = 177,                   /* TUPLUS  */
  YYSYMBOL_TUMINUS = 178,                  /* TUMINUS  */
  YYSYMBOL_TLNOT = 179,                    /* TLNOT  */
  YYSYMBOL_YYACCEPT = 180,                 /* $accept  */
  YYSYMBOL_program = 181,                  /* program  */
  YYSYMBOL_toplevel_stmt_ls = 182,         /* toplevel_stmt_ls  */
  YYSYMBOL_toplevel_stmt = 183,            /* toplevel_stmt  */
  YYSYMBOL_pragma_ls = 184,                /* pragma_ls  */
  YYSYMBOL_stmt = 185,                     /* stmt  */
  YYSYMBOL_deprecated_decl_stmt = 186,     /* deprecated_decl_stmt  */
  YYSYMBOL_deprecated_decl_base = 187,     /* deprecated_decl_base  */
  YYSYMBOL_module_decl_start = 188,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 189,         /* module_decl_stmt  */
  YYSYMBOL_access_control = 190,           /* access_control  */
  YYSYMBOL_opt_prototype = 191,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 192,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 193,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 194,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 195,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 196,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 197,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 198,              /* opt_only_ls  */
  YYSYMBOL_except_ls = 199,                /* except_ls  */
  YYSYMBOL_use_stmt = 200,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 201,              /* import_stmt  */
  YYSYMBOL_import_expr = 202,              /* import_expr  */
  YYSYMBOL_import_ls = 203,                /* import_ls  */
  YYSYMBOL_require_stmt = 204,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 205,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 206,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 207,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 208,                /* ident_def  */
  YYSYMBOL_ident_use = 209,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 210,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 211,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 212,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 213,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 214,              /* return_stmt  */
  YYSYMBOL_deprecated_class_level_stmt = 215, /* deprecated_class_level_stmt  */
  YYSYMBOL_class_level_stmt = 216,         /* class_level_stmt  */
  YYSYMBOL_217_1 = 217,                    /* $@1  */
  YYSYMBOL_218_2 = 218,                    /* $@2  */
  YYSYMBOL_inner_class_level_stmt = 219,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 220,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 221,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 222,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 223, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 224,  /* extern_export_decl_stmt  */
  YYSYMBOL_extern_block_stmt = 225,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 226,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 227,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 228,                  /* if_stmt  */
  YYSYMBOL_ifvar = 229,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 230,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 231,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 232,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 233,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 234, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 235,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 236,           /* ifc_constraint  */
  YYSYMBOL_defer_stmt = 237,               /* defer_stmt  */
  YYSYMBOL_try_stmt = 238,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 239,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 240,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 241,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 242,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 243,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 244,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 245,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 246,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 247,              /* class_start  */
  YYSYMBOL_class_tag = 248,                /* class_tag  */
  YYSYMBOL_opt_inherit = 249,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 250,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 251,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 252,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 253,                  /* enum_ls  */
  YYSYMBOL_254_3 = 254,                    /* $@3  */
  YYSYMBOL_deprecated_enum_item = 255,     /* deprecated_enum_item  */
  YYSYMBOL_enum_item = 256,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 257,         /* lambda_decl_expr  */
  YYSYMBOL_linkage_spec_empty = 258,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 259,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt_complete = 260,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 261,             /* fn_decl_stmt  */
  YYSYMBOL_262_4 = 262,                    /* $@4  */
  YYSYMBOL_fn_decl_stmt_inner = 263,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 264,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 265,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 266,                 /* fn_ident  */
  YYSYMBOL_assignop_ident = 267,           /* assignop_ident  */
  YYSYMBOL_opt_formal_ls = 268,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 269,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 270,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 271,                /* formal_ls  */
  YYSYMBOL_formal = 272,                   /* formal  */
  YYSYMBOL_opt_intent_tag = 273,           /* opt_intent_tag  */
  YYSYMBOL_required_intent_tag = 274,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 275,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 276,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 277,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 278,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 279,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 280,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 281,               /* query_expr  */
  YYSYMBOL_var_arg_expr = 282,             /* var_arg_expr  */
  YYSYMBOL_opt_lifetime_where = 283,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 284, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 285,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 286,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 287,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 288, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner = 289, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 290,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 291,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 292,            /* var_decl_stmt  */
  YYSYMBOL_var_decl_stmt_inner_ls = 293,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 294,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 295, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 296, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 297,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 298,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 299,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 300,                 /* opt_type  */
  YYSYMBOL_array_type = 301,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 302, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 303,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 304,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 305,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 306,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 307,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 308,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 309,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 310,                /* actual_ls  */
  YYSYMBOL_actual_expr = 311,              /* actual_expr  */
  YYSYMBOL_ident_expr = 312,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 313,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 314,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 315,                 /* for_expr  */
  YYSYMBOL_cond_expr = 316,                /* cond_expr  */
  YYSYMBOL_nil_expr = 317,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 318,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 319,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 320,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 321,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 322,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 323,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 324,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 325,          /* task_var_prefix  */
  YYSYMBOL_io_expr = 326,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 327,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 328,                 /* new_expr  */
  YYSYMBOL_let_expr = 329,                 /* let_expr  */
  YYSYMBOL_expr = 330,                     /* expr  */
  YYSYMBOL_opt_expr = 331,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 332,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 333,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 334,           /* call_base_expr  */
  YYSYMBOL_call_expr = 335,                /* call_expr  */
  YYSYMBOL_dot_expr = 336,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 337,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 338,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 339,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 340,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 341,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 342,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 343,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 344,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 345,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 346       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 355 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include other helper functions for the parser
  #include "parser-help.h"


#line 469 "bison-chpl-lib.cpp"

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
#define YYLAST   20333

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  167
/* YYNRULES -- Number of rules.  */
#define YYNRULES  700
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1246

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   434


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
     175,   176,   177,   178,   179
};

#if YYCHPL_DEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   630,   630,   635,   636,   641,   642,   647,   655,   665,
     666,   667,   668,   669,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   679,   680,   681,   682,   683,   684,   685,
     686,   687,   688,   689,   690,   700,   710,   711,   721,   731,
     732,   733,   740,   741,   743,   748,   749,   753,   766,   779,
     792,   806,   807,   809,   814,   815,   820,   837,   841,   845,
     855,   874,   875,   879,   885,   891,   896,   906,   912,   918,
     924,   929,   934,   944,   949,   953,   959,   963,   968,   973,
     978,   983,   988,   993,  1001,  1009,  1011,  1013,  1018,  1020,
    1025,  1032,  1036,  1040,  1044,  1048,  1052,  1061,  1062,  1066,
    1067,  1068,  1069,  1070,  1071,  1074,  1075,  1076,  1077,  1078,
    1079,  1091,  1092,  1103,  1104,  1105,  1106,  1107,  1108,  1109,
    1110,  1111,  1112,  1113,  1114,  1115,  1116,  1117,  1118,  1119,
    1120,  1121,  1125,  1126,  1127,  1128,  1129,  1130,  1131,  1132,
    1133,  1134,  1135,  1136,  1143,  1144,  1145,  1146,  1150,  1151,
    1155,  1159,  1166,  1167,  1169,  1174,  1178,  1184,  1184,  1190,
    1190,  1199,  1200,  1201,  1202,  1203,  1204,  1205,  1209,  1216,
    1223,  1230,  1240,  1247,  1248,  1252,  1260,  1268,  1276,  1296,
    1306,  1313,  1329,  1337,  1345,  1349,  1353,  1357,  1361,  1365,
    1369,  1373,  1386,  1390,  1394,  1398,  1402,  1406,  1410,  1414,
    1418,  1422,  1426,  1430,  1434,  1438,  1442,  1446,  1450,  1454,
    1458,  1462,  1466,  1470,  1474,  1479,  1487,  1496,  1498,  1500,
    1502,  1504,  1506,  1508,  1510,  1513,  1515,  1517,  1519,  1524,
    1525,  1529,  1531,  1536,  1537,  1542,  1547,  1548,  1549,  1550,
    1551,  1552,  1553,  1554,  1555,  1556,  1557,  1558,  1559,  1571,
    1572,  1573,  1574,  1583,  1584,  1588,  1590,  1592,  1597,  1599,
    1601,  1606,  1610,  1612,  1614,  1616,  1618,  1620,  1625,  1626,
    1630,  1631,  1632,  1636,  1637,  1641,  1645,  1647,  1652,  1653,
    1657,  1659,  1661,  1668,  1675,  1685,  1692,  1693,  1694,  1698,
    1699,  1704,  1705,  1707,  1712,  1727,  1739,  1752,  1756,  1761,
    1761,  1768,  1769,  1771,  1776,  1781,  1790,  1796,  1800,  1801,
    1803,  1808,  1818,  1815,  1838,  1846,  1854,  1864,  1874,  1883,
    1894,  1895,  1899,  1900,  1901,  1902,  1903,  1904,  1905,  1906,
    1907,  1908,  1909,  1910,  1911,  1912,  1913,  1914,  1915,  1916,
    1917,  1918,  1919,  1920,  1921,  1922,  1923,  1924,  1925,  1926,
    1935,  1936,  1937,  1938,  1939,  1940,  1941,  1942,  1943,  1944,
    1945,  1946,  1950,  1951,  1955,  1959,  1960,  1964,  1965,  1969,
    1972,  1974,  1976,  1978,  1980,  1987,  1988,  1992,  1993,  1994,
    1995,  1996,  1997,  1998,  1999,  2000,  2004,  2005,  2006,  2007,
    2008,  2009,  2013,  2014,  2015,  2019,  2020,  2021,  2022,  2023,
    2024,  2028,  2029,  2032,  2033,  2037,  2038,  2042,  2046,  2047,
    2048,  2052,  2053,  2055,  2057,  2059,  2064,  2066,  2071,  2073,
    2075,  2077,  2079,  2081,  2083,  2088,  2089,  2093,  2100,  2104,
    2109,  2117,  2122,  2131,  2132,  2134,  2139,  2140,  2141,  2142,
    2143,  2147,  2152,  2193,  2197,  2204,  2214,  2222,  2224,  2226,
    2231,  2233,  2235,  2242,  2243,  2244,  2249,  2253,  2257,  2261,
    2265,  2269,  2273,  2280,  2281,  2282,  2283,  2284,  2289,  2290,
    2291,  2292,  2293,  2313,  2317,  2321,  2325,  2332,  2333,  2334,
    2338,  2340,  2346,  2348,  2350,  2355,  2356,  2357,  2358,  2359,
    2365,  2366,  2367,  2368,  2372,  2373,  2377,  2378,  2379,  2383,
    2384,  2388,  2389,  2393,  2398,  2407,  2408,  2409,  2410,  2414,
    2415,  2426,  2428,  2430,  2436,  2437,  2438,  2439,  2440,  2441,
    2443,  2445,  2447,  2449,  2451,  2453,  2456,  2458,  2460,  2462,
    2464,  2466,  2468,  2470,  2473,  2475,  2480,  2482,  2484,  2486,
    2488,  2490,  2492,  2494,  2496,  2498,  2500,  2502,  2504,  2506,
    2508,  2510,  2512,  2517,  2527,  2535,  2536,  2537,  2538,  2539,
    2540,  2541,  2542,  2547,  2548,  2552,  2561,  2562,  2566,  2575,
    2576,  2580,  2593,  2597,  2604,  2605,  2606,  2607,  2608,  2609,
    2613,  2614,  2619,  2621,  2623,  2625,  2627,  2633,  2650,  2654,
    2658,  2662,  2669,  2677,  2678,  2679,  2680,  2681,  2682,  2683,
    2684,  2685,  2686,  2688,  2690,  2692,  2694,  2696,  2698,  2700,
    2719,  2720,  2724,  2725,  2726,  2730,  2731,  2732,  2733,  2742,
    2743,  2744,  2745,  2746,  2750,  2762,  2774,  2778,  2780,  2782,
    2784,  2786,  2792,  2805,  2806,  2807,  2808,  2812,  2813,  2817,
    2818,  2822,  2823,  2824,  2825,  2826,  2827,  2828,  2829,  2830,
    2831,  2832,  2833,  2837,  2845,  2846,  2850,  2851,  2852,  2853,
    2854,  2855,  2856,  2857,  2858,  2859,  2860,  2861,  2862,  2863,
    2864,  2865,  2866,  2867,  2868,  2869,  2870,  2871,  2872,  2876,
    2877,  2878,  2879,  2880,  2881,  2884,  2888,  2889,  2890,  2891,
    2895,  2896,  2897,  2898,  2903,  2904,  2905,  2906,  2907,  2908,
    2909
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
  "TLAMBDA", "TLET", "TLIFETIME", "TLOCAL", "TLOCALE", "TMINUSMINUS",
  "TMODULE", "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON",
  "TONLY", "TOPERATOR", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED",
  "TPARAM", "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC",
  "TPROTOTYPE", "TPUBLIC", "TREAL", "TRECORD", "TREDUCE", "TREF",
  "TREQUIRE", "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSHARED",
  "TSINGLE", "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS",
  "TTHROW", "TTHROWS", "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT",
  "TUNDERSCORE", "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TVOID", "TWHEN",
  "TWHERE", "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND",
  "TASSIGN", "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR", "TCOLON",
  "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL", "TEXP",
  "TGREATER", "TGREATEREQUAL", "THASH", "TIO", "TLESS", "TLESSEQUAL",
  "TMINUS", "TMOD", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP",
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUPLUS",
  "TUMINUS", "TLNOT", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "deprecated_decl_stmt",
  "deprecated_decl_base", "module_decl_start", "module_decl_stmt",
  "access_control", "opt_prototype", "include_module_stmt",
  "block_stmt_body", "block_stmt", "stmt_ls", "renames_ls",
  "use_renames_ls", "opt_only_ls", "except_ls", "use_stmt", "import_stmt",
  "import_expr", "import_ls", "require_stmt", "assignment_stmt",
  "opt_label_ident", "ident_fn_def", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "do_stmt", "return_stmt", "deprecated_class_level_stmt",
  "class_level_stmt", "$@1", "$@2", "inner_class_level_stmt",
  "forwarding_decl_stmt", "forwarding_decl_start", "extern_or_export",
  "extern_export_decl_stmt_start", "extern_export_decl_stmt",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "defer_stmt", "try_stmt",
  "catch_expr_ls", "catch_expr", "catch_expr_inner", "throw_stmt",
  "select_stmt", "when_stmt_ls", "when_stmt", "class_decl_stmt",
  "class_start", "class_tag", "opt_inherit", "class_level_stmt_ls",
  "enum_decl_stmt", "enum_header_lcbr", "enum_ls", "$@3",
  "deprecated_enum_item", "enum_item", "lambda_decl_expr",
  "linkage_spec_empty", "linkage_spec", "fn_decl_stmt_complete",
  "fn_decl_stmt", "$@4", "fn_decl_stmt_inner", "fn_decl_stmt_start",
  "fn_decl_receiver_expr", "fn_ident", "assignop_ident", "opt_formal_ls",
  "req_formal_ls", "formal_ls_inner", "formal_ls", "formal",
  "opt_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_iter_or_op", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "var_arg_expr", "opt_lifetime_where", "lifetime_components_expr",
  "lifetime_expr", "lifetime_ident", "type_alias_decl_stmt",
  "type_alias_decl_stmt_start", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt",
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

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
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
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434
};
#endif

#define YYPACT_NINF (-1079)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-651)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1079,   130,  3340, -1079,   -46,   207, -1079, -1079, -1079, -1079,
   -1079, -1079,  4732,   160,   286,   259, 14329,   294, 20098,   160,
   10970,   325,   244,   240,   286,  4732, 10970,  1581,  4732,   250,
   20184, -1079,    32,   436,  8202,  9414,  9414, -1079,  8374,   445,
     451,   336, -1079,   459, 20184, 20184, 20184,   353, 14695,  9586,
     467, 10970,   222, -1079,   472,   476, 10970, -1079, 14329, -1079,
   10970,   535,   375,   397,   400,   503, 20218, -1079,  9760,  7684,
   10970,  9586, 14329, 10970,   466,   532,   419,  4732,   539, 10970,
     540, 11142, 11142, -1079,   541, -1079, 14329, -1079,   543,  8374,
   10970, -1079, 10970, -1079, 10970, -1079, -1079, 13849, 10970, -1079,
   10970, -1079, -1079, -1079,  3688,  6820,  8548, 10970, -1079,  4558,
   -1079, -1079, -1079,   431, -1079,   197, -1079, -1079, -1079, -1079,
   -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079,  6992,
   -1079, 11314, -1079, -1079, -1079, -1079, -1079,   545, -1079, -1079,
   -1079, -1079, -1079, -1079, -1079, -1079,   458, 20184, -1079, 19944,
     155, -1079,   252, -1079, -1079,   234,   249, -1079, 20184, 14695,
   -1079,   440, -1079,   442, -1079, -1079,   447,   449,   460, 10970,
     452,   457, 19736,  2404,   209,   461,   463, -1079, -1079,   368,
   -1079, -1079, -1079, -1079, -1079,    43, -1079, -1079, 19736,   435,
    4732, -1079, -1079,   464, 10970, -1079, -1079, 10970, 10970, 10970,
   20184, -1079, 10970,  9760,  9760,   566,   382, -1079, -1079, -1079,
   -1079,   -18,   391, -1079, -1079,   465, 16292, -1079, 14695, -1079,
     474, -1079,   168, 19736, 19338, -1079, 20184,   521, -1079, -1079,
     546,   518,  7858, -1079, -1079, -1079, -1079, -1079, -1079, -1079,
   -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079,
   -1079, -1079, -1079, -1079, -1079, -1079, -1079,   470, -1079, -1079,
   -1079, 20184,   469,    33, 15879,    20, 15714,    20, 15796, 20184,
   20184,   -14, 14967, -1079, -1079,   546,  7858,   471,    72,  4732,
     583,   234,  1361,    51,    79, -1079,  4732, -1079, -1079, 16373,
     715, -1079,   477,   478, -1079, 16373,   -18,   715, -1079,  7858,
   16630, 16630, -1079, -1079,   175, 19736, 10970, 10970, -1079, 19736,
     479, 16947, -1079, 16373,   -18, 19736,   480,  7858, -1079, 19736,
   17030, -1079, -1079, 17112,  3149, -1079, -1079, 17193,   -18,    33,
   16373, 17274,   531,   531,  1915,   715,   715,   348, -1079, -1079,
    3862,   -61, -1079, 10970, -1079,   -49,   -35, -1079,   -38,    55,
   17355,    94,  1915,   636, -1079,  4036, 10970, -1079, 10970,   582,
     178, -1079, 15962,   563, 20184, 19736,   453, 20184,  9760,   485,
   -1079,   486, 19978,   536,   102, -1079, -1079, -1079, -1079, -1079,
   -1079,   570, -1079, -1079, -1079,    89,   575, -1079, -1079, -1079,
   13682,   547,   501,   198,   404, -1079, 10970,   520, 10970, 10970,
   10970,  9414,  9414, 10970,   408, 10970, 10970, 10970, 10970, 10970,
     228, 13849, 10970, 10970, 10970, 10970, 10970, 10970, 10970, 10970,
   10970, 10970, 10970, 10970, 10970, 10970, 10970, 10970,   601, -1079,
   -1079, -1079, -1079, -1079,  8720,  8720, -1079, -1079, -1079, -1079,
    8720, -1079, -1079,  8720,  8720,  7858,  7858,  9414,  9414,  7512,
   -1079, -1079, 16455, 16536, 17436,   505,    64, 20184,  4210, -1079,
    9414,    33,   253, -1079, 10970, -1079, -1079, 10970,   553, -1079,
     507,   538, -1079, -1079, -1079,   633,  9760, -1079,  4906,  9414,
   -1079,   513, -1079,    33,  5080,  9414, -1079,    33, -1079,    33,
    9414, -1079,    33,   561,   565,  4732,   653,  4732, -1079,   659,
   10970,   630,   525,  7858, 20184, -1079, -1079,    59, -1079, -1079,
   -1079, -1079, -1079, -1079,  1044,   554,   526, -1079, 14781, -1079,
      90, -1079,  1361, -1079,   555,   529, -1079, 11486,   580, 10970,
   14695, -1079, -1079, 10970, 10970, -1079,   533, -1079, -1079,  9760,
   -1079, 19736, 19736, -1079,    40, -1079,  7858,   534, -1079,   686,
   -1079,   686, -1079, -1079, -1079, -1079, -1079, -1079, -1079,  8894,
   -1079, 17517,  7166, -1079,  7340, -1079,  4732,   544,  9414,  9068,
    3514,   550, 10970,  9932, -1079, -1079,   350, -1079,  4384, -1079,
     326, 17598,   344, 16045, 20184, 11658, 10970, -1079, 20184,   458,
   -1079, 14695,   -30,   572,  1938, -1079, 20184, -1079, 10970,   215,
   -1079, -1079, -1079, 11830,   617, -1079,   562,   118, -1079,   588,
   -1079,   589,   591,   600,   593,   595, -1079,   598,   607,   602,
     604,   606,   138,   609,   616,   618, -1079, -1079, -1079, -1079,
   -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079,
   -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079, 10970,
   -1079,   613,   625,   619,   562,   562, -1079, 12002,   611, -1079,
   -1079, 17679,  3079,  3079,   473, -1079,   473, -1079,   473, 19047,
     761,  1784,  1007,   -18,   531, -1079,   409, -1079, -1079, -1079,
   -1079, -1079,  1915,  2817,   473,  2609,  2609,  3079,  2609,  2609,
    1202,   531,  2817, 16333,  1202,   715,   715,   531,  1915,   603,
     614,   615,   620,   622,   623,   608,   624, -1079,   473, -1079,
     473,   189, -1079, -1079, -1079,   -29, -1079,  1475, 19777,   291,
   12174,  9414, 12346,  9414, 10970,  7858,  9414, 14609,   610,   160,
   17760, -1079, -1079, 19736, 17841,  7858, -1079,  7858, 10970,   101,
    8374, 19736,    50, 16617,  7512, -1079,  8374, 19736,    35, 16127,
   -1079, -1079,    20, 16210, -1079, 10970, 10970,   740,  4732,   747,
   17922,  4732, 16699, 20184, -1079,   192, -1079,   315, -1079, -1079,
   -1079, 20184,  1173, -1079,  1361,   639,    36,   627,  1361,    51,
      -1,    45, 10970, 10970,  6646, -1079, -1079,   605,  8030, -1079,
   19736, -1079, 18003, 18084, -1079, -1079, 19736,   626,    -3,   629,
   -1079,  3083, -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079,
    4732,   108, 16782, -1079, -1079, 19736,  4732, 19736, -1079, 18165,
   -1079, -1079, 10970, -1079,    70,    86, 10970, -1079, 10104, 11658,
   10970, -1079,   628,   643,   638, 18246,   643,   640,   458,   621,
     352, -1079,  7858,   641,  1655, -1079, 19736, 20064,  6472, -1079,
   -1079,   184, -1079,    36,   583, -1079, 18327, -1079, 14885, -1079,
   -1079, -1079,   355, 20184,   634,   635, -1079, -1079, -1079, -1079,
   -1079, -1079, -1079, -1079, -1079, -1079,  7512, -1079, -1079, -1079,
   -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079, -1079,    39,
    9414,  9414, 10970,   756, 18408, 10970,   764, 18489,   323,   645,
   18570,  7858,    33,    33, -1079, -1079, -1079, -1079, 16373, -1079,
   15050,  5254, -1079,  5428, -1079,   324, -1079, 15133,  5602, -1079,
      33,  5776, -1079,    33, -1079,    33, -1079,    33, 19736, 19736,
    4732, -1079,  4732, 10970, -1079,  4732,   773,   651,   656, 20184,
     465,   639, -1079,   649, 10278,   243,   112, 10970,    73, -1079,
   -1079,   580,   648,    76, -1079, -1079,   652,   655, -1079,  5950,
    9760, -1079, -1079, -1079, 20184, -1079,   680,   465, -1079,  6124,
     658,  6298,   660, -1079, 10970, -1079, -1079, 10970, 18651,   105,
   16865,   663,   665, 10970, -1079, 10970, -1079,   664, -1079, -1079,
     330, -1079,   916, -1079, -1079, -1079, 19529, -1079, -1079, -1079,
     661, 13550,    77, -1079,   666, -1079,   698,   700,   562,   562,
   -1079, -1079, -1079, -1079,   580,   331,   334, 18733, 12518, 12690,
   18814, 12862, 13034, -1079, 13206, 13378,   337, -1079, -1079, -1079,
    4732,  8374, 19736,  8374, 19736,  7512, -1079,  4732,  8374, 19736,
   -1079,  8374, 19736, -1079, -1079, -1079, -1079, -1079, 19736,   805,
    4732, -1079, -1079, -1079, -1079,   243,   639,  9242, -1079, -1079,
   -1079,   169,  9760, -1079, -1079, -1079,    99, -1079,   -13, -1079,
     221, 18908, -1079, -1079, -1079, -1079, -1079,  9414, 14469,  7858,
    7858,  4732, -1079,    67,   669, 10970, -1079,  8374, -1079, 19736,
    4732,  8374, -1079, 19736,  4732, 19736,   273, 10450, 11658, 10970,
   11658, 10970, -1079, -1079, 18965, 19736, -1079,  2436,   684, 14735,
   -1079, -1079, -1079, -1079,   199, 14018,     7, -1079, -1079, -1079,
   -1079, 10970, 10970, 10970, 10970, 10970, 10970, 10970, 10970, -1079,
   17922, 15216, 15299, -1079, 17922, 15382, 15465,  4732, -1079, -1079,
   -1079,   243, 10624,    80, -1079, 19736, -1079, 10970,   112,    99,
      99,    99,    99,    99,    99,   112, 19005, -1079,   827,   679,
     683,   518, -1079,   465, 19736, 15548, -1079, 15631, -1079, -1079,
   -1079, 19736,   688,   692,   693,   694, 10970,  2830, -1079, -1079,
   -1079, -1079,   366, -1079, -1079, -1079, 19156, 19196, 19347, 19387,
   19538, 19578,  4732,  4732,  4732,  4732, -1079, -1079, -1079, -1079,
   -1079, -1079,   314,  9414, 14158, 19736, -1079, -1079, -1079, -1079,
   -1079, -1079, -1079,   718, 13378,   697,   702, -1079,  4732,  4732,
   -1079, -1079, -1079, -1079, 19736, -1079, 10970, 10970, 10970, 10970,
   10970, 10970, 17922, 17922, 17922, 17922, 19619, -1079, -1079,  1015,
   -1079, -1079, 17922, 17922, 10798, -1079
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   111,   643,   644,   645,   639,
     640,   646,     0,   563,    97,   132,   532,   139,   534,   563,
       0,   138,     0,   439,    97,     0,     0,   307,     0,   253,
     133,   174,   173,   637,     0,     0,     0,   172,     0,   137,
      51,   254,   309,   134,     0,     0,     0,     0,     0,     0,
     141,     0,   582,   554,   647,   142,     0,   310,   526,   436,
       0,     0,     0,   159,   157,   136,   535,   438,     0,     0,
       0,     0,   530,     0,     0,   140,     0,     0,   112,     0,
     638,     0,     0,   428,   135,   288,   528,   440,   143,     0,
       0,   696,     0,   698,     0,   699,   700,   609,     0,   697,
     694,   513,   155,   695,     0,     0,     0,     0,     4,     0,
       5,     9,    42,     0,    45,    54,    10,    60,    11,    12,
      13,    14,    15,   509,   510,    25,    46,   156,   166,     0,
     175,   610,   167,    16,    20,    17,    19,     0,   248,    18,
     601,    22,    23,    24,    21,   165,   289,     0,   163,     0,
     598,   308,     0,   161,   311,   395,   386,   164,     0,     0,
     162,   615,   594,   511,   595,   516,   514,     0,     0,     0,
     599,   600,     0,   515,     0,   616,   617,   618,   641,   642,
     593,   518,   517,   596,   597,     0,    41,    27,   524,     0,
       0,   564,    98,     0,     0,   534,   133,     0,     0,     0,
       0,   535,     0,     0,     0,     0,   598,   615,   514,   599,
     600,   533,   515,   616,   617,     0,   563,   429,     0,   437,
       0,   261,     0,   494,   307,   286,     0,   173,   287,    44,
      54,     0,   501,   105,   113,   125,   119,   118,   127,   108,
     117,   128,   114,   129,   106,   130,   123,   116,   124,   122,
     120,   121,   107,   109,   115,   126,   131,     0,   110,   180,
     430,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    53,    52,    54,   501,     0,     0,     0,
     375,   395,     0,     0,     0,   443,     0,   149,    35,     0,
     681,   586,   583,   584,   585,     0,   527,   682,     7,   501,
     307,   307,   407,   491,     0,   490,     0,     0,   150,   614,
       0,     0,    38,     0,   531,   519,     0,   501,    39,   525,
       0,   268,   264,     0,   515,   268,   265,     0,   529,     0,
       0,     0,   683,   685,   607,   680,   679,     0,    57,    61,
       0,     0,   496,     0,   498,     0,     0,   497,     0,     0,
     490,     0,   608,     0,     6,     0,     0,    55,     0,     0,
       0,   171,     0,   639,   535,   611,   307,     0,     0,     0,
     285,     0,     0,   304,     0,   297,   301,   393,   394,   392,
     319,   396,   399,   398,   400,     0,   390,   387,   388,   391,
       0,   433,     0,     0,   512,    26,     0,   587,     0,     0,
       0,     0,     0,     0,   684,     0,     0,     0,     0,     0,
       0,   606,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,   357,
     358,   359,   354,   356,     0,     0,   352,   355,   353,   351,
       0,   361,   360,     0,     0,   501,   501,     0,     0,     0,
      28,    29,     0,     0,     0,     0,     0,     0,     0,    30,
       0,     0,     0,    31,     0,    32,    43,     0,   509,   507,
       0,   502,   503,   508,   296,     0,     0,   190,     0,     0,
     189,     0,   198,     0,     0,     0,   196,     0,   206,     0,
       0,   204,     0,     0,     0,     0,   222,     0,   350,   218,
       0,     0,     0,     0,     0,   232,    33,   382,   377,   378,
     379,   383,   384,   385,   375,   368,     0,   365,     0,   376,
       0,   447,     0,   448,   450,     0,   472,     0,   453,     0,
       0,   148,    34,     0,     0,    36,     0,   160,   158,     0,
      90,   612,   613,   151,     0,    37,     0,     0,   275,   266,
     262,   267,   263,   183,   182,    40,    59,    58,    62,     0,
     648,     0,     0,   633,     0,   635,     0,     0,     0,     0,
       0,     0,     0,     0,   652,     8,     0,    48,     0,    88,
       0,    85,     0,    67,     0,     0,    73,   168,     0,   289,
     178,     0,   259,   290,     0,   295,     0,   303,     0,   299,
     294,   397,   467,     0,   401,   389,   362,    99,   344,   113,
     342,   119,   118,   102,   117,   114,   347,   129,   100,   130,
     116,   120,   101,   103,   115,   131,   341,   323,   326,   324,
     325,   348,   336,   327,   340,   332,   330,   343,   346,   331,
     329,   334,   339,   328,   333,   337,   338,   335,   345,     0,
     322,     0,   104,     0,   362,   362,   320,     0,   431,   427,
     442,   562,   676,   675,   678,   687,   686,   691,   690,   672,
     669,   670,   671,   603,   659,   111,     0,   629,   630,   112,
     628,   627,   604,   663,   674,   668,   666,   677,   667,   665,
     657,   662,   664,   673,   656,   660,   661,   658,   605,     0,
       0,     0,     0,     0,     0,     0,     0,   689,   688,   693,
     692,   574,   575,   577,   579,     0,   566,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   650,   259,   563,
     563,   186,   441,   495,     0,     0,   521,     0,     0,     0,
       0,   538,     0,     0,     0,   199,     0,   544,     0,     0,
     197,   207,     0,     0,   205,     0,     0,   221,     0,   217,
       0,     0,     0,     0,   520,     0,   235,     0,   233,   380,
     381,     0,   375,   364,     0,   485,   411,     0,   451,     0,
     146,   147,   145,   144,     0,   471,   470,   594,     0,   445,
     592,   444,     0,     0,   626,   493,   492,     0,     0,     0,
     522,     0,   269,   649,   602,   634,   499,   636,   500,   214,
       0,     0,     0,   651,   212,   548,     0,   654,   653,     0,
      50,    49,     0,    84,     0,     0,     0,    77,     0,     0,
      73,    47,   695,    76,     0,    63,    74,     0,   289,     0,
       0,   256,     0,     0,   307,   302,   305,     0,     0,   466,
     465,   594,   402,   411,   375,   318,     0,   349,     0,   314,
     315,   435,   594,     0,     0,     0,    96,    94,    95,    93,
      92,    91,   624,   625,   576,   578,     0,   565,   132,   139,
     138,   137,   134,   141,   142,   136,   140,   135,   143,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,   505,   506,   504,     0,   216,
       0,     0,   188,     0,   187,     0,   569,     0,     0,   194,
       0,     0,   192,     0,   202,     0,   200,     0,   230,   229,
       0,   224,     0,     0,   220,     0,   226,     0,   258,     0,
       0,   485,   366,     0,     0,   453,     0,     0,     0,   449,
     452,   453,     0,     0,   454,   455,     0,     0,   277,     0,
       0,   276,   279,   523,     0,   270,   273,     0,   215,     0,
       0,     0,     0,   213,     0,    89,    86,     0,    70,    69,
      68,     0,     0,     0,   169,     0,   170,     0,   291,   179,
       0,   284,   307,   159,   157,   283,   307,   292,   152,   300,
       0,   457,     0,   312,     0,   321,    99,   101,   362,   362,
     432,   631,   632,   567,   453,   615,   615,     0,     0,     0,
       0,     0,     0,   258,     0,     0,     0,   185,   184,   191,
       0,     0,   537,     0,   536,     0,   568,     0,     0,   543,
     195,     0,   542,   193,   203,   201,   223,   219,   553,   225,
       0,    56,   255,   234,   231,   453,   485,     0,   488,   487,
     489,   594,   408,   371,   369,   425,     0,   426,   413,   416,
       0,   412,   405,   406,   306,   446,   476,     0,   650,   501,
     501,     0,   281,     0,     0,     0,   271,     0,   210,   550,
       0,     0,   208,   549,     0,   655,     0,     0,     0,    73,
       0,    73,    78,    81,    65,    64,   291,   307,   260,   307,
     154,   293,   462,   460,   594,   650,     0,   363,   316,   317,
     571,     0,     0,     0,     0,     0,     0,     0,     0,   260,
     541,     0,     0,   570,   547,     0,     0,     0,   228,   372,
     370,   453,   477,     0,   410,   409,   424,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   474,   594,     0,
       0,   282,   280,     0,   274,     0,   211,     0,   209,    87,
      72,    71,     0,     0,     0,     0,     0,   307,   176,   257,
     153,   461,   594,   403,   313,   404,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   227,   374,   373,   479,
     480,   482,   594,     0,   650,   415,   417,   418,   421,   422,
     423,   419,   420,   414,     0,   588,   589,   272,     0,     0,
      80,    83,    79,    82,    66,   177,     0,     0,     0,     0,
       0,     0,   540,   539,   546,   545,     0,   481,   483,   594,
     590,   591,   552,   551,   477,   484
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1079, -1079, -1079,     0,  -261,  2238, -1079,     4, -1079, -1079,
      11,   592, -1079,  2141,   227,   514,  -561, -1079,  -801,  -796,
   -1079, -1079,    46, -1079, -1079,   372,   846, -1079,  1526,    48,
    -384, -1079,  -593,  2345,  -903,  -125,  -823, -1079, -1079,   129,
   -1079, -1079, -1079, -1079, -1079, -1079, -1079,   124, -1079,   787,
   -1079, -1079,   -55,   691, -1079, -1079, -1079, -1079, -1079,   560,
   -1079,   -78, -1079, -1079, -1079, -1079, -1079, -1079, -1079,  -555,
    -941, -1079, -1079, -1079, -1079,    44,  -344,  1227, -1079, -1079,
   -1079,   527, -1079, -1079, -1079, -1079,    29,  -172,  -639, -1079,
   -1079,    38,   122,   381, -1079, -1079, -1079,   642, -1079, -1079,
    -220,    93,  -933,    49,  -258,  -250,  -392, -1079, -1079,    47,
   -1079,    -5,   782,  -146,   384, -1079,  -504,  -909,  -947, -1079,
    -513,  -652, -1078, -1066,  -833,   -57, -1079,    23, -1079,  -267,
    -476,  -511,   347,  -524, -1079, -1079, -1079,  1289, -1079,     1,
   -1079, -1079,  -204, -1079,  -714, -1079, -1079, -1079,  1363,  1736,
     -12, -1079,     6,   564, -1079,  1810,  1877, -1079, -1079, -1079,
   -1079, -1079, -1079, -1079, -1079, -1079,  -437
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   339,   109,   759,   111,   112,   113,   114,
     115,   359,   116,   117,   118,   340,   833,   582,   837,   834,
     119,   120,   579,   580,   121,   122,   193,   650,   283,   123,
     258,   124,   785,   288,   125,   997,   126,   301,   300,   127,
     128,   129,   130,   131,   132,   133,   134,   899,   135,   271,
     136,   767,   768,   205,   138,   139,   140,   141,   142,   549,
     802,   967,   143,   144,   798,   962,   145,   146,   147,   369,
     844,   148,   149,   374,   847,   375,   376,   206,   151,   152,
     153,   154,  1116,   155,   156,   653,   654,   500,   855,   281,
     515,   516,   517,   518,   519,   390,   380,   385,   853,  1184,
    1074,   469,  1063,   948,  1068,  1069,  1070,   157,   158,   392,
     658,   159,   160,   284,   285,   524,   525,   789,   850,   604,
     528,   786,  1200,  1060,   945,   341,   222,   345,   346,   470,
     471,   472,   207,   162,   163,   164,   165,   208,   167,   190,
     191,   715,   483,   915,   716,   717,   168,   169,   209,   210,
     172,   366,   473,   212,   174,   213,   214,   177,   178,   179,
     180,   351,   181,   182,   183,   184,   185
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     188,   444,   108,   787,   211,   861,   652,   776,   216,   502,
     849,   304,   719,   393,   223,   859,   860,   218,   777,   514,
     215,   998,   264,   266,   268,   836,   272,   765,   597,   982,
     916,   229,   536,   981,   839,   799,  1064,   289,   230,   290,
     526,   797,  1075,   259,   295,  1073,   296,  1107,   297,   349,
     547,   275,   526,   286,  1113,  -253,   305,   309,   311,   313,
     314,   315,   487,   489,   492,   319,   286,   320,   918,   323,
     327,   959,   462,   675,   328,   310,  1201,   330,   331,   851,
     332,   481,   333,   911,   559,   334,   335,   495,   336,   675,
     602,   526,   305,   309,   350,   352,   562,   946,    69,  -468,
     286,  -254,  1065,   568,  1147,  1120,   676,   769,  1055,   560,
     564,   347,   726,   960,  -278,  1065,   876,   362,   186,   365,
     677,   404,  1139,   563,  1077,   726,  1237,   529,  1203,   410,
       3,   447,  1148,   862,   841,   448,   566,   565,  1238,   481,
     260,   842,  1098,   877,   567,   571,  1140,   456,   770,   678,
    -463,  -468,   444,   947,   481,   458,  -278,   397,   263,   265,
     267,   303,  1013,  -468,    69,  1177,  1245,   961,  1181,  1110,
     232,  1183,   679,   998,   481,  -468,   458,  1099,   705,   706,
    -463,  -468,   188,   527,  -468,   452,   453,   454,   679,   458,
     319,   305,   350,  -463,   680,   527,  -468,   303,   344,   303,
     569,  1067,   458,  1066,   458,    23,  -463,  -468,  1197,   569,
    -278,  -468,   539,  1073,  1067,  -468,   276,   461,   655,   458,
     309,   569,   569,  1141,   530,   569,   907,   481,   466,   570,
     348,   675,  1198,   603,   527,   230,   458,   874,   727,   573,
     291,   458,   458,   504,   356,  -464,   539,   599,   676,   898,
    1078,  1115,   845,  -463,  1204,   977,    59,  -105,  -463,  -468,
    -456,   381,   677,  -236,   309,  -111,   951,    67,   574,   836,
     943,    23,   600,   909,   950,  -464,   386,  -107,   875,   189,
     468,   357,   969,   987,   998,  -112,  1180,   309,  -464,   192,
    -456,   678,    87,  -486,   541,   542,   303,   303,  1173,   292,
    1175,  -464,  1172,  -456,  1174,   309,   377,   719,   321,   325,
     358,   593,   382,   464,  -486,  -237,  -456,   293,  -486,  -559,
     539,  1133,    59,   383,   468,   378,  -622,   387,  -622,   219,
     679,   561,   465,    67,   294,   379,   680,   737,   388,   540,
     558,  -486,   384,   530,   581,  1149,   583,   468,  -464,   161,
    -243,  1058,   217,  -464,   998,   218,   305,   389,    87,   161,
    -298,   591,   660,  -456,   938,   468,   990,   788,  -456,  1118,
    1119,  1150,   161,  1151,  1152,   161,  1014,  1153,  1154,   891,
     445,  -242,   446,   448,   661,  -298,   662,   663,   664,   666,
     668,   669,  1062,   670,   671,   672,   673,   674,   530,   682,
     683,   684,   685,   686,   687,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   698,  1096,   732,   983,   739,
    1061,   232,   309,   309,   161,  1026,  1157,  -458,   309,   537,
     538,   309,   309,   309,   309,   708,   710,   718,  -478,   719,
     700,   701,   459,  1169,   -53,   840,   702,   -52,   730,   703,
     704,   161,   733,   322,   326,   734,   161,  -458,   681,  -478,
     939,   303,   -53,  -478,   305,   -52,   741,   743,   737,  1035,
    -458,   822,   747,   749,   652,   737,  -573,  1114,   753,  -572,
      23,   -53,   737,  -458,   -52,   760,  -478,   940,   762,   826,
     823,   309,  -251,   468,   468,  1023,  1036,   530,   496,   499,
    -434,  -241,  1108,  -573,    42,   505,  -572,   276,   827,  1129,
     -53,   514,   186,   -52,   186,  -238,   989,   790,   556,  -434,
     820,   792,   793,  -245,   280,   665,   667,   796,  -249,    57,
    -458,    59,  -246,   273,   309,  -458,   274,   161,   836,  -623,
     836,  -623,    67,   298,   920,   923,   299,   796,   925,   927,
     309,   468,   309,  -622,  1158,  -622,   812,   796,   815,  -240,
     817,   819,  -619,   400,  -619,   316,   173,    87,   347,   303,
     347,   707,   709,   835,   835,  -621,   173,  -621,   558,  -620,
     864,  -620,   865,   996,   729,   806,   846,   808,  -244,   173,
     317,  1182,   173,   514,   468,  -250,  -252,  -239,   719,  -247,
     355,   367,   368,   742,  -556,   394,   449,   970,   972,   748,
     507,  -555,   404,   395,   752,   396,  -560,   408,  1202,   401,
     410,  -561,   457,   402,   413,  -558,   161,  -557,   451,   260,
     357,   508,   795,   161,   458,   509,   467,   856,   463,   474,
     476,   173,   503,   543,   575,   324,   324,   584,   533,   534,
     588,   546,   795,  -469,   594,   344,   595,   344,   510,   601,
     598,   511,   795,    61,   605,   659,  -469,   410,   173,   699,
     404,   657,   512,   173,  1146,   408,   725,   735,   410,   736,
    1202,   738,   413,   737,   744,   755,  1009,   161,   758,   756,
    1239,   513,   811,   137,   761,   763,  -469,   764,   773,   772,
     778,   779,   161,   137,   788,   794,   800,   801,   741,   894,
     747,   897,   760,   309,   900,   815,   137,   539,   810,   137,
    1202,   852,  -469,   309,   816,   309,   908,   953,   910,  -469,
     902,   903,   718,   854,   917,  -132,  -139,   656,  -138,  -108,
    -137,   906,  -134,   928,   929,  -141,  -106,   400,  -109,  -142,
    -469,  -136,   857,  -140,   173,  -367,   863,  1207,  1208,  1209,
    1210,  1211,  1212,  -135,  -110,  -143,   858,   866,   137,  -469,
     315,   319,   350,   468,  -469,   930,   309,  -469,   867,   868,
     872,   901,   932,   944,   869,   468,   870,   871,   983,  1018,
     988,  1002,   -75,   400,   955,   137,   958,  1021,   873,   949,
     137,   963,   984,   401,   986,   161,  1011,   402,  1050,  1012,
     581,   991,  1159,  1160,   978,  1051,   980,   835,   835,  1024,
    1052,  1056,  1076,  1079,  1085,   161,  1080,  1102,   905,  1103,
     309,   161,  1090,  1106,  1094,  1112,   350,  -105,  1117,  -107,
    1137,  1163,   161,   173,   161,   893,   996,   896,  1179,   401,
     173,  1215,  1220,   402,   404,  1216,  1221,  1222,  1223,   408,
    1240,   409,   410,  1148,   718,  1241,   413,   501,   975,   578,
     220,  1111,   976,   681,   420,  -473,   329,   303,   666,   708,
    1017,   137,   426,  1020,  1053,   551,  1084,  1008,  -473,   309,
     468,   999,  1004,   590,   942,   771,  1185,  1213,  1206,  1032,
     404,  1034,  1003,  1083,   173,   408,  1039,   409,   410,  1042,
    1010,   361,   413,   161,   791,     0,   996,   161,  -473,   173,
     420,  1048,     0,   520,  1109,   161,   424,   425,   426,     0,
       0,     0,     0,     0,     0,  1071,     0,     0,   225,     0,
       0,   303,    22,    23,  -473,     0,     0,     0,   305,   468,
       0,  -473,   226,     0,    31,   227,     0,  1089,     0,  1093,
      37,     0,  1095,     0,     0,   835,     0,    42,     0,     0,
     137,  1104,  -473,  1105,     0,     0,     0,   137,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   936,
       0,  -473,    57,     0,    59,     0,  -473,     0,   993,  -473,
    1143,   994,     0,   228,     0,    67,  1032,  1034,     0,  1039,
    1042,     0,  1089,  1093,   665,   707,     0,     0,  1130,  1131,
       0,  1132,   173,   718,    83,  1134,  1135,    85,   965,  1136,
      87,   137,     0,     0,     0,     0,     0,  1059,     0,   400,
       0,     0,   173,     0,     0,   350,   137,     0,   173,     0,
    1145,     0,     0,   303,     0,     0,     0,     0,     0,   173,
       0,   173,     0,  -475,   889,  1156,   815,   309,   309,     0,
       0,   507,     0,  1164,     0,  1165,  -475,     0,     0,  1167,
     102,     0,     0,     0,     0,  1171,   835,   835,   835,   835,
       0,     0,   508,     0,     0,   401,   509,     0,     0,   402,
       0,     0,     0,   815,     0,   161,  -475,     0,   161,  1130,
    1186,  1187,  1134,  1188,  1189,  1190,  1191,     0,     0,   510,
       0,     0,   511,     0,   353,     0,     0,   468,   468,     0,
     173,     0,  -475,   512,   173,  1205,     0,   499,     0,  -475,
       0,     0,   173,     0,   499,     0,   404,   405,     0,   137,
     303,   408,   513,   409,   410,  1144,     0,   161,   413,     0,
    -475,     0,     0,   161,  1224,     0,   420,  1054,     0,   137,
       0,     0,   424,   425,   426,   137,     0,     0,     0,  -475,
    1232,  1233,  1234,  1235,  -475,     0,   137,  -475,   137,     0,
       0,  1236,   815,     0,  1086,     0,     0,     0,     0,     0,
     507,     0,  1093,     0,     0,     0,  1242,  1243,     0,     0,
       0,     0,     0,     0,  1232,  1233,  1234,  1235,  1242,  1243,
       0,   508,     0,     0,     0,   509,     0,     0,     0,   150,
       0,     0,  1093,     0,   400,  1199,     0,  1015,  1016,   150,
       0,     0,     0,     0,     0,     0,     0,     0,   510,     0,
       0,   511,   150,    61,     0,   150,     0,   137,   161,     0,
     161,   137,   512,     0,     0,   161,     0,     0,   161,   137,
       0,     0,     0,     0,     0,     0,     0,   161,     0,   161,
       0,   513,   161,     0,     0,     0,     0,     0,     0,     0,
     401,   166,     0,     0,   402,     0,     0,  1199,     0,     0,
       0,   166,     0,     0,   150,     0,   161,     0,     0,     0,
       0,     0,     0,     0,   166,     0,   161,   166,   161,     0,
       0,     0,   173,     0,     0,   173,     0,     0,     0,     0,
       0,   150,     0,     0,     0,     0,   150,  1199,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
       0,     0,     0,   413,     0,     0,     0,     0,   499,   499,
       0,   420,   499,   499,   233,   170,   166,   424,   425,   426,
       0,     0,     0,     0,   173,   170,     0,   161,   234,   235,
     173,   236,     0,     0,   161,     0,   237,     0,   170,     0,
    1217,   170,   499,   166,   499,   238,     0,   161,   166,     0,
       0,   239,     0,     0,     0,     0,     0,   240,     0,     0,
       0,   241,     0,     0,   242,     0,     0,   150,     0,     0,
       0,     0,     0,     0,   243,     0,     0,     0,   161,     0,
     244,   245,     0,     0,     0,     0,     0,   161,   246,     0,
     170,   161,     0,     0,     0,     0,     0,   247,     0,   137,
       0,     0,   137,     0,     0,     0,   248,   249,     0,   250,
       0,   251,     0,   252,     0,     0,   253,   170,     0,     0,
     254,   521,   170,   255,     0,   173,   256,   173,   675,   166,
       0,     0,   173,     0,   161,   173,     0,     0,     0,     0,
       0,     0,   878,     0,   173,   879,   173,     0,     0,   173,
     880,   137,     0,     0,     0,     0,   150,   137,     0,     0,
       0,   196,     0,   150,     0,     0,     0,     0,     0,     0,
       0,   881,     0,   173,     0,     0,     0,     0,   882,     0,
       0,     0,   522,   173,     0,   173,     0,     0,   883,   161,
     161,   161,   161,     0,     0,   884,     0,     0,     0,     0,
       0,     0,     0,   170,     0,     0,   257,     0,     0,     0,
       0,   885,     0,     0,     0,   161,   161,   150,   166,     0,
     277,   278,   279,   886,     0,   166,     0,   679,     0,     0,
       0,     0,   150,     0,   887,     0,     0,     0,     0,   224,
     888,     0,     0,     0,   173,     0,     0,     0,     0,     0,
       0,   173,   137,   225,   137,     0,     0,    22,    23,   137,
       0,     0,   137,     0,   173,     0,     0,   226,     0,    31,
     227,   137,     0,   137,     0,    37,   137,     0,     0,   166,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   170,     0,   166,   173,   -51,     0,     0,   170,
     137,     0,     0,     0,   173,     0,     0,    57,   173,    59,
     137,     0,   137,    63,     0,   -51,    64,     0,   228,     0,
      67,     0,     0,   370,     0,   373,     0,   225,     0,     0,
       0,    22,    23,     0,   391,   150,   992,     0,     0,    83,
       0,   226,    85,    31,   227,    87,     0,     0,     0,    37,
       0,   173,     0,   170,     0,   150,    42,     0,     0,     0,
       0,   150,     0,     0,     0,     0,     0,     0,   170,     0,
       0,   137,   150,     0,   150,     0,   455,     0,   137,     0,
       0,    57,     0,    59,     0,    61,     0,   993,   171,     0,
     994,   137,   228,     0,    67,   102,     0,   166,   171,     0,
       0,     0,   257,     0,     0,     0,   173,   173,   173,   173,
       0,   171,     0,    83,   171,     0,    85,   166,     0,    87,
       0,     0,   137,   166,     0,     0,     0,     0,     0,     0,
       0,   137,   173,   173,   166,   137,   166,   475,     0,     0,
       0,     0,     0,   150,     0,   493,   494,   150,     0,     0,
       0,     0,     0,     0,     0,   150,     0,     0,   523,     0,
       0,     0,   175,   171,     0,     0,   400,     0,     0,   102,
       0,   170,   175,     0,     0,   995,     0,     0,   137,     0,
       0,     0,     0,     0,     0,   175,     0,     0,   175,     0,
     171,   170,     0,     0,     0,   171,     0,   170,     0,     0,
       0,     0,     0,     0,     0,   166,     0,     0,   170,   166,
     170,     0,     0,     0,     0,     0,     0,   166,     0,     0,
       0,     0,   401,     0,     0,     0,   402,     0,     0,   176,
       0,     0,     0,   137,   137,   137,   137,   175,     0,   176,
     589,     0,     0,   592,     0,     0,     0,     0,   373,     0,
       0,     0,   176,     0,     0,   176,     0,     0,     0,   137,
     137,     0,     0,     0,   175,     0,   651,     0,     0,   175,
       0,     0,     0,   404,   405,     0,   171,   407,   408,   170,
     409,   410,     0,   170,     0,   413,     0,     0,     0,   843,
       0,   170,     0,   420,     0,     0,     0,   400,     0,   424,
     425,   426,     0,     0,   176,     0,     0,     0,     0,     0,
    -291,     0,     0,     0,  -291,  -291,     0,     0,     0,  -291,
       0,     0,     0,     0,  -291,     0,  -291,  -291,     0,     0,
       0,   176,  -291,   728,     0,   150,   176,     0,   150,  -291,
       0,     0,  -291,     0,     0,     0,     0,     0,     0,     0,
     175,     0,     0,   401,     0,     0,     0,   402,     0,     0,
       0,  -291,     0,     0,  -291,   171,  -291,     0,  -291,     0,
    -291,  -291,   171,  -291,     0,  -291,     0,  -291,     0,     0,
     766,     0,     0,     0,     0,     0,     0,   150,     0,     0,
       0,     0,     0,   150,   775,     0,  -291,   166,   523,  -291,
     166,     0,  -291,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,     0,     0,     0,   413,   176,     0,     0,
       0,     0,     0,   419,   420,     0,   171,   423,     0,     0,
     424,   425,   426,     0,     0,     0,     0,     0,     0,   175,
       0,   171,     0,     0,     0,     0,   175,     0,     0,   166,
       0,     0,  -291,     0,     0,   166,     0,     0,  -291,     0,
     831,     0,     0,     0,   838,     0,     0,     0,     0,     0,
       0,   170,   373,     0,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   150,     0,
     150,     0,     0,     0,     0,   150,     0,     0,   150,     0,
     175,     0,     0,     0,     0,     0,   176,   150,     0,   150,
       0,     0,   150,   176,     0,   175,     0,     0,     0,     0,
       0,     0,     0,   170,     0,     0,     0,     0,     0,   170,
       0,     0,     0,     0,     0,     0,   150,     0,     0,     0,
     287,     0,     0,     0,   171,     0,   150,     0,   150,     0,
     166,     0,   166,     0,     0,     0,     0,   166,     0,     0,
     166,     0,   287,     0,   171,     0,     0,   176,     0,   166,
     171,   166,     0,     0,   166,     0,     0,     0,     0,     0,
       0,   171,   176,   171,     0,     0,     0,     0,     0,     0,
     110,     0,     0,     0,     0,     0,     0,     0,   166,     0,
     187,     0,     0,     0,     0,     0,     0,   150,   166,     0,
     166,     0,     0,   221,   150,     0,   231,     0,   175,     0,
       0,     0,     0,     0,   170,     0,   170,   150,     0,     0,
       0,   170,     0,     0,   170,     0,     0,     0,   175,   937,
       0,     0,     0,   170,   175,   170,     0,   941,   170,     0,
     523,     0,   171,     0,   523,   175,   171,   175,   150,     0,
       0,     0,     0,     0,   171,   318,     0,   150,     0,   166,
       0,   150,   170,     0,     0,     0,   166,   966,     0,     0,
       0,     0,   170,     0,   170,   176,     0,     0,     0,   166,
       0,     0,   110,     0,     0,     0,     0,   354,     0,     0,
       0,     0,     0,     0,     0,   176,     0,     0,     0,     0,
       0,   176,     0,     0,   150,     0,     0,     0,     0,     0,
     166,     0,   176,   373,   176,     0,   175,     0,     0,   166,
     175,     0,     0,   166,   651,     0,     0,     0,   175,   391,
       0,     0,     0,   170,     0,     0,     0,     0,     0,     0,
     170,     0,     0,     0,   287,   287,   287,   287,   287,   287,
       0,     0,     0,   170,     0,     0,   312,     0,     0,   150,
     150,   150,   150,     0,     0,     0,   166,     0,   450,     0,
     287,     0,     0,     0,     0,     0,   287,     0,     0,     0,
       0,     0,     0,   176,   170,   150,   150,   176,     0,     0,
       0,     0,     0,   170,   287,   176,     0,   170,   225,     0,
       0,     0,    22,    23,     0,   766,     0,   992,     0,     0,
     287,   287,   226,     0,    31,   227,     0,     0,     0,     0,
      37,   166,   166,   166,   166,     0,     0,    42,     0,     0,
     966,     0,     0,     0,   171,     0,     0,   171,     0,     0,
     170,     0,     0,     0,     0,     0,     0,   166,   166,     0,
       0,     0,    57,     0,    59,     0,    61,   506,   993,     0,
       0,   994,     0,   228,   531,    67,     0,     0,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,     0,    83,     0,   171,    85,     0,     0,
      87,     0,   171,     0,     0,   170,   170,   170,   170,  -580,
       0,     0,     0,     0,     0,     0,     0,     0,   175,     0,
       0,   175,   443,     0,     0,  -619,     0,  -619,   110,     0,
       0,   170,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   110,     0,     0,     0,     0,     0,     0,
     102,     0,   287,     0,     0,     0,  1178,     0,   477,   480,
     482,   486,   488,   491,     0,     0,     0,     0,     0,     0,
     175,     0,     0,     0,   287,     0,   175,     0,   287,     0,
     287,     0,     0,   287,   532,   176,     0,     0,   176,     0,
     535,   400,     0,     0,     0,     0,     0,   171,     0,   171,
       0,     0,     0,     0,   171,     0,     0,   171,   545,     0,
       0,     0,     0,     0,     0,     0,   171,     0,   171,     0,
       0,   171,     0,     0,   553,   554,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   176,     0,     0,
       0,     0,     0,   176,     0,   171,   110,   401,     0,     0,
       0,   402,     0,     0,     0,   171,     0,   171,     0,     0,
       0,     0,     0,     0,     0,     0,   531,     0,     0,     0,
       0,   175,   531,   175,     0,     0,     0,     0,   175,     0,
       0,   175,     0,   757,     0,     0,     0,     0,     0,     0,
     175,     0,   175,     0,     0,   175,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,     0,
     413,     0,     0,     0,     0,     0,   171,   419,   420,   175,
       0,   423,     0,   171,   424,   425,   426,     0,     0,   175,
       0,   175,     0,     0,     0,   427,   171,     0,   176,     0,
     176,     0,     0,     0,     0,   176,     0,     0,   176,     0,
       0,     0,     0,     0,   809,     0,   731,   176,   814,   176,
       0,     0,   176,     0,     0,     0,   110,   171,     0,     0,
       0,     0,     0,     0,     0,     0,   171,     0,   745,     0,
     171,     0,   750,     0,   751,     0,   176,   754,     0,     0,
     175,     0,     0,     0,     0,     0,   176,   175,   176,   400,
       0,     0,   225,     0,     0,     0,    22,    23,     0,     0,
     175,   992,     0,     0,     0,     0,   226,     0,    31,   227,
       0,     0,     0,   171,    37,     0,     0,     0,     0,     0,
       0,    42,     0,   287,   287,     0,     0,     0,     0,   287,
     287,   175,     0,   287,   287,     0,     0,     0,     0,     0,
     175,     0,     0,     0,   175,   401,    57,   176,    59,   402,
      61,     0,   993,     0,   176,   994,     0,   228,     0,    67,
       0,     0,     0,     0,     0,     0,     0,   176,   171,   171,
     171,   171,     0,     0,     0,     0,     0,     0,    83,     0,
       0,    85,     0,     0,    87,     0,     0,   175,     0,     0,
       0,     0,     0,     0,   171,   171,   404,   405,   176,   406,
     407,   408,     0,   409,   410,   411,     0,   176,   413,   414,
     415,   176,     0,   417,   418,   419,   420,     0,     0,   423,
       0,     0,   424,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,   427,   102,     0,   931,     0,     0,   934,
    1225,     0,   175,   175,   175,   175,     0,     0,     0,     0,
       0,     0,     0,     0,   176,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   175,   175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   287,   287,     0,     0,     0,   968,   287,
       0,     0,     0,     0,   973,     0,     0,     0,     0,     0,
       0,   287,     0,     0,   287,     0,   287,     0,   287,   176,
     176,   176,   176,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   233,   912,   914,  1072,
       0,     0,     0,   919,   922,   176,   176,   924,   926,     0,
     234,   235,     0,   236,     0,     0,     0,     0,   237,     0,
       0,   400,     0,     0,     0,     0,     0,   238,     0,     0,
       0,     0,     0,   239,     0,     0,     0,     0,     0,   240,
       0,     0,     0,   241,     0,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,   531,
       0,   531,   244,   245,     0,     0,   531,     0,     0,   531,
     246,     0,     0,     0,     0,     0,     0,   401,  1046,   247,
    1047,   402,     0,  1049,     0,     0,     0,     0,   248,   249,
       0,   250,     0,   251,     0,   252,     0,     0,   253,     0,
       0,     0,   254,     0,     0,   255,     0,  1082,   256,     0,
       0,     0,   403,     0,     0,     0,     0,  1088,     0,  1092,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,   287,   409,   410,   411,     0,   412,
     413,   414,   415,     0,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,  1027,  1028,     0,
       0,     0,   458,  1029,   964,   427,     0,  1072,     0,     0,
       0,     0,     0,     0,     0,  1040,     0,     0,  1043,     0,
    1044,     0,  1045,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,  1138,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   443,     0,  1161,
    -619,     0,  -619,     0,     0,     0,     0,     0,  1166,     0,
       0,     0,  1168,     0,     0,     0,     0,     0,     0,     0,
      -2,     4,     0,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,  1196,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -51,     0,    40,
      41,    42,     0,    43,  -307,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -307,     0,     0,    57,    58,    59,    60,
      61,    62,    63,  -307,   -51,    64,    65,    66,  1162,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -51,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,  -650,     0,    12,    13,
      14,    15,    16,  -650,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,  -650,    28,    29,  -650,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -51,     0,    40,    41,    42,     0,    43,  -307,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -307,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -307,   -51,    64,
      65,    66,  -650,    67,    68,    69,  -650,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -51,    87,    88,
       0,     0,    89,     0,    90,     0,     0,  -650,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -650,  -650,    94,  -650,  -650,  -650,  -650,
    -650,  -650,  -650,     0,  -650,  -650,  -650,  -650,  -650,     0,
    -650,  -650,  -650,  -650,  -650,  -650,  -650,  -650,   102,  -650,
    -650,  -650,     0,   104,  -650,   105,     0,   106,     0,   337,
    -650,     5,   302,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -51,     0,    40,    41,    42,
       0,    43,  -307,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -307,     0,     0,    57,    58,    59,    60,    61,    62,
      63,  -307,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -51,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,   338,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -51,
       0,    40,    41,    42,     0,    43,  -307,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -307,     0,     0,    57,    58,
      59,    60,    61,    62,    63,  -307,   -51,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -51,    87,    88,     0,     0,
      89,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   102,     0,     0,   103,
       0,   104,   557,   105,     0,   106,     0,   576,   107,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -51,     0,    40,    41,    42,     0,    43,
    -307,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -307,
       0,     0,    57,    58,    59,    60,    61,    62,    63,  -307,
     -51,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -51,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,   577,   105,     0,   106,
       0,   337,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -51,     0,    40,
      41,    42,     0,    43,  -307,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -307,     0,     0,    57,    58,    59,    60,
      61,    62,    63,  -307,   -51,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -51,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
     338,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -51,     0,    40,    41,    42,     0,    43,  -307,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -307,     0,     0,
      57,    58,    59,    60,    61,    62,    63,  -307,   -51,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -51,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,   821,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -51,     0,    40,    41,    42,
       0,    43,  -307,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -307,     0,     0,    57,    58,    59,    60,   353,    62,
      63,  -307,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -51,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -51,
       0,    40,    41,    42,     0,    43,  -307,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -307,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -307,   -51,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -51,    87,    88,     0,     0,
      89,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   102,     0,     0,   103,
       0,   104,     0,   105,     0,   106,     0,     4,   107,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,   740,    39,   -51,     0,    40,    41,    42,     0,    43,
    -307,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -307,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -307,
     -51,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -51,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,     0,   105,     0,   106,
       0,     4,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,   746,    39,   -51,     0,    40,
      41,    42,     0,    43,  -307,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -307,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -307,   -51,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -51,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1031,
      39,   -51,     0,    40,    41,    42,     0,    43,  -307,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -307,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -307,   -51,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -51,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,     0,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1033,    39,   -51,     0,    40,    41,    42,
       0,    43,  -307,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -307,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -307,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -51,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1038,    39,   -51,
       0,    40,    41,    42,     0,    43,  -307,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -307,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -307,   -51,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -51,    87,    88,     0,     0,
      89,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   102,     0,     0,   103,
       0,   104,     0,   105,     0,   106,     0,     4,   107,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1041,    39,   -51,     0,    40,    41,    42,     0,    43,
    -307,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -307,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -307,
     -51,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -51,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,     0,   105,     0,   106,
       0,     4,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,  1081,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -51,     0,    40,
      41,    42,     0,    43,  -307,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -307,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -307,   -51,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -51,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1087,
      39,   -51,     0,    40,    41,    42,     0,    43,  -307,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -307,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -307,   -51,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -51,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,     0,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1091,    39,   -51,     0,    40,    41,    42,
       0,    43,  -307,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -307,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -307,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -51,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,  1000,   107,     5,   302,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   203,     0,   105,     0,   204,  1001,   952,   107,     5,
     302,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     194,     0,     0,    15,    16,     0,    17,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   196,     0,     0,     0,    33,   197,   198,     0,
       0,   199,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   200,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   201,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   202,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   203,     0,   105,     0,   204,
       0,     0,   107,     5,   302,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,   306,   307,     0,    84,
     342,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,   343,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,     0,   204,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,   360,    23,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
      59,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,    67,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,    87,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   203,     0,   105,     0,   204,     0,     0,   107,     5,
     302,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     194,     0,     0,    15,    16,     0,    17,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   196,     0,     0,     0,    33,   197,   198,     0,
       0,   199,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   200,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   201,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   202,     0,    78,     0,
       0,    80,   306,   307,     0,    84,   342,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   203,     0,   105,   805,   204,
       0,     0,   107,     5,   302,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,   306,   307,     0,    84,
     342,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,   807,   204,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,     0,   711,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
     712,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,   713,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,   714,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   203,     0,   105,     0,   204,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   199,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,     0,    80,
     306,   307,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   308,     0,
       0,   103,     0,   203,     0,   105,     0,   204,     0,     0,
     107,     5,   302,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,   199,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,   306,   307,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,   204,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,   954,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,   306,   307,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,     0,   204,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
     261,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,   262,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   203,     0,   105,     0,   204,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,   269,     0,     0,     0,     0,     0,     0,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   199,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,   270,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,     0,   204,     0,     0,
     107,     5,   302,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,   199,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,   262,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,   204,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,   306,   307,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,     0,   204,     0,     0,   107,     5,   302,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   199,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,   803,   105,     0,   204,     0,     0,
     107,     5,   302,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,   199,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,   204,   813,     0,   107,     5,   302,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   203,     0,   105,     0,   204,  1142,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   199,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,   262,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,     0,   204,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     194,     0,     0,    15,    16,     0,    17,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   286,
      29,     0,   196,     0,     0,     0,    33,   197,   198,     0,
       0,   199,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   200,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   201,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   202,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   104,     0,   105,     0,   204,
       0,     0,   107,     5,   302,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,     0,   204,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   203,     0,   105,     0,   204,   818,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   199,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,     0,    80,
       0,     0,     0,    84,   979,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,     0,   204,     0,     0,
     107,     5,   302,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   780,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,   199,    39,     0,     0,     0,   781,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,   782,    74,    75,    76,   783,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,  1057,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,     0,     0,     0,    84,
    1170,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,     0,   204,     0,     0,   107,     5,   302,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   199,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,     0,  1057,     0,     0,
     107,     5,   302,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,  1128,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,   204,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,     0,   204,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   104,     0,   105,     0,   204,     0,     5,   107,     6,
       7,     8,   363,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   199,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   364,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,     0,   204,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     194,     0,     0,    15,    16,     0,    17,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
     780,     0,   196,     0,     0,     0,    33,   197,   198,     0,
       0,   199,    39,     0,     0,     0,   781,     0,     0,    43,
       0,     0,   200,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   201,     0,     0,     0,     0,     0,     0,
       0,    72,   782,    74,    75,    76,   783,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   203,     0,   105,     0,   784,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,   199,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   832,     0,   203,     0,   105,
       0,   204,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,   780,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,     0,     0,
     781,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,   782,    74,    75,    76,
     783,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,     0,   848,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   203,     0,   105,     0,   784,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   892,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,     0,   204,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     194,     0,     0,    15,    16,     0,    17,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   196,     0,     0,     0,    33,   197,   198,     0,
       0,   895,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   200,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   201,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   202,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   203,     0,   105,     0,   204,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,  1122,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,   204,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,  1123,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,     0,   204,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,  1125,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   203,     0,   105,     0,   204,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,  1126,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,     0,   204,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     194,     0,     0,    15,    16,     0,    17,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   196,     0,     0,     0,    33,   197,   198,     0,
       0,  1127,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   200,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   201,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   202,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   203,     0,   105,     0,   204,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,  1128,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,   204,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   606,     0,   607,     0,     0,     0,    92,
      93,    94,    95,    96,   608,     0,     0,     0,    97,   609,
     235,   610,   611,     0,     0,     0,     0,   612,    98,     0,
       0,    99,   100,   101,     0,     0,   238,   103,   196,     0,
       0,   105,   613,   848,     0,     0,   107,     0,   614,     0,
       0,     0,   241,     0,     0,   615,     0,   616,     0,     0,
       0,     0,     0,     0,     0,   617,     0,     0,     0,     0,
       0,   618,   619,     0,     0,     0,     0,     0,     0,   246,
       0,     0,     0,     0,     0,     0,     0,     0,   620,     0,
       0,     0,     0,     0,     0,     0,     0,   248,   249,     0,
     621,     0,   251,     0,   622,     0,     0,   623,     0,     0,
       0,   624,     0,     0,   255,     0,     0,   625,     0,     0,
       0,     0,     0,     0,     0,     0,   498,   429,   430,   431,
     432,   433,     0,     0,   436,   437,   438,   439,     0,   441,
     442,   626,   627,   628,   629,   630,   631,     0,   632,     0,
       0,     0,   633,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,     0,   644,     0,     0,   645,   646,   647,
     648,     0,     5,   649,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   194,     0,     0,    15,    16,     0,    17,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   196,     0,     0,     0,    33,
     197,   198,     0,     0,   199,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   200,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   201,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   202,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
       0,   100,   101,     0,     0,     0,   103,     0,   203,     0,
     105,     5,   204,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,   199,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,  -459,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,  -459,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,  -459,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,  -459,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     5,   302,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,   101,  -459,    21,     0,     0,     0,  -459,     0,   105,
       0,   848,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,   199,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,  -477,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     0,     0,  -477,     0,     0,     0,  -477,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   101,     0,     0,     0,     0,     0,   203,     0,   105,
    -477,  1057,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   194,     0,     0,    15,    16,     0,    17,
       0,   195,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   196,     0,     0,     0,    33,
     197,   198,     0,     0,   199,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   200,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   201,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   202,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,     0,
      94,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   194,     0,     0,    15,    16,     0,    17,
       0,   195,   101,     0,    21,     0,     0,     0,   203,     0,
     105,     0,   204,    29,     0,   196,     0,     0,     0,    33,
     197,   198,     0,     0,   199,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   200,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   201,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   202,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   194,     0,     0,    15,    16,     0,    17,
       0,   195,   101,     0,    21,     0,     0,     0,   203,     0,
     105,     0,   784,    29,     0,   196,     0,     0,     0,    33,
     197,   198,     0,     0,   199,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   200,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   201,     0,   233,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   202,
       0,    78,   234,   235,    80,   236,     0,     0,    84,     0,
     237,    86,     0,     0,    88,     0,     0,     0,     0,   238,
       0,     0,     0,     0,     0,   239,     0,     0,     0,     0,
       0,   240,     0,     0,     0,   241,     0,     0,   242,     0,
      94,     0,     0,     0,     0,     0,     0,   225,   243,     0,
       0,    22,    23,     0,   244,   245,     0,     0,     0,     0,
       0,   226,   246,    31,   227,     0,     0,     0,     0,    37,
     105,   247,   204,     0,   233,     0,    42,     0,     0,     0,
     248,   249,     0,   250,     0,   251,     0,   252,   234,   235,
     253,   236,     0,     0,   254,     0,   237,   255,     0,     0,
     256,    57,     0,    59,     0,   238,     0,   993,     0,     0,
     994,   239,   228,     0,    67,     0,     0,   240,     0,     0,
       0,   241,     0,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,    83,   243,     0,    85,     0,     0,    87,
     244,   245,     0,     0,     0,     0,     0,     0,   246,     0,
       0,     0,     0,     0,     0,     0,   282,   247,     0,     0,
       0,     0,     0,     0,     0,     0,   248,   249,     0,   250,
       0,   251,     0,   252,     0,     0,   253,     0,  1006,     0,
     254,     0,     0,   255,     0,     0,   256,   608,     0,   102,
       0,     0,   234,   235,   610,   236,     0,     0,     0,     0,
     237,     0,     0,     0,     0,     0,     0,     0,     0,   238,
       0,     0,     0,     0,     0,   613,     0,     0,     0,     0,
       0,   240,     0,     0,     0,   241,     0,     0,   242,     0,
     616,     0,     0,     0,     0,     0,     0,     0,   243,     0,
       0,     0,   774,     0,   618,   245,     0,     0,     0,     0,
       0,     0,   246,     0,     0,     0,     0,     0,     0,     0,
       0,   247,     0,     0,     0,     0,     0,     0,     0,   398,
     248,   249,     0,   250,     0,   251,   399,  1007,     0,     0,
     623,     0,     0,     0,   254,     0,     0,   255,     0,   400,
     256,     0,     0,     0,     0,     0,     0,     0,     0,   498,
     429,   430,   431,   432,   433,     0,     0,   436,   437,   438,
     439,     0,   441,   442,   626,   627,   628,   629,   630,   631,
       0,   632,     0,     0,     0,   633,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,     0,   644,     0,     0,
     645,   646,   647,   648,     0,   401,     0,     0,     0,   402,
       0,     0,   398,     0,     0,     0,     0,     0,   497,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
     403,   498,   429,   430,   431,   432,   433,     0,     0,   436,
     437,   438,   439,     0,   441,   442,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   458,     0,   401,     0,
       0,     0,   402,   427,     0,   398,     0,     0,     0,     0,
       0,  1030,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,   403,   498,   429,   430,   431,   432,   433,
       0,     0,   436,   437,   438,   439,     0,   441,   442,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   458,
       0,   401,     0,     0,     0,   402,   427,     0,   398,     0,
       0,     0,     0,     0,  1037,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,   403,   498,   429,   430,
     431,   432,   433,     0,     0,   436,   437,   438,   439,     0,
     441,   442,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   458,     0,   401,     0,     0,     0,   402,   427,
       0,   398,     0,     0,     0,     0,     0,  1192,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,   403,
     498,   429,   430,   431,   432,   433,     0,     0,   436,   437,
     438,   439,     0,   441,   442,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   458,     0,   401,     0,     0,
       0,   402,   427,     0,   398,     0,     0,     0,     0,     0,
    1193,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,   403,   498,   429,   430,   431,   432,   433,     0,
       0,   436,   437,   438,   439,     0,   441,   442,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,   458,     0,
     401,     0,     0,     0,   402,   427,     0,   398,     0,     0,
       0,     0,     0,  1194,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,   403,   498,   429,   430,   431,
     432,   433,     0,     0,   436,   437,   438,   439,     0,   441,
     442,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,   458,     0,   401,     0,     0,     0,   402,   427,     0,
     398,     0,     0,     0,     0,     0,  1195,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,   403,   498,
     429,   430,   431,   432,   433,     0,     0,   436,   437,   438,
     439,     0,   441,   442,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,   458,     0,   401,     0,     0,     0,
     402,   427,     0,   398,     0,     0,     0,     0,     0,  1218,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,   403,   498,   429,   430,   431,   432,   433,     0,     0,
     436,   437,   438,   439,     0,   441,   442,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,   458,     0,   401,
       0,     0,     0,   402,   427,     0,   398,     0,     0,     0,
       0,     0,  1219,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,   484,     0,     0,
       0,     0,     0,     0,   403,   498,   429,   430,   431,   432,
     433,     0,   485,   436,   437,   438,   439,     0,   441,   442,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   424,   425,   426,     0,
     458,     0,   401,     0,     0,     0,   402,   427,   398,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,   286,
       0,     0,     0,   481,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,   490,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,     0,   458,   401,     0,     0,     0,   402,     0,
     427,   398,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,   478,     0,     0,   481,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,   479,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   458,     0,   401,     0,     0,
       0,   402,   427,     0,   398,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,   585,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   586,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,   458,     0,
     401,     0,     0,     0,   402,   427,     0,   398,   828,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,   829,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   830,   417,   418,
     419,   420,   421,   422,   423,     0,   587,   424,   425,   426,
       0,     0,     0,   401,     0,     0,     0,   402,   427,   398,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     921,     0,     0,     0,     0,     0,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,     0,   401,     0,     0,     0,   402,
       0,   427,   398,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,   286,     0,     0,   481,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   458,     0,   401,     0,
       0,     0,   402,   427,   398,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,   481,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
     460,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   400,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   458,
     401,     0,     0,     0,   402,   398,   427,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,   286,     0,     0,     0,
       0,   189,     0,     0,     0,   403,     0,     0,     0,     0,
       0,   401,     0,     0,     0,   402,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,   403,   424,   425,   426,
       0,   401,     0,     0,     0,   402,     0,   398,   427,     0,
       0,     0,   404,   405,   399,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   400,   720,   417,
     418,   419,   420,   421,     0,   423,   403,     0,   424,   425,
     426,     0,     0,   721,     0,     0,     0,     0,     0,   427,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   458,   401,     0,     0,     0,   402,   398,   427,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,   722,
       0,     0,     0,     0,     0,     0,     0,     0,   403,     0,
       0,     0,     0,     0,   723,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,   401,     0,     0,     0,   402,   398,
       0,   427,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     913,     0,   225,     0,     0,     0,    22,    23,     0,   403,
       0,     0,     0,     0,     0,     0,   226,     0,    31,   227,
       0,     0,     0,     0,    37,   404,   405,     0,   406,   407,
     408,    42,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   401,    57,     0,    59,   402,
       0,   398,   427,     0,     0,     0,     0,   228,   399,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,    83,     0,
     403,    85,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   458,   401,     0,     0,
       0,   402,     0,   427,   398,     0,     0,     0,     0,     0,
     935,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,   458,     0,
     401,     0,     0,     0,   402,   427,     0,   398,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,   481,  1100,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,  1101,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,     0,     0,   401,     0,     0,   971,   402,   427,   398,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,     0,   401,     0,     0,     0,   402,
       0,   427,   398,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   544,     0,   401,     0,
       0,     0,   402,   427,   398,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,   548,   424,   425,   426,     0,     0,
     401,     0,     0,     0,   402,   398,   427,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,   550,   424,   425,   426,
       0,   401,     0,     0,     0,   402,   398,     0,   427,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,   552,   424,   425,
     426,     0,   401,     0,     0,     0,   402,   398,     0,   427,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,   555,   424,
     425,   426,     0,   401,     0,     0,     0,   402,   398,     0,
     427,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,   572,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,   401,     0,     0,     0,   402,   398,
       0,   427,     0,     0,     0,     0,   399,   724,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   401,     0,     0,     0,   402,
     398,   824,   427,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   401,     0,     0,   804,
     402,   398,     0,   427,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   825,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,   401,     0,     0,
       0,   402,   398,     0,   427,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,  -581,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,   401,     0,
       0,     0,   402,   398,     0,   427,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,   189,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   401,
       0,     0,     0,   402,   398,     0,   427,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,   933,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,   904,   424,   425,   426,     0,
     401,     0,     0,     0,   402,   398,     0,   427,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,   401,     0,     0,     0,   402,   398,     0,   427,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   401,     0,     0,   956,   402,   398,     0,   427,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,     0,   401,     0,     0,   957,   402,   398,   985,
     427,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,   974,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,   401,     0,     0,     0,   402,   398,
       0,   427,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   401,     0,     0,     0,   402,
     398,     0,   427,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,  1019,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   401,     0,     0,  1005,
     402,   398,     0,   427,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,  1022,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,   401,     0,     0,
       0,   402,   398,     0,   427,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,   401,     0,
       0,     0,   402,   398,  1097,   427,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   401,
       0,     0,     0,   402,  1025,   398,   427,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   424,   425,   426,     0,
       0,   401,     0,     0,     0,   402,   398,   427,     0,     0,
       0,     0,     0,   399,  1121,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   401,     0,     0,     0,   402,     0,     0,   427,
       0,     0,     0,     0,     0,  1124,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,  1155,
     417,   418,   419,   420,   421,   422,   423,   398,  1176,   424,
     425,   426,     0,     0,   399,     0,     0,     0,     0,     0,
     427,     0,     0,     0,     0,     0,   401,   400,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,   401,   409,   410,   411,   402,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,     0,     0,   400,
       0,     0,     0,     0,   427,     0,     0,     0,   403,     0,
       0,     0,     0,   401,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,   403,     0,
     424,   425,   426,     0,     0,   401,     0,     0,     0,   402,
       0,   427,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,   398,     0,
     424,   425,   426,     0,     0,   399,     0,     0,     0,  1214,
       0,   427,     0,     0,     0,     0,   404,   405,   400,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,     0,     0,   417,   418,   419,   420,   421,   398,   423,
       0,     0,   424,   425,   426,   399,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,     0,  1226,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,   401,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,   404,   405,  1227,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,   403,
       0,   424,   425,   426,     0,     0,     0,     0,     0,     0,
       0,     0,   427,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,   398,
     225,   424,   425,   426,    22,    23,   399,     0,     0,     0,
       0,     0,   427,     0,   226,     0,    31,   227,     0,   400,
       0,     0,    37,     0,     0,     0,     0,     0,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,   -51,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,     0,   400,
      63,     0,   -51,    64,     0,   228,     0,    67,     0,     0,
       0,     0,     0,     0,     0,   401,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,    83,     0,  1228,    85,
       0,     0,    87,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,   401,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,   404,   405,  1229,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   102,   417,   418,   419,   420,   421,   422,   423,
     403,     0,   424,   425,   426,     0,     0,     0,     0,     0,
       0,     0,     0,   427,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
     398,   225,   424,   425,   426,    22,    23,   399,     0,     0,
     992,     0,     0,   427,     0,   226,     0,    31,   227,     0,
     400,     0,     0,    37,     0,     0,     0,     0,     0,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,   353,
     400,   993,     0,     0,   994,     0,   228,     0,    67,     0,
       0,     0,     0,     0,     0,     0,   401,     0,     0,     0,
     402,   398,     0,     0,     0,     0,     0,    83,   399,  1230,
      85,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,   401,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,   404,   405,  1231,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   102,   417,   418,   419,   420,   421,   422,
     423,   403,     0,   424,   425,   426,     0,   401,     0,     0,
       0,   402,     0,     0,   427,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,   403,   424,   425,   426,     0,     0,   398,     0,
       0,     0,     0,     0,   427,   399,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,   400,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,     0,   398,
       0,     0,     0,  1244,     0,   427,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,   890,     0,     0,     0,   402,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
     403,   424,   425,   426,     0,     0,     0,     0,     0,     0,
       0,     0,   427,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,   371,     0,   233,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,     0,
       0,   234,   235,     0,   236,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,   372,     0,     0,   238,     0,
       0,   233,     0,     0,   239,     0,   596,     0,     0,     0,
     240,     0,     0,     0,   241,   234,   235,   242,   236,     0,
       0,     0,     0,   237,     0,     0,     0,   243,     0,     0,
       0,     0,   238,   244,   245,     0,     0,     0,   239,     0,
       0,   246,     0,     0,   240,     0,     0,     0,   241,     0,
     247,   242,     0,     0,     0,     0,     0,     0,     0,   248,
     249,   243,   250,     0,   251,     0,   252,   244,   245,   253,
       0,     0,     0,   254,     0,   246,   255,     0,     0,   256,
       0,     0,     0,     0,   247,     0,     0,   233,     0,     0,
       0,     0,     0,   248,   249,     0,   250,     0,   251,     0,
     252,   234,   235,   253,   236,     0,     0,   254,     0,   237,
     255,     0,     0,   256,     0,   372,     0,     0,   238,     0,
       0,  -286,     0,     0,   239,     0,     0,     0,     0,     0,
     240,     0,     0,     0,   241,  -286,  -286,   242,  -286,     0,
       0,     0,     0,  -286,     0,     0,     0,   243,     0,     0,
       0,     0,  -286,   244,   245,     0,     0,     0,  -286,     0,
       0,   246,     0,     0,  -286,     0,     0,     0,  -286,     0,
     247,  -286,     0,     0,     0,     0,     0,     0,     0,   248,
     249,  -286,   250,     0,   251,     0,   252,  -286,  -286,   253,
       0,     0,     0,   254,     0,  -286,   255,     0,     0,   256,
       0,     0,     0,     0,  -286,     0,     0,   233,     0,     0,
       0,     0,     0,  -286,  -286,     0,  -286,     0,  -286,     0,
    -286,   234,   235,  -286,   236,     0,     0,  -286,     0,   237,
    -286,     0,     0,  -286,     0,     0,     0,     0,   238,     0,
       0,  -287,     0,     0,   239,     0,     0,     0,     0,     0,
     240,     0,     0,     0,   241,  -287,  -287,   242,  -287,     0,
       0,     0,     0,  -287,     0,     0,     0,   243,     0,     0,
       0,     0,  -287,   244,   245,     0,     0,     0,  -287,     0,
       0,   246,     0,     0,  -287,     0,     0,     0,  -287,     0,
     247,  -287,     0,     0,     0,     0,     0,     0,     0,   248,
     249,  -287,   250,     0,   251,     0,   252,  -287,  -287,   253,
       0,     0,     0,   254,     0,  -287,   255,     0,     0,   256,
       0,     0,     0,     0,  -287,     0,     0,     0,     0,     0,
       0,     0,     0,  -287,  -287,     0,  -287,     0,  -287,     0,
    -287,     0,     0,  -287,     0,     0,     0,  -287,     0,     0,
    -287,     0,     0,  -287
};

static const yytype_int16 yycheck[] =
{
      12,   173,     2,   527,    16,   657,   390,   520,    20,   276,
     603,    68,   449,   159,    26,   654,   655,    22,   522,   280,
      19,   844,    34,    35,    36,   586,    38,   503,   372,   830,
     744,    27,   299,   829,   589,   546,   945,    49,    27,    51,
       1,     1,   951,    11,    56,   948,    58,   988,    60,   106,
     317,    40,     1,    33,  1001,    56,    68,    69,    70,    71,
      72,    73,   266,   267,   268,    77,    33,    79,    33,    81,
      82,    74,   218,     3,    86,    69,  1142,    89,    90,   603,
      92,   119,    94,    33,   145,    97,    98,   101,   100,     3,
       1,     1,   104,   105,   106,   107,   145,    61,    91,    48,
      33,    56,     3,    48,   117,  1014,    20,    48,   941,   170,
     145,   105,    48,   116,    74,     3,   145,   129,   164,   131,
      34,   139,  1055,   172,    48,    48,  1204,    48,    48,   147,
       0,    88,   145,   657,   164,    92,   174,   172,  1204,   119,
     108,   171,    37,   172,   348,   349,  1055,   204,    89,    63,
      61,    61,   324,   117,   119,   169,   116,   169,    34,    35,
      36,    68,   876,   124,    91,  1106,  1244,   170,  1115,   992,
     171,   164,   102,   996,   119,   124,   169,    72,   445,   446,
      91,    91,   194,   144,   145,   197,   198,   199,   102,   169,
     202,   203,   204,   104,   108,   144,   145,   104,   105,   106,
     145,   102,   169,    91,   169,    27,   117,   117,  1141,   145,
     170,   172,   145,  1116,   102,   164,   171,   216,   390,   169,
     232,   145,   145,  1056,   145,   145,   737,   119,   224,   174,
     106,     3,  1141,   144,   144,   224,   169,    48,   174,   145,
      18,   169,   169,   171,    47,    61,   145,   145,    20,   725,
     174,   174,   596,   164,   174,   169,    78,   139,   169,   169,
      61,    27,    34,    56,   276,   147,   779,    89,   174,   830,
     774,    27,   170,   172,   778,    91,    27,   139,    89,   119,
     232,    84,   174,   838,  1107,   147,  1109,   299,   104,     3,
      91,    63,   114,   124,   306,   307,   203,   204,  1099,    77,
    1101,   117,  1098,   104,  1100,   317,    54,   744,    81,    82,
     113,   368,    78,   145,   145,    56,   117,    95,   149,   164,
     145,  1035,    78,    89,   276,    73,   171,    78,   173,    89,
     102,   343,   164,    89,   112,    83,   108,   145,    89,   164,
     340,   172,   108,   145,   356,   124,   358,   299,   164,     2,
      56,   944,   108,   169,  1177,   360,   368,   108,   114,    12,
     145,   366,   164,   164,   172,   317,   842,   124,   169,  1008,
    1009,   150,    25,   152,   153,    28,   889,   156,   157,    88,
     171,    56,   173,    92,   396,   170,   398,   399,   400,   401,
     402,   403,   149,   405,   406,   407,   408,   409,   145,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   977,   164,   145,   476,
     944,   171,   434,   435,    77,   901,  1078,    61,   440,   300,
     301,   443,   444,   445,   446,   447,   448,   449,   124,   876,
     434,   435,   215,   170,    47,   591,   440,    47,   460,   443,
     444,   104,   464,    81,    82,   467,   109,    91,   410,   145,
     145,   368,    65,   149,   476,    65,   478,   479,   145,   145,
     104,   145,   484,   485,   858,   145,   145,  1001,   490,   145,
      27,    84,   145,   117,    84,   497,   172,   172,   500,   145,
     164,   503,    56,   445,   446,   172,   172,   145,   271,   272,
     145,    56,   172,   172,    51,   278,   172,   171,   164,   172,
     113,   772,   164,   113,   164,    56,   164,   529,   170,   164,
     170,   533,   534,    56,   171,   401,   402,   539,    56,    76,
     164,    78,    56,    82,   546,   169,    85,   190,  1099,   171,
    1101,   173,    89,     8,   748,   749,   171,   559,   752,   753,
     562,   503,   564,   171,  1078,   173,   568,   569,   570,    56,
     572,   573,   171,    32,   173,    99,     2,   114,   562,   476,
     564,   447,   448,   585,   586,   171,    12,   173,   578,   171,
     171,   173,   173,   844,   460,   562,   598,   564,    56,    25,
     171,  1115,    28,   854,   546,    56,    56,    56,  1035,    56,
     169,    56,   144,   479,   164,   163,   171,   811,   812,   485,
      27,   164,   139,   164,   490,   155,   164,   144,  1142,    88,
     147,   164,    56,    92,   151,   164,   279,   164,   164,   108,
      84,    48,   539,   286,   169,    52,   118,   649,   164,   169,
     171,    77,   171,   164,     8,    81,    82,    65,   171,   171,
      87,   171,   559,    48,   169,   562,   170,   564,    75,    89,
     124,    78,   569,    80,    89,   164,    61,   147,   104,    68,
     139,   124,    89,   109,  1066,   144,   171,   124,   147,   172,
    1204,    48,   151,   145,   171,   124,   858,   340,    35,   124,
    1214,   108,   568,     2,    35,    65,    91,   172,   172,   145,
     145,   172,   355,    12,   124,   172,   172,    21,   720,   721,
     722,   723,   724,   725,   726,   727,    25,   145,   174,    28,
    1244,   104,   117,   735,   174,   737,   738,   784,   740,   124,
     729,   730,   744,   171,   746,   147,   147,   390,   147,   139,
     147,   735,   147,   755,   756,   147,   139,    32,   139,   147,
     145,   147,   139,   147,   190,   172,   145,  1149,  1150,  1151,
    1152,  1153,  1154,   147,   139,   147,   147,   164,    77,   164,
     782,   783,   784,   725,   169,    35,   788,   172,   164,   164,
     172,   171,    35,   144,   164,   737,   164,   164,   145,    33,
     169,   848,   164,    32,   788,   104,   170,    33,   174,   172,
     109,   172,   164,    88,   164,   458,   172,    92,    35,   174,
     822,   170,  1079,  1080,   826,   164,   828,   829,   830,   174,
     164,   172,   174,   171,   144,   478,   171,   164,   735,   164,
     842,   484,   174,   169,   174,   174,   848,   139,   172,   139,
      35,   172,   495,   279,   497,   721,  1107,   723,   164,    88,
     286,   172,   164,    92,   139,   172,   164,   164,   164,   144,
     163,   146,   147,   145,   876,   163,   151,   275,   822,   355,
      24,   996,   824,   825,   159,    48,    89,   784,   890,   891,
     892,   190,   167,   895,   939,   325,   964,   858,    61,   901,
     842,   847,   854,   366,   772,   514,  1116,  1155,  1148,   911,
     139,   913,   853,   960,   340,   144,   918,   146,   147,   921,
     863,   129,   151,   566,   530,    -1,  1177,   570,    91,   355,
     159,   933,    -1,   281,     8,   578,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,   947,    -1,    -1,    22,    -1,
      -1,   848,    26,    27,   117,    -1,    -1,    -1,   960,   901,
      -1,   124,    36,    -1,    38,    39,    -1,   969,    -1,   971,
      44,    -1,   974,    -1,    -1,   977,    -1,    51,    -1,    -1,
     279,   983,   145,   985,    -1,    -1,    -1,   286,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   762,
      -1,   164,    76,    -1,    78,    -1,   169,    -1,    82,   172,
    1057,    85,    -1,    87,    -1,    89,  1018,  1019,    -1,  1021,
    1022,    -1,  1024,  1025,   890,   891,    -1,    -1,  1030,  1031,
      -1,  1033,   458,  1035,   108,  1037,  1038,   111,   801,  1041,
     114,   340,    -1,    -1,    -1,    -1,    -1,   944,    -1,    32,
      -1,    -1,   478,    -1,    -1,  1057,   355,    -1,   484,    -1,
    1062,    -1,    -1,   960,    -1,    -1,    -1,    -1,    -1,   495,
      -1,   497,    -1,    48,   717,  1077,  1078,  1079,  1080,    -1,
      -1,    27,    -1,  1085,    -1,  1087,    61,    -1,    -1,  1091,
     164,    -1,    -1,    -1,    -1,  1097,  1098,  1099,  1100,  1101,
      -1,    -1,    48,    -1,    -1,    88,    52,    -1,    -1,    92,
      -1,    -1,    -1,  1115,    -1,   758,    91,    -1,   761,  1121,
    1122,  1123,  1124,  1125,  1126,  1127,  1128,    -1,    -1,    75,
      -1,    -1,    78,    -1,    80,    -1,    -1,  1079,  1080,    -1,
     566,    -1,   117,    89,   570,  1147,    -1,   910,    -1,   124,
      -1,    -1,   578,    -1,   917,    -1,   139,   140,    -1,   458,
    1057,   144,   108,   146,   147,  1062,    -1,   810,   151,    -1,
     145,    -1,    -1,   816,  1176,    -1,   159,   940,    -1,   478,
      -1,    -1,   165,   166,   167,   484,    -1,    -1,    -1,   164,
    1192,  1193,  1194,  1195,   169,    -1,   495,   172,   497,    -1,
      -1,  1203,  1204,    -1,   967,    -1,    -1,    -1,    -1,    -1,
      27,    -1,  1214,    -1,    -1,    -1,  1218,  1219,    -1,    -1,
      -1,    -1,    -1,    -1,  1226,  1227,  1228,  1229,  1230,  1231,
      -1,    48,    -1,    -1,    -1,    52,    -1,    -1,    -1,     2,
      -1,    -1,  1244,    -1,    32,  1142,    -1,   890,   891,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    78,    25,    80,    -1,    28,    -1,   566,   911,    -1,
     913,   570,    89,    -1,    -1,   918,    -1,    -1,   921,   578,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   930,    -1,   932,
      -1,   108,   935,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,     2,    -1,    -1,    92,    -1,    -1,  1204,    -1,    -1,
      -1,    12,    -1,    -1,    77,    -1,   959,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    -1,   969,    28,   971,    -1,
      -1,    -1,   758,    -1,    -1,   761,    -1,    -1,    -1,    -1,
      -1,   104,    -1,    -1,    -1,    -1,   109,  1244,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
      -1,    -1,    -1,   151,    -1,    -1,    -1,    -1,  1131,  1132,
      -1,   159,  1135,  1136,     3,     2,    77,   165,   166,   167,
      -1,    -1,    -1,    -1,   810,    12,    -1,  1030,    17,    18,
     816,    20,    -1,    -1,  1037,    -1,    25,    -1,    25,    -1,
    1163,    28,  1165,   104,  1167,    34,    -1,  1050,   109,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,   190,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,  1081,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,  1090,    77,    -1,
      77,  1094,    -1,    -1,    -1,    -1,    -1,    86,    -1,   758,
      -1,    -1,   761,    -1,    -1,    -1,    95,    96,    -1,    98,
      -1,   100,    -1,   102,    -1,    -1,   105,   104,    -1,    -1,
     109,   110,   109,   112,    -1,   911,   115,   913,     3,   190,
      -1,    -1,   918,    -1,  1137,   921,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    -1,   930,    20,   932,    -1,    -1,   935,
      25,   810,    -1,    -1,    -1,    -1,   279,   816,    -1,    -1,
      -1,    36,    -1,   286,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    -1,   959,    -1,    -1,    -1,    -1,    53,    -1,
      -1,    -1,   171,   969,    -1,   971,    -1,    -1,    63,  1192,
    1193,  1194,  1195,    -1,    -1,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   190,    -1,    -1,    30,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,  1218,  1219,   340,   279,    -1,
      44,    45,    46,    98,    -1,   286,    -1,   102,    -1,    -1,
      -1,    -1,   355,    -1,   109,    -1,    -1,    -1,    -1,     8,
     115,    -1,    -1,    -1,  1030,    -1,    -1,    -1,    -1,    -1,
      -1,  1037,   911,    22,   913,    -1,    -1,    26,    27,   918,
      -1,    -1,   921,    -1,  1050,    -1,    -1,    36,    -1,    38,
      39,   930,    -1,   932,    -1,    44,   935,    -1,    -1,   340,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   279,    -1,   355,  1081,    65,    -1,    -1,   286,
     959,    -1,    -1,    -1,  1090,    -1,    -1,    76,  1094,    78,
     969,    -1,   971,    82,    -1,    84,    85,    -1,    87,    -1,
      89,    -1,    -1,   147,    -1,   149,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,   158,   458,    31,    -1,    -1,   108,
      -1,    36,   111,    38,    39,   114,    -1,    -1,    -1,    44,
      -1,  1137,    -1,   340,    -1,   478,    51,    -1,    -1,    -1,
      -1,   484,    -1,    -1,    -1,    -1,    -1,    -1,   355,    -1,
      -1,  1030,   495,    -1,   497,    -1,   200,    -1,  1037,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    82,     2,    -1,
      85,  1050,    87,    -1,    89,   164,    -1,   458,    12,    -1,
      -1,    -1,   226,    -1,    -1,    -1,  1192,  1193,  1194,  1195,
      -1,    25,    -1,   108,    28,    -1,   111,   478,    -1,   114,
      -1,    -1,  1081,   484,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1090,  1218,  1219,   495,  1094,   497,   261,    -1,    -1,
      -1,    -1,    -1,   566,    -1,   269,   270,   570,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   578,    -1,    -1,   282,    -1,
      -1,    -1,     2,    77,    -1,    -1,    32,    -1,    -1,   164,
      -1,   458,    12,    -1,    -1,   170,    -1,    -1,  1137,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,    -1,
     104,   478,    -1,    -1,    -1,   109,    -1,   484,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   566,    -1,    -1,   495,   570,
     497,    -1,    -1,    -1,    -1,    -1,    -1,   578,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,     2,
      -1,    -1,    -1,  1192,  1193,  1194,  1195,    77,    -1,    12,
     364,    -1,    -1,   367,    -1,    -1,    -1,    -1,   372,    -1,
      -1,    -1,    25,    -1,    -1,    28,    -1,    -1,    -1,  1218,
    1219,    -1,    -1,    -1,   104,    -1,   390,    -1,    -1,   109,
      -1,    -1,    -1,   139,   140,    -1,   190,   143,   144,   566,
     146,   147,    -1,   570,    -1,   151,    -1,    -1,    -1,     1,
      -1,   578,    -1,   159,    -1,    -1,    -1,    32,    -1,   165,
     166,   167,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
      -1,   104,    44,   457,    -1,   758,   109,    -1,   761,    51,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     190,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    73,    -1,    -1,    76,   279,    78,    -1,    80,    -1,
      82,    83,   286,    85,    -1,    87,    -1,    89,    -1,    -1,
     504,    -1,    -1,    -1,    -1,    -1,    -1,   810,    -1,    -1,
      -1,    -1,    -1,   816,   518,    -1,   108,   758,   522,   111,
     761,    -1,   114,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,    -1,    -1,    -1,   151,   190,    -1,    -1,
      -1,    -1,    -1,   158,   159,    -1,   340,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,   279,
      -1,   355,    -1,    -1,    -1,    -1,   286,    -1,    -1,   810,
      -1,    -1,   164,    -1,    -1,   816,    -1,    -1,   170,    -1,
     584,    -1,    -1,    -1,   588,    -1,    -1,    -1,    -1,    -1,
      -1,   758,   596,    -1,   761,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   911,    -1,
     913,    -1,    -1,    -1,    -1,   918,    -1,    -1,   921,    -1,
     340,    -1,    -1,    -1,    -1,    -1,   279,   930,    -1,   932,
      -1,    -1,   935,   286,    -1,   355,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   810,    -1,    -1,    -1,    -1,    -1,   816,
      -1,    -1,    -1,    -1,    -1,    -1,   959,    -1,    -1,    -1,
      49,    -1,    -1,    -1,   458,    -1,   969,    -1,   971,    -1,
     911,    -1,   913,    -1,    -1,    -1,    -1,   918,    -1,    -1,
     921,    -1,    71,    -1,   478,    -1,    -1,   340,    -1,   930,
     484,   932,    -1,    -1,   935,    -1,    -1,    -1,    -1,    -1,
      -1,   495,   355,   497,    -1,    -1,    -1,    -1,    -1,    -1,
       2,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   959,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,  1030,   969,    -1,
     971,    -1,    -1,    25,  1037,    -1,    28,    -1,   458,    -1,
      -1,    -1,    -1,    -1,   911,    -1,   913,  1050,    -1,    -1,
      -1,   918,    -1,    -1,   921,    -1,    -1,    -1,   478,   763,
      -1,    -1,    -1,   930,   484,   932,    -1,   771,   935,    -1,
     774,    -1,   566,    -1,   778,   495,   570,   497,  1081,    -1,
      -1,    -1,    -1,    -1,   578,    77,    -1,  1090,    -1,  1030,
      -1,  1094,   959,    -1,    -1,    -1,  1037,   801,    -1,    -1,
      -1,    -1,   969,    -1,   971,   458,    -1,    -1,    -1,  1050,
      -1,    -1,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   478,    -1,    -1,    -1,    -1,
      -1,   484,    -1,    -1,  1137,    -1,    -1,    -1,    -1,    -1,
    1081,    -1,   495,   847,   497,    -1,   566,    -1,    -1,  1090,
     570,    -1,    -1,  1094,   858,    -1,    -1,    -1,   578,   863,
      -1,    -1,    -1,  1030,    -1,    -1,    -1,    -1,    -1,    -1,
    1037,    -1,    -1,    -1,   263,   264,   265,   266,   267,   268,
      -1,    -1,    -1,  1050,    -1,    -1,    71,    -1,    -1,  1192,
    1193,  1194,  1195,    -1,    -1,    -1,  1137,    -1,   190,    -1,
     289,    -1,    -1,    -1,    -1,    -1,   295,    -1,    -1,    -1,
      -1,    -1,    -1,   566,  1081,  1218,  1219,   570,    -1,    -1,
      -1,    -1,    -1,  1090,   313,   578,    -1,  1094,    22,    -1,
      -1,    -1,    26,    27,    -1,   939,    -1,    31,    -1,    -1,
     329,   330,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,  1192,  1193,  1194,  1195,    -1,    -1,    51,    -1,    -1,
     964,    -1,    -1,    -1,   758,    -1,    -1,   761,    -1,    -1,
    1137,    -1,    -1,    -1,    -1,    -1,    -1,  1218,  1219,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,   279,    82,    -1,
      -1,    85,    -1,    87,   286,    89,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,   108,    -1,   810,   111,    -1,    -1,
     114,    -1,   816,    -1,    -1,  1192,  1193,  1194,  1195,   155,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   758,    -1,
      -1,   761,   168,    -1,    -1,   171,    -1,   173,   340,    -1,
      -1,  1218,  1219,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   355,    -1,    -1,    -1,    -1,    -1,    -1,
     164,    -1,   461,    -1,    -1,    -1,   170,    -1,   263,   264,
     265,   266,   267,   268,    -1,    -1,    -1,    -1,    -1,    -1,
     810,    -1,    -1,    -1,   483,    -1,   816,    -1,   487,    -1,
     489,    -1,    -1,   492,   289,   758,    -1,    -1,   761,    -1,
     295,    32,    -1,    -1,    -1,    -1,    -1,   911,    -1,   913,
      -1,    -1,    -1,    -1,   918,    -1,    -1,   921,   313,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   930,    -1,   932,    -1,
      -1,   935,    -1,    -1,   329,   330,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   810,    -1,    -1,
      -1,    -1,    -1,   816,    -1,   959,   458,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,   969,    -1,   971,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   478,    -1,    -1,    -1,
      -1,   911,   484,   913,    -1,    -1,    -1,    -1,   918,    -1,
      -1,   921,    -1,   495,    -1,    -1,    -1,    -1,    -1,    -1,
     930,    -1,   932,    -1,    -1,   935,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,    -1,
     151,    -1,    -1,    -1,    -1,    -1,  1030,   158,   159,   959,
      -1,   162,    -1,  1037,   165,   166,   167,    -1,    -1,   969,
      -1,   971,    -1,    -1,    -1,   176,  1050,    -1,   911,    -1,
     913,    -1,    -1,    -1,    -1,   918,    -1,    -1,   921,    -1,
      -1,    -1,    -1,    -1,   566,    -1,   461,   930,   570,   932,
      -1,    -1,   935,    -1,    -1,    -1,   578,  1081,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1090,    -1,   483,    -1,
    1094,    -1,   487,    -1,   489,    -1,   959,   492,    -1,    -1,
    1030,    -1,    -1,    -1,    -1,    -1,   969,  1037,   971,    32,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,
    1050,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,  1137,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,   742,   743,    -1,    -1,    -1,    -1,   748,
     749,  1081,    -1,   752,   753,    -1,    -1,    -1,    -1,    -1,
    1090,    -1,    -1,    -1,  1094,    88,    76,  1030,    78,    92,
      80,    -1,    82,    -1,  1037,    85,    -1,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1050,  1192,  1193,
    1194,  1195,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
      -1,   111,    -1,    -1,   114,    -1,    -1,  1137,    -1,    -1,
      -1,    -1,    -1,    -1,  1218,  1219,   139,   140,  1081,   142,
     143,   144,    -1,   146,   147,   148,    -1,  1090,   151,   152,
     153,  1094,    -1,   156,   157,   158,   159,    -1,    -1,   162,
      -1,    -1,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,   164,    -1,   758,    -1,    -1,   761,
     170,    -1,  1192,  1193,  1194,  1195,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1137,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1218,  1219,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   902,   903,    -1,    -1,    -1,   810,   908,
      -1,    -1,    -1,    -1,   816,    -1,    -1,    -1,    -1,    -1,
      -1,   920,    -1,    -1,   923,    -1,   925,    -1,   927,  1192,
    1193,  1194,  1195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,   742,   743,   948,
      -1,    -1,    -1,   748,   749,  1218,  1219,   752,   753,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,   911,
      -1,   913,    69,    70,    -1,    -1,   918,    -1,    -1,   921,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    88,   930,    86,
     932,    92,    -1,   935,    -1,    -1,    -1,    -1,    95,    96,
      -1,    98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,   959,   115,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,   969,    -1,   971,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,  1083,   146,   147,   148,    -1,   150,
     151,   152,   153,    -1,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,   902,   903,    -1,
      -1,    -1,   169,   908,   171,   176,    -1,  1116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   920,    -1,    -1,   923,    -1,
     925,    -1,   927,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,  1050,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,    -1,  1081,
     171,    -1,   173,    -1,    -1,    -1,    -1,    -1,  1090,    -1,
      -1,    -1,  1094,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       0,     1,    -1,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,  1137,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,  1083,    89,
      90,    91,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,    -1,   169,   170,   171,    -1,   173,    -1,     1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,   170,   171,
      -1,   173,    -1,     1,   176,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
      -1,   169,   170,   171,    -1,   173,    -1,     1,   176,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    93,
      94,    95,    96,    97,    98,    99,   100,    -1,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,   170,   171,    -1,   173,
      -1,     1,   176,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
     170,   171,    -1,   173,    -1,     1,   176,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,   170,   171,    -1,   173,    -1,     1,
     176,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     1,   176,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    93,
      94,    95,    96,    97,    98,    99,   100,    -1,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     1,   176,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,
     176,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     1,   176,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,    77,
      78,    79,    -1,    81,    82,    83,    84,    85,    86,    87,
      -1,    89,    90,    91,    -1,    93,    94,    95,    96,    97,
      98,    99,   100,    -1,   102,   103,    -1,   105,   106,   107,
     108,   109,    -1,   111,   112,   113,   114,   115,    -1,    -1,
     118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    -1,    89,    90,    91,    -1,    93,
      94,    95,    96,    97,    98,    99,   100,    -1,   102,   103,
      -1,   105,   106,   107,   108,   109,    -1,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
     164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     1,   176,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    -1,    89,
      90,    91,    -1,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,    -1,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    -1,    53,    54,    -1,
      56,    57,    58,    59,    60,    -1,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    -1,    89,    90,    91,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
     106,   107,   108,   109,    -1,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,
     176,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    -1,    89,    90,    91,
      -1,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     1,   176,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,   174,     1,   176,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,    -1,   176,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,   106,   107,    -1,   109,   110,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,   172,   173,
      -1,    -1,   176,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,   172,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,   114,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
     106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,   106,   107,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,   114,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,   121,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,   176,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,   170,   171,    -1,   173,    -1,    -1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,   174,    -1,   176,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,   174,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,
     176,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,    -1,   176,     3,     4,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,   174,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
     110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,    -1,   176,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,
     176,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,
     176,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,
     176,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,
     176,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    12,    -1,    -1,    -1,   148,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    34,   167,    36,    -1,
      -1,   171,    40,   173,    -1,    -1,   176,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,   146,    -1,
      -1,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    -1,   162,    -1,    -1,   165,   166,   167,
     168,    -1,     3,   171,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
      -1,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,     3,   173,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    61,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    91,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,   104,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,   117,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,   163,   164,    25,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,    -1,    -1,   145,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   163,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,
     172,   173,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,
     141,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,   163,    -1,    25,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,   163,    -1,    25,    -1,    -1,    -1,   169,    -1,
     171,    -1,   173,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    17,    18,   105,    20,    -1,    -1,   109,    -1,
      25,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    22,    63,    -1,
      -1,    26,    27,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    36,    77,    38,    39,    -1,    -1,    -1,    -1,    44,
     171,    86,   173,    -1,     3,    -1,    51,    -1,    -1,    -1,
      95,    96,    -1,    98,    -1,   100,    -1,   102,    17,    18,
     105,    20,    -1,    -1,   109,    -1,    25,   112,    -1,    -1,
     115,    76,    -1,    78,    -1,    34,    -1,    82,    -1,    -1,
      85,    40,    87,    -1,    89,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    63,    -1,   111,    -1,    -1,   114,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    98,
      -1,   100,    -1,   102,    -1,    -1,   105,    -1,     3,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    12,    -1,   164,
      -1,    -1,    17,    18,    19,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,   171,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      95,    96,    -1,    98,    -1,   100,    19,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    32,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
      -1,   146,    -1,    -1,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,    -1,   162,    -1,    -1,
     165,   166,   167,   168,    -1,    88,    -1,    -1,    -1,    92,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   101,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,   125,   126,   127,   128,   129,    -1,    -1,   132,
     133,   134,   135,    -1,   137,   138,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,
      -1,    -1,    92,   176,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   101,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,    -1,   137,   138,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    88,    -1,    -1,    -1,    92,   176,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   101,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,
     127,   128,   129,    -1,    -1,   132,   133,   134,   135,    -1,
     137,   138,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   101,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,    -1,    -1,   132,   133,
     134,   135,    -1,   137,   138,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,
      -1,    92,   176,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     101,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,    -1,
      -1,   132,   133,   134,   135,    -1,   137,   138,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,
      88,    -1,    -1,    -1,    92,   176,    -1,    12,    -1,    -1,
      -1,    -1,    -1,   101,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,    -1,   137,
     138,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   101,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,
     125,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,    -1,   137,   138,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,
      92,   176,    -1,    12,    -1,    -1,    -1,    -1,    -1,   101,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,   135,    -1,   137,   138,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,
      -1,    -1,    -1,    92,   176,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   101,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,    -1,    48,   132,   133,   134,   135,    -1,   137,   138,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    -1,    88,    -1,    -1,    -1,    92,   176,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    88,    -1,    -1,    -1,    92,    -1,
     176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,   119,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,
      -1,    92,   176,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    37,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    72,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,
      88,    -1,    -1,    -1,    92,   176,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    37,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    72,   156,   157,
     158,   159,   160,   161,   162,    -1,   164,   165,   166,   167,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    92,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,
      -1,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,   119,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,
      -1,    -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    32,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      88,    -1,    -1,    -1,    92,    12,   176,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,   123,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    -1,    12,   176,    -1,
      -1,    -1,   139,   140,    19,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,    32,    33,   156,
     157,   158,   159,   160,    -1,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    88,    -1,    -1,    -1,    92,    12,   176,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,
      -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,   139,   140,    -1,   142,   143,
     144,    51,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    88,    76,    -1,    78,    92,
      -1,    12,   176,    -1,    -1,    -1,    -1,    87,    19,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,
     123,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    88,    -1,    -1,
      -1,    92,    -1,   176,    12,    -1,    -1,    -1,    -1,    -1,
     101,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,
      88,    -1,    -1,    -1,    92,   176,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,   119,    37,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    72,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    -1,    -1,    88,    -1,    -1,   174,    92,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,
      -1,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,
      -1,    -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,   165,   166,   167,    -1,    -1,
      88,    -1,    -1,    -1,    92,    12,   176,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,   164,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,   164,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,
      -1,   176,    -1,    -1,    -1,    -1,    19,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,
      12,    13,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,   172,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    35,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,   164,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,   172,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,   172,    92,    12,    13,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   122,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,
      -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,
      12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,   172,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    12,    13,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,   174,    12,   176,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      -1,    88,    -1,    -1,    -1,    92,    12,   176,    -1,    -1,
      -1,    -1,    -1,    19,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,   176,
      -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    61,
     156,   157,   158,   159,   160,   161,   162,    12,    13,   165,
     166,   167,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    88,    32,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    88,   146,   147,   148,    92,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,
      -1,   176,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    12,    -1,
     165,   166,   167,    -1,    -1,    19,    -1,    -1,    -1,   174,
      -1,   176,    -1,    -1,    -1,    -1,   139,   140,    32,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,    12,   162,
      -1,    -1,   165,   166,   167,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   101,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   123,
      -1,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    12,
      22,   165,   166,   167,    26,    27,    19,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    36,    -1,    38,    39,    -1,    32,
      -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    65,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    -1,    32,
      82,    -1,    84,    85,    -1,    87,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,   101,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   101,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   164,   156,   157,   158,   159,   160,   161,   162,
     123,    -1,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      12,    22,   165,   166,   167,    26,    27,    19,    -1,    -1,
      31,    -1,    -1,   176,    -1,    36,    -1,    38,    39,    -1,
      32,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      32,    82,    -1,    -1,    85,    -1,    87,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      92,    12,    -1,    -1,    -1,    -1,    -1,   108,    19,   101,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   101,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,   164,   156,   157,   158,   159,   160,   161,
     162,   123,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    -1,    -1,   176,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,   123,   165,   166,   167,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   176,    19,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    32,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,    12,
      -1,    -1,    -1,   174,    -1,   176,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
     123,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,     1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,     3,    -1,    -1,    40,    -1,     8,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    17,    18,    53,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    34,    69,    70,    -1,    -1,    -1,    40,    -1,
      -1,    77,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      86,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    63,    98,    -1,   100,    -1,   102,    69,    70,   105,
      -1,    -1,    -1,   109,    -1,    77,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    86,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,
     102,    17,    18,   105,    20,    -1,    -1,   109,    -1,    25,
     112,    -1,    -1,   115,    -1,    31,    -1,    -1,    34,    -1,
      -1,     3,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    17,    18,    53,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    34,    69,    70,    -1,    -1,    -1,    40,    -1,
      -1,    77,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      86,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    63,    98,    -1,   100,    -1,   102,    69,    70,   105,
      -1,    -1,    -1,   109,    -1,    77,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    86,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,
     102,    17,    18,   105,    20,    -1,    -1,   109,    -1,    25,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    34,    -1,
      -1,     3,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    17,    18,    53,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    34,    69,    70,    -1,    -1,    -1,    40,    -1,
      -1,    77,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      86,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    63,    98,    -1,   100,    -1,   102,    69,    70,   105,
      -1,    -1,    -1,   109,    -1,    77,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   181,   182,     0,     1,     3,     5,     6,     7,     8,
       9,    10,    14,    15,    16,    17,    18,    20,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    33,    34,
      36,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      49,    50,    51,    53,    56,    57,    58,    59,    60,    62,
      63,    64,    66,    67,    69,    70,    71,    76,    77,    78,
      79,    80,    81,    82,    85,    86,    87,    89,    90,    91,
      93,    94,    95,    96,    97,    98,    99,   100,   102,   103,
     105,   106,   107,   108,   109,   111,   112,   114,   115,   118,
     120,   123,   139,   140,   141,   142,   143,   148,   158,   161,
     162,   163,   164,   167,   169,   171,   173,   176,   183,   184,
     185,   186,   187,   188,   189,   190,   192,   193,   194,   200,
     201,   204,   205,   209,   211,   214,   216,   219,   220,   221,
     222,   223,   224,   225,   226,   228,   230,   233,   234,   235,
     236,   237,   238,   242,   243,   246,   247,   248,   251,   252,
     257,   258,   259,   260,   261,   263,   264,   287,   288,   291,
     292,   312,   313,   314,   315,   316,   317,   318,   326,   327,
     328,   329,   330,   333,   334,   335,   336,   337,   338,   339,
     340,   342,   343,   344,   345,   346,   164,   185,   330,   119,
     319,   320,     3,   206,    14,    22,    36,    41,    42,    45,
      56,    87,   100,   169,   173,   233,   257,   312,   317,   328,
     329,   330,   333,   335,   336,   319,   330,   108,   291,    89,
     206,   185,   306,   330,     8,    22,    36,    39,    87,   187,
     190,   185,   171,     3,    17,    18,    20,    25,    34,    40,
      46,    50,    53,    63,    69,    70,    77,    86,    95,    96,
      98,   100,   102,   105,   109,   112,   115,   208,   210,    11,
     108,    78,   121,   227,   330,   227,   330,   227,   330,    27,
     114,   229,   330,    82,    85,   190,   171,   208,   208,   208,
     171,   269,   171,   208,   293,   294,    33,   193,   213,   330,
     330,    18,    77,    95,   112,   330,   330,   330,     8,   171,
     218,   217,     4,   281,   305,   330,   106,   107,   164,   330,
     332,   330,   213,   330,   330,   330,    99,   171,   185,   330,
     330,   194,   205,   330,   333,   194,   205,   330,   330,   229,
     330,   330,   330,   330,   330,   330,   330,     1,   170,   183,
     195,   305,   110,   149,   281,   307,   308,   332,   227,   305,
     330,   341,   330,    80,   185,   169,    47,    84,   113,   191,
      26,   292,   330,     8,    87,   330,   331,    56,   144,   249,
     208,     1,    31,   208,   253,   255,   256,    54,    73,    83,
     276,    27,    78,    89,   108,   277,    27,    78,    89,   108,
     275,   208,   289,   293,   163,   164,   155,   330,    12,    19,
      32,    88,    92,   123,   139,   140,   142,   143,   144,   146,
     147,   148,   150,   151,   152,   153,   154,   156,   157,   158,
     159,   160,   161,   162,   165,   166,   167,   176,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   168,   267,   171,   173,    88,    92,   171,
     185,   164,   330,   330,   330,   208,   305,    56,   169,   194,
      48,   319,   293,   164,   145,   164,   187,   118,   209,   281,
     309,   310,   311,   332,   169,   208,   171,   213,    33,    48,
     213,   119,   213,   322,    33,    48,   213,   322,   213,   322,
      48,   213,   322,   208,   208,   101,   194,   101,   124,   194,
     267,   191,   309,   171,   171,   194,   185,    27,    48,    52,
      75,    78,    89,   108,   184,   270,   271,   272,   273,   274,
     277,   110,   171,   208,   295,   296,     1,   144,   300,    48,
     145,   185,   213,   171,   171,   213,   309,   219,   219,   145,
     164,   330,   330,   164,   169,   213,   171,   309,   164,   239,
     164,   239,   164,   213,   213,   164,   170,   170,   183,   145,
     170,   330,   145,   172,   145,   172,   174,   322,    48,   145,
     174,   322,   122,   145,   174,     8,     1,   170,   195,   202,
     203,   330,   197,   330,    65,    37,    72,   164,    87,   208,
     261,   291,   208,   305,   169,   170,     8,   256,   124,   145,
     170,    89,     1,   144,   299,    89,     1,     3,    12,    17,
      19,    20,    25,    40,    46,    53,    55,    63,    69,    70,
      86,    98,   102,   105,   109,   115,   139,   140,   141,   142,
     143,   144,   146,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   162,   165,   166,   167,   168,   171,
     207,   208,   210,   265,   266,   267,   312,   124,   290,   164,
     164,   330,   330,   330,   330,   227,   330,   227,   330,   330,
     330,   330,   330,   330,   330,     3,    20,    34,    63,   102,
     108,   209,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,    68,
     332,   332,   332,   332,   332,   309,   309,   227,   330,   227,
     330,    27,    48,    89,   114,   321,   324,   325,   330,   346,
      33,    48,    33,    48,   101,   171,    48,   174,   208,   227,
     330,   213,   164,   330,   330,   124,   172,   145,    48,   305,
      45,   330,   227,   330,   171,   213,    45,   330,   227,   330,
     213,   213,   227,   330,   213,   124,   124,   185,    35,   185,
     330,    35,   330,    65,   172,   310,   208,   231,   232,    48,
      89,   273,   145,   172,   171,   208,   300,   296,   145,   172,
      34,    50,    96,   100,   173,   212,   301,   313,   124,   297,
     330,   294,   330,   330,   172,   281,   330,     1,   244,   311,
     172,    21,   240,   170,   172,   172,   307,   172,   307,   185,
     174,   227,   330,   174,   185,   330,   174,   330,   174,   330,
     170,   170,   145,   164,    13,   147,   145,   164,    13,    37,
      72,   208,   167,   196,   199,   330,   196,   198,   208,   249,
     293,   164,   171,     1,   250,   256,   330,   254,   173,   212,
     298,   313,   104,   278,   171,   268,   330,   139,   147,   268,
     268,   301,   313,   145,   171,   173,   164,   164,   164,   164,
     164,   164,   172,   174,    48,    89,   145,   172,    17,    20,
      25,    46,    53,    63,    70,    86,    98,   109,   115,   312,
      88,    88,    45,   227,   330,    45,   227,   330,   310,   227,
     330,   171,   319,   319,   164,   281,   332,   311,   330,   172,
     330,    33,   213,    33,   213,   323,   324,   330,    33,   213,
     322,    33,   213,   322,   213,   322,   213,   322,   330,   330,
      35,   185,    35,    35,   185,   101,   194,   208,   172,   145,
     172,   208,   272,   296,   144,   304,    61,   117,   283,   172,
     296,   300,     1,   305,    68,   332,   172,   172,   170,    74,
     116,   170,   245,   172,   171,   194,   208,   241,   185,   174,
     322,   174,   322,   185,   122,   202,   209,   169,   330,   110,
     330,   199,   198,   145,   164,    13,   164,   249,   169,   164,
     310,   170,    31,    82,    85,   170,   184,   215,   216,   255,
       1,   174,   305,   283,   271,   172,     3,   102,   266,   267,
     289,   172,   174,   324,   300,   312,   312,   330,    33,    33,
     330,    33,    33,   172,   174,   174,   310,   213,   213,   213,
     101,    45,   330,    45,   330,   145,   172,   101,    45,   330,
     213,    45,   330,   213,   213,   213,   185,   185,   330,   185,
      35,   164,   164,   232,   194,   304,   172,   173,   212,   281,
     303,   313,   149,   282,   297,     3,    91,   102,   284,   285,
     286,   330,   193,   214,   280,   297,   174,    48,   174,   171,
     171,    33,   185,   305,   241,   144,   194,    45,   185,   330,
     174,    45,   185,   330,   174,   330,   196,    13,    37,    72,
      37,    72,   164,   164,   330,   330,   169,   250,   172,     8,
     216,   215,   174,   298,   313,   174,   262,   172,   268,   268,
     297,   101,    45,    45,   101,    45,    45,    45,    45,   172,
     330,   330,   330,   324,   330,   330,   330,    35,   185,   282,
     297,   304,   174,   305,   281,   330,   286,   117,   145,   124,
     150,   152,   153,   156,   157,    61,   330,   301,   313,   309,
     309,   185,   213,   172,   330,   330,   185,   330,   185,   170,
     110,   330,   199,   198,   199,   198,    13,   250,   170,   164,
     216,   298,   313,   164,   279,   280,   330,   330,   330,   330,
     330,   330,   101,   101,   101,   101,   185,   282,   297,   281,
     302,   303,   313,    48,   174,   330,   285,   286,   286,   286,
     286,   286,   286,   284,   174,   172,   172,   194,   101,   101,
     164,   164,   164,   164,   330,   170,   101,   101,   101,   101,
     101,   101,   330,   330,   330,   330,   330,   302,   303,   313,
     163,   163,   330,   330,   174,   302
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   180,   181,   182,   182,   183,   183,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   186,   186,   186,   187,   187,   188,   189,   189,
     189,   190,   190,   190,   191,   191,   192,   193,   193,   193,
     194,   195,   195,   196,   196,   196,   196,   197,   197,   197,
     197,   197,   197,   198,   198,   199,   199,   200,   200,   200,
     200,   200,   200,   200,   201,   202,   202,   202,   203,   203,
     204,   205,   205,   205,   205,   205,   205,   206,   206,   207,
     207,   207,   207,   207,   207,   208,   208,   208,   208,   208,
     208,   209,   209,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   211,   211,   211,   211,   211,   211,   211,   211,
     211,   211,   211,   211,   212,   212,   212,   212,   213,   213,
     214,   214,   215,   215,   215,   216,   216,   217,   216,   218,
     216,   219,   219,   219,   219,   219,   219,   219,   220,   220,
     220,   220,   221,   222,   222,   223,   224,   224,   224,   224,
     225,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   227,   228,   228,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   229,
     229,   230,   230,   231,   231,   232,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   234,
     234,   234,   234,   234,   234,   235,   235,   235,   236,   236,
     236,   237,   238,   238,   238,   238,   238,   238,   239,   239,
     240,   240,   240,   241,   241,   242,   243,   243,   244,   244,
     245,   245,   245,   246,   246,   247,   248,   248,   248,   249,
     249,   250,   250,   250,   251,   251,   252,   253,   253,   254,
     253,   255,   255,   255,   256,   256,   257,   258,   259,   259,
     259,   260,   262,   261,   263,   263,   263,   263,   263,   264,
     265,   265,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     267,   267,   267,   267,   267,   267,   267,   267,   267,   267,
     267,   267,   268,   268,   269,   270,   270,   271,   271,   272,
     272,   272,   272,   272,   272,   273,   273,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   275,   275,   275,   275,
     275,   275,   276,   276,   276,   277,   277,   277,   277,   277,
     277,   278,   278,   279,   279,   280,   280,   281,   282,   282,
     282,   283,   283,   283,   283,   283,   284,   284,   285,   285,
     285,   285,   285,   285,   285,   286,   286,   287,   288,   288,
     288,   289,   289,   290,   290,   290,   291,   291,   291,   291,
     291,   292,   292,   293,   293,   294,   294,   295,   295,   295,
     296,   296,   296,   297,   297,   297,   298,   298,   298,   298,
     298,   298,   298,   299,   299,   299,   299,   299,   300,   300,
     300,   300,   300,   301,   301,   301,   301,   302,   302,   302,
     303,   303,   303,   303,   303,   304,   304,   304,   304,   304,
     305,   305,   305,   305,   306,   306,   307,   307,   307,   308,
     308,   309,   309,   310,   310,   311,   311,   311,   311,   312,
     312,   313,   313,   313,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   316,   317,   318,   318,   318,   318,   318,
     318,   318,   318,   319,   319,   320,   321,   321,   322,   323,
     323,   324,   324,   324,   325,   325,   325,   325,   325,   325,
     326,   326,   327,   327,   327,   327,   327,   328,   328,   328,
     328,   328,   329,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     331,   331,   332,   332,   332,   333,   333,   333,   333,   334,
     334,   334,   334,   334,   335,   335,   335,   336,   336,   336,
     336,   336,   336,   337,   337,   337,   337,   338,   338,   339,
     339,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   341,   341,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   343,
     343,   343,   343,   343,   343,   343,   344,   344,   344,   344,
     345,   345,   345,   345,   346,   346,   346,   346,   346,   346,
     346
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     2,     2,
       3,     2,     1,     3,     2,     1,     1,     4,     3,     4,
       4,     0,     1,     1,     0,     1,     6,     2,     3,     3,
       1,     1,     2,     1,     3,     3,     5,     1,     3,     3,
       3,     5,     5,     0,     1,     1,     1,     4,     6,     8,
       8,     6,     8,     8,     4,     1,     3,     5,     1,     3,
       3,     4,     4,     4,     4,     4,     4,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       2,     3,     1,     3,     2,     1,     1,     0,     3,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     3,     5,
       5,     2,     1,     1,     1,     1,     7,     8,     3,     5,
       2,     5,     3,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     3,     4,
       5,     6,     5,     6,     3,     4,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     5,     4,     4,     3,     6,
       5,     4,     3,     6,     5,     6,     5,     8,     7,     4,
       4,     6,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     6,     4,     7,     5,     3,
       6,     2,     3,     3,     2,     2,     3,     3,     0,     2,
       2,     3,     5,     1,     3,     3,     5,     5,     0,     2,
       3,     2,     3,     5,     5,     2,     1,     1,     1,     0,
       2,     0,     2,     3,     3,     3,     3,     1,     2,     0,
       4,     1,     3,     2,     1,     3,     6,     0,     1,     1,
       1,     1,     0,     7,     4,     4,     6,     6,     4,     2,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     3,     3,     1,     3,     0,     1,     4,
       5,     4,     5,     6,     6,     0,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     1,     0,     1,     2,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     0,     2,     2,     4,     4,     1,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     1,     3,     1,     2,
       2,     2,     4,     0,     2,     2,     1,     2,     1,     1,
       1,     4,     3,     1,     3,     3,     5,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     3,     2,     4,     3,
       3,     4,     3,     0,     2,     2,     2,     1,     0,     2,
       2,     2,     1,     4,     4,     6,     3,     0,     1,     1,
       3,     4,     3,     4,     6,     0,     2,     2,     2,     2,
       1,     1,     3,     3,     1,     3,     1,     1,     1,     3,
       3,     0,     1,     1,     3,     3,     3,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     2,
       4,     4,     4,     5,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     2,     1,     1,     6,     6,     4,     9,
       9,     7,     6,     6,     4,     9,     9,     7,     4,     6,
       6,     9,     9,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     0,     1,     4,     1,     3,     4,     1,
       3,     4,     3,     3,     1,     1,     2,     1,     2,     1,
       1,     3,     1,     2,     2,     2,     2,     2,     8,     8,
       9,     9,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     3,     3,     2,     2,     2,     1,
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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYCHPL_LTYPE_IS_TRIVIAL && YYCHPL_LTYPE_IS_TRIVIAL

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

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, ParserContext* context)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
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
#line 6387 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 635 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6393 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 636 "chpl.ypp"
                                      { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6399 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 642 "chpl.ypp"
                                      { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6405 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 648 "chpl.ypp"
    {
      SizedStr ss = (yyvsp[0].sizedStr);
      (yyval.uniqueStrList) = new UniqueStrList();
      PODUniqueString u = STR(ss.allocatedData);
      delete ss.allocatedData;
      (yyval.uniqueStrList)->push_back(u);
    }
#line 6417 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 656 "chpl.ypp"
    {
      SizedStr ss = (yyvsp[0].sizedStr);
      PODUniqueString u = STR(ss.allocatedData);
      delete ss.allocatedData;
      (yyvsp[-2].uniqueStrList)->push_back(u);
    }
#line 6428 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: deprecated_decl_stmt  */
#line 665 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6434 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: include_module_stmt  */
#line 666 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6440 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: block_stmt  */
#line 667 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6446 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: use_stmt  */
#line 668 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6452 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: import_stmt  */
#line 669 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6458 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: require_stmt  */
#line 670 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6464 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: assignment_stmt  */
#line 671 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6470 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 672 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6476 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: if_stmt  */
#line 673 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6482 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 674 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6488 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 675 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6494 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: loop_stmt  */
#line 676 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6500 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: select_stmt  */
#line 677 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6506 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: defer_stmt  */
#line 678 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6512 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: try_stmt  */
#line 679 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6518 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: throw_stmt  */
#line 680 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6524 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: return_stmt  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6530 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: stmt_level_expr TSEMI  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6536 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: TATOMIC stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6542 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 684 "chpl.ypp"
                                         { (yyval.commentsAndStmt) = context->finishStmt(TODOSTMT((yyloc))); }
#line 6548 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 685 "chpl.ypp"
                                         { (yyval.commentsAndStmt) = context->finishStmt(TODOSTMT((yyloc))); }
#line 6554 "bison-chpl-lib.cpp"
    break;

  case 30: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 686 "chpl.ypp"
                                         { (yyval.commentsAndStmt) = context->finishStmt(TODOSTMT((yyloc))); }
#line 6560 "bison-chpl-lib.cpp"
    break;

  case 31: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 687 "chpl.ypp"
                                         { (yyval.commentsAndStmt) = context->finishStmt(TODOSTMT((yyloc))); }
#line 6566 "bison-chpl-lib.cpp"
    break;

  case 32: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 688 "chpl.ypp"
                                         { (yyval.commentsAndStmt) = context->finishStmt(TODOSTMT((yyloc))); }
#line 6572 "bison-chpl-lib.cpp"
    break;

  case 33: /* stmt: TLABEL ident_def stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(TODOSTMT((yyloc))); }
#line 6578 "bison-chpl-lib.cpp"
    break;

  case 34: /* stmt: TLOCAL expr do_stmt  */
#line 691 "chpl.ypp"
  {
    // Pull up any comments that appear before the TLOCAL keyword.
    auto comments = context->gatherCommentsFromList((yyvsp[0].blockOrDo).exprList, (yylsp[-2]));
    // Create the Local stmt.
    auto node = Local::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             context->consumeList((yyvsp[0].blockOrDo).exprList),
                             (yyvsp[0].blockOrDo).usesDo);
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6592 "bison-chpl-lib.cpp"
    break;

  case 35: /* stmt: TLOCAL do_stmt  */
#line 701 "chpl.ypp"
  {
    // Pull up any comments that appear before the TLOCAL keyword.
    auto comments = context->gatherCommentsFromList((yyvsp[0].blockOrDo).exprList, (yylsp[-1]));
    // Create the Local stmt.
    auto node = Local::build(BUILDER, LOC((yylsp[-1])),
                             context->consumeList((yyvsp[0].blockOrDo).exprList),
                             (yyvsp[0].blockOrDo).usesDo);
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6606 "bison-chpl-lib.cpp"
    break;

  case 36: /* stmt: TON expr do_stmt  */
#line 710 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(TODOSTMT((yyloc))); }
#line 6612 "bison-chpl-lib.cpp"
    break;

  case 37: /* stmt: TSERIAL expr do_stmt  */
#line 712 "chpl.ypp"
  {
    // Pull up any comments that appear before the TSERIAL keyword.
    auto comments = context->gatherCommentsFromList((yyvsp[0].blockOrDo).exprList, (yylsp[-2]));
    // Create the Serial stmt.
    auto node = Serial::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                              context->consumeList((yyvsp[0].blockOrDo).exprList),
                              (yyvsp[0].blockOrDo).usesDo);
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6626 "bison-chpl-lib.cpp"
    break;

  case 38: /* stmt: TSERIAL do_stmt  */
#line 722 "chpl.ypp"
  {
    // Pull up any comments that appear before the TSERIAL keyword.
    auto comments = context->gatherCommentsFromList((yyvsp[0].blockOrDo).exprList, (yylsp[-1]));
    // Create the Serial stmt.
    auto node = Serial::build(BUILDER, LOC((yylsp[-1])),
                              context->consumeList((yyvsp[0].blockOrDo).exprList),
                              (yyvsp[0].blockOrDo).usesDo);
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 6640 "bison-chpl-lib.cpp"
    break;

  case 39: /* stmt: TSYNC stmt  */
#line 731 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(TODOSTMT((yyloc))); }
#line 6646 "bison-chpl-lib.cpp"
    break;

  case 40: /* stmt: TYIELD expr TSEMI  */
#line 732 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(TODOSTMT((yyloc))); }
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 41: /* stmt: error TSEMI  */
#line 734 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
  }
#line 6660 "bison-chpl-lib.cpp"
    break;

  case 43: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 742 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6666 "bison-chpl-lib.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 744 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6672 "bison-chpl-lib.cpp"
    break;

  case 47: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 754 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));

      ModuleParts parts = {context->gatherComments(loc), (yyvsp[-3].visibilityTag), (yyvsp[-2].moduleKind), (yyvsp[0].uniqueStr)};
      (yyval.moduleParts) = parts;
      context->resetDeclState();
      context->clearComments();
    }
#line 6686 "bison-chpl-lib.cpp"
    break;

  case 48: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 767 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-1]));

      ModuleParts parts = (yyvsp[-2].moduleParts);
      ParserExprList* body = context->makeList();
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-2])),
                               parts.name, parts.visibility, parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6703 "bison-chpl-lib.cpp"
    break;

  case 49: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 780 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-2]));

      ModuleParts parts = (yyvsp[-3].moduleParts);
      ParserExprList* body = (yyvsp[-1].exprList);
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])),
                               parts.name, parts.visibility, parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6720 "bison-chpl-lib.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 793 "chpl.ypp"
    {
      ModuleParts parts = (yyvsp[-3].moduleParts);
      ParserExprList* body =
              context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])),
                               parts.name, parts.visibility, parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 6735 "bison-chpl-lib.cpp"
    break;

  case 51: /* access_control: %empty  */
#line 806 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6741 "bison-chpl-lib.cpp"
    break;

  case 52: /* access_control: TPUBLIC  */
#line 807 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6748 "bison-chpl-lib.cpp"
    break;

  case 53: /* access_control: TPRIVATE  */
#line 809 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6755 "bison-chpl-lib.cpp"
    break;

  case 54: /* opt_prototype: %empty  */
#line 814 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6761 "bison-chpl-lib.cpp"
    break;

  case 55: /* opt_prototype: TPROTOTYPE  */
#line 815 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6768 "bison-chpl-lib.cpp"
    break;

  case 56: /* include_module_stmt: TINCLUDE access_control opt_prototype TMODULE ident_def TSEMI  */
#line 821 "chpl.ypp"
   {
     (yyval.commentsAndStmt) = TODOSTMT((yyloc));
     context->visibility = Decl::DEFAULT_VISIBILITY;
   }
#line 6777 "bison-chpl-lib.cpp"
    break;

  case 57: /* block_stmt_body: TLCBR TRCBR  */
#line 838 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6785 "bison-chpl-lib.cpp"
    break;

  case 58: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 842 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6793 "bison-chpl-lib.cpp"
    break;

  case 59: /* block_stmt_body: TLCBR error TRCBR  */
#line 846 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExp
    ParserExprList* lst = context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    (yyval.exprList) = lst;
  }
#line 6804 "bison-chpl-lib.cpp"
    break;

  case 60: /* block_stmt: block_stmt_body  */
#line 856 "chpl.ypp"
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
#line 6823 "bison-chpl-lib.cpp"
    break;

  case 61: /* stmt_ls: toplevel_stmt  */
#line 874 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 62: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 875 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6835 "bison-chpl-lib.cpp"
    break;

  case 63: /* renames_ls: expr  */
#line 880 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur((yyvsp[0].expr));;
      (yyval.renameList)->push_back(cur);
    }
#line 6845 "bison-chpl-lib.cpp"
    break;

  case 64: /* renames_ls: expr TAS expr  */
#line 886 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur((yyvsp[-2].expr), (yyvsp[0].expr));
      (yyval.renameList)->push_back(cur);
    }
#line 6855 "bison-chpl-lib.cpp"
    break;

  case 65: /* renames_ls: renames_ls TCOMMA expr  */
#line 892 "chpl.ypp"
    {
      PotentialRename cur((yyvsp[0].expr));
      (yyvsp[-2].renameList)->push_back(cur);
    }
#line 6864 "bison-chpl-lib.cpp"
    break;

  case 66: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 897 "chpl.ypp"
    {
      PotentialRename cur((yyvsp[-2].expr), (yyvsp[0].expr));
      (yyvsp[-4].renameList)->push_back(cur);
    }
#line 6873 "bison-chpl-lib.cpp"
    break;

  case 67: /* use_renames_ls: expr  */
#line 907 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur((yyvsp[0].expr));
      (yyval.renameList)->push_back(cur);
    }
#line 6883 "bison-chpl-lib.cpp"
    break;

  case 68: /* use_renames_ls: expr TAS expr  */
#line 913 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur((yyvsp[-2].expr), (yyvsp[0].expr));
      (yyval.renameList)->push_back(cur);
    }
#line 6893 "bison-chpl-lib.cpp"
    break;

  case 69: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 919 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur((yyvsp[-2].expr), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
      (yyval.renameList)->push_back(cur);
    }
#line 6903 "bison-chpl-lib.cpp"
    break;

  case 70: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 925 "chpl.ypp"
    {
      PotentialRename cur((yyvsp[0].expr));
      (yyvsp[-2].renameList)->push_back(cur);
     }
#line 6912 "bison-chpl-lib.cpp"
    break;

  case 71: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 930 "chpl.ypp"
     {
       PotentialRename cur((yyvsp[-2].expr), (yyvsp[0].expr));
       (yyvsp[-4].renameList)->push_back(cur);
     }
#line 6921 "bison-chpl-lib.cpp"
    break;

  case 72: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 935 "chpl.ypp"
     {
       PotentialRename cur((yyvsp[-2].expr), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
       (yyvsp[-4].renameList)->push_back(cur);
     }
#line 6930 "bison-chpl-lib.cpp"
    break;

  case 73: /* opt_only_ls: %empty  */
#line 944 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur(context->buildEmptyIdent((yyloc)));
      (yyval.renameList)->push_back(cur);
    }
#line 6940 "bison-chpl-lib.cpp"
    break;

  case 75: /* except_ls: TSTAR  */
#line 954 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur(context->buildEmptyIdent((yylsp[0])));
      (yyval.renameList)->push_back(cur);
     }
#line 6950 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_stmt: access_control TUSE use_renames_ls TSEMI  */
#line 964 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 6959 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_stmt: access_control TUSE expr TEXCEPT except_ls TSEMI  */
#line 969 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 6968 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_stmt: access_control TUSE expr TAS expr TEXCEPT except_ls TSEMI  */
#line 974 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 6977 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TEXCEPT except_ls TSEMI  */
#line 979 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 6986 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 984 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 6995 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 989 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7004 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 994 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7013 "bison-chpl-lib.cpp"
    break;

  case 84: /* import_stmt: access_control TIMPORT import_ls TSEMI  */
#line 1002 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7022 "bison-chpl-lib.cpp"
    break;

  case 85: /* import_expr: expr  */
#line 1010 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7028 "bison-chpl-lib.cpp"
    break;

  case 86: /* import_expr: expr TAS ident_use  */
#line 1012 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7034 "bison-chpl-lib.cpp"
    break;

  case 87: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1014 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7040 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_ls: import_expr  */
#line 1019 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7046 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_ls: import_ls TCOMMA import_expr  */
#line 1021 "chpl.ypp"
    { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7052 "bison-chpl-lib.cpp"
    break;

  case 90: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1026 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7060 "bison-chpl-lib.cpp"
    break;

  case 91: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1033 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7068 "bison-chpl-lib.cpp"
    break;

  case 92: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1037 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7076 "bison-chpl-lib.cpp"
    break;

  case 93: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1041 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7084 "bison-chpl-lib.cpp"
    break;

  case 94: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1045 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7092 "bison-chpl-lib.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1049 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7100 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1053 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7108 "bison-chpl-lib.cpp"
    break;

  case 97: /* opt_label_ident: %empty  */
#line 1061 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7114 "bison-chpl-lib.cpp"
    break;

  case 98: /* opt_label_ident: TIDENT  */
#line 1062 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7120 "bison-chpl-lib.cpp"
    break;

  case 99: /* ident_fn_def: TIDENT  */
#line 1066 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7126 "bison-chpl-lib.cpp"
    break;

  case 100: /* ident_fn_def: TNONE  */
#line 1067 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7132 "bison-chpl-lib.cpp"
    break;

  case 101: /* ident_fn_def: TTHIS  */
#line 1068 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7138 "bison-chpl-lib.cpp"
    break;

  case 102: /* ident_fn_def: TFALSE  */
#line 1069 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7144 "bison-chpl-lib.cpp"
    break;

  case 103: /* ident_fn_def: TTRUE  */
#line 1070 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7150 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: internal_type_ident_def  */
#line 1071 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7156 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_def: TIDENT  */
#line 1074 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7162 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_def: TNONE  */
#line 1075 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7168 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_def: TTHIS  */
#line 1076 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'this'"); }
#line 7174 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_def: TFALSE  */
#line 1077 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7180 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_def: TTRUE  */
#line 1078 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7186 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: internal_type_ident_def  */
#line 1079 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7192 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_use: TIDENT  */
#line 1091 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7198 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_use: TTHIS  */
#line 1092 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7204 "bison-chpl-lib.cpp"
    break;

  case 132: /* scalar_type: TBOOL  */
#line 1125 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7210 "bison-chpl-lib.cpp"
    break;

  case 133: /* scalar_type: TENUM  */
#line 1126 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7216 "bison-chpl-lib.cpp"
    break;

  case 134: /* scalar_type: TINT  */
#line 1127 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7222 "bison-chpl-lib.cpp"
    break;

  case 135: /* scalar_type: TUINT  */
#line 1128 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7228 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TREAL  */
#line 1129 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7234 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TIMAG  */
#line 1130 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7240 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TCOMPLEX  */
#line 1131 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7246 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TBYTES  */
#line 1132 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7252 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TSTRING  */
#line 1133 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7258 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TLOCALE  */
#line 1134 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7264 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TNOTHING  */
#line 1135 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7270 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TVOID  */
#line 1136 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7276 "bison-chpl-lib.cpp"
    break;

  case 148: /* do_stmt: TDO stmt  */
#line 1150 "chpl.ypp"
                      { (yyval.blockOrDo) = { context->makeList((yyvsp[0].commentsAndStmt)), true }; }
#line 7282 "bison-chpl-lib.cpp"
    break;

  case 149: /* do_stmt: block_stmt_body  */
#line 1151 "chpl.ypp"
                      { (yyval.blockOrDo) = { (yyvsp[0].exprList), false }; }
#line 7288 "bison-chpl-lib.cpp"
    break;

  case 150: /* return_stmt: TRETURN TSEMI  */
#line 1156 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7296 "bison-chpl-lib.cpp"
    break;

  case 151: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1160 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7304 "bison-chpl-lib.cpp"
    break;

  case 153: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 1168 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7310 "bison-chpl-lib.cpp"
    break;

  case 154: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 1170 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7316 "bison-chpl-lib.cpp"
    break;

  case 155: /* class_level_stmt: TSEMI  */
#line 1175 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), nullptr);
    }
#line 7324 "bison-chpl-lib.cpp"
    break;

  case 156: /* class_level_stmt: inner_class_level_stmt  */
#line 1179 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7334 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@1: %empty  */
#line 1184 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7341 "bison-chpl-lib.cpp"
    break;

  case 158: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1186 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7350 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@2: %empty  */
#line 1190 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7357 "bison-chpl-lib.cpp"
    break;

  case 160: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1192 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7366 "bison-chpl-lib.cpp"
    break;

  case 168: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1210 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-2].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr
      (yyval.commentsAndStmt) = cs;
    }
#line 7377 "bison-chpl-lib.cpp"
    break;

  case 169: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT except_ls TSEMI  */
#line 1217 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr and except_ls
      (yyval.commentsAndStmt) = cs;
    }
#line 7388 "bison-chpl-lib.cpp"
    break;

  case 170: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1224 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr and opt_only_ls
      (yyval.commentsAndStmt) = cs;
    }
#line 7399 "bison-chpl-lib.cpp"
    break;

  case 171: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1231 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-1].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save var_decl_stmt
      (yyval.commentsAndStmt) = cs;
    }
#line 7410 "bison-chpl-lib.cpp"
    break;

  case 172: /* forwarding_decl_start: TFORWARDING  */
#line 1241 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7418 "bison-chpl-lib.cpp"
    break;

  case 173: /* extern_or_export: TEXTERN  */
#line 1247 "chpl.ypp"
          { (yyval.linkageTag) = LinkageTag_EXTERN; }
#line 7424 "bison-chpl-lib.cpp"
    break;

  case 174: /* extern_or_export: TEXPORT  */
#line 1248 "chpl.ypp"
          { (yyval.linkageTag) = LinkageTag_EXPORT; }
#line 7430 "bison-chpl-lib.cpp"
    break;

  case 175: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1253 "chpl.ypp"
    {
      CommentsAndLinkage cl = {context->gatherComments((yylsp[0])), (yyvsp[0].linkageTag)};
      (yyval.commentsAndLinkage) = cl;
    }
#line 7439 "bison-chpl-lib.cpp"
    break;

  case 176: /* extern_export_decl_stmt: extern_export_decl_stmt_start TRECORD ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1261 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-6].commentsAndLinkage);
      //auto comments = context->gatherComments(@7);
      // TODO: store comments into the generated list
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7451 "bison-chpl-lib.cpp"
    break;

  case 177: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL TRECORD ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1269 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-7].commentsAndLinkage);
      //auto comments = context->gatherComments(@8);
      // TODO: store comments into the generated list
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7463 "bison-chpl-lib.cpp"
    break;

  case 178: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1277 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-2].commentsAndLinkage);
      Expression* optExpr = (yyvsp[-1].expr);
      FunctionParts fp = (yyvsp[0].functionParts);
      LinkageTag tag = cl.linkage;
      if (tag == LinkageTag_EXTERN)
        fp.linkage = Function::EXTERN;
      else if (tag == LinkageTag_EXPORT)
        fp.linkage = Function::EXPORT;
      else
        assert(false && "extern_export returned invalid linkage");

      if (fp.comments)
        delete fp.comments;
      fp.comments = cl.comments;
      fp.linkageNameExpr = optExpr;

      (yyval.commentsAndStmt) = context->buildFunctionDecl((yyloc), fp);
    }
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 179: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1297 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-4].commentsAndLinkage);
      context->clearComments();
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7498 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1307 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7506 "bison-chpl-lib.cpp"
    break;

  case 181: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1314 "chpl.ypp"
  {
    const bool usesImplicitBlock = !((yyvsp[-3].commentsAndStmt).stmt->isBlock());
    auto exprLst = context->makeList((yyvsp[-3].commentsAndStmt));
    auto comments = context->gatherCommentsFromList(exprLst, (yylsp[-4]));

    // Append any comment appearing before 'while' into the exprs of $2.
    exprLst = context->appendList(exprLst, context->gatherComments((yylsp[-2])));

    auto astLst = context->consumeList(exprLst);
    auto statements = BUILDER->flattenTopLevelBlocks(std::move(astLst));
    auto node = DoWhile::build(BUILDER, LOC((yylsp[-4])), std::move(statements),
                               toOwned((yyvsp[-1].expr)),
                               usesImplicitBlock);
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7526 "bison-chpl-lib.cpp"
    break;

  case 182: /* loop_stmt: TWHILE expr do_stmt  */
#line 1330 "chpl.ypp"
  {
    auto comments = context->gatherCommentsFromList((yyvsp[0].blockOrDo).exprList, (yylsp[-2]));
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             context->consumeList((yyvsp[0].blockOrDo).exprList),
                             (yyvsp[0].blockOrDo).usesDo);
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7538 "bison-chpl-lib.cpp"
    break;

  case 183: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1338 "chpl.ypp"
  {
    auto comments = context->gatherCommentsFromList((yyvsp[0].blockOrDo).exprList, (yylsp[-2]));
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             context->consumeList((yyvsp[0].blockOrDo).exprList),
                             (yyvsp[0].blockOrDo).usesDo);
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7550 "bison-chpl-lib.cpp"
    break;

  case 184: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1346 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 185: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1350 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7566 "bison-chpl-lib.cpp"
    break;

  case 186: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1354 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7574 "bison-chpl-lib.cpp"
    break;

  case 187: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1358 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7582 "bison-chpl-lib.cpp"
    break;

  case 188: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1362 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7590 "bison-chpl-lib.cpp"
    break;

  case 189: /* loop_stmt: TFOR expr do_stmt  */
#line 1366 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7598 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1370 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7606 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1374 "chpl.ypp"
  {
    Expression* ident = context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr));
    auto index = context->buildLoopIndexDecl((yylsp[-3]), toOwned(ident));
    auto comments = context->gatherCommentsFromList((yyvsp[0].blockOrDo).exprList, (yylsp[-5]));
    auto node = For::build(BUILDER, LOC((yylsp[-5])), std::move(index),
                           toOwned((yyvsp[-1].expr)),
                           context->consumeList((yyvsp[0].blockOrDo).exprList),
                           (yyvsp[0].blockOrDo).usesDo,
                           /*isExpressionLevel*/ false,
                           /*isParam*/ true);
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7623 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1387 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7631 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1391 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7639 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1395 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7647 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1399 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7655 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFORALL expr do_stmt  */
#line 1403 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7663 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1407 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7671 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1411 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7679 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1415 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7687 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1419 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7695 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1423 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7703 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1427 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7711 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1431 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7719 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1435 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7727 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1439 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7735 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1443 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7743 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1447 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7751 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1451 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7759 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1455 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7767 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1459 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7775 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1463 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7783 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1467 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7791 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1471 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7799 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1475 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7808 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1480 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7817 "bison-chpl-lib.cpp"
    break;

  case 216: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1488 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7827 "bison-chpl-lib.cpp"
    break;

  case 217: /* if_stmt: TIF expr TTHEN stmt  */
#line 1497 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7833 "bison-chpl-lib.cpp"
    break;

  case 218: /* if_stmt: TIF expr block_stmt  */
#line 1499 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7839 "bison-chpl-lib.cpp"
    break;

  case 219: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1501 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7845 "bison-chpl-lib.cpp"
    break;

  case 220: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1503 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7851 "bison-chpl-lib.cpp"
    break;

  case 221: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1505 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7857 "bison-chpl-lib.cpp"
    break;

  case 222: /* if_stmt: TIF ifvar block_stmt  */
#line 1507 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7863 "bison-chpl-lib.cpp"
    break;

  case 223: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1509 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7869 "bison-chpl-lib.cpp"
    break;

  case 224: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1511 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7875 "bison-chpl-lib.cpp"
    break;

  case 225: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1514 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7881 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1516 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7887 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1518 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7893 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1520 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7899 "bison-chpl-lib.cpp"
    break;

  case 229: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1524 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7905 "bison-chpl-lib.cpp"
    break;

  case 230: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1525 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7911 "bison-chpl-lib.cpp"
    break;

  case 231: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1530 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7917 "bison-chpl-lib.cpp"
    break;

  case 232: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1532 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7923 "bison-chpl-lib.cpp"
    break;

  case 233: /* ifc_formal_ls: ifc_formal  */
#line 1536 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7929 "bison-chpl-lib.cpp"
    break;

  case 234: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1537 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7935 "bison-chpl-lib.cpp"
    break;

  case 235: /* ifc_formal: ident_def  */
#line 1542 "chpl.ypp"
             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7941 "bison-chpl-lib.cpp"
    break;

  case 248: /* implements_type_ident: implements_type_error_ident  */
#line 1560 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    noteError((yyloc), context, s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 7953 "bison-chpl-lib.cpp"
    break;

  case 255: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1589 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7959 "bison-chpl-lib.cpp"
    break;

  case 256: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1591 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7965 "bison-chpl-lib.cpp"
    break;

  case 257: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1593 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7971 "bison-chpl-lib.cpp"
    break;

  case 258: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1598 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7977 "bison-chpl-lib.cpp"
    break;

  case 259: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1600 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7983 "bison-chpl-lib.cpp"
    break;

  case 260: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1602 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7989 "bison-chpl-lib.cpp"
    break;

  case 261: /* defer_stmt: TDEFER stmt  */
#line 1607 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7995 "bison-chpl-lib.cpp"
    break;

  case 262: /* try_stmt: TTRY expr TSEMI  */
#line 1611 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8001 "bison-chpl-lib.cpp"
    break;

  case 263: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1613 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8007 "bison-chpl-lib.cpp"
    break;

  case 264: /* try_stmt: TTRY assignment_stmt  */
#line 1615 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8013 "bison-chpl-lib.cpp"
    break;

  case 265: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1617 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8019 "bison-chpl-lib.cpp"
    break;

  case 266: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1619 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8025 "bison-chpl-lib.cpp"
    break;

  case 267: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1621 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8031 "bison-chpl-lib.cpp"
    break;

  case 268: /* catch_expr_ls: %empty  */
#line 1625 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 269: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1626 "chpl.ypp"
                           { context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8043 "bison-chpl-lib.cpp"
    break;

  case 270: /* catch_expr: TCATCH block_stmt  */
#line 1630 "chpl.ypp"
                                             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8049 "bison-chpl-lib.cpp"
    break;

  case 271: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1631 "chpl.ypp"
                                             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 272: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1632 "chpl.ypp"
                                             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8061 "bison-chpl-lib.cpp"
    break;

  case 273: /* catch_expr_inner: ident_def  */
#line 1636 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8067 "bison-chpl-lib.cpp"
    break;

  case 274: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1637 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8073 "bison-chpl-lib.cpp"
    break;

  case 275: /* throw_stmt: TTHROW expr TSEMI  */
#line 1642 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8079 "bison-chpl-lib.cpp"
    break;

  case 276: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1646 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8085 "bison-chpl-lib.cpp"
    break;

  case 277: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1648 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8091 "bison-chpl-lib.cpp"
    break;

  case 278: /* when_stmt_ls: %empty  */
#line 1652 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8097 "bison-chpl-lib.cpp"
    break;

  case 279: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1653 "chpl.ypp"
                          { context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8103 "bison-chpl-lib.cpp"
    break;

  case 280: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1658 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8109 "bison-chpl-lib.cpp"
    break;

  case 281: /* when_stmt: TOTHERWISE stmt  */
#line 1660 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8115 "bison-chpl-lib.cpp"
    break;

  case 282: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1662 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8121 "bison-chpl-lib.cpp"
    break;

  case 283: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1669 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      cs.stmt = TODOEXPR((yyloc));
      context->appendComments(&cs, context->gatherComments((yylsp[0])));
      (yyval.commentsAndStmt) = cs;
    }
#line 8132 "bison-chpl-lib.cpp"
    break;

  case 284: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1676 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      cs.stmt = TODOEXPR((yyloc));
      (yyval.commentsAndStmt) = cs;
    }
#line 8142 "bison-chpl-lib.cpp"
    break;

  case 285: /* class_start: class_tag ident_def  */
#line 1686 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 8150 "bison-chpl-lib.cpp"
    break;

  case 286: /* class_tag: TCLASS  */
#line 1692 "chpl.ypp"
           { (yyval.aggregateTag) = AggregateTag_CLASS; }
#line 8156 "bison-chpl-lib.cpp"
    break;

  case 287: /* class_tag: TRECORD  */
#line 1693 "chpl.ypp"
           { (yyval.aggregateTag) = AggregateTag_RECORD; }
#line 8162 "bison-chpl-lib.cpp"
    break;

  case 288: /* class_tag: TUNION  */
#line 1694 "chpl.ypp"
           { (yyval.aggregateTag) = AggregateTag_UNION; }
#line 8168 "bison-chpl-lib.cpp"
    break;

  case 289: /* opt_inherit: %empty  */
#line 1698 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8174 "bison-chpl-lib.cpp"
    break;

  case 290: /* opt_inherit: TCOLON expr_ls  */
#line 1699 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8180 "bison-chpl-lib.cpp"
    break;

  case 291: /* class_level_stmt_ls: %empty  */
#line 1704 "chpl.ypp"
    { (yyval.exprList) = context->makeList(); }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 292: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 1706 "chpl.ypp"
    { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8192 "bison-chpl-lib.cpp"
    break;

  case 293: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 1708 "chpl.ypp"
    { context->appendList((yyvsp[-2].exprList), TODOEXPR((yyloc))); }
#line 8198 "bison-chpl-lib.cpp"
    break;

  case 294: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 1713 "chpl.ypp"
    {
      EnumParts parts = (yyvsp[-2].enumParts);
      ParserExprList* list = (yyvsp[-1].exprList);
      // get any comments after the last element but before the closing brace
      context->appendList(list, context->gatherComments((yylsp[0])));

      auto decl = Enum::build(BUILDER, LOC((yyloc)),
                              parts.name, parts.visibility,
                              context->consumeList(list));
      CommentsAndStmt cs = {parts.comments, decl.release()};
      (yyval.commentsAndStmt) = cs;
      context->resetDeclState();
      context->clearComments();
    }
#line 8217 "bison-chpl-lib.cpp"
    break;

  case 295: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 1728 "chpl.ypp"
    {
      EnumParts parts = (yyvsp[-2].enumParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->resetDeclState();
      context->clearComments();
    }
#line 8230 "bison-chpl-lib.cpp"
    break;

  case 296: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 1740 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-2]));

      EnumParts parts = {context->gatherComments(loc), context->visibility, (yyvsp[-1].uniqueStr)};
      (yyval.enumParts) = parts;

      context->clearComments();
    }
#line 8244 "bison-chpl-lib.cpp"
    break;

  case 297: /* enum_ls: deprecated_enum_item  */
#line 1753 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 8252 "bison-chpl-lib.cpp"
    break;

  case 298: /* enum_ls: enum_ls TCOMMA  */
#line 1757 "chpl.ypp"
    {
      (yyval.exprList) = (yyvsp[-1].exprList);
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8261 "bison-chpl-lib.cpp"
    break;

  case 299: /* $@3: %empty  */
#line 1761 "chpl.ypp"
                 {context->clearCommentsBefore((yylsp[0]));}
#line 8267 "bison-chpl-lib.cpp"
    break;

  case 300: /* enum_ls: enum_ls TCOMMA $@3 deprecated_enum_item  */
#line 1762 "chpl.ypp"
    {
      context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 8275 "bison-chpl-lib.cpp"
    break;

  case 302: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 1770 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8281 "bison-chpl-lib.cpp"
    break;

  case 303: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 1772 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8287 "bison-chpl-lib.cpp"
    break;

  case 304: /* enum_item: ident_def  */
#line 1777 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)), (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8296 "bison-chpl-lib.cpp"
    break;

  case 305: /* enum_item: ident_def TASSIGN expr  */
#line 1782 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8306 "bison-chpl-lib.cpp"
    break;

  case 306: /* lambda_decl_expr: TLAMBDA req_formal_ls opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 1791 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 8314 "bison-chpl-lib.cpp"
    break;

  case 308: /* linkage_spec: linkage_spec_empty  */
#line 1800 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8320 "bison-chpl-lib.cpp"
    break;

  case 309: /* linkage_spec: TINLINE  */
#line 1801 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8327 "bison-chpl-lib.cpp"
    break;

  case 310: /* linkage_spec: TOVERRIDE  */
#line 1803 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8334 "bison-chpl-lib.cpp"
    break;

  case 311: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 1809 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8342 "bison-chpl-lib.cpp"
    break;

  case 312: /* $@4: %empty  */
#line 1818 "chpl.ypp"
    { context->clearComments(); }
#line 8348 "bison-chpl-lib.cpp"
    break;

  case 313: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@4 opt_function_body_stmt  */
#line 1820 "chpl.ypp"
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

      (yyval.functionParts) = fp;
    }
#line 8368 "bison-chpl-lib.cpp"
    break;

  case 314: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 1839 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      // TODO -- do something with $2
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8380 "bison-chpl-lib.cpp"
    break;

  case 315: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 1847 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      // TODO -- do something with $2
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8392 "bison-chpl-lib.cpp"
    break;

  case 316: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 1855 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])),
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8406 "bison-chpl-lib.cpp"
    break;

  case 317: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 1865 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])),
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8420 "bison-chpl-lib.cpp"
    break;

  case 318: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 1875 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8430 "bison-chpl-lib.cpp"
    break;

  case 319: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 1884 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-1].functionParts);
      auto loc = context->declStartLoc((yylsp[0]));
      fp.comments = context->gatherComments(loc);
      fp.kind = (yyvsp[0].functionKind);
      (yyval.functionParts) = fp;
    }
#line 8442 "bison-chpl-lib.cpp"
    break;

  case 321: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 1895 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8448 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_ident: ident_def TBANG  */
#line 1927 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8458 "bison-chpl-lib.cpp"
    break;

  case 362: /* opt_formal_ls: %empty  */
#line 1950 "chpl.ypp"
                     { (yyval.exprList) = nullptr; /* TODO: mark as no-parens */ }
#line 8464 "bison-chpl-lib.cpp"
    break;

  case 363: /* opt_formal_ls: TLP formal_ls TRP  */
#line 1951 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8470 "bison-chpl-lib.cpp"
    break;

  case 364: /* req_formal_ls: TLP formal_ls TRP  */
#line 1955 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8476 "bison-chpl-lib.cpp"
    break;

  case 365: /* formal_ls_inner: formal  */
#line 1959 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8482 "bison-chpl-lib.cpp"
    break;

  case 366: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 1960 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8488 "bison-chpl-lib.cpp"
    break;

  case 367: /* formal_ls: %empty  */
#line 1964 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8494 "bison-chpl-lib.cpp"
    break;

  case 368: /* formal_ls: formal_ls_inner  */
#line 1965 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8500 "bison-chpl-lib.cpp"
    break;

  case 369: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1970 "chpl.ypp"
    { (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), (yyvsp[-3].intentTag),
                         toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr))).release(); }
#line 8507 "bison-chpl-lib.cpp"
    break;

  case 370: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 1973 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8513 "bison-chpl-lib.cpp"
    break;

  case 371: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1975 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8519 "bison-chpl-lib.cpp"
    break;

  case 372: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 1977 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8525 "bison-chpl-lib.cpp"
    break;

  case 373: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 1979 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8531 "bison-chpl-lib.cpp"
    break;

  case 374: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 1981 "chpl.ypp"
    {
      (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
    }
#line 8539 "bison-chpl-lib.cpp"
    break;

  case 375: /* opt_intent_tag: %empty  */
#line 1987 "chpl.ypp"
                      { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8545 "bison-chpl-lib.cpp"
    break;

  case 376: /* opt_intent_tag: required_intent_tag  */
#line 1988 "chpl.ypp"
                      { (yyval.intentTag) = (yyvsp[0].intentTag); }
#line 8551 "bison-chpl-lib.cpp"
    break;

  case 377: /* required_intent_tag: TIN  */
#line 1992 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8557 "bison-chpl-lib.cpp"
    break;

  case 378: /* required_intent_tag: TINOUT  */
#line 1993 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8563 "bison-chpl-lib.cpp"
    break;

  case 379: /* required_intent_tag: TOUT  */
#line 1994 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8569 "bison-chpl-lib.cpp"
    break;

  case 380: /* required_intent_tag: TCONST TIN  */
#line 1995 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8575 "bison-chpl-lib.cpp"
    break;

  case 381: /* required_intent_tag: TCONST TREF  */
#line 1996 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8581 "bison-chpl-lib.cpp"
    break;

  case 382: /* required_intent_tag: TCONST  */
#line 1997 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8587 "bison-chpl-lib.cpp"
    break;

  case 383: /* required_intent_tag: TPARAM  */
#line 1998 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8593 "bison-chpl-lib.cpp"
    break;

  case 384: /* required_intent_tag: TREF  */
#line 1999 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8599 "bison-chpl-lib.cpp"
    break;

  case 385: /* required_intent_tag: TTYPE  */
#line 2000 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8605 "bison-chpl-lib.cpp"
    break;

  case 386: /* opt_this_intent_tag: %empty  */
#line 2004 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8611 "bison-chpl-lib.cpp"
    break;

  case 387: /* opt_this_intent_tag: TPARAM  */
#line 2005 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8617 "bison-chpl-lib.cpp"
    break;

  case 388: /* opt_this_intent_tag: TREF  */
#line 2006 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8623 "bison-chpl-lib.cpp"
    break;

  case 389: /* opt_this_intent_tag: TCONST TREF  */
#line 2007 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8629 "bison-chpl-lib.cpp"
    break;

  case 390: /* opt_this_intent_tag: TCONST  */
#line 2008 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8635 "bison-chpl-lib.cpp"
    break;

  case 391: /* opt_this_intent_tag: TTYPE  */
#line 2009 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8641 "bison-chpl-lib.cpp"
    break;

  case 392: /* proc_iter_or_op: TPROC  */
#line 2013 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8647 "bison-chpl-lib.cpp"
    break;

  case 393: /* proc_iter_or_op: TITER  */
#line 2014 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8653 "bison-chpl-lib.cpp"
    break;

  case 394: /* proc_iter_or_op: TOPERATOR  */
#line 2015 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8659 "bison-chpl-lib.cpp"
    break;

  case 395: /* opt_ret_tag: %empty  */
#line 2019 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8665 "bison-chpl-lib.cpp"
    break;

  case 396: /* opt_ret_tag: TCONST  */
#line 2020 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 8671 "bison-chpl-lib.cpp"
    break;

  case 397: /* opt_ret_tag: TCONST TREF  */
#line 2021 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 8677 "bison-chpl-lib.cpp"
    break;

  case 398: /* opt_ret_tag: TREF  */
#line 2022 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 8683 "bison-chpl-lib.cpp"
    break;

  case 399: /* opt_ret_tag: TPARAM  */
#line 2023 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 8689 "bison-chpl-lib.cpp"
    break;

  case 400: /* opt_ret_tag: TTYPE  */
#line 2024 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 8695 "bison-chpl-lib.cpp"
    break;

  case 401: /* opt_throws_error: %empty  */
#line 2028 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8701 "bison-chpl-lib.cpp"
    break;

  case 402: /* opt_throws_error: TTHROWS  */
#line 2029 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8707 "bison-chpl-lib.cpp"
    break;

  case 403: /* opt_function_body_stmt: TSEMI  */
#line 2032 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8713 "bison-chpl-lib.cpp"
    break;

  case 404: /* opt_function_body_stmt: function_body_stmt  */
#line 2033 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8719 "bison-chpl-lib.cpp"
    break;

  case 405: /* function_body_stmt: block_stmt_body  */
#line 2037 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8725 "bison-chpl-lib.cpp"
    break;

  case 406: /* function_body_stmt: return_stmt  */
#line 2038 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 8731 "bison-chpl-lib.cpp"
    break;

  case 407: /* query_expr: TQUERIEDIDENT  */
#line 2042 "chpl.ypp"
                      { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8737 "bison-chpl-lib.cpp"
    break;

  case 408: /* var_arg_expr: TDOTDOTDOT  */
#line 2046 "chpl.ypp"
                         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8743 "bison-chpl-lib.cpp"
    break;

  case 409: /* var_arg_expr: TDOTDOTDOT expr  */
#line 2047 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8749 "bison-chpl-lib.cpp"
    break;

  case 410: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 2048 "chpl.ypp"
                         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8755 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_lifetime_where: %empty  */
#line 2052 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8761 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_lifetime_where: TWHERE expr  */
#line 2054 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8767 "bison-chpl-lib.cpp"
    break;

  case 413: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2056 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8773 "bison-chpl-lib.cpp"
    break;

  case 414: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2058 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8779 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2060 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8785 "bison-chpl-lib.cpp"
    break;

  case 416: /* lifetime_components_expr: lifetime_expr  */
#line 2065 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8791 "bison-chpl-lib.cpp"
    break;

  case 417: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2067 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8797 "bison-chpl-lib.cpp"
    break;

  case 418: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2072 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8803 "bison-chpl-lib.cpp"
    break;

  case 419: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2074 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8809 "bison-chpl-lib.cpp"
    break;

  case 420: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2076 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8815 "bison-chpl-lib.cpp"
    break;

  case 421: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2078 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8821 "bison-chpl-lib.cpp"
    break;

  case 422: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2080 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8827 "bison-chpl-lib.cpp"
    break;

  case 423: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2082 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8833 "bison-chpl-lib.cpp"
    break;

  case 424: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2084 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8839 "bison-chpl-lib.cpp"
    break;

  case 425: /* lifetime_ident: TIDENT  */
#line 2088 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8845 "bison-chpl-lib.cpp"
    break;

  case 426: /* lifetime_ident: TTHIS  */
#line 2089 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8851 "bison-chpl-lib.cpp"
    break;

  case 427: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner TSEMI  */
#line 2094 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[-2].commentsAndStmt)); // TODO
    }
#line 8859 "bison-chpl-lib.cpp"
    break;

  case 428: /* type_alias_decl_stmt_start: TTYPE  */
#line 2101 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 8867 "bison-chpl-lib.cpp"
    break;

  case 429: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2105 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 8875 "bison-chpl-lib.cpp"
    break;

  case 430: /* type_alias_decl_stmt_start: TEXTERN TTYPE  */
#line 2110 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 8883 "bison-chpl-lib.cpp"
    break;

  case 431: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2118 "chpl.ypp"
    {
      // set name to $1
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 432: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 2123 "chpl.ypp"
    {
      // set name to $1
      // add new var to multivar
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 8902 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_init_type: %empty  */
#line 2131 "chpl.ypp"
    { (yyval.expr) = nullptr; }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_init_type: TASSIGN type_level_expr  */
#line 2133 "chpl.ypp"
    { (yyval.expr) = (yyvsp[0].expr); }
#line 8914 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_init_type: TASSIGN array_type  */
#line 2135 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 436: /* var_decl_type: TPARAM  */
#line 2139 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 437: /* var_decl_type: TCONST TREF  */
#line 2140 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 438: /* var_decl_type: TREF  */
#line 2141 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 439: /* var_decl_type: TCONST  */
#line 2142 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 440: /* var_decl_type: TVAR  */
#line 2143 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 8950 "bison-chpl-lib.cpp"
    break;

  case 441: /* var_decl_stmt: TCONFIG var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2148 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->resetDeclState();
    }
#line 8959 "bison-chpl-lib.cpp"
    break;

  case 442: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2153 "chpl.ypp"
    {
      ParserExprList* lst = (yyvsp[-1].exprList);
      int nVarDecls = 0;
      Variable* lastVariableDecl = nullptr;
      for (auto elt : *lst) {
        if (Variable* d = elt->toVariable()) {
          lastVariableDecl = d;
          nVarDecls++;
        }
      }
      assert(nVarDecls > 0);
      assert(lastVariableDecl);
      if (nVarDecls == 1) {
        // gather any comments from before the 'var'
        // intentionally ignores comments after 'var'
        CommentsAndStmt cs = {context->gatherCommentsFromList(lst, (yylsp[-2])),
                              lastVariableDecl};
        (yyval.commentsAndStmt) = cs;
        // delete any comments left in lst
        for (auto elt : *lst) {
          if (elt->isComment()) {
            delete elt;
          }
        }
        // delete the list
        delete lst;
      } else {
        // gather any comments from before the 'var'
        CommentsAndStmt cs = {context->gatherCommentsFromList(lst, (yylsp[-2])),
                              nullptr};
        auto multi = MultiDecl::build(BUILDER, LOC((yyloc)), context->visibility,
                                      context->consumeList(lst));
        cs.stmt = multi.release();
        (yyval.commentsAndStmt) = cs;
      }
      context->resetDeclState();
    }
#line 9001 "bison-chpl-lib.cpp"
    break;

  case 443: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2194 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9009 "bison-chpl-lib.cpp"
    break;

  case 444: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2198 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9017 "bison-chpl-lib.cpp"
    break;

  case 445: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2205 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     (yyvsp[-2].uniqueStr), context->visibility,
                                     context->varDeclKind,
                                     toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr)));
      // gather the comments like a statement does
      (yyval.commentsAndStmt) = STMT((yylsp[-2]), varDecl.release());
      context->clearComments();
    }
#line 9031 "bison-chpl-lib.cpp"
    break;

  case 446: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2215 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->clearComments();
    }
#line 9040 "bison-chpl-lib.cpp"
    break;

  case 447: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2223 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9046 "bison-chpl-lib.cpp"
    break;

  case 448: /* tuple_var_decl_component: ident_def  */
#line 2225 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9052 "bison-chpl-lib.cpp"
    break;

  case 449: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2227 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9058 "bison-chpl-lib.cpp"
    break;

  case 450: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2232 "chpl.ypp"
    { (yyval.exprList) = TODOLIST((yyloc)); }
#line 9064 "bison-chpl-lib.cpp"
    break;

  case 451: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA  */
#line 2234 "chpl.ypp"
    { (yyval.exprList) = TODOLIST((yyloc)); }
#line 9070 "bison-chpl-lib.cpp"
    break;

  case 452: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component TCOMMA tuple_var_decl_stmt_inner_ls  */
#line 2236 "chpl.ypp"
    { (yyval.exprList) = TODOLIST((yyloc)); }
#line 9076 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_init_expr: %empty  */
#line 2242 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9082 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2243 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9088 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2244 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9094 "bison-chpl-lib.cpp"
    break;

  case 456: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2250 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 457: /* ret_array_type: TLSBR TRSBR  */
#line 2254 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9110 "bison-chpl-lib.cpp"
    break;

  case 458: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2258 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9118 "bison-chpl-lib.cpp"
    break;

  case 459: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2262 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 460: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2266 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9134 "bison-chpl-lib.cpp"
    break;

  case 461: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2270 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9142 "bison-chpl-lib.cpp"
    break;

  case 462: /* ret_array_type: TLSBR error TRSBR  */
#line 2274 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_ret_type: %empty  */
#line 2280 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9156 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_ret_type: TCOLON type_level_expr  */
#line 2281 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_ret_type: TCOLON ret_array_type  */
#line 2282 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2283 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_ret_type: error  */
#line 2284 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9180 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_type: %empty  */
#line 2289 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9186 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_type: TCOLON type_level_expr  */
#line 2290 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_type: TCOLON array_type  */
#line 2291 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9198 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2292 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_type: error  */
#line 2293 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 473: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2314 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9218 "bison-chpl-lib.cpp"
    break;

  case 474: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2318 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9226 "bison-chpl-lib.cpp"
    break;

  case 475: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2322 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9234 "bison-chpl-lib.cpp"
    break;

  case 476: /* array_type: TLSBR error TRSBR  */
#line 2326 "chpl.ypp"
    {
      (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
    }
#line 9242 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_formal_array_elt_type: %empty  */
#line 2332 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9248 "bison-chpl-lib.cpp"
    break;

  case 478: /* opt_formal_array_elt_type: type_level_expr  */
#line 2333 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9254 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_formal_array_elt_type: query_expr  */
#line 2334 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9260 "bison-chpl-lib.cpp"
    break;

  case 480: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2339 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9266 "bison-chpl-lib.cpp"
    break;

  case 481: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2341 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9272 "bison-chpl-lib.cpp"
    break;

  case 482: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2347 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9278 "bison-chpl-lib.cpp"
    break;

  case 483: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2349 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9284 "bison-chpl-lib.cpp"
    break;

  case 484: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2351 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9290 "bison-chpl-lib.cpp"
    break;

  case 485: /* opt_formal_type: %empty  */
#line 2355 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9296 "bison-chpl-lib.cpp"
    break;

  case 486: /* opt_formal_type: TCOLON type_level_expr  */
#line 2356 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9302 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_formal_type: TCOLON query_expr  */
#line 2357 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9308 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2358 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9314 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_formal_type: TCOLON formal_array_type  */
#line 2359 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9320 "bison-chpl-lib.cpp"
    break;

  case 490: /* expr_ls: expr  */
#line 2365 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9326 "bison-chpl-lib.cpp"
    break;

  case 491: /* expr_ls: query_expr  */
#line 2366 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 492: /* expr_ls: expr_ls TCOMMA expr  */
#line 2367 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9338 "bison-chpl-lib.cpp"
    break;

  case 493: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2368 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9344 "bison-chpl-lib.cpp"
    break;

  case 494: /* simple_expr_ls: expr  */
#line 2372 "chpl.ypp"
                                   { (yyval.exprList) = TODOLIST((yyloc)); }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 495: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2373 "chpl.ypp"
                                   { (yyvsp[-2].exprList)->push_back((yyvsp[0].expr)); }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 496: /* tuple_component: TUNDERSCORE  */
#line 2377 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 497: /* tuple_component: opt_try_expr  */
#line 2378 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9368 "bison-chpl-lib.cpp"
    break;

  case 498: /* tuple_component: query_expr  */
#line 2379 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9374 "bison-chpl-lib.cpp"
    break;

  case 499: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2383 "chpl.ypp"
                                         { (yyval.exprList) = TODOLIST((yyloc)); }
#line 9380 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2384 "chpl.ypp"
                                         { (yyval.exprList) = TODOLIST((yyloc)); }
#line 9386 "bison-chpl-lib.cpp"
    break;

  case 501: /* opt_actual_ls: %empty  */
#line 2388 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9392 "bison-chpl-lib.cpp"
    break;

  case 502: /* opt_actual_ls: actual_ls  */
#line 2389 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9398 "bison-chpl-lib.cpp"
    break;

  case 503: /* actual_ls: actual_expr  */
#line 2394 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 504: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2399 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 505: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2407 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9423 "bison-chpl-lib.cpp"
    break;

  case 506: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2408 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 507: /* actual_expr: query_expr  */
#line 2409 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 508: /* actual_expr: opt_try_expr  */
#line 2410 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 509: /* ident_expr: ident_use  */
#line 2414 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 510: /* ident_expr: scalar_type  */
#line 2415 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 511: /* type_level_expr: sub_type_level_expr  */
#line 2427 "chpl.ypp"
    { (yyval.expr) = (yyvsp[0].expr); }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 512: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2429 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 513: /* type_level_expr: TQUESTION  */
#line 2431 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 519: /* sub_type_level_expr: TSINGLE expr  */
#line 2442 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 520: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2444 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9483 "bison-chpl-lib.cpp"
    break;

  case 521: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2446 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9489 "bison-chpl-lib.cpp"
    break;

  case 522: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2448 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9495 "bison-chpl-lib.cpp"
    break;

  case 523: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2450 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9501 "bison-chpl-lib.cpp"
    break;

  case 524: /* sub_type_level_expr: TATOMIC expr  */
#line 2452 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9507 "bison-chpl-lib.cpp"
    break;

  case 525: /* sub_type_level_expr: TSYNC expr  */
#line 2454 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9513 "bison-chpl-lib.cpp"
    break;

  case 526: /* sub_type_level_expr: TOWNED  */
#line 2457 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9519 "bison-chpl-lib.cpp"
    break;

  case 527: /* sub_type_level_expr: TOWNED expr  */
#line 2459 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9525 "bison-chpl-lib.cpp"
    break;

  case 528: /* sub_type_level_expr: TUNMANAGED  */
#line 2461 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9531 "bison-chpl-lib.cpp"
    break;

  case 529: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2463 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9537 "bison-chpl-lib.cpp"
    break;

  case 530: /* sub_type_level_expr: TSHARED  */
#line 2465 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9543 "bison-chpl-lib.cpp"
    break;

  case 531: /* sub_type_level_expr: TSHARED expr  */
#line 2467 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9549 "bison-chpl-lib.cpp"
    break;

  case 532: /* sub_type_level_expr: TBORROWED  */
#line 2469 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9555 "bison-chpl-lib.cpp"
    break;

  case 533: /* sub_type_level_expr: TBORROWED expr  */
#line 2471 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 534: /* sub_type_level_expr: TCLASS  */
#line 2474 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9567 "bison-chpl-lib.cpp"
    break;

  case 535: /* sub_type_level_expr: TRECORD  */
#line 2476 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9573 "bison-chpl-lib.cpp"
    break;

  case 536: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2481 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9579 "bison-chpl-lib.cpp"
    break;

  case 537: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2483 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9585 "bison-chpl-lib.cpp"
    break;

  case 538: /* for_expr: TFOR expr TDO expr  */
#line 2485 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9591 "bison-chpl-lib.cpp"
    break;

  case 539: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2487 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9597 "bison-chpl-lib.cpp"
    break;

  case 540: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2489 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9603 "bison-chpl-lib.cpp"
    break;

  case 541: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2491 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9609 "bison-chpl-lib.cpp"
    break;

  case 542: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2493 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9615 "bison-chpl-lib.cpp"
    break;

  case 543: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2495 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9621 "bison-chpl-lib.cpp"
    break;

  case 544: /* for_expr: TFORALL expr TDO expr  */
#line 2497 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9627 "bison-chpl-lib.cpp"
    break;

  case 545: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2499 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9633 "bison-chpl-lib.cpp"
    break;

  case 546: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2501 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9639 "bison-chpl-lib.cpp"
    break;

  case 547: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2503 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 548: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2505 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9651 "bison-chpl-lib.cpp"
    break;

  case 549: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2507 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 550: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2509 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9663 "bison-chpl-lib.cpp"
    break;

  case 551: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2511 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 552: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2513 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9675 "bison-chpl-lib.cpp"
    break;

  case 553: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2518 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9681 "bison-chpl-lib.cpp"
    break;

  case 554: /* nil_expr: TNIL  */
#line 2527 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9687 "bison-chpl-lib.cpp"
    break;

  case 562: /* stmt_level_expr: io_expr TIO expr  */
#line 2543 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9693 "bison-chpl-lib.cpp"
    break;

  case 563: /* opt_task_intent_ls: %empty  */
#line 2547 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9699 "bison-chpl-lib.cpp"
    break;

  case 564: /* opt_task_intent_ls: task_intent_clause  */
#line 2548 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9705 "bison-chpl-lib.cpp"
    break;

  case 565: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2553 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9715 "bison-chpl-lib.cpp"
    break;

  case 566: /* task_intent_ls: intent_expr  */
#line 2561 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9721 "bison-chpl-lib.cpp"
    break;

  case 567: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2562 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9727 "bison-chpl-lib.cpp"
    break;

  case 568: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2567 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9737 "bison-chpl-lib.cpp"
    break;

  case 569: /* forall_intent_ls: intent_expr  */
#line 2575 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9743 "bison-chpl-lib.cpp"
    break;

  case 570: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2576 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9749 "bison-chpl-lib.cpp"
    break;

  case 571: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 2581 "chpl.ypp"
  {
    if (auto ident = (yyvsp[-2].expr)->toIdentifier()) {
      auto name = ident->name();
      auto node = TaskVar::build(BUILDER, LOC((yyloc)), name, (yyvsp[-3].taskIntent),
                                        toOwned((yyvsp[-1].expr)),
                                        toOwned((yyvsp[0].expr)));
      (yyval.expr) = node.release();
    } else {
      const char* msg = "Expected identifier for task variable name";
      (yyval.expr) = raiseError((yyloc), context, msg);
    }
  }
#line 9766 "bison-chpl-lib.cpp"
    break;

  case 572: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2594 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9774 "bison-chpl-lib.cpp"
    break;

  case 573: /* intent_expr: expr TREDUCE ident_expr  */
#line 2598 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9782 "bison-chpl-lib.cpp"
    break;

  case 574: /* task_var_prefix: TCONST  */
#line 2604 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 9788 "bison-chpl-lib.cpp"
    break;

  case 575: /* task_var_prefix: TIN  */
#line 2605 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 9794 "bison-chpl-lib.cpp"
    break;

  case 576: /* task_var_prefix: TCONST TIN  */
#line 2606 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 9800 "bison-chpl-lib.cpp"
    break;

  case 577: /* task_var_prefix: TREF  */
#line 2607 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 9806 "bison-chpl-lib.cpp"
    break;

  case 578: /* task_var_prefix: TCONST TREF  */
#line 2608 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 9812 "bison-chpl-lib.cpp"
    break;

  case 579: /* task_var_prefix: TVAR  */
#line 2609 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 9818 "bison-chpl-lib.cpp"
    break;

  case 581: /* io_expr: io_expr TIO expr  */
#line 2615 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9824 "bison-chpl-lib.cpp"
    break;

  case 582: /* new_maybe_decorated: TNEW  */
#line 2620 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 9830 "bison-chpl-lib.cpp"
    break;

  case 583: /* new_maybe_decorated: TNEW TOWNED  */
#line 2622 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 9836 "bison-chpl-lib.cpp"
    break;

  case 584: /* new_maybe_decorated: TNEW TSHARED  */
#line 2624 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 9842 "bison-chpl-lib.cpp"
    break;

  case 585: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2626 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 9848 "bison-chpl-lib.cpp"
    break;

  case 586: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2628 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 9854 "bison-chpl-lib.cpp"
    break;

  case 587: /* new_expr: new_maybe_decorated expr  */
#line 2634 "chpl.ypp"
  {
    if (FnCall* fnCall = (yyvsp[0].expr)->toFnCall()) {
      (yyval.expr) = context->wrapCalledExpressionInNew((yyloc), (yyvsp[-1].newManagement), fnCall);
    } else {

      // It's an error for one reason or another. TODO: Specialize these
      // errors later (e.g. 'new a.field' would require parens around
      // the expression 'a.field'; 'new foo' would require an argument
      // list for 'foo'; and something like 'new __primitive()' just
      // doesn't make any sense...
      (yyval.expr) = raiseError((yyloc), context, "Invalid form for new expression");
    }
  }
#line 9872 "bison-chpl-lib.cpp"
    break;

  case 588: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2651 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 9880 "bison-chpl-lib.cpp"
    break;

  case 589: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2655 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 9888 "bison-chpl-lib.cpp"
    break;

  case 590: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2659 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 9896 "bison-chpl-lib.cpp"
    break;

  case 591: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2663 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 9904 "bison-chpl-lib.cpp"
    break;

  case 592: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2670 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9910 "bison-chpl-lib.cpp"
    break;

  case 602: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2687 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9916 "bison-chpl-lib.cpp"
    break;

  case 603: /* expr: expr TCOLON expr  */
#line 2689 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 604: /* expr: expr TDOTDOT expr  */
#line 2691 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 605: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2693 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9934 "bison-chpl-lib.cpp"
    break;

  case 606: /* expr: expr TDOTDOT  */
#line 2695 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9940 "bison-chpl-lib.cpp"
    break;

  case 607: /* expr: TDOTDOT expr  */
#line 2697 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9946 "bison-chpl-lib.cpp"
    break;

  case 608: /* expr: TDOTDOTOPENHIGH expr  */
#line 2699 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9952 "bison-chpl-lib.cpp"
    break;

  case 609: /* expr: TDOTDOT  */
#line 2701 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9958 "bison-chpl-lib.cpp"
    break;

  case 610: /* opt_expr: %empty  */
#line 2719 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 611: /* opt_expr: expr  */
#line 2720 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 612: /* opt_try_expr: TTRY expr  */
#line 2724 "chpl.ypp"
                  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9976 "bison-chpl-lib.cpp"
    break;

  case 613: /* opt_try_expr: TTRYBANG expr  */
#line 2725 "chpl.ypp"
                  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9982 "bison-chpl-lib.cpp"
    break;

  case 620: /* call_base_expr: expr TBANG  */
#line 2743 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9988 "bison-chpl-lib.cpp"
    break;

  case 621: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 2744 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9994 "bison-chpl-lib.cpp"
    break;

  case 624: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 2751 "chpl.ypp"
    {
      ASTList actuals;
      std::vector<UniqueString> actualNames;
      context->consumeNamedActuals((yyvsp[-1].maybeNamedActualList), actuals, actualNames);
      auto fnCall = FnCall::build(BUILDER, LOC((yyloc)),
                                  toOwned((yyvsp[-3].expr)),
                                  std::move(actuals),
                                  std::move(actualNames),
                                  /* square */ false);
      (yyval.expr) = fnCall.release();
    }
#line 10010 "bison-chpl-lib.cpp"
    break;

  case 625: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 2763 "chpl.ypp"
    {
      ASTList actuals;
      std::vector<UniqueString> actualNames;
      context->consumeNamedActuals((yyvsp[-1].maybeNamedActualList), actuals, actualNames);
      auto fnCall = FnCall::build(BUILDER, LOC((yyloc)),
                                  toOwned((yyvsp[-3].expr)),
                                  std::move(actuals),
                                  std::move(actualNames),
                                  /* square */ true);
      (yyval.expr) = fnCall.release();
    }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 626: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 2774 "chpl.ypp"
                                              { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10032 "bison-chpl-lib.cpp"
    break;

  case 627: /* dot_expr: expr TDOT ident_use  */
#line 2779 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10038 "bison-chpl-lib.cpp"
    break;

  case 628: /* dot_expr: expr TDOT TTYPE  */
#line 2781 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10044 "bison-chpl-lib.cpp"
    break;

  case 629: /* dot_expr: expr TDOT TDOMAIN  */
#line 2783 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 630: /* dot_expr: expr TDOT TLOCALE  */
#line 2785 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10056 "bison-chpl-lib.cpp"
    break;

  case 631: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 2787 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10066 "bison-chpl-lib.cpp"
    break;

  case 632: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 2793 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10076 "bison-chpl-lib.cpp"
    break;

  case 633: /* parenthesized_expr: TLP tuple_component TRP  */
#line 2805 "chpl.ypp"
                                    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10082 "bison-chpl-lib.cpp"
    break;

  case 634: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 2806 "chpl.ypp"
                                    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10088 "bison-chpl-lib.cpp"
    break;

  case 635: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 2807 "chpl.ypp"
                                    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10094 "bison-chpl-lib.cpp"
    break;

  case 636: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 2808 "chpl.ypp"
                                    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10100 "bison-chpl-lib.cpp"
    break;

  case 637: /* bool_literal: TFALSE  */
#line 2812 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10106 "bison-chpl-lib.cpp"
    break;

  case 638: /* bool_literal: TTRUE  */
#line 2813 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10112 "bison-chpl-lib.cpp"
    break;

  case 639: /* str_bytes_literal: STRINGLITERAL  */
#line 2817 "chpl.ypp"
                  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10118 "bison-chpl-lib.cpp"
    break;

  case 640: /* str_bytes_literal: BYTESLITERAL  */
#line 2818 "chpl.ypp"
                  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10124 "bison-chpl-lib.cpp"
    break;

  case 643: /* literal_expr: INTLITERAL  */
#line 2824 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10130 "bison-chpl-lib.cpp"
    break;

  case 644: /* literal_expr: REALLITERAL  */
#line 2825 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10136 "bison-chpl-lib.cpp"
    break;

  case 645: /* literal_expr: IMAGLITERAL  */
#line 2826 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10142 "bison-chpl-lib.cpp"
    break;

  case 646: /* literal_expr: CSTRINGLITERAL  */
#line 2827 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10148 "bison-chpl-lib.cpp"
    break;

  case 647: /* literal_expr: TNONE  */
#line 2828 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10154 "bison-chpl-lib.cpp"
    break;

  case 648: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 2829 "chpl.ypp"
                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10160 "bison-chpl-lib.cpp"
    break;

  case 649: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 2830 "chpl.ypp"
                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10166 "bison-chpl-lib.cpp"
    break;

  case 650: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 2831 "chpl.ypp"
                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10172 "bison-chpl-lib.cpp"
    break;

  case 651: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 2832 "chpl.ypp"
                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10178 "bison-chpl-lib.cpp"
    break;

  case 652: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 2834 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 10186 "bison-chpl-lib.cpp"
    break;

  case 653: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 2838 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 10194 "bison-chpl-lib.cpp"
    break;

  case 654: /* assoc_expr_ls: expr TALIAS expr  */
#line 2845 "chpl.ypp"
                                        { (yyval.exprList) = TODOLIST((yyloc)); }
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 655: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 2846 "chpl.ypp"
                                        { (yyvsp[-4].exprList)->push_back(TODOEXPR((yyloc))); }
#line 10206 "bison-chpl-lib.cpp"
    break;

  case 656: /* binary_op_expr: expr TPLUS expr  */
#line 2850 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10212 "bison-chpl-lib.cpp"
    break;

  case 657: /* binary_op_expr: expr TMINUS expr  */
#line 2851 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 658: /* binary_op_expr: expr TSTAR expr  */
#line 2852 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10224 "bison-chpl-lib.cpp"
    break;

  case 659: /* binary_op_expr: expr TDIVIDE expr  */
#line 2853 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10230 "bison-chpl-lib.cpp"
    break;

  case 660: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 2854 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10236 "bison-chpl-lib.cpp"
    break;

  case 661: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 2855 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 662: /* binary_op_expr: expr TMOD expr  */
#line 2856 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10248 "bison-chpl-lib.cpp"
    break;

  case 663: /* binary_op_expr: expr TEQUAL expr  */
#line 2857 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10254 "bison-chpl-lib.cpp"
    break;

  case 664: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 2858 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10260 "bison-chpl-lib.cpp"
    break;

  case 665: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 2859 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10266 "bison-chpl-lib.cpp"
    break;

  case 666: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 2860 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10272 "bison-chpl-lib.cpp"
    break;

  case 667: /* binary_op_expr: expr TLESS expr  */
#line 2861 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10278 "bison-chpl-lib.cpp"
    break;

  case 668: /* binary_op_expr: expr TGREATER expr  */
#line 2862 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10284 "bison-chpl-lib.cpp"
    break;

  case 669: /* binary_op_expr: expr TBAND expr  */
#line 2863 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10290 "bison-chpl-lib.cpp"
    break;

  case 670: /* binary_op_expr: expr TBOR expr  */
#line 2864 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10296 "bison-chpl-lib.cpp"
    break;

  case 671: /* binary_op_expr: expr TBXOR expr  */
#line 2865 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10302 "bison-chpl-lib.cpp"
    break;

  case 672: /* binary_op_expr: expr TAND expr  */
#line 2866 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10308 "bison-chpl-lib.cpp"
    break;

  case 673: /* binary_op_expr: expr TOR expr  */
#line 2867 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10314 "bison-chpl-lib.cpp"
    break;

  case 674: /* binary_op_expr: expr TEXP expr  */
#line 2868 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 675: /* binary_op_expr: expr TBY expr  */
#line 2869 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10326 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr TALIGN expr  */
#line 2870 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10332 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr THASH expr  */
#line 2871 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10338 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TDMAPPED expr  */
#line 2872 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10344 "bison-chpl-lib.cpp"
    break;

  case 679: /* unary_op_expr: TPLUS expr  */
#line 2876 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10350 "bison-chpl-lib.cpp"
    break;

  case 680: /* unary_op_expr: TMINUS expr  */
#line 2877 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10356 "bison-chpl-lib.cpp"
    break;

  case 681: /* unary_op_expr: TMINUSMINUS expr  */
#line 2878 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 10362 "bison-chpl-lib.cpp"
    break;

  case 682: /* unary_op_expr: TPLUSPLUS expr  */
#line 2879 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 10368 "bison-chpl-lib.cpp"
    break;

  case 683: /* unary_op_expr: TBANG expr  */
#line 2880 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10374 "bison-chpl-lib.cpp"
    break;

  case 684: /* unary_op_expr: expr TBANG  */
#line 2881 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10382 "bison-chpl-lib.cpp"
    break;

  case 685: /* unary_op_expr: TBNOT expr  */
#line 2884 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10388 "bison-chpl-lib.cpp"
    break;

  case 686: /* reduce_expr: expr TREDUCE expr  */
#line 2888 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10394 "bison-chpl-lib.cpp"
    break;

  case 687: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 2889 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10400 "bison-chpl-lib.cpp"
    break;

  case 688: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 2890 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10406 "bison-chpl-lib.cpp"
    break;

  case 689: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 2891 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10412 "bison-chpl-lib.cpp"
    break;

  case 690: /* scan_expr: expr TSCAN expr  */
#line 2895 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10418 "bison-chpl-lib.cpp"
    break;

  case 691: /* scan_expr: expr TSCAN zippered_iterator  */
#line 2896 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10424 "bison-chpl-lib.cpp"
    break;

  case 692: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 2897 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10430 "bison-chpl-lib.cpp"
    break;

  case 693: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 2898 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10436 "bison-chpl-lib.cpp"
    break;

  case 694: /* reduce_scan_op_expr: TPLUS  */
#line 2903 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10442 "bison-chpl-lib.cpp"
    break;

  case 695: /* reduce_scan_op_expr: TSTAR  */
#line 2904 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10448 "bison-chpl-lib.cpp"
    break;

  case 696: /* reduce_scan_op_expr: TAND  */
#line 2905 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10454 "bison-chpl-lib.cpp"
    break;

  case 697: /* reduce_scan_op_expr: TOR  */
#line 2906 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10460 "bison-chpl-lib.cpp"
    break;

  case 698: /* reduce_scan_op_expr: TBAND  */
#line 2907 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10466 "bison-chpl-lib.cpp"
    break;

  case 699: /* reduce_scan_op_expr: TBOR  */
#line 2908 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10472 "bison-chpl-lib.cpp"
    break;

  case 700: /* reduce_scan_op_expr: TBXOR  */
#line 2909 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10478 "bison-chpl-lib.cpp"
    break;


#line 10482 "bison-chpl-lib.cpp"

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
  yyerror (&yylloc, context, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
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
