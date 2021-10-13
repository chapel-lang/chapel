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
  YYSYMBOL_225_3 = 225,                    /* $@3  */
  YYSYMBOL_extern_block_stmt = 226,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 227,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 228,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 229,                  /* if_stmt  */
  YYSYMBOL_ifvar = 230,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 231,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 232,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 233,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 234,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 235, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 236,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 237,           /* ifc_constraint  */
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
  YYSYMBOL_254_4 = 254,                    /* $@4  */
  YYSYMBOL_deprecated_enum_item = 255,     /* deprecated_enum_item  */
  YYSYMBOL_enum_item = 256,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 257,         /* lambda_decl_expr  */
  YYSYMBOL_linkage_spec_empty = 258,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 259,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt_complete = 260,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 261,             /* fn_decl_stmt  */
  YYSYMBOL_262_5 = 262,                    /* $@5  */
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
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 289, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 290, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 291,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 292,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 293,            /* var_decl_stmt  */
  YYSYMBOL_294_6 = 294,                    /* $@6  */
  YYSYMBOL_var_decl_stmt_inner_ls = 295,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 296,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 297, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 298, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 299,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 300,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 301,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 302,                 /* opt_type  */
  YYSYMBOL_array_type = 303,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 304, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 305,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 306,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 307,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 308,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 309,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 310,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 311,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 312,                /* actual_ls  */
  YYSYMBOL_actual_expr = 313,              /* actual_expr  */
  YYSYMBOL_ident_expr = 314,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 315,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 316,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 317,                 /* for_expr  */
  YYSYMBOL_cond_expr = 318,                /* cond_expr  */
  YYSYMBOL_nil_expr = 319,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 320,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 321,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 322,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 323,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 324,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 325,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 326,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 327,          /* task_var_prefix  */
  YYSYMBOL_io_expr = 328,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 329,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 330,                 /* new_expr  */
  YYSYMBOL_let_expr = 331,                 /* let_expr  */
  YYSYMBOL_expr = 332,                     /* expr  */
  YYSYMBOL_opt_expr = 333,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 334,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 335,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 336,           /* call_base_expr  */
  YYSYMBOL_call_expr = 337,                /* call_expr  */
  YYSYMBOL_dot_expr = 338,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 339,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 340,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 341,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 342,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 343,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 344,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 345,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 346,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 347,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 348       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 358 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include other helper functions for the parser
  #include "parser-help.h"


#line 471 "bison-chpl-lib.cpp"

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
#define YYLAST   20492

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  169
/* YYNRULES -- Number of rules.  */
#define YYNRULES  702
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
       0,   636,   636,   641,   642,   647,   648,   653,   671,   691,
     692,   693,   694,   695,   696,   697,   698,   699,   700,   701,
     702,   703,   704,   717,   718,   719,   720,   721,   722,   736,
     745,   760,   769,   777,   800,   812,   823,   835,   847,   858,
     871,   878,   885,   886,   888,   893,   894,   898,   911,   924,
     937,   951,   952,   954,   959,   960,   965,   982,   986,   990,
    1001,  1020,  1021,  1025,  1029,  1034,  1038,  1048,  1053,  1059,
    1066,  1071,  1078,  1090,  1091,  1095,  1100,  1107,  1111,  1118,
    1126,  1135,  1142,  1150,  1162,  1169,  1173,  1179,  1188,  1189,
    1193,  1202,  1206,  1210,  1214,  1218,  1222,  1231,  1232,  1236,
    1237,  1238,  1239,  1240,  1241,  1244,  1245,  1246,  1247,  1248,
    1249,  1261,  1262,  1273,  1274,  1275,  1276,  1277,  1278,  1279,
    1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,  1289,
    1290,  1291,  1295,  1296,  1297,  1298,  1299,  1300,  1301,  1302,
    1303,  1304,  1305,  1306,  1313,  1314,  1315,  1316,  1320,  1321,
    1325,  1332,  1342,  1343,  1345,  1350,  1354,  1360,  1360,  1366,
    1366,  1375,  1376,  1377,  1378,  1379,  1380,  1381,  1385,  1392,
    1399,  1406,  1416,  1423,  1424,  1428,  1437,  1443,  1451,  1471,
    1470,  1483,  1490,  1508,  1521,  1534,  1538,  1542,  1546,  1550,
    1554,  1558,  1562,  1580,  1584,  1588,  1592,  1596,  1600,  1604,
    1608,  1612,  1616,  1620,  1624,  1628,  1632,  1636,  1640,  1644,
    1648,  1652,  1656,  1660,  1664,  1668,  1673,  1681,  1690,  1694,
    1698,  1702,  1706,  1710,  1714,  1718,  1722,  1727,  1732,  1737,
    1745,  1758,  1774,  1776,  1781,  1782,  1787,  1792,  1793,  1794,
    1795,  1796,  1797,  1798,  1799,  1800,  1801,  1802,  1803,  1804,
    1816,  1817,  1818,  1819,  1828,  1829,  1833,  1835,  1837,  1842,
    1844,  1846,  1851,  1855,  1859,  1863,  1867,  1871,  1878,  1879,
    1883,  1887,  1891,  1898,  1909,  1923,  1931,  1935,  1944,  1945,
    1949,  1953,  1958,  1968,  1973,  1984,  1991,  1992,  1993,  1997,
    1998,  2003,  2004,  2006,  2011,  2027,  2040,  2047,  2051,  2056,
    2056,  2063,  2064,  2066,  2071,  2076,  2085,  2091,  2095,  2096,
    2098,  2103,  2113,  2110,  2138,  2146,  2154,  2165,  2176,  2185,
    2196,  2197,  2201,  2202,  2203,  2204,  2205,  2206,  2207,  2208,
    2209,  2210,  2211,  2212,  2213,  2214,  2215,  2216,  2217,  2218,
    2219,  2220,  2221,  2222,  2223,  2224,  2225,  2226,  2227,  2228,
    2237,  2238,  2239,  2240,  2241,  2242,  2243,  2244,  2245,  2246,
    2247,  2248,  2252,  2253,  2257,  2261,  2262,  2266,  2267,  2271,
    2274,  2276,  2278,  2280,  2282,  2289,  2290,  2294,  2295,  2296,
    2297,  2298,  2299,  2300,  2301,  2302,  2306,  2307,  2308,  2309,
    2310,  2311,  2315,  2316,  2317,  2321,  2322,  2323,  2324,  2325,
    2326,  2330,  2331,  2334,  2335,  2339,  2340,  2344,  2348,  2349,
    2350,  2354,  2355,  2357,  2359,  2361,  2366,  2368,  2373,  2375,
    2377,  2379,  2381,  2383,  2385,  2390,  2391,  2395,  2403,  2407,
    2412,  2420,  2424,  2431,  2450,  2451,  2453,  2461,  2462,  2463,
    2464,  2465,  2470,  2469,  2478,  2486,  2490,  2497,  2511,  2525,
    2539,  2553,  2567,  2569,  2571,  2578,  2579,  2580,  2585,  2589,
    2593,  2597,  2601,  2605,  2609,  2616,  2617,  2618,  2619,  2620,
    2625,  2626,  2627,  2628,  2629,  2649,  2653,  2657,  2661,  2668,
    2669,  2670,  2674,  2679,  2687,  2692,  2696,  2703,  2704,  2705,
    2706,  2707,  2713,  2714,  2715,  2716,  2720,  2721,  2725,  2726,
    2727,  2731,  2735,  2742,  2743,  2747,  2752,  2761,  2762,  2763,
    2764,  2768,  2769,  2780,  2782,  2784,  2790,  2791,  2792,  2793,
    2794,  2795,  2797,  2799,  2801,  2803,  2805,  2807,  2810,  2812,
    2814,  2816,  2818,  2820,  2822,  2824,  2827,  2829,  2834,  2843,
    2852,  2860,  2862,  2864,  2866,  2875,  2884,  2892,  2894,  2896,
    2898,  2914,  2923,  2932,  2934,  2939,  2949,  2957,  2958,  2959,
    2960,  2961,  2962,  2963,  2964,  2969,  2970,  2974,  2983,  2984,
    2988,  2997,  2998,  3002,  3015,  3019,  3026,  3027,  3028,  3029,
    3030,  3031,  3035,  3036,  3041,  3043,  3045,  3047,  3049,  3055,
    3072,  3076,  3080,  3084,  3091,  3099,  3100,  3101,  3102,  3103,
    3104,  3105,  3106,  3107,  3108,  3110,  3114,  3119,  3124,  3129,
    3135,  3141,  3173,  3174,  3178,  3179,  3180,  3184,  3185,  3186,
    3187,  3196,  3197,  3198,  3199,  3200,  3204,  3216,  3228,  3235,
    3237,  3239,  3241,  3243,  3249,  3262,  3263,  3267,  3271,  3278,
    3279,  3283,  3284,  3288,  3289,  3290,  3291,  3292,  3293,  3294,
    3295,  3299,  3303,  3307,  3311,  3315,  3323,  3324,  3328,  3329,
    3330,  3331,  3332,  3333,  3334,  3335,  3336,  3337,  3338,  3339,
    3340,  3341,  3342,  3343,  3344,  3345,  3346,  3347,  3348,  3349,
    3350,  3354,  3355,  3356,  3357,  3358,  3359,  3362,  3366,  3370,
    3374,  3378,  3385,  3386,  3387,  3388,  3393,  3394,  3395,  3396,
    3397,  3398,  3399
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
  "extern_export_decl_stmt_start", "extern_export_decl_stmt", "$@3",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
  "catch_expr", "catch_expr_inner", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_start",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header_lcbr", "enum_ls", "$@4", "deprecated_enum_item",
  "enum_item", "lambda_decl_expr", "linkage_spec_empty", "linkage_spec",
  "fn_decl_stmt_complete", "fn_decl_stmt", "$@5", "fn_decl_stmt_inner",
  "fn_decl_stmt_start", "fn_decl_receiver_expr", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls_inner",
  "formal_ls", "formal", "opt_intent_tag", "required_intent_tag",
  "opt_this_intent_tag", "proc_iter_or_op", "opt_ret_tag",
  "opt_throws_error", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "var_arg_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner_ls", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt", "$@6",
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

