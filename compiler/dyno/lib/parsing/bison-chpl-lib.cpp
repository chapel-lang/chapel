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
  YYSYMBOL_TUSE = 114,                     /* TUSE  */
  YYSYMBOL_TVAR = 115,                     /* TVAR  */
  YYSYMBOL_TVOID = 116,                    /* TVOID  */
  YYSYMBOL_TWHEN = 117,                    /* TWHEN  */
  YYSYMBOL_TWHERE = 118,                   /* TWHERE  */
  YYSYMBOL_TWHILE = 119,                   /* TWHILE  */
  YYSYMBOL_TWITH = 120,                    /* TWITH  */
  YYSYMBOL_TYIELD = 121,                   /* TYIELD  */
  YYSYMBOL_TZIP = 122,                     /* TZIP  */
  YYSYMBOL_TALIAS = 123,                   /* TALIAS  */
  YYSYMBOL_TAND = 124,                     /* TAND  */
  YYSYMBOL_TASSIGN = 125,                  /* TASSIGN  */
  YYSYMBOL_TASSIGNBAND = 126,              /* TASSIGNBAND  */
  YYSYMBOL_TASSIGNBOR = 127,               /* TASSIGNBOR  */
  YYSYMBOL_TASSIGNBXOR = 128,              /* TASSIGNBXOR  */
  YYSYMBOL_TASSIGNDIVIDE = 129,            /* TASSIGNDIVIDE  */
  YYSYMBOL_TASSIGNEXP = 130,               /* TASSIGNEXP  */
  YYSYMBOL_TASSIGNLAND = 131,              /* TASSIGNLAND  */
  YYSYMBOL_TASSIGNLOR = 132,               /* TASSIGNLOR  */
  YYSYMBOL_TASSIGNMINUS = 133,             /* TASSIGNMINUS  */
  YYSYMBOL_TASSIGNMOD = 134,               /* TASSIGNMOD  */
  YYSYMBOL_TASSIGNMULTIPLY = 135,          /* TASSIGNMULTIPLY  */
  YYSYMBOL_TASSIGNPLUS = 136,              /* TASSIGNPLUS  */
  YYSYMBOL_TASSIGNREDUCE = 137,            /* TASSIGNREDUCE  */
  YYSYMBOL_TASSIGNSL = 138,                /* TASSIGNSL  */
  YYSYMBOL_TASSIGNSR = 139,                /* TASSIGNSR  */
  YYSYMBOL_TBANG = 140,                    /* TBANG  */
  YYSYMBOL_TBAND = 141,                    /* TBAND  */
  YYSYMBOL_TBNOT = 142,                    /* TBNOT  */
  YYSYMBOL_TBOR = 143,                     /* TBOR  */
  YYSYMBOL_TBXOR = 144,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 145,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 146,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 147,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 148,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 149,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 150,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 151,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 152,                     /* TEXP  */
  YYSYMBOL_TGREATER = 153,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 154,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 155,                    /* THASH  */
  YYSYMBOL_TIO = 156,                      /* TIO  */
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
  YYSYMBOL_deprecated_decl_base = 191,     /* deprecated_decl_base  */
  YYSYMBOL_module_decl_start = 192,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 193,         /* module_decl_stmt  */
  YYSYMBOL_opt_access_control = 194,       /* opt_access_control  */
  YYSYMBOL_opt_prototype = 195,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 196,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 197,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 198,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 199,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 200,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 201,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 202,              /* opt_only_ls  */
  YYSYMBOL_use_stmt = 203,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 204,              /* import_stmt  */
  YYSYMBOL_import_expr = 205,              /* import_expr  */
  YYSYMBOL_import_ls = 206,                /* import_ls  */
  YYSYMBOL_require_stmt = 207,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 208,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 209,          /* opt_label_ident  */
  YYSYMBOL_ident_fn_def = 210,             /* ident_fn_def  */
  YYSYMBOL_ident_def = 211,                /* ident_def  */
  YYSYMBOL_ident_use = 212,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 213,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 214,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 215,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 216,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 217,              /* return_stmt  */
  YYSYMBOL_deprecated_class_level_stmt = 218, /* deprecated_class_level_stmt  */
  YYSYMBOL_219_3 = 219,                    /* $@3  */
  YYSYMBOL_220_4 = 220,                    /* $@4  */
  YYSYMBOL_class_level_stmt = 221,         /* class_level_stmt  */
  YYSYMBOL_222_5 = 222,                    /* $@5  */
  YYSYMBOL_223_6 = 223,                    /* $@6  */
  YYSYMBOL_inner_class_level_stmt = 224,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 225,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 226,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 227,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 228, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 229,  /* extern_export_decl_stmt  */
  YYSYMBOL_230_7 = 230,                    /* $@7  */
  YYSYMBOL_231_8 = 231,                    /* $@8  */
  YYSYMBOL_extern_block_stmt = 232,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 233,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 234,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 235,                  /* if_stmt  */
  YYSYMBOL_ifvar = 236,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 237,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 238,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 239,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 240,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 241, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 242,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 243,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 244,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 245,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 246,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 247,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 248,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 249,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 250,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 251,                /* when_stmt  */
  YYSYMBOL_manager_expr = 252,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 253,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 254,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 255,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 256,              /* class_start  */
  YYSYMBOL_class_tag = 257,                /* class_tag  */
  YYSYMBOL_opt_inherit = 258,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 259,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 260,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 261,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 262,                  /* enum_ls  */
  YYSYMBOL_263_9 = 263,                    /* $@9  */
  YYSYMBOL_deprecated_enum_item = 264,     /* deprecated_enum_item  */
  YYSYMBOL_265_10 = 265,                   /* $@10  */
  YYSYMBOL_266_11 = 266,                   /* $@11  */
  YYSYMBOL_enum_item = 267,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 268,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 269,         /* lambda_decl_expr  */
  YYSYMBOL_270_12 = 270,                   /* $@12  */
  YYSYMBOL_linkage_spec_empty = 271,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 272,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt_complete = 273,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 274,             /* fn_decl_stmt  */
  YYSYMBOL_275_13 = 275,                   /* $@13  */
  YYSYMBOL_fn_decl_stmt_inner = 276,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 277,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 278,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 279,                 /* fn_ident  */
  YYSYMBOL_op_ident = 280,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 281,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 282,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 283,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 284,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 285,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 286,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 287,                /* formal_ls  */
  YYSYMBOL_formal = 288,                   /* formal  */
  YYSYMBOL_opt_formal_intent_tag = 289,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 290,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 291,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 292,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 293,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 294,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 295,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 296,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 297,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 298,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 299, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 300,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 301,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 302,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 303, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 304, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 305, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 306,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 307,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 308,            /* var_decl_stmt  */
  YYSYMBOL_309_14 = 309,                   /* $@14  */
  YYSYMBOL_var_decl_stmt_inner_ls = 310,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 311,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 312, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 313, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 314,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 315,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 316,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 317,                 /* opt_type  */
  YYSYMBOL_array_type = 318,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 319, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 320,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 321,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 322,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 323,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 324,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 325,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 326,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 327,                /* actual_ls  */
  YYSYMBOL_actual_expr = 328,              /* actual_expr  */
  YYSYMBOL_ident_expr = 329,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 330,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 331,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 332,                 /* for_expr  */
  YYSYMBOL_cond_expr = 333,                /* cond_expr  */
  YYSYMBOL_nil_expr = 334,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 335,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 336,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 337,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 338,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 339,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 340,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 341,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 342,          /* task_var_prefix  */
  YYSYMBOL_io_expr = 343,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 344,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 345,                 /* new_expr  */
  YYSYMBOL_let_expr = 346,                 /* let_expr  */
  YYSYMBOL_expr = 347,                     /* expr  */
  YYSYMBOL_opt_expr = 348,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 349,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 350,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 351,           /* call_base_expr  */
  YYSYMBOL_call_expr = 352,                /* call_expr  */
  YYSYMBOL_dot_expr = 353,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 354,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 355,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 356,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 357,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 358,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 359,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 360,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 361,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 362,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 363       /* reduce_scan_op_expr  */
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

#line 487 "bison-chpl-lib.cpp"

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
#define YYLAST   20353

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  183
/* YYNRULES -- Number of rules.  */
#define YYNRULES  720
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1275

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
       0,   609,   609,   613,   614,   619,   620,   628,   632,   640,
     641,   642,   643,   644,   645,   646,   647,   648,   649,   650,
     663,   664,   665,   674,   683,   687,   694,   702,   703,   704,
     705,   706,   707,   708,   709,   723,   738,   746,   758,   769,
     781,   793,   804,   820,   822,   821,   830,   829,   840,   841,
     845,   865,   880,   895,   911,   912,   914,   919,   920,   925,
     943,   947,   951,   962,   981,   982,   986,   990,   994,   999,
    1003,  1007,  1017,  1022,  1028,  1035,  1040,  1047,  1058,  1059,
    1063,  1067,  1074,  1082,  1091,  1098,  1106,  1118,  1125,  1129,
    1134,  1140,  1149,  1150,  1154,  1163,  1167,  1171,  1175,  1179,
    1183,  1192,  1193,  1197,  1198,  1199,  1200,  1201,  1202,  1208,
    1209,  1210,  1211,  1212,  1213,  1228,  1229,  1240,  1241,  1242,
    1243,  1244,  1245,  1246,  1247,  1248,  1249,  1250,  1251,  1252,
    1253,  1254,  1255,  1256,  1257,  1258,  1262,  1263,  1264,  1265,
    1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,  1280,  1281,
    1282,  1283,  1287,  1288,  1292,  1299,  1309,  1311,  1310,  1319,
    1318,  1329,  1333,  1339,  1339,  1345,  1345,  1354,  1355,  1356,
    1357,  1358,  1359,  1360,  1364,  1369,  1374,  1379,  1386,  1394,
    1395,  1399,  1408,  1414,  1422,  1441,  1440,  1450,  1449,  1462,
    1469,  1487,  1500,  1513,  1517,  1521,  1525,  1529,  1533,  1537,
    1541,  1559,  1563,  1567,  1571,  1575,  1579,  1583,  1587,  1591,
    1595,  1599,  1603,  1607,  1611,  1615,  1619,  1623,  1627,  1631,
    1635,  1639,  1643,  1647,  1652,  1660,  1669,  1673,  1677,  1681,
    1685,  1689,  1693,  1697,  1701,  1706,  1711,  1716,  1724,  1739,
    1757,  1761,  1768,  1769,  1774,  1779,  1780,  1781,  1782,  1783,
    1784,  1785,  1786,  1787,  1788,  1789,  1790,  1791,  1803,  1804,
    1805,  1806,  1815,  1816,  1820,  1824,  1828,  1835,  1839,  1843,
    1850,  1854,  1858,  1862,  1869,  1870,  1874,  1878,  1882,  1889,
    1902,  1918,  1926,  1930,  1939,  1940,  1944,  1948,  1953,  1961,
    1966,  1970,  1977,  1978,  1982,  1991,  1996,  2007,  2014,  2015,
    2016,  2020,  2021,  2025,  2029,  2033,  2040,  2057,  2070,  2077,
    2082,  2089,  2088,  2101,  2103,  2102,  2111,  2110,  2121,  2128,
    2140,  2157,  2155,  2182,  2186,  2187,  2189,  2194,  2204,  2201,
    2235,  2243,  2251,  2262,  2273,  2282,  2297,  2298,  2302,  2303,
    2304,  2313,  2314,  2315,  2316,  2317,  2318,  2319,  2320,  2321,
    2322,  2323,  2324,  2325,  2326,  2327,  2328,  2329,  2330,  2331,
    2332,  2333,  2334,  2335,  2336,  2337,  2338,  2342,  2343,  2344,
    2345,  2346,  2347,  2348,  2349,  2350,  2351,  2352,  2353,  2358,
    2359,  2363,  2364,  2365,  2369,  2370,  2374,  2378,  2379,  2383,
    2384,  2388,  2397,  2408,  2418,  2430,  2442,  2450,  2454,  2462,
    2463,  2464,  2465,  2466,  2467,  2468,  2469,  2470,  2474,  2475,
    2476,  2477,  2478,  2479,  2483,  2484,  2485,  2489,  2490,  2491,
    2492,  2493,  2494,  2498,  2499,  2502,  2503,  2507,  2508,  2512,
    2517,  2518,  2520,  2522,  2524,  2529,  2531,  2536,  2538,  2540,
    2542,  2544,  2546,  2548,  2553,  2554,  2558,  2567,  2571,  2579,
    2583,  2590,  2611,  2612,  2614,  2622,  2623,  2624,  2625,  2626,
    2631,  2630,  2639,  2647,  2651,  2658,  2674,  2691,  2695,  2699,
    2706,  2708,  2710,  2717,  2718,  2719,  2724,  2728,  2732,  2736,
    2740,  2744,  2748,  2755,  2756,  2757,  2758,  2759,  2764,  2765,
    2766,  2767,  2768,  2788,  2792,  2796,  2800,  2807,  2808,  2809,
    2813,  2818,  2826,  2831,  2835,  2842,  2843,  2844,  2845,  2846,
    2852,  2853,  2854,  2855,  2859,  2860,  2864,  2865,  2866,  2870,
    2874,  2881,  2882,  2886,  2891,  2900,  2901,  2902,  2903,  2907,
    2908,  2919,  2921,  2923,  2929,  2930,  2931,  2932,  2933,  2934,
    2936,  2938,  2940,  2942,  2948,  2950,  2953,  2955,  2957,  2959,
    2961,  2963,  2965,  2967,  2970,  2972,  2977,  2986,  2995,  3003,
    3017,  3031,  3045,  3054,  3063,  3071,  3085,  3099,  3113,  3130,
    3139,  3148,  3163,  3181,  3199,  3207,  3208,  3209,  3210,  3211,
    3212,  3213,  3214,  3219,  3220,  3224,  3233,  3234,  3238,  3247,
    3248,  3252,  3267,  3271,  3278,  3279,  3280,  3281,  3282,  3283,
    3287,  3288,  3293,  3295,  3297,  3299,  3301,  3307,  3314,  3326,
    3338,  3351,  3368,  3378,  3379,  3380,  3381,  3382,  3383,  3384,
    3385,  3386,  3387,  3391,  3395,  3400,  3405,  3410,  3416,  3422,
    3454,  3455,  3459,  3460,  3461,  3465,  3466,  3467,  3468,  3477,
    3478,  3481,  3482,  3483,  3487,  3499,  3511,  3518,  3520,  3522,
    3524,  3526,  3532,  3545,  3546,  3550,  3554,  3561,  3562,  3566,
    3567,  3571,  3572,  3573,  3574,  3575,  3576,  3577,  3578,  3583,
    3588,  3592,  3597,  3601,  3610,  3615,  3624,  3625,  3626,  3627,
    3628,  3629,  3630,  3631,  3632,  3633,  3634,  3635,  3636,  3637,
    3638,  3639,  3640,  3641,  3642,  3643,  3644,  3645,  3646,  3650,
    3651,  3652,  3653,  3654,  3655,  3658,  3662,  3666,  3670,  3674,
    3681,  3685,  3689,  3693,  3701,  3702,  3703,  3704,  3705,  3706,
    3707
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
  "module_decl_start", "module_decl_stmt", "opt_access_control",
  "opt_prototype", "include_module_stmt", "block_stmt_body", "block_stmt",
  "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls", "use_stmt",
  "import_stmt", "import_expr", "import_ls", "require_stmt",
  "assignment_stmt", "opt_label_ident", "ident_fn_def", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt",
  "deprecated_class_level_stmt", "$@3", "$@4", "class_level_stmt", "$@5",
  "$@6", "inner_class_level_stmt", "forwarding_decl_stmt",
  "forwarding_decl_start", "extern_or_export",
  "extern_export_decl_stmt_start", "extern_export_decl_stmt", "$@7", "$@8",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
  "catch_expr", "catch_expr_inner", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "manager_expr", "manager_expr_ls",
  "manage_stmt", "class_decl_stmt", "class_start", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header_lcbr", "enum_ls", "$@9", "deprecated_enum_item", "$@10",
  "$@11", "enum_item", "lambda_decl_start", "lambda_decl_expr", "$@12",
  "linkage_spec_empty", "linkage_spec", "fn_decl_stmt_complete",
  "fn_decl_stmt", "$@13", "fn_decl_stmt_inner", "fn_decl_stmt_start",
  "fn_decl_receiver_expr", "fn_ident", "op_ident", "assignop_ident",
  "all_op_name", "formal_var_arg_expr", "opt_formal_ls", "req_formal_ls",
  "formal_ls_inner", "formal_ls", "formal", "opt_formal_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_iter_or_op",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner_ls", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt", "$@14",
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

