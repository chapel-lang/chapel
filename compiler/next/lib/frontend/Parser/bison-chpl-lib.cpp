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
  YYSYMBOL_try_stmt = 237,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 238,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 239,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 240,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 241,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 242,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 243,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 244,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 245,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 246,              /* class_start  */
  YYSYMBOL_class_tag = 247,                /* class_tag  */
  YYSYMBOL_opt_inherit = 248,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 249,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 250,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 251,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 252,                  /* enum_ls  */
  YYSYMBOL_253_3 = 253,                    /* $@3  */
  YYSYMBOL_deprecated_enum_item = 254,     /* deprecated_enum_item  */
  YYSYMBOL_enum_item = 255,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 256,         /* lambda_decl_expr  */
  YYSYMBOL_linkage_spec_empty = 257,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 258,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt_complete = 259,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 260,             /* fn_decl_stmt  */
  YYSYMBOL_261_4 = 261,                    /* $@4  */
  YYSYMBOL_fn_decl_stmt_inner = 262,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 263,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 264,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 265,                 /* fn_ident  */
  YYSYMBOL_assignop_ident = 266,           /* assignop_ident  */
  YYSYMBOL_opt_formal_ls = 267,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 268,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 269,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 270,                /* formal_ls  */
  YYSYMBOL_formal = 271,                   /* formal  */
  YYSYMBOL_opt_intent_tag = 272,           /* opt_intent_tag  */
  YYSYMBOL_required_intent_tag = 273,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 274,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 275,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 276,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 277,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 278,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 279,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 280,               /* query_expr  */
  YYSYMBOL_var_arg_expr = 281,             /* var_arg_expr  */
  YYSYMBOL_opt_lifetime_where = 282,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 283, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 284,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 285,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 286,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 287, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner = 288, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 289,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 290,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 291,            /* var_decl_stmt  */
  YYSYMBOL_var_decl_stmt_inner_ls = 292,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 293,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 294, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 295, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 296,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 297,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 298,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 299,                 /* opt_type  */
  YYSYMBOL_array_type = 300,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 301, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 302,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 303,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 304,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 305,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 306,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 307,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 308,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 309,                /* actual_ls  */
  YYSYMBOL_actual_expr = 310,              /* actual_expr  */
  YYSYMBOL_ident_expr = 311,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 312,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 313,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 314,                 /* for_expr  */
  YYSYMBOL_cond_expr = 315,                /* cond_expr  */
  YYSYMBOL_nil_expr = 316,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 317,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 318,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 319,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 320,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 321,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 322,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 323,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 324,          /* task_var_prefix  */
  YYSYMBOL_io_expr = 325,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 326,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 327,                 /* new_expr  */
  YYSYMBOL_let_expr = 328,                 /* let_expr  */
  YYSYMBOL_expr = 329,                     /* expr  */
  YYSYMBOL_opt_expr = 330,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 331,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 332,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 333,           /* call_base_expr  */
  YYSYMBOL_call_expr = 334,                /* call_expr  */
  YYSYMBOL_dot_expr = 335,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 336,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 337,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 338,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 339,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 340,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 341,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 342,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 343,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 344,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 345       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 367 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include other helper functions for the parser
  #include "parser-help.h"


#line 468 "bison-chpl-lib.cpp"

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
#define YYLAST   20421

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  166
/* YYNRULES -- Number of rules.  */
#define YYNRULES  699
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1245

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
       0,   641,   641,   646,   647,   652,   653,   658,   676,   696,
     697,   698,   699,   700,   701,   702,   703,   704,   705,   706,
     707,   708,   709,   722,   723,   724,   725,   726,   727,   741,
     750,   765,   774,   782,   805,   817,   828,   840,   852,   863,
     864,   871,   878,   879,   881,   886,   887,   891,   904,   917,
     930,   944,   945,   947,   952,   953,   958,   975,   979,   983,
     994,  1013,  1014,  1018,  1024,  1030,  1035,  1045,  1051,  1057,
    1063,  1068,  1073,  1083,  1088,  1092,  1098,  1102,  1107,  1112,
    1117,  1122,  1127,  1132,  1140,  1148,  1150,  1152,  1157,  1159,
    1164,  1171,  1175,  1179,  1183,  1187,  1191,  1200,  1201,  1205,
    1206,  1207,  1208,  1209,  1210,  1213,  1214,  1215,  1216,  1217,
    1218,  1230,  1231,  1242,  1243,  1244,  1245,  1246,  1247,  1248,
    1249,  1250,  1251,  1252,  1253,  1254,  1255,  1256,  1257,  1258,
    1259,  1260,  1264,  1265,  1266,  1267,  1268,  1269,  1270,  1271,
    1272,  1273,  1274,  1275,  1282,  1283,  1284,  1285,  1289,  1290,
    1294,  1301,  1311,  1312,  1314,  1319,  1323,  1329,  1329,  1335,
    1335,  1344,  1345,  1346,  1347,  1348,  1349,  1350,  1354,  1361,
    1368,  1375,  1385,  1392,  1393,  1397,  1405,  1413,  1421,  1441,
    1451,  1458,  1476,  1489,  1502,  1506,  1510,  1514,  1518,  1522,
    1526,  1530,  1548,  1552,  1556,  1560,  1564,  1568,  1572,  1576,
    1580,  1584,  1588,  1592,  1596,  1600,  1604,  1608,  1612,  1616,
    1620,  1624,  1628,  1632,  1636,  1641,  1649,  1658,  1662,  1666,
    1670,  1674,  1678,  1682,  1686,  1690,  1695,  1700,  1705,  1713,
    1722,  1734,  1736,  1741,  1742,  1747,  1752,  1753,  1754,  1755,
    1756,  1757,  1758,  1759,  1760,  1761,  1762,  1763,  1764,  1776,
    1777,  1778,  1779,  1788,  1789,  1793,  1795,  1797,  1802,  1804,
    1806,  1811,  1813,  1815,  1817,  1819,  1821,  1826,  1827,  1831,
    1832,  1833,  1837,  1838,  1842,  1846,  1848,  1853,  1854,  1858,
    1860,  1862,  1869,  1876,  1886,  1893,  1894,  1895,  1899,  1900,
    1905,  1906,  1908,  1913,  1928,  1940,  1953,  1957,  1962,  1962,
    1969,  1970,  1972,  1977,  1982,  1991,  1997,  2001,  2002,  2004,
    2009,  2019,  2016,  2039,  2047,  2055,  2065,  2075,  2084,  2095,
    2096,  2100,  2101,  2102,  2103,  2104,  2105,  2106,  2107,  2108,
    2109,  2110,  2111,  2112,  2113,  2114,  2115,  2116,  2117,  2118,
    2119,  2120,  2121,  2122,  2123,  2124,  2125,  2126,  2127,  2136,
    2137,  2138,  2139,  2140,  2141,  2142,  2143,  2144,  2145,  2146,
    2147,  2151,  2152,  2156,  2160,  2161,  2165,  2166,  2170,  2173,
    2175,  2177,  2179,  2181,  2188,  2189,  2193,  2194,  2195,  2196,
    2197,  2198,  2199,  2200,  2201,  2205,  2206,  2207,  2208,  2209,
    2210,  2214,  2215,  2216,  2220,  2221,  2222,  2223,  2224,  2225,
    2229,  2230,  2233,  2234,  2238,  2239,  2243,  2247,  2248,  2249,
    2253,  2254,  2256,  2258,  2260,  2265,  2267,  2272,  2274,  2276,
    2278,  2280,  2282,  2284,  2289,  2290,  2294,  2301,  2305,  2310,
    2318,  2323,  2332,  2333,  2335,  2340,  2341,  2342,  2343,  2344,
    2348,  2353,  2394,  2398,  2405,  2415,  2428,  2437,  2446,  2458,
    2460,  2462,  2469,  2470,  2471,  2476,  2480,  2484,  2488,  2492,
    2496,  2500,  2507,  2508,  2509,  2510,  2511,  2516,  2517,  2518,
    2519,  2520,  2540,  2544,  2548,  2552,  2559,  2560,  2561,  2565,
    2567,  2573,  2575,  2577,  2582,  2583,  2584,  2585,  2586,  2592,
    2593,  2594,  2595,  2599,  2600,  2604,  2605,  2606,  2610,  2611,
    2615,  2616,  2620,  2625,  2634,  2635,  2636,  2637,  2641,  2642,
    2653,  2655,  2657,  2663,  2664,  2665,  2666,  2667,  2668,  2670,
    2672,  2674,  2676,  2678,  2680,  2683,  2685,  2687,  2689,  2691,
    2693,  2695,  2697,  2700,  2702,  2707,  2709,  2711,  2713,  2715,
    2717,  2719,  2721,  2723,  2725,  2727,  2729,  2731,  2733,  2735,
    2737,  2739,  2744,  2754,  2762,  2763,  2764,  2765,  2766,  2767,
    2768,  2769,  2774,  2775,  2779,  2788,  2789,  2793,  2802,  2803,
    2807,  2820,  2824,  2831,  2832,  2833,  2834,  2835,  2836,  2840,
    2841,  2846,  2848,  2850,  2852,  2854,  2860,  2877,  2881,  2885,
    2889,  2896,  2904,  2905,  2906,  2907,  2908,  2909,  2910,  2911,
    2912,  2913,  2915,  2917,  2919,  2921,  2923,  2925,  2927,  2946,
    2947,  2951,  2952,  2953,  2957,  2958,  2959,  2960,  2969,  2970,
    2971,  2972,  2973,  2977,  2989,  3001,  3005,  3007,  3009,  3011,
    3013,  3019,  3032,  3033,  3034,  3035,  3039,  3040,  3044,  3045,
    3049,  3050,  3051,  3052,  3053,  3054,  3055,  3056,  3057,  3058,
    3059,  3060,  3064,  3072,  3073,  3077,  3078,  3079,  3080,  3081,
    3082,  3083,  3084,  3085,  3086,  3087,  3088,  3089,  3090,  3091,
    3092,  3093,  3094,  3095,  3096,  3097,  3098,  3099,  3103,  3104,
    3105,  3106,  3107,  3108,  3111,  3115,  3116,  3117,  3118,  3122,
    3123,  3124,  3125,  3130,  3131,  3132,  3133,  3134,  3135,  3136
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
  "implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
  "catch_expr", "catch_expr_inner", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_start",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header_lcbr", "enum_ls", "$@3", "deprecated_enum_item",
  "enum_item", "lambda_decl_expr", "linkage_spec_empty", "linkage_spec",
  "fn_decl_stmt_complete", "fn_decl_stmt", "$@4", "fn_decl_stmt_inner",
  "fn_decl_stmt_start", "fn_decl_receiver_expr", "fn_ident",
  "assignop_ident", "opt_formal_ls", "req_formal_ls", "formal_ls_inner",
  "formal_ls", "formal", "opt_intent_tag", "required_intent_tag",
  "opt_this_intent_tag", "proc_iter_or_op", "opt_ret_tag",
  "opt_throws_error", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "var_arg_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner", "opt_init_type", "var_decl_type",
  "var_decl_stmt", "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
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