#define YYPACT_NINF (-1098)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-653)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1098,   120,  3746, -1098,   -35,   127, -1098, -1098, -1098, -1098,
   -1098, -1098,  5138,   148,   351,   285, 14608,   311, 20257,   148,
   11548,   369,   277,   400,   351,  5138, 11548,  2045,  5138,   331,
   20291, -1098,    96,   498,  8608,  9820,  9820, -1098,  8780,   503,
     298,   378, -1098,   508, 20291, 20291, 20291,   399,  1642,  9992,
     519, 11548,   220, -1098,   523,   526, 11548, -1098, 14608, -1098,
   11548,   582,   428,   211,   251,   548, 20377, -1098, 10166,  8090,
   11548,  9992, 14608, 11548,   506,   559,   447,  5138,   569, 11548,
     573, 11720, 11720, -1098,   584, -1098, 14608, -1098,   587,  8780,
   11548, -1098, 11548, -1098, 11548, -1098, -1098, 14128, 11548, -1098,
   11548, -1098, -1098, -1098,  4094,  7226,  8954, 11548, -1098,  4964,
   -1098, -1098, -1098,   475, -1098,    26, -1098, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,  7398,
   -1098, 10338, -1098, -1098, -1098, -1098, -1098,   594, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098,   510, 20291, -1098, 20137,   118,
   -1098,   280, -1098, -1098,    91,   487, -1098, 20291,  1642, -1098,
     491, -1098,   497, -1098, -1098,   495,   499,   520, 11548,   512,
     513, 19929,  1804,   107,   516,   517, -1098, -1098,   342, -1098,
   -1098, -1098, -1098, -1098,   504, -1098, -1098, 19929,   511,  5138,
   -1098, -1098,   521, 11548, -1098, -1098, 11548, 11548, 11548, 20291,
   -1098, 11548, 10166, 10166,   627,   355, -1098, -1098, -1098, -1098,
      -9,   461, -1098, -1098,   518, 16485, -1098,   230, -1098,   527,
   -1098,   -20, 19929,  2894, -1098, 20291,   585, -1098, -1098,   606,
     574,  8264, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098,   525, -1098, -1098, -1098,
   20291,   524,    39, 16072,    29, 15907,    29, 15989, 20291, 20291,
     141, 15160, -1098, -1098,   606,  8264,   531,   464,  5138,    57,
      91,  1406,   126,    83, -1098,  5138, -1098, -1098, 16566,   766,
   -1098,   532,   533, -1098, 16566,    -9,   766, -1098,  8264,  1738,
    1738, -1098, -1098,   172, 19929, 11548, 11548, -1098, 19929,   534,
   17140, -1098, 16566,    -9, 19929,   535,  8264, -1098, 19929, 17223,
   -1098, -1098, 17305,  3292, -1098, -1098, 17386,    -9,    39, 16566,
   17467,   463,   463,   480,   766,   766,   -30, -1098, -1098,  4268,
     168, -1098, 11548, -1098,   142,   194, -1098,   -16,   101, 17548,
     129,   480,   692, -1098,  4442, 11548, -1098, 11548,   640, -1098,
   -1098, 16155,   222,   510, 19929,   370, 20291, 10166,   538, -1098,
     549, 19741,   596,   323, -1098, -1098, -1098, -1098, -1098, -1098,
     619, -1098, -1098, -1098,    62,   632, -1098, -1098, -1098,  3502,
     598,   561,   586,   183,   478, -1098, 11548,   583, 11548, 11548,
   11548,  9820,  9820, 11548,   493, 11548, 11548, 11548, 11548, 11548,
     528, 14128, 11548, 11548, 11548, 11548, 11548, 11548, 11548, 11548,
   11548, 11548, 11548, 11548, 11548, 11548, 11548, 11548,   664, -1098,
   -1098, -1098, -1098, -1098,  9126,  9126, -1098, -1098, -1098, -1098,
    9126, -1098, -1098,  9126,  9126,  8264,  8264,  9820,  9820,  7918,
   -1098, -1098, 16648, 16729, 17629,   563,    18, 20291,  4616, -1098,
    9820,    39,  1642, -1098, 11548, -1098, -1098, 11548,   612, -1098,
     566,   595, -1098, -1098, -1098,   693, 10166, -1098,  5312,  9820,
   -1098,   571, -1098,    39,  5486,  9820, -1098,    39, -1098,    39,
    9820, -1098,    39,   622,   623,  5138,   715,  5138, -1098,   722,
   11548,   694,   591,  8264, 20291, -1098, -1098,    65, -1098, -1098,
   -1098, -1098, -1098, -1098,   754,   620,   597, -1098,  2205, -1098,
     110, -1098,  1406, -1098, -1098,   204, -1098, 11892,   642, 11548,
    1642, -1098, -1098, 11548, 11548, -1098,   608, -1098, -1098, 10166,
   -1098, 19929, 19929, -1098,    54, -1098,  8264,   610, -1098,   749,
   -1098,   749, -1098, -1098, -1098, -1098, -1098, -1098, -1098,  9300,
   -1098, 17710,  7572, -1098,  7746, -1098,  5138,   604,  9820,  9474,
    3920,   613, 11548, 10510, -1098, -1098,   167, -1098,  4790, -1098,
     281, 17791,   315, 16238, 20291, 12064, 11548, -1098,   510,   614,
     230, -1098,   -38,   639,  1208, -1098, 20291, -1098, 11548,   353,
   -1098, -1098, -1098, 12236,   682, -1098,   617,   391, -1098,   643,
   -1098,   644,   646,   650,   647,   648, -1098,   652,   661,   654,
     656,   657,   406,   666,   662,   663, -1098, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, 11548,
   -1098,   673,   674,   667,   617,   617, -1098, 12408, -1098, -1098,
   20291, -1098, 17872,  3227,  3227,   514, -1098,   514, -1098,   514,
   19240,  1425,  1162,  1468,    -9,   463, -1098,   496, -1098, -1098,
   -1098, -1098, -1098,   480, 19431,   514,  1204,  1204,  3227,  1204,
    1204,   609,   463, 19431, 16526,   609,   766,   766,   463,   480,
     660,   671,   672,   675,   677,   678,   649,   651, -1098,   514,
   -1098,   514,   236, -1098, -1098, -1098,   206, -1098,   830, 19970,
     529, 12580,  9820, 12752,  9820, 11548,  8264,  9820, 14888,   645,
     148, 17953, -1098,   316, 19929, 18034,  8264, -1098,  8264, 11548,
     226,  8780, 19929,    56, 16810,  7918, -1098,  8780, 19929,    31,
   16320, -1098, -1098,    29, 16403, -1098, 11548, 11548,   785,  5138,
     791, 18115,  5138, 16892, 20291, -1098,   247, -1098,   284, -1098,
   -1098, -1098, 20291,   882, -1098,  1406,   683,    38,   299,  1406,
     126,    50,    66, 11548, 11548,  7052, -1098, -1098,   205,  8436,
   -1098, 19929, -1098, 18196, 18277, -1098, -1098, 19929,   658,    77,
     676, -1098, 14974, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
   -1098,  5138,   149, 16975, -1098, -1098, 19929,  5138, 19929, -1098,
   18358, -1098, -1098, 11548, -1098,    95,    80, 11548, -1098, 10682,
   12064, 11548, -1098,   680,   704,   688, 18439,   704,   695,   687,
   -1098,  1642, -1098,  8264,   697,  1838, -1098, 19929, 20171,  6878,
   -1098, -1098,   201, -1098,    38,    57, -1098, 18520, -1098, 15078,
   -1098, -1098, -1098,   373, -1098,   685,   689, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098,  7918, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
      51,  9820,  9820, 11548,   831, 18601, 11548,   835, 18682,   309,
     698, 18763,  8264,    39,    39, -1098, -1098, -1098, -1098, -1098,
   16566, -1098, 15243,  5660, -1098,  5834, -1098,   312, -1098, 15326,
    6008, -1098,    39,  6182, -1098,    39, -1098,    39, -1098,    39,
   19929, 19929,  5138, -1098,  5138, 11548, -1098,  5138,   834,   707,
     709, 20291,   518,   683, -1098,   320, 10856,   375,   154, 11548,
      25, -1098, -1098,   642,   700,    33, -1098, -1098,   711,   714,
   -1098,  6356, 10166, -1098, -1098, -1098, 20291, -1098,   733,   518,
   -1098,  6530,   712,  6704,   713, -1098, 11548, -1098, -1098, 11548,
   18844,    87, 17058,   724,   725, 11548, -1098, 11548, -1098, -1098,
    2321,   380,   324, -1098,  1113, -1098, -1098, -1098,  3066, -1098,
   -1098, -1098,   717, 13956,    42, -1098,   720, -1098,   756,   758,
     617,   617, -1098, -1098, -1098,   642,   325,   329, 18926, 12924,
   13096, 19007, 13268, 13440, -1098, 13612, 13784,   338, -1098, -1098,
   -1098,  5138,  8780, 19929,  8780, 19929,  7918, -1098,  5138,  8780,
   19929, -1098,  8780, 19929, -1098, -1098, -1098, -1098, -1098, 19929,
     863,  5138, -1098, -1098, -1098, -1098,   375,   683,  9648, -1098,
   -1098, -1098,   116, 10166, -1098, -1098, -1098,   133, -1098,   138,
   -1098,   456, 19101, -1098, -1098, -1098, -1098, -1098,  9820, 14748,
    8264,  8264,  5138, -1098,    64,   727, 11548, -1098,  8780, -1098,
   19929,  5138,  8780, -1098, 19929,  5138, 19929,   366, 11028, 12064,
   11548, 12064, 11548, -1098, -1098, 19158, 19929,  2620, -1098, -1098,
     738,  2688, -1098, -1098, -1098, -1098,   425, 14297,   157, -1098,
   -1098, -1098, -1098, 11548, 11548, 11548, 11548, 11548, 11548, 11548,
   11548, -1098, 18115, 15409, 15492, -1098, 18115, 15575, 15658,  5138,
   -1098, -1098, -1098,   375, 11202,    67, -1098, 19929, -1098, 11548,
     154,   133,   133,   133,   133,   133,   133,   154, 19198, -1098,
     706,   732,   743,   574, -1098,   518, 19929, 15741, -1098, 15824,
   -1098, -1098, -1098, 19929,   755,   760,   762,   763, 11548, -1098,
   -1098, -1098, -1098,   429, -1098, -1098, -1098, 19349, 19389, 19540,
   19580, 19731, 19771,  5138,  5138,  5138,  5138, -1098, -1098, -1098,
   -1098, -1098, -1098,   203,  9820, 14437, 19929, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098,   773, 13784,   757,   759, -1098,  5138,
    5138, -1098, -1098, -1098, -1098, 19929, 11548, 11548, 11548, 11548,
   11548, 11548, 18115, 18115, 18115, 18115, 19812, -1098, -1098,  1479,
   -1098, -1098, 18115, 18115, 11376, -1098
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   111,   645,   646,   647,   641,
     642,   648,     0,   565,    97,   132,   534,   139,   536,   565,
       0,   138,   442,   440,    97,     0,     0,   307,     0,   254,
     133,   174,   173,   639,     0,     0,     0,   172,     0,   137,
      51,   255,   309,   134,     0,     0,     0,     0,     0,     0,
     141,     0,   584,   556,   649,   142,     0,   310,   528,   437,
       0,     0,     0,   159,   157,   136,   537,   439,     0,     0,
       0,     0,   532,     0,     0,   140,     0,     0,   112,     0,
     640,     0,     0,   428,   135,   288,   530,   441,   143,     0,
       0,   698,     0,   700,     0,   701,   702,   611,     0,   699,
     696,   515,   155,   697,     0,     0,     0,     0,     4,     0,
       5,     9,    42,     0,    45,    54,    10,    60,    11,    12,
      13,    14,    15,   511,   512,    25,    46,   156,   166,     0,
     175,   612,   167,    16,    20,    17,    19,     0,   249,    18,
     603,    23,    24,    21,   165,   289,     0,   163,     0,   600,
     308,     0,   161,   311,   395,   386,   164,     0,     0,   162,
     617,   596,   513,   597,   518,   516,     0,     0,     0,   601,
     602,     0,   517,     0,   618,   619,   620,   643,   644,   595,
     520,   519,   598,   599,     0,    41,    27,   526,     0,     0,
     566,    98,     0,     0,   536,   133,     0,     0,     0,     0,
     537,     0,     0,     0,     0,   600,   617,   516,   601,   602,
     535,   517,   618,   619,     0,   565,   429,     0,   438,     0,
      22,     0,   496,   307,   286,     0,   173,   287,    44,    54,
       0,   503,   105,   113,   125,   119,   118,   127,   108,   117,
     128,   114,   129,   106,   130,   123,   116,   124,   122,   120,
     121,   107,   109,   115,   126,   131,     0,   110,   181,   430,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    52,    54,   503,     0,     0,     0,   375,
     395,     0,     0,     0,   445,     0,   149,    35,     0,   683,
     588,   585,   586,   587,     0,   529,   684,     7,   503,   307,
     307,   407,   493,     0,   492,     0,     0,   150,   616,     0,
       0,    38,     0,   533,   521,     0,   503,    39,   527,     0,
     268,   264,     0,   517,   268,   265,     0,   531,     0,     0,
       0,   685,   687,   609,   682,   681,     0,    57,    61,     0,
       0,   498,     0,   500,     0,     0,   499,     0,     0,   492,
       0,   610,     0,     6,     0,     0,    55,     0,     0,   442,
     171,     0,   641,   289,   613,   179,     0,     0,     0,   285,
       0,     0,   304,     0,   297,   301,   393,   394,   392,   319,
     396,   399,   398,   400,     0,   390,   387,   388,   391,     0,
     434,     0,   431,     0,   514,    26,     0,   589,     0,     0,
       0,     0,     0,     0,   686,     0,     0,     0,     0,     0,
       0,   608,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   350,   357,
     358,   359,   354,   356,     0,     0,   352,   355,   353,   351,
       0,   361,   360,     0,     0,   503,   503,     0,     0,     0,
      28,    29,     0,     0,     0,     0,     0,     0,     0,    30,
       0,     0,     0,    31,     0,    32,    43,     0,   511,   509,
       0,   504,   505,   510,   296,     0,     0,   191,     0,     0,
     190,     0,   199,     0,     0,     0,   197,     0,   207,     0,
       0,   205,     0,     0,     0,     0,   223,     0,   350,   219,
       0,     0,     0,     0,     0,   233,    33,   382,   377,   378,
     379,   383,   384,   385,   375,   368,     0,   365,     0,   376,
       0,   449,     0,   450,   452,     0,   474,     0,   455,     0,
       0,   148,    34,     0,     0,    36,     0,   160,   158,     0,
      90,   614,   615,   151,     0,    37,     0,     0,   275,   266,
     262,   267,   263,   184,   183,    40,    59,    58,    62,     0,
     650,     0,     0,   635,     0,   637,     0,     0,     0,     0,
       0,     0,     0,     0,   654,     8,     0,    48,     0,    88,
       0,    85,     0,    67,     0,     0,    73,   168,   289,     0,
       0,   178,   260,   290,     0,   295,     0,   303,     0,   299,
     294,   397,   469,     0,   401,   389,   362,    99,   344,   113,
     342,   119,   118,   102,   117,   114,   347,   129,   100,   130,
     116,   120,   101,   103,   115,   131,   341,   323,   326,   324,
     325,   348,   336,   327,   340,   332,   330,   343,   346,   331,
     329,   334,   339,   328,   333,   337,   338,   335,   345,     0,
     322,     0,   104,     0,   362,   362,   320,     0,   433,   427,
       0,   444,   564,   678,   677,   680,   689,   688,   693,   692,
     674,   671,   672,   673,   605,   661,   111,     0,   631,   632,
     112,   630,   629,   606,   665,   676,   670,   668,   679,   669,
     667,   659,   664,   666,   675,   658,   662,   663,   660,   607,
       0,     0,     0,     0,     0,     0,     0,     0,   691,   690,
     695,   694,   576,   577,   579,   581,     0,   568,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   652,   260,
     565,   565,   187,     0,   497,     0,     0,   523,     0,     0,
       0,     0,   540,     0,     0,     0,   200,     0,   546,     0,
       0,   198,   208,     0,     0,   206,     0,     0,   222,     0,
     218,     0,     0,     0,     0,   522,     0,   236,     0,   234,
     380,   381,     0,   375,   364,     0,   487,   411,     0,   453,
       0,   146,   147,   145,   144,     0,   473,   472,   596,     0,
     447,   594,   446,     0,     0,   628,   495,   494,     0,     0,
       0,   524,     0,   269,   651,   604,   636,   501,   638,   502,
     215,     0,     0,     0,   653,   213,   550,     0,   656,   655,
       0,    50,    49,     0,    84,     0,     0,     0,    77,     0,
       0,    73,    47,   697,    76,     0,    63,    74,     0,     0,
     291,     0,   257,     0,     0,   307,   302,   305,     0,     0,
     468,   467,   596,   402,   411,   375,   318,     0,   349,     0,
     314,   315,   436,   596,   432,     0,     0,    96,    94,    95,
      93,    92,    91,   626,   627,   578,   580,     0,   567,   132,
     139,   138,   137,   134,   141,   142,   136,   140,   135,   143,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   443,   182,   507,   508,   506,
       0,   217,     0,     0,   189,     0,   188,     0,   571,     0,
       0,   195,     0,     0,   193,     0,   203,     0,   201,     0,
     231,   230,     0,   225,     0,     0,   221,     0,   227,     0,
     259,     0,     0,   487,   366,     0,     0,   455,     0,     0,
       0,   451,   454,   455,     0,     0,   456,   457,     0,     0,
     277,     0,     0,   276,   279,   525,     0,   270,   273,     0,
     216,     0,     0,     0,     0,   214,     0,    89,    86,     0,
      70,    69,    68,     0,     0,     0,   169,     0,   170,   291,
     307,     0,     0,   284,   307,   159,   157,   283,   307,   292,
     152,   300,     0,   459,     0,   312,     0,   321,    99,   101,
     362,   362,   633,   634,   569,   455,   617,   617,     0,     0,
       0,     0,     0,     0,   259,     0,     0,     0,   186,   185,
     192,     0,     0,   539,     0,   538,     0,   570,     0,     0,
     545,   196,     0,   544,   194,   204,   202,   224,   220,   555,
     226,     0,    56,   256,   235,   232,   455,   487,     0,   490,
     489,   491,   596,   408,   371,   369,   425,     0,   426,   413,
     416,     0,   412,   405,   406,   306,   448,   478,     0,   652,
     503,   503,     0,   281,     0,     0,     0,   271,     0,   211,
     552,     0,     0,   209,   551,     0,   657,     0,     0,     0,
      73,     0,    73,    78,    81,    65,    64,   307,   176,   180,
     261,   307,   154,   293,   464,   462,   596,   652,     0,   363,
     316,   317,   573,     0,     0,     0,     0,     0,     0,     0,
       0,   261,   543,     0,     0,   572,   549,     0,     0,     0,
     229,   372,   370,   455,   479,     0,   410,   409,   424,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   476,
     596,     0,     0,   282,   280,     0,   274,     0,   212,     0,
     210,    87,    72,    71,     0,     0,     0,     0,     0,   177,
     258,   153,   463,   596,   403,   313,   404,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   374,   373,
     481,   482,   484,   596,     0,   652,   415,   417,   418,   421,
     422,   423,   419,   420,   414,     0,   590,   591,   272,     0,
       0,    80,    83,    79,    82,    66,     0,     0,     0,     0,
       0,     0,   542,   541,   548,   547,     0,   483,   485,   596,
     592,   593,   554,   553,   479,   486
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1098, -1098, -1098,     3,  -267,  2349, -1098,    20, -1098, -1098,
      13,   668, -1098,  -896,  2554,   589,  -559, -1098,  -801,  -799,
   -1098, -1098,   113, -1098, -1098,   377,   914, -1098,  2817,   -64,
    -382, -1098,  -590,  2269,  -879,   -58,  -830, -1098, -1098,   240,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,    15, -1098,
     852, -1098, -1098,     5,   919, -1098, -1098, -1098, -1098,   624,
   -1098,   -17, -1098, -1098, -1098, -1098, -1098,   -98, -1098,  -329,
    -772, -1098, -1098, -1098, -1098,   103,  -336,   987, -1098, -1098,
   -1098,   588, -1098, -1098, -1098, -1098,    93,  -171,  -638, -1098,
   -1098,    99,   182,   442, -1098, -1098, -1098,   686, -1098, -1098,
    -150,   109,  -981,   115,  -194,  -180,  -794, -1098, -1098,   314,
   -1098, -1098,  -172,   843, -1098,  -155,   446,   198,  -490,  -911,
    -912, -1098,  -511,  -639, -1097, -1002,  -847,   -27, -1098,   108,
   -1098,  -273,  -475,  -508,   507,  -516, -1098, -1098, -1098,  1333,
   -1098,   -13, -1098, -1098,  -246, -1098,  -716, -1098, -1098, -1098,
    1527,  1706,   -12, -1098,     7,   575, -1098,  2024,  2314, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,  -439
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   338,   109,   760,   111,   112,   113,   114,
     115,   358,   116,   117,   118,   339,   834,   582,   838,   835,
     119,   120,   579,   580,   121,   122,   192,   650,   282,   123,
     257,   124,   786,   287,   125,   999,   126,   300,   299,   127,
     128,   129,   130,   131,   132,   590,   133,   134,   900,   135,
     270,   136,   768,   769,   204,   138,   139,   140,   141,   549,
     803,   969,   142,   143,   799,   964,   144,   145,   146,   368,
     845,   147,   148,   373,   848,   374,   375,   205,   150,   151,
     152,   153,  1118,   154,   155,   653,   654,   500,   856,   280,
     515,   516,   517,   518,   519,   389,   379,   384,   854,  1185,
    1075,   469,  1064,   950,  1069,  1070,  1071,   156,   157,   391,
     392,   658,   158,   159,   217,   283,   284,   524,   525,   790,
     851,   604,   528,   787,  1201,  1061,   947,   340,   221,   344,
     345,   470,   471,   472,   206,   161,   162,   163,   164,   207,
     166,   189,   190,   716,   483,   917,   717,   718,   167,   168,
     208,   209,   171,   365,   473,   211,   173,   212,   213,   176,
     177,   178,   179,   350,   180,   181,   182,   183,   184
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     187,   444,   502,   393,   210,   108,   214,   652,   215,   777,
     720,   788,   514,   850,   222,  1000,   860,   861,   862,   487,
     489,   492,   263,   265,   267,   536,   271,   837,   766,   918,
     984,   983,   778,   363,   589,   597,  1065,   288,   800,   289,
     229,   303,  1076,   547,   294,   462,   295,   228,   296,   262,
     264,   266,   526,   274,  1073,   798,   304,   308,   310,   312,
     313,   314,   285,   602,   920,   318,   727,   319,   990,   322,
     326,  1074,   285,   355,   327,  1141,   309,   329,   330,   348,
     331,  1078,   332,   676,   507,   333,   334,   852,   335,   913,
     727,  1115,   304,   308,   349,   351,  1056,   285,   676,   948,
     677,   567,   571,   481,  1122,   508,  -254,   258,  1237,   509,
     356,   526,   346,   770,   678,  1204,    69,   361,   380,   364,
       3,   347,  -255,  -465,  1099,   464,   842,   526,  -278,   185,
     404,   529,   510,   843,   185,   511,  1066,    61,   410,   357,
     556,   863,  1202,   679,   465,  1142,   512,  1245,   481,   568,
     481,   961,   444,  -465,   771,   949,   397,  1066,   566,  1100,
    1000,  1014,  1198,   569,  1112,   513,  -465,   468,  1000,   381,
    -278,  -470,   706,   707,  -470,  -470,   456,   302,   569,  -465,
     382,   187,   680,  -237,   452,   453,   454,   569,   681,   318,
     304,   349,   728,   962,   458,   527,  -470,   680,   458,   383,
     458,  -470,   461,  1238,   259,  1182,   603,  1079,   458,   539,
    1143,   468,   569,   302,   343,   302,  1117,  1107,   655,   308,
     481,   231,  1073,  -470,  -278,   458,  -465,  -470,   530,  -367,
     909,  -465,  1199,   458,   468,  1068,   229,   275,   290,  1074,
    -488,  1205,   495,   466,   224,  1067,   569,   963,    69,   979,
    -470,   899,   468,  -471,   527,  1149,  1068,    23,   -53,   839,
     846,  -488,  -466,   308,   588,  -488,  -471,   188,   481,   953,
     527,  -470,   837,  1148,   573,   570,   -53,  1000,   445,  -470,
     446,  1181,  -561,  1150,   875,   945,   308,   562,  -488,  -624,
    -470,  -624,  -466,   541,   542,   -53,  -471,   291,   -52,  1175,
    1174,  1177,  1176,   574,   308,  -466,   720,   733,    59,   227,
     458,   302,   302,   559,   563,   292,   -52,   539,  -466,    67,
    1135,  1184,  -471,   971,   -53,   876,   458,  -480,   530,  -471,
     561,   185,   293,    85,   376,   -52,   540,   821,   560,   564,
     593,  -238,   558,   581,    87,   583,   682,   661,  -480,   779,
    -471,   877,  -480,   377,   191,   304,  1059,  1208,  1209,  1210,
    1211,  1212,  1213,   378,   -52,  -466,   565,  -244,   992,  -471,
    -466,   539,  1120,  1121,  -471,  -480,   780,  -471,   878,  1015,
     272,   468,   468,   273,   662,   216,   663,   664,   665,   667,
     669,   670,   738,   671,   672,   673,   674,   675,   911,   683,
     684,   685,   686,   687,   688,   689,   690,   691,   692,   693,
     694,   695,   696,   697,   698,   699,   666,   668,   841,   940,
    1097,    42,   308,   308,  -307,  -243,   823,  1027,   308,   941,
    1062,   308,   308,   308,   308,   709,   711,   719,   720,   468,
    1159,   701,   702,  -307,   779,   824,    57,   703,   731,   740,
     704,   705,   734,  -307,   738,   735,   942,  1036,   321,   325,
     827,   530,   708,   710,   304,   779,   742,   744,   599,   738,
    -575,   951,   748,   750,  -574,   730,   302,   652,   754,   828,
     905,  1024,   468,   738,  1037,   761,  -458,  1116,   763,   218,
    -460,   308,  1057,   600,   743,   400,  1110,  -575,  -298,   789,
     749,  -574,   231,   922,   925,   753,   514,   927,   929,   160,
    1131,   985,   400,  -625,   385,  -625,  -458,   791,  -435,   160,
    -460,   793,   794,  -298,  1063,   530,  -624,   797,  -624,  -458,
    -105,   676,   160,  -460,   308,   160,  1171,  -435,  -111,   537,
     538,   837,  -458,   837,  1109,  -107,  -460,   797,   677,   275,
     308,   401,   308,  -112,  -252,   402,   813,   797,   816,  -242,
     818,   820,   678,  1160,  -239,   386,   972,   974,   401,   346,
     279,   346,   402,   836,   836,  -246,   387,   172,   998,  -250,
    1151,   558,  -247,   812,   160,   302,   847,   172,   514,  -458,
     297,   679,   447,  -460,  -458,   388,   448,   720,  -460,   298,
     172,  1183,   404,   172,  -241,   315,  1152,   408,  1153,  1154,
     410,   160,  1155,  1156,   413,  -245,   160,   892,   316,   404,
     405,   448,   406,   407,   408,  -251,   409,   410,  1203,  -253,
     680,   413,  -621,   458,  -621,   504,   681,   857,   419,   420,
    -240,   400,   423,  -248,   354,   424,   425,   426,   796,  -623,
     366,  -623,   172,   404,   367,  -558,   323,   323,   408,  -557,
     394,   410,   468,   395,  -622,   413,  -622,   865,   796,   866,
     807,   343,   809,   343,   468,   396,  -562,  -563,   796,   172,
    -560,  -559,   449,   457,   172,   451,   991,   458,  1011,  1203,
     356,   463,   467,   259,   474,   476,   160,   401,   543,  1239,
     575,   402,   503,   533,   534,   584,   546,   594,   601,   742,
     895,   748,   898,   761,   308,   901,   816,   903,   904,   595,
     598,   605,   657,   998,   308,   659,   308,   910,  1203,   912,
     410,   660,   700,   719,   726,   919,   736,   894,   737,   897,
     738,   739,   745,   908,   930,   931,   756,   757,   404,   405,
     759,   406,   407,   408,  -475,   409,   410,   762,   955,   764,
     413,   978,   682,   765,   172,   773,   789,  -475,   420,   774,
     802,   314,   318,   349,   424,   425,   426,   308,   811,   468,
     795,   507,   801,   840,   539,   160,   853,   817,   855,  -108,
    -132,  -139,   160,  -138,  -137,  -134,   957,  -475,   400,  -141,
    -106,  -142,   508,  -136,  -140,  -109,   509,  1161,  1162,  -135,
    -143,   581,   858,  -110,   859,   980,   902,   982,   836,   836,
     932,   873,  1004,  -475,   867,   874,   934,   946,   960,   510,
    -475,   308,   511,   676,   352,   868,   869,   349,   468,   870,
     998,   871,   872,   512,   -75,   907,   160,   879,   965,   985,
     880,  -475,   986,   172,   401,   881,   989,  1012,   402,   988,
     172,   160,   513,  1013,  1019,   719,   195,   993,  1022,  1051,
    -475,  1052,  1025,  1053,  1077,  -475,   882,  1086,  -475,   667,
     709,  1018,  1080,   883,  1021,  1081,  1091,  1095,  1103,  1104,
     308,  1114,  1119,   884,   302,  -105,   656,  -107,  1139,  1165,
     885,  1033,  1180,  1035,  1216,   404,   666,   708,  1040,   507,
     408,  1043,   409,   410,   172,  1217,   886,   413,  1150,  1221,
    1240,   137,  1241,  1049,  1222,   420,  1223,  1224,   887,   172,
     508,   137,   680,   426,   509,  1084,   977,  1072,   219,   888,
    1113,   328,   501,   578,   137,   889,  1054,   137,   551,  1085,
     304,  1001,  1010,   591,  1006,   944,   772,   510,   302,  1090,
     511,  1094,    61,  1214,  1096,   160,   520,   836,  1186,  1005,
    1207,   512,   360,  1105,   864,  1106,   792,   952,     0,     0,
       0,     0,     0,     0,     0,   160,     0,     0,     0,   149,
     513,   160,     0,     0,     0,     0,   137,     0,     0,   149,
       0,     0,   160,     0,   160,     0,     0,  1033,  1035,     0,
    1040,  1043,   149,  1090,  1094,   149,   468,   468,     0,  1132,
    1133,     0,  1134,   137,   719,     0,  1136,  1137,   137,     0,
    1138,  1145,     0,   172,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   349,     0,     0,     0,
       0,  1147,     0,   172,     0,  1060,     0,     0,     0,   172,
       0,     0,     0,     0,   149,     0,  1158,   816,   308,   308,
     172,   302,   172,   160,  1166,     0,  1167,   160,     0,     0,
    1169,     0,     0,     0,     0,   160,  1173,   836,   836,   836,
     836,   149,     0,     0,     0,     0,   149,     0,     0,     0,
       0,     0,     0,     0,     0,   816,     0,     0,   137,     0,
       0,  1132,  1187,  1188,  1136,  1189,  1190,  1191,  1192,     0,
       0,  1111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   224,     0,  1206,     0,    22,
      23,   172,     0,     0,     0,   172,     0,     0,     0,   225,
       0,    31,   226,   172,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,    42,     0,  1225,   302,     0,     0,
       0,     0,  1146,     0,     0,     0,   149,     0,     0,     0,
       0,  1232,  1233,  1234,  1235,     0,     0,     0,     0,    57,
       0,    59,  1236,   816,   400,   995,     0,   137,   996,     0,
     227,     0,    67,  1094,   137,     0,     0,  1242,  1243,   844,
       0,     0,     0,     0,  1232,  1233,  1234,  1235,  1242,  1243,
       0,    83,     0,     0,    85,   890,     0,    87,     0,     0,
    -291,     0,  1094,     0,  -291,  -291,   400,     0,     0,  -291,
       0,     0,     0,     0,  -291,     0,  -291,  -291,     0,     0,
     401,     0,  -291,  1200,   402,     0,     0,     0,   137,  -291,
       0,     0,  -291,     0,     0,   149,   160,     0,     0,   160,
       0,     0,   149,   137,     0,     0,     0,   102,     0,     0,
       0,  -291,     0,     0,  -291,     0,  -291,     0,  -291,     0,
    -291,  -291,   401,  -291,     0,  -291,   402,  -291,     0,     0,
       0,   404,   405,     0,     0,   407,   408,     0,   409,   410,
       0,     0,     0,   413,  1200,     0,  -291,     0,   160,  -291,
       0,   420,  -291,     0,   160,     0,   149,   424,   425,   426,
       0,     0,     0,     0,   172,   165,     0,   172,     0,     0,
       0,   149,     0,   404,   405,   165,   406,   407,   408,     0,
     409,   410,   411,  1200,     0,   413,     0,     0,   165,     0,
       0,   165,   419,   420,     0,     0,   423,     0,     0,   424,
     425,   426,  -291,     0,     0,     0,     0,   137,  -291,     0,
     427,     0,     0,     0,     0,     0,   172,     0,     0,     0,
       0,     0,   172,     0,     0,     0,     0,   137,  1016,  1017,
       0,     0,     0,   137,     0,     0,     0,     0,     0,   232,
     165,     0,     0,     0,   137,     0,   137,     0,     0,     0,
     160,     0,   160,   233,   234,     0,   235,   160,     0,     0,
     160,   236,     0,     0,     0,     0,     0,   165,     0,   160,
     237,   160,   165,     0,   160,   149,   238,     0,     0,     0,
       0,     0,   239,     0,     0,     0,   240,   400,     0,   241,
       0,     0,     0,     0,     0,   149,     0,     0,   160,   242,
       0,   149,     0,     0,     0,   243,   244,     0,   160,     0,
     160,     0,   149,   245,   149,   137,     0,     0,   172,   137,
     172,     0,   246,     0,     0,   172,     0,   137,   172,     0,
     400,   247,   248,     0,   249,     0,   250,   172,   251,   172,
       0,   252,   172,   401,     0,   253,   521,   402,   254,     0,
       0,   255,   165,     0,     0,     0,     0,  -477,     0,   169,
       0,     0,     0,     0,     0,     0,   172,     0,   160,   169,
    -477,     0,     0,     0,     0,   160,   172,     0,   172,     0,
       0,     0,   169,   149,     0,   169,   401,   149,   160,     0,
     402,     0,     0,     0,   404,   149,     0,     0,     0,   408,
    -477,   409,   410,     0,     0,     0,   413,   522,     0,     0,
       0,     0,     0,     0,   420,     0,     0,     0,     0,   160,
     424,   425,   426,     0,     0,     0,  -477,     0,   160,     0,
       0,     0,   160,  -477,   169,     0,   172,   404,   405,     0,
       0,   165,   408,   172,   409,   410,     0,     0,   165,   413,
       0,     0,     0,     0,  -477,     0,   172,   420,     0,     0,
       0,   169,     0,   424,   425,   426,   169,     0,     0,     0,
       0,     0,     0,  -477,     0,   232,   160,     0,  -477,     0,
       0,  -477,     0,     0,     0,     0,     0,   172,     0,   233,
     234,     0,   235,     0,     0,     0,   172,   236,     0,     0,
     172,     0,   165,     0,     0,     0,   237,     0,   137,     0,
       0,   137,   238,     0,     0,     0,     0,   165,   239,     0,
       0,     0,   240,     0,     0,   241,     0,     0,     0,     0,
     160,   160,   160,   160,     0,   242,     0,     0,   170,     0,
       0,   243,   244,     0,   172,     0,   169,     0,   170,   245,
       0,     0,     0,     0,     0,     0,   160,   160,   246,     0,
     137,   170,     0,     0,   170,     0,   137,   247,   248,     0,
     249,     0,   250,     0,   251,     0,   149,   252,     0,   149,
       0,   253,     0,     0,   254,     0,     0,   255,     0,     0,
     224,     0,     0,     0,    22,    23,     0,     0,   172,   172,
     172,   172,     0,     0,   225,     0,    31,   226,     0,     0,
       0,     0,    37,   170,     0,     0,     0,     0,     0,    42,
       0,   165,     0,     0,   172,   172,     0,     0,   149,     0,
       0,     0,     0,     0,   149,   169,     0,     0,     0,     0,
     170,   165,   169,   281,    57,   170,    59,   165,     0,     0,
       0,     0,     0,     0,     0,   227,     0,    67,   165,     0,
     165,     0,   137,     0,   137,     0,     0,     0,     0,   137,
       0,     0,   137,     0,     0,     0,    83,     0,     0,    85,
       0,   137,    87,   137,     0,     0,   137,     0,     0,     0,
     224,     0,     0,     0,    22,    23,   169,     0,     0,   994,
       0,     0,     0,     0,   225,     0,    31,   226,     0,     0,
     137,   169,    37,     0,     0,     0,     0,     0,     0,    42,
     137,     0,   137,     0,     0,   170,     0,     0,     0,   165,
     149,     0,   149,   165,     0,     0,     0,   149,     0,     0,
     149,   165,     0,     0,    57,     0,    59,     0,    61,   149,
     995,   149,     0,   996,   149,   227,     0,    67,   428,   429,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,     0,     0,     0,    83,     0,   149,    85,
     137,     0,    87,     0,     0,     0,     0,   137,   149,  -582,
     149,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     137,     0,   443,     0,     0,  -621,     0,  -621,     0,     0,
       0,     0,     0,     0,   170,   169,     0,     0,     0,     0,
       0,   170,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   137,   102,     0,     0,   169,     0,     0,   997,     0,
     137,   169,     0,     0,   137,     0,     0,     0,   149,     0,
       0,     0,   169,     0,   169,   149,   174,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   174,     0,   149,     0,
       0,     0,     0,     0,     0,   170,     0,     0,     0,   174,
       0,     0,   174,   223,     0,     0,     0,     0,   137,     0,
     170,     0,     0,     0,     0,     0,     0,   224,     0,   149,
       0,    22,    23,     0,     0,     0,     0,     0,   149,     0,
       0,   225,   149,    31,   226,     0,     0,     0,     0,    37,
       0,     0,   165,   169,     0,   165,    42,   169,     0,     0,
       0,   174,     0,     0,     0,   169,     0,     0,     0,     0,
     -51,     0,   137,   137,   137,   137,     0,     0,     0,     0,
       0,    57,     0,    59,     0,     0,   149,    63,   174,   -51,
      64,     0,   227,   174,    67,     0,     0,     0,   137,   137,
       0,     0,     0,     0,   165,     0,     0,     0,     0,     0,
     165,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,   170,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     149,   149,   149,   149,   170,     0,     0,     0,     0,     0,
     170,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   170,     0,   170,     0,     0,   149,   149,   232,   102,
       0,     0,     0,   174,     0,     0,     0,     0,     0,     0,
       0,     0,   233,   234,     0,   235,     0,     0,     0,     0,
     236,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,   238,   165,     0,   165,     0,
       0,   239,     0,   165,     0,   240,   165,     0,   241,     0,
       0,     0,     0,     0,     0,   165,     0,   165,   242,     0,
     165,     0,   170,     0,   243,   244,   170,     0,     0,     0,
       0,     0,   245,     0,   170,     0,   169,     0,     0,   169,
       0,   246,     0,     0,   165,     0,     0,     0,     0,     0,
     247,   248,   174,   249,   165,   250,   165,   251,     0,   174,
     252,     0,     0,     0,   253,     0,   175,   254,     0,     0,
     255,     0,     0,     0,     0,     0,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   169,   175,
     311,     0,   175,   224,   169,     0,     0,    22,    23,     0,
       0,   110,   994,     0,     0,     0,     0,   225,     0,    31,
     226,   186,     0,   174,   165,    37,     0,     0,     0,     0,
       0,   165,    42,     0,   220,     0,   775,   230,   174,     0,
       0,     0,     0,     0,   165,     0,     0,     0,     0,     0,
       0,   175,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,   995,     0,     0,   996,     0,   227,     0,
      67,     0,     0,     0,     0,   165,     0,     0,   175,     0,
       0,     0,     0,   175,   165,     0,   317,     0,   165,    83,
       0,     0,    85,     0,     0,    87,     0,     0,     0,     0,
     169,     0,   169,     0,     0,     0,     0,   169,     0,     0,
     169,     0,     0,   110,     0,     0,     0,     0,   353,   169,
       0,   169,     0,     0,   169,   170,     0,     0,   170,     0,
       0,     0,   165,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   174,     0,     0,   102,     0,     0,   169,     0,
       0,  1108,     0,     0,     0,     0,     0,     0,   169,     0,
     169,     0,   174,   175,     0,     0,     0,     0,   174,     0,
       0,     0,     0,     0,     0,     0,     0,   170,     0,   174,
       0,   174,     0,   170,     0,     0,   165,   165,   165,   165,
       0,   477,   480,   482,   486,   488,   491,     0,   450,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   165,   165,     0,     0,     0,   532,   169,     0,
       0,     0,     0,   535,     0,   169,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   169,     0,
       0,   545,     0,     0,     0,     0,     0,     0,     0,     0,
     174,     0,   175,     0,   174,     0,     0,   553,   554,   175,
       0,     0,   174,   286,     0,     0,     0,     0,     0,   169,
       0,     0,     0,     0,     0,     0,     0,     0,   169,   170,
       0,   170,   169,     0,     0,   286,   170,   506,     0,   170,
       0,     0,     0,     0,   531,   320,   324,     0,   170,     0,
     170,     0,   224,   170,     0,     0,    22,    23,     0,     0,
       0,   994,     0,   175,     0,     0,   225,     0,    31,   226,
       0,     0,     0,     0,    37,     0,   169,   170,   175,     0,
       0,    42,     0,     0,     0,     0,     0,   170,     0,   170,
       0,     0,     0,     0,     0,     0,     0,     0,   110,     0,
       0,     0,     0,     0,     0,     0,    57,     0,    59,     0,
      61,     0,   995,   110,     0,   996,     0,   227,     0,    67,
     224,     0,     0,     0,    22,    23,     0,     0,     0,     0,
     169,   169,   169,   169,   225,     0,    31,   226,    83,     0,
     732,    85,    37,     0,    87,     0,     0,   170,     0,    42,
       0,     0,     0,     0,   170,     0,   169,   169,     0,     0,
       0,     0,   746,     0,     0,     0,   751,   170,   752,     0,
       0,   755,     0,     0,    57,     0,    59,     0,   459,     0,
     995,     0,   175,   996,     0,   227,     0,    67,     0,     0,
       0,     0,     0,   174,   102,     0,   174,     0,   170,     0,
    1179,     0,   175,     0,     0,     0,    83,   170,   175,    85,
       0,   170,    87,     0,     0,     0,     0,   110,     0,   175,
       0,   175,     0,     0,     0,     0,   286,   286,   286,   286,
     286,   286,     0,     0,   496,   499,     0,   531,     0,     0,
       0,   505,     0,   531,     0,   174,     0,     0,     0,     0,
       0,   174,   286,     0,   758,   170,     0,   256,   286,     0,
       0,     0,   102,     0,     0,     0,     0,     0,     0,     0,
       0,   276,   277,   278,     0,     0,   286,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     175,     0,   286,   286,   175,     0,     0,     0,     0,     0,
       0,     0,   175,     0,     0,     0,     0,     0,     0,   170,
     170,   170,   170,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   810,   224,     0,     0,   815,
      22,    23,     0,     0,     0,   170,   170,   110,     0,     0,
     225,     0,    31,   226,     0,     0,     0,   174,    37,   174,
       0,     0,     0,     0,   174,    42,     0,   174,     0,     0,
       0,     0,     0,     0,     0,     0,   174,     0,   174,   -51,
       0,   174,     0,   369,     0,   372,     0,     0,     0,     0,
      57,     0,    59,     0,   390,     0,    63,     0,   -51,    64,
       0,   227,     0,    67,     0,   174,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   174,     0,   174,     0,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,   914,   916,     0,   286,   455,     0,   921,   924,
       0,     0,   926,   928,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   286,     0,     0,
       0,   286,   256,   286,     0,     0,   286,     0,     0,     0,
       0,     0,     0,     0,     0,   174,     0,     0,   102,     0,
       0,     0,   174,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   175,     0,   174,   175,   475,     0,     0,
       0,     0,     0,     0,     0,   493,   494,     0,   224,     0,
       0,     0,    22,    23,     0,     0,     0,   994,   523,     0,
       0,     0,   225,     0,    31,   226,   174,     0,   933,     0,
      37,   936,     0,     0,     0,   174,     0,    42,     0,   174,
       0,     0,     0,     0,     0,   175,     0,     0,     0,     0,
       0,   175,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,   352,     0,   995,     0,
       0,   996,     0,   227,     0,    67,     0,     0,     0,     0,
     970,     0,     0,   174,     0,     0,   975,     0,     0,     0,
       0,     0,  1028,  1029,    83,     0,     0,    85,     0,  1030,
      87,     0,     0,   592,     0,     0,     0,     0,   372,     0,
       0,  1041,     0,     0,  1044,     0,  1045,     0,  1046,     0,
       0,     0,     0,     0,     0,     0,   651,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   174,   174,   174,
     174,     0,     0,     0,     0,     0,     0,   175,     0,   175,
     102,     0,     0,     0,   175,     0,     0,   175,     0,     0,
       0,     0,     0,   174,   174,     0,   175,     0,   175,     0,
       0,   175,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,   531,     0,   531,     0,     0,     0,     0,   531,
       0,     0,   531,     0,   729,   175,     0,     0,     0,     0,
       0,  1047,     0,  1048,     0,   175,  1050,   175,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   286,   286,     0,
       0,     0,     0,   286,   286,     0,     0,   286,   286,     0,
    1083,     0,     0,     0,     0,   401,     0,   938,     0,   402,
    1089,   767,  1093,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   776,     0,     0,     0,   523,
       0,     0,     0,     0,     0,   175,     0,     0,     0,     0,
     403,     0,   175,  1164,     0,     0,   967,     0,     0,     0,
       0,     0,     0,     0,     0,   175,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,     0,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   175,     0,     0,     0,
    1140,   832,     0,   427,     0,   175,     0,     0,     0,   175,
       0,     0,     0,   372,     0,     0,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,  1163,     0,     0,     0,     0,     0,     0,     0,     0,
    1168,     0,     0,     0,  1170,     0,     0,     0,     0,     0,
       0,     0,     0,   175,     0,     0,     0,   286,   286,     0,
     443,     0,     0,  -621,   286,  -621,   499,     0,     0,     0,
       0,     0,     0,   499,     0,     0,   286,   390,     0,   286,
       0,   286,     0,   286,     0,     0,     0,     0,  1197,     0,
       0,     0,     0,     0,     0,     0,  1055,     0,     0,     0,
       0,     0,     0,   606,     0,   607,     0,   175,   175,   175,
     175,     0,     0,     0,   608,     0,     0,     0,     0,   609,
     234,   610,   611,  1087,     0,     0,     0,   612,     0,     0,
       0,     0,     0,   175,   175,     0,   237,     0,   195,     0,
       0,     0,   613,     0,     0,     0,     0,     0,   614,     0,
       0,     0,   240,     0,     0,   615,     0,   616,     0,     0,
       0,     0,     0,     0,     0,   617,     0,     0,     0,     0,
       0,   618,   619,     0,     0,     0,     0,     0,     0,   245,
       0,   939,     0,     0,     0,     0,     0,     0,   620,   943,
       0,     0,   523,     0,     0,     0,   523,   247,   248,     0,
     621,     0,   250,     0,   622,     0,     0,   623,     0,     0,
       0,   624,     0,     0,   254,     0,     0,   625,     0,   968,
       0,     0,     0,     0,     0,     0,   498,   429,   430,   431,
     432,   433,     0,     0,   436,   437,   438,   439,   286,   441,
     442,   626,   627,   628,   629,   630,   631,     0,   632,     0,
       0,     0,   633,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,     0,   644,   372,     0,   645,   646,   647,
     648,     0,     0,   649,     0,     0,   651,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   499,   499,     0,
       0,   499,   499,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1218,
       0,   499,     0,   499,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    -2,     4,     0,     5,
       0,     6,     7,     8,     9,    10,    11,     0,   767,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,   968,    31,    32,    33,    34,    35,    36,
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
     102,     0,     0,   103,     0,   104,     0,   105,     0,   106,
       0,     4,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,  -652,     0,    12,    13,    14,    15,    16,  -652,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,  -652,    28,    29,  -652,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -51,     0,    40,
      41,    42,     0,    43,  -307,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -307,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -307,   -51,    64,    65,    66,  -652,    67,
      68,    69,  -652,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -51,    87,    88,     0,     0,    89,     0,
      90,     0,     0,  -652,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -652,
    -652,    94,  -652,  -652,  -652,  -652,  -652,  -652,  -652,     0,
    -652,  -652,  -652,  -652,  -652,     0,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,  -652,   102,  -652,  -652,  -652,     0,   104,
    -652,   105,     0,   106,     0,   336,  -652,     5,   301,     6,
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
       0,   103,     0,   104,   337,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
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
     100,   101,   102,     0,     0,   103,     0,   104,   557,   105,
       0,   106,     0,   576,   107,     5,     0,     6,     7,     8,
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
       0,   104,   577,   105,     0,   106,     0,   336,   107,     5,
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
     102,     0,     0,   103,     0,   104,   337,   105,     0,   106,
       0,     4,   107,     5,     0,     6,     7,     8,     9,    10,
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
     822,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -51,     0,    40,    41,    42,     0,    43,  -307,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -307,     0,     0,
      57,    58,    59,    60,   352,    62,    63,  -307,   -51,    64,
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
      35,    36,    37,    38,    39,   -51,     0,    40,    41,    42,
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
      31,    32,    33,    34,    35,    36,    37,   741,    39,   -51,
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
      37,   747,    39,   -51,     0,    40,    41,    42,     0,    43,
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
      33,    34,    35,    36,    37,  1032,    39,   -51,     0,    40,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1034,
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
      35,    36,    37,  1039,    39,   -51,     0,    40,    41,    42,
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
      31,    32,    33,    34,    35,    36,    37,  1042,    39,   -51,
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
      20,    21,    22,    23,    24,    25,    26,    27,     0,  1082,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -51,     0,    40,    41,    42,     0,    43,
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
      33,    34,    35,    36,    37,  1088,    39,   -51,     0,    40,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1092,
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
       0,   103,     0,   104,     0,   105,     0,   106,     0,  1002,
     107,     5,   301,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,  1003,   954,   107,     5,   301,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     0,   107,     5,
     301,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,   305,   306,     0,    84,   341,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,   342,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,   359,    23,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,    59,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,    67,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,    87,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     0,   107,     5,   301,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,   305,   306,
       0,    84,   341,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,   806,   203,     0,     0,   107,     5,
     301,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,   305,   306,     0,    84,   341,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,   808,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,   712,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,   713,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,   714,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,   715,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,   305,   306,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   307,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     0,   107,     5,   301,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
     305,   306,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,   956,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,   305,   306,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,   260,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,   261,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,   268,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,   269,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     0,   107,     5,   301,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,   261,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,   305,   306,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   203,
       0,     0,   107,     5,   301,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
     804,   105,     0,   203,     0,     0,   107,     5,   301,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,   814,     0,
     107,     5,   301,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,  1144,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,   261,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,   285,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   104,     0,   105,     0,   203,     0,     0,   107,     5,
     301,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   203,
       0,     5,   107,     6,     7,     8,   362,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
      18,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,    66,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,    85,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,   819,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,   981,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     0,   107,     5,
     301,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
     781,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,   782,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,   783,    74,    75,    76,   784,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,  1058,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,  1172,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     0,   107,     5,   301,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,  1058,     0,     0,   107,     5,
     301,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,  1130,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   104,
       0,   105,     0,   203,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,   781,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,   782,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,   783,    74,
      75,    76,   784,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   785,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   833,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
     781,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,   782,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,   783,    74,    75,    76,   784,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   849,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   785,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   893,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   896,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,  1124,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,  1125,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,  1127,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   203,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,  1128,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,  1129,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,  1130,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   201,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,     0,     0,   105,     0,   849,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,     0,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       5,   203,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,  -461,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,  -461,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,  -461,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,  -461,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       5,   301,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
     101,  -461,    21,     0,     0,     0,  -461,     0,   105,     0,
     849,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,  -479,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,     0,  -479,     0,     0,     0,  -479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,   202,     0,   105,  -479,
    1058,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,    94,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,   101,     0,    21,     0,     0,     0,   202,     0,   105,
       0,   203,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    94,
       0,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,   101,     0,    21,     0,     0,     0,   202,     0,   105,
       0,   785,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,   232,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,   233,   234,    80,   235,     0,     0,    84,     0,   236,
      86,     0,     0,    88,     0,     0,     0,     0,   237,     0,
       0,     0,     0,     0,   238,     0,     0,     0,     0,     0,
     239,     0,     0,     0,   240,     0,     0,   241,     0,    94,
       0,     0,     0,     0,     0,     0,     0,   242,     0,     0,
       0,     0,     0,   243,   244,     0,     0,     0,     0,     0,
       0,   245,     0,     0,     0,     0,     0,     0,     0,   105,
     246,   203,     0,     0,     0,     0,     0,     0,     0,   247,
     248,     0,   249,     0,   250,     0,   251,     0,     0,   252,
       0,  1008,     0,   253,     0,     0,   254,     0,     0,   255,
     608,     0,     0,     0,     0,   233,   234,   610,   235,     0,
       0,     0,     0,   236,     0,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,   613,     0,
       0,     0,     0,     0,   239,     0,     0,     0,   240,     0,
       0,   241,     0,   616,     0,     0,     0,     0,     0,     0,
       0,   242,     0,   458,     0,   966,     0,   618,   244,     0,
       0,     0,     0,     0,     0,   245,     0,     0,     0,     0,
       0,     0,     0,     0,   246,     0,     0,     0,     0,     0,
       0,     0,   398,   247,   248,     0,   249,     0,   250,   399,
    1009,     0,     0,   623,     0,     0,     0,   253,     0,     0,
     254,     0,   400,   255,     0,     0,     0,     0,     0,     0,
       0,     0,   498,   429,   430,   431,   432,   433,     0,     0,
     436,   437,   438,   439,     0,   441,   442,   626,   627,   628,
     629,   630,   631,     0,   632,     0,     0,     0,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,     0,
     644,     0,     0,   645,   646,   647,   648,     0,   401,     0,
       0,     0,   402,     0,     0,   398,     0,     0,     0,     0,
       0,   497,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,   403,   498,   429,   430,   431,   432,   433,
       0,     0,   436,   437,   438,   439,     0,   441,   442,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   458,
       0,   401,     0,     0,     0,   402,   427,     0,   398,     0,
       0,     0,     0,     0,  1031,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,   403,   498,   429,   430,
     431,   432,   433,     0,     0,   436,   437,   438,   439,     0,
     441,   442,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   458,     0,   401,     0,     0,     0,   402,   427,
       0,   398,     0,     0,     0,     0,     0,  1038,   399,     0,
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
     402,   427,     0,   398,     0,     0,     0,     0,     0,  1196,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,   403,   498,   429,   430,   431,   432,   433,     0,     0,
     436,   437,   438,   439,     0,   441,   442,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,   458,     0,   401,
       0,     0,     0,   402,   427,     0,   398,     0,     0,     0,
       0,     0,  1219,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,   403,   498,   429,   430,   431,   432,
     433,     0,     0,   436,   437,   438,   439,     0,   441,   442,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   424,   425,   426,     0,
     458,     0,   401,     0,     0,     0,   402,   427,     0,   398,
       0,     0,     0,     0,     0,  1220,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     484,     0,     0,     0,     0,     0,     0,   403,   498,   429,
     430,   431,   432,   433,     0,   485,   436,   437,   438,   439,
       0,   441,   442,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,     0,   458,     0,   401,     0,     0,     0,   402,
     427,   398,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,   285,     0,     0,     0,   481,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,   490,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   458,   401,     0,     0,
       0,   402,     0,   427,   398,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   478,     0,     0,   481,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
     479,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,   458,     0,
     401,     0,     0,     0,   402,   427,     0,   398,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,   585,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   586,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,   458,     0,   401,     0,     0,     0,   402,   427,     0,
     398,   829,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,   830,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     831,   417,   418,   419,   420,   421,   422,   423,     0,   587,
     424,   425,   426,     0,     0,     0,   401,     0,     0,     0,
     402,   427,   398,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,   923,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,     0,   401,     0,
       0,     0,   402,     0,   427,   398,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,   285,     0,     0,   481,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   458,
       0,   401,     0,     0,     0,   402,   427,   398,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,   481,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,   460,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   400,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   458,   401,     0,     0,     0,   402,   398,   427,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,   285,
       0,     0,     0,     0,   188,     0,     0,     0,   403,     0,
       0,     0,     0,     0,   401,     0,     0,     0,   402,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,   403,
     424,   425,   426,     0,   401,     0,     0,     0,   402,     0,
     398,   427,     0,     0,     0,   404,   405,   399,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     400,   721,   417,   418,   419,   420,   421,     0,   423,   403,
       0,   424,   425,   426,     0,     0,   722,     0,     0,     0,
       0,     0,   427,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   458,   401,     0,     0,     0,
     402,   398,   427,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,   723,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,   724,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,   401,     0,     0,
       0,   402,   398,     0,   427,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,   915,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,   401,     0,
       0,     0,   402,     0,   398,   427,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   458,
     401,     0,     0,     0,   402,     0,   427,   398,     0,     0,
       0,     0,     0,   937,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,   458,     0,   401,     0,     0,     0,   402,   427,     0,
     398,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,   481,  1101,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
    1102,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,     0,     0,   401,     0,     0,   973,
     402,   427,   398,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,     0,   401,     0,
       0,     0,   402,     0,   427,   398,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   544,
       0,   401,     0,     0,     0,   402,   427,   398,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,   548,   424,   425,
     426,     0,     0,   401,     0,     0,     0,   402,   398,   427,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,   550,
     424,   425,   426,     0,   401,     0,     0,     0,   402,   398,
       0,   427,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
     552,   424,   425,   426,     0,   401,     0,     0,     0,   402,
     398,     0,   427,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,   555,   424,   425,   426,     0,   401,     0,     0,     0,
     402,   398,     0,   427,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
     572,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,   401,     0,     0,
       0,   402,   398,     0,   427,     0,     0,     0,     0,   399,
     725,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,   401,     0,
       0,     0,   402,   398,   825,   427,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   401,
       0,     0,   805,   402,   398,     0,   427,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   826,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   424,   425,   426,     0,
     401,     0,     0,     0,   402,   398,     0,   427,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,  -583,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,   401,     0,     0,     0,   402,   398,     0,   427,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,   188,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   401,     0,     0,     0,   402,   398,     0,   427,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
     935,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,   906,   424,
     425,   426,     0,   401,     0,     0,     0,   402,   398,     0,
     427,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   403,     0,
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
       0,   424,   425,   426,     0,   401,     0,     0,   958,   402,
     398,     0,   427,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   401,     0,     0,   959,
     402,   398,   987,   427,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
     976,   403,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,   402,   398,     0,   427,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,  1020,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   401,
       0,     0,  1007,   402,   398,     0,   427,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,  1023,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   424,   425,   426,     0,
     401,     0,     0,     0,   402,   398,     0,   427,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,   401,     0,     0,     0,   402,   398,  1098,   427,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   401,     0,     0,     0,   402,  1026,   398,   427,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,     0,     0,   401,     0,     0,     0,   402,   398,
     427,     0,     0,     0,     0,     0,   399,  1123,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   401,     0,     0,     0,   402,
       0,     0,   427,     0,     0,     0,     0,     0,  1126,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,  1157,   417,   418,   419,   420,   421,   422,   423,
     398,  1178,   424,   425,   426,     0,     0,   399,     0,     0,
       0,     0,     0,   427,     0,     0,     0,     0,     0,   401,
     400,     0,     0,   402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,     0,   406,   407,   408,   401,   409,   410,   411,
     402,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   424,   425,   426,     0,
       0,     0,   400,     0,     0,     0,     0,   427,     0,     0,
       0,   403,     0,     0,     0,     0,   401,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,   403,     0,   424,   425,   426,     0,     0,   401,     0,
       0,     0,   402,     0,   427,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,   398,     0,   424,   425,   426,     0,     0,   399,     0,
       0,     0,  1215,     0,   427,     0,     0,     0,     0,   404,
     405,   400,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,     0,     0,   417,   418,   419,   420,
     421,   398,   423,     0,     0,   424,   425,   426,   399,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,     0,     0,
    1226,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,   401,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,   404,   405,
    1227,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,   403,     0,   424,   425,   426,     0,     0,   401,
       0,     0,     0,   402,     0,   427,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,   398,     0,   424,   425,   426,     0,     0,   399,
       0,     0,     0,     0,     0,   427,     0,     0,     0,     0,
     404,   405,   400,   406,   407,   408,     0,   409,   410,   411,
       0,     0,   413,   414,   415,     0,     0,   417,   418,   419,
     420,     0,   398,   423,     0,     0,   424,   425,   426,   399,
       0,     0,     0,     0,     0,     0,     0,   427,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   401,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,  1228,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,   401,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,   404,
     405,  1229,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,   403,     0,   424,   425,   426,     0,     0,
       0,     0,     0,     0,     0,     0,   427,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,   398,   232,   424,   425,   426,     0,   596,
     399,     0,     0,     0,     0,     0,   427,     0,   233,   234,
       0,   235,     0,   400,     0,     0,   236,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,     0,     0,     0,
       0,   238,     0,   398,     0,     0,     0,   239,     0,     0,
     399,   240,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,     0,   400,   242,     0,     0,     0,     0,     0,
     243,   244,     0,     0,     0,     0,     0,     0,   245,   401,
       0,     0,     0,   402,   398,     0,     0,   246,     0,     0,
       0,   399,  1230,     0,     0,     0,   247,   248,     0,   249,
       0,   250,     0,   251,   400,     0,   252,     0,     0,     0,
     253,     0,     0,   254,   403,     0,   255,     0,     0,   401,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
     404,   405,  1231,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,   403,     0,   424,   425,   426,     0,
     401,     0,     0,     0,   402,     0,     0,   427,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,   403,   424,   425,   426,     0,
       0,   398,     0,     0,     0,     0,     0,   427,   399,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,   400,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,     0,   398,     0,     0,     0,  1244,     0,   427,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,   891,     0,
       0,     0,   402,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,   403,   424,   425,   426,     0,     0,     0,
       0,     0,     0,     0,     0,   427,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,   370,     0,
     232,     0,     0,     0,     0,     0,   427,     0,     0,     0,
       0,     0,     0,     0,   233,   234,     0,   235,     0,     0,
       0,     0,   236,     0,     0,     0,     0,     0,   371,     0,
       0,   237,     0,     0,   232,     0,     0,   238,     0,     0,
       0,     0,     0,   239,     0,     0,     0,   240,   233,   234,
     241,   235,     0,     0,     0,     0,   236,     0,     0,     0,
     242,     0,   371,     0,     0,   237,   243,   244,     0,     0,
       0,   238,     0,     0,   245,     0,     0,   239,     0,     0,
       0,   240,     0,   246,   241,     0,     0,     0,     0,     0,
       0,     0,   247,   248,   242,   249,     0,   250,     0,   251,
     243,   244,   252,     0,     0,     0,   253,     0,   245,   254,
       0,     0,   255,     0,     0,     0,     0,   246,     0,     0,
    -286,     0,     0,     0,     0,     0,   247,   248,     0,   249,
       0,   250,     0,   251,  -286,  -286,   252,  -286,     0,     0,
     253,     0,  -286,   254,     0,     0,   255,     0,     0,     0,
       0,  -286,     0,     0,   232,     0,     0,  -286,     0,     0,
       0,     0,     0,  -286,     0,     0,     0,  -286,   233,   234,
    -286,   235,     0,     0,     0,     0,   236,     0,     0,     0,
    -286,     0,     0,     0,     0,   237,  -286,  -286,     0,     0,
       0,   238,     0,     0,  -286,     0,     0,   239,     0,     0,
       0,   240,     0,  -286,   241,     0,     0,     0,     0,     0,
       0,     0,  -286,  -286,   242,  -286,     0,  -286,     0,  -286,
     243,   244,  -286,     0,     0,     0,  -286,     0,   245,  -286,
       0,     0,  -286,     0,     0,     0,     0,   246,     0,     0,
    -287,     0,     0,     0,     0,     0,   247,   248,     0,   249,
       0,   250,     0,   251,  -287,  -287,   252,  -287,     0,     0,
     253,     0,  -287,   254,     0,     0,   255,     0,     0,     0,
       0,  -287,     0,     0,     0,     0,     0,  -287,     0,     0,
       0,     0,     0,  -287,     0,     0,     0,  -287,     0,     0,
    -287,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -287,     0,     0,     0,     0,     0,  -287,  -287,     0,     0,
       0,     0,     0,     0,  -287,     0,     0,     0,     0,     0,
       0,     0,     0,  -287,     0,     0,     0,     0,     0,     0,
       0,     0,  -287,  -287,     0,  -287,     0,  -287,     0,  -287,
       0,     0,  -287,     0,     0,     0,  -287,     0,     0,  -287,
       0,     0,  -287
};