#define YYPACT_NINF (-1158)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-721)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1158,   103,  3873, -1158,    59,   221, -1158, -1158, -1158, -1158,
   -1158, -1158, 12239,    37,   259,   288, 14971,   305, 19486,    37,
   12239,   356,   217,   323,   259,  5273, 12239,   417,  5273,   254,
   19718, -1158,   453,   423,  9109, 10501, 10501, -1158,  9282,   509,
     431,   421, -1158,   512, 19718, 19718, 19718, -1158,  3143, 10674,
     523, 12239, 12239,   216, -1158,   545,   546, 12239, -1158, 14971,
   -1158, 12239,   607,   450,   309,   388,   588, 20237, -1158, 10849,
    8588, 12239, 10674, 14971, 12239,   556,   605,   473,  5273,   606,
   12239,   613,  7544,  7544, -1158,   616, -1158, 14971, -1158,   623,
    9282,  9455, -1158, 12239, -1158, 12239, -1158, -1158, 14488, 12239,
   -1158, 12239, -1158, -1158, -1158,  4223,  7719,  9630, 12239, -1158,
    5098, -1158, -1158, -1158, -1158,   510, -1158,   228, -1158, -1158,
   -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158,
   -1158,  7892, -1158, 11022, -1158, -1158, -1158, -1158, -1158,   630,
   -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158,   542, 19718,
   -1158, 20049,   528,   194, -1158,    57, -1158, -1158,    45,   238,
   -1158, 19718,  3143, -1158,   550, -1158,   552, -1158, -1158,   559,
     566,   580, 12239,   572,   575, 19748,  3224,   104,   577,   583,
   -1158, -1158,   329, -1158, -1158, -1158, -1158, -1158,   477, -1158,
   -1158, -1158, 12239, 12239, 12239, 19718, -1158, 12239, 10849, 10849,
     689,   456, -1158, -1158, -1158, -1158, 19748,   461, -1158, -1158,
     582,  5273, -1158, -1158,   584,   224,   585, 16756, -1158,   242,
   -1158,   591, -1158,   -47, 19748, -1158,  2761,   638,  8763, -1158,
   -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158,
   -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158,
   -1158, -1158, -1158,   590, -1158, -1158, 19718,   593,    40, 16362,
      32, 16273,    32, 16322, 19718, 19718,   -11, 15513, -1158, -1158,
     673,  8763,   594,   513,  5273,  1255,    84,    15, -1158,  5273,
   -1158, -1158, 16796, -1158,    20, 17305,   451, -1158,   596,   601,
   -1158, 16796,   224,   451, -1158,  8763,  2246,  2246, -1158, -1158,
     290, 19748, 12239, 12239, -1158, 19748,   597, 17412, -1158, 16796,
     224, 19748,   608,  8763, -1158, 19748, 17457, -1158, -1158, -1158,
   -1158,   224,    40, 16796,   598,   193,   193,  1697,   451,   451,
      67, -1158, -1158,  4398,   -23, -1158, 12239, -1158,   -40,   101,
   -1158,   -22,    97, 17503,   -45,  1697,   751, -1158,  4573, 12239,
   -1158, 12239,   695, -1158, -1158, 15204,    58,   542, 19748,   620,
   19718, 10849,   612, -1158,   614,   769,   661,   102, -1158, -1158,
     244,    45, -1158, -1158, -1158, -1158,   697, -1158, -1158, -1158,
     139,   699, -1158, -1158, -1158,  3413,   665,   369, -1158,   391,
     518, -1158, 12239,   643, 12239, 12239, 12239, 10501, 10501, 12239,
     529, 12239, 12239, 12239, 12239, 12239,   586, 14488, 12239, 12239,
   12239, 12239, 12239, 12239, 12239, 12239, 12239, 12239, 12239, 12239,
   12239, 12239, 12239, 12239,   723, -1158, -1158, -1158, -1158, -1158,
    9455,  9455, -1158, -1158, -1158, -1158,  9455, -1158, -1158,  9455,
    9455,  8763,  8763, 10501, 10501, 16836, 16988, 17609,   622,    16,
   19718,  8415, -1158, -1158,  4748, -1158, 10501,    40,  3143, -1158,
   12239, -1158,  2761, -1158, 19718, -1158, -1158, -1158,   673, 12239,
     672, -1158,   625,   653, -1158, -1158, -1158,   754, 10849, -1158,
    5448, 10501, -1158,   631, -1158,    40,  5623, 10501, -1158,    40,
   -1158,    40, 10501, -1158,    40,   680,   681,  5273,   772,  5273,
   -1158,   775, 12239,   755,   644,  8763, 19718, -1158, -1158, -1158,
    1255, -1158, -1158,   107, -1158, 12412,   698, 12239,  3143, -1158,
   -1158, 12239, -1158, 20136, 12239, 12239, -1158,   652, -1158, -1158,
   10849, -1158, 19748, 19748, -1158,    47, -1158,  8763,   656, -1158,
     809,   809, -1158, -1158, -1158, -1158, -1158, -1158,  9805, -1158,
   17655,  8067, -1158,  8242, -1158,  5273,   658, 10501,  9980,  4048,
     662, 12239, 11195, -1158, -1158,   345, -1158,  4923, -1158,   396,
   17695,   406, 16514, 19718,  7371,  7371, -1158,   542,   669, -1158,
     242, -1158,   327,   685,  2451, -1158, -1158, 19718, 12239,   164,
   -1158,    77, -1158, -1158, -1158, -1158, -1158, -1158,   279,   700,
     675, -1158, 15217, -1158,    66, -1158, -1158, 12585,   748, -1158,
     682,   337, -1158,   707, -1158,   710,   712,   721,   714,   716,
   -1158,   717,   728,   725,   726,   727,   341,   731,   729,   730,
   -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158,
   -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158,
   -1158, -1158, -1158, 12239, -1158,   736,   740,   737,   682, -1158,
     682, -1158, 12758, -1158, 19718, -1158, -1158, 17847, 19881, 19881,
     536, -1158,   536, -1158,   536,  3122,  2018,  1402,  1281,   224,
     193, -1158,   551, -1158, -1158, -1158, -1158, -1158,  1697,  2580,
     536,  1531,  1531, 19881,  1531,  1531,  1728,   193,  2580,  2427,
    1728,   451,   451,   193,  1697,   724,   734,   735,   738,   739,
     743,   718,   715, -1158,   536, -1158,   536, 12931, 10501, 13104,
   10501, 12239,  8763, 10501,  3689,   722,   180, -1158, -1158, -1158,
     115, -1158,  1097, 19788,   499,    37, 17892, -1158,   407, 19748,
   -1158, 17932,  8763, -1158,  8763, 12239,   173,  9282, 19748,    44,
   17028,  8415, -1158,  9282, 19748,    43, 16559, -1158, -1158,    32,
   16604, -1158, 12239, 12239,   858,  5273,   861, 18084,  5273, 17068,
   19718, -1158,   205, -1158,   206, -1158,   208,  1255,    84,    49,
      51, 12239, 12239,  7198, -1158, -1158,   197,  8936, -1158, 19748,
   -1158, -1158, -1158, 19718, 18124, 18164, -1158, -1158, 19748,   742,
      75,   732, -1158,  1922, -1158, -1158, -1158, -1158, -1158, -1158,
   -1158, -1158,  5273,   -20, 17220, -1158, -1158, 19748,  5273, 19748,
   -1158, 18316, -1158, -1158, 12239, -1158,    71, 15271, 12239, -1158,
   11368,  7371,  7371, -1158, 12239,   501, 12239,   565,   570, 12239,
   10153,   589,   418, -1158, -1158, -1158, 18356,   765,   749,   746,
   -1158, 19718,  3143, -1158,  8763,   747,  1446, 19718, -1158, 19748,
   19254, -1158, -1158, 19718,   836, -1158,  1255,   756,    29,  7023,
   -1158, -1158,   373, -1158,    29,   244, -1158, 18396, -1158, 15430,
   -1158, -1158, -1158,   432, -1158,   759,   752, -1158, -1158, -1158,
   -1158, -1158, -1158, -1158, -1158, 12239,   897, 18548, 12239,   900,
   18588,   299,   760, 18628,  8763, -1158, -1158,  8415, -1158, -1158,
   -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158, -1158,
      53, 10501, 10501,    40,    40, -1158, -1158, -1158, -1158, -1158,
   16796, -1158, 15589,  5798, -1158,  5973, -1158,   303, -1158, 15665,
    6148, -1158,    40,  6323, -1158,    40, -1158,    40, -1158,    40,
   19748, 19748,  5273, -1158,  5273, 12239, -1158,  5273,   901,   773,
     774, 19718,   585, -1158, -1158,   698,   762,    81, -1158, -1158,
   -1158,   768,   770, -1158,  6498, 10849, -1158, -1158, -1158, 19718,
   -1158,   796,   585, -1158,  6673,   771,  6848,   776, -1158, 12239,
   -1158, -1158,  7371, -1158, 18780,    83, 17260,   435,   779,  7371,
   -1158, 12239, -1158, -1158,  2593,   472,   478,   313, -1158,   940,
   -1158, -1158, -1158,   757, -1158, -1158, -1158, -1158,   756, -1158,
     321, 11543,   223,   148, 12239, -1158,   778, 14315,    89, -1158,
     777, -1158,   814,   815,   682,   682, -1158, -1158, 18820, 13277,
   13450, 18860, 13623, 13796, -1158, 13969, 14142,   324, -1158,   698,
     325,   338, -1158, -1158, -1158,  5273,  9282, 19748,  9282, 19748,
    8415, -1158,  5273,  9282, 19748, -1158,  9282, 19748, -1158, -1158,
   -1158, -1158, -1158, 19748,   914,  5273, -1158, -1158, -1158, -1158,
   -1158, -1158, 10501, 15112,  8763,  8763,  5273, -1158,    23,   784,
   12239, -1158,  9282, -1158, 19748,  5273,  9282, -1158, 19748,  5273,
   19748,   270, 11716,  7371,  7371,  7371,  7371, -1158, -1158, -1158,
   19012, 19748,  2950, -1158, -1158, -1158,   793, -1158,   587, -1158,
     223,   756, 10328, -1158, -1158, -1158,    -8, 10849, -1158, -1158,
   -1158,   109, -1158,   -36, -1158,   289, 19052,     5, -1158, -1158,
     390, 14658,   -21, -1158, -1158, -1158, 12239, 12239, 12239, 12239,
   12239, 12239, 12239, 12239, -1158, -1158, 18084, 15741, 15817, -1158,
   18084, 15893, 15969,  5273, -1158, 19092, -1158,   573,   786,   787,
     638, -1158,   585, 19748, 16045, -1158, 16121, -1158, -1158, -1158,
   19748,   481,   797,   486,   799, 12239, -1158, -1158,   587, -1158,
   -1158, -1158,   223, 11891,    91, -1158, 19748, -1158, 12239,   148,
     109,   109,   109,   109,   109,   109,   148, -1158, -1158, -1158,
   -1158,   490, -1158, -1158, -1158, 19244, 19284, 19324, 19476, 19516,
   19556,  5273,  5273,  5273,  5273, -1158, 14142,   801,   802, -1158,
    5273,  5273, -1158, -1158, -1158, -1158, 19748, -1158, -1158, -1158,
   -1158, -1158, -1158,   135, 10501, 14799, 19748, -1158, -1158, -1158,
   -1158, -1158, -1158, -1158,   821, 12239, 12239, 12239, 12239, 12239,
   12239, 18084, 18084, 18084, 18084,   686, -1158, -1158, 18084, 18084,
   19708, -1158, -1158, 12066, -1158
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   115,   663,   664,   665,   659,
     660,   666,     0,   583,   101,   136,   552,   143,   554,   583,
       0,   142,   460,   458,   101,     0,     0,    46,     0,   262,
     137,   180,   179,   657,     0,     0,     0,   178,     0,   141,
      54,   263,   325,   138,     0,     0,     0,   320,     0,     0,
     145,     0,     0,   602,   574,   667,   146,     0,   326,   546,
     455,     0,     0,     0,   165,   163,   140,   555,   457,     0,
       0,     0,     0,   550,     0,     0,   144,     0,     0,   116,
       0,   658,     0,     0,   447,   139,   300,   548,   459,   147,
       0,     0,   716,     0,   718,     0,   719,   720,   629,     0,
     717,   714,   533,   161,   715,     0,     0,     0,     0,     4,
       0,     5,     9,    10,    43,     0,    48,    57,    11,    63,
      12,    13,    14,    15,    27,   529,   530,    21,    49,   162,
     172,     0,   181,   630,   173,    16,    29,    28,    18,     0,
     257,    17,   621,    20,    33,    30,    31,   171,   301,     0,
     169,     0,     0,   618,   324,     0,   167,   327,   417,   408,
     170,     0,     0,   168,   635,   614,   531,   615,   536,   534,
       0,     0,     0,   619,   620,     0,   535,     0,   636,   637,
     638,   661,   662,   613,   538,   537,   616,   617,     0,    26,
     554,   137,     0,     0,     0,     0,   555,     0,     0,     0,
       0,   618,   635,   534,   619,   620,   544,   535,   636,   637,
       0,     0,   584,   102,     0,   553,     0,   583,   448,     0,
     456,     0,    19,     0,   514,    44,   323,     0,   521,   109,
     117,   129,   123,   122,   131,   112,   121,   132,   118,   133,
     110,   134,   127,   120,   128,   126,   124,   125,   111,   113,
     119,   130,   135,     0,   114,   189,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    56,    55,
      57,   521,     0,     0,     0,     0,     0,     0,   463,     0,
     153,    38,     0,   292,     0,   291,   701,   606,   603,   604,
     605,     0,   547,   702,     7,   521,   323,   323,   429,   511,
       0,   510,     0,     0,   154,   634,     0,     0,    41,     0,
     551,   539,     0,   521,    42,   545,     0,   270,   274,   271,
     274,   549,     0,     0,     0,   703,   705,   627,   700,   699,
       0,    60,    64,     0,     0,   516,     0,   518,     0,     0,
     517,     0,     0,   510,     0,   628,     0,     6,     0,     0,
      58,     0,     0,   460,   177,     0,   659,   301,   631,   185,
       0,     0,     0,   297,     0,   316,   318,     0,   309,   313,
     397,   417,   415,   416,   414,   335,   418,   421,   420,   422,
       0,   412,   409,   410,   413,     0,   452,     0,   449,     0,
     532,    32,     0,   607,     0,     0,     0,     0,     0,     0,
     704,     0,     0,     0,     0,     0,     0,   626,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   367,   374,   375,   376,   371,   373,
       0,     0,   369,   372,   370,   368,     0,   378,   377,     0,
       0,   521,   521,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    34,    22,     0,    35,     0,     0,     0,    23,
       0,    36,   323,   298,     0,   179,   299,    47,    57,     0,
     529,   527,     0,   522,   523,   528,   308,     0,     0,   199,
       0,     0,   198,     0,   207,     0,     0,     0,   205,     0,
     215,     0,     0,   213,     0,     0,     0,     0,   231,     0,
     367,   227,     0,     0,     0,     0,     0,   241,    24,   467,
       0,   468,   470,     0,   492,     0,   473,     0,     0,   152,
      37,     0,   294,     0,     0,     0,    39,     0,   166,   164,
       0,    94,   632,   633,   155,     0,    40,     0,     0,   281,
     272,   273,   192,   191,    25,    62,    61,    65,     0,   668,
       0,     0,   653,     0,   655,     0,     0,     0,     0,     0,
       0,     0,     0,   672,     8,     0,    51,     0,    92,     0,
      88,     0,    72,     0,     0,    78,   174,   301,     0,   187,
       0,   184,   268,   302,     0,   307,   314,     0,     0,   311,
     306,   404,   399,   400,   401,   405,   406,   407,   397,   390,
       0,   387,     0,   398,     0,   419,   487,     0,   423,   411,
     384,   103,   362,   117,   360,   123,   122,   106,   121,   118,
     365,   133,   104,   134,   120,   124,   105,   107,   119,   135,
     359,   341,   344,   342,   343,   366,   354,   345,   358,   350,
     348,   361,   364,   349,   347,   352,   357,   346,   351,   355,
     356,   353,   363,     0,   338,     0,   108,     0,   384,   339,
     384,   336,     0,   451,     0,   446,   462,   582,   696,   695,
     698,   707,   706,   711,   710,   692,   689,   690,   691,   623,
     679,   115,     0,   649,   650,   116,   648,   647,   624,   683,
     694,   688,   686,   697,   687,   685,   677,   682,   684,   693,
     676,   680,   681,   678,   625,     0,     0,     0,     0,     0,
       0,     0,     0,   709,   708,   713,   712,     0,     0,     0,
       0,     0,     0,     0,   670,   268,   594,   595,   597,   599,
       0,   586,     0,     0,     0,   583,   583,   195,     0,   515,
      45,     0,     0,   541,     0,     0,     0,     0,   558,     0,
       0,     0,   208,     0,   564,     0,     0,   206,   216,     0,
       0,   214,     0,     0,   230,     0,   226,     0,     0,     0,
       0,   540,     0,   244,     0,   242,     0,   471,     0,   150,
     151,   149,   148,     0,   491,   490,   614,     0,   465,   612,
     464,   293,   290,     0,     0,     0,   646,   513,   512,     0,
       0,     0,   542,     0,   275,   669,   622,   654,   519,   656,
     520,   223,     0,     0,     0,   671,   221,   568,     0,   674,
     673,     0,    53,    52,     0,    87,     0,     0,     0,    80,
       0,     0,    78,    50,   359,   341,   344,   342,   343,   352,
     351,   353,     0,   379,   380,    67,    66,    79,     0,     0,
     303,     0,     0,   265,     0,     0,   323,     0,   317,   319,
       0,   402,   403,     0,   397,   386,     0,   505,   430,     0,
     486,   485,   614,   424,   430,   397,   334,     0,   340,     0,
     330,   331,   454,   614,   450,     0,     0,   100,    98,    99,
      97,    96,    95,   644,   645,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   596,   598,     0,   585,   136,
     143,   142,   141,   138,   145,   146,   140,   144,   139,   147,
       0,     0,     0,     0,     0,   461,   190,   525,   526,   524,
       0,   225,     0,     0,   197,     0,   196,     0,   589,     0,
       0,   203,     0,     0,   201,     0,   211,     0,   209,     0,
     239,   238,     0,   233,     0,     0,   229,     0,   235,     0,
     267,     0,     0,   469,   472,   473,     0,     0,   474,   475,
     289,     0,     0,   283,     0,     0,   282,   285,   543,     0,
     276,   279,     0,   224,     0,     0,     0,     0,   222,     0,
      93,    90,     0,    89,    75,    74,    73,     0,     0,     0,
     175,     0,   176,   303,   323,     0,     0,     0,   296,   159,
     165,   163,   295,   323,   304,   156,   315,   312,   505,   388,
       0,     0,   473,     0,     0,   321,     0,   477,     0,   328,
       0,   337,   103,   105,   384,   384,   651,   652,     0,     0,
       0,     0,     0,     0,   267,     0,     0,     0,   587,   473,
     635,   635,   194,   193,   200,     0,     0,   557,     0,   556,
       0,   588,     0,     0,   563,   204,     0,   562,   202,   212,
     210,   232,   228,   573,   234,     0,    59,   264,   243,   240,
     466,   496,     0,   670,   521,   521,     0,   287,     0,     0,
       0,   277,     0,   219,   570,     0,     0,   217,   569,     0,
     675,     0,     0,     0,    78,     0,    78,    81,    84,    70,
      69,    68,   323,   182,   188,   186,   269,   157,   323,   305,
     473,   505,     0,   508,   507,   509,   614,   381,   393,   391,
     444,     0,   445,   432,   435,     0,   431,     0,   482,   480,
     614,   670,     0,   385,   332,   333,     0,     0,     0,     0,
       0,     0,     0,     0,   269,   591,   561,     0,     0,   590,
     567,     0,     0,     0,   237,     0,   494,   614,     0,     0,
     288,   286,     0,   280,     0,   220,     0,   218,    91,    77,
      76,     0,     0,     0,     0,     0,   183,   266,   323,   160,
     394,   392,   473,   497,     0,   383,   382,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   427,   428,   322,
     481,   614,   425,   329,   426,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   236,     0,   608,   609,   278,
       0,     0,    83,    86,    82,    85,    71,   158,   396,   395,
     499,   500,   502,   614,     0,   670,   434,   436,   437,   440,
     441,   442,   438,   439,   433,     0,     0,     0,     0,     0,
       0,   560,   559,   566,   565,   614,   610,   611,   572,   571,
       0,   501,   503,   497,   504
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1158, -1158, -1158,     7,  -368,  2362,   159, -1158, -1158, -1158,
    -199, -1158, -1158,   -25,   691, -1158,  -826,  2696,   621,  -536,
   -1158,  -804, -1158, -1158,   146, -1158, -1158, -1158,   947, -1158,
    2822,  -187,  -379, -1158,  -588,  1257,  -611,   -38, -1158, -1158,
    -825, -1158, -1158,    80, -1158, -1158, -1158, -1158, -1158, -1158,
   -1158, -1158, -1158,   161, -1158,   883, -1158, -1158,    17,  2148,
   -1158, -1158, -1158, -1158,   659, -1158,    -3, -1158, -1158, -1158,
   -1158,   460, -1158, -1158, -1158,  -104, -1158,  -327,  -795, -1158,
   -1158, -1158, -1158,   123, -1158, -1158,  -554, -1158,   824, -1158,
   -1158, -1158, -1158,   626, -1158, -1158, -1158, -1158,   105,  -373,
    -165,  -777, -1016,  -614, -1158, -1158,   111,   126,   393, -1158,
   -1158, -1158,   624, -1158, -1158,  -148,    99,   124,  -206,  -197,
    -681, -1158, -1158,   152,   340, -1158,  -209,   874, -1158,  -159,
     488,   230,  -492,  -913,  -958, -1158,  -584,  -645, -1157,  -978,
    -948,   -56, -1158,   175, -1158,  -270,  -489,  -505,   502,  -473,
   -1158, -1158, -1158,   842, -1158,   -14, -1158, -1158,  -227, -1158,
    -730, -1158, -1158, -1158,  1146,  1334,   -12, -1158,    22,  1656,
   -1158,  1849,  1948, -1158, -1158, -1158, -1158, -1158, -1158, -1158,
   -1158, -1158,  -444
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   332,   110,   766,   112,   113,   462,   226,
     114,   115,   116,   117,   352,   118,   119,   120,   333,   847,
     571,   848,   121,   122,   568,   569,   123,   124,   214,   654,
     276,   125,   254,   126,   784,   281,   127,  1014,  1188,  1118,
     128,   297,   296,   129,   130,   131,   132,   133,   134,   580,
     851,   135,   136,   902,   137,   266,   138,   774,   775,   200,
     140,   141,   142,   143,   540,   804,   982,   144,   145,   800,
     977,   283,   284,   146,   147,   148,   149,   362,   856,   150,
     151,   367,   860,   368,   857,   587,   369,   152,   201,  1137,
     154,   155,   156,   157,  1142,   158,   159,   657,   658,   843,
     844,   845,  1128,   876,   371,   599,   600,   601,   602,   603,
     385,   375,   380,   874,  1213,  1209,   471,  1025,  1133,  1134,
    1135,   160,   161,   387,   388,   663,   162,   163,   219,   277,
     278,   512,   513,   788,   871,   608,   516,   785,  1241,  1125,
    1022,   334,   223,   338,   339,   472,   473,   474,   202,   165,
     166,   167,   168,   203,   170,   211,   212,   730,   485,   937,
     731,   732,   171,   172,   204,   205,   175,   359,   475,   207,
     177,   208,   209,   180,   181,   182,   183,   344,   184,   185,
     186,   187,   188
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     206,   504,   598,   389,   215,   216,   656,   734,   217,   109,
     458,   440,   659,   300,   224,   270,   772,   882,   776,   870,
     868,   938,   259,   261,   263,   527,   267,   467,   998,   357,
     578,  1015,   801,   858,   489,   491,   494,   282,   842,   285,
     286,   470,   786,   538,   880,   291,   881,   292,   799,   293,
     993,   342,  1080,   279,   514,  1004,   279,   301,   305,   307,
     309,   310,   311,   517,   723,   279,   315,   514,   316,  1139,
    1120,    70,   376,   279,   681,   321,   940,   933,   323,   305,
     463,   325,  1198,   326,   470,   514,   327,   328,  1271,   329,
    1023,   497,   306,   301,   305,   343,   345,    70,   483,   460,
     483,   562,   502,     3,  1190,  -262,   551,  -263,   470,  1129,
    1199,   372,  1130,   324,   556,   560,  1274,  -506,   461,   355,
    1103,   358,  -284,   548,   377,   861,   470,  -488,   340,  1082,
     563,   373,  -488,   552,   872,   378,  1155,   723,  -506,  1244,
     606,   374,  -506,   449,  1212,   557,   466,  1024,   549,   454,
     974,  1130,   483,   555,   379,   984,  1104,   210,  -488,   454,
     393,   518,   558,   483,  -284,  -506,   521,   862,   299,   530,
      86,   711,   712,  1192,   685,   454,  1238,  1048,  -488,  1015,
     445,   446,   447,  1210,  -488,   315,   301,   343,  1015,   883,
     454,   724,   975,   454,   965,   258,   260,   262,   515,  -488,
    -483,   468,   454,   457,   299,   337,   299,  1191,  1112,  -488,
     454,   515,  1132,   454,   454,  1242,   305,   483,  -284,   687,
     660,   228,  1109,   271,   189,   396,  -488,   558,   905,   515,
    -488,  -483,   189,   901,   287,   558,  -488,   558,   545,   929,
    1131,   317,   319,   558,  -483,  -489,   976,   553,   589,  -488,
     849,  1132,   577,   777,   470,   470,  1083,  -483,  -489,   305,
    -498,   907,   213,   740,  1141,   381,  1245,  1272,   341,    23,
     906,   591,   559,   590,   554,   349,   441,  -245,   442,  1239,
     778,  -498,   397,   305,   607,  -498,   398,  1015,   908,  -489,
     532,   533,   592,  1189,   288,   997,   593,   299,   299,   738,
    1182,   305,  1184,  1016,  -483,   583,   591,   734,  -498,  -483,
    -310,  1207,   289,   350,   793,  -489,  1207,   382,   470,   530,
     594,    60,  -489,   595,   550,    62,   218,   592,   383,   290,
    1159,   593,    68,   400,   596,  -310,  1049,   570,   404,   572,
     547,   406,   351,  -489,  -246,   409,   931,   384,   787,   301,
     470,   744,   961,   597,   777,   594,   -56,    88,   595,  -579,
     346,  -252,  -489,  1237,   400,  1007,  -642,  -489,  -642,   596,
    -489,   852,   406,  1127,  1020,   -56,   528,   529,   960,   962,
     667,   963,   668,   669,   670,   672,   674,   675,   597,   676,
     677,   678,   679,   680,   -56,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   701,   702,
     703,   704,  -251,   220,  1200,  1047,   999,  -389,   305,   305,
    1144,  1145,   746,   -56,   305,   225,   228,   305,   305,   305,
     305,   714,   716,  1123,  -484,   -55,   530,   468,  1166,   733,
    1201,  1178,  1202,  1203,   736,   744,  1204,  1205,   739,  1060,
    1197,  -476,   706,   707,   -55,   531,  1101,   741,   708,   744,
     299,   709,   710,   734,   255,  -484,   301,   777,   748,   750,
     744,  -593,  1044,   -55,   754,   756,  1061,  -109,  -484,  -260,
     760,  -111,  -476,   396,  -592,  -115,  1116,   767,  1013,  -116,
     769,  -484,   853,   305,  1121,  -476,   598,  1154,  -593,   854,
     656,  -643,   -55,  -643,   164,   789,   659,   598,  -476,   285,
     189,  -592,   794,   795,   268,   664,   822,   269,   798,  1248,
    1249,  1250,  1251,  1252,  1253,   305,  1208,   164,   942,   945,
     164,  1208,   947,   949,   665,   470,   798,   518,  -484,   305,
     397,   305,   824,  -484,   398,   814,   798,   817,  1126,   819,
     821,  -478,   828,   518,  1140,  -476,   666,   470,   671,   673,
    -476,   825,   846,   846,   999,  -250,   443,  1181,  -247,  1183,
     444,   829,   925,   340,   547,   340,   859,   299,  -453,  -254,
     164,   999,  -478,  1000,   164,   164,   985,   987,   922,   681,
    -718,   400,   444,   271,  -718,  -478,   404,  -453,   405,   406,
    1107,  -258,  -255,   409,   713,   715,   682,   164,  -478,   463,
    1167,   416,   164,    22,    23,   294,   734,   735,   664,   422,
     683,  -493,   295,   464,   518,    31,   465,   999,  -642,   797,
    -642,    37,   999,  -639,  -493,  -639,  1013,  1114,    42,   991,
     687,   877,   749,  1115,  -249,   313,  1232,   797,   755,   684,
     337,  1234,   337,   759,  -719,  -478,   312,   797,  -719,  -720,
    -478,  -253,  -259,  -720,    58,  -493,    60,   470,  1211,  -261,
    1010,    42,  -248,  1011,  -323,   466,   400,    68,  -715,  -256,
     348,   404,  -715,   454,   406,   506,   360,   361,   409,   685,
    -641,  -493,  -641,  1006,  -323,   686,    84,    58,  -493,    86,
     370,  -640,    88,  -640,  -323,   748,   897,   754,   900,   767,
     305,   903,   817,   164,  1035,  -576,   390,   470,   813,  -493,
    1243,   923,   924,   885,  -575,   886,   808,   967,   810,   579,
     305,   391,   305,   930,  -495,   932,   392,  -580,  -493,   733,
    -581,   939,  -578,  -493,  1013,   450,  -493,  -495,  -577,   453,
     950,   951,   103,  1265,   451,   454,   459,   469,   350,   564,
     476,   573,   534,   544,   928,   478,   505,   502,   524,   311,
     315,   343,  1243,   525,   502,   305,   164,   586,  -495,   463,
     537,   164,   584,    22,    23,   585,   588,   605,  1009,   609,
     662,   406,   705,   464,   722,    31,   465,   742,   743,   744,
    1243,    37,   745,   751,  -495,   762,   763,   765,    42,   969,
     768,  -495,   570,  1028,  1168,  1169,   994,   771,   996,   846,
     846,   770,   325,   787,   326,   796,   153,   328,   329,   802,
     803,   530,  -495,   812,    58,   164,    60,   818,   346,   850,
    1010,   927,   305,  1011,   169,   466,   864,    68,   865,   153,
     164,  -495,   153,   873,   875,  -136,  -495,   343,  -143,  -495,
    -142,  -112,  -141,   591,  -138,  -145,    84,   169,  -110,    86,
     169,  -113,    88,  -146,  -140,  -144,   878,  -139,  -147,   896,
    -114,   899,   299,  1038,   592,   879,  1041,   661,   593,   887,
     894,   893,   305,   952,   904,   733,   954,   470,   470,   888,
     889,  1021,   153,   890,   891,   978,   153,   153,   892,   672,
     714,   999,   594,   973,  1002,   595,  1003,    62,  1008,  1088,
     169,  1057,   103,  1059,   169,   169,   596,  1037,  1064,   153,
    1039,  1067,  1036,  1042,   153,  1045,  1075,  1081,  1076,  1077,
    1084,  1090,  1085,  1073,  1108,   597,  1095,   169,  1117,  1163,
    1143,  1099,   169,  1138,  -109,  -111,   164,  1172,  1187,  1227,
    1228,   503,  1233,   301,  1235,  1266,  1267,  1199,   299,   567,
     990,   221,  1094,   322,  1098,  1119,  1089,  1100,  1078,   541,
     846,   791,   164,  1017,  1034,   581,  1030,  1110,   164,  1111,
    1019,   863,   502,   502,  1214,   604,   502,   502,  1029,   164,
    1254,   164,  1247,  1005,   884,   354,   790,   964,     0,   502,
       0,   502,  1136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1057,  1059,     0,
    1064,  1067,     0,  1094,  1098,   153,     0,     0,     0,     0,
       0,     0,     0,  1156,  1157,     0,  1158,     0,   733,     0,
    1160,  1161,     0,   169,  1162,     0,     0,   164,     0,     0,
       0,   164,     0,     0,     0,     0,  1194,     0,     0,   164,
    1165,   817,   305,   305,   299,     0,     0,     0,  1173,     0,
    1174,     0,   671,   713,  1176,     0,     0,     0,     0,     0,
    1180,   846,   846,   846,   846,     0,     0,     0,   153,     0,
     681,     0,     0,   153,     0,     0,     0,     0,     0,     0,
     343,     0,     0,     0,   909,  1196,   169,   910,     0,     0,
    1124,   169,   911,     0,     0,     0,     0,     0,     0,   817,
       0,     0,     0,   191,  1156,  1215,  1216,  1160,  1217,  1218,
    1219,  1220,     0,   912,     0,     0,     0,     0,   173,     0,
     913,     0,     0,     0,     0,     0,     0,   153,     0,     0,
     914,     0,     0,     0,     0,     0,     0,     0,   915,     0,
       0,   173,   153,  1236,   173,   169,     0,     0,     0,     0,
       0,     0,     0,     0,   916,     0,  1246,     0,     0,     0,
     169,     0,     0,     0,     0,     0,   917,     0,     0,     0,
     685,     0,     0,     0,     0,     0,     0,   918,     0,  1261,
    1262,  1263,  1264,   919,  1098,     0,     0,     0,  1268,  1269,
       0,   299,     0,     0,   173,     0,  1195,     0,   173,   173,
       0,     0,  1270,   817,   920,     0,     0,     0,     0,     0,
       0,     0,     0,  1261,  1262,  1263,  1264,  1268,  1269,     0,
       0,   173,     0,     0,     0,     0,   173,     0,   229,     0,
       0,  1098,     0,     0,     0,     0,     0,   164,     0,     0,
     164,     0,   230,   231,     0,   232,     0,     0,   153,     0,
     233,     0,     0,     0,     0,     0,     0,     0,     0,   234,
       0,     0,  1240,     0,     0,   235,   169,     0,     0,     0,
       0,   236,     0,     0,   153,   237,     0,     0,   238,     0,
     153,     0,     0,   396,   164,     0,     0,     0,   239,     0,
     164,   153,   169,   153,     0,   240,   241,     0,   169,   308,
       0,     0,     0,   242,     0,     0,   174,     0,     0,   169,
       0,   169,   243,     0,  1240,     0,     0,     0,     0,     0,
       0,   244,   245,     0,   246,     0,   247,   173,   248,   174,
       0,   249,   174,     0,     0,   250,   509,     0,   251,     0,
     397,   252,  1240,     0,   398,     0,     0,     0,     0,   153,
       0,     0,     0,   153,     0,     0,     0,     0,     0,     0,
       0,   153,     0,     0,     0,     0,     0,   169,     0,     0,
       0,   169,     0,     0,     0,     0,     0,     0,     0,   169,
       0,     0,   174,     0,     0,     0,   174,   174,     0,     0,
     173,   400,   401,  1050,  1051,   173,   404,   510,   405,   406,
       0,     0,     0,   409,   396,   164,     0,   164,     0,   174,
       0,   416,   164,     0,   174,   164,     0,   420,   421,   422,
       0,     0,     0,     0,   164,     0,   164,     0,     0,   164,
       0,     0,     0,     0,     0,     0,     0,     0,   463,     0,
       0,     0,    22,    23,     0,     0,   164,  1009,     0,   173,
       0,     0,   464,     0,    31,   465,   164,     0,   164,     0,
      37,   397,     0,     0,   173,   398,     0,    42,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   479,   482,   484,   488,   490,
     493,     0,     0,    58,     0,    60,     0,    62,     0,  1010,
       0,     0,  1011,     0,   466,     0,    68,     0,     0,   520,
       0,   522,   400,   401,     0,   174,   403,   404,   526,   405,
     406,     0,     0,     0,   409,    84,     0,   164,    86,     0,
       0,    88,   416,   396,   164,     0,   536,     0,   420,   421,
     422,     0,     0,     0,     0,     0,     0,   164,     0,   542,
     543,     0,     0,     0,     0,     0,     0,     0,   164,   153,
       0,     0,   153,     0,     0,     0,     0,   164,     0,     0,
     173,   164,     0,     0,     0,     0,     0,   169,   174,     0,
     169,   103,     0,   174,     0,     0,     0,  1012,     0,     0,
     397,     0,     0,     0,   398,     0,   173,     0,     0,     0,
       0,     0,   173,     0,     0,     0,   153,     0,     0,     0,
       0,     0,   153,   173,     0,   173,     0,     0,     0,     0,
       0,     0,     0,     0,   169,     0,     0,     0,   176,     0,
     169,     0,     0,     0,     0,   164,     0,   174,     0,     0,
       0,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,   176,   174,   409,   176,     0,     0,     0,     0,     0,
     415,   416,     0,     0,   419,     0,     0,   420,   421,   422,
       0,   173,     0,     0,     0,   173,     0,     0,   423,     0,
       0,     0,     0,   173,   737,     0,     0,     0,     0,     0,
       0,     0,     0,   164,   164,   164,   164,     0,     0,   396,
       0,     0,   164,   164,   176,     0,     0,     0,   176,   176,
       0,     0,   752,     0,     0,     0,   757,     0,   758,     0,
       0,   761,     0,     0,     0,     0,     0,   153,     0,   153,
     396,   176,     0,     0,   153,     0,   176,   153,     0,     0,
       0,     0,     0,     0,     0,   169,   153,   169,   153,     0,
       0,   153,   169,     0,     0,   169,   397,     0,   174,     0,
     398,     0,     0,     0,   169,     0,   169,     0,   153,   169,
       0,     0,     0,     0,     0,     0,     0,     0,   153,     0,
     153,     0,     0,     0,   174,     0,   169,   397,     0,     0,
     174,   398,     0,     0,     0,     0,   169,     0,   169,     0,
       0,   174,     0,   174,     0,     0,     0,   400,   401,     0,
     402,   403,   404,     0,   405,   406,     0,     0,     0,   409,
       0,   178,     0,     0,     0,     0,   415,   416,     0,     0,
     419,     0,     0,   420,   421,   422,     0,   176,   400,   401,
       0,   402,   403,   404,   178,   405,   406,   178,     0,   153,
     409,     0,     0,     0,     0,     0,   153,     0,   416,   174,
       0,     0,     0,   174,   420,   421,   422,   169,     0,   153,
       0,   174,     0,     0,   169,     0,     0,     0,     0,     0,
     153,   173,     0,     0,   173,     0,     0,   169,     0,   153,
       0,     0,     0,   153,     0,   229,     0,   178,   169,     0,
     176,   178,   178,     0,     0,   176,     0,   169,     0,   230,
     231,   169,   232,     0,     0,     0,     0,   233,     0,     0,
     179,     0,     0,     0,   178,     0,   234,     0,   173,   178,
       0,     0,   235,     0,   173,     0,     0,     0,   236,     0,
       0,     0,   237,   179,     0,   238,   179,     0,     0,     0,
       0,     0,     0,     0,     0,   239,     0,   153,     0,   176,
       0,     0,   240,   241,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,   176,   169,   934,   936,     0,   243,
       0,     0,   941,   944,     0,     0,   946,   948,   244,   245,
       0,   246,     0,   247,     0,   248,   179,     0,   249,     0,
     179,   179,   250,     0,     0,   251,     0,     0,   252,     0,
       0,     0,     0,     0,     0,   153,   153,   153,   153,     0,
     396,     0,     0,   179,   153,   153,     0,     0,   179,     0,
     178,     0,     0,   169,   169,   169,   169,     0,     0,     0,
       0,     0,   169,   169,     0,     0,     0,     0,     0,   173,
       0,   173,     0,     0,     0,     0,   173,     0,     0,   173,
       0,     0,   454,     0,   979,     0,     0,     0,   173,   174,
     173,     0,   174,   173,     0,     0,     0,   397,     0,     0,
     176,   398,     0,     0,     0,     0,     0,     0,     0,     0,
     173,     0,     0,   178,     0,     0,     0,     0,   178,     0,
     173,     0,   173,     0,     0,     0,   176,     0,     0,     0,
       0,     0,   176,     0,     0,     0,   174,     0,     0,     0,
     139,     0,   174,   176,     0,   176,     0,     0,   400,   179,
       0,     0,     0,   404,     0,   405,   406,     0,     0,     0,
     409,     0,     0,   139,     0,     0,   139,     0,   416,     0,
    1052,  1053,   178,     0,   420,   421,   422,  1054,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,  1065,
       0,   173,  1068,     0,  1069,     0,  1070,     0,   173,     0,
       0,   176,     0,     0,     0,   176,     0,     0,     0,     0,
       0,   173,   179,   176,     0,     0,   139,   179,     0,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,     0,
       0,   173,     0,     0,     0,   173,     0,     0,     0,     0,
       0,     0,     0,   139,     0,     0,     0,     0,   139,     0,
       0,     0,     0,     0,     0,     0,     0,   174,   463,   174,
       0,     0,    22,    23,   174,     0,     0,   174,     0,     0,
       0,   179,   464,     0,    31,   465,   174,     0,   174,     0,
      37,   174,     0,     0,     0,     0,   179,    42,     0,     0,
       0,     0,     0,   178,     0,     0,     0,     0,   174,   173,
       0,     0,     0,     0,     0,     0,     0,     0,   174,     0,
     174,     0,     0,    58,     0,    60,     0,     0,     0,   178,
       0,     0,     0,     0,   466,   178,    68,     0,     0,     0,
       0,     0,     0,     0,     0,  1171,   178,     0,   178,     0,
       0,     0,     0,     0,     0,    84,     0,     0,    86,   139,
       0,    88,     0,     0,   111,     0,     0,   173,   173,   173,
     173,     0,     0,     0,     0,     0,   173,   173,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   222,     0,   174,
     227,     0,     0,     0,     0,     0,   174,     0,     0,     0,
       0,     0,   179,     0,   178,     0,     0,     0,   178,   174,
       0,     0,     0,     0,     0,     0,   178,     0,     0,     0,
     174,   176,   139,     0,   176,     0,     0,   139,   179,   174,
       0,     0,     0,   174,   179,     0,     0,     0,     0,     0,
     314,     0,     0,     0,     0,   179,     0,   179,     0,     0,
       0,     0,   855,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,     0,   111,   176,     0,
       0,     0,   347,  -303,   176,     0,     0,  -303,  -303,     0,
       0,   139,  -303,     0,     0,     0,     0,  -303,     0,  -303,
    -303,     0,     0,     0,     0,  -303,   139,   174,     0,     0,
       0,     0,  -303,   179,     0,  -303,     0,   179,     0,     0,
       0,     0,     0,     0,     0,   179,   397,     0,     0,     0,
     398,     0,     0,     0,     0,  -303,     0,     0,  -303,     0,
    -303,     0,  -303,     0,  -303,  -303,     0,  -303,     0,  -303,
       0,  -303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,   174,   174,   174,   174,     0,
    -303,     0,     0,  -303,   174,   174,  -303,   400,   401,     0,
     402,   403,   404,   452,   405,   406,   407,     0,   408,   409,
     410,   411,     0,     0,   413,   414,   415,   416,   417,   176,
     419,   176,     0,   420,   421,   422,   176,     0,     0,   176,
       0,     0,   139,     0,   423,     0,     0,     0,   176,     0,
     176,     0,   396,   176,   178,   463,  -303,   178,     0,    22,
      23,     0,  -303,     0,  1009,     0,     0,     0,   139,   464,
     176,    31,   465,     0,   139,     0,   508,    37,     0,     0,
     176,   519,   176,     0,    42,   139,     0,   139,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,     0,     0,     0,     0,     0,   178,     0,   397,
      58,     0,    60,   398,    62,     0,  1010,     0,     0,  1011,
       0,   466,     0,    68,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,     0,     0,     0,
       0,     0,    84,   139,     0,    86,     0,   139,    88,     0,
     111,   176,     0,   179,     0,   139,   179,     0,   176,     0,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
       0,   176,   409,   410,   411,     0,     0,   413,   414,   415,
     416,     0,   176,   419,     0,   280,   420,   421,   422,     0,
       0,   176,     0,     0,     0,   176,     0,   423,   103,     0,
     179,     0,     0,     0,  1113,     0,   179,     0,   280,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   318,   320,
       0,     0,   178,   463,   178,     0,     0,    22,    23,   178,
       0,     0,   178,     0,     0,     0,     0,   464,     0,    31,
     465,   178,     0,   178,     0,    37,   178,     0,     0,     0,
       0,     0,    42,     0,     0,     0,   111,     0,     0,   176,
       0,     0,     0,   178,     0,     0,     0,   -54,     0,     0,
       0,     0,     0,   178,     0,   178,     0,     0,    58,     0,
      60,     0,   519,     0,    64,     0,   -54,    65,   519,   466,
       0,    68,   253,     0,     0,     0,     0,     0,     0,   764,
       0,     0,     0,     0,     0,     0,   272,   273,   274,     0,
      84,     0,     0,    86,     0,     0,    88,   176,   176,   176,
     176,   179,     0,   179,     0,     0,   176,   176,   179,     0,
       0,   179,     0,     0,     0,     0,     0,     0,     0,     0,
     179,     0,   179,     0,   178,   179,     0,     0,     0,     0,
       0,   178,   455,   139,     0,     0,   139,   811,     0,     0,
       0,   816,   179,     0,   178,     0,   103,     0,     0,   111,
       0,     0,   179,     0,   179,   178,     0,     0,     0,     0,
       0,     0,     0,     0,   178,     0,     0,     0,   178,     0,
       0,     0,     0,     0,   280,   280,   280,   280,   280,   280,
     139,     0,   498,   501,     0,     0,   139,     0,     0,   507,
       0,   363,   463,   366,     0,     0,    22,    23,   280,     0,
     280,  1009,     0,   386,     0,     0,   464,   280,    31,   465,
       0,     0,     0,     0,    37,     0,     0,     0,     0,     0,
       0,    42,     0,   179,     0,   280,     0,     0,     0,     0,
     179,     0,   178,     0,     0,     0,     0,   448,   280,   280,
       0,     0,     0,   179,     0,     0,     0,    58,     0,    60,
       0,    62,     0,  1010,   179,     0,  1011,     0,   466,     0,
      68,     0,     0,   179,     0,     0,     0,   179,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    84,
       0,     0,    86,     0,     0,    88,     0,     0,     0,     0,
     178,   178,   178,   178,     0,     0,     0,     0,   477,   178,
     178,   139,     0,   139,     0,     0,   495,   496,   139,     0,
       0,   139,     0,     0,     0,     0,     0,   511,     0,     0,
     139,     0,   139,     0,     0,   139,     0,     0,     0,     0,
       0,   179,     0,     0,     0,   103,     0,     0,     0,     0,
       0,  1186,   139,     0,     0,     0,     0,   953,     0,     0,
     956,     0,   139,     0,   139,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   229,     0,     0,     0,
       0,     0,     0,   280,   396,     0,     0,     0,     0,     0,
     230,   231,     0,   232,     0,     0,     0,     0,   233,   179,
     179,   179,   179,     0,   983,     0,     0,   234,   179,   179,
     988,   280,   582,   235,     0,   280,     0,   280,     0,   236,
     280,     0,     0,   237,     0,     0,   238,     0,     0,     0,
       0,     0,     0,   139,     0,     0,   239,   655,     0,     0,
     139,   397,     0,   240,   241,   398,     0,     0,     0,     0,
       0,   242,     0,   139,     0,     0,     0,     0,     0,     0,
     243,     0,     0,     0,   139,     0,     0,     0,     0,   244,
     245,     0,   246,   139,   247,     0,   248,   139,     0,   249,
       0,     0,     0,   250,     0,     0,   251,     0,     0,   252,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,   725,   408,   409,   410,   411,     0,     0,   413,
     414,   415,   416,   417,     0,   419,   253,     0,   420,   421,
     422,     0,     0,     0,     0,   519,     0,   519,     0,   423,
       0,     0,   519,     0,     0,   519,     0,     0,     0,     0,
       0,   139,     0,     0,  1071,   275,  1072,     0,     0,  1074,
       0,     0,     0,     0,     0,     0,     0,     0,   773,     0,
       0,     0,   511,     0,     0,     0,  1087,     0,     0,     0,
       0,     0,     0,     0,     0,   792,  1093,     0,  1097,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,     0,     0,     0,     0,     0,   139,
     139,   139,   139,     0,     0,     0,     0,     0,   139,   139,
    -600,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   439,     0,   833,  -639,     0,  -639,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   366,
       0,     0,     0,     0,   610,     0,   611,     0,     0,     0,
       0,     0,     0,     0,   867,   612,     0,     0,     0,     0,
     613,   231,   614,   615,     0,     0,     0,  1164,   616,     0,
       0,     0,     0,     0,     0,   280,   280,   234,  1170,   191,
       0,   280,   280,   617,     0,   280,   280,  1175,     0,   618,
       0,  1177,     0,   237,     0,   958,   619,     0,   620,     0,
       0,     0,     0,     0,     0,     0,   621,     0,     0,     0,
       0,     0,     0,   622,   623,     0,   386,     0,     0,     0,
       0,   242,     0,     0,     0,     0,     0,     0,     0,   980,
     624,     0,     0,     0,     0,     0,     0,     0,     0,   244,
     245,     0,   625,     0,   247,     0,   626,     0,     0,   627,
       0,     0,     0,   628,     0,  1225,   251,     0,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,   500,   425,
     426,   427,   428,   429,     0,     0,   432,   433,   434,   435,
       0,   437,   438,   630,   631,   632,   633,   634,   635,     0,
     636,     0,     0,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,   647,     0,   648,     0,     0,   649,
     650,   651,   652,     0,     0,   653,     0,     0,     0,     0,
       0,     0,   959,     0,     0,     0,     0,     0,     0,   511,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   970,     0,     0,     0,   280,
     280,     0,     0,     0,     0,   981,   280,     0,   501,     0,
       0,     0,     0,     0,     0,   501,     0,     0,   280,     0,
       0,   280,     0,   280,     0,   280,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1079,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   386,     0,     0,     0,     0,  1091,   366,
       0,     0,   366,     0,     0,  1018,     0,     0,   511,     0,
       0,     0,     5,     0,     6,     7,     8,     9,    10,    11,
       0,   655,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,   773,   280,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,   981,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   501,   501,     0,     0,   501,   501,     0,
       0,   106,     0,   199,     0,     0,     0,     0,  1229,     0,
     501,     0,   501,    -2,     4,     0,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -54,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -54,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -323,   -54,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -54,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
    -670,     0,    12,    13,    14,    15,    16,  -670,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
    -670,    28,    29,  -670,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -54,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -54,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -54,    65,    66,    67,  -670,    68,    69,
      70,  -670,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -54,    88,    89,     0,     0,    90,     0,    91,
       0,     0,  -670,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -670,  -670,
      95,  -670,  -670,  -670,  -670,  -670,  -670,  -670,     0,  -670,
    -670,  -670,  -670,  -670,     0,  -670,  -670,  -670,  -670,  -670,
    -670,  -670,  -670,   103,  -670,  -670,  -670,     0,   105,  -670,
     106,     0,   107,     0,   330,  -670,     5,   298,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -54,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -54,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -323,   -54,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -54,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   331,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -54,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -54,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -323,   -54,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -54,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,   546,
     106,     0,   107,     0,   565,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -54,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -54,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -323,   -54,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -54,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   566,   106,     0,   107,     0,   330,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -54,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -54,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,    62,
      63,    64,  -323,   -54,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -54,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,   331,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -54,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -54,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,    62,    63,    64,  -323,   -54,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -54,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,   823,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -54,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -54,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,   346,
      63,    64,  -323,   -54,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -54,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,    38,    39,
     -54,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -54,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -54,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -54,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,   747,    39,   -54,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -54,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -54,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -54,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,   753,    39,
     -54,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -54,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -54,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -54,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1056,    39,   -54,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -54,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -54,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -54,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1058,    39,
     -54,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -54,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -54,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -54,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1063,    39,   -54,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -54,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -54,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -54,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1066,    39,
     -54,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -54,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -54,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -54,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,  1086,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,    38,    39,   -54,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -54,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -54,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -54,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     4,   108,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,    13,    14,
      15,    16,     0,    17,     0,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,     0,    28,    29,     0,    30,
       0,    31,    32,    33,    34,    35,    36,    37,  1092,    39,
     -54,     0,    40,    41,    42,     0,    43,  -323,     0,    44,
      45,    46,    47,    48,     0,    49,    50,    51,    52,   -54,
      53,    54,     0,    55,    56,    57,     0,  -323,     0,     0,
      58,    59,    60,    61,     0,    63,    64,  -323,   -54,    65,
      66,    67,     0,    68,    69,    70,     0,    71,    72,    73,
      74,    75,    76,    77,    78,     0,    79,    80,     0,    81,
      82,    83,    84,    85,     0,    86,    87,   -54,    88,    89,
       0,     0,    90,     0,    91,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   103,     0,
       0,   104,     0,   105,     0,   106,     0,   107,     0,     4,
     108,     5,     0,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,    13,    14,    15,    16,     0,    17,     0,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
       0,    28,    29,     0,    30,     0,    31,    32,    33,    34,
      35,    36,    37,  1096,    39,   -54,     0,    40,    41,    42,
       0,    43,  -323,     0,    44,    45,    46,    47,    48,     0,
      49,    50,    51,    52,   -54,    53,    54,     0,    55,    56,
      57,     0,  -323,     0,     0,    58,    59,    60,    61,     0,
      63,    64,  -323,   -54,    65,    66,    67,     0,    68,    69,
      70,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,    82,    83,    84,    85,     0,
      86,    87,   -54,    88,    89,     0,     0,    90,     0,    91,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   103,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,  1026,   108,     5,   298,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,   194,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   196,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   197,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,  1027,   966,
     108,     5,   298,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,   612,     0,    12,     0,     0,    15,    16,
     614,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,   620,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,   500,   425,   426,   427,
     428,   429,     0,     0,   432,   433,   434,   435,     0,   437,
     438,   834,   835,   836,   837,   838,   635,     0,   636,     0,
      98,     0,   637,   638,   639,   640,   641,   642,   643,   644,
     839,   646,   647,   100,   840,   102,     0,   649,   650,   841,
     652,   198,     0,   106,     0,   199,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,    13,
       0,    15,    16,     0,    17,     0,   190,    19,    20,    21,
       0,     0,     0,     0,    26,     0,     0,    28,    29,     0,
     191,     0,     0,     0,    33,    34,    35,    36,     0,    38,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,    49,    50,    51,    52,
       0,    53,    54,     0,    55,    56,    57,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,    71,    72,
      73,    74,    75,    76,    77,    78,     0,    79,    80,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,    90,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   105,     0,   106,     0,   107,     0,
       0,   108,     5,   298,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,   302,   303,     0,    85,
     335,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,   336,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,   353,    23,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,    60,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,    68,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,    88,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     0,   108,
       5,   298,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,   302,   303,     0,    85,   335,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
     807,   199,     0,     0,   108,     5,   298,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,   302,
     303,     0,    85,   335,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,   809,   199,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,   726,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,   727,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,   728,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
     729,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,   302,   303,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,   304,     0,     0,   104,     0,   198,     0,
     106,     0,   199,     0,     0,   108,     5,   298,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,   194,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   196,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   197,     0,    79,     0,     0,    81,
     302,   303,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,   968,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,   302,   303,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,   256,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,   257,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,   264,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,   265,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,   302,   303,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     0,   108,     5,   298,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,   194,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,   257,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     198,     0,   106,     0,   199,     0,     0,   108,     5,   298,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,   805,   106,     0,   199,
       0,     0,   108,     5,   298,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,   194,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     198,     0,   106,     0,   199,   815,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,   194,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   196,  -714,     0,     0,     0,  -714,     0,     0,    73,
      74,    75,    76,    77,   197,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,     0,     0,
     108,     5,   298,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,  1193,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,   257,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   198,     0,   106,     0,   199,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   190,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   279,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   194,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   105,     0,   106,     0,   199,     0,
       0,   108,     5,   298,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
     356,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,    18,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,    86,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
     820,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,   995,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,     0,     0,   108,     5,   298,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,   779,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,   194,    39,
       0,     0,     0,   780,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   196,     0,     0,     0,     0,     0,     0,     0,    73,
     781,    75,    76,    77,   782,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,  1122,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,     0,     0,     0,    85,  1179,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     0,   108,     5,   298,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   198,     0,   106,     0,  1122,     0,     0,   108,     5,
     298,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,  1153,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,   779,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,   780,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,   781,
      75,    76,    77,   782,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   783,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,   779,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,   780,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,   781,    75,    76,    77,   782,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   869,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   783,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   895,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   198,     0,   106,     0,   199,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   190,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   898,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   198,     0,   106,     0,   199,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,  1147,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   199,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,  1148,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     198,     0,   106,     0,   199,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,    12,     0,     0,
      15,    16,     0,    17,     0,   190,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   191,
       0,     0,     0,    33,   192,   193,     0,     0,  1150,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   195,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   196,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   197,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,  1151,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,  1152,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,  1153,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,     0,   199,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   190,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,     0,     0,   106,     0,   869,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
       0,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     5,   199,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,  -479,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
    -479,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,  -479,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,  -479,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      95,     0,     5,   298,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   190,   102,  -479,    21,     0,     0,     0,  -479,     0,
     106,     0,   869,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,     0,  -497,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,  -497,     0,     0,     0,  -497,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   102,     0,     0,     0,     0,     0,   198,
       0,   106,  -497,  1122,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,     0,    95,     0,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   190,   102,     0,    21,     0,     0,
       0,   198,     0,   106,     0,   199,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,   394,     0,    81,     0,
     229,     0,    85,   395,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,   230,   231,   396,   232,     0,     0,
       0,   574,   233,     0,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,    95,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,   237,     0,     0,
     238,     0,     0,     0,   681,     0,   102,   575,     0,     0,
     239,     0,   198,   612,   106,     0,   783,   240,   241,     0,
     614,   682,     0,   397,     0,   242,     0,   398,     0,     0,
       0,     0,     0,     0,   243,   683,     0,     0,     0,     0,
       0,     0,     0,   244,   245,     0,   246,     0,   247,     0,
     248,     0,     0,   249,     0,     0,   620,   250,   399,     0,
     251,     0,     0,   252,   684,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,     0,   576,
     420,   421,   422,     0,   685,     0,     0,     0,     0,     0,
     686,   423,     0,     0,     0,     0,     0,     0,     0,   866,
       0,     0,     0,     0,     0,     0,   500,   425,   426,   427,
     428,   429,     0,     0,   432,   433,   434,   435,     0,   437,
     438,   630,   631,   632,   633,   634,   635,     0,   636,     0,
       0,     0,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,  1032,   648,     0,     0,   649,   650,   651,
     652,   992,   612,     0,     0,     0,     0,   230,   231,   614,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,     0,
     617,     0,     0,     0,     0,     0,   236,     0,     0,     0,
     237,     0,     0,   238,     0,   620,     0,     0,     0,     0,
       0,     0,     0,   239,     0,     0,     0,     0,     0,     0,
     622,   241,     0,     0,     0,     0,     0,     0,   242,     0,
       0,     0,     0,     0,     0,     0,     0,   243,     0,     0,
       0,     0,     0,     0,     0,   394,   244,   245,     0,   246,
       0,   247,   395,  1033,     0,     0,   627,     0,     0,     0,
     250,     0,     0,   251,     0,   396,   252,     0,     0,     0,
       0,     0,     0,     0,     0,   500,   425,   426,   427,   428,
     429,     0,     0,   432,   433,   434,   435,     0,   437,   438,
     630,   631,   632,   633,   634,   635,     0,   636,     0,     0,
       0,   637,   638,   639,   640,   641,   642,   643,   644,   645,
     646,   647,     0,   648,     0,     0,   649,   650,   651,   652,
       0,   394,   397,     0,     0,     0,   398,     0,   395,     0,
       0,     0,     0,     0,     0,   499,     0,     0,     0,     0,
       0,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   500,   425,
     426,   427,   428,   429,     0,     0,   432,   433,   434,   435,
       0,   437,   438,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,   411,   412,     0,
     413,   414,   415,   416,   417,   418,   419,   394,   397,   420,
     421,   422,   398,   454,   395,     0,     0,     0,     0,     0,
     423,  1055,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   500,   425,   426,   427,   428,   429,
       0,     0,   432,   433,   434,   435,     0,   437,   438,   400,
     401,     0,   402,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,   411,   412,     0,   413,   414,   415,   416,
     417,   418,   419,   394,   397,   420,   421,   422,   398,   454,
     395,     0,     0,     0,     0,     0,   423,  1062,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     500,   425,   426,   427,   428,   429,     0,     0,   432,   433,
     434,   435,     0,   437,   438,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,   411,
     412,     0,   413,   414,   415,   416,   417,   418,   419,   394,
     397,   420,   421,   422,   398,   454,   395,     0,     0,     0,
       0,     0,   423,  1221,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   500,   425,   426,   427,
     428,   429,     0,     0,   432,   433,   434,   435,     0,   437,
     438,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,     0,   408,   409,   410,   411,   412,     0,   413,   414,
     415,   416,   417,   418,   419,   394,   397,   420,   421,   422,
     398,   454,   395,     0,     0,     0,     0,     0,   423,  1222,
       0,     0,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,   500,   425,   426,   427,   428,   429,     0,     0,
     432,   433,   434,   435,     0,   437,   438,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,   411,   412,     0,   413,   414,   415,   416,   417,   418,
     419,   394,   397,   420,   421,   422,   398,   454,   395,     0,
       0,     0,     0,     0,   423,  1223,     0,     0,     0,     0,
       0,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,   500,   425,
     426,   427,   428,   429,     0,     0,   432,   433,   434,   435,
       0,   437,   438,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,   411,   412,     0,
     413,   414,   415,   416,   417,   418,   419,   394,   397,   420,
     421,   422,   398,   454,   395,     0,     0,     0,     0,     0,
     423,  1224,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,   500,   425,   426,   427,   428,   429,
       0,     0,   432,   433,   434,   435,     0,   437,   438,   400,
     401,     0,   402,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,   411,   412,     0,   413,   414,   415,   416,
     417,   418,   419,   394,   397,   420,   421,   422,   398,   454,
     395,     0,     0,     0,     0,     0,   423,  1230,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
     500,   425,   426,   427,   428,   429,     0,     0,   432,   433,
     434,   435,     0,   437,   438,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,   411,
     412,     0,   413,   414,   415,   416,   417,   418,   419,     0,
     397,   420,   421,   422,   398,   454,     0,     0,     0,     0,
       0,     0,   423,  1231,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,   500,   425,   426,   427,
     428,   429,     0,     0,   432,   433,   434,   435,     0,   437,
     438,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,     0,   408,   409,   410,   411,   412,     0,   413,   414,
     415,   416,   417,   418,   419,   394,     0,   420,   421,   422,
       0,   454,   395,     0,     0,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,   396,   486,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   487,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,   279,     0,     0,     0,     0,
       0,     0,   397,     0,     0,     0,   398,     0,     0,     0,
     492,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   483,   396,   480,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     481,   397,     0,   400,   401,   398,   402,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,   411,   412,     0,
     413,   414,   415,   416,   417,   418,   419,     0,     0,   420,
     421,   422,   483,   454,     0,     0,   399,     0,     0,     0,
     423,   397,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,   399,     0,   420,   421,
     422,     0,   454,     0,     0,     0,     0,     0,     0,   423,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,   394,   830,   420,   421,
     422,     0,   454,   395,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,   831,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,     0,   832,     0,     0,
       0,   396,   943,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   396,   279,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   397,     0,
       0,     0,   398,     0,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,     0,   483,
     420,   421,   422,   399,     0,     0,     0,     0,     0,     0,
       0,   423,     0,   397,     0,     0,     0,   398,     0,   400,
     401,     0,   402,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,   411,   412,     0,   413,   414,   415,   416,
     417,   418,   419,     0,   483,   420,   421,   422,   399,   454,
       0,     0,     0,     0,     0,     0,   423,     0,     0,     0,
       0,     0,     0,     0,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,   394,     0,
     420,   421,   422,     0,   454,   395,     0,     0,     0,     0,
       0,   423,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   456,     0,     0,     0,   394,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   396,   279,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   397,     0,     0,   394,   398,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   396,   717,
       0,     0,     0,     0,     0,     0,   210,     0,     0,     0,
     399,     0,     0,     0,   718,   397,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     399,     0,   420,   421,   422,   397,     0,     0,     0,   398,
       0,     0,     0,   423,     0,     0,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     399,     0,   420,   421,   422,     0,   454,     0,     0,     0,
       0,     0,     0,   423,     0,     0,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     394,     0,   420,   421,   422,     0,     0,   395,     0,     0,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
     396,   719,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   720,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,   935,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   397,     0,     0,
     394,   398,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,   397,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   399,     0,   420,   421,   422,   397,     0,     0,
       0,   398,     0,     0,     0,   423,     0,     0,   400,   401,
     957,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   399,     0,   420,   421,   422,     0,   454,     0,
       0,     0,     0,     0,     0,   423,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   394,     0,   420,   421,   422,     0,   454,   395,
       0,     0,     0,     0,     0,   423,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,  1105,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
       0,     0,     0,   398,     0,     0,     0,   394,   523,     0,
       0,     0,     0,     0,   395,     0,     0,     0,     0,     0,
       0,     0,     0,  1106,     0,     0,     0,   396,     0,     0,
     483,     0,     0,     0,   399,     0,     0,     0,     0,   397,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,   399,     0,   420,   421,   422,     0,
       0,     0,     0,     0,   397,   986,     0,   423,   398,     0,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,   394,     0,   420,   421,   422,   399,
       0,   395,     0,     0,     0,     0,     0,   423,     0,     0,
       0,     0,     0,     0,   396,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,   411,
     412,     0,   413,   414,   415,   416,   417,   418,   419,   394,
       0,   420,   421,   422,     0,     0,   395,     0,     0,     0,
       0,     0,   423,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   394,     0,     0,     0,     0,
       0,     0,   395,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   396,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,     0,     0,     0,
     398,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,     0,     0,   420,   421,
     422,   399,   535,     0,     0,     0,     0,     0,     0,   423,
       0,     0,   397,     0,     0,     0,   398,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,   411,   412,     0,   413,   414,   415,   416,   417,   418,
     419,   394,   539,   420,   421,   422,   561,   399,   395,     0,
       0,     0,     0,     0,   423,     0,     0,     0,     0,     0,
       0,   396,     0,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,   411,   412,     0,
     413,   414,   415,   416,   417,   418,   419,   394,     0,   420,
     421,   422,     0,     0,   395,     0,     0,     0,     0,     0,
     423,     0,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   397,     0,
       0,     0,   398,     0,     0,     0,     0,   394,   826,     0,
       0,   721,     0,     0,   395,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   397,     0,     0,     0,   398,   400,
     401,     0,   402,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,   411,   412,     0,   413,   414,   415,   416,
     417,   418,   419,     0,     0,   420,   421,   422,     0,   399,
       0,     0,     0,     0,   397,     0,   423,     0,   398,     0,
       0,     0,     0,     0,     0,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,   411,
     412,     0,   413,   414,   415,   416,   417,   418,   419,   399,
       0,   420,   421,   422,     0,     0,     0,     0,   806,     0,
       0,     0,   423,     0,     0,   400,   401,     0,   402,   403,
     404,     0,   405,   827,   407,     0,   408,   409,   410,   411,
     412,     0,   413,   414,   415,   416,   417,   418,   419,   394,
       0,   420,   421,   422,     0,     0,   395,     0,     0,     0,
       0,     0,   423,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,     0,     0,     0,
     398,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,     0,     0,     0,   398,     0,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,   411,   412,  -601,   413,   414,   415,   416,   417,   418,
     419,     0,   210,   420,   421,   422,   399,     0,     0,     0,
       0,   397,     0,     0,   423,   398,     0,     0,     0,     0,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,   399,     0,   420,   421,
     422,     0,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,   394,   926,   420,   421,
     422,     0,     0,   395,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,   396,     0,     0,   955,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   394,     0,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,     0,     0,   394,   398,     0,     0,
       0,     0,     0,   395,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,   397,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,   399,     0,
     420,   421,   422,   397,     0,     0,     0,   398,     0,     0,
       0,   423,     0,     0,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,   399,     0,
     420,   421,   422,     0,     0,     0,     0,   971,     0,     0,
       0,   423,     0,     0,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,   394,     0,
     420,   421,   422,     0,     0,   395,     0,   972,     0,     0,
       0,   423,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   394,  1001,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   397,     0,     0,   394,   398,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   989,
     399,     0,     0,     0,     0,   397,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     399,     0,   420,   421,   422,   397,     0,     0,     0,   398,
       0,     0,     0,   423,     0,     0,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     399,     0,   420,   421,   422,     0,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     394,     0,   420,   421,   422,     0,     0,   395,     0,  1031,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
     396,  1040,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,  1043,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   397,     0,     0,
     394,   398,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,   397,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   399,     0,   420,   421,   422,   397,     0,     0,
       0,   398,     0,     0,     0,   423,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   399,     0,   420,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,   423,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   394,  1102,   420,   421,   422,     0,     0,   395,
       0,     0,     0,  1046,     0,   423,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   394,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
       0,     0,   394,   398,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,   397,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
     400,   401,  1146,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,   399,     0,   420,   421,   422,   397,
       0,     0,     0,   398,     0,     0,     0,   423,     0,     0,
     400,   401,  1149,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,   399,     0,   420,   421,   422,     0,
       0,     0,     0,     0,     0,     0,     0,   423,     0,     0,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,   394,  1185,   420,   421,   422,     0,
       0,   395,     0,     0,     0,     0,     0,   423,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   394,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,     0,     0,   394,   398,     0,     0,     0,     0,
       0,   395,     0,  1206,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,   397,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,   399,     0,   420,   421,
     422,   397,     0,     0,     0,   398,     0,     0,     0,   423,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,   399,     0,   420,   421,
     422,     0,     0,     0,     0,     0,     0,     0,     0,   423,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,   394,   229,   420,   421,
     422,     0,     0,   395,     0,     0,     0,  1226,     0,   423,
       0,   230,   231,     0,   232,     0,   396,     0,     0,   233,
       0,     0,     0,     0,     0,   365,     0,     0,   234,     0,
       0,     0,     0,     0,   235,     0,   394,     0,     0,     0,
     236,     0,     0,   395,   237,     0,     0,   238,     0,     0,
       0,     0,     0,     0,     0,     0,   396,   239,     0,     0,
       0,     0,     0,     0,   240,   241,     0,     0,     0,     0,
       0,     0,   242,   397,     0,     0,   394,   398,     0,     0,
       0,   243,     0,   395,     0,     0,  1255,     0,     0,     0,
     244,   245,     0,   246,     0,   247,   396,   248,     0,     0,
     249,     0,     0,     0,   250,     0,     0,   251,   399,     0,
     252,     0,     0,   397,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,   400,   401,  1256,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,   399,     0,
     420,   421,   422,   397,     0,     0,     0,   398,     0,     0,
       0,   423,     0,     0,   400,   401,  1257,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,   399,     0,
     420,   421,   422,     0,     0,     0,     0,     0,     0,     0,
       0,   423,     0,     0,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,   394,  -298,
     420,   421,   422,     0,     0,   395,     0,     0,     0,     0,
       0,   423,     0,  -298,  -298,     0,  -298,     0,   396,     0,
       0,  -298,     0,     0,     0,     0,     0,     0,     0,     0,
    -298,     0,     0,     0,     0,     0,  -298,     0,   394,     0,
       0,     0,  -298,     0,     0,   395,  -298,     0,     0,  -298,
       0,     0,     0,     0,     0,     0,     0,     0,   396,  -298,
       0,     0,     0,     0,     0,     0,  -298,  -298,     0,     0,
       0,     0,     0,     0,  -298,   397,     0,     0,   394,   398,
       0,     0,     0,  -298,     0,   395,     0,     0,  1258,     0,
       0,     0,  -298,  -298,     0,  -298,     0,  -298,   396,  -298,
       0,     0,  -298,     0,     0,     0,  -298,     0,     0,  -298,
     399,     0,  -298,     0,     0,   397,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,   400,   401,  1259,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     399,     0,   420,   421,   422,   397,     0,     0,     0,   398,
       0,     0,     0,   423,     0,     0,   400,   401,  1260,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     399,     0,   420,   421,   422,     0,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     394,   229,   420,   421,   422,     0,     0,   395,     0,     0,
       0,     0,     0,   423,     0,   230,   231,     0,   232,     0,
     396,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,     0,   234,     0,     0,     0,     0,     0,   235,     0,
     394,     0,     0,     0,   236,     0,     0,   395,   237,     0,
       0,   238,     0,     0,     0,     0,     0,     0,     0,     0,
     396,   239,     0,     0,     0,     0,     0,     0,   240,   241,
       0,     0,     0,     0,     0,     0,   242,   397,     0,     0,
     394,   398,     0,     0,     0,   243,     0,   395,     0,     0,
       0,     0,     0,     0,   244,   245,     0,   246,     0,   247,
     396,   248,     0,     0,   249,     0,     0,     0,   250,     0,
       0,   251,   399,     0,   252,     0,     0,   397,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   399,     0,   420,   421,   422,   921,     0,     0,
       0,   398,     0,  1273,     0,   423,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   399,   396,   420,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,   423,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,     0,     0,   420,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,   423,     0,     0,     0,     0,
     397,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,     0,   408,   409,   410,   411,     0,     0,   413,   414,
     415,   416,   417,   418,   419,     0,     0,   420,   421,   422,
     364,     0,   229,     0,     0,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,   230,   231,     0,   232,
       0,     0,     0,     0,   233,     0,     0,     0,     0,     0,
     365,     0,     0,   234,     0,     0,     0,     0,     0,   235,
       0,     0,     0,     0,     0,   236,     0,     0,     0,   237,
       0,     0,   238,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   239,     0,     0,     0,     0,     0,     0,   240,
     241,     0,     0,     0,     0,     0,     0,   242,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,   229,
       0,     0,     0,     0,     0,   244,   245,     0,   246,     0,
     247,     0,   248,   230,   231,   249,   232,     0,     0,   250,
       0,   233,   251,    23,     0,   252,     0,     0,     0,     0,
     234,     0,     0,     0,     0,     0,   235,     0,     0,     0,
       0,     0,   236,     0,     0,     0,   237,     0,     0,   238,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   239,
       0,     0,     0,     0,     0,     0,   240,   241,     0,     0,
       0,     0,     0,     0,   242,    60,     0,     0,     0,     0,
       0,     0,     0,   243,     0,     0,    68,     0,     0,     0,
       0,     0,   244,   245,     0,   246,     0,   247,     0,   248,
    -299,     0,   249,     0,     0,     0,   250,     0,     0,   251,
       0,    88,   252,     0,  -299,  -299,     0,  -299,     0,     0,
       0,     0,  -299,     0,     0,     0,     0,     0,     0,     0,
       0,  -299,     0,     0,     0,     0,     0,  -299,     0,     0,
       0,     0,     0,  -299,     0,     0,     0,  -299,     0,     0,
    -299,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -299,     0,     0,     0,     0,     0,     0,  -299,  -299,     0,
       0,     0,     0,     0,     0,  -299,     0,     0,     0,     0,
       0,     0,     0,     0,  -299,     0,     0,     0,     0,     0,
       0,     0,     0,  -299,  -299,     0,  -299,     0,  -299,     0,
    -299,     0,     0,  -299,     0,     0,     0,  -299,     0,     0,
    -299,     0,     0,  -299
};

