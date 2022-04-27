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
#line 318 "chpl.ypp"

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
#define YYLAST   20675

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
       0,   601,   601,   605,   606,   611,   612,   620,   624,   632,
     633,   634,   635,   636,   637,   638,   639,   640,   641,   642,
     655,   656,   657,   666,   675,   679,   686,   694,   695,   696,
     697,   698,   699,   700,   701,   715,   730,   738,   750,   761,
     773,   785,   796,   812,   814,   813,   822,   821,   832,   833,
     837,   857,   872,   887,   903,   904,   906,   911,   912,   917,
     935,   939,   943,   954,   973,   974,   978,   982,   986,   991,
     995,   999,  1009,  1014,  1020,  1027,  1032,  1039,  1050,  1051,
    1055,  1059,  1066,  1074,  1083,  1090,  1098,  1110,  1117,  1121,
    1126,  1132,  1141,  1142,  1146,  1155,  1159,  1163,  1167,  1171,
    1175,  1184,  1185,  1189,  1190,  1191,  1192,  1193,  1194,  1200,
    1201,  1202,  1203,  1204,  1205,  1220,  1221,  1232,  1233,  1234,
    1235,  1236,  1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,
    1245,  1246,  1247,  1248,  1249,  1250,  1254,  1255,  1256,  1257,
    1258,  1259,  1260,  1261,  1262,  1263,  1264,  1265,  1272,  1273,
    1274,  1275,  1279,  1280,  1284,  1291,  1301,  1303,  1302,  1311,
    1310,  1321,  1325,  1331,  1331,  1337,  1337,  1346,  1347,  1348,
    1349,  1350,  1351,  1352,  1356,  1361,  1366,  1371,  1378,  1386,
    1387,  1391,  1400,  1406,  1414,  1433,  1432,  1442,  1441,  1454,
    1461,  1479,  1492,  1505,  1509,  1513,  1517,  1521,  1525,  1529,
    1533,  1551,  1555,  1559,  1563,  1567,  1571,  1575,  1579,  1583,
    1587,  1591,  1595,  1599,  1603,  1607,  1611,  1615,  1619,  1623,
    1627,  1631,  1635,  1639,  1644,  1652,  1661,  1665,  1669,  1673,
    1677,  1681,  1685,  1689,  1693,  1698,  1703,  1708,  1716,  1731,
    1749,  1753,  1760,  1761,  1766,  1771,  1772,  1773,  1774,  1775,
    1776,  1777,  1778,  1779,  1780,  1781,  1782,  1783,  1795,  1796,
    1797,  1798,  1807,  1808,  1812,  1816,  1820,  1827,  1831,  1835,
    1842,  1846,  1850,  1854,  1861,  1862,  1866,  1870,  1874,  1881,
    1894,  1910,  1918,  1922,  1931,  1932,  1936,  1940,  1945,  1953,
    1958,  1962,  1969,  1970,  1974,  1983,  1988,  1999,  2006,  2007,
    2008,  2012,  2013,  2017,  2021,  2025,  2032,  2049,  2062,  2069,
    2074,  2081,  2080,  2093,  2095,  2094,  2103,  2102,  2113,  2120,
    2132,  2149,  2147,  2174,  2178,  2179,  2181,  2186,  2196,  2193,
    2221,  2229,  2237,  2248,  2259,  2268,  2283,  2284,  2288,  2289,
    2290,  2299,  2300,  2301,  2302,  2303,  2304,  2305,  2306,  2307,
    2308,  2309,  2310,  2311,  2312,  2313,  2314,  2315,  2316,  2317,
    2318,  2319,  2320,  2321,  2322,  2323,  2324,  2328,  2329,  2330,
    2331,  2332,  2333,  2334,  2335,  2336,  2337,  2338,  2339,  2344,
    2345,  2349,  2350,  2351,  2355,  2356,  2360,  2364,  2365,  2369,
    2370,  2374,  2383,  2394,  2404,  2416,  2428,  2436,  2440,  2448,
    2449,  2450,  2451,  2452,  2453,  2454,  2455,  2456,  2460,  2461,
    2462,  2463,  2464,  2465,  2469,  2470,  2471,  2475,  2476,  2477,
    2478,  2479,  2480,  2484,  2485,  2488,  2489,  2493,  2494,  2498,
    2503,  2504,  2506,  2508,  2510,  2515,  2517,  2522,  2524,  2526,
    2528,  2530,  2532,  2534,  2539,  2540,  2544,  2553,  2557,  2565,
    2569,  2576,  2597,  2598,  2600,  2608,  2609,  2610,  2611,  2612,
    2617,  2616,  2625,  2633,  2637,  2644,  2660,  2677,  2681,  2685,
    2692,  2694,  2696,  2703,  2704,  2705,  2710,  2714,  2718,  2722,
    2726,  2730,  2734,  2741,  2742,  2743,  2744,  2745,  2750,  2751,
    2752,  2753,  2754,  2774,  2778,  2782,  2786,  2793,  2794,  2795,
    2799,  2804,  2812,  2817,  2821,  2828,  2829,  2830,  2831,  2832,
    2838,  2839,  2840,  2841,  2845,  2846,  2850,  2851,  2852,  2856,
    2860,  2867,  2868,  2872,  2877,  2886,  2887,  2888,  2889,  2893,
    2894,  2905,  2907,  2909,  2915,  2916,  2917,  2918,  2919,  2920,
    2922,  2924,  2926,  2928,  2934,  2936,  2939,  2941,  2943,  2945,
    2947,  2949,  2951,  2953,  2956,  2958,  2963,  2972,  2981,  2989,
    3003,  3017,  3031,  3040,  3049,  3057,  3071,  3085,  3099,  3116,
    3125,  3134,  3149,  3167,  3185,  3193,  3194,  3195,  3196,  3197,
    3198,  3199,  3200,  3205,  3206,  3210,  3219,  3220,  3224,  3233,
    3234,  3238,  3253,  3257,  3264,  3265,  3266,  3267,  3268,  3269,
    3273,  3274,  3279,  3281,  3283,  3285,  3287,  3293,  3300,  3312,
    3324,  3337,  3354,  3364,  3365,  3366,  3367,  3368,  3369,  3370,
    3371,  3372,  3373,  3377,  3381,  3386,  3391,  3396,  3402,  3408,
    3440,  3441,  3445,  3446,  3447,  3451,  3452,  3453,  3454,  3463,
    3464,  3467,  3468,  3469,  3473,  3485,  3497,  3504,  3506,  3508,
    3510,  3512,  3518,  3531,  3532,  3536,  3540,  3547,  3548,  3552,
    3553,  3557,  3558,  3559,  3560,  3561,  3562,  3563,  3564,  3568,
    3572,  3576,  3580,  3584,  3593,  3598,  3607,  3608,  3609,  3610,
    3611,  3612,  3613,  3614,  3615,  3616,  3617,  3618,  3619,  3620,
    3621,  3622,  3623,  3624,  3625,  3626,  3627,  3628,  3629,  3633,
    3634,  3635,  3636,  3637,  3638,  3641,  3645,  3649,  3653,  3658,
    3666,  3670,  3674,  3678,  3686,  3687,  3688,  3689,  3690,  3691,
    3692
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