static const yytype_int16 yycheck[] =
{
      12,   172,   275,   158,    16,     2,    19,   389,    20,   520,
     449,   527,   279,   603,    26,   845,   654,   655,   657,   265,
     266,   267,    34,    35,    36,   298,    38,   586,   503,   745,
     831,   830,   522,   131,   363,   371,   947,    49,   546,    51,
      27,    68,   953,   316,    56,   217,    58,    27,    60,    34,
      35,    36,     1,    40,   950,     1,    68,    69,    70,    71,
      72,    73,    33,     1,    33,    77,    48,    79,   840,    81,
      82,   950,    33,    47,    86,  1056,    69,    89,    90,   106,
      92,    48,    94,     3,    27,    97,    98,   603,   100,    33,
      48,  1003,   104,   105,   106,   107,   943,    33,     3,    61,
      20,   347,   348,   119,  1015,    48,    56,    11,  1205,    52,
      84,     1,   105,    48,    34,    48,    91,   129,    27,   131,
       0,   106,    56,    61,    37,   145,   164,     1,    74,   164,
     139,    48,    75,   171,   164,    78,     3,    80,   147,   113,
     170,   657,  1144,    63,   164,  1056,    89,  1244,   119,    48,
     119,    74,   323,    91,    89,   117,   168,     3,   174,    72,
     990,   877,  1143,   145,   994,   108,   104,   231,   998,    78,
     116,    61,   445,   446,    48,   124,   203,    68,   145,   117,
      89,   193,   102,    56,   196,   197,   198,   145,   108,   201,
     202,   203,   174,   116,   169,   144,   145,   102,   169,   108,
     169,    91,   215,  1205,   108,  1117,   144,   174,   169,   145,
    1057,   275,   145,   104,   105,   106,   174,   989,   389,   231,
     119,   171,  1118,   172,   170,   169,   164,   117,   145,   172,
     738,   169,  1143,   169,   298,   102,   223,   171,    18,  1118,
     124,   174,   101,   223,    22,    91,   145,   170,    91,   169,
     124,   726,   316,    48,   144,   117,   102,    27,    47,   588,
     596,   145,    61,   275,   362,   149,    61,   119,   119,   780,
     144,   145,   831,  1067,   145,   174,    65,  1107,   171,   169,
     173,  1111,   164,   145,    48,   775,   298,   145,   172,   171,
     164,   173,    91,   305,   306,    84,    91,    77,    47,  1100,
    1099,  1102,  1101,   174,   316,   104,   745,   462,    78,    87,
     169,   202,   203,   145,   172,    95,    65,   145,   117,    89,
    1036,   164,   117,   174,   113,    89,   169,   124,   145,   124,
     342,   164,   112,   111,    54,    84,   164,   170,   170,   145,
     367,    56,   339,   355,   114,   357,   410,   164,   145,   145,
     145,   145,   149,    73,     3,   367,   946,  1151,  1152,  1153,
    1154,  1155,  1156,    83,   113,   164,   172,    56,   843,   164,
     169,   145,  1010,  1011,   169,   172,   172,   172,   172,   890,
      82,   445,   446,    85,   396,   108,   398,   399,   400,   401,
     402,   403,   145,   405,   406,   407,   408,   409,   172,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   401,   402,   590,   172,
     979,    51,   434,   435,    54,    56,   145,   902,   440,   145,
     946,   443,   444,   445,   446,   447,   448,   449,   877,   503,
    1079,   434,   435,    73,   145,   164,    76,   440,   460,   476,
     443,   444,   464,    83,   145,   467,   172,   145,    81,    82,
     145,   145,   447,   448,   476,   145,   478,   479,   145,   145,
     145,   172,   484,   485,   145,   460,   367,   859,   490,   164,
     164,   172,   546,   145,   172,   497,    61,  1003,   500,    89,
      61,   503,   172,   170,   479,    32,   172,   172,   145,   124,
     485,   172,   171,   749,   750,   490,   773,   753,   754,     2,
     172,   145,    32,   171,    27,   173,    91,   529,   145,    12,
      91,   533,   534,   170,   149,   145,   171,   539,   173,   104,
     139,     3,    25,   104,   546,    28,   170,   164,   147,   299,
     300,  1100,   117,  1102,   164,   139,   117,   559,    20,   171,
     562,    88,   564,   147,    56,    92,   568,   569,   570,    56,
     572,   573,    34,  1079,    56,    78,   812,   813,    88,   562,
     171,   564,    92,   585,   586,    56,    89,     2,   845,    56,
     124,   578,    56,   568,    77,   476,   598,    12,   855,   164,
       8,    63,    88,   164,   169,   108,    92,  1036,   169,   171,
      25,  1117,   139,    28,    56,    99,   150,   144,   152,   153,
     147,   104,   156,   157,   151,    56,   109,    88,   171,   139,
     140,    92,   142,   143,   144,    56,   146,   147,  1144,    56,
     102,   151,   171,   169,   173,   171,   108,   649,   158,   159,
      56,    32,   162,    56,   169,   165,   166,   167,   539,   171,
      56,   173,    77,   139,   144,   164,    81,    82,   144,   164,
     163,   147,   726,   164,   171,   151,   173,   171,   559,   173,
     562,   562,   564,   564,   738,   155,   164,   164,   569,   104,
     164,   164,   171,    56,   109,   164,   841,   169,   859,  1205,
      84,   164,   118,   108,   169,   171,   189,    88,   164,  1215,
       8,    92,   171,   171,   171,    65,   171,   169,    89,   721,
     722,   723,   724,   725,   726,   727,   728,   730,   731,   170,
     124,    89,   124,   990,   736,   164,   738,   739,  1244,   741,
     147,   145,    68,   745,   171,   747,   124,   722,   172,   724,
     145,    48,   171,   736,   756,   757,   124,   124,   139,   140,
      35,   142,   143,   144,    48,   146,   147,    35,   785,    65,
     151,   825,   826,   172,   189,   145,   124,    61,   159,   172,
      21,   783,   784,   785,   165,   166,   167,   789,   174,   843,
     172,    27,   172,   169,   145,   278,   104,   174,   171,   139,
     147,   147,   285,   147,   147,   147,   789,    91,    32,   147,
     139,   147,    48,   147,   147,   139,    52,  1080,  1081,   147,
     147,   823,   139,   139,   147,   827,   171,   829,   830,   831,
      35,   172,   849,   117,   164,   174,    35,   144,   170,    75,
     124,   843,    78,     3,    80,   164,   164,   849,   902,   164,
    1107,   164,   164,    89,   164,   736,   339,    17,   172,   145,
      20,   145,   164,   278,    88,    25,   169,   172,    92,   164,
     285,   354,   108,   174,    33,   877,    36,   170,    33,    35,
     164,   164,   174,   164,   174,   169,    46,   144,   172,   891,
     892,   893,   171,    53,   896,   171,   174,   174,   164,   164,
     902,   174,   172,    63,   785,   139,   389,   139,    35,   172,
      70,   913,   164,   915,   172,   139,   891,   892,   920,    27,
     144,   923,   146,   147,   339,   172,    86,   151,   145,   164,
     163,     2,   163,   935,   164,   159,   164,   164,    98,   354,
      48,    12,   102,   167,    52,   962,   823,   949,    24,   109,
     998,    89,   274,   354,    25,   115,   941,    28,   324,   966,
     962,   848,   859,   365,   855,   773,   514,    75,   849,   971,
      78,   973,    80,  1157,   976,   458,   280,   979,  1118,   854,
    1150,    89,   129,   985,   660,   987,   530,   779,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   478,    -1,    -1,    -1,     2,
     108,   484,    -1,    -1,    -1,    -1,    77,    -1,    -1,    12,
      -1,    -1,   495,    -1,   497,    -1,    -1,  1019,  1020,    -1,
    1022,  1023,    25,  1025,  1026,    28,  1080,  1081,    -1,  1031,
    1032,    -1,  1034,   104,  1036,    -1,  1038,  1039,   109,    -1,
    1042,  1058,    -1,   458,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1058,    -1,    -1,    -1,
      -1,  1063,    -1,   478,    -1,   946,    -1,    -1,    -1,   484,
      -1,    -1,    -1,    -1,    77,    -1,  1078,  1079,  1080,  1081,
     495,   962,   497,   566,  1086,    -1,  1088,   570,    -1,    -1,
    1092,    -1,    -1,    -1,    -1,   578,  1098,  1099,  1100,  1101,
    1102,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1117,    -1,    -1,   189,    -1,
      -1,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,    -1,
      -1,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,  1149,    -1,    26,
      27,   566,    -1,    -1,    -1,   570,    -1,    -1,    -1,    36,
      -1,    38,    39,   578,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,  1178,  1058,    -1,    -1,
      -1,    -1,  1063,    -1,    -1,    -1,   189,    -1,    -1,    -1,
      -1,  1193,  1194,  1195,  1196,    -1,    -1,    -1,    -1,    76,
      -1,    78,  1204,  1205,    32,    82,    -1,   278,    85,    -1,
      87,    -1,    89,  1215,   285,    -1,    -1,  1219,  1220,     1,
      -1,    -1,    -1,    -1,  1226,  1227,  1228,  1229,  1230,  1231,
      -1,   108,    -1,    -1,   111,   718,    -1,   114,    -1,    -1,
      22,    -1,  1244,    -1,    26,    27,    32,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
      88,    -1,    44,  1144,    92,    -1,    -1,    -1,   339,    51,
      -1,    -1,    54,    -1,    -1,   278,   759,    -1,    -1,   762,
      -1,    -1,   285,   354,    -1,    -1,    -1,   164,    -1,    -1,
      -1,    73,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      82,    83,    88,    85,    -1,    87,    92,    89,    -1,    -1,
      -1,   139,   140,    -1,    -1,   143,   144,    -1,   146,   147,
      -1,    -1,    -1,   151,  1205,    -1,   108,    -1,   811,   111,
      -1,   159,   114,    -1,   817,    -1,   339,   165,   166,   167,
      -1,    -1,    -1,    -1,   759,     2,    -1,   762,    -1,    -1,
      -1,   354,    -1,   139,   140,    12,   142,   143,   144,    -1,
     146,   147,   148,  1244,    -1,   151,    -1,    -1,    25,    -1,
      -1,    28,   158,   159,    -1,    -1,   162,    -1,    -1,   165,
     166,   167,   164,    -1,    -1,    -1,    -1,   458,   170,    -1,
     176,    -1,    -1,    -1,    -1,    -1,   811,    -1,    -1,    -1,
      -1,    -1,   817,    -1,    -1,    -1,    -1,   478,   891,   892,
      -1,    -1,    -1,   484,    -1,    -1,    -1,    -1,    -1,     3,
      77,    -1,    -1,    -1,   495,    -1,   497,    -1,    -1,    -1,
     913,    -1,   915,    17,    18,    -1,    20,   920,    -1,    -1,
     923,    25,    -1,    -1,    -1,    -1,    -1,   104,    -1,   932,
      34,   934,   109,    -1,   937,   458,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    32,    -1,    53,
      -1,    -1,    -1,    -1,    -1,   478,    -1,    -1,   961,    63,
      -1,   484,    -1,    -1,    -1,    69,    70,    -1,   971,    -1,
     973,    -1,   495,    77,   497,   566,    -1,    -1,   913,   570,
     915,    -1,    86,    -1,    -1,   920,    -1,   578,   923,    -1,
      32,    95,    96,    -1,    98,    -1,   100,   932,   102,   934,
      -1,   105,   937,    88,    -1,   109,   110,    92,   112,    -1,
      -1,   115,   189,    -1,    -1,    -1,    -1,    48,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,   961,    -1,  1031,    12,
      61,    -1,    -1,    -1,    -1,  1038,   971,    -1,   973,    -1,
      -1,    -1,    25,   566,    -1,    28,    88,   570,  1051,    -1,
      92,    -1,    -1,    -1,   139,   578,    -1,    -1,    -1,   144,
      91,   146,   147,    -1,    -1,    -1,   151,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,    -1,  1082,
     165,   166,   167,    -1,    -1,    -1,   117,    -1,  1091,    -1,
      -1,    -1,  1095,   124,    77,    -1,  1031,   139,   140,    -1,
      -1,   278,   144,  1038,   146,   147,    -1,    -1,   285,   151,
      -1,    -1,    -1,    -1,   145,    -1,  1051,   159,    -1,    -1,
      -1,   104,    -1,   165,   166,   167,   109,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    -1,     3,  1139,    -1,   169,    -1,
      -1,   172,    -1,    -1,    -1,    -1,    -1,  1082,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,  1091,    25,    -1,    -1,
    1095,    -1,   339,    -1,    -1,    -1,    34,    -1,   759,    -1,
      -1,   762,    40,    -1,    -1,    -1,    -1,   354,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
    1193,  1194,  1195,  1196,    -1,    63,    -1,    -1,     2,    -1,
      -1,    69,    70,    -1,  1139,    -1,   189,    -1,    12,    77,
      -1,    -1,    -1,    -1,    -1,    -1,  1219,  1220,    86,    -1,
     811,    25,    -1,    -1,    28,    -1,   817,    95,    96,    -1,
      98,    -1,   100,    -1,   102,    -1,   759,   105,    -1,   762,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,  1193,  1194,
    1195,  1196,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
      -1,    -1,    44,    77,    -1,    -1,    -1,    -1,    -1,    51,
      -1,   458,    -1,    -1,  1219,  1220,    -1,    -1,   811,    -1,
      -1,    -1,    -1,    -1,   817,   278,    -1,    -1,    -1,    -1,
     104,   478,   285,   171,    76,   109,    78,   484,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    89,   495,    -1,
     497,    -1,   913,    -1,   915,    -1,    -1,    -1,    -1,   920,
      -1,    -1,   923,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,   932,   114,   934,    -1,    -1,   937,    -1,    -1,    -1,
      22,    -1,    -1,    -1,    26,    27,   339,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
     961,   354,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
     971,    -1,   973,    -1,    -1,   189,    -1,    -1,    -1,   566,
     913,    -1,   915,   570,    -1,    -1,    -1,   920,    -1,    -1,
     923,   578,    -1,    -1,    76,    -1,    78,    -1,    80,   932,
      82,   934,    -1,    85,   937,    87,    -1,    89,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,    -1,    -1,    -1,   108,    -1,   961,   111,
    1031,    -1,   114,    -1,    -1,    -1,    -1,  1038,   971,   155,
     973,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1051,    -1,   168,    -1,    -1,   171,    -1,   173,    -1,    -1,
      -1,    -1,    -1,    -1,   278,   458,    -1,    -1,    -1,    -1,
      -1,   285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1082,   164,    -1,    -1,   478,    -1,    -1,   170,    -1,
    1091,   484,    -1,    -1,  1095,    -1,    -1,    -1,  1031,    -1,
      -1,    -1,   495,    -1,   497,  1038,     2,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,  1051,    -1,
      -1,    -1,    -1,    -1,    -1,   339,    -1,    -1,    -1,    25,
      -1,    -1,    28,     8,    -1,    -1,    -1,    -1,  1139,    -1,
     354,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,  1082,
      -1,    26,    27,    -1,    -1,    -1,    -1,    -1,  1091,    -1,
      -1,    36,  1095,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,   759,   566,    -1,   762,    51,   570,    -1,    -1,
      -1,    77,    -1,    -1,    -1,   578,    -1,    -1,    -1,    -1,
      65,    -1,  1193,  1194,  1195,  1196,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    -1,  1139,    82,   104,    84,
      85,    -1,    87,   109,    89,    -1,    -1,    -1,  1219,  1220,
      -1,    -1,    -1,    -1,   811,    -1,    -1,    -1,    -1,    -1,
     817,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,   458,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1193,  1194,  1195,  1196,   478,    -1,    -1,    -1,    -1,    -1,
     484,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   495,    -1,   497,    -1,    -1,  1219,  1220,     3,   164,
      -1,    -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,   913,    -1,   915,    -1,
      -1,    46,    -1,   920,    -1,    50,   923,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,   932,    -1,   934,    63,    -1,
     937,    -1,   566,    -1,    69,    70,   570,    -1,    -1,    -1,
      -1,    -1,    77,    -1,   578,    -1,   759,    -1,    -1,   762,
      -1,    86,    -1,    -1,   961,    -1,    -1,    -1,    -1,    -1,
      95,    96,   278,    98,   971,   100,   973,   102,    -1,   285,
     105,    -1,    -1,    -1,   109,    -1,     2,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   811,    25,
      71,    -1,    28,    22,   817,    -1,    -1,    26,    27,    -1,
      -1,     2,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    12,    -1,   339,  1031,    44,    -1,    -1,    -1,    -1,
      -1,  1038,    51,    -1,    25,    -1,   171,    28,   354,    -1,
      -1,    -1,    -1,    -1,  1051,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    85,    -1,    87,    -1,
      89,    -1,    -1,    -1,    -1,  1082,    -1,    -1,   104,    -1,
      -1,    -1,    -1,   109,  1091,    -1,    77,    -1,  1095,   108,
      -1,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
     913,    -1,   915,    -1,    -1,    -1,    -1,   920,    -1,    -1,
     923,    -1,    -1,   104,    -1,    -1,    -1,    -1,   109,   932,
      -1,   934,    -1,    -1,   937,   759,    -1,    -1,   762,    -1,
      -1,    -1,  1139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   458,    -1,    -1,   164,    -1,    -1,   961,    -1,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   971,    -1,
     973,    -1,   478,   189,    -1,    -1,    -1,    -1,   484,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   811,    -1,   495,
      -1,   497,    -1,   817,    -1,    -1,  1193,  1194,  1195,  1196,
      -1,   262,   263,   264,   265,   266,   267,    -1,   189,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1219,  1220,    -1,    -1,    -1,   288,  1031,    -1,
      -1,    -1,    -1,   294,    -1,  1038,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1051,    -1,
      -1,   312,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     566,    -1,   278,    -1,   570,    -1,    -1,   328,   329,   285,
      -1,    -1,   578,    49,    -1,    -1,    -1,    -1,    -1,  1082,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1091,   913,
      -1,   915,  1095,    -1,    -1,    71,   920,   278,    -1,   923,
      -1,    -1,    -1,    -1,   285,    81,    82,    -1,   932,    -1,
     934,    -1,    22,   937,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    -1,   339,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,    -1,  1139,   961,   354,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,   971,    -1,   973,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   339,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      80,    -1,    82,   354,    -1,    85,    -1,    87,    -1,    89,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
    1193,  1194,  1195,  1196,    36,    -1,    38,    39,   108,    -1,
     461,   111,    44,    -1,   114,    -1,    -1,  1031,    -1,    51,
      -1,    -1,    -1,    -1,  1038,    -1,  1219,  1220,    -1,    -1,
      -1,    -1,   483,    -1,    -1,    -1,   487,  1051,   489,    -1,
      -1,   492,    -1,    -1,    76,    -1,    78,    -1,   214,    -1,
      82,    -1,   458,    85,    -1,    87,    -1,    89,    -1,    -1,
      -1,    -1,    -1,   759,   164,    -1,   762,    -1,  1082,    -1,
     170,    -1,   478,    -1,    -1,    -1,   108,  1091,   484,   111,
      -1,  1095,   114,    -1,    -1,    -1,    -1,   458,    -1,   495,
      -1,   497,    -1,    -1,    -1,    -1,   262,   263,   264,   265,
     266,   267,    -1,    -1,   270,   271,    -1,   478,    -1,    -1,
      -1,   277,    -1,   484,    -1,   811,    -1,    -1,    -1,    -1,
      -1,   817,   288,    -1,   495,  1139,    -1,    30,   294,    -1,
      -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,    -1,    -1,   312,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     566,    -1,   328,   329,   570,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   578,    -1,    -1,    -1,    -1,    -1,    -1,  1193,
    1194,  1195,  1196,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   566,    22,    -1,    -1,   570,
      26,    27,    -1,    -1,    -1,  1219,  1220,   578,    -1,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,   913,    44,   915,
      -1,    -1,    -1,    -1,   920,    51,    -1,   923,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   932,    -1,   934,    65,
      -1,   937,    -1,   146,    -1,   148,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,   157,    -1,    82,    -1,    84,    85,
      -1,    87,    -1,    89,    -1,   961,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   971,    -1,   973,    -1,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,   743,   744,    -1,   461,   199,    -1,   749,   750,
      -1,    -1,   753,   754,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   483,    -1,    -1,
      -1,   487,   225,   489,    -1,    -1,   492,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1031,    -1,    -1,   164,    -1,
      -1,    -1,  1038,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   759,    -1,  1051,   762,   260,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   268,   269,    -1,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,   281,    -1,
      -1,    -1,    36,    -1,    38,    39,  1082,    -1,   759,    -1,
      44,   762,    -1,    -1,    -1,  1091,    -1,    51,    -1,  1095,
      -1,    -1,    -1,    -1,    -1,   811,    -1,    -1,    -1,    -1,
      -1,   817,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    85,    -1,    87,    -1,    89,    -1,    -1,    -1,    -1,
     811,    -1,    -1,  1139,    -1,    -1,   817,    -1,    -1,    -1,
      -1,    -1,   903,   904,   108,    -1,    -1,   111,    -1,   910,
     114,    -1,    -1,   366,    -1,    -1,    -1,    -1,   371,    -1,
      -1,   922,    -1,    -1,   925,    -1,   927,    -1,   929,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   389,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1193,  1194,  1195,
    1196,    -1,    -1,    -1,    -1,    -1,    -1,   913,    -1,   915,
     164,    -1,    -1,    -1,   920,    -1,    -1,   923,    -1,    -1,
      -1,    -1,    -1,  1219,  1220,    -1,   932,    -1,   934,    -1,
      -1,   937,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,   913,    -1,   915,    -1,    -1,    -1,    -1,   920,
      -1,    -1,   923,    -1,   457,   961,    -1,    -1,    -1,    -1,
      -1,   932,    -1,   934,    -1,   971,   937,   973,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   743,   744,    -1,
      -1,    -1,    -1,   749,   750,    -1,    -1,   753,   754,    -1,
     961,    -1,    -1,    -1,    -1,    88,    -1,   763,    -1,    92,
     971,   504,   973,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   518,    -1,    -1,    -1,   522,
      -1,    -1,    -1,    -1,    -1,  1031,    -1,    -1,    -1,    -1,
     123,    -1,  1038,  1084,    -1,    -1,   802,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1051,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,  1082,    -1,    -1,    -1,
    1051,   584,    -1,   176,    -1,  1091,    -1,    -1,    -1,  1095,
      -1,    -1,    -1,   596,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,  1082,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1091,    -1,    -1,    -1,  1095,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1139,    -1,    -1,    -1,   903,   904,    -1,
     168,    -1,    -1,   171,   910,   173,   912,    -1,    -1,    -1,
      -1,    -1,    -1,   919,    -1,    -1,   922,   660,    -1,   925,
      -1,   927,    -1,   929,    -1,    -1,    -1,    -1,  1139,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   942,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,  1193,  1194,  1195,
    1196,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    17,
      18,    19,    20,   969,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,  1219,  1220,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,   764,    -1,    -1,    -1,    -1,    -1,    -1,    86,   772,
      -1,    -1,   775,    -1,    -1,    -1,   779,    95,    96,    -1,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,   802,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,  1084,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,   146,    -1,
      -1,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    -1,   162,   848,    -1,   165,   166,   167,
     168,    -1,    -1,   171,    -1,    -1,   859,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1133,  1134,    -1,
      -1,  1137,  1138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1165,
      -1,  1167,    -1,  1169,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,   941,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,   966,    38,    39,    40,    41,    42,    43,
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
     164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     1,   176,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    -1,    53,    54,    -1,    56,    57,    58,    59,
      60,    -1,    62,    63,    64,    65,    66,    67,    -1,    69,
      70,    71,    -1,    73,    -1,    -1,    76,    77,    78,    79,
      -1,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,    -1,   102,   103,    -1,   105,   106,   107,   108,   109,
      -1,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,    -1,   169,
     170,   171,    -1,   173,    -1,     1,   176,     3,     4,     5,
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
      -1,   173,   174,     1,   176,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,    -1,   148,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,   176,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,   106,   107,
      -1,   109,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,   172,   173,    -1,    -1,   176,     3,
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
      -1,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    48,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,   169,
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
     106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
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
      64,    -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,   106,   107,    -1,   109,    -1,    -1,   112,    -1,
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
      -1,    -1,    -1,    -1,    -1,    77,    78,    79,    -1,    81,
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
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,   114,   115,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,    -1,
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
      -1,   105,   106,   107,    -1,   109,    -1,    -1,   112,    -1,
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
     170,   171,    -1,   173,    -1,    -1,   176,     3,     4,     5,
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
      -1,   167,    -1,   169,    -1,   171,    -1,   173,   174,    -1,
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
      -1,   173,   174,     3,   176,     5,     6,     7,     8,     9,
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
      -1,   121,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,
      -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,
      -1,   171,    -1,   173,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    34,    -1,    36,    -1,
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
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,   111,
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
      -1,   171,    -1,   173,   174,     3,   176,     5,     6,     7,
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
      -1,   109,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
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
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,    -1,   176,     3,     4,     5,     6,     7,
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
      -1,    -1,    -1,   167,    -1,    -1,    -1,   171,    -1,   173,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
       3,   173,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    61,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    91,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,   104,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
     163,   164,    25,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
      -1,    -1,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     163,    -1,    -1,    -1,    -1,    -1,   169,    -1,   171,   172,
     173,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,   141,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,   163,    -1,    25,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,   163,    -1,    25,    -1,    -1,    -1,   169,    -1,   171,
      -1,   173,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    17,    18,   105,    20,    -1,    -1,   109,    -1,    25,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,   141,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   171,
      86,   173,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    -1,    98,    -1,   100,    -1,   102,    -1,    -1,   105,
      -1,     3,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      12,    -1,    -1,    -1,    -1,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,   169,    -1,   171,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    95,    96,    -1,    98,    -1,   100,    19,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    32,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,    -1,   146,    -1,    -1,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
     162,    -1,    -1,   165,   166,   167,   168,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,    12,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,   132,   133,   134,   135,    -1,   137,   138,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,    12,
      -1,    -1,    -1,    -1,    -1,   101,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,    -1,    48,   132,   133,   134,   135,
      -1,   137,   138,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,   119,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    88,    -1,    -1,
      -1,    92,    -1,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,   119,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,
      88,    -1,    -1,    -1,    92,   176,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    37,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    72,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,
      12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    37,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      72,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
     165,   166,   167,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      92,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    -1,    88,    -1,
      -1,    -1,    92,    -1,   176,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,   119,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    88,    -1,    -1,    -1,    92,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    32,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    88,    -1,    -1,    -1,    92,    12,   176,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,   123,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,    -1,
      12,   176,    -1,    -1,    -1,   139,   140,    19,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
      32,    33,   156,   157,   158,   159,   160,    -1,   162,   123,
      -1,   165,   166,   167,    -1,    -1,    48,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    88,    -1,    -1,    -1,
      92,    12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    -1,    12,   176,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,
      -1,    -1,    -1,   101,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,   119,    37,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      72,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    -1,    88,    -1,    -1,   174,
      92,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    -1,    88,    -1,
      -1,    -1,    92,    -1,   176,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,   169,
      -1,    88,    -1,    -1,    -1,    92,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,   165,   166,
     167,    -1,    -1,    88,    -1,    -1,    -1,    92,    12,   176,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,
      -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,   165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,
      12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   172,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,   164,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
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
      -1,   165,   166,   167,    -1,    88,    -1,    -1,   172,    92,
      12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,   172,
      92,    12,    13,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     122,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,   172,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    12,    13,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,   174,    12,   176,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,    12,
     176,    -1,    -1,    -1,    -1,    -1,    19,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,
      -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,   101,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    61,   156,   157,   158,   159,   160,   161,   162,
      12,    13,   165,   166,   167,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    88,
      32,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    88,   146,   147,   148,
      92,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   123,    -1,   165,   166,   167,    -1,    -1,    88,    -1,
      -1,    -1,    92,    -1,   176,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    12,    -1,   165,   166,   167,    -1,    -1,    19,    -1,
      -1,    -1,   174,    -1,   176,    -1,    -1,    -1,    -1,   139,
     140,    32,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,    12,   162,    -1,    -1,   165,   166,   167,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     101,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   123,    -1,   165,   166,   167,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,   176,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    12,    -1,   165,   166,   167,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
     139,   140,    32,   142,   143,   144,    -1,   146,   147,   148,
      -1,    -1,   151,   152,   153,    -1,    -1,   156,   157,   158,
     159,    -1,    12,   162,    -1,    -1,   165,   166,   167,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   101,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   123,    -1,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    12,     3,   165,   166,   167,    -1,     8,
      19,    -1,    -1,    -1,    -1,    -1,   176,    -1,    17,    18,
      -1,    20,    -1,    32,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    12,    -1,    -1,    -1,    46,    -1,    -1,
      19,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    63,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    88,
      -1,    -1,    -1,    92,    12,    -1,    -1,    86,    -1,    -1,
      -1,    19,   101,    -1,    -1,    -1,    95,    96,    -1,    98,
      -1,   100,    -1,   102,    32,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,   123,    -1,   115,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   101,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   123,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,   176,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,   123,   165,   166,   167,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   176,    19,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    32,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    -1,    12,    -1,    -1,    -1,   174,    -1,   176,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,   123,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,     1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    34,    -1,    -1,     3,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    17,    18,
      53,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      63,    -1,    31,    -1,    -1,    34,    69,    70,    -1,    -1,
      -1,    40,    -1,    -1,    77,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    86,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    63,    98,    -1,   100,    -1,   102,
      69,    70,   105,    -1,    -1,    -1,   109,    -1,    77,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    86,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    98,
      -1,   100,    -1,   102,    17,    18,   105,    20,    -1,    -1,
     109,    -1,    25,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    34,    -1,    -1,     3,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    17,    18,
      53,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    34,    69,    70,    -1,    -1,
      -1,    40,    -1,    -1,    77,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    86,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    63,    98,    -1,   100,    -1,   102,
      69,    70,   105,    -1,    -1,    -1,   109,    -1,    77,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    86,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    98,
      -1,   100,    -1,   102,    17,    18,   105,    20,    -1,    -1,
     109,    -1,    25,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115
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
     222,   223,   224,   226,   227,   229,   231,   234,   235,   236,
     237,   238,   242,   243,   246,   247,   248,   251,   252,   257,
     258,   259,   260,   261,   263,   264,   287,   288,   292,   293,
     314,   315,   316,   317,   318,   319,   320,   328,   329,   330,
     331,   332,   335,   336,   337,   338,   339,   340,   341,   342,
     344,   345,   346,   347,   348,   164,   185,   332,   119,   321,
     322,     3,   206,    14,    22,    36,    41,    42,    45,    56,
      87,   100,   169,   173,   234,   257,   314,   319,   330,   331,
     332,   335,   337,   338,   321,   332,   108,   294,    89,   206,
     185,   308,   332,     8,    22,    36,    39,    87,   187,   190,
     185,   171,     3,    17,    18,    20,    25,    34,    40,    46,
      50,    53,    63,    69,    70,    77,    86,    95,    96,    98,
     100,   102,   105,   109,   112,   115,   208,   210,    11,   108,
      78,   121,   228,   332,   228,   332,   228,   332,    27,   114,
     230,   332,    82,    85,   190,   171,   208,   208,   208,   171,
     269,   171,   208,   295,   296,    33,   194,   213,   332,   332,
      18,    77,    95,   112,   332,   332,   332,     8,   171,   218,
     217,     4,   281,   307,   332,   106,   107,   164,   332,   334,
     332,   213,   332,   332,   332,    99,   171,   185,   332,   332,
     194,   205,   332,   335,   194,   205,   332,   332,   230,   332,
     332,   332,   332,   332,   332,   332,     1,   170,   183,   195,
     307,   110,   149,   281,   309,   310,   334,   228,   307,   332,
     343,   332,    80,   185,   169,    47,    84,   113,   191,    26,
     293,   332,     8,   247,   332,   333,    56,   144,   249,   208,
       1,    31,   208,   253,   255,   256,    54,    73,    83,   276,
      27,    78,    89,   108,   277,    27,    78,    89,   108,   275,
     208,   289,   290,   295,   163,   164,   155,   332,    12,    19,
      32,    88,    92,   123,   139,   140,   142,   143,   144,   146,
     147,   148,   150,   151,   152,   153,   154,   156,   157,   158,
     159,   160,   161,   162,   165,   166,   167,   176,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   168,   267,   171,   173,    88,    92,   171,
     185,   164,   332,   332,   332,   208,   307,    56,   169,   194,
      48,   321,   292,   164,   145,   164,   187,   118,   209,   281,
     311,   312,   313,   334,   169,   208,   171,   213,    33,    48,
     213,   119,   213,   324,    33,    48,   213,   324,   213,   324,
      48,   213,   324,   208,   208,   101,   194,   101,   124,   194,
     267,   191,   311,   171,   171,   194,   185,    27,    48,    52,
      75,    78,    89,   108,   184,   270,   271,   272,   273,   274,
     277,   110,   171,   208,   297,   298,     1,   144,   302,    48,
     145,   185,   213,   171,   171,   213,   311,   219,   219,   145,
     164,   332,   332,   164,   169,   213,   171,   311,   164,   239,
     164,   239,   164,   213,   213,   164,   170,   170,   183,   145,
     170,   332,   145,   172,   145,   172,   174,   324,    48,   145,
     174,   324,   122,   145,   174,     8,     1,   170,   195,   202,
     203,   332,   197,   332,    65,    37,    72,   164,   247,   249,
     225,   261,   208,   307,   169,   170,     8,   256,   124,   145,
     170,    89,     1,   144,   301,    89,     1,     3,    12,    17,
      19,    20,    25,    40,    46,    53,    55,    63,    69,    70,
      86,    98,   102,   105,   109,   115,   139,   140,   141,   142,
     143,   144,   146,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   162,   165,   166,   167,   168,   171,
     207,   208,   210,   265,   266,   267,   314,   124,   291,   164,
     145,   164,   332,   332,   332,   332,   228,   332,   228,   332,
     332,   332,   332,   332,   332,   332,     3,    20,    34,    63,
     102,   108,   209,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
      68,   334,   334,   334,   334,   334,   311,   311,   228,   332,
     228,   332,    27,    48,    89,   114,   323,   326,   327,   332,
     348,    33,    48,    33,    48,   101,   171,    48,   174,   208,
     228,   332,   213,   295,   332,   332,   124,   172,   145,    48,
     307,    45,   332,   228,   332,   171,   213,    45,   332,   228,
     332,   213,   213,   228,   332,   213,   124,   124,   185,    35,
     185,   332,    35,   332,    65,   172,   312,   208,   232,   233,
      48,    89,   273,   145,   172,   171,   208,   302,   298,   145,
     172,    34,    50,    96,   100,   173,   212,   303,   315,   124,
     299,   332,   296,   332,   332,   172,   281,   332,     1,   244,
     313,   172,    21,   240,   170,   172,   172,   309,   172,   309,
     185,   174,   228,   332,   174,   185,   332,   174,   332,   174,
     332,   170,   170,   145,   164,    13,   147,   145,   164,    13,
      37,    72,   208,   167,   196,   199,   332,   196,   198,   249,
     169,   292,   164,   171,     1,   250,   256,   332,   254,   173,
     212,   300,   315,   104,   278,   171,   268,   332,   139,   147,
     268,   268,   303,   315,   289,   171,   173,   164,   164,   164,
     164,   164,   164,   172,   174,    48,    89,   145,   172,    17,
      20,    25,    46,    53,    63,    70,    86,    98,   109,   115,
     314,    88,    88,    45,   228,   332,    45,   228,   332,   312,
     228,   332,   171,   321,   321,   164,   164,   281,   334,   313,
     332,   172,   332,    33,   213,    33,   213,   325,   326,   332,
      33,   213,   324,    33,   213,   324,   213,   324,   213,   324,
     332,   332,    35,   185,    35,    35,   185,   101,   194,   208,
     172,   145,   172,   208,   272,   298,   144,   306,    61,   117,
     283,   172,   297,   302,     1,   307,    68,   334,   172,   172,
     170,    74,   116,   170,   245,   172,   171,   194,   208,   241,
     185,   174,   324,   174,   324,   185,   122,   202,   209,   169,
     332,   110,   332,   199,   198,   145,   164,    13,   164,   169,
     250,   295,   312,   170,    31,    82,    85,   170,   184,   215,
     216,   255,     1,   174,   307,   283,   271,   172,     3,   102,
     266,   267,   172,   174,   326,   302,   314,   314,   332,    33,
      33,   332,    33,    33,   172,   174,   174,   312,   213,   213,
     213,   101,    45,   332,    45,   332,   145,   172,   101,    45,
     332,   213,    45,   332,   213,   213,   213,   185,   185,   332,
     185,    35,   164,   164,   233,   194,   306,   172,   173,   212,
     281,   305,   315,   149,   282,   299,     3,    91,   102,   284,
     285,   286,   332,   193,   214,   280,   299,   174,    48,   174,
     171,   171,    33,   185,   307,   241,   144,   194,    45,   185,
     332,   174,    45,   185,   332,   174,   332,   196,    13,    37,
      72,    37,    72,   164,   164,   332,   332,   250,   170,   164,
     172,     8,   216,   215,   174,   300,   315,   174,   262,   172,
     268,   268,   299,   101,    45,    45,   101,    45,    45,    45,
      45,   172,   332,   332,   332,   326,   332,   332,   332,    35,
     185,   282,   299,   306,   174,   307,   281,   332,   286,   117,
     145,   124,   150,   152,   153,   156,   157,    61,   332,   303,
     315,   311,   311,   185,   213,   172,   332,   332,   185,   332,
     185,   170,   110,   332,   199,   198,   199,   198,    13,   170,
     164,   216,   300,   315,   164,   279,   280,   332,   332,   332,
     332,   332,   332,   101,   101,   101,   101,   185,   282,   299,
     281,   304,   305,   315,    48,   174,   332,   285,   286,   286,
     286,   286,   286,   286,   284,   174,   172,   172,   194,   101,
     101,   164,   164,   164,   164,   332,   101,   101,   101,   101,
     101,   101,   332,   332,   332,   332,   332,   304,   305,   315,
     163,   163,   332,   332,   174,   304
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
     220,   220,   221,   222,   222,   223,   224,   224,   224,   225,
     224,   226,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   227,   227,   227,
     227,   227,   227,   227,   227,   227,   227,   228,   229,   229,
     229,   229,   229,   229,   229,   229,   229,   229,   229,   229,
     230,   230,   231,   231,   232,   232,   233,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   234,   234,   234,
     235,   235,   235,   235,   235,   235,   236,   236,   236,   237,
     237,   237,   238,   238,   238,   238,   238,   238,   239,   239,
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
     288,   289,   289,   290,   291,   291,   291,   292,   292,   292,
     292,   292,   294,   293,   293,   295,   295,   296,   296,   297,
     297,   297,   298,   298,   298,   299,   299,   299,   300,   300,
     300,   300,   300,   300,   300,   301,   301,   301,   301,   301,
     302,   302,   302,   302,   302,   303,   303,   303,   303,   304,
     304,   304,   305,   305,   305,   305,   305,   306,   306,   306,
     306,   306,   307,   307,   307,   307,   308,   308,   309,   309,
     309,   310,   310,   311,   311,   312,   312,   313,   313,   313,
     313,   314,   314,   315,   315,   315,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   316,   316,   316,   317,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   318,   319,   320,   320,   320,
     320,   320,   320,   320,   320,   321,   321,   322,   323,   323,
     324,   325,   325,   326,   326,   326,   327,   327,   327,   327,
     327,   327,   328,   328,   329,   329,   329,   329,   329,   330,
     330,   330,   330,   330,   331,   332,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   333,   333,   334,   334,   334,   335,   335,   335,
     335,   336,   336,   336,   336,   336,   337,   337,   337,   338,
     338,   338,   338,   338,   338,   339,   339,   339,   339,   340,
     340,   341,   341,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   343,   343,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   345,   345,   345,   345,   345,   345,   345,   346,   346,
     346,   346,   347,   347,   347,   347,   348,   348,   348,   348,
     348,   348,   348
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     2,     2,     3,     3,
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
       5,     2,     1,     1,     1,     1,     6,     7,     3,     0,
       6,     2,     5,     3,     3,     6,     6,     4,     5,     5,
       3,     3,     6,     5,     6,     5,     6,     3,     4,     3,
       4,     5,     6,     5,     6,     3,     4,     3,     4,     6,
       7,     6,     7,     4,     5,     4,     5,     4,     4,     3,
       6,     5,     4,     3,     6,     5,     6,     5,     8,     7,
       4,     4,     6,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     6,     4,     7,     5,
       3,     6,     3,     3,     2,     2,     3,     3,     0,     2,
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
       2,     1,     3,     2,     0,     2,     2,     1,     2,     1,
       1,     1,     0,     5,     3,     1,     3,     3,     5,     1,
       1,     3,     1,     2,     3,     0,     2,     2,     3,     2,
       4,     3,     3,     4,     3,     0,     2,     2,     2,     1,
       0,     2,     2,     2,     1,     4,     4,     6,     3,     0,
       1,     1,     3,     4,     3,     4,     6,     0,     2,     2,
       2,     2,     1,     1,     3,     3,     1,     3,     1,     1,
       1,     3,     3,     0,     1,     1,     3,     3,     3,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     5,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     1,     1,     6,     6,
       4,     9,     9,     7,     6,     6,     4,     9,     9,     7,
       4,     6,     6,     9,     9,     6,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     0,     1,     4,     1,     3,
       4,     1,     3,     4,     3,     3,     1,     1,     2,     1,
       2,     1,     1,     3,     1,     2,     2,     2,     2,     2,
       8,     8,     9,     9,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     3,     3,     3,     2,     2,
       2,     1,     0,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     2,     2,     1,     1,     4,     4,     4,     3,
       3,     3,     3,     5,     5,     3,     4,     3,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     3,     4,     3,     5,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1,     1
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
#line 636 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6421 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 641 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6427 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 642 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6433 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 648 "chpl.ypp"
                                      { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6439 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 654 "chpl.ypp"
    {
      Expression* lit = (yyvsp[0].expr);
      if (lit->isStringLiteral()) {
        StringLiteral* strLit = lit->toStringLiteral();
        (yyval.uniqueStrList) = new UniqueStrList();
        PODUniqueString u = STR(strLit->str().c_str());
        delete strLit;
        (yyval.uniqueStrList)->push_back(u);
      } else {
        // in this case we expect that there was an error
        // reading the string literal so it is ErroneousExpression
        // and an error message has already been recorded.
        // Use "" as the string to indicate an error in the pragma.
        PODUniqueString u = STR("");
        (yyval.uniqueStrList)->push_back(u);
      }
    }
#line 6461 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 672 "chpl.ypp"
    {
      Expression* lit = (yyvsp[0].expr);
      if (lit->isStringLiteral()) {
        StringLiteral* strLit = lit->toStringLiteral();
        PODUniqueString u = STR(strLit->str().c_str());
        delete strLit;
        (yyvsp[-2].uniqueStrList)->push_back(u);
      } else {
        // in this case we expect that there was an error
        // reading the string literal so it is ErroneousExpression
        // and an error message has already been recorded.
        // Use "" as the string to indicate an error in the pragma.
        PODUniqueString u = STR("");
        (yyval.uniqueStrList)->push_back(u);
      }
    }
#line 6482 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: deprecated_decl_stmt  */
#line 691 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6488 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: include_module_stmt  */
#line 692 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6494 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: block_stmt  */
#line 693 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6500 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: use_stmt  */
#line 694 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6506 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: import_stmt  */
#line 695 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6512 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: require_stmt  */
#line 696 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6518 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: assignment_stmt  */
#line 697 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6524 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 698 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6530 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: if_stmt  */
#line 699 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6536 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 700 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6542 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 701 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6548 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: loop_stmt  */
#line 702 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6554 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: select_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6560 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: TDEFER stmt  */
#line 705 "chpl.ypp"
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
#line 6577 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: try_stmt  */
#line 717 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6583 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: throw_stmt  */
#line 718 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6589 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: return_stmt  */
#line 719 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6595 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: stmt_level_expr TSEMI  */
#line 720 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6601 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: TATOMIC stmt  */
#line 721 "chpl.ypp"
                            { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6607 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 723 "chpl.ypp"
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
#line 6625 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 737 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6638 "bison-chpl-lib.cpp"
    break;

  case 30: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
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
#line 6657 "bison-chpl-lib.cpp"
    break;

  case 31: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 761 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6670 "bison-chpl-lib.cpp"
    break;

  case 32: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 770 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6682 "bison-chpl-lib.cpp"
    break;

  case 33: /* stmt: TLABEL ident_def stmt  */
#line 778 "chpl.ypp"
  {
    if ((yyvsp[0].commentsAndStmt).stmt->isFor() || (yyvsp[0].commentsAndStmt).stmt->isWhile() || (yyvsp[0].commentsAndStmt).stmt->isDoWhile()) {
      auto exprLst = context->makeList((yyvsp[0].commentsAndStmt));
      auto comments = context->gatherCommentsFromList(exprLst, (yylsp[-2]));
      auto astLst = context->consumeList(exprLst);
      Loop* loop = nullptr;
      for (auto& ast : astLst) {
        if (ast->isLoop()) {
          loop = ast.release()->toLoop();
          break;
        }
      }
      assert(loop);
      auto node = Label::build(BUILDER, LOC((yyloc)), (yyvsp[-1].uniqueStr), toOwned(loop));
      (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
    } else {
      const char* msg = "can only label for-, while-do- "
                        "and do-while-statements";
      auto err = context->raiseError((yyloc), msg);
      (yyval.commentsAndStmt) = context->finishStmt(err);
    }
  }
#line 6709 "bison-chpl-lib.cpp"
    break;

  case 34: /* stmt: TLOCAL expr do_stmt  */
#line 801 "chpl.ypp"
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
#line 6725 "bison-chpl-lib.cpp"
    break;

  case 35: /* stmt: TLOCAL do_stmt  */
#line 813 "chpl.ypp"
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
#line 6740 "bison-chpl-lib.cpp"
    break;

  case 36: /* stmt: TON expr do_stmt  */
#line 824 "chpl.ypp"
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
#line 6756 "bison-chpl-lib.cpp"
    break;

  case 37: /* stmt: TSERIAL expr do_stmt  */
#line 836 "chpl.ypp"
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
#line 6772 "bison-chpl-lib.cpp"
    break;

  case 38: /* stmt: TSERIAL do_stmt  */
#line 848 "chpl.ypp"
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
#line 6787 "bison-chpl-lib.cpp"
    break;

  case 39: /* stmt: TSYNC stmt  */
#line 859 "chpl.ypp"
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
#line 6804 "bison-chpl-lib.cpp"
    break;

  case 40: /* stmt: TYIELD expr TSEMI  */
#line 872 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6815 "bison-chpl-lib.cpp"
    break;

  case 41: /* stmt: error TSEMI  */
#line 879 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
  }
#line 6823 "bison-chpl-lib.cpp"
    break;

  case 43: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 887 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 889 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6835 "bison-chpl-lib.cpp"
    break;

  case 47: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 899 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));

      ModuleParts parts = {context->gatherComments(loc), (yyvsp[-3].visibilityTag), (yyvsp[-2].moduleKind), (yyvsp[0].uniqueStr)};
      (yyval.moduleParts) = parts;
      context->resetDeclState();
      context->clearComments();
    }
