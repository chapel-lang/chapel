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
  YYSYMBOL_292_5 = 292,                    /* $@5  */
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
#line 363 "chpl.ypp"

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
#define YYLAST   20518

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  167
/* YYNRULES -- Number of rules.  */
#define YYNRULES  702
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1258

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
       0,   638,   638,   643,   644,   649,   650,   655,   673,   693,
     694,   695,   696,   697,   698,   699,   700,   701,   702,   703,
     704,   705,   706,   719,   720,   721,   722,   723,   724,   738,
     747,   762,   771,   779,   802,   814,   825,   837,   849,   860,
     873,   880,   887,   888,   890,   895,   896,   900,   913,   926,
     939,   953,   954,   956,   961,   962,   967,   984,   988,   992,
    1003,  1022,  1023,  1027,  1031,  1036,  1040,  1050,  1055,  1061,
    1068,  1073,  1080,  1092,  1093,  1097,  1102,  1109,  1113,  1120,
    1128,  1137,  1144,  1152,  1164,  1171,  1175,  1181,  1190,  1191,
    1195,  1202,  1206,  1210,  1214,  1218,  1222,  1231,  1232,  1236,
    1237,  1238,  1239,  1240,  1241,  1244,  1245,  1246,  1247,  1248,
    1249,  1261,  1262,  1273,  1274,  1275,  1276,  1277,  1278,  1279,
    1280,  1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,  1289,
    1290,  1291,  1295,  1296,  1297,  1298,  1299,  1300,  1301,  1302,
    1303,  1304,  1305,  1306,  1313,  1314,  1315,  1316,  1320,  1321,
    1325,  1332,  1342,  1343,  1345,  1350,  1354,  1360,  1360,  1366,
    1366,  1375,  1376,  1377,  1378,  1379,  1380,  1381,  1385,  1392,
    1399,  1406,  1416,  1423,  1424,  1428,  1436,  1444,  1453,  1461,
    1470,  1491,  1501,  1508,  1526,  1539,  1552,  1556,  1560,  1564,
    1568,  1572,  1576,  1580,  1598,  1602,  1606,  1610,  1614,  1618,
    1622,  1626,  1630,  1634,  1638,  1642,  1646,  1650,  1654,  1658,
    1662,  1666,  1670,  1674,  1678,  1682,  1686,  1691,  1699,  1708,
    1712,  1716,  1720,  1724,  1728,  1732,  1736,  1740,  1745,  1750,
    1755,  1763,  1774,  1788,  1790,  1795,  1796,  1801,  1806,  1807,
    1808,  1809,  1810,  1811,  1812,  1813,  1814,  1815,  1816,  1817,
    1818,  1830,  1831,  1832,  1833,  1842,  1843,  1847,  1849,  1851,
    1856,  1858,  1860,  1865,  1867,  1869,  1871,  1873,  1875,  1880,
    1881,  1885,  1886,  1887,  1891,  1892,  1896,  1900,  1902,  1907,
    1908,  1912,  1914,  1916,  1923,  1928,  1939,  1956,  1957,  1958,
    1962,  1963,  1968,  1969,  1971,  1976,  1992,  2005,  2022,  2026,
    2031,  2031,  2038,  2039,  2041,  2046,  2051,  2060,  2066,  2070,
    2071,  2073,  2078,  2088,  2085,  2112,  2120,  2128,  2139,  2150,
    2159,  2170,  2171,  2175,  2176,  2177,  2178,  2179,  2180,  2181,
    2182,  2183,  2184,  2185,  2186,  2187,  2188,  2189,  2190,  2191,
    2192,  2193,  2194,  2195,  2196,  2197,  2198,  2199,  2200,  2201,
    2202,  2211,  2212,  2213,  2214,  2215,  2216,  2217,  2218,  2219,
    2220,  2221,  2222,  2226,  2227,  2231,  2235,  2236,  2240,  2241,
    2245,  2248,  2250,  2252,  2254,  2256,  2263,  2264,  2268,  2269,
    2270,  2271,  2272,  2273,  2274,  2275,  2276,  2280,  2281,  2282,
    2283,  2284,  2285,  2289,  2290,  2291,  2295,  2296,  2297,  2298,
    2299,  2300,  2304,  2305,  2308,  2309,  2313,  2314,  2318,  2322,
    2323,  2324,  2328,  2329,  2331,  2333,  2335,  2340,  2342,  2347,
    2349,  2351,  2353,  2355,  2357,  2359,  2364,  2365,  2369,  2376,
    2380,  2385,  2393,  2398,  2407,  2408,  2410,  2415,  2416,  2417,
    2418,  2419,  2424,  2423,  2432,  2440,  2444,  2451,  2463,  2476,
    2487,  2498,  2510,  2512,  2514,  2521,  2522,  2523,  2528,  2532,
    2536,  2540,  2544,  2548,  2552,  2559,  2560,  2561,  2562,  2563,
    2568,  2569,  2570,  2571,  2572,  2592,  2596,  2600,  2604,  2611,
    2612,  2613,  2617,  2619,  2625,  2627,  2629,  2634,  2635,  2636,
    2637,  2638,  2644,  2645,  2646,  2647,  2651,  2652,  2656,  2657,
    2658,  2662,  2663,  2667,  2668,  2672,  2677,  2686,  2687,  2688,
    2689,  2693,  2694,  2705,  2707,  2709,  2715,  2716,  2717,  2718,
    2719,  2720,  2722,  2724,  2726,  2728,  2730,  2732,  2735,  2737,
    2739,  2741,  2743,  2745,  2747,  2749,  2752,  2754,  2759,  2761,
    2763,  2765,  2767,  2769,  2771,  2773,  2775,  2777,  2779,  2781,
    2783,  2785,  2787,  2789,  2791,  2796,  2806,  2814,  2815,  2816,
    2817,  2818,  2819,  2820,  2821,  2826,  2827,  2831,  2840,  2841,
    2845,  2854,  2855,  2859,  2872,  2876,  2883,  2884,  2885,  2886,
    2887,  2888,  2892,  2893,  2898,  2900,  2902,  2904,  2906,  2912,
    2929,  2933,  2937,  2941,  2948,  2956,  2957,  2958,  2959,  2960,
    2961,  2962,  2963,  2964,  2965,  2967,  2971,  2976,  2981,  2986,
    2992,  2998,  3030,  3031,  3035,  3036,  3037,  3041,  3042,  3043,
    3044,  3053,  3054,  3055,  3056,  3057,  3061,  3073,  3085,  3089,
    3091,  3093,  3095,  3097,  3103,  3116,  3117,  3118,  3119,  3123,
    3124,  3128,  3129,  3133,  3134,  3135,  3136,  3137,  3138,  3139,
    3140,  3144,  3148,  3152,  3156,  3160,  3168,  3169,  3173,  3174,
    3175,  3176,  3177,  3178,  3179,  3180,  3181,  3182,  3183,  3184,
    3185,  3186,  3187,  3188,  3189,  3190,  3191,  3192,  3193,  3194,
    3195,  3199,  3200,  3201,  3202,  3203,  3204,  3207,  3211,  3212,
    3213,  3214,  3218,  3219,  3220,  3221,  3226,  3227,  3228,  3229,
    3230,  3231,  3232
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
  "var_decl_stmt", "$@5", "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
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
   -1098,   114,  3720, -1098,    11,   124, -1098, -1098, -1098, -1098,
   -1098, -1098,  5112,    78,   264,   221, 14537,   285, 20283,    78,
   11350,   290,   181,   243,   264,  5112, 11350,  1586,  5112,   206,
   20369, -1098,   140,   402,  8410,  9622,  9622, -1098,  8582,   438,
     194,   214, -1098,   477, 20369, 20369, 20369,   299,  1887,  9794,
     519, 11350,   350, -1098,   530,   534, 11350, -1098, 14537, -1098,
   11350,   605,   450,   424,   498,   562, 20403, -1098,  9968,  7892,
   11350,  9794, 14537, 11350,   543,   594,   484,  5112,   607, 11350,
     609, 11522, 11522, -1098,   611, -1098, 14537, -1098,   612,  8582,
   11350, -1098, 11350, -1098, 11350, -1098, -1098, 14057, 11350, -1098,
   11350, -1098, -1098, -1098,  4068,  7200,  8756, 11350, -1098,  4938,
   -1098, -1098, -1098,   500, -1098,    32, -1098, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,  3529,
   -1098, 10140, -1098, -1098, -1098, -1098, -1098,   614, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098,   527, 20369, -1098, 20129,   164,
   -1098,   282, -1098, -1098,   275,   436, -1098, 20369,  1887, -1098,
     508, -1098,   512, -1098, -1098,   515,   523,   521, 11350,   524,
     526, 19694,  1668,   375,   531,   542, -1098, -1098,   451, -1098,
   -1098, -1098, -1098, -1098,   394, -1098, -1098, 19694,   520,  5112,
   -1098, -1098,   545, 11350, -1098, -1098, 11350, 11350, 11350, 20369,
   -1098, 11350,  9968,  9968,   627,   472, -1098, -1098, -1098, -1098,
     345,   476, -1098, -1098,   525, 16247, -1098,   391, -1098,   559,
   -1098,   199, 19694,  2681, -1098, 20369,   590, -1098, -1098,   618,
     589,  8066, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098,   555, -1098, -1098, -1098,
   20369,   561,    29, 15916,    58, 15751,    58, 15833, 20369, 20369,
     -11, 15004, -1098, -1098,   618,  8066,   567,   416,  5112,   210,
     275,  2465,   101,   105, -1098,  5112, -1098, -1098, 16329,  1228,
   -1098,   569,   571, -1098, 16329,   345,  1228, -1098,  8066,  2304,
    2304, -1098, -1098,   253, 19694, 11350, 11350, -1098, 19694,   579,
   16905, -1098, 16329,   345, 19694,   577,  8066, -1098, 19694, 16988,
   -1098, -1098, 17070,  1742, -1098, -1098, 17151,   345,    29, 16329,
   17232,   176,   176,   538,  1228,  1228,   102, -1098, -1098,  4242,
     329, -1098, 11350, -1098,   -34,   133, -1098,   -32,     2, 17313,
     -46,   538,   725, -1098,  4416, 11350, -1098, 11350,   685, -1098,
   -1098, 15999,   342, 20369, 20369, 19694,   417, 20369,  9968,   582,
   -1098,   583, 20163,   628,   358, -1098, -1098, -1098, -1098, -1098,
   -1098,   665, -1098, -1098, -1098,    70,   666, -1098, -1098, -1098,
   13890,   632,   593,   419,   483, -1098, 11350,   613, 11350, 11350,
   11350,  9622,  9622, 11350,   488, 11350, 11350, 11350, 11350, 11350,
     427, 14057, 11350, 11350, 11350, 11350, 11350, 11350, 11350, 11350,
   11350, 11350, 11350, 11350, 11350, 11350, 11350, 11350,   690, -1098,
   -1098, -1098, -1098, -1098,  8928,  8928, -1098, -1098, -1098, -1098,
    8928, -1098, -1098,  8928,  8928,  8066,  8066,  9622,  9622,  7720,
   -1098, -1098, 16412, 16494, 17394,   588,    33, 20369,  4590, -1098,
    9622,    29,  1887, -1098, 11350, -1098, -1098, 11350,   637, -1098,
     592,   620, -1098, -1098, -1098,   719,  9968, -1098,  5286,  9622,
   -1098,   597, -1098,    29,  5460,  9622, -1098,    29, -1098,    29,
    9622, -1098,    29,   645,   647,  5112,   740,  5112, -1098,   741,
   11350,   712,   608,  8066, 20369, -1098, -1098,    81, -1098, -1098,
   -1098, -1098, -1098, -1098,   714,   636,   610, -1098,  3311, -1098,
      71, -1098,  2465, -1098, -1098,   137, -1098, 11694,   659, 11350,
    1887, -1098, -1098, 11350, 11350, -1098,   616, -1098, -1098,  9968,
   -1098, 19694, 19694, -1098,    50, -1098,  8066,   621, -1098,   763,
   -1098,   763, -1098, -1098, -1098, -1098, -1098, -1098, -1098,  9102,
   -1098, 17475,  7374, -1098,  7548, -1098,  5112,   622,  9622,  9276,
    3894,   623, 11350, 10312, -1098, -1098,   276, -1098,  4764, -1098,
     431, 17556,   435, 16082, 20369, 11866, 11350, -1098, 20369, 20369,
     527,   527, -1098,  1887,   -27,   640,  1392, -1098, 20369, -1098,
   11350,   385, -1098, -1098, -1098, 12038,   682, -1098,   619,   379,
   -1098,   651, -1098,   652,   654,   663,   660,   661, -1098,   662,
     667,   664,   668,   670,   393,   671,   674,   676, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
   -1098, 11350, -1098,   689,   694,   688,   619,   619, -1098, 12210,
     684, -1098, -1098, 17637, 16535, 16535,   -24, -1098,   -24, -1098,
     -24, 19653,   699,  1017,  1166,   345,   176, -1098,   491, -1098,
   -1098, -1098, -1098, -1098,   538, 19858,   -24, 19911, 19911, 16535,
   19911, 19911,   845,   176, 19858, 19394,   845,  1228,  1228,   176,
     538,   672,   673,   683,   687,   691,   692,   680,   693, -1098,
     -24, -1098,   -24,   233, -1098, -1098, -1098,   179, -1098,  1563,
   19735,   432, 12382,  9622, 12554,  9622, 11350,  8066,  9622, 14817,
     686,    78, 17718, -1098,   465, 19694, 17799,  8066, -1098,  8066,
   11350,   180,  8582, 19694,    35, 16575,  7720, -1098,  8582, 19694,
      80,  3308, -1098, -1098,    58, 16164, -1098, 11350, 11350,   819,
    5112,   824, 17880,  5112, 16657, 20369, -1098,   186, -1098,   197,
   -1098, -1098, -1098, 20369,   752, -1098,  2465,   717,    23,   202,
    2465,   101,    40,    52, 11350, 11350,  7026, -1098, -1098,   209,
    8238, -1098, 19694, -1098, 17961, 18042, -1098, -1098, 19694,   700,
      48,   697, -1098,  3185, -1098, -1098, -1098, -1098, -1098, -1098,
   -1098, -1098,  5112,   -14, 16740, -1098, -1098, 19694,  5112, 19694,
   -1098, 18123, -1098, -1098, 11350, -1098,   107,    63, 11350, -1098,
   10484, 11866, 11350, -1098,   698,   726,   708, 18204,   726,   709,
     527,   527,   705,   711,   467, -1098,  8066,   715,  2859, -1098,
   19694, 20249,  6852, -1098, -1098,   152, -1098,    23,   210, -1098,
   18285, -1098, 14922, -1098, -1098, -1098,   471, 20369,   716,   707,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
    7720, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098, -1098,
   -1098, -1098, -1098,    72,  9622,  9622, 11350,   853, 18366, 11350,
     856, 18447,   271,   720, 18528,  8066,    29,    29, -1098, -1098,
   -1098, -1098, -1098, 16329, -1098, 15087,  5634, -1098,  5808, -1098,
     293, -1098, 15170,  5982, -1098,    29,  6156, -1098,    29, -1098,
      29, -1098,    29, 19694, 19694,  5112, -1098,  5112, 11350, -1098,
    5112,   860,   732,   733, 20369,   525,   717, -1098,   304, 10658,
     430,   238, 11350,    64, -1098, -1098,   659,   724,    55, -1098,
   -1098,   728,   731, -1098,  6330,  9968, -1098, -1098, -1098, 20369,
   -1098,   759,   525, -1098,  6504,   735,  6678,   736, -1098, 11350,
   -1098, -1098, 11350, 18609,    69, 16823,   743,   748, 11350, -1098,
   11350, -1098,   746,   747, -1098, -1098, -1098,   309, -1098,  2155,
   -1098, -1098, -1098, 20053, -1098, -1098, -1098,   744, 13758,    61,
   -1098,   750, -1098,   766,   784,   619,   619, -1098, -1098, -1098,
   -1098,   659,   330,   332, 18691, 12726, 12898, 18772, 13070, 13242,
   -1098, 13414, 13586,   351, -1098, -1098, -1098,  5112,  8582, 19694,
    8582, 19694,  7720, -1098,  5112,  8582, 19694, -1098,  8582, 19694,
   -1098, -1098, -1098, -1098, -1098, 19694,   889,  5112, -1098, -1098,
   -1098, -1098,   430,   717,  9450, -1098, -1098, -1098,   447,  9968,
   -1098, -1098, -1098,   136, -1098,   138, -1098,   359, 18866, -1098,
   -1098, -1098, -1098, -1098,  9622, 14677,  8066,  8066,  5112, -1098,
      -2,   755, 11350, -1098,  8582, -1098, 19694,  5112,  8582, -1098,
   19694,  5112, 19694,   414, 10830, 11866, 11350, 11866, 11350, -1098,
   -1098, 18923, 19694, -1098, -1098,  3051, 19118,   765,  2887, -1098,
   -1098, -1098, -1098,   170, 14226,   257, -1098, -1098, -1098, -1098,
   11350, 11350, 11350, 11350, 11350, 11350, 11350, 11350, -1098, 17880,
   15253, 15336, -1098, 17880, 15419, 15502,  5112, -1098, -1098, -1098,
     430, 11004,    77, -1098, 19694, -1098, 11350,   238,   136,   136,
     136,   136,   136,   136,   238, 18963, -1098,   575,   758,   760,
     589, -1098,   525, 19694, 15585, -1098, 15668, -1098, -1098, -1098,
   19694,   767,   770,   771,   772, 11350, 19309, 19877, -1098, -1098,
   -1098, -1098, -1098,   193, -1098, -1098, -1098, 19114, 19154, 19305,
   19345, 19496, 19536,  5112,  5112,  5112,  5112, -1098, -1098, -1098,
   -1098, -1098, -1098,   448,  9622, 14366, 19694, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098,   793, 13586,   776,   779, -1098,  5112,
    5112, -1098, -1098, -1098, -1098, 19694, -1098, -1098, 11350, 11350,
   11350, 11350, 11350, 11350, 17880, 17880, 17880, 17880, 19577, -1098,
   -1098,   830, -1098, -1098, 17880, 17880, 11178, -1098
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   111,   645,   646,   647,   641,
     642,   648,     0,   565,    97,   132,   534,   139,   536,   565,
       0,   138,   442,   440,    97,     0,     0,   308,     0,   255,
     133,   174,   173,   639,     0,     0,     0,   172,     0,   137,
      51,   256,   310,   134,     0,     0,     0,     0,     0,     0,
     141,     0,   584,   556,   649,   142,     0,   311,   528,   437,
       0,     0,     0,   159,   157,   136,   537,   439,     0,     0,
       0,     0,   532,     0,     0,   140,     0,     0,   112,     0,
     640,     0,     0,   429,   135,   289,   530,   441,   143,     0,
       0,   698,     0,   700,     0,   701,   702,   611,     0,   699,
     696,   515,   155,   697,     0,     0,     0,     0,     4,     0,
       5,     9,    42,     0,    45,    54,    10,    60,    11,    12,
      13,    14,    15,   511,   512,    25,    46,   156,   166,     0,
     175,   612,   167,    16,    20,    17,    19,     0,   250,    18,
     603,    23,    24,    21,   165,   290,     0,   163,     0,   600,
     309,     0,   161,   312,   396,   387,   164,     0,     0,   162,
     617,   596,   513,   597,   518,   516,     0,     0,     0,   601,
     602,     0,   517,     0,   618,   619,   620,   643,   644,   595,
     520,   519,   598,   599,     0,    41,    27,   526,     0,     0,
     566,    98,     0,     0,   536,   133,     0,     0,     0,     0,
     537,     0,     0,     0,     0,   600,   617,   516,   601,   602,
     535,   517,   618,   619,     0,   565,   430,     0,   438,     0,
      22,     0,   496,   308,   287,     0,   173,   288,    44,    54,
       0,   503,   105,   113,   125,   119,   118,   127,   108,   117,
     128,   114,   129,   106,   130,   123,   116,   124,   122,   120,
     121,   107,   109,   115,   126,   131,     0,   110,   182,   431,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    53,    52,    54,   503,     0,     0,     0,   376,
     396,     0,     0,     0,   445,     0,   149,    35,     0,   683,
     588,   585,   586,   587,     0,   529,   684,     7,   503,   308,
     308,   408,   493,     0,   492,     0,     0,   150,   616,     0,
       0,    38,     0,   533,   521,     0,   503,    39,   527,     0,
     269,   265,     0,   517,   269,   266,     0,   531,     0,     0,
       0,   685,   687,   609,   682,   681,     0,    57,    61,     0,
       0,   498,     0,   500,     0,     0,   499,     0,     0,   492,
       0,   610,     0,     6,     0,     0,    55,     0,     0,   442,
     171,     0,   641,   537,     0,   613,   308,     0,     0,     0,
     286,     0,     0,   305,     0,   298,   302,   394,   395,   393,
     320,   397,   400,   399,   401,     0,   391,   388,   389,   392,
       0,   434,     0,     0,   514,    26,     0,   589,     0,     0,
       0,     0,     0,     0,   686,     0,     0,     0,     0,     0,
       0,   608,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,   358,
     359,   360,   355,   357,     0,     0,   353,   356,   354,   352,
       0,   362,   361,     0,     0,   503,   503,     0,     0,     0,
      28,    29,     0,     0,     0,     0,     0,     0,     0,    30,
       0,     0,     0,    31,     0,    32,    43,     0,   511,   509,
       0,   504,   505,   510,   297,     0,     0,   192,     0,     0,
     191,     0,   200,     0,     0,     0,   198,     0,   208,     0,
       0,   206,     0,     0,     0,     0,   224,     0,   351,   220,
       0,     0,     0,     0,     0,   234,    33,   383,   378,   379,
     380,   384,   385,   386,   376,   369,     0,   366,     0,   377,
       0,   449,     0,   450,   452,     0,   474,     0,   455,     0,
       0,   148,    34,     0,     0,    36,     0,   160,   158,     0,
      90,   614,   615,   151,     0,    37,     0,     0,   276,   267,
     263,   268,   264,   185,   184,    40,    59,    58,    62,     0,
     650,     0,     0,   635,     0,   637,     0,     0,     0,     0,
       0,     0,     0,     0,   654,     8,     0,    48,     0,    88,
       0,    85,     0,    67,     0,     0,    73,   168,     0,     0,
     290,   290,   180,     0,   261,   291,     0,   296,     0,   304,
       0,   300,   295,   398,   469,     0,   402,   390,   363,    99,
     345,   113,   343,   119,   118,   102,   117,   114,   348,   129,
     100,   130,   116,   120,   101,   103,   115,   131,   342,   324,
     327,   325,   326,   349,   337,   328,   341,   333,   331,   344,
     347,   332,   330,   335,   340,   329,   334,   338,   339,   336,
     346,     0,   323,     0,   104,     0,   363,   363,   321,     0,
     432,   428,   444,   564,   678,   677,   680,   689,   688,   693,
     692,   674,   671,   672,   673,   605,   661,   111,     0,   631,
     632,   112,   630,   629,   606,   665,   676,   670,   668,   679,
     669,   667,   659,   664,   666,   675,   658,   662,   663,   660,
     607,     0,     0,     0,     0,     0,     0,     0,     0,   691,
     690,   695,   694,   576,   577,   579,   581,     0,   568,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   652,
     261,   565,   565,   188,     0,   497,     0,     0,   523,     0,
       0,     0,     0,   540,     0,     0,     0,   201,     0,   546,
       0,     0,   199,   209,     0,     0,   207,     0,     0,   223,
       0,   219,     0,     0,     0,     0,   522,     0,   237,     0,
     235,   381,   382,     0,   376,   365,     0,   487,   412,     0,
     453,     0,   146,   147,   145,   144,     0,   473,   472,   596,
       0,   447,   594,   446,     0,     0,   628,   495,   494,     0,
       0,     0,   524,     0,   270,   651,   604,   636,   501,   638,
     502,   216,     0,     0,     0,   653,   214,   550,     0,   656,
     655,     0,    50,    49,     0,    84,     0,     0,     0,    77,
       0,     0,    73,    47,   697,    76,     0,    63,    74,     0,
     290,   290,     0,     0,     0,   258,     0,     0,   308,   303,
     306,     0,     0,   468,   467,   596,   403,   412,   376,   319,
       0,   350,     0,   315,   316,   436,   596,     0,     0,     0,
      96,    94,    95,    93,    92,    91,   626,   627,   578,   580,
       0,   567,   132,   139,   138,   137,   134,   141,   142,   136,
     140,   135,   143,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   443,   183,
     507,   508,   506,     0,   218,     0,     0,   190,     0,   189,
       0,   571,     0,     0,   196,     0,     0,   194,     0,   204,
       0,   202,     0,   232,   231,     0,   226,     0,     0,   222,
       0,   228,     0,   260,     0,     0,   487,   367,     0,     0,
     455,     0,     0,     0,   451,   454,   455,     0,     0,   456,
     457,     0,     0,   278,     0,     0,   277,   280,   525,     0,
     271,   274,     0,   217,     0,     0,     0,     0,   215,     0,
      89,    86,     0,    70,    69,    68,     0,     0,     0,   169,
       0,   170,     0,     0,   292,   292,   181,     0,   285,   308,
     159,   157,   284,   308,   293,   152,   301,     0,   459,     0,
     313,     0,   322,    99,   101,   363,   363,   433,   633,   634,
     569,   455,   617,   617,     0,     0,     0,     0,     0,     0,
     260,     0,     0,     0,   187,   186,   193,     0,     0,   539,
       0,   538,     0,   570,     0,     0,   545,   197,     0,   544,
     195,   205,   203,   225,   221,   555,   227,     0,    56,   257,
     236,   233,   455,   487,     0,   490,   489,   491,   596,   409,
     372,   370,   426,     0,   427,   414,   417,     0,   413,   406,
     407,   307,   448,   478,     0,   652,   503,   503,     0,   282,
       0,     0,     0,   272,     0,   212,   552,     0,     0,   210,
     551,     0,   657,     0,     0,     0,    73,     0,    73,    78,
      81,    65,    64,   292,   292,   308,   308,   262,   308,   154,
     294,   464,   462,   596,   652,     0,   364,   317,   318,   573,
       0,     0,     0,     0,     0,     0,     0,     0,   262,   543,
       0,     0,   572,   549,     0,     0,     0,   230,   373,   371,
     455,   479,     0,   411,   410,   425,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   476,   596,     0,     0,
     283,   281,     0,   275,     0,   213,     0,   211,    87,    72,
      71,     0,     0,     0,     0,     0,   308,   308,   176,   178,
     259,   153,   463,   596,   404,   314,   405,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   229,   375,   374,
     481,   482,   484,   596,     0,   652,   416,   418,   419,   422,
     423,   424,   420,   421,   415,     0,   590,   591,   273,     0,
       0,    80,    83,    79,    82,    66,   177,   179,     0,     0,
       0,     0,     0,     0,   542,   541,   548,   547,     0,   483,
     485,   596,   592,   593,   554,   553,   479,   486
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1098, -1098, -1098,    -1,  -267,  2258, -1098,     3, -1098, -1098,
       7,   675, -1098,  -915,  2354,   591,  -557, -1098,  -805,  -799,
   -1098, -1098,   120, -1098, -1098,   552,   922, -1098,  2788,  -186,
    -383, -1098,  -599,  1098,  -913,   -55,  -820, -1098, -1098,   338,
   -1098, -1098, -1098, -1098, -1098, -1098, -1098,   167, -1098,   861,
   -1098, -1098,     8,   851, -1098, -1098, -1098, -1098,   633, -1098,
     -18, -1098, -1098, -1098, -1098, -1098, -1098, -1098,  -527,  -861,
   -1098, -1098, -1098, -1098,   108,  -351,  1160, -1098, -1098, -1098,
     595, -1098, -1098, -1098, -1098,    94,  -169,  -556, -1098, -1098,
     100,   191,   452, -1098, -1098, -1098,   701, -1098, -1098,  -157,
      89,  -987,   115,  -193,  -183,  -554, -1098, -1098,   106, -1098,
    -184,   848, -1098,  -143,   449,   203,  -505,  -914,  -932, -1098,
    -501,  -646, -1097, -1047,  -894,   -57, -1098,    36, -1098,  -273,
    -485,  -511,   347,  -507, -1098, -1098, -1098,  1250, -1098,   -10,
   -1098, -1098,  -212, -1098,  -730, -1098, -1098, -1098,  1493,  1754,
     -12, -1098,   -64,   485, -1098,  1832,  1942, -1098, -1098, -1098,
   -1098, -1098, -1098, -1098, -1098, -1098,  -439
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   338,   109,   761,   111,   112,   113,   114,
     115,   358,   116,   117,   118,   339,   835,   582,   839,   836,
     119,   120,   579,   580,   121,   122,   192,   652,   282,   123,
     257,   124,   787,   287,   125,  1004,   126,   300,   299,   127,
     128,   129,   130,   131,   132,   133,   134,   903,   135,   270,
     136,   769,   770,   204,   138,   139,   140,   141,   549,   804,
     972,   142,   143,   800,   967,   144,   145,   146,   369,   848,
     147,   148,   374,   851,   375,   376,   205,   150,   151,   152,
     153,  1125,   154,   155,   655,   656,   500,   859,   280,   515,
     516,   517,   518,   519,   390,   380,   385,   857,  1195,  1081,
     469,  1070,   953,  1075,  1076,  1077,   156,   157,   392,   660,
     158,   159,   217,   283,   284,   524,   525,   791,   854,   606,
     528,   788,  1211,  1067,   950,   340,   221,   344,   345,   470,
     471,   472,   206,   161,   162,   163,   164,   207,   166,   189,
     190,   717,   483,   920,   718,   719,   167,   168,   208,   209,
     171,   366,   473,   211,   173,   212,   213,   176,   177,   178,
     179,   350,   180,   181,   182,   183,   184
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     187,   108,   502,   444,   210,   309,   853,   654,   215,   214,
     721,   303,   514,   865,   222,   393,   921,   779,   767,   778,
     789,   599,   263,   265,   267,   536,   271,   987,  1005,   838,
     228,   285,   986,   462,   229,   801,  1071,   288,  1079,   289,
    1080,   346,  1082,   547,   294,   468,   295,   274,   296,   348,
     568,   799,  1062,   487,   489,   492,   304,   308,   310,   312,
     313,   314,   285,   842,   843,   318,   677,   319,   916,   322,
     326,   604,   526,   526,   327,  1148,  1122,   329,   330,   355,
     331,   728,   332,   678,   951,   333,   334,   481,   335,   468,
     495,   285,   304,   308,   349,   351,  -255,   679,   855,   573,
     863,   864,   526,  1084,  1212,   481,  1105,  1129,  -256,   728,
     677,   562,   468,   923,     3,   404,   356,   361,  1249,   365,
     408,   481,   964,   410,  -279,  1214,   680,   413,   574,   771,
     468,  -465,  -470,  1115,  1116,   567,   571,   845,   563,  1072,
     952,  1106,   566,   539,   846,   357,   456,   569,  1149,  -470,
    1020,   258,   866,   529,   444,    69,   397,   302,   458,  1257,
     974,  -465,  -470,  1208,   965,   681,  -279,   458,  1250,  1150,
     772,   682,   707,   708,  -465,   185,   570,   481,   569,  1119,
    -238,   187,   593,  1005,   452,   453,   454,  -465,  -470,   318,
     304,   349,  1192,   302,   343,   302,  -470,   188,   458,   481,
     569,   262,   264,   266,   458,   461,   569,   729,   400,   681,
    1079,   231,  1080,  -466,   605,   527,   527,  -470,   966,   308,
    -279,   657,   569,   275,   683,  -470,   466,   458,   912,  1085,
     229,  -458,   982,   458,  -465,  1124,  1209,   507,  1074,  -465,
    -470,  1072,   902,  -466,  -470,   527,  -470,   849,   259,   458,
     530,  1215,  1186,  1187,  -460,  1156,  -466,  -471,   508,   468,
     468,  -458,   509,   308,   401,  -470,   185,   191,   402,  -466,
    -471,   948,   556,   347,  -458,   838,   272,  -239,   564,   273,
     956,   878,   780,  1157,  -460,   510,   308,  -458,   511,   216,
      61,   302,   302,   541,   542,  1005,  1005,  -460,  1191,   512,
    -471,  1182,   381,  1184,   308,   565,  1181,   721,  1183,   781,
    -460,   595,  1142,   992,   993,   404,  -466,   468,   513,   734,
     408,  -466,   879,   410,   880,   539,  -471,   413,  -561,  1073,
     561,   739,   218,  -471,  -458,  -624,   377,  -624,   558,  -458,
    1074,  -245,   944,   581,   464,   583,  -244,   780,    69,   160,
    1065,   881,   914,   382,  -471,   378,   304,  -460,   943,   160,
     468,   997,  -460,   465,   383,   379,  1005,  1005,   290,   945,
     702,   703,   160,  -471,   954,   160,   704,   231,  -471,   705,
     706,  -471,  -368,   384,   663,   275,   664,   665,   666,   668,
     670,   671,  1021,   672,   673,   674,   675,   676,   539,   684,
     685,   686,   687,   688,   689,   690,   691,   692,   693,   694,
     695,   696,   697,   698,   699,   700,   739,   540,    23,   741,
    1033,  1194,   308,   308,   160,  1103,   458,   291,   308,   588,
     677,   308,   308,   308,   308,   710,   712,   720,  1042,  1166,
     185,   721,  1068,  1030,    23,   292,   822,   678,   732,   780,
     844,   160,   735,   589,   739,   736,   160,   302,  -253,  1127,
    1128,   679,   293,   386,   304,  1043,   743,   745,    42,    59,
     279,   -53,   749,   751,   559,  -575,  1063,  -574,   755,   654,
      67,  1117,   447,  1158,   404,   762,   448,   172,   764,   -53,
     680,   308,   410,    57,  -243,    59,   739,   172,   346,   560,
     346,  1123,  -575,   601,  -574,    87,    67,   514,   -53,  1159,
     172,  1160,  1161,   172,   387,  1162,  1163,   792,  -105,  1155,
     895,   794,   795,  1138,   448,   388,  -111,   798,   602,   681,
    -299,    87,  -107,  -240,   308,   682,   160,   -53,   925,   928,
    -112,   468,   930,   932,   389,   -52,   445,   798,   446,   838,
     308,   838,   308,   468,   790,  -299,   814,   798,   817,   988,
     819,   821,   172,   -52,   530,   302,   323,   323,   667,   669,
     400,  -488,  -480,   837,   837,  -247,   824,   558,  1167,  1069,
     828,  1003,   -52,   662,  1178,   458,  -251,   504,   850,   172,
    -248,   514,  -488,  -480,   172,   825,  -488,  -480,   808,   829,
     810,   975,   977,   721,  1218,  1219,  1220,  1221,  1222,  1223,
     530,   -52,   530,   297,   709,   711,  -435,  1193,  -242,  -488,
    -480,   298,  -625,  -475,  -625,   160,   401,   731,   797,   908,
     402,   996,   160,   321,   325,  -435,  -475,   537,   538,   860,
     981,   683,   315,  -624,  1213,  -624,   744,  -621,   797,  -621,
    -246,   343,   750,   343,  -623,   316,  -623,   754,   797,  -622,
     468,  -622,   868,  -252,   869,  -254,  -475,  -241,  -249,   354,
     367,   368,  -558,   911,   172,   394,   396,   404,   405,  -557,
     406,   407,   408,   457,   409,   410,   160,   395,  -562,   413,
    -563,   449,  -475,  1016,   458,  -560,   419,   420,   259,  -475,
     423,   160,   356,   424,   425,   426,  -559,   467,  1213,   451,
     743,   898,   749,   901,   762,   308,   904,   817,  1251,   468,
    -475,   906,   907,   463,   474,   308,   960,   308,   913,   958,
     915,   400,   476,   575,   720,   813,   922,   658,   503,  -475,
     533,   507,   534,   543,  -475,   933,   934,  -475,   546,  1213,
     584,   596,   600,   597,   603,   607,   659,   661,   701,   727,
     410,   737,   508,   172,   738,   739,   509,   740,   746,   757,
     172,   758,   314,   318,   349,   760,   763,   765,   308,   507,
     766,   774,   775,   790,   803,   539,   856,   401,   796,   510,
     858,   402,   511,   802,   352,  1009,   812,   818,  -132,  -139,
     508,  -138,  -108,   512,   509,   160,  -106,  -137,  -134,  -141,
    -109,  -142,   581,  1168,  1169,  -136,   983,  -140,   985,   837,
     837,  -135,   513,  -143,   172,   160,   910,   510,   861,   867,
     511,   160,    61,  -110,   308,   862,   870,   871,   404,   172,
     349,   512,   160,   408,   160,   409,   410,   872,  1003,  1003,
     413,   873,   876,   137,   935,   874,   875,   905,   420,   937,
     513,   949,   -75,   137,   424,   425,   426,   877,   720,   968,
     963,   988,   989,   991,   994,   302,   137,   400,  -477,   137,
     995,  1019,   668,   710,  1024,   998,  1025,  1027,  1018,  1028,
     897,  -477,   900,   308,  1031,  1057,  1058,  1059,  1083,  1086,
     468,   468,  1087,  1092,  1039,  -105,  1041,  1109,  1090,  1097,
    1101,  1046,  1110,   160,  1049,  1113,  1114,   160,  1121,  1003,
    1003,  -477,  1126,  -107,  1146,   160,  1055,  1172,   137,  1190,
    1226,  1231,  1227,   401,  1232,  1233,  1234,   402,  1157,  1252,
    1078,   302,  1253,   172,   980,   578,   219,  -477,  1120,   501,
     328,  1091,  1060,   304,  -477,   137,  1015,   551,  1011,  1006,
     137,   592,  1096,   172,  1100,   947,   773,  1102,  1196,   172,
     837,  1224,  1010,  1017,  1217,  -477,  1111,   360,  1112,   793,
     172,   520,   172,   955,   404,   405,     0,   406,   407,   408,
       0,   409,   410,     0,  -477,     0,   413,     0,     0,  -477,
       0,     0,  -477,     0,   420,     0,     0,  1152,     0,     0,
     424,   425,   426,  1039,  1041,     0,  1046,  1049,     0,  1096,
    1100,     0,     0,     0,     0,  1139,  1140,     0,  1141,     0,
     720,     0,  1143,  1144,     0,     0,  1145,     0,  1066,     0,
     137,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,   172,   349,     0,   302,   172,     0,  1154,     0,     0,
       0,   667,   709,   172,     0,     0,   893,     0,     0,     0,
       0,     0,  1165,   817,   308,   308,     0,     0,     0,     0,
    1173,     0,  1174,     0,     0,     0,  1176,     0,     0,     0,
       0,     0,  1180,   837,   837,   837,   837,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,   160,     0,   402,
     160,     0,   817,     0,     0,     0,     0,     0,  1139,  1197,
    1198,  1143,  1199,  1200,  1201,  1202,     0,     0,     0,   137,
       0,     0,     0,     0,     0,     0,   137,     0,     0,     0,
       0,     0,     0,     0,  1216,     0,     0,     0,     0,     0,
       0,     0,     0,   302,     0,     0,   404,   405,  1153,   160,
     407,   408,   149,   409,   410,   160,     0,     0,   413,   311,
       0,     0,   149,  1235,     0,     0,   420,     0,     0,     0,
       0,     0,   424,   425,   426,   149,     0,     0,   149,     0,
     137,  1244,  1245,  1246,  1247,     0,     0,     0,   400,     0,
       0,     0,  1248,   817,     0,   137,     0,     0,     0,     0,
       0,     0,     0,  1100,     0,     0,     0,  1254,  1255,     0,
       0,     0,     0,     0,     0,     0,  1244,  1245,  1246,  1247,
    1254,  1255,     0,     0,     0,     0,     0,   149,     0,     0,
    1210,  1022,  1023,     0,  1100,   172,     0,     0,   172,     0,
       0,     0,   165,     0,   401,     0,     0,     0,   402,     0,
     400,     0,   165,   160,   149,   160,     0,     0,     0,   149,
     160,     0,     0,   160,     0,   165,     0,     0,   165,     0,
       0,     0,   160,     0,   160,     0,     0,   160,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   172,     0,     0,
       0,     0,     0,   172,  1210,   404,   405,     0,     0,   137,
     408,   160,   409,   410,     0,     0,   401,   413,     0,     0,
     402,   160,     0,   160,     0,   420,     0,   165,     0,   137,
       0,   424,   425,   426,     0,   137,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1210,   137,     0,   137,   149,
       0,     0,     0,     0,   165,     0,     0,     0,     0,   165,
     477,   480,   482,   486,   488,   491,     0,   404,     0,     0,
       0,     0,   408,     0,   409,   410,     0,     0,     0,   413,
       0,     0,     0,     0,   160,     0,   532,   420,     0,     0,
       0,   160,   535,   847,     0,   426,     0,     0,     0,     0,
       0,   172,     0,   172,   160,     0,     0,     0,   172,     0,
     545,   172,     0,     0,  -292,     0,     0,   137,  -292,  -292,
     172,   137,   172,  -292,     0,   172,   553,   554,  -292,   137,
    -292,  -292,     0,     0,     0,   160,  -292,     0,   149,   165,
       0,     0,     0,  -292,   160,   149,  -292,     0,   160,   172,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   172,
       0,   172,     0,     0,     0,  -292,     0,     0,  -292,     0,
    -292,     0,  -292,     0,  -292,  -292,     0,  -292,     0,  -292,
       0,  -292,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   160,     0,   169,     0,     0,     0,   149,
    -292,     0,     0,  -292,     0,   169,  -292,     0,     0,     0,
       0,     0,     0,     0,   149,     0,     0,     0,   169,     0,
       0,   169,   172,     0,     0,     0,     0,     0,   165,   172,
       0,     0,     0,     0,     0,   165,     0,     0,     0,     0,
       0,     0,   172,     0,     0,     0,     0,     0,     0,     0,
     160,   160,   160,   160,     0,     0,  -292,     0,     0,   733,
       0,     0,  -292,     0,     0,     0,   677,     0,     0,     0,
     169,     0,     0,   172,     0,     0,   160,   160,     0,     0,
     882,   747,   172,   883,     0,   752,   172,   753,   884,   165,
     756,     0,     0,     0,   223,     0,     0,   169,     0,   195,
       0,     0,   169,     0,   165,     0,     0,     0,   224,   885,
       0,   137,    22,    23,   137,     0,   886,     0,   149,     0,
       0,     0,   225,     0,    31,   226,   887,     0,     0,     0,
      37,   172,     0,   888,     0,     0,     0,    42,   149,     0,
       0,     0,     0,     0,   149,     0,     0,     0,     0,   889,
       0,   -51,     0,     0,     0,   149,     0,   149,     0,     0,
       0,   890,    57,   137,    59,   681,     0,     0,    63,   137,
     -51,    64,   891,   227,     0,    67,     0,     0,   892,     0,
       0,     0,   169,     0,     0,     0,     0,     0,   172,   172,
     172,   172,     0,     0,    83,     0,     0,    85,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,   165,     0,
       0,     0,     0,     0,   172,   172,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   149,     0,   165,     0,
     149,     0,     0,     0,   165,     0,     0,     0,   149,     0,
       0,     0,     0,     0,     0,   165,     0,   165,     0,     0,
     102,     0,     0,     0,     0,     0,   170,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   170,   137,     0,   137,
       0,   169,     0,     0,   137,     0,     0,   137,   169,   170,
       0,     0,   170,     0,     0,     0,   137,     0,   137,     0,
       0,   137,   428,   429,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,     0,     0,     0,
       0,     0,     0,     0,     0,   137,   165,     0,     0,     0,
     165,     0,     0,  -582,     0,   137,     0,   137,   165,     0,
       0,   170,   169,     0,   174,     0,   443,     0,     0,  -621,
       0,  -621,   917,   919,   174,     0,     0,   169,   924,   927,
       0,     0,   929,   931,     0,     0,     0,   174,   170,     0,
     174,     0,     0,   170,     0,     0,   428,   429,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,     0,     0,     0,     0,     0,     0,     0,   137,     0,
     232,     0,     0,     0,     0,   137,     0,     0,     0,     0,
       0,     0,     0,     0,   233,   234,     0,   235,   137,   174,
     443,     0,   236,  -621,     0,  -621,     0,     0,     0,     0,
     149,   237,     0,   149,     0,     0,     0,   238,     0,     0,
       0,     0,     0,   239,     0,     0,   174,   240,     0,   137,
     241,   174,     0,   170,   175,     0,     0,     0,   137,     0,
     242,   169,   137,     0,   175,     0,   243,   244,     0,     0,
       0,     0,     0,     0,   245,     0,     0,   175,     0,     0,
     175,   169,   149,   246,     0,     0,     0,   169,   149,     0,
       0,     0,   247,   248,     0,   249,     0,   250,   169,   251,
     169,     0,   252,     0,     0,     0,   253,   137,     0,   254,
       0,     0,   255,     0,  1034,  1035,     0,     0,     0,     0,
     165,  1036,     0,   165,     0,     0,     0,     0,     0,   175,
       0,   174,     0,  1047,     0,     0,  1050,     0,  1051,     0,
    1052,     0,   170,     0,     0,     0,     0,     0,     0,   170,
       0,     0,     0,     0,     0,     0,   175,     0,     0,     0,
       0,   175,     0,     0,   137,   137,   137,   137,   281,   169,
       0,     0,   165,   169,     0,     0,     0,     0,   165,     0,
       0,   169,     0,     0,     0,     0,   149,     0,   149,     0,
     137,   137,     0,   149,     0,     0,   149,     0,     0,     0,
       0,     0,     0,   170,     0,   149,     0,   149,     0,     0,
     149,     0,     0,     0,     0,     0,     0,     0,   170,     0,
     174,     0,     0,     0,     0,     0,     0,   174,     0,     0,
       0,     0,     0,     0,   149,     0,     0,     0,     0,     0,
       0,   175,     0,     0,   149,     0,   149,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1118,     0,     0,   165,     0,   165,     0,
       0,   174,     0,   165,     0,     0,   165,   224,     0,     0,
       0,    22,    23,     0,     0,   165,   174,   165,  1171,     0,
     165,   225,     0,    31,   226,     0,     0,   149,     0,    37,
       0,     0,     0,     0,   149,     0,    42,     0,     0,     0,
       0,     0,   170,     0,   165,     0,     0,   149,     0,     0,
     175,     0,     0,     0,   165,     0,   165,   175,     0,     0,
       0,    57,   170,    59,     0,     0,     0,  1000,   170,     0,
    1001,     0,   227,     0,    67,     0,     0,     0,   149,   170,
       0,   170,     0,   169,     0,     0,   169,   149,     0,     0,
     110,   149,     0,    83,     0,     0,    85,     0,     0,    87,
     186,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   175,     0,   220,     0,     0,   230,   165,     0,     0,
     174,     0,     0,     0,   165,     0,   175,     0,     0,     0,
       0,     0,     0,     0,     0,   169,   149,   165,     0,     0,
     174,   169,     0,     0,     0,     0,   174,     0,     0,   102,
     170,     0,     0,     0,   170,     0,   224,   174,     0,   174,
      22,    23,   170,     0,     0,   317,     0,     0,   165,     0,
     225,     0,    31,   226,     0,     0,     0,   165,    37,     0,
       0,   165,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,   110,   149,   149,   149,   149,   353,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,     0,    59,     0,     0,     0,     0,     0,     0,   149,
     149,   227,     0,    67,     0,     0,   165,     0,   174,     0,
     175,     0,   174,   286,     0,     0,     0,     0,     0,   169,
     174,   169,    83,     0,     0,    85,   169,     0,    87,   169,
     175,     0,     0,     0,     0,   286,   175,     0,   169,     0,
     169,     0,     0,   169,     0,   320,   324,   175,     0,   175,
       0,     0,     0,     0,     0,     0,     0,   450,     0,     0,
       0,     0,     0,   165,   165,   165,   165,   169,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,   232,   169,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   165,
     165,     0,   233,   234,     0,   235,     0,     0,     0,     0,
     236,     0,     0,     0,     0,     0,     0,     0,     0,   237,
       0,     0,     0,     0,     0,   238,     0,     0,   175,     0,
       0,   239,   175,     0,   170,   240,     0,   170,   241,     0,
     175,     0,     0,     0,     0,     0,     0,     0,   242,     0,
     169,     0,     0,     0,   243,   244,   506,   169,     0,     0,
       0,     0,   245,   531,     0,     0,     0,     0,     0,     0,
     169,   246,     0,     0,     0,     0,     0,     0,     0,     0,
     247,   248,     0,   249,     0,   250,   170,   251,   459,     0,
     252,     0,   170,     0,   253,   521,     0,   254,     0,     0,
     255,   169,     0,     0,     0,     0,     0,     0,     0,     0,
     169,     0,   174,     0,   169,   174,     0,   110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   110,     0,     0,     0,   286,   286,   286,   286,
     286,   286,     0,     0,   496,   499,     0,     0,     0,     0,
       0,   505,     0,     0,     0,     0,   522,     0,     0,   169,
       0,     0,   286,     0,   174,     0,     0,     0,   286,     0,
     174,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   286,     0,     0,     0,
     170,     0,   170,     0,     0,     0,     0,   170,     0,     0,
     170,     0,   286,   286,     0,     0,     0,     0,     0,   170,
       0,   170,     0,     0,   170,     0,   169,   169,   169,   169,
       0,     0,   175,   224,     0,   175,     0,    22,    23,     0,
       0,     0,     0,     0,     0,     0,   110,   225,   170,    31,
     226,     0,   169,   169,     0,    37,     0,     0,   170,     0,
     170,     0,    42,     0,     0,     0,   531,     0,     0,     0,
       0,     0,   531,     0,     0,     0,   -51,     0,   174,     0,
     174,     0,     0,   759,   175,   174,     0,    57,   174,    59,
     175,     0,     0,    63,     0,   -51,    64,   174,   227,   174,
      67,     0,   174,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
       0,   170,    85,     0,     0,    87,   174,     0,   170,     0,
       0,     0,     0,     0,     0,     0,   174,     0,   174,     0,
       0,   170,     0,     0,     0,   286,     0,     0,   256,     0,
       0,     0,     0,     0,   811,     0,     0,     0,   816,     0,
       0,     0,   276,   277,   278,     0,   110,   286,     0,     0,
       0,   286,   170,   286,     0,   102,   286,     0,     0,     0,
       0,   170,     0,     0,     0,   170,     0,     0,   175,     0,
     175,     0,     0,     0,     0,   175,     0,     0,   175,   174,
       0,     0,     0,     0,     0,     0,   174,   175,     0,   175,
       0,   224,   175,     0,     0,    22,    23,     0,     0,   174,
     999,     0,     0,     0,     0,   225,     0,    31,   226,     0,
     170,     0,     0,    37,     0,     0,   175,     0,     0,   224,
      42,     0,     0,    22,    23,     0,   175,     0,   175,     0,
     174,     0,     0,   225,     0,    31,   226,     0,     0,   174,
       0,    37,     0,   174,   370,    57,   373,    59,    42,    61,
       0,  1000,     0,     0,  1001,   391,   227,     0,    67,     0,
       0,     0,     0,     0,     0,     0,     0,   170,   170,   170,
     170,     0,     0,    57,     0,    59,     0,    83,     0,  1000,
      85,     0,  1001,    87,   227,     0,    67,     0,   174,   175,
       0,     0,     0,   170,   170,     0,   175,   455,     0,     0,
       0,     0,     0,     0,     0,    83,     0,     0,    85,   175,
       0,    87,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   256,     0,     0,     0,     0,   936,     0,
       0,   939,     0,   102,     0,     0,     0,     0,     0,  1002,
     175,     0,     0,     0,     0,   174,   174,   174,   174,   175,
       0,     0,     0,   175,     0,     0,     0,     0,   475,     0,
       0,   102,     0,     0,     0,     0,   493,   494,     0,     0,
       0,   174,   174,     0,     0,     0,     0,     0,     0,   523,
     973,     0,     0,   224,     0,     0,   978,    22,    23,     0,
       0,     0,   999,     0,     0,     0,     0,   225,   175,    31,
     226,     0,     0,     0,     0,    37,     0,     0,   286,   286,
       0,     0,    42,     0,   286,   286,     0,     0,   286,   286,
       0,     0,     0,     0,     0,     0,     0,     0,   941,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,    61,     0,  1000,     0,     0,  1001,     0,   227,     0,
      67,     0,     0,     0,     0,   175,   175,   175,   175,     0,
       0,   590,   591,     0,     0,   594,     0,   970,     0,    83,
     373,     0,    85,     0,     0,    87,     0,     0,     0,     0,
       0,   175,   175,     0,   531,     0,   531,     0,   653,     0,
       0,   531,     0,     0,   531,     0,     0,     0,   232,     0,
       0,     0,     0,  1053,     0,  1054,     0,     0,  1056,     0,
       0,     0,   233,   234,     0,   235,     0,     0,     0,     0,
     236,     0,     0,     0,     0,   102,     0,     0,     0,   237,
       0,  1188,  1089,     0,     0,   238,     0,     0,     0,     0,
       0,   239,  1095,     0,  1099,   240,     0,     0,   241,     0,
       0,     0,     0,     0,     0,   730,     0,     0,   242,     0,
       0,     0,     0,     0,   243,   244,     0,     0,     0,     0,
     286,   286,   245,     0,     0,     0,     0,   286,     0,   499,
       0,   246,     0,     0,     0,     0,   499,     0,     0,   286,
     247,   248,   286,   249,   286,   250,   286,   251,     0,     0,
     252,     0,   768,     0,   253,     0,     0,   254,     0,  1061,
     255,     0,     0,     0,     0,     0,   777,     0,     0,     0,
     523,     0,     0,     0,   232,  1147,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,  1093,   399,   233,   234,
       0,   235,     0,     0,     0,     0,   236,     0,     0,     0,
     400,   926,     0,     0,     0,   237,  1170,     0,     0,     0,
       0,   238,     0,     0,   458,  1175,   969,   239,     0,  1177,
       0,   240,     0,     0,   241,     0,     0,     0,     0,     0,
       0,     0,   833,     0,   242,     0,   840,   841,     0,     0,
     243,   244,     0,     0,     0,     0,   373,     0,   245,     0,
       0,     0,     0,     0,     0,     0,   401,   246,     0,     0,
     402,     0,     0,     0,  1207,     0,   247,   248,     0,   249,
       0,   250,     0,   251,     0,     0,   252,     0,     0,     0,
     253,     0,     0,   254,     0,     0,   255,   481,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   286,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,   458,     0,     0,
       0,     0,   776,     0,   427,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   499,   499,     0,     0,   499,   499,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1228,     0,   499,     0,
     499,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   193,     0,     0,    15,    16,     0,    17,
       0,   194,     0,   942,    21,   359,    23,     0,     0,     0,
       0,   946,     0,    29,   523,   195,     0,     0,   523,    33,
     196,   197,     0,     0,   198,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   199,     0,     0,    47,    48,
       0,   971,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,    59,    60,     0,
      62,     0,     0,     0,     0,    65,   200,     0,    67,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   201,
       0,    78,     0,     0,    80,     0,     0,     0,    84,   373,
       0,    86,     0,    87,    88,     0,     0,     0,     0,     0,
     653,     0,    91,     0,     0,   391,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   202,     0,
     105,     0,   203,     0,     0,   107,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      -2,     4,     0,     5,     0,     6,     7,     8,     9,    10,
      11,     0,   768,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,   971,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -51,     0,    40,
      41,    42,     0,    43,  -308,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -308,     0,     0,    57,    58,    59,    60,
      61,    62,    63,  -308,   -51,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -51,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,  -652,     0,    12,    13,
      14,    15,    16,  -652,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,  -652,    28,    29,  -652,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -51,     0,    40,    41,    42,     0,    43,  -308,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -308,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -308,   -51,    64,
      65,    66,  -652,    67,    68,    69,  -652,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -51,    87,    88,
       0,     0,    89,     0,    90,     0,     0,  -652,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -652,  -652,    94,  -652,  -652,  -652,  -652,
    -652,  -652,  -652,     0,  -652,  -652,  -652,  -652,  -652,     0,
    -652,  -652,  -652,  -652,  -652,  -652,  -652,  -652,   102,  -652,
    -652,  -652,     0,   104,  -652,   105,     0,   106,     0,   336,
    -652,     5,   301,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -51,     0,    40,    41,    42,
       0,    43,  -308,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -308,     0,     0,    57,    58,    59,    60,    61,    62,
      63,  -308,   -51,    64,    65,    66,     0,    67,    68,    69,
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
       0,    40,    41,    42,     0,    43,  -308,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -308,     0,     0,    57,    58,
      59,    60,    61,    62,    63,  -308,   -51,    64,    65,    66,
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
    -308,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -308,
       0,     0,    57,    58,    59,    60,    61,    62,    63,  -308,
     -51,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -51,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,   577,   105,     0,   106,
       0,   336,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -51,     0,    40,
      41,    42,     0,    43,  -308,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -308,     0,     0,    57,    58,    59,    60,
      61,    62,    63,  -308,   -51,    64,    65,    66,     0,    67,
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
      39,   -51,     0,    40,    41,    42,     0,    43,  -308,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -308,     0,     0,
      57,    58,    59,    60,    61,    62,    63,  -308,   -51,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -51,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,   823,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -51,     0,    40,    41,    42,
       0,    43,  -308,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -308,     0,     0,    57,    58,    59,    60,   352,    62,
      63,  -308,   -51,    64,    65,    66,     0,    67,    68,    69,
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
       0,    40,    41,    42,     0,    43,  -308,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -308,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -308,   -51,    64,    65,    66,
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
      37,   742,    39,   -51,     0,    40,    41,    42,     0,    43,
    -308,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -308,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -308,
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
      33,    34,    35,    36,    37,   748,    39,   -51,     0,    40,
      41,    42,     0,    43,  -308,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -308,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -308,   -51,    64,    65,    66,     0,    67,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1038,
      39,   -51,     0,    40,    41,    42,     0,    43,  -308,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -308,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -308,   -51,    64,
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
      35,    36,    37,  1040,    39,   -51,     0,    40,    41,    42,
       0,    43,  -308,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -308,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -308,   -51,    64,    65,    66,     0,    67,    68,    69,
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
      31,    32,    33,    34,    35,    36,    37,  1045,    39,   -51,
       0,    40,    41,    42,     0,    43,  -308,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -51,    52,    53,
       0,    54,    55,    56,     0,  -308,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -308,   -51,    64,    65,    66,
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
      37,  1048,    39,   -51,     0,    40,    41,    42,     0,    43,
    -308,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -51,    52,    53,     0,    54,    55,    56,     0,  -308,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -308,
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
      26,    27,     0,  1088,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -51,     0,    40,
      41,    42,     0,    43,  -308,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -51,    52,    53,     0,    54,
      55,    56,     0,  -308,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -308,   -51,    64,    65,    66,     0,    67,
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
      30,     0,    31,    32,    33,    34,    35,    36,    37,  1094,
      39,   -51,     0,    40,    41,    42,     0,    43,  -308,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -51,
      52,    53,     0,    54,    55,    56,     0,  -308,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -308,   -51,    64,
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
      35,    36,    37,  1098,    39,   -51,     0,    40,    41,    42,
       0,    43,  -308,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -51,    52,    53,     0,    54,    55,    56,
       0,  -308,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -308,   -51,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -51,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,  1007,   107,     5,   301,     6,     7,     8,
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
       0,   202,     0,   105,     0,   203,  1008,   957,   107,     5,
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
     201,     0,    78,     0,     0,    80,   305,   306,     0,    84,
     341,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,   342,
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
     305,   306,     0,    84,   341,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,   807,   203,     0,     0,
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
      78,     0,     0,    80,   305,   306,     0,    84,   341,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
     809,   203,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   193,     0,     0,    15,    16,     0,
      17,     0,   194,     0,     0,    21,     0,   713,     0,     0,
       0,     0,     0,     0,    29,     0,   195,     0,     0,     0,
      33,   196,   197,     0,     0,   198,    39,     0,   714,     0,
      41,     0,     0,    43,     0,     0,   199,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   200,     0,   715,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     201,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,   716,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     5,   107,     6,     7,     8,
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
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   307,     0,     0,   103,
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
       0,    50,    51,     0,    52,    53,   959,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   201,     0,
      78,     0,     0,    80,   305,   306,     0,    84,     0,     0,
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
      55,     0,     0,     0,     0,     0,     0,    58,   260,    60,
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
      16,     0,    17,     0,   194,     0,     0,    21,     0,   268,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,   269,    88,     0,     0,
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
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,   261,     0,    91,
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
      78,     0,     0,    80,   305,   306,     0,    84,     0,     0,
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
       0,   202,   805,   105,     0,   203,     0,     0,   107,     5,
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
     815,     0,   107,     5,   301,     6,     7,     8,     9,    10,
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
       0,   105,     0,   203,  1151,     5,   107,     6,     7,     8,
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
       0,     0,     0,   261,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   202,     0,   105,     0,   203,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   285,    29,     0,
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
       0,   103,     0,   104,     0,   105,     0,   203,     0,     0,
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
       0,   364,    86,     0,     0,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     5,   107,     6,     7,     8,
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
       0,   202,     0,   105,     0,   203,   820,     5,   107,     6,
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
       0,     0,     0,    84,   984,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   203,     0,     0,
     107,     5,   301,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   782,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,   783,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,   784,    74,    75,    76,   785,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,  1064,     0,     5,   107,     6,     7,     8,     9,    10,
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
    1179,     0,    86,     0,     0,    88,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,  1064,     0,     0,
     107,     5,   301,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,  1137,    39,     0,     0,     0,    41,     0,
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
       0,    99,   100,   101,     0,     0,     0,   103,     0,   202,
       0,   105,     0,   203,     0,     5,   107,     6,     7,     8,
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
       0,   104,     0,   105,     0,   203,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   193,     0,
       0,    15,    16,     0,    17,     0,   194,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,   782,     0,
     195,     0,     0,     0,    33,   196,   197,     0,     0,   198,
      39,     0,     0,     0,   783,     0,     0,    43,     0,     0,
     199,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   200,     0,     0,     0,     0,     0,     0,     0,    72,
     784,    74,    75,    76,   785,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   202,     0,   105,     0,   786,     0,     5,
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
       0,     0,     0,   834,     0,   202,     0,   105,     0,   203,
       0,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   193,     0,     0,    15,    16,     0,    17,     0,
     194,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   782,     0,   195,     0,     0,     0,    33,   196,
     197,     0,     0,   198,    39,     0,     0,     0,   783,     0,
       0,    43,     0,     0,   199,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   200,     0,     0,     0,     0,
       0,     0,     0,    72,   784,    74,    75,    76,   785,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   202,     0,   105,
       0,   852,     0,     5,   107,     6,     7,     8,     9,    10,
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
       0,   105,     0,   786,     0,     5,   107,     6,     7,     8,
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
     195,     0,     0,     0,    33,   196,   197,     0,     0,   899,
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
       0,  1131,    39,     0,     0,     0,    41,     0,     0,    43,
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
     197,     0,     0,  1132,    39,     0,     0,     0,    41,     0,
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
      33,   196,   197,     0,     0,  1134,    39,     0,     0,     0,
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
       0,     0,    33,   196,   197,     0,     0,  1135,    39,     0,
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
     195,     0,     0,     0,    33,   196,   197,     0,     0,  1136,
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
       0,  1137,    39,     0,     0,     0,    41,     0,     0,    43,
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
       0,   608,     0,   609,     0,     0,     0,    92,    93,    94,
      95,    96,   610,     0,     0,     0,    97,   611,   234,   612,
     613,     0,     0,     0,     0,   614,    98,     0,     0,    99,
     100,   101,     0,     0,   237,   103,   195,     0,     0,   105,
     615,   852,     0,     0,   107,     0,   616,     0,     0,     0,
     240,     0,     0,   617,     0,   618,     0,     0,     0,     0,
       0,     0,     0,   619,     0,     0,     0,     0,     0,   620,
     621,     0,     0,     0,     0,     0,     0,   245,     0,     0,
       0,     0,     0,     0,     0,     0,   622,     0,     0,     0,
       0,     0,     0,     0,     0,   247,   248,     0,   623,     0,
     250,     0,   624,     0,     0,   625,     0,     0,     0,   626,
       0,     0,   254,     0,     0,   627,     0,     0,     0,     0,
       0,     0,     0,     0,   498,   429,   430,   431,   432,   433,
       0,     0,   436,   437,   438,   439,     0,   441,   442,   628,
     629,   630,   631,   632,   633,     0,   634,     0,     0,     0,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,     0,   646,     0,     0,   647,   648,   649,   650,     0,
       5,   651,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,     0,   100,
     101,     0,     0,     0,   103,     0,   202,     0,   105,     5,
     203,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,  -461,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,  -461,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
    -461,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,  -461,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     5,
     301,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     193,     0,     0,    15,    16,     0,    17,     0,   194,   101,
    -461,    21,     0,     0,     0,  -461,     0,   105,     0,   852,
      29,     0,   195,     0,     0,     0,    33,   196,   197,     0,
       0,   198,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   199,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   200,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   201,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,     0,
    -479,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,     0,     0,
       0,  -479,     0,     0,     0,  -479,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   101,
       0,     0,     0,     0,     0,   202,     0,   105,  -479,  1064,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,     0,    94,     0,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
     101,     0,    21,     0,     0,     0,   202,     0,   105,     0,
     203,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    94,     0,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
     101,     0,    21,     0,     0,     0,   202,     0,   105,     0,
     786,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,     0,  1013,    84,     0,     0,    86,
       0,     0,    88,     0,   610,     0,     0,     0,     0,   233,
     234,   612,   235,     0,     0,     0,     0,   236,     0,     0,
       0,     0,     0,     0,     0,     0,   237,     0,    94,     0,
       0,     0,   615,     0,     0,     0,     0,     0,   239,     0,
       0,     0,   240,     0,     0,   241,     0,   618,     0,     0,
       0,     0,     0,     0,     0,   242,     0,     0,   105,     0,
     203,   620,   244,     0,     0,     0,     0,     0,     0,   245,
       0,     0,     0,     0,     0,     0,     0,     0,   246,     0,
       0,     0,     0,     0,     0,     0,   398,   247,   248,     0,
     249,     0,   250,   399,  1014,     0,     0,   625,     0,     0,
       0,   253,     0,     0,   254,     0,   400,   255,     0,     0,
       0,     0,     0,     0,     0,     0,   498,   429,   430,   431,
     432,   433,     0,     0,   436,   437,   438,   439,     0,   441,
     442,   628,   629,   630,   631,   632,   633,     0,   634,     0,
       0,     0,   635,   636,   637,   638,   639,   640,   641,   642,
     643,   644,   645,     0,   646,     0,     0,   647,   648,   649,
     650,     0,   401,     0,     0,     0,   402,     0,     0,   398,
       0,     0,     0,     0,     0,   497,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,   403,   498,   429,
     430,   431,   432,   433,     0,     0,   436,   437,   438,   439,
       0,   441,   442,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,     0,   458,     0,   401,     0,     0,     0,   402,
     427,     0,   398,     0,     0,     0,     0,     0,  1037,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
     403,   498,   429,   430,   431,   432,   433,     0,     0,   436,
     437,   438,   439,     0,   441,   442,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   458,     0,   401,     0,
       0,     0,   402,   427,     0,   398,     0,     0,     0,     0,
       0,  1044,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,   403,   498,   429,   430,   431,   432,   433,
       0,     0,   436,   437,   438,   439,     0,   441,   442,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   458,
       0,   401,     0,     0,     0,   402,   427,     0,   398,     0,
       0,     0,     0,     0,  1203,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,   403,   498,   429,   430,
     431,   432,   433,     0,     0,   436,   437,   438,   439,     0,
     441,   442,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   458,     0,   401,     0,     0,     0,   402,   427,
       0,   398,     0,     0,     0,     0,     0,  1204,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,   403,
     498,   429,   430,   431,   432,   433,     0,     0,   436,   437,
     438,   439,     0,   441,   442,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   458,     0,   401,     0,     0,
       0,   402,   427,     0,   398,     0,     0,     0,     0,     0,
    1205,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,   403,   498,   429,   430,   431,   432,   433,     0,
       0,   436,   437,   438,   439,     0,   441,   442,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,   458,     0,
     401,     0,     0,     0,   402,   427,     0,   398,     0,     0,
       0,     0,     0,  1206,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,   403,   498,   429,   430,   431,
     432,   433,     0,     0,   436,   437,   438,   439,     0,   441,
     442,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,   458,     0,   401,     0,     0,     0,   402,   427,     0,
     398,     0,     0,     0,     0,     0,  1229,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,   403,   498,
     429,   430,   431,   432,   433,     0,     0,   436,   437,   438,
     439,     0,   441,   442,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,   458,     0,   401,     0,     0,     0,
     402,   427,     0,   398,     0,     0,     0,     0,     0,  1230,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,   484,     0,     0,     0,     0,     0,
       0,   403,   498,   429,   430,   431,   432,   433,     0,   485,
     436,   437,   438,   439,     0,   441,   442,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,   458,     0,   401,
       0,     0,     0,   402,   427,   398,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,   285,     0,     0,     0,
     481,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,   490,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   424,   425,   426,     0,
     458,   401,     0,     0,     0,   402,     0,   427,   398,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,   478,
       0,     0,   481,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,   479,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   458,     0,   401,     0,     0,     0,   402,   427,
       0,   398,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,   585,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   586,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   458,     0,   401,     0,     0,
       0,   402,   427,     0,   398,   830,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,   831,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   832,   417,   418,   419,   420,   421,
     422,   423,     0,   587,   424,   425,   426,     0,     0,     0,
     401,     0,     0,     0,   402,   427,   398,     0,     0,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,   285,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,     0,   401,     0,     0,     0,   402,     0,   427,   398,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,   481,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,   460,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,     0,   458,     0,   401,     0,     0,     0,   402,
     427,   398,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,   285,     0,     0,     0,   188,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,     0,   401,     0,     0,
       0,   402,     0,   427,   398,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   722,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
     723,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,   458,     0,
     401,     0,     0,     0,   402,   427,   398,     0,     0,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,   724,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,   725,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   400,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,     0,   401,     0,     0,     0,   402,   398,   427,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,   918,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,   401,     0,     0,     0,   402,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,   403,   424,
     425,   426,     0,   401,     0,     0,     0,   402,     0,   398,
     427,     0,     0,     0,   404,   405,   399,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   400,
       0,   417,   418,   419,   420,   421,   422,   423,   403,     0,
     424,   425,   426,     0,     0,     0,     0,     0,     0,     0,
       0,   427,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,   458,   401,     0,     0,     0,   402,
       0,   427,   398,     0,     0,     0,     0,     0,   940,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   458,     0,   401,     0,
       0,     0,   402,   427,     0,   398,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,   481,
    1107,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,  1108,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,     0,
       0,   401,     0,     0,   976,   402,   427,   398,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,     0,   401,     0,     0,     0,   402,     0,   427,
     398,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,   544,     0,   401,     0,     0,     0,
     402,   427,   398,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,   548,   424,   425,   426,     0,     0,   401,     0,
       0,     0,   402,   398,   427,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,   550,   424,   425,   426,     0,   401,
       0,     0,     0,   402,   398,     0,   427,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,   552,   424,   425,   426,     0,
     401,     0,     0,     0,   402,   398,     0,   427,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,   555,   424,   425,   426,
       0,   401,     0,     0,     0,   402,   398,     0,   427,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,   572,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   401,     0,     0,     0,   402,   398,     0,   427,
       0,     0,     0,     0,   399,   726,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,     0,   401,     0,     0,     0,   402,   398,   826,
     427,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,   401,     0,     0,   806,   402,   398,
       0,   427,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   827,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   401,     0,     0,     0,   402,
     398,     0,   427,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,  -583,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   401,     0,     0,     0,
     402,   398,     0,   427,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,   188,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,   401,     0,     0,
       0,   402,   398,     0,   427,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,   938,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,   909,   424,   425,   426,     0,   401,     0,
       0,     0,   402,   398,     0,   427,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   403,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,     0,
     412,   413,   414,   415,   416,     0,   417,   418,   419,   420,
     421,   422,   423,     0,     0,   424,   425,   426,     0,   401,
       0,     0,     0,   402,   398,     0,   427,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   424,   425,   426,     0,
     401,     0,     0,   961,   402,   398,     0,   427,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,     0,   417,   418,
     419,   420,   421,   422,   423,     0,     0,   424,   425,   426,
       0,   401,     0,     0,   962,   402,   398,   990,   427,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,   979,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,     0,     0,   424,   425,
     426,     0,   401,     0,     0,     0,   402,   398,     0,   427,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   403,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,     0,   424,
     425,   426,     0,   401,     0,     0,     0,   402,   398,     0,
     427,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,  1026,
       0,     0,     0,     0,     0,     0,     0,     0,   403,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,     0,     0,
     424,   425,   426,     0,   401,     0,     0,  1012,   402,   398,
       0,   427,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
    1029,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,   401,     0,     0,     0,   402,
     398,     0,   427,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     403,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,     0,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,     0,   424,   425,   426,     0,   401,     0,     0,     0,
     402,   398,  1104,   427,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   403,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,     0,   412,   413,
     414,   415,   416,     0,   417,   418,   419,   420,   421,   422,
     423,     0,     0,   424,   425,   426,     0,   401,     0,     0,
       0,   402,  1032,   398,   427,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
       0,     0,   403,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,     0,   412,
     413,   414,   415,   416,     0,   417,   418,   419,   420,   421,
     422,   423,     0,     0,   424,   425,   426,     0,     0,   401,
       0,     0,     0,   402,   398,   427,     0,     0,     0,     0,
       0,   399,  1130,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   403,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,   412,   413,   414,   415,   416,     0,   417,   418,   419,
     420,   421,   422,   423,     0,     0,   424,   425,   426,     0,
     401,     0,     0,     0,   402,     0,     0,   427,     0,     0,
       0,     0,     0,  1133,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   403,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,     0,   412,   413,   414,   415,   416,  1164,   417,   418,
     419,   420,   421,   422,   423,   398,  1185,   424,   425,   426,
       0,     0,   399,     0,     0,     0,     0,     0,   427,     0,
       0,     0,     0,     0,   401,   400,     0,     0,   402,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   404,   405,     0,   406,   407,
     408,   401,   409,   410,   411,   402,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
       0,   424,   425,   426,     0,     0,     0,     0,     0,     0,
       0,     0,   427,     0,     0,     0,   403,     0,     0,     0,
       0,   401,     0,     0,     0,   402,     0,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,   403,     0,   424,   425,
     426,     0,     0,     0,     0,     0,     0,     0,     0,   427,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,   416,     0,   417,
     418,   419,   420,   421,   422,   423,   398,     0,   424,   425,
     426,     0,     0,   399,     0,     0,     0,  1225,     0,   427,
     224,     0,     0,     0,    22,    23,   400,     0,     0,   999,
       0,     0,     0,     0,   225,     0,    31,   226,     0,     0,
       0,     0,    37,     0,     0,     0,   398,     0,     0,    42,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
    1000,     0,   401,  1001,     0,   227,   402,    67,     0,     0,
       0,     0,     0,     0,     0,  1238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,     0,     0,   403,     0,     0,
       0,     0,   401,     0,     0,     0,   402,     0,     0,     0,
       0,     0,     0,   404,   405,  1239,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,   403,     0,   424,
     425,   426,   102,     0,     0,     0,     0,     0,  1189,     0,
     427,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,   398,     0,   424,
     425,   426,     0,     0,   399,     0,     0,     0,     0,     0,
     427,   224,     0,     0,     0,    22,    23,   400,     0,     0,
     999,     0,     0,     0,     0,   225,     0,    31,   226,     0,
       0,     0,     0,    37,     0,     0,     0,   398,     0,     0,
      42,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,    57,     0,    59,     0,    61,
       0,  1000,     0,   401,  1001,     0,   227,   402,    67,     0,
       0,     0,     0,     0,     0,     0,  1240,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    83,     0,     0,
      85,     0,     0,    87,     0,     0,   400,     0,   403,     0,
       0,     0,     0,   401,     0,     0,     0,   402,     0,     0,
       0,     0,     0,     0,   404,   405,  1241,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,   403,     0,
     424,   425,   426,   102,     0,     0,     0,     0,     0,  1236,
       0,   427,   401,     0,   404,   405,   402,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
       0,   417,   418,   419,   420,   421,   422,   423,   398,     0,
     424,   425,   426,     0,     0,   399,     0,   403,     0,     0,
       0,   427,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   398,     0,
     417,   418,   419,   420,   421,   399,   423,     0,     0,   424,
     425,   426,     0,     0,     0,     0,     0,     0,   400,     0,
     427,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,     0,   402,   398,
       0,     0,     0,     0,     0,     0,   399,  1242,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   403,
       0,     0,     0,     0,   401,     0,     0,     0,   402,     0,
       0,     0,     0,     0,     0,   404,   405,  1243,   406,   407,
     408,     0,   409,   410,   411,     0,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,   403,
       0,   424,   425,   426,     0,   401,     0,     0,     0,   402,
       0,     0,   427,     0,     0,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,   400,   412,   413,   414,   415,
     416,     0,   417,   418,   419,   420,   421,   422,   423,     0,
     403,   424,   425,   426,     0,     0,   398,     0,     0,     0,
       0,     0,   427,   399,     0,     0,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,   400,   412,   413,   414,
     415,   416,     0,   417,   418,   419,   420,   421,   422,   423,
       0,   401,   424,   425,   426,   402,     0,   398,     0,     0,
       0,  1256,     0,   427,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,     0,     0,   402,     0,     0,     0,
       0,     0,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,     0,   412,   413,   414,   415,     0,     0,   417,
     418,   419,   420,   421,     0,   423,     0,   403,   424,   425,
     426,     0,     0,   894,     0,     0,     0,   402,     0,   427,
       0,     0,     0,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,     0,   412,   413,   414,   415,   416,     0,
     417,   418,   419,   420,   421,   422,   423,     0,   403,   424,
     425,   426,     0,     0,     0,     0,     0,     0,     0,     0,
     427,     0,     0,     0,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     400,   417,   418,   419,   420,   421,   422,   423,     0,   224,
     424,   425,   426,    22,    23,     0,     0,     0,   999,     0,
       0,   427,     0,   225,     0,    31,   226,     0,     0,     0,
       0,    37,     0,     0,     0,     0,     0,     0,    42,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,     0,     0,   401,     0,     0,     0,
     402,     0,     0,    57,     0,    59,     0,    61,     0,  1000,
       0,     0,  1001,     0,   227,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,     0,     0,    85,     0,
       0,    87,     0,     0,     0,     0,     0,   404,   405,   401,
     406,   407,   408,   402,   409,   410,   411,     0,     0,   413,
     414,   415,     0,     0,   417,   418,   419,   420,     0,     0,
     423,     0,     0,   424,   425,   426,     0,     0,     0,     0,
       0,     0,     0,     0,   427,     0,     0,     0,     0,     0,
       0,   102,     0,     0,     0,     0,     0,  1237,     0,     0,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
       0,     0,   413,     0,     0,     0,     0,     0,     0,   419,
     420,     0,     0,   423,     0,   224,   424,   425,   426,    22,
      23,     0,     0,     0,   999,     0,     0,   427,     0,   225,
       0,    31,   226,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,     0,    42,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    57,
     371,    59,   232,   352,     0,  1000,     0,     0,  1001,     0,
     227,     0,    67,     0,     0,     0,   233,   234,     0,   235,
       0,     0,     0,     0,   236,     0,     0,     0,     0,     0,
     372,    83,     0,   237,    85,     0,   232,    87,     0,   238,
       0,   598,     0,     0,     0,   239,     0,     0,     0,   240,
     233,   234,   241,   235,     0,     0,     0,     0,   236,     0,
       0,     0,   242,     0,     0,     0,     0,   237,   243,   244,
       0,     0,     0,   238,     0,     0,   245,     0,     0,   239,
       0,     0,     0,   240,     0,   246,   241,   102,     0,     0,
       0,     0,     0,     0,   247,   248,   242,   249,     0,   250,
       0,   251,   243,   244,   252,     0,     0,     0,   253,     0,
     245,   254,     0,     0,   255,     0,     0,     0,     0,   246,
       0,     0,   232,     0,     0,     0,     0,     0,   247,   248,
       0,   249,     0,   250,     0,   251,   233,   234,   252,   235,
       0,     0,   253,     0,   236,   254,     0,     0,   255,     0,
     372,     0,     0,   237,     0,     0,  -287,     0,     0,   238,
       0,     0,     0,     0,     0,   239,     0,     0,     0,   240,
    -287,  -287,   241,  -287,     0,     0,     0,     0,  -287,     0,
       0,     0,   242,     0,     0,     0,     0,  -287,   243,   244,
       0,     0,     0,  -287,     0,     0,   245,     0,     0,  -287,
       0,     0,     0,  -287,     0,   246,  -287,     0,     0,     0,
       0,     0,     0,     0,   247,   248,  -287,   249,     0,   250,
       0,   251,  -287,  -287,   252,     0,     0,     0,   253,     0,
    -287,   254,     0,     0,   255,     0,     0,     0,     0,  -287,
       0,     0,   232,     0,     0,     0,     0,     0,  -287,  -287,
       0,  -287,     0,  -287,     0,  -287,   233,   234,  -287,   235,
       0,     0,  -287,     0,   236,  -287,     0,     0,  -287,     0,
       0,     0,     0,   237,     0,     0,  -288,     0,     0,   238,
       0,     0,     0,     0,     0,   239,     0,     0,     0,   240,
    -288,  -288,   241,  -288,     0,     0,     0,     0,  -288,     0,
       0,     0,   242,     0,     0,     0,     0,  -288,   243,   244,
       0,     0,     0,  -288,     0,     0,   245,     0,     0,  -288,
       0,     0,     0,  -288,     0,   246,  -288,     0,     0,     0,
       0,     0,     0,     0,   247,   248,  -288,   249,     0,   250,
       0,   251,  -288,  -288,   252,     0,     0,     0,   253,     0,
    -288,   254,     0,     0,   255,     0,     0,     0,     0,  -288,
       0,     0,     0,     0,     0,     0,     0,     0,  -288,  -288,
       0,  -288,     0,  -288,     0,  -288,     0,     0,  -288,     0,
       0,     0,  -288,     0,     0,  -288,     0,     0,  -288
};

