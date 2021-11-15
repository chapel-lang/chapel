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
  YYSYMBOL_187_1 = 187,                    /* $@1  */
  YYSYMBOL_188_2 = 188,                    /* $@2  */
  YYSYMBOL_deprecated_decl_base = 189,     /* deprecated_decl_base  */
  YYSYMBOL_module_decl_start = 190,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 191,         /* module_decl_stmt  */
  YYSYMBOL_access_control = 192,           /* access_control  */
  YYSYMBOL_opt_prototype = 193,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 194,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 195,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 196,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 197,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 198,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 199,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 200,              /* opt_only_ls  */
  YYSYMBOL_except_ls = 201,                /* except_ls  */
  YYSYMBOL_use_stmt = 202,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 203,              /* import_stmt  */
  YYSYMBOL_import_expr = 204,              /* import_expr  */
  YYSYMBOL_import_ls = 205,                /* import_ls  */
  YYSYMBOL_require_stmt = 206,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 207,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 208,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 209,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 210,                /* ident_def  */
  YYSYMBOL_ident_use = 211,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 212,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 213,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 214,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 215,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 216,              /* return_stmt  */
  YYSYMBOL_deprecated_class_level_stmt = 217, /* deprecated_class_level_stmt  */
  YYSYMBOL_218_3 = 218,                    /* $@3  */
  YYSYMBOL_219_4 = 219,                    /* $@4  */
  YYSYMBOL_class_level_stmt = 220,         /* class_level_stmt  */
  YYSYMBOL_221_5 = 221,                    /* $@5  */
  YYSYMBOL_222_6 = 222,                    /* $@6  */
  YYSYMBOL_inner_class_level_stmt = 223,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 224,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 225,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 226,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 227, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 228,  /* extern_export_decl_stmt  */
  YYSYMBOL_229_7 = 229,                    /* $@7  */
  YYSYMBOL_extern_block_stmt = 230,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 231,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 232,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 233,                  /* if_stmt  */
  YYSYMBOL_ifvar = 234,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 235,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 236,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 237,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 238,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 239, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 240,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 241,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 242,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 243,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 244,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 245,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 246,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 247,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 248,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 249,                /* when_stmt  */
  YYSYMBOL_class_decl_stmt = 250,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 251,              /* class_start  */
  YYSYMBOL_class_tag = 252,                /* class_tag  */
  YYSYMBOL_opt_inherit = 253,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 254,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 255,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 256,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 257,                  /* enum_ls  */
  YYSYMBOL_258_8 = 258,                    /* $@8  */
  YYSYMBOL_deprecated_enum_item = 259,     /* deprecated_enum_item  */
  YYSYMBOL_260_9 = 260,                    /* $@9  */
  YYSYMBOL_261_10 = 261,                   /* $@10  */
  YYSYMBOL_enum_item = 262,                /* enum_item  */
  YYSYMBOL_lambda_decl_expr = 263,         /* lambda_decl_expr  */
  YYSYMBOL_linkage_spec_empty = 264,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 265,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt_complete = 266,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 267,             /* fn_decl_stmt  */
  YYSYMBOL_268_11 = 268,                   /* $@11  */
  YYSYMBOL_fn_decl_stmt_inner = 269,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 270,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 271,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 272,                 /* fn_ident  */
  YYSYMBOL_assignop_ident = 273,           /* assignop_ident  */
  YYSYMBOL_formal_var_arg_expr = 274,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 275,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 276,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 277,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 278,                /* formal_ls  */
  YYSYMBOL_formal = 279,                   /* formal  */
  YYSYMBOL_opt_formal_intent_tag = 280,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 281,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 282,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 283,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 284,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 285,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 286,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 287,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 288,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 289,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 290, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 291,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 292,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 293,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 294, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 295, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 296, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 297,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 298,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 299,            /* var_decl_stmt  */
  YYSYMBOL_300_12 = 300,                   /* $@12  */
  YYSYMBOL_var_decl_stmt_inner_ls = 301,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 302,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 303, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 304, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 305,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 306,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 307,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 308,                 /* opt_type  */
  YYSYMBOL_array_type = 309,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 310, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 311,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 312,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 313,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 314,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 315,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 316,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 317,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 318,                /* actual_ls  */
  YYSYMBOL_actual_expr = 319,              /* actual_expr  */
  YYSYMBOL_ident_expr = 320,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 321,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 322,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 323,                 /* for_expr  */
  YYSYMBOL_cond_expr = 324,                /* cond_expr  */
  YYSYMBOL_nil_expr = 325,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 326,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 327,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 328,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 329,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 330,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 331,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 332,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 333,          /* task_var_prefix  */
  YYSYMBOL_io_expr = 334,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 335,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 336,                 /* new_expr  */
  YYSYMBOL_let_expr = 337,                 /* let_expr  */
  YYSYMBOL_expr = 338,                     /* expr  */
  YYSYMBOL_opt_expr = 339,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 340,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 341,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 342,           /* call_base_expr  */
  YYSYMBOL_call_expr = 343,                /* call_expr  */
  YYSYMBOL_dot_expr = 344,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 345,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 346,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 347,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 348,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 349,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 350,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 351,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 352,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 353,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 354       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 362 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include other helper functions for the parser
  #include "parser-help.h"


#line 477 "bison-chpl-lib.cpp"

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
#define YYLAST   20387

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  175
/* YYNRULES -- Number of rules.  */
#define YYNRULES  708
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1252

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
       0,   639,   639,   643,   644,   649,   650,   658,   662,   670,
     671,   672,   673,   674,   675,   676,   677,   678,   679,   680,
     681,   682,   683,   696,   697,   698,   699,   700,   701,   715,
     724,   739,   748,   756,   779,   791,   802,   814,   826,   837,
     850,   857,   864,   866,   865,   874,   873,   884,   885,   889,
     909,   924,   939,   955,   956,   958,   963,   964,   969,   986,
     990,   994,  1005,  1024,  1025,  1029,  1033,  1038,  1042,  1052,
    1057,  1063,  1070,  1075,  1082,  1093,  1094,  1098,  1103,  1110,
    1114,  1121,  1129,  1138,  1145,  1153,  1165,  1172,  1176,  1182,
    1191,  1192,  1196,  1205,  1209,  1213,  1217,  1221,  1225,  1234,
    1235,  1239,  1240,  1241,  1242,  1243,  1244,  1247,  1248,  1249,
    1250,  1251,  1252,  1264,  1265,  1276,  1277,  1278,  1279,  1280,
    1281,  1282,  1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,
    1291,  1292,  1293,  1294,  1298,  1299,  1300,  1301,  1302,  1303,
    1304,  1305,  1306,  1307,  1308,  1309,  1316,  1317,  1318,  1319,
    1323,  1324,  1328,  1335,  1345,  1347,  1346,  1355,  1354,  1365,
    1369,  1375,  1375,  1381,  1381,  1390,  1391,  1392,  1393,  1394,
    1395,  1396,  1400,  1405,  1410,  1415,  1422,  1430,  1431,  1435,
    1444,  1450,  1458,  1478,  1477,  1490,  1497,  1515,  1528,  1541,
    1545,  1549,  1553,  1557,  1561,  1565,  1569,  1587,  1591,  1595,
    1599,  1603,  1607,  1611,  1615,  1619,  1623,  1627,  1631,  1635,
    1639,  1643,  1647,  1651,  1655,  1659,  1663,  1667,  1671,  1675,
    1680,  1688,  1697,  1701,  1705,  1709,  1713,  1717,  1721,  1725,
    1729,  1734,  1739,  1744,  1752,  1767,  1785,  1787,  1792,  1793,
    1798,  1803,  1804,  1805,  1806,  1807,  1808,  1809,  1810,  1811,
    1812,  1813,  1814,  1815,  1827,  1828,  1829,  1830,  1839,  1840,
    1844,  1846,  1848,  1853,  1855,  1857,  1862,  1866,  1870,  1874,
    1878,  1882,  1889,  1890,  1894,  1898,  1902,  1909,  1922,  1938,
    1946,  1950,  1959,  1960,  1964,  1968,  1973,  1983,  1988,  1999,
    2006,  2007,  2008,  2012,  2013,  2017,  2021,  2025,  2032,  2049,
    2062,  2069,  2074,  2081,  2080,  2093,  2095,  2094,  2103,  2102,
    2113,  2120,  2132,  2140,  2144,  2145,  2147,  2152,  2162,  2159,
    2187,  2195,  2203,  2214,  2225,  2234,  2247,  2248,  2252,  2253,
    2254,  2255,  2256,  2257,  2258,  2259,  2260,  2261,  2262,  2263,
    2264,  2265,  2266,  2267,  2268,  2269,  2270,  2271,  2272,  2273,
    2274,  2275,  2276,  2277,  2278,  2279,  2288,  2289,  2290,  2291,
    2292,  2293,  2294,  2295,  2296,  2297,  2298,  2299,  2303,  2304,
    2305,  2309,  2310,  2314,  2318,  2319,  2323,  2324,  2328,  2337,
    2348,  2358,  2370,  2382,  2390,  2394,  2402,  2403,  2404,  2405,
    2406,  2407,  2408,  2409,  2410,  2414,  2415,  2416,  2417,  2418,
    2419,  2423,  2424,  2425,  2429,  2430,  2431,  2432,  2433,  2434,
    2438,  2439,  2442,  2443,  2447,  2448,  2452,  2457,  2458,  2460,
    2462,  2464,  2469,  2471,  2476,  2478,  2480,  2482,  2484,  2486,
    2488,  2493,  2494,  2498,  2506,  2510,  2515,  2523,  2527,  2534,
    2555,  2556,  2558,  2566,  2567,  2568,  2569,  2570,  2575,  2574,
    2583,  2591,  2595,  2602,  2618,  2635,  2639,  2643,  2650,  2652,
    2654,  2661,  2662,  2663,  2668,  2672,  2676,  2680,  2684,  2688,
    2692,  2699,  2700,  2701,  2702,  2703,  2708,  2709,  2710,  2711,
    2712,  2732,  2736,  2740,  2744,  2751,  2752,  2753,  2757,  2762,
    2770,  2775,  2779,  2786,  2787,  2788,  2789,  2790,  2796,  2797,
    2798,  2799,  2803,  2804,  2808,  2809,  2810,  2814,  2818,  2825,
    2826,  2830,  2835,  2844,  2845,  2846,  2847,  2851,  2852,  2863,
    2865,  2867,  2873,  2874,  2875,  2876,  2877,  2878,  2880,  2882,
    2884,  2886,  2892,  2894,  2897,  2899,  2901,  2903,  2905,  2907,
    2909,  2911,  2914,  2916,  2921,  2930,  2939,  2947,  2961,  2975,
    2989,  2998,  3007,  3015,  3029,  3043,  3057,  3073,  3082,  3091,
    3106,  3124,  3142,  3150,  3151,  3152,  3153,  3154,  3155,  3156,
    3157,  3162,  3163,  3167,  3176,  3177,  3181,  3190,  3191,  3195,
    3210,  3214,  3221,  3222,  3223,  3224,  3225,  3226,  3230,  3231,
    3236,  3238,  3240,  3242,  3244,  3250,  3267,  3271,  3275,  3279,
    3286,  3294,  3295,  3296,  3297,  3298,  3299,  3300,  3301,  3302,
    3303,  3307,  3311,  3316,  3321,  3326,  3332,  3338,  3370,  3371,
    3375,  3376,  3377,  3381,  3382,  3383,  3384,  3393,  3394,  3395,
    3396,  3397,  3401,  3413,  3425,  3432,  3434,  3436,  3438,  3440,
    3446,  3459,  3460,  3464,  3468,  3475,  3476,  3480,  3481,  3485,
    3486,  3487,  3488,  3489,  3490,  3491,  3492,  3496,  3500,  3504,
    3508,  3512,  3521,  3526,  3535,  3536,  3537,  3538,  3539,  3540,
    3541,  3542,  3543,  3544,  3545,  3546,  3547,  3548,  3549,  3550,
    3551,  3552,  3553,  3554,  3555,  3556,  3557,  3561,  3562,  3563,
    3564,  3565,  3566,  3569,  3573,  3577,  3581,  3585,  3592,  3596,
    3600,  3604,  3612,  3613,  3614,  3615,  3616,  3617,  3618
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
  "toplevel_stmt", "pragma_ls", "stmt", "deprecated_decl_stmt", "$@1",
  "$@2", "deprecated_decl_base", "module_decl_start", "module_decl_stmt",
  "access_control", "opt_prototype", "include_module_stmt",
  "block_stmt_body", "block_stmt", "stmt_ls", "renames_ls",
  "use_renames_ls", "opt_only_ls", "except_ls", "use_stmt", "import_stmt",
  "import_expr", "import_ls", "require_stmt", "assignment_stmt",
  "opt_label_ident", "ident_fn_def", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "do_stmt", "return_stmt", "deprecated_class_level_stmt", "$@3", "$@4",
  "class_level_stmt", "$@5", "$@6", "inner_class_level_stmt",
  "forwarding_decl_stmt", "forwarding_decl_start", "extern_or_export",
  "extern_export_decl_stmt_start", "extern_export_decl_stmt", "$@7",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
  "catch_expr", "catch_expr_inner", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "class_decl_stmt", "class_start",
  "class_tag", "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header_lcbr", "enum_ls", "$@8", "deprecated_enum_item", "$@9",
  "$@10", "enum_item", "lambda_decl_expr", "linkage_spec_empty",
  "linkage_spec", "fn_decl_stmt_complete", "fn_decl_stmt", "$@11",
  "fn_decl_stmt_inner", "fn_decl_stmt_start", "fn_decl_receiver_expr",
  "fn_ident", "assignop_ident", "formal_var_arg_expr", "opt_formal_ls",
  "req_formal_ls", "formal_ls_inner", "formal_ls", "formal",
  "opt_formal_intent_tag", "required_intent_tag", "opt_this_intent_tag",
  "proc_iter_or_op", "opt_ret_tag", "opt_throws_error",
  "opt_function_body_stmt", "function_body_stmt", "query_expr",
  "opt_lifetime_where", "lifetime_components_expr", "lifetime_expr",
  "lifetime_ident", "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner_ls", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt", "$@12",
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