#line 6849 "bison-chpl-lib.cpp"
    break;

  case 48: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 912 "chpl.ypp"
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
#line 6866 "bison-chpl-lib.cpp"
    break;

  case 49: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 925 "chpl.ypp"
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
#line 6883 "bison-chpl-lib.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 938 "chpl.ypp"
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
#line 6898 "bison-chpl-lib.cpp"
    break;

  case 51: /* access_control: %empty  */
#line 951 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6904 "bison-chpl-lib.cpp"
    break;

  case 52: /* access_control: TPUBLIC  */
#line 952 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6911 "bison-chpl-lib.cpp"
    break;

  case 53: /* access_control: TPRIVATE  */
#line 954 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6918 "bison-chpl-lib.cpp"
    break;

  case 54: /* opt_prototype: %empty  */
#line 959 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6924 "bison-chpl-lib.cpp"
    break;

  case 55: /* opt_prototype: TPROTOTYPE  */
#line 960 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6931 "bison-chpl-lib.cpp"
    break;

  case 56: /* include_module_stmt: TINCLUDE access_control opt_prototype TMODULE ident_def TSEMI  */
#line 966 "chpl.ypp"
   {
     (yyval.commentsAndStmt) = TODOSTMT((yyloc));
     context->visibility = Decl::DEFAULT_VISIBILITY;
   }