#define YYPACT_NINF (-1126)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-721)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1126,   138,  3873, -1126,   -66,   220, -1126, -1126, -1126, -1126,
   -1126, -1126, 12239,    35,   285,   256, 14971,   317, 20385,    35,
   12239,   338,   217,    59,   285,  5273, 12239,   327,  5273,   240,
   20472, -1126,   424,   391,  9109, 10501, 10501, -1126,  9282,   408,
     482,   298, -1126,   429, 20472, 20472, 20472, -1126,  3143, 10674,
     445, 12239, 12239,    79, -1126,   500,   523, 12239, -1126, 14971,
   -1126, 12239,   509,   430,   228,   263,   571, 20559, -1126, 10849,
    8588, 12239, 10674, 14971, 12239,   551,   598,   489,  5273,   613,
   12239,   624,  7544,  7544, -1126,   631, -1126, 14971, -1126,   636,
    9282, 12239, -1126, 12239, -1126, 12239, -1126, -1126, 14488, 12239,
   -1126, 12239, -1126, -1126, -1126,  4223,  7719,  9457, 12239, -1126,
    5098, -1126, -1126, -1126, -1126,   533, -1126,   261, -1126, -1126,
   -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126,
   -1126,  7892, -1126, 11022, -1126, -1126, -1126, -1126, -1126,   659,
   -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126,   578, 20472,
   -1126, 20110,   544,   450, -1126,    57, -1126, -1126,    45,   238,
   -1126, 20472,  3143, -1126,   559, -1126,   561, -1126, -1126,   563,
     566,   570, 12239,   572,   575, 19790,  3224,   -49,   577,   580,
   -1126, -1126,   461, -1126, -1126, -1126, -1126, -1126,   283, -1126,
   -1126, -1126, 12239, 12239, 12239, 20472, -1126, 12239, 10849, 10849,
     680,   471, -1126, -1126, -1126, -1126, 19790,   507, -1126, -1126,
     576,  5273, -1126, -1126,   584,   130,   585, 16756, -1126,   242,
   -1126,   591, -1126,   102, 19790, -1126,  2761,   635,  8763, -1126,
   -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126,
   -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126,
   -1126, -1126, -1126,   588, -1126, -1126, 20472,   590,    40, 16362,
      32, 16273,    32, 16322, 20472, 20472,   -20, 15513, -1126, -1126,
     672,  8763,   593,   420,  5273,  1255,    84,    15, -1126,  5273,
   -1126, -1126, 16796, -1126,    20, 17305,   451, -1126,   594,   596,
   -1126, 16796,   130,   451, -1126,  8763,  2246,  2246, -1126, -1126,
     116, 19790, 12239, 12239, -1126, 19790,   595, 17412, -1126, 16796,
     130, 19790,   601,  8763, -1126, 19790, 17457, -1126, -1126, -1126,
   -1126,   130,    40, 16796, 17497,   193,   193,  1697,   451,   451,
      76, -1126, -1126,  4398,   173, -1126, 12239, -1126,   -55,   -43,
   -1126,   -16,    97, 17649,   -58,  1697,   751, -1126,  4573, 12239,
   -1126, 12239,   695, -1126, -1126, 15204,    58,   578, 19790,   620,
   20472, 10849,   607, -1126,   592,   772,   657,   308, -1126, -1126,
     244,    45, -1126, -1126, -1126, -1126,   696, -1126, -1126, -1126,
     139,   697, -1126, -1126, -1126,  3413,   660,   277, -1126,   290,
     511, -1126, 12239,   641, 12239, 12239, 12239, 10501, 10501, 12239,
     512, 12239, 12239, 12239, 12239, 12239,   586, 14488, 12239, 12239,
   12239, 12239, 12239, 12239, 12239, 12239, 12239, 12239, 12239, 12239,
   12239, 12239, 12239, 12239,   721, -1126, -1126, -1126, -1126, -1126,
    9630,  9630, -1126, -1126, -1126, -1126,  9630, -1126, -1126,  9630,
    9630,  8763,  8763, 10501, 10501, 16836, 16988, 17689,   619,    16,
   20472,  8415, -1126, -1126,  4748, -1126, 10501,    40,  3143, -1126,
   12239, -1126,  2761, -1126, 20472, -1126, -1126, -1126,   672, 12239,
     667, -1126,   621,   651, -1126, -1126, -1126,   750, 10849, -1126,
    5448, 10501, -1126,   627, -1126,    40,  5623, 10501, -1126,    40,
   -1126,    40, 10501, -1126,    40,   677,   678,  5273,   770,  5273,
   -1126,   771, 12239,   741,   637,  8763, 20472, -1126, -1126, -1126,
    1255, -1126, -1126,   -40, -1126, 12412,   692, 12239,  3143, -1126,
   -1126, 12239, -1126, 20197, 12239, 12239, -1126,   648, -1126, -1126,
   10849, -1126, 19790, 19790, -1126,    47, -1126,  8763,   650, -1126,
     804,   804, -1126, -1126, -1126, -1126, -1126, -1126,  9805, -1126,
   17729,  8067, -1126,  8242, -1126,  5273,   654, 10501,  9980,  4048,
     655, 12239, 11195, -1126, -1126,   423, -1126,  4923, -1126,   346,
   17881,   351, 16514, 20472,  7371,  7371, -1126,   578,   661, -1126,
     242, -1126,   194,   687,  2451, -1126, -1126, 20472, 12239,   313,
   -1126,    52, -1126, -1126, -1126, -1126, -1126, -1126,   279,   691,
     666, -1126, 15217, -1126,    66, -1126, -1126, 12585,   743, -1126,
     674,   137, -1126,   705, -1126,   706,   707,   718,   712,   713,
   -1126,   714,   724,   717,   720,   723,   453,   733,   726,   727,
   -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126,
   -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126,
   -1126, -1126, -1126, 12239, -1126,   736,   737,   730,   674, -1126,
     674, -1126, 12758, -1126, 20472, -1126, -1126, 17921, 19942, 19942,
     216, -1126,   216, -1126,   216,  3122,  2018,  1402,  1281,   130,
     193, -1126,   516, -1126, -1126, -1126, -1126, -1126,  1697,  2580,
     216,  1531,  1531, 19942,  1531,  1531,  1728,   193,  2580,  2427,
    1728,   451,   451,   193,  1697,   715,   725,   728,   729,   731,
     734,   716,   710, -1126,   216, -1126,   216, 12931, 10501, 13104,
   10501, 12239,  8763, 10501,  3689,   719,   184, -1126, -1126, -1126,
      69, -1126,  1097, 19866,   410,    35, 17966, -1126,   369, 19790,
   -1126, 18073,  8763, -1126,  8763, 12239,    88,  9282, 19790,    44,
   17028,  8415, -1126,  9282, 19790,    43, 16559, -1126, -1126,    32,
   16604, -1126, 12239, 12239,   865,  5273,   866, 18118,  5273, 17068,
   20472, -1126,   100, -1126,   107, -1126,   205,  1255,    84,    49,
      51, 12239, 12239,  7198, -1126, -1126,   197,  8936, -1126, 19790,
   -1126, -1126, -1126, 20472, 18158, 18310, -1126, -1126, 19790,   732,
      -4,   735, -1126,  1922, -1126, -1126, -1126, -1126, -1126, -1126,
   -1126, -1126,  5273,   -10, 17220, -1126, -1126, 19790,  5273, 19790,
   -1126, 18351, -1126, -1126, 12239, -1126,    71, 15271, 12239, -1126,
   11368,  7371,  7371, -1126, 12239,   477, 12239,   539,   583, 12239,
   10153,   589,   396, -1126, -1126, -1126, 18392,   758,   740,   744,
   -1126, 20472,  3143, -1126,  8763,   742,  1446, 20472, -1126, 19790,
   20298, -1126, -1126, 20472,   836, -1126,  1255,   766,    29,  7023,
   -1126, -1126,   373, -1126,    29,   244, -1126, 18510, -1126, 15430,
   -1126, -1126, -1126,   406, -1126,   745,   752, -1126, -1126, -1126,
   -1126, -1126, -1126, -1126, -1126, 12239,   883, 18551, 12239,   897,
   18592,   206,   760, 18710,  8763, -1126, -1126,  8415, -1126, -1126,
   -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126, -1126,
      53, 10501, 10501,    40,    40, -1126, -1126, -1126, -1126, -1126,
   16796, -1126, 15589,  5798, -1126,  5973, -1126,   208, -1126, 15665,
    6148, -1126,    40,  6323, -1126,    40, -1126,    40, -1126,    40,
   19790, 19790,  5273, -1126,  5273, 12239, -1126,  5273,   898,   767,
     773, 20472,   585, -1126, -1126,   692,   761,    81, -1126, -1126,
   -1126,   765,   768, -1126,  6498, 10849, -1126, -1126, -1126, 20472,
   -1126,   794,   585, -1126,  6673,   769,  6848,   774, -1126, 12239,
   -1126, -1126,  7371, -1126, 18751,   187, 17260,   407,   776,  7371,
   -1126, 12239, -1126, -1126,  2593,   418,   432,   267, -1126,   934,
   -1126, -1126, -1126,   757, -1126, -1126, -1126, -1126,   766, -1126,
     268, 11543,   348,   148, 12239, -1126,   775, 14315,    89, -1126,
     778, -1126,   806,   808,   674,   674, -1126, -1126, 18869, 13277,
   13450, 18945, 13623, 13796, -1126, 13969, 14142,   270, -1126,   692,
     299,   303, -1126, -1126, -1126,  5273,  9282, 19790,  9282, 19790,
    8415, -1126,  5273,  9282, 19790, -1126,  9282, 19790, -1126, -1126,
   -1126, -1126, -1126, 19790,   918,  5273, -1126, -1126, -1126, -1126,
   -1126, -1126, 10501, 15112,  8763,  8763,  5273, -1126,    23,   781,
   12239, -1126,  9282, -1126, 19790,  5273,  9282, -1126, 19790,  5273,
   19790,   331, 11716,  7371,  7371,  7371,  7371, -1126, -1126, -1126,
   19030, 19790,  2950, -1126, -1126, -1126,   790, -1126,   587, -1126,
     348,   766, 10328, -1126, -1126, -1126,   321, 10849, -1126, -1126,
   -1126,   109, -1126,    -2, -1126,   505, 19106,     6, -1126, -1126,
     390, 14658,   146, -1126, -1126, -1126, 12239, 12239, 12239, 12239,
   12239, 12239, 12239, 12239, -1126, -1126, 18118, 15741, 15817, -1126,
   18118, 15893, 15969,  5273, -1126, 19182, -1126,   573,   784,   785,
     635, -1126,   585, 19790, 16045, -1126, 16121, -1126, -1126, -1126,
   19790,   435,   795,   472,   796, 12239, -1126, -1126,   587, -1126,
   -1126, -1126,   348, 11891,    91, -1126, 19790, -1126, 12239,   148,
     109,   109,   109,   109,   109,   109,   148, -1126, -1126, -1126,
   -1126,   490, -1126, -1126, -1126, 19258, 19334, 19410, 19486, 19562,
   19638,  5273,  5273,  5273,  5273, -1126, 14142,   798,   800, -1126,
    5273,  5273, -1126, -1126, -1126, -1126, 19790, -1126, -1126, -1126,
   -1126, -1126, -1126,   364, 10501, 14799, 19790, -1126, -1126, -1126,
   -1126, -1126, -1126, -1126,   813, 12239, 12239, 12239, 12239, 12239,
   12239, 18118, 18118, 18118, 18118,   686, -1126, -1126, 18118, 18118,
   19714, -1126, -1126, 12066, -1126
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
   -1126, -1126, -1126,     7,  -368,  2362,   343, -1126, -1126, -1126,
    -199, -1126, -1126,   -25,   699, -1126,  -656,  2696,   617,  -536,
   -1126,  -804, -1126, -1126,   142, -1126, -1126, -1126,   943, -1126,
    2822,  -187,  -379, -1126,  -588,  1257,  -611,   -42, -1126, -1126,
    -825, -1126, -1126,   404, -1126, -1126, -1126, -1126, -1126, -1126,
   -1126, -1126, -1126,   161, -1126,   880, -1126, -1126,    12,  2148,
   -1126, -1126, -1126, -1126,   656, -1126,    -1, -1126, -1126, -1126,
   -1126,   454, -1126, -1126, -1126,  -104, -1126,  -327,  -795, -1126,
   -1126, -1126, -1126,   119, -1126, -1126,  -554, -1126,   824, -1126,
   -1126, -1126, -1126,   622, -1126, -1126, -1126, -1126,   104,  -373,
    -165,  -777,  -964,  -614, -1126, -1126,   110,   120,   388, -1126,
   -1126, -1126,   623, -1126, -1126,  -152,    99,   117,  -211,  -201,
    -681, -1126, -1126,   149,   339, -1126,  -209,   871, -1126,  -159,
     486,   229,  -492,  -913,  -958, -1126,  -584,  -645, -1125, -1092,
    -948,   -56, -1126,    93, -1126,  -270,  -489,  -505,   502,  -473,
   -1126, -1126, -1126,   842, -1126,   -14, -1126, -1126,  -227, -1126,
    -730, -1126, -1126, -1126,  1146,  1334,   -12, -1126,    22,  1656,
   -1126,  1849,  1948, -1126, -1126, -1126, -1126, -1126, -1126, -1126,
   -1126, -1126,  -444
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
    1120,   974,   376,   279,   681,   321,   940,   933,   323,   324,
     463,   325,   497,   326,   470,   514,   327,   328,   562,   329,
    1023,   551,   306,   301,   305,   343,   345,   287,    70,   189,
     861,  1242,   502,   553,   483,  -262,   777,  -263,   470,  1129,
     483,   372,  1130,   975,   556,   560,  1198,   563,   552,   355,
    1271,   358,  -284,   441,   377,   442,   470,  -488,   340,  1082,
     554,   373,  -488,   778,   872,   378,  1155,   723,     3,  1244,
     606,   374,   862,   449,  1199,   557,   466,  1024,  1274,   220,
     454,  1130,   483,  1272,   379,   210,  1190,   288,  -488,   555,
     393,   518,   558,   483,  -284,   984,   521,   976,   299,   530,
      86,   711,   712,  1192,   685,   289,   454,  1048,  -488,  1015,
     445,   446,   447,  1210,  -488,   315,   301,   343,  1015,   883,
     454,   724,   290,   454,   965,   258,   260,   262,   515,  -488,
    -483,   468,   454,   457,   299,   337,   299,  1191,  1112,  -488,
     454,   515,  1132,   454,   454,   907,   305,   483,  -284,   687,
     660,   228,  1109,   271,  1103,   396,  -488,   558,  1238,   515,
    -488,  -483,   905,   901,   530,   558,  -488,   558,    70,   929,
    1131,   189,   908,   558,  -483,  -489,   744,   545,   460,  -488,
     849,  1132,   577,   961,   470,   470,  1083,  -483,  -489,   305,
    1104,   931,   530,   740,  1141,   381,  1245,   461,   341,    23,
     400,   591,   559,   960,   906,   -56,  -245,  -109,   406,  1239,
     962,   531,   397,   305,   607,  -115,   398,  1015,   213,  -489,
     532,   533,   592,  1189,   -56,   997,   593,   299,   299,   738,
    1182,   305,  1184,  1016,  -483,   583,   591,   734,   349,  -483,
     -55,  1212,  -246,   -56,   793,  -489,   454,   382,   470,   548,
     594,    60,  -489,   595,   550,    62,   218,   592,   383,   -55,
    1159,   593,    68,   400,   596,   225,  1049,   570,   404,   572,
     547,   406,   -56,  -489,   549,   409,   350,   384,   -55,   301,
     470,   777,   744,   597,  1060,   594,   400,    88,   595,   853,
     346,   404,  -489,  1237,   406,  1007,   854,  -489,   409,   596,
    -489,   852,   443,  -252,  1020,   351,   444,   -55,   963,  1044,
     667,  1061,   668,   669,   670,   672,   674,   675,   597,   676,
     677,   678,   679,   680,  -251,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   698,   699,   700,   701,   702,
     703,   704,   228,   744,   777,  1047,   744,  -389,   305,   305,
    1144,  1145,   746,   664,   305,   317,   319,   305,   305,   305,
     305,   714,   716,  1123,  -484,   255,   518,   468,  1166,   733,
    1116,  1121,   665,  1154,   736,  -593,  -506,  -260,   739,  -592,
    1197,  -476,   706,   707,   589,   666,  1101,   741,   708,  -310,
     299,   709,   710,   734,  -250,  -484,   301,  -506,   748,   750,
     271,  -506,  -593,   787,   754,   756,  -592,   999,  -484,   590,
     760,  1207,  -476,   396,  -310,  -247,  1207,   767,  1013,  -498,
     769,  -484,   824,   305,  -506,  -476,   598,   828,  1127,   922,
     656,  -254,  1178,   444,   164,   789,   659,   598,  -476,   285,
    -498,   825,   794,   795,  -498,   518,   829,   294,   798,  1248,
    1249,  1250,  1251,  1252,  1253,   305,  1208,   164,   942,   945,
     164,  1208,   947,   949,   925,   470,   798,  -498,  -484,   305,
     397,   305,   999,  -484,   398,   814,   798,   817,  1126,   819,
     821,  -478,  -453,   999,  1140,  -476,  -258,   470,   671,   673,
    -476,  1000,   846,   846,   664,   268,  -718,  1181,   269,  1183,
    -718,  -453,  1107,   340,   547,   340,   859,   299,   518,  -255,
     164,   999,  -478,  1114,   164,   164,   985,   987,   189,   681,
     454,   400,   506,  -111,   822,  -478,   404,  1115,   405,   406,
    1232,  -116,   295,   409,   713,   715,   682,   164,  -478,   463,
    1167,   416,   164,    22,    23,  -579,   734,   735,   999,   422,
     683,  -493,  -642,   464,  -642,    31,   465,  -249,  -719,   797,
    1200,    37,  -719,  -643,  -493,  -643,  1013,  1234,    42,   991,
     687,   877,   749,  -642,   808,  -642,   810,   797,   755,   684,
     337,   312,   337,   759,  -253,  -478,  1201,   797,  1202,  1203,
    -478,   313,  1204,  1205,    58,  -493,    60,   470,  1211,  -259,
    1010,    42,  -720,  1011,  -323,   466,  -720,    68,  -715,  -639,
    -261,  -639,  -715,  -641,  -640,  -641,  -640,  -248,   885,   685,
     886,  -493,  -256,  1006,  -323,   686,    84,    58,  -493,    86,
     528,   529,    88,   348,  -323,   748,   897,   754,   900,   767,
     305,   903,   817,   164,  1035,   360,   370,   470,   813,  -493,
    1243,   923,   924,   361,  -576,   390,   392,   967,  -575,   579,
     305,   391,   305,   930,  -495,   932,   450,  -580,  -493,   733,
    -581,   939,  -578,  -493,  1013,  -577,  -493,  -495,   451,   453,
     950,   951,   103,  1265,   469,   454,   459,   350,   476,   564,
     534,   573,   478,   585,   928,   505,   524,   502,   525,   311,
     315,   343,  1243,   537,   502,   305,   164,   584,  -495,   463,
     586,   164,   588,    22,    23,   662,   605,   609,  1009,   406,
     705,   722,   742,   464,   743,    31,   465,   744,   745,   751,
    1243,    37,   762,   763,  -495,   765,   768,   770,    42,   969,
     771,  -495,   570,  1028,  1168,  1169,   994,   787,   996,   846,
     846,   796,   325,   802,   326,   803,   153,   328,   329,   812,
     818,   850,  -495,   530,    58,   164,    60,   864,   346,   865,
    1010,   927,   305,  1011,   169,   466,   875,    68,   873,   153,
     164,  -495,   153,  -136,  -143,  -142,  -495,   343,  -112,  -495,
    -141,  -138,  -145,   591,  -110,  -146,    84,   169,  -140,    86,
     169,  -144,    88,  -113,  -139,  -147,   878,  -114,   879,   896,
     887,   899,   299,  1038,   592,   894,  1041,   661,   593,   893,
     888,   904,   305,   889,   890,   733,   891,   470,   470,   892,
     952,   954,   153,   973,   999,  1002,   153,   153,   978,   672,
     714,  1021,   594,  1008,  1003,   595,  1039,    62,  1036,  1088,
     169,  1057,   103,  1059,   169,   169,   596,  1037,  1064,   153,
    1042,  1067,  1076,  1075,   153,  1045,  1081,  1084,  1077,  1090,
    1085,  1108,  1117,  1073,  1095,   597,  -109,   169,  -111,  1099,
    1138,  1143,   169,  1163,  1172,  1187,   164,  1227,  1228,  1199,
    1233,  1235,  1266,   301,  1267,   567,   990,   221,   299,   503,
     322,  1119,  1094,  1078,  1098,   791,   541,  1100,  1089,  1017,
     846,   581,   164,  1034,  1019,  1030,   863,  1110,   164,  1111,
    1214,  1029,   502,   502,   604,  1254,   502,   502,  1247,   164,
    1005,   164,   354,   884,   790,     0,   964,     0,     0,   502,
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
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,   257,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   199,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,   194,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,   302,   303,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
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
       0,   397,     0,     0,     0,   398,     0,     0,     0,   394,
       0,     0,     0,     0,     0,     0,   395,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,     0,     0,     0,
     398,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,     0,     0,   420,   421,
     422,   399,   535,     0,     0,     0,   397,     0,     0,   423,
     398,     0,     0,     0,     0,     0,     0,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,   411,   412,     0,   413,   414,   415,   416,   417,   418,
     419,   399,   539,   420,   421,   422,     0,     0,     0,     0,
       0,     0,     0,     0,   423,     0,     0,   400,   401,     0,
     402,   403,   404,     0,   405,   406,   407,     0,   408,   409,
     410,   411,   412,     0,   413,   414,   415,   416,   417,   418,
     419,   394,   544,   420,   421,   422,     0,     0,   395,     0,
       0,     0,     0,     0,   423,     0,     0,     0,     0,     0,
       0,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   394,     0,     0,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   397,     0,
       0,   394,   398,     0,     0,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,   399,     0,     0,     0,     0,   397,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,   400,
     401,   721,   402,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,   411,   412,     0,   413,   414,   415,   416,
     417,   418,   419,   399,     0,   420,   421,   422,   397,     0,
       0,     0,   398,     0,     0,     0,   423,     0,     0,   400,
     401,     0,   402,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,   411,   412,     0,   413,   414,   415,   416,
     417,   418,   419,   399,     0,   420,   421,   422,     0,     0,
       0,     0,     0,     0,     0,     0,   423,     0,     0,   400,
     401,     0,   402,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,   411,   412,     0,   413,   414,   415,   416,
     417,   418,   419,   394,   826,   420,   421,   422,     0,     0,
     395,     0,   806,     0,     0,     0,   423,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     397,     0,     0,     0,   398,     0,     0,     0,   394,     0,
       0,     0,     0,     0,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,   399,     0,     0,     0,     0,
     397,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,   400,   401,     0,   402,   403,   404,     0,   405,   827,
     407,     0,   408,   409,   410,   411,   412,     0,   413,   414,
     415,   416,   417,   418,   419,   399,     0,   420,   421,   422,
       0,     0,     0,     0,     0,   397,     0,     0,   423,   398,
       0,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,     0,   408,   409,   410,   411,   412,  -601,   413,   414,
     415,   416,   417,   418,   419,   394,   210,   420,   421,   422,
     399,     0,   395,     0,     0,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,   396,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     394,     0,   420,   421,   422,     0,     0,   395,     0,     0,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
     396,     0,     0,   955,     0,     0,     0,     0,     0,     0,
       0,     0,   397,     0,     0,     0,   398,     0,     0,     0,
     394,     0,     0,     0,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   397,     0,     0,
       0,   398,     0,   400,   401,     0,   402,   403,   404,     0,
     405,   406,   407,     0,   408,   409,   410,   411,   412,     0,
     413,   414,   415,   416,   417,   418,   419,     0,   926,   420,
     421,   422,   399,     0,     0,     0,     0,   397,     0,     0,
     423,   398,     0,     0,     0,     0,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   399,     0,   420,   421,   422,     0,     0,     0,
       0,     0,     0,     0,     0,   423,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   394,     0,   420,   421,   422,     0,     0,   395,
       0,   971,     0,     0,     0,   423,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   394,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
       0,     0,     0,   398,   394,  1001,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
     397,     0,     0,     0,   398,     0,     0,     0,     0,     0,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,   989,   399,   420,   421,   422,     0,
       0,   397,     0,   972,     0,   398,     0,   423,     0,     0,
       0,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,     0,   408,   409,   410,   411,   412,     0,   413,   414,
     415,   416,   417,   418,   419,     0,   399,   420,   421,   422,
       0,     0,   394,     0,     0,     0,     0,     0,   423,   395,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,   396,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,     0,     0,   420,   421,
     422,     0,     0,   394,     0,     0,     0,     0,     0,   423,
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,  1040,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
       0,     0,     0,   398,   394,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,  1043,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
     397,     0,     0,     0,   398,     0,     0,     0,     0,     0,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,     0,   399,   420,   421,   422,     0,
       0,   397,     0,  1031,     0,   398,     0,   423,     0,     0,
       0,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,     0,   408,   409,   410,   411,   412,     0,   413,   414,
     415,   416,   417,   418,   419,     0,   399,   420,   421,   422,
       0,     0,   394,     0,     0,     0,     0,     0,   423,   395,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,   396,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,     0,     0,   420,   421,
     422,     0,     0,   394,  1102,     0,     0,     0,     0,   423,
     395,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
     397,     0,     0,     0,   398,     0,     0,     0,     0,     0,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,     0,   399,   420,   421,   422,     0,
       0,   394,     0,     0,     0,  1046,     0,   423,   395,     0,
       0,   400,   401,     0,   402,   403,   404,     0,   405,   406,
     407,   396,   408,   409,   410,   411,   412,     0,   413,   414,
     415,   416,   417,   418,   419,     0,     0,   420,   421,   422,
       0,     0,     0,     0,     0,     0,     0,     0,   423,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   394,   397,     0,
       0,     0,   398,     0,   395,     0,     0,     0,     0,     0,
       0,  1146,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   399,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     401,     0,   402,   403,   404,     0,   405,   406,   407,     0,
     408,   409,   410,   411,   412,     0,   413,   414,   415,   416,
     417,   418,   419,     0,   397,   420,   421,   422,   398,     0,
       0,     0,   394,  1185,     0,     0,   423,  1149,     0,   395,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,   401,     0,   402,   403,
     404,     0,   405,   406,   407,     0,   408,   409,   410,   411,
     412,     0,   413,   414,   415,   416,   417,   418,   419,     0,
       0,   420,   421,   422,     0,     0,     0,     0,   394,   397,
       0,     0,   423,   398,     0,   395,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1206,     0,     0,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,   394,   397,   420,   421,   422,   398,
       0,   395,     0,     0,     0,     0,     0,   423,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     394,   397,   420,   421,   422,   398,     0,   395,     0,     0,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,   394,   397,   420,   421,
     422,   398,     0,   395,     0,     0,     0,  1226,     0,   423,
    1255,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   394,   397,   420,   421,   422,   398,     0,   395,
       0,     0,     0,     0,     0,   423,  1256,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,   394,   397,
     420,   421,   422,   398,     0,   395,     0,     0,     0,     0,
       0,   423,  1257,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,   394,   397,   420,   421,   422,   398,
       0,   395,     0,     0,     0,     0,     0,   423,  1258,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
     394,   397,   420,   421,   422,   398,     0,   395,     0,     0,
       0,     0,     0,   423,  1259,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,   412,     0,   413,
     414,   415,   416,   417,   418,   419,   394,   397,   420,   421,
     422,   398,     0,   395,     0,     0,     0,     0,     0,   423,
    1260,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,   401,
       0,   402,   403,   404,     0,   405,   406,   407,     0,   408,
     409,   410,   411,   412,     0,   413,   414,   415,   416,   417,
     418,   419,   394,   397,   420,   421,   422,   398,     0,   395,
       0,     0,     0,     0,     0,   423,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,   401,     0,   402,   403,   404,
       0,   405,   406,   407,     0,   408,   409,   410,   411,   412,
       0,   413,   414,   415,   416,   417,   418,   419,   394,   397,
     420,   421,   422,   398,     0,   395,     0,     0,     0,  1273,
       0,   423,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,   401,     0,   402,   403,   404,     0,   405,   406,   407,
       0,   408,   409,   410,   411,   412,     0,   413,   414,   415,
     416,   417,   418,   419,     0,   921,   420,   421,   422,   398,
       0,     0,     0,     0,     0,     0,     0,   423,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,   401,     0,   402,
     403,   404,     0,   405,   406,   407,     0,   408,   409,   410,
     411,   412,     0,   413,   414,   415,   416,   417,   418,   419,
       0,   397,   420,   421,   422,   398,     0,     0,     0,     0,
       0,     0,     0,   423,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,   401,     0,   402,   403,   404,     0,   405,
     406,   407,     0,   408,   409,   410,   411,     0,     0,   413,
     414,   415,   416,   417,   418,   419,     0,     0,   420,   421,
     422,   364,     0,   229,     0,     0,     0,     0,     0,   423,
       0,     0,     0,     0,     0,     0,     0,   230,   231,     0,
     232,     0,     0,     0,     0,   233,     0,     0,     0,     0,
       0,   365,     0,     0,   234,     0,     0,     0,     0,     0,
     235,     0,     0,     0,     0,     0,   236,     0,     0,     0,
     237,     0,     0,   238,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   239,     0,     0,     0,     0,     0,     0,
     240,   241,     0,     0,     0,     0,     0,     0,   242,     0,
       0,     0,     0,     0,     0,     0,     0,   243,     0,     0,
     229,     0,     0,     0,     0,     0,   244,   245,     0,   246,
       0,   247,     0,   248,   230,   231,   249,   232,     0,     0,
     250,     0,   233,   251,    23,     0,   252,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,   237,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,     0,     0,     0,     0,     0,   240,   241,     0,
       0,     0,     0,     0,     0,   242,    60,     0,     0,     0,
       0,     0,     0,     0,   243,     0,     0,    68,     0,     0,
       0,     0,     0,   244,   245,     0,   246,     0,   247,     0,
     248,   229,     0,   249,     0,     0,     0,   250,     0,     0,
     251,     0,    88,   252,     0,   230,   231,     0,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,   365,
       0,     0,   234,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,     0,   237,     0,
       0,   238,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   239,     0,     0,     0,     0,     0,     0,   240,   241,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,     0,     0,     0,   243,     0,     0,  -298,     0,
       0,     0,     0,     0,   244,   245,     0,   246,     0,   247,
       0,   248,  -298,  -298,   249,  -298,     0,     0,   250,     0,
    -298,   251,     0,     0,   252,     0,     0,     0,     0,  -298,
       0,     0,     0,     0,     0,  -298,     0,     0,     0,     0,
       0,  -298,     0,     0,     0,  -298,     0,     0,  -298,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -298,     0,
       0,     0,     0,     0,     0,  -298,  -298,     0,     0,     0,
       0,     0,     0,  -298,     0,     0,     0,     0,     0,     0,
       0,     0,  -298,     0,     0,   229,     0,     0,     0,     0,
       0,  -298,  -298,     0,  -298,     0,  -298,     0,  -298,   230,
     231,  -298,   232,     0,     0,  -298,     0,   233,  -298,     0,
       0,  -298,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,   235,     0,     0,     0,     0,     0,   236,     0,
       0,     0,   237,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   239,     0,     0,     0,     0,
       0,     0,   240,   241,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,   243,
       0,     0,  -299,     0,     0,     0,     0,     0,   244,   245,
       0,   246,     0,   247,     0,   248,  -299,  -299,   249,  -299,
       0,     0,   250,     0,  -299,   251,     0,     0,   252,     0,
       0,     0,     0,  -299,     0,     0,     0,     0,     0,  -299,
       0,     0,     0,     0,     0,  -299,     0,     0,     0,  -299,
       0,     0,  -299,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -299,     0,     0,     0,     0,     0,     0,  -299,
    -299,     0,     0,     0,     0,     0,     0,  -299,     0,     0,
       0,     0,     0,     0,     0,     0,  -299,     0,     0,     0,
       0,     0,     0,     0,     0,  -299,  -299,     0,  -299,     0,
    -299,     0,  -299,     0,     0,  -299,     0,     0,     0,  -299,
       0,     0,  -299,     0,     0,  -299
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
    1018,    75,    27,    33,     3,    87,    33,    33,    90,    91,
      22,    93,   102,    95,   271,     1,    98,    99,   146,   101,
      61,   146,    70,   105,   106,   107,   108,    18,    92,   165,
      48,  1193,   267,   146,   120,    56,   146,    56,   295,  1022,
     120,    54,     3,   117,   341,   342,   118,   175,   173,   131,
    1245,   133,    75,   172,    79,   174,   313,    61,   106,    48,
     173,    74,    48,   173,   607,    90,  1049,    48,     0,    48,
       1,    84,    90,   199,   146,    48,    88,   118,  1273,    90,
     170,     3,   120,  1245,   109,   120,  1120,    78,    92,   175,
     172,   146,   146,   120,   117,   175,   146,   171,    69,   146,
     112,   441,   442,  1121,   103,    96,   170,   907,   125,  1004,
     192,   193,   194,  1141,   118,   197,   198,   199,  1013,   662,
     170,   175,   113,   170,   778,    34,    35,    36,   145,   146,
      61,   226,   170,   217,   105,   106,   107,  1120,  1003,   125,
     170,   145,   103,   170,   170,   146,   228,   120,   171,   406,
     385,   172,   999,   172,    37,    32,   173,   146,  1192,   145,
     146,    92,    48,   722,   146,   146,   170,   146,    92,   744,
      92,   165,   173,   146,   105,    48,   146,   171,   146,   165,
     577,   103,   356,   146,   441,   442,   175,   118,    61,   271,
      73,   173,   146,   462,   175,    27,   175,   165,   107,    27,
     140,    27,   175,   173,    90,    47,    56,   140,   148,  1192,
     173,   165,    89,   295,   145,   148,    93,  1112,     3,    92,
     302,   303,    48,  1118,    66,   831,    52,   198,   199,   458,
    1104,   313,  1106,   857,   165,   361,    27,   751,    47,   170,
      47,   165,    56,    85,   523,   118,   170,    79,   505,   146,
      76,    79,   125,    79,   336,    81,   109,    48,    90,    66,
    1060,    52,    90,   140,    90,     8,   920,   349,   145,   351,
     333,   148,   114,   146,   171,   152,    85,   109,    85,   361,
     537,   146,   146,   109,   146,    76,   140,   115,    79,   165,
      81,   145,   165,  1188,   148,   854,   172,   170,   152,    90,
     173,   580,    89,    56,   866,   114,    93,   114,   173,   173,
     392,   173,   394,   395,   396,   397,   398,   399,   109,   401,
     402,   403,   404,   405,    56,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   172,   146,   146,   904,   146,   173,   430,   431,
    1034,  1035,   478,   146,   436,    82,    83,   439,   440,   441,
     442,   443,   444,  1021,    61,    11,   146,   462,  1083,   451,
     173,   173,   165,   173,   456,   146,   125,    56,   460,   146,
    1131,    61,   430,   431,   146,   165,   992,   469,   436,   146,
     361,   439,   440,   907,    56,    92,   478,   146,   480,   481,
     172,   150,   173,   125,   486,   487,   173,   146,   105,   171,
     492,  1137,    92,    32,   171,    56,  1142,   499,   856,   125,
     502,   118,   146,   505,   173,   105,   864,   146,   150,    89,
     879,    56,   171,    93,     2,   517,   879,   875,   118,   521,
     146,   165,   524,   525,   150,   146,   165,     8,   530,  1200,
    1201,  1202,  1203,  1204,  1205,   537,  1137,    25,   755,   756,
      28,  1142,   759,   760,   165,   722,   548,   173,   165,   551,
      89,   553,   146,   170,    93,   557,   558,   559,  1021,   561,
     562,    61,   146,   146,  1027,   165,    56,   744,   397,   398,
     170,   165,   574,   575,   146,    83,    89,  1103,    86,  1105,
      93,   165,   165,   551,   567,   553,   588,   478,   146,    56,
      78,   146,    92,   165,    82,    83,   813,   814,   165,     3,
     170,   140,   172,   140,   171,   105,   145,   165,   147,   148,
     165,   148,   172,   152,   443,   444,    20,   105,   118,    22,
    1083,   160,   110,    26,    27,   165,  1060,   456,   146,   168,
      34,    48,   172,    36,   174,    38,    39,    56,    89,   530,
     125,    44,    93,   172,    61,   174,  1004,   165,    51,   826,
     827,   653,   481,   172,   551,   174,   553,   548,   487,    63,
     551,   100,   553,   492,    56,   165,   151,   558,   153,   154,
     170,   172,   157,   158,    77,    92,    79,   854,  1141,    56,
      83,    51,    89,    86,    54,    88,    93,    90,    89,   172,
      56,   174,    93,   172,   172,   174,   174,    56,   172,   103,
     174,   118,    56,   852,    74,   109,   109,    77,   125,   112,
     296,   297,   115,   170,    84,   717,   718,   719,   720,   721,
     722,   723,   724,   211,   879,    56,   172,   904,   557,   146,
    1193,   735,   736,   145,   165,   164,   156,   783,   165,   109,
     742,   165,   744,   745,    48,   747,    56,   165,   165,   751,
     165,   753,   165,   170,  1112,   165,   173,    61,   172,   165,
     762,   763,   165,  1226,   119,   170,   165,    85,   170,     8,
     165,    66,   172,   171,   742,   172,   172,   932,   172,   781,
     782,   783,  1245,   172,   939,   787,   274,   170,    92,    22,
       8,   279,   125,    26,    27,   125,    90,    90,    31,   148,
      69,   172,   125,    36,   173,    38,    39,   146,    48,   172,
    1273,    44,   125,   125,   118,    35,    35,    66,    51,   787,
     173,   125,   824,   869,  1084,  1085,   828,   125,   830,   831,
     832,   173,   834,   173,   836,    21,     2,   839,   840,   175,
     175,   170,   146,   146,    77,   333,    79,   146,    81,   173,
      83,   742,   854,    86,     2,    88,   172,    90,   105,    25,
     348,   165,    28,   148,   148,   148,   170,   869,   140,   173,
     148,   148,   148,    27,   140,   148,   109,    25,   148,   112,
      28,   148,   115,   140,   148,   148,   140,   140,   148,   718,
     165,   720,   783,   895,    48,   175,   898,   385,    52,   173,
     165,   172,   904,   165,   165,   907,   165,  1084,  1085,   165,
      35,    35,    78,   171,   146,   165,    82,    83,   173,   921,
     922,   145,    76,   171,   170,    79,    33,    81,   173,   975,
      78,   933,   165,   935,    82,    83,    90,   175,   940,   105,
      33,   943,   165,    35,   110,   175,   175,   172,   165,   145,
     172,   165,     8,   955,   175,   109,   140,   105,   140,   175,
     175,   173,   110,    35,   173,   165,   454,   173,   173,   146,
     165,   165,   164,   975,   164,   348,   824,    24,   869,   270,
      90,  1013,   984,   961,   986,   521,   320,   989,   979,   860,
     992,   359,   480,   879,   864,   875,   598,   999,   486,  1001,
    1142,   874,  1157,  1158,   371,  1206,  1161,  1162,  1199,   497,
     851,   499,   131,   664,   518,    -1,   777,    -1,    -1,  1174,
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
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,
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
     100,   101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
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
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,   124,   170,    -1,    -1,    -1,    89,    -1,    -1,   177,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,   165,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,   165,   166,   167,   168,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   102,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    13,   166,   167,   168,    -1,    -1,
      19,    -1,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,   177,    93,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,   120,   166,   167,   168,
     124,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
     167,   168,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
     177,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,    -1,    -1,    19,
      -1,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   123,   124,   166,   167,   168,    -1,
      -1,    89,    -1,   173,    -1,    93,    -1,   177,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,   124,   166,   167,   168,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   177,    19,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    32,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   177,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,   124,   166,   167,   168,    -1,
      -1,    89,    -1,   173,    -1,    93,    -1,   177,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,   124,   166,   167,   168,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   177,    19,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    32,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,   177,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,   124,   166,   167,   168,    -1,
      -1,    12,    -1,    -1,    -1,   175,    -1,   177,    19,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    32,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    89,    -1,
      -1,    -1,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    89,   166,   167,   168,    93,    -1,
      -1,    -1,    12,    13,    -1,    -1,   177,   102,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    12,    89,
      -1,    -1,   177,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,   175,    -1,   177,
     102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
     102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,   175,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    89,   166,   167,   168,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      -1,    89,   166,   167,   168,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,    -1,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    -1,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,
      -1,   101,    -1,   103,    17,    18,   106,    20,    -1,    -1,
     110,    -1,    25,   113,    27,    -1,   116,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    90,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,     3,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    17,    18,   106,    20,    -1,    -1,   110,    -1,
      25,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,    17,
      18,   106,    20,    -1,    -1,   110,    -1,    25,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    17,    18,   106,    20,
      -1,    -1,   110,    -1,    25,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116
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
     198,   347,   236,   347,   347,   347,   347,   347,   347,   347,
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
#line 601 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6489 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 605 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6495 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 606 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6501 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 613 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6509 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 621 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6517 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 625 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6526 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 633 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6532 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: include_module_stmt  */
#line 634 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6538 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: block_stmt  */
#line 635 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6544 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: use_stmt  */
#line 636 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6550 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: import_stmt  */
#line 637 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6556 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: require_stmt  */
#line 638 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6562 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 639 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6568 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: implements_stmt  */
#line 640 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6574 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: interface_stmt  */
#line 641 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6580 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: TDEFER stmt  */
#line 643 "chpl.ypp"
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
#line 6597 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: try_stmt  */
#line 655 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6603 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: return_stmt  */
#line 656 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6609 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 658 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6622 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 667 "chpl.ypp"
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

  case 24: /* stmt: TLABEL ident_def stmt  */