static const yytype_int16 yycheck[] =
{
      12,     2,   275,   172,    16,    69,   605,   390,    20,    19,
     449,    68,   279,   659,    26,   158,   746,   522,   503,   520,
     527,   372,    34,    35,    36,   298,    38,   832,   848,   586,
      27,    33,   831,   217,    27,   546,   950,    49,   953,    51,
     953,   105,   956,   316,    56,   231,    58,    40,    60,   106,
      48,     1,   946,   265,   266,   267,    68,    69,    70,    71,
      72,    73,    33,   590,   591,    77,     3,    79,    33,    81,
      82,     1,     1,     1,    86,  1062,  1008,    89,    90,    47,
      92,    48,    94,    20,    61,    97,    98,   119,   100,   275,
     101,    33,   104,   105,   106,   107,    56,    34,   605,   145,
     656,   657,     1,    48,  1151,   119,    37,  1021,    56,    48,
       3,   145,   298,    33,     0,   139,    84,   129,  1215,   131,
     144,   119,    74,   147,    74,    48,    63,   151,   174,    48,
     316,    61,    61,   994,   995,   347,   348,   164,   172,     3,
     117,    72,   174,   145,   171,   113,   203,   145,  1062,    48,
     880,    11,   659,    48,   323,    91,   168,    68,   169,  1256,
     174,    91,    91,  1150,   116,   102,   116,   169,  1215,  1063,
      89,   108,   445,   446,   104,   164,   174,   119,   145,   999,
      56,   193,   366,  1003,   196,   197,   198,   117,   117,   201,
     202,   203,  1124,   104,   105,   106,   124,   119,   169,   119,
     145,    34,    35,    36,   169,   215,   145,   174,    32,   102,
    1125,   171,  1125,    61,   144,   144,   144,   145,   170,   231,
     170,   390,   145,   171,   410,   124,   223,   169,   739,   174,
     223,    61,   169,   169,   164,   174,  1150,    27,   102,   169,
     169,     3,   727,    91,   172,   144,   145,   598,   108,   169,
     145,   174,  1113,  1114,    61,   117,   104,    48,    48,   445,
     446,    91,    52,   275,    88,   164,   164,     3,    92,   117,
      61,   776,   170,   106,   104,   832,    82,    56,   145,    85,
     781,    48,   145,   145,    91,    75,   298,   117,    78,   108,
      80,   202,   203,   305,   306,  1115,  1116,   104,  1118,    89,
      91,  1106,    27,  1108,   316,   172,  1105,   746,  1107,   172,
     117,   368,  1042,   840,   841,   139,   164,   503,   108,   462,
     144,   169,    89,   147,   145,   145,   117,   151,   164,    91,
     342,   145,    89,   124,   164,   171,    54,   173,   339,   169,
     102,    56,   145,   355,   145,   357,    56,   145,    91,     2,
     949,   172,   172,    78,   145,    73,   368,   164,   172,    12,
     546,   846,   169,   164,    89,    83,  1186,  1187,    18,   172,
     434,   435,    25,   164,   172,    28,   440,   171,   169,   443,
     444,   172,   172,   108,   396,   171,   398,   399,   400,   401,
     402,   403,   893,   405,   406,   407,   408,   409,   145,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   145,   164,    27,   476,
     905,   164,   434,   435,    77,   982,   169,    77,   440,    87,
       3,   443,   444,   445,   446,   447,   448,   449,   145,  1085,
     164,   880,   949,   172,    27,    95,   170,    20,   460,   145,
     593,   104,   464,   111,   145,   467,   109,   368,    56,  1015,
    1016,    34,   112,    27,   476,   172,   478,   479,    51,    78,
     171,    47,   484,   485,   145,   145,   172,   145,   490,   862,
      89,   172,    88,   124,   139,   497,    92,     2,   500,    65,
      63,   503,   147,    76,    56,    78,   145,    12,   562,   170,
     564,  1008,   172,   145,   172,   114,    89,   774,    84,   150,
      25,   152,   153,    28,    78,   156,   157,   529,   139,  1073,
      88,   533,   534,   172,    92,    89,   147,   539,   170,   102,
     145,   114,   139,    56,   546,   108,   189,   113,   750,   751,
     147,   727,   754,   755,   108,    47,   171,   559,   173,  1106,
     562,  1108,   564,   739,   124,   170,   568,   569,   570,   145,
     572,   573,    77,    65,   145,   476,    81,    82,   401,   402,
      32,   124,   124,   585,   586,    56,   145,   578,  1085,   149,
     145,   848,    84,   164,   170,   169,    56,   171,   600,   104,
      56,   858,   145,   145,   109,   164,   149,   149,   562,   164,
     564,   813,   814,  1042,  1158,  1159,  1160,  1161,  1162,  1163,
     145,   113,   145,     8,   447,   448,   145,  1124,    56,   172,
     172,   171,   171,    48,   173,   278,    88,   460,   539,   164,
      92,   164,   285,    81,    82,   164,    61,   299,   300,   651,
     826,   827,    99,   171,  1151,   173,   479,   171,   559,   173,
      56,   562,   485,   564,   171,   171,   173,   490,   569,   171,
     846,   173,   171,    56,   173,    56,    91,    56,    56,   169,
      56,   144,   164,   737,   189,   163,   155,   139,   140,   164,
     142,   143,   144,    56,   146,   147,   339,   164,   164,   151,
     164,   171,   117,   862,   169,   164,   158,   159,   108,   124,
     162,   354,    84,   165,   166,   167,   164,   118,  1215,   164,
     722,   723,   724,   725,   726,   727,   728,   729,  1225,   905,
     145,   731,   732,   164,   169,   737,   790,   739,   740,   786,
     742,    32,   171,     8,   746,   568,   748,   390,   171,   164,
     171,    27,   171,   164,   169,   757,   758,   172,   171,  1256,
      65,   169,   124,   170,    89,    89,   124,   164,    68,   171,
     147,   124,    48,   278,   172,   145,    52,    48,   171,   124,
     285,   124,   784,   785,   786,    35,    35,    65,   790,    27,
     172,   145,   172,   124,    21,   145,   104,    88,   172,    75,
     171,    92,    78,   172,    80,   852,   174,   174,   147,   147,
      48,   147,   139,    89,    52,   458,   139,   147,   147,   147,
     139,   147,   824,  1086,  1087,   147,   828,   147,   830,   831,
     832,   147,   108,   147,   339,   478,   737,    75,   139,   145,
      78,   484,    80,   139,   846,   147,   164,   164,   139,   354,
     852,    89,   495,   144,   497,   146,   147,   164,  1115,  1116,
     151,   164,   172,     2,    35,   164,   164,   171,   159,    35,
     108,   144,   164,    12,   165,   166,   167,   174,   880,   172,
     170,   145,   164,   164,   169,   786,    25,    32,    48,    28,
     169,   174,   894,   895,   896,   170,    33,   899,   172,    33,
     723,    61,   725,   905,   174,    35,   164,   164,   174,   171,
    1086,  1087,   171,   144,   916,   139,   918,   164,   965,   174,
     174,   923,   164,   566,   926,   169,   169,   570,   174,  1186,
    1187,    91,   172,   139,    35,   578,   938,   172,    77,   164,
     172,   164,   172,    88,   164,   164,   164,    92,   145,   163,
     952,   852,   163,   458,   824,   354,    24,   117,  1003,   274,
      89,   969,   944,   965,   124,   104,   862,   324,   858,   851,
     109,   366,   974,   478,   976,   774,   514,   979,  1125,   484,
     982,  1164,   857,   867,  1157,   145,   988,   129,   990,   530,
     495,   280,   497,   780,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,    -1,   164,    -1,   151,    -1,    -1,   169,
      -1,    -1,   172,    -1,   159,    -1,    -1,  1064,    -1,    -1,
     165,   166,   167,  1025,  1026,    -1,  1028,  1029,    -1,  1031,
    1032,    -1,    -1,    -1,    -1,  1037,  1038,    -1,  1040,    -1,
    1042,    -1,  1044,  1045,    -1,    -1,  1048,    -1,   949,    -1,
     189,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,   566,  1064,    -1,   965,   570,    -1,  1069,    -1,    -1,
      -1,   894,   895,   578,    -1,    -1,   719,    -1,    -1,    -1,
      -1,    -1,  1084,  1085,  1086,  1087,    -1,    -1,    -1,    -1,
    1092,    -1,  1094,    -1,    -1,    -1,  1098,    -1,    -1,    -1,
      -1,    -1,  1104,  1105,  1106,  1107,  1108,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,   760,    -1,    92,
     763,    -1,  1124,    -1,    -1,    -1,    -1,    -1,  1130,  1131,
    1132,  1133,  1134,  1135,  1136,  1137,    -1,    -1,    -1,   278,
      -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1156,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1064,    -1,    -1,   139,   140,  1069,   812,
     143,   144,     2,   146,   147,   818,    -1,    -1,   151,    71,
      -1,    -1,    12,  1185,    -1,    -1,   159,    -1,    -1,    -1,
      -1,    -1,   165,   166,   167,    25,    -1,    -1,    28,    -1,
     339,  1203,  1204,  1205,  1206,    -1,    -1,    -1,    32,    -1,
      -1,    -1,  1214,  1215,    -1,   354,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1225,    -1,    -1,    -1,  1229,  1230,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1238,  1239,  1240,  1241,
    1242,  1243,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
    1151,   894,   895,    -1,  1256,   760,    -1,    -1,   763,    -1,
      -1,    -1,     2,    -1,    88,    -1,    -1,    -1,    92,    -1,
      32,    -1,    12,   916,   104,   918,    -1,    -1,    -1,   109,
     923,    -1,    -1,   926,    -1,    25,    -1,    -1,    28,    -1,
      -1,    -1,   935,    -1,   937,    -1,    -1,   940,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   812,    -1,    -1,
      -1,    -1,    -1,   818,  1215,   139,   140,    -1,    -1,   458,
     144,   964,   146,   147,    -1,    -1,    88,   151,    -1,    -1,
      92,   974,    -1,   976,    -1,   159,    -1,    77,    -1,   478,
      -1,   165,   166,   167,    -1,   484,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1256,   495,    -1,   497,   189,
      -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,    -1,   109,
     262,   263,   264,   265,   266,   267,    -1,   139,    -1,    -1,
      -1,    -1,   144,    -1,   146,   147,    -1,    -1,    -1,   151,
      -1,    -1,    -1,    -1,  1037,    -1,   288,   159,    -1,    -1,
      -1,  1044,   294,     1,    -1,   167,    -1,    -1,    -1,    -1,
      -1,   916,    -1,   918,  1057,    -1,    -1,    -1,   923,    -1,
     312,   926,    -1,    -1,    22,    -1,    -1,   566,    26,    27,
     935,   570,   937,    31,    -1,   940,   328,   329,    36,   578,
      38,    39,    -1,    -1,    -1,  1088,    44,    -1,   278,   189,
      -1,    -1,    -1,    51,  1097,   285,    54,    -1,  1101,   964,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   974,
      -1,   976,    -1,    -1,    -1,    73,    -1,    -1,    76,    -1,
      78,    -1,    80,    -1,    82,    83,    -1,    85,    -1,    87,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1146,    -1,     2,    -1,    -1,    -1,   339,
     108,    -1,    -1,   111,    -1,    12,   114,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   354,    -1,    -1,    -1,    25,    -1,
      -1,    28,  1037,    -1,    -1,    -1,    -1,    -1,   278,  1044,
      -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,    -1,    -1,
      -1,    -1,  1057,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1203,  1204,  1205,  1206,    -1,    -1,   164,    -1,    -1,   461,
      -1,    -1,   170,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      77,    -1,    -1,  1088,    -1,    -1,  1229,  1230,    -1,    -1,
      17,   483,  1097,    20,    -1,   487,  1101,   489,    25,   339,
     492,    -1,    -1,    -1,     8,    -1,    -1,   104,    -1,    36,
      -1,    -1,   109,    -1,   354,    -1,    -1,    -1,    22,    46,
      -1,   760,    26,    27,   763,    -1,    53,    -1,   458,    -1,
      -1,    -1,    36,    -1,    38,    39,    63,    -1,    -1,    -1,
      44,  1146,    -1,    70,    -1,    -1,    -1,    51,   478,    -1,
      -1,    -1,    -1,    -1,   484,    -1,    -1,    -1,    -1,    86,
      -1,    65,    -1,    -1,    -1,   495,    -1,   497,    -1,    -1,
      -1,    98,    76,   812,    78,   102,    -1,    -1,    82,   818,
      84,    85,   109,    87,    -1,    89,    -1,    -1,   115,    -1,
      -1,    -1,   189,    -1,    -1,    -1,    -1,    -1,  1203,  1204,
    1205,  1206,    -1,    -1,   108,    -1,    -1,   111,    -1,    -1,
     114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   458,    -1,
      -1,    -1,    -1,    -1,  1229,  1230,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   566,    -1,   478,    -1,
     570,    -1,    -1,    -1,   484,    -1,    -1,    -1,   578,    -1,
      -1,    -1,    -1,    -1,    -1,   495,    -1,   497,    -1,    -1,
     164,    -1,    -1,    -1,    -1,    -1,     2,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,   916,    -1,   918,
      -1,   278,    -1,    -1,   923,    -1,    -1,   926,   285,    25,
      -1,    -1,    28,    -1,    -1,    -1,   935,    -1,   937,    -1,
      -1,   940,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   964,   566,    -1,    -1,    -1,
     570,    -1,    -1,   155,    -1,   974,    -1,   976,   578,    -1,
      -1,    77,   339,    -1,     2,    -1,   168,    -1,    -1,   171,
      -1,   173,   744,   745,    12,    -1,    -1,   354,   750,   751,
      -1,    -1,   754,   755,    -1,    -1,    -1,    25,   104,    -1,
      28,    -1,    -1,   109,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1037,    -1,
       3,    -1,    -1,    -1,    -1,  1044,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    17,    18,    -1,    20,  1057,    77,
     168,    -1,    25,   171,    -1,   173,    -1,    -1,    -1,    -1,
     760,    34,    -1,   763,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,   104,    50,    -1,  1088,
      53,   109,    -1,   189,     2,    -1,    -1,    -1,  1097,    -1,
      63,   458,  1101,    -1,    12,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    25,    -1,    -1,
      28,   478,   812,    86,    -1,    -1,    -1,   484,   818,    -1,
      -1,    -1,    95,    96,    -1,    98,    -1,   100,   495,   102,
     497,    -1,   105,    -1,    -1,    -1,   109,  1146,    -1,   112,
      -1,    -1,   115,    -1,   906,   907,    -1,    -1,    -1,    -1,
     760,   913,    -1,   763,    -1,    -1,    -1,    -1,    -1,    77,
      -1,   189,    -1,   925,    -1,    -1,   928,    -1,   930,    -1,
     932,    -1,   278,    -1,    -1,    -1,    -1,    -1,    -1,   285,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,   109,    -1,    -1,  1203,  1204,  1205,  1206,   171,   566,
      -1,    -1,   812,   570,    -1,    -1,    -1,    -1,   818,    -1,
      -1,   578,    -1,    -1,    -1,    -1,   916,    -1,   918,    -1,
    1229,  1230,    -1,   923,    -1,    -1,   926,    -1,    -1,    -1,
      -1,    -1,    -1,   339,    -1,   935,    -1,   937,    -1,    -1,
     940,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   354,    -1,
     278,    -1,    -1,    -1,    -1,    -1,    -1,   285,    -1,    -1,
      -1,    -1,    -1,    -1,   964,    -1,    -1,    -1,    -1,    -1,
      -1,   189,    -1,    -1,   974,    -1,   976,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     8,    -1,    -1,   916,    -1,   918,    -1,
      -1,   339,    -1,   923,    -1,    -1,   926,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,   935,   354,   937,  1090,    -1,
     940,    36,    -1,    38,    39,    -1,    -1,  1037,    -1,    44,
      -1,    -1,    -1,    -1,  1044,    -1,    51,    -1,    -1,    -1,
      -1,    -1,   458,    -1,   964,    -1,    -1,  1057,    -1,    -1,
     278,    -1,    -1,    -1,   974,    -1,   976,   285,    -1,    -1,
      -1,    76,   478,    78,    -1,    -1,    -1,    82,   484,    -1,
      85,    -1,    87,    -1,    89,    -1,    -1,    -1,  1088,   495,
      -1,   497,    -1,   760,    -1,    -1,   763,  1097,    -1,    -1,
       2,  1101,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   339,    -1,    25,    -1,    -1,    28,  1037,    -1,    -1,
     458,    -1,    -1,    -1,  1044,    -1,   354,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   812,  1146,  1057,    -1,    -1,
     478,   818,    -1,    -1,    -1,    -1,   484,    -1,    -1,   164,
     566,    -1,    -1,    -1,   570,    -1,    22,   495,    -1,   497,
      26,    27,   578,    -1,    -1,    77,    -1,    -1,  1088,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,  1097,    44,    -1,
      -1,  1101,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,    -1,   104,  1203,  1204,  1205,  1206,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,  1229,
    1230,    87,    -1,    89,    -1,    -1,  1146,    -1,   566,    -1,
     458,    -1,   570,    49,    -1,    -1,    -1,    -1,    -1,   916,
     578,   918,   108,    -1,    -1,   111,   923,    -1,   114,   926,
     478,    -1,    -1,    -1,    -1,    71,   484,    -1,   935,    -1,
     937,    -1,    -1,   940,    -1,    81,    82,   495,    -1,   497,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   189,    -1,    -1,
      -1,    -1,    -1,  1203,  1204,  1205,  1206,   964,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   974,     3,   976,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1229,
    1230,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,   566,    -1,
      -1,    46,   570,    -1,   760,    50,    -1,   763,    53,    -1,
     578,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
    1037,    -1,    -1,    -1,    69,    70,   278,  1044,    -1,    -1,
      -1,    -1,    77,   285,    -1,    -1,    -1,    -1,    -1,    -1,
    1057,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    -1,    98,    -1,   100,   812,   102,   214,    -1,
     105,    -1,   818,    -1,   109,   110,    -1,   112,    -1,    -1,
     115,  1088,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1097,    -1,   760,    -1,  1101,   763,    -1,   339,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   354,    -1,    -1,    -1,   262,   263,   264,   265,
     266,   267,    -1,    -1,   270,   271,    -1,    -1,    -1,    -1,
      -1,   277,    -1,    -1,    -1,    -1,   171,    -1,    -1,  1146,
      -1,    -1,   288,    -1,   812,    -1,    -1,    -1,   294,    -1,
     818,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   312,    -1,    -1,    -1,
     916,    -1,   918,    -1,    -1,    -1,    -1,   923,    -1,    -1,
     926,    -1,   328,   329,    -1,    -1,    -1,    -1,    -1,   935,
      -1,   937,    -1,    -1,   940,    -1,  1203,  1204,  1205,  1206,
      -1,    -1,   760,    22,    -1,   763,    -1,    26,    27,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   458,    36,   964,    38,
      39,    -1,  1229,  1230,    -1,    44,    -1,    -1,   974,    -1,
     976,    -1,    51,    -1,    -1,    -1,   478,    -1,    -1,    -1,
      -1,    -1,   484,    -1,    -1,    -1,    65,    -1,   916,    -1,
     918,    -1,    -1,   495,   812,   923,    -1,    76,   926,    78,
     818,    -1,    -1,    82,    -1,    84,    85,   935,    87,   937,
      89,    -1,   940,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,  1037,   111,    -1,    -1,   114,   964,    -1,  1044,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   974,    -1,   976,    -1,
      -1,  1057,    -1,    -1,    -1,   461,    -1,    -1,    30,    -1,
      -1,    -1,    -1,    -1,   566,    -1,    -1,    -1,   570,    -1,
      -1,    -1,    44,    45,    46,    -1,   578,   483,    -1,    -1,
      -1,   487,  1088,   489,    -1,   164,   492,    -1,    -1,    -1,
      -1,  1097,    -1,    -1,    -1,  1101,    -1,    -1,   916,    -1,
     918,    -1,    -1,    -1,    -1,   923,    -1,    -1,   926,  1037,
      -1,    -1,    -1,    -1,    -1,    -1,  1044,   935,    -1,   937,
      -1,    22,   940,    -1,    -1,    26,    27,    -1,    -1,  1057,
      31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
    1146,    -1,    -1,    44,    -1,    -1,   964,    -1,    -1,    22,
      51,    -1,    -1,    26,    27,    -1,   974,    -1,   976,    -1,
    1088,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,  1097,
      -1,    44,    -1,  1101,   146,    76,   148,    78,    51,    80,
      -1,    82,    -1,    -1,    85,   157,    87,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1203,  1204,  1205,
    1206,    -1,    -1,    76,    -1,    78,    -1,   108,    -1,    82,
     111,    -1,    85,   114,    87,    -1,    89,    -1,  1146,  1037,
      -1,    -1,    -1,  1229,  1230,    -1,  1044,   199,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,   111,  1057,
      -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   225,    -1,    -1,    -1,    -1,   760,    -1,
      -1,   763,    -1,   164,    -1,    -1,    -1,    -1,    -1,   170,
    1088,    -1,    -1,    -1,    -1,  1203,  1204,  1205,  1206,  1097,
      -1,    -1,    -1,  1101,    -1,    -1,    -1,    -1,   260,    -1,
      -1,   164,    -1,    -1,    -1,    -1,   268,   269,    -1,    -1,
      -1,  1229,  1230,    -1,    -1,    -1,    -1,    -1,    -1,   281,
     812,    -1,    -1,    22,    -1,    -1,   818,    26,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,  1146,    38,
      39,    -1,    -1,    -1,    -1,    44,    -1,    -1,   744,   745,
      -1,    -1,    51,    -1,   750,   751,    -1,    -1,   754,   755,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   764,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    85,    -1,    87,    -1,
      89,    -1,    -1,    -1,    -1,  1203,  1204,  1205,  1206,    -1,
      -1,   363,   364,    -1,    -1,   367,    -1,   803,    -1,   108,
     372,    -1,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,  1229,  1230,    -1,   916,    -1,   918,    -1,   390,    -1,
      -1,   923,    -1,    -1,   926,    -1,    -1,    -1,     3,    -1,
      -1,    -1,    -1,   935,    -1,   937,    -1,    -1,   940,    -1,
      -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    34,
      -1,   170,   964,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,   974,    -1,   976,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,   457,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,
     906,   907,    77,    -1,    -1,    -1,    -1,   913,    -1,   915,
      -1,    86,    -1,    -1,    -1,    -1,   922,    -1,    -1,   925,
      95,    96,   928,    98,   930,   100,   932,   102,    -1,    -1,
     105,    -1,   504,    -1,   109,    -1,    -1,   112,    -1,   945,
     115,    -1,    -1,    -1,    -1,    -1,   518,    -1,    -1,    -1,
     522,    -1,    -1,    -1,     3,  1057,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   972,    19,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    34,  1088,    -1,    -1,    -1,
      -1,    40,    -1,    -1,   169,  1097,   171,    46,    -1,  1101,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   584,    -1,    63,    -1,   588,   589,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,   598,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    88,    86,    -1,    -1,
      92,    -1,    -1,    -1,  1146,    -1,    95,    96,    -1,    98,
      -1,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,   119,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1090,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,    -1,
      -1,    -1,   171,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1140,  1141,    -1,    -1,  1144,  1145,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1172,    -1,  1174,    -1,
    1176,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,   765,    25,    26,    27,    -1,    -1,    -1,
      -1,   773,    -1,    34,   776,    36,    -1,    -1,   780,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,   803,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,   851,
      -1,   112,    -1,   114,   115,    -1,    -1,    -1,    -1,    -1,
     862,    -1,   123,    -1,    -1,   867,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,    -1,   176,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       0,     1,    -1,     3,    -1,     5,     6,     7,     8,     9,
      10,    -1,   944,    -1,    14,    15,    16,    17,    18,    -1,
      20,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    33,    34,    -1,    36,   969,    38,    39,
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
     106,   107,    -1,   109,   110,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,
      -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,
      -1,   167,    -1,   169,    -1,   171,   172,   173,    -1,    -1,
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
     102,    -1,    -1,   105,   106,   107,    -1,   109,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
     172,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    48,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,   114,   115,    -1,    -1,    -1,    -1,
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
      98,    99,   100,    -1,   102,    -1,    -1,   105,   106,   107,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,
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
      -1,    63,    64,    -1,    66,    67,    68,    69,    70,    -1,
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
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
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
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
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
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,
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
     102,    -1,    -1,   105,   106,   107,    -1,   109,    -1,    -1,
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
      -1,   169,   170,   171,    -1,   173,    -1,    -1,   176,     3,
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
     174,    -1,   176,     3,     4,     5,     6,     7,     8,     9,
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
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,   121,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,
     148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,
      -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
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
      -1,   111,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
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
      -1,     1,    -1,     3,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    12,    -1,    -1,    -1,   148,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    34,   167,    36,    -1,    -1,   171,
      40,   173,    -1,    -1,   176,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    -1,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,    -1,   146,    -1,    -1,    -1,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,    -1,   162,    -1,    -1,   165,   166,   167,   168,    -1,
       3,   171,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,    -1,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,     3,
     173,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    61,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
     104,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,   163,
     164,    25,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,    -1,   102,    -1,
      -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,
      -1,   145,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,
      -1,    -1,    -1,    -1,    -1,   169,    -1,   171,   172,   173,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,    -1,   141,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
     163,    -1,    25,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
     163,    -1,    25,    -1,    -1,    -1,   169,    -1,   171,    -1,
     173,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,     3,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    12,    -1,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,   141,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,   171,    -1,
     173,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    95,    96,    -1,
      98,    -1,   100,    19,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    32,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,   146,    -1,
      -1,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,    -1,   162,    -1,    -1,   165,   166,   167,
     168,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    12,
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
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,   123,   124,   125,   126,   127,   128,   129,    -1,    48,
     132,   133,   134,   135,    -1,   137,   138,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,
      -1,    -1,    -1,    92,   176,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
     119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    88,    -1,    -1,    -1,    92,    -1,   176,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,   176,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    72,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,
      -1,    92,   176,    -1,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    37,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    72,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,   165,   166,   167,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    92,   176,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    -1,    88,    -1,    -1,    -1,    92,    -1,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,   119,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    -1,    88,    -1,    -1,
      -1,    92,    -1,   176,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,
      88,    -1,    -1,    -1,    92,   176,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    32,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    -1,    88,    -1,    -1,    -1,    92,    12,   176,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,   123,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    -1,    12,
     176,    -1,    -1,    -1,   139,   140,    19,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,    32,
      -1,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    88,    -1,    -1,    -1,    92,
      -1,   176,    12,    -1,    -1,    -1,    -1,    -1,   101,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,
      -1,    -1,    92,   176,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,   119,
      37,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    72,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,
      -1,    88,    -1,    -1,   174,    92,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,   176,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,
      92,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,   165,   166,   167,    -1,    -1,    88,    -1,
      -1,    -1,    92,    12,   176,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
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
     158,   159,   160,   161,   162,    -1,   164,   165,   166,   167,
      -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,   101,    -1,    -1,    -1,    -1,
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
     165,   166,   167,    -1,    88,    -1,    -1,   172,    92,    12,
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
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,   165,   166,   167,    -1,    88,    -1,
      -1,    -1,    92,    12,    -1,   176,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
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
      88,    -1,    -1,   172,    92,    12,    -1,   176,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,    88,    -1,    -1,   172,    92,    12,    13,   176,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   122,   123,    -1,    -1,    -1,
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
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,
     176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,    88,    -1,    -1,   172,    92,    12,
      -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,
      12,    -1,   176,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,
      92,    12,    13,   176,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,    88,    -1,    -1,
      -1,    92,   174,    12,   176,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,    -1,    88,
      -1,    -1,    -1,    92,    12,   176,    -1,    -1,    -1,    -1,
      -1,    19,   101,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
      88,    -1,    -1,    -1,    92,    -1,    -1,   176,    -1,    -1,
      -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    61,   156,   157,
     158,   159,   160,   161,   162,    12,    13,   165,   166,   167,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   176,    -1,
      -1,    -1,    -1,    -1,    88,    32,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    88,   146,   147,   148,    92,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   176,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,   123,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    12,    -1,   165,   166,
     167,    -1,    -1,    19,    -1,    -1,    -1,   174,    -1,   176,
      22,    -1,    -1,    -1,    26,    27,    32,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
      -1,    -1,    44,    -1,    -1,    -1,    12,    -1,    -1,    51,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,
      82,    -1,    88,    85,    -1,    87,    92,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,   111,
      -1,    -1,   114,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   101,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   123,    -1,   165,
     166,   167,   164,    -1,    -1,    -1,    -1,    -1,   170,    -1,
     176,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    12,    -1,   165,
     166,   167,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     176,    22,    -1,    -1,    -1,    26,    27,    32,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    12,    -1,    -1,
      51,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
      -1,    82,    -1,    88,    85,    -1,    87,    92,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,
     111,    -1,    -1,   114,    -1,    -1,    32,    -1,   123,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   101,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,   164,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   176,    88,    -1,   139,   140,    92,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    12,    -1,
     165,   166,   167,    -1,    -1,    19,    -1,   123,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,    12,    -1,
     156,   157,   158,   159,   160,    19,   162,    -1,    -1,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,   139,   140,   101,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,   123,
      -1,   165,   166,   167,    -1,    88,    -1,    -1,    -1,    92,
      -1,    -1,   176,    -1,    -1,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    32,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
     123,   165,   166,   167,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   176,    19,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    32,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    88,   165,   166,   167,    92,    -1,    12,    -1,    -1,
      -1,   174,    -1,   176,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,    -1,    -1,   156,
     157,   158,   159,   160,    -1,   162,    -1,   123,   165,   166,
     167,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,   176,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,   123,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      32,   156,   157,   158,   159,   160,   161,   162,    -1,    22,
     165,   166,   167,    26,    27,    -1,    -1,    -1,    31,    -1,
      -1,   176,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,
      -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    88,    -1,    -1,    -1,
      92,    -1,    -1,    76,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    85,    -1,    87,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   108,    -1,    -1,   111,    -1,
      -1,   114,    -1,    -1,    -1,    -1,    -1,   139,   140,    88,
     142,   143,   144,    92,   146,   147,   148,    -1,    -1,   151,
     152,   153,    -1,    -1,   156,   157,   158,   159,    -1,    -1,
     162,    -1,    -1,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      -1,   164,    -1,    -1,    -1,    -1,    -1,   170,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,    -1,   151,    -1,    -1,    -1,    -1,    -1,    -1,   158,
     159,    -1,    -1,   162,    -1,    22,   165,   166,   167,    26,
      27,    -1,    -1,    -1,    31,    -1,    -1,   176,    -1,    36,
      -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
       1,    78,     3,    80,    -1,    82,    -1,    -1,    85,    -1,
      87,    -1,    89,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,   108,    -1,    34,   111,    -1,     3,   114,    -1,    40,
      -1,     8,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      17,    18,    53,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    34,    69,    70,
      -1,    -1,    -1,    40,    -1,    -1,    77,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    86,    53,   164,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    63,    98,    -1,   100,
      -1,   102,    69,    70,   105,    -1,    -1,    -1,   109,    -1,
      77,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    86,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    98,    -1,   100,    -1,   102,    17,    18,   105,    20,
      -1,    -1,   109,    -1,    25,   112,    -1,    -1,   115,    -1,
      31,    -1,    -1,    34,    -1,    -1,     3,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      17,    18,    53,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    34,    69,    70,
      -1,    -1,    -1,    40,    -1,    -1,    77,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    86,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    63,    98,    -1,   100,
      -1,   102,    69,    70,   105,    -1,    -1,    -1,   109,    -1,
      77,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    86,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    98,    -1,   100,    -1,   102,    17,    18,   105,    20,
      -1,    -1,   109,    -1,    25,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    34,    -1,    -1,     3,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      17,    18,    53,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    34,    69,    70,
      -1,    -1,    -1,    40,    -1,    -1,    77,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    86,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    63,    98,    -1,   100,
      -1,   102,    69,    70,   105,    -1,    -1,    -1,   109,    -1,
      77,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      -1,    98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115
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
     312,   313,   314,   315,   316,   317,   318,   326,   327,   328,
     329,   330,   333,   334,   335,   336,   337,   338,   339,   340,
     342,   343,   344,   345,   346,   164,   185,   330,   119,   319,
     320,     3,   206,    14,    22,    36,    41,    42,    45,    56,
      87,   100,   169,   173,   233,   256,   312,   317,   328,   329,
     330,   333,   335,   336,   319,   330,   108,   292,    89,   206,
     185,   306,   330,     8,    22,    36,    39,    87,   187,   190,
     185,   171,     3,    17,    18,    20,    25,    34,    40,    46,
      50,    53,    63,    69,    70,    77,    86,    95,    96,    98,
     100,   102,   105,   109,   112,   115,   208,   210,    11,   108,
      78,   121,   227,   330,   227,   330,   227,   330,    27,   114,
     229,   330,    82,    85,   190,   171,   208,   208,   208,   171,
     268,   171,   208,   293,   294,    33,   194,   213,   330,   330,
      18,    77,    95,   112,   330,   330,   330,     8,   171,   218,
     217,     4,   280,   305,   330,   106,   107,   164,   330,   332,
     330,   213,   330,   330,   330,    99,   171,   185,   330,   330,
     194,   205,   330,   333,   194,   205,   330,   330,   229,   330,
     330,   330,   330,   330,   330,   330,     1,   170,   183,   195,
     305,   110,   149,   280,   307,   308,   332,   227,   305,   330,
     341,   330,    80,   185,   169,    47,    84,   113,   191,    26,
     291,   330,     8,    87,   111,   330,   331,    56,   144,   248,
     208,     1,    31,   208,   252,   254,   255,    54,    73,    83,
     275,    27,    78,    89,   108,   276,    27,    78,    89,   108,
     274,   208,   288,   293,   163,   164,   155,   330,    12,    19,
      32,    88,    92,   123,   139,   140,   142,   143,   144,   146,
     147,   148,   150,   151,   152,   153,   154,   156,   157,   158,
     159,   160,   161,   162,   165,   166,   167,   176,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   168,   266,   171,   173,    88,    92,   171,
     185,   164,   330,   330,   330,   208,   305,    56,   169,   194,
      48,   319,   290,   164,   145,   164,   187,   118,   209,   280,
     309,   310,   311,   332,   169,   208,   171,   213,    33,    48,
     213,   119,   213,   322,    33,    48,   213,   322,   213,   322,
      48,   213,   322,   208,   208,   101,   194,   101,   124,   194,
     266,   191,   309,   171,   171,   194,   185,    27,    48,    52,
      75,    78,    89,   108,   184,   269,   270,   271,   272,   273,
     276,   110,   171,   208,   295,   296,     1,   144,   300,    48,
     145,   185,   213,   171,   171,   213,   309,   219,   219,   145,
     164,   330,   330,   164,   169,   213,   171,   309,   164,   238,
     164,   238,   164,   213,   213,   164,   170,   170,   183,   145,
     170,   330,   145,   172,   145,   172,   174,   322,    48,   145,
     174,   322,   122,   145,   174,     8,     1,   170,   195,   202,
     203,   330,   197,   330,    65,    37,    72,   164,    87,   111,
     208,   208,   260,   290,   208,   305,   169,   170,     8,   255,
     124,   145,   170,    89,     1,   144,   299,    89,     1,     3,
      12,    17,    19,    20,    25,    40,    46,    53,    55,    63,
      69,    70,    86,    98,   102,   105,   109,   115,   139,   140,
     141,   142,   143,   144,   146,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   162,   165,   166,   167,
     168,   171,   207,   208,   210,   264,   265,   266,   312,   124,
     289,   164,   164,   330,   330,   330,   330,   227,   330,   227,
     330,   330,   330,   330,   330,   330,   330,     3,    20,    34,
      63,   102,   108,   209,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     330,    68,   332,   332,   332,   332,   332,   309,   309,   227,
     330,   227,   330,    27,    48,    89,   114,   321,   324,   325,
     330,   346,    33,    48,    33,    48,   101,   171,    48,   174,
     208,   227,   330,   213,   293,   330,   330,   124,   172,   145,
      48,   305,    45,   330,   227,   330,   171,   213,    45,   330,
     227,   330,   213,   213,   227,   330,   213,   124,   124,   185,
      35,   185,   330,    35,   330,    65,   172,   310,   208,   231,
     232,    48,    89,   272,   145,   172,   171,   208,   300,   296,
     145,   172,    34,    50,    96,   100,   173,   212,   301,   313,
     124,   297,   330,   294,   330,   330,   172,   280,   330,     1,
     243,   311,   172,    21,   239,   170,   172,   172,   307,   172,
     307,   185,   174,   227,   330,   174,   185,   330,   174,   330,
     174,   330,   170,   170,   145,   164,    13,   147,   145,   164,
      13,    37,    72,   208,   167,   196,   199,   330,   196,   198,
     208,   208,   248,   248,   293,   164,   171,     1,   249,   255,
     330,   253,   173,   212,   298,   313,   104,   277,   171,   267,
     330,   139,   147,   267,   267,   301,   313,   145,   171,   173,
     164,   164,   164,   164,   164,   164,   172,   174,    48,    89,
     145,   172,    17,    20,    25,    46,    53,    63,    70,    86,
      98,   109,   115,   312,    88,    88,    45,   227,   330,    45,
     227,   330,   310,   227,   330,   171,   319,   319,   164,   164,
     280,   332,   311,   330,   172,   330,    33,   213,    33,   213,
     323,   324,   330,    33,   213,   322,    33,   213,   322,   213,
     322,   213,   322,   330,   330,    35,   185,    35,    35,   185,
     101,   194,   208,   172,   145,   172,   208,   271,   296,   144,
     304,    61,   117,   282,   172,   295,   300,     1,   305,    68,
     332,   172,   172,   170,    74,   116,   170,   244,   172,   171,
     194,   208,   240,   185,   174,   322,   174,   322,   185,   122,
     202,   209,   169,   330,   110,   330,   199,   198,   145,   164,
      13,   164,   248,   248,   169,   169,   164,   310,   170,    31,
      82,    85,   170,   184,   215,   216,   254,     1,   174,   305,
     282,   270,   172,     3,   102,   265,   266,   288,   172,   174,
     324,   300,   312,   312,   330,    33,    33,   330,    33,    33,
     172,   174,   174,   310,   213,   213,   213,   101,    45,   330,
      45,   330,   145,   172,   101,    45,   330,   213,    45,   330,
     213,   213,   213,   185,   185,   330,   185,    35,   164,   164,
     232,   194,   304,   172,   173,   212,   280,   303,   313,   149,
     281,   297,     3,    91,   102,   283,   284,   285,   330,   193,
     214,   279,   297,   174,    48,   174,   171,   171,    33,   185,
     305,   240,   144,   194,    45,   185,   330,   174,    45,   185,
     330,   174,   330,   196,    13,    37,    72,    37,    72,   164,
     164,   330,   330,   169,   169,   249,   249,   172,     8,   216,
     215,   174,   298,   313,   174,   261,   172,   267,   267,   297,
     101,    45,    45,   101,    45,    45,    45,    45,   172,   330,
     330,   330,   324,   330,   330,   330,    35,   185,   281,   297,
     304,   174,   305,   280,   330,   285,   117,   145,   124,   150,
     152,   153,   156,   157,    61,   330,   301,   313,   309,   309,
     185,   213,   172,   330,   330,   185,   330,   185,   170,   110,
     330,   199,   198,   199,   198,    13,   249,   249,   170,   170,
     164,   216,   298,   313,   164,   278,   279,   330,   330,   330,
     330,   330,   330,   101,   101,   101,   101,   185,   281,   297,
     280,   302,   303,   313,    48,   174,   330,   284,   285,   285,
     285,   285,   285,   285,   283,   174,   172,   172,   194,   101,
     101,   164,   164,   164,   164,   330,   170,   170,   101,   101,
     101,   101,   101,   101,   330,   330,   330,   330,   330,   302,
     303,   313,   163,   163,   330,   330,   174,   302
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
     224,   224,   225,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   227,   228,
     228,   228,   228,   228,   228,   228,   228,   228,   228,   228,
     228,   229,   229,   230,   230,   231,   231,   232,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   234,   234,   234,   234,   234,   234,   235,   235,   235,
     236,   236,   236,   237,   237,   237,   237,   237,   237,   238,
     238,   239,   239,   239,   240,   240,   241,   242,   242,   243,
     243,   244,   244,   244,   245,   245,   246,   247,   247,   247,
     248,   248,   249,   249,   249,   250,   250,   251,   252,   252,
     253,   252,   254,   254,   254,   255,   255,   256,   257,   258,
     258,   258,   259,   261,   260,   262,   262,   262,   262,   262,
     263,   264,   264,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   265,   265,   265,   265,   265,   265,   265,   265,   265,
     265,   266,   266,   266,   266,   266,   266,   266,   266,   266,
     266,   266,   266,   267,   267,   268,   269,   269,   270,   270,
     271,   271,   271,   271,   271,   271,   272,   272,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   274,   274,   274,
     274,   274,   274,   275,   275,   275,   276,   276,   276,   276,
     276,   276,   277,   277,   278,   278,   279,   279,   280,   281,
     281,   281,   282,   282,   282,   282,   282,   283,   283,   284,
     284,   284,   284,   284,   284,   284,   285,   285,   286,   287,
     287,   287,   288,   288,   289,   289,   289,   290,   290,   290,
     290,   290,   292,   291,   291,   293,   293,   294,   294,   295,
     295,   295,   296,   296,   296,   297,   297,   297,   298,   298,
     298,   298,   298,   298,   298,   299,   299,   299,   299,   299,
     300,   300,   300,   300,   300,   301,   301,   301,   301,   302,
     302,   302,   303,   303,   303,   303,   303,   304,   304,   304,
     304,   304,   305,   305,   305,   305,   306,   306,   307,   307,
     307,   308,   308,   309,   309,   310,   310,   311,   311,   311,
     311,   312,   312,   313,   313,   313,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   316,   317,   318,   318,   318,
     318,   318,   318,   318,   318,   319,   319,   320,   321,   321,
     322,   323,   323,   324,   324,   324,   325,   325,   325,   325,
     325,   325,   326,   326,   327,   327,   327,   327,   327,   328,
     328,   328,   328,   328,   329,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   331,   331,   332,   332,   332,   333,   333,   333,
     333,   334,   334,   334,   334,   334,   335,   335,   335,   336,
     336,   336,   336,   336,   336,   337,   337,   337,   337,   338,
     338,   339,   339,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   341,   341,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   342,   342,   342,   342,   342,
     342,   343,   343,   343,   343,   343,   343,   343,   344,   344,
     344,   344,   345,   345,   345,   345,   346,   346,   346,   346,
     346,   346,   346
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
       5,     2,     1,     1,     1,     1,     7,     8,     7,     8,
       3,     5,     2,     5,     3,     3,     6,     6,     4,     5,
       5,     3,     3,     6,     5,     6,     5,     6,     3,     4,
       3,     4,     5,     6,     5,     6,     3,     4,     3,     4,
       6,     7,     6,     7,     4,     5,     4,     5,     4,     4,
       3,     6,     5,     4,     3,     6,     5,     6,     5,     8,
       7,     4,     4,     6,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     4,     7,
       5,     3,     6,     3,     3,     2,     2,     3,     3,     0,
       2,     2,     3,     5,     1,     3,     3,     5,     5,     0,
       2,     3,     2,     3,     5,     5,     2,     1,     1,     1,
       0,     2,     0,     2,     3,     3,     3,     3,     1,     2,
       0,     4,     1,     3,     2,     1,     3,     6,     0,     1,
       1,     1,     1,     0,     7,     4,     4,     6,     6,     4,
       2,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     3,     1,     3,     0,     1,
       4,     5,     4,     5,     6,     6,     0,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     1,     0,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     0,     2,     2,     4,     4,     1,     3,     3,
       3,     3,     3,     3,     3,     2,     1,     1,     3,     1,
       2,     2,     2,     4,     0,     2,     2,     1,     2,     1,
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
#line 638 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6425 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 643 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6431 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 644 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6437 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 650 "chpl.ypp"
                                      { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6443 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 656 "chpl.ypp"
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
#line 6465 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 674 "chpl.ypp"
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
#line 6486 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: deprecated_decl_stmt  */
#line 693 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6492 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: include_module_stmt  */
#line 694 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6498 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: block_stmt  */
#line 695 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6504 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: use_stmt  */
#line 696 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6510 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: import_stmt  */
#line 697 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6516 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: require_stmt  */
#line 698 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6522 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: assignment_stmt  */
#line 699 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6528 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 700 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6534 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: if_stmt  */
#line 701 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6540 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 702 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6546 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6552 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: loop_stmt  */
#line 704 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6558 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: select_stmt  */
#line 705 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6564 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: TDEFER stmt  */
#line 707 "chpl.ypp"
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
#line 6581 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: try_stmt  */
#line 719 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6587 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: throw_stmt  */
#line 720 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6593 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: return_stmt  */
#line 721 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6599 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: stmt_level_expr TSEMI  */
#line 722 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: TATOMIC stmt  */
#line 723 "chpl.ypp"
                            { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6611 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 725 "chpl.ypp"
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
#line 6629 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 739 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6642 "bison-chpl-lib.cpp"
    break;

  case 30: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 748 "chpl.ypp"
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
#line 6661 "bison-chpl-lib.cpp"
    break;

  case 31: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 763 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6674 "bison-chpl-lib.cpp"
    break;

  case 32: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 772 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6686 "bison-chpl-lib.cpp"
    break;

  case 33: /* stmt: TLABEL ident_def stmt  */
#line 780 "chpl.ypp"
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
#line 6713 "bison-chpl-lib.cpp"
    break;

  case 34: /* stmt: TLOCAL expr do_stmt  */
#line 803 "chpl.ypp"
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
#line 6729 "bison-chpl-lib.cpp"
    break;

  case 35: /* stmt: TLOCAL do_stmt  */
#line 815 "chpl.ypp"
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
#line 6744 "bison-chpl-lib.cpp"
    break;

  case 36: /* stmt: TON expr do_stmt  */
#line 826 "chpl.ypp"
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
#line 6760 "bison-chpl-lib.cpp"
    break;

  case 37: /* stmt: TSERIAL expr do_stmt  */
#line 838 "chpl.ypp"
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
#line 6776 "bison-chpl-lib.cpp"
    break;

  case 38: /* stmt: TSERIAL do_stmt  */
#line 850 "chpl.ypp"
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
#line 6791 "bison-chpl-lib.cpp"
    break;

  case 39: /* stmt: TSYNC stmt  */
#line 861 "chpl.ypp"
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
#line 6808 "bison-chpl-lib.cpp"
    break;

  case 40: /* stmt: TYIELD expr TSEMI  */
#line 874 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6819 "bison-chpl-lib.cpp"
    break;

  case 41: /* stmt: error TSEMI  */
#line 881 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
  }
#line 6827 "bison-chpl-lib.cpp"
    break;

  case 43: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL deprecated_decl_base  */
#line 889 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6833 "bison-chpl-lib.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED deprecated_decl_base  */
#line 891 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6839 "bison-chpl-lib.cpp"
    break;

  case 47: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 901 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));

      ModuleParts parts = {context->gatherComments(loc), (yyvsp[-3].visibilityTag), (yyvsp[-2].moduleKind), (yyvsp[0].uniqueStr)};
      (yyval.moduleParts) = parts;
      context->resetDeclState();
      context->clearComments();
    }
