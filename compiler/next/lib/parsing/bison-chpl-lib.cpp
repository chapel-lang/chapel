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
  YYSYMBOL_tryable_stmt = 186,             /* tryable_stmt  */
  YYSYMBOL_deprecated_decl_stmt = 187,     /* deprecated_decl_stmt  */
  YYSYMBOL_188_1 = 188,                    /* $@1  */
  YYSYMBOL_189_2 = 189,                    /* $@2  */
  YYSYMBOL_deprecated_decl_base = 190,     /* deprecated_decl_base  */
  YYSYMBOL_module_decl_start = 191,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 192,         /* module_decl_stmt  */
  YYSYMBOL_access_control = 193,           /* access_control  */
  YYSYMBOL_opt_prototype = 194,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 195,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 196,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 197,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 198,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 199,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 200,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 201,              /* opt_only_ls  */
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
  YYSYMBOL_op_ident = 273,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 274,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 275,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 276,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 277,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 278,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 279,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 280,                /* formal_ls  */
  YYSYMBOL_formal = 281,                   /* formal  */
  YYSYMBOL_opt_formal_intent_tag = 282,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 283,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 284,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 285,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 286,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 287,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 288,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 289,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 290,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 291,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 292, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 293,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 294,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 295,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 296, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 297, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 298, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 299,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 300,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 301,            /* var_decl_stmt  */
  YYSYMBOL_302_12 = 302,                   /* $@12  */
  YYSYMBOL_var_decl_stmt_inner_ls = 303,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 304,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 305, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 306, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 307,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 308,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 309,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 310,                 /* opt_type  */
  YYSYMBOL_array_type = 311,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 312, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 313,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 314,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 315,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 316,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 317,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 318,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 319,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 320,                /* actual_ls  */
  YYSYMBOL_actual_expr = 321,              /* actual_expr  */
  YYSYMBOL_ident_expr = 322,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 323,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 324,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 325,                 /* for_expr  */
  YYSYMBOL_cond_expr = 326,                /* cond_expr  */
  YYSYMBOL_nil_expr = 327,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 328,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 329,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 330,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 331,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 332,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 333,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 334,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 335,          /* task_var_prefix  */
  YYSYMBOL_io_expr = 336,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 337,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 338,                 /* new_expr  */
  YYSYMBOL_let_expr = 339,                 /* let_expr  */
  YYSYMBOL_expr = 340,                     /* expr  */
  YYSYMBOL_opt_expr = 341,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 342,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 343,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 344,           /* call_base_expr  */
  YYSYMBOL_call_expr = 345,                /* call_expr  */
  YYSYMBOL_dot_expr = 346,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 347,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 348,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 349,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 350,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 351,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 352,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 353,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 354,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 355,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 356       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 362 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include other helper functions for the parser
  #include "parser-help.h"


#line 479 "bison-chpl-lib.cpp"

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
#define YYLAST   20137

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  180
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  177
/* YYNRULES -- Number of rules.  */
#define YYNRULES  711
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1260

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
     693,   694,   695,   696,   705,   714,   737,   744,   751,   752,
     753,   754,   755,   756,   757,   771,   786,   794,   806,   817,
     829,   841,   852,   868,   870,   869,   878,   877,   888,   889,
     893,   913,   928,   943,   959,   960,   962,   967,   968,   973,
     991,   995,   999,  1010,  1029,  1030,  1034,  1038,  1042,  1047,
    1051,  1055,  1065,  1070,  1076,  1083,  1088,  1095,  1106,  1107,
    1111,  1115,  1122,  1130,  1139,  1146,  1154,  1166,  1173,  1177,
    1184,  1190,  1199,  1200,  1204,  1213,  1217,  1221,  1225,  1229,
    1233,  1242,  1243,  1247,  1248,  1249,  1250,  1251,  1252,  1255,
    1256,  1257,  1258,  1259,  1260,  1272,  1273,  1284,  1285,  1286,
    1287,  1288,  1289,  1290,  1291,  1292,  1293,  1294,  1295,  1296,
    1297,  1298,  1299,  1300,  1301,  1302,  1306,  1307,  1308,  1309,
    1310,  1311,  1312,  1313,  1314,  1315,  1316,  1317,  1324,  1325,
    1326,  1327,  1331,  1332,  1336,  1343,  1353,  1355,  1354,  1363,
    1362,  1373,  1377,  1383,  1383,  1389,  1389,  1398,  1399,  1400,
    1401,  1402,  1403,  1404,  1408,  1413,  1418,  1423,  1430,  1438,
    1439,  1443,  1452,  1458,  1466,  1486,  1485,  1498,  1505,  1523,
    1536,  1549,  1553,  1557,  1561,  1565,  1569,  1573,  1577,  1595,
    1599,  1603,  1607,  1611,  1615,  1619,  1623,  1627,  1631,  1635,
    1639,  1643,  1647,  1651,  1655,  1659,  1663,  1667,  1671,  1675,
    1679,  1683,  1688,  1696,  1705,  1709,  1713,  1717,  1721,  1725,
    1729,  1733,  1737,  1742,  1747,  1752,  1760,  1775,  1793,  1795,
    1800,  1801,  1806,  1811,  1812,  1813,  1814,  1815,  1816,  1817,
    1818,  1819,  1820,  1821,  1822,  1823,  1835,  1836,  1837,  1838,
    1847,  1848,  1852,  1854,  1856,  1861,  1863,  1865,  1870,  1874,
    1878,  1882,  1889,  1890,  1894,  1898,  1902,  1909,  1922,  1938,
    1946,  1950,  1959,  1960,  1964,  1968,  1973,  1983,  1988,  1999,
    2006,  2007,  2008,  2012,  2013,  2017,  2021,  2025,  2032,  2049,
    2062,  2069,  2074,  2081,  2080,  2093,  2095,  2094,  2103,  2102,
    2113,  2120,  2132,  2140,  2144,  2145,  2147,  2152,  2162,  2159,
    2187,  2195,  2203,  2214,  2225,  2234,  2247,  2248,  2252,  2253,
    2254,  2263,  2264,  2265,  2266,  2267,  2268,  2269,  2270,  2271,
    2272,  2273,  2274,  2275,  2276,  2277,  2278,  2279,  2280,  2281,
    2282,  2283,  2284,  2285,  2286,  2287,  2288,  2292,  2293,  2294,
    2295,  2296,  2297,  2298,  2299,  2300,  2301,  2302,  2303,  2308,
    2309,  2313,  2314,  2315,  2319,  2320,  2324,  2328,  2329,  2333,
    2334,  2338,  2347,  2358,  2368,  2380,  2392,  2400,  2404,  2412,
    2413,  2414,  2415,  2416,  2417,  2418,  2419,  2420,  2424,  2425,
    2426,  2427,  2428,  2429,  2433,  2434,  2435,  2439,  2440,  2441,
    2442,  2443,  2444,  2448,  2449,  2452,  2453,  2457,  2458,  2462,
    2467,  2468,  2470,  2472,  2474,  2479,  2481,  2486,  2488,  2490,
    2492,  2494,  2496,  2498,  2503,  2504,  2508,  2516,  2520,  2525,
    2533,  2537,  2544,  2565,  2566,  2568,  2576,  2577,  2578,  2579,
    2580,  2585,  2584,  2593,  2601,  2605,  2612,  2628,  2645,  2649,
    2653,  2660,  2662,  2664,  2671,  2672,  2673,  2678,  2682,  2686,
    2690,  2694,  2698,  2702,  2709,  2710,  2711,  2712,  2713,  2718,
    2719,  2720,  2721,  2722,  2742,  2746,  2750,  2754,  2761,  2762,
    2763,  2767,  2772,  2780,  2785,  2789,  2796,  2797,  2798,  2799,
    2800,  2806,  2807,  2808,  2809,  2813,  2814,  2818,  2819,  2820,
    2824,  2828,  2835,  2836,  2840,  2845,  2854,  2855,  2856,  2857,
    2861,  2862,  2873,  2875,  2877,  2883,  2884,  2885,  2886,  2887,
    2888,  2890,  2892,  2894,  2896,  2902,  2904,  2907,  2909,  2911,
    2913,  2915,  2917,  2919,  2921,  2924,  2926,  2931,  2940,  2949,
    2957,  2971,  2985,  2999,  3008,  3017,  3025,  3039,  3053,  3067,
    3084,  3093,  3102,  3117,  3135,  3153,  3161,  3162,  3163,  3164,
    3165,  3166,  3167,  3168,  3173,  3174,  3178,  3187,  3188,  3192,
    3201,  3202,  3206,  3221,  3225,  3232,  3233,  3234,  3235,  3236,
    3237,  3241,  3242,  3247,  3249,  3251,  3253,  3255,  3261,  3278,
    3282,  3286,  3290,  3297,  3305,  3306,  3307,  3308,  3309,  3310,
    3311,  3312,  3313,  3314,  3318,  3322,  3327,  3332,  3337,  3343,
    3349,  3381,  3382,  3386,  3387,  3388,  3392,  3393,  3394,  3395,
    3404,  3405,  3406,  3407,  3408,  3412,  3424,  3436,  3443,  3445,
    3447,  3449,  3451,  3457,  3470,  3471,  3475,  3479,  3486,  3487,
    3491,  3492,  3496,  3497,  3498,  3499,  3500,  3501,  3502,  3503,
    3507,  3511,  3515,  3519,  3523,  3532,  3537,  3546,  3547,  3548,
    3549,  3550,  3551,  3552,  3553,  3554,  3555,  3556,  3557,  3558,
    3559,  3560,  3561,  3562,  3563,  3564,  3565,  3566,  3567,  3568,
    3572,  3573,  3574,  3575,  3576,  3577,  3580,  3584,  3588,  3592,
    3596,  3603,  3607,  3611,  3615,  3623,  3624,  3625,  3626,  3627,
    3628,  3629
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
  "toplevel_stmt", "pragma_ls", "stmt", "tryable_stmt",
  "deprecated_decl_stmt", "$@1", "$@2", "deprecated_decl_base",
  "module_decl_start", "module_decl_stmt", "access_control",
  "opt_prototype", "include_module_stmt", "block_stmt_body", "block_stmt",
  "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls", "use_stmt",
  "import_stmt", "import_expr", "import_ls", "require_stmt",
  "assignment_stmt", "opt_label_ident", "ident_fn_def", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt",
  "deprecated_class_level_stmt", "$@3", "$@4", "class_level_stmt", "$@5",
  "$@6", "inner_class_level_stmt", "forwarding_decl_stmt",
  "forwarding_decl_start", "extern_or_export",
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
  "fn_ident", "op_ident", "assignop_ident", "all_op_name",
  "formal_var_arg_expr", "opt_formal_ls", "req_formal_ls",
  "formal_ls_inner", "formal_ls", "formal", "opt_formal_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_iter_or_op",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_start",
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