static const yytype_int16 yycheck[] =
{
      12,   271,   370,   162,    16,    19,   385,   451,    20,     2,
     219,   176,   385,    69,    26,    40,   505,   662,   510,   607,
     604,   751,    34,    35,    36,   295,    38,   226,   832,   133,
     357,   856,   537,   587,   261,   262,   263,    49,   574,    51,
      52,   228,   515,   313,   658,    57,   660,    59,     1,    61,
     827,   107,   965,    33,     1,   850,    33,    69,    70,    71,
      72,    73,    74,    48,    48,    33,    78,     1,    80,  1027,
    1018,    92,    27,    33,     3,    87,    33,    33,    90,    91,
      22,    93,   118,    95,   271,     1,    98,    99,  1245,   101,
      61,   102,    70,   105,   106,   107,   108,    92,   120,   146,
     120,   146,   267,     0,  1120,    56,   146,    56,   295,  1022,
     146,    54,     3,    91,   341,   342,  1273,   125,   165,   131,
      37,   133,    75,   146,    79,    48,   313,    61,   106,    48,
     175,    74,    48,   173,   607,    90,  1049,    48,   146,    48,
       1,    84,   150,   199,   165,    48,    88,   118,   171,   170,
      75,     3,   120,   175,   109,   175,    73,   120,    92,   170,
     172,   146,   146,   120,   117,   173,   146,    90,    69,   146,
     112,   441,   442,  1121,   103,   170,  1192,   907,   125,  1004,
     192,   193,   194,  1141,   118,   197,   198,   199,  1013,   662,
     170,   175,   117,   170,   778,    34,    35,    36,   145,   146,
      61,   226,   170,   217,   105,   106,   107,  1120,  1003,   125,
     170,   145,   103,   170,   170,  1193,   228,   120,   171,   406,
     385,   172,   999,   172,   165,    32,   173,   146,    48,   145,
     146,    92,   165,   722,    18,   146,   170,   146,   171,   744,
      92,    82,    83,   146,   105,    48,   171,   146,   146,   165,
     577,   103,   356,   146,   441,   442,   175,   118,    61,   271,
     125,   146,     3,   462,   175,    27,   175,  1245,   107,    27,
      90,    27,   175,   171,   173,    47,   172,    56,   174,  1192,
     173,   146,    89,   295,   145,   150,    93,  1112,   173,    92,
     302,   303,    48,  1118,    78,   831,    52,   198,   199,   458,
    1104,   313,  1106,   857,   165,   361,    27,   751,   173,   170,
     146,  1137,    96,    85,   523,   118,  1142,    79,   505,   146,
      76,    79,   125,    79,   336,    81,   109,    48,    90,   113,
    1060,    52,    90,   140,    90,   171,   920,   349,   145,   351,
     333,   148,   114,   146,    56,   152,   173,   109,   125,   361,
     537,   146,   146,   109,   146,    76,    47,   115,    79,   165,
      81,    56,   165,  1188,   140,   854,   172,   170,   174,    90,
     173,   580,   148,   150,   866,    66,   296,   297,   173,   173,
     392,   173,   394,   395,   396,   397,   398,   399,   109,   401,
     402,   403,   404,   405,    85,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,    56,    90,   125,   904,   146,   173,   430,   431,
    1034,  1035,   478,   114,   436,     8,   172,   439,   440,   441,
     442,   443,   444,  1021,    61,    47,   146,   462,  1083,   451,
     151,   171,   153,   154,   456,   146,   157,   158,   460,   146,
    1131,    61,   430,   431,    66,   165,   992,   469,   436,   146,
     361,   439,   440,   907,    11,    92,   478,   146,   480,   481,
     146,   146,   173,    85,   486,   487,   173,   140,   105,    56,
     492,   140,    92,    32,   146,   148,   173,   499,   856,   148,
     502,   118,   165,   505,   173,   105,   864,   173,   173,   172,
     879,   172,   114,   174,     2,   517,   879,   875,   118,   521,
     165,   173,   524,   525,    83,   146,   171,    86,   530,  1200,
    1201,  1202,  1203,  1204,  1205,   537,  1137,    25,   755,   756,
      28,  1142,   759,   760,   165,   722,   548,   146,   165,   551,
      89,   553,   146,   170,    93,   557,   558,   559,  1021,   561,
     562,    61,   146,   146,  1027,   165,   165,   744,   397,   398,
     170,   165,   574,   575,   146,    56,    89,  1103,    56,  1105,
      93,   165,   165,   551,   567,   553,   588,   478,   146,    56,
      78,   146,    92,   165,    82,    83,   813,   814,    89,     3,
      89,   140,    93,   172,    93,   105,   145,   165,   147,   148,
     165,    56,    56,   152,   443,   444,    20,   105,   118,    22,
    1083,   160,   110,    26,    27,     8,  1060,   456,   146,   168,
      34,    48,   172,    36,   146,    38,    39,   146,   172,   530,
     174,    44,   146,   172,    61,   174,  1004,   165,    51,   826,
     827,   653,   481,   165,    56,   172,   165,   548,   487,    63,
     551,   165,   553,   492,    89,   165,   100,   558,    93,    89,
     170,    56,    56,    93,    77,    92,    79,   854,  1141,    56,
      83,    51,    56,    86,    54,    88,   140,    90,    89,    56,
     170,   145,    93,   170,   148,   172,    56,   145,   152,   103,
     172,   118,   174,   852,    74,   109,   109,    77,   125,   112,
     172,   172,   115,   174,    84,   717,   718,   719,   720,   721,
     722,   723,   724,   211,   879,   165,   164,   904,   557,   146,
    1193,   735,   736,   172,   165,   174,   551,   783,   553,   109,
     742,   165,   744,   745,    48,   747,   156,   165,   165,   751,
     165,   753,   165,   170,  1112,    56,   173,    61,   165,   165,
     762,   763,   165,  1226,   172,   170,   165,   119,    85,     8,
     170,    66,   165,   165,   742,   172,   172,   932,   172,   781,
     782,   783,  1245,   172,   939,   787,   274,     8,    92,    22,
     172,   279,   170,    26,    27,   171,   125,    90,    31,    90,
     125,   148,    69,    36,   172,    38,    39,   125,   173,   146,
    1273,    44,    48,   172,   118,   125,   125,    35,    51,   787,
      35,   125,   824,   869,  1084,  1085,   828,   173,   830,   831,
     832,    66,   834,   125,   836,   173,     2,   839,   840,   173,
      21,   146,   146,   175,    77,   333,    79,   175,    81,   170,
      83,   742,   854,    86,     2,    88,   146,    90,   173,    25,
     348,   165,    28,   105,   172,   148,   170,   869,   148,   173,
     148,   140,   148,    27,   148,   148,   109,    25,   140,   112,
      28,   140,   115,   148,   148,   148,   140,   148,   148,   718,
     140,   720,   783,   895,    48,   148,   898,   385,    52,   165,
     175,   173,   904,    35,   172,   907,    35,  1084,  1085,   165,
     165,   145,    78,   165,   165,   173,    82,    83,   165,   921,
     922,   146,    76,   171,   165,    79,   170,    81,   171,   975,
      78,   933,   165,   935,    82,    83,    90,   175,   940,   105,
      33,   943,   173,    33,   110,   175,    35,   175,   165,   165,
     172,   145,   172,   955,   165,   109,   175,   105,     8,    35,
     173,   175,   110,   175,   140,   140,   454,   173,   165,   173,
     173,   270,   165,   975,   165,   164,   164,   146,   869,   348,
     824,    24,   984,    90,   986,  1013,   979,   989,   961,   320,
     992,   521,   480,   860,   879,   359,   875,   999,   486,  1001,
     864,   598,  1157,  1158,  1142,   371,  1161,  1162,   874,   497,
    1206,   499,  1199,   851,   664,   131,   518,   777,    -1,  1174,
      -1,  1176,  1024,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1039,  1040,    -1,
    1042,  1043,    -1,  1045,  1046,   211,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1055,  1056,    -1,  1058,    -1,  1060,    -1,
    1062,  1063,    -1,   211,  1066,    -1,    -1,   555,    -1,    -1,
      -1,   559,    -1,    -1,    -1,    -1,  1122,    -1,    -1,   567,
    1082,  1083,  1084,  1085,   975,    -1,    -1,    -1,  1090,    -1,
    1092,    -1,   921,   922,  1096,    -1,    -1,    -1,    -1,    -1,
    1102,  1103,  1104,  1105,  1106,    -1,    -1,    -1,   274,    -1,
       3,    -1,    -1,   279,    -1,    -1,    -1,    -1,    -1,    -1,
    1122,    -1,    -1,    -1,    17,  1127,   274,    20,    -1,    -1,
    1021,   279,    25,    -1,    -1,    -1,    -1,    -1,    -1,  1141,
      -1,    -1,    -1,    36,  1146,  1147,  1148,  1149,  1150,  1151,
    1152,  1153,    -1,    46,    -1,    -1,    -1,    -1,     2,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,   333,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    71,    -1,
      -1,    25,   348,  1185,    28,   333,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,  1198,    -1,    -1,    -1,
     348,    -1,    -1,    -1,    -1,    -1,    99,    -1,    -1,    -1,
     103,    -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,  1221,
    1222,  1223,  1224,   116,  1226,    -1,    -1,    -1,  1230,  1231,
      -1,  1122,    -1,    -1,    78,    -1,  1127,    -1,    82,    83,
      -1,    -1,  1244,  1245,   732,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1255,  1256,  1257,  1258,  1259,  1260,    -1,
      -1,   105,    -1,    -1,    -1,    -1,   110,    -1,     3,    -1,
      -1,  1273,    -1,    -1,    -1,    -1,    -1,   765,    -1,    -1,
     768,    -1,    17,    18,    -1,    20,    -1,    -1,   454,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,  1193,    -1,    -1,    40,   454,    -1,    -1,    -1,
      -1,    46,    -1,    -1,   480,    50,    -1,    -1,    53,    -1,
     486,    -1,    -1,    32,   812,    -1,    -1,    -1,    63,    -1,
     818,   497,   480,   499,    -1,    70,    71,    -1,   486,    72,
      -1,    -1,    -1,    78,    -1,    -1,     2,    -1,    -1,   497,
      -1,   499,    87,    -1,  1245,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,   211,   103,    25,
      -1,   106,    28,    -1,    -1,   110,   111,    -1,   113,    -1,
      89,   116,  1273,    -1,    93,    -1,    -1,    -1,    -1,   555,
      -1,    -1,    -1,   559,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   567,    -1,    -1,    -1,    -1,    -1,   555,    -1,    -1,
      -1,   559,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   567,
      -1,    -1,    78,    -1,    -1,    -1,    82,    83,    -1,    -1,
     274,   140,   141,   921,   922,   279,   145,   172,   147,   148,
      -1,    -1,    -1,   152,    32,   933,    -1,   935,    -1,   105,
      -1,   160,   940,    -1,   110,   943,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,   952,    -1,   954,    -1,    -1,   957,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,   974,    31,    -1,   333,
      -1,    -1,    36,    -1,    38,    39,   984,    -1,   986,    -1,
      44,    89,    -1,    -1,   348,    93,    -1,    51,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   258,   259,   260,   261,   262,
     263,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    83,
      -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,   282,
      -1,   284,   140,   141,    -1,   211,   144,   145,   291,   147,
     148,    -1,    -1,    -1,   152,   109,    -1,  1055,   112,    -1,
      -1,   115,   160,    32,  1062,    -1,   309,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,  1075,    -1,   322,
     323,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1086,   765,
      -1,    -1,   768,    -1,    -1,    -1,    -1,  1095,    -1,    -1,
     454,  1099,    -1,    -1,    -1,    -1,    -1,   765,   274,    -1,
     768,   165,    -1,   279,    -1,    -1,    -1,   171,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,   480,    -1,    -1,    -1,
      -1,    -1,   486,    -1,    -1,    -1,   812,    -1,    -1,    -1,
      -1,    -1,   818,   497,    -1,   499,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   812,    -1,    -1,    -1,     2,    -1,
     818,    -1,    -1,    -1,    -1,  1163,    -1,   333,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    25,   348,   152,    28,    -1,    -1,    -1,    -1,    -1,
     159,   160,    -1,    -1,   163,    -1,    -1,   166,   167,   168,
      -1,   555,    -1,    -1,    -1,   559,    -1,    -1,   177,    -1,
      -1,    -1,    -1,   567,   457,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1221,  1222,  1223,  1224,    -1,    -1,    32,
      -1,    -1,  1230,  1231,    78,    -1,    -1,    -1,    82,    83,
      -1,    -1,   485,    -1,    -1,    -1,   489,    -1,   491,    -1,
      -1,   494,    -1,    -1,    -1,    -1,    -1,   933,    -1,   935,
      32,   105,    -1,    -1,   940,    -1,   110,   943,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   933,   952,   935,   954,    -1,
      -1,   957,   940,    -1,    -1,   943,    89,    -1,   454,    -1,
      93,    -1,    -1,    -1,   952,    -1,   954,    -1,   974,   957,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   984,    -1,
     986,    -1,    -1,    -1,   480,    -1,   974,    89,    -1,    -1,
     486,    93,    -1,    -1,    -1,    -1,   984,    -1,   986,    -1,
      -1,   497,    -1,   499,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,    -1,    -1,    -1,   152,
      -1,     2,    -1,    -1,    -1,    -1,   159,   160,    -1,    -1,
     163,    -1,    -1,   166,   167,   168,    -1,   211,   140,   141,
      -1,   143,   144,   145,    25,   147,   148,    28,    -1,  1055,
     152,    -1,    -1,    -1,    -1,    -1,  1062,    -1,   160,   555,
      -1,    -1,    -1,   559,   166,   167,   168,  1055,    -1,  1075,
      -1,   567,    -1,    -1,  1062,    -1,    -1,    -1,    -1,    -1,
    1086,   765,    -1,    -1,   768,    -1,    -1,  1075,    -1,  1095,
      -1,    -1,    -1,  1099,    -1,     3,    -1,    78,  1086,    -1,
     274,    82,    83,    -1,    -1,   279,    -1,  1095,    -1,    17,
      18,  1099,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
       2,    -1,    -1,    -1,   105,    -1,    34,    -1,   812,   110,
      -1,    -1,    40,    -1,   818,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    25,    -1,    53,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,  1163,    -1,   333,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,   348,  1163,   749,   750,    -1,    87,
      -1,    -1,   755,   756,    -1,    -1,   759,   760,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    78,    -1,   106,    -1,
      82,    83,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,  1221,  1222,  1223,  1224,    -1,
      32,    -1,    -1,   105,  1230,  1231,    -1,    -1,   110,    -1,
     211,    -1,    -1,  1221,  1222,  1223,  1224,    -1,    -1,    -1,
      -1,    -1,  1230,  1231,    -1,    -1,    -1,    -1,    -1,   933,
      -1,   935,    -1,    -1,    -1,    -1,   940,    -1,    -1,   943,
      -1,    -1,   170,    -1,   172,    -1,    -1,    -1,   952,   765,
     954,    -1,   768,   957,    -1,    -1,    -1,    89,    -1,    -1,
     454,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     974,    -1,    -1,   274,    -1,    -1,    -1,    -1,   279,    -1,
     984,    -1,   986,    -1,    -1,    -1,   480,    -1,    -1,    -1,
      -1,    -1,   486,    -1,    -1,    -1,   812,    -1,    -1,    -1,
       2,    -1,   818,   497,    -1,   499,    -1,    -1,   140,   211,
      -1,    -1,    -1,   145,    -1,   147,   148,    -1,    -1,    -1,
     152,    -1,    -1,    25,    -1,    -1,    28,    -1,   160,    -1,
     923,   924,   333,    -1,   166,   167,   168,   930,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   348,    -1,   942,
      -1,  1055,   945,    -1,   947,    -1,   949,    -1,  1062,    -1,
      -1,   555,    -1,    -1,    -1,   559,    -1,    -1,    -1,    -1,
      -1,  1075,   274,   567,    -1,    -1,    78,   279,    -1,    -1,
      -1,    -1,  1086,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1095,    -1,    -1,    -1,  1099,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   105,    -1,    -1,    -1,    -1,   110,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   933,    22,   935,
      -1,    -1,    26,    27,   940,    -1,    -1,   943,    -1,    -1,
      -1,   333,    36,    -1,    38,    39,   952,    -1,   954,    -1,
      44,   957,    -1,    -1,    -1,    -1,   348,    51,    -1,    -1,
      -1,    -1,    -1,   454,    -1,    -1,    -1,    -1,   974,  1163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   984,    -1,
     986,    -1,    -1,    77,    -1,    79,    -1,    -1,    -1,   480,
      -1,    -1,    -1,    -1,    88,   486,    90,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1088,   497,    -1,   499,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,   112,   211,
      -1,   115,    -1,    -1,     2,    -1,    -1,  1221,  1222,  1223,
    1224,    -1,    -1,    -1,    -1,    -1,  1230,  1231,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,  1055,
      28,    -1,    -1,    -1,    -1,    -1,  1062,    -1,    -1,    -1,
      -1,    -1,   454,    -1,   555,    -1,    -1,    -1,   559,  1075,
      -1,    -1,    -1,    -1,    -1,    -1,   567,    -1,    -1,    -1,
    1086,   765,   274,    -1,   768,    -1,    -1,   279,   480,  1095,
      -1,    -1,    -1,  1099,   486,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,   497,    -1,   499,    -1,    -1,
      -1,    -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   105,   812,    -1,
      -1,    -1,   110,    22,   818,    -1,    -1,    26,    27,    -1,
      -1,   333,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,    -1,    -1,    -1,    -1,    44,   348,  1163,    -1,    -1,
      -1,    -1,    51,   555,    -1,    54,    -1,   559,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   567,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    74,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    83,    84,    -1,    86,    -1,    88,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,  1221,  1222,  1223,  1224,    -1,
     109,    -1,    -1,   112,  1230,  1231,   115,   140,   141,    -1,
     143,   144,   145,   211,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,   157,   158,   159,   160,   161,   933,
     163,   935,    -1,   166,   167,   168,   940,    -1,    -1,   943,
      -1,    -1,   454,    -1,   177,    -1,    -1,    -1,   952,    -1,
     954,    -1,    32,   957,   765,    22,   165,   768,    -1,    26,
      27,    -1,   171,    -1,    31,    -1,    -1,    -1,   480,    36,
     974,    38,    39,    -1,   486,    -1,   274,    44,    -1,    -1,
     984,   279,   986,    -1,    51,   497,    -1,   499,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   812,    -1,    -1,    -1,    -1,    -1,   818,    -1,    89,
      77,    -1,    79,    93,    81,    -1,    83,    -1,    -1,    86,
      -1,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   333,    -1,    -1,    -1,    -1,
      -1,    -1,   109,   555,    -1,   112,    -1,   559,   115,    -1,
     348,  1055,    -1,   765,    -1,   567,   768,    -1,  1062,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,  1075,   152,   153,   154,    -1,    -1,   157,   158,   159,
     160,    -1,  1086,   163,    -1,    49,   166,   167,   168,    -1,
      -1,  1095,    -1,    -1,    -1,  1099,    -1,   177,   165,    -1,
     812,    -1,    -1,    -1,   171,    -1,   818,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,    83,
      -1,    -1,   933,    22,   935,    -1,    -1,    26,    27,   940,
      -1,    -1,   943,    -1,    -1,    -1,    -1,    36,    -1,    38,
      39,   952,    -1,   954,    -1,    44,   957,    -1,    -1,    -1,
      -1,    -1,    51,    -1,    -1,    -1,   454,    -1,    -1,  1163,
      -1,    -1,    -1,   974,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    -1,    -1,   984,    -1,   986,    -1,    -1,    77,    -1,
      79,    -1,   480,    -1,    83,    -1,    85,    86,   486,    88,
      -1,    90,    30,    -1,    -1,    -1,    -1,    -1,    -1,   497,
      -1,    -1,    -1,    -1,    -1,    -1,    44,    45,    46,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,  1221,  1222,  1223,
    1224,   933,    -1,   935,    -1,    -1,  1230,  1231,   940,    -1,
      -1,   943,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     952,    -1,   954,    -1,  1055,   957,    -1,    -1,    -1,    -1,
      -1,  1062,   216,   765,    -1,    -1,   768,   555,    -1,    -1,
      -1,   559,   974,    -1,  1075,    -1,   165,    -1,    -1,   567,
      -1,    -1,   984,    -1,   986,  1086,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1095,    -1,    -1,    -1,  1099,    -1,
      -1,    -1,    -1,    -1,   258,   259,   260,   261,   262,   263,
     812,    -1,   266,   267,    -1,    -1,   818,    -1,    -1,   273,
      -1,   149,    22,   151,    -1,    -1,    26,    27,   282,    -1,
     284,    31,    -1,   161,    -1,    -1,    36,   291,    38,    39,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    51,    -1,  1055,    -1,   309,    -1,    -1,    -1,    -1,
    1062,    -1,  1163,    -1,    -1,    -1,    -1,   195,   322,   323,
      -1,    -1,    -1,  1075,    -1,    -1,    -1,    77,    -1,    79,
      -1,    81,    -1,    83,  1086,    -1,    86,    -1,    88,    -1,
      90,    -1,    -1,  1095,    -1,    -1,    -1,  1099,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,
    1221,  1222,  1223,  1224,    -1,    -1,    -1,    -1,   256,  1230,
    1231,   933,    -1,   935,    -1,    -1,   264,   265,   940,    -1,
      -1,   943,    -1,    -1,    -1,    -1,    -1,   275,    -1,    -1,
     952,    -1,   954,    -1,    -1,   957,    -1,    -1,    -1,    -1,
      -1,  1163,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,
      -1,   171,   974,    -1,    -1,    -1,    -1,   765,    -1,    -1,
     768,    -1,   984,    -1,   986,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,   457,    32,    -1,    -1,    -1,    -1,    -1,
      17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,  1221,
    1222,  1223,  1224,    -1,   812,    -1,    -1,    34,  1230,  1231,
     818,   485,   360,    40,    -1,   489,    -1,   491,    -1,    46,
     494,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,  1055,    -1,    -1,    63,   385,    -1,    -1,
    1062,    89,    -1,    70,    71,    93,    -1,    -1,    -1,    -1,
      -1,    78,    -1,  1075,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,  1086,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,  1095,   101,    -1,   103,  1099,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,   450,   151,   152,   153,   154,    -1,    -1,   157,
     158,   159,   160,   161,    -1,   163,   464,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,   933,    -1,   935,    -1,   177,
      -1,    -1,   940,    -1,    -1,   943,    -1,    -1,    -1,    -1,
      -1,  1163,    -1,    -1,   952,   172,   954,    -1,    -1,   957,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   506,    -1,
      -1,    -1,   510,    -1,    -1,    -1,   974,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   523,   984,    -1,   986,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,    -1,    -1,    -1,    -1,  1221,
    1222,  1223,  1224,    -1,    -1,    -1,    -1,    -1,  1230,  1231,
     156,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   169,    -1,   573,   172,    -1,   174,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   587,
      -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   602,    12,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    -1,    -1,    -1,  1075,    25,    -1,
      -1,    -1,    -1,    -1,    -1,   749,   750,    34,  1086,    36,
      -1,   755,   756,    40,    -1,   759,   760,  1095,    -1,    46,
      -1,  1099,    -1,    50,    -1,   769,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,   664,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   803,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,  1163,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
     147,    -1,    -1,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,   163,    -1,    -1,   166,
     167,   168,   169,    -1,    -1,   172,    -1,    -1,    -1,    -1,
      -1,    -1,   770,    -1,    -1,    -1,    -1,    -1,    -1,   777,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   793,    -1,    -1,    -1,   923,
     924,    -1,    -1,    -1,    -1,   803,   930,    -1,   932,    -1,
      -1,    -1,    -1,    -1,    -1,   939,    -1,    -1,   942,    -1,
      -1,   945,    -1,   947,    -1,   949,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   962,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   851,    -1,    -1,    -1,    -1,   982,   857,
      -1,    -1,   860,    -1,    -1,   863,    -1,    -1,   866,    -1,
      -1,    -1,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,   879,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,   961,  1088,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,   979,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1157,  1158,    -1,    -1,  1161,  1162,    -1,
      -1,   172,    -1,   174,    -1,    -1,    -1,    -1,  1172,    -1,
    1174,    -1,  1176,     0,     1,    -1,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      12,    -1,    14,    15,    16,    17,    18,    19,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,   171,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,    -1,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    33,    34,    -1,    36,
      -1,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    -1,    53,    54,    -1,    56,
      57,    58,    59,    60,    -1,    62,    63,    64,    65,    66,
      67,    68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,
      77,    78,    79,    80,    -1,    82,    83,    84,    85,    86,
      87,    88,    -1,    90,    91,    92,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,    -1,   103,   104,    -1,   106,
     107,   108,   109,   110,    -1,   112,   113,   114,   115,   116,
      -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,
     177,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    16,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    33,    34,    -1,    36,    -1,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      -1,    53,    54,    -1,    56,    57,    58,    59,    60,    -1,
      62,    63,    64,    65,    66,    67,    68,    -1,    70,    71,
      72,    -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    -1,    90,    91,
      92,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,   107,   108,   109,   110,    -1,
     112,   113,   114,   115,   116,    -1,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     1,   177,     3,     4,     5,     6,
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
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,     1,
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
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,    -1,     3,   177,     5,     6,     7,     8,
       9,    10,    -1,    12,    -1,    14,    -1,    -1,    17,    18,
      19,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    56,    -1,    -1,
      59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,   147,    -1,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,    -1,   166,   167,   168,
     169,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      -1,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    33,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    43,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    62,    63,    64,    65,
      -1,    67,    68,    -1,    70,    71,    72,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,   119,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
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
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    26,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     103,    -1,    -1,   106,   107,   108,    -1,   110,   111,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
     173,   174,    -1,    -1,   177,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,     3,   177,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,
      25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    48,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
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
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,
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
     107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    69,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
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
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,   122,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
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
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,
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
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
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
      87,    88,    89,    -1,    -1,    -1,    93,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
     162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,    -1,   174,   175,     3,   177,     5,     6,     7,     8,
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
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     3,   177,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    -1,
      36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,
      -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
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
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
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
      -1,    -1,   110,    -1,   112,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
     175,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
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
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
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
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   106,    -1,    -1,    -1,   110,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
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
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
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
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
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
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
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
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
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
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,
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
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
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
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
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
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
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
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
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
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
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
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,    -1,    -1,   172,    -1,   174,
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
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
     142,   143,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,
      -1,   163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,
     172,     3,   174,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    61,
      -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      92,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,   105,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,   118,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     142,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,   164,   165,    25,    -1,    -1,    -1,   170,    -1,
     172,    -1,   174,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,   146,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   164,    -1,    -1,    -1,    -1,    -1,   170,
      -1,   172,   173,   174,     3,    -1,     5,     6,     7,     8,
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
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,    -1,   142,    -1,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,   164,    -1,    25,    -1,    -1,
      -1,   170,    -1,   172,    -1,   174,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    12,    -1,   106,    -1,
       3,    -1,   110,    19,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    17,    18,    32,    20,    -1,    -1,
      -1,    37,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,   142,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,     3,    -1,   164,    73,    -1,    -1,
      63,    -1,   170,    12,   172,    -1,   174,    70,    71,    -1,
      19,    20,    -1,    89,    -1,    78,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    55,   110,   124,    -1,
     113,    -1,    -1,   116,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,   165,
     166,   167,   168,    -1,   103,    -1,    -1,    -1,    -1,    -1,
     109,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,   142,   143,   144,   145,    -1,   147,    -1,
      -1,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,     3,   163,    -1,    -1,   166,   167,   168,
     169,   170,    12,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    96,    97,    -1,    99,
      -1,   101,    19,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,    32,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,   129,
     130,    -1,    -1,   133,   134,   135,   136,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,    -1,   147,    -1,    -1,
      -1,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   163,    -1,    -1,   166,   167,   168,   169,
      -1,    12,    89,    -1,    -1,    -1,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,   170,
      19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,   170,
      19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      89,   166,   167,   168,    93,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
      -1,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    32,    33,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      48,    89,    -1,   140,   141,    93,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   120,   170,    -1,    -1,   124,    -1,    -1,    -1,
     177,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    13,   166,   167,
     168,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,   120,
     166,   167,   168,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    89,    -1,    -1,    -1,    93,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,   120,   166,   167,   168,   124,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    48,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
     102,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    13,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    73,    -1,    -1,    -1,    32,    -1,    -1,
     120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    89,   175,    -1,   177,    93,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    -1,   166,   167,   168,   124,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   124,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    89,    -1,    -1,    -1,    93,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,   165,   166,   167,   168,   123,   124,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    -1,   166,
     167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    12,    13,    -1,
      -1,   102,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,   124,
      -1,    -1,    -1,    -1,    89,    -1,   177,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   124,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,   173,    -1,
      -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    -1,   120,   166,   167,   168,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,   177,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,   165,   166,   167,
     168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    12,    93,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,   173,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,    -1,    19,    -1,   173,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   123,
     124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,   173,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    13,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,   175,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   102,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,   102,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    13,   166,   167,   168,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,     3,   166,   167,
     168,    -1,    -1,    19,    -1,    -1,    -1,   175,    -1,   177,
      -1,    17,    18,    -1,    20,    -1,    32,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    12,    -1,    -1,    -1,
      46,    -1,    -1,    19,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    89,    -1,    -1,    12,    93,    -1,    -1,
      -1,    87,    -1,    19,    -1,    -1,   102,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    32,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,   124,    -1,
     116,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   102,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,   102,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,     3,
     166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    17,    18,    -1,    20,    -1,    32,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    12,    -1,
      -1,    -1,    46,    -1,    -1,    19,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    87,    -1,    19,    -1,    -1,   102,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    32,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
     124,    -1,   116,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   102,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,   102,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,     3,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    17,    18,    -1,    20,    -1,
      32,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      12,    -1,    -1,    -1,    46,    -1,    -1,    19,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    89,    -1,    -1,
      12,    93,    -1,    -1,    -1,    87,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      32,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,   124,    -1,   116,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    89,    -1,    -1,
      -1,    93,    -1,   175,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    32,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
       1,    -1,     3,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,
      -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    17,    18,   106,    20,    -1,    -1,   110,
      -1,    25,   113,    27,    -1,   116,    -1,    -1,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
       3,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116
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
     104,   106,   107,   108,   109,   110,   112,   113,   115,   116,
     119,   121,   124,   140,   141,   142,   143,   144,   149,   159,
     162,   163,   164,   165,   168,   170,   172,   174,   177,   184,
     185,   186,   187,   188,   191,   192,   193,   194,   196,   197,
     198,   203,   204,   207,   208,   212,   214,   217,   221,   224,
     225,   226,   227,   228,   229,   232,   233,   235,   237,   240,
     241,   242,   243,   244,   248,   249,   254,   255,   256,   257,
     260,   261,   268,   269,   271,   272,   273,   274,   276,   277,
     302,   303,   307,   308,   329,   330,   331,   332,   333,   334,
     335,   343,   344,   345,   346,   347,   350,   351,   352,   353,
     354,   355,   356,   357,   359,   360,   361,   362,   363,   165,
      22,    36,    41,    42,    45,    56,    88,   101,   170,   174,
     240,   269,   329,   334,   345,   346,   347,   350,   352,   353,
     120,   336,   337,     3,   209,   347,   336,   347,   109,   309,
      90,   209,   186,   323,   347,     8,   190,   186,   172,     3,
      17,    18,    20,    25,    34,    40,    46,    50,    53,    63,
      70,    71,    78,    87,    96,    97,    99,   101,   103,   106,
     110,   113,   116,   211,   213,    11,    79,   122,   234,   347,
     234,   347,   234,   347,    27,   115,   236,   347,    83,    86,
     194,   172,   211,   211,   211,   172,   211,   310,   311,    33,
     198,   216,   347,   252,   253,   347,   347,    18,    78,    96,
     113,   347,   347,   347,     8,   172,   223,   222,     4,   297,
     322,   347,   107,   108,   165,   347,   349,   347,   216,   347,
     347,   347,   100,   172,   186,   347,   347,   187,   198,   187,
     198,   347,   236,   347,   349,   347,   347,   347,   347,   347,
       1,   171,   184,   199,   322,   111,   150,   297,   324,   325,
     349,   234,   322,   347,   358,   347,    81,   186,   170,    47,
      85,   114,   195,    26,   308,   347,     8,   256,   347,   348,
      56,   145,   258,   211,     1,    31,   211,   262,   264,   267,
     172,   285,    54,    74,    84,   292,    27,    79,    90,   109,
     293,    27,    79,    90,   109,   291,   211,   304,   305,   310,
     164,   165,   156,   347,    12,    19,    32,    89,    93,   124,
     140,   141,   143,   144,   145,   147,   148,   149,   151,   152,
     153,   154,   155,   157,   158,   159,   160,   161,   162,   163,
     166,   167,   168,   177,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   169,
     281,   172,   174,    89,    93,   347,   347,   347,   211,   322,
      56,   172,   186,   165,   170,   198,    48,   336,   307,   165,
     146,   165,   189,    22,    36,    39,    88,   191,   194,   119,
     212,   297,   326,   327,   328,   349,   170,   211,   172,   216,
      33,    48,   216,   120,   216,   339,    33,    48,   216,   339,
     216,   339,    48,   216,   339,   211,   211,   102,   198,   102,
     125,   198,   281,   195,   326,   172,   172,   198,   186,   111,
     172,   211,   312,   313,     1,   145,   317,    48,   146,   186,
     216,   146,   216,    13,   172,   172,   216,   326,   224,   224,
     146,   165,   347,   347,   165,   170,   216,   172,   326,   165,
     245,   245,   216,   216,   165,   171,   171,   184,   146,   171,
     347,   146,   173,   146,   173,   175,   339,    48,   146,   175,
     339,   123,   146,   175,     8,     1,   171,   199,   205,   206,
     347,   201,   347,    66,    37,    73,   165,   256,   258,   109,
     230,   274,   211,   322,   170,   171,     8,   266,   125,   146,
     171,    27,    48,    52,    76,    79,    90,   109,   185,   286,
     287,   288,   289,   290,   293,    90,     1,   145,   316,    90,
       1,     3,    12,    17,    19,    20,    25,    40,    46,    53,
      55,    63,    70,    71,    87,    99,   103,   106,   110,   116,
     140,   141,   142,   143,   144,   145,   147,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   163,   166,
     167,   168,   169,   172,   210,   211,   213,   278,   279,   280,
     281,   329,   125,   306,   146,   165,   165,   347,   347,   347,
     347,   234,   347,   234,   347,   347,   347,   347,   347,   347,
     347,     3,    20,    34,    63,   103,   109,   212,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,    69,   349,   349,   349,   349,
     349,   326,   326,   234,   347,   234,   347,    33,    48,    33,
      48,   102,   172,    48,   175,   211,    27,    48,    90,   115,
     338,   341,   342,   347,   363,   234,   347,   216,   310,   347,
     191,   347,   125,   173,   146,    48,   322,    45,   347,   234,
     347,   172,   216,    45,   347,   234,   347,   216,   216,   234,
     347,   216,   125,   125,   186,    35,   186,   347,    35,   347,
      66,   173,   327,   211,   238,   239,   313,   146,   173,    34,
      50,    97,   101,   174,   215,   318,   330,   125,   314,   347,
     311,   252,   211,   307,   347,   347,   173,   297,   347,     1,
     250,   328,   173,    21,   246,   171,   173,   173,   324,   173,
     324,   186,   175,   234,   347,   175,   186,   347,   175,   347,
     175,   347,   171,   171,   146,   165,    13,   148,   146,   165,
      13,    37,    73,   211,   140,   141,   142,   143,   144,   159,
     163,   168,   200,   280,   281,   282,   347,   200,   202,   258,
     170,   231,   307,   165,   172,     1,   259,   265,   267,   347,
     263,    48,    90,   289,   146,   173,   172,   211,   317,   174,
     215,   315,   330,   105,   294,   172,   284,   347,   140,   148,
     284,   284,   318,   330,   305,   172,   174,   165,   165,   165,
     165,   165,   165,   173,   175,    45,   234,   347,    45,   234,
     347,   327,   234,   347,   172,    48,    90,   146,   173,    17,
      20,    25,    46,    53,    63,    71,    87,    99,   110,   116,
     329,    89,    89,   336,   336,   165,   165,   297,   349,   328,
     347,   173,   347,    33,   216,    33,   216,   340,   341,   347,
      33,   216,   339,    33,   216,   339,   216,   339,   216,   339,
     347,   347,    35,   186,    35,    35,   186,   102,   198,   211,
     173,   146,   173,   173,   312,   317,     1,   322,    69,   349,
     211,   173,   173,   171,    75,   117,   171,   251,   173,   172,
     198,   211,   247,   186,   175,   339,   175,   339,   186,   123,
     205,   212,   170,   282,   347,   111,   347,   200,   202,   146,
     165,    13,   165,   170,   259,   304,   310,   327,   171,    31,
      83,    86,   171,   185,   218,   221,   267,   264,   211,   288,
     313,   145,   321,    61,   118,   298,     1,   175,   322,   298,
     287,   173,     3,   103,   279,   281,   173,   175,   347,    33,
      33,   347,    33,    33,   173,   175,   175,   327,   341,   317,
     329,   329,   216,   216,   216,   102,    45,   347,    45,   347,
     146,   173,   102,    45,   347,   216,    45,   347,   216,   216,
     216,   186,   186,   347,   186,    35,   165,   165,   239,   198,
     314,   175,    48,   175,   172,   172,    33,   186,   322,   247,
     145,   198,    45,   186,   347,   175,    45,   186,   347,   175,
     347,   200,    13,    37,    73,    37,    73,   165,   165,   282,
     347,   347,   259,   171,   165,   165,   173,     8,   220,   218,
     321,   173,   174,   215,   297,   320,   330,   150,   283,   314,
       3,    92,   103,   299,   300,   301,   347,   270,   175,   315,
     330,   175,   275,   173,   284,   284,   102,    45,    45,   102,
      45,    45,    45,    45,   173,   314,   347,   347,   347,   341,
     347,   347,   347,    35,   186,   347,   318,   330,   326,   326,
     186,   216,   173,   347,   347,   186,   347,   186,   171,   111,
     347,   200,   202,   200,   202,    13,   171,   165,   219,   221,
     283,   314,   321,   175,   322,   297,   347,   301,   118,   146,
     125,   151,   153,   154,   157,   158,    61,   197,   217,   296,
     315,   330,   165,   295,   296,   347,   347,   347,   347,   347,
     347,   102,   102,   102,   102,   186,   175,   173,   173,   198,
     102,   102,   165,   165,   165,   165,   347,   221,   283,   314,
     297,   319,   320,   330,    48,   175,   347,   300,   301,   301,
     301,   301,   301,   301,   299,   102,   102,   102,   102,   102,
     102,   347,   347,   347,   347,   330,   164,   164,   347,   347,
     347,   319,   320,   175,   319
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   188,   189,   188,   190,   188,   191,   191,
     192,   193,   193,   193,   194,   194,   194,   195,   195,   196,
     197,   197,   197,   198,   199,   199,   200,   200,   200,   200,
     200,   200,   201,   201,   201,   201,   201,   201,   202,   202,
     203,   203,   203,   203,   203,   203,   203,   204,   205,   205,
     205,   205,   206,   206,   207,   208,   208,   208,   208,   208,
     208,   209,   209,   210,   210,   210,   210,   210,   210,   211,
     211,   211,   211,   211,   211,   212,   212,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   215,   215,
     215,   215,   216,   216,   217,   217,   218,   219,   218,   220,
     218,   221,   221,   222,   221,   223,   221,   224,   224,   224,
     224,   224,   224,   224,   225,   225,   225,   225,   226,   227,
     227,   228,   229,   229,   229,   230,   229,   231,   229,   232,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   233,   233,   233,   233,   233,
     233,   233,   233,   233,   233,   234,   235,   235,   235,   235,
     235,   235,   235,   235,   235,   235,   235,   235,   236,   236,
     237,   237,   238,   238,   239,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   241,   241,
     241,   241,   241,   241,   242,   242,   242,   243,   243,   243,
     244,   244,   244,   244,   245,   245,   246,   246,   246,   247,
     247,   248,   249,   249,   250,   250,   251,   251,   251,   252,
     252,   252,   253,   253,   254,   255,   255,   256,   257,   257,
     257,   258,   258,   259,   259,   259,   260,   260,   261,   262,
     262,   263,   262,   264,   265,   264,   266,   264,   267,   267,
     268,   270,   269,   271,   272,   272,   272,   273,   275,   274,
     276,   276,   276,   276,   276,   277,   278,   278,   279,   279,
     279,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   281,   281,   281,
     281,   281,   281,   281,   281,   281,   281,   281,   281,   282,
     282,   283,   283,   283,   284,   284,   285,   286,   286,   287,
     287,   288,   288,   288,   288,   288,   288,   289,   289,   290,
     290,   290,   290,   290,   290,   290,   290,   290,   291,   291,
     291,   291,   291,   291,   292,   292,   292,   293,   293,   293,
     293,   293,   293,   294,   294,   295,   295,   296,   296,   297,
     298,   298,   298,   298,   298,   299,   299,   300,   300,   300,
     300,   300,   300,   300,   301,   301,   302,   303,   303,   304,
     304,   305,   306,   306,   306,   307,   307,   307,   307,   307,
     309,   308,   308,   310,   310,   311,   311,   312,   312,   312,
     313,   313,   313,   314,   314,   314,   315,   315,   315,   315,
     315,   315,   315,   316,   316,   316,   316,   316,   317,   317,
     317,   317,   317,   318,   318,   318,   318,   319,   319,   319,
     320,   320,   320,   320,   320,   321,   321,   321,   321,   321,
     322,   322,   322,   322,   323,   323,   324,   324,   324,   325,
     325,   326,   326,   327,   327,   328,   328,   328,   328,   329,
     329,   330,   330,   330,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   332,   332,   332,   332,
     332,   332,   332,   332,   332,   332,   332,   332,   332,   332,
     332,   332,   332,   333,   334,   335,   335,   335,   335,   335,
     335,   335,   335,   336,   336,   337,   338,   338,   339,   340,
     340,   341,   341,   341,   342,   342,   342,   342,   342,   342,
     343,   343,   344,   344,   344,   344,   344,   345,   345,   345,
     345,   345,   346,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     348,   348,   349,   349,   349,   350,   350,   350,   350,   351,
     351,   351,   351,   351,   352,   352,   352,   353,   353,   353,
     353,   353,   353,   354,   354,   354,   354,   355,   355,   356,
     356,   357,   357,   357,   357,   357,   357,   357,   357,   357,
     357,   357,   357,   357,   358,   358,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   359,
     359,   359,   359,   359,   359,   359,   359,   359,   359,   360,
     360,   360,   360,   360,   360,   360,   361,   361,   361,   361,
     362,   362,   362,   362,   363,   363,   363,   363,   363,   363,
     363
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     3,     3,     3,     3,     2,     1,     1,     1,
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
       1,     1,     6,     7,     3,     0,     6,     0,     6,     2,
       5,     3,     3,     6,     6,     4,     5,     5,     3,     3,
       6,     5,     6,     5,     6,     3,     4,     3,     4,     5,
       6,     5,     6,     3,     4,     3,     4,     6,     7,     6,
       7,     4,     5,     4,     5,     4,     4,     3,     6,     5,
       4,     3,     6,     5,     6,     5,     8,     7,     4,     4,
       6,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     6,     4,     7,     5,     3,     6,
       2,     2,     3,     3,     0,     2,     2,     3,     5,     1,
       3,     3,     5,     5,     0,     2,     3,     2,     3,     4,
       3,     1,     1,     3,     3,     5,     5,     2,     1,     1,
       1,     0,     2,     0,     2,     3,     3,     3,     3,     1,
       2,     0,     4,     1,     0,     4,     0,     3,     1,     3,
       1,     0,     7,     0,     1,     1,     1,     1,     0,     7,
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
       3,     3,     3,     2,     1,     1,     3,     1,     2,     1,
       3,     2,     0,     2,     2,     1,     2,     1,     1,     1,
       0,     5,     3,     1,     3,     3,     5,     1,     1,     3,
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
#line 609 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6425 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 613 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6431 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 614 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6437 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 621 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6445 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 629 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6453 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 633 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6462 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 641 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6468 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: include_module_stmt  */
#line 642 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6474 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: block_stmt  */
#line 643 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6480 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: use_stmt  */
#line 644 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6486 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: import_stmt  */
#line 645 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6492 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: require_stmt  */
#line 646 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6498 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 647 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6504 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: implements_stmt  */
#line 648 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6510 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: interface_stmt  */
#line 649 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6516 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: TDEFER stmt  */
#line 651 "chpl.ypp"
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
#line 6533 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: try_stmt  */
#line 663 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6539 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: return_stmt  */
#line 664 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6545 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 666 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6558 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 675 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6571 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: TLABEL ident_def stmt  */
#line 684 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6579 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TYIELD opt_try_expr TSEMI  */
#line 688 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6590 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: error TSEMI  */
#line 695 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6599 "bison-chpl-lib.cpp"
    break;

  case 27: /* tryable_stmt: assignment_stmt  */
#line 702 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6605 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: if_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6611 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: loop_stmt  */
#line 704 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6617 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: select_stmt  */
#line 705 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6623 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: manage_stmt  */
#line 706 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6629 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 707 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6635 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: throw_stmt  */
#line 708 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6641 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 710 "chpl.ypp"
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
#line 6659 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 724 "chpl.ypp"
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
#line 6678 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 739 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6690 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 747 "chpl.ypp"
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
#line 6706 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL do_stmt  */
#line 759 "chpl.ypp"
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
#line 6721 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TON expr do_stmt  */
#line 770 "chpl.ypp"
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
#line 6737 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 782 "chpl.ypp"
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
#line 6753 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL do_stmt  */
#line 794 "chpl.ypp"
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
#line 6768 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSYNC stmt  */
#line 805 "chpl.ypp"
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
#line 6785 "bison-chpl-lib.cpp"
    break;

  case 44: /* $@1: %empty  */
#line 822 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6793 "bison-chpl-lib.cpp"
    break;

  case 45: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 deprecated_decl_base  */
#line 826 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6801 "bison-chpl-lib.cpp"
    break;

  case 46: /* $@2: %empty  */
#line 830 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6809 "bison-chpl-lib.cpp"
    break;

  case 47: /* deprecated_decl_stmt: TDEPRECATED $@2 deprecated_decl_base  */
#line 834 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6817 "bison-chpl-lib.cpp"
    break;

  case 50: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 846 "chpl.ypp"
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
#line 6838 "bison-chpl-lib.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 866 "chpl.ypp"
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
#line 6857 "bison-chpl-lib.cpp"
    break;

  case 52: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 881 "chpl.ypp"
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
#line 6876 "bison-chpl-lib.cpp"
    break;

  case 53: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 896 "chpl.ypp"
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
#line 6893 "bison-chpl-lib.cpp"
    break;

  case 54: /* opt_access_control: %empty  */
#line 911 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6899 "bison-chpl-lib.cpp"
    break;

  case 55: /* opt_access_control: TPUBLIC  */
#line 912 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6906 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_access_control: TPRIVATE  */
#line 914 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6913 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_prototype: %empty  */
#line 919 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6919 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_prototype: TPROTOTYPE  */
#line 920 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6926 "bison-chpl-lib.cpp"
    break;

  case 59: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 926 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6936 "bison-chpl-lib.cpp"
    break;

  case 60: /* block_stmt_body: TLCBR TRCBR  */
#line 944 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6944 "bison-chpl-lib.cpp"
    break;

  case 61: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 948 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6952 "bison-chpl-lib.cpp"
    break;

  case 62: /* block_stmt_body: TLCBR error TRCBR  */
#line 952 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6964 "bison-chpl-lib.cpp"
    break;

  case 63: /* block_stmt: block_stmt_body  */
#line 963 "chpl.ypp"
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
#line 6983 "bison-chpl-lib.cpp"
    break;

  case 64: /* stmt_ls: toplevel_stmt  */
#line 981 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6989 "bison-chpl-lib.cpp"
    break;

  case 65: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 982 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6995 "bison-chpl-lib.cpp"
    break;

  case 66: /* renames_ls: expr  */
#line 987 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7003 "bison-chpl-lib.cpp"
    break;

  case 67: /* renames_ls: all_op_name  */
#line 991 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 68: /* renames_ls: expr TAS expr  */
#line 995 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7020 "bison-chpl-lib.cpp"
    break;

  case 69: /* renames_ls: renames_ls TCOMMA expr  */
#line 1000 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7028 "bison-chpl-lib.cpp"
    break;

  case 70: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1004 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7036 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1008 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7045 "bison-chpl-lib.cpp"
    break;

  case 72: /* use_renames_ls: expr  */
#line 1018 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7054 "bison-chpl-lib.cpp"
    break;

  case 73: /* use_renames_ls: expr TAS expr  */
#line 1023 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7064 "bison-chpl-lib.cpp"
    break;

  case 74: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1029 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7075 "bison-chpl-lib.cpp"
    break;

  case 75: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1036 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7084 "bison-chpl-lib.cpp"
    break;

  case 76: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1041 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1048 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7107 "bison-chpl-lib.cpp"
    break;

  case 78: /* opt_only_ls: %empty  */
#line 1058 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7113 "bison-chpl-lib.cpp"
    break;

  case 79: /* opt_only_ls: renames_ls  */
#line 1059 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1064 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7127 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1068 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7138 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1075 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7150 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1083 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7163 "bison-chpl-lib.cpp"
    break;

  case 84: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1092 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7174 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1099 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7186 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1107 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7199 "bison-chpl-lib.cpp"
    break;

  case 87: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1119 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7207 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_expr: expr  */
#line 1126 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7215 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_expr: expr TDOT all_op_name  */
#line 1130 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7224 "bison-chpl-lib.cpp"
    break;

  case 90: /* import_expr: expr TAS ident_use  */
#line 1135 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7234 "bison-chpl-lib.cpp"
    break;

  case 91: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1141 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7244 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_ls: import_expr  */
#line 1149 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7250 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_ls: import_ls TCOMMA import_expr  */
#line 1150 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7256 "bison-chpl-lib.cpp"
    break;

  case 94: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1155 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7266 "bison-chpl-lib.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1164 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7274 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1168 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7282 "bison-chpl-lib.cpp"
    break;

  case 97: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1172 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7290 "bison-chpl-lib.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1176 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7298 "bison-chpl-lib.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1180 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7306 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1184 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7314 "bison-chpl-lib.cpp"
    break;

  case 101: /* opt_label_ident: %empty  */
#line 1192 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7320 "bison-chpl-lib.cpp"
    break;

  case 102: /* opt_label_ident: TIDENT  */
#line 1193 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7326 "bison-chpl-lib.cpp"
    break;

  case 103: /* ident_fn_def: TIDENT  */
#line 1197 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7332 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: TNONE  */
#line 1198 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'none'"); }
#line 7338 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_fn_def: TTHIS  */
#line 1199 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7344 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_fn_def: TFALSE  */
#line 1200 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'false'"); }
#line 7350 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_fn_def: TTRUE  */
#line 1201 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'true'"); }
#line 7356 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_fn_def: internal_type_ident_def  */
#line 1202 "chpl.ypp"
                           { std::string msg = "attempt to redefine reserved type '";
                             msg += (yyvsp[0].uniqueStr).c_str();
                             msg += "'";
                             (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), msg); }