#define YYPACT_NINF (-1096)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-659)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1096,   167,  3477, -1096,   107,   289, -1096, -1096, -1096, -1096,
   -1096, -1096,  4913,   214,   352,   303, 14383,   397, 20152,   214,
   11323,   401,   254,   284,   352,  4913, 11323,   458,  4913,   304,
   20186, -1096,   134,   457,  8383,  9595,  9595, -1096,  8555,   461,
     331,   338, -1096,   478, 20186, 20186, 20186,   378, 14835,  9767,
     509, 11323,   215, -1096,   521,   525, 11323, -1096, 14383, -1096,
   11323,   583,   436,   265,   411,   558, 20272, -1096,  9941,  7865,
   11323,  9767, 14383, 11323,   516,   562,   448,  4913,   566, 11323,
     569, 11495, 11495, -1096,   570, -1096, 14383, -1096,   575,  8555,
   11323, -1096, 11323, -1096, 11323, -1096, -1096, 13903, 11323, -1096,
   11323, -1096, -1096, -1096,  3869,  7001,  8729, 11323, -1096,  4739,
   -1096, -1096, -1096,   477, -1096,   267, -1096, -1096, -1096, -1096,
   -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096,  7173,
   -1096, 10113, -1096, -1096, -1096, -1096, -1096,   592, -1096, -1096,
   -1096, -1096, -1096, -1096, -1096,   514, 20186, -1096, 20032,    83,
   -1096,    45, -1096, -1096,   257,   413, -1096, 20186, 14835, -1096,
     488, -1096,   496, -1096, -1096,   497,   498,   505, 11323,   501,
     504, 19824,  2521,   -63,   506,   507, -1096, -1096,   120, -1096,
   -1096, -1096, -1096, -1096,    33, -1096, -1096, 19824,   503,  4913,
   -1096, -1096,   511, 11323, -1096, -1096, 11323, 11323, 11323, 20186,
   -1096, 11323,  9941,  9941,   613,   423, -1096, -1096, -1096, -1096,
     200,   459, -1096, -1096,   512, 16380, -1096,    74, -1096,   520,
   -1096,   -41, 19824, -1096,  1303,   567,  8039, -1096, -1096, -1096,
   -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096,
   -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096,
   -1096,   523, -1096, -1096, -1096, 20186,   524,    38, 15967,    35,
   15802,    35, 15884, 20186, 20186,    -5, 15055, -1096, -1096,   609,
    8039,   527,   464,  4913,   160,   257, 14749,    54,    98, -1096,
    4913, -1096, -1096, 16461,   169, -1096,   528,   531, -1096, 16461,
     200,   169, -1096,  8039,  1513,  1513, -1096, -1096,   178, 19824,
   11323, 11323, -1096, 19824,   530, 17035, -1096, 16461,   200, 19824,
     534,  8039, -1096, 19824, 17118, -1096, -1096, 17200,  1810, -1096,
   -1096, 17281,   200,    38, 16461, 17362,   209,   209,  1623,   169,
     169,   408, -1096, -1096,  4043,   106, -1096, 11323, -1096,   100,
     150, -1096,    -6,    -3, 17443,   101,  1623,   695, -1096,  4217,
   11323, -1096, 11323,   643, -1096, -1096, 16050,    62,   514, 19824,
     522, 20186,  9941,   551, -1096,   554,   717,   612,   149, -1096,
   -1096, -1096, -1096, -1096, -1096,   644, -1096, -1096, -1096,    75,
     651, -1096, -1096, -1096,  3233,   617,   578,   600,   317,   463,
   -1096, 11323,   603, 11323, 11323, 11323,  9595,  9595, 11323,   466,
   11323, 11323, 11323, 11323, 11323,   430, 13903, 11323, 11323, 11323,
   11323, 11323, 11323, 11323, 11323, 11323, 11323, 11323, 11323, 11323,
   11323, 11323, 11323,   684, -1096, -1096, -1096, -1096, -1096,  8901,
    8901, -1096, -1096, -1096, -1096,  8901, -1096, -1096,  8901,  8901,
    8039,  8039,  9595,  9595,  7693, -1096, -1096, 16543, 16624, 17524,
     586,    76, 20186,  4391, -1096,  9595,    38, 14835, -1096, 11323,
   -1096,  1303, -1096, 20186,   646, -1096, -1096,   609, 11323,   634,
   -1096,   587,   615, -1096, -1096, -1096,   713,  9941, -1096,  5087,
    9595, -1096,   591, -1096,    38,  5261,  9595, -1096,    38, -1096,
      38,  9595, -1096,    38,   640,   648,  4913,   732,  4913, -1096,
     733, 11323,   711,   605,  8039, 20186, -1096, -1096,    59, -1096,
   -1096, -1096, -1096, -1096, -1096,   691,   633,   610, -1096, 14869,
   -1096,    53, -1096, 14749, -1096, -1096,   172, -1096, 11667,   659,
   11323, 14835, -1096, -1096, 11323, 11323, -1096,   618, -1096, -1096,
    9941, -1096, 19824, 19824, -1096,    46, -1096,  8039,   620, -1096,
     764, -1096,   764, -1096, -1096, -1096, -1096, -1096, -1096, -1096,
    9075, -1096, 17605,  7347, -1096,  7521, -1096,  4913,   621,  9595,
    9249,  3695,   622, 11323, 10285, -1096, -1096,   446, -1096,  4565,
   -1096,   366, 17686,   367, 16133, 20186, 11839, 11323, -1096,   514,
     625,    74, -1096,   116,   642,  2780, -1096, -1096, 20186, 11323,
     184, -1096, -1096, -1096, 12011,   694, -1096,   629,   313, -1096,
     654, -1096,   658,   660,   667,   663,   665, -1096,   668,   677,
     675,   676,   678,   453,   689,   682,   683, -1096, -1096, -1096,
   -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096,
   -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096,
   11323, -1096,   692,   696,   687,   629,   629, -1096, 12183, -1096,
   -1096, 20186, -1096, 17767,  1688,  1688,   416, -1096,   416, -1096,
     416, 19333,   462,  2378,  1016,   200,   209, -1096,   480, -1096,
   -1096, -1096, -1096, -1096,  1623, 19517,   416,  1115,  1115,  1688,
    1115,  1115,   893,   209, 19517, 16421,   893,   169,   169,   209,
    1623,   672,   673,   674,   679,   681,   685,   688,   680, -1096,
     416, -1096,   416,    61, -1096, -1096, -1096,   192, -1096,  2001,
   19865,    34, 12355,  9595, 12527,  9595, 11323,  8039,  9595, 14663,
     690,   214, 17848, -1096,   382, 19824, -1096, 17929,  8039, -1096,
    8039, 11323,   274,  8555, 19824,    40, 16705,  7693, -1096,  8555,
   19824,    58, 16215, -1096, -1096,    35, 16298, -1096, 11323, 11323,
     812,  4913,   813, 18010,  4913, 16787, 20186, -1096,   277, -1096,
     306, -1096, -1096, -1096, 20186,   766, -1096, 14749,   707,    36,
     310, 14749,    54,    23,    44, 11323, 11323,  6827, -1096, -1096,
     532,  8211, -1096, 19824, -1096, 18091, 18172, -1096, -1096, 19824,
     686,   154,   697, -1096,  3126, -1096, -1096, -1096, -1096, -1096,
   -1096, -1096, -1096,  4913,   112, 16870, -1096, -1096, 19824,  4913,
   19824, -1096, 18253, -1096, -1096, 11323, -1096,   127,   109, 11323,
   -1096, 10457, 11839, 11323, -1096,   698,   708,   699, 18334,   708,
     700,   701, -1096, 14835, -1096,  8039,   702,  1881, 20186, -1096,
   19824, 20066,  6653, -1096, -1096,   194, -1096,    36,   160, -1096,
   18415, -1096, 14973, -1096, -1096, -1096,   402, -1096,   703,   704,
   -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096,
    7693, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096, -1096,
   -1096, -1096, -1096,    51,  9595,  9595, 11323,   833, 18496, 11323,
     844, 18577,   316,   705, 18658,  8039,    38,    38, -1096, -1096,
   -1096, -1096, -1096, 16461, -1096, 15138,  5435, -1096,  5609, -1096,
     318, -1096, 15221,  5783, -1096,    38,  5957, -1096,    38, -1096,
      38, -1096,    38, 19824, 19824,  4913, -1096,  4913, 11323, -1096,
    4913,   845,   721,   724, 20186,   512,   707, -1096,   327, 10631,
      16,    78, 11323,    -8, -1096, -1096,   659,   715,    79, -1096,
   -1096,   710,   719, -1096,  6131,  9941, -1096, -1096, -1096, 20186,
   -1096,   750,   512, -1096,  6305,   722,  6479,   723, -1096, 11323,
   -1096, -1096, 11323, 18739,    69, 16953,   731,   734, 11323, -1096,
   11323, -1096, -1096,  2958,   419,   353, -1096,   892, -1096, -1096,
   -1096,  2301, -1096, -1096, -1096, -1096,   727, 13731,    85, -1096,
     730, -1096,   768,   769,   629,   629, -1096, -1096, -1096,   659,
     370,   384, 18821, 12699, 12871, 18902, 13043, 13215, -1096, 13387,
   13559,   398, -1096, -1096, -1096,  4913,  8555, 19824,  8555, 19824,
    7693, -1096,  4913,  8555, 19824, -1096,  8555, 19824, -1096, -1096,
   -1096, -1096, -1096, 19824,   868,  4913, -1096, -1096, -1096, -1096,
      16,   707,  9423, -1096, -1096, -1096,   128,  9941, -1096, -1096,
   -1096,   135, -1096,   115, -1096,   526, 18996, -1096, -1096, -1096,
   -1096, -1096,  9595, 14523,  8039,  8039,  4913, -1096,    57,   740,
   11323, -1096,  8555, -1096, 19824,  4913,  8555, -1096, 19824,  4913,
   19824,   356, 10803, 11839, 11323, 11839, 11323, -1096, -1096, 19053,
   19824, 19248, -1096, -1096,   751, -1096,  2112, -1096, -1096, -1096,
     270, 14072,   157, -1096, -1096, -1096, -1096, 11323, 11323, 11323,
   11323, 11323, 11323, 11323, 11323, -1096, 18010, 15304, 15387, -1096,
   18010, 15470, 15553,  4913, -1096, -1096, -1096,    16, 10977,    91,
   -1096, 19824, -1096, 11323,    78,   135,   135,   135,   135,   135,
     135,    78, 19093, -1096,   639,   744,   745,   567, -1096,   512,
   19824, 15636, -1096, 15719, -1096, -1096, -1096, 19824,   754,   755,
     756,   757, 11323, -1096, -1096,  2112, -1096, -1096,   563, -1096,
   -1096, -1096, 19244, 19284, 19435, 19475, 19626, 19666,  4913,  4913,
    4913,  4913, -1096, -1096, -1096, -1096, -1096, -1096,   335,  9595,
   14212, 19824, -1096, -1096, -1096, -1096, -1096, -1096, -1096,   779,
   13559,   767,   770, -1096,  4913,  4913, -1096, -1096, -1096, -1096,
   19824, -1096, 11323, 11323, 11323, 11323, 11323, 11323, 18010, 18010,
   18010, 18010, 19707, -1096, -1096,   741, -1096, -1096, 18010, 18010,
   11151, -1096
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   113,   651,   652,   653,   647,
     648,   654,     0,   571,    99,   134,   540,   141,   542,   571,
       0,   140,   448,   446,    99,     0,     0,    45,     0,   258,
     135,   178,   177,   645,     0,     0,     0,   176,     0,   139,
      53,   259,   315,   136,     0,     0,     0,     0,     0,     0,
     143,     0,   590,   562,   655,   144,     0,   316,   534,   443,
       0,     0,     0,   163,   161,   138,   543,   445,     0,     0,
       0,     0,   538,     0,     0,   142,     0,     0,   114,     0,
     646,     0,     0,   434,   137,   292,   536,   447,   145,     0,
       0,   704,     0,   706,     0,   707,   708,   617,     0,   705,
     702,   521,   159,   703,     0,     0,     0,     0,     4,     0,
       5,     9,    42,     0,    47,    56,    10,    62,    11,    12,
      13,    14,    15,   517,   518,    25,    48,   160,   170,     0,
     179,   618,   171,    16,    20,    17,    19,     0,   253,    18,
     609,    23,    24,    21,   169,   293,     0,   167,     0,   606,
     314,     0,   165,   317,   404,   395,   168,     0,     0,   166,
     623,   602,   519,   603,   524,   522,     0,     0,     0,   607,
     608,     0,   523,     0,   624,   625,   626,   649,   650,   601,
     526,   525,   604,   605,     0,    41,    27,   532,     0,     0,
     572,   100,     0,     0,   542,   135,     0,     0,     0,     0,
     543,     0,     0,     0,     0,   606,   623,   522,   607,   608,
     541,   523,   624,   625,     0,   571,   435,     0,   444,     0,
      22,     0,   502,    43,   313,     0,   509,   107,   115,   127,
     121,   120,   129,   110,   119,   130,   116,   131,   108,   132,
     125,   118,   126,   124,   122,   123,   109,   111,   117,   128,
     133,     0,   112,   185,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    55,    54,    56,
     509,     0,     0,     0,   384,   404,     0,     0,     0,   451,
       0,   151,    35,     0,   689,   594,   591,   592,   593,     0,
     535,   690,     7,   509,   313,   313,   416,   499,     0,   498,
       0,     0,   152,   622,     0,     0,    38,     0,   539,   527,
       0,   509,    39,   533,     0,   272,   268,     0,   523,   272,
     269,     0,   537,     0,     0,     0,   691,   693,   615,   688,
     687,     0,    59,    63,     0,     0,   504,     0,   506,     0,
       0,   505,     0,     0,   498,     0,   616,     0,     6,     0,
       0,    57,     0,     0,   448,   175,     0,   647,   293,   619,
     183,     0,     0,     0,   289,     0,   308,   310,     0,   301,
     305,   402,   403,   401,   325,   405,   408,   407,   409,     0,
     399,   396,   397,   400,     0,   440,     0,   437,     0,   520,
      26,     0,   595,     0,     0,     0,     0,     0,     0,   692,
       0,     0,     0,     0,     0,     0,   614,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   356,   363,   364,   365,   360,   362,     0,
       0,   358,   361,   359,   357,     0,   367,   366,     0,     0,
     509,   509,     0,     0,     0,    28,    29,     0,     0,     0,
       0,     0,     0,     0,    30,     0,     0,     0,    31,     0,
      32,   313,   290,     0,   177,   291,    46,    56,     0,   517,
     515,     0,   510,   511,   516,   300,     0,     0,   195,     0,
       0,   194,     0,   203,     0,     0,     0,   201,     0,   211,
       0,     0,   209,     0,     0,     0,     0,   227,     0,   356,
     223,     0,     0,     0,     0,     0,   237,    33,   391,   386,
     387,   388,   392,   393,   394,   384,   377,     0,   374,     0,
     385,     0,   455,     0,   456,   458,     0,   480,     0,   461,
       0,     0,   150,    34,     0,     0,    36,     0,   164,   162,
       0,    92,   620,   621,   153,     0,    37,     0,     0,   279,
     270,   266,   271,   267,   188,   187,    40,    61,    60,    64,
       0,   656,     0,     0,   641,     0,   643,     0,     0,     0,
       0,     0,     0,     0,     0,   660,     8,     0,    50,     0,
      90,     0,    87,     0,    69,     0,     0,    75,   172,   293,
       0,     0,   182,   264,   294,     0,   299,   306,     0,     0,
     303,   298,   406,   475,     0,   410,   398,   371,   101,   350,
     115,   348,   121,   120,   104,   119,   116,   353,   131,   102,
     132,   118,   122,   103,   105,   117,   133,   347,   329,   332,
     330,   331,   354,   342,   333,   346,   338,   336,   349,   352,
     337,   335,   340,   345,   334,   339,   343,   344,   341,   351,
       0,   328,     0,   106,     0,   371,   371,   326,     0,   439,
     433,     0,   450,   570,   684,   683,   686,   695,   694,   699,
     698,   680,   677,   678,   679,   611,   667,   113,     0,   637,
     638,   114,   636,   635,   612,   671,   682,   676,   674,   685,
     675,   673,   665,   670,   672,   681,   664,   668,   669,   666,
     613,     0,     0,     0,     0,     0,     0,     0,     0,   697,
     696,   701,   700,   582,   583,   585,   587,     0,   574,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   658,
     264,   571,   571,   191,     0,   503,    44,     0,     0,   529,
       0,     0,     0,     0,   546,     0,     0,     0,   204,     0,
     552,     0,     0,   202,   212,     0,     0,   210,     0,     0,
     226,     0,   222,     0,     0,     0,     0,   528,     0,   240,
       0,   238,   389,   390,     0,   384,   373,     0,   493,   417,
       0,   459,     0,   148,   149,   147,   146,     0,   479,   478,
     602,     0,   453,   600,   452,     0,     0,   634,   501,   500,
       0,     0,     0,   530,     0,   273,   657,   610,   642,   507,
     644,   508,   219,     0,     0,     0,   659,   217,   556,     0,
     662,   661,     0,    52,    51,     0,    86,     0,     0,     0,
      79,     0,     0,    75,    49,   703,    78,     0,    65,    76,
       0,     0,   295,     0,   261,     0,     0,   313,     0,   309,
     311,     0,     0,   474,   473,   602,   411,   417,   384,   324,
       0,   355,     0,   320,   321,   442,   602,   438,     0,     0,
      98,    96,    97,    95,    94,    93,   632,   633,   584,   586,
       0,   573,   134,   141,   140,   139,   136,   143,   144,   138,
     142,   137,   145,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   449,   186,
     513,   514,   512,     0,   221,     0,     0,   193,     0,   192,
       0,   577,     0,     0,   199,     0,     0,   197,     0,   207,
       0,   205,     0,   235,   234,     0,   229,     0,     0,   225,
       0,   231,     0,   263,     0,     0,   493,   375,     0,     0,
     461,     0,     0,     0,   457,   460,   461,     0,     0,   462,
     463,     0,     0,   281,     0,     0,   280,   283,   531,     0,
     274,   277,     0,   220,     0,     0,     0,     0,   218,     0,
      91,    88,     0,    72,    71,    70,     0,     0,     0,   173,
       0,   174,   295,   313,     0,     0,   288,   157,   163,   161,
     287,   313,   296,   154,   307,   304,     0,   465,     0,   318,
       0,   327,   101,   103,   371,   371,   639,   640,   575,   461,
     623,   623,     0,     0,     0,     0,     0,     0,   263,     0,
       0,     0,   190,   189,   196,     0,     0,   545,     0,   544,
       0,   576,     0,     0,   551,   200,     0,   550,   198,   208,
     206,   228,   224,   561,   230,     0,    58,   260,   239,   236,
     461,   493,     0,   496,   495,   497,   602,   368,   380,   378,
     431,     0,   432,   419,   422,     0,   418,   414,   415,   312,
     454,   484,     0,   658,   509,   509,     0,   285,     0,     0,
       0,   275,     0,   215,   558,     0,     0,   213,   557,     0,
     663,     0,     0,     0,    75,     0,    75,    80,    83,    67,
      66,   313,   180,   184,   265,   155,   313,   297,   470,   468,
     602,   658,     0,   372,   322,   323,   579,     0,     0,     0,
       0,     0,     0,     0,     0,   265,   549,     0,     0,   578,
     555,     0,     0,     0,   233,   381,   379,   461,   485,     0,
     370,   369,   430,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   482,   602,     0,     0,   286,   284,     0,
     278,     0,   216,     0,   214,    89,    74,    73,     0,     0,
       0,     0,     0,   181,   262,   313,   158,   469,   602,   412,
     319,   413,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   232,   383,   382,   487,   488,   490,   602,     0,
     658,   421,   423,   424,   427,   428,   429,   425,   426,   420,
       0,   596,   597,   276,     0,     0,    82,    85,    81,    84,
      68,   156,     0,     0,     0,     0,     0,     0,   548,   547,
     554,   553,     0,   489,   491,   602,   598,   599,   560,   559,
     485,   492
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1096, -1096, -1096,     7,  -259,  2147, -1096, -1096, -1096,  -194,
   -1096, -1096,   -24,   662, -1096,  -917,  2526,   580,  -567, -1096,
    -800,  -798, -1096, -1096,   102, -1096, -1096,   339,   908, -1096,
    2256,    64,  -377, -1096,  -583,   383,  -912,   -65, -1096, -1096,
    -842, -1096, -1096,   202, -1096, -1096, -1096, -1096, -1096, -1096,
   -1096, -1096,    28, -1096,   846, -1096, -1096,    -7,   661, -1096,
   -1096, -1096, -1096,   619, -1096,   -30, -1096, -1096, -1096, -1096,
   -1096,   -93, -1096,  -327,  -799, -1096, -1096, -1096, -1096,    93,
   -1096, -1096,  -569,  1167, -1096, -1096, -1096,   582, -1096, -1096,
   -1096, -1096,    81,  -171,  -973,  -638, -1096, -1096,    87,   171,
     432, -1096, -1096, -1096,   693, -1096, -1096,  -174,  1331,    94,
    -212,  -204,  -788, -1096, -1096,   291, -1096, -1096,  -198,   826,
   -1096,  -155,   425,   176,  -495,  -884,  -918, -1096,  -479,  -645,
   -1095, -1050,  -841,   -66, -1096,    92, -1096,  -258,  -493,  -515,
     475,  -501, -1096, -1096, -1096,  1239, -1096,    -9, -1096, -1096,
    -211, -1096,  -722, -1096, -1096, -1096,  1471,  1534,   -12, -1096,
       6,   508, -1096,  1775,  1963, -1096, -1096, -1096, -1096, -1096,
   -1096, -1096, -1096, -1096,  -438
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   333,   109,   762,   111,   461,   224,   112,
     113,   114,   115,   353,   116,   117,   118,   334,   836,   583,
     840,   837,   119,   120,   580,   581,   121,   122,   192,   651,
     277,   123,   252,   124,   788,   282,   125,  1002,  1185,  1116,
     126,   295,   294,   127,   128,   129,   130,   131,   132,   591,
     133,   134,   903,   135,   265,   136,   770,   771,   204,   138,
     139,   140,   141,   550,   805,   972,   142,   143,   801,   967,
     144,   145,   146,   363,   847,   147,   148,   368,   851,   369,
     848,   598,   370,   205,   150,   151,   152,   153,  1122,   154,
     155,   654,   655,   501,  1068,   859,   275,   516,   517,   518,
     519,   520,   384,   374,   379,   857,  1190,  1079,   470,   953,
    1073,  1074,  1075,   156,   157,   386,   387,   659,   158,   159,
     217,   278,   279,   525,   526,   792,   854,   605,   529,   789,
    1206,  1065,   950,   335,   221,   339,   340,   471,   472,   473,
     206,   161,   162,   163,   164,   207,   166,   189,   190,   717,
     484,   920,   718,   719,   167,   168,   208,   209,   171,   360,
     474,   211,   173,   212,   213,   176,   177,   178,   179,   345,
     180,   181,   182,   183,   184
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     187,   439,   298,   388,   210,  1003,   721,   653,   215,   108,
     214,   768,   503,   865,   222,   515,   269,   863,   864,   457,
     839,   853,   258,   260,   262,   921,   266,   790,   780,   849,
     466,   590,   802,   987,   986,   537,  1077,   283,   358,   284,
     343,  1078,   779,   993,   289,   569,   290,   800,   291,   488,
     490,   493,   527,   548,   527,   527,   299,   303,   305,   307,
     308,   309,   257,   259,   261,   313,  1069,   314,   280,   317,
     321,   280,  1080,   916,   322,   304,   603,   324,   325,  -258,
     326,  1070,   327,    69,   462,   328,   329,  1145,   330,  1119,
     280,   923,   299,   303,   344,   346,   496,   951,  1207,   371,
    -259,    23,  -476,   855,   459,  1060,  1103,   772,   440,   878,
     441,   341,   677,   482,  -476,  1243,   482,   356,   372,   359,
    -282,   442,   895,   460,   728,   443,   443,  1082,   373,   678,
     677,   568,   572,   728,   342,  1126,  -471,   451,  1070,  1209,
     791,  1104,   570,   679,  -476,   253,   530,   439,   773,   465,
     879,  1003,    59,   952,   482,  1251,   392,   866,  1018,  1003,
    1244,   453,  -282,    67,   453,  1067,  -471,     3,   567,  1071,
    -476,   571,   680,    85,  1203,  -476,  1146,   482,  -476,  -471,
    1072,   187,   707,   708,   447,   448,   449,   508,    87,   313,
     299,   344,  -471,  1111,   226,   528,  -476,   528,   528,  -476,
     467,   395,   540,  1187,   453,  1077,   456,   453,   509,   453,
    1078,   681,   510,   656,   303,   270,  -282,   682,  -476,   604,
    1147,   570,  -476,  -476,   570,   912,   453,   453,   964,   681,
     570,   482,  1153,   285,   902,   511,   570,  1072,   512,  -471,
      61,   395,   254,   531,  -471,   563,   574,  -567,    69,   513,
     729,   560,  -494,  1083,  -630,  -472,  -630,   396,   303,  1121,
    1154,   397,   841,  1204,   589,  1210,   839,   736,   514,  1003,
     965,   185,   564,  -494,  1186,   575,   561,  -494,   982,  1004,
     844,   303,   948,  1152,   375,  -472,   974,   845,   542,   543,
     469,  -631,   286,  -631,   600,   565,   594,   396,  -472,   303,
    -494,   397,   734,   956,  1179,  1178,  1181,  1180,   399,   721,
     287,  -472,   -55,   403,   350,   404,   405,   781,  1139,   601,
     408,  1189,   566,   540,   966,   562,   453,   288,   415,  -302,
     -55,  -464,  -376,   188,   469,   376,   421,   880,   582,   399,
     584,   559,   541,  1231,   782,  -241,   377,   405,   399,   -55,
     299,   351,   995,   403,  -302,   191,   405,   469,  -472,  -242,
     408,  -464,   216,  -472,   881,   378,  1063,  1213,  1214,  1215,
    1216,  1217,  1218,   218,  -464,   469,  1124,  1125,   -55,   663,
     352,   664,   665,   666,   668,   670,   671,  -464,   672,   673,
     674,   675,   676,   843,   684,   685,   686,   687,   688,   689,
     690,   691,   692,   693,   694,   695,   696,   697,   698,   699,
     700,   742,  1031,   267,  1019,  1101,   268,   303,   303,   540,
     316,   320,   740,   303,   667,   669,   303,   303,   303,   303,
     710,   712,   720,   677,  -464,   702,   703,   467,  1163,  -464,
     380,   704,   721,   732,   705,   706,   914,   735,  1066,   943,
     678,   944,  -107,  -248,   306,   781,   737,  -247,   -54,  -486,
    -113,   740,   531,  1040,   679,   299,   223,   744,   746,   683,
     709,   711,   781,   750,   752,   226,   -54,   160,   945,   756,
    -486,   662,   954,   731,  -486,   653,   763,   160,  1028,   765,
    1041,   381,   303,   680,   395,   -54,   538,   539,   740,  1061,
     160,   988,   382,   160,   469,   469,  1120,  -486,   745,   270,
     172,   825,   829,  -256,   751,  -581,   515,  -246,   793,   755,
     172,   383,   795,   796,   -54,  1114,  1175,   531,   799,  -580,
     826,   830,   681,   172,  -243,   303,   172,   839,   682,   839,
     925,   928,  -581,   740,   930,   932,   908,  -441,   799,   274,
     396,   303,   160,   303,   397,   399,  -580,   815,   799,   818,
     403,   820,   822,   405,   531,  -250,  -441,   408,   469,   341,
    1135,   341,   185,    42,   838,   838,  -313,  -254,   557,   160,
    -477,  -251,  1164,  1113,   160,   172,   559,   850,  1001,   318,
     318,   292,  -109,  -477,  -630,  -313,  -630,   814,    57,   515,
    -114,   399,   721,   975,   977,  -313,   403,   293,   404,   405,
     185,   469,   172,   408,  -245,   310,   823,   172,  -249,   311,
    1188,   415,  -255,  -477,  -466,  -257,  -244,   419,   420,   421,
    -627,  -252,  -627,   453,  -629,   505,  -629,  -628,   860,  -628,
     478,   481,   483,   487,   489,   492,   349,  1208,   361,  -477,
    1155,   868,  -564,   869,  -466,   809,  -477,   811,   362,   389,
     391,  -563,   390,   137,   160,  -568,   533,  -466,  -569,   452,
    -566,  -565,   536,   137,   444,   446,  1156,  -477,  1157,  1158,
    -466,   453,  1159,  1160,   458,   468,   137,  -481,   994,   137,
     546,  1015,   475,   351,   544,   477,  -477,   172,   504,   534,
    -481,  -477,   535,   576,  -477,   547,   554,   555,   585,  1208,
     744,   898,   750,   901,   763,   303,   904,   818,   508,  1245,
     595,   958,   906,   907,   596,   597,   303,  -466,   303,   913,
    -481,   915,  -466,   602,  1001,   720,   599,   922,   137,   509,
     606,   658,   660,   510,   911,   661,   933,   934,   160,  1208,
     405,   897,   701,   900,   254,   160,  -481,   727,   738,   739,
     740,   741,   747,  -481,   758,   137,   511,   761,   764,   512,
     137,   347,   759,   309,   313,   344,   766,   767,   775,   303,
     513,   172,   776,   791,  -481,   804,  1008,   540,   172,  -483,
     797,   469,   803,   508,   842,   813,   819,   960,   856,   514,
     858,  -134,  -483,  -481,   469,  -141,  -110,  -140,  -481,   160,
    -139,  -481,  -136,   582,   509,  -143,  -108,   983,   510,   985,
     838,   838,  -144,  -138,   160,  -142,  1165,  1166,  -111,  -137,
    -145,   861,  -483,   303,   862,  -112,   870,   871,   872,   733,
     344,   511,   172,   873,   512,   874,    61,   935,   937,   875,
     137,   949,  1001,   988,   877,   513,   963,   172,  -483,   657,
     876,   905,   -77,   989,   991,  -483,  1023,   748,   720,   968,
     992,   753,   996,   754,   514,  1016,   757,  1026,  1017,  1029,
    1055,  1084,   668,   710,  1022,  1056,  -483,  1025,  1057,  1081,
    1085,   981,   683,   303,  1090,  1107,  1095,  1099,  1108,  1088,
    1115,  1118,  1123,  1143,  1037,  -483,  1039,  -107,  -109,   469,
    -483,  1044,  1169,  -483,  1047,  1184,  1221,  1222,  1226,  1227,
    1228,  1229,   667,   709,  1154,   395,  1053,   980,   160,   579,
    1246,   502,   219,  1247,   137,   323,  1117,  1058,   552,  1089,
    1076,   137,   592,  1014,  1005,  1010,   947,   774,  1191,  1219,
    1212,  1009,   867,   299,   160,   355,   794,   955,     0,     0,
     160,   172,  1094,     0,  1098,     0,     0,  1100,   521,   469,
     838,   160,     0,   160,     0,     0,  1109,     0,  1110,     0,
       0,   396,     0,     0,     0,   397,     0,   172,     0,     0,
       0,     0,     0,   172,     0,   137,  1149,     0,     0,     0,
       0,     0,     0,     0,   172,     0,   172,     0,     0,     0,
     137,  1037,  1039,     0,  1044,  1047,     0,  1094,  1098,     0,
       0,     0,     0,  1136,  1137,     0,  1138,     0,   720,     0,
    1140,  1141,   399,   400,  1142,   401,   402,   403,     0,   404,
     405,     0,   160,     0,   408,     0,   160,     0,   395,     0,
     344,     0,   415,     0,   160,  1151,     0,     0,   419,   420,
     421,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1162,   818,   303,   303,     0,   172,     0,     0,  1170,   172,
    1171,     0,     0,     0,  1173,     0,     0,   172,     0,     0,
    1177,   838,   838,   838,   838,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,   397,   818,
       0,     0,     0,     0,   137,  1136,  1192,  1193,  1140,  1194,
    1195,  1196,  1197,     0,     0,     0,     0,     0,   917,   919,
       0,     0,     0,     0,   924,   927,     0,     0,   929,   931,
     137,  1211,     0,     0,     0,     0,   137,   395,   469,   469,
       0,     0,     0,     0,     0,   399,   400,   137,     0,   137,
     403,     0,   404,   405,     0,     0,     0,   408,     0,   149,
    1230,     0,     0,     0,     0,   415,     0,     0,     0,   149,
       0,   419,   420,   421,     0,     0,  1238,  1239,  1240,  1241,
       0,     0,   149,     0,   893,   149,     0,  1242,   818,     0,
       0,     0,     0,   396,     0,     0,     0,   397,  1098,     0,
       0,     0,  1248,  1249,     0,     0,     0,     0,     0,     0,
    1238,  1239,  1240,  1241,  1248,  1249,     0,     0,   137,     0,
       0,     0,   137,     0,     0,     0,   160,     0,  1098,   160,
     137,   165,     0,     0,   149,     0,     0,     0,     0,     0,
       0,   165,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,   165,     0,   408,   165,     0,   172,
       0,   149,   172,   414,   415,     0,   149,   418,     0,     0,
     419,   420,   421,     0,     0,     0,     0,     0,   160,  1032,
    1033,   422,     0,     0,   160,     0,  1034,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1045,     0,
       0,  1048,     0,  1049,     0,  1050,   165,     0,     0,     0,
       0,   172,     0,     0,     0,   462,     0,   172,     0,    22,
      23,     0,     0,     0,     0,     0,     0,     0,     0,   463,
       0,    31,   464,   165,     0,     0,     0,    37,   165,     0,
       0,     0,     0,     0,    42,     0,   149,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -53,  1020,
    1021,     0,     0,     0,     0,     0,     0,     0,     0,    57,
       0,    59,     0,     0,     0,    63,     0,   -53,    64,     0,
     465,   160,    67,   160,     0,     0,     0,     0,   160,   297,
       0,   160,     0,     0,     0,     0,     0,     0,     0,     0,
     160,    83,   160,     0,    85,   160,     0,    87,     0,     0,
       0,     0,   137,     0,   172,   137,   172,     0,   165,     0,
       0,   172,     0,     0,   172,   297,   338,   297,     0,   160,
     149,     0,     0,   172,     0,   172,     0,   149,   172,   160,
       0,   160,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   102,     0,     0,
       0,  1168,   172,   169,   137,     0,     0,     0,     0,     0,
     137,     0,   172,   169,   172,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   169,     0,     0,   169,
       0,   149,     0,     0,     0,     0,     0,     0,     0,     0,
     160,     0,   165,     0,     0,     0,   149,   160,     0,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     160,     0,     0,   297,   297,   462,   170,     0,     0,    22,
      23,     0,     0,   172,     0,     0,   170,     0,   169,   463,
     172,    31,   464,     0,     0,     0,     0,    37,     0,   170,
       0,   160,   170,   172,    42,     0,     0,     0,     0,     0,
     160,     0,     0,   165,   160,   169,     0,   137,     0,   137,
     169,     0,     0,     0,   137,     0,     0,   137,   165,    57,
       0,    59,     0,     0,   172,     0,   137,     0,   137,     0,
     465,   137,    67,   172,     0,     0,     0,   172,     0,     0,
       0,   170,     0,     0,     0,     0,     0,     0,   160,     0,
     149,    83,     0,     0,    85,   137,     0,    87,     0,     0,
       0,     0,     0,     0,     0,   137,     0,   137,   170,     0,
       0,     0,     0,   170,     0,     0,   149,     0,     0,     0,
       0,   172,   149,     0,     0,   395,     0,     0,     0,     0,
     169,     0,     0,   149,     0,   149,     0,     0,     0,     0,
       0,     0,     0,   160,   160,   160,   160,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   165,   297,     0,     0,   137,     0,     0,   160,
     160,     0,     0,   137,     0,     0,   172,   172,   172,   172,
       0,   396,     0,     0,     0,   397,   137,     0,   165,     0,
     395,     0,     0,   170,   165,     0,     0,     0,     0,     0,
       0,     0,   172,   172,   149,   165,     0,   165,   149,     0,
       0,     0,     0,     0,   169,     0,   149,   137,     0,     0,
       0,   169,     0,     0,     0,     0,   137,     0,     0,     0,
     137,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,     0,     0,     0,   408,     0,   396,   174,     0,     0,
     397,   414,   415,     0,     0,   418,     0,   174,   419,   420,
     421,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     174,     0,     0,   174,   137,   169,   165,   170,   297,     0,
     165,   398,     0,     0,   170,     0,     0,     0,   165,     0,
     169,     0,     0,     0,     0,     0,     0,   399,   400,     0,
     401,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,   410,     0,     0,   412,   413,   414,   415,   416,   417,
     418,     0,   174,   419,   420,   421,     0,     0,     0,   137,
     137,   137,   137,     0,   422,     0,     0,     0,   170,     0,
       0,   798,     0,     0,     0,     0,     0,     0,     0,   174,
       0,     0,     0,   170,   174,   137,   137,     0,     0,     0,
       0,   798,     0,     0,   338,     0,   338,     0,     0,     0,
       0,   798,     0,   462,     0,     0,     0,    22,    23,     0,
       0,     0,   997,     0,     0,     0,     0,   463,     0,    31,
     464,     0,     0,     0,   169,    37,     0,     0,   149,     0,
       0,   149,    42,     0,   423,   424,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,   436,   437,     0,
     169,     0,     0,     0,     0,     0,   169,    57,     0,    59,
       0,    61,     0,   998,   174,   175,   999,   169,   465,   169,
      67,     0,     0,     0,     0,   175,     0,     0,   438,     0,
     149,  -627,     0,  -627,     0,     0,   149,   170,   175,    83,
       0,   175,    85,     0,     0,    87,     0,     0,     0,     0,
     165,     0,     0,   165,   677,     0,     0,     0,     0,     0,
       0,     0,     0,   170,     0,     0,     0,     0,   882,   170,
       0,   883,     0,     0,     0,     0,   884,     0,     0,     0,
     170,     0,   170,     0,     0,     0,     0,   195,   169,     0,
     175,     0,   169,     0,     0,   102,     0,   885,   174,     0,
     169,  1000,   165,     0,   886,   174,     0,     0,   165,     0,
       0,     0,     0,     0,   887,     0,     0,   175,     0,   910,
       0,   888,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   149,     0,   149,     0,   889,     0,     0,
     149,     0,     0,   149,     0,     0,     0,     0,     0,   890,
       0,   170,   149,   681,   149,   170,     0,   149,     0,   174,
     891,     0,     0,   170,     0,     0,   892,     0,   297,     0,
       0,     0,     0,     0,   174,     0,     0,     0,     0,     0,
       0,   149,     0,     0,   462,     0,     0,     0,    22,    23,
       0,   149,     0,   149,     0,     0,     0,     0,   463,   110,
      31,   464,   175,     0,     0,   165,    37,   165,     0,   186,
       0,     0,   165,    42,     0,   165,     0,     0,     0,     0,
       0,     0,   220,     0,   165,   225,   165,     0,     0,   165,
       0,     0,     0,   297,     0,     0,     0,     0,    57,     0,
      59,     0,     0,     0,   998,     0,     0,   999,     0,   465,
       0,    67,   149,   165,     0,     0,     0,     0,     0,   149,
       0,     0,     0,   165,     0,   165,     0,     0,     0,     0,
      83,     0,   149,    85,   312,     0,    87,     0,   174,     0,
       0,     0,   169,     0,     0,   169,   175,     0,     0,     0,
       0,     0,     0,   175,     0,     0,     0,     0,     0,     0,
       0,   110,     0,   149,   174,     0,   348,     0,     0,     0,
     174,     0,   149,     0,     0,     0,   149,     0,     0,     0,
       0,   174,     0,   174,   165,     0,   102,     0,     0,     0,
    1064,   165,     0,     0,   169,     0,   251,     0,     0,     0,
     169,     0,     0,     0,   165,   170,   297,   175,   170,     0,
     271,   272,   273,     0,     0,     0,     0,     0,     0,     0,
     149,     0,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   462,     0,   165,     0,    22,    23,     0,
       0,     0,   997,     0,   165,     0,   445,   463,   165,    31,
     464,     0,   174,     0,     0,    37,   174,   170,     0,     0,
       0,     0,    42,   170,   174,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   149,   149,   149,   149,     0,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,   347,   165,   998,     0,     0,   999,   169,   465,   169,
      67,   149,   149,   297,   169,     0,     0,   169,  1150,     0,
       0,     0,   364,     0,   367,     0,   169,     0,   169,    83,
     395,   169,    85,   385,     0,    87,   175,     0,     0,     0,
     507,     0,     0,     0,     0,     0,     0,   532,     0,     0,
       0,     0,     0,     0,     0,   169,     0,   165,   165,   165,
     165,     0,   175,     0,     0,   169,     0,   169,   175,     0,
     170,     0,   170,     0,     0,   450,     0,   170,     0,   175,
     170,   175,     0,   165,   165,   102,   396,     0,     0,   170,
     397,   170,     0,     0,   170,     0,     0,     0,     0,  1205,
       0,   110,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   110,     0,   170,     0,
       0,     0,     0,     0,     0,     0,   169,     0,   170,     0,
     170,   476,     0,   169,     0,     0,     0,   399,   400,   494,
     495,   402,   403,     0,   404,   405,   169,     0,     0,   408,
     175,     0,   524,     0,   175,     0,   174,   415,     0,   174,
       0,  1205,   175,   419,   420,   421,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   169,     0,     0,
       0,     0,     0,     0,     0,     0,   169,     0,     0,   170,
     169,     0,     0,     0,     0,   281,   170,     0,     0,     0,
       0,  1205,     0,     0,     0,     0,     0,     0,   174,   170,
       0,     0,     0,     0,   174,     0,     0,   281,     0,     0,
     110,     0,     0,     0,     0,     0,     0,   315,   319,     0,
       0,     0,     0,     0,   169,     0,     0,   593,     0,     0,
     170,     0,     0,     0,     0,     0,   532,     0,     0,   170,
       0,     0,   532,   170,     0,     0,     0,     0,     0,     0,
     652,     0,     0,   760,     0,   423,   424,   425,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,   437,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   169,
     169,   169,   169,     0,     0,     0,  -588,   170,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   438,
       0,   174,  -627,   174,  -627,   169,   169,     0,   174,     0,
       0,   174,     0,     0,     0,     0,     0,     0,   730,     0,
     174,     0,   174,     0,   812,   174,     0,     0,   817,   251,
       0,     0,     0,     0,   175,     0,   110,   175,     0,     0,
       0,     0,   170,   170,   170,   170,     0,     0,     0,   174,
     454,     0,     0,     0,     0,     0,     0,     0,     0,   174,
       0,   174,     0,     0,     0,     0,     0,     0,   170,   170,
       0,   769,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   778,   175,     0,     0,   524,
       0,   846,   175,   281,   281,   281,   281,   281,   281,     0,
       0,   497,   500,     0,     0,     0,     0,     0,   506,     0,
       0,     0,  -295,     0,     0,     0,  -295,  -295,     0,   281,
     174,  -295,     0,     0,     0,   281,  -295,   174,  -295,  -295,
       0,     0,     0,     0,  -295,     0,     0,     0,     0,     0,
     174,  -295,     0,   281,  -295,     0,     0,     0,     0,     0,
       0,   834,     0,     0,     0,     0,     0,     0,     0,   281,
     281,     0,     0,  -295,   367,     0,  -295,     0,  -295,     0,
    -295,   174,  -295,  -295,     0,  -295,     0,  -295,     0,  -295,
     174,     0,     0,     0,   174,     0,     0,     0,     0,   175,
       0,   175,     0,     0,     0,     0,   175,     0,  -295,   175,
       0,  -295,     0,     0,  -295,     0,     0,     0,   175,     0,
     175,     0,     0,   175,     0,     0,     0,     0,   936,     0,
       0,   939,     0,     0,     0,     0,     0,   385,   174,     0,
       0,     0,     0,     0,     0,     0,     0,   175,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   175,     0,   175,
       0,     0,     0,     0,  -295,     0,     0,     0,     0,     0,
    -295,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     973,     0,     0,     0,     0,     0,   978,     0,     0,     0,
       0,     0,     0,   174,   174,   174,   174,     0,     0,     0,
     462,     0,   281,     0,    22,    23,     0,     0,     0,   997,
       0,     0,     0,     0,   463,     0,    31,   464,   175,   174,
     174,     0,    37,     0,     0,   175,     0,     0,     0,    42,
     281,     0,     0,     0,   281,     0,   281,     0,   175,   281,
       0,     0,   942,     0,     0,     0,     0,     0,     0,     0,
     946,     0,     0,   524,    57,     0,    59,   524,    61,     0,
     998,     0,     0,   999,     0,   465,     0,    67,     0,   175,
       0,     0,     0,     0,     0,     0,     0,     0,   175,     0,
     971,     0,   175,   532,     0,   532,    83,     0,     0,    85,
     532,     0,    87,   532,     0,     0,     0,     0,     0,     0,
       0,     0,  1051,     0,  1052,     0,     0,  1054,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,     0,   175,   367,     0,     0,
       0,  1087,     0,     0,     0,     0,     0,     0,   652,     0,
       0,  1093,   102,  1097,     0,     0,     0,     0,  1112,   227,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   228,   229,     0,   230,     0,     0,     0,
       0,   231,     0,     0,     0,     0,     0,     0,     0,     0,
     232,   175,   175,   175,   175,     0,   233,     0,     0,     0,
       0,     0,   234,     0,     0,     0,   235,     0,     0,   236,
       0,     0,     0,     0,     0,     0,     0,   175,   175,   237,
       0,     0,     0,     0,     0,   238,   239,     0,     0,     0,
     769,     0,  1144,   240,     0,     0,     0,     0,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
       0,   242,   243,     0,   244,   971,   245,     0,   246,     0,
       0,   247,     0,  1167,   607,   248,   608,     0,   249,     0,
       0,   250,  1172,     0,     0,   609,  1174,     0,     0,     0,
     610,   229,   611,   612,     0,     0,     0,     0,   613,     0,
       0,     0,     0,     0,     0,     0,     0,   232,     0,   195,
       0,   281,   281,   614,     0,     0,     0,   281,   281,   615,
       0,   281,   281,   235,     0,     0,   616,     0,   617,     0,
    1202,   941,     0,     0,     0,   453,   618,   969,     0,     0,
       0,     0,   619,   620,     0,     0,     0,     0,     0,     0,
     240,     0,     0,     0,     0,     0,     0,     0,     0,   621,
       0,     0,     0,     0,     0,     0,     0,     0,   242,   243,
     970,   622,     0,   245,     0,   623,     0,     0,   624,     0,
       0,     0,   625,     0,     0,   249,     0,     0,   626,     0,
       0,     0,     0,     0,     0,     0,     0,   499,   424,   425,
     426,   427,   428,     0,     0,   431,   432,   433,   434,     0,
     436,   437,   627,   628,   629,   630,   631,   632,     0,   633,
       0,     0,     0,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,   644,     0,   645,     0,     0,   646,   647,
     648,   649,     0,     0,   650,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   281,   281,     0,     0,     0,     0,     0,   281,
       0,   500,     0,     0,     0,     0,     0,     0,   500,     0,
       0,   281,     0,     0,   281,     0,   281,     0,   281,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1059,     0,     0,     0,     0,     0,    -2,     4,     0,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,  1091,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -53,     0,    40,    41,    42,     0,
      43,  -313,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -53,    52,    53,     0,    54,    55,    56,     0,
    -313,     0,     0,    57,    58,    59,    60,    61,    62,    63,
    -313,   -53,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -53,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   281,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,     0,   105,     0,
     106,     0,     0,   107,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   500,   500,     0,     0,   500,   500,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1223,     4,   500,     5,   500,
       6,     7,     8,     9,    10,    11,     0,  -658,     0,    12,
      13,    14,    15,    16,  -658,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,  -658,    28,    29,
    -658,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -53,     0,    40,    41,    42,     0,    43,  -313,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
     -53,    52,    53,     0,    54,    55,    56,     0,  -313,     0,
       0,    57,    58,    59,    60,     0,    62,    63,  -313,   -53,
      64,    65,    66,  -658,    67,    68,    69,  -658,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,     0,
      80,    81,    82,    83,    84,     0,    85,    86,   -53,    87,
      88,     0,     0,    89,     0,    90,     0,     0,  -658,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -658,  -658,    94,  -658,  -658,  -658,
    -658,  -658,  -658,  -658,     0,  -658,  -658,  -658,  -658,  -658,
       0,  -658,  -658,  -658,  -658,  -658,  -658,  -658,  -658,   102,
    -658,  -658,  -658,     0,   104,  -658,   105,     0,   106,     0,
     331,  -658,     5,   296,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -53,     0,    40,    41,
      42,     0,    43,  -313,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -53,    52,    53,     0,    54,    55,
      56,     0,  -313,     0,     0,    57,    58,    59,    60,    61,
      62,    63,  -313,   -53,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -53,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,   332,
     105,     0,   106,     0,     4,   107,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -53,     0,    40,    41,    42,     0,    43,  -313,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -53,    52,
      53,     0,    54,    55,    56,     0,  -313,     0,     0,    57,
      58,    59,    60,    61,    62,    63,  -313,   -53,    64,    65,
      66,     0,    67,    68,    69,     0,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -53,    87,    88,     0,
       0,    89,     0,    90,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,   102,     0,     0,
     103,     0,   104,   558,   105,     0,   106,     0,   577,   107,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -53,     0,    40,    41,    42,     0,
      43,  -313,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -53,    52,    53,     0,    54,    55,    56,     0,
    -313,     0,     0,    57,    58,    59,    60,    61,    62,    63,
    -313,   -53,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -53,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,   578,   105,     0,
     106,     0,   331,   107,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -53,     0,
      40,    41,    42,     0,    43,  -313,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,   -53,    52,    53,     0,
      54,    55,    56,     0,  -313,     0,     0,    57,    58,    59,
      60,    61,    62,    63,  -313,   -53,    64,    65,    66,     0,
      67,    68,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,     0,    80,    81,    82,    83,
      84,     0,    85,    86,   -53,    87,    88,     0,     0,    89,
       0,    90,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,   102,     0,     0,   103,     0,
     104,   332,   105,     0,   106,     0,     4,   107,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -53,     0,    40,    41,    42,     0,    43,  -313,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
     -53,    52,    53,     0,    54,    55,    56,     0,  -313,     0,
       0,    57,    58,    59,    60,    61,    62,    63,  -313,   -53,
      64,    65,    66,     0,    67,    68,    69,     0,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,     0,
      80,    81,    82,    83,    84,     0,    85,    86,   -53,    87,
      88,     0,     0,    89,     0,    90,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   102,
       0,     0,   103,     0,   104,   824,   105,     0,   106,     0,
       4,   107,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,    38,    39,   -53,     0,    40,    41,
      42,     0,    43,  -313,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -53,    52,    53,     0,    54,    55,
      56,     0,  -313,     0,     0,    57,    58,    59,    60,   347,
      62,    63,  -313,   -53,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -53,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,     0,
     105,     0,   106,     0,     4,   107,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -53,     0,    40,    41,    42,     0,    43,  -313,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -53,    52,
      53,     0,    54,    55,    56,     0,  -313,     0,     0,    57,
      58,    59,    60,     0,    62,    63,  -313,   -53,    64,    65,
      66,     0,    67,    68,    69,     0,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -53,    87,    88,     0,
       0,    89,     0,    90,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,   102,     0,     0,
     103,     0,   104,     0,   105,     0,   106,     0,     4,   107,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,   743,    39,   -53,     0,    40,    41,    42,     0,
      43,  -313,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -53,    52,    53,     0,    54,    55,    56,     0,
    -313,     0,     0,    57,    58,    59,    60,     0,    62,    63,
    -313,   -53,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -53,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,     0,   105,     0,
     106,     0,     4,   107,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,   749,    39,   -53,     0,
      40,    41,    42,     0,    43,  -313,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,   -53,    52,    53,     0,
      54,    55,    56,     0,  -313,     0,     0,    57,    58,    59,
      60,     0,    62,    63,  -313,   -53,    64,    65,    66,     0,
      67,    68,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,     0,    80,    81,    82,    83,
      84,     0,    85,    86,   -53,    87,    88,     0,     0,    89,
       0,    90,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,   102,     0,     0,   103,     0,
     104,     0,   105,     0,   106,     0,     4,   107,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1036,    39,   -53,     0,    40,    41,    42,     0,    43,  -313,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
     -53,    52,    53,     0,    54,    55,    56,     0,  -313,     0,
       0,    57,    58,    59,    60,     0,    62,    63,  -313,   -53,
      64,    65,    66,     0,    67,    68,    69,     0,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,     0,
      80,    81,    82,    83,    84,     0,    85,    86,   -53,    87,
      88,     0,     0,    89,     0,    90,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   102,
       0,     0,   103,     0,   104,     0,   105,     0,   106,     0,
       4,   107,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,  1038,    39,   -53,     0,    40,    41,
      42,     0,    43,  -313,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -53,    52,    53,     0,    54,    55,
      56,     0,  -313,     0,     0,    57,    58,    59,    60,     0,
      62,    63,  -313,   -53,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -53,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,     0,
     105,     0,   106,     0,     4,   107,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1043,    39,
     -53,     0,    40,    41,    42,     0,    43,  -313,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,   -53,    52,
      53,     0,    54,    55,    56,     0,  -313,     0,     0,    57,
      58,    59,    60,     0,    62,    63,  -313,   -53,    64,    65,
      66,     0,    67,    68,    69,     0,    70,    71,    72,    73,
      74,    75,    76,    77,     0,    78,    79,     0,    80,    81,
      82,    83,    84,     0,    85,    86,   -53,    87,    88,     0,
       0,    89,     0,    90,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,   102,     0,     0,
     103,     0,   104,     0,   105,     0,   106,     0,     4,   107,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1046,    39,   -53,     0,    40,    41,    42,     0,
      43,  -313,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,   -53,    52,    53,     0,    54,    55,    56,     0,
    -313,     0,     0,    57,    58,    59,    60,     0,    62,    63,
    -313,   -53,    64,    65,    66,     0,    67,    68,    69,     0,
      70,    71,    72,    73,    74,    75,    76,    77,     0,    78,
      79,     0,    80,    81,    82,    83,    84,     0,    85,    86,
     -53,    87,    88,     0,     0,    89,     0,    90,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,   102,     0,     0,   103,     0,   104,     0,   105,     0,
     106,     0,     4,   107,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,  1086,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -53,     0,
      40,    41,    42,     0,    43,  -313,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,   -53,    52,    53,     0,
      54,    55,    56,     0,  -313,     0,     0,    57,    58,    59,
      60,     0,    62,    63,  -313,   -53,    64,    65,    66,     0,
      67,    68,    69,     0,    70,    71,    72,    73,    74,    75,
      76,    77,     0,    78,    79,     0,    80,    81,    82,    83,
      84,     0,    85,    86,   -53,    87,    88,     0,     0,    89,
       0,    90,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,   102,     0,     0,   103,     0,
     104,     0,   105,     0,   106,     0,     4,   107,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
      13,    14,    15,    16,     0,    17,     0,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1092,    39,   -53,     0,    40,    41,    42,     0,    43,  -313,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
     -53,    52,    53,     0,    54,    55,    56,     0,  -313,     0,
       0,    57,    58,    59,    60,     0,    62,    63,  -313,   -53,
      64,    65,    66,     0,    67,    68,    69,     0,    70,    71,
      72,    73,    74,    75,    76,    77,     0,    78,    79,     0,
      80,    81,    82,    83,    84,     0,    85,    86,   -53,    87,
      88,     0,     0,    89,     0,    90,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   102,
       0,     0,   103,     0,   104,     0,   105,     0,   106,     0,
       4,   107,     5,     0,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,    13,    14,    15,    16,     0,    17,
       0,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     0,    28,    29,     0,    30,     0,    31,    32,    33,
      34,    35,    36,    37,  1096,    39,   -53,     0,    40,    41,
      42,     0,    43,  -313,     0,    44,    45,    46,    47,    48,
       0,    49,    50,    51,   -53,    52,    53,     0,    54,    55,
      56,     0,  -313,     0,     0,    57,    58,    59,    60,     0,
      62,    63,  -313,   -53,    64,    65,    66,     0,    67,    68,
      69,     0,    70,    71,    72,    73,    74,    75,    76,    77,
       0,    78,    79,     0,    80,    81,    82,    83,    84,     0,
      85,    86,   -53,    87,    88,     0,     0,    89,     0,    90,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,   102,     0,     0,   103,     0,   104,     0,
     105,     0,   106,     0,  1006,   107,     5,   296,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     0,   203,  1007,   957,   107,
       5,   296,     6,     7,     8,     9,    10,    11,     0,     0,
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
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   202,     0,   105,     0,
     203,     0,     0,   107,     5,   296,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,     0,     0,    80,   300,   301,     0,
      84,   336,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
     337,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     202,     0,   105,     0,   203,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,   354,
      23,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,    59,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,    67,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,    87,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     0,   203,     0,     0,   107,
       5,   296,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,   300,   301,     0,    84,   336,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   202,     0,   105,   808,
     203,     0,     0,   107,     5,   296,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,     0,     0,    80,   300,   301,     0,
      84,   336,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     202,     0,   105,   810,   203,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,     0,
     713,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,   714,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,   715,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,   716,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     0,   203,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   193,
       0,     0,    15,    16,     0,    17,     0,   194,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   195,     0,     0,     0,    33,   196,   197,     0,     0,
     198,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   199,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   200,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   201,     0,    78,     0,     0,
      80,   300,   301,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,   302,
       0,     0,   103,     0,   202,     0,   105,     0,   203,     0,
       0,   107,     5,   296,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   193,     0,     0,    15,    16,     0,    17,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   195,     0,     0,     0,    33,
     196,   197,     0,     0,   198,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   199,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   200,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   201,
       0,    78,     0,     0,    80,   300,   301,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   202,     0,
     105,     0,   203,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,   959,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,     0,     0,    80,   300,   301,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     202,     0,   105,     0,   203,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,   255,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,   256,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     0,   203,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   193,
       0,     0,    15,    16,     0,    17,     0,   194,     0,     0,
      21,     0,   263,     0,     0,     0,     0,     0,     0,    29,
       0,   195,     0,     0,     0,    33,   196,   197,     0,     0,
     198,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   199,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   200,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   201,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,   264,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   202,     0,   105,     0,   203,     0,
       0,   107,     5,   296,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   193,     0,     0,    15,    16,     0,    17,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   195,     0,     0,     0,    33,
     196,   197,     0,     0,   198,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   199,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   200,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   201,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
     256,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   202,     0,
     105,     0,   203,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,     0,     0,    80,   300,   301,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     202,     0,   105,     0,   203,     0,     0,   107,     5,   296,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   193,
       0,     0,    15,    16,     0,    17,     0,   194,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   195,     0,     0,     0,    33,   196,   197,     0,     0,
     198,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   199,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   200,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   201,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   202,   806,   105,     0,   203,     0,
       0,   107,     5,   296,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   193,     0,     0,    15,    16,     0,    17,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   195,     0,     0,     0,    33,
     196,   197,     0,     0,   198,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   199,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   200,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   201,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   202,     0,
     105,     0,   203,   816,     0,   107,     5,   296,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     0,   203,  1148,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   193,
       0,     0,    15,    16,     0,    17,     0,   194,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   195,     0,     0,     0,    33,   196,   197,     0,     0,
     198,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   199,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   200,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   201,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,   256,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   202,     0,   105,     0,   203,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
     280,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   104,     0,   105,     0,
     203,     0,     0,   107,     5,   296,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     202,     0,   105,     0,   203,     0,     5,   107,     6,     7,
       8,   357,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,    18,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
      66,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,    85,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     0,   203,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   193,
       0,     0,    15,    16,     0,    17,     0,   194,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   195,     0,     0,     0,    33,   196,   197,     0,     0,
     198,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   199,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   200,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   201,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   202,     0,   105,     0,   203,   821,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,     0,     0,    84,   984,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   202,     0,   105,     0,
     203,     0,     0,   107,     5,   296,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   783,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,   784,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,   785,    74,    75,
      76,   786,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     202,     0,   105,     0,  1062,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,  1176,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     0,   203,     0,     0,   107,
       5,   296,     6,     7,     8,     9,    10,    11,     0,     0,
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
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   202,     0,   105,     0,
    1062,     0,     0,   107,     5,   296,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,  1134,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     202,     0,   105,     0,   203,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     0,   203,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   193,
       0,     0,    15,    16,     0,    17,     0,   194,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   195,     0,     0,     0,    33,   196,   197,     0,     0,
     198,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   199,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   200,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   201,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   104,     0,   105,     0,   203,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,   783,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   198,    39,     0,     0,     0,   784,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,   785,    74,    75,    76,   786,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   202,     0,   105,     0,
     787,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   193,     0,     0,    15,    16,     0,    17,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   195,     0,     0,     0,    33,
     196,   197,     0,     0,   198,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   199,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   200,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   201,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   835,     0,   202,     0,
     105,     0,   203,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,   783,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,   784,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,   785,    74,    75,
      76,   786,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     202,     0,   105,     0,   852,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     0,   787,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   193,
       0,     0,    15,    16,     0,    17,     0,   194,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   195,     0,     0,     0,    33,   196,   197,     0,     0,
     896,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   199,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   200,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   201,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   202,     0,   105,     0,   203,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,   899,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   202,     0,   105,     0,
     203,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   193,     0,     0,    15,    16,     0,    17,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   195,     0,     0,     0,    33,
     196,   197,     0,     0,  1128,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   199,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   200,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   201,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   202,     0,
     105,     0,   203,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,  1129,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
     202,     0,   105,     0,   203,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,  1131,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,    91,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,    99,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     0,   203,     0,     5,   107,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   193,
       0,     0,    15,    16,     0,    17,     0,   194,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   195,     0,     0,     0,    33,   196,   197,     0,     0,
    1132,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   199,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   200,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   201,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,    97,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,    99,   100,   101,     0,
       0,     0,   103,     0,   202,     0,   105,     0,   203,     0,
       5,   107,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   193,     0,     0,    15,    16,     0,    17,     0,   194,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   195,     0,     0,     0,    33,   196,   197,
       0,     0,  1133,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   199,     0,     0,    47,    48,     0,     0,
      50,    51,     0,    52,    53,     0,    54,    55,     0,     0,
       0,     0,     0,     0,    58,     0,    60,     0,    62,     0,
       0,     0,     0,    65,   200,     0,     0,     0,     0,     0,
       0,     0,    72,    73,    74,    75,    76,   201,     0,    78,
       0,     0,    80,     0,     0,     0,    84,     0,     0,    86,
       0,     0,    88,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    92,    93,    94,    95,
      96,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,     0,     0,    99,   100,
     101,     0,     0,     0,   103,     0,   202,     0,   105,     0,
     203,     0,     5,   107,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   193,     0,     0,    15,    16,     0,    17,
       0,   194,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   195,     0,     0,     0,    33,
     196,   197,     0,     0,  1134,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   199,     0,     0,    47,    48,
       0,     0,    50,    51,     0,    52,    53,     0,    54,    55,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
      62,     0,     0,     0,     0,    65,   200,     0,     0,     0,
       0,     0,     0,     0,    72,    73,    74,    75,    76,   201,
       0,    78,     0,     0,    80,     0,     0,     0,    84,     0,
       0,    86,     0,     0,    88,     0,     0,     0,     0,     0,
       0,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    92,    93,
      94,    95,    96,     0,     0,     0,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    98,     0,     0,
      99,   100,   101,     0,     0,     0,   103,     0,   202,     0,
     105,     0,   203,     0,     5,   107,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   193,     0,     0,    15,    16,
       0,    17,     0,   194,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   195,     0,     0,
       0,    33,   196,   197,     0,     0,   198,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   199,     0,     0,
      47,    48,     0,     0,    50,    51,     0,    52,    53,     0,
      54,    55,     0,     0,     0,     0,     0,     0,    58,     0,
      60,     0,    62,     0,     0,     0,     0,    65,   200,     0,
       0,     0,     0,     0,     0,     0,    72,    73,    74,    75,
      76,   201,     0,    78,     0,     0,    80,     0,     0,     0,
      84,     0,     0,    86,     0,     0,    88,     0,     0,     0,
       0,     0,     0,     0,    91,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      92,    93,    94,    95,    96,     0,     0,     0,     0,    97,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    98,
       0,     0,    99,   100,   101,     0,     0,     0,   103,     0,
       0,     0,   105,     0,   852,     0,     5,   107,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,    93,    94,    95,    96,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    98,     0,     0,     0,   100,   101,     0,     0,     0,
     103,     0,   202,     0,   105,     5,   203,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,  -467,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,  -467,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,  -467,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,  -467,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     5,   296,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   193,     0,     0,    15,
      16,     0,    17,     0,   194,   101,  -467,    21,     0,     0,
       0,  -467,     0,   105,     0,   852,    29,     0,   195,     0,
       0,     0,    33,   196,   197,     0,     0,   198,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   199,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   200,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   201,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,  -485,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,  -485,     0,     0,
       0,  -485,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,     0,     0,
       0,   202,     0,   105,  -485,  1062,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    92,     0,    94,     0,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,   101,     0,    21,     0,
       0,     0,   202,     0,   105,     0,   203,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,     0,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,     0,     0,    80,     0,
       0,     0,    84,     0,     0,    86,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    94,     0,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   193,     0,     0,
      15,    16,     0,    17,     0,   194,   101,     0,    21,     0,
       0,     0,   202,     0,   105,     0,   787,    29,     0,   195,
       0,     0,     0,    33,   196,   197,     0,     0,   198,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   199,
       0,     0,    47,    48,     0,     0,    50,    51,     0,    52,
      53,     0,    54,    55,     0,     0,     0,     0,     0,     0,
      58,     0,    60,     0,    62,     0,     0,     0,     0,    65,
     200,     0,   227,     0,     0,     0,     0,     0,    72,    73,
      74,    75,    76,   201,     0,    78,   228,   229,    80,   230,
       0,     0,    84,     0,   231,    86,     0,     0,    88,     0,
       0,     0,     0,   232,     0,     0,     0,     0,     0,   233,
       0,     0,     0,     0,     0,   234,     0,     0,     0,   235,
       0,     0,   236,     0,    94,     0,     0,     0,     0,     0,
       0,     0,   237,     0,     0,     0,     0,     0,   238,   239,
       0,     0,     0,     0,     0,     0,   240,     0,     0,     0,
       0,     0,     0,     0,   105,   241,   203,     0,   227,     0,
       0,     0,     0,     0,   242,   243,     0,   244,     0,   245,
       0,   246,   228,   229,   247,   230,     0,     0,   248,   522,
     231,   249,     0,     0,   250,     0,     0,     0,     0,   232,
       0,     0,   227,     0,     0,   233,     0,     0,     0,     0,
       0,   234,     0,     0,     0,   235,   228,   229,   236,   230,
       0,     0,     0,     0,   231,     0,     0,     0,   237,     0,
       0,     0,     0,   232,   238,   239,     0,     0,     0,   233,
       0,     0,   240,     0,     0,   234,     0,     0,     0,   235,
     523,   241,   236,     0,     0,     0,     0,     0,     0,     0,
     242,   243,   237,   244,     0,   245,     0,   246,   238,   239,
     247,     0,     0,     0,   248,     0,   240,   249,     0,     0,
     250,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,   242,   243,     0,   244,     0,   245,
       0,   246,     0,     0,   247,     0,  1012,     0,   248,     0,
       0,   249,     0,     0,   250,   609,     0,     0,     0,     0,
     228,   229,   611,   230,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,     0,     0,   276,   232,     0,     0,
       0,     0,     0,   614,     0,     0,     0,     0,     0,   234,
       0,     0,     0,   235,     0,     0,   236,     0,   617,     0,
       0,     0,     0,     0,     0,     0,   237,     0,     0,     0,
     777,     0,   619,   239,     0,     0,     0,     0,     0,     0,
     240,     0,     0,     0,     0,     0,     0,     0,     0,   241,
       0,     0,     0,     0,     0,     0,     0,   393,   242,   243,
       0,   244,     0,   245,   394,  1013,     0,     0,   624,     0,
       0,     0,   248,     0,     0,   249,     0,   395,   250,     0,
       0,     0,     0,     0,     0,     0,     0,   499,   424,   425,
     426,   427,   428,     0,     0,   431,   432,   433,   434,     0,
     436,   437,   627,   628,   629,   630,   631,   632,     0,   633,
       0,     0,     0,   634,   635,   636,   637,   638,   639,   640,
     641,   642,   643,   644,     0,   645,     0,     0,   646,   647,
     648,   649,     0,   396,     0,     0,     0,   397,     0,     0,
     393,     0,     0,     0,     0,     0,   498,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,   398,   499,
     424,   425,   426,   427,   428,     0,     0,   431,   432,   433,
     434,     0,   436,   437,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,   453,     0,   396,     0,     0,     0,
     397,   422,     0,   393,     0,     0,     0,     0,     0,  1035,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,   398,   499,   424,   425,   426,   427,   428,     0,     0,
     431,   432,   433,   434,     0,   436,   437,   399,   400,     0,
     401,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,     0,     0,   419,   420,   421,     0,   453,     0,   396,
       0,     0,     0,   397,   422,     0,   393,     0,     0,     0,
       0,     0,  1042,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,     0,     0,     0,
       0,     0,     0,     0,   398,   499,   424,   425,   426,   427,
     428,     0,     0,   431,   432,   433,   434,     0,   436,   437,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,     0,     0,   419,   420,   421,     0,
     453,     0,   396,     0,     0,     0,   397,   422,     0,   393,
       0,     0,     0,     0,     0,  1198,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,   398,   499,   424,
     425,   426,   427,   428,     0,     0,   431,   432,   433,   434,
       0,   436,   437,   399,   400,     0,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,     0,   419,
     420,   421,     0,   453,     0,   396,     0,     0,     0,   397,
     422,     0,   393,     0,     0,     0,     0,     0,  1199,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
     398,   499,   424,   425,   426,   427,   428,     0,     0,   431,
     432,   433,   434,     0,   436,   437,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
       0,     0,   419,   420,   421,     0,   453,     0,   396,     0,
       0,     0,   397,   422,     0,   393,     0,     0,     0,     0,
       0,  1200,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,   398,   499,   424,   425,   426,   427,   428,
       0,     0,   431,   432,   433,   434,     0,   436,   437,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,     0,     0,   419,   420,   421,     0,   453,
       0,   396,     0,     0,     0,   397,   422,     0,   393,     0,
       0,     0,     0,     0,  1201,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,   398,   499,   424,   425,
     426,   427,   428,     0,     0,   431,   432,   433,   434,     0,
     436,   437,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,   453,     0,   396,     0,     0,     0,   397,   422,
       0,   393,     0,     0,     0,     0,     0,  1224,   394,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,   398,
     499,   424,   425,   426,   427,   428,     0,     0,   431,   432,
     433,   434,     0,   436,   437,   399,   400,     0,   401,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,     0,
       0,   419,   420,   421,     0,   453,     0,   396,     0,     0,
       0,   397,   422,     0,   393,     0,     0,     0,     0,     0,
    1225,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,   485,     0,     0,     0,     0,
       0,     0,   398,   499,   424,   425,   426,   427,   428,     0,
     486,   431,   432,   433,   434,     0,   436,   437,   399,   400,
       0,   401,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,     0,     0,   419,   420,   421,     0,   453,     0,
     396,     0,     0,     0,   397,   422,   393,     0,     0,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,   280,     0,     0,
       0,   482,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,   491,     0,     0,     0,     0,     0,     0,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,     0,     0,   419,   420,   421,
       0,   453,   396,     0,     0,     0,   397,     0,   422,   393,
       0,     0,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
     479,     0,     0,   482,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,   480,     0,     0,     0,     0,
       0,     0,     0,   399,   400,     0,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,     0,   419,
     420,   421,     0,   453,     0,   396,     0,     0,     0,   397,
     422,     0,   393,     0,     0,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,   586,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
     410,   411,   587,   412,   413,   414,   415,   416,   417,   418,
       0,     0,   419,   420,   421,     0,   453,     0,   396,     0,
       0,     0,   397,   422,     0,   393,   831,     0,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
     832,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,   833,   412,   413,   414,   415,
     416,   417,   418,     0,   588,   419,   420,   421,     0,     0,
       0,   396,     0,     0,     0,   397,   422,   393,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,   926,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,     0,   396,     0,     0,     0,   397,     0,   422,
     393,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,   280,     0,     0,   482,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,   453,     0,   396,     0,     0,     0,
     397,   422,   393,     0,     0,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,   482,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,   455,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,     0,
     401,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,   410,   411,   395,   412,   413,   414,   415,   416,   417,
     418,     0,     0,   419,   420,   421,     0,   453,   396,     0,
       0,     0,   397,   393,   422,     0,     0,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,   280,     0,     0,     0,     0,   188,
       0,     0,     0,   398,     0,     0,     0,     0,     0,   396,
       0,     0,     0,   397,     0,     0,     0,     0,     0,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,     0,   398,   419,   420,   421,     0,   396,
       0,     0,     0,   397,     0,   393,   422,     0,     0,     0,
     399,   400,   394,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   395,   722,   412,   413,   414,
     415,   416,     0,   418,   398,     0,   419,   420,   421,     0,
       0,   723,     0,     0,     0,     0,     0,   422,     0,     0,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,     0,     0,   419,   420,   421,     0,
     453,   396,     0,     0,     0,   397,   393,   422,     0,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,   724,     0,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,   725,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,   396,     0,     0,     0,   397,   393,     0,   422,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,   918,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,     0,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,     0,   419,
     420,   421,     0,   396,     0,     0,     0,   397,     0,   393,
     422,     0,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,   453,   396,     0,     0,     0,   397,
       0,   422,   393,     0,     0,     0,     0,     0,   940,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
       0,     0,   419,   420,   421,     0,   453,     0,   396,     0,
       0,     0,   397,   422,     0,   393,     0,     0,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,   482,
    1105,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,  1106,   412,   413,   414,   415,
     416,   417,   418,     0,     0,   419,   420,   421,     0,     0,
       0,   396,     0,     0,   976,   397,   422,   393,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,     0,   396,     0,     0,     0,   397,     0,   422,
     393,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,   545,     0,   396,     0,     0,     0,
     397,   422,   393,     0,     0,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,     0,
     401,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,     0,   549,   419,   420,   421,     0,     0,   396,     0,
       0,     0,   397,   393,   422,     0,     0,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,     0,   551,   419,   420,   421,     0,   396,
       0,     0,     0,   397,   393,     0,   422,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,     0,   553,   419,   420,   421,     0,
     396,     0,     0,     0,   397,   393,     0,   422,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,     0,   556,   419,   420,   421,
       0,   396,     0,     0,     0,   397,   393,     0,   422,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,     0,     0,     0,
       0,     0,     0,     0,     0,   573,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,   396,     0,     0,     0,   397,   393,     0,   422,
       0,     0,     0,     0,   394,   726,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,     0,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,     0,   419,
     420,   421,     0,   396,     0,     0,     0,   397,   393,   827,
     422,     0,     0,     0,     0,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,   396,     0,     0,   807,   397,   393,
       0,   422,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   400,     0,   401,   402,
     403,     0,   404,   828,   406,     0,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,     0,
       0,   419,   420,   421,     0,   396,     0,     0,     0,   397,
     393,     0,   422,     0,     0,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
     410,   411,  -589,   412,   413,   414,   415,   416,   417,   418,
       0,     0,   419,   420,   421,     0,   396,     0,     0,     0,
     397,   393,     0,   422,     0,     0,     0,     0,   394,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,   188,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,     0,
     401,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,     0,     0,   419,   420,   421,     0,   396,     0,     0,
       0,   397,   393,     0,   422,     0,     0,     0,     0,   394,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   395,     0,     0,   938,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,   400,
       0,   401,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,     0,   909,   419,   420,   421,     0,   396,     0,
       0,     0,   397,   393,     0,   422,     0,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     400,     0,   401,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   410,   411,     0,   412,   413,   414,   415,
     416,   417,   418,     0,     0,   419,   420,   421,     0,   396,
       0,     0,     0,   397,   393,     0,   422,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,     0,     0,   419,   420,   421,     0,
     396,     0,     0,   961,   397,   393,     0,   422,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,     0,   412,   413,
     414,   415,   416,   417,   418,     0,     0,   419,   420,   421,
       0,   396,     0,     0,   962,   397,   393,   990,   422,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,     0,     0,     0,
       0,     0,     0,     0,     0,   979,   398,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,     0,     0,   419,   420,
     421,     0,   396,     0,     0,     0,   397,   393,     0,   422,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   400,     0,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,     0,   419,
     420,   421,     0,   396,     0,     0,     0,   397,   393,     0,
     422,     0,     0,     0,     0,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   395,  1024,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,     0,   396,     0,     0,  1011,   397,   393,
       0,   422,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
    1027,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   400,     0,   401,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,     0,
       0,   419,   420,   421,     0,   396,     0,     0,     0,   397,
     393,     0,   422,     0,     0,     0,     0,   394,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
       0,     0,   419,   420,   421,     0,   396,     0,     0,     0,
     397,   393,  1102,   422,     0,     0,     0,     0,   394,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   400,     0,
     401,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,   410,   411,     0,   412,   413,   414,   415,   416,   417,
     418,     0,     0,   419,   420,   421,     0,   396,     0,     0,
       0,   397,  1030,   393,   422,     0,     0,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,   400,
       0,   401,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,   410,   411,     0,   412,   413,   414,   415,   416,
     417,   418,     0,     0,   419,   420,   421,     0,     0,   396,
       0,     0,     0,   397,   393,   422,     0,     0,     0,     0,
       0,   394,  1127,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   395,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,   400,     0,   401,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,   410,   411,     0,   412,   413,   414,
     415,   416,   417,   418,     0,     0,   419,   420,   421,     0,
     396,     0,     0,     0,   397,     0,     0,   422,     0,     0,
       0,     0,     0,  1130,     0,     0,     0,     0,   393,     0,
       0,     0,     0,     0,     0,   394,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,   400,     0,   401,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,   410,   411,  1161,   412,   413,
     414,   415,   416,   417,   418,   393,  1182,   419,   420,   421,
       0,     0,   394,     0,     0,     0,     0,     0,   422,     0,
       0,     0,     0,     0,   396,   395,     0,     0,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   393,     0,     0,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   400,     0,   401,   402,
     403,   396,   404,   405,   406,   397,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,     0,
       0,   419,   420,   421,     0,     0,     0,     0,     0,     0,
       0,     0,   422,     0,     0,     0,   398,     0,     0,     0,
       0,   396,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,   398,     0,   419,   420,
     421,     0,     0,     0,     0,     0,     0,     0,     0,   422,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   411,     0,   412,
     413,   414,   415,   416,   417,   418,   393,     0,   419,   420,
     421,     0,     0,   394,     0,     0,     0,  1220,     0,   422,
     462,     0,     0,     0,    22,    23,   395,     0,     0,   997,
       0,     0,     0,     0,   463,     0,    31,   464,     0,     0,
       0,     0,    37,     0,     0,     0,   393,     0,     0,    42,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   395,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    61,     0,
     998,     0,   396,   999,     0,   465,   397,    67,     0,     0,
       0,     0,     0,     0,     0,  1232,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,     0,     0,    85,
       0,     0,    87,     0,     0,   395,     0,   398,     0,     0,
       0,     0,   396,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,   399,   400,  1233,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   398,     0,   419,
     420,   421,   102,     0,     0,     0,     0,     0,  1183,     0,
     422,   396,     0,   399,   400,   397,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,   393,     0,   419,
     420,   421,     0,     0,   394,     0,     0,     0,     0,     0,
     422,     0,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,   399,   400,     0,   401,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,   410,   393,     0,   412,
     413,   414,   415,   416,   394,   418,     0,     0,   419,   420,
     421,     0,     0,     0,     0,     0,     0,   395,     0,   422,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,   397,     0,     0,
       0,     0,     0,     0,     0,     0,  1234,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,   396,     0,     0,     0,   397,     0,     0,
       0,     0,     0,     0,   399,   400,  1235,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,   398,     0,
     419,   420,   421,     0,     0,   396,     0,     0,     0,   397,
       0,   422,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,   393,     0,
     419,   420,   421,     0,     0,   394,     0,     0,     0,     0,
       0,   422,     0,     0,     0,     0,   399,   400,   395,   401,
     402,   403,     0,   404,   405,   406,     0,     0,   408,   409,
     410,     0,     0,   412,   413,   414,   415,     0,   393,   418,
       0,     0,   419,   420,   421,   394,     0,     0,     0,     0,
       0,     0,     0,   422,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,   397,   393,
       0,     0,     0,     0,     0,     0,   394,  1236,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,   396,     0,     0,     0,   397,     0,
       0,     0,     0,     0,     0,   399,   400,  1237,   401,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,   398,
       0,   419,   420,   421,     0,   396,     0,     0,     0,   397,
       0,     0,   422,     0,     0,   399,   400,     0,   401,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,   410,
     411,     0,   412,   413,   414,   415,   416,   417,   418,     0,
     398,   419,   420,   421,     0,     0,   393,     0,     0,     0,
       0,     0,   422,   394,     0,     0,   399,   400,     0,   401,
     402,   403,     0,   404,   405,   406,   395,   407,   408,   409,
     410,   411,     0,   412,   413,   414,   415,   416,   417,   418,
       0,     0,   419,   420,   421,     0,     0,   393,     0,     0,
       0,  1250,     0,   422,   394,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,   894,     0,     0,     0,   397,     0,     0,
       0,     0,     0,   399,   400,     0,   401,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,   410,   411,     0,
     412,   413,   414,   415,   416,   417,   418,     0,   398,   419,
     420,   421,     0,     0,     0,     0,     0,     0,     0,     0,
     422,     0,     0,     0,   399,   400,     0,   401,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,   410,   411,
       0,   412,   413,   414,   415,   416,   417,   418,     0,     0,
     419,   420,   421,   365,     0,   227,     0,     0,     0,     0,
       0,   422,     0,     0,     0,     0,     0,     0,     0,   228,
     229,     0,   230,     0,     0,     0,     0,   231,     0,     0,
       0,     0,     0,   366,     0,     0,   232,     0,     0,   227,
       0,     0,   233,     0,     0,     0,     0,     0,   234,     0,
       0,     0,   235,   228,   229,   236,   230,     0,     0,     0,
       0,   231,     0,     0,     0,   237,     0,   366,     0,     0,
     232,   238,   239,     0,     0,     0,   233,     0,     0,   240,
       0,     0,   234,     0,     0,     0,   235,     0,   241,   236,
       0,     0,     0,     0,     0,     0,     0,   242,   243,   237,
     244,     0,   245,     0,   246,   238,   239,   247,     0,     0,
       0,   248,     0,   240,   249,     0,     0,   250,     0,     0,
       0,     0,   241,     0,     0,  -290,     0,     0,     0,     0,
       0,   242,   243,     0,   244,     0,   245,     0,   246,  -290,
    -290,   247,  -290,     0,     0,   248,     0,  -290,   249,     0,
       0,   250,     0,     0,     0,     0,  -290,     0,     0,   227,
       0,     0,  -290,     0,     0,     0,     0,     0,  -290,     0,
       0,     0,  -290,   228,   229,  -290,   230,     0,     0,     0,
       0,   231,     0,     0,     0,  -290,     0,     0,     0,     0,
     232,  -290,  -290,     0,     0,     0,   233,     0,     0,  -290,
       0,     0,   234,     0,     0,     0,   235,     0,  -290,   236,
       0,     0,     0,     0,     0,     0,     0,  -290,  -290,   237,
    -290,     0,  -290,     0,  -290,   238,   239,  -290,     0,     0,
       0,  -290,     0,   240,  -290,     0,     0,  -290,     0,     0,
       0,     0,   241,     0,     0,  -291,     0,     0,     0,     0,
       0,   242,   243,     0,   244,     0,   245,     0,   246,  -291,
    -291,   247,  -291,     0,     0,   248,     0,  -291,   249,     0,
       0,   250,     0,     0,     0,     0,  -291,     0,     0,     0,
       0,     0,  -291,     0,     0,     0,     0,     0,  -291,     0,
       0,     0,  -291,     0,     0,  -291,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -291,     0,     0,     0,     0,
       0,  -291,  -291,     0,     0,     0,     0,     0,     0,  -291,
       0,     0,     0,     0,     0,     0,     0,     0,  -291,     0,
       0,     0,     0,     0,     0,     0,     0,  -291,  -291,     0,
    -291,     0,  -291,     0,  -291,     0,     0,  -291,     0,     0,
       0,  -291,     0,     0,  -291,     0,     0,  -291
};