#line 676 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6643 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TYIELD expr TSEMI  */
#line 680 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6654 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: error TSEMI  */
#line 687 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclState();
  }
#line 6663 "bison-chpl-lib.cpp"
    break;

  case 27: /* tryable_stmt: assignment_stmt  */
#line 694 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6669 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: if_stmt  */
#line 695 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6675 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: loop_stmt  */
#line 696 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6681 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: select_stmt  */
#line 697 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6687 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: manage_stmt  */
#line 698 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6693 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 699 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6699 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: throw_stmt  */
#line 700 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6705 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
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
#line 6723 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 716 "chpl.ypp"
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
#line 6742 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 731 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6754 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 739 "chpl.ypp"
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
#line 6770 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL do_stmt  */
#line 751 "chpl.ypp"
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
#line 6785 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TON expr do_stmt  */
#line 762 "chpl.ypp"
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
#line 6801 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 774 "chpl.ypp"
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
#line 6817 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL do_stmt  */
#line 786 "chpl.ypp"
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
#line 6832 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSYNC stmt  */
#line 797 "chpl.ypp"
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
#line 6849 "bison-chpl-lib.cpp"
    break;

  case 44: /* $@1: %empty  */
#line 814 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6857 "bison-chpl-lib.cpp"
    break;

  case 45: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 deprecated_decl_base  */