#define YYPACT_NINF (-1122)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-712)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1122,   103,  3574, -1122,   -30,   100, -1122, -1122, -1122, -1122,
   -1122, -1122,  4966,   130,   285,   256, 14735,   262, 19902,   130,
   11892,   269,   302,   344,   285,  4966, 11892,   428,  4966,   275,
   19936, -1122,    55,   425,  8780, 10164, 10164, -1122,  8952,   436,
     201,   335, -1122,   454, 19936, 19936, 19936,   367,  1800, 10336,
     459, 11892,   340, -1122,   471,   492, 11892, -1122, 14735, -1122,
   11892,   571,   415,   421,   423,   541, 20022, -1122, 10510,  8262,
   11892, 10336, 14735, 11892,   503,   554,   474,  4966,   569, 11892,
     586,  7224,  7224, -1122,   604, -1122, 14735, -1122,   632,  8952,
   11892, -1122, 11892, -1122, 11892, -1122, -1122, 14255, 11892, -1122,
   11892, -1122, -1122, -1122,  3922,  7398,  9126, 11892, -1122,  4792,
   -1122, -1122, -1122, -1122,   522, -1122,   249, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
    7570, -1122, 10682, -1122, -1122, -1122, -1122, -1122,   647, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122,   562, 19936, -1122,  3209,
     388, -1122,   180, -1122, -1122,   268,   424, -1122, 19936,  1800,
   -1122,   552, -1122,   557, -1122, -1122,   553,   582,   580, 11892,
     589,   594, 19709,  1976,   193,   596,   597, -1122, -1122,   521,
   -1122, -1122, -1122, -1122, -1122,   185, -1122, -1122, 19709,   568,
    4966, -1122, -1122,   601, 11892, -1122, -1122, 11892, 11892, 11892,
   19936, -1122, 11892, 10510, 10510,   713,   527, -1122, -1122, -1122,
   -1122,   -14,   528, -1122, -1122,   605, 16427, -1122,   238, -1122,
     609, -1122,   -28, 19709, -1122,  2888,   657,  8436, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122,   608, -1122, -1122, -1122, 19936,   611,    37, 16014,
      31,  3204,    31, 15932, 19936, 19936,   -22, 15185, -1122, -1122,
     695,  8436,   613,   533,  4966,   265,   268,  1332,    75,    65,
   -1122,  4966, -1122, -1122, 16508,   497, -1122,   616,   617, -1122,
   16508,   -14,   497, -1122,  8436,  2581,  2581, -1122, -1122,    96,
   19709, 11892, 11892, -1122, 19709,   626, 17082, -1122, 16508,   -14,
   19709,   620,  8436, -1122, 19709, 17165, -1122, -1122, -1122, -1122,
     -14,    37, 16508, 17247,   140,   140,  1161,   497,   497,   205,
   -1122, -1122,  4096,   -35, -1122, 11892, -1122,   -23,   -18, -1122,
      -8,     7, 17328,   -21,  1161,   772, -1122,  4270, 11892, -1122,
   11892,   728, -1122, -1122, 16097,   228,   562, 19709,   679, 19936,
   10510,   625, -1122,   627,   787,   672,    99, -1122, -1122, -1122,
   -1122, -1122, -1122,   709, -1122, -1122, -1122,    67,   710, -1122,
   -1122, -1122, 14088,   676,   637,   658,   330,   574, -1122, 11892,
     655, 11892, 11892, 11892, 10164, 10164, 11892,   577, 11892, 11892,
   11892, 11892, 11892,   242, 14255, 11892, 11892, 11892, 11892, 11892,
   11892, 11892, 11892, 11892, 11892, 11892, 11892, 11892, 11892, 11892,
   11892,   736, -1122, -1122, -1122, -1122, -1122,  9298,  9298, -1122,
   -1122, -1122, -1122,  9298, -1122, -1122,  9298,  9298,  8436,  8436,
   10164, 10164,  8090, -1122, -1122, 16590, 16671, 17409,   634,    14,
   19936,  4444, -1122, 10164,    37,  1800, -1122, 11892, -1122,  2888,
   -1122, 19936,   701, -1122, -1122,   695, 11892,   687, -1122,   640,
     668, -1122, -1122, -1122,   771, 10510, -1122,  5140, 10164, -1122,
     650, -1122,    37,  5314, 10164, -1122,    37, -1122,    37, 10164,
   -1122,    37,   699,   700,  4966,   792,  4966, -1122,   793, 11892,
     764,   661,  8436, 19936, -1122, -1122,    54, -1122, -1122, -1122,
   -1122, -1122, -1122,   546,   690,   666, -1122,  2180, -1122,    53,
   -1122,  1332, -1122, -1122,   113, -1122, 12064,   718, 11892,  1800,
   -1122, -1122, 11892, 11892, -1122,   673, -1122, -1122, 10510, -1122,
   19709, 19709, -1122,    32, -1122,  8436,   675, -1122,   830,   830,
   -1122, -1122, -1122, -1122, -1122, -1122,  9472, -1122, 17490,  7744,
   -1122,  7918, -1122,  4966,   678, 10164,  9646,  3748,   684, 11892,
   10854, -1122, -1122,   420, -1122,  4618, -1122,   405, 17571,   418,
   16180, 19936,  7052,  7052, -1122,   562,   691,   238, -1122,   -52,
     714,  1984, -1122, -1122, 19936, 11892,   202, -1122, -1122, -1122,
   12236,   763, -1122,   698,   354, -1122,   721, -1122,   723,   724,
     733,   726,   729, -1122,   730,   739,   732,   734,   735,   370,
     741,   737,   738, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122, 11892, -1122,   744,   747,
     740,   698, -1122,   698, -1122, 12408, -1122, -1122, 19936, -1122,
   17652, 16468, 16468,   216, -1122,   216, -1122,   216, 14867,   163,
     697,   530,   -14,   140, -1122,   578, -1122, -1122, -1122, -1122,
   -1122,  1161, 19080,   216,  3250,  3250, 16468,  3250,  3250,  1131,
     140, 19080, 19027,  1131,   497,   497,   140,  1161,   711,   742,
     743,   745,   749,   750,   720,   727, -1122,   216, -1122,   216,
      57, -1122, -1122, -1122,   148, -1122,  1517, 19750,   525, 12580,
   10164, 12752, 10164, 11892,  8436, 10164,  3422,   751,   130, 17733,
   -1122,   427, 19709, -1122, 17814,  8436, -1122,  8436, 11892,   181,
    8952, 19709,    39, 16752,  8090, -1122,  8952, 19709,    36, 16262,
   -1122, -1122,    31, 16345, -1122, 11892, 11892,   865,  4966,   868,
   17895,  4966, 16834, 19936, -1122,   189, -1122,   196, -1122, -1122,
   -1122, 19936,   867, -1122,  1332,   760,    23,   295,  1332,    75,
      43,    45, 11892, 11892,  6880, -1122, -1122,   187,  8608, -1122,
   19709, -1122, 17976, 18057, -1122, -1122, 19709,   746,    13,   752,
   -1122,  2621, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,
    4966,   106, 16917, -1122, -1122, 19709,  4966, 19709, -1122, 18138,
   -1122, -1122, 11892, -1122,    60, 14945, 11892, -1122, 11026,  7052,
    7052, -1122, 11892,   575, 11892,   590,   591, 11892,  9818,   598,
     448, -1122, -1122, -1122, 18219,   766,   754,   756, -1122,  1800,
   -1122,  8436,   757,  2354, 19936, -1122, 19709, 19521,  6706, -1122,
   -1122,   274, -1122,    23,   265, -1122, 18300, -1122, 15103, -1122,
   -1122, -1122,   455, -1122,   758,   761, -1122, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122,  8090, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122, -1122,    49,
   10164, 10164, 11892,   888, 18381, 11892,   893, 18462,   297,   762,
   18543,  8436,    37,    37, -1122, -1122, -1122, -1122, -1122, 16508,
   -1122, 15268,  5488, -1122,  5662, -1122,   317, -1122, 15351,  5836,
   -1122,    37,  6010, -1122,    37, -1122,    37, -1122,    37, 19709,
   19709,  4966, -1122,  4966, 11892, -1122,  4966,   896,   773,   774,
   19936,   605,   760, -1122,   353, 11200,   290,    40, 11892,     5,
   -1122, -1122,   718,   765,    56, -1122, -1122,   770,   777, -1122,
    6184, 10510, -1122, -1122, -1122, 19936, -1122,   784,   605, -1122,
    6358,   769,  6532,   775, -1122, 11892, -1122, -1122,  7052, -1122,
   18624,    95, 17000,   469,   780,  7052, -1122, 11892, -1122, -1122,
    2860,   486,   359, -1122,   942, -1122, -1122, -1122,  1624, -1122,
   -1122, -1122, -1122,   778, 13956,    59, -1122,   781, -1122,   815,
     816,   698,   698, -1122, -1122, -1122,   718,   379,   394, 18706,
   12924, 13096, 18787, 13268, 13440, -1122, 13612, 13784,   395, -1122,
   -1122, -1122,  4966,  8952, 19709,  8952, 19709,  8090, -1122,  4966,
    8952, 19709, -1122,  8952, 19709, -1122, -1122, -1122, -1122, -1122,
   19709,   922,  4966, -1122, -1122, -1122, -1122,   290,   760,  9992,
   -1122, -1122, -1122,   331, 10510, -1122, -1122, -1122,    78, -1122,
     123, -1122,   114, 18881, -1122, -1122, -1122, -1122, -1122, 10164,
   14875,  8436,  8436,  4966, -1122,    20,   789, 11892, -1122,  8952,
   -1122, 19709,  4966,  8952, -1122, 19709,  4966, 19709,   308, 11372,
    7052,  7052,  7052,  7052, -1122, -1122, -1122, 18938, 19709,  3029,
   -1122, -1122,   798, -1122, 19311, -1122, -1122, -1122,   439, 14424,
     155, -1122, -1122, -1122, -1122, 11892, 11892, 11892, 11892, 11892,
   11892, 11892, 11892, -1122, 17895, 15434, 15517, -1122, 17895, 15600,
   15683,  4966, -1122, -1122, -1122,   290, 11546,    82, -1122, 19709,
   -1122, 11892,    40,    78,    78,    78,    78,    78,    78,    40,
   18978, -1122,   451,   791,   795,   657, -1122,   605, 19709, 15766,
   -1122, 15849, -1122, -1122, -1122, 19709,   502,   800,   504,   801,
   11892, -1122, -1122, 19311, -1122, -1122,   555, -1122, -1122, -1122,
   19129, 19169, 19320, 19360, 19511, 19551,  4966,  4966,  4966,  4966,
   -1122, -1122, -1122, -1122, -1122, -1122,   373, 10164, 14564, 19709,
   -1122, -1122, -1122, -1122, -1122, -1122, -1122,   824, 13784,   808,
     809, -1122,  4966,  4966, -1122, -1122, -1122, -1122, 19709, -1122,
   11892, 11892, 11892, 11892, 11892, 11892, 17895, 17895, 17895, 17895,
   19592, -1122, -1122,   614, -1122, -1122, 17895, 17895, 11720, -1122
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   115,   654,   655,   656,   650,
     651,   657,     0,   574,   101,   136,   543,   143,   545,   574,
       0,   142,   451,   449,   101,     0,     0,    46,     0,   260,
     137,   180,   179,   648,     0,     0,     0,   178,     0,   141,
      54,   261,   315,   138,     0,     0,     0,     0,     0,     0,
     145,     0,   593,   565,   658,   146,     0,   316,   537,   446,
       0,     0,     0,   165,   163,   140,   546,   448,     0,     0,
       0,     0,   541,     0,     0,   144,     0,     0,   116,     0,
     649,     0,     0,   437,   139,   292,   539,   450,   147,     0,
       0,   707,     0,   709,     0,   710,   711,   620,     0,   708,
     705,   524,   161,   706,     0,     0,     0,     0,     4,     0,
       5,     9,    10,    43,     0,    48,    57,    11,    63,    12,
      13,    14,    15,    28,   520,   521,    21,    49,   162,   172,
       0,   181,   621,   173,    16,    30,    29,    18,     0,   255,
      17,   612,    20,    33,    31,   171,   293,     0,   169,     0,
     609,   314,     0,   167,   317,   407,   398,   170,     0,     0,
     168,   626,   605,   522,   606,   527,   525,     0,     0,     0,
     610,   611,     0,   526,     0,   627,   628,   629,   652,   653,
     604,   529,   528,   607,   608,     0,    27,    22,   535,     0,
       0,   575,   102,     0,     0,   545,   137,     0,     0,     0,
       0,   546,     0,     0,     0,     0,   609,   626,   525,   610,
     611,   544,   526,   627,   628,     0,   574,   438,     0,   447,
       0,    19,     0,   505,    44,   313,     0,   512,   109,   117,
     129,   123,   122,   131,   112,   121,   132,   118,   133,   110,
     134,   127,   120,   128,   126,   124,   125,   111,   113,   119,
     130,   135,     0,   114,   187,   439,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    55,
      57,   512,     0,     0,     0,   387,   407,     0,     0,     0,
     454,     0,   153,    38,     0,   692,   597,   594,   595,   596,
       0,   538,   693,     7,   512,   313,   313,   419,   502,     0,
     501,     0,     0,   154,   625,     0,     0,    41,     0,   542,
     530,     0,   512,    42,   536,     0,   268,   272,   269,   272,
     540,     0,     0,     0,   694,   696,   618,   691,   690,     0,
      60,    64,     0,     0,   507,     0,   509,     0,     0,   508,
       0,     0,   501,     0,   619,     0,     6,     0,     0,    58,
       0,     0,   451,   177,     0,   650,   293,   622,   185,     0,
       0,     0,   289,     0,   308,   310,     0,   301,   305,   405,
     406,   404,   325,   408,   411,   410,   412,     0,   402,   399,
     400,   403,     0,   443,     0,   440,     0,   523,    32,     0,
     598,     0,     0,     0,     0,     0,     0,   695,     0,     0,
       0,     0,     0,     0,   617,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   357,   364,   365,   366,   361,   363,     0,     0,   359,
     362,   360,   358,     0,   368,   367,     0,     0,   512,   512,
       0,     0,     0,    34,    23,     0,     0,     0,     0,     0,
       0,     0,    35,     0,     0,     0,    24,     0,    36,   313,
     290,     0,   179,   291,    47,    57,     0,   520,   518,     0,
     513,   514,   519,   300,     0,     0,   197,     0,     0,   196,
       0,   205,     0,     0,     0,   203,     0,   213,     0,     0,
     211,     0,     0,     0,     0,   229,     0,   357,   225,     0,
       0,     0,     0,     0,   239,    25,   394,   389,   390,   391,
     395,   396,   397,   387,   380,     0,   377,     0,   388,     0,
     458,     0,   459,   461,     0,   483,     0,   464,     0,     0,
     152,    37,     0,     0,    39,     0,   166,   164,     0,    94,
     623,   624,   155,     0,    40,     0,     0,   279,   270,   271,
     190,   189,    26,    62,    61,    65,     0,   659,     0,     0,
     644,     0,   646,     0,     0,     0,     0,     0,     0,     0,
       0,   663,     8,     0,    51,     0,    92,     0,    88,     0,
      72,     0,     0,    78,   174,   293,     0,     0,   184,   266,
     294,     0,   299,   306,     0,     0,   303,   298,   409,   478,
       0,   413,   401,   374,   103,   352,   117,   350,   123,   122,
     106,   121,   118,   355,   133,   104,   134,   120,   124,   105,
     107,   119,   135,   349,   331,   334,   332,   333,   356,   344,
     335,   348,   340,   338,   351,   354,   339,   337,   342,   347,
     336,   341,   345,   346,   343,   353,     0,   328,     0,   108,
       0,   374,   329,   374,   326,     0,   442,   436,     0,   453,
     573,   687,   686,   689,   698,   697,   702,   701,   683,   680,
     681,   682,   614,   670,   115,     0,   640,   641,   116,   639,
     638,   615,   674,   685,   679,   677,   688,   678,   676,   668,
     673,   675,   684,   667,   671,   672,   669,   616,     0,     0,
       0,     0,     0,     0,     0,     0,   700,   699,   704,   703,
     585,   586,   588,   590,     0,   577,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   661,   266,   574,   574,
     193,     0,   506,    45,     0,     0,   532,     0,     0,     0,
       0,   549,     0,     0,     0,   206,     0,   555,     0,     0,
     204,   214,     0,     0,   212,     0,     0,   228,     0,   224,
       0,     0,     0,     0,   531,     0,   242,     0,   240,   392,
     393,     0,   387,   376,     0,   496,   420,     0,   462,     0,
     150,   151,   149,   148,     0,   482,   481,   605,     0,   456,
     603,   455,     0,     0,   637,   504,   503,     0,     0,     0,
     533,     0,   273,   660,   613,   645,   510,   647,   511,   221,
       0,     0,     0,   662,   219,   559,     0,   665,   664,     0,
      53,    52,     0,    87,     0,     0,     0,    80,     0,     0,
      78,    50,   349,   331,   334,   332,   333,   342,   341,   343,
       0,   369,   370,    67,    66,    79,     0,     0,   295,     0,
     263,     0,     0,   313,     0,   309,   311,     0,     0,   477,
     476,   605,   414,   420,   387,   324,     0,   330,     0,   320,
     321,   445,   605,   441,     0,     0,   100,    98,    99,    97,
      96,    95,   635,   636,   587,   589,     0,   576,   136,   143,
     142,   141,   138,   145,   146,   140,   144,   139,   147,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   452,   188,   516,   517,   515,     0,
     223,     0,     0,   195,     0,   194,     0,   580,     0,     0,
     201,     0,     0,   199,     0,   209,     0,   207,     0,   237,
     236,     0,   231,     0,     0,   227,     0,   233,     0,   265,
       0,     0,   496,   378,     0,     0,   464,     0,     0,     0,
     460,   463,   464,     0,     0,   465,   466,     0,     0,   281,
       0,     0,   280,   283,   534,     0,   274,   277,     0,   222,
       0,     0,     0,     0,   220,     0,    93,    90,     0,    89,
      75,    74,    73,     0,     0,     0,   175,     0,   176,   295,
     313,     0,     0,   288,   159,   165,   163,   287,   313,   296,
     156,   307,   304,     0,   468,     0,   318,     0,   327,   103,
     105,   374,   374,   642,   643,   578,   464,   626,   626,     0,
       0,     0,     0,     0,     0,   265,     0,     0,     0,   192,
     191,   198,     0,     0,   548,     0,   547,     0,   579,     0,
       0,   554,   202,     0,   553,   200,   210,   208,   230,   226,
     564,   232,     0,    59,   262,   241,   238,   464,   496,     0,
     499,   498,   500,   605,   371,   383,   381,   434,     0,   435,
     422,   425,     0,   421,   417,   418,   312,   457,   487,     0,
     661,   512,   512,     0,   285,     0,     0,     0,   275,     0,
     217,   561,     0,     0,   215,   560,     0,   666,     0,     0,
       0,    78,     0,    78,    81,    84,    70,    69,    68,   313,
     182,   186,   267,   157,   313,   297,   473,   471,   605,   661,
       0,   375,   322,   323,   582,     0,     0,     0,     0,     0,
       0,     0,     0,   267,   552,     0,     0,   581,   558,     0,
       0,     0,   235,   384,   382,   464,   488,     0,   373,   372,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   485,   605,     0,     0,   286,   284,     0,   278,     0,
     218,     0,   216,    91,    77,    76,     0,     0,     0,     0,
       0,   183,   264,   313,   160,   472,   605,   415,   319,   416,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     234,   386,   385,   490,   491,   493,   605,     0,   661,   424,
     426,   427,   430,   431,   432,   428,   429,   423,     0,   599,
     600,   276,     0,     0,    83,    86,    82,    85,    71,   158,
       0,     0,     0,     0,     0,     0,   551,   550,   557,   556,
       0,   492,   494,   605,   601,   602,   563,   562,   488,   495
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1122, -1122, -1122,    -1,  -258,  2226,   341, -1122, -1122, -1122,
    -198, -1122, -1122,   -27,   704, -1122,  -918,  2273,   633,  -575,
   -1122,  -810, -1122, -1122,   159, -1122, -1122, -1122,   958, -1122,
    2636,  -196,  -377, -1122,  -585,  1025,  -917,   -24, -1122, -1122,
    -782, -1122, -1122,   356, -1122, -1122, -1122, -1122, -1122, -1122,
   -1122, -1122,   413, -1122,   898, -1122, -1122,    38,  2068, -1122,
   -1122, -1122, -1122,   670, -1122,    15, -1122, -1122, -1122, -1122,
   -1122,  -103, -1122,  -328,  -803, -1122, -1122, -1122, -1122,   135,
   -1122, -1122,  -573,   576, -1122, -1122, -1122,   635, -1122, -1122,
   -1122, -1122,   126,  -372,    30,  -778,  -994,  -602, -1122, -1122,
     131,   225,   485, -1122, -1122, -1122,   725, -1122, -1122,  -131,
      71,   137,  -167,  -158,  -707, -1122, -1122,   347, -1122, -1122,
    -202,   877, -1122,  -156,   479,   232,  -502,  -926,  -906, -1122,
    -481,  -646, -1121, -1073,  -831,   -66, -1122,   195, -1122,  -260,
    -477,  -513,   483,  -491, -1122, -1122, -1122,   659, -1122,    -7,
   -1122, -1122,  -172, -1122,  -726, -1122, -1122, -1122,   984,  1378,
     -12, -1122,    46,  1567, -1122,  1892,  1927, -1122, -1122, -1122,
   -1122, -1122, -1122, -1122, -1122, -1122,  -436
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   331,   109,   759,   111,   112,   459,   225,
     113,   114,   115,   116,   351,   117,   118,   119,   332,   845,
     579,   846,   120,   121,   576,   577,   122,   123,   193,   647,
     278,   124,   253,   125,   785,   283,   126,  1009,  1193,  1124,
     127,   296,   295,   128,   129,   130,   131,   132,   133,   587,
     134,   135,   909,   136,   266,   137,   767,   768,   205,   139,
     140,   141,   142,   548,   802,   978,   143,   144,   798,   973,
     145,   146,   147,   361,   853,   148,   149,   366,   857,   367,
     854,   594,   368,   206,   151,   152,   153,   154,  1130,   155,
     156,   650,   651,   841,   842,   843,  1075,   865,   276,   514,
     515,   516,   517,   518,   382,   372,   377,   863,  1198,  1086,
     468,   959,  1080,  1081,  1082,   157,   158,   384,   385,   656,
     159,   160,   218,   279,   280,   523,   524,   789,   860,   601,
     527,   786,  1214,  1072,   956,   333,   222,   337,   338,   469,
     470,   471,   207,   162,   163,   164,   165,   208,   167,   190,
     191,   714,   482,   926,   715,   716,   168,   169,   209,   210,
     172,   358,   472,   212,   174,   213,   214,   177,   178,   179,
     180,   343,   181,   182,   183,   184,   185
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     188,   108,   299,   386,   211,   649,   718,   840,   216,   871,
     652,   501,   215,   270,   223,   859,   455,   513,   927,   777,
     994,   855,   259,   261,   263,   765,   267,   464,   586,   356,
    1076,   467,   799,   797,   535,   787,  1087,   284,   776,   285,
     341,  1084,  1085,  1077,   290,  1000,   291,   989,   292,   869,
     525,   870,   546,   281,   525,   565,   300,   304,   306,   308,
     309,   310,   725,   674,   281,   314,   254,   315,   599,   929,
     281,  1010,   922,  1153,   320,   467,   525,   322,   323,   494,
     324,  1077,   325,  1215,   957,   326,   327,   970,   328,   486,
     488,   491,   300,   304,   342,   344,    69,  1251,   467,  -260,
    1134,  -261,   769,     3,  1089,   884,  -282,   725,  1127,   861,
     556,   480,   850,   528,  -479,   305,   467,   457,   354,   851,
     357,  1067,   559,  -479,   570,   397,   480,   561,  -474,   971,
    1217,  1078,  1110,   403,   186,   557,   458,  1259,   449,   298,
     958,  1154,  1079,   770,  -479,  1252,   885,   451,  -282,   560,
     480,   339,   566,   571,   562,   480,  -243,   390,  -474,   566,
    1025,  1211,   678,   255,   872,   538,   563,  1111,   564,   568,
    -479,  -474,   393,  -479,   451,   298,   336,   298,   704,   705,
    1079,   567,   188,   972,  -474,   445,   446,   447,   726,   451,
     314,   300,   342,   526,  -479,   393,  1119,   526,   465,  -479,
     451,   566,  -282,   437,   566,   451,   451,   680,   451,   454,
     529,   600,  1084,  1085,   227,   304,   271,  1116,  1010,   526,
    -479,  -479,  -479,  1195,   918,   480,  1010,   566,   394,  1212,
    1090,  -474,   395,  1129,   369,  -480,  -474,  1155,  1163,  -479,
    1161,   538,   467,   467,   596,   674,    69,   908,  -480,   189,
     460,   394,   585,   370,   993,   395,  1218,   847,   778,   304,
     539,   733,   675,   371,  1164,    23,  1165,  1166,  1162,   597,
    1167,  1168,   954,   440,   298,   298,   676,   441,  -480,   397,
     980,  1011,   304,   268,   401,   779,   269,   403,   192,   540,
     541,   406,   506,   886,   590,   373,   348,   499,   962,   731,
     304,  1187,   397,  1189,  -480,   677,   467,   401,   718,   402,
     403,  -480,  -244,   507,   406,   463,    59,   508,  -250,  1197,
     887,  1147,   413,   558,   451,  -249,   538,    67,   417,   418,
     419,   555,  -480,   349,   737,  -475,   578,  1010,   580,    85,
     509,   950,  1194,   510,   678,    61,   374,  -302,   300,   467,
     679,  -480,    87,   920,   511,   397,  -480,   375,   286,  -480,
     401,   949,   350,   403,   438,  -475,   439,   406,   951,   186,
    1070,  1160,  -302,   512,  1002,   553,   376,   660,  -475,   661,
     662,   663,   665,   667,   668,   849,   669,   670,   671,   672,
     673,  -475,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,   691,   692,   693,   694,   695,   696,   697,   739,
     217,  1239,   653,  1108,   788,   304,   304,   287,  1026,  1132,
    1133,   304,   316,   318,   304,   304,   304,   304,   707,   709,
     717,   298,   465,   219,  1038,   288,   224,  -379,  -475,  1074,
     778,   729,   737,  -475,  1171,   732,   227,   258,   260,   262,
     718,   378,   289,   995,   734,  -497,  1221,  1222,  1223,  1224,
    1225,  1226,  1047,   300,  1073,   741,   743,   960,   -56,  1035,
     -55,   747,   749,   699,   700,   529,  -497,   753,  1183,   701,
    -497,  -258,   702,   703,   760,   161,   -56,   762,   -55,  1048,
     304,   649,  -248,  -109,   659,   161,   652,  -489,   778,  -484,
    -467,  -115,   379,  -497,   737,   -56,   271,   -55,   161,  -111,
    -245,   161,  -484,   380,   513,  -252,   790,  -116,  -489,   340,
     792,   793,  -489,  1128,  -584,  1068,   796,  -256,   467,   393,
    -467,  1122,   381,   304,   -56,  1186,   -55,  1188,   275,  -583,
     737,   467,  -484,  -467,   796,  -489,   298,   304,  -253,   304,
     822,  -584,  -570,   812,   796,   815,  -467,   817,   819,  -633,
     161,  -633,   393,   826,   161,   161,  -583,  1143,  -484,   823,
     844,   844,   529,   506,   555,  -484,   931,   934,   150,   293,
     936,   938,   827,   856,   186,   394,   294,   161,   150,   395,
     820,   914,   161,   995,   507,  1008,  -484,  -247,   508,  1172,
    -444,   150,   311,  -467,   150,   339,   513,   339,  -467,   795,
    -251,   718,   996,   901,   995,  -484,  -469,   441,   394,  -444,
    -484,   509,   395,  -484,   510,  -257,   345,   795,   987,   680,
     336,   529,   336,  1114,   866,   511,   397,   795,  1196,   981,
     983,   401,  -259,   402,   403,   312,  -469,   995,   406,   995,
    1121,   536,   537,   150,   512,   467,   413,   150,   150,  -469,
    -246,   166,  -486,  -709,   419,  1216,  1234,  -709,  1236,   397,
     398,   166,  -469,   161,   401,  -486,   402,   403,  -710,  -711,
     150,   406,  -710,  -711,   166,   150,  -706,   166,  -254,   413,
    -706,   347,  -634,  1001,  -634,   417,   418,   419,  -633,  -630,
    -633,  -630,   451,   359,   503,  -486,   360,   741,   904,   747,
     907,   760,   304,   910,   815,   467,  -567,  -566,   964,  -469,
     387,   912,   913,   304,  -469,   304,   919,  1216,   921,   393,
      42,  -486,   717,  -313,   928,   389,   166,  1253,  -486,   442,
     166,   166,  1008,   939,   940,  -632,   388,  -632,  -631,   874,
    -631,   875,  -313,  -571,   806,    57,   808,   161,  -572,  -486,
    -569,  -568,  -313,   166,   161,   444,   150,  1216,   166,   450,
     310,   314,   342,   456,   451,   466,   304,   473,  -486,   349,
     572,   917,   475,  -486,   502,   394,  -486,   532,   533,   395,
     542,   545,  1015,   581,   591,   593,   595,   592,   598,   602,
     655,   657,   403,   658,   698,   724,   916,   664,   666,   255,
     578,   735,   736,   737,   990,   161,   992,   844,   844,   738,
     324,   744,   325,   755,   756,   327,   328,   758,   761,   763,
     161,  1173,  1174,   764,   966,   772,   397,   398,   773,   304,
     400,   401,   788,   402,   403,   794,   342,   800,   406,   166,
     150,   801,   810,   706,   708,   298,   413,   150,   816,   538,
     848,  1008,   417,   418,   419,   654,   728,   862,  -136,   864,
    -143,  -142,  -112,  -141,   717,   876,  -138,  -145,  -110,  -146,
    -113,  -140,  -144,   867,  -139,  -147,  -114,   868,   665,   707,
    1029,   742,   882,  1032,   506,   467,   467,   748,  1022,   304,
     941,   883,   752,   943,   955,  1095,   877,   878,   150,   879,
    1044,   995,  1046,   880,   881,   507,   969,  1051,   998,   508,
    1054,  1030,   911,   150,   974,   999,  1033,  1003,  1097,   298,
    1023,  1062,  1060,   166,   161,  1024,  1036,  1063,  1064,  1088,
     166,  1091,   509,  1102,  1115,   510,  1083,    61,  1092,  1106,
    1123,   499,  1126,  1131,  -109,  -111,   511,  1151,   499,   300,
     161,  1177,  1192,  1229,  1235,  1237,   161,  1230,  1101,  1162,
    1105,  1254,  1255,  1107,   500,   512,   844,   161,   811,   161,
     575,   986,   220,  1117,  1125,  1118,   170,   321,  1065,   549,
    1096,   166,  1012,   588,  1021,  1017,   170,   953,   771,  1199,
    1016,   519,  1227,  1157,  1220,   873,   166,   353,   791,   170,
     961,     0,   170,     0,     0,     0,     0,     0,  1044,  1046,
       0,  1051,  1054,     0,  1101,  1105,  1071,   150,     0,     0,
    1144,  1145,     0,  1146,     0,   717,     0,  1148,  1149,     0,
       0,  1150,   298,     0,     0,     0,   161,     0,     0,     0,
     161,     0,     0,   150,     0,     0,     0,   342,   161,   150,
       0,   170,  1159,     0,     0,   170,   170,     0,     0,     0,
     150,     0,   150,     0,     0,     0,     0,  1170,   815,   304,
     304,     0,     0,     0,     0,  1178,     0,  1179,   170,     0,
       0,  1181,     0,   170,     0,     0,   307,  1185,   844,   844,
     844,   844,     0,     0,     0,     0,     0,     0,     0,     0,
     166,     0,     0,     0,     0,     0,     0,   815,     0,     0,
       0,     0,     0,  1144,  1200,  1201,  1148,  1202,  1203,  1204,
    1205,     0,     0,   903,     0,   906,   166,     0,     0,   150,
     298,     0,   166,   150,     0,  1158,     0,     0,     0,  1219,
       0,   150,     0,   166,     0,   166,     0,     0,     0,     0,
       0,     0,     0,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   170,   499,   499,     0,  1238,   499,
     499,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   393,  1246,  1247,  1248,  1249,     0,   899,
       0,     0,     0,     0,     0,  1250,   815,     0,     0,   499,
       0,   499,     0,     0,     0,     0,  1105,     0,     0,   394,
    1256,  1257,   166,   395,     0,     0,   166,  1213,  1246,  1247,
    1248,  1249,  1256,  1257,   166,     0,     0,     0,     0,     0,
       0,   161,     0,     0,   161,     0,  1105,     0,     0,   394,
       0,     0,     0,   395,     0,     0,     0,     0,   170,     0,
       0,     0,     0,     0,     0,   170,     0,     0,     0,     0,
     397,   398,     0,   399,   400,   401,     0,   402,   403,     0,
       0,     0,   406,   476,   479,   481,   485,   487,   490,  1213,
     413,     0,     0,   161,     0,     0,   417,   418,   419,   161,
     397,   398,     0,   399,   400,   401,     0,   402,   403,   531,
       0,     0,   406,   664,   706,   534,   170,     0,     0,   412,
     413,     0,     0,   416,     0,     0,   417,   418,   419,  1213,
       0,   170,     0,   544,   150,   228,     0,   150,     0,     0,
       0,     0,     0,     0,     0,     0,   550,   551,     0,   229,
     230,     0,   231,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,   235,     0,
     171,     0,   236,  1027,  1028,   237,   150,     0,     0,     0,
     171,     0,   150,     0,     0,   238,     0,     0,     0,     0,
       0,   239,   240,   171,     0,   161,   171,   161,     0,   241,
       0,     0,   161,     0,     0,   161,     0,   166,   242,     0,
     166,     0,     0,     0,   161,     0,   161,   243,   244,   161,
     245,     0,   246,     0,   247,   170,     0,   248,     0,     0,
       0,   249,   520,     0,   250,     0,     0,   251,     0,     0,
       0,     0,     0,   161,     0,   171,     0,     0,     0,   171,
     171,   170,     0,   161,     0,   161,     0,   170,     0,   166,
       0,     0,     0,     0,     0,   166,     0,     0,   170,   730,
     170,     0,   171,     0,     0,     0,     0,   171,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   150,     0,
     150,     0,     0,   521,     0,   150,     0,   745,   150,     0,
       0,   750,     0,   751,     0,     0,   754,   150,     0,   150,
     674,     0,   150,     0,     0,   161,     0,     0,     0,     0,
       0,     0,   161,     0,   888,     0,     0,   889,     0,     0,
       0,     0,   890,     0,     0,   161,   150,   170,     0,     0,
       0,   170,     0,   196,     0,     0,   150,     0,   150,   170,
       0,     0,     0,   891,     0,     0,     0,     0,   171,   173,
     892,     0,     0,     0,     0,     0,   161,     0,     0,   173,
     893,   166,     0,   166,     0,   161,     0,   894,   166,   161,
       0,   166,   173,     0,     0,   173,     0,     0,     0,     0,
     166,     0,   166,   895,     0,   166,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   896,     0,     0,   150,   678,
       0,     0,     0,     0,     0,   150,   897,     0,     0,   166,
       0,     0,   898,     0,   161,     0,     0,     0,   150,   166,
       0,   166,     0,     0,   173,     0,   460,     0,   173,   173,
      22,    23,   171,     0,     0,  1004,     0,     0,     0,   171,
     461,     0,    31,   462,     0,     0,     0,     0,    37,   150,
       0,   173,     0,     0,     0,    42,   173,     0,   150,     0,
       0,     0,   150,     0,     0,     0,     0,     0,     0,   161,
     161,   161,   161,     0,     0,     0,     0,     0,     0,     0,
      57,   166,    59,     0,   345,     0,  1005,     0,   166,  1006,
     171,   463,     0,    67,     0,   161,   161,     0,     0,     0,
       0,   166,     0,     0,     0,   171,     0,   150,     0,     0,
       0,     0,    83,     0,     0,    85,     0,     0,    87,     0,
       0,     0,   170,     0,     0,   170,     0,     0,     0,     0,
       0,     0,   166,     0,     0,     0,     0,   173,     0,     0,
       0,   166,     0,     0,     0,   166,     0,   923,   925,     0,
       0,     0,     0,   930,   933,     0,     0,   935,   937,     0,
       0,     0,   150,   150,   150,   150,     0,     0,   102,     0,
       0,     0,     0,     0,   170,     0,     0,     0,     0,     0,
     170,     0,     0,   228,     0,     0,     0,     0,   150,   150,
     166,     0,     0,     0,     0,     0,     0,   229,   230,     0,
     231,     0,     0,     0,     0,   232,     0,     0,     0,   171,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     234,   173,     0,     0,     0,     0,   235,     0,   173,     0,
     236,     0,     0,   237,     0,   171,     0,     0,     0,     0,
       0,   171,     0,   238,     0,   166,   166,   166,   166,   239,
     240,     0,   171,     0,   171,     0,     0,   241,     0,     0,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,   166,   166,     0,   175,   243,   244,     0,   245,   173,
     246,     0,   247,     0,   175,   248,   170,     0,   170,   249,
       0,     0,   250,   170,   173,   251,   170,   175,     0,     0,
     175,     0,     0,     0,     0,   170,     0,   170,     0,   176,
     170,     0,     0,     0,     0,     0,     0,  1039,  1040,   176,
       0,   171,     0,     0,  1041,   171,     0,     0,     0,     0,
       0,     0,   176,   171,   170,   176,  1052,     0,     0,  1055,
       0,  1056,     0,  1057,   170,     0,   170,     0,     0,   175,
       0,   277,     0,   175,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   852,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   175,     0,     0,     0,
       0,   175,     0,     0,   176,     0,  -295,     0,   176,   176,
    -295,  -295,     0,     0,     0,  -295,     0,     0,   173,     0,
    -295,     0,  -295,  -295,     0,     0,   170,     0,  -295,     0,
       0,   176,     0,   170,     0,  -295,   176,     0,  -295,     0,
       0,     0,     0,     0,   173,     0,   170,     0,     0,     0,
     173,     0,     0,     0,     0,     0,     0,  -295,     0,     0,
    -295,   173,  -295,   173,  -295,     0,  -295,  -295,     0,  -295,
     138,  -295,     0,  -295,     0,     0,     0,   170,     0,     0,
     138,     0,   175,     0,     0,     0,   170,     0,     0,     0,
     170,     0,  -295,   138,     0,  -295,   138,     0,  -295,     0,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,   433,   434,   435,     0,     0,   176,     0,     0,
    1176,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     173,  -591,     0,     0,   173,   170,   171,     0,     0,   171,
       0,     0,   173,     0,   436,   138,     0,  -630,  -295,  -630,
       0,     0,     0,     0,  -295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   175,     0,     0,     0,
       0,     0,   138,   175,     0,     0,     0,   138,     0,     0,
       0,     0,     0,   228,     0,     0,     0,     0,   171,     0,
     170,   170,   170,   170,   171,     0,     0,   229,   230,     0,
     231,   176,     0,     0,     0,   232,     0,     0,   176,     0,
       0,     0,     0,     0,   233,     0,   170,   170,     0,     0,
     234,     0,     0,     0,   175,     0,   235,     0,   110,     0,
     236,     0,     0,   237,     0,     0,     0,     0,   187,   175,
       0,     0,     0,   238,     0,     0,     0,     0,     0,   239,
     240,   221,     0,     0,   226,     0,     0,   241,   138,   176,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,     0,     0,   176,   243,   244,     0,   245,     0,
     246,     0,   247,     0,     0,   248,     0,     0,     0,   249,
       0,     0,   250,     0,     0,   251,     0,     0,     0,     0,
     171,     0,   171,   313,     0,     0,     0,   171,     0,     0,
     171,     0,     0,     0,     0,     0,     0,     0,     0,   171,
       0,   171,   282,     0,   171,   173,     0,     0,   173,     0,
     110,     0,     0,     0,     0,   346,     0,     0,     0,     0,
       0,     0,   138,   175,   282,     0,     0,     0,   171,   138,
       0,   774,     0,     0,   317,   319,     0,     0,   171,     0,
     171,     0,     0,     0,     0,     0,     0,     0,     0,   175,
       0,     0,     0,     0,     0,   175,   460,   173,   176,     0,
      22,    23,     0,   173,     0,  1004,   175,     0,   175,     0,
     461,     0,    31,   462,     0,     0,     0,     0,    37,     0,
     138,     0,     0,     0,   176,    42,     0,     0,     0,     0,
     176,     0,     0,     0,     0,   138,   443,     0,     0,     0,
     171,   176,     0,   176,     0,     0,     0,   171,     0,     0,
      57,     0,    59,     0,    61,     0,  1005,     0,     0,  1006,
     171,   463,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,     0,     0,     0,   175,
       0,     0,    83,     0,     0,    85,     0,   175,    87,     0,
       0,   171,     0,     0,     0,     0,     0,     0,     0,     0,
     171,     0,     0,     0,   171,     0,     0,     0,   452,   173,
     176,   173,     0,     0,   176,     0,   173,     0,     0,   173,
     505,     0,   176,     0,     0,     0,     0,   530,   173,     0,
     173,     0,     0,   173,     0,     0,     0,     0,   102,   138,
       0,     0,     0,     0,  1007,     0,     0,     0,     0,   171,
       0,   282,   282,   282,   282,   282,   282,   173,     0,   495,
     498,     0,     0,     0,     0,   138,   504,   173,     0,   173,
       0,   138,     0,     0,     0,     0,     0,   282,   110,     0,
       0,     0,   138,   282,   138,     0,     0,     0,     0,     0,
       0,     0,     0,   110,     0,     0,     0,     0,     0,     0,
       0,   282,     0,     0,   171,   171,   171,   171,     0,     0,
       0,     0,     0,     0,   282,   282,     0,     0,     0,     0,
       0,     0,     0,   460,     0,     0,     0,    22,    23,   173,
     171,   171,     0,     0,     0,     0,   173,   461,     0,    31,
     462,     0,     0,     0,   228,    37,     0,     0,     0,   173,
       0,   138,    42,     0,     0,   138,     0,     0,   229,   230,
       0,   231,     0,   138,     0,     0,   232,     0,     0,     0,
     175,     0,     0,   175,     0,   233,     0,    57,     0,    59,
     173,   234,     0,     0,     0,     0,   252,   235,   463,   173,
      67,   236,     0,   173,   237,     0,     0,   110,     0,     0,
     272,   273,   274,     0,   238,   176,     0,     0,   176,    83,
     239,   240,    85,     0,     0,    87,     0,     0,   241,     0,
       0,     0,   175,   530,     0,     0,     0,   242,   175,   530,
       0,     0,     0,     0,     0,     0,   243,   244,   173,   245,
     757,   246,     0,   247,     0,     0,   248,   282,     0,     0,
     249,     0,     0,   250,     0,     0,   251,   176,     0,     0,
       0,     0,     0,   176,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   282,     0,     0,     0,   282,
       0,   282,     0,     0,   282,     0,     0,     0,     0,     0,
       0,     0,     0,   173,   173,   173,   173,     0,     0,     0,
       0,     0,     0,   362,     0,   365,     0,     0,     0,   809,
     451,     0,   975,   814,   383,     0,     0,     0,     0,   173,
     173,   110,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   175,     0,   175,     0,     0,     0,
       0,   175,     0,     0,   175,     0,   138,     0,     0,   138,
       0,     0,     0,   175,     0,   175,   448,     0,   175,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,   176,     0,     0,     0,     0,   176,     0,     0,   176,
       0,     0,   175,     0,     0,     0,     0,     0,   176,     0,
     176,     0,   175,   176,   175,     0,     0,     0,   138,     0,
       0,     0,   460,     0,   138,     0,    22,    23,     0,     0,
       0,  1004,   474,     0,     0,     0,   461,   176,    31,   462,
     492,   493,     0,     0,    37,     0,     0,   176,     0,   176,
     460,    42,     0,   522,    22,    23,     0,     0,     0,     0,
       0,     0,     0,     0,   461,     0,    31,   462,     0,     0,
       0,     0,    37,     0,   175,     0,    57,     0,    59,    42,
      61,   175,  1005,     0,     0,  1006,     0,   463,     0,    67,
       0,     0,     0,   -54,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,     0,    59,     0,    83,   176,
      63,    85,   -54,    64,    87,   463,   176,    67,     0,     0,
       0,     0,     0,     0,   942,   175,     0,   945,     0,   176,
     138,     0,   138,     0,   175,   589,    83,   138,   175,    85,
     138,     0,    87,     0,     0,     0,     0,     0,     0,   138,
       0,   138,     0,     0,   138,   282,   282,     0,   648,     0,
     176,   282,   282,     0,   102,   282,   282,     0,     0,   176,
    1120,     0,     0,   176,     0,   947,   979,     0,   138,     0,
       0,     0,   984,   175,     0,     0,     0,     0,   138,     0,
     138,   460,   102,     0,     0,    22,    23,     0,     0,     0,
    1004,     0,     0,     0,     0,   461,     0,    31,   462,     0,
       0,     0,     0,    37,   976,     0,     0,     0,   176,     0,
      42,     0,     0,     0,     0,     0,   727,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   252,   175,   175,
     175,   175,     0,     0,     0,    57,     0,    59,     0,    61,
     138,  1005,     0,     0,  1006,     0,   463,   138,    67,     0,
       0,     0,     0,     0,   175,   175,     0,     0,     0,     0,
     138,     0,     0,   176,   176,   176,   176,    83,     0,   766,
      85,     0,     0,    87,     0,     0,     0,     0,   530,     0,
     530,     0,     0,   775,     0,   530,     0,   522,   530,   176,
     176,   138,     0,     0,     0,     0,     0,  1058,     0,  1059,
     138,     0,  1061,     0,   138,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   282,   282,     0,     0,     0,
       0,     0,   282,   102,   498,     0,  1094,     0,     0,  1191,
       0,   498,     0,     0,   282,     0,  1100,   282,  1104,   282,
     363,   282,   228,     0,     0,     0,   391,   831,     0,   138,
       0,     0,     0,   392,  1066,     0,   229,   230,     0,   231,
     365,     0,     0,     0,   232,     0,   393,   483,     0,     0,
     364,     0,     0,   233,     0,     0,     0,     0,     0,   234,
       0,  1098,   484,     0,     0,   235,     0,     0,     0,   236,
       0,     0,   237,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   238,     0,   138,   138,   138,   138,   239,   240,
       0,     0,   393,     0,     0,     0,   241,     0,  1152,     0,
       0,     0,   394,     0,   383,   242,   395,     0,     0,     0,
     138,   138,     0,     0,   243,   244,     0,   245,     0,   246,
       0,   247,     0,     0,   248,     0,     0,     0,   249,  1175,
       0,   250,     0,   480,   251,     0,     0,   396,  1180,     0,
       0,     0,  1182,     0,     0,     0,     0,     0,   394,     0,
       0,     0,   395,   397,   398,     0,   399,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,   408,   409,     0,
     410,   411,   412,   413,   414,   415,   416,     0,   282,   417,
     418,   419,     0,   451,     0,     0,     0,  1210,     0,     0,
     420,     0,     0,     0,     0,     0,     0,     0,     0,   397,
     398,     0,   399,   400,   401,     0,   402,   403,   404,   948,
       0,   406,     0,     0,     0,     0,     0,   952,   412,   413,
     522,     0,   416,     0,   522,   417,   418,   419,   498,   498,
       0,     0,   498,   498,     0,     5,   420,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,   977,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,     0,     0,
    1231,     0,   498,     0,   498,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
     365,    54,    55,   365,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,   648,     0,     0,     0,    65,   201,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    94,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    -2,     4,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,   766,     0,    12,    13,
      14,    15,    16,   105,    17,   204,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,   977,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -54,     0,    40,    41,    42,     0,    43,  -313,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -54,
      52,    53,     0,    54,    55,    56,     0,  -313,     0,     0,
      57,    58,    59,    60,    61,    62,    63,  -313,   -54,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -54,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,     0,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
    -661,     0,    12,    13,    14,    15,    16,  -661,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    -661,    28,    29,  -661,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -54,     0,    40,    41,    42,
       0,    43,  -313,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -54,    52,    53,     0,    54,    55,    56,
       0,  -313,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -313,   -54,    64,    65,    66,  -661,    67,    68,    69,
    -661,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -54,    87,    88,     0,     0,    89,     0,    90,     0,
       0,  -661,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -661,  -661,    94,
    -661,  -661,  -661,  -661,  -661,  -661,  -661,     0,  -661,  -661,
    -661,  -661,  -661,     0,  -661,  -661,  -661,  -661,  -661,  -661,
    -661,  -661,   102,  -661,  -661,  -661,     0,   104,  -661,   105,
       0,   106,     0,   329,  -661,     5,   297,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -54,
       0,    40,    41,    42,     0,    43,  -313,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -54,    52,    53,
       0,    54,    55,    56,     0,  -313,     0,     0,    57,    58,
      59,    60,    61,    62,    63,  -313,   -54,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -54,    87,    88,     0,     0,
      89,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   102,     0,     0,   103,
       0,   104,   330,   105,     0,   106,     0,     4,   107,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -54,     0,    40,    41,    42,     0,    43,
    -313,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -54,    52,    53,     0,    54,    55,    56,     0,  -313,
       0,     0,    57,    58,    59,    60,    61,    62,    63,  -313,
     -54,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -54,
      87,    88,     0,     0,    89,     0,    90,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
     102,     0,     0,   103,     0,   104,   554,   105,     0,   106,
       0,   573,   107,     5,     0,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,    13,    14,    15,    16,     0,
      17,     0,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -54,     0,    40,
      41,    42,     0,    43,  -313,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -54,    52,    53,     0,    54,
      55,    56,     0,  -313,     0,     0,    57,    58,    59,    60,
      61,    62,    63,  -313,   -54,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -54,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
     574,   105,     0,   106,     0,   329,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -54,     0,    40,    41,    42,     0,    43,  -313,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -54,
      52,    53,     0,    54,    55,    56,     0,  -313,     0,     0,
      57,    58,    59,    60,    61,    62,    63,  -313,   -54,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -54,    87,    88,
       0,     0,    89,     0,    90,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,   102,     0,
       0,   103,     0,   104,   330,   105,     0,   106,     0,     4,
     107,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -54,     0,    40,    41,    42,
       0,    43,  -313,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -54,    52,    53,     0,    54,    55,    56,
       0,  -313,     0,     0,    57,    58,    59,    60,    61,    62,
      63,  -313,   -54,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -54,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,   821,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -54,
       0,    40,    41,    42,     0,    43,  -313,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -54,    52,    53,
       0,    54,    55,    56,     0,  -313,     0,     0,    57,    58,
      59,    60,   345,    62,    63,  -313,   -54,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -54,    87,    88,     0,     0,
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
      37,    38,    39,   -54,     0,    40,    41,    42,     0,    43,
    -313,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -54,    52,    53,     0,    54,    55,    56,     0,  -313,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -313,
     -54,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -54,
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
      33,    34,    35,    36,    37,   740,    39,   -54,     0,    40,
      41,    42,     0,    43,  -313,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -54,    52,    53,     0,    54,
      55,    56,     0,  -313,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -313,   -54,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -54,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,    28,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,   746,
      39,   -54,     0,    40,    41,    42,     0,    43,  -313,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -54,
      52,    53,     0,    54,    55,    56,     0,  -313,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -313,   -54,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -54,    87,    88,
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
      35,    36,    37,  1043,    39,   -54,     0,    40,    41,    42,
       0,    43,  -313,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -54,    52,    53,     0,    54,    55,    56,
       0,  -313,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -313,   -54,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -54,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1045,    39,   -54,
       0,    40,    41,    42,     0,    43,  -313,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -54,    52,    53,
       0,    54,    55,    56,     0,  -313,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -313,   -54,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -54,    87,    88,     0,     0,
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
      37,  1050,    39,   -54,     0,    40,    41,    42,     0,    43,
    -313,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,   -54,    52,    53,     0,    54,    55,    56,     0,  -313,
       0,     0,    57,    58,    59,    60,     0,    62,    63,  -313,
     -54,    64,    65,    66,     0,    67,    68,    69,     0,    70,
      71,    72,    73,    74,    75,    76,    77,     0,    78,    79,
       0,    80,    81,    82,    83,    84,     0,    85,    86,   -54,
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
      33,    34,    35,    36,    37,  1053,    39,   -54,     0,    40,
      41,    42,     0,    43,  -313,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,   -54,    52,    53,     0,    54,
      55,    56,     0,  -313,     0,     0,    57,    58,    59,    60,
       0,    62,    63,  -313,   -54,    64,    65,    66,     0,    67,
      68,    69,     0,    70,    71,    72,    73,    74,    75,    76,
      77,     0,    78,    79,     0,    80,    81,    82,    83,    84,
       0,    85,    86,   -54,    87,    88,     0,     0,    89,     0,
      90,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,   102,     0,     0,   103,     0,   104,
       0,   105,     0,   106,     0,     4,   107,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
      14,    15,    16,     0,    17,     0,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,     0,  1093,    29,     0,
      30,     0,    31,    32,    33,    34,    35,    36,    37,    38,
      39,   -54,     0,    40,    41,    42,     0,    43,  -313,     0,
      44,    45,    46,    47,    48,     0,    49,    50,    51,   -54,
      52,    53,     0,    54,    55,    56,     0,  -313,     0,     0,
      57,    58,    59,    60,     0,    62,    63,  -313,   -54,    64,
      65,    66,     0,    67,    68,    69,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
      81,    82,    83,    84,     0,    85,    86,   -54,    87,    88,
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
      35,    36,    37,  1099,    39,   -54,     0,    40,    41,    42,
       0,    43,  -313,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,   -54,    52,    53,     0,    54,    55,    56,
       0,  -313,     0,     0,    57,    58,    59,    60,     0,    62,
      63,  -313,   -54,    64,    65,    66,     0,    67,    68,    69,
       0,    70,    71,    72,    73,    74,    75,    76,    77,     0,
      78,    79,     0,    80,    81,    82,    83,    84,     0,    85,
      86,   -54,    87,    88,     0,     0,    89,     0,    90,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,   102,     0,     0,   103,     0,   104,     0,   105,
       0,   106,     0,     4,   107,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1103,    39,   -54,
       0,    40,    41,    42,     0,    43,  -313,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,   -54,    52,    53,
       0,    54,    55,    56,     0,  -313,     0,     0,    57,    58,
      59,    60,     0,    62,    63,  -313,   -54,    64,    65,    66,
       0,    67,    68,    69,     0,    70,    71,    72,    73,    74,
      75,    76,    77,     0,    78,    79,     0,    80,    81,    82,
      83,    84,     0,    85,    86,   -54,    87,    88,     0,     0,
      89,     0,    90,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   102,     0,     0,   103,
       0,   104,     0,   105,     0,   106,     0,  1013,   107,     5,
     297,     6,     7,     8,     9,    10,    11,     0,     0,     0,
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
    1014,   963,   107,     5,   297,     6,     7,     8,     9,    10,
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
       9,    10,    11,     0,   605,     0,   194,     0,     0,    15,
      16,   607,    17,     0,   195,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,   613,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,   497,   422,   423,   424,
     425,   426,     0,     0,   429,   430,   431,   432,     0,   434,
     435,   832,   833,   834,   835,   836,   628,     0,   629,     0,
      97,     0,   630,   631,   632,   633,   634,   635,   636,   637,
     837,   639,   640,    99,   838,   101,     0,   642,   643,   839,
     645,   203,     0,   105,     0,   204,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,    13,
       0,    15,    16,     0,    17,     0,   195,    19,    20,    21,
       0,     0,     0,     0,    26,     0,     0,    28,    29,     0,
     196,     0,     0,     0,    33,    34,    35,    36,     0,    38,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,    49,    50,    51,     0,
      52,    53,     0,    54,    55,    56,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,    70,    71,    72,
      73,    74,    75,    76,    77,     0,    78,    79,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,    89,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   104,     0,   105,     0,   106,     0,     0,
     107,     5,   297,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,   199,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,   301,   302,     0,    84,   334,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,   335,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,   204,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,   352,    23,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,    59,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,    67,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,    87,    88,     0,     0,     0,     0,
       0,     0,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,     0,   204,     0,     0,   107,     5,   297,     6,
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
     301,   302,     0,    84,   334,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,   805,   204,     0,     0,
     107,     5,   297,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,   199,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,   301,   302,     0,    84,   334,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
     807,   204,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,   710,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   199,    39,     0,   711,     0,
      41,     0,     0,    43,     0,     0,   200,     0,     0,    47,
      48,     0,     0,    50,    51,     0,    52,    53,     0,    54,
      55,     0,     0,     0,     0,     0,     0,    58,     0,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,   712,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,   713,    88,     0,     0,     0,     0,
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
      75,    76,   202,     0,    78,     0,     0,    80,   301,   302,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,   303,     0,     0,   103,
       0,   203,     0,   105,     0,   204,     0,     0,   107,     5,
     297,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     194,     0,     0,    15,    16,     0,    17,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   196,     0,     0,     0,    33,   197,   198,     0,
       0,   199,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   200,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   201,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   202,     0,    78,     0,
       0,    80,   301,   302,     0,    84,     0,     0,    86,     0,
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
     198,     0,     0,   199,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,   965,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,     0,     0,     0,     0,
       0,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,   301,   302,     0,    84,     0,     0,
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
      55,     0,     0,     0,     0,     0,     0,    58,   256,    60,
       0,    62,     0,     0,     0,     0,    65,   201,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
     202,     0,    78,     0,     0,    80,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
       0,   257,     0,    91,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    92,
      93,    94,    95,    96,     0,     0,     0,     0,    97,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    98,     0,
       0,    99,   100,   101,     0,     0,     0,   103,     0,   203,
       0,   105,     0,   204,     0,     5,   107,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,   195,     0,     0,    21,     0,   264,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,   201,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,     0,    86,     0,   265,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   203,     0,   105,     0,   204,     0,     0,   107,     5,
     297,     6,     7,     8,     9,    10,    11,     0,     0,     0,
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
       0,    88,     0,     0,     0,     0,     0,   257,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   203,     0,   105,     0,   204,
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
      78,     0,     0,    80,   301,   302,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,   204,     0,     0,   107,     5,   297,     6,     7,     8,
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
       0,   203,   803,   105,     0,   204,     0,     0,   107,     5,
     297,     6,     7,     8,     9,    10,    11,     0,     0,     0,
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
     813,     5,   107,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   194,     0,     0,    15,    16,     0,    17,     0,
     195,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   196,     0,     0,     0,    33,   197,
     198,     0,     0,   199,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   200,     0,     0,    47,    48,     0,
       0,    50,    51,     0,    52,    53,     0,    54,    55,     0,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,     0,     0,     0,    65,   201,  -705,     0,     0,     0,
    -705,     0,     0,    72,    73,    74,    75,    76,   202,     0,
      78,     0,     0,    80,     0,     0,     0,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,     0,     0,     0,
       0,    91,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,    93,    94,
      95,    96,     0,     0,     0,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    98,     0,     0,    99,
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,   204,     0,     0,   107,     5,   297,     6,     7,     8,
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
       0,   203,     0,   105,     0,   204,  1156,     5,   107,     6,
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
       0,     0,     0,     0,     0,   257,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,     0,   204,     0,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     194,     0,     0,    15,    16,     0,    17,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,   281,
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
       0,     0,   107,     5,   297,     6,     7,     8,     9,    10,
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
     355,    10,    11,     0,     0,     0,   194,     0,     0,    15,
      16,     0,    17,     0,    18,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   196,     0,
       0,     0,    33,   197,   198,     0,     0,   199,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   200,     0,
       0,    47,    48,     0,     0,    50,    51,     0,    52,    53,
       0,    54,    55,     0,     0,     0,     0,     0,     0,    58,
       0,    60,     0,    62,     0,     0,     0,     0,    65,    66,
       0,     0,     0,     0,     0,     0,     0,    72,    73,    74,
      75,    76,   202,     0,    78,     0,     0,    80,     0,     0,
       0,    84,     0,    85,    86,     0,     0,    88,     0,     0,
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
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,     0,   204,   818,     5,
     107,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     194,     0,     0,    15,    16,     0,    17,     0,   195,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   196,     0,     0,     0,    33,   197,   198,     0,
       0,   199,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   200,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   201,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   202,     0,    78,     0,
       0,    80,     0,     0,     0,    84,   991,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    92,    93,    94,    95,    96,
       0,     0,     0,     0,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    98,     0,     0,    99,   100,   101,
       0,     0,     0,   103,     0,   203,     0,   105,     0,   204,
       0,     0,   107,     5,   297,     6,     7,     8,     9,    10,
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
       0,   105,     0,  1069,     0,     5,   107,     6,     7,     8,
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
       0,    84,  1184,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    92,    93,    94,    95,    96,     0,     0,     0,     0,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,     0,     0,    99,   100,   101,     0,     0,     0,   103,
       0,   203,     0,   105,     0,   204,     0,     0,   107,     5,
     297,     6,     7,     8,     9,    10,    11,     0,     0,     0,
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
       0,     0,     0,   103,     0,   203,     0,   105,     0,  1069,
       0,     0,   107,     5,   297,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,  1142,    39,     0,     0,     0,
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
       0,   203,     0,   105,     0,   204,     0,     5,   107,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,   780,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   199,
      39,     0,     0,     0,   781,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,     0,     0,    72,
     782,    74,    75,    76,   783,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    92,    93,    94,    95,    96,     0,     0,
       0,     0,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    98,     0,     0,    99,   100,   101,     0,     0,
       0,   103,     0,   203,     0,   105,     0,   784,     0,     5,
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
       0,     0,     0,   103,     0,   203,     0,   105,     0,   858,
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
     100,   101,     0,     0,     0,   103,     0,   203,     0,   105,
       0,   784,     0,     5,   107,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   194,     0,     0,    15,    16,     0,
      17,     0,   195,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   196,     0,     0,     0,
      33,   197,   198,     0,     0,   902,    39,     0,     0,     0,
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
       0,     0,    33,   197,   198,     0,     0,   905,    39,     0,
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
     196,     0,     0,     0,    33,   197,   198,     0,     0,  1136,
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
       0,  1137,    39,     0,     0,     0,    41,     0,     0,    43,
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
     198,     0,     0,  1139,    39,     0,     0,     0,    41,     0,
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
      33,   197,   198,     0,     0,  1140,    39,     0,     0,     0,
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
       0,     0,    33,   197,   198,     0,     0,  1141,    39,     0,
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
     196,     0,     0,     0,    33,   197,   198,     0,     0,  1142,
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
       0,   199,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   200,     0,     0,    47,    48,     0,     0,    50,
      51,     0,    52,    53,     0,    54,    55,     0,     0,     0,
       0,     0,     0,    58,     0,    60,     0,    62,     0,     0,
       0,     0,    65,   201,     0,     0,     0,     0,     0,     0,
       0,    72,    73,    74,    75,    76,   202,     0,    78,     0,
       0,    80,     0,     0,     0,    84,     0,     0,    86,     0,
       0,    88,     0,     0,     0,     0,     0,     0,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   603,
       0,   604,     0,     0,     0,    92,    93,    94,    95,    96,
     605,     0,     0,     0,    97,   606,   230,   607,   608,     0,
       0,     0,     0,   609,    98,     0,     0,    99,   100,   101,
       0,     0,   233,   103,   196,     0,     0,   105,   610,   858,
       0,     0,   107,     0,   611,     0,     0,     0,   236,     0,
       0,   612,     0,   613,     0,     0,     0,     0,     0,     0,
       0,   614,     0,     0,     0,     0,     0,   615,   616,     0,
       0,     0,     0,     0,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,   617,     0,     0,     0,     0,     0,
       0,     0,     0,   243,   244,     0,   618,     0,   246,     0,
     619,     0,     0,   620,     0,     0,     0,   621,     0,     0,
     250,     0,     0,   622,     0,     0,     0,     0,     0,     0,
       0,     0,   497,   422,   423,   424,   425,   426,     0,     0,
     429,   430,   431,   432,     0,   434,   435,   623,   624,   625,
     626,   627,   628,     0,   629,     0,     0,     0,   630,   631,
     632,   633,   634,   635,   636,   637,   638,   639,   640,     0,
     641,     0,     0,   642,   643,   644,   645,     0,     5,   646,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   194,
       0,     0,    15,    16,     0,    17,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   196,     0,     0,     0,    33,   197,   198,     0,     0,
     199,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   200,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   201,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   202,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,    93,    94,    95,    96,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    98,     0,     0,     0,   100,   101,     0,
       0,     0,   103,     0,   203,     0,   105,     5,   204,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   199,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,  -470,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,  -470,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,  -470,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,  -470,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     5,   297,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   194,     0,
       0,    15,    16,     0,    17,     0,   195,   101,  -470,    21,
       0,     0,     0,  -470,     0,   105,     0,   858,    29,     0,
     196,     0,     0,     0,    33,   197,   198,     0,     0,   199,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     200,     0,     0,    47,    48,     0,     0,    50,    51,     0,
      52,    53,     0,    54,    55,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,     0,     0,     0,
      65,   201,     0,     0,     0,     0,     0,     0,     0,    72,
      73,    74,    75,    76,   202,     0,    78,     0,     0,    80,
       0,     0,     0,    84,     0,     0,    86,     0,     0,    88,
       0,     0,     0,     0,     0,     0,     0,     0,  -488,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    94,     0,     0,     0,  -488,
       0,     0,     0,  -488,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   101,     0,     0,
       0,     0,     0,   203,     0,   105,  -488,  1069,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   194,
       0,     0,    15,    16,     0,    17,     0,   195,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   196,     0,     0,     0,    33,   197,   198,     0,     0,
     199,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   200,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,    62,     0,     0,     0,
       0,    65,   201,     0,     0,     0,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   202,     0,    78,     0,     0,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    92,     0,    94,     0,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   194,
       0,     0,    15,    16,     0,    17,     0,   195,   101,   393,
      21,     0,     0,     0,   203,     0,   105,     0,   204,    29,
       0,   196,     0,     0,     0,    33,   197,   198,     0,     0,
     199,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   200,     0,     0,    47,    48,     0,     0,    50,    51,
       0,    52,    53,     0,    54,    55,     0,     0,   674,     0,
       0,     0,    58,     0,    60,   394,    62,   605,     0,   395,
       0,    65,   201,     0,   607,   675,     0,     0,     0,     0,
      72,    73,    74,    75,    76,   202,     0,    78,     0,   676,
      80,     0,     0,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     613,     0,     0,     0,     0,     0,   397,   398,   677,   399,
     400,   401,     0,   402,   403,   404,    94,   405,   406,   407,
     408,     0,     0,   410,   411,   412,   413,   414,     0,   416,
       0,     0,   417,   418,   419,     0,     0,     0,   101,     0,
       0,     0,     0,   420,   203,     0,   105,   678,   784,     0,
       0,     0,     0,   679,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   497,
     422,   423,   424,   425,   426,     0,     0,   429,   430,   431,
     432,     0,   434,   435,   623,   624,   625,   626,   627,   628,
       0,   629,     0,     0,     0,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,  1019,   641,     0,     0,
     642,   643,   644,   645,   988,   605,     0,     0,     0,     0,
     229,   230,   607,   231,     0,     0,     0,     0,   232,     0,
       0,     0,     0,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,   610,     0,     0,     0,     0,     0,   235,
       0,     0,     0,   236,     0,     0,   237,     0,   613,     0,
       0,     0,     0,     0,     0,     0,   238,     0,     0,     0,
       0,     0,   615,   240,     0,     0,     0,     0,     0,     0,
     241,     0,     0,     0,     0,     0,     0,     0,     0,   242,
       0,     0,     0,     0,     0,     0,     0,   391,   243,   244,
       0,   245,     0,   246,   392,  1020,     0,     0,   620,     0,
       0,     0,   249,     0,     0,   250,     0,   393,   251,     0,
       0,     0,     0,     0,     0,     0,     0,   497,   422,   423,
     424,   425,   426,     0,     0,   429,   430,   431,   432,     0,
     434,   435,   623,   624,   625,   626,   627,   628,     0,   629,
       0,     0,     0,   630,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,     0,   641,     0,     0,   642,   643,
     644,   645,     0,   394,     0,     0,     0,   395,     0,     0,
     391,     0,     0,     0,     0,     0,   496,   392,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     393,     0,     0,     0,     0,     0,     0,     0,   396,   497,
     422,   423,   424,   425,   426,     0,     0,   429,   430,   431,
     432,     0,   434,   435,   397,   398,     0,   399,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,   408,   409,
       0,   410,   411,   412,   413,   414,   415,   416,     0,     0,
     417,   418,   419,     0,   451,     0,   394,     0,     0,     0,
     395,   420,     0,   391,     0,     0,     0,     0,     0,  1042,
     392,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   393,     0,     0,     0,     0,     0,     0,
       0,   396,   497,   422,   423,   424,   425,   426,     0,     0,
     429,   430,   431,   432,     0,   434,   435,   397,   398,     0,
     399,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,   408,   409,     0,   410,   411,   412,   413,   414,   415,
     416,     0,     0,   417,   418,   419,     0,   451,     0,   394,
       0,     0,     0,   395,   420,     0,   391,     0,     0,     0,
       0,     0,  1049,   392,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   393,     0,     0,     0,
       0,     0,     0,     0,   396,   497,   422,   423,   424,   425,
     426,     0,     0,   429,   430,   431,   432,     0,   434,   435,
     397,   398,     0,   399,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,   408,   409,     0,   410,   411,   412,
     413,   414,   415,   416,     0,     0,   417,   418,   419,     0,
     451,     0,   394,     0,     0,     0,   395,   420,     0,   391,
       0,     0,     0,     0,     0,  1206,   392,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   393,
       0,     0,     0,     0,     0,     0,     0,   396,   497,   422,
     423,   424,   425,   426,     0,     0,   429,   430,   431,   432,
       0,   434,   435,   397,   398,     0,   399,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,   408,   409,     0,
     410,   411,   412,   413,   414,   415,   416,     0,     0,   417,
     418,   419,     0,   451,     0,   394,     0,     0,     0,   395,
     420,     0,   391,     0,     0,     0,     0,     0,  1207,   392,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   393,     0,     0,     0,     0,     0,     0,     0,
     396,   497,   422,   423,   424,   425,   426,     0,     0,   429,
     430,   431,   432,     0,   434,   435,   397,   398,     0,   399,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
     408,   409,     0,   410,   411,   412,   413,   414,   415,   416,
       0,     0,   417,   418,   419,     0,   451,     0,   394,     0,
       0,     0,   395,   420,     0,   391,     0,     0,     0,     0,
       0,  1208,   392,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   393,     0,     0,     0,     0,
       0,     0,     0,   396,   497,   422,   423,   424,   425,   426,
       0,     0,   429,   430,   431,   432,     0,   434,   435,   397,
     398,     0,   399,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,   408,   409,     0,   410,   411,   412,   413,
     414,   415,   416,     0,     0,   417,   418,   419,     0,   451,
       0,   394,     0,     0,     0,   395,   420,     0,   391,     0,
       0,     0,     0,     0,  1209,   392,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   393,     0,
       0,     0,     0,     0,     0,     0,   396,   497,   422,   423,
     424,   425,   426,     0,     0,   429,   430,   431,   432,     0,
     434,   435,   397,   398,     0,   399,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,   408,   409,     0,   410,
     411,   412,   413,   414,   415,   416,     0,     0,   417,   418,
     419,     0,   451,     0,   394,     0,     0,     0,   395,   420,
       0,   391,     0,     0,     0,     0,     0,  1232,   392,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   393,     0,     0,     0,     0,     0,     0,     0,   396,
     497,   422,   423,   424,   425,   426,     0,     0,   429,   430,
     431,   432,     0,   434,   435,   397,   398,     0,   399,   400,
     401,     0,   402,   403,   404,     0,   405,   406,   407,   408,
     409,     0,   410,   411,   412,   413,   414,   415,   416,     0,
       0,   417,   418,   419,     0,   451,     0,   394,     0,     0,
       0,   395,   420,     0,   391,     0,     0,     0,     0,     0,
    1233,   392,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   393,   281,     0,     0,     0,     0,
       0,     0,   396,   497,   422,   423,   424,   425,   426,     0,
     489,   429,   430,   431,   432,     0,   434,   435,   397,   398,
       0,   399,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,   408,   409,     0,   410,   411,   412,   413,   414,
     415,   416,     0,     0,   417,   418,   419,     0,   451,     0,
     394,     0,     0,     0,   395,   420,   391,     0,     0,     0,
       0,     0,     0,   392,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   393,   477,     0,     0,
       0,   480,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,   478,     0,     0,     0,     0,     0,     0,     0,
       0,   397,   398,     0,   399,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,   408,   409,     0,   410,   411,
     412,   413,   414,   415,   416,     0,     0,   417,   418,   419,
       0,   451,   394,     0,     0,     0,   395,     0,   420,   391,
       0,     0,     0,     0,     0,     0,   392,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   393,
       0,     0,     0,     0,   582,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,   398,     0,   399,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,   408,   409,   583,
     410,   411,   412,   413,   414,   415,   416,     0,     0,   417,
     418,   419,     0,   451,     0,   394,     0,     0,     0,   395,
     420,     0,   391,   828,     0,     0,     0,     0,     0,   392,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   393,     0,     0,     0,     0,   829,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,   398,     0,   399,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
     408,   409,   830,   410,   411,   412,   413,   414,   415,   416,
       0,   584,   417,   418,   419,     0,     0,     0,   394,     0,
       0,     0,   395,   420,   391,     0,     0,     0,     0,     0,
       0,   392,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   393,   932,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
     398,     0,   399,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,   408,   409,     0,   410,   411,   412,   413,
     414,   415,   416,     0,     0,   417,   418,   419,     0,     0,
     394,     0,     0,     0,   395,     0,   420,   391,     0,     0,
       0,     0,     0,     0,   392,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   393,   281,     0,
       0,   480,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,   398,     0,   399,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,   408,   409,     0,   410,   411,
     412,   413,   414,   415,   416,     0,     0,   417,   418,   419,
       0,   451,     0,   394,     0,     0,     0,   395,   420,   391,
       0,     0,     0,     0,     0,     0,   392,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   393,
       0,     0,     0,     0,   480,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,   453,     0,     0,     0,     0,
       0,     0,     0,     0,   397,   398,     0,   399,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,   408,   409,
     393,   410,   411,   412,   413,   414,   415,   416,     0,     0,
     417,   418,   419,     0,   451,   394,     0,     0,     0,   395,
     391,   420,     0,     0,     0,     0,     0,   392,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     393,   281,     0,     0,     0,     0,   189,     0,     0,     0,
     396,     0,     0,     0,     0,     0,   394,     0,     0,     0,
     395,     0,     0,     0,     0,     0,   397,   398,     0,   399,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
     408,   409,     0,   410,   411,   412,   413,   414,   415,   416,
       0,   396,   417,   418,   419,     0,   394,     0,     0,     0,
     395,     0,   391,   420,     0,     0,     0,   397,   398,   392,
     399,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,   408,   393,   719,   410,   411,   412,   413,   414,   415,
     416,   396,     0,   417,   418,   419,     0,     0,   720,     0,
       0,     0,     0,     0,   420,     0,     0,   397,   398,     0,
     399,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,   408,   409,     0,   410,   411,   412,   413,   414,   415,
     416,     0,     0,   417,   418,   419,     0,   451,   394,     0,
       0,     0,   395,   391,   420,     0,     0,     0,     0,     0,
     392,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   393,   721,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,   722,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
     398,     0,   399,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,   408,   409,     0,   410,   411,   412,   413,
     414,   415,   416,     0,     0,   417,   418,   419,     0,   394,
       0,     0,     0,   395,   391,     0,   420,     0,     0,     0,
       0,   392,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   393,   924,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     397,   398,     0,   399,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,   408,   409,     0,   410,   411,   412,
     413,   414,   415,   416,     0,     0,   417,   418,   419,     0,
     394,     0,     0,     0,   395,     0,   391,   420,     0,     0,
       0,     0,     0,   392,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   393,     0,     0,     0,
       0,     0,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,   398,     0,   399,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,   408,   409,     0,   410,   411,
     412,   413,   414,   415,   416,     0,     0,   417,   418,   419,
       0,   451,   394,     0,     0,     0,   395,     0,   420,   391,
       0,     0,     0,     0,     0,   946,   392,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   393,
       0,     0,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,   398,     0,   399,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,   408,   409,     0,
     410,   411,   412,   413,   414,   415,   416,     0,     0,   417,
     418,   419,     0,   451,     0,   394,     0,     0,     0,   395,
     420,     0,   391,     0,     0,     0,     0,     0,     0,   392,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   393,     0,     0,     0,   480,  1112,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,   398,     0,   399,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
     408,   409,  1113,   410,   411,   412,   413,   414,   415,   416,
       0,     0,   417,   418,   419,     0,     0,     0,   394,     0,
       0,   982,   395,   420,   391,     0,     0,     0,     0,     0,
       0,   392,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   393,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
     398,     0,   399,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,   408,   409,     0,   410,   411,   412,   413,
     414,   415,   416,     0,     0,   417,   418,   419,     0,     0,
     394,     0,     0,     0,   395,     0,   420,   391,     0,     0,
       0,     0,     0,     0,   392,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   393,     0,     0,
       0,     0,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,   398,     0,   399,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,   408,   409,     0,   410,   411,
     412,   413,   414,   415,   416,     0,     0,   417,   418,   419,
       0,   543,     0,   394,     0,     0,     0,   395,   420,   391,
       0,     0,     0,     0,     0,     0,   392,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   393,
       0,     0,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   397,   398,     0,   399,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,   408,   409,
       0,   410,   411,   412,   413,   414,   415,   416,     0,   547,
     417,   418,   419,     0,     0,   394,     0,     0,     0,   395,
     391,   420,     0,     0,     0,     0,     0,   392,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     393,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,   398,     0,   399,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
     408,   409,     0,   410,   411,   412,   413,   414,   415,   416,
       0,   552,   417,   418,   419,     0,   394,     0,     0,     0,
     395,   391,     0,   420,     0,     0,     0,     0,   392,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   393,     0,     0,     0,     0,     0,     0,     0,     0,
     569,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   397,   398,     0,
     399,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,   408,   409,     0,   410,   411,   412,   413,   414,   415,
     416,     0,     0,   417,   418,   419,     0,   394,     0,     0,
       0,   395,   391,     0,   420,     0,     0,     0,     0,   392,
     723,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   393,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   397,   398,
       0,   399,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,   408,   409,     0,   410,   411,   412,   413,   414,
     415,   416,     0,     0,   417,   418,   419,     0,   394,     0,
       0,     0,   395,   391,   824,   420,     0,     0,     0,     0,
     392,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
     398,     0,   399,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,   408,   409,     0,   410,   411,   412,   413,
     414,   415,   416,     0,     0,   417,   418,   419,     0,   394,
       0,     0,   804,   395,   391,     0,   420,     0,     0,     0,
       0,   392,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   393,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     397,   398,     0,   399,   400,   401,     0,   402,   825,   404,
       0,   405,   406,   407,   408,   409,     0,   410,   411,   412,
     413,   414,   415,   416,     0,     0,   417,   418,   419,     0,
     394,     0,     0,     0,   395,   391,     0,   420,     0,     0,
       0,     0,   392,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   393,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,   398,     0,   399,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,   408,   409,  -592,   410,   411,
     412,   413,   414,   415,   416,     0,     0,   417,   418,   419,
       0,   394,     0,     0,     0,   395,   391,     0,   420,     0,
       0,     0,     0,   392,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   393,     0,     0,     0,
       0,     0,   189,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   397,   398,     0,   399,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,   408,   409,     0,   410,
     411,   412,   413,   414,   415,   416,     0,     0,   417,   418,
     419,     0,   394,     0,     0,     0,   395,   391,     0,   420,
       0,     0,     0,     0,   392,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   393,     0,     0,
     944,     0,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,   398,     0,   399,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,   408,   409,     0,
     410,   411,   412,   413,   414,   415,   416,     0,   915,   417,
     418,   419,     0,   394,     0,     0,     0,   395,   391,     0,
     420,     0,     0,     0,     0,   392,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   393,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   397,   398,     0,   399,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,   408,   409,
       0,   410,   411,   412,   413,   414,   415,   416,     0,     0,
     417,   418,   419,     0,   394,     0,     0,     0,   395,   391,
       0,   420,     0,     0,     0,     0,   392,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   393,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   397,   398,     0,   399,   400,
     401,     0,   402,   403,   404,     0,   405,   406,   407,   408,
     409,     0,   410,   411,   412,   413,   414,   415,   416,     0,
       0,   417,   418,   419,     0,   394,     0,     0,   967,   395,
     391,     0,   420,     0,     0,     0,     0,   392,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     393,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,   398,     0,   399,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
     408,   409,     0,   410,   411,   412,   413,   414,   415,   416,
       0,     0,   417,   418,   419,     0,   394,     0,     0,   968,
     395,   391,   997,   420,     0,     0,     0,     0,   392,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   393,     0,     0,     0,     0,     0,     0,     0,     0,
     985,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   397,   398,     0,
     399,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,   408,   409,     0,   410,   411,   412,   413,   414,   415,
     416,     0,     0,   417,   418,   419,     0,   394,     0,     0,
       0,   395,   391,     0,   420,     0,     0,     0,     0,   392,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   393,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   397,   398,
       0,   399,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,   408,   409,     0,   410,   411,   412,   413,   414,
     415,   416,     0,     0,   417,   418,   419,     0,   394,     0,
       0,     0,   395,   391,     0,   420,     0,     0,     0,     0,
     392,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   393,  1031,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
     398,     0,   399,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,   408,   409,     0,   410,   411,   412,   413,
     414,   415,   416,     0,     0,   417,   418,   419,     0,   394,
       0,     0,  1018,   395,   391,     0,   420,     0,     0,     0,
       0,   392,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   393,  1034,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     397,   398,     0,   399,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,   408,   409,     0,   410,   411,   412,
     413,   414,   415,   416,     0,     0,   417,   418,   419,     0,
     394,     0,     0,     0,   395,   391,     0,   420,     0,     0,
       0,     0,   392,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   393,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,   398,     0,   399,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,   408,   409,     0,   410,   411,
     412,   413,   414,   415,   416,     0,     0,   417,   418,   419,
       0,   394,     0,     0,     0,   395,   391,  1109,   420,     0,
       0,     0,     0,   392,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   393,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   397,   398,     0,   399,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,   408,   409,     0,   410,
     411,   412,   413,   414,   415,   416,     0,     0,   417,   418,
     419,     0,   394,     0,     0,     0,   395,  1037,   391,   420,
       0,     0,     0,     0,     0,   392,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   393,     0,
       0,     0,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,   398,     0,   399,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,   408,   409,     0,
     410,   411,   412,   413,   414,   415,   416,     0,     0,   417,
     418,   419,     0,     0,   394,     0,     0,     0,   395,   391,
     420,     0,     0,     0,     0,     0,   392,  1135,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   393,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   397,   398,     0,   399,   400,
     401,     0,   402,   403,   404,     0,   405,   406,   407,   408,
     409,     0,   410,   411,   412,   413,   414,   415,   416,     0,
       0,   417,   418,   419,     0,   394,     0,     0,     0,   395,
       0,     0,   420,     0,     0,     0,     0,     0,  1138,     0,
       0,     0,     0,   391,     0,     0,     0,     0,     0,     0,
     392,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,   393,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,   398,     0,   399,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
     408,   409,  1169,   410,   411,   412,   413,   414,   415,   416,
     391,  1190,   417,   418,   419,     0,     0,   392,     0,     0,
       0,     0,     0,   420,     0,     0,     0,     0,     0,   394,
     393,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     391,     0,     0,     0,     0,     0,     0,   392,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
     393,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     397,   398,     0,   399,   400,   401,   394,   402,   403,   404,
     395,   405,   406,   407,   408,   409,     0,   410,   411,   412,
     413,   414,   415,   416,     0,     0,   417,   418,   419,     0,
       0,     0,     0,     0,     0,     0,     0,   420,     0,   393,
       0,   396,     0,     0,     0,     0,   394,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,   397,   398,     0,
     399,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,   408,   409,     0,   410,   411,   412,   413,   414,   415,
     416,   396,     0,   417,   418,   419,     0,     0,     0,     0,
       0,     0,   393,     0,   420,   394,     0,   397,   398,   395,
     399,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,   408,   409,     0,   410,   411,   412,   413,   414,   415,
     416,   391,     0,   417,   418,   419,     0,     0,   392,     0,
     396,     0,  1228,     0,   420,     0,     0,     0,     0,     0,
       0,   393,     0,     0,     0,     0,   397,   398,   394,   399,
     400,   401,   395,   402,   403,   404,     0,   405,   406,   407,
     408,   391,     0,   410,   411,   412,   413,   414,   392,   416,
       0,     0,   417,   418,   419,     0,     0,     0,     0,     0,
       0,   393,     0,   420,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   394,     0,   397,
     398,   395,   399,   400,   401,     0,   402,   403,   404,     0,
    1240,   406,   407,   408,     0,     0,   410,   411,   412,   413,
       0,     0,   416,     0,     0,   417,   418,   419,     0,     0,
       0,     0,   396,     0,     0,     0,   420,   394,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,   397,   398,
    1241,   399,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,   408,   409,     0,   410,   411,   412,   413,   414,
     415,   416,   396,     0,   417,   418,   419,     0,     0,     0,
       0,     0,     0,     0,     0,   420,     0,     0,   397,   398,
       0,   399,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,   408,   409,     0,   410,   411,   412,   413,   414,
     415,   416,   391,   460,   417,   418,   419,    22,    23,   392,
       0,     0,     0,     0,     0,   420,     0,   461,     0,    31,
     462,     0,   393,     0,     0,    37,     0,     0,     0,     0,
       0,     0,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   391,     0,     0,     0,     0,     0,     0,   392,
       0,     0,     0,     0,     0,     0,     0,    57,     0,    59,
       0,     0,   393,  1005,     0,     0,  1006,     0,   463,     0,
      67,     0,     0,     0,     0,     0,     0,     0,   394,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,    83,
       0,  1242,    85,     0,     0,    87,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,   394,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,   397,
     398,  1243,   399,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,   408,   409,   102,   410,   411,   412,   413,
     414,   415,   416,   396,     0,   417,   418,   419,     0,     0,
       0,     0,     0,     0,     0,     0,   420,     0,     0,   397,
     398,     0,   399,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,   408,   409,     0,   410,   411,   412,   413,
     414,   415,   416,   391,   228,   417,   418,   419,     0,     0,
     392,     0,     0,     0,     0,     0,   420,     0,   229,   230,
       0,   231,     0,   393,     0,     0,   232,     0,     0,     0,
       0,     0,   364,     0,     0,   233,     0,     0,     0,     0,
       0,   234,     0,   391,     0,     0,     0,   235,     0,     0,
     392,   236,     0,     0,   237,     0,     0,     0,     0,     0,
       0,     0,     0,   393,   238,     0,     0,     0,     0,     0,
     239,   240,     0,     0,     0,     0,     0,     0,   241,   394,
       0,     0,     0,   395,   391,     0,     0,   242,     0,     0,
       0,   392,  1244,     0,     0,     0,   243,   244,     0,   245,
       0,   246,     0,   247,   393,     0,   248,     0,     0,     0,
     249,     0,     0,   250,   396,     0,   251,     0,     0,   394,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
     397,   398,  1245,   399,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,   408,   409,     0,   410,   411,   412,
     413,   414,   415,   416,   396,     0,   417,   418,   419,     0,
     394,     0,     0,     0,   395,     0,     0,   420,     0,     0,
     397,   398,     0,   399,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,   408,   409,     0,   410,   411,   412,
     413,   414,   415,   416,     0,   396,   417,   418,   419,     0,
       0,   391,     0,     0,     0,     0,     0,   420,   392,     0,
       0,   397,   398,     0,   399,   400,   401,     0,   402,   403,
     404,   393,   405,   406,   407,   408,   409,     0,   410,   411,
     412,   413,   414,   415,   416,     0,     0,   417,   418,   419,
       0,     0,   391,     0,     0,     0,  1258,     0,   420,   392,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   393,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   394,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,   900,     0,
       0,     0,   395,     0,     0,     0,     0,     0,   397,   398,
       0,   399,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,   408,   409,     0,   410,   411,   412,   413,   414,
     415,   416,     0,   396,   417,   418,   419,     0,     0,     0,
       0,     0,     0,     0,     0,   420,     0,     0,     0,   397,
     398,     0,   399,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,   408,   409,  -290,   410,   411,   412,   413,
     414,   415,   416,     0,     0,   417,   418,   419,     0,  -290,
    -290,     0,  -290,     0,     0,     0,   420,  -290,     0,     0,
       0,     0,     0,     0,     0,     0,  -290,     0,     0,   228,
       0,     0,  -290,     0,     0,     0,     0,     0,  -290,     0,
       0,     0,  -290,   229,   230,  -290,   231,     0,     0,     0,
       0,   232,     0,     0,     0,  -290,     0,     0,     0,     0,
     233,  -290,  -290,     0,     0,     0,   234,     0,     0,  -290,
       0,     0,   235,     0,     0,     0,   236,     0,  -290,   237,
       0,     0,     0,     0,     0,     0,     0,  -290,  -290,   238,
    -290,     0,  -290,     0,  -290,   239,   240,  -290,     0,     0,
       0,  -290,     0,   241,  -290,     0,     0,  -290,     0,     0,
       0,     0,   242,     0,     0,  -291,     0,     0,     0,     0,
       0,   243,   244,     0,   245,     0,   246,     0,   247,  -291,
    -291,   248,  -291,     0,     0,   249,     0,  -291,   250,     0,
       0,   251,     0,     0,     0,     0,  -291,     0,     0,     0,
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
      12,     2,    68,   159,    16,   382,   442,   582,    20,   655,
     382,   271,    19,    40,    26,   600,   218,   275,   744,   521,
     830,   594,    34,    35,    36,   502,    38,   225,   356,   132,
     956,   227,   545,     1,   294,   526,   962,    49,   519,    51,
     106,   959,   959,     3,    56,   848,    58,   825,    60,   651,
       1,   653,   312,    33,     1,    48,    68,    69,    70,    71,
      72,    73,    48,     3,    33,    77,    11,    79,     1,    33,
      33,   853,    33,  1067,    86,   271,     1,    89,    90,   101,
      92,     3,    94,  1156,    61,    97,    98,    74,   100,   261,
     262,   263,   104,   105,   106,   107,    91,  1218,   294,    56,
    1026,    56,    48,     0,    48,    48,    74,    48,  1014,   600,
     145,   119,   164,    48,    61,    69,   312,   145,   130,   171,
     132,   952,   145,    48,   145,   139,   119,   145,    61,   116,
      48,    91,    37,   147,   164,   170,   164,  1258,   204,    68,
     117,  1067,   102,    89,    91,  1218,    89,   169,   116,   172,
     119,   105,   145,   174,   172,   119,    56,   169,    91,   145,
     886,  1155,   102,   108,   655,   145,   174,    72,   340,   341,
     117,   104,    32,   124,   169,   104,   105,   106,   438,   439,
     102,   174,   194,   170,   117,   197,   198,   199,   174,   169,
     202,   203,   204,   144,   145,    32,   999,   144,   225,   124,
     169,   145,   170,   173,   145,   169,   169,   403,   169,   216,
     145,   144,  1130,  1130,   171,   227,   171,   995,  1000,   144,
     145,   172,   169,  1129,   737,   119,  1008,   145,    88,  1155,
     174,   164,    92,   174,    54,    48,   169,  1068,   124,   164,
     117,   145,   438,   439,   145,     3,    91,   724,    61,   119,
      22,    88,   355,    73,   829,    92,   174,   585,   145,   271,
     164,   459,    20,    83,   150,    27,   152,   153,   145,   170,
     156,   157,   774,    88,   203,   204,    34,    92,    91,   139,
     174,   854,   294,    82,   144,   172,    85,   147,     3,   301,
     302,   151,    27,   145,   360,    27,    47,   267,   779,   455,
     312,  1111,   139,  1113,   117,    63,   502,   144,   744,   146,
     147,   124,    56,    48,   151,    87,    78,    52,    56,   164,
     172,  1047,   159,   335,   169,    56,   145,    89,   165,   166,
     167,   332,   145,    84,   145,    61,   348,  1119,   350,   111,
      75,   145,  1124,    78,   102,    80,    78,   145,   360,   545,
     108,   164,   114,   172,    89,   139,   169,    89,    18,   172,
     144,   172,   113,   147,   171,    91,   173,   151,   172,   164,
     955,  1078,   170,   108,   851,   170,   108,   389,   104,   391,
     392,   393,   394,   395,   396,   587,   398,   399,   400,   401,
     402,   117,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   475,
     108,  1193,   382,   988,   124,   427,   428,    77,   899,  1021,
    1022,   433,    81,    82,   436,   437,   438,   439,   440,   441,
     442,   360,   459,    89,   911,    95,     8,   172,   164,   149,
     145,   453,   145,   169,  1090,   457,   171,    34,    35,    36,
     886,    27,   112,   145,   466,   124,  1163,  1164,  1165,  1166,
    1167,  1168,   145,   475,   955,   477,   478,   172,    47,   172,
      47,   483,   484,   427,   428,   145,   145,   489,   170,   433,
     149,    56,   436,   437,   496,     2,    65,   499,    65,   172,
     502,   868,    56,   139,   164,    12,   868,   124,   145,    48,
      61,   147,    78,   172,   145,    84,   171,    84,    25,   139,
      56,    28,    61,    89,   772,    56,   528,   147,   145,   106,
     532,   533,   149,  1014,   145,   172,   538,    56,   724,    32,
      91,   172,   108,   545,   113,  1110,   113,  1112,   171,   145,
     145,   737,    91,   104,   556,   172,   475,   559,    56,   561,
     145,   172,   164,   565,   566,   567,   117,   569,   570,   171,
      77,   173,    32,   145,    81,    82,   172,   172,   117,   164,
     582,   583,   145,    27,   575,   124,   748,   749,     2,     8,
     752,   753,   164,   595,   164,    88,   171,   104,    12,    92,
     170,   164,   109,   145,    48,   853,   145,    56,    52,  1090,
     145,    25,    99,   164,    28,   559,   864,   561,   169,   538,
      56,  1047,   164,    88,   145,   164,    61,    92,    88,   164,
     169,    75,    92,   172,    78,    56,    80,   556,   824,   825,
     559,   145,   561,   164,   646,    89,   139,   566,  1129,   811,
     812,   144,    56,   146,   147,   171,    91,   145,   151,   145,
     164,   295,   296,    77,   108,   851,   159,    81,    82,   104,
      56,     2,    48,    88,   167,  1156,   164,    92,   164,   139,
     140,    12,   117,   190,   144,    61,   146,   147,    88,    88,
     104,   151,    92,    92,    25,   109,    88,    28,    56,   159,
      92,   169,   171,   849,   173,   165,   166,   167,   171,   171,
     173,   173,   169,    56,   171,    91,   144,   719,   720,   721,
     722,   723,   724,   725,   726,   911,   164,   164,   784,   164,
     163,   728,   729,   735,   169,   737,   738,  1218,   740,    32,
      51,   117,   744,    54,   746,   155,    77,  1228,   124,   171,
      81,    82,  1000,   755,   756,   171,   164,   173,   171,   171,
     173,   173,    73,   164,   559,    76,   561,   274,   164,   145,
     164,   164,    83,   104,   281,   164,   190,  1258,   109,    56,
     782,   783,   784,   164,   169,   118,   788,   169,   164,    84,
       8,   735,   171,   169,   171,    88,   172,   171,   171,    92,
     164,   171,   858,    65,   169,     8,   124,   170,    89,    89,
     124,   164,   147,   145,    68,   171,   735,   394,   395,   108,
     822,   124,   172,   145,   826,   332,   828,   829,   830,    48,
     832,   171,   834,   124,   124,   837,   838,    35,    35,    65,
     347,  1091,  1092,   172,   788,   145,   139,   140,   172,   851,
     143,   144,   124,   146,   147,   172,   858,   172,   151,   190,
     274,    21,   174,   440,   441,   784,   159,   281,   174,   145,
     169,  1119,   165,   166,   167,   382,   453,   104,   147,   171,
     147,   147,   139,   147,   886,   164,   147,   147,   139,   147,
     139,   147,   147,   139,   147,   147,   139,   147,   900,   901,
     902,   478,   172,   905,    27,  1091,  1092,   484,   868,   911,
      35,   174,   489,    35,   144,   971,   164,   164,   332,   164,
     922,   145,   924,   164,   164,    48,   170,   929,   164,    52,
     932,    33,   171,   347,   172,   169,    33,   170,   144,   858,
     172,    35,   944,   274,   451,   174,   174,   164,   164,   174,
     281,   171,    75,   174,   164,    78,   958,    80,   171,   174,
       8,   921,   174,   172,   139,   139,    89,    35,   928,   971,
     477,   172,   164,   172,   164,   164,   483,   172,   980,   145,
     982,   163,   163,   985,   270,   108,   988,   494,   565,   496,
     347,   822,    24,   995,  1008,   997,     2,    89,   950,   319,
     975,   332,   857,   358,   868,   864,    12,   772,   513,  1130,
     863,   276,  1169,  1069,  1162,   658,   347,   130,   529,    25,
     778,    -1,    28,    -1,    -1,    -1,    -1,    -1,  1030,  1031,
      -1,  1033,  1034,    -1,  1036,  1037,   955,   451,    -1,    -1,
    1042,  1043,    -1,  1045,    -1,  1047,    -1,  1049,  1050,    -1,
      -1,  1053,   971,    -1,    -1,    -1,   563,    -1,    -1,    -1,
     567,    -1,    -1,   477,    -1,    -1,    -1,  1069,   575,   483,
      -1,    77,  1074,    -1,    -1,    81,    82,    -1,    -1,    -1,
     494,    -1,   496,    -1,    -1,    -1,    -1,  1089,  1090,  1091,
    1092,    -1,    -1,    -1,    -1,  1097,    -1,  1099,   104,    -1,
      -1,  1103,    -1,   109,    -1,    -1,    71,  1109,  1110,  1111,
    1112,  1113,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     451,    -1,    -1,    -1,    -1,    -1,    -1,  1129,    -1,    -1,
      -1,    -1,    -1,  1135,  1136,  1137,  1138,  1139,  1140,  1141,
    1142,    -1,    -1,   720,    -1,   722,   477,    -1,    -1,   563,
    1069,    -1,   483,   567,    -1,  1074,    -1,    -1,    -1,  1161,
      -1,   575,    -1,   494,    -1,   496,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   190,  1145,  1146,    -1,  1190,  1149,
    1150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,  1206,  1207,  1208,  1209,    -1,   716,
      -1,    -1,    -1,    -1,    -1,  1217,  1218,    -1,    -1,  1179,
      -1,  1181,    -1,    -1,    -1,    -1,  1228,    -1,    -1,    88,
    1232,  1233,   563,    92,    -1,    -1,   567,  1156,  1240,  1241,
    1242,  1243,  1244,  1245,   575,    -1,    -1,    -1,    -1,    -1,
      -1,   758,    -1,    -1,   761,    -1,  1258,    -1,    -1,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,   274,    -1,
      -1,    -1,    -1,    -1,    -1,   281,    -1,    -1,    -1,    -1,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,    -1,
      -1,    -1,   151,   258,   259,   260,   261,   262,   263,  1218,
     159,    -1,    -1,   810,    -1,    -1,   165,   166,   167,   816,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   284,
      -1,    -1,   151,   900,   901,   290,   332,    -1,    -1,   158,
     159,    -1,    -1,   162,    -1,    -1,   165,   166,   167,  1258,
      -1,   347,    -1,   308,   758,     3,    -1,   761,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   321,   322,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
       2,    -1,    50,   900,   901,    53,   810,    -1,    -1,    -1,
      12,    -1,   816,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    69,    70,    25,    -1,   922,    28,   924,    -1,    77,
      -1,    -1,   929,    -1,    -1,   932,    -1,   758,    86,    -1,
     761,    -1,    -1,    -1,   941,    -1,   943,    95,    96,   946,
      98,    -1,   100,    -1,   102,   451,    -1,   105,    -1,    -1,
      -1,   109,   110,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,   970,    -1,    77,    -1,    -1,    -1,    81,
      82,   477,    -1,   980,    -1,   982,    -1,   483,    -1,   810,
      -1,    -1,    -1,    -1,    -1,   816,    -1,    -1,   494,   454,
     496,    -1,   104,    -1,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   922,    -1,
     924,    -1,    -1,   171,    -1,   929,    -1,   482,   932,    -1,
      -1,   486,    -1,   488,    -1,    -1,   491,   941,    -1,   943,
       3,    -1,   946,    -1,    -1,  1042,    -1,    -1,    -1,    -1,
      -1,    -1,  1049,    -1,    17,    -1,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,  1062,   970,   563,    -1,    -1,
      -1,   567,    -1,    36,    -1,    -1,   980,    -1,   982,   575,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    -1,   190,     2,
      53,    -1,    -1,    -1,    -1,    -1,  1093,    -1,    -1,    12,
      63,   922,    -1,   924,    -1,  1102,    -1,    70,   929,  1106,
      -1,   932,    25,    -1,    -1,    28,    -1,    -1,    -1,    -1,
     941,    -1,   943,    86,    -1,   946,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,  1042,   102,
      -1,    -1,    -1,    -1,    -1,  1049,   109,    -1,    -1,   970,
      -1,    -1,   115,    -1,  1151,    -1,    -1,    -1,  1062,   980,
      -1,   982,    -1,    -1,    77,    -1,    22,    -1,    81,    82,
      26,    27,   274,    -1,    -1,    31,    -1,    -1,    -1,   281,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,  1093,
      -1,   104,    -1,    -1,    -1,    51,   109,    -1,  1102,    -1,
      -1,    -1,  1106,    -1,    -1,    -1,    -1,    -1,    -1,  1206,
    1207,  1208,  1209,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      76,  1042,    78,    -1,    80,    -1,    82,    -1,  1049,    85,
     332,    87,    -1,    89,    -1,  1232,  1233,    -1,    -1,    -1,
      -1,  1062,    -1,    -1,    -1,   347,    -1,  1151,    -1,    -1,
      -1,    -1,   108,    -1,    -1,   111,    -1,    -1,   114,    -1,
      -1,    -1,   758,    -1,    -1,   761,    -1,    -1,    -1,    -1,
      -1,    -1,  1093,    -1,    -1,    -1,    -1,   190,    -1,    -1,
      -1,  1102,    -1,    -1,    -1,  1106,    -1,   742,   743,    -1,
      -1,    -1,    -1,   748,   749,    -1,    -1,   752,   753,    -1,
      -1,    -1,  1206,  1207,  1208,  1209,    -1,    -1,   164,    -1,
      -1,    -1,    -1,    -1,   810,    -1,    -1,    -1,    -1,    -1,
     816,    -1,    -1,     3,    -1,    -1,    -1,    -1,  1232,  1233,
    1151,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,   451,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,   274,    -1,    -1,    -1,    -1,    46,    -1,   281,    -1,
      50,    -1,    -1,    53,    -1,   477,    -1,    -1,    -1,    -1,
      -1,   483,    -1,    63,    -1,  1206,  1207,  1208,  1209,    69,
      70,    -1,   494,    -1,   496,    -1,    -1,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,  1232,  1233,    -1,     2,    95,    96,    -1,    98,   332,
     100,    -1,   102,    -1,    12,   105,   922,    -1,   924,   109,
      -1,    -1,   112,   929,   347,   115,   932,    25,    -1,    -1,
      28,    -1,    -1,    -1,    -1,   941,    -1,   943,    -1,     2,
     946,    -1,    -1,    -1,    -1,    -1,    -1,   912,   913,    12,
      -1,   563,    -1,    -1,   919,   567,    -1,    -1,    -1,    -1,
      -1,    -1,    25,   575,   970,    28,   931,    -1,    -1,   934,
      -1,   936,    -1,   938,   980,    -1,   982,    -1,    -1,    77,
      -1,   171,    -1,    81,    82,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   104,    -1,    -1,    -1,
      -1,   109,    -1,    -1,    77,    -1,    22,    -1,    81,    82,
      26,    27,    -1,    -1,    -1,    31,    -1,    -1,   451,    -1,
      36,    -1,    38,    39,    -1,    -1,  1042,    -1,    44,    -1,
      -1,   104,    -1,  1049,    -1,    51,   109,    -1,    54,    -1,
      -1,    -1,    -1,    -1,   477,    -1,  1062,    -1,    -1,    -1,
     483,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      76,   494,    78,   496,    80,    -1,    82,    83,    -1,    85,
       2,    87,    -1,    89,    -1,    -1,    -1,  1093,    -1,    -1,
      12,    -1,   190,    -1,    -1,    -1,  1102,    -1,    -1,    -1,
    1106,    -1,   108,    25,    -1,   111,    28,    -1,   114,    -1,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,    -1,    -1,   190,    -1,    -1,
    1095,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     563,   155,    -1,    -1,   567,  1151,   758,    -1,    -1,   761,
      -1,    -1,   575,    -1,   168,    77,    -1,   171,   164,   173,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,
      -1,    -1,   104,   281,    -1,    -1,    -1,   109,    -1,    -1,
      -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,   810,    -1,
    1206,  1207,  1208,  1209,   816,    -1,    -1,    17,    18,    -1,
      20,   274,    -1,    -1,    -1,    25,    -1,    -1,   281,    -1,
      -1,    -1,    -1,    -1,    34,    -1,  1232,  1233,    -1,    -1,
      40,    -1,    -1,    -1,   332,    -1,    46,    -1,     2,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    12,   347,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    69,
      70,    25,    -1,    -1,    28,    -1,    -1,    77,   190,   332,
      -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   347,    95,    96,    -1,    98,    -1,
     100,    -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
     922,    -1,   924,    77,    -1,    -1,    -1,   929,    -1,    -1,
     932,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   941,
      -1,   943,    49,    -1,   946,   758,    -1,    -1,   761,    -1,
     104,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,    -1,
      -1,    -1,   274,   451,    71,    -1,    -1,    -1,   970,   281,
      -1,   171,    -1,    -1,    81,    82,    -1,    -1,   980,    -1,
     982,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   477,
      -1,    -1,    -1,    -1,    -1,   483,    22,   810,   451,    -1,
      26,    27,    -1,   816,    -1,    31,   494,    -1,   496,    -1,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
     332,    -1,    -1,    -1,   477,    51,    -1,    -1,    -1,    -1,
     483,    -1,    -1,    -1,    -1,   347,   190,    -1,    -1,    -1,
    1042,   494,    -1,   496,    -1,    -1,    -1,  1049,    -1,    -1,
      76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    85,
    1062,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   563,    -1,    -1,    -1,   567,
      -1,    -1,   108,    -1,    -1,   111,    -1,   575,   114,    -1,
      -1,  1093,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1102,    -1,    -1,    -1,  1106,    -1,    -1,    -1,   215,   922,
     563,   924,    -1,    -1,   567,    -1,   929,    -1,    -1,   932,
     274,    -1,   575,    -1,    -1,    -1,    -1,   281,   941,    -1,
     943,    -1,    -1,   946,    -1,    -1,    -1,    -1,   164,   451,
      -1,    -1,    -1,    -1,   170,    -1,    -1,    -1,    -1,  1151,
      -1,   258,   259,   260,   261,   262,   263,   970,    -1,   266,
     267,    -1,    -1,    -1,    -1,   477,   273,   980,    -1,   982,
      -1,   483,    -1,    -1,    -1,    -1,    -1,   284,   332,    -1,
      -1,    -1,   494,   290,   496,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   347,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   308,    -1,    -1,  1206,  1207,  1208,  1209,    -1,    -1,
      -1,    -1,    -1,    -1,   321,   322,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,  1042,
    1232,  1233,    -1,    -1,    -1,    -1,  1049,    36,    -1,    38,
      39,    -1,    -1,    -1,     3,    44,    -1,    -1,    -1,  1062,
      -1,   563,    51,    -1,    -1,   567,    -1,    -1,    17,    18,
      -1,    20,    -1,   575,    -1,    -1,    25,    -1,    -1,    -1,
     758,    -1,    -1,   761,    -1,    34,    -1,    76,    -1,    78,
    1093,    40,    -1,    -1,    -1,    -1,    30,    46,    87,  1102,
      89,    50,    -1,  1106,    53,    -1,    -1,   451,    -1,    -1,
      44,    45,    46,    -1,    63,   758,    -1,    -1,   761,   108,
      69,    70,   111,    -1,    -1,   114,    -1,    -1,    77,    -1,
      -1,    -1,   810,   477,    -1,    -1,    -1,    86,   816,   483,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,  1151,    98,
     494,   100,    -1,   102,    -1,    -1,   105,   454,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,   810,    -1,    -1,
      -1,    -1,    -1,   816,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   482,    -1,    -1,    -1,   486,
      -1,   488,    -1,    -1,   491,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1206,  1207,  1208,  1209,    -1,    -1,    -1,
      -1,    -1,    -1,   147,    -1,   149,    -1,    -1,    -1,   563,
     169,    -1,   171,   567,   158,    -1,    -1,    -1,    -1,  1232,
    1233,   575,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   922,    -1,   924,    -1,    -1,    -1,
      -1,   929,    -1,    -1,   932,    -1,   758,    -1,    -1,   761,
      -1,    -1,    -1,   941,    -1,   943,   200,    -1,   946,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   922,
      -1,   924,    -1,    -1,    -1,    -1,   929,    -1,    -1,   932,
      -1,    -1,   970,    -1,    -1,    -1,    -1,    -1,   941,    -1,
     943,    -1,   980,   946,   982,    -1,    -1,    -1,   810,    -1,
      -1,    -1,    22,    -1,   816,    -1,    26,    27,    -1,    -1,
      -1,    31,   256,    -1,    -1,    -1,    36,   970,    38,    39,
     264,   265,    -1,    -1,    44,    -1,    -1,   980,    -1,   982,
      22,    51,    -1,   277,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,    -1,
      -1,    -1,    44,    -1,  1042,    -1,    76,    -1,    78,    51,
      80,  1049,    82,    -1,    -1,    85,    -1,    87,    -1,    89,
      -1,    -1,    -1,    65,  1062,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    76,    -1,    78,    -1,   108,  1042,
      82,   111,    84,    85,   114,    87,  1049,    89,    -1,    -1,
      -1,    -1,    -1,    -1,   758,  1093,    -1,   761,    -1,  1062,
     922,    -1,   924,    -1,  1102,   359,   108,   929,  1106,   111,
     932,    -1,   114,    -1,    -1,    -1,    -1,    -1,    -1,   941,
      -1,   943,    -1,    -1,   946,   742,   743,    -1,   382,    -1,
    1093,   748,   749,    -1,   164,   752,   753,    -1,    -1,  1102,
     170,    -1,    -1,  1106,    -1,   762,   810,    -1,   970,    -1,
      -1,    -1,   816,  1151,    -1,    -1,    -1,    -1,   980,    -1,
     982,    22,   164,    -1,    -1,    26,    27,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,    -1,
      -1,    -1,    -1,    44,   801,    -1,    -1,    -1,  1151,    -1,
      51,    -1,    -1,    -1,    -1,    -1,   450,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   461,  1206,  1207,
    1208,  1209,    -1,    -1,    -1,    76,    -1,    78,    -1,    80,
    1042,    82,    -1,    -1,    85,    -1,    87,  1049,    89,    -1,
      -1,    -1,    -1,    -1,  1232,  1233,    -1,    -1,    -1,    -1,
    1062,    -1,    -1,  1206,  1207,  1208,  1209,   108,    -1,   503,
     111,    -1,    -1,   114,    -1,    -1,    -1,    -1,   922,    -1,
     924,    -1,    -1,   517,    -1,   929,    -1,   521,   932,  1232,
    1233,  1093,    -1,    -1,    -1,    -1,    -1,   941,    -1,   943,
    1102,    -1,   946,    -1,  1106,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   912,   913,    -1,    -1,    -1,
      -1,    -1,   919,   164,   921,    -1,   970,    -1,    -1,   170,
      -1,   928,    -1,    -1,   931,    -1,   980,   934,   982,   936,
       1,   938,     3,    -1,    -1,    -1,    12,   581,    -1,  1151,
      -1,    -1,    -1,    19,   951,    -1,    17,    18,    -1,    20,
     594,    -1,    -1,    -1,    25,    -1,    32,    33,    -1,    -1,
      31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,   978,    48,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,  1206,  1207,  1208,  1209,    69,    70,
      -1,    -1,    32,    -1,    -1,    -1,    77,    -1,  1062,    -1,
      -1,    -1,    88,    -1,   658,    86,    92,    -1,    -1,    -1,
    1232,  1233,    -1,    -1,    95,    96,    -1,    98,    -1,   100,
      -1,   102,    -1,    -1,   105,    -1,    -1,    -1,   109,  1093,
      -1,   112,    -1,   119,   115,    -1,    -1,   123,  1102,    -1,
      -1,    -1,  1106,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    92,   139,   140,    -1,   142,   143,   144,    -1,
     146,   147,   148,    -1,   150,   151,   152,   153,   154,    -1,
     156,   157,   158,   159,   160,   161,   162,    -1,  1095,   165,
     166,   167,    -1,   169,    -1,    -1,    -1,  1151,    -1,    -1,
     176,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,   763,
      -1,   151,    -1,    -1,    -1,    -1,    -1,   771,   158,   159,
     774,    -1,   162,    -1,   778,   165,   166,   167,  1145,  1146,
      -1,    -1,  1149,  1150,    -1,     3,   176,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,   801,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
    1177,    -1,  1179,    -1,  1181,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
     854,    69,    70,   857,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,   868,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     0,     1,    -1,     3,    -1,     5,
       6,     7,     8,     9,    10,    -1,   950,    -1,    14,    15,
      16,    17,    18,   171,    20,   173,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    33,    34,    -1,
      36,   975,    38,    39,    40,    41,    42,    43,    44,    45,
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
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    -1,    69,    70,    71,
      -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,    -1,
     102,   103,    -1,   105,   106,   107,   108,   109,    -1,   111,
     112,   113,   114,   115,    -1,    -1,   118,    -1,   120,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,    -1,   169,   170,   171,
      -1,   173,    -1,     1,   176,     3,     4,     5,     6,     7,
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
     174,     1,   176,     3,     4,     5,     6,     7,     8,     9,
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
       8,     9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,
      18,    19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    64,    -1,    66,    67,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,    -1,   102,    -1,    -1,   105,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,   132,   133,   134,   135,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,    -1,   146,    -1,
     148,    -1,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,   165,   166,   167,
     168,   169,    -1,   171,    -1,   173,    -1,     3,   176,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      -1,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    64,    -1,
      66,    67,    -1,    69,    70,    71,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    93,    94,    95,
      96,    97,    98,    99,   100,    -1,   102,   103,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,   118,    -1,    -1,    -1,    -1,   123,    -1,    -1,
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
     102,    -1,    -1,   105,   106,   107,    -1,   109,   110,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,   141,
     142,   143,    -1,    -1,    -1,    -1,   148,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   158,    -1,    -1,   161,
     162,   163,    -1,    -1,    -1,   167,    -1,   169,    -1,   171,
      -1,   173,    -1,     3,   176,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    26,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    64,    -1,    66,    67,    -1,    69,
      70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,
      -1,    81,    -1,    -1,    -1,    -1,    86,    87,    -1,    89,
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
     174,     3,   176,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      -1,    63,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      92,    -1,    -1,    95,    96,    97,    98,    99,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
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
      -1,   109,    -1,   111,   112,    -1,    -1,   115,    -1,    -1,
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
      -1,   167,    -1,   169,    -1,   171,    -1,   173,   174,     3,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,     3,    -1,    -1,    -1,   139,   140,   141,   142,   143,
      12,    -1,    -1,    -1,   148,    17,    18,    19,    20,    -1,
      -1,    -1,    -1,    25,   158,    -1,    -1,   161,   162,   163,
      -1,    -1,    34,   167,    36,    -1,    -1,   171,    40,   173,
      -1,    -1,   176,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    -1,    98,    -1,   100,    -1,
     102,    -1,    -1,   105,    -1,    -1,    -1,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,    -1,    -1,
     132,   133,   134,   135,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,    -1,   146,    -1,    -1,    -1,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,    -1,
     162,    -1,    -1,   165,   166,   167,   168,    -1,     3,   171,
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
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,   140,   141,   142,   143,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   158,    -1,    -1,    -1,   162,   163,    -1,
      -1,    -1,   167,    -1,   169,    -1,   171,     3,   173,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    61,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    91,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,   104,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,   117,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,   163,   164,    25,
      -1,    -1,    -1,   169,    -1,   171,    -1,   173,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,    -1,   102,    -1,    -1,   105,
      -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,   145,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   163,    -1,    -1,
      -1,    -1,    -1,   169,    -1,   171,   172,   173,     3,    -1,
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
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   139,    -1,   141,    -1,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,   163,    32,
      25,    -1,    -1,    -1,   169,    -1,   171,    -1,   173,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    64,
      -1,    66,    67,    -1,    69,    70,    -1,    -1,     3,    -1,
      -1,    -1,    77,    -1,    79,    88,    81,    12,    -1,    92,
      -1,    86,    87,    -1,    19,    20,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,    -1,   102,    -1,    34,
     105,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,   139,   140,    63,   142,
     143,   144,    -1,   146,   147,   148,   141,   150,   151,   152,
     153,    -1,    -1,   156,   157,   158,   159,   160,    -1,   162,
      -1,    -1,   165,   166,   167,    -1,    -1,    -1,   163,    -1,
      -1,    -1,    -1,   176,   169,    -1,   171,   102,   173,    -1,
      -1,    -1,    -1,   108,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,    -1,    -1,   132,   133,   134,
     135,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
      -1,   146,    -1,    -1,    -1,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,     3,   162,    -1,    -1,
     165,   166,   167,   168,   169,    12,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    32,
      -1,   123,    -1,    -1,    -1,    -1,    88,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,    -1,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,   123,    -1,   165,   166,   167,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,   176,    88,    -1,   139,   140,    92,
     142,   143,   144,    -1,   146,   147,   148,    -1,   150,   151,
     152,   153,   154,    -1,   156,   157,   158,   159,   160,   161,
     162,    12,    -1,   165,   166,   167,    -1,    -1,    19,    -1,
     123,    -1,   174,    -1,   176,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,   139,   140,    88,   142,
     143,   144,    92,   146,   147,   148,    -1,   150,   151,   152,
     153,    12,    -1,   156,   157,   158,   159,   160,    19,   162,
      -1,    -1,   165,   166,   167,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,   176,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,   139,
     140,    92,   142,   143,   144,    -1,   146,   147,   148,    -1,
     101,   151,   152,   153,    -1,    -1,   156,   157,   158,   159,
      -1,    -1,   162,    -1,    -1,   165,   166,   167,    -1,    -1,
      -1,    -1,   123,    -1,    -1,    -1,   176,    88,    -1,    -1,
      -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,   139,   140,
     101,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,   123,    -1,   165,   166,   167,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,    -1,   150,
     151,   152,   153,   154,    -1,   156,   157,   158,   159,   160,
     161,   162,    12,    22,   165,   166,   167,    26,    27,    19,
      -1,    -1,    -1,    -1,    -1,   176,    -1,    36,    -1,    38,
      39,    -1,    32,    -1,    -1,    44,    -1,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    -1,    32,    82,    -1,    -1,    85,    -1,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,   108,
      -1,   101,   111,    -1,    -1,   114,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,    -1,    -1,    -1,    -1,    88,    -1,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,   139,
     140,   101,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,   164,   156,   157,   158,   159,
     160,   161,   162,   123,    -1,   165,   166,   167,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   176,    -1,    -1,   139,
     140,    -1,   142,   143,   144,    -1,   146,   147,   148,    -1,
     150,   151,   152,   153,   154,    -1,   156,   157,   158,   159,
     160,   161,   162,    12,     3,   165,   166,   167,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   176,    -1,    17,    18,
      -1,    20,    -1,    32,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,
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
     150,   151,   152,   153,   154,     3,   156,   157,   158,   159,
     160,   161,   162,    -1,    -1,   165,   166,   167,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,   176,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,     3,
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

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
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
     185,   186,   187,   190,   191,   192,   193,   195,   196,   197,
     202,   203,   206,   207,   211,   213,   216,   220,   223,   224,
     225,   226,   227,   228,   230,   231,   233,   235,   238,   239,
     240,   241,   242,   246,   247,   250,   251,   252,   255,   256,
     263,   264,   265,   266,   267,   269,   270,   295,   296,   300,
     301,   322,   323,   324,   325,   326,   327,   328,   336,   337,
     338,   339,   340,   343,   344,   345,   346,   347,   348,   349,
     350,   352,   353,   354,   355,   356,   164,   185,   340,   119,
     329,   330,     3,   208,    14,    22,    36,    41,    42,    45,
      56,    87,   100,   169,   173,   238,   263,   322,   327,   338,
     339,   340,   343,   345,   346,   329,   340,   108,   302,    89,
     208,   185,   316,   340,     8,   189,   185,   171,     3,    17,
      18,    20,    25,    34,    40,    46,    50,    53,    63,    69,
      70,    77,    86,    95,    96,    98,   100,   102,   105,   109,
     112,   115,   210,   212,    11,   108,    78,   121,   232,   340,
     232,   340,   232,   340,    27,   114,   234,   340,    82,    85,
     193,   171,   210,   210,   210,   171,   278,   171,   210,   303,
     304,    33,   197,   215,   340,   340,    18,    77,    95,   112,
     340,   340,   340,     8,   171,   222,   221,     4,   290,   315,
     340,   106,   107,   164,   340,   342,   340,   215,   340,   340,
     340,    99,   171,   185,   340,   340,   186,   197,   186,   197,
     340,   234,   340,   340,   340,   340,   340,   340,   340,     1,
     170,   183,   198,   315,   110,   149,   290,   317,   318,   342,
     232,   315,   340,   351,   340,    80,   185,   169,    47,    84,
     113,   194,    26,   301,   340,     8,   251,   340,   341,    56,
     144,   253,   210,     1,    31,   210,   257,   259,   262,    54,
      73,    83,   285,    27,    78,    89,   108,   286,    27,    78,
      89,   108,   284,   210,   297,   298,   303,   163,   164,   155,
     340,    12,    19,    32,    88,    92,   123,   139,   140,   142,
     143,   144,   146,   147,   148,   150,   151,   152,   153,   154,
     156,   157,   158,   159,   160,   161,   162,   165,   166,   167,
     176,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   168,   274,   171,   173,
      88,    92,   171,   185,   164,   340,   340,   340,   210,   315,
      56,   169,   197,    48,   329,   300,   164,   145,   164,   188,
      22,    36,    39,    87,   190,   193,   118,   211,   290,   319,
     320,   321,   342,   169,   210,   171,   215,    33,    48,   215,
     119,   215,   332,    33,    48,   215,   332,   215,   332,    48,
     215,   332,   210,   210,   101,   197,   101,   124,   197,   274,
     194,   319,   171,   171,   197,   185,    27,    48,    52,    75,
      78,    89,   108,   184,   279,   280,   281,   282,   283,   286,
     110,   171,   210,   305,   306,     1,   144,   310,    48,   145,
     185,   215,   171,   171,   215,   319,   223,   223,   145,   164,
     340,   340,   164,   169,   215,   171,   319,   164,   243,   243,
     215,   215,   164,   170,   170,   183,   145,   170,   340,   145,
     172,   145,   172,   174,   332,    48,   145,   174,   332,   122,
     145,   174,     8,     1,   170,   198,   204,   205,   340,   200,
     340,    65,    37,    72,   164,   251,   253,   229,   267,   210,
     315,   169,   170,     8,   261,   124,   145,   170,    89,     1,
     144,   309,    89,     1,     3,    12,    17,    19,    20,    25,
      40,    46,    53,    55,    63,    69,    70,    86,    98,   102,
     105,   109,   115,   139,   140,   141,   142,   143,   144,   146,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   162,   165,   166,   167,   168,   171,   209,   210,   212,
     271,   272,   273,   274,   322,   124,   299,   164,   145,   164,
     340,   340,   340,   340,   232,   340,   232,   340,   340,   340,
     340,   340,   340,   340,     3,    20,    34,    63,   102,   108,
     211,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,    68,   342,
     342,   342,   342,   342,   319,   319,   232,   340,   232,   340,
      27,    48,    89,   114,   331,   334,   335,   340,   356,    33,
      48,    33,    48,   101,   171,    48,   174,   210,   232,   340,
     215,   303,   340,   190,   340,   124,   172,   145,    48,   315,
      45,   340,   232,   340,   171,   215,    45,   340,   232,   340,
     215,   215,   232,   340,   215,   124,   124,   185,    35,   185,
     340,    35,   340,    65,   172,   320,   210,   236,   237,    48,
      89,   282,   145,   172,   171,   210,   310,   306,   145,   172,
      34,    50,    96,   100,   173,   214,   311,   323,   124,   307,
     340,   304,   340,   340,   172,   290,   340,     1,   248,   321,
     172,    21,   244,   170,   172,   172,   317,   172,   317,   185,
     174,   232,   340,   174,   185,   340,   174,   340,   174,   340,
     170,   170,   145,   164,    13,   147,   145,   164,    13,    37,
      72,   210,   139,   140,   141,   142,   143,   158,   162,   167,
     199,   273,   274,   275,   340,   199,   201,   253,   169,   300,
     164,   171,     1,   254,   260,   262,   340,   258,   173,   214,
     308,   323,   104,   287,   171,   277,   340,   139,   147,   277,
     277,   311,   323,   297,   171,   173,   164,   164,   164,   164,
     164,   164,   172,   174,    48,    89,   145,   172,    17,    20,
      25,    46,    53,    63,    70,    86,    98,   109,   115,   322,
      88,    88,    45,   232,   340,    45,   232,   340,   320,   232,
     340,   171,   329,   329,   164,   164,   290,   342,   321,   340,
     172,   340,    33,   215,    33,   215,   333,   334,   340,    33,
     215,   332,    33,   215,   332,   215,   332,   215,   332,   340,
     340,    35,   185,    35,    35,   185,   101,   197,   210,   172,
     145,   172,   210,   281,   306,   144,   314,    61,   117,   291,
     172,   305,   310,     1,   315,    68,   342,   172,   172,   170,
      74,   116,   170,   249,   172,   171,   197,   210,   245,   185,
     174,   332,   174,   332,   185,   122,   204,   211,   169,   275,
     340,   110,   340,   199,   201,   145,   164,    13,   164,   169,
     254,   303,   320,   170,    31,    82,    85,   170,   184,   217,
     220,   262,   259,     1,   174,   315,   291,   280,   172,     3,
     102,   272,   274,   172,   174,   334,   310,   322,   322,   340,
      33,    33,   340,    33,    33,   172,   174,   174,   320,   215,
     215,   215,   101,    45,   340,    45,   340,   145,   172,   101,
      45,   340,   215,    45,   340,   215,   215,   215,   185,   185,
     340,   185,    35,   164,   164,   237,   197,   314,   172,   173,
     214,   290,   313,   323,   149,   276,   307,     3,    91,   102,
     292,   293,   294,   340,   196,   216,   289,   307,   174,    48,
     174,   171,   171,    33,   185,   315,   245,   144,   197,    45,
     185,   340,   174,    45,   185,   340,   174,   340,   199,    13,
      37,    72,    37,    72,   164,   164,   275,   340,   340,   254,
     170,   164,   172,     8,   219,   217,   174,   308,   323,   174,
     268,   172,   277,   277,   307,   101,    45,    45,   101,    45,
      45,    45,    45,   172,   340,   340,   340,   334,   340,   340,
     340,    35,   185,   276,   307,   314,   174,   315,   290,   340,
     294,   117,   145,   124,   150,   152,   153,   156,   157,    61,
     340,   311,   323,   319,   319,   185,   215,   172,   340,   340,
     185,   340,   185,   170,   110,   340,   199,   201,   199,   201,
      13,   170,   164,   218,   220,   308,   323,   164,   288,   289,
     340,   340,   340,   340,   340,   340,   101,   101,   101,   101,
     185,   276,   307,   290,   312,   313,   323,    48,   174,   340,
     293,   294,   294,   294,   294,   294,   294,   292,   174,   172,
     172,   197,   101,   101,   164,   164,   164,   164,   340,   220,
     101,   101,   101,   101,   101,   101,   340,   340,   340,   340,
     340,   312,   313,   323,   163,   163,   340,   340,   174,   312
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   180,   181,   182,   182,   183,   183,   184,   184,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   185,   185,
     185,   185,   185,   185,   185,   185,   185,   185,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   187,   188,   187,   189,   187,   190,   190,
     191,   192,   192,   192,   193,   193,   193,   194,   194,   195,
     196,   196,   196,   197,   198,   198,   199,   199,   199,   199,
     199,   199,   200,   200,   200,   200,   200,   200,   201,   201,
     202,   202,   202,   202,   202,   202,   202,   203,   204,   204,
     204,   204,   205,   205,   206,   207,   207,   207,   207,   207,
     207,   208,   208,   209,   209,   209,   209,   209,   209,   210,
     210,   210,   210,   210,   210,   211,   211,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   214,   214,
     214,   214,   215,   215,   216,   216,   217,   218,   217,   219,
     217,   220,   220,   221,   220,   222,   220,   223,   223,   223,
     223,   223,   223,   223,   224,   224,   224,   224,   225,   226,
     226,   227,   228,   228,   228,   229,   228,   230,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   231,   231,   231,   231,   231,   231,   231,
     231,   231,   231,   232,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   234,   234,   235,   235,
     236,   236,   237,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   239,   239,   239,   239,
     239,   239,   240,   240,   240,   241,   241,   241,   242,   242,
     242,   242,   243,   243,   244,   244,   244,   245,   245,   246,
     247,   247,   248,   248,   249,   249,   249,   250,   250,   251,
     252,   252,   252,   253,   253,   254,   254,   254,   255,   255,
     256,   257,   257,   258,   257,   259,   260,   259,   261,   259,
     262,   262,   263,   264,   265,   265,   265,   266,   268,   267,
     269,   269,   269,   269,   269,   270,   271,   271,   272,   272,
     272,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   273,   273,   273,
     273,   273,   273,   273,   273,   273,   273,   274,   274,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   275,
     275,   276,   276,   276,   277,   277,   278,   279,   279,   280,
     280,   281,   281,   281,   281,   281,   281,   282,   282,   283,
     283,   283,   283,   283,   283,   283,   283,   283,   284,   284,
     284,   284,   284,   284,   285,   285,   285,   286,   286,   286,
     286,   286,   286,   287,   287,   288,   288,   289,   289,   290,
     291,   291,   291,   291,   291,   292,   292,   293,   293,   293,
     293,   293,   293,   293,   294,   294,   295,   296,   296,   296,
     297,   297,   298,   299,   299,   299,   300,   300,   300,   300,
     300,   302,   301,   301,   303,   303,   304,   304,   305,   305,
     305,   306,   306,   306,   307,   307,   307,   308,   308,   308,
     308,   308,   308,   308,   309,   309,   309,   309,   309,   310,
     310,   310,   310,   310,   311,   311,   311,   311,   312,   312,
     312,   313,   313,   313,   313,   313,   314,   314,   314,   314,
     314,   315,   315,   315,   315,   316,   316,   317,   317,   317,
     318,   318,   319,   319,   320,   320,   321,   321,   321,   321,
     322,   322,   323,   323,   323,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   324,   324,   324,
     324,   324,   324,   324,   324,   324,   324,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   326,   327,   328,   328,   328,   328,
     328,   328,   328,   328,   329,   329,   330,   331,   331,   332,
     333,   333,   334,   334,   334,   335,   335,   335,   335,   335,
     335,   336,   336,   337,   337,   337,   337,   337,   338,   338,
     338,   338,   338,   339,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   341,   341,   342,   342,   342,   343,   343,   343,   343,
     344,   344,   344,   344,   344,   345,   345,   345,   346,   346,
     346,   346,   346,   346,   347,   347,   347,   347,   348,   348,
     349,   349,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   351,   351,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     353,   353,   353,   353,   353,   353,   353,   354,   354,   354,
     354,   355,   355,   355,   355,   356,   356,   356,   356,   356,
     356,   356
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     2,     3,     3,     3,     3,     2,     1,     1,
       1,     1,     2,     1,     3,     3,     3,     3,     2,     3,
       3,     2,     2,     1,     0,     4,     0,     3,     1,     1,
       4,     3,     4,     4,     0,     1,     1,     0,     1,     6,
       2,     3,     3,     1,     1,     2,     1,     1,     3,     3,
       3,     5,     1,     3,     3,     3,     5,     5,     0,     1,
       4,     6,     8,     8,     6,     8,     8,     4,     1,     3,
       3,     5,     1,     3,     3,     4,     4,     4,     4,     4,
       4,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     3,     1,     0,     4,     0,
       3,     1,     1,     0,     3,     0,     3,     1,     1,     1,
       1,     1,     1,     1,     3,     5,     5,     2,     1,     1,
       1,     1,     6,     7,     3,     0,     6,     2,     5,     3,
       3,     6,     6,     4,     5,     5,     3,     3,     6,     5,
       6,     5,     6,     3,     4,     3,     4,     5,     6,     5,
       6,     3,     4,     3,     4,     6,     7,     6,     7,     4,
       5,     4,     5,     4,     4,     3,     6,     5,     4,     3,
       6,     5,     6,     5,     8,     7,     4,     4,     6,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     4,     7,     5,     3,     6,     2,     2,
       3,     3,     0,     2,     2,     3,     5,     1,     3,     3,
       5,     5,     0,     2,     3,     2,     3,     5,     5,     2,
       1,     1,     1,     0,     2,     0,     2,     3,     3,     3,
       3,     1,     2,     0,     4,     1,     0,     4,     0,     3,
       1,     3,     6,     0,     1,     1,     1,     1,     0,     7,
       4,     4,     6,     6,     4,     2,     1,     3,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     2,     0,     3,     3,     1,     3,     0,
       1,     4,     5,     4,     5,     6,     6,     0,     1,     1,
       1,     1,     2,     2,     1,     1,     1,     1,     0,     1,
       1,     2,     1,     1,     1,     1,     1,     0,     1,     2,
       1,     1,     1,     0,     1,     1,     1,     1,     1,     1,
       0,     2,     2,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     3,     1,     2,     2,
       1,     3,     2,     0,     2,     2,     1,     2,     1,     1,
       1,     0,     5,     3,     1,     3,     3,     5,     1,     1,
       3,     1,     2,     3,     0,     2,     2,     3,     2,     4,
       3,     3,     4,     3,     0,     2,     2,     2,     1,     0,
       2,     2,     2,     1,     4,     4,     6,     3,     0,     1,
       1,     3,     4,     3,     4,     6,     0,     2,     2,     2,
       2,     1,     1,     3,     3,     1,     3,     1,     1,     1,
       3,     3,     0,     1,     1,     3,     3,     3,     1,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       2,     4,     4,     4,     5,     2,     2,     1,     2,     1,
       2,     1,     2,     1,     2,     1,     1,     6,     6,     4,
       9,     9,     7,     6,     6,     4,     9,     9,     7,     4,
       6,     6,     9,     9,     6,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     0,     1,     4,     1,     3,     4,
       1,     3,     4,     3,     3,     1,     1,     2,     1,     2,
       1,     1,     3,     1,     2,     2,     2,     2,     2,     8,
       8,     9,     9,     4,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     3,     2,     2,     2,
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
#line 639 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6361 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 643 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6367 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 644 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6373 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6381 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6389 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 663 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6398 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 671 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6404 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: include_module_stmt  */
#line 672 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6410 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: block_stmt  */
#line 673 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6416 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: use_stmt  */
#line 674 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6422 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: import_stmt  */
#line 675 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6428 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: require_stmt  */
#line 676 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6434 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 677 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6440 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: implements_stmt  */
#line 678 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6446 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: interface_stmt  */
#line 679 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6452 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: TDEFER stmt  */
#line 681 "chpl.ypp"
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
#line 6469 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: try_stmt  */
#line 693 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6475 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: return_stmt  */
#line 694 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6481 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: TATOMIC stmt  */
#line 695 "chpl.ypp"
                            { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6487 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 697 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6500 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 706 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6513 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TLABEL ident_def stmt  */
#line 715 "chpl.ypp"
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
#line 6540 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: TYIELD expr TSEMI  */
#line 738 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6551 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: error TSEMI  */
#line 745 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
  }