static const yytype_int16 yycheck[] =
{
      12,   172,    68,   158,    16,   847,   444,   384,    20,     2,
      19,   504,   270,   658,    26,   274,    40,   655,   656,   217,
     587,   604,    34,    35,    36,   747,    38,   528,   523,   598,
     224,   358,   547,   833,   832,   293,   953,    49,   131,    51,
     106,   953,   521,   842,    56,    48,    58,     1,    60,   260,
     261,   262,     1,   311,     1,     1,    68,    69,    70,    71,
      72,    73,    34,    35,    36,    77,   950,    79,    33,    81,
      82,    33,   956,    33,    86,    69,     1,    89,    90,    56,
      92,     3,    94,    91,    22,    97,    98,  1060,   100,  1007,
      33,    33,   104,   105,   106,   107,   101,    61,  1148,    54,
      56,    27,    48,   604,   145,   946,    37,    48,   171,    48,
     173,   105,     3,   119,    61,  1210,   119,   129,    73,   131,
      74,    88,    88,   164,    48,    92,    92,    48,    83,    20,
       3,   342,   343,    48,   106,  1019,    61,   203,     3,    48,
     124,    72,   145,    34,    91,    11,    48,   318,    89,    87,
      89,   993,    78,   117,   119,  1250,   168,   658,   880,  1001,
    1210,   169,   116,    89,   169,   149,    91,     0,   174,    91,
     117,   174,    63,   111,  1147,   124,  1060,   119,   124,   104,
     102,   193,   440,   441,   196,   197,   198,    27,   114,   201,
     202,   203,   117,   992,   171,   144,   145,   144,   144,   145,
     224,    32,   145,  1121,   169,  1122,   215,   169,    48,   169,
    1122,   102,    52,   384,   226,   171,   170,   108,   164,   144,
    1061,   145,   169,   172,   145,   740,   169,   169,    74,   102,
     145,   119,   117,    18,   727,    75,   145,   102,    78,   164,
      80,    32,   108,   145,   169,   145,   145,   164,    91,    89,
     174,   145,   124,   174,   171,    61,   173,    88,   270,   174,
     145,    92,   589,  1147,   357,   174,   833,   461,   108,  1111,
     116,   164,   172,   145,  1116,   174,   170,   149,   169,   848,
     164,   293,   777,  1071,    27,    91,   174,   171,   300,   301,
     226,   171,    77,   173,   145,   145,   362,    88,   104,   311,
     172,    92,   457,   782,  1104,  1103,  1106,  1105,   139,   747,
      95,   117,    47,   144,    47,   146,   147,   145,  1040,   170,
     151,   164,   172,   145,   170,   337,   169,   112,   159,   145,
      65,    61,   172,   119,   270,    78,   167,   145,   350,   139,
     352,   334,   164,  1185,   172,    56,    89,   147,   139,    84,
     362,    84,   845,   144,   170,     3,   147,   293,   164,    56,
     151,    91,   108,   169,   172,   108,   949,  1155,  1156,  1157,
    1158,  1159,  1160,    89,   104,   311,  1014,  1015,   113,   391,
     113,   393,   394,   395,   396,   397,   398,   117,   400,   401,
     402,   403,   404,   591,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   477,   905,    82,   893,   982,    85,   429,   430,   145,
      81,    82,   145,   435,   396,   397,   438,   439,   440,   441,
     442,   443,   444,     3,   164,   429,   430,   461,  1083,   169,
      27,   435,   880,   455,   438,   439,   172,   459,   949,   172,
      20,   145,   139,    56,    71,   145,   468,    56,    47,   124,
     147,   145,   145,   145,    34,   477,     8,   479,   480,   405,
     442,   443,   145,   485,   486,   171,    65,     2,   172,   491,
     145,   164,   172,   455,   149,   862,   498,    12,   172,   501,
     172,    78,   504,    63,    32,    84,   294,   295,   145,   172,
      25,   145,    89,    28,   440,   441,  1007,   172,   480,   171,
       2,   145,   145,    56,   486,   145,   775,    56,   530,   491,
      12,   108,   534,   535,   113,   172,   170,   145,   540,   145,
     164,   164,   102,    25,    56,   547,    28,  1104,   108,  1106,
     751,   752,   172,   145,   755,   756,   164,   145,   560,   171,
      88,   563,    77,   565,    92,   139,   172,   569,   570,   571,
     144,   573,   574,   147,   145,    56,   164,   151,   504,   563,
     172,   565,   164,    51,   586,   587,    54,    56,   170,   104,
      48,    56,  1083,   164,   109,    77,   579,   599,   847,    81,
      82,     8,   139,    61,   171,    73,   173,   569,    76,   858,
     147,   139,  1040,   814,   815,    83,   144,   171,   146,   147,
     164,   547,   104,   151,    56,    99,   170,   109,    56,   171,
    1121,   159,    56,    91,    61,    56,    56,   165,   166,   167,
     171,    56,   173,   169,   171,   171,   173,   171,   650,   173,
     257,   258,   259,   260,   261,   262,   169,  1148,    56,   117,
     124,   171,   164,   173,    91,   563,   124,   565,   144,   163,
     155,   164,   164,     2,   189,   164,   283,   104,   164,    56,
     164,   164,   289,    12,   171,   164,   150,   145,   152,   153,
     117,   169,   156,   157,   164,   118,    25,    48,   843,    28,
     307,   862,   169,    84,   164,   171,   164,   189,   171,   171,
      61,   169,   171,     8,   172,   171,   323,   324,    65,  1210,
     722,   723,   724,   725,   726,   727,   728,   729,    27,  1220,
     169,   787,   731,   732,   170,     8,   738,   164,   740,   741,
      91,   743,   169,    89,   993,   747,   124,   749,    77,    48,
      89,   124,   164,    52,   738,   145,   758,   759,   273,  1250,
     147,   723,    68,   725,   108,   280,   117,   171,   124,   172,
     145,    48,   171,   124,   124,   104,    75,    35,    35,    78,
     109,    80,   124,   785,   786,   787,    65,   172,   145,   791,
      89,   273,   172,   124,   145,    21,   852,   145,   280,    48,
     172,   727,   172,    27,   169,   174,   174,   791,   104,   108,
     171,   147,    61,   164,   740,   147,   139,   147,   169,   334,
     147,   172,   147,   825,    48,   147,   139,   829,    52,   831,
     832,   833,   147,   147,   349,   147,  1084,  1085,   139,   147,
     147,   139,    91,   845,   147,   139,   164,   164,   164,   456,
     852,    75,   334,   164,    78,   164,    80,    35,    35,   164,
     189,   144,  1111,   145,   174,    89,   170,   349,   117,   384,
     172,   171,   164,   164,   164,   124,    33,   484,   880,   172,
     169,   488,   170,   490,   108,   172,   493,    33,   174,   174,
      35,   171,   894,   895,   896,   164,   145,   899,   164,   174,
     171,   827,   828,   905,   144,   164,   174,   174,   164,   965,
       8,   174,   172,    35,   916,   164,   918,   139,   139,   845,
     169,   923,   172,   172,   926,   164,   172,   172,   164,   164,
     164,   164,   894,   895,   145,    32,   938,   825,   453,   349,
     163,   269,    24,   163,   273,    89,  1001,   944,   319,   969,
     952,   280,   360,   862,   851,   858,   775,   515,  1122,  1161,
    1154,   857,   661,   965,   479,   129,   531,   781,    -1,    -1,
     485,   453,   974,    -1,   976,    -1,    -1,   979,   275,   905,
     982,   496,    -1,   498,    -1,    -1,   988,    -1,   990,    -1,
      -1,    88,    -1,    -1,    -1,    92,    -1,   479,    -1,    -1,
      -1,    -1,    -1,   485,    -1,   334,  1062,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   496,    -1,   498,    -1,    -1,    -1,
     349,  1023,  1024,    -1,  1026,  1027,    -1,  1029,  1030,    -1,
      -1,    -1,    -1,  1035,  1036,    -1,  1038,    -1,  1040,    -1,
    1042,  1043,   139,   140,  1046,   142,   143,   144,    -1,   146,
     147,    -1,   567,    -1,   151,    -1,   571,    -1,    32,    -1,
    1062,    -1,   159,    -1,   579,  1067,    -1,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1082,  1083,  1084,  1085,    -1,   567,    -1,    -1,  1090,   571,
    1092,    -1,    -1,    -1,  1096,    -1,    -1,   579,    -1,    -1,
    1102,  1103,  1104,  1105,  1106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,    92,  1121,
      -1,    -1,    -1,    -1,   453,  1127,  1128,  1129,  1130,  1131,
    1132,  1133,  1134,    -1,    -1,    -1,    -1,    -1,   745,   746,
      -1,    -1,    -1,    -1,   751,   752,    -1,    -1,   755,   756,
     479,  1153,    -1,    -1,    -1,    -1,   485,    32,  1084,  1085,
      -1,    -1,    -1,    -1,    -1,   139,   140,   496,    -1,   498,
     144,    -1,   146,   147,    -1,    -1,    -1,   151,    -1,     2,
    1182,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,    12,
      -1,   165,   166,   167,    -1,    -1,  1198,  1199,  1200,  1201,
      -1,    -1,    25,    -1,   719,    28,    -1,  1209,  1210,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    92,  1220,    -1,
      -1,    -1,  1224,  1225,    -1,    -1,    -1,    -1,    -1,    -1,
    1232,  1233,  1234,  1235,  1236,  1237,    -1,    -1,   567,    -1,
      -1,    -1,   571,    -1,    -1,    -1,   761,    -1,  1250,   764,
     579,     2,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    25,    -1,   151,    28,    -1,   761,
      -1,   104,   764,   158,   159,    -1,   109,   162,    -1,    -1,
     165,   166,   167,    -1,    -1,    -1,    -1,    -1,   813,   906,
     907,   176,    -1,    -1,   819,    -1,   913,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   925,    -1,
      -1,   928,    -1,   930,    -1,   932,    77,    -1,    -1,    -1,
      -1,   813,    -1,    -1,    -1,    22,    -1,   819,    -1,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    36,
      -1,    38,    39,   104,    -1,    -1,    -1,    44,   109,    -1,
      -1,    -1,    -1,    -1,    51,    -1,   189,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,   894,
     895,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    -1,    -1,    82,    -1,    84,    85,    -1,
      87,   916,    89,   918,    -1,    -1,    -1,    -1,   923,    68,
      -1,   926,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     935,   108,   937,    -1,   111,   940,    -1,   114,    -1,    -1,
      -1,    -1,   761,    -1,   916,   764,   918,    -1,   189,    -1,
      -1,   923,    -1,    -1,   926,   104,   105,   106,    -1,   964,
     273,    -1,    -1,   935,    -1,   937,    -1,   280,   940,   974,
      -1,   976,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,
      -1,  1088,   964,     2,   813,    -1,    -1,    -1,    -1,    -1,
     819,    -1,   974,    12,   976,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,
      -1,   334,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1035,    -1,   273,    -1,    -1,    -1,   349,  1042,    -1,   280,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1055,    -1,    -1,   202,   203,    22,     2,    -1,    -1,    26,
      27,    -1,    -1,  1035,    -1,    -1,    12,    -1,    77,    36,
    1042,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,    25,
      -1,  1086,    28,  1055,    51,    -1,    -1,    -1,    -1,    -1,
    1095,    -1,    -1,   334,  1099,   104,    -1,   916,    -1,   918,
     109,    -1,    -1,    -1,   923,    -1,    -1,   926,   349,    76,
      -1,    78,    -1,    -1,  1086,    -1,   935,    -1,   937,    -1,
      87,   940,    89,  1095,    -1,    -1,    -1,  1099,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,  1143,    -1,
     453,   108,    -1,    -1,   111,   964,    -1,   114,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   974,    -1,   976,   104,    -1,
      -1,    -1,    -1,   109,    -1,    -1,   479,    -1,    -1,    -1,
      -1,  1143,   485,    -1,    -1,    32,    -1,    -1,    -1,    -1,
     189,    -1,    -1,   496,    -1,   498,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1198,  1199,  1200,  1201,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   453,   362,    -1,    -1,  1035,    -1,    -1,  1224,
    1225,    -1,    -1,  1042,    -1,    -1,  1198,  1199,  1200,  1201,
      -1,    88,    -1,    -1,    -1,    92,  1055,    -1,   479,    -1,
      32,    -1,    -1,   189,   485,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1224,  1225,   567,   496,    -1,   498,   571,    -1,
      -1,    -1,    -1,    -1,   273,    -1,   579,  1086,    -1,    -1,
      -1,   280,    -1,    -1,    -1,    -1,  1095,    -1,    -1,    -1,
    1099,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,    -1,    -1,    -1,   151,    -1,    88,     2,    -1,    -1,
      92,   158,   159,    -1,    -1,   162,    -1,    12,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    28,  1143,   334,   567,   273,   477,    -1,
     571,   123,    -1,    -1,   280,    -1,    -1,    -1,   579,    -1,
     349,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,    -1,    -1,   156,   157,   158,   159,   160,   161,
     162,    -1,    77,   165,   166,   167,    -1,    -1,    -1,  1198,
    1199,  1200,  1201,    -1,   176,    -1,    -1,    -1,   334,    -1,
      -1,   540,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   104,
      -1,    -1,    -1,   349,   109,  1224,  1225,    -1,    -1,    -1,
      -1,   560,    -1,    -1,   563,    -1,   565,    -1,    -1,    -1,
      -1,   570,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    -1,    -1,    -1,   453,    44,    -1,    -1,   761,    -1,
      -1,   764,    51,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,    -1,
     479,    -1,    -1,    -1,    -1,    -1,   485,    76,    -1,    78,
      -1,    80,    -1,    82,   189,     2,    85,   496,    87,   498,
      89,    -1,    -1,    -1,    -1,    12,    -1,    -1,   168,    -1,
     813,   171,    -1,   173,    -1,    -1,   819,   453,    25,   108,
      -1,    28,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
     761,    -1,    -1,   764,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   479,    -1,    -1,    -1,    -1,    17,   485,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
     496,    -1,   498,    -1,    -1,    -1,    -1,    36,   567,    -1,
      77,    -1,   571,    -1,    -1,   164,    -1,    46,   273,    -1,
     579,   170,   813,    -1,    53,   280,    -1,    -1,   819,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,   104,    -1,   738,
      -1,    70,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   916,    -1,   918,    -1,    86,    -1,    -1,
     923,    -1,    -1,   926,    -1,    -1,    -1,    -1,    -1,    98,
      -1,   567,   935,   102,   937,   571,    -1,   940,    -1,   334,
     109,    -1,    -1,   579,    -1,    -1,   115,    -1,   787,    -1,
      -1,    -1,    -1,    -1,   349,    -1,    -1,    -1,    -1,    -1,
      -1,   964,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,
      -1,   974,    -1,   976,    -1,    -1,    -1,    -1,    36,     2,
      38,    39,   189,    -1,    -1,   916,    44,   918,    -1,    12,
      -1,    -1,   923,    51,    -1,   926,    -1,    -1,    -1,    -1,
      -1,    -1,    25,    -1,   935,    28,   937,    -1,    -1,   940,
      -1,    -1,    -1,   852,    -1,    -1,    -1,    -1,    76,    -1,
      78,    -1,    -1,    -1,    82,    -1,    -1,    85,    -1,    87,
      -1,    89,  1035,   964,    -1,    -1,    -1,    -1,    -1,  1042,
      -1,    -1,    -1,   974,    -1,   976,    -1,    -1,    -1,    -1,
     108,    -1,  1055,   111,    77,    -1,   114,    -1,   453,    -1,
      -1,    -1,   761,    -1,    -1,   764,   273,    -1,    -1,    -1,
      -1,    -1,    -1,   280,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   104,    -1,  1086,   479,    -1,   109,    -1,    -1,    -1,
     485,    -1,  1095,    -1,    -1,    -1,  1099,    -1,    -1,    -1,
      -1,   496,    -1,   498,  1035,    -1,   164,    -1,    -1,    -1,
     949,  1042,    -1,    -1,   813,    -1,    30,    -1,    -1,    -1,
     819,    -1,    -1,    -1,  1055,   761,   965,   334,   764,    -1,
      44,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1143,    -1,   349,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,  1086,    -1,    26,    27,    -1,
      -1,    -1,    31,    -1,  1095,    -1,   189,    36,  1099,    38,
      39,    -1,   567,    -1,    -1,    44,   571,   813,    -1,    -1,
      -1,    -1,    51,   819,   579,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1198,  1199,  1200,  1201,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    80,  1143,    82,    -1,    -1,    85,   916,    87,   918,
      89,  1224,  1225,  1062,   923,    -1,    -1,   926,  1067,    -1,
      -1,    -1,   146,    -1,   148,    -1,   935,    -1,   937,   108,
      32,   940,   111,   157,    -1,   114,   453,    -1,    -1,    -1,
     273,    -1,    -1,    -1,    -1,    -1,    -1,   280,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   964,    -1,  1198,  1199,  1200,
    1201,    -1,   479,    -1,    -1,   974,    -1,   976,   485,    -1,
     916,    -1,   918,    -1,    -1,   199,    -1,   923,    -1,   496,
     926,   498,    -1,  1224,  1225,   164,    88,    -1,    -1,   935,
      92,   937,    -1,    -1,   940,    -1,    -1,    -1,    -1,  1148,
      -1,   334,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   349,    -1,   964,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1035,    -1,   974,    -1,
     976,   255,    -1,  1042,    -1,    -1,    -1,   139,   140,   263,
     264,   143,   144,    -1,   146,   147,  1055,    -1,    -1,   151,
     567,    -1,   276,    -1,   571,    -1,   761,   159,    -1,   764,
      -1,  1210,   579,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1086,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1095,    -1,    -1,  1035,
    1099,    -1,    -1,    -1,    -1,    49,  1042,    -1,    -1,    -1,
      -1,  1250,    -1,    -1,    -1,    -1,    -1,    -1,   813,  1055,
      -1,    -1,    -1,    -1,   819,    -1,    -1,    71,    -1,    -1,
     453,    -1,    -1,    -1,    -1,    -1,    -1,    81,    82,    -1,
      -1,    -1,    -1,    -1,  1143,    -1,    -1,   361,    -1,    -1,
    1086,    -1,    -1,    -1,    -1,    -1,   479,    -1,    -1,  1095,
      -1,    -1,   485,  1099,    -1,    -1,    -1,    -1,    -1,    -1,
     384,    -1,    -1,   496,    -1,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1198,
    1199,  1200,  1201,    -1,    -1,    -1,   155,  1143,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   168,
      -1,   916,   171,   918,   173,  1224,  1225,    -1,   923,    -1,
      -1,   926,    -1,    -1,    -1,    -1,    -1,    -1,   452,    -1,
     935,    -1,   937,    -1,   567,   940,    -1,    -1,   571,   463,
      -1,    -1,    -1,    -1,   761,    -1,   579,   764,    -1,    -1,
      -1,    -1,  1198,  1199,  1200,  1201,    -1,    -1,    -1,   964,
     214,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   974,
      -1,   976,    -1,    -1,    -1,    -1,    -1,    -1,  1224,  1225,
      -1,   505,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   519,   813,    -1,    -1,   523,
      -1,     1,   819,   257,   258,   259,   260,   261,   262,    -1,
      -1,   265,   266,    -1,    -1,    -1,    -1,    -1,   272,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,   283,
    1035,    31,    -1,    -1,    -1,   289,    36,  1042,    38,    39,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
    1055,    51,    -1,   307,    54,    -1,    -1,    -1,    -1,    -1,
      -1,   585,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   323,
     324,    -1,    -1,    73,   598,    -1,    76,    -1,    78,    -1,
      80,  1086,    82,    83,    -1,    85,    -1,    87,    -1,    89,
    1095,    -1,    -1,    -1,  1099,    -1,    -1,    -1,    -1,   916,
      -1,   918,    -1,    -1,    -1,    -1,   923,    -1,   108,   926,
      -1,   111,    -1,    -1,   114,    -1,    -1,    -1,   935,    -1,
     937,    -1,    -1,   940,    -1,    -1,    -1,    -1,   761,    -1,
      -1,   764,    -1,    -1,    -1,    -1,    -1,   661,  1143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   964,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   974,    -1,   976,
      -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     813,    -1,    -1,    -1,    -1,    -1,   819,    -1,    -1,    -1,
      -1,    -1,    -1,  1198,  1199,  1200,  1201,    -1,    -1,    -1,
      22,    -1,   456,    -1,    26,    27,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,  1035,  1224,
    1225,    -1,    44,    -1,    -1,  1042,    -1,    -1,    -1,    51,
     484,    -1,    -1,    -1,   488,    -1,   490,    -1,  1055,   493,
      -1,    -1,   766,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     774,    -1,    -1,   777,    76,    -1,    78,   781,    80,    -1,
      82,    -1,    -1,    85,    -1,    87,    -1,    89,    -1,  1086,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1095,    -1,
     804,    -1,  1099,   916,    -1,   918,   108,    -1,    -1,   111,
     923,    -1,   114,   926,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   935,    -1,   937,    -1,    -1,   940,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   848,    -1,  1143,   851,    -1,    -1,
      -1,   964,    -1,    -1,    -1,    -1,    -1,    -1,   862,    -1,
      -1,   974,   164,   976,    -1,    -1,    -1,    -1,   170,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,  1198,  1199,  1200,  1201,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1224,  1225,    63,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
     944,    -1,  1055,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    -1,    98,   969,   100,    -1,   102,    -1,
      -1,   105,    -1,  1086,     1,   109,     3,    -1,   112,    -1,
      -1,   115,  1095,    -1,    -1,    12,  1099,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,   745,   746,    40,    -1,    -1,    -1,   751,   752,    46,
      -1,   755,   756,    50,    -1,    -1,    53,    -1,    55,    -1,
    1143,   765,    -1,    -1,    -1,   169,    63,   171,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
     804,    98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,    -1,    -1,   132,   133,   134,   135,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,    -1,   146,
      -1,    -1,    -1,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,    -1,   162,    -1,    -1,   165,   166,
     167,   168,    -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   906,   907,    -1,    -1,    -1,    -1,    -1,   913,
      -1,   915,    -1,    -1,    -1,    -1,    -1,    -1,   922,    -1,
      -1,   925,    -1,    -1,   928,    -1,   930,    -1,   932,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   945,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,   972,    22,
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
      -1,    -1,    -1,    -1,  1088,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1137,  1138,    -1,    -1,  1141,  1142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1169,     1,  1171,     3,  1173,
       5,     6,     7,     8,     9,    10,    -1,    12,    -1,    14,
      15,    16,    17,    18,    19,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,   169,   170,   171,    -1,   173,    -1,
       1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,   170,
     171,    -1,   173,    -1,     1,   176,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,   170,   171,    -1,   173,    -1,     1,   176,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
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
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,   170,   171,    -1,
     173,    -1,     1,   176,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,   170,   171,    -1,   173,    -1,     1,   176,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,   170,   171,    -1,   173,    -1,
       1,   176,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     1,   176,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     1,   176,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       1,   176,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     1,   176,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,    76,
      77,    78,    79,    -1,    81,    82,    83,    84,    85,    86,
      87,    -1,    89,    90,    91,    -1,    93,    94,    95,    96,
      97,    98,    99,   100,    -1,   102,   103,    -1,   105,   106,
     107,   108,   109,    -1,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,   164,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     1,   176,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    -1,    81,    82,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    -1,
      93,    94,    95,    96,    97,    98,    99,   100,    -1,   102,
     103,    -1,   105,   106,   107,   108,   109,    -1,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,   164,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     1,   176,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    -1,
      89,    90,    91,    -1,    93,    94,    95,    96,    97,    98,
      99,   100,    -1,   102,   103,    -1,   105,   106,   107,   108,
     109,    -1,   111,   112,   113,   114,   115,    -1,    -1,   118,
      -1,   120,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,   164,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     1,   176,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      15,    16,    17,    18,    -1,    20,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    33,    34,
      -1,    36,    -1,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    -1,    53,    54,
      -1,    56,    57,    58,    59,    60,    -1,    62,    63,    64,
      65,    66,    67,    -1,    69,    70,    71,    -1,    73,    -1,
      -1,    76,    77,    78,    79,    -1,    81,    82,    83,    84,
      85,    86,    87,    -1,    89,    90,    91,    -1,    93,    94,
      95,    96,    97,    98,    99,   100,    -1,   102,   103,    -1,
     105,   106,   107,   108,   109,    -1,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       1,   176,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    -1,    33,    34,    -1,    36,    -1,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    -1,    53,    54,    -1,    56,    57,    58,    59,    60,
      -1,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    -1,    89,    90,
      91,    -1,    93,    94,    95,    96,    97,    98,    99,   100,
      -1,   102,   103,    -1,   105,   106,   107,   108,   109,    -1,
     111,   112,   113,   114,   115,    -1,    -1,   118,    -1,   120,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,   164,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     1,   176,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,   174,     1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,   114,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,   106,   107,    -1,   109,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,   172,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,   172,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      27,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    48,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,   114,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,   106,   107,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,   164,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
      -1,   102,    -1,    -1,   105,   106,   107,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,   121,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,   114,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
      -1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
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
     121,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,   106,   107,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,    -1,   176,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,   170,   171,    -1,   173,    -1,
      -1,   176,     3,     4,     5,     6,     7,     8,     9,    10,
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
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,   174,    -1,   176,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,   174,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,   121,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,   111,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,   174,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,
      -1,    -1,    -1,    86,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,    -1,   102,
      -1,    -1,   105,    -1,    -1,    -1,   109,   110,    -1,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,   110,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,    -1,   176,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,    -1,   176,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
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
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
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
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
     169,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,    -1,   173,    -1,     3,   176,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,
      -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,
      -1,    86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    -1,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,    -1,   173,    -1,
       3,   176,     5,     6,     7,     8,     9,    10,    -1,    -1,
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
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,   142,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,    -1,
     173,    -1,     3,   176,     5,     6,     7,     8,     9,    10,
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
      -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     141,   142,   143,    -1,    -1,    -1,    -1,   148,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,
     171,    -1,   173,    -1,     3,   176,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     139,   140,   141,   142,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,   167,    -1,
      -1,    -1,   171,    -1,   173,    -1,     3,   176,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,   141,   142,   143,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   158,    -1,    -1,    -1,   162,   163,    -1,    -1,    -1,
     167,    -1,   169,    -1,   171,     3,   173,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    61,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    91,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,   104,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,   163,   164,    25,    -1,    -1,
      -1,   169,    -1,   171,    -1,   173,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,   145,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,    -1,    -1,
      -1,   169,    -1,   171,   172,   173,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    -1,   141,    -1,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,   163,    -1,    25,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,   163,    -1,    25,    -1,
      -1,    -1,   169,    -1,   171,    -1,   173,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    -1,     3,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,    -1,   102,    17,    18,   105,    20,
      -1,    -1,   109,    -1,    25,   112,    -1,    -1,   115,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,   141,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   171,    86,   173,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,
      -1,   102,    17,    18,   105,    20,    -1,    -1,   109,   110,
      25,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    34,
      -1,    -1,     3,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    17,    18,    53,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    34,    69,    70,    -1,    -1,    -1,    40,
      -1,    -1,    77,    -1,    -1,    46,    -1,    -1,    -1,    50,
     171,    86,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    63,    98,    -1,   100,    -1,   102,    69,    70,
     105,    -1,    -1,    -1,   109,    -1,    77,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,    -1,     3,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    12,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   171,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
     171,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,   125,   126,   127,   128,   129,    -1,    -1,   132,   133,
     134,   135,    -1,   137,   138,   139,   140,    -1,   142,   143,
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
      -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,
      -1,    92,   176,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     101,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,   125,   126,   127,   128,   129,    -1,
      48,   132,   133,   134,   135,    -1,   137,   138,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    -1,    -1,   165,   166,   167,    -1,   169,    -1,
      88,    -1,    -1,    -1,    92,   176,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,   119,    -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   139,   140,    -1,   142,   143,   144,    -1,   146,   147,
     148,    -1,   150,   151,   152,   153,   154,    -1,   156,   157,
     158,   159,   160,   161,   162,    -1,    -1,   165,   166,   167,
      -1,   169,    88,    -1,    -1,    -1,    92,    -1,   176,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,    92,
     176,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,
     123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    -1,   150,   151,   152,
     153,   154,    72,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,   169,    -1,    88,    -1,
      -1,    -1,    92,   176,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      37,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    72,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,   165,   166,   167,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,   176,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,   176,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,   119,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,    -1,   169,    -1,    88,    -1,    -1,    -1,
      92,   176,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,   119,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    32,   156,   157,   158,   159,   160,   161,
     162,    -1,    -1,   165,   166,   167,    -1,   169,    88,    -1,
      -1,    -1,    92,    12,   176,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,   119,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    -1,   123,   165,   166,   167,    -1,    88,
      -1,    -1,    -1,    92,    -1,    12,   176,    -1,    -1,    -1,
     139,   140,    19,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,    32,    33,   156,   157,   158,
     159,   160,    -1,   162,   123,    -1,   165,   166,   167,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
      -1,   150,   151,   152,   153,   154,    -1,   156,   157,   158,
     159,   160,   161,   162,    -1,    -1,   165,   166,   167,    -1,
     169,    88,    -1,    -1,    -1,    92,    12,   176,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,   154,    -1,   156,
     157,   158,   159,   160,   161,   162,    -1,    -1,   165,   166,
     167,    -1,    88,    -1,    -1,    -1,    92,    12,    -1,   176,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,    -1,   165,
     166,   167,    -1,    88,    -1,    -1,    -1,    92,    -1,    12,
     176,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
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
      -1,    -1,   114,    -1,    -1,    32,    -1,   123,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,   139,   140,   101,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,   123,    -1,   165,
     166,   167,   164,    -1,    -1,    -1,    -1,    -1,   170,    -1,
     176,    88,    -1,   139,   140,    92,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    12,    -1,   165,
     166,   167,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,    -1,   150,   151,   152,   153,    12,    -1,   156,
     157,   158,   159,   160,    19,   162,    -1,    -1,   165,   166,
     167,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,   176,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   101,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,   123,    -1,
     165,   166,   167,    -1,    -1,    88,    -1,    -1,    -1,    92,
      -1,   176,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    12,    -1,
     165,   166,   167,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,   139,   140,    32,   142,
     143,   144,    -1,   146,   147,   148,    -1,    -1,   151,   152,
     153,    -1,    -1,   156,   157,   158,   159,    -1,    12,   162,
      -1,    -1,   165,   166,   167,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   176,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     144,    -1,   146,   147,   148,    -1,   150,   151,   152,   153,
     154,    -1,   156,   157,   158,   159,   160,   161,   162,    -1,
     123,   165,   166,   167,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,   176,    19,    -1,    -1,   139,   140,    -1,   142,
     143,   144,    -1,   146,   147,   148,    32,   150,   151,   152,
     153,   154,    -1,   156,   157,   158,   159,   160,   161,   162,
      -1,    -1,   165,   166,   167,    -1,    -1,    12,    -1,    -1,
      -1,   174,    -1,   176,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,    -1,    -1,
      -1,    -1,    -1,    88,    -1,    -1,    -1,    92,    -1,    -1,
      -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,   123,   165,
     166,   167,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     176,    -1,    -1,    -1,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,    -1,   150,   151,   152,   153,   154,
      -1,   156,   157,   158,   159,   160,   161,   162,    -1,    -1,
     165,   166,   167,     1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,     3,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    17,    18,    53,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    63,    -1,    31,    -1,    -1,
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
      -1,    -1,    86,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    95,    96,    -1,    98,    -1,   100,    -1,   102,    17,
      18,   105,    20,    -1,    -1,   109,    -1,    25,   112,    -1,
      -1,   115,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    -1,
      98,    -1,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115
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
     185,   186,   189,   190,   191,   192,   194,   195,   196,   202,
     203,   206,   207,   211,   213,   216,   220,   223,   224,   225,
     226,   227,   228,   230,   231,   233,   235,   238,   239,   240,
     241,   242,   246,   247,   250,   251,   252,   255,   256,   263,
     264,   265,   266,   267,   269,   270,   293,   294,   298,   299,
     320,   321,   322,   323,   324,   325,   326,   334,   335,   336,
     337,   338,   341,   342,   343,   344,   345,   346,   347,   348,
     350,   351,   352,   353,   354,   164,   185,   338,   119,   327,
     328,     3,   208,    14,    22,    36,    41,    42,    45,    56,
      87,   100,   169,   173,   238,   263,   320,   325,   336,   337,
     338,   341,   343,   344,   327,   338,   108,   300,    89,   208,
     185,   314,   338,     8,   188,   185,   171,     3,    17,    18,
      20,    25,    34,    40,    46,    50,    53,    63,    69,    70,
      77,    86,    95,    96,    98,   100,   102,   105,   109,   112,
     115,   210,   212,    11,   108,    78,   121,   232,   338,   232,
     338,   232,   338,    27,   114,   234,   338,    82,    85,   192,
     171,   210,   210,   210,   171,   276,   171,   210,   301,   302,
      33,   196,   215,   338,   338,    18,    77,    95,   112,   338,
     338,   338,     8,   171,   222,   221,     4,   288,   313,   338,
     106,   107,   164,   338,   340,   338,   215,   338,   338,   338,
      99,   171,   185,   338,   338,   196,   207,   338,   341,   196,
     207,   338,   338,   234,   338,   338,   338,   338,   338,   338,
     338,     1,   170,   183,   197,   313,   110,   149,   288,   315,
     316,   340,   232,   313,   338,   349,   338,    80,   185,   169,
      47,    84,   113,   193,    26,   299,   338,     8,   251,   338,
     339,    56,   144,   253,   210,     1,    31,   210,   257,   259,
     262,    54,    73,    83,   283,    27,    78,    89,   108,   284,
      27,    78,    89,   108,   282,   210,   295,   296,   301,   163,
     164,   155,   338,    12,    19,    32,    88,    92,   123,   139,
     140,   142,   143,   144,   146,   147,   148,   150,   151,   152,
     153,   154,   156,   157,   158,   159,   160,   161,   162,   165,
     166,   167,   176,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   168,   273,
     171,   173,    88,    92,   171,   185,   164,   338,   338,   338,
     210,   313,    56,   169,   196,    48,   327,   298,   164,   145,
     164,   187,    22,    36,    39,    87,   189,   192,   118,   211,
     288,   317,   318,   319,   340,   169,   210,   171,   215,    33,
      48,   215,   119,   215,   330,    33,    48,   215,   330,   215,
     330,    48,   215,   330,   210,   210,   101,   196,   101,   124,
     196,   273,   193,   317,   171,   171,   196,   185,    27,    48,
      52,    75,    78,    89,   108,   184,   277,   278,   279,   280,
     281,   284,   110,   171,   210,   303,   304,     1,   144,   308,
      48,   145,   185,   215,   171,   171,   215,   317,   223,   223,
     145,   164,   338,   338,   164,   169,   215,   171,   317,   164,
     243,   164,   243,   164,   215,   215,   164,   170,   170,   183,
     145,   170,   338,   145,   172,   145,   172,   174,   330,    48,
     145,   174,   330,   122,   145,   174,     8,     1,   170,   197,
     204,   205,   338,   199,   338,    65,    37,    72,   164,   251,
     253,   229,   267,   210,   313,   169,   170,     8,   261,   124,
     145,   170,    89,     1,   144,   307,    89,     1,     3,    12,
      17,    19,    20,    25,    40,    46,    53,    55,    63,    69,
      70,    86,    98,   102,   105,   109,   115,   139,   140,   141,
     142,   143,   144,   146,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   162,   165,   166,   167,   168,
     171,   209,   210,   212,   271,   272,   273,   320,   124,   297,
     164,   145,   164,   338,   338,   338,   338,   232,   338,   232,
     338,   338,   338,   338,   338,   338,   338,     3,    20,    34,
      63,   102,   108,   211,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,    68,   340,   340,   340,   340,   340,   317,   317,   232,
     338,   232,   338,    27,    48,    89,   114,   329,   332,   333,
     338,   354,    33,    48,    33,    48,   101,   171,    48,   174,
     210,   232,   338,   215,   301,   338,   189,   338,   124,   172,
     145,    48,   313,    45,   338,   232,   338,   171,   215,    45,
     338,   232,   338,   215,   215,   232,   338,   215,   124,   124,
     185,    35,   185,   338,    35,   338,    65,   172,   318,   210,
     236,   237,    48,    89,   280,   145,   172,   171,   210,   308,
     304,   145,   172,    34,    50,    96,   100,   173,   214,   309,
     321,   124,   305,   338,   302,   338,   338,   172,   288,   338,
       1,   248,   319,   172,    21,   244,   170,   172,   172,   315,
     172,   315,   185,   174,   232,   338,   174,   185,   338,   174,
     338,   174,   338,   170,   170,   145,   164,    13,   147,   145,
     164,    13,    37,    72,   210,   167,   198,   201,   338,   198,
     200,   253,   169,   298,   164,   171,     1,   254,   260,   262,
     338,   258,   173,   214,   306,   321,   104,   285,   171,   275,
     338,   139,   147,   275,   275,   309,   321,   295,   171,   173,
     164,   164,   164,   164,   164,   164,   172,   174,    48,    89,
     145,   172,    17,    20,    25,    46,    53,    63,    70,    86,
      98,   109,   115,   320,    88,    88,    45,   232,   338,    45,
     232,   338,   318,   232,   338,   171,   327,   327,   164,   164,
     288,   340,   319,   338,   172,   338,    33,   215,    33,   215,
     331,   332,   338,    33,   215,   330,    33,   215,   330,   215,
     330,   215,   330,   338,   338,    35,   185,    35,    35,   185,
     101,   196,   210,   172,   145,   172,   210,   279,   304,   144,
     312,    61,   117,   289,   172,   303,   308,     1,   313,    68,
     340,   172,   172,   170,    74,   116,   170,   249,   172,   171,
     196,   210,   245,   185,   174,   330,   174,   330,   185,   122,
     204,   211,   169,   338,   110,   338,   201,   200,   145,   164,
      13,   164,   169,   254,   301,   318,   170,    31,    82,    85,
     170,   184,   217,   220,   262,   259,     1,   174,   313,   289,
     278,   172,     3,   102,   272,   273,   172,   174,   332,   308,
     320,   320,   338,    33,    33,   338,    33,    33,   172,   174,
     174,   318,   215,   215,   215,   101,    45,   338,    45,   338,
     145,   172,   101,    45,   338,   215,    45,   338,   215,   215,
     215,   185,   185,   338,   185,    35,   164,   164,   237,   196,
     312,   172,   173,   214,   288,   311,   321,   149,   274,   305,
       3,    91,   102,   290,   291,   292,   338,   195,   216,   287,
     305,   174,    48,   174,   171,   171,    33,   185,   313,   245,
     144,   196,    45,   185,   338,   174,    45,   185,   338,   174,
     338,   198,    13,    37,    72,    37,    72,   164,   164,   338,
     338,   254,   170,   164,   172,     8,   219,   217,   174,   306,
     321,   174,   268,   172,   275,   275,   305,   101,    45,    45,
     101,    45,    45,    45,    45,   172,   338,   338,   338,   332,
     338,   338,   338,    35,   185,   274,   305,   312,   174,   313,
     288,   338,   292,   117,   145,   124,   150,   152,   153,   156,
     157,    61,   338,   309,   321,   317,   317,   185,   215,   172,
     338,   338,   185,   338,   185,   170,   110,   338,   201,   200,
     201,   200,    13,   170,   164,   218,   220,   306,   321,   164,
     286,   287,   338,   338,   338,   338,   338,   338,   101,   101,
     101,   101,   185,   274,   305,   288,   310,   311,   321,    48,
     174,   338,   291,   292,   292,   292,   292,   292,   292,   290,
     174,   172,   172,   196,   101,   101,   164,   164,   164,   164,
     338,   220,   101,   101,   101,   101,   101,   101,   338,   338,
     338,   338,   338,   310,   311,   321,   163,   163,   338,   338,
     174,   310
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int16 yyr1[] =
{
       0,   180,   181,   182,   182,   183,   183,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   186,   187,   186,   188,   186,   189,   189,   190,
     191,   191,   191,   192,   192,   192,   193,   193,   194,   195,
     195,   195,   196,   197,   197,   198,   198,   198,   198,   199,
     199,   199,   199,   199,   199,   200,   200,   201,   201,   202,
     202,   202,   202,   202,   202,   202,   203,   204,   204,   204,
     205,   205,   206,   207,   207,   207,   207,   207,   207,   208,
     208,   209,   209,   209,   209,   209,   209,   210,   210,   210,
     210,   210,   210,   211,   211,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   214,   214,   214,   214,
     215,   215,   216,   216,   217,   218,   217,   219,   217,   220,
     220,   221,   220,   222,   220,   223,   223,   223,   223,   223,
     223,   223,   224,   224,   224,   224,   225,   226,   226,   227,
     228,   228,   228,   229,   228,   230,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   232,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   234,   234,   235,   235,   236,   236,
     237,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   239,   239,   239,   239,   239,   239,
     240,   240,   240,   241,   241,   241,   242,   242,   242,   242,
     242,   242,   243,   243,   244,   244,   244,   245,   245,   246,
     247,   247,   248,   248,   249,   249,   249,   250,   250,   251,
     252,   252,   252,   253,   253,   254,   254,   254,   255,   255,
     256,   257,   257,   258,   257,   259,   260,   259,   261,   259,
     262,   262,   263,   264,   265,   265,   265,   266,   268,   267,
     269,   269,   269,   269,   269,   270,   271,   271,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   272,   272,   272,   272,
     272,   272,   272,   272,   272,   272,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   274,   274,
     274,   275,   275,   276,   277,   277,   278,   278,   279,   279,
     279,   279,   279,   279,   280,   280,   281,   281,   281,   281,
     281,   281,   281,   281,   281,   282,   282,   282,   282,   282,
     282,   283,   283,   283,   284,   284,   284,   284,   284,   284,
     285,   285,   286,   286,   287,   287,   288,   289,   289,   289,
     289,   289,   290,   290,   291,   291,   291,   291,   291,   291,
     291,   292,   292,   293,   294,   294,   294,   295,   295,   296,
     297,   297,   297,   298,   298,   298,   298,   298,   300,   299,
     299,   301,   301,   302,   302,   303,   303,   303,   304,   304,
     304,   305,   305,   305,   306,   306,   306,   306,   306,   306,
     306,   307,   307,   307,   307,   307,   308,   308,   308,   308,
     308,   309,   309,   309,   309,   310,   310,   310,   311,   311,
     311,   311,   311,   312,   312,   312,   312,   312,   313,   313,
     313,   313,   314,   314,   315,   315,   315,   316,   316,   317,
     317,   318,   318,   319,   319,   319,   319,   320,   320,   321,
     321,   321,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   323,   323,   323,   323,   323,   323,
     323,   323,   323,   323,   323,   323,   323,   323,   323,   323,
     323,   324,   325,   326,   326,   326,   326,   326,   326,   326,
     326,   327,   327,   328,   329,   329,   330,   331,   331,   332,
     332,   332,   333,   333,   333,   333,   333,   333,   334,   334,
     335,   335,   335,   335,   335,   336,   336,   336,   336,   336,
     337,   338,   338,   338,   338,   338,   338,   338,   338,   338,
     338,   338,   338,   338,   338,   338,   338,   338,   339,   339,
     340,   340,   340,   341,   341,   341,   341,   342,   342,   342,
     342,   342,   343,   343,   343,   344,   344,   344,   344,   344,
     344,   345,   345,   345,   345,   346,   346,   347,   347,   348,
     348,   348,   348,   348,   348,   348,   348,   348,   348,   348,
     348,   348,   349,   349,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   351,   351,   351,
     351,   351,   351,   351,   352,   352,   352,   352,   353,   353,
     353,   353,   354,   354,   354,   354,   354,   354,   354
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     2,     2,     3,     3,
       3,     3,     3,     3,     3,     2,     3,     3,     2,     2,
       3,     2,     1,     0,     4,     0,     3,     1,     1,     4,
       3,     4,     4,     0,     1,     1,     0,     1,     6,     2,
       3,     3,     1,     1,     2,     1,     3,     3,     5,     1,
       3,     3,     3,     5,     5,     0,     1,     1,     1,     4,
       6,     8,     8,     6,     8,     8,     4,     1,     3,     5,
       1,     3,     3,     4,     4,     4,     4,     4,     4,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     3,     1,     0,     4,     0,     3,     1,
       1,     0,     3,     0,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     5,     5,     2,     1,     1,     1,     1,
       6,     7,     3,     0,     6,     2,     5,     3,     3,     6,
       6,     4,     5,     5,     3,     3,     6,     5,     6,     5,
       6,     3,     4,     3,     4,     5,     6,     5,     6,     3,
       4,     3,     4,     6,     7,     6,     7,     4,     5,     4,
       5,     4,     4,     3,     6,     5,     4,     3,     6,     5,
       6,     5,     8,     7,     4,     4,     6,     3,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       6,     4,     7,     5,     3,     6,     3,     3,     2,     2,
       3,     3,     0,     2,     2,     3,     5,     1,     3,     3,
       5,     5,     0,     2,     3,     2,     3,     5,     5,     2,
       1,     1,     1,     0,     2,     0,     2,     3,     3,     3,
       3,     1,     2,     0,     4,     1,     0,     4,     0,     3,
       1,     3,     6,     0,     1,     1,     1,     1,     0,     7,
       4,     4,     6,     6,     4,     2,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     0,     3,     3,     1,     3,     0,     1,     4,     5,
       4,     5,     6,     6,     0,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     0,     2,     2,
       4,     4,     1,     3,     3,     3,     3,     3,     3,     3,
       2,     1,     1,     3,     1,     2,     2,     1,     3,     2,
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
#line 639 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6410 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 643 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6416 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 644 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6422 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6430 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6438 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 663 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6447 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: deprecated_decl_stmt  */
#line 670 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6453 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: include_module_stmt  */
#line 671 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6459 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: block_stmt  */
#line 672 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6465 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: use_stmt  */
#line 673 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6471 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: import_stmt  */
#line 674 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6477 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: require_stmt  */
#line 675 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6483 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: assignment_stmt  */
#line 676 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6489 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 677 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6495 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: if_stmt  */
#line 678 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6501 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: implements_stmt  */
#line 679 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6507 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: interface_stmt  */
#line 680 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6513 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: loop_stmt  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6519 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: select_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6525 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: TDEFER stmt  */
#line 684 "chpl.ypp"
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
#line 6542 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: try_stmt  */
#line 696 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6548 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: throw_stmt  */
#line 697 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6554 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: return_stmt  */
#line 698 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6560 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: stmt_level_expr TSEMI  */
#line 699 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6566 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: TATOMIC stmt  */
#line 700 "chpl.ypp"
                            { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6572 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt: TBEGIN opt_task_intent_ls stmt  */
#line 702 "chpl.ypp"
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
#line 6590 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 716 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6603 "bison-chpl-lib.cpp"
    break;

  case 30: /* stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 725 "chpl.ypp"
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
#line 6622 "bison-chpl-lib.cpp"
    break;

  case 31: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 740 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6635 "bison-chpl-lib.cpp"
    break;

  case 32: /* stmt: TDELETE simple_expr_ls TSEMI  */
#line 749 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6647 "bison-chpl-lib.cpp"
    break;

  case 33: /* stmt: TLABEL ident_def stmt  */
#line 757 "chpl.ypp"
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
#line 6674 "bison-chpl-lib.cpp"
    break;

  case 34: /* stmt: TLOCAL expr do_stmt  */
#line 780 "chpl.ypp"
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
#line 6690 "bison-chpl-lib.cpp"
    break;

  case 35: /* stmt: TLOCAL do_stmt  */
#line 792 "chpl.ypp"
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
#line 6705 "bison-chpl-lib.cpp"
    break;

  case 36: /* stmt: TON expr do_stmt  */
#line 803 "chpl.ypp"
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
#line 6721 "bison-chpl-lib.cpp"
    break;

  case 37: /* stmt: TSERIAL expr do_stmt  */
#line 815 "chpl.ypp"
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
#line 6737 "bison-chpl-lib.cpp"
    break;

  case 38: /* stmt: TSERIAL do_stmt  */
#line 827 "chpl.ypp"
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
#line 6752 "bison-chpl-lib.cpp"
    break;

  case 39: /* stmt: TSYNC stmt  */
#line 838 "chpl.ypp"
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
#line 6769 "bison-chpl-lib.cpp"
    break;

  case 40: /* stmt: TYIELD expr TSEMI  */
#line 851 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6780 "bison-chpl-lib.cpp"
    break;

  case 41: /* stmt: error TSEMI  */
#line 858 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
  }
#line 6788 "bison-chpl-lib.cpp"
    break;

  case 43: /* $@1: %empty  */
#line 866 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6796 "bison-chpl-lib.cpp"
    break;

  case 44: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 deprecated_decl_base  */
#line 870 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6804 "bison-chpl-lib.cpp"
    break;

  case 45: /* $@2: %empty  */
#line 874 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6812 "bison-chpl-lib.cpp"
    break;

  case 46: /* deprecated_decl_stmt: TDEPRECATED $@2 deprecated_decl_base  */
#line 878 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6820 "bison-chpl-lib.cpp"
    break;

  case 49: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 890 "chpl.ypp"
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
#line 6841 "bison-chpl-lib.cpp"
    break;

  case 50: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 910 "chpl.ypp"
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
#line 6860 "bison-chpl-lib.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 925 "chpl.ypp"
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
#line 6879 "bison-chpl-lib.cpp"
    break;

  case 52: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 940 "chpl.ypp"
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
#line 6896 "bison-chpl-lib.cpp"
    break;

  case 53: /* access_control: %empty  */
#line 955 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6902 "bison-chpl-lib.cpp"
    break;

  case 54: /* access_control: TPUBLIC  */
#line 956 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 55: /* access_control: TPRIVATE  */
#line 958 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6916 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_prototype: %empty  */
#line 963 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6922 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_prototype: TPROTOTYPE  */
#line 964 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6929 "bison-chpl-lib.cpp"
    break;

  case 58: /* include_module_stmt: TINCLUDE access_control opt_prototype TMODULE ident_def TSEMI  */
#line 970 "chpl.ypp"
   {
     (yyval.commentsAndStmt) = TODOSTMT((yyloc));
     context->visibility = Decl::DEFAULT_VISIBILITY;
   }
#line 6938 "bison-chpl-lib.cpp"
    break;

  case 59: /* block_stmt_body: TLCBR TRCBR  */
#line 987 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6946 "bison-chpl-lib.cpp"
    break;

  case 60: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 991 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6954 "bison-chpl-lib.cpp"
    break;

  case 61: /* block_stmt_body: TLCBR error TRCBR  */
#line 995 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6966 "bison-chpl-lib.cpp"
    break;

  case 62: /* block_stmt: block_stmt_body  */
#line 1006 "chpl.ypp"
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
#line 6985 "bison-chpl-lib.cpp"
    break;

  case 63: /* stmt_ls: toplevel_stmt  */
#line 1024 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6991 "bison-chpl-lib.cpp"
    break;

  case 64: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1025 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6997 "bison-chpl-lib.cpp"
    break;

  case 65: /* renames_ls: expr  */
#line 1030 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 66: /* renames_ls: expr TAS expr  */
#line 1034 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7014 "bison-chpl-lib.cpp"
    break;

  case 67: /* renames_ls: renames_ls TCOMMA expr  */
#line 1039 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7022 "bison-chpl-lib.cpp"
    break;

  case 68: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1043 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7031 "bison-chpl-lib.cpp"
    break;

  case 69: /* use_renames_ls: expr  */
#line 1053 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(node);
  }
#line 7040 "bison-chpl-lib.cpp"
    break;

  case 70: /* use_renames_ls: expr TAS expr  */
#line 1058 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7050 "bison-chpl-lib.cpp"
    break;

  case 71: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1064 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7061 "bison-chpl-lib.cpp"
    break;

  case 72: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1071 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7070 "bison-chpl-lib.cpp"
    break;

  case 73: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1076 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7081 "bison-chpl-lib.cpp"
    break;

  case 74: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1083 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7093 "bison-chpl-lib.cpp"
    break;

  case 75: /* opt_only_ls: %empty  */
#line 1093 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7099 "bison-chpl-lib.cpp"
    break;

  case 76: /* opt_only_ls: renames_ls  */
#line 1094 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7105 "bison-chpl-lib.cpp"
    break;

  case 77: /* except_ls: TSTAR  */
#line 1099 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.exprList) = context->makeList(ident);
  }