#line 6853 "bison-chpl-lib.cpp"
    break;

  case 48: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 914 "chpl.ypp"
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
#line 6870 "bison-chpl-lib.cpp"
    break;

  case 49: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 927 "chpl.ypp"
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
#line 6887 "bison-chpl-lib.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 940 "chpl.ypp"
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
#line 6902 "bison-chpl-lib.cpp"
    break;

  case 51: /* access_control: %empty  */
#line 953 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6908 "bison-chpl-lib.cpp"
    break;

  case 52: /* access_control: TPUBLIC  */
#line 954 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6915 "bison-chpl-lib.cpp"
    break;

  case 53: /* access_control: TPRIVATE  */
#line 956 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6922 "bison-chpl-lib.cpp"
    break;

  case 54: /* opt_prototype: %empty  */
#line 961 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6928 "bison-chpl-lib.cpp"
    break;

  case 55: /* opt_prototype: TPROTOTYPE  */
#line 962 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6935 "bison-chpl-lib.cpp"
    break;

  case 56: /* include_module_stmt: TINCLUDE access_control opt_prototype TMODULE ident_def TSEMI  */
#line 968 "chpl.ypp"
   {
     (yyval.commentsAndStmt) = TODOSTMT((yyloc));
     context->visibility = Decl::DEFAULT_VISIBILITY;
   }