#line 6559 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: assignment_stmt  */
#line 751 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6565 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: if_stmt  */
#line 752 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6571 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: loop_stmt  */
#line 753 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6577 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: select_stmt  */
#line 754 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6583 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 755 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6589 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: throw_stmt  */
#line 756 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6595 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 758 "chpl.ypp"
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
#line 6613 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 772 "chpl.ypp"
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
#line 6632 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 787 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6644 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 795 "chpl.ypp"
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
#line 6660 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL do_stmt  */
#line 807 "chpl.ypp"
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
#line 6675 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TON expr do_stmt  */
#line 818 "chpl.ypp"
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
#line 6691 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 830 "chpl.ypp"
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
#line 6707 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL do_stmt  */
#line 842 "chpl.ypp"
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
#line 6722 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSYNC stmt  */
#line 853 "chpl.ypp"
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
#line 6739 "bison-chpl-lib.cpp"
    break;

  case 44: /* $@1: %empty  */
#line 870 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6747 "bison-chpl-lib.cpp"
    break;

  case 45: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 deprecated_decl_base  */
#line 874 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6755 "bison-chpl-lib.cpp"
    break;

  case 46: /* $@2: %empty  */
#line 878 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6763 "bison-chpl-lib.cpp"
    break;

  case 47: /* deprecated_decl_stmt: TDEPRECATED $@2 deprecated_decl_base  */