#define YYPACT_NINF (-1097)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-650)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1097,   108,  3727, -1097,    -3,   110, -1097, -1097, -1097, -1097,
   -1097, -1097,  5162,   124,   285,   221, 14458,   268, 20186,   124,
   11226,   273,   218,   270,   285,  5162, 11226,   760,  5162,   197,
   20272, -1097,    62,   389,  8458,  9670,  9670, -1097,  8630,   445,
     358,   331, -1097,   457, 20272, 20272, 20272,   357,  2119,  9842,
     488, 11226,    63, -1097,   491,   497, 11226, -1097, 14458, -1097,
   11226,   409,   391,   392,   394,   513, 20306, -1097, 10016,  7940,
   11226,  9842, 14458, 11226,   484,   544,   423,  5162,   548, 11226,
     552, 11398, 11398, -1097,   553, -1097, 14458, -1097,   562,  8630,
   11226, -1097, 11226, -1097, 11226, -1097, -1097, 13978, 11226, -1097,
   11226, -1097, -1097, -1097,  4118,  7076,  8804, 11226, -1097,  4988,
   -1097, -1097, -1097,   462, -1097,   226, -1097, -1097, -1097, -1097,
   -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097,  7248,
   -1097, 11570, -1097, -1097, -1097, -1097, -1097,   565, -1097, -1097,
   -1097, -1097, -1097, -1097, -1097,   490, 20272, -1097, 20032,   344,
   -1097,    48, -1097, -1097,   440,   456, -1097, 20272,  2119, -1097,
     468, -1097,   476, -1097, -1097,   478,   481,   485, 11226,   483,
     486, 19611,  3077,   -48,   493,   495, -1097, -1097,   287, -1097,
   -1097, -1097, -1097, -1097,   156, -1097, -1097, 19611,   477,  5162,
   -1097, -1097,   498, 11226, -1097, -1097, 11226, 11226, 11226, 20272,
   -1097, 11226, 10016, 10016,   596,   379, -1097, -1097, -1097, -1097,
     -17,   390, -1097, -1097,   489, 16167, -1097,  2119, -1097,   499,
   -1097,   111, 19611, 19890, -1097, 20272,   559, -1097, -1097,   584,
     551,  8114, -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097,
   -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097,
   -1097, -1097, -1097, -1097, -1097, -1097,   503, -1097, -1097, -1097,
   20272,   502,    33, 15754,    10, 15589,    10, 15671, 20272, 20272,
     -14,  3024, -1097, -1097,   584,  8114,   504,   420,  5162,   434,
     440,  1245,   117,    31, -1097,  5162, -1097, -1097, 16248,   835,
   -1097,   505,   508, -1097, 16248,   -17,   835, -1097,  8114,  2675,
    2675, -1097, -1097,   201, 19611, 11226, 11226, -1097, 19611,   523,
   16822, -1097, 16248,   -17, 19611,   517,  8114, -1097, 19611, 16905,
   -1097, -1097, 16987,  3522, -1097, -1097, 17068,   -17,    33, 16248,
   17149,   184,   184,   538,   835,   835,    64, -1097, -1097,  4292,
     109, -1097, 11226, -1097,   -36,   102, -1097,   -23,    91, 17230,
     -46,   538,   682, -1097,  4466, 11226, -1097, 11226,   626,   177,
   -1097, 15837,   605, 20272, 19611,   260, 20272, 10016,   525, -1097,
     532, 20066,   571,   172, -1097, -1097, -1097, -1097, -1097, -1097,
     610, -1097, -1097, -1097,   134,   628, -1097, -1097, -1097,  3472,
     591,   566,   202,   428, -1097, 11226,   573, 11226, 11226, 11226,
    9670,  9670, 11226,   442, 11226, 11226, 11226, 11226, 11226,   422,
   13978, 11226, 11226, 11226, 11226, 11226, 11226, 11226, 11226, 11226,
   11226, 11226, 11226, 11226, 11226, 11226, 11226,   661, -1097, -1097,
   -1097, -1097, -1097,  8976,  8976, -1097, -1097, -1097, -1097,  8976,
   -1097, -1097,  8976,  8976,  8114,  8114,  9670,  9670,  7768, -1097,
   -1097, 16330, 16411, 17311,   570,    72, 20272,  4640, -1097,  9670,
      33,   274, -1097, 11226, -1097, -1097, 11226,   620, -1097,   575,
     603, -1097, -1097, -1097,   701, 10016, -1097,  5336,  9670, -1097,
     579, -1097,    33,  5510,  9670, -1097,    33, -1097,    33,  9670,
   -1097,    33,   627,   629,  5162,   725,  5162, -1097,   727, 11226,
     698,   592,  8114, 20272, -1097, -1097,    53, -1097, -1097, -1097,
   -1097, -1097, -1097,   527,   621,   595, -1097,  2458, -1097,    89,
   -1097,  1245, -1097, -1097,   123, -1097, 11742,   649, 11226,  2119,
   -1097, -1097, 11226, 11226, -1097,   604, -1097, -1097, 10016, -1097,
   19611, 19611, -1097,    41, -1097,  8114,   606, -1097,   753, -1097,
     753, -1097, -1097, -1097, -1097, -1097, -1097, -1097,  9150, -1097,
   17392,  7422, -1097,  7596, -1097,  5162,   607,  9670,  9324,  3944,
     609, 11226, 10188, -1097, -1097,   361, -1097,  4814, -1097,   324,
   17473,   328, 15920, 20272, 11914, 11226, -1097, 20272,   490, -1097,
    2119,    96,   632,  2609, -1097, 20272, -1097, 11226,   212, -1097,
   -1097, -1097, 12086,   676, -1097,   613,   169, -1097,   638, -1097,
     642,   643,   655,   648,   650, -1097,   654,   664,   658,   659,
     660,   175,   669,   665,   671, -1097, -1097, -1097, -1097, -1097,
   -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097,
   -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097, 11226, -1097,
     680,   681,   675,   613,   613, -1097, 12258,   679, -1097, -1097,
   17554, 16208, 16208,   388, -1097,   388, -1097,   388, 19570,  1592,
    1242,  1487,   -17,   184, -1097,   452, -1097, -1097, -1097, -1097,
   -1097,   538,  1789,   388, 19748, 19748, 16208, 19748, 19748,  1003,
     184,  1789, 19311,  1003,   835,   835,   184,   538,   663,   668,
     686,   687,   688,   689,   656,   666, -1097,   388, -1097,   388,
      65, -1097, -1097, -1097,   137, -1097,  1141, 19652,   289, 12430,
    9670, 12602,  9670, 11226,  8114,  9670, 14738,   672,   124, 17635,
   -1097, -1097, 19611, 17716,  8114, -1097,  8114, 11226,   168,  8630,
   19611,    43, 16492,  7768, -1097,  8630, 19611,    14, 16002, -1097,
   -1097,    10, 16085, -1097, 11226, 11226,   811,  5162,   819, 17797,
    5162, 16574, 20272, -1097,   182, -1097,   188, -1097, -1097, -1097,
   20272,   713, -1097,  1245,   711,    28,   189,  1245,   117,    -2,
       7, 11226, 11226,  6902, -1097, -1097,   450,  8286, -1097, 19611,
   -1097, 17878, 17959, -1097, -1097, 19611,   690,   148,   684, -1097,
    1452, -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097,  5162,
     -21, 16657, -1097, -1097, 19611,  5162, 19611, -1097, 18040, -1097,
   -1097, 11226, -1097,    61,    80, 11226, -1097, 10360, 11914, 11226,
   -1097,   694,   714,   697, 18121,   714,   700,   490,   693,   336,
   -1097,  8114,   695, 19035, -1097, 19611, 20152,  3287, -1097, -1097,
     198, -1097,    28,   434, -1097, 18202, -1097, 14843, -1097, -1097,
   -1097,   346, 20272,   708,   692, -1097, -1097, -1097, -1097, -1097,
   -1097, -1097, -1097, -1097, -1097,  7768, -1097, -1097, -1097, -1097,
   -1097, -1097, -1097, -1097, -1097, -1097, -1097, -1097,    70,  9670,
    9670, 11226,   836, 18283, 11226,   837, 18364,   192,   699, 18445,
    8114,    33,    33, -1097, -1097, -1097, -1097, 16248, -1097, 14925,
    5684, -1097,  5858, -1097,   246, -1097, 15008,  6032, -1097,    33,
    6206, -1097,    33, -1097,    33, -1097,    33, 19611, 19611,  5162,
   -1097,  5162, 11226, -1097,  5162,   846,   720,   726, 20272,   489,
     711, -1097,   281, 10534,   326,   135, 11226,    58, -1097, -1097,
     649,   717,    76, -1097, -1097,   721,   722, -1097,  6380, 10016,
   -1097, -1097, -1097, 20272, -1097,   728,   489, -1097,  6554,   723,
    6728,   729, -1097, 11226, -1097, -1097, 11226, 18526,    60, 16740,
     730,   732, 11226, -1097, 11226, -1097,   737, -1097, -1097,   292,
   -1097,  2913, -1097, -1097, -1097, 19956, -1097, -1097, -1097,   738,
   13806,    78, -1097,   739, -1097,   762,   774,   613,   613, -1097,
   -1097, -1097, -1097,   649,   307,   317, 18608, 12774, 12946, 18689,
   13118, 13290, -1097, 13462, 13634,   323, -1097, -1097, -1097,  5162,
    8630, 19611,  8630, 19611,  7768, -1097,  5162,  8630, 19611, -1097,
    8630, 19611, -1097, -1097, -1097, -1097, -1097, 19611,   864,  5162,
   -1097, -1097, -1097, -1097,   326,   711,  9498, -1097, -1097, -1097,
      -8, 10016, -1097, -1097, -1097,   101, -1097,   -11, -1097,   602,
   18783, -1097, -1097, -1097, -1097, -1097,  9670, 14598,  8114,  8114,
    5162, -1097,    29,   742, 11226, -1097,  8630, -1097, 19611,  5162,
    8630, -1097, 19611,  5162, 19611,   310, 10706, 11914, 11226, 11914,
   11226, -1097, -1097, 18840, 19611, -1097, 19226,   751,  3477, -1097,
   -1097, -1097, -1097,   259, 14147,    71, -1097, -1097, -1097, -1097,
   11226, 11226, 11226, 11226, 11226, 11226, 11226, 11226, -1097, 17797,
   15091, 15174, -1097, 17797, 15257, 15340,  5162, -1097, -1097, -1097,
     326, 10880,    79, -1097, 19611, -1097, 11226,   135,   101,   101,
     101,   101,   101,   101,   135, 18880, -1097,   529,   744,   745,
     551, -1097,   489, 19611, 15423, -1097, 15506, -1097, -1097, -1097,
   19611,   754,   757,   758,   761, 11226, 19794, -1097, -1097, -1097,
   -1097,   280, -1097, -1097, -1097, 19031, 19071, 19222, 19262, 19413,
   19453,  5162,  5162,  5162,  5162, -1097, -1097, -1097, -1097, -1097,
   -1097,   176,  9670, 14287, 19611, -1097, -1097, -1097, -1097, -1097,
   -1097, -1097,   781, 13634,   765,   766, -1097,  5162,  5162, -1097,
   -1097, -1097, -1097, 19611, -1097, 11226, 11226, 11226, 11226, 11226,
   11226, 17797, 17797, 17797, 17797, 19494, -1097, -1097,   828, -1097,
   -1097, 17797, 17797, 11054, -1097
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   111,   642,   643,   644,   638,
     639,   645,     0,   562,    97,   132,   531,   139,   533,   562,
       0,   138,     0,   438,    97,     0,     0,   306,     0,   253,
     133,   174,   173,   636,     0,     0,     0,   172,     0,   137,
      51,   254,   308,   134,     0,     0,     0,     0,     0,     0,
     141,     0,   581,   553,   646,   142,     0,   309,   525,   435,
       0,     0,     0,   159,   157,   136,   534,   437,     0,     0,
       0,     0,   529,     0,     0,   140,     0,     0,   112,     0,
     637,     0,     0,   427,   135,   287,   527,   439,   143,     0,
       0,   695,     0,   697,     0,   698,   699,   608,     0,   696,
     693,   512,   155,   694,     0,     0,     0,     0,     4,     0,
       5,     9,    42,     0,    45,    54,    10,    60,    11,    12,
      13,    14,    15,   508,   509,    25,    46,   156,   166,     0,
     175,   609,   167,    16,    20,    17,    19,     0,   248,    18,
     600,    23,    24,    21,   165,   288,     0,   163,     0,   597,
     307,     0,   161,   310,   394,   385,   164,     0,     0,   162,
     614,   593,   510,   594,   515,   513,     0,     0,     0,   598,
     599,     0,   514,     0,   615,   616,   617,   640,   641,   592,
     517,   516,   595,   596,     0,    41,    27,   523,     0,     0,
     563,    98,     0,     0,   533,   133,     0,     0,     0,     0,
     534,     0,     0,     0,     0,   597,   614,   513,   598,   599,
     532,   514,   615,   616,     0,   562,   428,     0,   436,     0,
      22,     0,   493,   306,   285,     0,   173,   286,    44,    54,
       0,   500,   105,   113,   125,   119,   118,   127,   108,   117,
     128,   114,   129,   106,   130,   123,   116,   124,   122,   120,
     121,   107,   109,   115,   126,   131,     0,   110,   180,   429,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    52,    54,   500,     0,     0,     0,   374,
     394,     0,     0,     0,   442,     0,   149,    35,     0,   680,
     585,   582,   583,   584,     0,   526,   681,     7,   500,   306,
     306,   406,   490,     0,   489,     0,     0,   150,   613,     0,
       0,    38,     0,   530,   518,     0,   500,    39,   524,     0,
     267,   263,     0,   514,   267,   264,     0,   528,     0,     0,
       0,   682,   684,   606,   679,   678,     0,    57,    61,     0,
       0,   495,     0,   497,     0,     0,   496,     0,     0,   489,
       0,   607,     0,     6,     0,     0,    55,     0,     0,     0,
     171,     0,   638,   534,   610,   306,     0,     0,     0,   284,
       0,     0,   303,     0,   296,   300,   392,   393,   391,   318,
     395,   398,   397,   399,     0,   389,   386,   387,   390,     0,
     432,     0,     0,   511,    26,     0,   586,     0,     0,     0,
       0,     0,     0,   683,     0,     0,     0,     0,     0,     0,
     605,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   349,   356,   357,
     358,   353,   355,     0,     0,   351,   354,   352,   350,     0,
     360,   359,     0,     0,   500,   500,     0,     0,     0,    28,
      29,     0,     0,     0,     0,     0,     0,     0,    30,     0,
       0,     0,    31,     0,    32,    43,     0,   508,   506,     0,
     501,   502,   507,   295,     0,     0,   190,     0,     0,   189,
       0,   198,     0,     0,     0,   196,     0,   206,     0,     0,
     204,     0,     0,     0,     0,   222,     0,   349,   218,     0,
       0,     0,     0,     0,   232,    33,   381,   376,   377,   378,
     382,   383,   384,   374,   367,     0,   364,     0,   375,     0,
     446,     0,   447,   449,     0,   471,     0,   452,     0,     0,
     148,    34,     0,     0,    36,     0,   160,   158,     0,    90,
     611,   612,   151,     0,    37,     0,     0,   274,   265,   261,
     266,   262,   183,   182,    40,    59,    58,    62,     0,   647,
       0,     0,   632,     0,   634,     0,     0,     0,     0,     0,
       0,     0,     0,   651,     8,     0,    48,     0,    88,     0,
      85,     0,    67,     0,     0,    73,   168,     0,   288,   178,
       0,   259,   289,     0,   294,     0,   302,     0,   298,   293,
     396,   466,     0,   400,   388,   361,    99,   343,   113,   341,
     119,   118,   102,   117,   114,   346,   129,   100,   130,   116,
     120,   101,   103,   115,   131,   340,   322,   325,   323,   324,
     347,   335,   326,   339,   331,   329,   342,   345,   330,   328,
     333,   338,   327,   332,   336,   337,   334,   344,     0,   321,
       0,   104,     0,   361,   361,   319,     0,   430,   426,   441,
     561,   675,   674,   677,   686,   685,   690,   689,   671,   668,
     669,   670,   602,   658,   111,     0,   628,   629,   112,   627,
     626,   603,   662,   673,   667,   665,   676,   666,   664,   656,
     661,   663,   672,   655,   659,   660,   657,   604,     0,     0,
       0,     0,     0,     0,     0,     0,   688,   687,   692,   691,
     573,   574,   576,   578,     0,   565,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   649,   259,   562,   562,
     186,   440,   494,     0,     0,   520,     0,     0,     0,     0,
     537,     0,     0,     0,   199,     0,   543,     0,     0,   197,
     207,     0,     0,   205,     0,     0,   221,     0,   217,     0,
       0,     0,     0,   519,     0,   235,     0,   233,   379,   380,
       0,   374,   363,     0,   484,   410,     0,   450,     0,   146,
     147,   145,   144,     0,   470,   469,   593,     0,   444,   591,
     443,     0,     0,   625,   492,   491,     0,     0,     0,   521,
       0,   268,   648,   601,   633,   498,   635,   499,   214,     0,
       0,     0,   650,   212,   547,     0,   653,   652,     0,    50,
      49,     0,    84,     0,     0,     0,    77,     0,     0,    73,
      47,   694,    76,     0,    63,    74,     0,   288,     0,     0,
     256,     0,     0,   306,   301,   304,     0,     0,   465,   464,
     593,   401,   410,   374,   317,     0,   348,     0,   313,   314,
     434,   593,     0,     0,     0,    96,    94,    95,    93,    92,
      91,   623,   624,   575,   577,     0,   564,   132,   139,   138,
     137,   134,   141,   142,   136,   140,   135,   143,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   181,   504,   505,   503,     0,   216,     0,
       0,   188,     0,   187,     0,   568,     0,     0,   194,     0,
       0,   192,     0,   202,     0,   200,     0,   230,   229,     0,
     224,     0,     0,   220,     0,   226,     0,   258,     0,     0,
     484,   365,     0,     0,   452,     0,     0,     0,   448,   451,
     452,     0,     0,   453,   454,     0,     0,   276,     0,     0,
     275,   278,   522,     0,   269,   272,     0,   215,     0,     0,
       0,     0,   213,     0,    89,    86,     0,    70,    69,    68,
       0,     0,     0,   169,     0,   170,     0,   290,   179,     0,
     283,   306,   159,   157,   282,   306,   291,   152,   299,     0,
     456,     0,   311,     0,   320,    99,   101,   361,   361,   431,
     630,   631,   566,   452,   614,   614,     0,     0,     0,     0,
       0,     0,   258,     0,     0,     0,   185,   184,   191,     0,
       0,   536,     0,   535,     0,   567,     0,     0,   542,   195,
       0,   541,   193,   203,   201,   223,   219,   552,   225,     0,
      56,   255,   234,   231,   452,   484,     0,   487,   486,   488,
     593,   407,   370,   368,   424,     0,   425,   412,   415,     0,
     411,   404,   405,   305,   445,   475,     0,   649,   500,   500,
       0,   280,     0,     0,     0,   270,     0,   210,   549,     0,
       0,   208,   548,     0,   654,     0,     0,     0,    73,     0,
      73,    78,    81,    65,    64,   290,   306,   260,   306,   154,
     292,   461,   459,   593,   649,     0,   362,   315,   316,   570,
       0,     0,     0,     0,     0,     0,     0,     0,   260,   540,
       0,     0,   569,   546,     0,     0,     0,   228,   371,   369,
     452,   476,     0,   409,   408,   423,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   473,   593,     0,     0,
     281,   279,     0,   273,     0,   211,     0,   209,    87,    72,
      71,     0,     0,     0,     0,     0,   306,   176,   257,   153,
     460,   593,   402,   312,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   227,   373,   372,   478,   479,
     481,   593,     0,   649,   414,   416,   417,   420,   421,   422,
     418,   419,   413,     0,   587,   588,   271,     0,     0,    80,
      83,    79,    82,    66,   177,     0,     0,     0,     0,     0,
       0,   539,   538,   545,   544,     0,   480,   482,   593,   589,
     590,   551,   550,   476,   483
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1097, -1097, -1097,     5,  -267,  2345, -1097,     8, -1097, -1097,
       9,   657, -1097,  -902,  2782,   576,  -560, -1097,  -808,  -798,
   -1097, -1097,   112, -1097, -1097,   412,   908, -1097,  1598,   312,
    -383, -1097,  -592,  1715,  -895,   -60,  -823, -1097, -1097,   237,
   -1097, -1097, -1097, -1097, -1097, -1097, -1097,   165, -1097,   847,
   -1097, -1097,    -1,   798, -1097, -1097, -1097, -1097,   614, -1097,
     -24, -1097, -1097, -1097, -1097, -1097, -1097, -1097,  -557,  -932,
   -1097, -1097, -1097, -1097,    94,  -338,  1094, -1097, -1097, -1097,
     577, -1097, -1097, -1097, -1097,    84,  -171,  -635, -1097, -1097,
      90,   173,   433, -1097, -1097, -1097,   673, -1097, -1097,  -167,
     103,  -963,    97,  -200,  -192,  -414, -1097, -1097,    95, -1097,
      -7,   830,  -142,   431,   185,  -489,  -910,  -947, -1097,  -508,
    -653, -1096, -1057,  -868,   -55, -1097,    66, -1097,  -248,  -485,
    -507,   494,  -497, -1097, -1097, -1097,  1323, -1097,   -10, -1097,
   -1097,  -155, -1097,  -715, -1097, -1097, -1097,  1516,  1812,   -12,
   -1097,   -64,   556, -1097,  1840,  2043, -1097, -1097, -1097, -1097,
   -1097, -1097, -1097, -1097, -1097,  -446
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   338,   109,   758,   111,   112,   113,   114,
     115,   358,   116,   117,   118,   339,   832,   581,   836,   833,
     119,   120,   578,   579,   121,   122,   192,   649,   282,   123,
     257,   124,   784,   287,   125,   996,   126,   300,   299,   127,
     128,   129,   130,   131,   132,   133,   134,   898,   135,   270,
     136,   766,   767,   204,   138,   139,   140,   141,   548,   801,
     966,   142,   143,   797,   961,   144,   145,   146,   368,   843,
     147,   148,   373,   846,   374,   375,   205,   150,   151,   152,
     153,  1115,   154,   155,   652,   653,   499,   854,   280,   514,
     515,   516,   517,   518,   389,   379,   384,   852,  1183,  1073,
     468,  1062,   947,  1067,  1068,  1069,   156,   157,   391,   657,
     158,   159,   283,   284,   523,   524,   788,   849,   603,   527,
     785,  1199,  1059,   944,   340,   221,   344,   345,   469,   470,
     471,   206,   161,   162,   163,   164,   207,   166,   189,   190,
     714,   482,   914,   715,   716,   167,   168,   208,   209,   171,
     365,   472,   211,   173,   212,   213,   176,   177,   178,   179,
     350,   180,   181,   182,   183,   184
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     187,   443,   718,   860,   210,   309,   651,   108,   215,   214,
     848,   775,   513,   303,   222,   217,   392,   764,   858,   859,
     997,   981,   263,   265,   267,   835,   271,   501,   915,   786,
     980,   838,   776,   596,  1063,   228,   229,   288,   798,   289,
    1074,   346,   796,   285,   294,  1071,   295,   917,   296,   274,
     535,   348,  1072,  1112,  -253,  1106,   304,   308,   310,   312,
     313,   314,   285,  -254,   674,   318,   285,   319,   546,   322,
     326,   525,  1054,   258,   327,   461,   910,   329,   330,   528,
     331,   290,   332,   674,  1200,   333,   334,   494,   335,   945,
     525,  1138,   304,   308,   349,   351,   480,  1097,   480,   572,
     675,   768,   376,  1119,  1064,   850,  1146,  1236,     3,   561,
     486,   488,   491,   873,   676,  -277,  -485,   361,   525,   364,
     725,   377,   403,   444,  1076,   445,   725,  1202,   573,   480,
     409,   378,  1098,   480,  1147,   601,   562,  -485,  1064,   567,
     291,  -485,   769,   677,  1139,   946,  1237,  1244,   455,    69,
    -467,   565,   443,   968,   874,   457,   396,  -277,   292,   861,
    1012,   185,    69,   678,  -485,  -467,  -236,  1180,  1109,   231,
     259,   302,   997,  1176,   538,   293,   529,  1196,   275,   457,
    -467,   187,   678,   457,   451,   452,   453,  1140,   679,   318,
     304,   349,   566,   570,  -467,  -462,   704,   705,   457,   262,
     264,   266,   457,  1066,    23,   460,  -467,   302,   343,   302,
     480,  -277,   457,  1071,   526,  -467,   399,   568,   654,   308,
    1072,   568,   958,   568,   568,  -462,  1065,   457,   185,   906,
    1197,   465,   229,   526,   555,  1182,   568,  1066,  -462,   897,
     457,  -467,  -467,   188,   446,    23,   726,   563,   447,   976,
    1077,  -462,  1114,  1203,   558,    59,   463,   844,  -467,  -463,
     840,   526,  -467,   308,   959,   569,    67,   841,   777,   835,
     950,   347,   400,   355,   564,   464,   401,  -237,   602,   559,
     986,  -467,   875,   997,   942,  1179,   308,    23,   191,  -463,
    1172,    87,  1174,   540,   541,   778,    59,   718,  -462,  1171,
    -477,  1173,  -463,  -462,   308,   302,   302,    67,  -105,   876,
     356,    42,   592,   538,  -107,  -463,  -111,   598,   960,  1132,
    -455,  -477,  -112,   403,  -243,  -477,   216,   736,   407,  -242,
     560,   409,    87,   938,   777,   412,    57,   736,    59,   357,
     908,  -457,   599,   580,   557,   582,   538,   529,  -477,    67,
    -455,  1057,   217,   997,   937,   304,   989,  -297,   590,   218,
     939,   948,  -463,  -455,  1022,   539,   659,  -463,   231,   699,
     700,  -457,  1117,  1118,    87,   701,  -455,   890,   702,   703,
    1013,   447,  -297,   660,  -457,   661,   662,   663,   665,   667,
     668,  1034,   669,   670,   671,   672,   673,  -457,   681,   682,
     683,   684,   685,   686,   687,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,  1025,  1095,   297,  1035,   529,
     738,   308,   308,  -455,  1156,   674,   777,   308,  -455,   718,
     308,   308,   308,   308,   707,   709,   717,   736,   731,   -53,
     272,   -52,   675,   273,  -457,  -251,  1060,   729,   839,  -457,
     787,   732,  -572,  1055,   733,   982,   676,   -53,  -622,   -52,
    -622,   506,  -571,   304,  1107,   740,   742,   380,   736,   821,
     302,   746,   748,   825,   651,  1061,   -53,   752,   -52,  -572,
    1168,   529,   507,   385,   759,   677,   508,   761,   822,  -571,
     308,  -433,   826,   321,   325,  1128,   160,   346,  -468,   346,
     988,  -241,   275,  1113,   513,   -53,   160,   -52,  -558,   509,
    -433,  -468,   510,  -238,    61,  -621,   789,  -621,   381,   160,
     791,   792,   160,   511,   678,   185,   795,   403,   279,   382,
     679,   819,   407,   308,   386,   409,   536,   537,   835,   412,
     835,  -468,   512,   467,  -245,   387,   795,  -249,   383,   308,
    -621,   308,  -621,  -246,   506,   811,   795,   814,   172,   816,
     818,  -618,   298,  -618,   388,   664,   666,  -468,   172,  -240,
     399,   160,   834,   834,  -468,   507,   995,  -472,   302,   508,
    1157,   172,   557,   315,   172,   845,   513,   467,   718,   457,
    -472,   503,   919,   922,   316,  -468,   924,   926,   160,  -620,
    -244,  -620,   509,   160,  -250,   510,  -366,   352,  -252,  -239,
     467,   706,   708,  -619,  -468,  -619,   511,  1181,  -247,  -468,
    -472,   366,  -468,   863,   728,   864,   400,   805,   467,   807,
     401,   354,  -555,   172,   367,   512,   855,   323,   323,   393,
     395,   794,  -554,   741,  1201,   394,  -472,  -559,   448,   747,
    -560,  1145,   456,  -472,   751,   969,   971,  -557,   457,  -556,
     172,   794,   450,   462,   343,   172,   343,   259,   356,   466,
     905,   794,   473,   475,  -472,   502,   532,   403,   404,   533,
     405,   406,   407,   160,   408,   409,  1008,   542,   545,   412,
     574,   583,   587,  -472,   593,   597,   418,   419,  -472,   600,
     422,  -472,   594,   423,   424,   425,  1201,   740,   893,   746,
     896,   759,   308,   899,   814,   656,  1238,   604,   901,   902,
     409,   680,   308,   954,   308,   907,  1148,   909,   952,   698,
     658,   717,   810,   916,  1206,  1207,  1208,  1209,  1210,  1211,
     506,   724,   927,   928,   734,   172,  1201,   735,   736,   737,
     743,   754,  1149,   755,  1150,  1151,   467,   467,  1152,  1153,
     757,   507,   760,   762,   763,   508,   771,   772,   223,   314,
     318,   349,   160,   787,   800,   308,   793,   538,   799,   160,
     851,   809,   224,   815,   853,  -132,    22,    23,   509,  -139,
    -138,   510,  1001,    61,  -108,  -137,   225,  -134,    31,   226,
     137,  -141,   511,  -106,    37,  -142,  -136,  -140,  -109,   580,
     137,    42,  -135,   977,   467,   979,   834,   834,  -143,   856,
    -110,   512,   857,   137,   862,   -51,   137,   865,   871,   308,
    1158,  1159,   866,   160,   172,   349,    57,   904,    59,   995,
     872,   172,    63,   900,   -51,    64,   929,   227,   160,    67,
     867,   868,   869,   870,   931,   943,   962,   467,   -75,   982,
     957,   983,   987,   717,   985,   990,  1011,   399,    83,  1017,
    1020,    85,  1084,  1023,    87,   137,  -474,   665,   707,  1016,
    1010,  1049,  1019,   655,  1050,   892,   302,   895,   308,  -474,
    1051,  1075,  1078,  1079,  1101,   172,  1102,  1089,  1031,  1136,
    1033,  -105,   137,  1093,  1082,  1038,  1105,   137,  1041,   995,
     172,  1116,  1111,  -107,  1162,  1178,  1214,  1215,  1219,  -474,
    1047,  1220,  1221,   400,   102,  1222,  1147,   401,  1239,  1240,
     577,   500,   219,   974,  1070,  1110,   328,  1052,   550,  1083,
     998,  1007,   589,  1003,   941,  -474,   770,   304,  1184,  1002,
     302,   160,  -474,   519,  1212,  1205,  1088,  1009,  1092,   360,
     790,  1094,   949,     0,   834,     0,     0,     0,     0,     0,
    1103,   160,  1104,  -474,   403,     0,     0,   160,     0,   407,
       0,   408,   409,     0,     0,     0,   412,   137,   160,     0,
     160,     0,  -474,     0,   419,     0,     0,  -474,     0,     0,
    -474,  1142,   425,     0,     0,  1031,  1033,     0,  1038,  1041,
       0,  1088,  1092,   172,     0,     0,     0,  1129,  1130,     0,
    1131,     0,   717,     0,  1133,  1134,     0,     0,  1135,     0,
       0,     0,     0,   172,     0,   399,   467,     0,     0,   172,
       0,     0,     0,     0,   349,     0,  1058,     0,   467,  1144,
     172,     0,   172,     0,   664,   706,     0,     0,     0,   160,
       0,     0,   302,   160,  1155,   814,   308,   308,     0,     0,
       0,   160,  1163,     0,  1164,     0,   137,     0,  1166,     0,
       0,     0,     0,   137,  1170,   834,   834,   834,   834,     0,
       0,   400,     0,     0,     0,   401,   149,     0,     0,     0,
       0,     0,   814,     0,     0,     0,   149,     0,  1129,  1185,
    1186,  1133,  1187,  1188,  1189,  1190,     0,     0,     0,   149,
       0,   172,   149,     0,     0,   172,     0,     0,     0,     0,
       0,     0,     0,   172,  1204,   975,   680,   137,     0,     0,
       0,     0,   403,   404,   674,   405,   406,   407,     0,   408,
     409,     0,   137,   467,   412,     0,     0,     0,   877,   302,
       0,   878,   419,  1223,  1143,     0,   879,     0,   423,   424,
     425,   149,     0,     0,     0,     0,     0,   195,     0,  1231,
    1232,  1233,  1234,     0,     0,     0,     0,   880,     0,     0,
    1235,   814,     0,     0,   881,     0,     0,     0,   149,     0,
       0,  1092,     0,   149,   882,  1241,  1242,     0,     0,     0,
     888,   883,   467,  1231,  1232,  1233,  1234,  1241,  1242,     0,
       0,     0,     0,     0,     0,     0,     0,   884,     0,     0,
       0,  1092,     0,     0,     0,     0,     0,     0,     0,   885,
       0,     0,     0,   678,  1198,     0,     0,     0,   232,     0,
     886,   160,     0,     0,   160,   137,   887,     0,     0,     0,
       0,     0,   233,   234,     0,   235,     0,     0,     0,     0,
     236,     0,     0,     0,   399,   137,     0,     0,     0,   237,
       0,   137,     0,   149,     0,   238,     0,     0,     0,     0,
       0,   239,   137,     0,   137,   240,     0,     0,   241,     0,
       0,     0,     0,   160,     0,     0,  1198,     0,   242,   160,
       0,     0,     0,   172,   243,   244,   172,     0,     0,     0,
       0,     0,   245,     0,     0,   165,     0,     0,     0,     0,
     400,   246,     0,     0,   401,   165,     0,     0,     0,     0,
     247,   248,     0,   249,     0,   250,  1198,   251,   165,     0,
     252,   165,     0,     0,   253,   520,     0,   254,     0,     0,
     255,     0,     0,   137,     0,   172,     0,   137,     0,     0,
       0,   172,   149,     0,     0,   137,     0,     0,     0,   149,
       0,   403,   404,  1014,  1015,   406,   407,     0,   408,   409,
     467,   467,     0,   412,     0,     0,     0,     0,     0,     0,
     165,   419,     0,     0,   160,     0,   160,   423,   424,   425,
       0,   160,     0,     0,   160,     0,   521,     0,     0,     0,
       0,     0,     0,   160,     0,   160,     0,   165,   160,     0,
       0,     0,   165,   149,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   149,     0,
       0,     0,   160,     0,     0,   232,     0,     0,     0,     0,
       0,     0,   160,     0,   160,     0,   172,     0,   172,   233,
     234,     0,   235,   172,     0,     0,   172,   236,     0,     0,
       0,     0,     0,     0,     0,   172,   237,   172,     0,     0,
     172,     0,   238,     0,     0,     0,     0,     0,   239,     0,
       0,     0,   240,     0,     0,   241,     0,     0,     0,     0,
       0,     0,   165,     0,   172,   242,     0,     0,   169,   399,
       0,   243,   244,   160,   172,     0,   172,     0,   169,   245,
     160,     0,     0,     0,     0,     0,     0,     0,   246,     0,
       0,   169,     0,   160,   169,     0,     0,   247,   248,     0,
     249,   149,   250,     0,   251,   137,     0,   252,   137,     0,
       0,   253,     0,     0,   254,     0,     0,   255,     0,     0,
       0,   149,     0,     0,   160,   400,     0,   149,     0,   401,
       0,     0,     0,   160,     0,   172,     0,   160,   149,     0,
     149,     0,   172,   169,     0,     0,     0,     0,     0,     0,
       0,   165,     0,     0,     0,   172,     0,   137,   165,     0,
       0,     0,     0,   137,     0,     0,     0,     0,     0,     0,
     169,   457,     0,   963,   399,   169,   403,   404,   256,     0,
     160,   407,     0,   408,   409,     0,   172,     0,   412,     0,
       0,     0,   276,   277,   278,   172,   419,     0,     0,   172,
       0,     0,   423,   424,   425,     0,     0,     0,     0,   149,
       0,     0,   165,   149,     0,     0,     0,     0,     0,     0,
       0,   149,     0,     0,     0,     0,     0,   165,     0,     0,
     400,     0,     0,     0,   401,   160,   160,   160,   160,     0,
       0,     0,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   169,     0,     0,   137,     0,
     137,   160,   160,     0,     0,   137,     0,     0,   137,     0,
       0,     0,     0,     0,     0,     0,     0,   137,     0,   137,
       0,   403,   137,     0,     0,     0,   407,     0,   408,   409,
       0,     0,     0,   412,   369,     0,   372,   172,   172,   172,
     172,   419,     0,     0,     0,   390,   137,   423,   424,   425,
       0,     0,     0,     0,     0,     0,   137,     0,   137,     0,
       0,     0,     0,   172,   172,     0,     0,     0,     0,     0,
     165,     0,     0,     0,     0,     0,   311,     0,     0,     0,
       0,     0,     0,     0,   169,     0,     0,   454,     0,     0,
     165,   169,     0,     0,     0,     0,   165,     0,     0,     0,
       0,     0,     0,     0,   170,     0,     0,   165,     0,   165,
       0,   399,     0,   256,   170,     0,     0,   137,     0,     0,
       0,     0,     0,     0,   137,     0,     0,   170,     0,     0,
     170,     0,   174,     0,     0,     0,     0,   137,     0,     0,
       0,   149,   174,     0,   149,   169,     0,     0,   474,     0,
       0,     0,     0,     0,     0,   174,   492,   493,   174,     0,
     169,     0,     0,     0,     0,     0,     0,   400,   137,   522,
       0,   401,     0,     0,     0,     0,     0,   137,   165,   170,
       0,   137,   165,     0,     0,     0,     0,     0,     0,     0,
     165,     0,     0,   149,     0,     0,     0,     0,     0,   149,
       0,     0,     0,     0,     0,     0,   170,   174,     0,     0,
       0,   170,     0,     0,     0,     0,     0,     0,   403,   404,
       0,   405,   406,   407,   137,   408,   409,   410,     0,     0,
     412,   413,   414,     0,   174,   416,   417,   418,   419,   174,
       0,   422,     0,     0,   423,   424,   425,     0,     0,     0,
       0,   588,     0,     0,   591,   426,     0,     0,     0,   372,
       0,     0,     0,   169,     0,     0,     0,   476,   479,   481,
     485,   487,   490,     0,     0,     0,     0,   650,     0,   137,
     137,   137,   137,   169,     0,     0,     0,     0,     0,   169,
       0,   170,     0,   531,   149,     0,   149,     0,     0,   534,
     169,   149,   169,     0,   149,   137,   137,     0,     0,     0,
       0,     0,     0,   149,     0,   149,     0,   544,   149,   174,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   552,   553,   175,     0,     0,     0,     0,
       0,     0,   149,     0,   727,   175,     0,     0,     0,     0,
       0,     0,   149,     0,   149,     0,     0,     0,   175,     0,
       0,   175,     0,     0,     0,     0,     0,     0,     0,     0,
     165,   169,     0,   165,     0,   169,     0,     0,     0,     0,
     170,     0,     0,   169,     0,     0,     0,   170,     0,     0,
       0,   765,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   774,     0,     0,   174,   522,
     175,     0,   232,   149,     0,   174,     0,     0,     0,     0,
     149,     0,   165,     0,     0,     0,   233,   234,   165,   235,
       0,     0,     0,   149,   236,     0,     0,   175,     0,     0,
       0,   170,   175,   237,     0,     0,     0,     0,     0,   238,
       0,     0,     0,     0,     0,   239,   170,     0,     0,   240,
       0,     0,   241,     0,   149,   730,     0,     0,     0,   174,
       0,   830,   242,   149,     0,   837,     0,   149,   243,   244,
       0,     0,     0,   372,   174,     0,   245,   744,     0,     0,
       0,   749,     0,   750,     0,   246,   753,     0,     0,     0,
       0,     0,     0,     0,   247,   248,     0,   249,     0,   250,
       0,   251,     0,     0,   252,     0,     0,     0,   253,     0,
     149,   254,   175,   165,   255,   165,     0,     0,     0,     0,
     165,     0,     0,   165,     0,     0,     0,     0,     0,     0,
       0,     0,   165,     0,   165,     0,     0,   165,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   170,
       0,     0,     0,   169,     0,     0,   169,     0,     0,     0,
       0,   165,     0,     0,     0,   149,   149,   149,   149,   170,
     281,   165,     0,   165,     0,   170,     0,   174,     0,     0,
       0,     0,     0,     0,     0,     0,   170,     0,   170,     0,
       0,   149,   149,     0,     0,     0,     0,   174,     0,     0,
       0,   175,     0,   174,     0,   169,     0,     0,   175,     0,
       0,   169,     0,     0,   174,     0,   174,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   110,     0,     0,
       0,     0,   165,     0,     0,     0,     0,   186,     0,   165,
     936,     0,     0,     0,     0,     0,     0,     0,   940,     0,
     220,   522,   165,   230,     0,   522,     0,   170,     0,     0,
       0,   170,   175,     0,     0,     0,     0,     0,     0,   170,
       0,     0,     0,     0,     0,     0,     0,   175,   965,     0,
       0,     0,     0,   165,     0,   174,     0,     0,     0,   174,
       0,     0,   165,     0,     0,     0,   165,   174,     0,     0,
       0,     0,   317,     0,     0,     0,   169,     0,   169,     0,
       0,     0,     0,   169,     0,     0,   169,     0,     0,     0,
       0,     0,     0,     0,   372,   169,     0,   169,     0,   110,
     169,     0,     0,     0,   353,   650,   911,   913,     0,   165,
     390,   232,   918,   921,     0,     0,   923,   925,     0,     0,
       0,     0,     0,     0,   169,   233,   234,     0,   235,     0,
       0,     0,     0,   236,   169,     0,   169,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,   238,     0,
     175,     0,     0,     0,   239,     0,     0,     0,   240,     0,
       0,   241,     0,     0,   165,   165,   165,   165,     0,     0,
     175,   242,     0,     0,     0,     0,   175,   243,   244,     0,
       0,     0,     0,     0,   449,   245,   765,   175,     0,   175,
     165,   165,     0,     0,   246,   169,     0,     0,     0,     0,
       0,     0,   169,   247,   248,     0,   249,     0,   250,     0,
     251,   965,     0,   252,     0,   169,     0,   253,     0,   170,
     254,     0,   170,   255,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   169,   174,     0,     0,
     174,     0,     0,     0,     0,   169,     0,     0,   175,   169,
     842,     0,   175,     0,     0,     0,  1026,  1027,     0,     0,
     175,   170,  1028,   505,     0,     0,     0,   170,     0,   773,
     530,  -290,     0,     0,  1039,  -290,  -290,  1042,     0,  1043,
    -290,  1044,     0,     0,     0,  -290,     0,  -290,  -290,   174,
       0,     0,   169,  -290,     0,   174,     0,     0,     0,     0,
    -290,     0,     0,  -290,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -290,     0,   110,  -290,     0,  -290,     0,  -290,
       0,  -290,  -290,     0,  -290,     0,  -290,   224,  -290,   110,
       0,    22,    23,     0,     0,     0,     0,   169,   169,   169,
     169,   225,     0,    31,   226,     0,     0,  -290,     0,    37,
    -290,     0,   170,  -290,   170,     0,    42,     0,     0,   170,
       0,     0,   170,   169,   169,     0,     0,     0,     0,     0,
       0,   170,     0,   170,     0,     0,   170,     0,     0,     0,
     174,    57,   174,    59,     0,     0,     0,   174,     0,     0,
     174,     0,   227,     0,    67,     0,     0,     0,     0,   174,
     170,   174,     0,  -290,   174,     0,     0,     0,     0,  -290,
     170,     0,   170,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,     0,     0,     0,  1161,   174,     0,
     175,     0,   110,   175,     0,     0,     0,     0,   174,     0,
     174,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   530,     0,     0,     0,     0,     0,   530,     0,
       0,   286,     0,     0,     0,     0,     0,     0,     0,   756,
       0,   170,     0,     0,     0,     0,     0,     0,   170,     0,
       0,     0,   175,   286,     0,     0,     0,     0,   175,     0,
       0,   170,     0,   320,   324,     0,     0,     0,     0,   174,
       0,     0,     0,     0,     0,     0,   174,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   174,
       0,     0,   170,     0,     0,     0,     0,     0,     0,     0,
       0,   170,     0,     0,     0,   170,     0,     0,     0,     0,
     808,     0,     0,     0,   813,     0,     0,     0,     0,     0,
     174,  1108,   110,     0,     0,     0,     0,     0,     0,   174,
       0,     0,     0,   174,     0,   224,     0,     0,     0,    22,
      23,     0,     0,     0,     0,     0,     0,     0,   170,   225,
       0,    31,   226,   175,     0,   175,     0,    37,     0,     0,
     175,     0,     0,   175,    42,     0,     0,     0,     0,     0,
       0,     0,   175,     0,   175,     0,   174,   175,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,     0,     0,   992,   458,     0,   993,     0,
     227,   175,    67,   170,   170,   170,   170,     0,     0,     0,
       0,   175,     0,   175,     0,     0,     0,     0,     0,     0,
       0,    83,     0,     0,    85,     0,     0,    87,     0,   170,
     170,   174,   174,   174,   174,     0,   397,     0,     0,     0,
       0,     0,     0,   398,   286,   286,   286,   286,   286,   286,
       0,     0,   495,   498,     0,     0,   399,   174,   174,   504,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     286,     0,   175,     0,     0,     0,   286,   102,     0,   175,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   175,     0,   286,     0,     0,     0,     0,     0,
       0,     0,   930,     0,     0,   933,     0,     0,     0,     0,
     286,   286,   400,     0,     0,     0,   401,     0,     0,     0,
       0,     0,     0,   175,     0,   496,     0,     0,     0,     0,
       0,     0,   175,     0,     0,     0,   175,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,   497,   428,
     429,   430,   431,   432,   967,     0,   435,   436,   437,   438,
     972,   440,   441,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,     0,   411,   412,   413,   414,   415,   175,
     416,   417,   418,   419,   420,   421,   422,     0,     0,   423,
     424,   425,     0,   457,     0,     0,     0,     0,     0,     0,
     426,   427,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -579,     0,   175,   175,   175,   175,     0,     0,
       0,     0,   286,     0,     0,   442,     0,     0,  -618,     0,
    -618,     0,     0,     0,     0,   530,     0,   530,     0,     0,
     175,   175,   530,     0,   286,   530,     0,     0,   286,     0,
     286,     0,     0,   286,  1045,     0,  1046,     0,     0,  1048,
       0,     0,     0,     0,     0,     0,     0,     0,   999,     0,
       5,   301,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,  1081,    15,    16,     0,    17,     0,   194,
       0,     0,    21,  1087,     0,  1091,     0,     0,     0,     0,
       0,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,  1137,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1160,    92,    93,    94,    95,
      96,     0,     0,     0,  1165,    97,     0,     0,  1167,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   202,     0,   105,     0,
     203,  1000,     0,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   605,     0,   606,     0,     0,     0,     0,
       0,  1195,     0,     0,   607,     0,     0,     0,     0,   608,
     234,   609,   610,     0,     0,     0,     0,   611,     0,   224,
       0,     0,     0,    22,    23,     0,   237,     0,   195,     0,
       0,     0,   612,   225,     0,    31,   226,     0,   613,     0,
       0,    37,   240,   286,   286,   614,     0,   615,    42,   286,
     286,     0,     0,   286,   286,   616,     0,     0,     0,     0,
       0,   617,   618,   935,     0,     0,     0,     0,     0,   245,
       0,     0,     0,    57,     0,    59,     0,     0,   619,   992,
       0,     0,   993,     0,   227,     0,    67,   247,   248,     0,
     620,     0,   250,     0,   621,     0,     0,   622,     0,     0,
       0,   623,   964,     0,   254,    83,     0,   624,    85,     0,
       0,    87,     0,     0,     0,     0,   497,   428,   429,   430,
     431,   432,     0,     0,   435,   436,   437,   438,     0,   440,
     441,   625,   626,   627,   628,   629,   630,     0,   631,     0,
       0,     0,   632,   633,   634,   635,   636,   637,   638,   639,
     640,   641,   642,     0,   643,     0,     0,   644,   645,   646,
     647,   102,     0,   648,     0,     0,   427,   428,   429,   430,
     431,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     441,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   286,   286,     0,     0,     0,     0,   286,
     442,   498,     0,  -618,     0,  -618,     0,     0,   498,     0,
       0,   286,     0,     0,   286,     0,   286,     0,   286,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1053,     0,     0,     0,     0,     0,    -2,     4,     0,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,  1085,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -51,     0,    40,    41,    42,     0,
      43,  -306,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -51,    52,    53,     0,    54,    55,    56,     0,
    -306,     0,     0,    57,    58,    59,    60,    61,    62,    63,
    -306,   -51,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -51,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   286,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,     0,   105,     0,
     106,     0,     0,   107,     0,     0,     0,     0,     0,     0,
       0,     0,   498,   498,     0,     0,   498,   498,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1216,     4,   498,     5,   498,     6,
       7,     8,     9,    10,    11,     0,  -649,     0,    12,    13,
      14,    15,    16,  -649,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,  -649,    28,    29,  -649,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -51,     0,    40,    41,    42,     0,    43,  -306,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -306,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -306,   -51,    64,
      65,    66,  -649,    67,    68,    69,  -649,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -51,    87,    88,
       0,     0,    89,     0,    90,     0,     0,  -649,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -649,  -649,    94,  -649,  -649,  -649,  -649,
    -649,  -649,  -649,     0,  -649,  -649,  -649,  -649,  -649,     0,
    -649,  -649,  -649,  -649,  -649,  -649,  -649,  -649,   102,  -649,
    -649,  -649,     0,   104,  -649,   105,     0,   106,     0,   336,
    -649,     5,   301,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -51,     0,    40,    41,    42,
       0,    43,  -306,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -306,     0,     0,    57,    58,    59,    60,    61,    62,
      63,  -306,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -51,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,   337,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -51,
       0,    40,    41,    42,     0,    43,  -306,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -306,     0,     0,    57,    58,
      59,    60,    61,    62,    63,  -306,   -51,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -51,    87,    88,     0,     0,
      89,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   102,     0,     0,   103,
       0,   104,   556,   105,     0,   106,     0,   575,   107,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -51,     0,    40,    41,    42,     0,    43,
    -306,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -306,
       0,     0,    57,    58,    59,    60,    61,    62,    63,  -306,
     -51,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -51,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,   576,   105,     0,   106,
       0,   336,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -51,     0,    40,
      41,    42,     0,    43,  -306,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -306,     0,     0,    57,    58,    59,    60,
      61,    62,    63,  -306,   -51,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -51,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
     337,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -51,     0,    40,    41,    42,     0,    43,  -306,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -306,     0,     0,
      57,    58,    59,    60,    61,    62,    63,  -306,   -51,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -51,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,   820,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -51,     0,    40,    41,    42,
       0,    43,  -306,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -306,     0,     0,    57,    58,    59,    60,   352,    62,
      63,  -306,   -51,    64,    65,    66,     0,    67,    68,    69,
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
       0,    40,    41,    42,     0,    43,  -306,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -306,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -306,   -51,    64,    65,    66,
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
      37,   739,    39,   -51,     0,    40,    41,    42,     0,    43,
    -306,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -306,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -306,
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
      33,    34,    35,    36,    37,   745,    39,   -51,     0,    40,
      41,    42,     0,    43,  -306,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -306,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -306,   -51,    64,    65,    66,     0,    67,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1030,
      39,   -51,     0,    40,    41,    42,     0,    43,  -306,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -306,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -306,   -51,    64,
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
      35,    36,    37,  1032,    39,   -51,     0,    40,    41,    42,
       0,    43,  -306,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -306,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -306,   -51,    64,    65,    66,     0,    67,    68,    69,
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
      31,    32,    33,    34,    35,    36,    37,  1037,    39,   -51,
       0,    40,    41,    42,     0,    43,  -306,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -306,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -306,   -51,    64,    65,    66,
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
      37,  1040,    39,   -51,     0,    40,    41,    42,     0,    43,
    -306,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -306,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -306,
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
      26,    27,     0,  1080,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -51,     0,    40,
      41,    42,     0,    43,  -306,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -306,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -306,   -51,    64,    65,    66,     0,    67,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1086,
      39,   -51,     0,    40,    41,    42,     0,    43,  -306,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -306,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -306,   -51,    64,
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
      35,    36,    37,  1090,    39,   -51,     0,    40,    41,    42,
       0,    43,  -306,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -306,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -306,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -51,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,   951,   107,     5,   301,     6,     7,     8,
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
       0,   202,     0,   105,   804,   203,     0,     0,   107,     5,
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
       0,     0,     0,   103,     0,   202,     0,   105,   806,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,   710,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,   711,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,   712,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,   713,    88,     0,     0,     0,     0,     0,     0,
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
      51,     0,    52,    53,   953,    54,    55,     0,     0,     0,
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
     802,   105,     0,   203,     0,     0,   107,     5,   301,     6,
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
       0,   103,     0,   202,     0,   105,     0,   203,   812,     0,
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
       0,   203,  1141,     5,   107,     6,     7,     8,     9,    10,
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
       0,   203,   817,     5,   107,     6,     7,     8,     9,    10,
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
     978,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     0,   107,     5,   301,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,   779,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,   780,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
     781,    74,    75,    76,   782,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,  1056,     0,     5,
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
       0,    80,     0,     0,     0,    84,  1169,     0,    86,     0,
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
       0,   105,     0,  1056,     0,     0,   107,     5,   301,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,  1127,
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
     100,   101,     0,     0,     0,   103,     0,   104,     0,   105,
       0,   203,     0,     5,   107,     6,     7,     8,   362,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   363,     0,     0,
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
       0,     0,     0,     0,     0,     0,   779,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,   780,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,   781,    74,
      75,    76,   782,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   783,     0,     5,   107,     6,
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
       0,   831,     0,   202,     0,   105,     0,   203,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
     779,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,   780,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,   781,    74,    75,    76,   782,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   202,     0,   105,     0,   847,
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
       0,   783,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   891,    39,     0,     0,     0,
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
       0,     0,    33,   196,   197,     0,     0,   894,    39,     0,
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
     195,     0,     0,     0,    33,   196,   197,     0,     0,  1121,
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
       0,  1122,    39,     0,     0,     0,    41,     0,     0,    43,
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
     197,     0,     0,  1124,    39,     0,     0,     0,    41,     0,
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
      33,   196,   197,     0,     0,  1125,    39,     0,     0,     0,
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
       0,     0,    33,   196,   197,     0,     0,  1126,    39,     0,
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
     195,     0,     0,     0,    33,   196,   197,     0,     0,  1127,
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
       0,     0,     0,   103,     0,     0,     0,   105,     0,   847,
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
      43,     0,     0,   199,     0,     0,    47,    48,  -458,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,  -458,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,  -458,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,  -458,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       5,   301,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
     101,  -458,    21,     0,     0,     0,  -458,     0,   105,     0,
     847,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,  -476,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       0,     0,  -476,     0,     0,     0,  -476,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,     0,     0,   202,     0,   105,  -476,
    1056,     5,     0,     6,     7,     8,     9,    10,    11,     0,
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
       0,   783,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,     0,     0,  1005,    84,     0,     0,
      86,     0,     0,    88,     0,   607,     0,     0,     0,     0,
     233,   234,   609,   235,     0,     0,     0,     0,   236,     0,
       0,     0,     0,     0,     0,     0,     0,   237,     0,    94,
       0,     0,     0,   612,     0,     0,     0,     0,     0,   239,
       0,     0,     0,   240,     0,     0,   241,     0,   615,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,   105,
       0,   203,   617,   244,     0,     0,     0,     0,     0,     0,
     245,     0,     0,     0,     0,     0,     0,     0,     0,   246,
       0,     0,     0,     0,     0,     0,     0,   397,   247,   248,
       0,   249,     0,   250,   398,  1006,     0,     0,   622,     0,
       0,     0,   253,     0,     0,   254,     0,   399,   255,     0,
       0,     0,     0,     0,     0,     0,     0,   497,   428,   429,
     430,   431,   432,     0,     0,   435,   436,   437,   438,     0,
     440,   441,   625,   626,   627,   628,   629,   630,     0,   631,
       0,     0,     0,   632,   633,   634,   635,   636,   637,   638,
     639,   640,   641,   642,     0,   643,     0,     0,   644,   645,
     646,   647,     0,   400,     0,     0,     0,   401,     0,     0,
     397,     0,     0,     0,     0,     0,  1029,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,   402,   497,
     428,   429,   430,   431,   432,     0,     0,   435,   436,   437,
     438,     0,   440,   441,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,   414,   415,
       0,   416,   417,   418,   419,   420,   421,   422,     0,     0,
     423,   424,   425,     0,   457,     0,   400,     0,     0,     0,
     401,   426,     0,   397,     0,     0,     0,     0,     0,  1036,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,   402,   497,   428,   429,   430,   431,   432,     0,     0,
     435,   436,   437,   438,     0,   440,   441,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,   414,   415,     0,   416,   417,   418,   419,   420,   421,
     422,     0,     0,   423,   424,   425,     0,   457,     0,   400,
       0,     0,     0,   401,   426,     0,   397,     0,     0,     0,
       0,     0,  1191,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,   402,   497,   428,   429,   430,   431,
     432,     0,     0,   435,   436,   437,   438,     0,   440,   441,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,   414,   415,     0,   416,   417,   418,
     419,   420,   421,   422,     0,     0,   423,   424,   425,     0,
     457,     0,   400,     0,     0,     0,   401,   426,     0,   397,
       0,     0,     0,     0,     0,  1192,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,   402,   497,   428,
     429,   430,   431,   432,     0,     0,   435,   436,   437,   438,
       0,   440,   441,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,     0,   411,   412,   413,   414,   415,     0,
     416,   417,   418,   419,   420,   421,   422,     0,     0,   423,
     424,   425,     0,   457,     0,   400,     0,     0,     0,   401,
     426,     0,   397,     0,     0,     0,     0,     0,  1193,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
     402,   497,   428,   429,   430,   431,   432,     0,     0,   435,
     436,   437,   438,     0,   440,   441,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
     414,   415,     0,   416,   417,   418,   419,   420,   421,   422,
       0,     0,   423,   424,   425,     0,   457,     0,   400,     0,
       0,     0,   401,   426,     0,   397,     0,     0,     0,     0,
       0,  1194,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,   402,   497,   428,   429,   430,   431,   432,
       0,     0,   435,   436,   437,   438,     0,   440,   441,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,   414,   415,     0,   416,   417,   418,   419,
     420,   421,   422,     0,     0,   423,   424,   425,     0,   457,
       0,   400,     0,     0,     0,   401,   426,     0,   397,     0,
       0,     0,     0,     0,  1217,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,   402,   497,   428,   429,
     430,   431,   432,     0,     0,   435,   436,   437,   438,     0,
     440,   441,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,     0,   411,   412,   413,   414,   415,     0,   416,
     417,   418,   419,   420,   421,   422,     0,     0,   423,   424,
     425,     0,   457,     0,   400,     0,     0,     0,   401,   426,
       0,   397,     0,     0,     0,     0,     0,  1218,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,   483,     0,     0,     0,     0,     0,     0,   402,
     497,   428,   429,   430,   431,   432,     0,   484,   435,   436,
     437,   438,     0,   440,   441,   403,   404,     0,   405,   406,
     407,     0,   408,   409,   410,     0,   411,   412,   413,   414,
     415,     0,   416,   417,   418,   419,   420,   421,   422,     0,
       0,   423,   424,   425,     0,   457,     0,   400,     0,     0,
       0,   401,   426,   397,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   285,     0,     0,     0,   480,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,   489,
       0,     0,     0,     0,     0,     0,     0,     0,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,     0,   411,
     412,   413,   414,   415,     0,   416,   417,   418,   419,   420,
     421,   422,     0,     0,   423,   424,   425,     0,   457,   400,
       0,     0,     0,   401,     0,   426,   397,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   477,     0,     0,
     480,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,   478,     0,     0,     0,     0,     0,     0,     0,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,   414,   415,     0,   416,   417,   418,
     419,   420,   421,   422,     0,     0,   423,   424,   425,     0,
     457,     0,   400,     0,     0,     0,   401,   426,     0,   397,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,   584,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,     0,   411,   412,   413,   414,   415,   585,
     416,   417,   418,   419,   420,   421,   422,     0,     0,   423,
     424,   425,     0,   457,     0,   400,     0,     0,     0,   401,
     426,     0,   397,   827,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,   828,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
     414,   415,   829,   416,   417,   418,   419,   420,   421,   422,
       0,   586,   423,   424,   425,     0,     0,     0,   400,     0,
       0,     0,   401,   426,   397,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   920,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,   414,   415,     0,   416,   417,   418,   419,
     420,   421,   422,     0,     0,   423,   424,   425,     0,     0,
     400,     0,     0,     0,   401,     0,   426,   397,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   285,     0,
       0,   480,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,     0,   411,   412,   413,   414,   415,     0,   416,   417,
     418,   419,   420,   421,   422,     0,     0,   423,   424,   425,
       0,   457,     0,   400,     0,     0,     0,   401,   426,   397,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,   480,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,   459,     0,     0,     0,     0,
       0,     0,     0,     0,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,   414,   415,
     399,   416,   417,   418,   419,   420,   421,   422,     0,     0,
     423,   424,   425,     0,   457,   400,     0,     0,     0,   401,
     397,   426,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,   285,     0,     0,     0,     0,   188,     0,     0,     0,
     402,     0,     0,     0,     0,     0,   400,     0,     0,     0,
     401,     0,     0,     0,     0,     0,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
     414,   415,     0,   416,   417,   418,   419,   420,   421,   422,
       0,   402,   423,   424,   425,     0,   400,     0,     0,     0,
     401,     0,   397,   426,     0,     0,     0,   403,   404,   398,
     405,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,   414,   399,   719,   416,   417,   418,   419,   420,   421,
     422,   402,     0,   423,   424,   425,     0,     0,   720,     0,
       0,     0,     0,     0,   426,     0,     0,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,   414,   415,     0,   416,   417,   418,   419,   420,   421,
     422,     0,     0,   423,   424,   425,     0,   457,   400,     0,
       0,     0,   401,   397,   426,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   721,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,     0,     0,     0,   722,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,   414,   415,     0,   416,   417,   418,   419,
     420,   421,   422,     0,     0,   423,   424,   425,     0,   400,
       0,     0,     0,   401,   397,     0,   426,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   912,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,   414,   415,     0,   416,   417,   418,
     419,   420,   421,   422,     0,     0,   423,   424,   425,     0,
     400,     0,     0,     0,   401,     0,   397,   426,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,     0,   411,   412,   413,   414,   415,     0,   416,   417,
     418,   419,   420,   421,   422,     0,     0,   423,   424,   425,
       0,   457,   400,     0,     0,     0,   401,     0,   426,   397,
       0,     0,     0,     0,     0,   934,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,     0,   411,   412,   413,   414,   415,     0,
     416,   417,   418,   419,   420,   421,   422,     0,     0,   423,
     424,   425,     0,   457,     0,   400,     0,     0,     0,   401,
     426,     0,   397,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,   480,  1099,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
     414,   415,  1100,   416,   417,   418,   419,   420,   421,   422,
       0,     0,   423,   424,   425,     0,     0,     0,   400,     0,
       0,   970,   401,   426,   397,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,   402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,   414,   415,     0,   416,   417,   418,   419,
     420,   421,   422,     0,     0,   423,   424,   425,     0,     0,
     400,     0,     0,     0,   401,     0,   426,   397,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,     0,   411,   412,   413,   414,   415,     0,   416,   417,
     418,   419,   420,   421,   422,     0,     0,   423,   424,   425,
       0,   543,     0,   400,     0,     0,     0,   401,   426,   397,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,   414,   415,
       0,   416,   417,   418,   419,   420,   421,   422,     0,   547,
     423,   424,   425,     0,     0,   400,     0,     0,     0,   401,
     397,   426,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
     414,   415,     0,   416,   417,   418,   419,   420,   421,   422,
       0,   549,   423,   424,   425,     0,   400,     0,     0,     0,
     401,   397,     0,   426,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,   414,   415,     0,   416,   417,   418,   419,   420,   421,
     422,     0,   551,   423,   424,   425,     0,   400,     0,     0,
       0,   401,   397,     0,   426,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,     0,   411,
     412,   413,   414,   415,     0,   416,   417,   418,   419,   420,
     421,   422,     0,   554,   423,   424,   425,     0,   400,     0,
       0,     0,   401,   397,     0,   426,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,   571,   402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,   414,   415,     0,   416,   417,   418,   419,
     420,   421,   422,     0,     0,   423,   424,   425,     0,   400,
       0,     0,     0,   401,   397,     0,   426,     0,     0,     0,
       0,   398,   723,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,   414,   415,     0,   416,   417,   418,
     419,   420,   421,   422,     0,     0,   423,   424,   425,     0,
     400,     0,     0,     0,   401,   397,   823,   426,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,     0,   411,   412,   413,   414,   415,     0,   416,   417,
     418,   419,   420,   421,   422,     0,     0,   423,   424,   425,
       0,   400,     0,     0,   803,   401,   397,     0,   426,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   403,   404,     0,   405,   406,   407,     0,   408,
     824,   410,     0,   411,   412,   413,   414,   415,     0,   416,
     417,   418,   419,   420,   421,   422,     0,     0,   423,   424,
     425,     0,   400,     0,     0,     0,   401,   397,     0,   426,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,     0,   411,   412,   413,   414,   415,  -580,
     416,   417,   418,   419,   420,   421,   422,     0,     0,   423,
     424,   425,     0,   400,     0,     0,     0,   401,   397,     0,
     426,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,   188,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,   414,   415,
       0,   416,   417,   418,   419,   420,   421,   422,     0,     0,
     423,   424,   425,     0,   400,     0,     0,     0,   401,   397,
       0,   426,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,   932,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,   404,     0,   405,   406,
     407,     0,   408,   409,   410,     0,   411,   412,   413,   414,
     415,     0,   416,   417,   418,   419,   420,   421,   422,     0,
     903,   423,   424,   425,     0,   400,     0,     0,     0,   401,
     397,     0,   426,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,     0,   411,   412,   413,
     414,   415,     0,   416,   417,   418,   419,   420,   421,   422,
       0,     0,   423,   424,   425,     0,   400,     0,     0,     0,
     401,   397,     0,   426,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,   414,   415,     0,   416,   417,   418,   419,   420,   421,
     422,     0,     0,   423,   424,   425,     0,   400,     0,     0,
     955,   401,   397,     0,   426,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,     0,   411,
     412,   413,   414,   415,     0,   416,   417,   418,   419,   420,
     421,   422,     0,     0,   423,   424,   425,     0,   400,     0,
       0,   956,   401,   397,   984,   426,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,   973,   402,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,   414,   415,     0,   416,   417,   418,   419,
     420,   421,   422,     0,     0,   423,   424,   425,     0,   400,
       0,     0,     0,   401,   397,     0,   426,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   402,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,   414,   415,     0,   416,   417,   418,
     419,   420,   421,   422,     0,     0,   423,   424,   425,     0,
     400,     0,     0,     0,   401,   397,     0,   426,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,  1018,     0,     0,     0,
       0,     0,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,     0,   411,   412,   413,   414,   415,     0,   416,   417,
     418,   419,   420,   421,   422,     0,     0,   423,   424,   425,
       0,   400,     0,     0,  1004,   401,   397,     0,   426,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,  1021,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,     0,   411,   412,   413,   414,   415,     0,   416,
     417,   418,   419,   420,   421,   422,     0,     0,   423,   424,
     425,     0,   400,     0,     0,     0,   401,   397,     0,   426,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,     0,   411,   412,   413,   414,   415,     0,
     416,   417,   418,   419,   420,   421,   422,     0,     0,   423,
     424,   425,     0,   400,     0,     0,     0,   401,   397,  1096,
     426,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,     0,   411,   412,   413,   414,   415,
       0,   416,   417,   418,   419,   420,   421,   422,     0,     0,
     423,   424,   425,     0,   400,     0,     0,     0,   401,  1024,
     397,   426,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,   402,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,   404,     0,   405,   406,
     407,     0,   408,   409,   410,     0,   411,   412,   413,   414,
     415,     0,   416,   417,   418,   419,   420,   421,   422,     0,
       0,   423,   424,   425,     0,     0,   400,     0,     0,     0,
     401,   397,   426,     0,     0,     0,     0,     0,   398,  1120,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,     0,   411,   412,
     413,   414,   415,     0,   416,   417,   418,   419,   420,   421,
     422,     0,     0,   423,   424,   425,     0,   400,     0,     0,
       0,   401,     0,     0,   426,     0,     0,     0,     0,     0,
    1123,     0,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,     0,   411,
     412,   413,   414,   415,  1154,   416,   417,   418,   419,   420,
     421,   422,   397,  1175,   423,   424,   425,     0,     0,   398,
       0,     0,     0,     0,     0,   426,     0,     0,     0,     0,
       0,   400,   399,     0,     0,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,   402,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   403,   404,     0,   405,   406,   407,   400,   408,
     409,   410,   401,   411,   412,   413,   414,   415,     0,   416,
     417,   418,   419,   420,   421,   422,     0,     0,   423,   424,
     425,     0,     0,     0,     0,     0,     0,     0,     0,   426,
       0,     0,     0,   402,     0,     0,     0,     0,   400,     0,
       0,     0,   401,     0,     0,     0,     0,     0,     0,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,   414,   415,     0,   416,   417,   418,   419,
     420,   421,   422,   402,     0,   423,   424,   425,     0,     0,
       0,     0,     0,     0,     0,     0,   426,     0,     0,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,   414,   415,     0,   416,   417,   418,   419,
     420,   421,   422,   397,     0,   423,   424,   425,     0,     0,
     398,     0,     0,     0,  1213,     0,   426,   224,     0,     0,
       0,    22,    23,   399,     0,     0,   991,     0,     0,     0,
       0,   225,     0,    31,   226,     0,     0,     0,     0,    37,
       0,     0,     0,   397,     0,     0,    42,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,    57,     0,    59,     0,    61,     0,   992,     0,   400,
     993,     0,   227,   401,    67,     0,     0,     0,     0,     0,
       0,     0,  1225,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,     0,     0,    85,     0,     0,    87,
       0,     0,     0,     0,   402,     0,     0,     0,     0,   400,
       0,     0,     0,   401,     0,     0,     0,     0,     0,     0,
     403,   404,  1226,   405,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,   414,   415,     0,   416,   417,   418,
     419,   420,   421,   422,   402,     0,   423,   424,   425,   102,
       0,     0,     0,     0,     0,   994,     0,   426,     0,     0,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,   414,   415,     0,   416,   417,   418,
     419,   420,   421,   422,   397,     0,   423,   424,   425,     0,
       0,   398,     0,     0,     0,     0,     0,   426,   224,     0,
       0,     0,    22,    23,   399,     0,     0,   991,     0,     0,
       0,     0,   225,     0,    31,   226,     0,     0,     0,     0,
      37,     0,     0,     0,   397,     0,     0,    42,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,    57,     0,    59,     0,    61,     0,   992,     0,
     400,   993,     0,   227,   401,    67,     0,     0,     0,     0,
       0,     0,     0,  1227,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,   399,     0,   402,     0,     0,     0,     0,
     400,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,   403,   404,  1228,   405,   406,   407,     0,   408,   409,
     410,     0,   411,   412,   413,   414,   415,     0,   416,   417,
     418,   419,   420,   421,   422,   402,     0,   423,   424,   425,
     102,     0,     0,     0,     0,     0,  1177,     0,   426,   400,
       0,   403,   404,   401,   405,   406,   407,     0,   408,   409,
     410,     0,   411,   412,   413,   414,   415,     0,   416,   417,
     418,   419,   420,   421,   422,   397,     0,   423,   424,   425,
       0,     0,   398,     0,   402,     0,     0,     0,   426,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,   414,   397,     0,   416,   417,   418,
     419,   420,   398,   422,     0,     0,   423,   424,   425,     0,
       0,     0,     0,     0,     0,   399,     0,   426,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,   401,   397,     0,     0,     0,
       0,     0,     0,   398,  1229,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   402,     0,     0,     0,
       0,   400,     0,     0,     0,   401,     0,     0,     0,     0,
       0,     0,   403,   404,  1230,   405,   406,   407,     0,   408,
     409,   410,     0,   411,   412,   413,   414,   415,     0,   416,
     417,   418,   419,   420,   421,   422,   402,     0,   423,   424,
     425,     0,   400,     0,     0,     0,   401,     0,     0,   426,
       0,     0,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   399,   411,   412,   413,   414,   415,     0,   416,
     417,   418,   419,   420,   421,   422,     0,   402,   423,   424,
     425,     0,     0,   397,     0,     0,     0,     0,     0,   426,
     398,     0,     0,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,   399,   411,   412,   413,   414,   415,     0,
     416,   417,   418,   419,   420,   421,   422,     0,   400,   423,
     424,   425,   401,     0,   397,     0,     0,     0,  1243,     0,
     426,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,   401,     0,     0,     0,     0,     0,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,     0,
     411,   412,   413,   414,     0,     0,   416,   417,   418,   419,
     420,     0,   422,     0,   402,   423,   424,   425,     0,     0,
     889,     0,     0,     0,   401,     0,   426,     0,     0,     0,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
       0,   411,   412,   413,   414,   415,     0,   416,   417,   418,
     419,   420,   421,   422,     0,   402,   423,   424,   425,     0,
     399,     0,     0,     0,     0,     0,     0,   426,     0,     0,
       0,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,     0,   411,   412,   413,   414,   415,     0,   416,   417,
     418,   419,   420,   421,   422,     0,   224,   423,   424,   425,
      22,    23,     0,     0,     0,   991,     0,     0,   426,     0,
     225,     0,    31,   226,     0,     0,   400,     0,    37,     0,
     401,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,    61,     0,   992,     0,     0,   993,
       0,   227,     0,    67,     0,     0,     0,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,     0,     0,   412,
       0,     0,    83,     0,     0,    85,   418,   419,    87,     0,
     422,     0,   224,   423,   424,   425,    22,    23,     0,     0,
       0,     0,     0,     0,   426,     0,   225,     0,    31,   226,
       0,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,    42,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -51,     0,     0,   102,     0,
       0,     0,     0,     0,  1224,     0,    57,     0,    59,     0,
       0,     0,    63,     0,   -51,    64,     0,   227,   224,    67,
       0,     0,    22,    23,     0,     0,     0,   991,     0,     0,
       0,     0,   225,     0,    31,   226,     0,     0,    83,     0,
      37,    85,     0,     0,    87,     0,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    57,   370,    59,   232,   352,     0,   992,     0,
       0,   993,     0,   227,     0,    67,     0,     0,     0,   233,
     234,     0,   235,     0,   102,     0,     0,   236,     0,     0,
       0,     0,     0,   371,    83,     0,   237,    85,     0,   232,
      87,     0,   238,     0,   595,     0,     0,     0,   239,     0,
       0,     0,   240,   233,   234,   241,   235,     0,     0,     0,
       0,   236,     0,     0,     0,   242,     0,     0,     0,     0,
     237,   243,   244,     0,     0,     0,   238,     0,     0,   245,
       0,     0,   239,     0,     0,     0,   240,     0,   246,   241,
     102,     0,     0,     0,     0,     0,     0,   247,   248,   242,
     249,     0,   250,     0,   251,   243,   244,   252,     0,     0,
       0,   253,     0,   245,   254,     0,     0,   255,     0,     0,
       0,     0,   246,     0,     0,   232,     0,     0,     0,     0,
       0,   247,   248,     0,   249,     0,   250,     0,   251,   233,
     234,   252,   235,     0,     0,   253,     0,   236,   254,     0,
       0,   255,     0,   371,     0,     0,   237,     0,     0,  -285,
       0,     0,   238,     0,     0,     0,     0,     0,   239,     0,
       0,     0,   240,  -285,  -285,   241,  -285,     0,     0,     0,
       0,  -285,     0,     0,     0,   242,     0,     0,     0,     0,
    -285,   243,   244,     0,     0,     0,  -285,     0,     0,   245,
       0,     0,  -285,     0,     0,     0,  -285,     0,   246,  -285,
       0,     0,     0,     0,     0,     0,     0,   247,   248,  -285,
     249,     0,   250,     0,   251,  -285,  -285,   252,     0,     0,
       0,   253,     0,  -285,   254,     0,     0,   255,     0,     0,
       0,     0,  -285,     0,     0,   232,     0,     0,     0,     0,
       0,  -285,  -285,     0,  -285,     0,  -285,     0,  -285,   233,
     234,  -285,   235,     0,     0,  -285,     0,   236,  -285,     0,
       0,  -285,     0,     0,     0,     0,   237,     0,     0,  -286,
       0,     0,   238,     0,     0,     0,     0,     0,   239,     0,
       0,     0,   240,  -286,  -286,   241,  -286,     0,     0,     0,
       0,  -286,     0,     0,     0,   242,     0,     0,     0,     0,
    -286,   243,   244,     0,     0,     0,  -286,     0,     0,   245,
       0,     0,  -286,     0,     0,     0,  -286,     0,   246,  -286,
       0,     0,     0,     0,     0,     0,     0,   247,   248,  -286,
     249,     0,   250,     0,   251,  -286,  -286,   252,     0,     0,
       0,   253,     0,  -286,   254,     0,     0,   255,     0,     0,
       0,     0,  -286,     0,     0,     0,     0,     0,     0,     0,
       0,  -286,  -286,     0,  -286,     0,  -286,     0,  -286,     0,
       0,  -286,     0,     0,     0,  -286,     0,     0,  -286,     0,
       0,  -286
};