#line 7114 "bison-chpl-lib.cpp"
    break;

  case 78: /* except_ls: renames_ls  */
#line 1104 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[0].exprList);
  }
#line 7122 "bison-chpl-lib.cpp"
    break;

  case 79: /* use_stmt: access_control TUSE use_renames_ls TSEMI  */
#line 1111 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7130 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_stmt: access_control TUSE expr TEXCEPT except_ls TSEMI  */
#line 1115 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7141 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: access_control TUSE expr TAS expr TEXCEPT except_ls TSEMI  */
#line 1122 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7153 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TEXCEPT except_ls TSEMI  */
#line 1130 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7166 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1139 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7177 "bison-chpl-lib.cpp"
    break;

  case 84: /* use_stmt: access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1146 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7189 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1154 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7202 "bison-chpl-lib.cpp"
    break;

  case 86: /* import_stmt: access_control TIMPORT import_ls TSEMI  */
#line 1166 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7210 "bison-chpl-lib.cpp"
    break;

  case 87: /* import_expr: expr  */
#line 1173 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
  }
#line 7218 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_expr: expr TAS ident_use  */
#line 1177 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as));
  }
#line 7228 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1183 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)));
  }
#line 7238 "bison-chpl-lib.cpp"
    break;

  case 90: /* import_ls: import_expr  */