#line 6940 "bison-chpl-lib.cpp"
    break;

  case 57: /* block_stmt_body: TLCBR TRCBR  */
#line 983 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6948 "bison-chpl-lib.cpp"
    break;

  case 58: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 987 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6956 "bison-chpl-lib.cpp"
    break;

  case 59: /* block_stmt_body: TLCBR error TRCBR  */
#line 991 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6968 "bison-chpl-lib.cpp"
    break;

  case 60: /* block_stmt: block_stmt_body  */
#line 1002 "chpl.ypp"
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
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 61: /* stmt_ls: toplevel_stmt  */
#line 1020 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6993 "bison-chpl-lib.cpp"
    break;

  case 62: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1021 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 63: /* renames_ls: expr  */
#line 1026 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7007 "bison-chpl-lib.cpp"
    break;

  case 64: /* renames_ls: expr TAS expr  */
#line 1030 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7016 "bison-chpl-lib.cpp"
    break;

  case 65: /* renames_ls: renames_ls TCOMMA expr  */
#line 1035 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7024 "bison-chpl-lib.cpp"
    break;

  case 66: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1039 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7033 "bison-chpl-lib.cpp"
    break;

  case 67: /* use_renames_ls: expr  */
#line 1049 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(node);
  }
#line 7042 "bison-chpl-lib.cpp"
    break;

  case 68: /* use_renames_ls: expr TAS expr  */