#line 818 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6865 "bison-chpl-lib.cpp"
    break;

  case 46: /* $@2: %empty  */
#line 822 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6873 "bison-chpl-lib.cpp"
    break;

  case 47: /* deprecated_decl_stmt: TDEPRECATED $@2 deprecated_decl_base  */
#line 826 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6881 "bison-chpl-lib.cpp"
    break;

  case 50: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 838 "chpl.ypp"
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
#line 6902 "bison-chpl-lib.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 858 "chpl.ypp"
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
#line 6921 "bison-chpl-lib.cpp"
    break;

  case 52: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 873 "chpl.ypp"
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
#line 6940 "bison-chpl-lib.cpp"
    break;

  case 53: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 888 "chpl.ypp"
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
#line 6957 "bison-chpl-lib.cpp"
    break;

  case 54: /* opt_access_control: %empty  */
#line 903 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6963 "bison-chpl-lib.cpp"
    break;

  case 55: /* opt_access_control: TPUBLIC  */
#line 904 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6970 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_access_control: TPRIVATE  */
#line 906 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6977 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_prototype: %empty  */
#line 911 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6983 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_prototype: TPROTOTYPE  */
#line 912 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6990 "bison-chpl-lib.cpp"
    break;

  case 59: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 918 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7000 "bison-chpl-lib.cpp"
    break;

  case 60: /* block_stmt_body: TLCBR TRCBR  */