#line 7365 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_def: TIDENT  */
#line 1208 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7371 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: TNONE  */
#line 1209 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'none'"); }
#line 7377 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_def: TTHIS  */
#line 1210 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'this'"); }
#line 7383 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_def: TFALSE  */
#line 1211 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'false'"); }
#line 7389 "bison-chpl-lib.cpp"
    break;

  case 113: /* ident_def: TTRUE  */
#line 1212 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'true'"); }
#line 7395 "bison-chpl-lib.cpp"
    break;

  case 114: /* ident_def: internal_type_ident_def  */
#line 1213 "chpl.ypp"
                           { std::string msg = "attempt to redefine reserved type '";
                             msg += (yyvsp[0].uniqueStr).c_str();
                             msg += "'";
                             (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), msg); }
#line 7404 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_use: TIDENT  */
#line 1228 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7410 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_use: TTHIS  */
#line 1229 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7416 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TBOOL  */
#line 1262 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7422 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TENUM  */
#line 1263 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7428 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TINT  */
#line 1264 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7434 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TUINT  */
#line 1265 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7440 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TREAL  */
#line 1266 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7446 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TIMAG  */
#line 1267 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7452 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TCOMPLEX  */
#line 1268 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7458 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TBYTES  */
#line 1269 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7464 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TSTRING  */
#line 1270 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7470 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TLOCALE  */
#line 1271 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7476 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TNOTHING  */
#line 1272 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7482 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TVOID  */
#line 1273 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7488 "bison-chpl-lib.cpp"
    break;

  case 152: /* do_stmt: TDO stmt  */