#line 1054 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7052 "bison-chpl-lib.cpp"
    break;

  case 69: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1060 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7063 "bison-chpl-lib.cpp"
    break;

  case 70: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1067 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7072 "bison-chpl-lib.cpp"
    break;

  case 71: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1072 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 72: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1079 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 73: /* opt_only_ls: %empty  */
#line 1090 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 74: /* opt_only_ls: renames_ls  */
#line 1091 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7107 "bison-chpl-lib.cpp"
    break;

  case 75: /* except_ls: TSTAR  */
#line 1096 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.exprList) = context->makeList(ident);
  }
#line 7116 "bison-chpl-lib.cpp"
    break;

  case 76: /* except_ls: renames_ls  */
#line 1101 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[0].exprList);
  }
#line 7124 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_stmt: access_control TUSE use_renames_ls TSEMI  */
#line 1108 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7132 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_stmt: access_control TUSE expr TEXCEPT except_ls TSEMI  */
#line 1112 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7143 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_stmt: access_control TUSE expr TAS expr TEXCEPT except_ls TSEMI  */
#line 1119 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7155 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TEXCEPT except_ls TSEMI  */
#line 1127 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7168 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1136 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7179 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1143 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1151 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7204 "bison-chpl-lib.cpp"
    break;

  case 84: /* import_stmt: access_control TIMPORT import_ls TSEMI  */
#line 1163 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7212 "bison-chpl-lib.cpp"
    break;

  case 85: /* import_expr: expr  */
#line 1170 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
  }
#line 7220 "bison-chpl-lib.cpp"
    break;

  case 86: /* import_expr: expr TAS ident_use  */
#line 1174 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as));
  }
#line 7230 "bison-chpl-lib.cpp"
    break;

  case 87: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1180 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)));
  }
#line 7240 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_ls: import_expr  */
#line 1188 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7246 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_ls: import_ls TCOMMA import_expr  */
#line 1189 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7252 "bison-chpl-lib.cpp"
    break;

  case 90: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1194 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7262 "bison-chpl-lib.cpp"
    break;

  case 91: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1203 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7270 "bison-chpl-lib.cpp"
    break;

  case 92: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1207 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7278 "bison-chpl-lib.cpp"
    break;

  case 93: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1211 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7286 "bison-chpl-lib.cpp"
    break;

  case 94: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1215 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7294 "bison-chpl-lib.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1219 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7302 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1223 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7310 "bison-chpl-lib.cpp"
    break;

  case 97: /* opt_label_ident: %empty  */
#line 1231 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7316 "bison-chpl-lib.cpp"
    break;

  case 98: /* opt_label_ident: TIDENT  */
#line 1232 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7322 "bison-chpl-lib.cpp"
    break;

  case 99: /* ident_fn_def: TIDENT  */
#line 1236 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7328 "bison-chpl-lib.cpp"
    break;

  case 100: /* ident_fn_def: TNONE  */
#line 1237 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7334 "bison-chpl-lib.cpp"
    break;

  case 101: /* ident_fn_def: TTHIS  */
#line 1238 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7340 "bison-chpl-lib.cpp"
    break;

  case 102: /* ident_fn_def: TFALSE  */
#line 1239 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7346 "bison-chpl-lib.cpp"
    break;

  case 103: /* ident_fn_def: TTRUE  */
#line 1240 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7352 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: internal_type_ident_def  */
#line 1241 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7358 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_def: TIDENT  */
#line 1244 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7364 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_def: TNONE  */
#line 1245 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7370 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_def: TTHIS  */
#line 1246 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'this'"); }
#line 7376 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_def: TFALSE  */
#line 1247 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7382 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_def: TTRUE  */
#line 1248 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7388 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: internal_type_ident_def  */
#line 1249 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7394 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_use: TIDENT  */
#line 1261 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7400 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_use: TTHIS  */
#line 1262 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7406 "bison-chpl-lib.cpp"
    break;

  case 132: /* scalar_type: TBOOL  */
#line 1295 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7412 "bison-chpl-lib.cpp"
    break;

  case 133: /* scalar_type: TENUM  */
#line 1296 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7418 "bison-chpl-lib.cpp"
    break;

  case 134: /* scalar_type: TINT  */
#line 1297 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7424 "bison-chpl-lib.cpp"
    break;

  case 135: /* scalar_type: TUINT  */
#line 1298 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7430 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TREAL  */
#line 1299 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7436 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TIMAG  */
#line 1300 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7442 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TCOMPLEX  */
#line 1301 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7448 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TBYTES  */
#line 1302 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7454 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TSTRING  */
#line 1303 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7460 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TLOCALE  */
#line 1304 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7466 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TNOTHING  */
#line 1305 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7472 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TVOID  */
#line 1306 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7478 "bison-chpl-lib.cpp"
    break;

  case 148: /* do_stmt: TDO stmt  */
#line 1320 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7484 "bison-chpl-lib.cpp"
    break;

  case 149: /* do_stmt: block_stmt  */
#line 1321 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7490 "bison-chpl-lib.cpp"
    break;

  case 150: /* return_stmt: TRETURN TSEMI  */
#line 1326 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7501 "bison-chpl-lib.cpp"
    break;

  case 151: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1333 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7512 "bison-chpl-lib.cpp"
    break;

  case 153: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 1344 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7518 "bison-chpl-lib.cpp"
    break;

  case 154: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 1346 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7524 "bison-chpl-lib.cpp"
    break;

  case 155: /* class_level_stmt: TSEMI  */
#line 1351 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), nullptr);
    }
#line 7532 "bison-chpl-lib.cpp"
    break;

  case 156: /* class_level_stmt: inner_class_level_stmt  */
#line 1355 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7542 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@1: %empty  */
#line 1360 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7549 "bison-chpl-lib.cpp"
    break;

  case 158: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1362 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@2: %empty  */
#line 1366 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7565 "bison-chpl-lib.cpp"
    break;

  case 160: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1368 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7574 "bison-chpl-lib.cpp"
    break;

  case 168: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1386 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-2].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr
      (yyval.commentsAndStmt) = cs;
    }
#line 7585 "bison-chpl-lib.cpp"
    break;

  case 169: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT except_ls TSEMI  */
#line 1393 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr and except_ls
      (yyval.commentsAndStmt) = cs;
    }
#line 7596 "bison-chpl-lib.cpp"
    break;

  case 170: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1400 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr and opt_only_ls
      (yyval.commentsAndStmt) = cs;
    }
#line 7607 "bison-chpl-lib.cpp"
    break;

  case 171: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1407 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-1].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save var_decl_stmt
      (yyval.commentsAndStmt) = cs;
    }
#line 7618 "bison-chpl-lib.cpp"
    break;

  case 172: /* forwarding_decl_start: TFORWARDING  */
#line 1417 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7626 "bison-chpl-lib.cpp"
    break;

  case 173: /* extern_or_export: TEXTERN  */
#line 1423 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7632 "bison-chpl-lib.cpp"
    break;

  case 174: /* extern_or_export: TEXPORT  */
#line 1424 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7638 "bison-chpl-lib.cpp"
    break;

  case 175: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1429 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 176: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1439 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7657 "bison-chpl-lib.cpp"
    break;

  case 177: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1445 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7668 "bison-chpl-lib.cpp"
    break;

  case 178: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1452 "chpl.ypp"
  {
    auto loc = context->declStartLoc((yylsp[-2]));

    Expression* optExpr = (yyvsp[-1].expr);
    FunctionParts fp = (yyvsp[0].functionParts);

    auto commentList = context->appendList(context->makeList(),
                                           fp.comments);

    auto comments = context->gatherCommentsFromList(commentList, loc);
    delete commentList;

    fp.linkage = (yyvsp[-2].linkageTag);
    fp.comments = comments;
    fp.linkageNameExpr = optExpr;

    (yyval.commentsAndStmt) = context->buildFunctionDecl((yyloc), fp);
  }
#line 7691 "bison-chpl-lib.cpp"
    break;

  case 179: /* $@3: %empty  */
#line 1471 "chpl.ypp"
  {
    // This will be consumed when building the first variable in the list.
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7700 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1476 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDecl((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7709 "bison-chpl-lib.cpp"
    break;

  case 181: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1484 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7717 "bison-chpl-lib.cpp"
    break;

  case 182: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1491 "chpl.ypp"
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
#line 7739 "bison-chpl-lib.cpp"
    break;

  case 183: /* loop_stmt: TWHILE expr do_stmt  */
#line 1509 "chpl.ypp"
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
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 184: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1522 "chpl.ypp"
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
#line 7773 "bison-chpl-lib.cpp"
    break;

  case 185: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1535 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7781 "bison-chpl-lib.cpp"
    break;

  case 186: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1539 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7789 "bison-chpl-lib.cpp"
    break;

  case 187: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1543 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7797 "bison-chpl-lib.cpp"
    break;

  case 188: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1547 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7805 "bison-chpl-lib.cpp"
    break;

  case 189: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1551 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7813 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TFOR expr do_stmt  */
#line 1555 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7821 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1559 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7829 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1563 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-5]), (yylsp[0]), (yyvsp[0].blockOrDo));
    Expression* ident = context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr));
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
#line 7851 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1581 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7859 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1585 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7867 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1589 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7875 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1593 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7883 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFORALL expr do_stmt  */
#line 1597 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7891 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1601 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7899 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1605 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7907 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1609 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7915 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1613 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7923 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1617 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7931 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1621 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7939 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1625 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7947 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1629 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7955 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1633 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7963 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1637 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7971 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1641 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7979 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1645 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7987 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1649 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7995 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1653 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8003 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1657 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8011 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1661 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8019 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1665 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8027 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1669 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8036 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1674 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8045 "bison-chpl-lib.cpp"
    break;

  case 217: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1682 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 218: /* if_stmt: TIF expr TTHEN stmt  */
#line 1691 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8063 "bison-chpl-lib.cpp"
    break;

  case 219: /* if_stmt: TIF expr block_stmt  */
#line 1695 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8071 "bison-chpl-lib.cpp"
    break;

  case 220: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8079 "bison-chpl-lib.cpp"
    break;

  case 221: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8087 "bison-chpl-lib.cpp"
    break;

  case 222: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1707 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8095 "bison-chpl-lib.cpp"
    break;

  case 223: /* if_stmt: TIF ifvar block_stmt  */
#line 1711 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8103 "bison-chpl-lib.cpp"
    break;

  case 224: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1715 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8111 "bison-chpl-lib.cpp"
    break;

  case 225: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1719 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8119 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1723 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8128 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1728 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8137 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1733 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8146 "bison-chpl-lib.cpp"
    break;

  case 229: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1738 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8155 "bison-chpl-lib.cpp"
    break;

  case 230: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1746 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr),
                                   Decl::DEFAULT_VISIBILITY,
                                   Decl::DEFAULT_LINKAGE,
                                   /*linkageName*/ nullptr,
                                   Variable::VAR,
                                   /*isConfig*/ false,
                                   context->currentScopeIsAggregate(),
                                   /*typeExpression*/ nullptr,
                                   toOwned((yyvsp[0].expr)));
    (yyval.expr) = varDecl.release();
  }