static const yytype_int16 yycheck[] =
{
      12,   172,   448,   656,    16,    69,   389,     2,    20,    19,
     602,   519,   279,    68,    26,    22,   158,   502,   653,   654,
     843,   829,    34,    35,    36,   585,    38,   275,   743,   526,
     828,   588,   521,   371,   944,    27,    27,    49,   545,    51,
     950,   105,     1,    33,    56,   947,    58,    33,    60,    40,
     298,   106,   947,  1000,    56,   987,    68,    69,    70,    71,
      72,    73,    33,    56,     3,    77,    33,    79,   316,    81,
      82,     1,   940,    11,    86,   217,    33,    89,    90,    48,
      92,    18,    94,     3,  1141,    97,    98,   101,   100,    61,
       1,  1054,   104,   105,   106,   107,   119,    37,   119,   145,
      20,    48,    54,  1013,     3,   602,   117,  1203,     0,   145,
     265,   266,   267,    48,    34,    74,   124,   129,     1,   131,
      48,    73,   139,   171,    48,   173,    48,    48,   174,   119,
     147,    83,    72,   119,   145,     1,   172,   145,     3,    48,
      77,   149,    89,    63,  1054,   117,  1203,  1243,   203,    91,
      61,   174,   323,   174,    89,   169,   168,   116,    95,   656,
     875,   164,    91,   102,   172,    48,    56,  1114,   991,   171,
     108,    68,   995,  1105,   145,   112,   145,  1140,   171,   169,
      91,   193,   102,   169,   196,   197,   198,  1055,   108,   201,
     202,   203,   347,   348,   124,    61,   444,   445,   169,    34,
      35,    36,   169,   102,    27,   215,   117,   104,   105,   106,
     119,   170,   169,  1115,   144,   145,    32,   145,   389,   231,
    1115,   145,    74,   145,   145,    91,    91,   169,   164,   736,
    1140,   223,   223,   144,   170,   164,   145,   102,   104,   724,
     169,   124,   172,   119,    88,    27,   174,   145,    92,   169,
     174,   117,   174,   174,   145,    78,   145,   595,   169,    61,
     164,   144,   145,   275,   116,   174,    89,   171,   145,   829,
     778,   106,    88,    47,   172,   164,    92,    56,   144,   170,
     837,   164,   145,  1106,   773,  1108,   298,    27,     3,    91,
    1098,   114,  1100,   305,   306,   172,    78,   743,   164,  1097,
     124,  1099,   104,   169,   316,   202,   203,    89,   139,   172,
      84,    51,   367,   145,   139,   117,   147,   145,   170,  1034,
      61,   145,   147,   139,    56,   149,   108,   145,   144,    56,
     342,   147,   114,   145,   145,   151,    76,   145,    78,   113,
     172,    61,   170,   355,   339,   357,   145,   145,   172,    89,
      91,   943,   359,  1176,   172,   367,   841,   145,   365,    89,
     172,   172,   164,   104,   172,   164,   164,   169,   171,   433,
     434,    91,  1007,  1008,   114,   439,   117,    88,   442,   443,
     888,    92,   170,   395,   104,   397,   398,   399,   400,   401,
     402,   145,   404,   405,   406,   407,   408,   117,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   900,   976,     8,   172,   145,
     475,   433,   434,   164,  1077,     3,   145,   439,   169,   875,
     442,   443,   444,   445,   446,   447,   448,   145,   164,    47,
      82,    47,    20,    85,   164,    56,   943,   459,   590,   169,
     124,   463,   145,   172,   466,   145,    34,    65,   171,    65,
     173,    27,   145,   475,   172,   477,   478,    27,   145,   145,
     367,   483,   484,   145,   857,   149,    84,   489,    84,   172,
     170,   145,    48,    27,   496,    63,    52,   499,   164,   172,
     502,   145,   164,    81,    82,   172,     2,   561,    48,   563,
     164,    56,   171,  1000,   771,   113,    12,   113,   164,    75,
     164,    61,    78,    56,    80,   171,   528,   173,    78,    25,
     532,   533,    28,    89,   102,   164,   538,   139,   171,    89,
     108,   170,   144,   545,    78,   147,   299,   300,  1098,   151,
    1100,    91,   108,   231,    56,    89,   558,    56,   108,   561,
     171,   563,   173,    56,    27,   567,   568,   569,     2,   571,
     572,   171,   171,   173,   108,   400,   401,   117,    12,    56,
      32,    77,   584,   585,   124,    48,   843,    48,   475,    52,
    1077,    25,   577,    99,    28,   597,   853,   275,  1034,   169,
      61,   171,   747,   748,   171,   145,   751,   752,   104,   171,
      56,   173,    75,   109,    56,    78,   172,    80,    56,    56,
     298,   446,   447,   171,   164,   173,    89,  1114,    56,   169,
      91,    56,   172,   171,   459,   173,    88,   561,   316,   563,
      92,   169,   164,    77,   144,   108,   648,    81,    82,   163,
     155,   538,   164,   478,  1141,   164,   117,   164,   171,   484,
     164,  1065,    56,   124,   489,   810,   811,   164,   169,   164,
     104,   558,   164,   164,   561,   109,   563,   108,    84,   118,
     734,   568,   169,   171,   145,   171,   171,   139,   140,   171,
     142,   143,   144,   189,   146,   147,   857,   164,   171,   151,
       8,    65,    87,   164,   169,   124,   158,   159,   169,    89,
     162,   172,   170,   165,   166,   167,  1203,   719,   720,   721,
     722,   723,   724,   725,   726,   124,  1213,    89,   728,   729,
     147,   409,   734,   787,   736,   737,   124,   739,   783,    68,
     164,   743,   567,   745,  1148,  1149,  1150,  1151,  1152,  1153,
      27,   171,   754,   755,   124,   189,  1243,   172,   145,    48,
     171,   124,   150,   124,   152,   153,   444,   445,   156,   157,
      35,    48,    35,    65,   172,    52,   145,   172,     8,   781,
     782,   783,   278,   124,    21,   787,   172,   145,   172,   285,
     104,   174,    22,   174,   171,   147,    26,    27,    75,   147,
     147,    78,   847,    80,   139,   147,    36,   147,    38,    39,
       2,   147,    89,   139,    44,   147,   147,   147,   139,   821,
      12,    51,   147,   825,   502,   827,   828,   829,   147,   139,
     139,   108,   147,    25,   145,    65,    28,   164,   172,   841,
    1078,  1079,   164,   339,   278,   847,    76,   734,    78,  1106,
     174,   285,    82,   171,    84,    85,    35,    87,   354,    89,
     164,   164,   164,   164,    35,   144,   172,   545,   164,   145,
     170,   164,   169,   875,   164,   170,   174,    32,   108,    33,
      33,   111,   144,   174,   114,    77,    48,   889,   890,   891,
     172,    35,   894,   389,   164,   720,   783,   722,   900,    61,
     164,   174,   171,   171,   164,   339,   164,   174,   910,    35,
     912,   139,   104,   174,   959,   917,   169,   109,   920,  1176,
     354,   172,   174,   139,   172,   164,   172,   172,   164,    91,
     932,   164,   164,    88,   164,   164,   145,    92,   163,   163,
     354,   274,    24,   821,   946,   995,    89,   938,   324,   963,
     846,   857,   365,   853,   771,   117,   513,   959,  1115,   852,
     847,   457,   124,   280,  1154,  1147,   968,   862,   970,   129,
     529,   973,   777,    -1,   976,    -1,    -1,    -1,    -1,    -1,
     982,   477,   984,   145,   139,    -1,    -1,   483,    -1,   144,
      -1,   146,   147,    -1,    -1,    -1,   151,   189,   494,    -1,
     496,    -1,   164,    -1,   159,    -1,    -1,   169,    -1,    -1,
     172,  1056,   167,    -1,    -1,  1017,  1018,    -1,  1020,  1021,
      -1,  1023,  1024,   457,    -1,    -1,    -1,  1029,  1030,    -1,
    1032,    -1,  1034,    -1,  1036,  1037,    -1,    -1,  1040,    -1,
      -1,    -1,    -1,   477,    -1,    32,   724,    -1,    -1,   483,
      -1,    -1,    -1,    -1,  1056,    -1,   943,    -1,   736,  1061,
     494,    -1,   496,    -1,   889,   890,    -1,    -1,    -1,   565,
      -1,    -1,   959,   569,  1076,  1077,  1078,  1079,    -1,    -1,
      -1,   577,  1084,    -1,  1086,    -1,   278,    -1,  1090,    -1,
      -1,    -1,    -1,   285,  1096,  1097,  1098,  1099,  1100,    -1,
      -1,    88,    -1,    -1,    -1,    92,     2,    -1,    -1,    -1,
      -1,    -1,  1114,    -1,    -1,    -1,    12,    -1,  1120,  1121,
    1122,  1123,  1124,  1125,  1126,  1127,    -1,    -1,    -1,    25,
      -1,   565,    28,    -1,    -1,   569,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   577,  1146,   823,   824,   339,    -1,    -1,
      -1,    -1,   139,   140,     3,   142,   143,   144,    -1,   146,
     147,    -1,   354,   841,   151,    -1,    -1,    -1,    17,  1056,
      -1,    20,   159,  1175,  1061,    -1,    25,    -1,   165,   166,
     167,    77,    -1,    -1,    -1,    -1,    -1,    36,    -1,  1191,
    1192,  1193,  1194,    -1,    -1,    -1,    -1,    46,    -1,    -1,
    1202,  1203,    -1,    -1,    53,    -1,    -1,    -1,   104,    -1,
      -1,  1213,    -1,   109,    63,  1217,  1218,    -1,    -1,    -1,
     716,    70,   900,  1225,  1226,  1227,  1228,  1229,  1230,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,  1243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,    -1,   102,  1141,    -1,    -1,    -1,     3,    -1,
     109,   757,    -1,    -1,   760,   457,   115,    -1,    -1,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    32,   477,    -1,    -1,    -1,    34,
      -1,   483,    -1,   189,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,   494,    -1,   496,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,   809,    -1,    -1,  1203,    -1,    63,   815,
      -1,    -1,    -1,   757,    69,    70,   760,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    -1,     2,    -1,    -1,    -1,    -1,
      88,    86,    -1,    -1,    92,    12,    -1,    -1,    -1,    -1,
      95,    96,    -1,    98,    -1,   100,  1243,   102,    25,    -1,
     105,    28,    -1,    -1,   109,   110,    -1,   112,    -1,    -1,
     115,    -1,    -1,   565,    -1,   809,    -1,   569,    -1,    -1,
      -1,   815,   278,    -1,    -1,   577,    -1,    -1,    -1,   285,
      -1,   139,   140,   889,   890,   143,   144,    -1,   146,   147,
    1078,  1079,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,
      77,   159,    -1,    -1,   910,    -1,   912,   165,   166,   167,
      -1,   917,    -1,    -1,   920,    -1,   171,    -1,    -1,    -1,
      -1,    -1,    -1,   929,    -1,   931,    -1,   104,   934,    -1,
      -1,    -1,   109,   339,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,    -1,
      -1,    -1,   958,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,   968,    -1,   970,    -1,   910,    -1,   912,    17,
      18,    -1,    20,   917,    -1,    -1,   920,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   929,    34,   931,    -1,    -1,
     934,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,   189,    -1,   958,    63,    -1,    -1,     2,    32,
      -1,    69,    70,  1029,   968,    -1,   970,    -1,    12,    77,
    1036,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    25,    -1,  1049,    28,    -1,    -1,    95,    96,    -1,
      98,   457,   100,    -1,   102,   757,    -1,   105,   760,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,   477,    -1,    -1,  1080,    88,    -1,   483,    -1,    92,
      -1,    -1,    -1,  1089,    -1,  1029,    -1,  1093,   494,    -1,
     496,    -1,  1036,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   278,    -1,    -1,    -1,  1049,    -1,   809,   285,    -1,
      -1,    -1,    -1,   815,    -1,    -1,    -1,    -1,    -1,    -1,
     104,   169,    -1,   171,    32,   109,   139,   140,    30,    -1,
    1136,   144,    -1,   146,   147,    -1,  1080,    -1,   151,    -1,
      -1,    -1,    44,    45,    46,  1089,   159,    -1,    -1,  1093,
      -1,    -1,   165,   166,   167,    -1,    -1,    -1,    -1,   565,
      -1,    -1,   339,   569,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   577,    -1,    -1,    -1,    -1,    -1,   354,    -1,    -1,
      88,    -1,    -1,    -1,    92,  1191,  1192,  1193,  1194,    -1,
      -1,    -1,  1136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,   910,    -1,
     912,  1217,  1218,    -1,    -1,   917,    -1,    -1,   920,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   929,    -1,   931,
      -1,   139,   934,    -1,    -1,    -1,   144,    -1,   146,   147,
      -1,    -1,    -1,   151,   146,    -1,   148,  1191,  1192,  1193,
    1194,   159,    -1,    -1,    -1,   157,   958,   165,   166,   167,
      -1,    -1,    -1,    -1,    -1,    -1,   968,    -1,   970,    -1,
      -1,    -1,    -1,  1217,  1218,    -1,    -1,    -1,    -1,    -1,
     457,    -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,    -1,    -1,   199,    -1,    -1,
     477,   285,    -1,    -1,    -1,    -1,   483,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     2,    -1,    -1,   494,    -1,   496,
      -1,    32,    -1,   225,    12,    -1,    -1,  1029,    -1,    -1,
      -1,    -1,    -1,    -1,  1036,    -1,    -1,    25,    -1,    -1,
      28,    -1,     2,    -1,    -1,    -1,    -1,  1049,    -1,    -1,
      -1,   757,    12,    -1,   760,   339,    -1,    -1,   260,    -1,
      -1,    -1,    -1,    -1,    -1,    25,   268,   269,    28,    -1,
     354,    -1,    -1,    -1,    -1,    -1,    -1,    88,  1080,   281,
      -1,    92,    -1,    -1,    -1,    -1,    -1,  1089,   565,    77,
      -1,  1093,   569,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     577,    -1,    -1,   809,    -1,    -1,    -1,    -1,    -1,   815,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    77,    -1,    -1,
      -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,  1136,   146,   147,   148,    -1,    -1,
     151,   152,   153,    -1,   104,   156,   157,   158,   159,   109,
      -1,   162,    -1,    -1,   165,   166,   167,    -1,    -1,    -1,
      -1,   363,    -1,    -1,   366,   176,    -1,    -1,    -1,   371,
      -1,    -1,    -1,   457,    -1,    -1,    -1,   262,   263,   264,
     265,   266,   267,    -1,    -1,    -1,    -1,   389,    -1,  1191,
    1192,  1193,  1194,   477,    -1,    -1,    -1,    -1,    -1,   483,
      -1,   189,    -1,   288,   910,    -1,   912,    -1,    -1,   294,
     494,   917,   496,    -1,   920,  1217,  1218,    -1,    -1,    -1,
      -1,    -1,    -1,   929,    -1,   931,    -1,   312,   934,   189,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   328,   329,     2,    -1,    -1,    -1,    -1,
      -1,    -1,   958,    -1,   456,    12,    -1,    -1,    -1,    -1,
      -1,    -1,   968,    -1,   970,    -1,    -1,    -1,    25,    -1,
      -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     757,   565,    -1,   760,    -1,   569,    -1,    -1,    -1,    -1,
     278,    -1,    -1,   577,    -1,    -1,    -1,   285,    -1,    -1,
      -1,   503,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   517,    -1,    -1,   278,   521,
      77,    -1,     3,  1029,    -1,   285,    -1,    -1,    -1,    -1,
    1036,    -1,   809,    -1,    -1,    -1,    17,    18,   815,    20,
      -1,    -1,    -1,  1049,    25,    -1,    -1,   104,    -1,    -1,
      -1,   339,   109,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,   354,    -1,    -1,    50,
      -1,    -1,    53,    -1,  1080,   460,    -1,    -1,    -1,   339,
      -1,   583,    63,  1089,    -1,   587,    -1,  1093,    69,    70,
      -1,    -1,    -1,   595,   354,    -1,    77,   482,    -1,    -1,
      -1,   486,    -1,   488,    -1,    86,   491,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,
    1136,   112,   189,   910,   115,   912,    -1,    -1,    -1,    -1,
     917,    -1,    -1,   920,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   929,    -1,   931,    -1,    -1,   934,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   457,
      -1,    -1,    -1,   757,    -1,    -1,   760,    -1,    -1,    -1,
      -1,   958,    -1,    -1,    -1,  1191,  1192,  1193,  1194,   477,
     171,   968,    -1,   970,    -1,   483,    -1,   457,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   494,    -1,   496,    -1,
      -1,  1217,  1218,    -1,    -1,    -1,    -1,   477,    -1,    -1,
      -1,   278,    -1,   483,    -1,   809,    -1,    -1,   285,    -1,
      -1,   815,    -1,    -1,   494,    -1,   496,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,
      -1,    -1,  1029,    -1,    -1,    -1,    -1,    12,    -1,  1036,
     762,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   770,    -1,
      25,   773,  1049,    28,    -1,   777,    -1,   565,    -1,    -1,
      -1,   569,   339,    -1,    -1,    -1,    -1,    -1,    -1,   577,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,   800,    -1,
      -1,    -1,    -1,  1080,    -1,   565,    -1,    -1,    -1,   569,
      -1,    -1,  1089,    -1,    -1,    -1,  1093,   577,    -1,    -1,
      -1,    -1,    77,    -1,    -1,    -1,   910,    -1,   912,    -1,
      -1,    -1,    -1,   917,    -1,    -1,   920,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   846,   929,    -1,   931,    -1,   104,
     934,    -1,    -1,    -1,   109,   857,   741,   742,    -1,  1136,
     862,     3,   747,   748,    -1,    -1,   751,   752,    -1,    -1,
      -1,    -1,    -1,    -1,   958,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,   968,    -1,   970,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
     457,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,  1191,  1192,  1193,  1194,    -1,    -1,
     477,    63,    -1,    -1,    -1,    -1,   483,    69,    70,    -1,
      -1,    -1,    -1,    -1,   189,    77,   938,   494,    -1,   496,
    1217,  1218,    -1,    -1,    86,  1029,    -1,    -1,    -1,    -1,
      -1,    -1,  1036,    95,    96,    -1,    98,    -1,   100,    -1,
     102,   963,    -1,   105,    -1,  1049,    -1,   109,    -1,   757,
     112,    -1,   760,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1080,   757,    -1,    -1,
     760,    -1,    -1,    -1,    -1,  1089,    -1,    -1,   565,  1093,
       1,    -1,   569,    -1,    -1,    -1,   901,   902,    -1,    -1,
     577,   809,   907,   278,    -1,    -1,    -1,   815,    -1,   171,
     285,    22,    -1,    -1,   919,    26,    27,   922,    -1,   924,
      31,   926,    -1,    -1,    -1,    36,    -1,    38,    39,   809,
      -1,    -1,  1136,    44,    -1,   815,    -1,    -1,    -1,    -1,
      51,    -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    73,    -1,   339,    76,    -1,    78,    -1,    80,
      -1,    82,    83,    -1,    85,    -1,    87,    22,    89,   354,
      -1,    26,    27,    -1,    -1,    -1,    -1,  1191,  1192,  1193,
    1194,    36,    -1,    38,    39,    -1,    -1,   108,    -1,    44,
     111,    -1,   910,   114,   912,    -1,    51,    -1,    -1,   917,
      -1,    -1,   920,  1217,  1218,    -1,    -1,    -1,    -1,    -1,
      -1,   929,    -1,   931,    -1,    -1,   934,    -1,    -1,    -1,
     910,    76,   912,    78,    -1,    -1,    -1,   917,    -1,    -1,
     920,    -1,    87,    -1,    89,    -1,    -1,    -1,    -1,   929,
     958,   931,    -1,   164,   934,    -1,    -1,    -1,    -1,   170,
     968,    -1,   970,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1082,   958,    -1,
     757,    -1,   457,   760,    -1,    -1,    -1,    -1,   968,    -1,
     970,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   477,    -1,    -1,    -1,    -1,    -1,   483,    -1,
      -1,    49,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   494,
      -1,  1029,    -1,    -1,    -1,    -1,    -1,    -1,  1036,    -1,
      -1,    -1,   809,    71,    -1,    -1,    -1,    -1,   815,    -1,
      -1,  1049,    -1,    81,    82,    -1,    -1,    -1,    -1,  1029,
      -1,    -1,    -1,    -1,    -1,    -1,  1036,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1049,
      -1,    -1,  1080,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1089,    -1,    -1,    -1,  1093,    -1,    -1,    -1,    -1,
     565,    -1,    -1,    -1,   569,    -1,    -1,    -1,    -1,    -1,
    1080,     8,   577,    -1,    -1,    -1,    -1,    -1,    -1,  1089,
      -1,    -1,    -1,  1093,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1136,    36,
      -1,    38,    39,   910,    -1,   912,    -1,    44,    -1,    -1,
     917,    -1,    -1,   920,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   929,    -1,   931,    -1,  1136,   934,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    -1,    -1,    82,   214,    -1,    85,    -1,
      87,   958,    89,  1191,  1192,  1193,  1194,    -1,    -1,    -1,
      -1,   968,    -1,   970,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,  1217,
    1218,  1191,  1192,  1193,  1194,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,   262,   263,   264,   265,   266,   267,
      -1,    -1,   270,   271,    -1,    -1,    32,  1217,  1218,   277,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     288,    -1,  1029,    -1,    -1,    -1,   294,   164,    -1,  1036,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1049,    -1,   312,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   757,    -1,    -1,   760,    -1,    -1,    -1,    -1,
     328,   329,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,  1080,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,  1089,    -1,    -1,    -1,  1093,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,   809,    -1,   132,   133,   134,   135,
     815,   137,   138,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,  1136,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    -1,    -1,    -1,    -1,    -1,
     176,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   155,    -1,  1191,  1192,  1193,  1194,    -1,    -1,
      -1,    -1,   460,    -1,    -1,   168,    -1,    -1,   171,    -1,
     173,    -1,    -1,    -1,    -1,   910,    -1,   912,    -1,    -1,
    1217,  1218,   917,    -1,   482,   920,    -1,    -1,   486,    -1,
     488,    -1,    -1,   491,   929,    -1,   931,    -1,    -1,   934,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,   958,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,   968,    -1,   970,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,  1049,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1080,   139,   140,   141,   142,
     143,    -1,    -1,    -1,  1089,   148,    -1,    -1,  1093,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,   174,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,  1136,    -1,    -1,    12,    -1,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    22,
      -1,    -1,    -1,    26,    27,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    36,    -1,    38,    39,    -1,    46,    -1,
      -1,    44,    50,   741,   742,    53,    -1,    55,    51,   747,
     748,    -1,    -1,   751,   752,    63,    -1,    -1,    -1,    -1,
      -1,    69,    70,   761,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    76,    -1,    78,    -1,    -1,    86,    82,
      -1,    -1,    85,    -1,    87,    -1,    89,    95,    96,    -1,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,   800,    -1,   112,   108,    -1,   115,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,   146,    -1,
      -1,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    -1,   162,    -1,    -1,   165,   166,   167,
     168,   164,    -1,   171,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   901,   902,    -1,    -1,    -1,    -1,   907,
     168,   909,    -1,   171,    -1,   173,    -1,    -1,   916,    -1,
      -1,   919,    -1,    -1,   922,    -1,   924,    -1,   926,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   939,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,   966,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1082,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1130,  1131,    -1,    -1,  1134,  1135,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1162,     1,  1164,     3,  1166,     5,
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
      -1,   105,    -1,    -1,    -1,   109,   110,    -1,   112,    -1,
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
      -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,     3,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    12,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,   141,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,   171,
      -1,   173,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    95,    96,
      -1,    98,    -1,   100,    19,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    32,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,    -1,    -1,   132,   133,   134,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,    -1,   146,
      -1,    -1,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,   162,    -1,    -1,   165,   166,
     167,   168,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
     126,   127,   128,   129,    -1,    -1,   132,   133,   134,   135,
      -1,   137,   138,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    -1,    12,    -1,    -1,    -1,    -1,    -1,   101,    19,
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
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,    -1,    48,   132,   133,
     134,   135,    -1,   137,   138,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,
      -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,   119,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    88,
      -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
     119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    -1,    88,    -1,    -1,    -1,    92,   176,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    37,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    72,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    72,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,   165,   166,   167,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      32,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    88,    -1,    -1,    -1,    92,
      12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,   123,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    -1,    12,   176,    -1,    -1,    -1,   139,   140,    19,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,    32,    33,   156,   157,   158,   159,   160,   161,
     162,   123,    -1,   165,   166,   167,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,   169,    88,    -1,
      -1,    -1,    92,    12,   176,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    -1,    12,   176,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    88,    -1,    -1,    -1,    92,    -1,   176,    12,
      -1,    -1,    -1,    -1,    -1,   101,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,   119,    37,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    72,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    -1,    -1,    88,    -1,
      -1,   174,    92,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
     165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,
      12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
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
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    13,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,   172,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,
      -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,   123,
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
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
     172,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,
      -1,   172,    92,    12,    13,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   122,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,   172,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,    13,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,   174,
      12,   176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,
      92,    12,   176,    -1,    -1,    -1,    -1,    -1,    19,   101,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
     101,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    61,   156,   157,   158,   159,   160,
     161,   162,    12,    13,   165,   166,   167,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,    88,    32,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    88,   146,
     147,   148,    92,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,   123,    -1,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    12,    -1,   165,   166,   167,    -1,    -1,
      19,    -1,    -1,    -1,   174,    -1,   176,    22,    -1,    -1,
      -1,    26,    27,    32,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,
      -1,    -1,    -1,    12,    -1,    -1,    51,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    82,    -1,    88,
      85,    -1,    87,    92,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   101,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,   123,    -1,   165,   166,   167,   164,
      -1,    -1,    -1,    -1,    -1,   170,    -1,   176,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    12,    -1,   165,   166,   167,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   176,    22,    -1,
      -1,    -1,    26,    27,    32,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    12,    -1,    -1,    51,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    82,    -1,
      88,    85,    -1,    87,    92,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    32,    -1,   123,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   101,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,   123,    -1,   165,   166,   167,
     164,    -1,    -1,    -1,    -1,    -1,   170,    -1,   176,    88,
      -1,   139,   140,    92,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    12,    -1,   165,   166,   167,
      -1,    -1,    19,    -1,   123,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,    12,    -1,   156,   157,   158,
     159,   160,    19,   162,    -1,    -1,   165,   166,   167,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,   176,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,   101,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   101,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,   176,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    32,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,   123,   165,   166,
     167,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   176,
      19,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    32,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    88,   165,
     166,   167,    92,    -1,    12,    -1,    -1,    -1,   174,    -1,
     176,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
     160,    -1,   162,    -1,   123,   165,   166,   167,    -1,    -1,
      88,    -1,    -1,    -1,    92,    -1,   176,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,   123,   165,   166,   167,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    22,   165,   166,   167,
      26,    27,    -1,    -1,    -1,    31,    -1,    -1,   176,    -1,
      36,    -1,    38,    39,    -1,    -1,    88,    -1,    44,    -1,
      92,    -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
      -1,    87,    -1,    89,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,    -1,   151,
      -1,    -1,   108,    -1,    -1,   111,   158,   159,   114,    -1,
     162,    -1,    22,   165,   166,   167,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,   170,    -1,    76,    -1,    78,    -1,
      -1,    -1,    82,    -1,    84,    85,    -1,    87,    22,    89,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,   108,    -1,
      44,   111,    -1,    -1,   114,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,     1,    78,     3,    80,    -1,    82,    -1,
      -1,    85,    -1,    87,    -1,    89,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,   164,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,   108,    -1,    34,   111,    -1,     3,
     114,    -1,    40,    -1,     8,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    17,    18,    53,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      34,    69,    70,    -1,    -1,    -1,    40,    -1,    -1,    77,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    86,    53,
     164,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    63,
      98,    -1,   100,    -1,   102,    69,    70,   105,    -1,    -1,
      -1,   109,    -1,    77,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    86,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,    17,
      18,   105,    20,    -1,    -1,   109,    -1,    25,   112,    -1,
      -1,   115,    -1,    31,    -1,    -1,    34,    -1,    -1,     3,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    17,    18,    53,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      34,    69,    70,    -1,    -1,    -1,    40,    -1,    -1,    77,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    86,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    63,
      98,    -1,   100,    -1,   102,    69,    70,   105,    -1,    -1,
      -1,   109,    -1,    77,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    86,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,    17,
      18,   105,    20,    -1,    -1,   109,    -1,    25,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    34,    -1,    -1,     3,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    17,    18,    53,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      34,    69,    70,    -1,    -1,    -1,    40,    -1,    -1,    77,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    86,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    63,
      98,    -1,   100,    -1,   102,    69,    70,   105,    -1,    -1,
      -1,   109,    -1,    77,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115
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
     236,   237,   241,   242,   245,   246,   247,   250,   251,   256,
     257,   258,   259,   260,   262,   263,   286,   287,   290,   291,
     311,   312,   313,   314,   315,   316,   317,   325,   326,   327,
     328,   329,   332,   333,   334,   335,   336,   337,   338,   339,
     341,   342,   343,   344,   345,   164,   185,   329,   119,   318,
     319,     3,   206,    14,    22,    36,    41,    42,    45,    56,
      87,   100,   169,   173,   233,   256,   311,   316,   327,   328,
     329,   332,   334,   335,   318,   329,   108,   290,    89,   206,
     185,   305,   329,     8,    22,    36,    39,    87,   187,   190,
     185,   171,     3,    17,    18,    20,    25,    34,    40,    46,
      50,    53,    63,    69,    70,    77,    86,    95,    96,    98,
     100,   102,   105,   109,   112,   115,   208,   210,    11,   108,
      78,   121,   227,   329,   227,   329,   227,   329,    27,   114,
     229,   329,    82,    85,   190,   171,   208,   208,   208,   171,
     268,   171,   208,   292,   293,    33,   194,   213,   329,   329,
      18,    77,    95,   112,   329,   329,   329,     8,   171,   218,
     217,     4,   280,   304,   329,   106,   107,   164,   329,   331,
     329,   213,   329,   329,   329,    99,   171,   185,   329,   329,
     194,   205,   329,   332,   194,   205,   329,   329,   229,   329,
     329,   329,   329,   329,   329,   329,     1,   170,   183,   195,
     304,   110,   149,   280,   306,   307,   331,   227,   304,   329,
     340,   329,    80,   185,   169,    47,    84,   113,   191,    26,
     291,   329,     8,    87,   329,   330,    56,   144,   248,   208,
       1,    31,   208,   252,   254,   255,    54,    73,    83,   275,
      27,    78,    89,   108,   276,    27,    78,    89,   108,   274,
     208,   288,   292,   163,   164,   155,   329,    12,    19,    32,
      88,    92,   123,   139,   140,   142,   143,   144,   146,   147,
     148,   150,   151,   152,   153,   154,   156,   157,   158,   159,
     160,   161,   162,   165,   166,   167,   176,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   168,   266,   171,   173,    88,    92,   171,   185,
     164,   329,   329,   329,   208,   304,    56,   169,   194,    48,
     318,   292,   164,   145,   164,   187,   118,   209,   280,   308,
     309,   310,   331,   169,   208,   171,   213,    33,    48,   213,
     119,   213,   321,    33,    48,   213,   321,   213,   321,    48,
     213,   321,   208,   208,   101,   194,   101,   124,   194,   266,
     191,   308,   171,   171,   194,   185,    27,    48,    52,    75,
      78,    89,   108,   184,   269,   270,   271,   272,   273,   276,
     110,   171,   208,   294,   295,     1,   144,   299,    48,   145,
     185,   213,   171,   171,   213,   308,   219,   219,   145,   164,
     329,   329,   164,   169,   213,   171,   308,   164,   238,   164,
     238,   164,   213,   213,   164,   170,   170,   183,   145,   170,
     329,   145,   172,   145,   172,   174,   321,    48,   145,   174,
     321,   122,   145,   174,     8,     1,   170,   195,   202,   203,
     329,   197,   329,    65,    37,    72,   164,    87,   208,   260,
     290,   208,   304,   169,   170,     8,   255,   124,   145,   170,
      89,     1,   144,   298,    89,     1,     3,    12,    17,    19,
      20,    25,    40,    46,    53,    55,    63,    69,    70,    86,
      98,   102,   105,   109,   115,   139,   140,   141,   142,   143,
     144,   146,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   162,   165,   166,   167,   168,   171,   207,
     208,   210,   264,   265,   266,   311,   124,   289,   164,   164,
     329,   329,   329,   329,   227,   329,   227,   329,   329,   329,
     329,   329,   329,   329,     3,    20,    34,    63,   102,   108,
     209,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,    68,   331,
     331,   331,   331,   331,   308,   308,   227,   329,   227,   329,
      27,    48,    89,   114,   320,   323,   324,   329,   345,    33,
      48,    33,    48,   101,   171,    48,   174,   208,   227,   329,
     213,   164,   329,   329,   124,   172,   145,    48,   304,    45,
     329,   227,   329,   171,   213,    45,   329,   227,   329,   213,
     213,   227,   329,   213,   124,   124,   185,    35,   185,   329,
      35,   329,    65,   172,   309,   208,   231,   232,    48,    89,
     272,   145,   172,   171,   208,   299,   295,   145,   172,    34,
      50,    96,   100,   173,   212,   300,   312,   124,   296,   329,
     293,   329,   329,   172,   280,   329,     1,   243,   310,   172,
      21,   239,   170,   172,   172,   306,   172,   306,   185,   174,
     227,   329,   174,   185,   329,   174,   329,   174,   329,   170,
     170,   145,   164,    13,   147,   145,   164,    13,    37,    72,
     208,   167,   196,   199,   329,   196,   198,   208,   248,   292,
     164,   171,     1,   249,   255,   329,   253,   173,   212,   297,
     312,   104,   277,   171,   267,   329,   139,   147,   267,   267,
     300,   312,   145,   171,   173,   164,   164,   164,   164,   164,
     164,   172,   174,    48,    89,   145,   172,    17,    20,    25,
      46,    53,    63,    70,    86,    98,   109,   115,   311,    88,
      88,    45,   227,   329,    45,   227,   329,   309,   227,   329,
     171,   318,   318,   164,   280,   331,   310,   329,   172,   329,
      33,   213,    33,   213,   322,   323,   329,    33,   213,   321,
      33,   213,   321,   213,   321,   213,   321,   329,   329,    35,
     185,    35,    35,   185,   101,   194,   208,   172,   145,   172,
     208,   271,   295,   144,   303,    61,   117,   282,   172,   294,
     299,     1,   304,    68,   331,   172,   172,   170,    74,   116,
     170,   244,   172,   171,   194,   208,   240,   185,   174,   321,
     174,   321,   185,   122,   202,   209,   169,   329,   110,   329,
     199,   198,   145,   164,    13,   164,   248,   169,   164,   309,
     170,    31,    82,    85,   170,   184,   215,   216,   254,     1,
     174,   304,   282,   270,   172,     3,   102,   265,   266,   288,
     172,   174,   323,   299,   311,   311,   329,    33,    33,   329,
      33,    33,   172,   174,   174,   309,   213,   213,   213,   101,
      45,   329,    45,   329,   145,   172,   101,    45,   329,   213,
      45,   329,   213,   213,   213,   185,   185,   329,   185,    35,
     164,   164,   232,   194,   303,   172,   173,   212,   280,   302,
     312,   149,   281,   296,     3,    91,   102,   283,   284,   285,
     329,   193,   214,   279,   296,   174,    48,   174,   171,   171,
      33,   185,   304,   240,   144,   194,    45,   185,   329,   174,
      45,   185,   329,   174,   329,   196,    13,    37,    72,    37,
      72,   164,   164,   329,   329,   169,   249,   172,     8,   216,
     215,   174,   297,   312,   174,   261,   172,   267,   267,   296,
     101,    45,    45,   101,    45,    45,    45,    45,   172,   329,
     329,   329,   323,   329,   329,   329,    35,   185,   281,   296,
     303,   174,   304,   280,   329,   285,   117,   145,   124,   150,
     152,   153,   156,   157,    61,   329,   300,   312,   308,   308,
     185,   213,   172,   329,   329,   185,   329,   185,   170,   110,
     329,   199,   198,   199,   198,    13,   249,   170,   164,   216,
     297,   312,   164,   278,   279,   329,   329,   329,   329,   329,
     329,   101,   101,   101,   101,   185,   281,   296,   280,   301,
     302,   312,    48,   174,   329,   284,   285,   285,   285,   285,
     285,   285,   283,   174,   172,   172,   194,   101,   101,   164,
     164,   164,   164,   329,   170,   101,   101,   101,   101,   101,
     101,   329,   329,   329,   329,   329,   301,   302,   312,   163,
     163,   329,   329,   174,   301
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
     236,   237,   237,   237,   237,   237,   237,   238,   238,   239,
     239,   239,   240,   240,   241,   242,   242,   243,   243,   244,
     244,   244,   245,   245,   246,   247,   247,   247,   248,   248,
     249,   249,   249,   250,   250,   251,   252,   252,   253,   252,
     254,   254,   254,   255,   255,   256,   257,   258,   258,   258,
     259,   261,   260,   262,   262,   262,   262,   262,   263,   264,
     264,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   266,
     266,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   267,   267,   268,   269,   269,   270,   270,   271,   271,
     271,   271,   271,   271,   272,   272,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   274,   274,   274,   274,   274,
     274,   275,   275,   275,   276,   276,   276,   276,   276,   276,
     277,   277,   278,   278,   279,   279,   280,   281,   281,   281,
     282,   282,   282,   282,   282,   283,   283,   284,   284,   284,
     284,   284,   284,   284,   285,   285,   286,   287,   287,   287,
     288,   288,   289,   289,   289,   290,   290,   290,   290,   290,
     291,   291,   292,   292,   293,   293,   294,   294,   294,   295,
     295,   295,   296,   296,   296,   297,   297,   297,   297,   297,
     297,   297,   298,   298,   298,   298,   298,   299,   299,   299,
     299,   299,   300,   300,   300,   300,   301,   301,   301,   302,
     302,   302,   302,   302,   303,   303,   303,   303,   303,   304,
     304,   304,   304,   305,   305,   306,   306,   306,   307,   307,
     308,   308,   309,   309,   310,   310,   310,   310,   311,   311,
     312,   312,   312,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   313,
     313,   313,   313,   313,   313,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   315,   316,   317,   317,   317,   317,   317,   317,
     317,   317,   318,   318,   319,   320,   320,   321,   322,   322,
     323,   323,   323,   324,   324,   324,   324,   324,   324,   325,
     325,   326,   326,   326,   326,   326,   327,   327,   327,   327,
     327,   328,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   330,
     330,   331,   331,   331,   332,   332,   332,   332,   333,   333,
     333,   333,   333,   334,   334,   334,   335,   335,   335,   335,
     335,   335,   336,   336,   336,   336,   337,   337,   338,   338,
     339,   339,   339,   339,   339,   339,   339,   339,   339,   339,
     339,   339,   339,   340,   340,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   341,   341,   342,   342,
     342,   342,   342,   342,   342,   343,   343,   343,   343,   344,
     344,   344,   344,   345,   345,   345,   345,   345,   345,   345
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
       5,     2,     1,     1,     1,     1,     7,     8,     3,     5,
       2,     5,     3,     3,     6,     6,     4,     5,     5,     3,
       3,     6,     5,     6,     5,     6,     3,     4,     3,     4,
       5,     6,     5,     6,     3,     4,     3,     4,     6,     7,
       6,     7,     4,     5,     4,     5,     4,     4,     3,     6,
       5,     4,     3,     6,     5,     6,     5,     8,     7,     4,
       4,     6,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     6,     4,     7,     5,     3,
       6,     3,     3,     2,     2,     3,     3,     0,     2,     2,
       3,     5,     1,     3,     3,     5,     5,     0,     2,     3,
       2,     3,     5,     5,     2,     1,     1,     1,     0,     2,
       0,     2,     3,     3,     3,     3,     1,     2,     0,     4,
       1,     3,     2,     1,     3,     6,     0,     1,     1,     1,
       1,     0,     7,     4,     4,     6,     6,     4,     2,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     3,     3,     1,     3,     0,     1,     4,     5,
       4,     5,     6,     6,     0,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       0,     2,     2,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     3,     1,     2,     2,
       2,     4,     0,     2,     2,     1,     2,     1,     1,     1,
       4,     3,     1,     3,     3,     5,     1,     1,     3,     1,
       2,     3,     0,     2,     2,     3,     2,     4,     3,     3,
       4,     3,     0,     2,     2,     2,     1,     0,     2,     2,
       2,     1,     4,     4,     6,     3,     0,     1,     1,     3,
       4,     3,     4,     6,     0,     2,     2,     2,     2,     1,
       1,     3,     3,     1,     3,     1,     1,     1,     3,     3,
       0,     1,     1,     3,     3,     3,     1,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     2,     4,
       4,     4,     5,     2,     2,     1,     2,     1,     2,     1,
       2,     1,     2,     1,     1,     6,     6,     4,     9,     9,
       7,     6,     6,     4,     9,     9,     7,     4,     6,     6,
       9,     9,     6,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     4,     1,     3,     4,     1,     3,
       4,     3,     3,     1,     1,     2,     1,     2,     1,     1,
       3,     1,     2,     2,     2,     2,     2,     8,     8,     9,
       9,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     3,     3,     2,     2,     2,     1,     0,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     4,     4,     4,     3,     3,     3,     3,
       5,     5,     3,     4,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     4,     3,
       4,     3,     4,     3,     5,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     2,     2,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1
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
#line 641 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6400 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 646 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6406 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 647 "chpl.ypp"
                                      { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6412 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 653 "chpl.ypp"
                                      { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6418 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
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
#line 6440 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 677 "chpl.ypp"
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
#line 6461 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: deprecated_decl_stmt  */
#line 696 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6467 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: include_module_stmt  */
#line 697 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6473 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: block_stmt  */
#line 698 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6479 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: use_stmt  */
#line 699 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6485 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: import_stmt  */
#line 700 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6491 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: require_stmt  */
#line 701 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6497 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: assignment_stmt  */
#line 702 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6503 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6509 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: if_stmt  */
#line 704 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6515 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 705 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6521 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 706 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6527 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: loop_stmt  */
#line 707 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6533 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: select_stmt  */
#line 708 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6539 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: TDEFER stmt  */
#line 710 "chpl.ypp"
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
#line 6556 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: try_stmt  */
#line 722 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6562 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: throw_stmt  */
#line 723 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6568 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: return_stmt  */
#line 724 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6574 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: stmt_level_expr TSEMI  */
#line 725 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6580 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: TATOMIC stmt  */
#line 726 "chpl.ypp"
                            { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6586 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 728 "chpl.ypp"
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
#line 6604 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 742 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6617 "bison-chpl-lib.cpp"
    break;

  case 30: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 751 "chpl.ypp"
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
#line 6636 "bison-chpl-lib.cpp"
    break;

  case 31: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 766 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6649 "bison-chpl-lib.cpp"
    break;

  case 32: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 775 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6661 "bison-chpl-lib.cpp"
    break;

  case 33: /* stmt: TLABEL ident_def stmt  */
#line 783 "chpl.ypp"
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
#line 6688 "bison-chpl-lib.cpp"
    break;

  case 34: /* stmt: TLOCAL expr do_stmt  */
#line 806 "chpl.ypp"
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
#line 6704 "bison-chpl-lib.cpp"
    break;

  case 35: /* stmt: TLOCAL do_stmt  */
#line 818 "chpl.ypp"
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
#line 6719 "bison-chpl-lib.cpp"
    break;

  case 36: /* stmt: TON expr do_stmt  */
#line 829 "chpl.ypp"
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
#line 6735 "bison-chpl-lib.cpp"
    break;

  case 37: /* stmt: TSERIAL expr do_stmt  */
#line 841 "chpl.ypp"
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
#line 6751 "bison-chpl-lib.cpp"
    break;

  case 38: /* stmt: TSERIAL do_stmt  */
#line 853 "chpl.ypp"
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
#line 6766 "bison-chpl-lib.cpp"
    break;

  case 39: /* stmt: TSYNC stmt  */
#line 863 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(TODOSTMT((yyloc))); }
#line 6772 "bison-chpl-lib.cpp"
    break;

  case 40: /* stmt: TYIELD expr TSEMI  */
#line 865 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6783 "bison-chpl-lib.cpp"
    break;

  case 41: /* stmt: error TSEMI  */
#line 872 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
  }
#line 6791 "bison-chpl-lib.cpp"
    break;

  case 43: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 880 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6797 "bison-chpl-lib.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 882 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6803 "bison-chpl-lib.cpp"
    break;

  case 47: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 892 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));

      ModuleParts parts = {context->gatherComments(loc), (yyvsp[-3].visibilityTag), (yyvsp[-2].moduleKind), (yyvsp[0].uniqueStr)};
      (yyval.moduleParts) = parts;
      context->resetDeclState();
      context->clearComments();
    }