#line 1191 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7244 "bison-chpl-lib.cpp"
    break;

  case 91: /* import_ls: import_ls TCOMMA import_expr  */
#line 1192 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7250 "bison-chpl-lib.cpp"
    break;

  case 92: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1197 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7260 "bison-chpl-lib.cpp"
    break;

  case 93: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1206 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7268 "bison-chpl-lib.cpp"
    break;

  case 94: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1210 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7276 "bison-chpl-lib.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1214 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7284 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1218 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7292 "bison-chpl-lib.cpp"
    break;

  case 97: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1222 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7300 "bison-chpl-lib.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1226 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7308 "bison-chpl-lib.cpp"
    break;

  case 99: /* opt_label_ident: %empty  */
#line 1234 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7314 "bison-chpl-lib.cpp"
    break;

  case 100: /* opt_label_ident: TIDENT  */
#line 1235 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7320 "bison-chpl-lib.cpp"
    break;

  case 101: /* ident_fn_def: TIDENT  */
#line 1239 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7326 "bison-chpl-lib.cpp"
    break;

  case 102: /* ident_fn_def: TNONE  */
#line 1240 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7332 "bison-chpl-lib.cpp"
    break;

  case 103: /* ident_fn_def: TTHIS  */
#line 1241 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7338 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: TFALSE  */
#line 1242 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7344 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_fn_def: TTRUE  */
#line 1243 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7350 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_fn_def: internal_type_ident_def  */
#line 1244 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7356 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_def: TIDENT  */
#line 1247 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7362 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_def: TNONE  */
#line 1248 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7368 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_def: TTHIS  */
#line 1249 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'this'"); }
#line 7374 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: TFALSE  */
#line 1250 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7380 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_def: TTRUE  */
#line 1251 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7386 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_def: internal_type_ident_def  */
#line 1252 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7392 "bison-chpl-lib.cpp"
    break;

  case 113: /* ident_use: TIDENT  */