#line 8172 "bison-chpl-lib.cpp"
    break;

  case 231: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1759 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr),
                                   Decl::DEFAULT_VISIBILITY,
                                   Decl::DEFAULT_LINKAGE,
                                   /*linkageName*/ nullptr,
                                   Variable::CONST,
                                   /*isConfig*/ false,
                                   context->currentScopeIsAggregate(),
                                   /*typeExpression*/ nullptr,
                                   toOwned((yyvsp[0].expr)));
    (yyval.expr) = varDecl.release();
  }
#line 8189 "bison-chpl-lib.cpp"
    break;

  case 232: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1775 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8195 "bison-chpl-lib.cpp"
    break;

  case 233: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1777 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8201 "bison-chpl-lib.cpp"
    break;

  case 234: /* ifc_formal_ls: ifc_formal  */
#line 1781 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8207 "bison-chpl-lib.cpp"
    break;

  case 235: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1782 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 236: /* ifc_formal: ident_def  */
#line 1787 "chpl.ypp"
             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8219 "bison-chpl-lib.cpp"
    break;

  case 249: /* implements_type_ident: implements_type_error_ident  */
#line 1805 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 256: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1834 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8237 "bison-chpl-lib.cpp"
    break;

  case 257: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1836 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8243 "bison-chpl-lib.cpp"
    break;

  case 258: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1838 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8249 "bison-chpl-lib.cpp"
    break;

  case 259: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1843 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8255 "bison-chpl-lib.cpp"
    break;

  case 260: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1845 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8261 "bison-chpl-lib.cpp"
    break;

  case 261: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1847 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8267 "bison-chpl-lib.cpp"
    break;

  case 262: /* try_stmt: TTRY expr TSEMI  */
#line 1852 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[-1].expr), false);
  }
#line 8275 "bison-chpl-lib.cpp"
    break;

  case 263: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1856 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[-1].expr), true);
  }
#line 8283 "bison-chpl-lib.cpp"
    break;

  case 264: /* try_stmt: TTRY assignment_stmt  */
#line 1860 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8291 "bison-chpl-lib.cpp"
    break;

  case 265: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1864 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8299 "bison-chpl-lib.cpp"
    break;

  case 266: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1868 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8307 "bison-chpl-lib.cpp"
    break;

  case 267: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1872 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8315 "bison-chpl-lib.cpp"
    break;

  case 268: /* catch_expr_ls: %empty  */
#line 1878 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8321 "bison-chpl-lib.cpp"
    break;

  case 269: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1879 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8327 "bison-chpl-lib.cpp"
    break;

  case 270: /* catch_expr: TCATCH block_stmt  */
#line 1884 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8335 "bison-chpl-lib.cpp"
    break;

  case 271: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1888 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8343 "bison-chpl-lib.cpp"
    break;

  case 272: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1892 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8351 "bison-chpl-lib.cpp"
    break;

  case 273: /* catch_expr_inner: ident_def  */
#line 1899 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[0].uniqueStr), Decl::DEFAULT_VISIBILITY,
                         Decl::DEFAULT_LINKAGE,
                         /*linkageName*/ nullptr,
                         Variable::VAR,
                         /*isConfig*/ false,
                         /*isField*/ false,
                         /*typeExpression*/ nullptr,
                         /*initExpression*/ nullptr).release();
  }
#line 8366 "bison-chpl-lib.cpp"
    break;

  case 274: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1910 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), Decl::DEFAULT_VISIBILITY,
                         Decl::DEFAULT_LINKAGE,
                         /*linkageName*/ nullptr,
                         Variable::VAR,
                         /*isConfig*/ false,
                         /*isField*/ false,
                         /*typeExpression*/ toOwned((yyvsp[0].expr)),
                         /*initExpression*/ nullptr).release();
  }
#line 8381 "bison-chpl-lib.cpp"
    break;

  case 275: /* throw_stmt: TTHROW expr TSEMI  */
#line 1924 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8391 "bison-chpl-lib.cpp"
    break;

  case 276: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1932 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8399 "bison-chpl-lib.cpp"
    break;

  case 277: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1936 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8409 "bison-chpl-lib.cpp"
    break;

  case 278: /* when_stmt_ls: %empty  */
#line 1944 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8415 "bison-chpl-lib.cpp"
    break;

  case 279: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1945 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8421 "bison-chpl-lib.cpp"
    break;

  case 280: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1950 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8429 "bison-chpl-lib.cpp"
    break;

  case 281: /* when_stmt: TOTHERWISE stmt  */
#line 1954 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8438 "bison-chpl-lib.cpp"
    break;

  case 282: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1959 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8447 "bison-chpl-lib.cpp"
    break;

  case 283: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1969 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8456 "bison-chpl-lib.cpp"
    break;

  case 284: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1974 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8467 "bison-chpl-lib.cpp"
    break;

  case 285: /* class_start: class_tag ident_def  */
#line 1985 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8475 "bison-chpl-lib.cpp"
    break;

  case 286: /* class_tag: TCLASS  */
#line 1991 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8481 "bison-chpl-lib.cpp"
    break;

  case 287: /* class_tag: TRECORD  */
#line 1992 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8487 "bison-chpl-lib.cpp"
    break;

  case 288: /* class_tag: TUNION  */
#line 1993 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8493 "bison-chpl-lib.cpp"
    break;

  case 289: /* opt_inherit: %empty  */
#line 1997 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8499 "bison-chpl-lib.cpp"
    break;

  case 290: /* opt_inherit: TCOLON expr_ls  */
#line 1998 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8505 "bison-chpl-lib.cpp"
    break;

  case 291: /* class_level_stmt_ls: %empty  */
#line 2003 "chpl.ypp"
    { (yyval.exprList) = context->makeList(); }
#line 8511 "bison-chpl-lib.cpp"
    break;

  case 292: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2005 "chpl.ypp"
    { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8517 "bison-chpl-lib.cpp"
    break;

  case 293: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2007 "chpl.ypp"
    { context->appendList((yyvsp[-2].exprList), TODOEXPR((yyloc))); }
#line 8523 "bison-chpl-lib.cpp"
    break;

  case 294: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2012 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      ParserExprList* list = (yyvsp[-1].exprList);
      // get any comments after the last element but before the closing brace
      context->appendList(list, context->gatherComments((yylsp[0])));

      auto decl = Enum::build(BUILDER, LOC((yyloc)),
                              parts.name, parts.visibility,
                              context->consumeList(list));
      CommentsAndStmt cs = {parts.comments, decl.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8543 "bison-chpl-lib.cpp"
    break;

  case 295: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2028 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8557 "bison-chpl-lib.cpp"
    break;

  case 296: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2041 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8565 "bison-chpl-lib.cpp"
    break;

  case 297: /* enum_ls: deprecated_enum_item  */
#line 2048 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 8573 "bison-chpl-lib.cpp"
    break;

  case 298: /* enum_ls: enum_ls TCOMMA  */
#line 2052 "chpl.ypp"
    {
      (yyval.exprList) = (yyvsp[-1].exprList);
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8582 "bison-chpl-lib.cpp"
    break;

  case 299: /* $@4: %empty  */
#line 2056 "chpl.ypp"
                 {context->clearCommentsBefore((yylsp[0]));}
#line 8588 "bison-chpl-lib.cpp"
    break;

  case 300: /* enum_ls: enum_ls TCOMMA $@4 deprecated_enum_item  */
#line 2057 "chpl.ypp"
    {
      context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 302: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 2065 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 303: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 2067 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8608 "bison-chpl-lib.cpp"
    break;

  case 304: /* enum_item: ident_def  */
#line 2072 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)), (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8617 "bison-chpl-lib.cpp"
    break;

  case 305: /* enum_item: ident_def TASSIGN expr  */
#line 2077 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8627 "bison-chpl-lib.cpp"
    break;

  case 306: /* lambda_decl_expr: TLAMBDA req_formal_ls opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 2086 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 8635 "bison-chpl-lib.cpp"
    break;

  case 308: /* linkage_spec: linkage_spec_empty  */
#line 2095 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8641 "bison-chpl-lib.cpp"
    break;

  case 309: /* linkage_spec: TINLINE  */
#line 2096 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8648 "bison-chpl-lib.cpp"
    break;

  case 310: /* linkage_spec: TOVERRIDE  */
#line 2098 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8655 "bison-chpl-lib.cpp"
    break;

  case 311: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2104 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8663 "bison-chpl-lib.cpp"
    break;

  case 312: /* $@5: %empty  */
#line 2113 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name);
    }
#line 8673 "bison-chpl-lib.cpp"
    break;

  case 313: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@5 opt_function_body_stmt  */
#line 2119 "chpl.ypp"
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
      context->exitScope(asttags::Function, (yyvsp[-6].functionParts).name);

      (yyval.functionParts) = fp;
    }
#line 8694 "bison-chpl-lib.cpp"
    break;

  case 314: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2139 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8706 "bison-chpl-lib.cpp"
    break;

  case 315: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2147 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8718 "bison-chpl-lib.cpp"
    break;

  case 316: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2155 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])),
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8733 "bison-chpl-lib.cpp"
    break;

  case 317: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2166 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])),
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8748 "bison-chpl-lib.cpp"
    break;

  case 318: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2177 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8758 "bison-chpl-lib.cpp"
    break;

  case 319: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2186 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-1].functionParts);
      auto loc = context->declStartLoc((yylsp[0]));
      fp.comments = context->gatherComments(loc);
      fp.kind = (yyvsp[0].functionKind);
      (yyval.functionParts) = fp;
    }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 321: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2197 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 349: /* fn_ident: ident_def TBANG  */
#line 2229 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8786 "bison-chpl-lib.cpp"
    break;

  case 362: /* opt_formal_ls: %empty  */
#line 2252 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8792 "bison-chpl-lib.cpp"
    break;

  case 363: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2253 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8798 "bison-chpl-lib.cpp"
    break;

  case 364: /* req_formal_ls: TLP formal_ls TRP  */
#line 2257 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8804 "bison-chpl-lib.cpp"
    break;

  case 365: /* formal_ls_inner: formal  */
#line 2261 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8810 "bison-chpl-lib.cpp"
    break;

  case 366: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2262 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8816 "bison-chpl-lib.cpp"
    break;

  case 367: /* formal_ls: %empty  */
#line 2266 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8822 "bison-chpl-lib.cpp"
    break;

  case 368: /* formal_ls: formal_ls_inner  */
#line 2267 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8828 "bison-chpl-lib.cpp"
    break;

  case 369: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2272 "chpl.ypp"
    { (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), (yyvsp[-3].intentTag),
                         toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr))).release(); }
#line 8835 "bison-chpl-lib.cpp"
    break;

  case 370: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2275 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8841 "bison-chpl-lib.cpp"
    break;

  case 371: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 2277 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8847 "bison-chpl-lib.cpp"
    break;

  case 372: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 2279 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8853 "bison-chpl-lib.cpp"
    break;

  case 373: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2281 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8859 "bison-chpl-lib.cpp"
    break;

  case 374: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 2283 "chpl.ypp"
    {
      (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
    }
#line 8867 "bison-chpl-lib.cpp"
    break;

  case 375: /* opt_intent_tag: %empty  */
#line 2289 "chpl.ypp"
                      { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8873 "bison-chpl-lib.cpp"
    break;

  case 376: /* opt_intent_tag: required_intent_tag  */
#line 2290 "chpl.ypp"
                      { (yyval.intentTag) = (yyvsp[0].intentTag); }
#line 8879 "bison-chpl-lib.cpp"
    break;

  case 377: /* required_intent_tag: TIN  */
#line 2294 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8885 "bison-chpl-lib.cpp"
    break;

  case 378: /* required_intent_tag: TINOUT  */
#line 2295 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8891 "bison-chpl-lib.cpp"
    break;

  case 379: /* required_intent_tag: TOUT  */
#line 2296 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8897 "bison-chpl-lib.cpp"
    break;

  case 380: /* required_intent_tag: TCONST TIN  */
#line 2297 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8903 "bison-chpl-lib.cpp"
    break;

  case 381: /* required_intent_tag: TCONST TREF  */
#line 2298 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8909 "bison-chpl-lib.cpp"
    break;

  case 382: /* required_intent_tag: TCONST  */
#line 2299 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8915 "bison-chpl-lib.cpp"
    break;

  case 383: /* required_intent_tag: TPARAM  */
#line 2300 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8921 "bison-chpl-lib.cpp"
    break;

  case 384: /* required_intent_tag: TREF  */
#line 2301 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8927 "bison-chpl-lib.cpp"
    break;

  case 385: /* required_intent_tag: TTYPE  */
#line 2302 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8933 "bison-chpl-lib.cpp"
    break;

  case 386: /* opt_this_intent_tag: %empty  */
#line 2306 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8939 "bison-chpl-lib.cpp"
    break;

  case 387: /* opt_this_intent_tag: TPARAM  */
#line 2307 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8945 "bison-chpl-lib.cpp"
    break;

  case 388: /* opt_this_intent_tag: TREF  */
#line 2308 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8951 "bison-chpl-lib.cpp"
    break;

  case 389: /* opt_this_intent_tag: TCONST TREF  */
#line 2309 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8957 "bison-chpl-lib.cpp"
    break;

  case 390: /* opt_this_intent_tag: TCONST  */
#line 2310 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8963 "bison-chpl-lib.cpp"
    break;

  case 391: /* opt_this_intent_tag: TTYPE  */
#line 2311 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8969 "bison-chpl-lib.cpp"
    break;

  case 392: /* proc_iter_or_op: TPROC  */
#line 2315 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8975 "bison-chpl-lib.cpp"
    break;

  case 393: /* proc_iter_or_op: TITER  */
#line 2316 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8981 "bison-chpl-lib.cpp"
    break;

  case 394: /* proc_iter_or_op: TOPERATOR  */
#line 2317 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8987 "bison-chpl-lib.cpp"
    break;

  case 395: /* opt_ret_tag: %empty  */
#line 2321 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8993 "bison-chpl-lib.cpp"
    break;

  case 396: /* opt_ret_tag: TCONST  */
#line 2322 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 8999 "bison-chpl-lib.cpp"
    break;

  case 397: /* opt_ret_tag: TCONST TREF  */
#line 2323 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 9005 "bison-chpl-lib.cpp"
    break;

  case 398: /* opt_ret_tag: TREF  */
#line 2324 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 9011 "bison-chpl-lib.cpp"
    break;

  case 399: /* opt_ret_tag: TPARAM  */
#line 2325 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9017 "bison-chpl-lib.cpp"
    break;

  case 400: /* opt_ret_tag: TTYPE  */
#line 2326 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9023 "bison-chpl-lib.cpp"
    break;

  case 401: /* opt_throws_error: %empty  */
#line 2330 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9029 "bison-chpl-lib.cpp"
    break;

  case 402: /* opt_throws_error: TTHROWS  */
#line 2331 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9035 "bison-chpl-lib.cpp"
    break;

  case 403: /* opt_function_body_stmt: TSEMI  */
#line 2334 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9041 "bison-chpl-lib.cpp"
    break;

  case 404: /* opt_function_body_stmt: function_body_stmt  */
#line 2335 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9047 "bison-chpl-lib.cpp"
    break;

  case 405: /* function_body_stmt: block_stmt_body  */
#line 2339 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9053 "bison-chpl-lib.cpp"
    break;

  case 406: /* function_body_stmt: return_stmt  */
#line 2340 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9059 "bison-chpl-lib.cpp"
    break;

  case 407: /* query_expr: TQUERIEDIDENT  */
#line 2344 "chpl.ypp"
                      { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9065 "bison-chpl-lib.cpp"
    break;

  case 408: /* var_arg_expr: TDOTDOTDOT  */
#line 2348 "chpl.ypp"
                         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9071 "bison-chpl-lib.cpp"
    break;

  case 409: /* var_arg_expr: TDOTDOTDOT expr  */
#line 2349 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9077 "bison-chpl-lib.cpp"
    break;

  case 410: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 2350 "chpl.ypp"
                         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9083 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_lifetime_where: %empty  */
#line 2354 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9089 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_lifetime_where: TWHERE expr  */
#line 2356 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9095 "bison-chpl-lib.cpp"
    break;

  case 413: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2358 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9101 "bison-chpl-lib.cpp"
    break;

  case 414: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2360 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9107 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2362 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9113 "bison-chpl-lib.cpp"
    break;

  case 416: /* lifetime_components_expr: lifetime_expr  */
#line 2367 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9119 "bison-chpl-lib.cpp"
    break;

  case 417: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2369 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9125 "bison-chpl-lib.cpp"
    break;

  case 418: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2374 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9131 "bison-chpl-lib.cpp"
    break;

  case 419: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2376 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9137 "bison-chpl-lib.cpp"
    break;

  case 420: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2378 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9143 "bison-chpl-lib.cpp"
    break;

  case 421: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2380 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9149 "bison-chpl-lib.cpp"
    break;

  case 422: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2382 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9155 "bison-chpl-lib.cpp"
    break;

  case 423: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2384 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9161 "bison-chpl-lib.cpp"
    break;

  case 424: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2386 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9167 "bison-chpl-lib.cpp"
    break;

  case 425: /* lifetime_ident: TIDENT  */
#line 2390 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9173 "bison-chpl-lib.cpp"
    break;

  case 426: /* lifetime_ident: TTHIS  */
#line 2391 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9179 "bison-chpl-lib.cpp"
    break;

  case 427: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2396 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDecl((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9188 "bison-chpl-lib.cpp"
    break;

  case 428: /* type_alias_decl_stmt_start: TTYPE  */
#line 2404 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9196 "bison-chpl-lib.cpp"
    break;

  case 429: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2408 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9205 "bison-chpl-lib.cpp"
    break;

  case 430: /* type_alias_decl_stmt_start: TEXTERN TTYPE  */
#line 2413 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteLinkage(Decl::EXTERN);
  }
#line 9214 "bison-chpl-lib.cpp"
    break;

  case 431: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2421 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 432: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner TCOMMA type_alias_decl_stmt_inner_ls  */
#line 2425 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[0].exprList), (yyvsp[-2].commentsAndStmt));
  }
#line 9230 "bison-chpl-lib.cpp"
    break;

  case 433: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2432 "chpl.ypp"
  {
    // TODO (dlongnecke-cray): Add a helper to build this and var_decl_stmt.
    auto node = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[-1].uniqueStr),
                                context->visibility,
                                context->linkage,
                                context->consumeVarDeclLinkageName(),
                                context->varDeclKind,
                                context->isVarDeclConfig,
                                context->currentScopeIsAggregate(),
                                /*typeExpression*/ nullptr,
                                toOwned((yyvsp[0].expr)));
      // Gather the comments like a statement does.
      (yyval.commentsAndStmt) = STMT((yylsp[-1]), node.release());
      context->clearComments();
  }
#line 9250 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_init_type: %empty  */
#line 2450 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9256 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_init_type: TASSIGN type_level_expr  */
#line 2452 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9262 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_init_type: TASSIGN array_type  */
#line 2454 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9271 "bison-chpl-lib.cpp"
    break;

  case 437: /* var_decl_type: TPARAM  */
#line 2461 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 438: /* var_decl_type: TCONST TREF  */
#line 2462 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9283 "bison-chpl-lib.cpp"
    break;

  case 439: /* var_decl_type: TREF  */
#line 2463 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9289 "bison-chpl-lib.cpp"
    break;

  case 440: /* var_decl_type: TCONST  */
#line 2464 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9295 "bison-chpl-lib.cpp"
    break;

  case 441: /* var_decl_type: TVAR  */
#line 2465 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 442: /* $@6: %empty  */
#line 2470 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9310 "bison-chpl-lib.cpp"
    break;

  case 443: /* var_decl_stmt: TCONFIG $@6 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2474 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDecl((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9319 "bison-chpl-lib.cpp"
    break;

  case 444: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2479 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDecl((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9328 "bison-chpl-lib.cpp"
    break;

  case 445: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2487 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9336 "bison-chpl-lib.cpp"
    break;

  case 446: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2491 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9344 "bison-chpl-lib.cpp"
    break;

  case 447: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2498 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr),
                                     context->visibility,
                                     context->linkage,
                                     context->consumeVarDeclLinkageName(),
                                     context->varDeclKind,
                                     context->isVarDeclConfig,
                                     context->currentScopeIsAggregate(),
                                     toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr)));
      // gather the comments like a statement does
      (yyval.commentsAndStmt) = STMT((yylsp[-2]), varDecl.release());
      context->clearComments();
    }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 448: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2512 "chpl.ypp"
    {
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        context->visibility,
                                        context->linkage,
                                        context->varDeclKind,
                                        context->consumeList((yyvsp[-3].exprList)),
                                        toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yylsp[-4]), tupleDecl.release());
      context->clearComments();
    }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 449: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2526 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[0].uniqueStr),
                                     context->visibility,
                                     context->linkage,
                                     context->consumeVarDeclLinkageName(),
                                     context->varDeclKind,
                                     context->isVarDeclConfig,
                                     context->currentScopeIsAggregate(),
                                     /*typeExpression*/ nullptr,
                                     /*initExpression*/ nullptr);
      (yyval.expr) = varDecl.release();
    }
#line 9394 "bison-chpl-lib.cpp"
    break;

  case 450: /* tuple_var_decl_component: ident_def  */
#line 2540 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[0].uniqueStr),
                                     context->visibility,
                                     context->linkage,
                                     context->consumeVarDeclLinkageName(),
                                     context->varDeclKind,
                                     context->isVarDeclConfig,
                                     context->currentScopeIsAggregate(),
                                     /*typeExpression*/ nullptr,
                                     /*initExpression*/ nullptr);
      (yyval.expr) = varDecl.release();
    }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 451: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2554 "chpl.ypp"
    {
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        context->visibility,
                                        context->linkage,
                                        context->varDeclKind,
                                        context->consumeList((yyvsp[-1].exprList)),
                                        /*typeExpression*/ nullptr,
                                        /*initExpression*/ nullptr);
      (yyval.expr) = tupleDecl.release();
    }
#line 9426 "bison-chpl-lib.cpp"
    break;

  case 452: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2568 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9432 "bison-chpl-lib.cpp"
    break;

  case 453: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2570 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9438 "bison-chpl-lib.cpp"
    break;

  case 454: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2572 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9444 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_init_expr: %empty  */
#line 2578 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9450 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2579 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9456 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2580 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9462 "bison-chpl-lib.cpp"
    break;

  case 458: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2586 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9470 "bison-chpl-lib.cpp"
    break;

  case 459: /* ret_array_type: TLSBR TRSBR  */