#line 6944 "bison-chpl-lib.cpp"
    break;

  case 57: /* block_stmt_body: TLCBR TRCBR  */
#line 985 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6952 "bison-chpl-lib.cpp"
    break;

  case 58: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 989 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6960 "bison-chpl-lib.cpp"
    break;

  case 59: /* block_stmt_body: TLCBR error TRCBR  */
#line 993 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6972 "bison-chpl-lib.cpp"
    break;

  case 60: /* block_stmt: block_stmt_body  */
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
#line 6991 "bison-chpl-lib.cpp"
    break;

  case 61: /* stmt_ls: toplevel_stmt  */
#line 1022 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6997 "bison-chpl-lib.cpp"
    break;

  case 62: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1023 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7003 "bison-chpl-lib.cpp"
    break;

  case 63: /* renames_ls: expr  */
#line 1028 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 64: /* renames_ls: expr TAS expr  */
#line 1032 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7020 "bison-chpl-lib.cpp"
    break;

  case 65: /* renames_ls: renames_ls TCOMMA expr  */
#line 1037 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7028 "bison-chpl-lib.cpp"
    break;

  case 66: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1041 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7037 "bison-chpl-lib.cpp"
    break;

  case 67: /* use_renames_ls: expr  */
#line 1051 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(node);
  }