#line 6817 "bison-chpl-lib.cpp"
    break;

  case 48: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 905 "chpl.ypp"
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
#line 6834 "bison-chpl-lib.cpp"
    break;

  case 49: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 918 "chpl.ypp"
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
#line 6851 "bison-chpl-lib.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 931 "chpl.ypp"
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
#line 6866 "bison-chpl-lib.cpp"
    break;

  case 51: /* access_control: %empty  */
#line 944 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6872 "bison-chpl-lib.cpp"
    break;

  case 52: /* access_control: TPUBLIC  */
#line 945 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6879 "bison-chpl-lib.cpp"
    break;

  case 53: /* access_control: TPRIVATE  */
#line 947 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6886 "bison-chpl-lib.cpp"
    break;

  case 54: /* opt_prototype: %empty  */
#line 952 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6892 "bison-chpl-lib.cpp"
    break;

  case 55: /* opt_prototype: TPROTOTYPE  */
#line 953 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6899 "bison-chpl-lib.cpp"
    break;

  case 56: /* include_module_stmt: TINCLUDE access_control opt_prototype TMODULE ident_def TSEMI  */
#line 959 "chpl.ypp"
   {
     (yyval.commentsAndStmt) = TODOSTMT((yyloc));
     context->visibility = Decl::DEFAULT_VISIBILITY;
   }