#line 1264 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7398 "bison-chpl-lib.cpp"
    break;

  case 114: /* ident_use: TTHIS  */
#line 1265 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7404 "bison-chpl-lib.cpp"
    break;

  case 134: /* scalar_type: TBOOL  */
#line 1298 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7410 "bison-chpl-lib.cpp"
    break;

  case 135: /* scalar_type: TENUM  */
#line 1299 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7416 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TINT  */
#line 1300 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7422 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TUINT  */
#line 1301 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7428 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TREAL  */
#line 1302 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7434 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TIMAG  */
#line 1303 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7440 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TCOMPLEX  */
#line 1304 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7446 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TBYTES  */
#line 1305 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7452 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TSTRING  */
#line 1306 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7458 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TLOCALE  */
#line 1307 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7464 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TNOTHING  */
#line 1308 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7470 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TVOID  */
#line 1309 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7476 "bison-chpl-lib.cpp"
    break;

  case 150: /* do_stmt: TDO stmt  */
#line 1323 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7482 "bison-chpl-lib.cpp"
    break;

  case 151: /* do_stmt: block_stmt  */
#line 1324 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7488 "bison-chpl-lib.cpp"
    break;

  case 152: /* return_stmt: TRETURN TSEMI  */
#line 1329 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7499 "bison-chpl-lib.cpp"
    break;

  case 153: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1336 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 155: /* $@3: %empty  */