#line 7046 "bison-chpl-lib.cpp"
    break;

  case 68: /* use_renames_ls: expr TAS expr  */
#line 1056 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7056 "bison-chpl-lib.cpp"
    break;

  case 69: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1062 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7067 "bison-chpl-lib.cpp"
    break;

  case 70: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1069 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7076 "bison-chpl-lib.cpp"
    break;

  case 71: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1074 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7087 "bison-chpl-lib.cpp"
    break;

  case 72: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1081 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7099 "bison-chpl-lib.cpp"
    break;

  case 73: /* opt_only_ls: %empty  */
#line 1092 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7105 "bison-chpl-lib.cpp"
    break;

  case 74: /* opt_only_ls: renames_ls  */
#line 1093 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7111 "bison-chpl-lib.cpp"
    break;

  case 75: /* except_ls: TSTAR  */
#line 1098 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.exprList) = context->makeList(ident);
  }
#line 7120 "bison-chpl-lib.cpp"
    break;

  case 76: /* except_ls: renames_ls  */
#line 1103 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[0].exprList);
  }
#line 7128 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_stmt: access_control TUSE use_renames_ls TSEMI  */
#line 1110 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7136 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_stmt: access_control TUSE expr TEXCEPT except_ls TSEMI  */
#line 1114 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7147 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_stmt: access_control TUSE expr TAS expr TEXCEPT except_ls TSEMI  */
#line 1121 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7159 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TEXCEPT except_ls TSEMI  */
#line 1129 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7172 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1138 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7183 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1145 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7195 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1153 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7208 "bison-chpl-lib.cpp"
    break;

  case 84: /* import_stmt: access_control TIMPORT import_ls TSEMI  */