#line 6908 "bison-chpl-lib.cpp"
    break;

  case 57: /* block_stmt_body: TLCBR TRCBR  */
#line 976 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6916 "bison-chpl-lib.cpp"
    break;

  case 58: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 980 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6924 "bison-chpl-lib.cpp"
    break;

  case 59: /* block_stmt_body: TLCBR error TRCBR  */
#line 984 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6936 "bison-chpl-lib.cpp"
    break;

  case 60: /* block_stmt: block_stmt_body  */
#line 995 "chpl.ypp"
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
#line 6955 "bison-chpl-lib.cpp"
    break;

  case 61: /* stmt_ls: toplevel_stmt  */
#line 1013 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6961 "bison-chpl-lib.cpp"
    break;

  case 62: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1014 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6967 "bison-chpl-lib.cpp"
    break;

  case 63: /* renames_ls: expr  */
#line 1019 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur((yyvsp[0].expr));;
      (yyval.renameList)->push_back(cur);
    }
#line 6977 "bison-chpl-lib.cpp"
    break;

  case 64: /* renames_ls: expr TAS expr  */
#line 1025 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur((yyvsp[-2].expr), (yyvsp[0].expr));
      (yyval.renameList)->push_back(cur);
    }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 65: /* renames_ls: renames_ls TCOMMA expr  */