#line 1287 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7494 "bison-chpl-lib.cpp"
    break;

  case 153: /* do_stmt: block_stmt  */
#line 1288 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7500 "bison-chpl-lib.cpp"
    break;

  case 154: /* return_stmt: TRETURN TSEMI  */
#line 1293 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7511 "bison-chpl-lib.cpp"
    break;

  case 155: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1300 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7522 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@3: %empty  */
#line 1311 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7530 "bison-chpl-lib.cpp"
    break;

  case 158: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@3 class_level_stmt  */
#line 1315 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7538 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@4: %empty  */
#line 1319 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 160: /* deprecated_class_level_stmt: TDEPRECATED $@4 class_level_stmt  */
#line 1323 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7554 "bison-chpl-lib.cpp"
    break;

  case 161: /* class_level_stmt: TSEMI  */
#line 1330 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7562 "bison-chpl-lib.cpp"
    break;

  case 162: /* class_level_stmt: inner_class_level_stmt  */
#line 1334 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7572 "bison-chpl-lib.cpp"
    break;

  case 163: /* $@5: %empty  */
#line 1339 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7579 "bison-chpl-lib.cpp"
    break;

  case 164: /* class_level_stmt: TPUBLIC $@5 inner_class_level_stmt  */
#line 1341 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7588 "bison-chpl-lib.cpp"
    break;

  case 165: /* $@6: %empty  */