#line 936 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7008 "bison-chpl-lib.cpp"
    break;

  case 61: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 940 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7016 "bison-chpl-lib.cpp"
    break;

  case 62: /* block_stmt_body: TLCBR error TRCBR  */
#line 944 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7028 "bison-chpl-lib.cpp"
    break;

  case 63: /* block_stmt: block_stmt_body  */
#line 955 "chpl.ypp"
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
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 64: /* stmt_ls: toplevel_stmt  */
#line 973 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7053 "bison-chpl-lib.cpp"
    break;

  case 65: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 974 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7059 "bison-chpl-lib.cpp"
    break;

  case 66: /* renames_ls: expr  */
#line 979 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7067 "bison-chpl-lib.cpp"
    break;

  case 67: /* renames_ls: all_op_name  */
#line 983 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7075 "bison-chpl-lib.cpp"
    break;

  case 68: /* renames_ls: expr TAS expr  */
#line 987 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7084 "bison-chpl-lib.cpp"
    break;

  case 69: /* renames_ls: renames_ls TCOMMA expr  */
#line 992 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7092 "bison-chpl-lib.cpp"
    break;

  case 70: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 996 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7100 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1000 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 72: /* use_renames_ls: expr  */
#line 1010 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7118 "bison-chpl-lib.cpp"
    break;

  case 73: /* use_renames_ls: expr TAS expr  */
#line 1015 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7128 "bison-chpl-lib.cpp"
    break;

  case 74: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1021 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7139 "bison-chpl-lib.cpp"
    break;

  case 75: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1028 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7148 "bison-chpl-lib.cpp"
    break;

  case 76: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1033 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7159 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1040 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7171 "bison-chpl-lib.cpp"
    break;

  case 78: /* opt_only_ls: %empty  */
#line 1050 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7177 "bison-chpl-lib.cpp"
    break;

  case 79: /* opt_only_ls: renames_ls  */
#line 1051 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7183 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1056 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1060 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7202 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1067 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7214 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1075 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7227 "bison-chpl-lib.cpp"
    break;

  case 84: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1084 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7238 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1091 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7250 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1099 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7263 "bison-chpl-lib.cpp"
    break;

  case 87: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1111 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7271 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_expr: expr  */
#line 1118 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7279 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_expr: expr TDOT all_op_name  */
#line 1122 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7288 "bison-chpl-lib.cpp"
    break;

  case 90: /* import_expr: expr TAS ident_use  */
#line 1127 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7298 "bison-chpl-lib.cpp"
    break;

  case 91: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1133 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7308 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_ls: import_expr  */
#line 1141 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7314 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_ls: import_ls TCOMMA import_expr  */
#line 1142 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7320 "bison-chpl-lib.cpp"
    break;

  case 94: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1147 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7330 "bison-chpl-lib.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1156 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7338 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1160 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7346 "bison-chpl-lib.cpp"
    break;

  case 97: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1164 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7354 "bison-chpl-lib.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1168 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7362 "bison-chpl-lib.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1172 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7370 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1176 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7378 "bison-chpl-lib.cpp"
    break;

  case 101: /* opt_label_ident: %empty  */
#line 1184 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7384 "bison-chpl-lib.cpp"
    break;

  case 102: /* opt_label_ident: TIDENT  */
#line 1185 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7390 "bison-chpl-lib.cpp"
    break;

  case 103: /* ident_fn_def: TIDENT  */
#line 1189 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7396 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: TNONE  */
#line 1190 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'none'"); }
#line 7402 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_fn_def: TTHIS  */
#line 1191 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7408 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_fn_def: TFALSE  */
#line 1192 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'false'"); }
#line 7414 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_fn_def: TTRUE  */
#line 1193 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'true'"); }
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_fn_def: internal_type_ident_def  */
#line 1194 "chpl.ypp"
                           { std::string msg = "attempt to redefine reserved type '";
                             msg += (yyvsp[0].uniqueStr).c_str();
                             msg += "'";
                             (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), msg); }
#line 7429 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_def: TIDENT  */
#line 1200 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7435 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: TNONE  */
#line 1201 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'none'"); }
#line 7441 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_def: TTHIS  */
#line 1202 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'this'"); }
#line 7447 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_def: TFALSE  */
#line 1203 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'false'"); }
#line 7453 "bison-chpl-lib.cpp"
    break;

  case 113: /* ident_def: TTRUE  */
#line 1204 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'true'"); }
#line 7459 "bison-chpl-lib.cpp"
    break;

  case 114: /* ident_def: internal_type_ident_def  */
#line 1205 "chpl.ypp"
                           { std::string msg = "attempt to redefine reserved type '";
                             msg += (yyvsp[0].uniqueStr).c_str();
                             msg += "'";
                             (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), msg); }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_use: TIDENT  */
#line 1220 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7474 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_use: TTHIS  */
#line 1221 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7480 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TBOOL  */
#line 1254 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7486 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TENUM  */
#line 1255 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7492 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TINT  */
#line 1256 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7498 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TUINT  */
#line 1257 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7504 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TREAL  */
#line 1258 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TIMAG  */
#line 1259 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7516 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TCOMPLEX  */
#line 1260 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7522 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TBYTES  */
#line 1261 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7528 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TSTRING  */
#line 1262 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TLOCALE  */
#line 1263 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7540 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TNOTHING  */
#line 1264 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TVOID  */
#line 1265 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7552 "bison-chpl-lib.cpp"
    break;

  case 152: /* do_stmt: TDO stmt  */
#line 1279 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 153: /* do_stmt: block_stmt  */
#line 1280 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7564 "bison-chpl-lib.cpp"
    break;

  case 154: /* return_stmt: TRETURN TSEMI  */
#line 1285 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7575 "bison-chpl-lib.cpp"
    break;

  case 155: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1292 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@3: %empty  */
#line 1303 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7594 "bison-chpl-lib.cpp"
    break;

  case 158: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@3 class_level_stmt  */
#line 1307 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7602 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@4: %empty  */
#line 1311 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 160: /* deprecated_class_level_stmt: TDEPRECATED $@4 class_level_stmt  */
#line 1315 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7618 "bison-chpl-lib.cpp"
    break;

  case 161: /* class_level_stmt: TSEMI  */
#line 1322 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7626 "bison-chpl-lib.cpp"
    break;

  case 162: /* class_level_stmt: inner_class_level_stmt  */
#line 1326 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7636 "bison-chpl-lib.cpp"
    break;

  case 163: /* $@5: %empty  */
#line 1331 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7643 "bison-chpl-lib.cpp"
    break;

  case 164: /* class_level_stmt: TPUBLIC $@5 inner_class_level_stmt  */
#line 1333 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7652 "bison-chpl-lib.cpp"
    break;

  case 165: /* $@6: %empty  */
#line 1337 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7659 "bison-chpl-lib.cpp"
    break;

  case 166: /* class_level_stmt: TPRIVATE $@6 inner_class_level_stmt  */
#line 1339 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7668 "bison-chpl-lib.cpp"
    break;

  case 174: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1357 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7677 "bison-chpl-lib.cpp"
    break;

  case 175: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1362 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7686 "bison-chpl-lib.cpp"
    break;

  case 176: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1367 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7695 "bison-chpl-lib.cpp"
    break;

  case 177: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1372 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7703 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_start: TFORWARDING  */
#line 1379 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7712 "bison-chpl-lib.cpp"
    break;

  case 179: /* extern_or_export: TEXTERN  */
#line 1386 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7718 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_or_export: TEXPORT  */
#line 1387 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7724 "bison-chpl-lib.cpp"
    break;

  case 181: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1392 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7734 "bison-chpl-lib.cpp"
    break;

  case 182: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1402 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7743 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1408 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7754 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1415 "chpl.ypp"
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
#line 7776 "bison-chpl-lib.cpp"
    break;

  case 185: /* $@7: %empty  */
#line 1433 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7784 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@7 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1437 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7793 "bison-chpl-lib.cpp"
    break;

  case 187: /* $@8: %empty  */
#line 1442 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7802 "bison-chpl-lib.cpp"
    break;

  case 188: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@8 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1447 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7811 "bison-chpl-lib.cpp"
    break;

  case 189: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1455 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7819 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1462 "chpl.ypp"
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
#line 7841 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TWHILE expr do_stmt  */
#line 1480 "chpl.ypp"
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
#line 7858 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1493 "chpl.ypp"
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
#line 7875 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1506 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7883 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1510 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7891 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1514 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7899 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1518 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7907 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1522 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7915 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFOR expr do_stmt  */
#line 1526 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7923 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1530 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7931 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1534 "chpl.ypp"
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
#line 7953 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1552 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7961 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1556 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7969 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1560 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7977 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1564 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7985 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr do_stmt  */
#line 1568 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7993 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1572 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8001 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1576 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8009 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1580 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8017 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1584 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8025 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1588 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8033 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1592 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8041 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1596 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8049 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1600 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8057 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1604 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8065 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1608 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8073 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1612 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1616 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8089 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1620 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8097 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1624 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8105 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1628 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8113 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1632 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8121 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1636 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8129 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1640 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8138 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1645 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8147 "bison-chpl-lib.cpp"
    break;

  case 225: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1653 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8157 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF expr TTHEN stmt  */
#line 1662 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8165 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF expr block_stmt  */
#line 1666 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8173 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1670 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8181 "bison-chpl-lib.cpp"
    break;

  case 229: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8189 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1678 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8197 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF ifvar block_stmt  */