#line 2590 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9478 "bison-chpl-lib.cpp"
    break;

  case 460: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2594 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9486 "bison-chpl-lib.cpp"
    break;

  case 461: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2598 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9494 "bison-chpl-lib.cpp"
    break;

  case 462: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2602 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9502 "bison-chpl-lib.cpp"
    break;

  case 463: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2606 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9510 "bison-chpl-lib.cpp"
    break;

  case 464: /* ret_array_type: TLSBR error TRSBR  */
#line 2610 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9518 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_ret_type: %empty  */
#line 2616 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9524 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_ret_type: TCOLON type_level_expr  */
#line 2617 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9530 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_ret_type: TCOLON ret_array_type  */
#line 2618 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9536 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2619 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9542 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_ret_type: error  */
#line 2620 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9548 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_type: %empty  */
#line 2625 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9554 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_type: TCOLON type_level_expr  */
#line 2626 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9560 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_type: TCOLON array_type  */
#line 2627 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9566 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2628 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_type: error  */
#line 2629 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9578 "bison-chpl-lib.cpp"
    break;

  case 475: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2650 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9586 "bison-chpl-lib.cpp"
    break;

  case 476: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2654 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9594 "bison-chpl-lib.cpp"
    break;

  case 477: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2658 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9602 "bison-chpl-lib.cpp"
    break;

  case 478: /* array_type: TLSBR error TRSBR  */
#line 2662 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 9610 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_formal_array_elt_type: %empty  */
#line 2668 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 480: /* opt_formal_array_elt_type: type_level_expr  */
#line 2669 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9622 "bison-chpl-lib.cpp"
    break;

  case 481: /* opt_formal_array_elt_type: query_expr  */
#line 2670 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9628 "bison-chpl-lib.cpp"
    break;

  case 482: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2675 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9637 "bison-chpl-lib.cpp"
    break;

  case 483: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2680 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 484: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2688 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9654 "bison-chpl-lib.cpp"
    break;

  case 485: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2693 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9662 "bison-chpl-lib.cpp"
    break;

  case 486: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2697 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9670 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_formal_type: %empty  */
#line 2703 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9676 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_formal_type: TCOLON type_level_expr  */
#line 2704 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_formal_type: TCOLON query_expr  */
#line 2705 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9688 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2706 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9694 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_formal_type: TCOLON formal_array_type  */
#line 2707 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9700 "bison-chpl-lib.cpp"
    break;

  case 492: /* expr_ls: expr  */
#line 2713 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9706 "bison-chpl-lib.cpp"
    break;

  case 493: /* expr_ls: query_expr  */
#line 2714 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9712 "bison-chpl-lib.cpp"
    break;

  case 494: /* expr_ls: expr_ls TCOMMA expr  */
#line 2715 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9718 "bison-chpl-lib.cpp"
    break;

  case 495: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2716 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9724 "bison-chpl-lib.cpp"
    break;

  case 496: /* simple_expr_ls: expr  */
#line 2720 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9730 "bison-chpl-lib.cpp"
    break;

  case 497: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2721 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9736 "bison-chpl-lib.cpp"
    break;

  case 498: /* tuple_component: TUNDERSCORE  */
#line 2725 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9742 "bison-chpl-lib.cpp"
    break;

  case 499: /* tuple_component: opt_try_expr  */
#line 2726 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9748 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_component: query_expr  */
#line 2727 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9754 "bison-chpl-lib.cpp"
    break;

  case 501: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2732 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9762 "bison-chpl-lib.cpp"
    break;

  case 502: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2736 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9770 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_actual_ls: %empty  */
#line 2742 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9776 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_actual_ls: actual_ls  */
#line 2743 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9782 "bison-chpl-lib.cpp"
    break;

  case 505: /* actual_ls: actual_expr  */
#line 2748 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9791 "bison-chpl-lib.cpp"
    break;

  case 506: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2753 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9801 "bison-chpl-lib.cpp"
    break;

  case 507: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2761 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9807 "bison-chpl-lib.cpp"
    break;

  case 508: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2762 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9813 "bison-chpl-lib.cpp"
    break;

  case 509: /* actual_expr: query_expr  */
#line 2763 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9819 "bison-chpl-lib.cpp"
    break;

  case 510: /* actual_expr: opt_try_expr  */
#line 2764 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9825 "bison-chpl-lib.cpp"
    break;

  case 511: /* ident_expr: ident_use  */
#line 2768 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 512: /* ident_expr: scalar_type  */
#line 2769 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9837 "bison-chpl-lib.cpp"
    break;

  case 513: /* type_level_expr: sub_type_level_expr  */
#line 2781 "chpl.ypp"
    { (yyval.expr) = (yyvsp[0].expr); }
#line 9843 "bison-chpl-lib.cpp"
    break;

  case 514: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2783 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9849 "bison-chpl-lib.cpp"
    break;

  case 515: /* type_level_expr: TQUESTION  */
#line 2785 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9855 "bison-chpl-lib.cpp"
    break;

  case 521: /* sub_type_level_expr: TSINGLE expr  */
#line 2796 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9861 "bison-chpl-lib.cpp"
    break;

  case 522: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2798 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9867 "bison-chpl-lib.cpp"
    break;

  case 523: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2800 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9873 "bison-chpl-lib.cpp"
    break;

  case 524: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2802 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9879 "bison-chpl-lib.cpp"
    break;

  case 525: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2804 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9885 "bison-chpl-lib.cpp"
    break;

  case 526: /* sub_type_level_expr: TATOMIC expr  */
#line 2806 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9891 "bison-chpl-lib.cpp"
    break;

  case 527: /* sub_type_level_expr: TSYNC expr  */
#line 2808 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9897 "bison-chpl-lib.cpp"
    break;

  case 528: /* sub_type_level_expr: TOWNED  */
#line 2811 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9903 "bison-chpl-lib.cpp"
    break;

  case 529: /* sub_type_level_expr: TOWNED expr  */
#line 2813 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9909 "bison-chpl-lib.cpp"
    break;

  case 530: /* sub_type_level_expr: TUNMANAGED  */
#line 2815 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9915 "bison-chpl-lib.cpp"
    break;

  case 531: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2817 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9921 "bison-chpl-lib.cpp"
    break;

  case 532: /* sub_type_level_expr: TSHARED  */
#line 2819 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9927 "bison-chpl-lib.cpp"
    break;

  case 533: /* sub_type_level_expr: TSHARED expr  */
#line 2821 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9933 "bison-chpl-lib.cpp"
    break;

  case 534: /* sub_type_level_expr: TBORROWED  */
#line 2823 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9939 "bison-chpl-lib.cpp"
    break;

  case 535: /* sub_type_level_expr: TBORROWED expr  */
#line 2825 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9945 "bison-chpl-lib.cpp"
    break;

  case 536: /* sub_type_level_expr: TCLASS  */
#line 2828 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9951 "bison-chpl-lib.cpp"
    break;

  case 537: /* sub_type_level_expr: TRECORD  */
#line 2830 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9957 "bison-chpl-lib.cpp"
    break;

  case 538: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2835 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 539: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2844 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 9983 "bison-chpl-lib.cpp"
    break;

  case 540: /* for_expr: TFOR expr TDO expr  */
#line 2853 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[0].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 9995 "bison-chpl-lib.cpp"
    break;

  case 541: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2861 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10001 "bison-chpl-lib.cpp"
    break;

  case 542: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2863 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10007 "bison-chpl-lib.cpp"
    break;

  case 543: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2865 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10013 "bison-chpl-lib.cpp"
    break;

  case 544: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2867 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 545: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2876 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10039 "bison-chpl-lib.cpp"
    break;

  case 546: /* for_expr: TFORALL expr TDO expr  */
#line 2885 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[0].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10051 "bison-chpl-lib.cpp"
    break;

  case 547: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2893 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10057 "bison-chpl-lib.cpp"
    break;

  case 548: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2895 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10063 "bison-chpl-lib.cpp"
    break;

  case 549: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2897 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10069 "bison-chpl-lib.cpp"
    break;

  case 550: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2899 "chpl.ypp"
  {
    if ((yyvsp[-2].exprList)->size() > 1) {
      const char* msg = "Invalid iterand expression";
      (yyval.expr) = context->raiseError((yylsp[-2]), msg);
    } else {
      auto iterandExpr = context->consumeList((yyvsp[-2].exprList))[0].release();
      assert(iterandExpr);
      (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), /*index*/ nullptr,
                              toOwned((yyvsp[0].expr)),
                              /*withClause*/ nullptr,
                              BlockStyle::IMPLICIT,
                              context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                              /*isExpressionLevel*/ true).release();
    }
  }
#line 10089 "bison-chpl-lib.cpp"
    break;

  case 551: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2915 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 552: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2924 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10115 "bison-chpl-lib.cpp"
    break;

  case 553: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2933 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10121 "bison-chpl-lib.cpp"
    break;

  case 554: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2935 "chpl.ypp"
  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10127 "bison-chpl-lib.cpp"
    break;

  case 555: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2940 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10133 "bison-chpl-lib.cpp"
    break;

  case 556: /* nil_expr: TNIL  */
#line 2949 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10139 "bison-chpl-lib.cpp"
    break;

  case 564: /* stmt_level_expr: io_expr TIO expr  */
#line 2965 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10145 "bison-chpl-lib.cpp"
    break;

  case 565: /* opt_task_intent_ls: %empty  */
#line 2969 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10151 "bison-chpl-lib.cpp"
    break;

  case 566: /* opt_task_intent_ls: task_intent_clause  */
#line 2970 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10157 "bison-chpl-lib.cpp"
    break;

  case 567: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2975 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10167 "bison-chpl-lib.cpp"
    break;

  case 568: /* task_intent_ls: intent_expr  */
#line 2983 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10173 "bison-chpl-lib.cpp"
    break;

  case 569: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2984 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10179 "bison-chpl-lib.cpp"
    break;

  case 570: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2989 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10189 "bison-chpl-lib.cpp"
    break;

  case 571: /* forall_intent_ls: intent_expr  */
#line 2997 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10195 "bison-chpl-lib.cpp"
    break;

  case 572: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2998 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10201 "bison-chpl-lib.cpp"
    break;

  case 573: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3003 "chpl.ypp"
  {
    if (auto ident = (yyvsp[-2].expr)->toIdentifier()) {
      auto name = ident->name();
      auto node = TaskVar::build(BUILDER, LOC((yyloc)), name, (yyvsp[-3].taskIntent),
                                        toOwned((yyvsp[-1].expr)),
                                        toOwned((yyvsp[0].expr)));
      (yyval.expr) = node.release();
    } else {
      const char* msg = "Expected identifier for task variable name";
      (yyval.expr) = context->raiseError((yyloc), msg);
    }
  }
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 574: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3016 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 10226 "bison-chpl-lib.cpp"
    break;

  case 575: /* intent_expr: expr TREDUCE ident_expr  */
#line 3020 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10234 "bison-chpl-lib.cpp"
    break;

  case 576: /* task_var_prefix: TCONST  */
#line 3026 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10240 "bison-chpl-lib.cpp"
    break;

  case 577: /* task_var_prefix: TIN  */
#line 3027 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10246 "bison-chpl-lib.cpp"
    break;

  case 578: /* task_var_prefix: TCONST TIN  */
#line 3028 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10252 "bison-chpl-lib.cpp"
    break;

  case 579: /* task_var_prefix: TREF  */
#line 3029 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 580: /* task_var_prefix: TCONST TREF  */
#line 3030 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10264 "bison-chpl-lib.cpp"
    break;

  case 581: /* task_var_prefix: TVAR  */
#line 3031 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10270 "bison-chpl-lib.cpp"
    break;

  case 583: /* io_expr: io_expr TIO expr  */
#line 3037 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10276 "bison-chpl-lib.cpp"
    break;

  case 584: /* new_maybe_decorated: TNEW  */
#line 3042 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10282 "bison-chpl-lib.cpp"
    break;

  case 585: /* new_maybe_decorated: TNEW TOWNED  */
#line 3044 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10288 "bison-chpl-lib.cpp"
    break;

  case 586: /* new_maybe_decorated: TNEW TSHARED  */
#line 3046 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10294 "bison-chpl-lib.cpp"
    break;

  case 587: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3048 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10300 "bison-chpl-lib.cpp"
    break;

  case 588: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3050 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10306 "bison-chpl-lib.cpp"
    break;

  case 589: /* new_expr: new_maybe_decorated expr  */
#line 3056 "chpl.ypp"
  {
    if (FnCall* fnCall = (yyvsp[0].expr)->toFnCall()) {
      (yyval.expr) = context->wrapCalledExpressionInNew((yyloc), (yyvsp[-1].newManagement), fnCall);
    } else {

      // It's an error for one reason or another. TODO: Specialize these
      // errors later (e.g. 'new a.field' would require parens around
      // the expression 'a.field'; 'new foo' would require an argument
      // list for 'foo'; and something like 'new __primitive()' just
      // doesn't make any sense...
      (yyval.expr) = context->raiseError((yyloc), "Invalid form for new expression");
    }
  }
#line 10324 "bison-chpl-lib.cpp"
    break;

  case 590: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3073 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10332 "bison-chpl-lib.cpp"
    break;

  case 591: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3077 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10340 "bison-chpl-lib.cpp"
    break;

  case 592: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3081 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10348 "bison-chpl-lib.cpp"
    break;

  case 593: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3085 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10356 "bison-chpl-lib.cpp"
    break;

  case 594: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3092 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10362 "bison-chpl-lib.cpp"
    break;

  case 604: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 3109 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10368 "bison-chpl-lib.cpp"
    break;

  case 605: /* expr: expr TCOLON expr  */
#line 3111 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10376 "bison-chpl-lib.cpp"
    break;

  case 606: /* expr: expr TDOTDOT expr  */
#line 3115 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10385 "bison-chpl-lib.cpp"
    break;

  case 607: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 3120 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10394 "bison-chpl-lib.cpp"
    break;

  case 608: /* expr: expr TDOTDOT  */
#line 3125 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10403 "bison-chpl-lib.cpp"
    break;

  case 609: /* expr: TDOTDOT expr  */
#line 3130 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10413 "bison-chpl-lib.cpp"
    break;

  case 610: /* expr: TDOTDOTOPENHIGH expr  */
#line 3136 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10423 "bison-chpl-lib.cpp"
    break;

  case 611: /* expr: TDOTDOT  */
#line 3142 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10433 "bison-chpl-lib.cpp"
    break;

  case 612: /* opt_expr: %empty  */
#line 3173 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10439 "bison-chpl-lib.cpp"
    break;

  case 613: /* opt_expr: expr  */
#line 3174 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10445 "bison-chpl-lib.cpp"
    break;

  case 614: /* opt_try_expr: TTRY expr  */
#line 3178 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10451 "bison-chpl-lib.cpp"
    break;

  case 615: /* opt_try_expr: TTRYBANG expr  */
#line 3179 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10457 "bison-chpl-lib.cpp"
    break;

  case 616: /* opt_try_expr: expr  */
#line 3180 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10463 "bison-chpl-lib.cpp"
    break;

  case 622: /* call_base_expr: expr TBANG  */
#line 3197 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10469 "bison-chpl-lib.cpp"
    break;

  case 623: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3198 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10475 "bison-chpl-lib.cpp"
    break;

  case 626: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3205 "chpl.ypp"
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
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 627: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3217 "chpl.ypp"
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
#line 10507 "bison-chpl-lib.cpp"
    break;

  case 628: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3229 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10515 "bison-chpl-lib.cpp"
    break;

  case 629: /* dot_expr: expr TDOT ident_use  */
#line 3236 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10521 "bison-chpl-lib.cpp"
    break;

  case 630: /* dot_expr: expr TDOT TTYPE  */
#line 3238 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10527 "bison-chpl-lib.cpp"
    break;

  case 631: /* dot_expr: expr TDOT TDOMAIN  */
#line 3240 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10533 "bison-chpl-lib.cpp"
    break;

  case 632: /* dot_expr: expr TDOT TLOCALE  */
#line 3242 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10539 "bison-chpl-lib.cpp"
    break;

  case 633: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3244 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10549 "bison-chpl-lib.cpp"
    break;

  case 634: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3250 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10559 "bison-chpl-lib.cpp"
    break;

  case 635: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3262 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10565 "bison-chpl-lib.cpp"
    break;

  case 636: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3264 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10573 "bison-chpl-lib.cpp"
    break;

  case 637: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3268 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10581 "bison-chpl-lib.cpp"
    break;

  case 638: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3272 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10589 "bison-chpl-lib.cpp"
    break;

  case 639: /* bool_literal: TFALSE  */
#line 3278 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10595 "bison-chpl-lib.cpp"
    break;

  case 640: /* bool_literal: TTRUE  */
#line 3279 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10601 "bison-chpl-lib.cpp"
    break;

  case 641: /* str_bytes_literal: STRINGLITERAL  */
#line 3283 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10607 "bison-chpl-lib.cpp"
    break;

  case 642: /* str_bytes_literal: BYTESLITERAL  */
#line 3284 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10613 "bison-chpl-lib.cpp"
    break;

  case 645: /* literal_expr: INTLITERAL  */
#line 3290 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10619 "bison-chpl-lib.cpp"
    break;

  case 646: /* literal_expr: REALLITERAL  */
#line 3291 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10625 "bison-chpl-lib.cpp"
    break;

  case 647: /* literal_expr: IMAGLITERAL  */
#line 3292 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10631 "bison-chpl-lib.cpp"
    break;

  case 648: /* literal_expr: CSTRINGLITERAL  */
#line 3293 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10637 "bison-chpl-lib.cpp"
    break;

  case 649: /* literal_expr: TNONE  */
#line 3294 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10643 "bison-chpl-lib.cpp"
    break;

  case 650: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3296 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10651 "bison-chpl-lib.cpp"
    break;

  case 651: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3300 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10659 "bison-chpl-lib.cpp"
    break;

  case 652: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3304 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10667 "bison-chpl-lib.cpp"
    break;

  case 653: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3308 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10675 "bison-chpl-lib.cpp"
    break;

  case 654: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3312 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10683 "bison-chpl-lib.cpp"
    break;

  case 655: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3316 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10691 "bison-chpl-lib.cpp"
    break;

  case 656: /* assoc_expr_ls: expr TALIAS expr  */
#line 3323 "chpl.ypp"
                                        { (yyval.exprList) = TODOLIST((yyloc)); }
#line 10697 "bison-chpl-lib.cpp"
    break;

  case 657: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3324 "chpl.ypp"
                                        { (yyvsp[-4].exprList)->push_back(TODOEXPR((yyloc))); }
#line 10703 "bison-chpl-lib.cpp"
    break;

  case 658: /* binary_op_expr: expr TPLUS expr  */
#line 3328 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10709 "bison-chpl-lib.cpp"
    break;

  case 659: /* binary_op_expr: expr TMINUS expr  */
#line 3329 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10715 "bison-chpl-lib.cpp"
    break;

  case 660: /* binary_op_expr: expr TSTAR expr  */
#line 3330 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10721 "bison-chpl-lib.cpp"
    break;

  case 661: /* binary_op_expr: expr TDIVIDE expr  */
#line 3331 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10727 "bison-chpl-lib.cpp"
    break;

  case 662: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3332 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10733 "bison-chpl-lib.cpp"
    break;

  case 663: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3333 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10739 "bison-chpl-lib.cpp"
    break;

  case 664: /* binary_op_expr: expr TMOD expr  */
#line 3334 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10745 "bison-chpl-lib.cpp"
    break;

  case 665: /* binary_op_expr: expr TEQUAL expr  */
#line 3335 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10751 "bison-chpl-lib.cpp"
    break;

  case 666: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3336 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10757 "bison-chpl-lib.cpp"
    break;

  case 667: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3337 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10763 "bison-chpl-lib.cpp"
    break;

  case 668: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3338 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10769 "bison-chpl-lib.cpp"
    break;

  case 669: /* binary_op_expr: expr TLESS expr  */
#line 3339 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10775 "bison-chpl-lib.cpp"
    break;

  case 670: /* binary_op_expr: expr TGREATER expr  */
#line 3340 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10781 "bison-chpl-lib.cpp"
    break;

  case 671: /* binary_op_expr: expr TBAND expr  */
#line 3341 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10787 "bison-chpl-lib.cpp"
    break;

  case 672: /* binary_op_expr: expr TBOR expr  */
#line 3342 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10793 "bison-chpl-lib.cpp"
    break;

  case 673: /* binary_op_expr: expr TBXOR expr  */
#line 3343 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10799 "bison-chpl-lib.cpp"
    break;

  case 674: /* binary_op_expr: expr TAND expr  */
#line 3344 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10805 "bison-chpl-lib.cpp"
    break;

  case 675: /* binary_op_expr: expr TOR expr  */
#line 3345 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10811 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr TEXP expr  */
#line 3346 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10817 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TBY expr  */
#line 3347 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10823 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TALIGN expr  */
#line 3348 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10829 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr THASH expr  */
#line 3349 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10835 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TDMAPPED expr  */
#line 3350 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10841 "bison-chpl-lib.cpp"
    break;

  case 681: /* unary_op_expr: TPLUS expr  */
#line 3354 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10847 "bison-chpl-lib.cpp"
    break;

  case 682: /* unary_op_expr: TMINUS expr  */
#line 3355 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10853 "bison-chpl-lib.cpp"
    break;

  case 683: /* unary_op_expr: TMINUSMINUS expr  */
#line 3356 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 10859 "bison-chpl-lib.cpp"
    break;

  case 684: /* unary_op_expr: TPLUSPLUS expr  */
#line 3357 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 10865 "bison-chpl-lib.cpp"
    break;

  case 685: /* unary_op_expr: TBANG expr  */
#line 3358 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10871 "bison-chpl-lib.cpp"
    break;

  case 686: /* unary_op_expr: expr TBANG  */
#line 3359 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10879 "bison-chpl-lib.cpp"
    break;

  case 687: /* unary_op_expr: TBNOT expr  */
#line 3362 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10885 "bison-chpl-lib.cpp"
    break;

  case 688: /* reduce_expr: expr TREDUCE expr  */
#line 3367 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10893 "bison-chpl-lib.cpp"
    break;

  case 689: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3371 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10901 "bison-chpl-lib.cpp"
    break;

  case 690: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3375 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 10909 "bison-chpl-lib.cpp"
    break;

  case 691: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3379 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 10917 "bison-chpl-lib.cpp"
    break;

  case 692: /* scan_expr: expr TSCAN expr  */
#line 3385 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10923 "bison-chpl-lib.cpp"
    break;

  case 693: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3386 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10929 "bison-chpl-lib.cpp"
    break;

  case 694: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3387 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10935 "bison-chpl-lib.cpp"
    break;

  case 695: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3388 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10941 "bison-chpl-lib.cpp"
    break;


#line 10945 "bison-chpl-lib.cpp"

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