#line 1345 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7595 "bison-chpl-lib.cpp"
    break;

  case 166: /* class_level_stmt: TPRIVATE $@6 inner_class_level_stmt  */
#line 1347 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7604 "bison-chpl-lib.cpp"
    break;

  case 174: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1365 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7613 "bison-chpl-lib.cpp"
    break;

  case 175: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1370 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7622 "bison-chpl-lib.cpp"
    break;

  case 176: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1375 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7631 "bison-chpl-lib.cpp"
    break;

  case 177: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1380 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7639 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_start: TFORWARDING  */
#line 1387 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7648 "bison-chpl-lib.cpp"
    break;

  case 179: /* extern_or_export: TEXTERN  */
#line 1394 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7654 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_or_export: TEXPORT  */
#line 1395 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7660 "bison-chpl-lib.cpp"
    break;

  case 181: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1400 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7670 "bison-chpl-lib.cpp"
    break;

  case 182: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1410 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7679 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1416 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7690 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1423 "chpl.ypp"
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
#line 7712 "bison-chpl-lib.cpp"
    break;

  case 185: /* $@7: %empty  */
#line 1441 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7720 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@7 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1445 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7729 "bison-chpl-lib.cpp"
    break;

  case 187: /* $@8: %empty  */
#line 1450 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7738 "bison-chpl-lib.cpp"
    break;

  case 188: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@8 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1455 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7747 "bison-chpl-lib.cpp"
    break;

  case 189: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1463 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7755 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1470 "chpl.ypp"
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
#line 7777 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TWHILE expr do_stmt  */
#line 1488 "chpl.ypp"
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
#line 7794 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1501 "chpl.ypp"
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
#line 7811 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1514 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7819 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1518 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7827 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1522 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7835 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1526 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7843 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1530 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7851 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFOR expr do_stmt  */
#line 1534 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7859 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1538 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7867 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1542 "chpl.ypp"
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
#line 7889 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1560 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7897 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1564 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7905 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1568 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7913 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1572 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7921 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr do_stmt  */
#line 1576 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7929 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1580 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7937 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1584 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7945 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1588 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7953 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1592 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7961 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1596 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7969 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1600 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7977 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1604 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7985 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1608 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7993 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1612 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8001 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1616 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8009 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1620 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8017 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1624 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8025 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1628 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8033 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1632 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8041 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1636 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8049 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1640 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8057 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1644 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8065 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1648 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8074 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1653 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8083 "bison-chpl-lib.cpp"
    break;

  case 225: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1661 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8093 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF expr TTHEN stmt  */