#line 1682 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8205 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1686 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1690 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8221 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1694 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8230 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1699 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8239 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1704 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8248 "bison-chpl-lib.cpp"
    break;

  case 237: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1709 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8257 "bison-chpl-lib.cpp"
    break;

  case 238: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1717 "chpl.ypp"
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
#line 8276 "bison-chpl-lib.cpp"
    break;

  case 239: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1732 "chpl.ypp"
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
#line 8295 "bison-chpl-lib.cpp"
    break;

  case 240: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1750 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8303 "bison-chpl-lib.cpp"
    break;

  case 241: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1754 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8311 "bison-chpl-lib.cpp"
    break;

  case 242: /* ifc_formal_ls: ifc_formal  */
#line 1760 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8317 "bison-chpl-lib.cpp"
    break;

  case 243: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1761 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8323 "bison-chpl-lib.cpp"
    break;

  case 244: /* ifc_formal: ident_def  */
#line 1766 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8329 "bison-chpl-lib.cpp"
    break;

  case 257: /* implements_type_ident: implements_type_error_ident  */
#line 1784 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8341 "bison-chpl-lib.cpp"
    break;

  case 264: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1813 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8349 "bison-chpl-lib.cpp"
    break;

  case 265: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1817 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8357 "bison-chpl-lib.cpp"
    break;

  case 266: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1821 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8365 "bison-chpl-lib.cpp"
    break;

  case 267: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1828 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8373 "bison-chpl-lib.cpp"
    break;

  case 268: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1832 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8381 "bison-chpl-lib.cpp"
    break;

  case 269: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1836 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8389 "bison-chpl-lib.cpp"
    break;

  case 270: /* try_stmt: TTRY tryable_stmt  */
#line 1843 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8397 "bison-chpl-lib.cpp"
    break;

  case 271: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1847 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8405 "bison-chpl-lib.cpp"
    break;

  case 272: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1851 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8413 "bison-chpl-lib.cpp"
    break;

  case 273: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1855 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8421 "bison-chpl-lib.cpp"
    break;

  case 274: /* catch_expr_ls: %empty  */
#line 1861 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8427 "bison-chpl-lib.cpp"
    break;

  case 275: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1862 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8433 "bison-chpl-lib.cpp"
    break;

  case 276: /* catch_expr: TCATCH block_stmt  */
#line 1867 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8441 "bison-chpl-lib.cpp"
    break;

  case 277: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1871 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8449 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1875 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8457 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr_inner: ident_def  */
#line 1882 "chpl.ypp"
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
#line 8474 "bison-chpl-lib.cpp"
    break;

  case 280: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1895 "chpl.ypp"
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
#line 8491 "bison-chpl-lib.cpp"
    break;

  case 281: /* throw_stmt: TTHROW expr TSEMI  */
#line 1911 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8501 "bison-chpl-lib.cpp"
    break;

  case 282: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1919 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8509 "bison-chpl-lib.cpp"
    break;

  case 283: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1923 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8519 "bison-chpl-lib.cpp"
    break;

  case 284: /* when_stmt_ls: %empty  */
#line 1931 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8525 "bison-chpl-lib.cpp"
    break;

  case 285: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1932 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8531 "bison-chpl-lib.cpp"
    break;

  case 286: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1937 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8539 "bison-chpl-lib.cpp"
    break;

  case 287: /* when_stmt: TOTHERWISE stmt  */
#line 1941 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8548 "bison-chpl-lib.cpp"
    break;

  case 288: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1946 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8557 "bison-chpl-lib.cpp"
    break;

  case 289: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 1954 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8566 "bison-chpl-lib.cpp"
    break;

  case 290: /* manager_expr: expr TAS ident_def  */
#line 1959 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8574 "bison-chpl-lib.cpp"
    break;

  case 291: /* manager_expr: expr  */
#line 1963 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8582 "bison-chpl-lib.cpp"
    break;

  case 292: /* manager_expr_ls: manager_expr  */
#line 1969 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8588 "bison-chpl-lib.cpp"
    break;

  case 293: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 1970 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8594 "bison-chpl-lib.cpp"
    break;

  case 294: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 1975 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 295: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1984 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8611 "bison-chpl-lib.cpp"
    break;

  case 296: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1989 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8622 "bison-chpl-lib.cpp"
    break;

  case 297: /* class_start: class_tag ident_def  */
#line 2000 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8630 "bison-chpl-lib.cpp"
    break;

  case 298: /* class_tag: TCLASS  */
#line 2006 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8636 "bison-chpl-lib.cpp"
    break;

  case 299: /* class_tag: TRECORD  */
#line 2007 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8642 "bison-chpl-lib.cpp"
    break;

  case 300: /* class_tag: TUNION  */
#line 2008 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8648 "bison-chpl-lib.cpp"
    break;

  case 301: /* opt_inherit: %empty  */
#line 2012 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8654 "bison-chpl-lib.cpp"
    break;

  case 302: /* opt_inherit: TCOLON expr_ls  */
#line 2013 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8660 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_level_stmt_ls: %empty  */
#line 2017 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8669 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2022 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8677 "bison-chpl-lib.cpp"
    break;

  case 305: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2026 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8685 "bison-chpl-lib.cpp"
    break;

  case 306: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
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
#line 8706 "bison-chpl-lib.cpp"
    break;

  case 307: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
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
#line 8720 "bison-chpl-lib.cpp"
    break;

  case 308: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2063 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 309: /* enum_ls: deprecated_enum_item  */
#line 2070 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8737 "bison-chpl-lib.cpp"
    break;

  case 310: /* enum_ls: enum_ls TCOMMA  */
#line 2075 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8747 "bison-chpl-lib.cpp"
    break;

  case 311: /* $@9: %empty  */
#line 2081 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8756 "bison-chpl-lib.cpp"
    break;

  case 312: /* enum_ls: enum_ls TCOMMA $@9 deprecated_enum_item  */
#line 2086 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8765 "bison-chpl-lib.cpp"
    break;

  case 314: /* $@10: %empty  */
#line 2095 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8773 "bison-chpl-lib.cpp"
    break;

  case 315: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@10 enum_item  */
#line 2099 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8781 "bison-chpl-lib.cpp"
    break;

  case 316: /* $@11: %empty  */
#line 2103 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8789 "bison-chpl-lib.cpp"
    break;

  case 317: /* deprecated_enum_item: TDEPRECATED $@11 enum_item  */
#line 2107 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8797 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_item: ident_def  */
#line 2114 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8808 "bison-chpl-lib.cpp"
    break;

  case 319: /* enum_item: ident_def TASSIGN expr  */
#line 2121 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8821 "bison-chpl-lib.cpp"
    break;

  case 320: /* lambda_decl_start: TLAMBDA  */
#line 2133 "chpl.ypp"
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
#line 8837 "bison-chpl-lib.cpp"
    break;

  case 321: /* $@12: %empty  */
#line 2149 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8847 "bison-chpl-lib.cpp"
    break;

  case 322: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_lifetime_where $@12 function_body_stmt  */
#line 2155 "chpl.ypp"
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
#line 8869 "bison-chpl-lib.cpp"
    break;

  case 324: /* linkage_spec: linkage_spec_empty  */
#line 2178 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8875 "bison-chpl-lib.cpp"
    break;

  case 325: /* linkage_spec: TINLINE  */
#line 2179 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8882 "bison-chpl-lib.cpp"
    break;

  case 326: /* linkage_spec: TOVERRIDE  */
#line 2181 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8889 "bison-chpl-lib.cpp"
    break;

  case 327: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2187 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8897 "bison-chpl-lib.cpp"
    break;

  case 328: /* $@13: %empty  */
#line 2196 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
#line 8907 "bison-chpl-lib.cpp"
    break;

  case 329: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@13 opt_function_body_stmt  */
#line 2202 "chpl.ypp"
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
      context->exitScope(asttags::Function, (yyvsp[-6].functionParts).name->name());

      (yyval.functionParts) = fp;
    }
#line 8928 "bison-chpl-lib.cpp"
    break;

  case 330: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2222 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8940 "bison-chpl-lib.cpp"
    break;

  case 331: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2230 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8952 "bison-chpl-lib.cpp"
    break;

  case 332: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2238 "chpl.ypp"
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
#line 8967 "bison-chpl-lib.cpp"
    break;

  case 333: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2249 "chpl.ypp"
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
#line 8982 "bison-chpl-lib.cpp"
    break;

  case 334: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2260 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8992 "bison-chpl-lib.cpp"
    break;

  case 335: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2269 "chpl.ypp"
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
#line 9008 "bison-chpl-lib.cpp"
    break;

  case 337: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2284 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9014 "bison-chpl-lib.cpp"
    break;

  case 340: /* fn_ident: ident_def TBANG  */
#line 2291 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9024 "bison-chpl-lib.cpp"
    break;

  case 381: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2349 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 382: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2350 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9036 "bison-chpl-lib.cpp"
    break;

  case 383: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2351 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9042 "bison-chpl-lib.cpp"
    break;

  case 384: /* opt_formal_ls: %empty  */
#line 2355 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9048 "bison-chpl-lib.cpp"
    break;

  case 385: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2356 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9054 "bison-chpl-lib.cpp"
    break;

  case 386: /* req_formal_ls: TLP formal_ls TRP  */
#line 2360 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9060 "bison-chpl-lib.cpp"
    break;

  case 387: /* formal_ls_inner: formal  */
#line 2364 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9066 "bison-chpl-lib.cpp"
    break;

  case 388: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2365 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 389: /* formal_ls: %empty  */
#line 2369 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 390: /* formal_ls: formal_ls_inner  */
#line 2370 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 391: /* formal: opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2375 "chpl.ypp"
  {
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9097 "bison-chpl-lib.cpp"
    break;

  case 392: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2384 "chpl.ypp"
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
#line 9112 "bison-chpl-lib.cpp"
    break;

  case 393: /* formal: opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2395 "chpl.ypp"
  {
    (yyval.expr) = VarArgFormal::build(BUILDER, LOC((yyloc)),
                             /*attributes*/ nullptr,
                             /*name*/ (yyvsp[-2].uniqueStr),
                             /*intent*/ (yyvsp[-3].intentTag),
                             toOwned((yyvsp[-1].expr)),
                             toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 394: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2405 "chpl.ypp"
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
#line 9142 "bison-chpl-lib.cpp"
    break;

  case 395: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2418 "chpl.ypp"
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
#line 9157 "bison-chpl-lib.cpp"
    break;

  case 396: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type formal_var_arg_expr  */
#line 2430 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
  }
#line 9165 "bison-chpl-lib.cpp"
    break;

  case 397: /* opt_formal_intent_tag: %empty  */
#line 2436 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9174 "bison-chpl-lib.cpp"
    break;

  case 398: /* opt_formal_intent_tag: required_intent_tag  */
#line 2441 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9183 "bison-chpl-lib.cpp"
    break;

  case 399: /* required_intent_tag: TIN  */
#line 2448 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9189 "bison-chpl-lib.cpp"
    break;

  case 400: /* required_intent_tag: TINOUT  */
#line 2449 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9195 "bison-chpl-lib.cpp"
    break;

  case 401: /* required_intent_tag: TOUT  */
#line 2450 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9201 "bison-chpl-lib.cpp"
    break;

  case 402: /* required_intent_tag: TCONST TIN  */
#line 2451 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9207 "bison-chpl-lib.cpp"
    break;

  case 403: /* required_intent_tag: TCONST TREF  */
#line 2452 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9213 "bison-chpl-lib.cpp"
    break;

  case 404: /* required_intent_tag: TCONST  */
#line 2453 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9219 "bison-chpl-lib.cpp"
    break;

  case 405: /* required_intent_tag: TPARAM  */
#line 2454 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9225 "bison-chpl-lib.cpp"
    break;

  case 406: /* required_intent_tag: TREF  */
#line 2455 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9231 "bison-chpl-lib.cpp"
    break;

  case 407: /* required_intent_tag: TTYPE  */
#line 2456 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9237 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_this_intent_tag: %empty  */
#line 2460 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9243 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_this_intent_tag: TPARAM  */
#line 2461 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9249 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_this_intent_tag: TREF  */
#line 2462 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9255 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_this_intent_tag: TCONST TREF  */
#line 2463 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9261 "bison-chpl-lib.cpp"
    break;

  case 412: /* opt_this_intent_tag: TCONST  */
#line 2464 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9267 "bison-chpl-lib.cpp"
    break;

  case 413: /* opt_this_intent_tag: TTYPE  */
#line 2465 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9273 "bison-chpl-lib.cpp"
    break;

  case 414: /* proc_iter_or_op: TPROC  */