#line 882 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6771 "bison-chpl-lib.cpp"
    break;

  case 50: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 894 "chpl.ypp"
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
#line 6792 "bison-chpl-lib.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 914 "chpl.ypp"
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
#line 6811 "bison-chpl-lib.cpp"
    break;

  case 52: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 929 "chpl.ypp"
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
#line 6830 "bison-chpl-lib.cpp"
    break;

  case 53: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 944 "chpl.ypp"
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
#line 6847 "bison-chpl-lib.cpp"
    break;

  case 54: /* access_control: %empty  */
#line 959 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6853 "bison-chpl-lib.cpp"
    break;

  case 55: /* access_control: TPUBLIC  */
#line 960 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6860 "bison-chpl-lib.cpp"
    break;

  case 56: /* access_control: TPRIVATE  */
#line 962 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6867 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_prototype: %empty  */
#line 967 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6873 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_prototype: TPROTOTYPE  */
#line 968 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6880 "bison-chpl-lib.cpp"
    break;

  case 59: /* include_module_stmt: TINCLUDE access_control opt_prototype TMODULE ident_def TSEMI  */
#line 974 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6890 "bison-chpl-lib.cpp"
    break;

  case 60: /* block_stmt_body: TLCBR TRCBR  */
#line 992 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6898 "bison-chpl-lib.cpp"
    break;

  case 61: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 996 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6906 "bison-chpl-lib.cpp"
    break;

  case 62: /* block_stmt_body: TLCBR error TRCBR  */