#line 1347 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7518 "bison-chpl-lib.cpp"
    break;

  case 156: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@3 class_level_stmt  */
#line 1351 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7526 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@4: %empty  */
#line 1355 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 158: /* deprecated_class_level_stmt: TDEPRECATED $@4 class_level_stmt  */
#line 1359 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7542 "bison-chpl-lib.cpp"
    break;

  case 159: /* class_level_stmt: TSEMI  */
#line 1366 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), nullptr);
    }
#line 7550 "bison-chpl-lib.cpp"
    break;

  case 160: /* class_level_stmt: inner_class_level_stmt  */
#line 1370 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7560 "bison-chpl-lib.cpp"
    break;

  case 161: /* $@5: %empty  */
#line 1375 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7567 "bison-chpl-lib.cpp"
    break;

  case 162: /* class_level_stmt: TPUBLIC $@5 inner_class_level_stmt  */
#line 1377 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 163: /* $@6: %empty  */
#line 1381 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7583 "bison-chpl-lib.cpp"
    break;

  case 164: /* class_level_stmt: TPRIVATE $@6 inner_class_level_stmt  */
#line 1383 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7592 "bison-chpl-lib.cpp"
    break;

  case 172: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1401 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7601 "bison-chpl-lib.cpp"
    break;

  case 173: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT except_ls TSEMI  */
#line 1406 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 174: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1411 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7619 "bison-chpl-lib.cpp"
    break;

  case 175: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1416 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7627 "bison-chpl-lib.cpp"
    break;

  case 176: /* forwarding_decl_start: TFORWARDING  */
#line 1423 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 177: /* extern_or_export: TEXTERN  */
#line 1430 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7642 "bison-chpl-lib.cpp"
    break;

  case 178: /* extern_or_export: TEXPORT  */
#line 1431 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 179: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1436 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7658 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1446 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7667 "bison-chpl-lib.cpp"
    break;

  case 181: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1452 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7678 "bison-chpl-lib.cpp"
    break;

  case 182: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1459 "chpl.ypp"
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
#line 7701 "bison-chpl-lib.cpp"
    break;

  case 183: /* $@7: %empty  */
#line 1478 "chpl.ypp"
  {
    // This will be consumed when building the first variable in the list.
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7710 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@7 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1483 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7719 "bison-chpl-lib.cpp"
    break;

  case 185: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1491 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7727 "bison-chpl-lib.cpp"
    break;

  case 186: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1498 "chpl.ypp"
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
#line 7749 "bison-chpl-lib.cpp"
    break;

  case 187: /* loop_stmt: TWHILE expr do_stmt  */
#line 1516 "chpl.ypp"
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
#line 7766 "bison-chpl-lib.cpp"
    break;

  case 188: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1529 "chpl.ypp"
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
#line 7783 "bison-chpl-lib.cpp"
    break;

  case 189: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1542 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7791 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1546 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7799 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1550 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7807 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1554 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7815 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1558 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7823 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TFOR expr do_stmt  */
#line 1562 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7831 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1566 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7839 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1570 "chpl.ypp"
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
#line 7861 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1588 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7869 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1592 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7877 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1596 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7885 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1600 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7893 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFORALL expr do_stmt  */
#line 1604 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7901 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1608 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7909 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1612 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7917 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1616 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7925 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1620 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7933 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1624 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7941 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1628 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7949 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1632 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7957 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1636 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7965 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1640 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7973 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1644 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7981 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1648 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7989 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1652 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7997 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1656 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8005 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1660 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8013 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1664 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8021 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1668 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8029 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1672 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8037 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1676 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8046 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1681 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 221: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1689 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8065 "bison-chpl-lib.cpp"
    break;

  case 222: /* if_stmt: TIF expr TTHEN stmt  */
#line 1698 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8073 "bison-chpl-lib.cpp"
    break;

  case 223: /* if_stmt: TIF expr block_stmt  */
#line 1702 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 224: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1706 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8089 "bison-chpl-lib.cpp"
    break;

  case 225: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1710 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8097 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1714 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8105 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF ifvar block_stmt  */
#line 1718 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8113 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1722 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8121 "bison-chpl-lib.cpp"
    break;

  case 229: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1726 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8129 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1730 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8138 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1735 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8147 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1740 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8156 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1745 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8165 "bison-chpl-lib.cpp"
    break;

  case 234: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1753 "chpl.ypp"
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
#line 8184 "bison-chpl-lib.cpp"
    break;

  case 235: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1768 "chpl.ypp"
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
#line 8203 "bison-chpl-lib.cpp"
    break;

  case 236: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1786 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8209 "bison-chpl-lib.cpp"
    break;

  case 237: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1788 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8215 "bison-chpl-lib.cpp"
    break;

  case 238: /* ifc_formal_ls: ifc_formal  */
#line 1792 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8221 "bison-chpl-lib.cpp"
    break;

  case 239: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1793 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8227 "bison-chpl-lib.cpp"
    break;

  case 240: /* ifc_formal: ident_def  */
#line 1798 "chpl.ypp"
             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8233 "bison-chpl-lib.cpp"
    break;

  case 253: /* implements_type_ident: implements_type_error_ident  */
#line 1816 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8245 "bison-chpl-lib.cpp"
    break;

  case 260: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1845 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8251 "bison-chpl-lib.cpp"
    break;

  case 261: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1847 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8257 "bison-chpl-lib.cpp"
    break;

  case 262: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1849 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8263 "bison-chpl-lib.cpp"
    break;

  case 263: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1854 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8269 "bison-chpl-lib.cpp"
    break;

  case 264: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1856 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8275 "bison-chpl-lib.cpp"
    break;

  case 265: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1858 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8281 "bison-chpl-lib.cpp"
    break;

  case 266: /* try_stmt: TTRY expr TSEMI  */
#line 1863 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[-1].expr), false);
  }
#line 8289 "bison-chpl-lib.cpp"
    break;

  case 267: /* try_stmt: TTRYBANG expr TSEMI  */
#line 1867 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[-1].expr), true);
  }
#line 8297 "bison-chpl-lib.cpp"
    break;

  case 268: /* try_stmt: TTRY assignment_stmt  */
#line 1871 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8305 "bison-chpl-lib.cpp"
    break;

  case 269: /* try_stmt: TTRYBANG assignment_stmt  */
#line 1875 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8313 "bison-chpl-lib.cpp"
    break;

  case 270: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1879 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8321 "bison-chpl-lib.cpp"
    break;

  case 271: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1883 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8329 "bison-chpl-lib.cpp"
    break;

  case 272: /* catch_expr_ls: %empty  */
#line 1889 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8335 "bison-chpl-lib.cpp"
    break;

  case 273: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1890 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8341 "bison-chpl-lib.cpp"
    break;

  case 274: /* catch_expr: TCATCH block_stmt  */
#line 1895 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8349 "bison-chpl-lib.cpp"
    break;

  case 275: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1899 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8357 "bison-chpl-lib.cpp"
    break;

  case 276: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1903 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8365 "bison-chpl-lib.cpp"
    break;

  case 277: /* catch_expr_inner: ident_def  */
#line 1910 "chpl.ypp"
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
#line 8382 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1923 "chpl.ypp"
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
#line 8399 "bison-chpl-lib.cpp"
    break;

  case 279: /* throw_stmt: TTHROW expr TSEMI  */
#line 1939 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8409 "bison-chpl-lib.cpp"
    break;

  case 280: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1947 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8417 "bison-chpl-lib.cpp"
    break;

  case 281: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1951 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8427 "bison-chpl-lib.cpp"
    break;

  case 282: /* when_stmt_ls: %empty  */
#line 1959 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8433 "bison-chpl-lib.cpp"
    break;

  case 283: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1960 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8439 "bison-chpl-lib.cpp"
    break;

  case 284: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1965 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8447 "bison-chpl-lib.cpp"
    break;

  case 285: /* when_stmt: TOTHERWISE stmt  */
#line 1969 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8456 "bison-chpl-lib.cpp"
    break;

  case 286: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1974 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8465 "bison-chpl-lib.cpp"
    break;

  case 287: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1984 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8474 "bison-chpl-lib.cpp"
    break;

  case 288: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1989 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8485 "bison-chpl-lib.cpp"
    break;

  case 289: /* class_start: class_tag ident_def  */
#line 2000 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8493 "bison-chpl-lib.cpp"
    break;

  case 290: /* class_tag: TCLASS  */
#line 2006 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8499 "bison-chpl-lib.cpp"
    break;

  case 291: /* class_tag: TRECORD  */
#line 2007 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8505 "bison-chpl-lib.cpp"
    break;

  case 292: /* class_tag: TUNION  */
#line 2008 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8511 "bison-chpl-lib.cpp"
    break;

  case 293: /* opt_inherit: %empty  */
#line 2012 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8517 "bison-chpl-lib.cpp"
    break;

  case 294: /* opt_inherit: TCOLON expr_ls  */
#line 2013 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8523 "bison-chpl-lib.cpp"
    break;

  case 295: /* class_level_stmt_ls: %empty  */
#line 2017 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8532 "bison-chpl-lib.cpp"
    break;

  case 296: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2022 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8540 "bison-chpl-lib.cpp"
    break;

  case 297: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2026 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8548 "bison-chpl-lib.cpp"
    break;

  case 298: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2033 "chpl.ypp"
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
#line 8569 "bison-chpl-lib.cpp"
    break;

  case 299: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2050 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8583 "bison-chpl-lib.cpp"
    break;

  case 300: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2063 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8591 "bison-chpl-lib.cpp"
    break;

  case 301: /* enum_ls: deprecated_enum_item  */
#line 2070 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8600 "bison-chpl-lib.cpp"
    break;

  case 302: /* enum_ls: enum_ls TCOMMA  */
#line 2075 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8610 "bison-chpl-lib.cpp"
    break;

  case 303: /* $@8: %empty  */
#line 2081 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8619 "bison-chpl-lib.cpp"
    break;

  case 304: /* enum_ls: enum_ls TCOMMA $@8 deprecated_enum_item  */
#line 2086 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8628 "bison-chpl-lib.cpp"
    break;

  case 306: /* $@9: %empty  */
#line 2095 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8636 "bison-chpl-lib.cpp"
    break;

  case 307: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@9 enum_item  */
#line 2099 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8644 "bison-chpl-lib.cpp"
    break;

  case 308: /* $@10: %empty  */
#line 2103 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8652 "bison-chpl-lib.cpp"
    break;

  case 309: /* deprecated_enum_item: TDEPRECATED $@10 enum_item  */
#line 2107 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8660 "bison-chpl-lib.cpp"
    break;

  case 310: /* enum_item: ident_def  */
#line 2114 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8671 "bison-chpl-lib.cpp"
    break;

  case 311: /* enum_item: ident_def TASSIGN expr  */
#line 2121 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8684 "bison-chpl-lib.cpp"
    break;

  case 312: /* lambda_decl_expr: TLAMBDA req_formal_ls opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 2133 "chpl.ypp"
  {
    // TODO (dlongnecke): Leave this unimplemented for now? Since we will
    // be addressing FCFs in 1.26...
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 8694 "bison-chpl-lib.cpp"
    break;

  case 314: /* linkage_spec: linkage_spec_empty  */
#line 2144 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8700 "bison-chpl-lib.cpp"
    break;

  case 315: /* linkage_spec: TINLINE  */
#line 2145 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8707 "bison-chpl-lib.cpp"
    break;

  case 316: /* linkage_spec: TOVERRIDE  */
#line 2147 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8714 "bison-chpl-lib.cpp"
    break;

  case 317: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2153 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8722 "bison-chpl-lib.cpp"
    break;

  case 318: /* $@11: %empty  */
#line 2162 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name);
    }
#line 8732 "bison-chpl-lib.cpp"
    break;

  case 319: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@11 opt_function_body_stmt  */
#line 2168 "chpl.ypp"
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
#line 8753 "bison-chpl-lib.cpp"
    break;

  case 320: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2188 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8765 "bison-chpl-lib.cpp"
    break;

  case 321: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2196 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8777 "bison-chpl-lib.cpp"
    break;

  case 322: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2204 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])), /*attributes*/ nullptr,
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8792 "bison-chpl-lib.cpp"
    break;

  case 323: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2215 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])), /*attributes*/ nullptr,
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8807 "bison-chpl-lib.cpp"
    break;

  case 324: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2226 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8817 "bison-chpl-lib.cpp"
    break;

  case 325: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2235 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-1].functionParts);
      auto loc = context->declStartLoc((yylsp[0]));
      fp.comments = context->gatherComments(loc);
      fp.attributes = context->buildAttributes((yyloc)).release();
      context->resetAttributePartsState();
      fp.kind = (yyvsp[0].functionKind);
      (yyval.functionParts) = fp;
    }
#line 8831 "bison-chpl-lib.cpp"
    break;

  case 327: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2248 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8837 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_ident: ident_def TBANG  */
#line 2280 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8847 "bison-chpl-lib.cpp"
    break;

  case 368: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2303 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8853 "bison-chpl-lib.cpp"
    break;

  case 369: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2304 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8859 "bison-chpl-lib.cpp"
    break;

  case 370: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2305 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8865 "bison-chpl-lib.cpp"
    break;

  case 371: /* opt_formal_ls: %empty  */
#line 2309 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8871 "bison-chpl-lib.cpp"
    break;

  case 372: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2310 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8877 "bison-chpl-lib.cpp"
    break;

  case 373: /* req_formal_ls: TLP formal_ls TRP  */
#line 2314 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8883 "bison-chpl-lib.cpp"
    break;

  case 374: /* formal_ls_inner: formal  */
#line 2318 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8889 "bison-chpl-lib.cpp"
    break;

  case 375: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2319 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8895 "bison-chpl-lib.cpp"
    break;

  case 376: /* formal_ls: %empty  */
#line 2323 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8901 "bison-chpl-lib.cpp"
    break;

  case 377: /* formal_ls: formal_ls_inner  */
#line 2324 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8907 "bison-chpl-lib.cpp"
    break;

  case 378: /* formal: opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2329 "chpl.ypp"
  {
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 379: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2338 "chpl.ypp"
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
#line 8935 "bison-chpl-lib.cpp"
    break;

  case 380: /* formal: opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2349 "chpl.ypp"
  {
    (yyval.expr) = VarArgFormal::build(BUILDER, LOC((yyloc)),
                             /*attributes*/ nullptr,
                             /*name*/ (yyvsp[-2].uniqueStr),
                             /*intent*/ (yyvsp[-3].intentTag),
                             toOwned((yyvsp[-1].expr)),
                             toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 8949 "bison-chpl-lib.cpp"
    break;

  case 381: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2359 "chpl.ypp"
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
#line 8965 "bison-chpl-lib.cpp"
    break;

  case 382: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2372 "chpl.ypp"
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
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 383: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type formal_var_arg_expr  */
#line 2384 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
  }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 384: /* opt_formal_intent_tag: %empty  */
#line 2390 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8997 "bison-chpl-lib.cpp"
    break;

  case 385: /* opt_formal_intent_tag: required_intent_tag  */
#line 2395 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 386: /* required_intent_tag: TIN  */
#line 2402 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 387: /* required_intent_tag: TINOUT  */
#line 2403 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 388: /* required_intent_tag: TOUT  */
#line 2404 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 389: /* required_intent_tag: TCONST TIN  */
#line 2405 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 390: /* required_intent_tag: TCONST TREF  */
#line 2406 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 391: /* required_intent_tag: TCONST  */
#line 2407 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 392: /* required_intent_tag: TPARAM  */
#line 2408 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 393: /* required_intent_tag: TREF  */
#line 2409 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 394: /* required_intent_tag: TTYPE  */
#line 2410 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 395: /* opt_this_intent_tag: %empty  */
#line 2414 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 396: /* opt_this_intent_tag: TPARAM  */
#line 2415 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 397: /* opt_this_intent_tag: TREF  */
#line 2416 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 398: /* opt_this_intent_tag: TCONST TREF  */
#line 2417 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 399: /* opt_this_intent_tag: TCONST  */
#line 2418 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 400: /* opt_this_intent_tag: TTYPE  */
#line 2419 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 401: /* proc_iter_or_op: TPROC  */
#line 2423 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 402: /* proc_iter_or_op: TITER  */
#line 2424 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 403: /* proc_iter_or_op: TOPERATOR  */
#line 2425 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 404: /* opt_ret_tag: %empty  */
#line 2429 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 405: /* opt_ret_tag: TCONST  */
#line 2430 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 406: /* opt_ret_tag: TCONST TREF  */
#line 2431 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 407: /* opt_ret_tag: TREF  */
#line 2432 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_ret_tag: TPARAM  */
#line 2433 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_ret_tag: TTYPE  */
#line 2434 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_throws_error: %empty  */
#line 2438 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9156 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_throws_error: TTHROWS  */
#line 2439 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_function_body_stmt: TSEMI  */
#line 2442 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 413: /* opt_function_body_stmt: function_body_stmt  */
#line 2443 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 414: /* function_body_stmt: block_stmt_body  */
#line 2447 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9180 "bison-chpl-lib.cpp"
    break;

  case 415: /* function_body_stmt: return_stmt  */