#line 2469 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9279 "bison-chpl-lib.cpp"
    break;

  case 415: /* proc_iter_or_op: TITER  */
#line 2470 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9285 "bison-chpl-lib.cpp"
    break;

  case 416: /* proc_iter_or_op: TOPERATOR  */
#line 2471 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9291 "bison-chpl-lib.cpp"
    break;

  case 417: /* opt_ret_tag: %empty  */
#line 2475 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9297 "bison-chpl-lib.cpp"
    break;

  case 418: /* opt_ret_tag: TCONST  */
#line 2476 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 9303 "bison-chpl-lib.cpp"
    break;

  case 419: /* opt_ret_tag: TCONST TREF  */
#line 2477 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 9309 "bison-chpl-lib.cpp"
    break;

  case 420: /* opt_ret_tag: TREF  */
#line 2478 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 9315 "bison-chpl-lib.cpp"
    break;

  case 421: /* opt_ret_tag: TPARAM  */
#line 2479 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9321 "bison-chpl-lib.cpp"
    break;

  case 422: /* opt_ret_tag: TTYPE  */
#line 2480 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9327 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_throws_error: %empty  */
#line 2484 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9333 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_throws_error: TTHROWS  */
#line 2485 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9339 "bison-chpl-lib.cpp"
    break;

  case 425: /* opt_function_body_stmt: TSEMI  */
#line 2488 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9345 "bison-chpl-lib.cpp"
    break;

  case 426: /* opt_function_body_stmt: function_body_stmt  */
#line 2489 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9351 "bison-chpl-lib.cpp"
    break;

  case 427: /* function_body_stmt: block_stmt_body  */
#line 2493 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9357 "bison-chpl-lib.cpp"
    break;

  case 428: /* function_body_stmt: return_stmt  */
#line 2494 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9363 "bison-chpl-lib.cpp"
    break;

  case 429: /* query_expr: TQUERIEDIDENT  */
#line 2498 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9369 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_lifetime_where: %empty  */
#line 2503 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9375 "bison-chpl-lib.cpp"
    break;

  case 431: /* opt_lifetime_where: TWHERE expr  */
#line 2505 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9381 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2507 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9387 "bison-chpl-lib.cpp"
    break;

  case 433: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2509 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9393 "bison-chpl-lib.cpp"
    break;

  case 434: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2511 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9399 "bison-chpl-lib.cpp"
    break;

  case 435: /* lifetime_components_expr: lifetime_expr  */
#line 2516 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9405 "bison-chpl-lib.cpp"
    break;

  case 436: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2518 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9411 "bison-chpl-lib.cpp"
    break;

  case 437: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2523 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9417 "bison-chpl-lib.cpp"
    break;

  case 438: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2525 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9423 "bison-chpl-lib.cpp"
    break;

  case 439: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2527 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9429 "bison-chpl-lib.cpp"
    break;

  case 440: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2529 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9435 "bison-chpl-lib.cpp"
    break;

  case 441: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2531 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9441 "bison-chpl-lib.cpp"
    break;

  case 442: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2533 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 443: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2535 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9453 "bison-chpl-lib.cpp"
    break;

  case 444: /* lifetime_ident: TIDENT  */
#line 2539 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9459 "bison-chpl-lib.cpp"
    break;

  case 445: /* lifetime_ident: TTHIS  */
#line 2540 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9465 "bison-chpl-lib.cpp"
    break;

  case 446: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2545 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9474 "bison-chpl-lib.cpp"
    break;

  case 447: /* type_alias_decl_stmt_start: TTYPE  */
#line 2554 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9482 "bison-chpl-lib.cpp"
    break;

  case 448: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2558 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9491 "bison-chpl-lib.cpp"
    break;

  case 449: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2566 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9499 "bison-chpl-lib.cpp"
    break;

  case 450: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2570 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9507 "bison-chpl-lib.cpp"
    break;

  case 451: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2577 "chpl.ypp"
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
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_init_type: %empty  */
#line 2597 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_init_type: TASSIGN type_level_expr  */
#line 2599 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 454: /* opt_init_type: TASSIGN array_type  */
#line 2601 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9550 "bison-chpl-lib.cpp"
    break;

  case 455: /* var_decl_type: TPARAM  */
#line 2608 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9556 "bison-chpl-lib.cpp"
    break;

  case 456: /* var_decl_type: TCONST TREF  */
#line 2609 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9562 "bison-chpl-lib.cpp"
    break;

  case 457: /* var_decl_type: TREF  */
#line 2610 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9568 "bison-chpl-lib.cpp"
    break;

  case 458: /* var_decl_type: TCONST  */
#line 2611 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9574 "bison-chpl-lib.cpp"
    break;

  case 459: /* var_decl_type: TVAR  */
#line 2612 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9580 "bison-chpl-lib.cpp"
    break;

  case 460: /* $@14: %empty  */
#line 2617 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9589 "bison-chpl-lib.cpp"
    break;

  case 461: /* var_decl_stmt: TCONFIG $@14 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2621 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9598 "bison-chpl-lib.cpp"
    break;

  case 462: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2626 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9607 "bison-chpl-lib.cpp"
    break;

  case 463: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2634 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9615 "bison-chpl-lib.cpp"
    break;

  case 464: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2638 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9623 "bison-chpl-lib.cpp"
    break;

  case 465: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2645 "chpl.ypp"
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
#line 9643 "bison-chpl-lib.cpp"
    break;

  case 466: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2661 "chpl.ypp"
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
#line 9661 "bison-chpl-lib.cpp"
    break;

  case 467: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2678 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 468: /* tuple_var_decl_component: ident_def  */
#line 2682 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9677 "bison-chpl-lib.cpp"
    break;

  case 469: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2686 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9685 "bison-chpl-lib.cpp"
    break;

  case 470: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2693 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9691 "bison-chpl-lib.cpp"
    break;

  case 471: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2695 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9697 "bison-chpl-lib.cpp"
    break;

  case 472: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2697 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9703 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_init_expr: %empty  */
#line 2703 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9709 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2704 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9715 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2705 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9721 "bison-chpl-lib.cpp"
    break;

  case 476: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2711 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9729 "bison-chpl-lib.cpp"
    break;

  case 477: /* ret_array_type: TLSBR TRSBR  */
#line 2715 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 9737 "bison-chpl-lib.cpp"
    break;

  case 478: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2719 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9745 "bison-chpl-lib.cpp"
    break;

  case 479: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2723 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 9753 "bison-chpl-lib.cpp"
    break;

  case 480: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2727 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 481: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2731 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9769 "bison-chpl-lib.cpp"
    break;

  case 482: /* ret_array_type: TLSBR error TRSBR  */
#line 2735 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "invalid expression for domain of array return type");
  }
#line 9777 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_ret_type: %empty  */
#line 2741 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9783 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_ret_type: TCOLON type_level_expr  */
#line 2742 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9789 "bison-chpl-lib.cpp"
    break;

  case 485: /* opt_ret_type: TCOLON ret_array_type  */
#line 2743 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9795 "bison-chpl-lib.cpp"
    break;

  case 486: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2744 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9801 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_ret_type: error  */
#line 2745 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9807 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_type: %empty  */
#line 2750 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9813 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_type: TCOLON type_level_expr  */
#line 2751 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9819 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_type: TCOLON array_type  */
#line 2752 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9825 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2753 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9831 "bison-chpl-lib.cpp"
    break;

  case 492: /* opt_type: error  */
#line 2754 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9837 "bison-chpl-lib.cpp"
    break;

  case 493: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2775 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9845 "bison-chpl-lib.cpp"
    break;

  case 494: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2779 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9853 "bison-chpl-lib.cpp"
    break;

  case 495: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2783 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9861 "bison-chpl-lib.cpp"
    break;

  case 496: /* array_type: TLSBR error TRSBR  */
#line 2787 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 9869 "bison-chpl-lib.cpp"
    break;

  case 497: /* opt_formal_array_elt_type: %empty  */
#line 2793 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9875 "bison-chpl-lib.cpp"
    break;

  case 498: /* opt_formal_array_elt_type: type_level_expr  */
#line 2794 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9881 "bison-chpl-lib.cpp"
    break;

  case 499: /* opt_formal_array_elt_type: query_expr  */
#line 2795 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9887 "bison-chpl-lib.cpp"
    break;

  case 500: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2800 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9896 "bison-chpl-lib.cpp"
    break;

  case 501: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2805 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9904 "bison-chpl-lib.cpp"
    break;

  case 502: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2813 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9913 "bison-chpl-lib.cpp"
    break;

  case 503: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2818 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9921 "bison-chpl-lib.cpp"
    break;

  case 504: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2822 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9929 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_formal_type: %empty  */
#line 2828 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9935 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_formal_type: TCOLON type_level_expr  */
#line 2829 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9941 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_formal_type: TCOLON query_expr  */
#line 2830 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9947 "bison-chpl-lib.cpp"
    break;

  case 508: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2831 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9953 "bison-chpl-lib.cpp"
    break;

  case 509: /* opt_formal_type: TCOLON formal_array_type  */
#line 2832 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9959 "bison-chpl-lib.cpp"
    break;

  case 510: /* expr_ls: expr  */
#line 2838 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9965 "bison-chpl-lib.cpp"
    break;

  case 511: /* expr_ls: query_expr  */
#line 2839 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9971 "bison-chpl-lib.cpp"
    break;

  case 512: /* expr_ls: expr_ls TCOMMA expr  */
#line 2840 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9977 "bison-chpl-lib.cpp"
    break;

  case 513: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2841 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9983 "bison-chpl-lib.cpp"
    break;

  case 514: /* simple_expr_ls: expr  */
#line 2845 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9989 "bison-chpl-lib.cpp"
    break;

  case 515: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2846 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9995 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_component: TUNDERSCORE  */
#line 2850 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10001 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_component: opt_try_expr  */
#line 2851 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10007 "bison-chpl-lib.cpp"
    break;

  case 518: /* tuple_component: query_expr  */
#line 2852 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10013 "bison-chpl-lib.cpp"
    break;

  case 519: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2857 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10021 "bison-chpl-lib.cpp"
    break;

  case 520: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2861 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10029 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_actual_ls: %empty  */
#line 2867 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10035 "bison-chpl-lib.cpp"
    break;

  case 522: /* opt_actual_ls: actual_ls  */
#line 2868 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10041 "bison-chpl-lib.cpp"
    break;

  case 523: /* actual_ls: actual_expr  */
#line 2873 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 524: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2878 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10060 "bison-chpl-lib.cpp"
    break;

  case 525: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2886 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10066 "bison-chpl-lib.cpp"
    break;

  case 526: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2887 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10072 "bison-chpl-lib.cpp"
    break;

  case 527: /* actual_expr: query_expr  */
#line 2888 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10078 "bison-chpl-lib.cpp"
    break;

  case 528: /* actual_expr: opt_try_expr  */
#line 2889 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10084 "bison-chpl-lib.cpp"
    break;

  case 529: /* ident_expr: ident_use  */
#line 2893 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 530: /* ident_expr: scalar_type  */
#line 2894 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10096 "bison-chpl-lib.cpp"
    break;

  case 531: /* type_level_expr: sub_type_level_expr  */
#line 2906 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 10102 "bison-chpl-lib.cpp"
    break;

  case 532: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2908 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10108 "bison-chpl-lib.cpp"
    break;

  case 533: /* type_level_expr: TQUESTION  */
#line 2910 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10114 "bison-chpl-lib.cpp"
    break;

  case 539: /* sub_type_level_expr: TSINGLE expr  */
#line 2921 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10120 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2923 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10126 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2925 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10132 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2927 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10138 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2929 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10148 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TATOMIC expr  */
#line 2935 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10154 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TSYNC expr  */
#line 2937 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10160 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TOWNED  */
#line 2940 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10166 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TOWNED expr  */
#line 2942 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10172 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TUNMANAGED  */
#line 2944 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10178 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2946 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10184 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TSHARED  */
#line 2948 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10190 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TSHARED expr  */
#line 2950 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10196 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TBORROWED  */
#line 2952 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10202 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TBORROWED expr  */
#line 2954 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10208 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TCLASS  */
#line 2957 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10214 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TRECORD  */
#line 2959 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10220 "bison-chpl-lib.cpp"
    break;

  case 556: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2964 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10233 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2973 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10246 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TFOR expr TDO expr  */