#line 1165 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7216 "bison-chpl-lib.cpp"
    break;

  case 85: /* import_expr: expr  */
#line 1172 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
  }
#line 7224 "bison-chpl-lib.cpp"
    break;

  case 86: /* import_expr: expr TAS ident_use  */
#line 1176 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as));
  }
#line 7234 "bison-chpl-lib.cpp"
    break;

  case 87: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1182 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)));
  }
#line 7244 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_ls: import_expr  */
#line 1190 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7250 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_ls: import_ls TCOMMA import_expr  */
#line 1191 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7256 "bison-chpl-lib.cpp"
    break;

  case 90: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1196 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7264 "bison-chpl-lib.cpp"
    break;

  case 91: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1203 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7272 "bison-chpl-lib.cpp"
    break;

  case 92: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1207 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7280 "bison-chpl-lib.cpp"
    break;

  case 93: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1211 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7288 "bison-chpl-lib.cpp"
    break;

  case 94: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1215 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7296 "bison-chpl-lib.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1219 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7304 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1223 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7312 "bison-chpl-lib.cpp"
    break;

  case 97: /* opt_label_ident: %empty  */
#line 1231 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7318 "bison-chpl-lib.cpp"
    break;

  case 98: /* opt_label_ident: TIDENT  */
#line 1232 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7324 "bison-chpl-lib.cpp"
    break;

  case 99: /* ident_fn_def: TIDENT  */
#line 1236 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7330 "bison-chpl-lib.cpp"
    break;

  case 100: /* ident_fn_def: TNONE  */
#line 1237 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7336 "bison-chpl-lib.cpp"
    break;

  case 101: /* ident_fn_def: TTHIS  */
#line 1238 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7342 "bison-chpl-lib.cpp"
    break;

  case 102: /* ident_fn_def: TFALSE  */
#line 1239 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7348 "bison-chpl-lib.cpp"
    break;

  case 103: /* ident_fn_def: TTRUE  */
#line 1240 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7354 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: internal_type_ident_def  */
#line 1241 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7360 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_def: TIDENT  */
#line 1244 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7366 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_def: TNONE  */
#line 1245 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7372 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_def: TTHIS  */
#line 1246 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'this'"); }
#line 7378 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_def: TFALSE  */
#line 1247 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7384 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_def: TTRUE  */
#line 1248 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7390 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: internal_type_ident_def  */
#line 1249 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7396 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_use: TIDENT  */
#line 1261 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7402 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_use: TTHIS  */
#line 1262 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7408 "bison-chpl-lib.cpp"
    break;

  case 132: /* scalar_type: TBOOL  */
#line 1295 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7414 "bison-chpl-lib.cpp"
    break;

  case 133: /* scalar_type: TENUM  */
#line 1296 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 134: /* scalar_type: TINT  */
#line 1297 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7426 "bison-chpl-lib.cpp"
    break;

  case 135: /* scalar_type: TUINT  */
#line 1298 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7432 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TREAL  */
#line 1299 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7438 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TIMAG  */
#line 1300 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7444 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TCOMPLEX  */
#line 1301 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7450 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TBYTES  */
#line 1302 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7456 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TSTRING  */
#line 1303 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7462 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TLOCALE  */
#line 1304 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TNOTHING  */
#line 1305 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7474 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TVOID  */
#line 1306 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7480 "bison-chpl-lib.cpp"
    break;

  case 148: /* do_stmt: TDO stmt  */
#line 1320 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7486 "bison-chpl-lib.cpp"
    break;

  case 149: /* do_stmt: block_stmt  */
#line 1321 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7492 "bison-chpl-lib.cpp"
    break;

  case 150: /* return_stmt: TRETURN TSEMI  */
#line 1326 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7503 "bison-chpl-lib.cpp"
    break;

  case 151: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1333 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7514 "bison-chpl-lib.cpp"
    break;

  case 153: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL class_level_stmt  */
#line 1344 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7520 "bison-chpl-lib.cpp"
    break;

  case 154: /* deprecated_class_level_stmt: TDEPRECATED class_level_stmt  */
#line 1346 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 7526 "bison-chpl-lib.cpp"
    break;

  case 155: /* class_level_stmt: TSEMI  */
#line 1351 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), nullptr);
    }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 156: /* class_level_stmt: inner_class_level_stmt  */
#line 1355 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7544 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@1: %empty  */
#line 1360 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7551 "bison-chpl-lib.cpp"
    break;

  case 158: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1362 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7560 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@2: %empty  */
#line 1366 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7567 "bison-chpl-lib.cpp"
    break;

  case 160: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1368 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 168: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1386 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-2].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr
      (yyval.commentsAndStmt) = cs;
    }
#line 7587 "bison-chpl-lib.cpp"
    break;

  case 169: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT except_ls TSEMI  */
#line 1393 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr and except_ls
      (yyval.commentsAndStmt) = cs;
    }
#line 7598 "bison-chpl-lib.cpp"
    break;

  case 170: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1400 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-4].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save expr and opt_only_ls
      (yyval.commentsAndStmt) = cs;
    }
#line 7609 "bison-chpl-lib.cpp"
    break;

  case 171: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1407 "chpl.ypp"
    {
      CommentsAndStmt cs = (yyvsp[-1].commentsAndStmt);
      //ForwardingDecl* decl = toForwardingDecl(cs.stmt);
      // TODO: save var_decl_stmt
      (yyval.commentsAndStmt) = cs;
    }
#line 7620 "bison-chpl-lib.cpp"
    break;

  case 172: /* forwarding_decl_start: TFORWARDING  */
#line 1417 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7628 "bison-chpl-lib.cpp"
    break;

  case 173: /* extern_or_export: TEXTERN  */
#line 1423 "chpl.ypp"
          { (yyval.linkageTag) = LinkageTag_EXTERN; }
#line 7634 "bison-chpl-lib.cpp"
    break;

  case 174: /* extern_or_export: TEXPORT  */
#line 1424 "chpl.ypp"
          { (yyval.linkageTag) = LinkageTag_EXPORT; }
#line 7640 "bison-chpl-lib.cpp"
    break;

  case 175: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1429 "chpl.ypp"
    {
      CommentsAndLinkage cl = {context->gatherComments((yylsp[0])), (yyvsp[0].linkageTag)};
      (yyval.commentsAndLinkage) = cl;
    }
#line 7649 "bison-chpl-lib.cpp"
    break;

  case 176: /* extern_export_decl_stmt: extern_export_decl_stmt_start TRECORD ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1437 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-6].commentsAndLinkage);
      //auto comments = context->gatherComments(@7);
      // TODO: store comments into the generated list
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7661 "bison-chpl-lib.cpp"
    break;

  case 177: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL TRECORD ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1445 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-7].commentsAndLinkage);
      //auto comments = context->gatherComments(@8);
      // TODO: store comments into the generated list
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7673 "bison-chpl-lib.cpp"
    break;

  case 178: /* extern_export_decl_stmt: extern_export_decl_stmt_start TUNION ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1454 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-6].commentsAndLinkage);
      //auto comments = context->gatherComments(@7);
      // TODO: store comments into the generated list
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7685 "bison-chpl-lib.cpp"
    break;

  case 179: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL TUNION ident_def opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1462 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-7].commentsAndLinkage);
      //auto comments = context->gatherComments(@8);
      // TODO: store comments into the generated list
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7697 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1471 "chpl.ypp"
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
#line 7721 "bison-chpl-lib.cpp"
    break;

  case 181: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1492 "chpl.ypp"
    {
      CommentsAndLinkage cl = (yyvsp[-4].commentsAndLinkage);
      context->clearComments();
      CommentsAndStmt cs = {cl.comments, TODOEXPR((yyloc))};
      (yyval.commentsAndStmt) = cs;
    }
#line 7732 "bison-chpl-lib.cpp"
    break;

  case 182: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1502 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 7740 "bison-chpl-lib.cpp"
    break;

  case 183: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1509 "chpl.ypp"
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
#line 7762 "bison-chpl-lib.cpp"
    break;

  case 184: /* loop_stmt: TWHILE expr do_stmt  */