#line 1000 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6918 "bison-chpl-lib.cpp"
    break;

  case 63: /* block_stmt: block_stmt_body  */
#line 1011 "chpl.ypp"
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
#line 6937 "bison-chpl-lib.cpp"
    break;

  case 64: /* stmt_ls: toplevel_stmt  */
#line 1029 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6943 "bison-chpl-lib.cpp"
    break;

  case 65: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1030 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6949 "bison-chpl-lib.cpp"
    break;

  case 66: /* renames_ls: expr  */
#line 1035 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6957 "bison-chpl-lib.cpp"
    break;

  case 67: /* renames_ls: all_op_name  */
#line 1039 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6965 "bison-chpl-lib.cpp"
    break;

  case 68: /* renames_ls: expr TAS expr  */
#line 1043 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6974 "bison-chpl-lib.cpp"
    break;

  case 69: /* renames_ls: renames_ls TCOMMA expr  */
#line 1048 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6982 "bison-chpl-lib.cpp"
    break;

  case 70: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1052 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6990 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1056 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 72: /* use_renames_ls: expr  */
#line 1066 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(node);
  }
#line 7008 "bison-chpl-lib.cpp"
    break;

  case 73: /* use_renames_ls: expr TAS expr  */
#line 1071 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7018 "bison-chpl-lib.cpp"
    break;

  case 74: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1077 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7029 "bison-chpl-lib.cpp"
    break;

  case 75: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1084 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7038 "bison-chpl-lib.cpp"
    break;

  case 76: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1089 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7049 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1096 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7061 "bison-chpl-lib.cpp"
    break;

  case 78: /* opt_only_ls: %empty  */