#line 2982 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 2990 "chpl.ypp"
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
#line 10276 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
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
#line 10294 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3018 "chpl.ypp"
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
#line 10312 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3032 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10325 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3041 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10338 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr: TFORALL expr TDO expr  */
#line 3050 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10350 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3058 "chpl.ypp"
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
#line 10368 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3072 "chpl.ypp"
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
#line 10386 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3086 "chpl.ypp"
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
#line 10404 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 3100 "chpl.ypp"
  {
    owned<AstNode> iterand = nullptr;
    auto iterExprs = context->consumeList((yyvsp[-2].exprList));
    if (iterExprs.size() > 1) {
      iterand = Domain::build(BUILDER, LOC((yylsp[-2])), std::move(iterExprs));
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
#line 10425 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3117 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10438 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3126 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10451 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3135 "chpl.ypp"
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
#line 10470 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3150 "chpl.ypp"
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
#line 10489 "bison-chpl-lib.cpp"
    break;

  case 573: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3168 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10503 "bison-chpl-lib.cpp"
    break;

  case 574: /* nil_expr: TNIL  */
#line 3185 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10509 "bison-chpl-lib.cpp"
    break;

  case 582: /* stmt_level_expr: io_expr TIO expr  */
#line 3201 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10515 "bison-chpl-lib.cpp"
    break;

  case 583: /* opt_task_intent_ls: %empty  */
#line 3205 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10521 "bison-chpl-lib.cpp"
    break;

  case 584: /* opt_task_intent_ls: task_intent_clause  */
#line 3206 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10527 "bison-chpl-lib.cpp"
    break;

  case 585: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3211 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10537 "bison-chpl-lib.cpp"
    break;

  case 586: /* task_intent_ls: intent_expr  */
#line 3219 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10543 "bison-chpl-lib.cpp"
    break;

  case 587: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3220 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10549 "bison-chpl-lib.cpp"
    break;

  case 588: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3225 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10559 "bison-chpl-lib.cpp"
    break;

  case 589: /* forall_intent_ls: intent_expr  */
#line 3233 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10565 "bison-chpl-lib.cpp"
    break;

  case 590: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3234 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10571 "bison-chpl-lib.cpp"
    break;

  case 591: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3239 "chpl.ypp"
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
#line 10590 "bison-chpl-lib.cpp"
    break;

  case 592: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3254 "chpl.ypp"
  { auto ident = Identifier::build(BUILDER, LOC((yylsp[-2])), (yyvsp[-2].uniqueStr));
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), std::move(ident), toOwned((yyvsp[0].expr))).release();
  }
#line 10598 "bison-chpl-lib.cpp"
    break;

  case 593: /* intent_expr: expr TREDUCE ident_expr  */
#line 3258 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10606 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_var_prefix: TCONST  */
#line 3264 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10612 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_var_prefix: TIN  */
#line 3265 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10618 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_var_prefix: TCONST TIN  */
#line 3266 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10624 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_var_prefix: TREF  */
#line 3267 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10630 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_var_prefix: TCONST TREF  */
#line 3268 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10636 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_var_prefix: TVAR  */
#line 3269 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10642 "bison-chpl-lib.cpp"
    break;

  case 601: /* io_expr: io_expr TIO expr  */
#line 3275 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10648 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW  */
#line 3280 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10654 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_maybe_decorated: TNEW TOWNED  */
#line 3282 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10660 "bison-chpl-lib.cpp"
    break;

  case 604: /* new_maybe_decorated: TNEW TSHARED  */
#line 3284 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10666 "bison-chpl-lib.cpp"
    break;

  case 605: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3286 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10672 "bison-chpl-lib.cpp"
    break;

  case 606: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3288 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10678 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_expr: new_maybe_decorated expr  */
#line 3294 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10686 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3301 "chpl.ypp"
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
#line 10702 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3313 "chpl.ypp"
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
#line 10718 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3325 "chpl.ypp"
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
#line 10735 "bison-chpl-lib.cpp"
    break;

  case 611: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3338 "chpl.ypp"
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
#line 10753 "bison-chpl-lib.cpp"
    break;

  case 612: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3355 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10761 "bison-chpl-lib.cpp"
    break;

  case 622: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 3374 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10769 "bison-chpl-lib.cpp"
    break;

  case 623: /* expr: expr TCOLON expr  */
#line 3378 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10777 "bison-chpl-lib.cpp"
    break;

  case 624: /* expr: expr TDOTDOT expr  */
#line 3382 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10786 "bison-chpl-lib.cpp"
    break;

  case 625: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 3387 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10795 "bison-chpl-lib.cpp"
    break;

  case 626: /* expr: expr TDOTDOT  */
#line 3392 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10804 "bison-chpl-lib.cpp"
    break;

  case 627: /* expr: TDOTDOT expr  */
#line 3397 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10814 "bison-chpl-lib.cpp"
    break;

  case 628: /* expr: TDOTDOTOPENHIGH expr  */
#line 3403 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10824 "bison-chpl-lib.cpp"
    break;

  case 629: /* expr: TDOTDOT  */
#line 3409 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10834 "bison-chpl-lib.cpp"
    break;

  case 630: /* opt_expr: %empty  */
#line 3440 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10840 "bison-chpl-lib.cpp"
    break;

  case 631: /* opt_expr: expr  */
#line 3441 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10846 "bison-chpl-lib.cpp"
    break;

  case 632: /* opt_try_expr: TTRY expr  */
#line 3445 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10852 "bison-chpl-lib.cpp"
    break;

  case 633: /* opt_try_expr: TTRYBANG expr  */
#line 3446 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10858 "bison-chpl-lib.cpp"
    break;

  case 634: /* opt_try_expr: expr  */
#line 3447 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10864 "bison-chpl-lib.cpp"
    break;

  case 640: /* call_base_expr: expr TBANG  */
#line 3464 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10872 "bison-chpl-lib.cpp"
    break;

  case 641: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3467 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10878 "bison-chpl-lib.cpp"
    break;

  case 644: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3474 "chpl.ypp"
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
#line 10894 "bison-chpl-lib.cpp"
    break;

  case 645: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3486 "chpl.ypp"
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
#line 10910 "bison-chpl-lib.cpp"
    break;

  case 646: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3498 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10918 "bison-chpl-lib.cpp"
    break;

  case 647: /* dot_expr: expr TDOT ident_use  */
#line 3505 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10924 "bison-chpl-lib.cpp"
    break;

  case 648: /* dot_expr: expr TDOT TTYPE  */
#line 3507 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10930 "bison-chpl-lib.cpp"
    break;

  case 649: /* dot_expr: expr TDOT TDOMAIN  */
#line 3509 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10936 "bison-chpl-lib.cpp"
    break;

  case 650: /* dot_expr: expr TDOT TLOCALE  */
#line 3511 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10942 "bison-chpl-lib.cpp"
    break;

  case 651: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3513 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10952 "bison-chpl-lib.cpp"
    break;

  case 652: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3519 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10962 "bison-chpl-lib.cpp"
    break;

  case 653: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3531 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10968 "bison-chpl-lib.cpp"
    break;

  case 654: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3533 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10976 "bison-chpl-lib.cpp"
    break;

  case 655: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3537 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10984 "bison-chpl-lib.cpp"
    break;

  case 656: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3541 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10992 "bison-chpl-lib.cpp"
    break;

  case 657: /* bool_literal: TFALSE  */
#line 3547 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10998 "bison-chpl-lib.cpp"
    break;

  case 658: /* bool_literal: TTRUE  */
#line 3548 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 11004 "bison-chpl-lib.cpp"
    break;

  case 659: /* str_bytes_literal: STRINGLITERAL  */
#line 3552 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11010 "bison-chpl-lib.cpp"
    break;

  case 660: /* str_bytes_literal: BYTESLITERAL  */
#line 3553 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11016 "bison-chpl-lib.cpp"
    break;

  case 663: /* literal_expr: INTLITERAL  */
#line 3559 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 11022 "bison-chpl-lib.cpp"
    break;

  case 664: /* literal_expr: REALLITERAL  */
#line 3560 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11028 "bison-chpl-lib.cpp"
    break;

  case 665: /* literal_expr: IMAGLITERAL  */
#line 3561 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11034 "bison-chpl-lib.cpp"
    break;

  case 666: /* literal_expr: CSTRINGLITERAL  */
#line 3562 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11040 "bison-chpl-lib.cpp"
    break;

  case 667: /* literal_expr: TNONE  */
#line 3563 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11046 "bison-chpl-lib.cpp"
    break;

  case 668: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3565 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11054 "bison-chpl-lib.cpp"
    break;

  case 669: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3569 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11062 "bison-chpl-lib.cpp"
    break;

  case 670: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3573 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11070 "bison-chpl-lib.cpp"
    break;

  case 671: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3577 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11078 "bison-chpl-lib.cpp"
    break;

  case 672: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3581 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11086 "bison-chpl-lib.cpp"
    break;

  case 673: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3585 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11095 "bison-chpl-lib.cpp"
    break;

  case 674: /* assoc_expr_ls: expr TALIAS expr  */
#line 3594 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11104 "bison-chpl-lib.cpp"
    break;

  case 675: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3599 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11114 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr TPLUS expr  */
#line 3607 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11120 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TMINUS expr  */
#line 3608 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11126 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TSTAR expr  */
#line 3609 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11132 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr TDIVIDE expr  */
#line 3610 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11138 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3611 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11144 "bison-chpl-lib.cpp"
    break;

  case 681: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3612 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11150 "bison-chpl-lib.cpp"
    break;

  case 682: /* binary_op_expr: expr TMOD expr  */
#line 3613 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11156 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TEQUAL expr  */
#line 3614 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11162 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3615 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11168 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3616 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11174 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3617 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11180 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TLESS expr  */
#line 3618 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11186 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TGREATER expr  */
#line 3619 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11192 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TBAND expr  */
#line 3620 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11198 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TBOR expr  */
#line 3621 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11204 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TBXOR expr  */
#line 3622 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11210 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TAND expr  */
#line 3623 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11216 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TOR expr  */
#line 3624 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11222 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TEXP expr  */
#line 3625 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11228 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TBY expr  */
#line 3626 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11234 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TALIGN expr  */
#line 3627 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11240 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr THASH expr  */
#line 3628 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11246 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TDMAPPED expr  */
#line 3629 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11252 "bison-chpl-lib.cpp"
    break;

  case 699: /* unary_op_expr: TPLUS expr  */
#line 3633 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11258 "bison-chpl-lib.cpp"
    break;

  case 700: /* unary_op_expr: TMINUS expr  */
#line 3634 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11264 "bison-chpl-lib.cpp"
    break;

  case 701: /* unary_op_expr: TMINUSMINUS expr  */
#line 3635 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11270 "bison-chpl-lib.cpp"
    break;

  case 702: /* unary_op_expr: TPLUSPLUS expr  */
#line 3636 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11276 "bison-chpl-lib.cpp"
    break;

  case 703: /* unary_op_expr: TBANG expr  */
#line 3637 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11282 "bison-chpl-lib.cpp"
    break;

  case 704: /* unary_op_expr: expr TBANG  */
#line 3638 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11290 "bison-chpl-lib.cpp"
    break;

  case 705: /* unary_op_expr: TBNOT expr  */
#line 3641 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11296 "bison-chpl-lib.cpp"
    break;

  case 706: /* reduce_expr: expr TREDUCE expr  */
#line 3646 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11304 "bison-chpl-lib.cpp"
    break;

  case 707: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3650 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11312 "bison-chpl-lib.cpp"
    break;

  case 708: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3654 "chpl.ypp"
  {
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-2])), (yyvsp[-2].uniqueStr));
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), std::move(ident), toOwned((yyvsp[0].expr))).release();
  }
#line 11321 "bison-chpl-lib.cpp"
    break;

  case 709: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3659 "chpl.ypp"
  {
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-2])), (yyvsp[-2].uniqueStr));
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), std::move(ident), toOwned((yyvsp[0].expr))).release();
  }
#line 11330 "bison-chpl-lib.cpp"
    break;

  case 710: /* scan_expr: expr TSCAN expr  */
#line 3667 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11338 "bison-chpl-lib.cpp"
    break;

  case 711: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3671 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11346 "bison-chpl-lib.cpp"
    break;

  case 712: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3675 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11354 "bison-chpl-lib.cpp"
    break;

  case 713: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3679 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11362 "bison-chpl-lib.cpp"
    break;


#line 11366 "bison-chpl-lib.cpp"

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