#line 1527 "chpl.ypp"
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
#line 7779 "bison-chpl-lib.cpp"
    break;

  case 185: /* loop_stmt: TWHILE ifvar do_stmt  */
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
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 186: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1553 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7804 "bison-chpl-lib.cpp"
    break;

  case 187: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1557 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7812 "bison-chpl-lib.cpp"
    break;

  case 188: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1561 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7820 "bison-chpl-lib.cpp"
    break;

  case 189: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1565 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7828 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1569 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7836 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TFOR expr do_stmt  */
#line 1573 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7844 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1577 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7852 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1581 "chpl.ypp"
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
#line 7874 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1599 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7882 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1603 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7890 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1607 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7898 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1611 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7906 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFORALL expr do_stmt  */
#line 1615 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7914 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1619 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7922 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1623 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7930 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1627 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7938 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1631 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7946 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1635 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7954 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7962 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1643 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7970 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7978 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7986 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7994 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8002 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8010 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8018 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8026 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8034 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1679 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8042 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1683 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8050 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1687 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8059 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1692 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8068 "bison-chpl-lib.cpp"
    break;

  case 218: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1700 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8078 "bison-chpl-lib.cpp"
    break;

  case 219: /* if_stmt: TIF expr TTHEN stmt  */
#line 1709 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8086 "bison-chpl-lib.cpp"
    break;

  case 220: /* if_stmt: TIF expr block_stmt  */
#line 1713 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8094 "bison-chpl-lib.cpp"
    break;

  case 221: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1717 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8102 "bison-chpl-lib.cpp"
    break;

  case 222: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1721 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8110 "bison-chpl-lib.cpp"
    break;

  case 223: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8118 "bison-chpl-lib.cpp"
    break;

  case 224: /* if_stmt: TIF ifvar block_stmt  */
#line 1729 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8126 "bison-chpl-lib.cpp"
    break;

  case 225: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1733 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8134 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1737 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8142 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1741 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8151 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1746 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8160 "bison-chpl-lib.cpp"
    break;

  case 229: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1751 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8169 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1756 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8178 "bison-chpl-lib.cpp"
    break;

  case 231: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1764 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr),
                                   Decl::DEFAULT_VISIBILITY,
                                   Variable::VAR,
                                   /*isConfig*/ false,
                                   context->currentScopeIsAggregate(),
                                   /*typeExpression*/ nullptr,
                                   toOwned((yyvsp[0].expr)));
    (yyval.expr) = varDecl.release();
  }
#line 8193 "bison-chpl-lib.cpp"
    break;

  case 232: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1775 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr),
                                   Decl::DEFAULT_VISIBILITY,
                                   Variable::CONST,
                                   /*isConfig*/ false,
                                   context->currentScopeIsAggregate(),
                                   /*typeExpression*/ nullptr,
                                   toOwned((yyvsp[0].expr)));
    (yyval.expr) = varDecl.release();
  }
#line 8208 "bison-chpl-lib.cpp"
    break;

  case 233: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1789 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8214 "bison-chpl-lib.cpp"
    break;

  case 234: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1791 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8220 "bison-chpl-lib.cpp"
    break;

  case 235: /* ifc_formal_ls: ifc_formal  */
#line 1795 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8226 "bison-chpl-lib.cpp"
    break;

  case 236: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1796 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8232 "bison-chpl-lib.cpp"
    break;

  case 237: /* ifc_formal: ident_def  */
#line 1801 "chpl.ypp"
             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8238 "bison-chpl-lib.cpp"
    break;

  case 250: /* implements_type_ident: implements_type_error_ident  */
#line 1819 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8250 "bison-chpl-lib.cpp"
    break;

  case 257: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1848 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8256 "bison-chpl-lib.cpp"
    break;

  case 258: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1850 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8262 "bison-chpl-lib.cpp"
    break;

  case 259: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1852 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8268 "bison-chpl-lib.cpp"
    break;

  case 260: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1857 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8274 "bison-chpl-lib.cpp"
    break;

  case 261: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1859 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8280 "bison-chpl-lib.cpp"
    break;

  case 262: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1861 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8286 "bison-chpl-lib.cpp"
    break;

  case 263: /* try_stmt: TTRY expr TSEMI  */
#line 1866 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8292 "bison-chpl-lib.cpp"
    break;

  case 264: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1868 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8298 "bison-chpl-lib.cpp"
    break;

  case 265: /* try_stmt: TTRY assignment_stmt  */
#line 1870 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8304 "bison-chpl-lib.cpp"
    break;

  case 266: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1872 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8310 "bison-chpl-lib.cpp"
    break;

  case 267: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1874 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8316 "bison-chpl-lib.cpp"
    break;

  case 268: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1876 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8322 "bison-chpl-lib.cpp"
    break;

  case 269: /* catch_expr_ls: %empty  */
#line 1880 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8328 "bison-chpl-lib.cpp"
    break;

  case 270: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1881 "chpl.ypp"
                           { context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8334 "bison-chpl-lib.cpp"
    break;

  case 271: /* catch_expr: TCATCH block_stmt  */
#line 1885 "chpl.ypp"
                                             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8340 "bison-chpl-lib.cpp"
    break;

  case 272: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1886 "chpl.ypp"
                                             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8346 "bison-chpl-lib.cpp"
    break;

  case 273: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1887 "chpl.ypp"
                                             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8352 "bison-chpl-lib.cpp"
    break;

  case 274: /* catch_expr_inner: ident_def  */
#line 1891 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8358 "bison-chpl-lib.cpp"
    break;

  case 275: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1892 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8364 "bison-chpl-lib.cpp"
    break;

  case 276: /* throw_stmt: TTHROW expr TSEMI  */
#line 1897 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8370 "bison-chpl-lib.cpp"
    break;

  case 277: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1901 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8376 "bison-chpl-lib.cpp"
    break;

  case 278: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1903 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8382 "bison-chpl-lib.cpp"
    break;

  case 279: /* when_stmt_ls: %empty  */
#line 1907 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8388 "bison-chpl-lib.cpp"
    break;

  case 280: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1908 "chpl.ypp"
                          { context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8394 "bison-chpl-lib.cpp"
    break;

  case 281: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1913 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8400 "bison-chpl-lib.cpp"
    break;

  case 282: /* when_stmt: TOTHERWISE stmt  */
#line 1915 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8406 "bison-chpl-lib.cpp"
    break;

  case 283: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1917 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8412 "bison-chpl-lib.cpp"
    break;

  case 284: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1924 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8421 "bison-chpl-lib.cpp"
    break;

  case 285: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1929 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 286: /* class_start: class_tag ident_def  */
#line 1940 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));
      context->enterScope((yyvsp[-1].astTag), (yyvsp[0].uniqueStr));

      TypeDeclParts parts = {context->gatherComments(loc),
                             context->visibility,
                             (yyvsp[0].uniqueStr),
                             (yyvsp[-1].astTag)};
      (yyval.typeDeclParts) = parts;

      context->clearComments();
    }
#line 8450 "bison-chpl-lib.cpp"
    break;

  case 287: /* class_tag: TCLASS  */
#line 1956 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8456 "bison-chpl-lib.cpp"
    break;

  case 288: /* class_tag: TRECORD  */
#line 1957 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8462 "bison-chpl-lib.cpp"
    break;

  case 289: /* class_tag: TUNION  */
#line 1958 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8468 "bison-chpl-lib.cpp"
    break;

  case 290: /* opt_inherit: %empty  */
#line 1962 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8474 "bison-chpl-lib.cpp"
    break;

  case 291: /* opt_inherit: TCOLON expr_ls  */
#line 1963 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8480 "bison-chpl-lib.cpp"
    break;

  case 292: /* class_level_stmt_ls: %empty  */
#line 1968 "chpl.ypp"
    { (yyval.exprList) = context->makeList(); }
#line 8486 "bison-chpl-lib.cpp"
    break;

  case 293: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 1970 "chpl.ypp"
    { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8492 "bison-chpl-lib.cpp"
    break;

  case 294: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 1972 "chpl.ypp"
    { context->appendList((yyvsp[-2].exprList), TODOEXPR((yyloc))); }
#line 8498 "bison-chpl-lib.cpp"
    break;

  case 295: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 1977 "chpl.ypp"
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
#line 8518 "bison-chpl-lib.cpp"
    break;

  case 296: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 1993 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8532 "bison-chpl-lib.cpp"
    break;

  case 297: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2006 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-2]));
      context->enterScope(asttags::Enum, (yyvsp[-1].uniqueStr));

      TypeDeclParts parts = {context->gatherComments(loc),
                             context->visibility,
                             (yyvsp[-1].uniqueStr),
                             asttags::Enum};
      (yyval.typeDeclParts) = parts;

      context->clearComments();
    }
#line 8550 "bison-chpl-lib.cpp"
    break;

  case 298: /* enum_ls: deprecated_enum_item  */
#line 2023 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 8558 "bison-chpl-lib.cpp"
    break;

  case 299: /* enum_ls: enum_ls TCOMMA  */
#line 2027 "chpl.ypp"
    {
      (yyval.exprList) = (yyvsp[-1].exprList);
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8567 "bison-chpl-lib.cpp"
    break;

  case 300: /* $@3: %empty  */
#line 2031 "chpl.ypp"
                 {context->clearCommentsBefore((yylsp[0]));}
#line 8573 "bison-chpl-lib.cpp"
    break;

  case 301: /* enum_ls: enum_ls TCOMMA $@3 deprecated_enum_item  */
#line 2032 "chpl.ypp"
    {
      context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 8581 "bison-chpl-lib.cpp"
    break;

  case 303: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL enum_item  */
#line 2040 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8587 "bison-chpl-lib.cpp"
    break;

  case 304: /* deprecated_enum_item: TDEPRECATED enum_item  */
#line 2042 "chpl.ypp"
{ (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8593 "bison-chpl-lib.cpp"
    break;

  case 305: /* enum_item: ident_def  */
#line 2047 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)), (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 306: /* enum_item: ident_def TASSIGN expr  */
#line 2052 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8612 "bison-chpl-lib.cpp"
    break;

  case 307: /* lambda_decl_expr: TLAMBDA req_formal_ls opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 2061 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 309: /* linkage_spec: linkage_spec_empty  */
#line 2070 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8626 "bison-chpl-lib.cpp"
    break;

  case 310: /* linkage_spec: TINLINE  */
#line 2071 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8633 "bison-chpl-lib.cpp"
    break;

  case 311: /* linkage_spec: TOVERRIDE  */
#line 2073 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8640 "bison-chpl-lib.cpp"
    break;

  case 312: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2079 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8648 "bison-chpl-lib.cpp"
    break;

  case 313: /* $@4: %empty  */
#line 2088 "chpl.ypp"
    {
      context->clearComments();
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name);
    }
#line 8657 "bison-chpl-lib.cpp"
    break;

  case 314: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@4 opt_function_body_stmt  */
#line 2093 "chpl.ypp"
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
#line 8678 "bison-chpl-lib.cpp"
    break;

  case 315: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2113 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8690 "bison-chpl-lib.cpp"
    break;

  case 316: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2121 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8702 "bison-chpl-lib.cpp"
    break;

  case 317: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2129 "chpl.ypp"
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
#line 8717 "bison-chpl-lib.cpp"
    break;

  case 318: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2140 "chpl.ypp"
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
#line 8732 "bison-chpl-lib.cpp"
    break;

  case 319: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2151 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8742 "bison-chpl-lib.cpp"
    break;

  case 320: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2160 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-1].functionParts);
      auto loc = context->declStartLoc((yylsp[0]));
      fp.comments = context->gatherComments(loc);
      fp.kind = (yyvsp[0].functionKind);
      (yyval.functionParts) = fp;
    }
#line 8754 "bison-chpl-lib.cpp"
    break;

  case 322: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2171 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8760 "bison-chpl-lib.cpp"
    break;

  case 350: /* fn_ident: ident_def TBANG  */
#line 2203 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8770 "bison-chpl-lib.cpp"
    break;

  case 363: /* opt_formal_ls: %empty  */
#line 2226 "chpl.ypp"
                     { (yyval.exprList) = nullptr; /* TODO: mark as no-parens */ }
#line 8776 "bison-chpl-lib.cpp"
    break;

  case 364: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2227 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8782 "bison-chpl-lib.cpp"
    break;

  case 365: /* req_formal_ls: TLP formal_ls TRP  */
#line 2231 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 366: /* formal_ls_inner: formal  */
#line 2235 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 367: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2236 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 368: /* formal_ls: %empty  */
#line 2240 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8806 "bison-chpl-lib.cpp"
    break;

  case 369: /* formal_ls: formal_ls_inner  */
#line 2241 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 370: /* formal: opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2246 "chpl.ypp"
    { (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), (yyvsp[-3].intentTag),
                         toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr))).release(); }
#line 8819 "bison-chpl-lib.cpp"
    break;

  case 371: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2249 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8825 "bison-chpl-lib.cpp"
    break;

  case 372: /* formal: opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 2251 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8831 "bison-chpl-lib.cpp"
    break;

  case 373: /* formal: pragma_ls opt_intent_tag ident_def opt_formal_type var_arg_expr  */
#line 2253 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8837 "bison-chpl-lib.cpp"
    break;

  case 374: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2255 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8843 "bison-chpl-lib.cpp"
    break;

  case 375: /* formal: opt_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type var_arg_expr  */