#line 1106 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7067 "bison-chpl-lib.cpp"
    break;

  case 79: /* opt_only_ls: renames_ls  */
#line 1107 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7073 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_stmt: access_control TUSE use_renames_ls TSEMI  */
#line 1112 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7081 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1116 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7092 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1123 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7104 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1131 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7117 "bison-chpl-lib.cpp"
    break;

  case 84: /* use_stmt: access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1140 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7128 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1147 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7140 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1155 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7153 "bison-chpl-lib.cpp"
    break;

  case 87: /* import_stmt: access_control TIMPORT import_ls TSEMI  */
#line 1167 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7161 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_expr: expr  */
#line 1174 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
  }
#line 7169 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_expr: expr TDOT all_op_name  */
#line 1178 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-2].expr)),
                                        VisibilityClause::NONE,
                                        context->consume(ident));
  }
#line 7180 "bison-chpl-lib.cpp"
    break;

  case 90: /* import_expr: expr TAS ident_use  */
#line 1185 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as));
  }
#line 7190 "bison-chpl-lib.cpp"
    break;

  case 91: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1191 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)));
  }
#line 7200 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_ls: import_expr  */
#line 1199 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7206 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_ls: import_ls TCOMMA import_expr  */
#line 1200 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7212 "bison-chpl-lib.cpp"
    break;

  case 94: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1205 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7222 "bison-chpl-lib.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1214 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7230 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1218 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7238 "bison-chpl-lib.cpp"
    break;

  case 97: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1222 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7246 "bison-chpl-lib.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1226 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7254 "bison-chpl-lib.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1230 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7262 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1234 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7270 "bison-chpl-lib.cpp"
    break;

  case 101: /* opt_label_ident: %empty  */