#line 2448 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9186 "bison-chpl-lib.cpp"
    break;

  case 416: /* query_expr: TQUERIEDIDENT  */
#line 2452 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 417: /* opt_lifetime_where: %empty  */
#line 2457 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9198 "bison-chpl-lib.cpp"
    break;

  case 418: /* opt_lifetime_where: TWHERE expr  */
#line 2459 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 419: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2461 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 420: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2463 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9216 "bison-chpl-lib.cpp"
    break;

  case 421: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2465 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 422: /* lifetime_components_expr: lifetime_expr  */
#line 2470 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9228 "bison-chpl-lib.cpp"
    break;

  case 423: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2472 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9234 "bison-chpl-lib.cpp"
    break;

  case 424: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2477 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9240 "bison-chpl-lib.cpp"
    break;

  case 425: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2479 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9246 "bison-chpl-lib.cpp"
    break;

  case 426: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2481 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9252 "bison-chpl-lib.cpp"
    break;

  case 427: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2483 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9258 "bison-chpl-lib.cpp"
    break;

  case 428: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2485 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9264 "bison-chpl-lib.cpp"
    break;

  case 429: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2487 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9270 "bison-chpl-lib.cpp"
    break;

  case 430: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2489 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9276 "bison-chpl-lib.cpp"
    break;

  case 431: /* lifetime_ident: TIDENT  */
#line 2493 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9282 "bison-chpl-lib.cpp"
    break;

  case 432: /* lifetime_ident: TTHIS  */
#line 2494 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9288 "bison-chpl-lib.cpp"
    break;

  case 433: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2499 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9297 "bison-chpl-lib.cpp"
    break;

  case 434: /* type_alias_decl_stmt_start: TTYPE  */
#line 2507 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 435: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2511 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9314 "bison-chpl-lib.cpp"
    break;

  case 436: /* type_alias_decl_stmt_start: TEXTERN TTYPE  */
#line 2516 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteLinkage(Decl::EXTERN);
  }
#line 9323 "bison-chpl-lib.cpp"
    break;

  case 437: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2524 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 438: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner TCOMMA type_alias_decl_stmt_inner_ls  */
#line 2528 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[0].exprList), (yyvsp[-2].commentsAndStmt));
  }
#line 9339 "bison-chpl-lib.cpp"
    break;

  case 439: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2535 "chpl.ypp"
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
#line 9361 "bison-chpl-lib.cpp"
    break;

  case 440: /* opt_init_type: %empty  */
#line 2555 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9367 "bison-chpl-lib.cpp"
    break;

  case 441: /* opt_init_type: TASSIGN type_level_expr  */
#line 2557 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9373 "bison-chpl-lib.cpp"
    break;

  case 442: /* opt_init_type: TASSIGN array_type  */
#line 2559 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9382 "bison-chpl-lib.cpp"
    break;

  case 443: /* var_decl_type: TPARAM  */
#line 2566 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9388 "bison-chpl-lib.cpp"
    break;

  case 444: /* var_decl_type: TCONST TREF  */
#line 2567 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9394 "bison-chpl-lib.cpp"
    break;

  case 445: /* var_decl_type: TREF  */
#line 2568 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9400 "bison-chpl-lib.cpp"
    break;

  case 446: /* var_decl_type: TCONST  */
#line 2569 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9406 "bison-chpl-lib.cpp"
    break;

  case 447: /* var_decl_type: TVAR  */
#line 2570 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9412 "bison-chpl-lib.cpp"
    break;

  case 448: /* $@12: %empty  */
#line 2575 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9421 "bison-chpl-lib.cpp"
    break;

  case 449: /* var_decl_stmt: TCONFIG $@12 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2579 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9430 "bison-chpl-lib.cpp"
    break;

  case 450: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2584 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 451: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2592 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 452: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2596 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9455 "bison-chpl-lib.cpp"
    break;

  case 453: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2603 "chpl.ypp"
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
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 454: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2619 "chpl.ypp"
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
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 455: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2636 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9501 "bison-chpl-lib.cpp"
    break;

  case 456: /* tuple_var_decl_component: ident_def  */
#line 2640 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9509 "bison-chpl-lib.cpp"
    break;

  case 457: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2644 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 458: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2651 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 459: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2653 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 460: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2655 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_init_expr: %empty  */
#line 2661 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2662 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2663 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 464: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2669 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 465: /* ret_array_type: TLSBR TRSBR  */
#line 2673 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 9569 "bison-chpl-lib.cpp"
    break;

  case 466: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2677 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 467: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2681 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 9585 "bison-chpl-lib.cpp"
    break;

  case 468: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2685 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9593 "bison-chpl-lib.cpp"
    break;

  case 469: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2689 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9601 "bison-chpl-lib.cpp"
    break;

  case 470: /* ret_array_type: TLSBR error TRSBR  */
#line 2693 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "invalid expression for domain of array return type");
  }
#line 9609 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_ret_type: %empty  */
#line 2699 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9615 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_ret_type: TCOLON type_level_expr  */
#line 2700 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9621 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_ret_type: TCOLON ret_array_type  */
#line 2701 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9627 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2702 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9633 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_ret_type: error  */
#line 2703 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9639 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_type: %empty  */
#line 2708 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_type: TCOLON type_level_expr  */
#line 2709 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9651 "bison-chpl-lib.cpp"
    break;

  case 478: /* opt_type: TCOLON array_type  */
#line 2710 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2711 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9663 "bison-chpl-lib.cpp"
    break;

  case 480: /* opt_type: error  */
#line 2712 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 481: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2733 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9677 "bison-chpl-lib.cpp"
    break;

  case 482: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2737 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9685 "bison-chpl-lib.cpp"
    break;

  case 483: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2741 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9693 "bison-chpl-lib.cpp"
    break;

  case 484: /* array_type: TLSBR error TRSBR  */
#line 2745 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 9701 "bison-chpl-lib.cpp"
    break;

  case 485: /* opt_formal_array_elt_type: %empty  */
#line 2751 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9707 "bison-chpl-lib.cpp"
    break;

  case 486: /* opt_formal_array_elt_type: type_level_expr  */
#line 2752 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9713 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_formal_array_elt_type: query_expr  */
#line 2753 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9719 "bison-chpl-lib.cpp"
    break;

  case 488: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2758 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9728 "bison-chpl-lib.cpp"
    break;

  case 489: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2763 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9736 "bison-chpl-lib.cpp"
    break;

  case 490: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2771 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9745 "bison-chpl-lib.cpp"
    break;

  case 491: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2776 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 492: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2780 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 493: /* opt_formal_type: %empty  */
#line 2786 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9767 "bison-chpl-lib.cpp"
    break;

  case 494: /* opt_formal_type: TCOLON type_level_expr  */
#line 2787 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9773 "bison-chpl-lib.cpp"
    break;

  case 495: /* opt_formal_type: TCOLON query_expr  */
#line 2788 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9779 "bison-chpl-lib.cpp"
    break;

  case 496: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2789 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9785 "bison-chpl-lib.cpp"
    break;

  case 497: /* opt_formal_type: TCOLON formal_array_type  */
#line 2790 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9791 "bison-chpl-lib.cpp"
    break;

  case 498: /* expr_ls: expr  */
#line 2796 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9797 "bison-chpl-lib.cpp"
    break;

  case 499: /* expr_ls: query_expr  */
#line 2797 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 500: /* expr_ls: expr_ls TCOMMA expr  */
#line 2798 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9809 "bison-chpl-lib.cpp"
    break;

  case 501: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2799 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9815 "bison-chpl-lib.cpp"
    break;

  case 502: /* simple_expr_ls: expr  */
#line 2803 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9821 "bison-chpl-lib.cpp"
    break;

  case 503: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2804 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9827 "bison-chpl-lib.cpp"
    break;

  case 504: /* tuple_component: TUNDERSCORE  */
#line 2808 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9833 "bison-chpl-lib.cpp"
    break;

  case 505: /* tuple_component: opt_try_expr  */
#line 2809 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9839 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_component: query_expr  */
#line 2810 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9845 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2815 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9853 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2819 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9861 "bison-chpl-lib.cpp"
    break;

  case 509: /* opt_actual_ls: %empty  */
#line 2825 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9867 "bison-chpl-lib.cpp"
    break;

  case 510: /* opt_actual_ls: actual_ls  */
#line 2826 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9873 "bison-chpl-lib.cpp"
    break;

  case 511: /* actual_ls: actual_expr  */
#line 2831 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9882 "bison-chpl-lib.cpp"
    break;

  case 512: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2836 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9892 "bison-chpl-lib.cpp"
    break;

  case 513: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2844 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9898 "bison-chpl-lib.cpp"
    break;

  case 514: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2845 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9904 "bison-chpl-lib.cpp"
    break;

  case 515: /* actual_expr: query_expr  */
#line 2846 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9910 "bison-chpl-lib.cpp"
    break;

  case 516: /* actual_expr: opt_try_expr  */
#line 2847 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9916 "bison-chpl-lib.cpp"
    break;

  case 517: /* ident_expr: ident_use  */
#line 2851 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 518: /* ident_expr: scalar_type  */
#line 2852 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9928 "bison-chpl-lib.cpp"
    break;

  case 519: /* type_level_expr: sub_type_level_expr  */
#line 2864 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9934 "bison-chpl-lib.cpp"
    break;

  case 520: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2866 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 9940 "bison-chpl-lib.cpp"
    break;

  case 521: /* type_level_expr: TQUESTION  */
#line 2868 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 9946 "bison-chpl-lib.cpp"
    break;

  case 527: /* sub_type_level_expr: TSINGLE expr  */
#line 2879 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9952 "bison-chpl-lib.cpp"
    break;

  case 528: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2881 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9958 "bison-chpl-lib.cpp"
    break;

  case 529: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2883 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9964 "bison-chpl-lib.cpp"
    break;

  case 530: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2885 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9970 "bison-chpl-lib.cpp"
    break;

  case 531: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2887 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9980 "bison-chpl-lib.cpp"
    break;

  case 532: /* sub_type_level_expr: TATOMIC expr  */
#line 2893 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9986 "bison-chpl-lib.cpp"
    break;

  case 533: /* sub_type_level_expr: TSYNC expr  */
#line 2895 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9992 "bison-chpl-lib.cpp"
    break;

  case 534: /* sub_type_level_expr: TOWNED  */
#line 2898 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9998 "bison-chpl-lib.cpp"
    break;

  case 535: /* sub_type_level_expr: TOWNED expr  */
#line 2900 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10004 "bison-chpl-lib.cpp"
    break;

  case 536: /* sub_type_level_expr: TUNMANAGED  */
#line 2902 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10010 "bison-chpl-lib.cpp"
    break;

  case 537: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2904 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10016 "bison-chpl-lib.cpp"
    break;

  case 538: /* sub_type_level_expr: TSHARED  */
#line 2906 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10022 "bison-chpl-lib.cpp"
    break;

  case 539: /* sub_type_level_expr: TSHARED expr  */
#line 2908 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10028 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TBORROWED  */
#line 2910 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10034 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TBORROWED expr  */
#line 2912 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10040 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TCLASS  */
#line 2915 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10046 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TRECORD  */
#line 2917 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10052 "bison-chpl-lib.cpp"
    break;

  case 544: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2922 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10065 "bison-chpl-lib.cpp"
    break;

  case 545: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2931 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10078 "bison-chpl-lib.cpp"
    break;

  case 546: /* for_expr: TFOR expr TDO expr  */
#line 2940 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[0].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 547: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2948 "chpl.ypp"
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
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 548: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2962 "chpl.ypp"
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
#line 10126 "bison-chpl-lib.cpp"
    break;

  case 549: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2976 "chpl.ypp"
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
#line 10144 "bison-chpl-lib.cpp"
    break;

  case 550: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 2990 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10157 "bison-chpl-lib.cpp"
    break;

  case 551: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 2999 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10170 "bison-chpl-lib.cpp"
    break;

  case 552: /* for_expr: TFORALL expr TDO expr  */
#line 3008 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[0].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10182 "bison-chpl-lib.cpp"
    break;

  case 553: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3016 "chpl.ypp"
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
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 554: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3030 "chpl.ypp"
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
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 555: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3044 "chpl.ypp"
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
#line 10236 "bison-chpl-lib.cpp"
    break;

  case 556: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 3058 "chpl.ypp"
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
#line 10256 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3074 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10269 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3083 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10282 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3092 "chpl.ypp"
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
#line 10301 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3107 "chpl.ypp"
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
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 561: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3125 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10334 "bison-chpl-lib.cpp"
    break;

  case 562: /* nil_expr: TNIL  */
#line 3142 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10340 "bison-chpl-lib.cpp"
    break;

  case 570: /* stmt_level_expr: io_expr TIO expr  */
#line 3158 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 571: /* opt_task_intent_ls: %empty  */
#line 3162 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10352 "bison-chpl-lib.cpp"
    break;

  case 572: /* opt_task_intent_ls: task_intent_clause  */
#line 3163 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10358 "bison-chpl-lib.cpp"
    break;

  case 573: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3168 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10368 "bison-chpl-lib.cpp"
    break;

  case 574: /* task_intent_ls: intent_expr  */
#line 3176 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10374 "bison-chpl-lib.cpp"
    break;

  case 575: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3177 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10380 "bison-chpl-lib.cpp"
    break;

  case 576: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3182 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10390 "bison-chpl-lib.cpp"
    break;

  case 577: /* forall_intent_ls: intent_expr  */
#line 3190 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10396 "bison-chpl-lib.cpp"
    break;

  case 578: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3191 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10402 "bison-chpl-lib.cpp"
    break;

  case 579: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3196 "chpl.ypp"
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
#line 10421 "bison-chpl-lib.cpp"
    break;

  case 580: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3211 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 10429 "bison-chpl-lib.cpp"
    break;

  case 581: /* intent_expr: expr TREDUCE ident_expr  */
#line 3215 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10437 "bison-chpl-lib.cpp"
    break;

  case 582: /* task_var_prefix: TCONST  */
#line 3221 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10443 "bison-chpl-lib.cpp"
    break;

  case 583: /* task_var_prefix: TIN  */
#line 3222 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10449 "bison-chpl-lib.cpp"
    break;

  case 584: /* task_var_prefix: TCONST TIN  */
#line 3223 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10455 "bison-chpl-lib.cpp"
    break;

  case 585: /* task_var_prefix: TREF  */
#line 3224 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10461 "bison-chpl-lib.cpp"
    break;

  case 586: /* task_var_prefix: TCONST TREF  */
#line 3225 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10467 "bison-chpl-lib.cpp"
    break;

  case 587: /* task_var_prefix: TVAR  */
#line 3226 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10473 "bison-chpl-lib.cpp"
    break;

  case 589: /* io_expr: io_expr TIO expr  */
#line 3232 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10479 "bison-chpl-lib.cpp"
    break;

  case 590: /* new_maybe_decorated: TNEW  */
#line 3237 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 591: /* new_maybe_decorated: TNEW TOWNED  */
#line 3239 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 592: /* new_maybe_decorated: TNEW TSHARED  */
#line 3241 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10497 "bison-chpl-lib.cpp"
    break;

  case 593: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3243 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10503 "bison-chpl-lib.cpp"
    break;

  case 594: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3245 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10509 "bison-chpl-lib.cpp"
    break;

  case 595: /* new_expr: new_maybe_decorated expr  */
#line 3251 "chpl.ypp"
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
#line 10527 "bison-chpl-lib.cpp"
    break;

  case 596: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3268 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10535 "bison-chpl-lib.cpp"
    break;

  case 597: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3272 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10543 "bison-chpl-lib.cpp"
    break;

  case 598: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3276 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10551 "bison-chpl-lib.cpp"
    break;

  case 599: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3280 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10559 "bison-chpl-lib.cpp"
    break;

  case 600: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3287 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10565 "bison-chpl-lib.cpp"
    break;

  case 610: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 3304 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10573 "bison-chpl-lib.cpp"
    break;

  case 611: /* expr: expr TCOLON expr  */
#line 3308 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10581 "bison-chpl-lib.cpp"
    break;

  case 612: /* expr: expr TDOTDOT expr  */
#line 3312 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10590 "bison-chpl-lib.cpp"
    break;

  case 613: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 3317 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10599 "bison-chpl-lib.cpp"
    break;

  case 614: /* expr: expr TDOTDOT  */
#line 3322 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10608 "bison-chpl-lib.cpp"
    break;

  case 615: /* expr: TDOTDOT expr  */
#line 3327 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10618 "bison-chpl-lib.cpp"
    break;

  case 616: /* expr: TDOTDOTOPENHIGH expr  */
#line 3333 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 617: /* expr: TDOTDOT  */
#line 3339 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10638 "bison-chpl-lib.cpp"
    break;

  case 618: /* opt_expr: %empty  */
#line 3370 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10644 "bison-chpl-lib.cpp"
    break;

  case 619: /* opt_expr: expr  */
#line 3371 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10650 "bison-chpl-lib.cpp"
    break;

  case 620: /* opt_try_expr: TTRY expr  */
#line 3375 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10656 "bison-chpl-lib.cpp"
    break;

  case 621: /* opt_try_expr: TTRYBANG expr  */
#line 3376 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10662 "bison-chpl-lib.cpp"
    break;

  case 622: /* opt_try_expr: expr  */
#line 3377 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10668 "bison-chpl-lib.cpp"
    break;

  case 628: /* call_base_expr: expr TBANG  */
#line 3394 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10674 "bison-chpl-lib.cpp"
    break;

  case 629: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3395 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10680 "bison-chpl-lib.cpp"
    break;

  case 632: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3402 "chpl.ypp"
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
#line 10696 "bison-chpl-lib.cpp"
    break;

  case 633: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3414 "chpl.ypp"
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
#line 10712 "bison-chpl-lib.cpp"
    break;

  case 634: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3426 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10720 "bison-chpl-lib.cpp"
    break;

  case 635: /* dot_expr: expr TDOT ident_use  */
#line 3433 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10726 "bison-chpl-lib.cpp"
    break;

  case 636: /* dot_expr: expr TDOT TTYPE  */
#line 3435 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10732 "bison-chpl-lib.cpp"
    break;

  case 637: /* dot_expr: expr TDOT TDOMAIN  */
#line 3437 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10738 "bison-chpl-lib.cpp"
    break;

  case 638: /* dot_expr: expr TDOT TLOCALE  */
#line 3439 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10744 "bison-chpl-lib.cpp"
    break;

  case 639: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3441 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10754 "bison-chpl-lib.cpp"
    break;

  case 640: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3447 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10764 "bison-chpl-lib.cpp"
    break;

  case 641: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3459 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10770 "bison-chpl-lib.cpp"
    break;

  case 642: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3461 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10778 "bison-chpl-lib.cpp"
    break;

  case 643: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3465 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10786 "bison-chpl-lib.cpp"
    break;

  case 644: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3469 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10794 "bison-chpl-lib.cpp"
    break;

  case 645: /* bool_literal: TFALSE  */
#line 3475 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10800 "bison-chpl-lib.cpp"
    break;

  case 646: /* bool_literal: TTRUE  */
#line 3476 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10806 "bison-chpl-lib.cpp"
    break;

  case 647: /* str_bytes_literal: STRINGLITERAL  */
#line 3480 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10812 "bison-chpl-lib.cpp"
    break;

  case 648: /* str_bytes_literal: BYTESLITERAL  */
#line 3481 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10818 "bison-chpl-lib.cpp"
    break;

  case 651: /* literal_expr: INTLITERAL  */
#line 3487 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10824 "bison-chpl-lib.cpp"
    break;

  case 652: /* literal_expr: REALLITERAL  */
#line 3488 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10830 "bison-chpl-lib.cpp"
    break;

  case 653: /* literal_expr: IMAGLITERAL  */
#line 3489 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10836 "bison-chpl-lib.cpp"
    break;

  case 654: /* literal_expr: CSTRINGLITERAL  */
#line 3490 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10842 "bison-chpl-lib.cpp"
    break;

  case 655: /* literal_expr: TNONE  */
#line 3491 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10848 "bison-chpl-lib.cpp"
    break;

  case 656: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3493 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10856 "bison-chpl-lib.cpp"
    break;

  case 657: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3497 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10864 "bison-chpl-lib.cpp"
    break;

  case 658: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3501 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10872 "bison-chpl-lib.cpp"
    break;

  case 659: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3505 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10880 "bison-chpl-lib.cpp"
    break;

  case 660: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3509 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10888 "bison-chpl-lib.cpp"
    break;

  case 661: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3513 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10897 "bison-chpl-lib.cpp"
    break;

  case 662: /* assoc_expr_ls: expr TALIAS expr  */
#line 3522 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10906 "bison-chpl-lib.cpp"
    break;

  case 663: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3527 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10916 "bison-chpl-lib.cpp"
    break;

  case 664: /* binary_op_expr: expr TPLUS expr  */
#line 3535 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10922 "bison-chpl-lib.cpp"
    break;

  case 665: /* binary_op_expr: expr TMINUS expr  */
#line 3536 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10928 "bison-chpl-lib.cpp"
    break;

  case 666: /* binary_op_expr: expr TSTAR expr  */
#line 3537 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10934 "bison-chpl-lib.cpp"
    break;

  case 667: /* binary_op_expr: expr TDIVIDE expr  */
#line 3538 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10940 "bison-chpl-lib.cpp"
    break;

  case 668: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3539 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10946 "bison-chpl-lib.cpp"
    break;

  case 669: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3540 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10952 "bison-chpl-lib.cpp"
    break;

  case 670: /* binary_op_expr: expr TMOD expr  */
#line 3541 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10958 "bison-chpl-lib.cpp"
    break;

  case 671: /* binary_op_expr: expr TEQUAL expr  */
#line 3542 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10964 "bison-chpl-lib.cpp"
    break;

  case 672: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3543 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10970 "bison-chpl-lib.cpp"
    break;

  case 673: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3544 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10976 "bison-chpl-lib.cpp"
    break;

  case 674: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3545 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10982 "bison-chpl-lib.cpp"
    break;

  case 675: /* binary_op_expr: expr TLESS expr  */
#line 3546 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10988 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr TGREATER expr  */
#line 3547 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10994 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TBAND expr  */
#line 3548 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11000 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TBOR expr  */
#line 3549 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11006 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr TBXOR expr  */
#line 3550 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11012 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TAND expr  */
#line 3551 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11018 "bison-chpl-lib.cpp"
    break;

  case 681: /* binary_op_expr: expr TOR expr  */
#line 3552 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11024 "bison-chpl-lib.cpp"
    break;

  case 682: /* binary_op_expr: expr TEXP expr  */
#line 3553 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11030 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TBY expr  */
#line 3554 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11036 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TALIGN expr  */
#line 3555 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11042 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr THASH expr  */
#line 3556 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11048 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TDMAPPED expr  */
#line 3557 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11054 "bison-chpl-lib.cpp"
    break;

  case 687: /* unary_op_expr: TPLUS expr  */
#line 3561 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11060 "bison-chpl-lib.cpp"
    break;

  case 688: /* unary_op_expr: TMINUS expr  */
#line 3562 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11066 "bison-chpl-lib.cpp"
    break;

  case 689: /* unary_op_expr: TMINUSMINUS expr  */
#line 3563 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 11072 "bison-chpl-lib.cpp"
    break;

  case 690: /* unary_op_expr: TPLUSPLUS expr  */
#line 3564 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 11078 "bison-chpl-lib.cpp"
    break;

  case 691: /* unary_op_expr: TBANG expr  */
#line 3565 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11084 "bison-chpl-lib.cpp"
    break;

  case 692: /* unary_op_expr: expr TBANG  */
#line 3566 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11092 "bison-chpl-lib.cpp"
    break;

  case 693: /* unary_op_expr: TBNOT expr  */
#line 3569 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11098 "bison-chpl-lib.cpp"
    break;

  case 694: /* reduce_expr: expr TREDUCE expr  */
#line 3574 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11106 "bison-chpl-lib.cpp"
    break;

  case 695: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3578 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11114 "bison-chpl-lib.cpp"
    break;

  case 696: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3582 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11122 "bison-chpl-lib.cpp"
    break;

  case 697: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3586 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11130 "bison-chpl-lib.cpp"
    break;

  case 698: /* scan_expr: expr TSCAN expr  */
#line 3593 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11138 "bison-chpl-lib.cpp"
    break;

  case 699: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3597 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11146 "bison-chpl-lib.cpp"
    break;

  case 700: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3601 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11154 "bison-chpl-lib.cpp"
    break;

  case 701: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3605 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11162 "bison-chpl-lib.cpp"
    break;


#line 11166 "bison-chpl-lib.cpp"

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