#line 1670 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8101 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF expr block_stmt  */
#line 1674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8109 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1678 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8117 "bison-chpl-lib.cpp"
    break;

  case 229: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1682 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8125 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1686 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8133 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF ifvar block_stmt  */
#line 1690 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8141 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1694 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8149 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1698 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8157 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1702 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8166 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1707 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8175 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1712 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8184 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1717 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8193 "bison-chpl-lib.cpp"
    break;

  case 238: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1725 "chpl.ypp"
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
#line 8212 "bison-chpl-lib.cpp"
    break;

  case 239: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1740 "chpl.ypp"
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
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 240: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1758 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8239 "bison-chpl-lib.cpp"
    break;

  case 241: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1762 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8247 "bison-chpl-lib.cpp"
    break;

  case 242: /* ifc_formal_ls: ifc_formal  */
#line 1768 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8253 "bison-chpl-lib.cpp"
    break;

  case 243: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1769 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8259 "bison-chpl-lib.cpp"
    break;

  case 244: /* ifc_formal: ident_def  */
#line 1774 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8265 "bison-chpl-lib.cpp"
    break;

  case 257: /* implements_type_ident: implements_type_error_ident  */
#line 1792 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8277 "bison-chpl-lib.cpp"
    break;

  case 264: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1821 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8285 "bison-chpl-lib.cpp"
    break;

  case 265: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1825 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8293 "bison-chpl-lib.cpp"
    break;

  case 266: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1829 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8301 "bison-chpl-lib.cpp"
    break;

  case 267: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1836 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8309 "bison-chpl-lib.cpp"
    break;

  case 268: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1840 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8317 "bison-chpl-lib.cpp"
    break;

  case 269: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1844 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8325 "bison-chpl-lib.cpp"
    break;

  case 270: /* try_stmt: TTRY tryable_stmt  */
#line 1851 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8333 "bison-chpl-lib.cpp"
    break;

  case 271: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1855 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8341 "bison-chpl-lib.cpp"
    break;

  case 272: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1859 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8349 "bison-chpl-lib.cpp"
    break;

  case 273: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1863 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8357 "bison-chpl-lib.cpp"
    break;

  case 274: /* catch_expr_ls: %empty  */
#line 1869 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8363 "bison-chpl-lib.cpp"
    break;

  case 275: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1870 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8369 "bison-chpl-lib.cpp"
    break;

  case 276: /* catch_expr: TCATCH block_stmt  */
#line 1875 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8377 "bison-chpl-lib.cpp"
    break;

  case 277: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1879 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8385 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1883 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8393 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr_inner: ident_def  */
#line 1890 "chpl.ypp"
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
#line 8410 "bison-chpl-lib.cpp"
    break;

  case 280: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1903 "chpl.ypp"
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
#line 8427 "bison-chpl-lib.cpp"
    break;

  case 281: /* throw_stmt: TTHROW expr TSEMI  */
#line 1919 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8437 "bison-chpl-lib.cpp"
    break;

  case 282: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1927 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8445 "bison-chpl-lib.cpp"
    break;

  case 283: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1931 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8455 "bison-chpl-lib.cpp"
    break;

  case 284: /* when_stmt_ls: %empty  */
#line 1939 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8461 "bison-chpl-lib.cpp"
    break;

  case 285: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1940 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8467 "bison-chpl-lib.cpp"
    break;

  case 286: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1945 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8475 "bison-chpl-lib.cpp"
    break;

  case 287: /* when_stmt: TOTHERWISE stmt  */
#line 1949 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8484 "bison-chpl-lib.cpp"
    break;

  case 288: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1954 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8493 "bison-chpl-lib.cpp"
    break;

  case 289: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 1962 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8502 "bison-chpl-lib.cpp"
    break;

  case 290: /* manager_expr: expr TAS ident_def  */
#line 1967 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8510 "bison-chpl-lib.cpp"
    break;

  case 291: /* manager_expr: expr  */
#line 1971 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8518 "bison-chpl-lib.cpp"
    break;

  case 292: /* manager_expr_ls: manager_expr  */
#line 1977 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8524 "bison-chpl-lib.cpp"
    break;

  case 293: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 1978 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8530 "bison-chpl-lib.cpp"
    break;

  case 294: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 1983 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8538 "bison-chpl-lib.cpp"
    break;

  case 295: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1992 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8547 "bison-chpl-lib.cpp"
    break;

  case 296: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1997 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8558 "bison-chpl-lib.cpp"
    break;

  case 297: /* class_start: class_tag ident_def  */
#line 2008 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8566 "bison-chpl-lib.cpp"
    break;

  case 298: /* class_tag: TCLASS  */
#line 2014 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8572 "bison-chpl-lib.cpp"
    break;

  case 299: /* class_tag: TRECORD  */
#line 2015 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8578 "bison-chpl-lib.cpp"
    break;

  case 300: /* class_tag: TUNION  */
#line 2016 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 301: /* opt_inherit: %empty  */
#line 2020 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 302: /* opt_inherit: TCOLON expr_ls  */
#line 2021 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_level_stmt_ls: %empty  */
#line 2025 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8605 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2030 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8613 "bison-chpl-lib.cpp"
    break;

  case 305: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2034 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8621 "bison-chpl-lib.cpp"
    break;

  case 306: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2041 "chpl.ypp"
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
#line 8642 "bison-chpl-lib.cpp"
    break;

  case 307: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2058 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8656 "bison-chpl-lib.cpp"
    break;

  case 308: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2071 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8664 "bison-chpl-lib.cpp"
    break;

  case 309: /* enum_ls: deprecated_enum_item  */
#line 2078 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8673 "bison-chpl-lib.cpp"
    break;

  case 310: /* enum_ls: enum_ls TCOMMA  */
#line 2083 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8683 "bison-chpl-lib.cpp"
    break;

  case 311: /* $@9: %empty  */
#line 2089 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8692 "bison-chpl-lib.cpp"
    break;

  case 312: /* enum_ls: enum_ls TCOMMA $@9 deprecated_enum_item  */
#line 2094 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8701 "bison-chpl-lib.cpp"
    break;

  case 314: /* $@10: %empty  */
#line 2103 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8709 "bison-chpl-lib.cpp"
    break;

  case 315: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@10 enum_item  */
#line 2107 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8717 "bison-chpl-lib.cpp"
    break;

  case 316: /* $@11: %empty  */
#line 2111 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8725 "bison-chpl-lib.cpp"
    break;

  case 317: /* deprecated_enum_item: TDEPRECATED $@11 enum_item  */
#line 2115 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8733 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_item: ident_def  */
#line 2122 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8744 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_item: ident_def TASSIGN expr  */
#line 2129 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8757 "bison-chpl-lib.cpp"
    break;

  case 320: /* lambda_decl_start: TLAMBDA  */
#line 2141 "chpl.ypp"
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
#line 8773 "bison-chpl-lib.cpp"
    break;

  case 321: /* $@12: %empty  */
#line 2157 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8783 "bison-chpl-lib.cpp"
    break;

  case 322: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_lifetime_where $@12 function_body_stmt  */
#line 2163 "chpl.ypp"
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
#line 8805 "bison-chpl-lib.cpp"
    break;

  case 324: /* linkage_spec: linkage_spec_empty  */
#line 2186 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8811 "bison-chpl-lib.cpp"
    break;

  case 325: /* linkage_spec: TINLINE  */
#line 2187 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8818 "bison-chpl-lib.cpp"
    break;

  case 326: /* linkage_spec: TOVERRIDE  */
#line 2189 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8825 "bison-chpl-lib.cpp"
    break;

  case 327: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2195 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8833 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@13: %empty  */
#line 2204 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 8847 "bison-chpl-lib.cpp"
    break;

  case 329: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@13 opt_function_body_stmt  */
#line 2214 "chpl.ypp"
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
#line 8870 "bison-chpl-lib.cpp"
    break;

  case 330: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2236 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8882 "bison-chpl-lib.cpp"
    break;

  case 331: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2244 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8894 "bison-chpl-lib.cpp"
    break;

  case 332: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2252 "chpl.ypp"
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
#line 8909 "bison-chpl-lib.cpp"
    break;

  case 333: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2263 "chpl.ypp"
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
#line 8924 "bison-chpl-lib.cpp"
    break;

  case 334: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2274 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8934 "bison-chpl-lib.cpp"
    break;

  case 335: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2283 "chpl.ypp"
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
#line 8950 "bison-chpl-lib.cpp"
    break;

  case 337: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2298 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8956 "bison-chpl-lib.cpp"
    break;

  case 340: /* fn_ident: ident_def TBANG  */
#line 2305 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8966 "bison-chpl-lib.cpp"
    break;

  case 381: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2363 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8972 "bison-chpl-lib.cpp"
    break;

  case 382: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2364 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8978 "bison-chpl-lib.cpp"
    break;

  case 383: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2365 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8984 "bison-chpl-lib.cpp"
    break;

  case 384: /* opt_formal_ls: %empty  */
#line 2369 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8990 "bison-chpl-lib.cpp"
    break;

  case 385: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2370 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8996 "bison-chpl-lib.cpp"
    break;

  case 386: /* req_formal_ls: TLP formal_ls TRP  */
#line 2374 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9002 "bison-chpl-lib.cpp"
    break;

  case 387: /* formal_ls_inner: formal  */
#line 2378 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9008 "bison-chpl-lib.cpp"
    break;

  case 388: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2379 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9014 "bison-chpl-lib.cpp"
    break;

  case 389: /* formal_ls: %empty  */
#line 2383 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9020 "bison-chpl-lib.cpp"
    break;

  case 390: /* formal_ls: formal_ls_inner  */
#line 2384 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9026 "bison-chpl-lib.cpp"
    break;

  case 391: /* formal: opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2389 "chpl.ypp"
  {
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9039 "bison-chpl-lib.cpp"
    break;

  case 392: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2398 "chpl.ypp"
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
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 393: /* formal: opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2409 "chpl.ypp"
  {
    (yyval.expr) = VarArgFormal::build(BUILDER, LOC((yyloc)),
                             /*attributes*/ nullptr,
                             /*name*/ (yyvsp[-2].uniqueStr),
                             /*intent*/ (yyvsp[-3].intentTag),
                             toOwned((yyvsp[-1].expr)),
                             toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9068 "bison-chpl-lib.cpp"
    break;

  case 394: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2419 "chpl.ypp"
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
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 395: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2432 "chpl.ypp"
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
#line 9099 "bison-chpl-lib.cpp"
    break;

  case 396: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type formal_var_arg_expr  */
#line 2444 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
  }
#line 9107 "bison-chpl-lib.cpp"
    break;

  case 397: /* opt_formal_intent_tag: %empty  */
#line 2450 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9116 "bison-chpl-lib.cpp"
    break;

  case 398: /* opt_formal_intent_tag: required_intent_tag  */
#line 2455 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9125 "bison-chpl-lib.cpp"
    break;

  case 399: /* required_intent_tag: TIN  */
#line 2462 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9131 "bison-chpl-lib.cpp"
    break;

  case 400: /* required_intent_tag: TINOUT  */
#line 2463 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9137 "bison-chpl-lib.cpp"
    break;

  case 401: /* required_intent_tag: TOUT  */
#line 2464 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9143 "bison-chpl-lib.cpp"
    break;

  case 402: /* required_intent_tag: TCONST TIN  */
#line 2465 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9149 "bison-chpl-lib.cpp"
    break;

  case 403: /* required_intent_tag: TCONST TREF  */
#line 2466 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9155 "bison-chpl-lib.cpp"
    break;

  case 404: /* required_intent_tag: TCONST  */
#line 2467 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9161 "bison-chpl-lib.cpp"
    break;

  case 405: /* required_intent_tag: TPARAM  */
#line 2468 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9167 "bison-chpl-lib.cpp"
    break;

  case 406: /* required_intent_tag: TREF  */
#line 2469 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9173 "bison-chpl-lib.cpp"
    break;

  case 407: /* required_intent_tag: TTYPE  */
#line 2470 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9179 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_this_intent_tag: %empty  */
#line 2474 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9185 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_this_intent_tag: TPARAM  */
#line 2475 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9191 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_this_intent_tag: TREF  */
#line 2476 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9197 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_this_intent_tag: TCONST TREF  */
#line 2477 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9203 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_this_intent_tag: TCONST  */
#line 2478 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9209 "bison-chpl-lib.cpp"
    break;

  case 413: /* opt_this_intent_tag: TTYPE  */
#line 2479 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9215 "bison-chpl-lib.cpp"
    break;

  case 414: /* proc_iter_or_op: TPROC  */
#line 2483 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9221 "bison-chpl-lib.cpp"
    break;

  case 415: /* proc_iter_or_op: TITER  */
#line 2484 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9227 "bison-chpl-lib.cpp"
    break;

  case 416: /* proc_iter_or_op: TOPERATOR  */