#line 1031 "chpl.ypp"
    {
      PotentialRename cur((yyvsp[0].expr));
      (yyvsp[-2].renameList)->push_back(cur);
    }
#line 6996 "bison-chpl-lib.cpp"
    break;

  case 66: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1036 "chpl.ypp"
    {
      PotentialRename cur((yyvsp[-2].expr), (yyvsp[0].expr));
      (yyvsp[-4].renameList)->push_back(cur);
    }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 67: /* use_renames_ls: expr  */
#line 1046 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur((yyvsp[0].expr));
      (yyval.renameList)->push_back(cur);
    }
#line 7015 "bison-chpl-lib.cpp"
    break;

  case 68: /* use_renames_ls: expr TAS expr  */
#line 1052 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur((yyvsp[-2].expr), (yyvsp[0].expr));
      (yyval.renameList)->push_back(cur);
    }
#line 7025 "bison-chpl-lib.cpp"
    break;

  case 69: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1058 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur((yyvsp[-2].expr), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
      (yyval.renameList)->push_back(cur);
    }
#line 7035 "bison-chpl-lib.cpp"
    break;

  case 70: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1064 "chpl.ypp"
    {
      PotentialRename cur((yyvsp[0].expr));
      (yyvsp[-2].renameList)->push_back(cur);
     }
#line 7044 "bison-chpl-lib.cpp"
    break;

  case 71: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1069 "chpl.ypp"
     {
       PotentialRename cur((yyvsp[-2].expr), (yyvsp[0].expr));
       (yyvsp[-4].renameList)->push_back(cur);
     }
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 72: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1074 "chpl.ypp"
     {
       PotentialRename cur((yyvsp[-2].expr), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
       (yyvsp[-4].renameList)->push_back(cur);
     }
#line 7062 "bison-chpl-lib.cpp"
    break;

  case 73: /* opt_only_ls: %empty  */
#line 1083 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur(context->buildEmptyIdent((yyloc)));
      (yyval.renameList)->push_back(cur);
    }
#line 7072 "bison-chpl-lib.cpp"
    break;

  case 75: /* except_ls: TSTAR  */
#line 1093 "chpl.ypp"
    {
      (yyval.renameList) = new RenameList();
      PotentialRename cur(context->buildEmptyIdent((yylsp[0])));
      (yyval.renameList)->push_back(cur);
     }
#line 7082 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_stmt: access_control TUSE use_renames_ls TSEMI  */
#line 1103 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7091 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_stmt: access_control TUSE expr TEXCEPT except_ls TSEMI  */
#line 1108 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7100 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_stmt: access_control TUSE expr TAS expr TEXCEPT except_ls TSEMI  */
#line 1113 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TEXCEPT except_ls TSEMI  */
#line 1118 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7118 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1123 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7127 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1128 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7136 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1133 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7145 "bison-chpl-lib.cpp"
    break;

  case 84: /* import_stmt: access_control TIMPORT import_ls TSEMI  */
#line 1141 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7154 "bison-chpl-lib.cpp"
    break;

  case 85: /* import_expr: expr  */
#line 1149 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7160 "bison-chpl-lib.cpp"
    break;

  case 86: /* import_expr: expr TAS ident_use  */
#line 1151 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7166 "bison-chpl-lib.cpp"
    break;

  case 87: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1153 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 7172 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_ls: import_expr  */
#line 1158 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7178 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_ls: import_ls TCOMMA import_expr  */
#line 1160 "chpl.ypp"
    { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7184 "bison-chpl-lib.cpp"
    break;

  case 90: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1165 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7192 "bison-chpl-lib.cpp"
    break;

  case 91: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1172 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7200 "bison-chpl-lib.cpp"
    break;

  case 92: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1176 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7208 "bison-chpl-lib.cpp"
    break;

  case 93: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1180 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7216 "bison-chpl-lib.cpp"
    break;

  case 94: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1184 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7224 "bison-chpl-lib.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1188 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7232 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1192 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7240 "bison-chpl-lib.cpp"
    break;

  case 97: /* opt_label_ident: %empty  */
#line 1200 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7246 "bison-chpl-lib.cpp"
    break;

  case 98: /* opt_label_ident: TIDENT  */
#line 1201 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7252 "bison-chpl-lib.cpp"
    break;

  case 99: /* ident_fn_def: TIDENT  */
#line 1205 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7258 "bison-chpl-lib.cpp"
    break;

  case 100: /* ident_fn_def: TNONE  */
#line 1206 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7264 "bison-chpl-lib.cpp"
    break;

  case 101: /* ident_fn_def: TTHIS  */
#line 1207 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7270 "bison-chpl-lib.cpp"
    break;

  case 102: /* ident_fn_def: TFALSE  */
#line 1208 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7276 "bison-chpl-lib.cpp"
    break;

  case 103: /* ident_fn_def: TTRUE  */
#line 1209 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7282 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: internal_type_ident_def  */
#line 1210 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7288 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_def: TIDENT  */
#line 1213 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7294 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_def: TNONE  */
#line 1214 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7300 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_def: TTHIS  */
#line 1215 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'this'"); }
#line 7306 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_def: TFALSE  */
#line 1216 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7312 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_def: TTRUE  */
#line 1217 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7318 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: internal_type_ident_def  */
#line 1218 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7324 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_use: TIDENT  */
#line 1230 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7330 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_use: TTHIS  */
#line 1231 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7336 "bison-chpl-lib.cpp"
    break;

  case 132: /* scalar_type: TBOOL  */
#line 1264 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7342 "bison-chpl-lib.cpp"
    break;

  case 133: /* scalar_type: TENUM  */
#line 1265 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7348 "bison-chpl-lib.cpp"
    break;

  case 134: /* scalar_type: TINT  */
#line 1266 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7354 "bison-chpl-lib.cpp"
    break;

  case 135: /* scalar_type: TUINT  */
#line 1267 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7360 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TREAL  */
#line 1268 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7366 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TIMAG  */
#line 1269 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7372 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TCOMPLEX  */
#line 1270 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7378 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TBYTES  */
#line 1271 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7384 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TSTRING  */
#line 1272 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7390 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TLOCALE  */
#line 1273 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7396 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TNOTHING  */
#line 1274 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7402 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TVOID  */
#line 1275 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7408 "bison-chpl-lib.cpp"
    break;

  case 148: /* do_stmt: TDO stmt  */
#line 1289 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7414 "bison-chpl-lib.cpp"
    break;

  case 149: /* do_stmt: block_stmt  */
#line 1290 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 150: /* return_stmt: TRETURN TSEMI  */
#line 1295 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7431 "bison-chpl-lib.cpp"
    break;

  case 151: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1302 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7442 "bison-chpl-lib.cpp"
    break;

  case 153: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 1313 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7448 "bison-chpl-lib.cpp"
    break;

  case 154: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 1315 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7454 "bison-chpl-lib.cpp"
    break;

  case 155: /* class_level_stmt: TSEMI  */
#line 1320 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), nullptr);
    }
#line 7462 "bison-chpl-lib.cpp"
    break;

  case 156: /* class_level_stmt: inner_class_level_stmt  */
#line 1324 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7472 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@1: %empty  */
#line 1329 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7479 "bison-chpl-lib.cpp"
    break;

  case 158: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1331 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7488 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@2: %empty  */
#line 1335 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 160: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1337 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7504 "bison-chpl-lib.cpp"
    break;

  case 168: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1355 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-2].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr
      (yyval.commentsAndStmt) = cs;
    }
#line 7515 "bison-chpl-lib.cpp"
    break;

  case 169: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT except_ls TSEMI  */
#line 1362 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr and except_ls
      (yyval.commentsAndStmt) = cs;
    }
#line 7526 "bison-chpl-lib.cpp"
    break;

  case 170: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1369 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr and opt_only_ls
      (yyval.commentsAndStmt) = cs;
    }
#line 7537 "bison-chpl-lib.cpp"
    break;

  case 171: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1376 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-1].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save var_decl_stmt
      (yyval.commentsAndStmt) = cs;
    }
#line 7548 "bison-chpl-lib.cpp"
    break;

  case 172: /* forwarding_decl_start: TFORWARDING  */
#line 1386 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7556 "bison-chpl-lib.cpp"
    break;

  case 173: /* extern_or_export: TEXTERN  */
#line 1392 "chpl.ypp"
          { (yyval.linkageTag) = LinkageTag_EXTERN; }
#line 7562 "bison-chpl-lib.cpp"
    break;

  case 174: /* extern_or_export: TEXPORT  */