#line 2257 "chpl.ypp"
    {
      (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
    }
#line 8851 "bison-chpl-lib.cpp"
    break;

  case 376: /* opt_intent_tag: %empty  */
#line 2263 "chpl.ypp"
                      { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8857 "bison-chpl-lib.cpp"
    break;

  case 377: /* opt_intent_tag: required_intent_tag  */
#line 2264 "chpl.ypp"
                      { (yyval.intentTag) = (yyvsp[0].intentTag); }
#line 8863 "bison-chpl-lib.cpp"
    break;

  case 378: /* required_intent_tag: TIN  */
#line 2268 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8869 "bison-chpl-lib.cpp"
    break;

  case 379: /* required_intent_tag: TINOUT  */
#line 2269 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8875 "bison-chpl-lib.cpp"
    break;

  case 380: /* required_intent_tag: TOUT  */
#line 2270 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8881 "bison-chpl-lib.cpp"
    break;

  case 381: /* required_intent_tag: TCONST TIN  */
#line 2271 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8887 "bison-chpl-lib.cpp"
    break;

  case 382: /* required_intent_tag: TCONST TREF  */
#line 2272 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8893 "bison-chpl-lib.cpp"
    break;

  case 383: /* required_intent_tag: TCONST  */
#line 2273 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8899 "bison-chpl-lib.cpp"
    break;

  case 384: /* required_intent_tag: TPARAM  */
#line 2274 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8905 "bison-chpl-lib.cpp"
    break;

  case 385: /* required_intent_tag: TREF  */
#line 2275 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 8911 "bison-chpl-lib.cpp"
    break;

  case 386: /* required_intent_tag: TTYPE  */
#line 2276 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 8917 "bison-chpl-lib.cpp"
    break;

  case 387: /* opt_this_intent_tag: %empty  */
#line 2280 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 8923 "bison-chpl-lib.cpp"
    break;

  case 388: /* opt_this_intent_tag: TPARAM  */
#line 2281 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 8929 "bison-chpl-lib.cpp"
    break;

  case 389: /* opt_this_intent_tag: TREF  */
#line 2282 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 8935 "bison-chpl-lib.cpp"
    break;

  case 390: /* opt_this_intent_tag: TCONST TREF  */
#line 2283 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 8941 "bison-chpl-lib.cpp"
    break;

  case 391: /* opt_this_intent_tag: TCONST  */
#line 2284 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 8947 "bison-chpl-lib.cpp"
    break;

  case 392: /* opt_this_intent_tag: TTYPE  */
#line 2285 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 8953 "bison-chpl-lib.cpp"
    break;

  case 393: /* proc_iter_or_op: TPROC  */
#line 2289 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 8959 "bison-chpl-lib.cpp"
    break;

  case 394: /* proc_iter_or_op: TITER  */
#line 2290 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 8965 "bison-chpl-lib.cpp"
    break;

  case 395: /* proc_iter_or_op: TOPERATOR  */
#line 2291 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 8971 "bison-chpl-lib.cpp"
    break;

  case 396: /* opt_ret_tag: %empty  */
#line 2295 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 8977 "bison-chpl-lib.cpp"
    break;

  case 397: /* opt_ret_tag: TCONST  */
#line 2296 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 8983 "bison-chpl-lib.cpp"
    break;

  case 398: /* opt_ret_tag: TCONST TREF  */
#line 2297 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 8989 "bison-chpl-lib.cpp"
    break;

  case 399: /* opt_ret_tag: TREF  */
#line 2298 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 8995 "bison-chpl-lib.cpp"
    break;

  case 400: /* opt_ret_tag: TPARAM  */
#line 2299 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9001 "bison-chpl-lib.cpp"
    break;

  case 401: /* opt_ret_tag: TTYPE  */
#line 2300 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9007 "bison-chpl-lib.cpp"
    break;

  case 402: /* opt_throws_error: %empty  */
#line 2304 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9013 "bison-chpl-lib.cpp"
    break;

  case 403: /* opt_throws_error: TTHROWS  */
#line 2305 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9019 "bison-chpl-lib.cpp"
    break;

  case 404: /* opt_function_body_stmt: TSEMI  */
#line 2308 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9025 "bison-chpl-lib.cpp"
    break;

  case 405: /* opt_function_body_stmt: function_body_stmt  */
#line 2309 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9031 "bison-chpl-lib.cpp"
    break;

  case 406: /* function_body_stmt: block_stmt_body  */
#line 2313 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9037 "bison-chpl-lib.cpp"
    break;

  case 407: /* function_body_stmt: return_stmt  */
#line 2314 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9043 "bison-chpl-lib.cpp"
    break;

  case 408: /* query_expr: TQUERIEDIDENT  */
#line 2318 "chpl.ypp"
                      { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9049 "bison-chpl-lib.cpp"
    break;

  case 409: /* var_arg_expr: TDOTDOTDOT  */
#line 2322 "chpl.ypp"
                         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9055 "bison-chpl-lib.cpp"
    break;

  case 410: /* var_arg_expr: TDOTDOTDOT expr  */
#line 2323 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9061 "bison-chpl-lib.cpp"
    break;

  case 411: /* var_arg_expr: TDOTDOTDOT query_expr  */
#line 2324 "chpl.ypp"
                         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9067 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_lifetime_where: %empty  */
#line 2328 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9073 "bison-chpl-lib.cpp"
    break;

  case 413: /* opt_lifetime_where: TWHERE expr  */
#line 2330 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9079 "bison-chpl-lib.cpp"
    break;

  case 414: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2332 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9085 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2334 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9091 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2336 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9097 "bison-chpl-lib.cpp"
    break;

  case 417: /* lifetime_components_expr: lifetime_expr  */
#line 2341 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9103 "bison-chpl-lib.cpp"
    break;

  case 418: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2343 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9109 "bison-chpl-lib.cpp"
    break;

  case 419: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2348 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9115 "bison-chpl-lib.cpp"
    break;

  case 420: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2350 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9121 "bison-chpl-lib.cpp"
    break;

  case 421: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2352 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9127 "bison-chpl-lib.cpp"
    break;

  case 422: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2354 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9133 "bison-chpl-lib.cpp"
    break;

  case 423: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2356 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9139 "bison-chpl-lib.cpp"
    break;

  case 424: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2358 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9145 "bison-chpl-lib.cpp"
    break;

  case 425: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2360 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9151 "bison-chpl-lib.cpp"
    break;

  case 426: /* lifetime_ident: TIDENT  */
#line 2364 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9157 "bison-chpl-lib.cpp"
    break;

  case 427: /* lifetime_ident: TTHIS  */
#line 2365 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9163 "bison-chpl-lib.cpp"
    break;

  case 428: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner TSEMI  */
#line 2370 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[-2].commentsAndStmt)); // TODO
    }
#line 9171 "bison-chpl-lib.cpp"
    break;

  case 429: /* type_alias_decl_stmt_start: TTYPE  */
#line 2377 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 9179 "bison-chpl-lib.cpp"
    break;

  case 430: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2381 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 9187 "bison-chpl-lib.cpp"
    break;

  case 431: /* type_alias_decl_stmt_start: TEXTERN TTYPE  */
#line 2386 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = TODOSTMT((yyloc));
    }
#line 9195 "bison-chpl-lib.cpp"
    break;

  case 432: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2394 "chpl.ypp"
    {
      // set name to $1
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 433: /* type_alias_decl_stmt_inner: ident_def opt_init_type TCOMMA type_alias_decl_stmt_inner  */
#line 2399 "chpl.ypp"
    {
      // set name to $1
      // add new var to multivar
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9214 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_init_type: %empty  */
#line 2407 "chpl.ypp"
    { (yyval.expr) = nullptr; }
#line 9220 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_init_type: TASSIGN type_level_expr  */
#line 2409 "chpl.ypp"
    { (yyval.expr) = (yyvsp[0].expr); }
#line 9226 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_init_type: TASSIGN array_type  */
#line 2411 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9232 "bison-chpl-lib.cpp"
    break;

  case 437: /* var_decl_type: TPARAM  */
#line 2415 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9238 "bison-chpl-lib.cpp"
    break;

  case 438: /* var_decl_type: TCONST TREF  */
#line 2416 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9244 "bison-chpl-lib.cpp"
    break;

  case 439: /* var_decl_type: TREF  */
#line 2417 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9250 "bison-chpl-lib.cpp"
    break;

  case 440: /* var_decl_type: TCONST  */
#line 2418 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9256 "bison-chpl-lib.cpp"
    break;

  case 441: /* var_decl_type: TVAR  */
#line 2419 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9262 "bison-chpl-lib.cpp"
    break;

  case 442: /* $@5: %empty  */
#line 2424 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9271 "bison-chpl-lib.cpp"
    break;

  case 443: /* var_decl_stmt: TCONFIG $@5 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2428 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDecl((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9280 "bison-chpl-lib.cpp"
    break;

  case 444: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2433 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDecl((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9289 "bison-chpl-lib.cpp"
    break;

  case 445: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2441 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9297 "bison-chpl-lib.cpp"
    break;

  case 446: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2445 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 447: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2452 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     (yyvsp[-2].uniqueStr), context->visibility,
                                     context->varDeclKind,
                                     context->isVarDeclConfig,
                                     context->currentScopeIsAggregate(),
                                     toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr)));
      // gather the comments like a statement does
      (yyval.commentsAndStmt) = STMT((yylsp[-2]), varDecl.release());
      context->clearComments();
    }
#line 9321 "bison-chpl-lib.cpp"
    break;

  case 448: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2464 "chpl.ypp"
    {
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        context->visibility,
                                        context->varDeclKind,
                                        context->consumeList((yyvsp[-3].exprList)),
                                        toOwned((yyvsp[-1].expr)), toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yylsp[-4]), tupleDecl.release());
      context->clearComments();
    }
#line 9335 "bison-chpl-lib.cpp"
    break;

  case 449: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2477 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     (yyvsp[0].uniqueStr), context->visibility,
                                     context->varDeclKind,
                                     context->isVarDeclConfig,
                                     context->currentScopeIsAggregate(),
                                     nullptr, nullptr);
      (yyval.expr) = varDecl.release();
    }
#line 9349 "bison-chpl-lib.cpp"
    break;

  case 450: /* tuple_var_decl_component: ident_def  */
#line 2488 "chpl.ypp"
    {
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     (yyvsp[0].uniqueStr), context->visibility,
                                     context->varDeclKind,
                                     context->isVarDeclConfig,
                                     context->currentScopeIsAggregate(),
                                     nullptr, nullptr);
      (yyval.expr) = varDecl.release();
    }
#line 9363 "bison-chpl-lib.cpp"
    break;

  case 451: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2499 "chpl.ypp"
    {
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        context->visibility,
                                        context->varDeclKind,
                                        context->consumeList((yyvsp[-1].exprList)),
                                        nullptr, nullptr);
      (yyval.expr) = tupleDecl.release();
    }
#line 9376 "bison-chpl-lib.cpp"
    break;

  case 452: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2511 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9382 "bison-chpl-lib.cpp"
    break;

  case 453: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2513 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9388 "bison-chpl-lib.cpp"
    break;

  case 454: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2515 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9394 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_init_expr: %empty  */
#line 2521 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9400 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2522 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9406 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2523 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9412 "bison-chpl-lib.cpp"
    break;

  case 458: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2529 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9420 "bison-chpl-lib.cpp"
    break;

  case 459: /* ret_array_type: TLSBR TRSBR  */
#line 2533 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9428 "bison-chpl-lib.cpp"
    break;

  case 460: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2537 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9436 "bison-chpl-lib.cpp"
    break;

  case 461: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2541 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9444 "bison-chpl-lib.cpp"
    break;

  case 462: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2545 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9452 "bison-chpl-lib.cpp"
    break;

  case 463: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2549 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9460 "bison-chpl-lib.cpp"
    break;

  case 464: /* ret_array_type: TLSBR error TRSBR  */
#line 2553 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9468 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_ret_type: %empty  */
#line 2559 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9474 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_ret_type: TCOLON type_level_expr  */
#line 2560 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9480 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_ret_type: TCOLON ret_array_type  */
#line 2561 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9486 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2562 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9492 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_ret_type: error  */
#line 2563 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9498 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_type: %empty  */
#line 2568 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9504 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_type: TCOLON type_level_expr  */
#line 2569 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9510 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_type: TCOLON array_type  */
#line 2570 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9516 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2571 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9522 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_type: error  */
#line 2572 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9528 "bison-chpl-lib.cpp"
    break;

  case 475: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2593 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9536 "bison-chpl-lib.cpp"
    break;

  case 476: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2597 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9544 "bison-chpl-lib.cpp"
    break;

  case 477: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2601 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 9552 "bison-chpl-lib.cpp"
    break;

  case 478: /* array_type: TLSBR error TRSBR  */
#line 2605 "chpl.ypp"
    {
      (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
    }
#line 9560 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_formal_array_elt_type: %empty  */
#line 2611 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9566 "bison-chpl-lib.cpp"
    break;

  case 480: /* opt_formal_array_elt_type: type_level_expr  */
#line 2612 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 481: /* opt_formal_array_elt_type: query_expr  */
#line 2613 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9578 "bison-chpl-lib.cpp"
    break;

  case 482: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2618 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9584 "bison-chpl-lib.cpp"
    break;

  case 483: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2620 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9590 "bison-chpl-lib.cpp"
    break;

  case 484: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2626 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9596 "bison-chpl-lib.cpp"
    break;

  case 485: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2628 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9602 "bison-chpl-lib.cpp"
    break;

  case 486: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2630 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9608 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_formal_type: %empty  */
#line 2634 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9614 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_formal_type: TCOLON type_level_expr  */
#line 2635 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9620 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_formal_type: TCOLON query_expr  */
#line 2636 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9626 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2637 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_formal_type: TCOLON formal_array_type  */
#line 2638 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9638 "bison-chpl-lib.cpp"
    break;

  case 492: /* expr_ls: expr  */
#line 2644 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9644 "bison-chpl-lib.cpp"
    break;

  case 493: /* expr_ls: query_expr  */
#line 2645 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9650 "bison-chpl-lib.cpp"
    break;

  case 494: /* expr_ls: expr_ls TCOMMA expr  */
#line 2646 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 495: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2647 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9662 "bison-chpl-lib.cpp"
    break;

  case 496: /* simple_expr_ls: expr  */
#line 2651 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9668 "bison-chpl-lib.cpp"
    break;

  case 497: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2652 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 498: /* tuple_component: TUNDERSCORE  */
#line 2656 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9680 "bison-chpl-lib.cpp"
    break;

  case 499: /* tuple_component: opt_try_expr  */
#line 2657 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9686 "bison-chpl-lib.cpp"
    break;

  case 500: /* tuple_component: query_expr  */
#line 2658 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9692 "bison-chpl-lib.cpp"
    break;

  case 501: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2662 "chpl.ypp"
                                         { (yyval.exprList) = TODOLIST((yyloc)); }
#line 9698 "bison-chpl-lib.cpp"
    break;

  case 502: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2663 "chpl.ypp"
                                         { (yyval.exprList) = TODOLIST((yyloc)); }
#line 9704 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_actual_ls: %empty  */
#line 2667 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_actual_ls: actual_ls  */
#line 2668 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9716 "bison-chpl-lib.cpp"
    break;

  case 505: /* actual_ls: actual_expr  */
#line 2673 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9725 "bison-chpl-lib.cpp"
    break;

  case 506: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2678 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9735 "bison-chpl-lib.cpp"
    break;

  case 507: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2686 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9741 "bison-chpl-lib.cpp"
    break;

  case 508: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2687 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9747 "bison-chpl-lib.cpp"
    break;

  case 509: /* actual_expr: query_expr  */
#line 2688 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 510: /* actual_expr: opt_try_expr  */
#line 2689 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9759 "bison-chpl-lib.cpp"
    break;

  case 511: /* ident_expr: ident_use  */
#line 2693 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9765 "bison-chpl-lib.cpp"
    break;

  case 512: /* ident_expr: scalar_type  */
#line 2694 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9771 "bison-chpl-lib.cpp"
    break;

  case 513: /* type_level_expr: sub_type_level_expr  */
#line 2706 "chpl.ypp"
    { (yyval.expr) = (yyvsp[0].expr); }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 514: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2708 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9783 "bison-chpl-lib.cpp"
    break;

  case 515: /* type_level_expr: TQUESTION  */
#line 2710 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 521: /* sub_type_level_expr: TSINGLE expr  */
#line 2721 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9795 "bison-chpl-lib.cpp"
    break;

  case 522: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2723 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9801 "bison-chpl-lib.cpp"
    break;

  case 523: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2725 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9807 "bison-chpl-lib.cpp"
    break;

  case 524: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2727 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9813 "bison-chpl-lib.cpp"
    break;

  case 525: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2729 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9819 "bison-chpl-lib.cpp"
    break;

  case 526: /* sub_type_level_expr: TATOMIC expr  */
#line 2731 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9825 "bison-chpl-lib.cpp"
    break;

  case 527: /* sub_type_level_expr: TSYNC expr  */
#line 2733 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 528: /* sub_type_level_expr: TOWNED  */
#line 2736 "chpl.ypp"
    { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9837 "bison-chpl-lib.cpp"
    break;

  case 529: /* sub_type_level_expr: TOWNED expr  */
#line 2738 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9843 "bison-chpl-lib.cpp"
    break;

  case 530: /* sub_type_level_expr: TUNMANAGED  */
#line 2740 "chpl.ypp"
    { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9849 "bison-chpl-lib.cpp"
    break;

  case 531: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2742 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9855 "bison-chpl-lib.cpp"
    break;

  case 532: /* sub_type_level_expr: TSHARED  */
#line 2744 "chpl.ypp"
    { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9861 "bison-chpl-lib.cpp"
    break;

  case 533: /* sub_type_level_expr: TSHARED expr  */
#line 2746 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9867 "bison-chpl-lib.cpp"
    break;

  case 534: /* sub_type_level_expr: TBORROWED  */
#line 2748 "chpl.ypp"
    { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9873 "bison-chpl-lib.cpp"
    break;

  case 535: /* sub_type_level_expr: TBORROWED expr  */
#line 2750 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9879 "bison-chpl-lib.cpp"
    break;

  case 536: /* sub_type_level_expr: TCLASS  */
#line 2753 "chpl.ypp"
    { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9885 "bison-chpl-lib.cpp"
    break;

  case 537: /* sub_type_level_expr: TRECORD  */
#line 2755 "chpl.ypp"
    { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9891 "bison-chpl-lib.cpp"
    break;

  case 538: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2760 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9897 "bison-chpl-lib.cpp"
    break;

  case 539: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2762 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9903 "bison-chpl-lib.cpp"
    break;

  case 540: /* for_expr: TFOR expr TDO expr  */
#line 2764 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9909 "bison-chpl-lib.cpp"
    break;

  case 541: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2766 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9915 "bison-chpl-lib.cpp"
    break;

  case 542: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2768 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9921 "bison-chpl-lib.cpp"
    break;

  case 543: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2770 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9927 "bison-chpl-lib.cpp"
    break;

  case 544: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2772 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9933 "bison-chpl-lib.cpp"
    break;

  case 545: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2774 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9939 "bison-chpl-lib.cpp"
    break;

  case 546: /* for_expr: TFORALL expr TDO expr  */
#line 2776 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9945 "bison-chpl-lib.cpp"
    break;

  case 547: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 2778 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9951 "bison-chpl-lib.cpp"
    break;

  case 548: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2780 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9957 "bison-chpl-lib.cpp"
    break;

  case 549: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 2782 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9963 "bison-chpl-lib.cpp"
    break;

  case 550: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 2784 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9969 "bison-chpl-lib.cpp"
    break;

  case 551: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 2786 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9975 "bison-chpl-lib.cpp"
    break;

  case 552: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 2788 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9981 "bison-chpl-lib.cpp"
    break;

  case 553: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 2790 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9987 "bison-chpl-lib.cpp"
    break;

  case 554: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 2792 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9993 "bison-chpl-lib.cpp"
    break;

  case 555: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 2797 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9999 "bison-chpl-lib.cpp"
    break;

  case 556: /* nil_expr: TNIL  */
#line 2806 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10005 "bison-chpl-lib.cpp"
    break;

  case 564: /* stmt_level_expr: io_expr TIO expr  */
#line 2822 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10011 "bison-chpl-lib.cpp"
    break;

  case 565: /* opt_task_intent_ls: %empty  */
#line 2826 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10017 "bison-chpl-lib.cpp"
    break;

  case 566: /* opt_task_intent_ls: task_intent_clause  */
#line 2827 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10023 "bison-chpl-lib.cpp"
    break;

  case 567: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 2832 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10033 "bison-chpl-lib.cpp"
    break;

  case 568: /* task_intent_ls: intent_expr  */
#line 2840 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10039 "bison-chpl-lib.cpp"
    break;

  case 569: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 2841 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10045 "bison-chpl-lib.cpp"
    break;

  case 570: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 2846 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10055 "bison-chpl-lib.cpp"
    break;

  case 571: /* forall_intent_ls: intent_expr  */
#line 2854 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10061 "bison-chpl-lib.cpp"
    break;

  case 572: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 2855 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10067 "bison-chpl-lib.cpp"
    break;

  case 573: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 2860 "chpl.ypp"
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
#line 10084 "bison-chpl-lib.cpp"
    break;

  case 574: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 2873 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 10092 "bison-chpl-lib.cpp"
    break;

  case 575: /* intent_expr: expr TREDUCE ident_expr  */
#line 2877 "chpl.ypp"
    {
      (yyval.expr) = TODOEXPR((yyloc));
    }
#line 10100 "bison-chpl-lib.cpp"
    break;

  case 576: /* task_var_prefix: TCONST  */
#line 2883 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10106 "bison-chpl-lib.cpp"
    break;

  case 577: /* task_var_prefix: TIN  */
#line 2884 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10112 "bison-chpl-lib.cpp"
    break;

  case 578: /* task_var_prefix: TCONST TIN  */
#line 2885 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10118 "bison-chpl-lib.cpp"
    break;

  case 579: /* task_var_prefix: TREF  */
#line 2886 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10124 "bison-chpl-lib.cpp"
    break;

  case 580: /* task_var_prefix: TCONST TREF  */
#line 2887 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10130 "bison-chpl-lib.cpp"
    break;

  case 581: /* task_var_prefix: TVAR  */
#line 2888 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10136 "bison-chpl-lib.cpp"
    break;

  case 583: /* io_expr: io_expr TIO expr  */
#line 2894 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10142 "bison-chpl-lib.cpp"
    break;

  case 584: /* new_maybe_decorated: TNEW  */
#line 2899 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10148 "bison-chpl-lib.cpp"
    break;

  case 585: /* new_maybe_decorated: TNEW TOWNED  */
#line 2901 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10154 "bison-chpl-lib.cpp"
    break;

  case 586: /* new_maybe_decorated: TNEW TSHARED  */
#line 2903 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10160 "bison-chpl-lib.cpp"
    break;

  case 587: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 2905 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10166 "bison-chpl-lib.cpp"
    break;

  case 588: /* new_maybe_decorated: TNEW TBORROWED  */
#line 2907 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10172 "bison-chpl-lib.cpp"
    break;

  case 589: /* new_expr: new_maybe_decorated expr  */
#line 2913 "chpl.ypp"
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
#line 10190 "bison-chpl-lib.cpp"
    break;

  case 590: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2930 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10198 "bison-chpl-lib.cpp"
    break;

  case 591: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 2934 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10206 "bison-chpl-lib.cpp"
    break;

  case 592: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2938 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10214 "bison-chpl-lib.cpp"
    break;

  case 593: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 2942 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10222 "bison-chpl-lib.cpp"
    break;

  case 594: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 2949 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10228 "bison-chpl-lib.cpp"
    break;

  case 604: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 2966 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10234 "bison-chpl-lib.cpp"
    break;

  case 605: /* expr: expr TCOLON expr  */
#line 2968 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 606: /* expr: expr TDOTDOT expr  */
#line 2972 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10251 "bison-chpl-lib.cpp"
    break;

  case 607: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 2977 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10260 "bison-chpl-lib.cpp"
    break;

  case 608: /* expr: expr TDOTDOT  */
#line 2982 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10269 "bison-chpl-lib.cpp"
    break;

  case 609: /* expr: TDOTDOT expr  */
#line 2987 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10279 "bison-chpl-lib.cpp"
    break;

  case 610: /* expr: TDOTDOTOPENHIGH expr  */
#line 2993 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10289 "bison-chpl-lib.cpp"
    break;

  case 611: /* expr: TDOTDOT  */
#line 2999 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10299 "bison-chpl-lib.cpp"
    break;

  case 612: /* opt_expr: %empty  */
#line 3030 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10305 "bison-chpl-lib.cpp"
    break;

  case 613: /* opt_expr: expr  */
#line 3031 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10311 "bison-chpl-lib.cpp"
    break;

  case 614: /* opt_try_expr: TTRY expr  */
#line 3035 "chpl.ypp"
                  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10317 "bison-chpl-lib.cpp"
    break;

  case 615: /* opt_try_expr: TTRYBANG expr  */
#line 3036 "chpl.ypp"
                  { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10323 "bison-chpl-lib.cpp"
    break;

  case 616: /* opt_try_expr: expr  */
#line 3037 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10329 "bison-chpl-lib.cpp"
    break;

  case 622: /* call_base_expr: expr TBANG  */
#line 3054 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10335 "bison-chpl-lib.cpp"
    break;

  case 623: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3055 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10341 "bison-chpl-lib.cpp"
    break;

  case 626: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3062 "chpl.ypp"
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
#line 10357 "bison-chpl-lib.cpp"
    break;

  case 627: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3074 "chpl.ypp"
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
#line 10373 "bison-chpl-lib.cpp"
    break;

  case 628: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3085 "chpl.ypp"
                                              { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10379 "bison-chpl-lib.cpp"
    break;

  case 629: /* dot_expr: expr TDOT ident_use  */
#line 3090 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10385 "bison-chpl-lib.cpp"
    break;

  case 630: /* dot_expr: expr TDOT TTYPE  */
#line 3092 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10391 "bison-chpl-lib.cpp"
    break;

  case 631: /* dot_expr: expr TDOT TDOMAIN  */
#line 3094 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10397 "bison-chpl-lib.cpp"
    break;

  case 632: /* dot_expr: expr TDOT TLOCALE  */
#line 3096 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10403 "bison-chpl-lib.cpp"
    break;

  case 633: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3098 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10413 "bison-chpl-lib.cpp"
    break;

  case 634: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3104 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10423 "bison-chpl-lib.cpp"
    break;

  case 635: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3116 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10429 "bison-chpl-lib.cpp"
    break;

  case 636: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3117 "chpl.ypp"
                                    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10435 "bison-chpl-lib.cpp"
    break;

  case 637: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3118 "chpl.ypp"
                                    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10441 "bison-chpl-lib.cpp"
    break;

  case 638: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3119 "chpl.ypp"
                                    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10447 "bison-chpl-lib.cpp"
    break;

  case 639: /* bool_literal: TFALSE  */
#line 3123 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10453 "bison-chpl-lib.cpp"
    break;

  case 640: /* bool_literal: TTRUE  */
#line 3124 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10459 "bison-chpl-lib.cpp"
    break;

  case 641: /* str_bytes_literal: STRINGLITERAL  */
#line 3128 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10465 "bison-chpl-lib.cpp"
    break;

  case 642: /* str_bytes_literal: BYTESLITERAL  */
#line 3129 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10471 "bison-chpl-lib.cpp"
    break;

  case 645: /* literal_expr: INTLITERAL  */
#line 3135 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10477 "bison-chpl-lib.cpp"
    break;

  case 646: /* literal_expr: REALLITERAL  */
#line 3136 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10483 "bison-chpl-lib.cpp"
    break;

  case 647: /* literal_expr: IMAGLITERAL  */
#line 3137 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10489 "bison-chpl-lib.cpp"
    break;

  case 648: /* literal_expr: CSTRINGLITERAL  */
#line 3138 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10495 "bison-chpl-lib.cpp"
    break;

  case 649: /* literal_expr: TNONE  */
#line 3139 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10501 "bison-chpl-lib.cpp"
    break;

  case 650: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3141 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10509 "bison-chpl-lib.cpp"
    break;

  case 651: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3145 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10517 "bison-chpl-lib.cpp"
    break;

  case 652: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3149 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10525 "bison-chpl-lib.cpp"
    break;

  case 653: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3153 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10533 "bison-chpl-lib.cpp"
    break;

  case 654: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3157 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10541 "bison-chpl-lib.cpp"
    break;

  case 655: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3161 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10549 "bison-chpl-lib.cpp"
    break;

  case 656: /* assoc_expr_ls: expr TALIAS expr  */
#line 3168 "chpl.ypp"
                                        { (yyval.exprList) = TODOLIST((yyloc)); }
#line 10555 "bison-chpl-lib.cpp"
    break;

  case 657: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3169 "chpl.ypp"
                                        { (yyvsp[-4].exprList)->push_back(TODOEXPR((yyloc))); }
#line 10561 "bison-chpl-lib.cpp"
    break;

  case 658: /* binary_op_expr: expr TPLUS expr  */
#line 3173 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10567 "bison-chpl-lib.cpp"
    break;

  case 659: /* binary_op_expr: expr TMINUS expr  */
#line 3174 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10573 "bison-chpl-lib.cpp"
    break;

  case 660: /* binary_op_expr: expr TSTAR expr  */
#line 3175 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10579 "bison-chpl-lib.cpp"
    break;

  case 661: /* binary_op_expr: expr TDIVIDE expr  */
#line 3176 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10585 "bison-chpl-lib.cpp"
    break;

  case 662: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3177 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10591 "bison-chpl-lib.cpp"
    break;

  case 663: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3178 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10597 "bison-chpl-lib.cpp"
    break;

  case 664: /* binary_op_expr: expr TMOD expr  */
#line 3179 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10603 "bison-chpl-lib.cpp"
    break;

  case 665: /* binary_op_expr: expr TEQUAL expr  */
#line 3180 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10609 "bison-chpl-lib.cpp"
    break;

  case 666: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3181 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10615 "bison-chpl-lib.cpp"
    break;

  case 667: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3182 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10621 "bison-chpl-lib.cpp"
    break;

  case 668: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3183 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10627 "bison-chpl-lib.cpp"
    break;

  case 669: /* binary_op_expr: expr TLESS expr  */
#line 3184 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10633 "bison-chpl-lib.cpp"
    break;

  case 670: /* binary_op_expr: expr TGREATER expr  */
#line 3185 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10639 "bison-chpl-lib.cpp"
    break;

  case 671: /* binary_op_expr: expr TBAND expr  */
#line 3186 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10645 "bison-chpl-lib.cpp"
    break;

  case 672: /* binary_op_expr: expr TBOR expr  */
#line 3187 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10651 "bison-chpl-lib.cpp"
    break;

  case 673: /* binary_op_expr: expr TBXOR expr  */
#line 3188 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10657 "bison-chpl-lib.cpp"
    break;

  case 674: /* binary_op_expr: expr TAND expr  */
#line 3189 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10663 "bison-chpl-lib.cpp"
    break;

  case 675: /* binary_op_expr: expr TOR expr  */
#line 3190 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10669 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr TEXP expr  */
#line 3191 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10675 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TBY expr  */
#line 3192 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10681 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TALIGN expr  */
#line 3193 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10687 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr THASH expr  */
#line 3194 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10693 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TDMAPPED expr  */
#line 3195 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10699 "bison-chpl-lib.cpp"
    break;

  case 681: /* unary_op_expr: TPLUS expr  */
#line 3199 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10705 "bison-chpl-lib.cpp"
    break;

  case 682: /* unary_op_expr: TMINUS expr  */
#line 3200 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10711 "bison-chpl-lib.cpp"
    break;

  case 683: /* unary_op_expr: TMINUSMINUS expr  */
#line 3201 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 10717 "bison-chpl-lib.cpp"
    break;

  case 684: /* unary_op_expr: TPLUSPLUS expr  */
#line 3202 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 10723 "bison-chpl-lib.cpp"
    break;

  case 685: /* unary_op_expr: TBANG expr  */
#line 3203 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10729 "bison-chpl-lib.cpp"
    break;

  case 686: /* unary_op_expr: expr TBANG  */
#line 3204 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10737 "bison-chpl-lib.cpp"
    break;

  case 687: /* unary_op_expr: TBNOT expr  */
#line 3207 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10743 "bison-chpl-lib.cpp"
    break;

  case 688: /* reduce_expr: expr TREDUCE expr  */
#line 3211 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10749 "bison-chpl-lib.cpp"
    break;

  case 689: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3212 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10755 "bison-chpl-lib.cpp"
    break;

  case 690: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3213 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10761 "bison-chpl-lib.cpp"
    break;

  case 691: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3214 "chpl.ypp"
                                                 { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10767 "bison-chpl-lib.cpp"
    break;

  case 692: /* scan_expr: expr TSCAN expr  */
#line 3218 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10773 "bison-chpl-lib.cpp"
    break;

  case 693: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3219 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10779 "bison-chpl-lib.cpp"
    break;

  case 694: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3220 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10785 "bison-chpl-lib.cpp"
    break;

  case 695: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3221 "chpl.ypp"
                                               { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10791 "bison-chpl-lib.cpp"
    break;

  case 696: /* reduce_scan_op_expr: TPLUS  */
#line 3226 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10797 "bison-chpl-lib.cpp"
    break;

  case 697: /* reduce_scan_op_expr: TSTAR  */
#line 3227 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10803 "bison-chpl-lib.cpp"
    break;

  case 698: /* reduce_scan_op_expr: TAND  */
#line 3228 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10809 "bison-chpl-lib.cpp"
    break;

  case 699: /* reduce_scan_op_expr: TOR  */
#line 3229 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10815 "bison-chpl-lib.cpp"
    break;

  case 700: /* reduce_scan_op_expr: TBAND  */
#line 3230 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10821 "bison-chpl-lib.cpp"
    break;

  case 701: /* reduce_scan_op_expr: TBOR  */
#line 3231 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10827 "bison-chpl-lib.cpp"
    break;

  case 702: /* reduce_scan_op_expr: TBXOR  */
#line 3232 "chpl.ypp"
         { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10833 "bison-chpl-lib.cpp"
    break;


#line 10837 "bison-chpl-lib.cpp"

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