#line 2485 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9233 "bison-chpl-lib.cpp"
    break;

  case 417: /* opt_ret_tag: %empty  */
#line 2489 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9239 "bison-chpl-lib.cpp"
    break;

  case 418: /* opt_ret_tag: TCONST  */
#line 2490 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 9245 "bison-chpl-lib.cpp"
    break;

  case 419: /* opt_ret_tag: TCONST TREF  */
#line 2491 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 9251 "bison-chpl-lib.cpp"
    break;

  case 420: /* opt_ret_tag: TREF  */
#line 2492 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 9257 "bison-chpl-lib.cpp"
    break;

  case 421: /* opt_ret_tag: TPARAM  */
#line 2493 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9263 "bison-chpl-lib.cpp"
    break;

  case 422: /* opt_ret_tag: TTYPE  */
#line 2494 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9269 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_throws_error: %empty  */
#line 2498 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9275 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_throws_error: TTHROWS  */
#line 2499 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9281 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_function_body_stmt: TSEMI  */
#line 2502 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9287 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_function_body_stmt: function_body_stmt  */
#line 2503 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9293 "bison-chpl-lib.cpp"
    break;

  case 427: /* function_body_stmt: block_stmt_body  */
#line 2507 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9299 "bison-chpl-lib.cpp"
    break;

  case 428: /* function_body_stmt: return_stmt  */
#line 2508 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9305 "bison-chpl-lib.cpp"
    break;

  case 429: /* query_expr: TQUERIEDIDENT  */
#line 2512 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9311 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_lifetime_where: %empty  */
#line 2517 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9317 "bison-chpl-lib.cpp"
    break;

  case 431: /* opt_lifetime_where: TWHERE expr  */
#line 2519 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9323 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2521 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9329 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2523 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9335 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2525 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9341 "bison-chpl-lib.cpp"
    break;

  case 435: /* lifetime_components_expr: lifetime_expr  */
#line 2530 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9347 "bison-chpl-lib.cpp"
    break;

  case 436: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2532 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9353 "bison-chpl-lib.cpp"
    break;

  case 437: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2537 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9359 "bison-chpl-lib.cpp"
    break;

  case 438: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2539 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9365 "bison-chpl-lib.cpp"
    break;

  case 439: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2541 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9371 "bison-chpl-lib.cpp"
    break;

  case 440: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2543 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9377 "bison-chpl-lib.cpp"
    break;

  case 441: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2545 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9383 "bison-chpl-lib.cpp"
    break;

  case 442: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2547 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9389 "bison-chpl-lib.cpp"
    break;

  case 443: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2549 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9395 "bison-chpl-lib.cpp"
    break;

  case 444: /* lifetime_ident: TIDENT  */
#line 2553 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9401 "bison-chpl-lib.cpp"
    break;

  case 445: /* lifetime_ident: TTHIS  */
#line 2554 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9407 "bison-chpl-lib.cpp"
    break;

  case 446: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2559 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9416 "bison-chpl-lib.cpp"
    break;

  case 447: /* type_alias_decl_stmt_start: TTYPE  */
#line 2568 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9424 "bison-chpl-lib.cpp"
    break;

  case 448: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2572 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9433 "bison-chpl-lib.cpp"
    break;

  case 449: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2580 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 450: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2584 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 451: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2591 "chpl.ypp"
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
#line 9471 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_init_type: %empty  */
#line 2611 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9477 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_init_type: TASSIGN type_level_expr  */
#line 2613 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9483 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_init_type: TASSIGN array_type  */
#line 2615 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9492 "bison-chpl-lib.cpp"
    break;

  case 455: /* var_decl_type: TPARAM  */
#line 2622 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9498 "bison-chpl-lib.cpp"
    break;

  case 456: /* var_decl_type: TCONST TREF  */
#line 2623 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9504 "bison-chpl-lib.cpp"
    break;

  case 457: /* var_decl_type: TREF  */
#line 2624 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9510 "bison-chpl-lib.cpp"
    break;

  case 458: /* var_decl_type: TCONST  */
#line 2625 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9516 "bison-chpl-lib.cpp"
    break;

  case 459: /* var_decl_type: TVAR  */
#line 2626 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9522 "bison-chpl-lib.cpp"
    break;

  case 460: /* $@14: %empty  */
#line 2631 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9531 "bison-chpl-lib.cpp"
    break;

  case 461: /* var_decl_stmt: TCONFIG $@14 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2635 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9540 "bison-chpl-lib.cpp"
    break;

  case 462: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2640 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9549 "bison-chpl-lib.cpp"
    break;

  case 463: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2648 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9557 "bison-chpl-lib.cpp"
    break;

  case 464: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2652 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9565 "bison-chpl-lib.cpp"
    break;

  case 465: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2659 "chpl.ypp"
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
#line 9585 "bison-chpl-lib.cpp"
    break;

  case 466: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2675 "chpl.ypp"
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
#line 9603 "bison-chpl-lib.cpp"
    break;

  case 467: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2692 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9611 "bison-chpl-lib.cpp"
    break;

  case 468: /* tuple_var_decl_component: ident_def  */
#line 2696 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9619 "bison-chpl-lib.cpp"
    break;

  case 469: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2700 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9627 "bison-chpl-lib.cpp"
    break;

  case 470: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2707 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9633 "bison-chpl-lib.cpp"
    break;

  case 471: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2709 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9639 "bison-chpl-lib.cpp"
    break;

  case 472: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2711 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9645 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_init_expr: %empty  */
#line 2717 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9651 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2718 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9657 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2719 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9663 "bison-chpl-lib.cpp"
    break;

  case 476: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2725 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9671 "bison-chpl-lib.cpp"
    break;

  case 477: /* ret_array_type: TLSBR TRSBR  */
#line 2729 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 9679 "bison-chpl-lib.cpp"
    break;

  case 478: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2733 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9687 "bison-chpl-lib.cpp"
    break;

  case 479: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2737 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 9695 "bison-chpl-lib.cpp"
    break;

  case 480: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2741 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9703 "bison-chpl-lib.cpp"
    break;

  case 481: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2745 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9711 "bison-chpl-lib.cpp"
    break;

  case 482: /* ret_array_type: TLSBR error TRSBR  */
#line 2749 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "invalid expression for domain of array return type");
  }
#line 9719 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_ret_type: %empty  */
#line 2755 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9725 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_ret_type: TCOLON type_level_expr  */
#line 2756 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9731 "bison-chpl-lib.cpp"
    break;

  case 485: /* opt_ret_type: TCOLON ret_array_type  */
#line 2757 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9737 "bison-chpl-lib.cpp"
    break;

  case 486: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2758 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9743 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_ret_type: error  */
#line 2759 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9749 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_type: %empty  */
#line 2764 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9755 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_type: TCOLON type_level_expr  */
#line 2765 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_type: TCOLON array_type  */
#line 2766 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9767 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2767 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9773 "bison-chpl-lib.cpp"
    break;

  case 492: /* opt_type: error  */
#line 2768 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9779 "bison-chpl-lib.cpp"
    break;

  case 493: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2789 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9787 "bison-chpl-lib.cpp"
    break;

  case 494: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2793 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9795 "bison-chpl-lib.cpp"
    break;

  case 495: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2797 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9803 "bison-chpl-lib.cpp"
    break;

  case 496: /* array_type: TLSBR error TRSBR  */
#line 2801 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 9811 "bison-chpl-lib.cpp"
    break;

  case 497: /* opt_formal_array_elt_type: %empty  */
#line 2807 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9817 "bison-chpl-lib.cpp"
    break;

  case 498: /* opt_formal_array_elt_type: type_level_expr  */
#line 2808 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9823 "bison-chpl-lib.cpp"
    break;

  case 499: /* opt_formal_array_elt_type: query_expr  */
#line 2809 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9829 "bison-chpl-lib.cpp"
    break;

  case 500: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2814 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9838 "bison-chpl-lib.cpp"
    break;

  case 501: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2819 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9846 "bison-chpl-lib.cpp"
    break;

  case 502: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2827 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9855 "bison-chpl-lib.cpp"
    break;

  case 503: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2832 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9863 "bison-chpl-lib.cpp"
    break;

  case 504: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2836 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9871 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_formal_type: %empty  */
#line 2842 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9877 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_formal_type: TCOLON type_level_expr  */
#line 2843 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9883 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_formal_type: TCOLON query_expr  */
#line 2844 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9889 "bison-chpl-lib.cpp"
    break;

  case 508: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2845 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9895 "bison-chpl-lib.cpp"
    break;

  case 509: /* opt_formal_type: TCOLON formal_array_type  */
#line 2846 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9901 "bison-chpl-lib.cpp"
    break;

  case 510: /* expr_ls: expr  */
#line 2852 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9907 "bison-chpl-lib.cpp"
    break;

  case 511: /* expr_ls: query_expr  */
#line 2853 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9913 "bison-chpl-lib.cpp"
    break;

  case 512: /* expr_ls: expr_ls TCOMMA expr  */
#line 2854 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9919 "bison-chpl-lib.cpp"
    break;

  case 513: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2855 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9925 "bison-chpl-lib.cpp"
    break;

  case 514: /* simple_expr_ls: expr  */
#line 2859 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9931 "bison-chpl-lib.cpp"
    break;

  case 515: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2860 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9937 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_component: TUNDERSCORE  */
#line 2864 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9943 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_component: opt_try_expr  */
#line 2865 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9949 "bison-chpl-lib.cpp"
    break;

  case 518: /* tuple_component: query_expr  */
#line 2866 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9955 "bison-chpl-lib.cpp"
    break;

  case 519: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2871 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9963 "bison-chpl-lib.cpp"
    break;

  case 520: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2875 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9971 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_actual_ls: %empty  */
#line 2881 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9977 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_actual_ls: actual_ls  */
#line 2882 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9983 "bison-chpl-lib.cpp"
    break;

  case 523: /* actual_ls: actual_expr  */
#line 2887 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9992 "bison-chpl-lib.cpp"
    break;

  case 524: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2892 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10002 "bison-chpl-lib.cpp"
    break;

  case 525: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2900 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10008 "bison-chpl-lib.cpp"
    break;

  case 526: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2901 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10014 "bison-chpl-lib.cpp"
    break;

  case 527: /* actual_expr: query_expr  */
#line 2902 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10020 "bison-chpl-lib.cpp"
    break;

  case 528: /* actual_expr: opt_try_expr  */
#line 2903 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10026 "bison-chpl-lib.cpp"
    break;

  case 529: /* ident_expr: ident_use  */
#line 2907 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10032 "bison-chpl-lib.cpp"
    break;

  case 530: /* ident_expr: scalar_type  */
#line 2908 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10038 "bison-chpl-lib.cpp"
    break;

  case 531: /* type_level_expr: sub_type_level_expr  */
#line 2920 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 10044 "bison-chpl-lib.cpp"
    break;

  case 532: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2922 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 533: /* type_level_expr: TQUESTION  */
#line 2924 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10056 "bison-chpl-lib.cpp"
    break;

  case 539: /* sub_type_level_expr: TSINGLE expr  */
#line 2935 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10062 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2937 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10068 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2939 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2941 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10080 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2943 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TATOMIC expr  */
#line 2949 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10096 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TSYNC expr  */
#line 2951 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TOWNED  */
#line 2954 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TOWNED expr  */
#line 2956 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10114 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TUNMANAGED  */
#line 2958 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10120 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2960 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10126 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TSHARED  */
#line 2962 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10132 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TSHARED expr  */
#line 2964 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10138 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TBORROWED  */
#line 2966 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10144 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TBORROWED expr  */
#line 2968 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10150 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TCLASS  */
#line 2971 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10156 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TRECORD  */
#line 2973 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10162 "bison-chpl-lib.cpp"
    break;

  case 556: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2978 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10175 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2987 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10188 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TFOR expr TDO expr  */
#line 2996 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3004 "chpl.ypp"
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
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3018 "chpl.ypp"
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
#line 10236 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3032 "chpl.ypp"
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
#line 10254 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3046 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10267 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3055 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10280 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr: TFORALL expr TDO expr  */
#line 3064 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10292 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3072 "chpl.ypp"
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
#line 10310 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3086 "chpl.ypp"
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
#line 10328 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3100 "chpl.ypp"
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
#line 10346 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 3114 "chpl.ypp"
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
#line 10367 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3131 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10380 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3140 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10393 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3149 "chpl.ypp"
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
#line 10412 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3164 "chpl.ypp"
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
#line 10431 "bison-chpl-lib.cpp"
    break;

  case 573: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3182 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10445 "bison-chpl-lib.cpp"
    break;

  case 574: /* nil_expr: TNIL  */
#line 3199 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10451 "bison-chpl-lib.cpp"
    break;

  case 582: /* stmt_level_expr: io_expr TIO expr  */
#line 3215 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10457 "bison-chpl-lib.cpp"
    break;

  case 583: /* opt_task_intent_ls: %empty  */
#line 3219 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10463 "bison-chpl-lib.cpp"
    break;

  case 584: /* opt_task_intent_ls: task_intent_clause  */
#line 3220 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10469 "bison-chpl-lib.cpp"
    break;

  case 585: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3225 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10479 "bison-chpl-lib.cpp"
    break;

  case 586: /* task_intent_ls: intent_expr  */
#line 3233 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10485 "bison-chpl-lib.cpp"
    break;

  case 587: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3234 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10491 "bison-chpl-lib.cpp"
    break;

  case 588: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3239 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10501 "bison-chpl-lib.cpp"
    break;

  case 589: /* forall_intent_ls: intent_expr  */
#line 3247 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10507 "bison-chpl-lib.cpp"
    break;

  case 590: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3248 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10513 "bison-chpl-lib.cpp"
    break;

  case 591: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3253 "chpl.ypp"
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
#line 10532 "bison-chpl-lib.cpp"
    break;

  case 592: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3268 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10540 "bison-chpl-lib.cpp"
    break;

  case 593: /* intent_expr: expr TREDUCE ident_expr  */
#line 3272 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10548 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_var_prefix: TCONST  */
#line 3278 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10554 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_var_prefix: TIN  */
#line 3279 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10560 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_var_prefix: TCONST TIN  */
#line 3280 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10566 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_var_prefix: TREF  */
#line 3281 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10572 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_var_prefix: TCONST TREF  */
#line 3282 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10578 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_var_prefix: TVAR  */
#line 3283 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10584 "bison-chpl-lib.cpp"
    break;

  case 601: /* io_expr: io_expr TIO expr  */
#line 3289 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10590 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW  */
#line 3294 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10596 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_maybe_decorated: TNEW TOWNED  */
#line 3296 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10602 "bison-chpl-lib.cpp"
    break;

  case 604: /* new_maybe_decorated: TNEW TSHARED  */
#line 3298 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10608 "bison-chpl-lib.cpp"
    break;

  case 605: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3300 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10614 "bison-chpl-lib.cpp"
    break;

  case 606: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3302 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10620 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_expr: new_maybe_decorated expr  */
#line 3308 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10628 "bison-chpl-lib.cpp"
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
#line 10644 "bison-chpl-lib.cpp"
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
#line 10660 "bison-chpl-lib.cpp"
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
#line 10677 "bison-chpl-lib.cpp"
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
#line 10695 "bison-chpl-lib.cpp"
    break;

  case 612: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3369 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10703 "bison-chpl-lib.cpp"
    break;

  case 622: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 3388 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10711 "bison-chpl-lib.cpp"
    break;

  case 623: /* expr: expr TCOLON expr  */
#line 3392 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10719 "bison-chpl-lib.cpp"
    break;

  case 624: /* expr: expr TDOTDOT expr  */
#line 3396 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10728 "bison-chpl-lib.cpp"
    break;

  case 625: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 3401 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10737 "bison-chpl-lib.cpp"
    break;

  case 626: /* expr: expr TDOTDOT  */
#line 3406 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10746 "bison-chpl-lib.cpp"
    break;

  case 627: /* expr: TDOTDOT expr  */
#line 3411 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10756 "bison-chpl-lib.cpp"
    break;

  case 628: /* expr: TDOTDOTOPENHIGH expr  */
#line 3417 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10766 "bison-chpl-lib.cpp"
    break;

  case 629: /* expr: TDOTDOT  */
#line 3423 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10776 "bison-chpl-lib.cpp"
    break;

  case 630: /* opt_expr: %empty  */
#line 3454 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10782 "bison-chpl-lib.cpp"
    break;

  case 631: /* opt_expr: expr  */
#line 3455 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10788 "bison-chpl-lib.cpp"
    break;

  case 632: /* opt_try_expr: TTRY expr  */
#line 3459 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10794 "bison-chpl-lib.cpp"
    break;

  case 633: /* opt_try_expr: TTRYBANG expr  */
#line 3460 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10800 "bison-chpl-lib.cpp"
    break;

  case 634: /* opt_try_expr: expr  */
#line 3461 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10806 "bison-chpl-lib.cpp"
    break;

  case 640: /* call_base_expr: expr TBANG  */
#line 3478 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10814 "bison-chpl-lib.cpp"
    break;

  case 641: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3481 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10820 "bison-chpl-lib.cpp"
    break;

  case 644: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3488 "chpl.ypp"
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
#line 10836 "bison-chpl-lib.cpp"
    break;

  case 645: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3500 "chpl.ypp"
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
#line 10852 "bison-chpl-lib.cpp"
    break;

  case 646: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3512 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10860 "bison-chpl-lib.cpp"
    break;

  case 647: /* dot_expr: expr TDOT ident_use  */
#line 3519 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10866 "bison-chpl-lib.cpp"
    break;

  case 648: /* dot_expr: expr TDOT TTYPE  */
#line 3521 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10872 "bison-chpl-lib.cpp"
    break;

  case 649: /* dot_expr: expr TDOT TDOMAIN  */
#line 3523 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10878 "bison-chpl-lib.cpp"
    break;

  case 650: /* dot_expr: expr TDOT TLOCALE  */
#line 3525 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10884 "bison-chpl-lib.cpp"
    break;

  case 651: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3527 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10894 "bison-chpl-lib.cpp"
    break;

  case 652: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3533 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10904 "bison-chpl-lib.cpp"
    break;

  case 653: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3545 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10910 "bison-chpl-lib.cpp"
    break;

  case 654: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3547 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10918 "bison-chpl-lib.cpp"
    break;

  case 655: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3551 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10926 "bison-chpl-lib.cpp"
    break;

  case 656: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3555 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10934 "bison-chpl-lib.cpp"
    break;

  case 657: /* bool_literal: TFALSE  */
#line 3561 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10940 "bison-chpl-lib.cpp"
    break;

  case 658: /* bool_literal: TTRUE  */
#line 3562 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10946 "bison-chpl-lib.cpp"
    break;

  case 659: /* str_bytes_literal: STRINGLITERAL  */
#line 3566 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10952 "bison-chpl-lib.cpp"
    break;

  case 660: /* str_bytes_literal: BYTESLITERAL  */
#line 3567 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10958 "bison-chpl-lib.cpp"
    break;

  case 663: /* literal_expr: INTLITERAL  */
#line 3573 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10964 "bison-chpl-lib.cpp"
    break;

  case 664: /* literal_expr: REALLITERAL  */
#line 3574 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10970 "bison-chpl-lib.cpp"
    break;

  case 665: /* literal_expr: IMAGLITERAL  */
#line 3575 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10976 "bison-chpl-lib.cpp"
    break;

  case 666: /* literal_expr: CSTRINGLITERAL  */
#line 3576 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10982 "bison-chpl-lib.cpp"
    break;

  case 667: /* literal_expr: TNONE  */
#line 3577 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10988 "bison-chpl-lib.cpp"
    break;

  case 668: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3579 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10997 "bison-chpl-lib.cpp"
    break;

  case 669: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3584 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11006 "bison-chpl-lib.cpp"
    break;

  case 670: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3589 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11014 "bison-chpl-lib.cpp"
    break;

  case 671: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3593 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11023 "bison-chpl-lib.cpp"
    break;

  case 672: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3598 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11031 "bison-chpl-lib.cpp"
    break;

  case 673: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3602 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11040 "bison-chpl-lib.cpp"
    break;

  case 674: /* assoc_expr_ls: expr TALIAS expr  */
#line 3611 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11049 "bison-chpl-lib.cpp"
    break;

  case 675: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3616 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11059 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr TPLUS expr  */
#line 3624 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11065 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TMINUS expr  */
#line 3625 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11071 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TSTAR expr  */
#line 3626 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11077 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr TDIVIDE expr  */
#line 3627 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11083 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3628 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11089 "bison-chpl-lib.cpp"
    break;

  case 681: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3629 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11095 "bison-chpl-lib.cpp"
    break;

  case 682: /* binary_op_expr: expr TMOD expr  */
#line 3630 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11101 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TEQUAL expr  */
#line 3631 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11107 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3632 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11113 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3633 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11119 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3634 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11125 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TLESS expr  */
#line 3635 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11131 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TGREATER expr  */
#line 3636 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11137 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TBAND expr  */
#line 3637 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11143 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TBOR expr  */
#line 3638 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11149 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TBXOR expr  */
#line 3639 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11155 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TAND expr  */
#line 3640 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11161 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TOR expr  */
#line 3641 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11167 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TEXP expr  */
#line 3642 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11173 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TBY expr  */
#line 3643 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11179 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TALIGN expr  */
#line 3644 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11185 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr THASH expr  */
#line 3645 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11191 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TDMAPPED expr  */
#line 3646 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11197 "bison-chpl-lib.cpp"
    break;

  case 699: /* unary_op_expr: TPLUS expr  */
#line 3650 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11203 "bison-chpl-lib.cpp"
    break;

  case 700: /* unary_op_expr: TMINUS expr  */
#line 3651 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11209 "bison-chpl-lib.cpp"
    break;

  case 701: /* unary_op_expr: TMINUSMINUS expr  */
#line 3652 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11215 "bison-chpl-lib.cpp"
    break;

  case 702: /* unary_op_expr: TPLUSPLUS expr  */
#line 3653 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11221 "bison-chpl-lib.cpp"
    break;

  case 703: /* unary_op_expr: TBANG expr  */
#line 3654 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11227 "bison-chpl-lib.cpp"
    break;

  case 704: /* unary_op_expr: expr TBANG  */
#line 3655 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11235 "bison-chpl-lib.cpp"
    break;

  case 705: /* unary_op_expr: TBNOT expr  */
#line 3658 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11241 "bison-chpl-lib.cpp"
    break;

  case 706: /* reduce_expr: expr TREDUCE expr  */
#line 3663 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11249 "bison-chpl-lib.cpp"
    break;

  case 707: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3667 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11257 "bison-chpl-lib.cpp"
    break;

  case 708: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3671 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11265 "bison-chpl-lib.cpp"
    break;

  case 709: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3675 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11273 "bison-chpl-lib.cpp"
    break;

  case 710: /* scan_expr: expr TSCAN expr  */
#line 3682 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11281 "bison-chpl-lib.cpp"
    break;

  case 711: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3686 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11289 "bison-chpl-lib.cpp"
    break;

  case 712: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3690 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11297 "bison-chpl-lib.cpp"
    break;

  case 713: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3694 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11305 "bison-chpl-lib.cpp"
    break;


#line 11309 "bison-chpl-lib.cpp"

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