#line 1393 "chpl.ypp"
          { (yyval.linkageTag) = LinkageTag_EXPORT; }
#line 7568 "bison-chpl-lib.cpp"
    break;

  case 175: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1398 "chpl.ypp"
    {
      CommentsAndLinkage cl = {context->gatherComments((yylsp[0])), (yyvsp[0].linkageTag)};
      (yyval.commentsAndLinkage) = cl;
    }
#line 7577 "bison-chpl-lib.cpp"
    break;

  case 176: /* extern_export_decl_stmt: extern_export_decl_stmt_start TRECORD ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1406 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-6].commentsAndLinkage);
      //auto comments = context->gatherComments(@7);
      // TODO: store comments into the generated list
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7589 "bison-chpl-lib.cpp"
    break;

  case 177: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL TRECORD ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1414 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-7].commentsAndLinkage);
      //auto comments = context->gatherComments(@8);
      // TODO: store comments into the generated list
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7601 "bison-chpl-lib.cpp"
    break;

  case 178: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1422 "chpl.ypp"
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
#line 7625 "bison-chpl-lib.cpp"
    break;

  case 179: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1442 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-4].commentsAndLinkage);
      context->clearComments();
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1452 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7644 "bison-chpl-lib.cpp"
    break;

  case 181: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1459 "chpl.ypp"
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
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = DoWhile::build(BUILDER, LOC((yyloc)), blockStyle,
                               std::move(stmts),
                               toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7666 "bison-chpl-lib.cpp"
    break;

  case 182: /* loop_stmt: TWHILE expr do_stmt  */
#line 1477 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7683 "bison-chpl-lib.cpp"
    break;

  case 183: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1490 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(stmts));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7700 "bison-chpl-lib.cpp"
    break;

  case 184: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1503 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7708 "bison-chpl-lib.cpp"
    break;

  case 185: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1507 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7716 "bison-chpl-lib.cpp"
    break;

  case 186: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1511 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7724 "bison-chpl-lib.cpp"
    break;

  case 187: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1515 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7732 "bison-chpl-lib.cpp"
    break;

  case 188: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1519 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7740 "bison-chpl-lib.cpp"
    break;

  case 189: /* loop_stmt: TFOR expr do_stmt  */
#line 1523 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7748 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1527 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7756 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1531 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-5]), (yylsp[0]), (yyvsp[0].blockOrDo));
    Expression* ident = context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr));
    auto index = context->buildLoopIndexDecl((yylsp[-3]), toOwned(ident));
    auto stmts = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = For::build(BUILDER, LOC((yylsp[-5])), std::move(index),
                           toOwned((yyvsp[-1].expr)),
                           blockStyle,
                           std::move(stmts),
                           /*isExpressionLevel*/ false,
                           /*isParam*/ true);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1549 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7786 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1553 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7794 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1557 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7802 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1561 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7810 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFORALL expr do_stmt  */
#line 1565 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7818 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1569 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7826 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1573 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7834 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1577 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7842 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1581 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7850 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1585 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7858 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1589 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7866 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1593 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7874 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1597 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7882 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1601 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7890 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1605 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7898 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1609 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7906 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1613 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7914 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1617 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7922 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1621 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7930 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1625 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7938 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1629 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7946 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1633 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7954 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1637 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7963 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1642 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7972 "bison-chpl-lib.cpp"
    break;

  case 216: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1650 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7982 "bison-chpl-lib.cpp"
    break;

  case 217: /* if_stmt: TIF expr TTHEN stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7990 "bison-chpl-lib.cpp"
    break;

  case 218: /* if_stmt: TIF expr block_stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7998 "bison-chpl-lib.cpp"
    break;

  case 219: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8006 "bison-chpl-lib.cpp"
    break;

  case 220: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8014 "bison-chpl-lib.cpp"
    break;

  case 221: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8022 "bison-chpl-lib.cpp"
    break;

  case 222: /* if_stmt: TIF ifvar block_stmt  */
#line 1679 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8030 "bison-chpl-lib.cpp"
    break;

  case 223: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1683 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8038 "bison-chpl-lib.cpp"
    break;

  case 224: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1687 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8046 "bison-chpl-lib.cpp"
    break;

  case 225: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1691 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1696 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8064 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1701 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8073 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1706 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8082 "bison-chpl-lib.cpp"
    break;

  case 229: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1714 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr),
                                   Decl::DEFAULT_VISIBILITY,
                                   Variable::VAR,
                                   /*typeExpression*/ nullptr,
                                   toOwned((yyvsp[0].expr)));
    (yyval.expr) = varDecl.release();
  }
#line 8095 "bison-chpl-lib.cpp"
    break;

  case 230: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1723 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr),
                                   Decl::DEFAULT_VISIBILITY,
                                   Variable::CONST,
                                   /*typeExpression*/ nullptr,
                                   toOwned((yyvsp[0].expr)));
    (yyval.expr) = varDecl.release();
  }
#line 8108 "bison-chpl-lib.cpp"
    break;

  case 231: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1735 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8114 "bison-chpl-lib.cpp"
    break;

  case 232: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1737 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8120 "bison-chpl-lib.cpp"
    break;

  case 233: /* ifc_formal_ls: ifc_formal  */
#line 1741 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8126 "bison-chpl-lib.cpp"
    break;

  case 234: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1742 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8132 "bison-chpl-lib.cpp"
    break;

  case 235: /* ifc_formal: ident_def  */
#line 1747 "chpl.ypp"
             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8138 "bison-chpl-lib.cpp"
    break;

  case 248: /* implements_type_ident: implements_type_error_ident  */
#line 1765 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8150 "bison-chpl-lib.cpp"
    break;

  case 255: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1794 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8156 "bison-chpl-lib.cpp"
    break;

  case 256: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1796 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8162 "bison-chpl-lib.cpp"
    break;

  case 257: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1798 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8168 "bison-chpl-lib.cpp"
    break;

  case 258: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1803 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8174 "bison-chpl-lib.cpp"
    break;

  case 259: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1805 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8180 "bison-chpl-lib.cpp"
    break;

  case 260: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1807 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8186 "bison-chpl-lib.cpp"
    break;

  case 261: /* try_stmt: TTRY expr TSEMI  */
#line 1812 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8192 "bison-chpl-lib.cpp"
    break;

  case 262: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1814 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8198 "bison-chpl-lib.cpp"
    break;

  case 263: /* try_stmt: TTRY assignment_stmt  */
#line 1816 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8204 "bison-chpl-lib.cpp"
    break;

  case 264: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1818 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8210 "bison-chpl-lib.cpp"
    break;

  case 265: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1820 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8216 "bison-chpl-lib.cpp"
    break;

  case 266: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1822 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8222 "bison-chpl-lib.cpp"
    break;

  case 267: /* catch_expr_ls: %empty  */
#line 1826 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8228 "bison-chpl-lib.cpp"
    break;

  case 268: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1827 "chpl.ypp"
                           { context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8234 "bison-chpl-lib.cpp"
    break;

  case 269: /* catch_expr: TCATCH block_stmt  */
#line 1831 "chpl.ypp"
                                             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8240 "bison-chpl-lib.cpp"
    break;

  case 270: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1832 "chpl.ypp"
                                             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8246 "bison-chpl-lib.cpp"
    break;

  case 271: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1833 "chpl.ypp"
                                             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8252 "bison-chpl-lib.cpp"
    break;

  case 272: /* catch_expr_inner: ident_def  */
#line 1837 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8258 "bison-chpl-lib.cpp"
    break;

  case 273: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1838 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8264 "bison-chpl-lib.cpp"
    break;

  case 274: /* throw_stmt: TTHROW expr TSEMI  */
#line 1843 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8270 "bison-chpl-lib.cpp"
    break;

  case 275: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1847 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8276 "bison-chpl-lib.cpp"
    break;

  case 276: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1849 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8282 "bison-chpl-lib.cpp"
    break;

  case 277: /* when_stmt_ls: %empty  */
#line 1853 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8288 "bison-chpl-lib.cpp"
    break;

  case 278: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1854 "chpl.ypp"
                          { context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8294 "bison-chpl-lib.cpp"
    break;

  case 279: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1859 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8300 "bison-chpl-lib.cpp"
    break;

  case 280: /* when_stmt: TOTHERWISE stmt  */
#line 1861 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8306 "bison-chpl-lib.cpp"
    break;

  case 281: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1863 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8312 "bison-chpl-lib.cpp"
    break;

  case 282: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1870 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      cs.stmt = TODOEXPR((yyloc));
      context->appendComments(&cs, context->gatherComments((yylsp[0])));
      (yyval.commentsAndStmt) = cs;
    }
#line 8323 "bison-chpl-lib.cpp"
    break;

  case 283: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1877 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      cs.stmt = TODOEXPR((yyloc));
      (yyval.commentsAndStmt) = cs;
    }
#line 8333 "bison-chpl-lib.cpp"
    break;

  case 284: /* class_start: class_tag ident_def  */
#line 1887 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 8341 "bison-chpl-lib.cpp"
    break;

  case 285: /* class_tag: TCLASS  */
#line 1893 "chpl.ypp"
           { (yyval.aggregateTag) = AggregateTag_CLASS; }
#line 8347 "bison-chpl-lib.cpp"
    break;

  case 286: /* class_tag: TRECORD  */
#line 1894 "chpl.ypp"
           { (yyval.aggregateTag) = AggregateTag_RECORD; }
#line 8353 "bison-chpl-lib.cpp"
    break;

  case 287: /* class_tag: TUNION  */
#line 1895 "chpl.ypp"
           { (yyval.aggregateTag) = AggregateTag_UNION; }
#line 8359 "bison-chpl-lib.cpp"
    break;

  case 288: /* opt_inherit: %empty  */
#line 1899 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8365 "bison-chpl-lib.cpp"
    break;

  case 289: /* opt_inherit: TCOLON expr_ls  */
#line 1900 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8371 "bison-chpl-lib.cpp"
    break;

  case 290: /* class_level_stmt_ls: %empty  */
#line 1905 "chpl.ypp"
    { (yyval.exprList) = context->makeList(); }
#line 8377 "bison-chpl-lib.cpp"
    break;

  case 291: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 1907 "chpl.ypp"
    { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8383 "bison-chpl-lib.cpp"
    break;

  case 292: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 1909 "chpl.ypp"
    { context->appendList((yyvsp[-2].exprList), TODOEXPR((yyloc))); }
#line 8389 "bison-chpl-lib.cpp"
    break;

  case 293: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 1914 "chpl.ypp"
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
#line 8408 "bison-chpl-lib.cpp"
    break;

  case 294: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 1929 "chpl.ypp"
    {
      EnumParts parts = (yyvsp[-2].enumParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->resetDeclState();
      context->clearComments();
    }
#line 8421 "bison-chpl-lib.cpp"
    break;

  case 295: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 1941 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-2]));

      EnumParts parts = {context->gatherComments(loc), context->visibility, (yyvsp[-1].uniqueStr)};
      (yyval.enumParts) = parts;

      context->clearComments();
    }
#line 8435 "bison-chpl-lib.cpp"
    break;

  case 296: /* enum_ls: deprecated_enum_item  */
#line 1954 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 8443 "bison-chpl-lib.cpp"
    break;

  case 297: /* enum_ls: enum_ls TCOMMA  */
#line 1958 "chpl.ypp"
    {
      (yyval.exprList) = (yyvsp[-1].exprList);
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8452 "bison-chpl-lib.cpp"
    break;

  case 298: /* $@3: %empty  */
#line 1962 "chpl.ypp"
                 {context->clearCommentsBefore((yylsp[0]));}
#line 8458 "bison-chpl-lib.cpp"
    break;

  case 299: /* enum_ls: enum_ls TCOMMA $@3 deprecated_enum_item  */
#line 1963 "chpl.ypp"
    {
      context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 8466 "bison-chpl-lib.cpp"
    break;

  case 301: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 1971 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8472 "bison-chpl-lib.cpp"
    break;

  case 302: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 1973 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8478 "bison-chpl-lib.cpp"
    break;

  case 303: /* enum_item: ident_def  */
#line 1978 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)), (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8487 "bison-chpl-lib.cpp"
    break;

  case 304: /* enum_item: ident_def TASSIGN expr  */
#line 1983 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8497 "bison-chpl-lib.cpp"
    break;

  case 305: /* lambda_decl_expr: TLAMBDA req_formal_ls opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 1992 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 8505 "bison-chpl-lib.cpp"
    break;

  case 307: /* linkage_spec: linkage_spec_empty  */
#line 2001 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8511 "bison-chpl-lib.cpp"
    break;

  case 308: /* linkage_spec: TINLINE  */
#line 2002 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8518 "bison-chpl-lib.cpp"
    break;

  case 309: /* linkage_spec: TOVERRIDE  */
#line 2004 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8525 "bison-chpl-lib.cpp"
    break;

  case 310: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2010 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8533 "bison-chpl-lib.cpp"
    break;

  case 311: /* $@4: %empty  */
#line 2019 "chpl.ypp"
    { context->clearComments(); }
#line 8539 "bison-chpl-lib.cpp"
    break;

  case 312: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@4 opt_function_body_stmt  */
#line 2021 "chpl.ypp"
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
#line 8559 "bison-chpl-lib.cpp"
    break;

  case 313: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2040 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      // TODO -- do something with $2
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8571 "bison-chpl-lib.cpp"
    break;

  case 314: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2048 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      // TODO -- do something with $2
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 315: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2056 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])),
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8597 "bison-chpl-lib.cpp"
    break;

  case 316: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2066 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])),
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8611 "bison-chpl-lib.cpp"
    break;

  case 317: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2076 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8621 "bison-chpl-lib.cpp"
    break;

  case 318: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2085 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-1].functionParts);
      auto loc = context->declStartLoc((yylsp[0]));
      fp.comments = context->gatherComments(loc);
      fp.kind = (yyvsp[0].functionKind);
      (yyval.functionParts) = fp;
    }
#line 8633 "bison-chpl-lib.cpp"
    break;

  case 320: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2096 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8639 "bison-chpl-lib.cpp"
    break;

  case 348: /* fn_ident: ident_def TBANG  */
#line 2128 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8649 "bison-chpl-lib.cpp"
    break;

  case 361: /* opt_formal_ls: %empty  */
#line 2151 "chpl.ypp"
                     { (yyval.exprList) = nullptr; /* TODO: mark as no-parens */ }
#line 8655 "bison-chpl-lib.cpp"
    break;

  case 362: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2152 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8661 "bison-chpl-lib.cpp"
    break;

  case 363: /* req_formal_ls: TLP formal_ls TRP  */
#line 2156 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8667 "bison-chpl-lib.cpp"
    break;

  case 364: /* formal_ls_inner: formal  */
#line 2160 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8673 "bison-chpl-lib.cpp"
    break;

  case 365: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2161 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8679 "bison-chpl-lib.cpp"
    break;

  case 366: /* formal_ls: %empty  */
#line 2165 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8685 "bison-chpl-lib.cpp"
    break;

  case 367: /* formal_ls: formal_ls_inner  */
#line 2166 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8691 "bison-chpl-lib.cpp"
    break;

  case 368: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2171 "chpl.ypp"
    { (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), (yyvsp[-3].intentTag),
                         toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr))).release(); }