#line 1242 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7276 "bison-chpl-lib.cpp"
    break;

  case 102: /* opt_label_ident: TIDENT  */
#line 1243 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7282 "bison-chpl-lib.cpp"
    break;

  case 103: /* ident_fn_def: TIDENT  */
#line 1247 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7288 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: TNONE  */
#line 1248 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7294 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_fn_def: TTHIS  */
#line 1249 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7300 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_fn_def: TFALSE  */
#line 1250 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7306 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_fn_def: TTRUE  */
#line 1251 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7312 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_fn_def: internal_type_ident_def  */
#line 1252 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7318 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_def: TIDENT  */
#line 1255 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7324 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: TNONE  */
#line 1256 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7330 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_def: TTHIS  */
#line 1257 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'this'"); }
#line 7336 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_def: TFALSE  */
#line 1258 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7342 "bison-chpl-lib.cpp"
    break;

  case 113: /* ident_def: TTRUE  */
#line 1259 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7348 "bison-chpl-lib.cpp"
    break;

  case 114: /* ident_def: internal_type_ident_def  */
#line 1260 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7354 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_use: TIDENT  */
#line 1272 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7360 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_use: TTHIS  */
#line 1273 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7366 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TBOOL  */
#line 1306 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7372 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TENUM  */
#line 1307 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7378 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TINT  */
#line 1308 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7384 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TUINT  */
#line 1309 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7390 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TREAL  */
#line 1310 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7396 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TIMAG  */
#line 1311 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7402 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TCOMPLEX  */
#line 1312 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7408 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TBYTES  */
#line 1313 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7414 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TSTRING  */
#line 1314 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TLOCALE  */
#line 1315 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7426 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TNOTHING  */
#line 1316 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7432 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TVOID  */
#line 1317 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7438 "bison-chpl-lib.cpp"
    break;

  case 152: /* do_stmt: TDO stmt  */
#line 1331 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7444 "bison-chpl-lib.cpp"
    break;

  case 153: /* do_stmt: block_stmt  */
#line 1332 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7450 "bison-chpl-lib.cpp"
    break;

  case 154: /* return_stmt: TRETURN TSEMI  */
#line 1337 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7461 "bison-chpl-lib.cpp"
    break;

  case 155: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1344 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7472 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@3: %empty  */
#line 1355 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7480 "bison-chpl-lib.cpp"
    break;

  case 158: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@3 class_level_stmt  */
#line 1359 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7488 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@4: %empty  */
#line 1363 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7496 "bison-chpl-lib.cpp"
    break;

  case 160: /* deprecated_class_level_stmt: TDEPRECATED $@4 class_level_stmt  */
#line 1367 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7504 "bison-chpl-lib.cpp"
    break;

  case 161: /* class_level_stmt: TSEMI  */
#line 1374 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), nullptr);
    }
#line 7512 "bison-chpl-lib.cpp"
    break;

  case 162: /* class_level_stmt: inner_class_level_stmt  */
#line 1378 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7522 "bison-chpl-lib.cpp"
    break;

  case 163: /* $@5: %empty  */
#line 1383 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7529 "bison-chpl-lib.cpp"
    break;

  case 164: /* class_level_stmt: TPUBLIC $@5 inner_class_level_stmt  */
#line 1385 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7538 "bison-chpl-lib.cpp"
    break;

  case 165: /* $@6: %empty  */
#line 1389 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7545 "bison-chpl-lib.cpp"
    break;

  case 166: /* class_level_stmt: TPRIVATE $@6 inner_class_level_stmt  */
#line 1391 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7554 "bison-chpl-lib.cpp"
    break;

  case 174: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1409 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7563 "bison-chpl-lib.cpp"
    break;

  case 175: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1414 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7572 "bison-chpl-lib.cpp"
    break;

  case 176: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1419 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7581 "bison-chpl-lib.cpp"
    break;

  case 177: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1424 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7589 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_start: TFORWARDING  */
#line 1431 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7598 "bison-chpl-lib.cpp"
    break;

  case 179: /* extern_or_export: TEXTERN  */
#line 1438 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7604 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_or_export: TEXPORT  */
#line 1439 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 181: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1444 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7620 "bison-chpl-lib.cpp"
    break;

  case 182: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1454 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7629 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1460 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7640 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1467 "chpl.ypp"
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
#line 7663 "bison-chpl-lib.cpp"
    break;

  case 185: /* $@7: %empty  */
#line 1486 "chpl.ypp"
  {
    // This will be consumed when building the first variable in the list.
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7672 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@7 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1491 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7681 "bison-chpl-lib.cpp"
    break;

  case 187: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1499 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7689 "bison-chpl-lib.cpp"
    break;

  case 188: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1506 "chpl.ypp"
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
#line 7711 "bison-chpl-lib.cpp"
    break;

  case 189: /* loop_stmt: TWHILE expr do_stmt  */
#line 1524 "chpl.ypp"
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
#line 7728 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1537 "chpl.ypp"
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
#line 7745 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1550 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7753 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1554 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7761 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1558 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7769 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1562 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7777 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1566 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7785 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFOR expr do_stmt  */
#line 1570 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7793 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1574 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7801 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1578 "chpl.ypp"
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
#line 7823 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1596 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7831 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1600 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7839 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1604 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7847 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1608 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7855 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFORALL expr do_stmt  */
#line 1612 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7863 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1616 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7871 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1620 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7879 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1624 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7887 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1628 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7895 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1632 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7903 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1636 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7911 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1640 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7919 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1644 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7927 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1648 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7935 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1652 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7943 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1656 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7951 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1660 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7959 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1664 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7967 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1668 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7975 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1672 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7983 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1676 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7991 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1680 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7999 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1684 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8008 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1689 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8017 "bison-chpl-lib.cpp"
    break;

  case 223: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1697 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8027 "bison-chpl-lib.cpp"
    break;

  case 224: /* if_stmt: TIF expr TTHEN stmt  */
#line 1706 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8035 "bison-chpl-lib.cpp"
    break;

  case 225: /* if_stmt: TIF expr block_stmt  */
#line 1710 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8043 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1714 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8051 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1718 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8059 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1722 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8067 "bison-chpl-lib.cpp"
    break;

  case 229: /* if_stmt: TIF ifvar block_stmt  */
#line 1726 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1730 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8083 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1734 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8091 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1738 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8100 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1743 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8109 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1748 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8118 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1753 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8127 "bison-chpl-lib.cpp"
    break;

  case 236: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1761 "chpl.ypp"
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
#line 8146 "bison-chpl-lib.cpp"
    break;

  case 237: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1776 "chpl.ypp"
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
#line 8165 "bison-chpl-lib.cpp"
    break;

  case 238: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1794 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8171 "bison-chpl-lib.cpp"
    break;

  case 239: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1796 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8177 "bison-chpl-lib.cpp"
    break;

  case 240: /* ifc_formal_ls: ifc_formal  */
#line 1800 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8183 "bison-chpl-lib.cpp"
    break;

  case 241: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1801 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8189 "bison-chpl-lib.cpp"
    break;

  case 242: /* ifc_formal: ident_def  */
#line 1806 "chpl.ypp"
             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8195 "bison-chpl-lib.cpp"
    break;

  case 255: /* implements_type_ident: implements_type_error_ident  */
#line 1824 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8207 "bison-chpl-lib.cpp"
    break;

  case 262: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1853 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 263: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1855 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8219 "bison-chpl-lib.cpp"
    break;

  case 264: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1857 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8225 "bison-chpl-lib.cpp"
    break;

  case 265: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1862 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 266: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1864 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8237 "bison-chpl-lib.cpp"
    break;

  case 267: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1866 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8243 "bison-chpl-lib.cpp"
    break;

  case 268: /* try_stmt: TTRY tryable_stmt  */
#line 1871 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8251 "bison-chpl-lib.cpp"
    break;

  case 269: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1875 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8259 "bison-chpl-lib.cpp"
    break;

  case 270: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1879 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8267 "bison-chpl-lib.cpp"
    break;

  case 271: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1883 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8275 "bison-chpl-lib.cpp"
    break;

  case 272: /* catch_expr_ls: %empty  */
#line 1889 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8281 "bison-chpl-lib.cpp"
    break;

  case 273: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1890 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8287 "bison-chpl-lib.cpp"
    break;

  case 274: /* catch_expr: TCATCH block_stmt  */
#line 1895 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8295 "bison-chpl-lib.cpp"
    break;

  case 275: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1899 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8303 "bison-chpl-lib.cpp"
    break;

  case 276: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1903 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8311 "bison-chpl-lib.cpp"
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
#line 8328 "bison-chpl-lib.cpp"
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
#line 8345 "bison-chpl-lib.cpp"
    break;

  case 279: /* throw_stmt: TTHROW expr TSEMI  */
#line 1939 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8355 "bison-chpl-lib.cpp"
    break;

  case 280: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1947 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8363 "bison-chpl-lib.cpp"
    break;

  case 281: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1951 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8373 "bison-chpl-lib.cpp"
    break;

  case 282: /* when_stmt_ls: %empty  */
#line 1959 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8379 "bison-chpl-lib.cpp"
    break;

  case 283: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1960 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8385 "bison-chpl-lib.cpp"
    break;

  case 284: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1965 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8393 "bison-chpl-lib.cpp"
    break;

  case 285: /* when_stmt: TOTHERWISE stmt  */
#line 1969 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8402 "bison-chpl-lib.cpp"
    break;

  case 286: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1974 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8411 "bison-chpl-lib.cpp"
    break;

  case 287: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1984 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8420 "bison-chpl-lib.cpp"
    break;

  case 288: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1989 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8431 "bison-chpl-lib.cpp"
    break;

  case 289: /* class_start: class_tag ident_def  */
#line 2000 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8439 "bison-chpl-lib.cpp"
    break;

  case 290: /* class_tag: TCLASS  */
#line 2006 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8445 "bison-chpl-lib.cpp"
    break;

  case 291: /* class_tag: TRECORD  */
#line 2007 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8451 "bison-chpl-lib.cpp"
    break;

  case 292: /* class_tag: TUNION  */
#line 2008 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8457 "bison-chpl-lib.cpp"
    break;

  case 293: /* opt_inherit: %empty  */
#line 2012 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8463 "bison-chpl-lib.cpp"
    break;

  case 294: /* opt_inherit: TCOLON expr_ls  */
#line 2013 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8469 "bison-chpl-lib.cpp"
    break;

  case 295: /* class_level_stmt_ls: %empty  */
#line 2017 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8478 "bison-chpl-lib.cpp"
    break;

  case 296: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2022 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8486 "bison-chpl-lib.cpp"
    break;

  case 297: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2026 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8494 "bison-chpl-lib.cpp"
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
#line 8515 "bison-chpl-lib.cpp"
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
#line 8529 "bison-chpl-lib.cpp"
    break;

  case 300: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2063 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8537 "bison-chpl-lib.cpp"
    break;

  case 301: /* enum_ls: deprecated_enum_item  */
#line 2070 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8546 "bison-chpl-lib.cpp"
    break;

  case 302: /* enum_ls: enum_ls TCOMMA  */
#line 2075 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8556 "bison-chpl-lib.cpp"
    break;

  case 303: /* $@8: %empty  */
#line 2081 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8565 "bison-chpl-lib.cpp"
    break;

  case 304: /* enum_ls: enum_ls TCOMMA $@8 deprecated_enum_item  */
#line 2086 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8574 "bison-chpl-lib.cpp"
    break;

  case 306: /* $@9: %empty  */
#line 2095 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8582 "bison-chpl-lib.cpp"
    break;

  case 307: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@9 enum_item  */
#line 2099 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 308: /* $@10: %empty  */
#line 2103 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8598 "bison-chpl-lib.cpp"
    break;

  case 309: /* deprecated_enum_item: TDEPRECATED $@10 enum_item  */
#line 2107 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8606 "bison-chpl-lib.cpp"
    break;

  case 310: /* enum_item: ident_def  */
#line 2114 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8617 "bison-chpl-lib.cpp"
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
#line 8630 "bison-chpl-lib.cpp"
    break;

  case 312: /* lambda_decl_expr: TLAMBDA req_formal_ls opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 2133 "chpl.ypp"
  {
    // TODO (dlongnecke): Leave this unimplemented for now? Since we will
    // be addressing FCFs in 1.26...
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 8640 "bison-chpl-lib.cpp"
    break;

  case 314: /* linkage_spec: linkage_spec_empty  */
#line 2144 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8646 "bison-chpl-lib.cpp"
    break;

  case 315: /* linkage_spec: TINLINE  */
#line 2145 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8653 "bison-chpl-lib.cpp"
    break;

  case 316: /* linkage_spec: TOVERRIDE  */
#line 2147 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8660 "bison-chpl-lib.cpp"
    break;

  case 317: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2153 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8668 "bison-chpl-lib.cpp"
    break;

  case 318: /* $@11: %empty  */
#line 2162 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name);
    }
#line 8678 "bison-chpl-lib.cpp"
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
#line 8699 "bison-chpl-lib.cpp"
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
#line 8711 "bison-chpl-lib.cpp"
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
#line 8723 "bison-chpl-lib.cpp"
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
#line 8738 "bison-chpl-lib.cpp"
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
#line 8753 "bison-chpl-lib.cpp"
    break;

  case 324: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2226 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8763 "bison-chpl-lib.cpp"
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
#line 8777 "bison-chpl-lib.cpp"
    break;

  case 327: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2248 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8783 "bison-chpl-lib.cpp"
    break;

  case 330: /* fn_ident: ident_def TBANG  */
#line 2255 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8793 "bison-chpl-lib.cpp"
    break;

  case 371: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2313 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8799 "bison-chpl-lib.cpp"
    break;

  case 372: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2314 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8805 "bison-chpl-lib.cpp"
    break;

  case 373: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2315 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8811 "bison-chpl-lib.cpp"
    break;

  case 374: /* opt_formal_ls: %empty  */
#line 2319 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8817 "bison-chpl-lib.cpp"
    break;

  case 375: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2320 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8823 "bison-chpl-lib.cpp"
    break;

  case 376: /* req_formal_ls: TLP formal_ls TRP  */
#line 2324 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8829 "bison-chpl-lib.cpp"
    break;

  case 377: /* formal_ls_inner: formal  */
#line 2328 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8835 "bison-chpl-lib.cpp"
    break;

  case 378: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2329 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8841 "bison-chpl-lib.cpp"
    break;

  case 379: /* formal_ls: %empty  */
#line 2333 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8847 "bison-chpl-lib.cpp"
    break;

  case 380: /* formal_ls: formal_ls_inner  */
#line 2334 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8853 "bison-chpl-lib.cpp"
    break;

  case 381: /* formal: opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2339 "chpl.ypp"
  {
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 8866 "bison-chpl-lib.cpp"
    break;

  case 382: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2348 "chpl.ypp"
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
#line 8881 "bison-chpl-lib.cpp"
    break;

  case 383: /* formal: opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2359 "chpl.ypp"
  {
    (yyval.expr) = VarArgFormal::build(BUILDER, LOC((yyloc)),
                             /*attributes*/ nullptr,
                             /*name*/ (yyvsp[-2].uniqueStr),
                             /*intent*/ (yyvsp[-3].intentTag),
                             toOwned((yyvsp[-1].expr)),
                             toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 8895 "bison-chpl-lib.cpp"
    break;

  case 384: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2369 "chpl.ypp"
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
#line 8911 "bison-chpl-lib.cpp"
    break;

  case 385: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2382 "chpl.ypp"
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
#line 8926 "bison-chpl-lib.cpp"
    break;

  case 386: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type formal_var_arg_expr  */
#line 2394 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
  }
#line 8934 "bison-chpl-lib.cpp"
    break;

  case 387: /* opt_formal_intent_tag: %empty  */
#line 2400 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8943 "bison-chpl-lib.cpp"
    break;

  case 388: /* opt_formal_intent_tag: required_intent_tag  */
#line 2405 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8952 "bison-chpl-lib.cpp"
    break;

  case 389: /* required_intent_tag: TIN  */
#line 2412 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 8958 "bison-chpl-lib.cpp"
    break;

  case 390: /* required_intent_tag: TINOUT  */
#line 2413 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 8964 "bison-chpl-lib.cpp"
    break;

  case 391: /* required_intent_tag: TOUT  */
#line 2414 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 8970 "bison-chpl-lib.cpp"
    break;

  case 392: /* required_intent_tag: TCONST TIN  */
#line 2415 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 8976 "bison-chpl-lib.cpp"
    break;

  case 393: /* required_intent_tag: TCONST TREF  */
#line 2416 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 8982 "bison-chpl-lib.cpp"
    break;

  case 394: /* required_intent_tag: TCONST  */
#line 2417 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 8988 "bison-chpl-lib.cpp"
    break;

  case 395: /* required_intent_tag: TPARAM  */
#line 2418 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 8994 "bison-chpl-lib.cpp"
    break;

  case 396: /* required_intent_tag: TREF  */
#line 2419 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9000 "bison-chpl-lib.cpp"
    break;

  case 397: /* required_intent_tag: TTYPE  */
#line 2420 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 398: /* opt_this_intent_tag: %empty  */
#line 2424 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9012 "bison-chpl-lib.cpp"
    break;

  case 399: /* opt_this_intent_tag: TPARAM  */
#line 2425 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9018 "bison-chpl-lib.cpp"
    break;

  case 400: /* opt_this_intent_tag: TREF  */
#line 2426 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 401: /* opt_this_intent_tag: TCONST TREF  */
#line 2427 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 402: /* opt_this_intent_tag: TCONST  */
#line 2428 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 403: /* opt_this_intent_tag: TTYPE  */
#line 2429 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 404: /* proc_iter_or_op: TPROC  */
#line 2433 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 405: /* proc_iter_or_op: TITER  */
#line 2434 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 406: /* proc_iter_or_op: TOPERATOR  */
#line 2435 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 407: /* opt_ret_tag: %empty  */
#line 2439 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_ret_tag: TCONST  */
#line 2440 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_ret_tag: TCONST TREF  */
#line 2441 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_ret_tag: TREF  */
#line 2442 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_ret_tag: TPARAM  */
#line 2443 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_ret_tag: TTYPE  */
#line 2444 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 413: /* opt_throws_error: %empty  */
#line 2448 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 414: /* opt_throws_error: TTHROWS  */
#line 2449 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_function_body_stmt: TSEMI  */
#line 2452 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_function_body_stmt: function_body_stmt  */
#line 2453 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 417: /* function_body_stmt: block_stmt_body  */
#line 2457 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 418: /* function_body_stmt: return_stmt  */
#line 2458 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9132 "bison-chpl-lib.cpp"
    break;

  case 419: /* query_expr: TQUERIEDIDENT  */
#line 2462 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9138 "bison-chpl-lib.cpp"
    break;

  case 420: /* opt_lifetime_where: %empty  */
#line 2467 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9144 "bison-chpl-lib.cpp"
    break;

  case 421: /* opt_lifetime_where: TWHERE expr  */
#line 2469 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9150 "bison-chpl-lib.cpp"
    break;

  case 422: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2471 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9156 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2473 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9162 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2475 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 425: /* lifetime_components_expr: lifetime_expr  */
#line 2480 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 426: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2482 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9180 "bison-chpl-lib.cpp"
    break;

  case 427: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2487 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9186 "bison-chpl-lib.cpp"
    break;

  case 428: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2489 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9192 "bison-chpl-lib.cpp"
    break;

  case 429: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2491 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9198 "bison-chpl-lib.cpp"
    break;

  case 430: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2493 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9204 "bison-chpl-lib.cpp"
    break;

  case 431: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2495 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9210 "bison-chpl-lib.cpp"
    break;

  case 432: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2497 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9216 "bison-chpl-lib.cpp"
    break;

  case 433: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2499 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9222 "bison-chpl-lib.cpp"
    break;

  case 434: /* lifetime_ident: TIDENT  */
#line 2503 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9228 "bison-chpl-lib.cpp"
    break;

  case 435: /* lifetime_ident: TTHIS  */
#line 2504 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9234 "bison-chpl-lib.cpp"
    break;

  case 436: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2509 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9243 "bison-chpl-lib.cpp"
    break;

  case 437: /* type_alias_decl_stmt_start: TTYPE  */
#line 2517 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9251 "bison-chpl-lib.cpp"
    break;

  case 438: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2521 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9260 "bison-chpl-lib.cpp"
    break;

  case 439: /* type_alias_decl_stmt_start: TEXTERN TTYPE  */
#line 2526 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteLinkage(Decl::EXTERN);
  }
#line 9269 "bison-chpl-lib.cpp"
    break;

  case 440: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2534 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 441: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner TCOMMA type_alias_decl_stmt_inner_ls  */
#line 2538 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[0].exprList), (yyvsp[-2].commentsAndStmt));
  }
#line 9285 "bison-chpl-lib.cpp"
    break;

  case 442: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2545 "chpl.ypp"
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
#line 9307 "bison-chpl-lib.cpp"
    break;

  case 443: /* opt_init_type: %empty  */
#line 2565 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9313 "bison-chpl-lib.cpp"
    break;

  case 444: /* opt_init_type: TASSIGN type_level_expr  */
#line 2567 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9319 "bison-chpl-lib.cpp"
    break;

  case 445: /* opt_init_type: TASSIGN array_type  */
#line 2569 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9328 "bison-chpl-lib.cpp"
    break;

  case 446: /* var_decl_type: TPARAM  */
#line 2576 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9334 "bison-chpl-lib.cpp"
    break;

  case 447: /* var_decl_type: TCONST TREF  */
#line 2577 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9340 "bison-chpl-lib.cpp"
    break;

  case 448: /* var_decl_type: TREF  */
#line 2578 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9346 "bison-chpl-lib.cpp"
    break;

  case 449: /* var_decl_type: TCONST  */
#line 2579 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9352 "bison-chpl-lib.cpp"
    break;

  case 450: /* var_decl_type: TVAR  */
#line 2580 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9358 "bison-chpl-lib.cpp"
    break;

  case 451: /* $@12: %empty  */
#line 2585 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9367 "bison-chpl-lib.cpp"
    break;

  case 452: /* var_decl_stmt: TCONFIG $@12 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2589 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9376 "bison-chpl-lib.cpp"
    break;

  case 453: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2594 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9385 "bison-chpl-lib.cpp"
    break;

  case 454: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2602 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9393 "bison-chpl-lib.cpp"
    break;

  case 455: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2606 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 456: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2613 "chpl.ypp"
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
#line 9421 "bison-chpl-lib.cpp"
    break;

  case 457: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2629 "chpl.ypp"
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
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 458: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2646 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 459: /* tuple_var_decl_component: ident_def  */
#line 2650 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9455 "bison-chpl-lib.cpp"
    break;

  case 460: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2654 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9463 "bison-chpl-lib.cpp"
    break;

  case 461: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2661 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9469 "bison-chpl-lib.cpp"
    break;

  case 462: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2663 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9475 "bison-chpl-lib.cpp"
    break;

  case 463: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2665 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9481 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_init_expr: %empty  */
#line 2671 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9487 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2672 "chpl.ypp"
                        { (yyval.expr) = TODOEXPR((yyloc)); }
#line 9493 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2673 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 467: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2679 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9507 "bison-chpl-lib.cpp"
    break;

  case 468: /* ret_array_type: TLSBR TRSBR  */
#line 2683 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 469: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2687 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 470: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2691 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 9531 "bison-chpl-lib.cpp"
    break;

  case 471: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2695 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9539 "bison-chpl-lib.cpp"
    break;

  case 472: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2699 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 473: /* ret_array_type: TLSBR error TRSBR  */
#line 2703 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "invalid expression for domain of array return type");
  }
#line 9555 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_ret_type: %empty  */
#line 2709 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9561 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_ret_type: TCOLON type_level_expr  */
#line 2710 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9567 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_ret_type: TCOLON ret_array_type  */
#line 2711 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9573 "bison-chpl-lib.cpp"
    break;

  case 477: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2712 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9579 "bison-chpl-lib.cpp"
    break;

  case 478: /* opt_ret_type: error  */
#line 2713 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9585 "bison-chpl-lib.cpp"
    break;

  case 479: /* opt_type: %empty  */
#line 2718 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9591 "bison-chpl-lib.cpp"
    break;

  case 480: /* opt_type: TCOLON type_level_expr  */
#line 2719 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9597 "bison-chpl-lib.cpp"
    break;

  case 481: /* opt_type: TCOLON array_type  */
#line 2720 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9603 "bison-chpl-lib.cpp"
    break;

  case 482: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2721 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9609 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_type: error  */
#line 2722 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9615 "bison-chpl-lib.cpp"
    break;

  case 484: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2743 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9623 "bison-chpl-lib.cpp"
    break;

  case 485: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2747 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9631 "bison-chpl-lib.cpp"
    break;

  case 486: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2751 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9639 "bison-chpl-lib.cpp"
    break;

  case 487: /* array_type: TLSBR error TRSBR  */
#line 2755 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 9647 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_formal_array_elt_type: %empty  */
#line 2761 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9653 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_formal_array_elt_type: type_level_expr  */
#line 2762 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9659 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_formal_array_elt_type: query_expr  */
#line 2763 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9665 "bison-chpl-lib.cpp"
    break;

  case 491: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2768 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 492: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2773 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9682 "bison-chpl-lib.cpp"
    break;

  case 493: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2781 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9691 "bison-chpl-lib.cpp"
    break;

  case 494: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2786 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9699 "bison-chpl-lib.cpp"
    break;

  case 495: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2790 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9707 "bison-chpl-lib.cpp"
    break;

  case 496: /* opt_formal_type: %empty  */
#line 2796 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9713 "bison-chpl-lib.cpp"
    break;

  case 497: /* opt_formal_type: TCOLON type_level_expr  */
#line 2797 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9719 "bison-chpl-lib.cpp"
    break;

  case 498: /* opt_formal_type: TCOLON query_expr  */
#line 2798 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9725 "bison-chpl-lib.cpp"
    break;

  case 499: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2799 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9731 "bison-chpl-lib.cpp"
    break;

  case 500: /* opt_formal_type: TCOLON formal_array_type  */
#line 2800 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9737 "bison-chpl-lib.cpp"
    break;

  case 501: /* expr_ls: expr  */
#line 2806 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9743 "bison-chpl-lib.cpp"
    break;

  case 502: /* expr_ls: query_expr  */
#line 2807 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9749 "bison-chpl-lib.cpp"
    break;

  case 503: /* expr_ls: expr_ls TCOMMA expr  */
#line 2808 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9755 "bison-chpl-lib.cpp"
    break;

  case 504: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2809 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 505: /* simple_expr_ls: expr  */
#line 2813 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9767 "bison-chpl-lib.cpp"
    break;

  case 506: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2814 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9773 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_component: TUNDERSCORE  */
#line 2818 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9779 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_component: opt_try_expr  */
#line 2819 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9785 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_component: query_expr  */
#line 2820 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9791 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2825 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9799 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2829 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9807 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_actual_ls: %empty  */
#line 2835 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9813 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_actual_ls: actual_ls  */
#line 2836 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9819 "bison-chpl-lib.cpp"
    break;

  case 514: /* actual_ls: actual_expr  */
#line 2841 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9828 "bison-chpl-lib.cpp"
    break;

  case 515: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2846 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9838 "bison-chpl-lib.cpp"
    break;

  case 516: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2854 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9844 "bison-chpl-lib.cpp"
    break;

  case 517: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2855 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9850 "bison-chpl-lib.cpp"
    break;

  case 518: /* actual_expr: query_expr  */
#line 2856 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9856 "bison-chpl-lib.cpp"
    break;

  case 519: /* actual_expr: opt_try_expr  */
#line 2857 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9862 "bison-chpl-lib.cpp"
    break;

  case 520: /* ident_expr: ident_use  */
#line 2861 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9868 "bison-chpl-lib.cpp"
    break;

  case 521: /* ident_expr: scalar_type  */
#line 2862 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9874 "bison-chpl-lib.cpp"
    break;

  case 522: /* type_level_expr: sub_type_level_expr  */
#line 2874 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9880 "bison-chpl-lib.cpp"
    break;

  case 523: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2876 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 9886 "bison-chpl-lib.cpp"
    break;

  case 524: /* type_level_expr: TQUESTION  */
#line 2878 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 9892 "bison-chpl-lib.cpp"
    break;

  case 530: /* sub_type_level_expr: TSINGLE expr  */
#line 2889 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9898 "bison-chpl-lib.cpp"
    break;

  case 531: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2891 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9904 "bison-chpl-lib.cpp"
    break;

  case 532: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2893 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9910 "bison-chpl-lib.cpp"
    break;

  case 533: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2895 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9916 "bison-chpl-lib.cpp"
    break;

  case 534: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2897 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9926 "bison-chpl-lib.cpp"
    break;

  case 535: /* sub_type_level_expr: TATOMIC expr  */
#line 2903 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9932 "bison-chpl-lib.cpp"
    break;

  case 536: /* sub_type_level_expr: TSYNC expr  */
#line 2905 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9938 "bison-chpl-lib.cpp"
    break;

  case 537: /* sub_type_level_expr: TOWNED  */
#line 2908 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9944 "bison-chpl-lib.cpp"
    break;

  case 538: /* sub_type_level_expr: TOWNED expr  */
#line 2910 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9950 "bison-chpl-lib.cpp"
    break;

  case 539: /* sub_type_level_expr: TUNMANAGED  */
#line 2912 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9956 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2914 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9962 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TSHARED  */
#line 2916 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9968 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TSHARED expr  */
#line 2918 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9974 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TBORROWED  */
#line 2920 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9980 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TBORROWED expr  */
#line 2922 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9986 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TCLASS  */
#line 2925 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9992 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TRECORD  */
#line 2927 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9998 "bison-chpl-lib.cpp"
    break;

  case 547: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2932 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10011 "bison-chpl-lib.cpp"
    break;

  case 548: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2941 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10024 "bison-chpl-lib.cpp"
    break;

  case 549: /* for_expr: TFOR expr TDO expr  */
#line 2950 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[0].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10036 "bison-chpl-lib.cpp"
    break;

  case 550: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2958 "chpl.ypp"
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
#line 10054 "bison-chpl-lib.cpp"
    break;

  case 551: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 2972 "chpl.ypp"
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
#line 10072 "bison-chpl-lib.cpp"
    break;

  case 552: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 2986 "chpl.ypp"
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
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 553: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3000 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10103 "bison-chpl-lib.cpp"
    break;

  case 554: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3009 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10116 "bison-chpl-lib.cpp"
    break;

  case 555: /* for_expr: TFORALL expr TDO expr  */
#line 3018 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[0].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10128 "bison-chpl-lib.cpp"
    break;

  case 556: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3026 "chpl.ypp"
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
#line 10146 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3040 "chpl.ypp"
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
#line 10164 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3054 "chpl.ypp"
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
#line 10182 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 3068 "chpl.ypp"
  {
    // TODO: We have to handle the possibility of [1..2, 3..4] here.
    if ((yyvsp[-2].exprList)->size() > 1) {
      const char* msg = "Invalid iterand expression";
      (yyval.expr) = context->raiseError((yylsp[-2]), msg);
    } else {
      auto iterand = context->consumeList((yyvsp[-2].exprList))[0].release()->toExpression();
      assert(iterand);
      (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), /*index*/ nullptr,
                              toOwned(iterand),
                              /*withClause*/ nullptr,
                              BlockStyle::IMPLICIT,
                              context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                              /*isExpressionLevel*/ true).release();
    }
  }
#line 10203 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3085 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10216 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3094 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10229 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3103 "chpl.ypp"
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
#line 10248 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3118 "chpl.ypp"
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
#line 10267 "bison-chpl-lib.cpp"
    break;

  case 564: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3136 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10281 "bison-chpl-lib.cpp"
    break;

  case 565: /* nil_expr: TNIL  */
#line 3153 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10287 "bison-chpl-lib.cpp"
    break;

  case 573: /* stmt_level_expr: io_expr TIO expr  */
#line 3169 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10293 "bison-chpl-lib.cpp"
    break;

  case 574: /* opt_task_intent_ls: %empty  */
#line 3173 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10299 "bison-chpl-lib.cpp"
    break;

  case 575: /* opt_task_intent_ls: task_intent_clause  */
#line 3174 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10305 "bison-chpl-lib.cpp"
    break;

  case 576: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3179 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10315 "bison-chpl-lib.cpp"
    break;

  case 577: /* task_intent_ls: intent_expr  */
#line 3187 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10321 "bison-chpl-lib.cpp"
    break;

  case 578: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3188 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10327 "bison-chpl-lib.cpp"
    break;

  case 579: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3193 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10337 "bison-chpl-lib.cpp"
    break;

  case 580: /* forall_intent_ls: intent_expr  */
#line 3201 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10343 "bison-chpl-lib.cpp"
    break;

  case 581: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3202 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10349 "bison-chpl-lib.cpp"
    break;

  case 582: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3207 "chpl.ypp"
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
#line 10368 "bison-chpl-lib.cpp"
    break;

  case 583: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3222 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 10376 "bison-chpl-lib.cpp"
    break;

  case 584: /* intent_expr: expr TREDUCE ident_expr  */
#line 3226 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10384 "bison-chpl-lib.cpp"
    break;

  case 585: /* task_var_prefix: TCONST  */
#line 3232 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10390 "bison-chpl-lib.cpp"
    break;

  case 586: /* task_var_prefix: TIN  */
#line 3233 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10396 "bison-chpl-lib.cpp"
    break;

  case 587: /* task_var_prefix: TCONST TIN  */
#line 3234 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10402 "bison-chpl-lib.cpp"
    break;

  case 588: /* task_var_prefix: TREF  */
#line 3235 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10408 "bison-chpl-lib.cpp"
    break;

  case 589: /* task_var_prefix: TCONST TREF  */
#line 3236 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10414 "bison-chpl-lib.cpp"
    break;

  case 590: /* task_var_prefix: TVAR  */
#line 3237 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10420 "bison-chpl-lib.cpp"
    break;

  case 592: /* io_expr: io_expr TIO expr  */
#line 3243 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10426 "bison-chpl-lib.cpp"
    break;

  case 593: /* new_maybe_decorated: TNEW  */
#line 3248 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10432 "bison-chpl-lib.cpp"
    break;

  case 594: /* new_maybe_decorated: TNEW TOWNED  */
#line 3250 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10438 "bison-chpl-lib.cpp"
    break;

  case 595: /* new_maybe_decorated: TNEW TSHARED  */
#line 3252 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10444 "bison-chpl-lib.cpp"
    break;

  case 596: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3254 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10450 "bison-chpl-lib.cpp"
    break;

  case 597: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3256 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10456 "bison-chpl-lib.cpp"
    break;

  case 598: /* new_expr: new_maybe_decorated expr  */
#line 3262 "chpl.ypp"
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
#line 10474 "bison-chpl-lib.cpp"
    break;

  case 599: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3279 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10482 "bison-chpl-lib.cpp"
    break;

  case 600: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3283 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10490 "bison-chpl-lib.cpp"
    break;

  case 601: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3287 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10498 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3291 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10506 "bison-chpl-lib.cpp"
    break;

  case 603: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3298 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10512 "bison-chpl-lib.cpp"
    break;

  case 613: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 3315 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10520 "bison-chpl-lib.cpp"
    break;

  case 614: /* expr: expr TCOLON expr  */
#line 3319 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10528 "bison-chpl-lib.cpp"
    break;

  case 615: /* expr: expr TDOTDOT expr  */
#line 3323 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10537 "bison-chpl-lib.cpp"
    break;

  case 616: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 3328 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10546 "bison-chpl-lib.cpp"
    break;

  case 617: /* expr: expr TDOTDOT  */
#line 3333 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10555 "bison-chpl-lib.cpp"
    break;

  case 618: /* expr: TDOTDOT expr  */
#line 3338 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10565 "bison-chpl-lib.cpp"
    break;

  case 619: /* expr: TDOTDOTOPENHIGH expr  */
#line 3344 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10575 "bison-chpl-lib.cpp"
    break;

  case 620: /* expr: TDOTDOT  */
#line 3350 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10585 "bison-chpl-lib.cpp"
    break;

  case 621: /* opt_expr: %empty  */
#line 3381 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10591 "bison-chpl-lib.cpp"
    break;

  case 622: /* opt_expr: expr  */
#line 3382 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10597 "bison-chpl-lib.cpp"
    break;

  case 623: /* opt_try_expr: TTRY expr  */
#line 3386 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10603 "bison-chpl-lib.cpp"
    break;

  case 624: /* opt_try_expr: TTRYBANG expr  */
#line 3387 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10609 "bison-chpl-lib.cpp"
    break;

  case 625: /* opt_try_expr: expr  */
#line 3388 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10615 "bison-chpl-lib.cpp"
    break;

  case 631: /* call_base_expr: expr TBANG  */
#line 3405 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10621 "bison-chpl-lib.cpp"
    break;

  case 632: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3406 "chpl.ypp"
                                { (yyval.expr) = TODOEXPR((yyloc)); }
#line 10627 "bison-chpl-lib.cpp"
    break;

  case 635: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3413 "chpl.ypp"
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
#line 10643 "bison-chpl-lib.cpp"
    break;

  case 636: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3425 "chpl.ypp"
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
#line 10659 "bison-chpl-lib.cpp"
    break;

  case 637: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3437 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10667 "bison-chpl-lib.cpp"
    break;

  case 638: /* dot_expr: expr TDOT ident_use  */
#line 3444 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10673 "bison-chpl-lib.cpp"
    break;

  case 639: /* dot_expr: expr TDOT TTYPE  */
#line 3446 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10679 "bison-chpl-lib.cpp"
    break;

  case 640: /* dot_expr: expr TDOT TDOMAIN  */
#line 3448 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10685 "bison-chpl-lib.cpp"
    break;

  case 641: /* dot_expr: expr TDOT TLOCALE  */
#line 3450 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10691 "bison-chpl-lib.cpp"
    break;

  case 642: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3452 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10701 "bison-chpl-lib.cpp"
    break;

  case 643: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3458 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10711 "bison-chpl-lib.cpp"
    break;

  case 644: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3470 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10717 "bison-chpl-lib.cpp"
    break;

  case 645: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3472 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10725 "bison-chpl-lib.cpp"
    break;

  case 646: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3476 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10733 "bison-chpl-lib.cpp"
    break;

  case 647: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3480 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10741 "bison-chpl-lib.cpp"
    break;

  case 648: /* bool_literal: TFALSE  */
#line 3486 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10747 "bison-chpl-lib.cpp"
    break;

  case 649: /* bool_literal: TTRUE  */
#line 3487 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10753 "bison-chpl-lib.cpp"
    break;

  case 650: /* str_bytes_literal: STRINGLITERAL  */
#line 3491 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10759 "bison-chpl-lib.cpp"
    break;

  case 651: /* str_bytes_literal: BYTESLITERAL  */
#line 3492 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10765 "bison-chpl-lib.cpp"
    break;

  case 654: /* literal_expr: INTLITERAL  */
#line 3498 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10771 "bison-chpl-lib.cpp"
    break;

  case 655: /* literal_expr: REALLITERAL  */
#line 3499 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10777 "bison-chpl-lib.cpp"
    break;

  case 656: /* literal_expr: IMAGLITERAL  */
#line 3500 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10783 "bison-chpl-lib.cpp"
    break;

  case 657: /* literal_expr: CSTRINGLITERAL  */
#line 3501 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10789 "bison-chpl-lib.cpp"
    break;

  case 658: /* literal_expr: TNONE  */
#line 3502 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10795 "bison-chpl-lib.cpp"
    break;

  case 659: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3504 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10803 "bison-chpl-lib.cpp"
    break;

  case 660: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3508 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10811 "bison-chpl-lib.cpp"
    break;

  case 661: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3512 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10819 "bison-chpl-lib.cpp"
    break;

  case 662: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3516 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10827 "bison-chpl-lib.cpp"
    break;

  case 663: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3520 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10835 "bison-chpl-lib.cpp"
    break;

  case 664: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3524 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10844 "bison-chpl-lib.cpp"
    break;

  case 665: /* assoc_expr_ls: expr TALIAS expr  */
#line 3533 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10853 "bison-chpl-lib.cpp"
    break;

  case 666: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3538 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10863 "bison-chpl-lib.cpp"
    break;

  case 667: /* binary_op_expr: expr TPLUS expr  */
#line 3546 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10869 "bison-chpl-lib.cpp"
    break;

  case 668: /* binary_op_expr: expr TMINUS expr  */
#line 3547 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10875 "bison-chpl-lib.cpp"
    break;

  case 669: /* binary_op_expr: expr TSTAR expr  */
#line 3548 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10881 "bison-chpl-lib.cpp"
    break;

  case 670: /* binary_op_expr: expr TDIVIDE expr  */
#line 3549 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10887 "bison-chpl-lib.cpp"
    break;

  case 671: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3550 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10893 "bison-chpl-lib.cpp"
    break;

  case 672: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3551 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10899 "bison-chpl-lib.cpp"
    break;

  case 673: /* binary_op_expr: expr TMOD expr  */
#line 3552 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10905 "bison-chpl-lib.cpp"
    break;

  case 674: /* binary_op_expr: expr TEQUAL expr  */
#line 3553 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10911 "bison-chpl-lib.cpp"
    break;

  case 675: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3554 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10917 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3555 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10923 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3556 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10929 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TLESS expr  */
#line 3557 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10935 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr TGREATER expr  */
#line 3558 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10941 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TBAND expr  */
#line 3559 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10947 "bison-chpl-lib.cpp"
    break;

  case 681: /* binary_op_expr: expr TBOR expr  */
#line 3560 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10953 "bison-chpl-lib.cpp"
    break;

  case 682: /* binary_op_expr: expr TBXOR expr  */
#line 3561 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10959 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TAND expr  */
#line 3562 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10965 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TOR expr  */
#line 3563 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10971 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TEXP expr  */
#line 3564 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10977 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TBY expr  */
#line 3565 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10983 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TALIGN expr  */
#line 3566 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10989 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr THASH expr  */
#line 3567 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10995 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TDMAPPED expr  */
#line 3568 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11001 "bison-chpl-lib.cpp"
    break;

  case 690: /* unary_op_expr: TPLUS expr  */
#line 3572 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11007 "bison-chpl-lib.cpp"
    break;

  case 691: /* unary_op_expr: TMINUS expr  */
#line 3573 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11013 "bison-chpl-lib.cpp"
    break;

  case 692: /* unary_op_expr: TMINUSMINUS expr  */
#line 3574 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 11019 "bison-chpl-lib.cpp"
    break;

  case 693: /* unary_op_expr: TPLUSPLUS expr  */
#line 3575 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 11025 "bison-chpl-lib.cpp"
    break;

  case 694: /* unary_op_expr: TBANG expr  */
#line 3576 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11031 "bison-chpl-lib.cpp"
    break;

  case 695: /* unary_op_expr: expr TBANG  */
#line 3577 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11039 "bison-chpl-lib.cpp"
    break;

  case 696: /* unary_op_expr: TBNOT expr  */
#line 3580 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11045 "bison-chpl-lib.cpp"
    break;

  case 697: /* reduce_expr: expr TREDUCE expr  */
#line 3585 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11053 "bison-chpl-lib.cpp"
    break;

  case 698: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3589 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11061 "bison-chpl-lib.cpp"
    break;

  case 699: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3593 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11069 "bison-chpl-lib.cpp"
    break;

  case 700: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3597 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11077 "bison-chpl-lib.cpp"
    break;

  case 701: /* scan_expr: expr TSCAN expr  */
#line 3604 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11085 "bison-chpl-lib.cpp"
    break;

  case 702: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3608 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11093 "bison-chpl-lib.cpp"
    break;

  case 703: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3612 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11101 "bison-chpl-lib.cpp"
    break;

  case 704: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3616 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11109 "bison-chpl-lib.cpp"
    break;


#line 11113 "bison-chpl-lib.cpp"

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