#line 8698 "bison-chpl-lib.cpp"
    break;

  case 369: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2174 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8704 "bison-chpl-lib.cpp"
    break;

  case 370: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 2176 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8710 "bison-chpl-lib.cpp"
    break;

  case 371: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 2178 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 372: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2180 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8722 "bison-chpl-lib.cpp"
    break;

  case 373: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 2182 "chpl.ypp"
    {
      (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
    }
#line 8730 "bison-chpl-lib.cpp"
    break;

  case 374: /* opt_intent_tag: %empty  */
#line 2188 "chpl.ypp"
                      { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8736 "bison-chpl-lib.cpp"
    break;

  case 375: /* opt_intent_tag: required_intent_tag  */
#line 2189 "chpl.ypp"
                      { (yyval.intentTag) = (yyvsp[0].intentTag); }
#line 8742 "bison-chpl-lib.cpp"
    break;

  case 376: /* required_intent_tag: TIN  */
#line 2193 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8748 "bison-chpl-lib.cpp"
    break;

  case 377: /* required_intent_tag: TINOUT  */
#line 2194 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8754 "bison-chpl-lib.cpp"
    break;

  case 378: /* required_intent_tag: TOUT  */
#line 2195 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8760 "bison-chpl-lib.cpp"
    break;

  case 379: /* required_intent_tag: TCONST TIN  */
#line 2196 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8766 "bison-chpl-lib.cpp"
    break;

  case 380: /* required_intent_tag: TCONST TREF  */
#line 2197 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8772 "bison-chpl-lib.cpp"
    break;

  case 381: /* required_intent_tag: TCONST  */
#line 2198 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8778 "bison-chpl-lib.cpp"
    break;

  case 382: /* required_intent_tag: TPARAM  */
#line 2199 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8784 "bison-chpl-lib.cpp"
    break;

  case 383: /* required_intent_tag: TREF  */
#line 2200 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8790 "bison-chpl-lib.cpp"
    break;

  case 384: /* required_intent_tag: TTYPE  */
#line 2201 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8796 "bison-chpl-lib.cpp"
    break;

  case 385: /* opt_this_intent_tag: %empty  */
#line 2205 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8802 "bison-chpl-lib.cpp"
    break;

  case 386: /* opt_this_intent_tag: TPARAM  */
#line 2206 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8808 "bison-chpl-lib.cpp"
    break;

  case 387: /* opt_this_intent_tag: TREF  */
#line 2207 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8814 "bison-chpl-lib.cpp"
    break;

  case 388: /* opt_this_intent_tag: TCONST TREF  */
#line 2208 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8820 "bison-chpl-lib.cpp"
    break;

  case 389: /* opt_this_intent_tag: TCONST  */
#line 2209 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8826 "bison-chpl-lib.cpp"
    break;

  case 390: /* opt_this_intent_tag: TTYPE  */
#line 2210 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 391: /* proc_iter_or_op: TPROC  */
#line 2214 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8838 "bison-chpl-lib.cpp"
    break;

  case 392: /* proc_iter_or_op: TITER  */
#line 2215 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8844 "bison-chpl-lib.cpp"
    break;

  case 393: /* proc_iter_or_op: TOPERATOR  */
#line 2216 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 394: /* opt_ret_tag: %empty  */
#line 2220 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8856 "bison-chpl-lib.cpp"
    break;

  case 395: /* opt_ret_tag: TCONST  */
#line 2221 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 8862 "bison-chpl-lib.cpp"
    break;

  case 396: /* opt_ret_tag: TCONST TREF  */
#line 2222 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 8868 "bison-chpl-lib.cpp"
    break;

  case 397: /* opt_ret_tag: TREF  */
#line 2223 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 8874 "bison-chpl-lib.cpp"
    break;

  case 398: /* opt_ret_tag: TPARAM  */
#line 2224 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 8880 "bison-chpl-lib.cpp"
    break;

  case 399: /* opt_ret_tag: TTYPE  */
#line 2225 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 8886 "bison-chpl-lib.cpp"
    break;

  case 400: /* opt_throws_error: %empty  */
#line 2229 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 8892 "bison-chpl-lib.cpp"
    break;

  case 401: /* opt_throws_error: TTHROWS  */
#line 2230 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 8898 "bison-chpl-lib.cpp"
    break;

  case 402: /* opt_function_body_stmt: TSEMI  */
#line 2233 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 8904 "bison-chpl-lib.cpp"
    break;

  case 403: /* opt_function_body_stmt: function_body_stmt  */
#line 2234 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8910 "bison-chpl-lib.cpp"
    break;

  case 404: /* function_body_stmt: block_stmt_body  */
#line 2238 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8916 "bison-chpl-lib.cpp"
    break;

  case 405: /* function_body_stmt: return_stmt  */
#line 2239 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 8922 "bison-chpl-lib.cpp"
    break;

  case 406: /* query_expr: TQUERIEDIDENT  */
#line 2243 "chpl.ypp"
                      { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8928 "bison-chpl-lib.cpp"
    break;

  case 407: /* var_arg_expr: TDOTDOTDOT  */
#line 2247 "chpl.ypp"
                         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8934 "bison-chpl-lib.cpp"
    break;

  case 408: /* var_arg_expr: TDOTDOTDOT expr  */
#line 2248 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8940 "bison-chpl-lib.cpp"
    break;

  case 409: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 2249 "chpl.ypp"
                         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8946 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_lifetime_where: %empty  */
#line 2253 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 8952 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_lifetime_where: TWHERE expr  */
#line 2255 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 8958 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2257 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 8964 "bison-chpl-lib.cpp"
    break;

  case 413: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2259 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 8970 "bison-chpl-lib.cpp"
    break;

  case 414: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2261 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 8976 "bison-chpl-lib.cpp"
    break;

  case 415: /* lifetime_components_expr: lifetime_expr  */
#line 2266 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8982 "bison-chpl-lib.cpp"
    break;

  case 416: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2268 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 417: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2273 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 8994 "bison-chpl-lib.cpp"
    break;

  case 418: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2275 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9000 "bison-chpl-lib.cpp"
    break;

  case 419: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2277 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 420: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2279 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 421: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2281 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 422: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2283 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 423: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2285 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 424: /* lifetime_ident: TIDENT  */
#line 2289 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 425: /* lifetime_ident: TTHIS  */
#line 2290 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 426: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner TSEMI  */
#line 2295 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[-2].commentsAndStmt)); // TODO
    }
#line 9050 "bison-chpl-lib.cpp"
    break;

  case 427: /* type_alias_decl_stmt_start: TTYPE  */
#line 2302 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 9058 "bison-chpl-lib.cpp"
    break;

  case 428: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2306 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 429: /* type_alias_decl_stmt_start: TEXTERN TTYPE  */
#line 2311 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 9074 "bison-chpl-lib.cpp"
    break;

  case 430: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2319 "chpl.ypp"
    {
      // set name to $1
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9083 "bison-chpl-lib.cpp"
    break;

  case 431: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 2324 "chpl.ypp"
    {
      // set name to $1
      // add new var to multivar
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9093 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_init_type: %empty  */
#line 2332 "chpl.ypp"
    { (yyval.expr) = nullptr; }
#line 9099 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_init_type: TASSIGN type_level_expr  */
#line 2334 "chpl.ypp"
    { (yyval.expr) = (yyvsp[0].expr); }
#line 9105 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_init_type: TASSIGN array_type  */
#line 2336 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9111 "bison-chpl-lib.cpp"
    break;

  case 435: /* var_decl_type: TPARAM  */
#line 2340 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9117 "bison-chpl-lib.cpp"
    break;

  case 436: /* var_decl_type: TCONST TREF  */
#line 2341 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9123 "bison-chpl-lib.cpp"
    break;

  case 437: /* var_decl_type: TREF  */
#line 2342 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9129 "bison-chpl-lib.cpp"
    break;

  case 438: /* var_decl_type: TCONST  */
#line 2343 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9135 "bison-chpl-lib.cpp"
    break;

  case 439: /* var_decl_type: TVAR  */
#line 2344 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9141 "bison-chpl-lib.cpp"
    break;

  case 440: /* var_decl_stmt: TCONFIG var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2349 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
      context->resetDeclState();
    }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 441: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2354 "chpl.ypp"
    {
      ParserExprList* lst = (yyvsp[-1].exprList);
      int nDecls = 0;
      Decl* lastDecl = nullptr;
      for (auto elt : *lst) {
        if (Decl* d = elt->toDecl()) {
          lastDecl = d;
          nDecls++;
        }
      }
      assert(nDecls > 0);
      assert(lastDecl);
      if (nDecls == 1) {
        // gather any comments from before the 'var'
        // intentionally ignores comments after 'var'
        CommentsAndStmt cs = {context->gatherCommentsFromList(lst, (yylsp[-2])),
                              lastDecl};
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
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 442: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2395 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9200 "bison-chpl-lib.cpp"
    break;

  case 443: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2399 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9208 "bison-chpl-lib.cpp"
    break;

  case 444: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2406 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     (yyvsp[-2].uniqueStr), context->visibility,
                                     context->varDeclKind,
                                     toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr)));
      // gather the comments like a statement does
      (yyval.commentsAndStmt) = STMT((yylsp[-2]), varDecl.release());
      context->clearComments();
    }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 445: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2416 "chpl.ypp"
    {
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        context->visibility,
                                        context->varDeclKind,
                                        context->consumeList((yyvsp[-3].exprList)),
                                        toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yylsp[-4]), tupleDecl.release());
      context->clearComments();
    }
#line 9236 "bison-chpl-lib.cpp"
    break;

  case 446: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2429 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     (yyvsp[0].uniqueStr), context->visibility,
                                     context->varDeclKind,
                                     nullptr, nullptr);
      (yyval.expr) = varDecl.release();
    }
#line 9248 "bison-chpl-lib.cpp"
    break;

  case 447: /* tuple_var_decl_component: ident_def  */
#line 2438 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     (yyvsp[0].uniqueStr), context->visibility,
                                     context->varDeclKind,
                                     nullptr, nullptr);
      (yyval.expr) = varDecl.release();
    }
#line 9260 "bison-chpl-lib.cpp"
    break;

  case 448: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2447 "chpl.ypp"
    {
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        context->visibility,
                                        context->varDeclKind,
                                        context->consumeList((yyvsp[-1].exprList)),
                                        nullptr, nullptr);
      (yyval.expr) = tupleDecl.release();
    }
#line 9273 "bison-chpl-lib.cpp"
    break;

  case 449: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2459 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9279 "bison-chpl-lib.cpp"
    break;

  case 450: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2461 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9285 "bison-chpl-lib.cpp"
    break;

  case 451: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2463 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9291 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_init_expr: %empty  */
#line 2469 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9297 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2470 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9303 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2471 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9309 "bison-chpl-lib.cpp"
    break;

  case 455: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2477 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9317 "bison-chpl-lib.cpp"
    break;

  case 456: /* ret_array_type: TLSBR TRSBR  */
#line 2481 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 457: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2485 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9333 "bison-chpl-lib.cpp"
    break;

  case 458: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2489 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9341 "bison-chpl-lib.cpp"
    break;

  case 459: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2493 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9349 "bison-chpl-lib.cpp"
    break;

  case 460: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2497 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9357 "bison-chpl-lib.cpp"
    break;

  case 461: /* ret_array_type: TLSBR error TRSBR  */
#line 2501 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9365 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_ret_type: %empty  */
#line 2507 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_ret_type: TCOLON type_level_expr  */
#line 2508 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_ret_type: TCOLON ret_array_type  */
#line 2509 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9383 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2510 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9389 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_ret_type: error  */
#line 2511 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_type: %empty  */
#line 2516 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_type: TCOLON type_level_expr  */
#line 2517 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_type: TCOLON array_type  */
#line 2518 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9413 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2519 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9419 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_type: error  */
#line 2520 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9425 "bison-chpl-lib.cpp"
    break;

  case 472: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2541 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9433 "bison-chpl-lib.cpp"
    break;

  case 473: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2545 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 474: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2549 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 475: /* array_type: TLSBR error TRSBR  */
#line 2553 "chpl.ypp"
    {
      (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
    }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_formal_array_elt_type: %empty  */
#line 2559 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9463 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_formal_array_elt_type: type_level_expr  */
#line 2560 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9469 "bison-chpl-lib.cpp"
    break;

  case 478: /* opt_formal_array_elt_type: query_expr  */
#line 2561 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 479: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2566 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9481 "bison-chpl-lib.cpp"
    break;

  case 480: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2568 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9487 "bison-chpl-lib.cpp"
    break;

  case 481: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2574 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 482: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2576 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 483: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2578 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_formal_type: %empty  */
#line 2582 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9511 "bison-chpl-lib.cpp"
    break;

  case 485: /* opt_formal_type: TCOLON type_level_expr  */
#line 2583 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 486: /* opt_formal_type: TCOLON query_expr  */
#line 2584 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2585 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_formal_type: TCOLON formal_array_type  */
#line 2586 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 489: /* expr_ls: expr  */
#line 2592 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 490: /* expr_ls: query_expr  */
#line 2593 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 491: /* expr_ls: expr_ls TCOMMA expr  */
#line 2594 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 492: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2595 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9559 "bison-chpl-lib.cpp"
    break;

  case 493: /* simple_expr_ls: expr  */
#line 2599 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9565 "bison-chpl-lib.cpp"
    break;

  case 494: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2600 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9571 "bison-chpl-lib.cpp"
    break;

  case 495: /* tuple_component: TUNDERSCORE  */
#line 2604 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 496: /* tuple_component: opt_try_expr  */
#line 2605 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9583 "bison-chpl-lib.cpp"
    break;

  case 497: /* tuple_component: query_expr  */
#line 2606 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9589 "bison-chpl-lib.cpp"
    break;

  case 498: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2610 "chpl.ypp"
                                         { (yyval.exprList) = TODOLIST((yyloc)); }
#line 9595 "bison-chpl-lib.cpp"
    break;

  case 499: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2611 "chpl.ypp"
                                         { (yyval.exprList) = TODOLIST((yyloc)); }
#line 9601 "bison-chpl-lib.cpp"
    break;

  case 500: /* opt_actual_ls: %empty  */
#line 2615 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9607 "bison-chpl-lib.cpp"
    break;

  case 501: /* opt_actual_ls: actual_ls  */
#line 2616 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9613 "bison-chpl-lib.cpp"
    break;

  case 502: /* actual_ls: actual_expr  */
#line 2621 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9622 "bison-chpl-lib.cpp"
    break;

  case 503: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2626 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 504: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2634 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9638 "bison-chpl-lib.cpp"
    break;

  case 505: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2635 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9644 "bison-chpl-lib.cpp"
    break;

  case 506: /* actual_expr: query_expr  */
#line 2636 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9650 "bison-chpl-lib.cpp"
    break;

  case 507: /* actual_expr: opt_try_expr  */
#line 2637 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 508: /* ident_expr: ident_use  */
#line 2641 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9662 "bison-chpl-lib.cpp"
    break;

  case 509: /* ident_expr: scalar_type  */
#line 2642 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9668 "bison-chpl-lib.cpp"
    break;

  case 510: /* type_level_expr: sub_type_level_expr  */
#line 2654 "chpl.ypp"
    { (yyval.expr) = (yyvsp[0].expr); }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 511: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2656 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9680 "bison-chpl-lib.cpp"
    break;

  case 512: /* type_level_expr: TQUESTION  */
#line 2658 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9686 "bison-chpl-lib.cpp"
    break;

  case 518: /* sub_type_level_expr: TSINGLE expr  */
#line 2669 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9692 "bison-chpl-lib.cpp"
    break;

  case 519: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2671 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9698 "bison-chpl-lib.cpp"
    break;

  case 520: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2673 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9704 "bison-chpl-lib.cpp"
    break;

  case 521: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2675 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 522: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2677 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9716 "bison-chpl-lib.cpp"
    break;

  case 523: /* sub_type_level_expr: TATOMIC expr  */
#line 2679 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9722 "bison-chpl-lib.cpp"
    break;

  case 524: /* sub_type_level_expr: TSYNC expr  */
#line 2681 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9728 "bison-chpl-lib.cpp"
    break;

  case 525: /* sub_type_level_expr: TOWNED  */
#line 2684 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9734 "bison-chpl-lib.cpp"
    break;

  case 526: /* sub_type_level_expr: TOWNED expr  */
#line 2686 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9740 "bison-chpl-lib.cpp"
    break;

  case 527: /* sub_type_level_expr: TUNMANAGED  */
#line 2688 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9746 "bison-chpl-lib.cpp"
    break;

  case 528: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2690 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9752 "bison-chpl-lib.cpp"
    break;

  case 529: /* sub_type_level_expr: TSHARED  */
#line 2692 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9758 "bison-chpl-lib.cpp"
    break;

  case 530: /* sub_type_level_expr: TSHARED expr  */
#line 2694 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9764 "bison-chpl-lib.cpp"
    break;

  case 531: /* sub_type_level_expr: TBORROWED  */
#line 2696 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9770 "bison-chpl-lib.cpp"
    break;

  case 532: /* sub_type_level_expr: TBORROWED expr  */
#line 2698 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9776 "bison-chpl-lib.cpp"
    break;

  case 533: /* sub_type_level_expr: TCLASS  */
#line 2701 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9782 "bison-chpl-lib.cpp"
    break;

  case 534: /* sub_type_level_expr: TRECORD  */
#line 2703 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9788 "bison-chpl-lib.cpp"
    break;

  case 535: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2708 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9794 "bison-chpl-lib.cpp"
    break;

  case 536: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2710 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9800 "bison-chpl-lib.cpp"
    break;

  case 537: /* for_expr: TFOR expr TDO expr  */
#line 2712 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9806 "bison-chpl-lib.cpp"
    break;

  case 538: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2714 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9812 "bison-chpl-lib.cpp"
    break;

  case 539: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2716 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9818 "bison-chpl-lib.cpp"
    break;

  case 540: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2718 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9824 "bison-chpl-lib.cpp"
    break;

  case 541: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2720 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9830 "bison-chpl-lib.cpp"
    break;

  case 542: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2722 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9836 "bison-chpl-lib.cpp"
    break;

  case 543: /* for_expr: TFORALL expr TDO expr  */
#line 2724 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9842 "bison-chpl-lib.cpp"
    break;

  case 544: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2726 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9848 "bison-chpl-lib.cpp"
    break;

  case 545: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2728 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9854 "bison-chpl-lib.cpp"
    break;

  case 546: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2730 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9860 "bison-chpl-lib.cpp"
    break;

  case 547: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2732 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9866 "bison-chpl-lib.cpp"
    break;

  case 548: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2734 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9872 "bison-chpl-lib.cpp"
    break;

  case 549: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2736 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9878 "bison-chpl-lib.cpp"
    break;

  case 550: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2738 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9884 "bison-chpl-lib.cpp"
    break;

  case 551: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2740 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9890 "bison-chpl-lib.cpp"
    break;

  case 552: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2745 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9896 "bison-chpl-lib.cpp"
    break;

  case 553: /* nil_expr: TNIL  */
#line 2754 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9902 "bison-chpl-lib.cpp"
    break;

  case 561: /* stmt_level_expr: io_expr TIO expr  */
#line 2770 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9908 "bison-chpl-lib.cpp"
    break;

  case 562: /* opt_task_intent_ls: %empty  */
#line 2774 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 9914 "bison-chpl-lib.cpp"
    break;

  case 563: /* opt_task_intent_ls: task_intent_clause  */
#line 2775 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 9920 "bison-chpl-lib.cpp"
    break;

  case 564: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2780 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9930 "bison-chpl-lib.cpp"
    break;

  case 565: /* task_intent_ls: intent_expr  */
#line 2788 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9936 "bison-chpl-lib.cpp"
    break;

  case 566: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2789 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9942 "bison-chpl-lib.cpp"
    break;

  case 567: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2794 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 9952 "bison-chpl-lib.cpp"
    break;

  case 568: /* forall_intent_ls: intent_expr  */
#line 2802 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9958 "bison-chpl-lib.cpp"
    break;

  case 569: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2803 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 570: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 2808 "chpl.ypp"
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
#line 9981 "bison-chpl-lib.cpp"
    break;

  case 571: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2821 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9989 "bison-chpl-lib.cpp"
    break;

  case 572: /* intent_expr: expr TREDUCE ident_expr  */
#line 2825 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9997 "bison-chpl-lib.cpp"
    break;

  case 573: /* task_var_prefix: TCONST  */
#line 2831 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10003 "bison-chpl-lib.cpp"
    break;

  case 574: /* task_var_prefix: TIN  */
#line 2832 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10009 "bison-chpl-lib.cpp"
    break;

  case 575: /* task_var_prefix: TCONST TIN  */
#line 2833 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10015 "bison-chpl-lib.cpp"
    break;

  case 576: /* task_var_prefix: TREF  */
#line 2834 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10021 "bison-chpl-lib.cpp"
    break;

  case 577: /* task_var_prefix: TCONST TREF  */
#line 2835 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10027 "bison-chpl-lib.cpp"
    break;

  case 578: /* task_var_prefix: TVAR  */
#line 2836 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10033 "bison-chpl-lib.cpp"
    break;

  case 580: /* io_expr: io_expr TIO expr  */
#line 2842 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10039 "bison-chpl-lib.cpp"
    break;

  case 581: /* new_maybe_decorated: TNEW  */
#line 2847 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10045 "bison-chpl-lib.cpp"
    break;

  case 582: /* new_maybe_decorated: TNEW TOWNED  */
#line 2849 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10051 "bison-chpl-lib.cpp"
    break;

  case 583: /* new_maybe_decorated: TNEW TSHARED  */
#line 2851 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10057 "bison-chpl-lib.cpp"
    break;

  case 584: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2853 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10063 "bison-chpl-lib.cpp"
    break;

  case 585: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2855 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10069 "bison-chpl-lib.cpp"
    break;

  case 586: /* new_expr: new_maybe_decorated expr  */
#line 2861 "chpl.ypp"
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
#line 10087 "bison-chpl-lib.cpp"
    break;

  case 587: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2878 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10095 "bison-chpl-lib.cpp"
    break;

  case 588: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2882 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10103 "bison-chpl-lib.cpp"
    break;

  case 589: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2886 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10111 "bison-chpl-lib.cpp"
    break;

  case 590: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2890 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10119 "bison-chpl-lib.cpp"
    break;

  case 591: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2897 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10125 "bison-chpl-lib.cpp"
    break;

  case 601: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2914 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10131 "bison-chpl-lib.cpp"
    break;

  case 602: /* expr: expr TCOLON expr  */
#line 2916 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10137 "bison-chpl-lib.cpp"
    break;

  case 603: /* expr: expr TDOTDOT expr  */
#line 2918 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10143 "bison-chpl-lib.cpp"
    break;

  case 604: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2920 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10149 "bison-chpl-lib.cpp"
    break;

  case 605: /* expr: expr TDOTDOT  */
#line 2922 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10155 "bison-chpl-lib.cpp"
    break;

  case 606: /* expr: TDOTDOT expr  */
#line 2924 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10161 "bison-chpl-lib.cpp"
    break;

  case 607: /* expr: TDOTDOTOPENHIGH expr  */
#line 2926 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10167 "bison-chpl-lib.cpp"
    break;

  case 608: /* expr: TDOTDOT  */
#line 2928 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10173 "bison-chpl-lib.cpp"
    break;

  case 609: /* opt_expr: %empty  */
#line 2946 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10179 "bison-chpl-lib.cpp"
    break;

  case 610: /* opt_expr: expr  */
#line 2947 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10185 "bison-chpl-lib.cpp"
    break;

  case 611: /* opt_try_expr: TTRY expr  */
#line 2951 "chpl.ypp"
                  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10191 "bison-chpl-lib.cpp"
    break;

  case 612: /* opt_try_expr: TTRYBANG expr  */
#line 2952 "chpl.ypp"
                  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10197 "bison-chpl-lib.cpp"
    break;

  case 613: /* opt_try_expr: expr  */
#line 2953 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10203 "bison-chpl-lib.cpp"
    break;

  case 619: /* call_base_expr: expr TBANG  */
#line 2970 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10209 "bison-chpl-lib.cpp"
    break;

  case 620: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 2971 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10215 "bison-chpl-lib.cpp"
    break;

  case 623: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 2978 "chpl.ypp"
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
#line 10231 "bison-chpl-lib.cpp"
    break;

  case 624: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 2990 "chpl.ypp"
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
#line 10247 "bison-chpl-lib.cpp"
    break;

  case 625: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3001 "chpl.ypp"
                                              { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10253 "bison-chpl-lib.cpp"
    break;

  case 626: /* dot_expr: expr TDOT ident_use  */
#line 3006 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10259 "bison-chpl-lib.cpp"
    break;

  case 627: /* dot_expr: expr TDOT TTYPE  */
#line 3008 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10265 "bison-chpl-lib.cpp"
    break;

  case 628: /* dot_expr: expr TDOT TDOMAIN  */
#line 3010 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10271 "bison-chpl-lib.cpp"
    break;

  case 629: /* dot_expr: expr TDOT TLOCALE  */
#line 3012 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10277 "bison-chpl-lib.cpp"
    break;

  case 630: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3014 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10287 "bison-chpl-lib.cpp"
    break;

  case 631: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3020 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10297 "bison-chpl-lib.cpp"
    break;

  case 632: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3032 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10303 "bison-chpl-lib.cpp"
    break;

  case 633: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3033 "chpl.ypp"
                                    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10309 "bison-chpl-lib.cpp"
    break;

  case 634: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3034 "chpl.ypp"
                                    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10315 "bison-chpl-lib.cpp"
    break;

  case 635: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3035 "chpl.ypp"
                                    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10321 "bison-chpl-lib.cpp"
    break;

  case 636: /* bool_literal: TFALSE  */
#line 3039 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10327 "bison-chpl-lib.cpp"
    break;

  case 637: /* bool_literal: TTRUE  */
#line 3040 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10333 "bison-chpl-lib.cpp"
    break;

  case 638: /* str_bytes_literal: STRINGLITERAL  */
#line 3044 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10339 "bison-chpl-lib.cpp"
    break;

  case 639: /* str_bytes_literal: BYTESLITERAL  */
#line 3045 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10345 "bison-chpl-lib.cpp"
    break;

  case 642: /* literal_expr: INTLITERAL  */
#line 3051 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10351 "bison-chpl-lib.cpp"
    break;

  case 643: /* literal_expr: REALLITERAL  */
#line 3052 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10357 "bison-chpl-lib.cpp"
    break;

  case 644: /* literal_expr: IMAGLITERAL  */
#line 3053 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10363 "bison-chpl-lib.cpp"
    break;

  case 645: /* literal_expr: CSTRINGLITERAL  */
#line 3054 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10369 "bison-chpl-lib.cpp"
    break;

  case 646: /* literal_expr: TNONE  */
#line 3055 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10375 "bison-chpl-lib.cpp"
    break;

  case 647: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3056 "chpl.ypp"
                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10381 "bison-chpl-lib.cpp"
    break;

  case 648: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3057 "chpl.ypp"
                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10387 "bison-chpl-lib.cpp"
    break;

  case 649: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3058 "chpl.ypp"
                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10393 "bison-chpl-lib.cpp"
    break;

  case 650: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3059 "chpl.ypp"
                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10399 "bison-chpl-lib.cpp"
    break;

  case 651: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3061 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 10407 "bison-chpl-lib.cpp"
    break;

  case 652: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3065 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 10415 "bison-chpl-lib.cpp"
    break;

  case 653: /* assoc_expr_ls: expr TALIAS expr  */
#line 3072 "chpl.ypp"
                                        { (yyval.exprList) = TODOLIST((yyloc)); }
#line 10421 "bison-chpl-lib.cpp"
    break;

  case 654: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3073 "chpl.ypp"
                                        { (yyvsp[-4].exprList)->push_back(TODOEXPR((yyloc))); }
#line 10427 "bison-chpl-lib.cpp"
    break;

  case 655: /* binary_op_expr: expr TPLUS expr  */
#line 3077 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10433 "bison-chpl-lib.cpp"
    break;

  case 656: /* binary_op_expr: expr TMINUS expr  */
#line 3078 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10439 "bison-chpl-lib.cpp"
    break;

  case 657: /* binary_op_expr: expr TSTAR expr  */
#line 3079 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10445 "bison-chpl-lib.cpp"
    break;

  case 658: /* binary_op_expr: expr TDIVIDE expr  */
#line 3080 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10451 "bison-chpl-lib.cpp"
    break;

  case 659: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3081 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10457 "bison-chpl-lib.cpp"
    break;

  case 660: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3082 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10463 "bison-chpl-lib.cpp"
    break;

  case 661: /* binary_op_expr: expr TMOD expr  */
#line 3083 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10469 "bison-chpl-lib.cpp"
    break;

  case 662: /* binary_op_expr: expr TEQUAL expr  */
#line 3084 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10475 "bison-chpl-lib.cpp"
    break;

  case 663: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3085 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10481 "bison-chpl-lib.cpp"
    break;

  case 664: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3086 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10487 "bison-chpl-lib.cpp"
    break;

  case 665: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3087 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10493 "bison-chpl-lib.cpp"
    break;

  case 666: /* binary_op_expr: expr TLESS expr  */
#line 3088 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10499 "bison-chpl-lib.cpp"
    break;

  case 667: /* binary_op_expr: expr TGREATER expr  */
#line 3089 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10505 "bison-chpl-lib.cpp"
    break;

  case 668: /* binary_op_expr: expr TBAND expr  */
#line 3090 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10511 "bison-chpl-lib.cpp"
    break;

  case 669: /* binary_op_expr: expr TBOR expr  */
#line 3091 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10517 "bison-chpl-lib.cpp"
    break;

  case 670: /* binary_op_expr: expr TBXOR expr  */
#line 3092 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10523 "bison-chpl-lib.cpp"
    break;

  case 671: /* binary_op_expr: expr TAND expr  */
#line 3093 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10529 "bison-chpl-lib.cpp"
    break;

  case 672: /* binary_op_expr: expr TOR expr  */
#line 3094 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10535 "bison-chpl-lib.cpp"
    break;

  case 673: /* binary_op_expr: expr TEXP expr  */
#line 3095 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10541 "bison-chpl-lib.cpp"
    break;

  case 674: /* binary_op_expr: expr TBY expr  */
#line 3096 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10547 "bison-chpl-lib.cpp"
    break;

  case 675: /* binary_op_expr: expr TALIGN expr  */
#line 3097 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10553 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr THASH expr  */
#line 3098 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10559 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TDMAPPED expr  */
#line 3099 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10565 "bison-chpl-lib.cpp"
    break;

  case 678: /* unary_op_expr: TPLUS expr  */
#line 3103 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10571 "bison-chpl-lib.cpp"
    break;

  case 679: /* unary_op_expr: TMINUS expr  */
#line 3104 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10577 "bison-chpl-lib.cpp"
    break;

  case 680: /* unary_op_expr: TMINUSMINUS expr  */
#line 3105 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 10583 "bison-chpl-lib.cpp"
    break;

  case 681: /* unary_op_expr: TPLUSPLUS expr  */
#line 3106 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 10589 "bison-chpl-lib.cpp"
    break;

  case 682: /* unary_op_expr: TBANG expr  */
#line 3107 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10595 "bison-chpl-lib.cpp"
    break;

  case 683: /* unary_op_expr: expr TBANG  */
#line 3108 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10603 "bison-chpl-lib.cpp"
    break;

  case 684: /* unary_op_expr: TBNOT expr  */
#line 3111 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10609 "bison-chpl-lib.cpp"
    break;

  case 685: /* reduce_expr: expr TREDUCE expr  */
#line 3115 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10615 "bison-chpl-lib.cpp"
    break;

  case 686: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3116 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10621 "bison-chpl-lib.cpp"
    break;

  case 687: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3117 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10627 "bison-chpl-lib.cpp"
    break;

  case 688: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3118 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10633 "bison-chpl-lib.cpp"
    break;

  case 689: /* scan_expr: expr TSCAN expr  */
#line 3122 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10639 "bison-chpl-lib.cpp"
    break;

  case 690: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3123 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10645 "bison-chpl-lib.cpp"
    break;

  case 691: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3124 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10651 "bison-chpl-lib.cpp"
    break;

  case 692: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3125 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10657 "bison-chpl-lib.cpp"
    break;

  case 693: /* reduce_scan_op_expr: TPLUS  */
#line 3130 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10663 "bison-chpl-lib.cpp"
    break;

  case 694: /* reduce_scan_op_expr: TSTAR  */
#line 3131 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10669 "bison-chpl-lib.cpp"
    break;

  case 695: /* reduce_scan_op_expr: TAND  */
#line 3132 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10675 "bison-chpl-lib.cpp"
    break;

  case 696: /* reduce_scan_op_expr: TOR  */
#line 3133 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10681 "bison-chpl-lib.cpp"
    break;

  case 697: /* reduce_scan_op_expr: TBAND  */
#line 3134 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10687 "bison-chpl-lib.cpp"
    break;

  case 698: /* reduce_scan_op_expr: TBOR  */
#line 3135 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10693 "bison-chpl-lib.cpp"
    break;

  case 699: /* reduce_scan_op_expr: TBXOR  */
#line 3136 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10699 "bison-chpl-lib.cpp"
    break;


#line 10703 "bison-chpl-lib.cpp"

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
