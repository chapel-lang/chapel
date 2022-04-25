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
  YYSYMBOL_extern_block_stmt = 231,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 232,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 233,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 234,                  /* if_stmt  */
  YYSYMBOL_ifvar = 235,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 236,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 237,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 238,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 239,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 240, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 241,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 242,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 243,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 244,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 245,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 246,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 247,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 248,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 249,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 250,                /* when_stmt  */
  YYSYMBOL_manager_expr = 251,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 252,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 253,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 254,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 255,              /* class_start  */
  YYSYMBOL_class_tag = 256,                /* class_tag  */
  YYSYMBOL_opt_inherit = 257,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 258,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 259,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 260,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 261,                  /* enum_ls  */
  YYSYMBOL_262_8 = 262,                    /* $@8  */
  YYSYMBOL_deprecated_enum_item = 263,     /* deprecated_enum_item  */
  YYSYMBOL_264_9 = 264,                    /* $@9  */
  YYSYMBOL_265_10 = 265,                   /* $@10  */
  YYSYMBOL_enum_item = 266,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 267,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 268,         /* lambda_decl_expr  */
  YYSYMBOL_269_11 = 269,                   /* $@11  */
  YYSYMBOL_linkage_spec_empty = 270,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 271,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt_complete = 272,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 273,             /* fn_decl_stmt  */
  YYSYMBOL_274_12 = 274,                   /* $@12  */
  YYSYMBOL_fn_decl_stmt_inner = 275,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 276,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 277,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 278,                 /* fn_ident  */
  YYSYMBOL_op_ident = 279,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 280,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 281,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 282,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 283,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 284,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 285,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 286,                /* formal_ls  */
  YYSYMBOL_formal = 287,                   /* formal  */
  YYSYMBOL_opt_formal_intent_tag = 288,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 289,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 290,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 291,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 292,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 293,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 294,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 295,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 296,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 297,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 298, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 299,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 300,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 301,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 302, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 303, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 304, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 305,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 306,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 307,            /* var_decl_stmt  */
  YYSYMBOL_308_13 = 308,                   /* $@13  */
  YYSYMBOL_var_decl_stmt_inner_ls = 309,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 310,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 311, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 312, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 313,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 314,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 315,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 316,                 /* opt_type  */
  YYSYMBOL_array_type = 317,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 318, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 319,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 320,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 321,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 322,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 323,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 324,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 325,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 326,                /* actual_ls  */
  YYSYMBOL_actual_expr = 327,              /* actual_expr  */
  YYSYMBOL_ident_expr = 328,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 329,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 330,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 331,                 /* for_expr  */
  YYSYMBOL_cond_expr = 332,                /* cond_expr  */
  YYSYMBOL_nil_expr = 333,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 334,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 335,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 336,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 337,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 338,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 339,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 340,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 341,          /* task_var_prefix  */
  YYSYMBOL_io_expr = 342,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 343,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 344,                 /* new_expr  */
  YYSYMBOL_let_expr = 345,                 /* let_expr  */
  YYSYMBOL_expr = 346,                     /* expr  */
  YYSYMBOL_opt_expr = 347,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 348,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 349,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 350,           /* call_base_expr  */
  YYSYMBOL_call_expr = 351,                /* call_expr  */
  YYSYMBOL_dot_expr = 352,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 353,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 354,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 355,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 356,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 357,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 358,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 359,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 360,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 361,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 362       /* reduce_scan_op_expr  */
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

#line 486 "bison-chpl-lib.cpp"

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
#define YYLAST   20645

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  182
/* YYNRULES -- Number of rules.  */
#define YYNRULES  719
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1272

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
     655,   656,   657,   666,   675,   698,   705,   713,   714,   715,
     716,   717,   718,   719,   720,   734,   749,   757,   769,   780,
     792,   804,   815,   831,   833,   832,   841,   840,   851,   852,
     856,   876,   891,   906,   922,   923,   925,   930,   931,   936,
     954,   958,   962,   973,   992,   993,   997,  1001,  1005,  1010,
    1014,  1018,  1028,  1033,  1039,  1046,  1051,  1058,  1069,  1070,
    1074,  1078,  1085,  1093,  1102,  1109,  1117,  1129,  1136,  1140,
    1145,  1151,  1160,  1161,  1165,  1174,  1178,  1182,  1186,  1190,
    1194,  1203,  1204,  1208,  1209,  1210,  1211,  1212,  1213,  1219,
    1220,  1221,  1222,  1223,  1224,  1239,  1240,  1251,  1252,  1253,
    1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,
    1264,  1265,  1266,  1267,  1268,  1269,  1273,  1274,  1275,  1276,
    1277,  1278,  1279,  1280,  1281,  1282,  1283,  1284,  1291,  1292,
    1293,  1294,  1298,  1299,  1303,  1310,  1320,  1322,  1321,  1330,
    1329,  1340,  1344,  1350,  1350,  1356,  1356,  1365,  1366,  1367,
    1368,  1369,  1370,  1371,  1375,  1380,  1385,  1390,  1397,  1405,
    1406,  1410,  1419,  1425,  1433,  1452,  1451,  1464,  1471,  1489,
    1502,  1515,  1519,  1523,  1527,  1531,  1535,  1539,  1543,  1561,
    1565,  1569,  1573,  1577,  1581,  1585,  1589,  1593,  1597,  1601,
    1605,  1609,  1613,  1617,  1621,  1625,  1629,  1633,  1637,  1641,
    1645,  1649,  1654,  1662,  1671,  1675,  1679,  1683,  1687,  1691,
    1695,  1699,  1703,  1708,  1713,  1718,  1726,  1741,  1759,  1763,
    1770,  1771,  1776,  1781,  1782,  1783,  1784,  1785,  1786,  1787,
    1788,  1789,  1790,  1791,  1792,  1793,  1805,  1806,  1807,  1808,
    1817,  1818,  1822,  1826,  1830,  1837,  1841,  1845,  1852,  1856,
    1860,  1864,  1871,  1872,  1876,  1880,  1884,  1891,  1904,  1920,
    1928,  1932,  1941,  1942,  1946,  1950,  1955,  1963,  1968,  1972,
    1979,  1980,  1984,  1993,  1998,  2009,  2016,  2017,  2018,  2022,
    2023,  2027,  2031,  2035,  2042,  2059,  2072,  2079,  2084,  2091,
    2090,  2103,  2105,  2104,  2113,  2112,  2123,  2130,  2142,  2159,
    2157,  2184,  2188,  2189,  2191,  2196,  2206,  2203,  2231,  2239,
    2247,  2258,  2269,  2278,  2293,  2294,  2298,  2299,  2300,  2309,
    2310,  2311,  2312,  2313,  2314,  2315,  2316,  2317,  2318,  2319,
    2320,  2321,  2322,  2323,  2324,  2325,  2326,  2327,  2328,  2329,
    2330,  2331,  2332,  2333,  2334,  2338,  2339,  2340,  2341,  2342,
    2343,  2344,  2345,  2346,  2347,  2348,  2349,  2354,  2355,  2359,
    2360,  2361,  2365,  2366,  2370,  2374,  2375,  2379,  2380,  2384,
    2393,  2404,  2414,  2426,  2438,  2446,  2450,  2458,  2459,  2460,
    2461,  2462,  2463,  2464,  2465,  2466,  2470,  2471,  2472,  2473,
    2474,  2475,  2479,  2480,  2481,  2485,  2486,  2487,  2488,  2489,
    2490,  2494,  2495,  2498,  2499,  2503,  2504,  2508,  2513,  2514,
    2516,  2518,  2520,  2525,  2527,  2532,  2534,  2536,  2538,  2540,
    2542,  2544,  2549,  2550,  2554,  2562,  2566,  2571,  2579,  2583,
    2590,  2611,  2612,  2614,  2622,  2623,  2624,  2625,  2626,  2631,
    2630,  2639,  2647,  2651,  2658,  2674,  2691,  2695,  2699,  2706,
    2708,  2710,  2717,  2718,  2719,  2724,  2728,  2732,  2736,  2740,
    2744,  2748,  2755,  2756,  2757,  2758,  2759,  2764,  2765,  2766,
    2767,  2768,  2788,  2792,  2796,  2800,  2807,  2808,  2809,  2813,
    2818,  2826,  2831,  2835,  2842,  2843,  2844,  2845,  2846,  2852,
    2853,  2854,  2855,  2859,  2860,  2864,  2865,  2866,  2870,  2874,
    2881,  2882,  2886,  2891,  2900,  2901,  2902,  2903,  2907,  2908,
    2919,  2921,  2923,  2929,  2930,  2931,  2932,  2933,  2934,  2936,
    2938,  2940,  2942,  2948,  2950,  2953,  2955,  2957,  2959,  2961,
    2963,  2965,  2967,  2970,  2972,  2977,  2986,  2995,  3003,  3017,
    3031,  3045,  3054,  3063,  3071,  3085,  3099,  3113,  3130,  3139,
    3148,  3163,  3181,  3199,  3207,  3208,  3209,  3210,  3211,  3212,
    3213,  3214,  3219,  3220,  3224,  3233,  3234,  3238,  3247,  3248,
    3252,  3267,  3271,  3278,  3279,  3280,  3281,  3282,  3283,  3287,
    3288,  3293,  3295,  3297,  3299,  3301,  3307,  3314,  3326,  3338,
    3351,  3368,  3378,  3379,  3380,  3381,  3382,  3383,  3384,  3385,
    3386,  3387,  3391,  3395,  3400,  3405,  3410,  3416,  3422,  3454,
    3455,  3459,  3460,  3461,  3465,  3466,  3467,  3468,  3477,  3478,
    3481,  3482,  3483,  3487,  3499,  3511,  3518,  3520,  3522,  3524,
    3526,  3532,  3545,  3546,  3550,  3554,  3561,  3562,  3566,  3567,
    3571,  3572,  3573,  3574,  3575,  3576,  3577,  3578,  3582,  3586,
    3590,  3594,  3598,  3607,  3612,  3621,  3622,  3623,  3624,  3625,
    3626,  3627,  3628,  3629,  3630,  3631,  3632,  3633,  3634,  3635,
    3636,  3637,  3638,  3639,  3640,  3641,  3642,  3643,  3647,  3648,
    3649,  3650,  3651,  3652,  3655,  3659,  3663,  3667,  3672,  3680,
    3684,  3688,  3692,  3700,  3701,  3702,  3703,  3704,  3705,  3706
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
  "extern_export_decl_stmt_start", "extern_export_decl_stmt", "$@7",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
  "catch_expr", "catch_expr_inner", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "manager_expr", "manager_expr_ls",
  "manage_stmt", "class_decl_stmt", "class_start", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header_lcbr", "enum_ls", "$@8", "deprecated_enum_item", "$@9",
  "$@10", "enum_item", "lambda_decl_start", "lambda_decl_expr", "$@11",
  "linkage_spec_empty", "linkage_spec", "fn_decl_stmt_complete",
  "fn_decl_stmt", "$@12", "fn_decl_stmt_inner", "fn_decl_stmt_start",
  "fn_decl_receiver_expr", "fn_ident", "op_ident", "assignop_ident",
  "all_op_name", "formal_var_arg_expr", "opt_formal_ls", "req_formal_ls",
  "formal_ls_inner", "formal_ls", "formal", "opt_formal_intent_tag",
  "required_intent_tag", "opt_this_intent_tag", "proc_iter_or_op",
  "opt_ret_tag", "opt_throws_error", "opt_function_body_stmt",
  "function_body_stmt", "query_expr", "opt_lifetime_where",
  "lifetime_components_expr", "lifetime_expr", "lifetime_ident",
  "type_alias_decl_stmt", "type_alias_decl_stmt_start",
  "type_alias_decl_stmt_inner_ls", "type_alias_decl_stmt_inner",
  "opt_init_type", "var_decl_type", "var_decl_stmt", "$@13",
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

#define YYPACT_NINF (-1094)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-720)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1094,    85,  3741, -1094,   -63,    82, -1094, -1094, -1094, -1094,
   -1094, -1094, 11934,     5,   167,   157, 14794,   207,  3208,     5,
   11934,   287,   247,   275,   167,  5141, 11934,   405,  5141,   267,
   20442, -1094,    52,   399,  8804, 10196, 10196, -1094,  8977,   422,
     574,   326, -1094,   467, 20442, 20442, 20442, -1094, 15181, 10369,
     477, 11934, 11934,   257, -1094,   515,   550, 11934, -1094, 14794,
   -1094, 11934,   419,   471,   387,   571,   614, 20529, -1094, 10544,
    8283, 11934, 10369, 14794, 11934,   595,   619,   524,  5141,   648,
   11934,   662,  7412,  7412, -1094,   664, -1094, 14794, -1094,   667,
    8977, 11934, -1094, 11934, -1094, 11934, -1094, -1094, 14311, 11934,
   -1094, 11934, -1094, -1094, -1094,  4091,  7587,  9152, 11934, -1094,
    4966, -1094, -1094, -1094, -1094,   554, -1094,    27, -1094, -1094,
   -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094,
   -1094,  3549, -1094, 10717, -1094, -1094, -1094, -1094, -1094,   671,
   -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094,   586, 20442,
   -1094, 20167,   566,   497, -1094,   382, -1094, -1094,   255,   446,
   -1094, 20442, 15181, -1094,   577, -1094,   579, -1094, -1094,   581,
     582,   593, 11934,   588,   590, 19778,  3031,   106,   591,   592,
   -1094, -1094,   445, -1094, -1094, -1094, -1094, -1094,   138, -1094,
   -1094, -1094, 11934, 11934, 11934, 20442, -1094, 11934, 10544, 10544,
     703,   480, -1094, -1094, -1094, -1094, 19778,   491, -1094, -1094,
     602,  5141, -1094, -1094,   599,   -17,   597, 16744, -1094,   231,
   -1094,   600, -1094,   -58, 19778, -1094,  1764,   641,  8458, -1094,
   -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094,
   -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094,
   -1094, -1094, -1094,   608, -1094, -1094, -1094, 20442,   611,    32,
    2712,    13,  2893,    13, 16305, 20442, 20442,   -30, 15545, -1094,
   -1094,   699,  8458,   615,   511,  5141,  1852,   105,    23, -1094,
    5141, -1094, -1094, 16784, -1094,    49, 17293,   442, -1094,   616,
     617, -1094, 16784,   -17,   442, -1094,  8458,  1882,  1882, -1094,
   -1094,   193, 19778, 11934, 11934, -1094, 19778,   621, 17400, -1094,
   16784,   -17, 19778,   622,  8458, -1094, 19778, 17445, -1094, -1094,
   -1094, -1094,   -17,    32, 16784, 17485,   214,   214,   736,   442,
     442,   224, -1094, -1094,  4266,   -24, -1094, 11934, -1094,   -62,
     -28, -1094,   112,   174, 17637,   -55,   736,   783, -1094,  4441,
   11934, -1094, 11934,   729, -1094, -1094, 16345,   201,   586, 19778,
     584, 20442, 10544,   626, -1094,   628,   789,   675,   135, -1094,
   -1094,    53,   255, -1094, -1094, -1094, -1094,   712, -1094, -1094,
   -1094,   123,   713, -1094, -1094, -1094, 14143,   683,   211, -1094,
     363,   514, -1094, 11934,   663, 11934, 11934, 11934, 10196, 10196,
   11934,   527, 11934, 11934, 11934, 11934, 11934,   266, 14311, 11934,
   11934, 11934, 11934, 11934, 11934, 11934, 11934, 11934, 11934, 11934,
   11934, 11934, 11934, 11934, 11934,   741, -1094, -1094, -1094, -1094,
   -1094,  9325,  9325, -1094, -1094, -1094, -1094,  9325, -1094, -1094,
    9325,  9325,  8458,  8458, 10196, 10196, 16824, 16976, 17677,   642,
      68, 20442,  8110, -1094, -1094,  4616, -1094, 10196,    32, 15181,
   -1094, 11934, -1094,  1764, -1094, 20442,   704, -1094, -1094,   699,
   11934,   690, -1094,   650,   680, -1094, -1094, -1094,   773, 10544,
   -1094,  5316, 10196, -1094,   681, -1094,    32,  5491, 10196, -1094,
      32, -1094,    32, 10196, -1094,    32,   709,   711,  5141,   817,
    5141, -1094,   819, 11934,   791,   682,  8458, 20442, -1094, -1094,
   -1094,  1852, -1094, -1094,   182, -1094, 12107,   734, 11934, 15181,
   -1094, -1094, 11934, -1094, 20254, 11934, 11934, -1094,   688, -1094,
   -1094, 10544, -1094, 19778, 19778, -1094,    40, -1094,  8458,   689,
   -1094,   844,   844, -1094, -1094, -1094, -1094, -1094, -1094,  9500,
   -1094, 17717,  7762, -1094,  7937, -1094,  5141,   692, 10196,  9675,
    3916,   697, 11934, 10890, -1094, -1094,   415, -1094,  4791, -1094,
     376, 17869,   378, 16502, 20442,  7239,  7239, -1094,   586,   694,
     231, -1094,   195,   727,  1406, -1094, -1094, 20442, 11934,   297,
   -1094,    65, -1094, -1094, -1094, -1094, -1094, -1094,  1077,   732,
     702, -1094, 15268, -1094,    75, -1094, -1094, 12280,   781, -1094,
     715,   126, -1094,   742, -1094,   744,   745,   749,   753,   757,
   -1094,   758,   772,   765,   766,   767,   232,   776,   769,   770,
   -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094,
   -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094,
   -1094, -1094, -1094, 11934, -1094,   784,   792,   775,   715, -1094,
     715, -1094, 12453, -1094, 20442, -1094, -1094, 17909, 19930, 19930,
     494, -1094,   494, -1094,   494, 18781,  1360,   464,   164,   -17,
     214, -1094,   542, -1094, -1094, -1094, -1094, -1094,   736,  1942,
     494,  1144,  1144, 19930,  1144,  1144,  1021,   214,  1942, 20006,
    1021,   442,   442,   214,   736,   764,   768,   771,   774,   778,
     779,   724,   760, -1094,   494, -1094,   494, 12626, 10196, 12799,
   10196, 11934,  8458, 10196, 15076,   777,    69, -1094, -1094, -1094,
     190, -1094,  1302, 19854,   510,     5, 17954, -1094,   403, 19778,
   -1094, 18061,  8458, -1094,  8458, 11934,   205,  8977, 19778,    34,
   17016,  8110, -1094,  8977, 19778,    22, 16547, -1094, -1094,    13,
   16592, -1094, 11934, 11934,   902,  5141,   905, 18106,  5141, 17056,
   20442, -1094,   206, -1094,   291, -1094,   304,  1852,   105,   -20,
     -18, 11934, 11934,  7066, -1094, -1094,   575,  8631, -1094, 19778,
   -1094, -1094, -1094, 20442, 18146, 18298, -1094, -1094, 19778,   780,
      39,   787, -1094,  3369, -1094, -1094, -1094, -1094, -1094, -1094,
   -1094, -1094,  5141,   139, 17208, -1094, -1094, 19778,  5141, 19778,
   -1094, 18339, -1094, -1094, 11934, -1094,    48, 15303, 11934, -1094,
   11063,  7239,  7239, -1094, 11934,   536, 11934,   555,   562, 11934,
    9848,   583,   408, -1094, -1094, -1094, 18380,   795,   785,   786,
   -1094, 15181, -1094,  8458,   790,  1548, 20442, -1094, 19778, 20355,
   -1094, -1094, 20442,  1123, -1094,  1852,   800,    12,  6891, -1094,
   -1094,   256, -1094,    12,    53, -1094, 18498, -1094, 15462, -1094,
   -1094, -1094,   410, -1094,   793,   788, -1094, -1094, -1094, -1094,
   -1094, -1094, -1094, -1094, 11934,   913, 18539, 11934,   921, 18580,
     316,   794, 18698,  8458, -1094, -1094,  8110, -1094, -1094, -1094,
   -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094,    63,
   10196, 10196,    32,    32, -1094, -1094, -1094, -1094, -1094, 16784,
   -1094, 15621,  5666, -1094,  5841, -1094,   319, -1094, 15697,  6016,
   -1094,    32,  6191, -1094,    32, -1094,    32, -1094,    32, 19778,
   19778,  5141, -1094,  5141, 11934, -1094,  5141,   922,   802,   803,
   20442,   597, -1094, -1094,   734,   797,    79, -1094, -1094, -1094,
     805,   808, -1094,  6366, 10544, -1094, -1094, -1094, 20442, -1094,
     820,   597, -1094,  6541,   799,  6716,   807, -1094, 11934, -1094,
   -1094,  7239, -1094, 18739,    71, 17248,   412,   810,  7239, -1094,
   11934, -1094, -1094,  2314,   413,   347, -1094,   977, -1094, -1094,
   -1094,   754, -1094, -1094, -1094, -1094,   800, -1094,   356, 11238,
     334,    47, 11934, -1094,   812, 14010,   110, -1094,   818, -1094,
     852,   855,   715,   715, -1094, -1094, 18857, 12972, 13145, 18933,
   13318, 13491, -1094, 13664, 13837,   357, -1094,   734,   371,   372,
   -1094, -1094, -1094,  5141,  8977, 19778,  8977, 19778,  8110, -1094,
    5141,  8977, 19778, -1094,  8977, 19778, -1094, -1094, -1094, -1094,
   -1094, 19778,   962,  5141, -1094, -1094, -1094, -1094, -1094, -1094,
   10196, 14935,  8458,  8458,  5141, -1094,    59,   825, 11934, -1094,
    8977, -1094, 19778,  5141,  8977, -1094, 19778,  5141, 19778,   333,
   11411,  7239,  7239,  7239,  7239, -1094, -1094, -1094, 19018, 19778,
    3316, -1094, -1094,   834, -1094,  1568, -1094,   334,   800, 10023,
   -1094, -1094, -1094,    92, 10544, -1094, -1094, -1094,    74, -1094,
      94, -1094,   329, 19094,    54, -1094, -1094,   508, 14481,   172,
   -1094, -1094, -1094, 11934, 11934, 11934, 11934, 11934, 11934, 11934,
   11934, -1094, -1094, 18106, 15773, 15849, -1094, 18106, 15925, 16001,
    5141, -1094, 19170, -1094,   636,   827,   828,   641, -1094,   597,
   19778, 16077, -1094, 16153, -1094, -1094, -1094, 19778,   414,   837,
     416,   838, 11934, -1094, -1094,  1568, -1094, -1094, -1094,   334,
   11586,   115, -1094, 19778, -1094, 11934,    47,    74,    74,    74,
      74,    74,    74,    47, -1094, -1094, -1094, -1094,   509, -1094,
   -1094, -1094, 19246, 19322, 19398, 19474, 19550, 19626,  5141,  5141,
    5141,  5141, -1094, 13837,   840,   841, -1094,  5141,  5141, -1094,
   -1094, -1094, -1094, 19778, -1094, -1094, -1094, -1094, -1094, -1094,
     298, 10196, 14622, 19778, -1094, -1094, -1094, -1094, -1094, -1094,
   -1094,   861, 11934, 11934, 11934, 11934, 11934, 11934, 18106, 18106,
   18106, 18106,   782, -1094, -1094, 18106, 18106, 19702, -1094, -1094,
   11761, -1094
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   115,   662,   663,   664,   658,
     659,   665,     0,   582,   101,   136,   551,   143,   553,   582,
       0,   142,   459,   457,   101,     0,     0,    46,     0,   260,
     137,   180,   179,   656,     0,     0,     0,   178,     0,   141,
      54,   261,   323,   138,     0,     0,     0,   318,     0,     0,
     145,     0,     0,   601,   573,   666,   146,     0,   324,   545,
     454,     0,     0,     0,   165,   163,   140,   554,   456,     0,
       0,     0,     0,   549,     0,     0,   144,     0,     0,   116,
       0,   657,     0,     0,   445,   139,   298,   547,   458,   147,
       0,     0,   715,     0,   717,     0,   718,   719,   628,     0,
     716,   713,   532,   161,   714,     0,     0,     0,     0,     4,
       0,     5,     9,    10,    43,     0,    48,    57,    11,    63,
      12,    13,    14,    15,    27,   528,   529,    21,    49,   162,
     172,     0,   181,   629,   173,    16,    29,    28,    18,     0,
     255,    17,   620,    20,    33,    30,    31,   171,   299,     0,
     169,     0,     0,   617,   322,     0,   167,   325,   415,   406,
     170,     0,     0,   168,   634,   613,   530,   614,   535,   533,
       0,     0,     0,   618,   619,     0,   534,     0,   635,   636,
     637,   660,   661,   612,   537,   536,   615,   616,     0,    26,
     553,   137,     0,     0,     0,     0,   554,     0,     0,     0,
       0,   617,   634,   533,   618,   619,   543,   534,   635,   636,
       0,     0,   583,   102,     0,   552,     0,   582,   446,     0,
     455,     0,    19,     0,   513,    44,   321,     0,   520,   109,
     117,   129,   123,   122,   131,   112,   121,   132,   118,   133,
     110,   134,   127,   120,   128,   126,   124,   125,   111,   113,
     119,   130,   135,     0,   114,   187,   447,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      55,    57,   520,     0,     0,     0,     0,     0,     0,   462,
       0,   153,    38,     0,   290,     0,   289,   700,   605,   602,
     603,   604,     0,   546,   701,     7,   520,   321,   321,   427,
     510,     0,   509,     0,     0,   154,   633,     0,     0,    41,
       0,   550,   538,     0,   520,    42,   544,     0,   268,   272,
     269,   272,   548,     0,     0,     0,   702,   704,   626,   699,
     698,     0,    60,    64,     0,     0,   515,     0,   517,     0,
       0,   516,     0,     0,   509,     0,   627,     0,     6,     0,
       0,    58,     0,     0,   459,   177,     0,   658,   299,   630,
     185,     0,     0,     0,   295,     0,   314,   316,     0,   307,
     311,   395,   415,   413,   414,   412,   333,   416,   419,   418,
     420,     0,   410,   407,   408,   411,     0,   451,     0,   448,
       0,   531,    32,     0,   606,     0,     0,     0,     0,     0,
       0,   703,     0,     0,     0,     0,     0,     0,   625,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,   372,   373,   374,   369,
     371,     0,     0,   367,   370,   368,   366,     0,   376,   375,
       0,     0,   520,   520,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    34,    22,     0,    35,     0,     0,     0,
      23,     0,    36,   321,   296,     0,   179,   297,    47,    57,
       0,   528,   526,     0,   521,   522,   527,   306,     0,     0,
     197,     0,     0,   196,     0,   205,     0,     0,     0,   203,
       0,   213,     0,     0,   211,     0,     0,     0,     0,   229,
       0,   365,   225,     0,     0,     0,     0,     0,   239,    24,
     466,     0,   467,   469,     0,   491,     0,   472,     0,     0,
     152,    37,     0,   292,     0,     0,     0,    39,     0,   166,
     164,     0,    94,   631,   632,   155,     0,    40,     0,     0,
     279,   270,   271,   190,   189,    25,    62,    61,    65,     0,
     667,     0,     0,   652,     0,   654,     0,     0,     0,     0,
       0,     0,     0,     0,   671,     8,     0,    51,     0,    92,
       0,    88,     0,    72,     0,     0,    78,   174,   299,     0,
       0,   184,   266,   300,     0,   305,   312,     0,     0,   309,
     304,   402,   397,   398,   399,   403,   404,   405,   395,   388,
       0,   385,     0,   396,     0,   417,   486,     0,   421,   409,
     382,   103,   360,   117,   358,   123,   122,   106,   121,   118,
     363,   133,   104,   134,   120,   124,   105,   107,   119,   135,
     357,   339,   342,   340,   341,   364,   352,   343,   356,   348,
     346,   359,   362,   347,   345,   350,   355,   344,   349,   353,
     354,   351,   361,     0,   336,     0,   108,     0,   382,   337,
     382,   334,     0,   450,     0,   444,   461,   581,   695,   694,
     697,   706,   705,   710,   709,   691,   688,   689,   690,   622,
     678,   115,     0,   648,   649,   116,   647,   646,   623,   682,
     693,   687,   685,   696,   686,   684,   676,   681,   683,   692,
     675,   679,   680,   677,   624,     0,     0,     0,     0,     0,
       0,     0,     0,   708,   707,   712,   711,     0,     0,     0,
       0,     0,     0,     0,   669,   266,   593,   594,   596,   598,
       0,   585,     0,     0,     0,   582,   582,   193,     0,   514,
      45,     0,     0,   540,     0,     0,     0,     0,   557,     0,
       0,     0,   206,     0,   563,     0,     0,   204,   214,     0,
       0,   212,     0,     0,   228,     0,   224,     0,     0,     0,
       0,   539,     0,   242,     0,   240,     0,   470,     0,   150,
     151,   149,   148,     0,   490,   489,   613,     0,   464,   611,
     463,   291,   288,     0,     0,     0,   645,   512,   511,     0,
       0,     0,   541,     0,   273,   668,   621,   653,   518,   655,
     519,   221,     0,     0,     0,   670,   219,   567,     0,   673,
     672,     0,    53,    52,     0,    87,     0,     0,     0,    80,
       0,     0,    78,    50,   357,   339,   342,   340,   341,   350,
     349,   351,     0,   377,   378,    67,    66,    79,     0,     0,
     301,     0,   263,     0,     0,   321,     0,   315,   317,     0,
     400,   401,     0,   395,   384,     0,   504,   428,     0,   485,
     484,   613,   422,   428,   395,   332,     0,   338,     0,   328,
     329,   453,   613,   449,     0,     0,   100,    98,    99,    97,
      96,    95,   643,   644,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   595,   597,     0,   584,   136,   143,
     142,   141,   138,   145,   146,   140,   144,   139,   147,     0,
       0,     0,     0,     0,   460,   188,   524,   525,   523,     0,
     223,     0,     0,   195,     0,   194,     0,   588,     0,     0,
     201,     0,     0,   199,     0,   209,     0,   207,     0,   237,
     236,     0,   231,     0,     0,   227,     0,   233,     0,   265,
       0,     0,   468,   471,   472,     0,     0,   473,   474,   287,
       0,     0,   281,     0,     0,   280,   283,   542,     0,   274,
     277,     0,   222,     0,     0,     0,     0,   220,     0,    93,
      90,     0,    89,    75,    74,    73,     0,     0,     0,   175,
       0,   176,   301,   321,     0,     0,   294,   159,   165,   163,
     293,   321,   302,   156,   313,   310,   504,   386,     0,     0,
     472,     0,     0,   319,     0,   476,     0,   326,     0,   335,
     103,   105,   382,   382,   650,   651,     0,     0,     0,     0,
       0,     0,   265,     0,     0,     0,   586,   472,   634,   634,
     192,   191,   198,     0,     0,   556,     0,   555,     0,   587,
       0,     0,   562,   202,     0,   561,   200,   210,   208,   230,
     226,   572,   232,     0,    59,   262,   241,   238,   465,   495,
       0,   669,   520,   520,     0,   285,     0,     0,     0,   275,
       0,   217,   569,     0,     0,   215,   568,     0,   674,     0,
       0,     0,    78,     0,    78,    81,    84,    70,    69,    68,
     321,   182,   186,   267,   157,   321,   303,   472,   504,     0,
     507,   506,   508,   613,   379,   391,   389,   442,     0,   443,
     430,   433,     0,   429,     0,   481,   479,   613,   669,     0,
     383,   330,   331,     0,     0,     0,     0,     0,     0,     0,
       0,   267,   590,   560,     0,     0,   589,   566,     0,     0,
       0,   235,     0,   493,   613,     0,     0,   286,   284,     0,
     278,     0,   218,     0,   216,    91,    77,    76,     0,     0,
       0,     0,     0,   183,   264,   321,   160,   392,   390,   472,
     496,     0,   381,   380,   441,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   425,   426,   320,   480,   613,   423,
     327,   424,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   234,     0,   607,   608,   276,     0,     0,    83,
      86,    82,    85,    71,   158,   394,   393,   498,   499,   501,
     613,     0,   669,   432,   434,   435,   438,   439,   440,   436,
     437,   431,     0,     0,     0,     0,     0,     0,   559,   558,
     565,   564,   613,   609,   610,   571,   570,     0,   500,   502,
     496,   503
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1094, -1094, -1094,    -1,  -358,  2256,   240, -1094, -1094, -1094,
    -196, -1094, -1094,   -25,   738, -1094,  -692,  2352,   665,  -569,
   -1094,  -807, -1094, -1094,   187, -1094, -1094, -1094,   988, -1094,
    2874,  -124,  -383, -1094,  -595,  2624,  -627,     2, -1094, -1094,
    -838, -1094, -1094,   392, -1094, -1094, -1094, -1094, -1094, -1094,
   -1094, -1094,   140, -1094,   925, -1094, -1094,    56,  2025, -1094,
   -1094, -1094, -1094,   696, -1094,    41, -1094, -1094, -1094, -1094,
     496, -1094, -1094, -1094,  -102, -1094,  -326,  -802, -1094, -1094,
   -1094, -1094,   161, -1094, -1094,  -560, -1094,   483, -1094, -1094,
   -1094, -1094,   661, -1094, -1094, -1094, -1094,   144,  -379,  -165,
    -792, -1061,  -616, -1094, -1094,   149,   170,   426, -1094, -1094,
   -1094,   655, -1094, -1094,  -109,   128,   162,  -169,  -159,  -351,
   -1094, -1094, -1094,   374, -1094,  -209,   909, -1094,  -160,   517,
     268,  -492,  -910,  -972, -1094,  -575,  -646,  -969, -1093,  -947,
     -64, -1094,   163, -1094,  -244,  -485,  -505,   166,  -498, -1094,
   -1094, -1094,   843, -1094,     1, -1094, -1094,  -164, -1094,  -717,
   -1094, -1094, -1094,  1189,  1254,   -12, -1094,    20,  1486, -1094,
    1557,  1735, -1094, -1094, -1094, -1094, -1094, -1094, -1094, -1094,
   -1094,  -443
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   333,   110,   766,   112,   113,   463,   226,
     114,   115,   116,   117,   353,   118,   119,   120,   334,   847,
     572,   848,   121,   122,   569,   570,   123,   124,   214,   654,
     277,   125,   254,   126,   784,   282,   127,  1012,  1185,  1115,
     128,   298,   297,   129,   130,   131,   132,   133,   134,   580,
     135,   136,   901,   137,   267,   138,   774,   775,   200,   140,
     141,   142,   143,   541,   804,   981,   144,   145,   800,   976,
     284,   285,   146,   147,   148,   149,   363,   855,   150,   151,
     368,   859,   369,   856,   587,   370,   152,   201,  1134,   154,
     155,   156,   157,  1139,   158,   159,   657,   658,   843,   844,
     845,  1125,   875,   372,   599,   600,   601,   602,   603,   386,
     376,   381,   873,  1210,  1206,   472,  1023,  1130,  1131,  1132,
     160,   161,   388,   389,   663,   162,   163,   219,   278,   279,
     513,   514,   788,   870,   608,   517,   785,  1238,  1122,  1020,
     335,   223,   339,   340,   473,   474,   475,   202,   165,   166,
     167,   168,   203,   170,   211,   212,   730,   486,   936,   731,
     732,   171,   172,   204,   205,   175,   360,   476,   207,   177,
     208,   209,   180,   181,   182,   183,   345,   184,   185,   186,
     187,   188
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     206,   109,   390,   656,   215,   301,   842,   659,   217,   734,
     459,   441,   869,   598,   224,   271,   881,  1013,   786,   776,
     216,   772,   260,   262,   264,   997,   268,   857,   505,   867,
     468,   358,   579,   801,   937,   992,  -260,   283,  -261,   286,
     287,   799,   879,   343,   880,   292,   280,   293,  1003,   294,
    1127,   681,   528,  1136,  1078,   939,  1187,   302,   306,   308,
     310,   311,   312,   255,   515,   280,   316,   932,   317,  1117,
     539,   518,   498,  1021,   350,   322,   515,  1127,   324,   325,
     591,   326,   280,   327,   552,     3,   328,   329,   461,   330,
     307,   563,   280,   302,   306,   344,   346,  1239,   490,   492,
     495,   592,   189,   503,   471,   593,   515,   462,  1101,   871,
    1126,   553,   351,   860,   973,  -282,   723,   904,   554,   356,
     564,   359,   549,   401,   606,   210,   341,  1080,  1235,   594,
    1022,   407,   595,   484,    62,   450,  -487,  1152,  -243,  1128,
     455,   352,   484,   596,  1102,   555,    70,   550,   471,  1269,
    1129,   685,   228,  -487,   272,   861,   974,  -282,   723,   905,
     394,   256,   597,  1241,   882,  1013,  1207,  -487,   164,   519,
     213,  1189,   471,  1013,   259,   261,   263,  1129,   557,   561,
     446,   447,   448,   455,  -482,   316,   302,   344,  -487,  1046,
     471,   164,   455,  -487,   164,   522,   397,   300,   711,   712,
    1110,   469,   455,   964,   455,   531,  1107,  1188,   516,  -487,
     975,  -282,  1195,  -244,   559,  -482,   306,  -505,   458,   455,
     516,   660,   558,   464,   455,   559,  -387,   444,  -482,   455,
    -487,   445,   484,   300,   338,   300,  -487,   900,  -505,   928,
    1196,  -482,  -505,   724,   164,  -487,   397,   342,   164,   164,
     516,  -487,   849,   398,  1081,   578,   559,   399,    23,   484,
     306,   559,   996,  -250,    70,  -505,  -109,   740,   607,   681,
    -487,   164,  1013,  1268,  -115,   288,   164,  1186,   442,  1236,
     443,   589,   377,   687,   306,  1138,   682,   556,  -482,   467,
    1242,   533,   534,  -482,   484,  1179,  1014,  1181,   583,   738,
     683,  1271,   306,   398,   401,   402,   590,   399,   734,   405,
      60,   406,   407,    86,   983,   793,   410,  -483,   471,   471,
     559,    68,   318,   320,   417,   551,   300,   300,   777,   684,
     421,   422,   423,   548,   378,   289,   906,  1209,   571,   531,
     573,  1156,   455,  -249,  1047,   379,    88,  1234,  -483,   560,
     302,   531,   744,   290,   401,   778,   218,   664,   532,   405,
     852,  -483,   407,   907,   380,   220,   410,   853,  1005,   685,
     291,   851,  -111,  1018,  -483,   686,   665,   164,   930,   959,
    -116,   667,   471,   668,   669,   670,   672,   674,   675,   189,
     676,   677,   678,   679,   680,   546,   688,   689,   690,   691,
     692,   693,   694,   695,   696,   697,   698,   699,   700,   701,
     702,   703,   704,   225,   471,   746,  1141,  1142,  1045,   306,
     306,  -483,  1099,  -497,  1120,   306,  -483,   295,   306,   306,
     306,   306,   714,   716,   -56,  1163,   373,   960,   469,   228,
     733,   164,  1204,  -308,  -497,   736,   164,  1204,  -497,   739,
     777,   706,   707,   -56,  1197,  -258,   374,   708,   741,   787,
     709,   710,   744,   734,   961,  1058,   375,   302,  -308,   748,
     750,  -497,   -56,   382,   397,   754,   756,   962,  -248,   998,
    1198,   760,  1199,  1200,  1124,   153,  1201,  1202,   767,  1042,
     300,   769,  1059,   744,   306,   656,   397,  1011,   272,   659,
     164,   -56,   777,   744,  1175,   598,   789,  1205,   153,   519,
     286,   153,  1205,   794,   795,   164,   598,  -592,  -591,   798,
    1113,  1123,   824,  -245,   828,   383,   306,  1137,   666,  1118,
    1151,   398,  1178,  -252,  1180,   399,   384,   798,   671,   673,
     306,   825,   306,   829,  -592,  -591,   814,   798,   817,   519,
     819,   821,   661,   398,   998,   385,  -452,   399,   998,   519,
     998,   153,   998,   846,   846,   153,   153,   548,   924,  -475,
    -477,  -256,   341,   999,   341,  -452,   858,  1105,  1112,  1229,
     189,  1231,   401,  1164,   713,   715,   822,   405,   153,   406,
     407,   941,   944,   153,   410,   946,   948,   735,   471,   921,
    -475,  -477,   417,   445,   401,   402,  -253,   300,   404,   405,
     423,   406,   407,  -475,  -477,   734,   410,  -642,   -55,  -642,
     471,   164,   749,  -488,   417,  -717,  -475,  -477,   755,  -717,
     421,   422,   423,   759,   401,    42,  -488,   -55,  -321,   405,
    1208,   876,   407,   296,  -718,  1011,   410,   164,  -718,   984,
     986,  -719,  -641,   164,  -641,  -719,   -55,   269,  -321,   797,
     270,    58,  -578,  -638,   164,  -638,   164,  -488,  -321,  -641,
    -247,  -641,  -714,  -475,  -477,  -251,  -714,   797,  -475,  -477,
     338,   455,   338,   507,  -492,   -55,  -640,   797,  -640,   529,
     530,  1004,  1240,  -488,   153,   313,   314,  -492,   813,  -639,
    -488,  -639,   990,   687,  -257,   748,   896,   754,   899,   767,
     306,   902,   817,  1033,   884,   808,   885,   810,  -259,   966,
    -246,  -488,   164,  -254,   349,  1262,   164,   361,  -492,   471,
     306,   362,   306,   929,   164,   931,   922,   923,   371,   733,
    -488,   938,  -575,   391,  1240,  -488,  -574,   392,  -488,   393,
     949,   950,  1011,  -579,  -492,  -580,  -577,  -576,   153,   451,
     470,  -492,   927,   153,   454,   460,   503,   455,   397,   312,
     316,   344,  1240,   503,   452,   306,   464,  1194,   477,   471,
      22,    23,  -492,   479,   351,  1007,   535,   506,   525,   526,
     465,   565,    31,   466,   538,   574,   584,   586,    37,   585,
     588,  -492,   605,   609,  1026,    42,  -492,   968,   662,  -492,
     705,   407,   571,   256,   722,   742,   993,   153,   995,   846,
     846,   745,   326,   743,   327,   398,   744,   329,   330,   399,
    -494,    58,   153,    60,   762,   347,   763,  1008,  1165,  1166,
    1009,   306,   467,  -494,    68,   169,  1245,  1246,  1247,  1248,
    1249,  1250,   765,   751,   768,   771,   344,   770,   895,   787,
     898,   796,   802,    84,   850,   803,    86,   812,   169,    88,
     926,   169,   818,   531,  -494,   864,   401,   402,   863,   403,
     404,   405,  1036,   406,   407,  1039,   872,   874,   410,  -112,
    -136,   306,  -143,  -142,   733,   416,   417,   892,   919,   420,
    -494,  -141,   421,   422,   423,  -138,  -145,  -494,   672,   714,
    1086,   300,  -110,  -146,  -140,  -144,  -113,  -139,  -147,   103,
    1055,   169,  1057,   878,   877,   169,   169,  1062,  -494,   886,
    1065,   164,  -114,   887,   164,   893,   888,   951,   153,   889,
     953,   998,  1071,   890,   891,  1019,  1037,  -494,   169,   903,
    1001,   972,  -494,   169,  1040,  -494,  1002,  1073,   471,   471,
     977,  1006,   302,  1035,   153,  1088,  1034,  1074,  1075,  1043,
     153,  1092,  1079,  1096,  1093,  1106,  1098,  1082,   164,   846,
    1083,   153,  1097,   153,   164,  1114,  1108,  1135,  1109,   503,
     503,  1140,  -109,   503,   503,  -111,   300,  1160,  1169,  1184,
    1224,  1225,  1230,  1232,  1263,  1264,   503,  1196,   503,   504,
    1133,   989,   221,  1116,   568,   323,  1076,   542,   791,  1087,
    1015,   581,  1032,  1028,   862,  1055,  1057,   604,  1062,  1065,
    1211,  1092,  1096,  1017,  1251,  1027,   790,  1244,   883,   153,
     355,  1153,  1154,   153,  1155,   963,   733,     0,  1157,  1158,
       0,   153,  1159,   397,   169,  1191,     0,     0,     0,     0,
     671,   713,     0,     0,     0,     0,     0,     0,  1162,   817,
     306,   306,     0,     0,     0,     0,  1170,     0,  1171,     0,
       0,     0,  1173,     0,     0,     0,  1048,  1049,  1177,   846,
     846,   846,   846,     0,     0,     0,     0,     0,   164,     0,
     164,     0,   300,     0,   591,   164,     0,   344,   164,     0,
     398,     0,  1193,     0,   399,     0,     0,   164,   169,   164,
       0,     0,   164,   169,     0,   592,   817,     0,     0,   593,
       0,  1153,  1212,  1213,  1157,  1214,  1215,  1216,  1217,   164,
       0,     0,     0,     0,     0,     0,     0,  1121,     0,   164,
     591,   164,     0,   594,     0,     0,   595,     0,   347,     0,
       0,   401,   402,     0,   403,   404,   405,   596,   406,   407,
    1233,   592,     0,   410,     0,   593,   397,   169,     0,     0,
       0,   417,     0,  1243,     0,     0,   597,   421,   422,   423,
       0,   173,   169,     0,     0,     0,     0,     0,     0,   594,
       0,     0,   595,     0,    62,     0,  1258,  1259,  1260,  1261,
       0,  1096,     0,   596,   173,  1265,  1266,   173,     0,   164,
       0,     0,     0,     0,     0,     0,   164,     0,     0,  1267,
     817,     0,   597,   398,     0,     0,     0,   399,     0,   164,
    1258,  1259,  1260,  1261,  1265,  1266,     0,   300,   153,     0,
     164,   153,  1192,     0,     0,     0,   174,     0,  1096,   164,
       0,     0,     0,   164,     0,     0,     0,   173,     0,     0,
       0,   173,   173,     0,     0,     0,     0,     0,     0,   174,
       0,     0,   174,     0,   401,   402,     0,   403,   404,   405,
       0,   406,   407,   408,   173,   153,   410,     0,   169,   173,
       0,   153,     0,   416,   417,   681,     0,   420,     0,     0,
     421,   422,   423,     0,     0,     0,     0,     0,  1237,   908,
       0,   424,   909,     0,   169,     0,   164,   910,     0,     0,
     169,     0,   174,     0,     0,     0,   174,   174,   191,     0,
       0,   169,     0,   169,     0,     0,     0,     0,   911,     0,
       0,     0,     0,     0,     0,   912,     0,     0,     0,   174,
       0,     0,     0,     0,   174,   913,     0,     0,     0,     0,
    1237,     0,     0,   914,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   164,   164,   164,   164,     0,   915,
       0,     0,   397,   164,   164,     0,     0,     0,  1237,   169,
     173,   916,     0,   169,     0,   685,     0,   854,     0,     0,
       0,   169,   917,     0,     0,   153,     0,   153,   918,     0,
       0,     0,   153,     0,     0,   153,     0,     0,  -301,     0,
       0,     0,  -301,  -301,   153,     0,   153,  -301,     0,   153,
       0,     0,  -301,     0,  -301,  -301,     0,     0,     0,   398,
    -301,     0,     0,   399,     0,     0,   153,  -301,     0,     0,
    -301,     0,     0,     0,   173,   174,   153,     0,   153,   173,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -301,     0,     0,  -301,     0,  -301,     0,  -301,   176,  -301,
    -301,     0,  -301,     0,  -301,     0,  -301,     0,     0,     0,
     401,     0,     0,     0,     0,   405,     0,   406,   407,     0,
       0,   176,   410,     0,   176,  -301,     0,     0,  -301,     0,
     417,  -301,     0,   173,     0,     0,   421,   422,   423,   174,
       0,     0,     0,     0,   174,     0,   153,     0,   173,     0,
       0,     0,     0,   153,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   153,     0,     0,   178,
       0,     0,     0,     0,   176,     0,     0,   153,   176,   176,
     464,  -301,     0,     0,    22,    23,   153,  -301,     0,  1007,
     153,     0,   178,     0,   465,   178,    31,   466,   174,     0,
     464,   176,    37,     0,    22,    23,   176,     0,     0,    42,
       0,     0,     0,   174,   465,     0,    31,   466,   169,     0,
       0,   169,    37,     0,     0,     0,     0,     0,     0,    42,
       0,     0,     0,     0,     0,    58,     0,    60,     0,    62,
       0,  1008,     0,     0,  1009,   178,   467,     0,    68,   178,
     178,     0,     0,   153,   173,    58,     0,    60,     0,     0,
       0,  1008,     0,     0,  1009,   169,   467,    84,    68,     0,
      86,   169,   178,    88,     0,     0,     0,   178,     0,     0,
     173,     0,     0,     0,     0,     0,   173,    84,     0,     0,
      86,     0,     0,    88,     0,     0,     0,   173,     0,   173,
       0,     0,     0,     0,     0,     0,     0,   176,     0,     0,
       0,   153,   153,   153,   153,     0,     0,     0,     0,   174,
     153,   153,     0,   103,     0,     0,     0,     0,     0,  1010,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   103,     0,   174,     0,   179,     0,     0,
       0,   174,     0,     0,     0,   173,     0,     0,     0,   173,
       0,     0,   174,     0,   174,     0,     0,   173,     0,     0,
     179,   176,     0,   179,     0,     0,   176,     0,   178,     0,
       0,     0,     0,     0,     0,   169,     0,   169,     0,     0,
       0,     0,   169,     0,     0,   169,   464,     0,     0,     0,
      22,    23,     0,     0,   169,     0,   169,     0,     0,   169,
     465,     0,    31,   466,     0,     0,     0,     0,    37,     0,
     174,     0,     0,   179,   174,    42,   169,   179,   179,     0,
     176,     0,   174,     0,     0,     0,   169,     0,   169,     0,
     -54,     0,   178,     0,     0,   176,     0,   178,     0,     0,
     179,    58,     0,    60,     0,   179,     0,    64,     0,   -54,
      65,     0,   467,     0,    68,   229,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   230,
     231,     0,   232,    84,     0,     0,    86,   233,     0,    88,
       0,     0,     0,     0,     0,     0,   234,     0,     0,     0,
       0,   178,   235,     0,     0,     0,   169,     0,   236,     0,
       0,     0,   237,   169,   464,   238,   178,     0,    22,    23,
       0,     0,     0,     0,     0,   239,   169,     0,   465,     0,
      31,   466,   240,   241,     0,     0,    37,   169,     0,   103,
     242,     0,     0,    42,     0,     0,   169,     0,     0,   243,
     169,   176,     0,     0,     0,     0,   179,     0,   244,   245,
       0,   246,     0,   247,   173,   248,     0,   173,   249,    58,
       0,    60,   250,   510,     0,   251,     0,   176,   252,     0,
     467,     0,    68,   176,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   176,     0,   176,     0,     0,     0,
       0,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,   173,     0,   169,     0,     0,     0,   173,     0,     0,
     179,     0,   178,     0,     0,   179,     0,     0,     0,   174,
       0,     0,   174,     0,   511,     0,     0,   139,     0,     0,
       0,   398,     0,     0,     0,   399,     0,     0,   178,     0,
       0,     0,   176,     0,   178,     0,   176,     0,     0,     0,
     139,     0,     0,   139,   176,   178,     0,   178,     0,     0,
       0,   169,   169,   169,   169,     0,   174,     0,     0,   179,
     169,   169,   174,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,   402,   179,   403,   404,   405,     0,   406,
     407,   408,     0,     0,   410,   411,   412,     0,     0,   414,
     415,   416,   417,   139,     0,   420,     0,     0,   421,   422,
     423,     0,     0,   178,     0,     0,     0,   178,     0,   424,
       0,   173,     0,   173,     0,   178,     0,     0,   173,     0,
     139,   173,     0,     0,     0,   139,     0,     0,     0,     0,
     173,     0,   173,     0,     0,   173,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   173,     0,   173,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   174,     0,   174,     0,
     179,     0,     0,   174,     0,     0,   174,     0,     0,     0,
       0,     0,     0,     0,     0,   174,     0,   174,     0,     0,
     174,     0,     0,     0,     0,     0,   179,     0,     0,     0,
       0,     0,   179,     0,     0,     0,     0,   174,     0,     0,
       0,     0,     0,   179,     0,   179,   139,   174,     0,   174,
       0,     0,   173,     0,     0,     0,     0,     0,     0,   173,
       0,   176,     0,     0,   176,     0,     0,     0,   111,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   173,     0,     0,     0,     0,     0,     0,
       0,   222,   173,     0,   227,     0,   173,     0,     0,     0,
       0,   179,     0,     0,     0,   179,     0,     0,   176,     0,
     139,     0,     0,   179,   176,   139,     0,   174,     0,     0,
       0,     0,     0,     0,   174,     0,     0,     0,     0,     0,
       0,     0,   178,     0,     0,   178,     0,   174,     0,     0,
       0,     0,     0,     0,   315,     0,   464,     0,   174,     0,
      22,    23,     0,     0,     0,  1007,     0,   174,     0,   173,
     465,   174,    31,   466,     0,     0,     0,     0,    37,   139,
       0,   111,     0,     0,     0,    42,   348,     0,     0,   178,
       0,     0,     0,     0,   139,   178,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    58,     0,    60,     0,    62,     0,  1008,     0,     0,
    1009,   281,   467,     0,    68,     0,     0,   173,   173,   173,
     173,     0,     0,     0,   174,     0,   173,   173,   176,     0,
     176,     0,     0,    84,   281,   176,    86,     0,   176,    88,
       0,     0,     0,     0,   319,   321,     0,   176,     0,   176,
       0,     0,   176,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,     0,     0,     0,     0,     0,     0,   453,     0,   176,
       0,   176,   174,   174,   174,   174,     0,     0,     0,   103,
     139,   174,   174,     0,     0,  1111,     0,     0,     0,   178,
       0,   178,     0,     0,     0,     0,   178,     0,     0,   178,
     179,     0,     0,   179,     0,     0,   139,     0,   178,     0,
     178,     0,   139,   178,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   139,     0,   139,     0,     0,     0,     0,
     178,   509,     0,     0,     0,     0,   520,     0,     0,   176,
     178,     0,   178,     0,     0,     0,   176,   179,     0,     0,
       0,     0,     0,   179,     0,     0,     0,     0,     0,   176,
       0,     0,     0,     0,     0,     0,     0,     0,   456,     0,
     176,     0,     0,     0,     0,     0,     0,     0,     0,   176,
       0,   139,     0,   176,     0,   139,     0,     0,     0,     0,
     111,     0,     0,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   111,     0,     0,     0,     0,
     178,   281,   281,   281,   281,   281,   281,   178,     0,   499,
     502,     0,     0,     0,     0,     0,   508,     0,     0,     0,
     178,     0,     0,     0,     0,   281,     0,   281,     0,     0,
       0,   178,     0,     0,   281,     0,   176,     0,     0,     0,
     178,     0,     0,     0,   178,     0,     0,     0,     0,     0,
       0,     0,   281,     0,     0,     0,     0,   179,     0,   179,
       0,     0,     0,     0,   179,   281,   281,   179,     0,     0,
       0,     0,     0,     0,     0,     0,   179,     0,   179,     0,
       0,   179,     0,     0,     0,     0,   309,     0,     0,     0,
       0,     0,     0,     0,   176,   176,   176,   176,   179,     0,
       0,   111,     0,   176,   176,     0,     0,   178,   179,     0,
     179,     0,     0,     0,   395,     0,     0,     0,     0,     0,
       0,   396,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,     0,   520,   397,   481,     0,     0,     0,     0,
       0,     0,     0,     0,   764,     0,     0,     0,     0,     0,
     482,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178,   178,   178,   178,     0,
       0,     0,     0,     0,   178,   178,     0,     0,   179,     0,
     139,     0,     0,   139,     0,   179,     0,     0,     0,     0,
       0,   398,     0,     0,     0,   399,     0,     0,   179,     0,
     281,     0,   811,     0,     0,     0,   816,     0,     0,   179,
       0,     0,     0,     0,   111,     0,     0,     0,   179,     0,
       0,     0,   179,     0,     0,     0,   400,   139,   281,     0,
       0,     0,   281,   139,   281,     0,     0,   281,     0,     0,
       0,     0,   401,   402,     0,   403,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,   412,   413,     0,   414,
     415,   416,   417,   418,   419,   420,     0,     0,   421,   422,
     423,     0,   455,   480,   483,   485,   489,   491,   494,   424,
       0,     0,     0,     0,     0,   179,     0,     0,     0,     0,
       0,     0,     0,     0,   253,   395,     0,   521,     0,   523,
       0,     0,   396,     0,     0,     0,   527,     0,   273,   274,
     275,     0,     0,     0,     0,   397,   487,     0,     0,     0,
       0,     0,     0,     0,   537,     0,     0,     0,     0,     0,
       0,   488,     0,     0,     0,     0,     0,   543,   544,     0,
       0,     0,     0,   179,   179,   179,   179,   139,     0,   139,
       0,     0,   179,   179,   139,     0,     0,   139,     0,     0,
       0,     0,     0,     0,     0,     0,   139,     0,   139,     0,
       0,   139,   398,     0,     0,     0,   399,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   139,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   139,     0,
     139,     0,     0,   484,     0,     0,     0,   400,     0,     0,
       0,   952,     0,   364,   955,   367,     0,     0,     0,     0,
       0,     0,     0,   401,   402,   387,   403,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,   412,   413,     0,
     414,   415,   416,   417,   418,   419,   420,     0,     0,   421,
     422,   423,     0,   455,     0,     0,     0,     0,   982,   449,
     424,     0,     0,     0,   987,     0,     0,     0,   139,     0,
       0,     0,   737,     0,     0,   139,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   139,     0,
       0,   281,   281,     0,     0,     0,     0,   281,   281,   139,
     752,   281,   281,     0,   757,     0,   758,     0,   139,   761,
       0,   957,   139,     0,     0,     0,     0,     0,     0,     0,
       0,   478,     0,     0,     0,     0,     0,     0,     0,   496,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     512,     0,     0,     0,     0,   979,   425,   426,   427,   428,
     429,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   139,     0,  -599,   520,     0,
     520,     0,     0,     0,     0,   520,     0,     0,   520,     0,
     440,     0,     0,  -638,     0,  -638,     0,  1069,     0,  1070,
       0,  -296,  1072,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -296,  -296,     0,  -296,  1085,
       0,     0,     0,  -296,     0,   582,     0,     0,     0,  1091,
       0,  1095,  -296,   139,   139,   139,   139,     0,  -296,     0,
       0,     0,   139,   139,  -296,     0,     0,     0,  -296,     0,
     655,  -296,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -296,     0,     0,   281,   281,     0,     0,  -296,  -296,
       0,   281,     0,   502,     0,     0,  -296,     0,     0,     0,
     502,     0,     0,   281,     0,  -296,   281,     0,   281,     0,
     281,     0,     0,     0,  -296,  -296,     0,  -296,     0,  -296,
       0,  -296,     0,  1077,  -296,     0,     0,     0,  -296,     0,
       0,  -296,     0,     0,  -296,   725,     0,     0,     0,  1161,
       0,     0,     0,  1089,     0,     0,     0,     0,   464,   253,
    1167,     0,    22,    23,     0,     0,     0,  1007,     0,  1172,
       0,     0,   465,  1174,    31,   466,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,     0,    42,     0,     0,
       0,     0,   229,   933,   935,     0,     0,     0,     0,   940,
     943,   773,     0,   945,   947,   512,   230,   231,     0,   232,
       0,     0,     0,    58,   233,    60,     0,    62,   792,  1008,
       0,     0,  1009,   234,   467,     0,    68,     0,     0,   235,
       0,     0,     0,     0,     0,   236,  1222,     0,     0,   237,
       0,     0,   238,     0,     0,    84,     0,     0,    86,     0,
       0,    88,   239,     0,     0,     0,     0,     0,   281,   240,
     241,     0,     0,     0,     0,     0,     0,   242,   833,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,     0,
       0,   367,     0,     0,     0,   244,   245,     0,   246,     0,
     247,     0,   248,     0,     0,   249,   866,     0,     0,   250,
       0,   103,   251,     0,     0,   252,     0,  1183,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   502,   502,     0,     0,
     502,   502,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1226,     0,   502,     0,   502,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   387,   455,
       0,   978,     0,     0,     0,     0,  1050,  1051,     0,     0,
       0,     0,     5,  1052,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,  1063,    15,    16,  1066,    17,
    1067,   190,  1068,     0,    21,   354,    23,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,    60,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,    68,
       0,     0,     0,     0,   958,    73,    74,    75,    76,    77,
     197,   512,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,    88,    89,     0,   969,     0,     0,
       0,     0,     0,    92,     0,     0,     0,   980,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
    1168,   100,   101,   102,     0,     0,     0,   104,     0,   198,
       0,   106,     0,   199,     0,     0,   108,     0,     0,     0,
     367,     0,     0,   367,     0,     0,  1016,     0,     0,   512,
       0,    -2,     4,     0,     5,     0,     6,     7,     8,     9,
      10,    11,   655,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -54,     0,
      40,    41,    42,     0,    43,  -321,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -54,    53,    54,
       0,    55,    56,    57,     0,  -321,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -321,   -54,    65,    66,    67,
       0,    68,    69,    70,   773,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,   980,    86,    87,   -54,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,  -669,     0,
      12,    13,    14,    15,    16,  -669,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,  -669,    28,
      29,  -669,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -54,     0,    40,    41,    42,     0,    43,
    -321,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -54,    53,    54,     0,    55,    56,    57,     0,
    -321,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -321,   -54,    65,    66,    67,  -669,    68,    69,    70,  -669,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -54,    88,    89,     0,     0,    90,     0,    91,     0,     0,
    -669,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -669,  -669,    95,  -669,
    -669,  -669,  -669,  -669,  -669,  -669,     0,  -669,  -669,  -669,
    -669,  -669,     0,  -669,  -669,  -669,  -669,  -669,  -669,  -669,
    -669,   103,  -669,  -669,  -669,     0,   105,  -669,   106,     0,
     107,     0,   331,  -669,     5,   299,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -54,     0,
      40,    41,    42,     0,    43,  -321,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -54,    53,    54,
       0,    55,    56,    57,     0,  -321,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -321,   -54,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -54,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,   332,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -54,     0,    40,    41,    42,     0,    43,
    -321,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -54,    53,    54,     0,    55,    56,    57,     0,
    -321,     0,     0,    58,    59,    60,    61,    62,    63,    64,
    -321,   -54,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -54,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,   547,   106,     0,
     107,     0,   566,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -54,     0,
      40,    41,    42,     0,    43,  -321,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -54,    53,    54,
       0,    55,    56,    57,     0,  -321,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -321,   -54,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -54,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,   567,   106,     0,   107,     0,   331,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -54,     0,    40,    41,    42,     0,    43,
    -321,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -54,    53,    54,     0,    55,    56,    57,     0,
    -321,     0,     0,    58,    59,    60,    61,    62,    63,    64,
    -321,   -54,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -54,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,   332,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -54,     0,
      40,    41,    42,     0,    43,  -321,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -54,    53,    54,
       0,    55,    56,    57,     0,  -321,     0,     0,    58,    59,
      60,    61,    62,    63,    64,  -321,   -54,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -54,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,   823,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -54,     0,    40,    41,    42,     0,    43,
    -321,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -54,    53,    54,     0,    55,    56,    57,     0,
    -321,     0,     0,    58,    59,    60,    61,   347,    63,    64,
    -321,   -54,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -54,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,    38,    39,   -54,     0,
      40,    41,    42,     0,    43,  -321,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -54,    53,    54,
       0,    55,    56,    57,     0,  -321,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -321,   -54,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -54,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,   747,    39,   -54,     0,    40,    41,    42,     0,    43,
    -321,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -54,    53,    54,     0,    55,    56,    57,     0,
    -321,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -321,   -54,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -54,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,   753,    39,   -54,     0,
      40,    41,    42,     0,    43,  -321,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -54,    53,    54,
       0,    55,    56,    57,     0,  -321,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -321,   -54,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -54,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1054,    39,   -54,     0,    40,    41,    42,     0,    43,
    -321,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -54,    53,    54,     0,    55,    56,    57,     0,
    -321,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -321,   -54,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -54,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1056,    39,   -54,     0,
      40,    41,    42,     0,    43,  -321,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -54,    53,    54,
       0,    55,    56,    57,     0,  -321,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -321,   -54,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -54,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1061,    39,   -54,     0,    40,    41,    42,     0,    43,
    -321,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -54,    53,    54,     0,    55,    56,    57,     0,
    -321,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -321,   -54,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -54,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1064,    39,   -54,     0,
      40,    41,    42,     0,    43,  -321,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -54,    53,    54,
       0,    55,    56,    57,     0,  -321,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -321,   -54,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -54,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,  1084,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -54,     0,    40,    41,    42,     0,    43,
    -321,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -54,    53,    54,     0,    55,    56,    57,     0,
    -321,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -321,   -54,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -54,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,     4,   108,     5,     0,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,    13,    14,    15,    16,
       0,    17,     0,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,     0,    28,    29,     0,    30,     0,    31,
      32,    33,    34,    35,    36,    37,  1090,    39,   -54,     0,
      40,    41,    42,     0,    43,  -321,     0,    44,    45,    46,
      47,    48,     0,    49,    50,    51,    52,   -54,    53,    54,
       0,    55,    56,    57,     0,  -321,     0,     0,    58,    59,
      60,    61,     0,    63,    64,  -321,   -54,    65,    66,    67,
       0,    68,    69,    70,     0,    71,    72,    73,    74,    75,
      76,    77,    78,     0,    79,    80,     0,    81,    82,    83,
      84,    85,     0,    86,    87,   -54,    88,    89,     0,     0,
      90,     0,    91,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,   103,     0,     0,   104,
       0,   105,     0,   106,     0,   107,     0,     4,   108,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,    13,    14,    15,    16,     0,    17,     0,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,  1094,    39,   -54,     0,    40,    41,    42,     0,    43,
    -321,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -54,    53,    54,     0,    55,    56,    57,     0,
    -321,     0,     0,    58,    59,    60,    61,     0,    63,    64,
    -321,   -54,    65,    66,    67,     0,    68,    69,    70,     0,
      71,    72,    73,    74,    75,    76,    77,    78,     0,    79,
      80,     0,    81,    82,    83,    84,    85,     0,    86,    87,
     -54,    88,    89,     0,     0,    90,     0,    91,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,   103,     0,     0,   104,     0,   105,     0,   106,     0,
     107,     0,  1024,   108,     5,   299,     6,     7,     8,     9,
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
       0,   198,     0,   106,     0,   199,  1025,   965,   108,     5,
     299,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
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
       0,   612,     0,    12,     0,     0,    15,    16,   614,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   191,     0,     0,     0,    33,
     192,   193,     0,     0,   194,    39,     0,     0,     0,    41,
       0,     0,    43,     0,   620,   195,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   196,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     197,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,   501,   426,   427,   428,   429,   430,
       0,     0,   433,   434,   435,   436,     0,   438,   439,   834,
     835,   836,   837,   838,   635,     0,   636,     0,    98,     0,
     637,   638,   639,   640,   641,   642,   643,   644,   839,   646,
     647,   100,   840,   102,     0,   649,   650,   841,   652,   198,
       0,   106,     0,   199,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,     0,    15,
      16,     0,    17,     0,   190,    19,    20,    21,     0,     0,
       0,     0,    26,     0,     0,    28,    29,     0,   191,     0,
       0,     0,    33,    34,    35,    36,     0,    38,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,    49,    50,    51,    52,     0,    53,
      54,     0,    55,    56,    57,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,    90,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   105,     0,   106,     0,   107,     0,     0,   108,
       5,   299,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,   303,   304,     0,    85,   336,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,   337,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   199,     0,     0,   108,     5,   299,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,   303,
     304,     0,    85,   336,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   198,     0,   106,   807,   199,     0,     0,   108,
       5,   299,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,   303,   304,     0,    85,   336,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
     809,   199,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,   726,     0,     0,
       0,     0,     0,     0,    29,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,   194,    39,     0,   727,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
     728,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,   729,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     198,     0,   106,     0,   199,     0,     5,   108,     6,     7,
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
     303,   304,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,   305,     0,
       0,   104,     0,   198,     0,   106,     0,   199,     0,     0,
     108,     5,   299,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,   303,   304,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
     967,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,     0,    81,   303,   304,
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
     191,     0,     0,     0,    33,   192,   193,     0,     0,   194,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,   257,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,   258,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   198,     0,   106,     0,   199,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,   265,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,   266,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   199,     0,     0,   108,     5,   299,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   190,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,   258,     0,    92,     0,     0,     0,
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
       0,    81,   303,   304,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   198,     0,   106,     0,   199,
       0,     0,   108,     5,   299,     6,     7,     8,     9,    10,
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
     198,   805,   106,     0,   199,     0,     0,   108,     5,   299,
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
     815,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,  -713,     0,     0,
       0,  -713,     0,     0,    73,    74,    75,    76,    77,   197,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,   199,     0,     0,   108,     5,   299,     6,     7,
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
       0,   104,     0,   198,     0,   106,     0,   199,  1190,     5,
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
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,   258,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   198,     0,   106,     0,
     199,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,    12,     0,     0,    15,    16,     0,    17,
       0,   190,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,   280,    29,     0,   191,     0,     0,     0,    33,
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
       0,   100,   101,   102,     0,     0,     0,   104,     0,   105,
       0,   106,     0,   199,     0,     0,   108,     5,   299,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   190,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
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
       0,     0,   104,     0,   198,     0,   106,     0,   199,     0,
       5,   108,     6,     7,     8,   357,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,    18,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,    67,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   197,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,    86,
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
     198,     0,   106,     0,   199,   820,     5,   108,     6,     7,
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
       0,     0,     0,    85,   994,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   198,     0,   106,     0,   199,     0,     0,
     108,     5,   299,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,   779,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,   194,    39,     0,     0,     0,   780,     0,
       0,    43,     0,     0,   195,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   196,     0,     0,     0,
       0,     0,     0,     0,    73,   781,    75,    76,    77,   782,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   198,     0,
     106,     0,  1119,     0,     5,   108,     6,     7,     8,     9,
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
       0,    85,  1176,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   198,     0,   106,     0,   199,     0,     0,   108,     5,
     299,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
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
    1119,     0,     0,   108,     5,   299,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,  1150,    39,     0,     0,
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
       0,     0,   104,     0,   198,     0,   106,     0,   199,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,     0,     0,    15,    16,     0,    17,     0,   190,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,   779,     0,   191,     0,     0,     0,    33,   192,   193,
       0,     0,   194,    39,     0,     0,     0,   780,     0,     0,
      43,     0,     0,   195,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   196,     0,     0,     0,     0,
       0,     0,     0,    73,   781,    75,    76,    77,   782,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   198,     0,   106,
       0,   783,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,    12,     0,     0,    15,    16,     0,
      17,     0,   190,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,   779,     0,   191,     0,     0,     0,
      33,   192,   193,     0,     0,   194,    39,     0,     0,     0,
     780,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,   781,    75,    76,
      77,   782,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     198,     0,   106,     0,   868,     0,     5,   108,     6,     7,
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
       0,   104,     0,   198,     0,   106,     0,   783,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   190,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   894,    39,     0,     0,     0,    41,     0,     0,    43,
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
     192,   193,     0,     0,   897,    39,     0,     0,     0,    41,
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
       0,     0,    33,   192,   193,     0,     0,  1144,    39,     0,
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
    1145,    39,     0,     0,     0,    41,     0,     0,    43,     0,
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
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,    12,     0,     0,    15,    16,     0,    17,     0,
     190,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   191,     0,     0,     0,    33,   192,
     193,     0,     0,  1147,    39,     0,     0,     0,    41,     0,
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
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,  1148,    39,     0,     0,
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
     191,     0,     0,     0,    33,   192,   193,     0,     0,  1149,
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
       0,     0,  1150,    39,     0,     0,     0,    41,     0,     0,
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
      33,   192,   193,     0,     0,   194,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   195,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   196,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   197,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   610,     0,   611,     0,     0,     0,
      93,    94,    95,    96,    97,   612,     0,     0,     0,    98,
     613,   231,   614,   615,     0,     0,     0,     0,   616,    99,
       0,     0,   100,   101,   102,     0,     0,   234,   104,   191,
       0,     0,   106,   617,   868,     0,     0,   108,     0,   618,
       0,     0,     0,   237,     0,     0,   619,     0,   620,     0,
       0,     0,     0,     0,     0,     0,   621,     0,     0,     0,
       0,     0,     0,   622,   623,     0,     0,     0,     0,     0,
       0,   242,     0,     0,     0,     0,     0,     0,     0,     0,
     624,     0,     0,     0,     0,     0,     0,     0,     0,   244,
     245,     0,   625,     0,   247,     0,   626,     0,     0,   627,
       0,     0,     0,   628,     0,     0,   251,     0,     0,   629,
       0,     0,     0,     0,     0,     0,     0,     0,   501,   426,
     427,   428,   429,   430,     0,     0,   433,   434,   435,   436,
       0,   438,   439,   630,   631,   632,   633,   634,   635,     0,
     636,     0,     0,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,   647,     0,   648,     0,     0,   649,
     650,   651,   652,     0,     5,   653,     6,     7,     8,     9,
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
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,     0,   101,   102,     0,     0,     0,   104,
       0,   198,     0,   106,     5,   199,     6,     7,     8,     9,
      10,    11,     0,     0,     0,    12,     0,     0,    15,    16,
       0,    17,     0,   190,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   191,     0,     0,
       0,    33,   192,   193,     0,     0,   194,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   195,     0,     0,
      47,    48,  -478,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   196,
       0,     0,     0,  -478,     0,     0,     0,    73,    74,    75,
      76,    77,   197,     0,    79,     0,  -478,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,  -478,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     5,   299,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,     0,     0,    15,
      16,     0,    17,     0,   190,   102,  -478,    21,     0,     0,
       0,  -478,     0,   106,     0,   868,    29,     0,   191,     0,
       0,     0,    33,   192,   193,     0,     0,   194,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   195,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     196,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   197,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,     0,  -496,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,     0,     0,  -496,     0,
       0,     0,  -496,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   102,     0,     0,     0,
       0,     0,   198,     0,   106,  -496,  1119,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,    12,     0,
       0,    15,    16,     0,    17,     0,   190,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     191,     0,     0,     0,    33,   192,   193,     0,     0,   194,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     195,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   196,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   197,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,     0,    95,     0,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,    12,
       0,     0,    15,    16,     0,    17,     0,   190,   102,     0,
      21,     0,     0,     0,   198,     0,   106,     0,   199,    29,
       0,   191,     0,     0,     0,    33,   192,   193,     0,     0,
     194,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   195,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   196,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   197,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,     0,     5,
       0,     6,     7,     8,     9,    10,    11,     0,     0,     0,
      12,     0,     0,    15,    16,     0,    17,     0,   190,   102,
       0,    21,     0,     0,     0,   198,     0,   106,     0,   783,
      29,     0,   191,     0,     0,     0,    33,   192,   193,     0,
       0,   194,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   195,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   196,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   197,     0,    79,
       0,     0,    81,     0,   229,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,   230,   231,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,   234,     0,     0,    95,     0,
       0,   235,     0,     0,     0,     0,     0,   236,     0,     0,
       0,   237,     0,     0,   238,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   239,     0,     0,     0,   106,     0,
     199,   240,   241,     0,     0,     0,     0,     0,     0,   242,
       0,     0,     0,     0,     0,     0,     0,     0,   243,     0,
       0,   229,     0,     0,     0,     0,     0,   244,   245,     0,
     246,     0,   247,     0,   248,   230,   231,   249,   232,     0,
       0,   250,     0,   233,   251,     0,     0,   252,     0,     0,
       0,     0,   234,     0,     0,     0,   681,     0,   235,     0,
       0,     0,     0,     0,   236,   612,     0,     0,   237,     0,
       0,   238,   614,   682,     0,     0,     0,     0,     0,     0,
       0,   239,     0,     0,     0,     0,     0,   683,   240,   241,
       0,     0,     0,     0,     0,     0,   242,     0,     0,     0,
       0,     0,     0,   276,     0,   243,     0,     0,   620,     0,
       0,     0,     0,     0,   244,   245,   684,   246,     0,   247,
       0,   248,     0,     0,   249,     0,     0,     0,   250,     0,
       0,   251,     0,     0,   252,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   685,     0,     0,     0,
       0,     0,   686,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   501,   426,
     427,   428,   429,   430,     0,     0,   433,   434,   435,   436,
     865,   438,   439,   630,   631,   632,   633,   634,   635,     0,
     636,     0,     0,     0,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,   647,  1030,   648,     0,     0,   649,
     650,   651,   652,   991,   612,     0,     0,     0,     0,   230,
     231,   614,   232,     0,     0,     0,     0,   233,     0,     0,
       0,     0,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,   617,     0,     0,     0,     0,     0,   236,     0,
       0,     0,   237,     0,     0,   238,     0,   620,     0,     0,
       0,     0,     0,     0,     0,   239,     0,     0,     0,     0,
       0,     0,   622,   241,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,   243,
       0,     0,     0,     0,     0,     0,     0,   395,   244,   245,
       0,   246,     0,   247,   396,  1031,     0,     0,   627,     0,
       0,     0,   250,     0,     0,   251,     0,   397,   252,     0,
       0,     0,     0,     0,     0,     0,     0,   501,   426,   427,
     428,   429,   430,     0,     0,   433,   434,   435,   436,     0,
     438,   439,   630,   631,   632,   633,   634,   635,     0,   636,
       0,     0,     0,   637,   638,   639,   640,   641,   642,   643,
     644,   645,   646,   647,     0,   648,     0,     0,   649,   650,
     651,   652,     0,   395,   398,     0,     0,     0,   399,     0,
     396,     0,     0,     0,     0,     0,     0,   500,     0,     0,
       0,     0,     0,   397,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     501,   426,   427,   428,   429,   430,     0,     0,   433,   434,
     435,   436,     0,   438,   439,   401,   402,     0,   403,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,   412,
     413,     0,   414,   415,   416,   417,   418,   419,   420,   395,
     398,   421,   422,   423,   399,   455,   396,     0,     0,     0,
       0,     0,   424,  1053,     0,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,   501,   426,   427,   428,
     429,   430,     0,     0,   433,   434,   435,   436,     0,   438,
     439,   401,   402,     0,   403,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,   395,   398,   421,   422,   423,
     399,   455,   396,     0,     0,     0,     0,     0,   424,  1060,
       0,     0,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,   501,   426,   427,   428,   429,   430,     0,     0,
     433,   434,   435,   436,     0,   438,   439,   401,   402,     0,
     403,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,   395,   398,   421,   422,   423,   399,   455,   396,     0,
       0,     0,     0,     0,   424,  1218,     0,     0,     0,     0,
       0,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,   501,   426,
     427,   428,   429,   430,     0,     0,   433,   434,   435,   436,
       0,   438,   439,   401,   402,     0,   403,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,   412,   413,     0,
     414,   415,   416,   417,   418,   419,   420,   395,   398,   421,
     422,   423,   399,   455,   396,     0,     0,     0,     0,     0,
     424,  1219,     0,     0,     0,     0,     0,   397,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   400,   501,   426,   427,   428,   429,   430,
       0,     0,   433,   434,   435,   436,     0,   438,   439,   401,
     402,     0,   403,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,   412,   413,     0,   414,   415,   416,   417,
     418,   419,   420,   395,   398,   421,   422,   423,   399,   455,
     396,     0,     0,     0,     0,     0,   424,  1220,     0,     0,
       0,     0,     0,   397,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     501,   426,   427,   428,   429,   430,     0,     0,   433,   434,
     435,   436,     0,   438,   439,   401,   402,     0,   403,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,   412,
     413,     0,   414,   415,   416,   417,   418,   419,   420,   395,
     398,   421,   422,   423,   399,   455,   396,     0,     0,     0,
       0,     0,   424,  1221,     0,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   400,   501,   426,   427,   428,
     429,   430,     0,     0,   433,   434,   435,   436,     0,   438,
     439,   401,   402,     0,   403,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,   395,   398,   421,   422,   423,
     399,   455,   396,     0,     0,     0,     0,     0,   424,  1227,
       0,     0,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,   501,   426,   427,   428,   429,   430,     0,     0,
     433,   434,   435,   436,     0,   438,   439,   401,   402,     0,
     403,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,     0,   398,   421,   422,   423,   399,   455,     0,     0,
       0,     0,     0,     0,   424,  1228,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   400,   501,   426,
     427,   428,   429,   430,     0,     0,   433,   434,   435,   436,
       0,   438,   439,   401,   402,     0,   403,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,   412,   413,     0,
     414,   415,   416,   417,   418,   419,   420,   395,     0,   421,
     422,   423,     0,   455,   396,     0,     0,     0,     0,     0,
     424,     0,     0,     0,     0,     0,     0,   397,   280,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   493,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   397,     0,     0,
       0,     0,   575,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   576,     0,
       0,     0,     0,     0,     0,   484,     0,     0,     0,   400,
       0,     0,     0,     0,   398,     0,     0,     0,   399,     0,
       0,     0,     0,     0,     0,   401,   402,     0,   403,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,   412,
     413,     0,   414,   415,   416,   417,   418,   419,   420,   400,
       0,   421,   422,   423,     0,   455,     0,     0,     0,     0,
       0,     0,   424,     0,     0,   401,   402,     0,   403,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,   412,
     413,     0,   414,   415,   416,   417,   418,   419,   420,     0,
     577,   421,   422,   423,   395,   830,     0,     0,     0,     0,
       0,   396,   424,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   397,     0,     0,     0,     0,   831,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,   832,     0,     0,     0,   397,
     942,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,   399,     0,     0,     0,     0,
       0,     0,     0,     0,   395,     0,     0,     0,     0,     0,
       0,   396,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   397,   280,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,     0,
     399,     0,   401,   402,     0,   403,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,   412,   413,     0,   414,
     415,   416,   417,   418,   419,   420,     0,   484,   421,   422,
     423,   400,     0,     0,     0,     0,     0,     0,     0,   424,
       0,   398,     0,     0,     0,   399,     0,   401,   402,     0,
     403,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,     0,   484,   421,   422,   423,   400,   455,     0,     0,
       0,     0,     0,     0,   424,     0,     0,     0,     0,     0,
       0,     0,   401,   402,     0,   403,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,   412,   413,     0,   414,
     415,   416,   417,   418,   419,   420,   395,     0,   421,   422,
     423,     0,   455,   396,     0,     0,     0,     0,     0,   424,
       0,     0,     0,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   457,     0,     0,     0,   395,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,   280,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,   395,   399,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,   717,     0,     0,
       0,     0,     0,     0,   210,     0,     0,     0,   400,     0,
       0,     0,   718,   398,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,   401,   402,     0,   403,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,   412,   413,
       0,   414,   415,   416,   417,   418,   419,   420,   400,     0,
     421,   422,   423,   398,     0,     0,     0,   399,     0,     0,
       0,   424,     0,     0,   401,   402,     0,   403,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,   412,   413,
       0,   414,   415,   416,   417,   418,   419,   420,   400,     0,
     421,   422,   423,     0,   455,     0,     0,     0,     0,     0,
       0,   424,     0,     0,   401,   402,     0,   403,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,   412,   413,
       0,   414,   415,   416,   417,   418,   419,   420,   395,     0,
     421,   422,   423,     0,     0,   396,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,     0,   397,   719,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   720,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   397,   934,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,   395,   399,
       0,     0,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,   398,     0,     0,     0,   399,
       0,     0,     0,     0,     0,     0,   401,   402,     0,   403,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
     400,     0,   421,   422,   423,   398,     0,     0,     0,   399,
       0,     0,     0,   424,     0,     0,   401,   402,   956,   403,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
     400,     0,   421,   422,   423,     0,   455,     0,     0,     0,
       0,     0,     0,   424,     0,     0,   401,   402,     0,   403,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
     395,     0,   421,   422,   423,     0,   455,   396,     0,     0,
       0,     0,     0,   424,     0,     0,     0,     0,     0,     0,
     397,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     395,     0,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     397,     0,     0,     0,     0,  1103,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,   399,     0,     0,     0,   395,   524,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,     0,
       0,  1104,     0,     0,     0,   397,     0,     0,   484,     0,
       0,     0,   400,     0,     0,     0,     0,   398,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,   401,   402,
       0,   403,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,   400,     0,   421,   422,   423,     0,     0,     0,
       0,     0,   398,   985,     0,   424,   399,     0,   401,   402,
       0,   403,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,   395,     0,   421,   422,   423,   400,     0,   396,
       0,     0,     0,     0,     0,   424,     0,     0,     0,     0,
       0,     0,   397,   401,   402,     0,   403,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,   412,   413,     0,
     414,   415,   416,   417,   418,   419,   420,   395,     0,   421,
     422,   423,     0,     0,   396,     0,     0,     0,     0,     0,
     424,     0,     0,     0,     0,     0,     0,   397,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,   399,     0,     0,     0,   395,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   397,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,   399,     0,
     401,   402,     0,   403,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,     0,   421,   422,   423,   400,
     536,     0,     0,     0,   398,     0,     0,   424,   399,     0,
       0,     0,     0,     0,     0,   401,   402,     0,   403,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,   412,
     413,     0,   414,   415,   416,   417,   418,   419,   420,   400,
     540,   421,   422,   423,     0,     0,     0,     0,     0,     0,
       0,     0,   424,     0,     0,   401,   402,     0,   403,   404,
     405,     0,   406,   407,   408,     0,   409,   410,   411,   412,
     413,     0,   414,   415,   416,   417,   418,   419,   420,   395,
     545,   421,   422,   423,     0,     0,   396,     0,     0,     0,
       0,     0,   424,     0,     0,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   395,
       0,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   398,     0,     0,   395,
     399,     0,     0,     0,     0,     0,   396,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     562,   400,     0,     0,     0,     0,   398,     0,     0,     0,
     399,     0,     0,     0,     0,     0,     0,   401,   402,   721,
     403,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,   400,     0,   421,   422,   423,   398,     0,     0,     0,
     399,     0,     0,     0,   424,     0,     0,   401,   402,     0,
     403,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,   400,     0,   421,   422,   423,     0,     0,     0,     0,
       0,     0,     0,     0,   424,     0,     0,   401,   402,     0,
     403,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,   395,   826,   421,   422,   423,     0,     0,   396,     0,
     806,     0,     0,     0,   424,     0,     0,     0,     0,     0,
       0,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   398,     0,
       0,     0,   399,     0,     0,     0,   395,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,   400,     0,     0,     0,     0,   398,     0,
       0,     0,   399,     0,     0,     0,     0,     0,     0,   401,
     402,     0,   403,   404,   405,     0,   406,   827,   408,     0,
     409,   410,   411,   412,   413,     0,   414,   415,   416,   417,
     418,   419,   420,   400,     0,   421,   422,   423,     0,     0,
       0,     0,     0,   398,     0,     0,   424,   399,     0,   401,
     402,     0,   403,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,   412,   413,  -600,   414,   415,   416,   417,
     418,   419,   420,   395,   210,   421,   422,   423,   400,     0,
     396,     0,     0,     0,     0,     0,   424,     0,     0,     0,
       0,     0,     0,   397,   401,   402,     0,   403,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,   412,   413,
       0,   414,   415,   416,   417,   418,   419,   420,   395,     0,
     421,   422,   423,     0,     0,   396,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,     0,   397,     0,
       0,   954,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,   399,     0,     0,     0,   395,     0,
       0,     0,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   397,     0,
       0,     0,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,   399,
       0,   401,   402,     0,   403,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,   412,   413,     0,   414,   415,
     416,   417,   418,   419,   420,     0,   925,   421,   422,   423,
     400,     0,     0,     0,     0,   398,     0,     0,   424,   399,
       0,     0,     0,     0,     0,     0,   401,   402,     0,   403,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
     400,     0,   421,   422,   423,     0,     0,     0,     0,     0,
       0,     0,     0,   424,     0,     0,   401,   402,     0,   403,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
     395,     0,   421,   422,   423,     0,     0,   396,     0,   970,
       0,     0,     0,   424,     0,     0,     0,     0,     0,     0,
     397,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   395,     0,     0,     0,     0,     0,     0,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,   399,   395,  1000,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,   398,     0,
       0,     0,   399,     0,     0,     0,     0,     0,   401,   402,
       0,   403,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,   988,   400,   421,   422,   423,     0,     0,   398,
       0,   971,     0,   399,     0,   424,     0,     0,     0,   401,
     402,     0,   403,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,   412,   413,     0,   414,   415,   416,   417,
     418,   419,   420,     0,   400,   421,   422,   423,     0,     0,
     395,     0,     0,     0,     0,     0,   424,   396,     0,     0,
     401,   402,     0,   403,   404,   405,     0,   406,   407,   408,
     397,   409,   410,   411,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,     0,   421,   422,   423,     0,
       0,   395,     0,     0,     0,     0,     0,   424,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,  1038,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,   399,   395,     0,     0,     0,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   397,  1041,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,   398,     0,
       0,     0,   399,     0,     0,     0,     0,     0,   401,   402,
       0,   403,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,     0,   400,   421,   422,   423,     0,     0,   398,
       0,  1029,     0,   399,     0,   424,     0,     0,     0,   401,
     402,     0,   403,   404,   405,     0,   406,   407,   408,     0,
     409,   410,   411,   412,   413,     0,   414,   415,   416,   417,
     418,   419,   420,     0,   400,   421,   422,   423,     0,     0,
     395,     0,     0,     0,     0,     0,   424,   396,     0,     0,
     401,   402,     0,   403,   404,   405,     0,   406,   407,   408,
     397,   409,   410,   411,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,     0,     0,   421,   422,   423,     0,
       0,   395,  1100,     0,     0,     0,     0,   424,   396,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,   399,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   397,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,   398,     0,
       0,     0,   399,     0,     0,     0,     0,     0,   401,   402,
       0,   403,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,     0,   400,   421,   422,   423,     0,     0,   395,
     398,     0,     0,  1044,   399,   424,   396,     0,     0,   401,
     402,     0,   403,   404,   405,     0,   406,   407,   408,   397,
     409,   410,   411,   412,   413,     0,   414,   415,   416,   417,
     418,   419,   420,     0,     0,   421,   422,   423,     0,     0,
       0,     0,     0,     0,     0,     0,   424,     0,     0,     0,
       0,   401,   402,     0,   403,   404,   405,     0,   406,   407,
     408,     0,   409,   410,   411,   412,     0,     0,   414,   415,
     416,   417,   418,     0,   420,   395,   398,   421,   422,   423,
     399,     0,   396,     0,     0,     0,     0,     0,   424,  1143,
       0,     0,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   400,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,   402,     0,
     403,   404,   405,     0,   406,   407,   408,     0,   409,   410,
     411,   412,   413,     0,   414,   415,   416,   417,   418,   419,
     420,     0,   398,   421,   422,   423,   399,     0,     0,     0,
     395,  1182,     0,     0,   424,  1146,     0,   396,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     397,     0,     0,     0,     0,     0,     0,   400,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,   402,     0,   403,   404,   405,     0,
     406,   407,   408,     0,   409,   410,   411,   412,   413,     0,
     414,   415,   416,   417,   418,   419,   420,     0,     0,   421,
     422,   423,     0,     0,     0,     0,   395,   398,     0,     0,
     424,   399,     0,   396,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1203,     0,     0,   401,   402,
       0,   403,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,   395,   398,   421,   422,   423,   399,     0,   396,
       0,     0,     0,     0,     0,   424,     0,     0,     0,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,   402,     0,   403,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,   412,   413,
       0,   414,   415,   416,   417,   418,   419,   420,   395,   398,
     421,   422,   423,   399,     0,   396,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,     0,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     401,   402,     0,   403,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,   395,   398,   421,   422,   423,   399,
       0,   396,     0,     0,     0,  1223,     0,   424,  1252,     0,
       0,     0,     0,     0,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,   402,     0,   403,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
     395,   398,   421,   422,   423,   399,     0,   396,     0,     0,
       0,     0,     0,   424,  1253,     0,     0,     0,     0,     0,
     397,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,   402,     0,   403,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,   412,   413,     0,   414,
     415,   416,   417,   418,   419,   420,   395,   398,   421,   422,
     423,   399,     0,   396,     0,     0,     0,     0,     0,   424,
    1254,     0,     0,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   401,   402,
       0,   403,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,   395,   398,   421,   422,   423,   399,     0,   396,
       0,     0,     0,     0,     0,   424,  1255,     0,     0,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,   402,     0,   403,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,   412,   413,
       0,   414,   415,   416,   417,   418,   419,   420,   395,   398,
     421,   422,   423,   399,     0,   396,     0,     0,     0,     0,
       0,   424,  1256,     0,     0,     0,     0,     0,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     401,   402,     0,   403,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,   412,   413,     0,   414,   415,   416,
     417,   418,   419,   420,   395,   398,   421,   422,   423,   399,
       0,   396,     0,     0,     0,     0,     0,   424,  1257,     0,
       0,     0,     0,     0,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,   402,     0,   403,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
     412,   413,     0,   414,   415,   416,   417,   418,   419,   420,
     395,   398,   421,   422,   423,   399,     0,   396,     0,     0,
       0,     0,     0,   424,     0,     0,     0,     0,     0,     0,
     397,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,   402,     0,   403,   404,   405,     0,   406,
     407,   408,     0,   409,   410,   411,   412,   413,     0,   414,
     415,   416,   417,   418,   419,   420,   395,   398,   421,   422,
     423,   399,     0,   396,     0,     0,     0,  1270,     0,   424,
       0,     0,     0,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   401,   402,
       0,   403,   404,   405,     0,   406,   407,   408,     0,   409,
     410,   411,   412,   413,     0,   414,   415,   416,   417,   418,
     419,   420,     0,   920,   421,   422,   423,   399,     0,     0,
       0,     0,     0,     0,     0,   424,     0,     0,     0,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,   402,     0,   403,   404,   405,
       0,   406,   407,   408,     0,   409,   410,   411,   412,   413,
       0,   414,   415,   416,   417,   418,   419,   420,     0,   398,
     421,   422,   423,   399,     0,     0,     0,     0,     0,     0,
       0,   424,     0,     0,     0,     0,     0,     0,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     401,   402,     0,   403,   404,   405,     0,   406,   407,   408,
       0,   409,   410,   411,   412,     0,     0,   414,   415,   416,
     417,   418,   419,   420,     0,   398,   421,   422,   423,   399,
       0,     0,     0,     0,     0,     0,     0,   424,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   401,   402,     0,   403,
     404,   405,     0,   406,   407,   408,     0,   409,   410,   411,
     412,     0,     0,   414,   415,   416,   417,   418,   365,   420,
     229,     0,   421,   422,   423,     0,     0,     0,     0,     0,
       0,     0,     0,   424,   230,   231,     0,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,   366,     0,
       0,   234,     0,     0,     0,     0,     0,   235,     0,     0,
       0,     0,     0,   236,     0,     0,     0,   237,     0,     0,
     238,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     239,     0,     0,     0,     0,     0,     0,   240,   241,     0,
       0,     0,     0,     0,     0,   242,     0,     0,     0,     0,
       0,     0,     0,     0,   243,     0,     0,   229,     0,     0,
       0,     0,     0,   244,   245,     0,   246,     0,   247,     0,
     248,   230,   231,   249,   232,     0,     0,   250,     0,   233,
     251,    23,     0,   252,     0,     0,     0,     0,   234,     0,
       0,     0,     0,     0,   235,     0,     0,     0,     0,     0,
     236,     0,     0,     0,   237,     0,     0,   238,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   239,     0,     0,
       0,     0,     0,     0,   240,   241,     0,     0,     0,     0,
       0,     0,   242,    60,     0,     0,     0,     0,     0,     0,
       0,   243,     0,     0,    68,     0,     0,     0,     0,     0,
     244,   245,     0,   246,     0,   247,     0,   248,   229,     0,
     249,     0,     0,     0,   250,     0,     0,   251,     0,    88,
     252,     0,   230,   231,     0,   232,     0,     0,     0,     0,
     233,     0,     0,     0,     0,     0,   366,     0,     0,   234,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,   236,     0,     0,     0,   237,     0,     0,   238,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   239,     0,
       0,     0,     0,     0,     0,   240,   241,     0,     0,     0,
       0,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,   243,     0,     0,   229,     0,     0,     0,     0,
       0,   244,   245,     0,   246,     0,   247,     0,   248,   230,
     231,   249,   232,     0,     0,   250,     0,   233,   251,     0,
       0,   252,     0,     0,     0,     0,   234,     0,     0,     0,
       0,     0,   235,     0,     0,     0,     0,     0,   236,     0,
       0,     0,   237,     0,     0,   238,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   239,     0,     0,     0,     0,
       0,     0,   240,   241,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,     0,     0,     0,   243,
       0,     0,  -297,     0,     0,     0,     0,     0,   244,   245,
       0,   246,     0,   247,     0,   248,  -297,  -297,   249,  -297,
       0,     0,   250,     0,  -297,   251,     0,     0,   252,     0,
       0,     0,     0,  -297,     0,     0,     0,     0,     0,  -297,
       0,     0,     0,     0,     0,  -297,     0,     0,     0,  -297,
       0,     0,  -297,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -297,     0,     0,     0,     0,     0,     0,  -297,
    -297,     0,     0,     0,     0,     0,     0,  -297,     0,     0,
       0,     0,     0,     0,     0,     0,  -297,     0,     0,     0,
       0,     0,     0,     0,     0,  -297,  -297,     0,  -297,     0,
    -297,     0,  -297,     0,     0,  -297,     0,     0,     0,  -297,
       0,     0,  -297,     0,     0,  -297
};

static const yytype_int16 yycheck[] =
{
      12,     2,   162,   386,    16,    69,   575,   386,    20,   452,
     219,   176,   607,   371,    26,    40,   662,   855,   516,   511,
      19,   506,    34,    35,    36,   832,    38,   587,   272,   604,
     226,   133,   358,   538,   751,   827,    56,    49,    56,    51,
      52,     1,   658,   107,   660,    57,    33,    59,   850,    61,
       3,     3,   296,  1025,   964,    33,  1117,    69,    70,    71,
      72,    73,    74,    11,     1,    33,    78,    33,    80,  1016,
     314,    48,   102,    61,    47,    87,     1,     3,    90,    91,
      27,    93,    33,    95,   146,     0,    98,    99,   146,   101,
      70,   146,    33,   105,   106,   107,   108,  1190,   262,   263,
     264,    48,   165,   268,   228,    52,     1,   165,    37,   607,
    1020,   173,    85,    48,    75,    75,    48,    48,   146,   131,
     175,   133,   146,   140,     1,   120,   106,    48,  1189,    76,
     118,   148,    79,   120,    81,   199,    61,  1047,    56,    92,
     170,   114,   120,    90,    73,   173,    92,   171,   272,  1242,
     103,   103,   172,    48,   172,    90,   117,   117,    48,    90,
     172,   109,   109,    48,   662,  1003,  1138,    92,     2,   146,
       3,  1118,   296,  1011,    34,    35,    36,   103,   342,   343,
     192,   193,   194,   170,    61,   197,   198,   199,   125,   906,
     314,    25,   170,   118,    28,   146,    32,    69,   442,   443,
    1002,   226,   170,   778,   170,   146,   998,  1117,   145,   146,
     171,   171,   118,    56,   146,    92,   228,   125,   217,   170,
     145,   386,    48,    22,   170,   146,   173,    89,   105,   170,
     125,    93,   120,   105,   106,   107,   173,   722,   146,   744,
     146,   118,   150,   175,    78,   170,    32,   107,    82,    83,
     145,   146,   578,    89,   175,   357,   146,    93,    27,   120,
     272,   146,   831,    56,    92,   173,   140,   463,   145,     3,
     165,   105,  1110,  1242,   148,    18,   110,  1115,   172,  1189,
     174,   146,    27,   407,   296,   175,    20,   175,   165,    88,
     175,   303,   304,   170,   120,  1102,   856,  1104,   362,   459,
      34,  1270,   314,    89,   140,   141,   171,    93,   751,   145,
      79,   147,   148,   112,   175,   524,   152,    61,   442,   443,
     146,    90,    82,    83,   160,   337,   198,   199,   146,    63,
     166,   167,   168,   334,    79,    78,   146,   165,   350,   146,
     352,  1058,   170,    56,   919,    90,   115,  1185,    92,   175,
     362,   146,   146,    96,   140,   173,   109,   146,   165,   145,
     165,   105,   148,   173,   109,    90,   152,   172,   853,   103,
     113,   580,   140,   865,   118,   109,   165,   211,   173,   173,
     148,   393,   506,   395,   396,   397,   398,   399,   400,   165,
     402,   403,   404,   405,   406,   171,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,     8,   538,   479,  1032,  1033,   903,   431,
     432,   165,   991,   125,  1019,   437,   170,     8,   440,   441,
     442,   443,   444,   445,    47,  1081,    54,   146,   463,   172,
     452,   275,  1134,   146,   146,   457,   280,  1139,   150,   461,
     146,   431,   432,    66,   125,    56,    74,   437,   470,   125,
     440,   441,   146,   906,   173,   146,    84,   479,   171,   481,
     482,   173,    85,    27,    32,   487,   488,   173,    56,   146,
     151,   493,   153,   154,   150,     2,   157,   158,   500,   173,
     362,   503,   173,   146,   506,   878,    32,   855,   172,   878,
     334,   114,   146,   146,   171,   863,   518,  1134,    25,   146,
     522,    28,  1139,   525,   526,   349,   874,   146,   146,   531,
     173,  1019,   146,    56,   146,    79,   538,  1025,   165,   173,
     173,    89,  1101,    56,  1103,    93,    90,   549,   398,   399,
     552,   165,   554,   165,   173,   173,   558,   559,   560,   146,
     562,   563,   386,    89,   146,   109,   146,    93,   146,   146,
     146,    78,   146,   575,   576,    82,    83,   568,   165,    61,
      61,    56,   552,   165,   554,   165,   588,   165,   165,   165,
     165,   165,   140,  1081,   444,   445,   171,   145,   105,   147,
     148,   755,   756,   110,   152,   759,   760,   457,   722,    89,
      92,    92,   160,    93,   140,   141,    56,   479,   144,   145,
     168,   147,   148,   105,   105,  1058,   152,   172,    47,   174,
     744,   455,   482,    48,   160,    89,   118,   118,   488,    93,
     166,   167,   168,   493,   140,    51,    61,    66,    54,   145,
    1138,   653,   148,   172,    89,  1003,   152,   481,    93,   813,
     814,    89,   172,   487,   174,    93,    85,    83,    74,   531,
      86,    77,   165,   172,   498,   174,   500,    92,    84,   172,
      56,   174,    89,   165,   165,    56,    93,   549,   170,   170,
     552,   170,   554,   172,    48,   114,   172,   559,   174,   297,
     298,   851,  1190,   118,   211,   100,   172,    61,   558,   172,
     125,   174,   826,   827,    56,   717,   718,   719,   720,   721,
     722,   723,   724,   878,   172,   552,   174,   554,    56,   783,
      56,   146,   556,    56,   170,  1223,   560,    56,    92,   853,
     742,   145,   744,   745,   568,   747,   735,   736,   172,   751,
     165,   753,   165,   164,  1242,   170,   165,   165,   173,   156,
     762,   763,  1110,   165,   118,   165,   165,   165,   275,    56,
     119,   125,   742,   280,   165,   165,   931,   170,    32,   781,
     782,   783,  1270,   938,   172,   787,    22,  1128,   170,   903,
      26,    27,   146,   172,    85,    31,   165,   172,   172,   172,
      36,     8,    38,    39,   172,    66,   170,     8,    44,   171,
     125,   165,    90,    90,   868,    51,   170,   787,   125,   173,
      69,   148,   824,   109,   172,   125,   828,   334,   830,   831,
     832,    48,   834,   173,   836,    89,   146,   839,   840,    93,
      48,    77,   349,    79,   125,    81,   125,    83,  1082,  1083,
      86,   853,    88,    61,    90,     2,  1197,  1198,  1199,  1200,
    1201,  1202,    35,   172,    35,   173,   868,    66,   718,   125,
     720,   173,   173,   109,   170,    21,   112,   175,    25,   115,
     742,    28,   175,   146,    92,   173,   140,   141,   146,   143,
     144,   145,   894,   147,   148,   897,   105,   172,   152,   140,
     148,   903,   148,   148,   906,   159,   160,   173,   732,   163,
     118,   148,   166,   167,   168,   148,   148,   125,   920,   921,
     974,   783,   140,   148,   148,   148,   140,   148,   148,   165,
     932,    78,   934,   148,   140,    82,    83,   939,   146,   165,
     942,   765,   140,   165,   768,   175,   165,    35,   455,   165,
      35,   146,   954,   165,   165,   145,    33,   165,   105,   172,
     165,   171,   170,   110,    33,   173,   170,    35,  1082,  1083,
     173,   171,   974,   175,   481,   145,   173,   165,   165,   175,
     487,   983,   175,   985,   175,   165,   988,   172,   812,   991,
     172,   498,   175,   500,   818,     8,   998,   175,  1000,  1154,
    1155,   173,   140,  1158,  1159,   140,   868,    35,   173,   165,
     173,   173,   165,   165,   164,   164,  1171,   146,  1173,   271,
    1022,   824,    24,  1011,   349,    90,   960,   321,   522,   978,
     859,   360,   878,   874,   598,  1037,  1038,   372,  1040,  1041,
    1139,  1043,  1044,   863,  1203,   873,   519,  1196,   664,   556,
     131,  1053,  1054,   560,  1056,   777,  1058,    -1,  1060,  1061,
      -1,   568,  1064,    32,   211,  1119,    -1,    -1,    -1,    -1,
     920,   921,    -1,    -1,    -1,    -1,    -1,    -1,  1080,  1081,
    1082,  1083,    -1,    -1,    -1,    -1,  1088,    -1,  1090,    -1,
      -1,    -1,  1094,    -1,    -1,    -1,   920,   921,  1100,  1101,
    1102,  1103,  1104,    -1,    -1,    -1,    -1,    -1,   932,    -1,
     934,    -1,   974,    -1,    27,   939,    -1,  1119,   942,    -1,
      89,    -1,  1124,    -1,    93,    -1,    -1,   951,   275,   953,
      -1,    -1,   956,   280,    -1,    48,  1138,    -1,    -1,    52,
      -1,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,   973,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1019,    -1,   983,
      27,   985,    -1,    76,    -1,    -1,    79,    -1,    81,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    90,   147,   148,
    1182,    48,    -1,   152,    -1,    52,    32,   334,    -1,    -1,
      -1,   160,    -1,  1195,    -1,    -1,   109,   166,   167,   168,
      -1,     2,   349,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    -1,    79,    -1,    81,    -1,  1218,  1219,  1220,  1221,
      -1,  1223,    -1,    90,    25,  1227,  1228,    28,    -1,  1053,
      -1,    -1,    -1,    -1,    -1,    -1,  1060,    -1,    -1,  1241,
    1242,    -1,   109,    89,    -1,    -1,    -1,    93,    -1,  1073,
    1252,  1253,  1254,  1255,  1256,  1257,    -1,  1119,   765,    -1,
    1084,   768,  1124,    -1,    -1,    -1,     2,    -1,  1270,  1093,
      -1,    -1,    -1,  1097,    -1,    -1,    -1,    78,    -1,    -1,
      -1,    82,    83,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      -1,    -1,    28,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,   105,   812,   152,    -1,   455,   110,
      -1,   818,    -1,   159,   160,     3,    -1,   163,    -1,    -1,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,  1190,    17,
      -1,   177,    20,    -1,   481,    -1,  1160,    25,    -1,    -1,
     487,    -1,    78,    -1,    -1,    -1,    82,    83,    36,    -1,
      -1,   498,    -1,   500,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,   105,
      -1,    -1,    -1,    -1,   110,    63,    -1,    -1,    -1,    -1,
    1242,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1218,  1219,  1220,  1221,    -1,    87,
      -1,    -1,    32,  1227,  1228,    -1,    -1,    -1,  1270,   556,
     211,    99,    -1,   560,    -1,   103,    -1,     1,    -1,    -1,
      -1,   568,   110,    -1,    -1,   932,    -1,   934,   116,    -1,
      -1,    -1,   939,    -1,    -1,   942,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,   951,    -1,   953,    31,    -1,   956,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    89,
      44,    -1,    -1,    93,    -1,    -1,   973,    51,    -1,    -1,
      54,    -1,    -1,    -1,   275,   211,   983,    -1,   985,   280,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    77,    -1,    79,    -1,    81,     2,    83,
      84,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
     140,    -1,    -1,    -1,    -1,   145,    -1,   147,   148,    -1,
      -1,    25,   152,    -1,    28,   109,    -1,    -1,   112,    -1,
     160,   115,    -1,   334,    -1,    -1,   166,   167,   168,   275,
      -1,    -1,    -1,    -1,   280,    -1,  1053,    -1,   349,    -1,
      -1,    -1,    -1,  1060,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1073,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    78,    -1,    -1,  1084,    82,    83,
      22,   165,    -1,    -1,    26,    27,  1093,   171,    -1,    31,
    1097,    -1,    25,    -1,    36,    28,    38,    39,   334,    -1,
      22,   105,    44,    -1,    26,    27,   110,    -1,    -1,    51,
      -1,    -1,    -1,   349,    36,    -1,    38,    39,   765,    -1,
      -1,   768,    44,    -1,    -1,    -1,    -1,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    83,    -1,    -1,    86,    78,    88,    -1,    90,    82,
      83,    -1,    -1,  1160,   455,    77,    -1,    79,    -1,    -1,
      -1,    83,    -1,    -1,    86,   812,    88,   109,    90,    -1,
     112,   818,   105,   115,    -1,    -1,    -1,   110,    -1,    -1,
     481,    -1,    -1,    -1,    -1,    -1,   487,   109,    -1,    -1,
     112,    -1,    -1,   115,    -1,    -1,    -1,   498,    -1,   500,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,    -1,    -1,
      -1,  1218,  1219,  1220,  1221,    -1,    -1,    -1,    -1,   455,
    1227,  1228,    -1,   165,    -1,    -1,    -1,    -1,    -1,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   165,    -1,   481,    -1,     2,    -1,    -1,
      -1,   487,    -1,    -1,    -1,   556,    -1,    -1,    -1,   560,
      -1,    -1,   498,    -1,   500,    -1,    -1,   568,    -1,    -1,
      25,   275,    -1,    28,    -1,    -1,   280,    -1,   211,    -1,
      -1,    -1,    -1,    -1,    -1,   932,    -1,   934,    -1,    -1,
      -1,    -1,   939,    -1,    -1,   942,    22,    -1,    -1,    -1,
      26,    27,    -1,    -1,   951,    -1,   953,    -1,    -1,   956,
      36,    -1,    38,    39,    -1,    -1,    -1,    -1,    44,    -1,
     556,    -1,    -1,    78,   560,    51,   973,    82,    83,    -1,
     334,    -1,   568,    -1,    -1,    -1,   983,    -1,   985,    -1,
      66,    -1,   275,    -1,    -1,   349,    -1,   280,    -1,    -1,
     105,    77,    -1,    79,    -1,   110,    -1,    83,    -1,    85,
      86,    -1,    88,    -1,    90,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,   109,    -1,    -1,   112,    25,    -1,   115,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,   334,    40,    -1,    -1,    -1,  1053,    -1,    46,    -1,
      -1,    -1,    50,  1060,    22,    53,   349,    -1,    26,    27,
      -1,    -1,    -1,    -1,    -1,    63,  1073,    -1,    36,    -1,
      38,    39,    70,    71,    -1,    -1,    44,  1084,    -1,   165,
      78,    -1,    -1,    51,    -1,    -1,  1093,    -1,    -1,    87,
    1097,   455,    -1,    -1,    -1,    -1,   211,    -1,    96,    97,
      -1,    99,    -1,   101,   765,   103,    -1,   768,   106,    77,
      -1,    79,   110,   111,    -1,   113,    -1,   481,   116,    -1,
      88,    -1,    90,   487,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   498,    -1,   500,    -1,    -1,    -1,
      -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,   812,    -1,  1160,    -1,    -1,    -1,   818,    -1,    -1,
     275,    -1,   455,    -1,    -1,   280,    -1,    -1,    -1,   765,
      -1,    -1,   768,    -1,   172,    -1,    -1,     2,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,   481,    -1,
      -1,    -1,   556,    -1,   487,    -1,   560,    -1,    -1,    -1,
      25,    -1,    -1,    28,   568,   498,    -1,   500,    -1,    -1,
      -1,  1218,  1219,  1220,  1221,    -1,   812,    -1,    -1,   334,
    1227,  1228,   818,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   349,   143,   144,   145,    -1,   147,
     148,   149,    -1,    -1,   152,   153,   154,    -1,    -1,   157,
     158,   159,   160,    78,    -1,   163,    -1,    -1,   166,   167,
     168,    -1,    -1,   556,    -1,    -1,    -1,   560,    -1,   177,
      -1,   932,    -1,   934,    -1,   568,    -1,    -1,   939,    -1,
     105,   942,    -1,    -1,    -1,   110,    -1,    -1,    -1,    -1,
     951,    -1,   953,    -1,    -1,   956,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   973,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   983,    -1,   985,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   932,    -1,   934,    -1,
     455,    -1,    -1,   939,    -1,    -1,   942,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   951,    -1,   953,    -1,    -1,
     956,    -1,    -1,    -1,    -1,    -1,   481,    -1,    -1,    -1,
      -1,    -1,   487,    -1,    -1,    -1,    -1,   973,    -1,    -1,
      -1,    -1,    -1,   498,    -1,   500,   211,   983,    -1,   985,
      -1,    -1,  1053,    -1,    -1,    -1,    -1,    -1,    -1,  1060,
      -1,   765,    -1,    -1,   768,    -1,    -1,    -1,     2,    -1,
      -1,    -1,  1073,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1084,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,  1093,    -1,    28,    -1,  1097,    -1,    -1,    -1,
      -1,   556,    -1,    -1,    -1,   560,    -1,    -1,   812,    -1,
     275,    -1,    -1,   568,   818,   280,    -1,  1053,    -1,    -1,
      -1,    -1,    -1,    -1,  1060,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   765,    -1,    -1,   768,    -1,  1073,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    22,    -1,  1084,    -1,
      26,    27,    -1,    -1,    -1,    31,    -1,  1093,    -1,  1160,
      36,  1097,    38,    39,    -1,    -1,    -1,    -1,    44,   334,
      -1,   105,    -1,    -1,    -1,    51,   110,    -1,    -1,   812,
      -1,    -1,    -1,    -1,   349,   818,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    83,    -1,    -1,
      86,    49,    88,    -1,    90,    -1,    -1,  1218,  1219,  1220,
    1221,    -1,    -1,    -1,  1160,    -1,  1227,  1228,   932,    -1,
     934,    -1,    -1,   109,    72,   939,   112,    -1,   942,   115,
      -1,    -1,    -1,    -1,    82,    83,    -1,   951,    -1,   953,
      -1,    -1,   956,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   973,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   211,    -1,   983,
      -1,   985,  1218,  1219,  1220,  1221,    -1,    -1,    -1,   165,
     455,  1227,  1228,    -1,    -1,   171,    -1,    -1,    -1,   932,
      -1,   934,    -1,    -1,    -1,    -1,   939,    -1,    -1,   942,
     765,    -1,    -1,   768,    -1,    -1,   481,    -1,   951,    -1,
     953,    -1,   487,   956,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   498,    -1,   500,    -1,    -1,    -1,    -1,
     973,   275,    -1,    -1,    -1,    -1,   280,    -1,    -1,  1053,
     983,    -1,   985,    -1,    -1,    -1,  1060,   812,    -1,    -1,
      -1,    -1,    -1,   818,    -1,    -1,    -1,    -1,    -1,  1073,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   216,    -1,
    1084,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1093,
      -1,   556,    -1,  1097,    -1,   560,    -1,    -1,    -1,    -1,
     334,    -1,    -1,   568,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   349,    -1,    -1,    -1,    -1,
    1053,   259,   260,   261,   262,   263,   264,  1060,    -1,   267,
     268,    -1,    -1,    -1,    -1,    -1,   274,    -1,    -1,    -1,
    1073,    -1,    -1,    -1,    -1,   283,    -1,   285,    -1,    -1,
      -1,  1084,    -1,    -1,   292,    -1,  1160,    -1,    -1,    -1,
    1093,    -1,    -1,    -1,  1097,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   310,    -1,    -1,    -1,    -1,   932,    -1,   934,
      -1,    -1,    -1,    -1,   939,   323,   324,   942,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   951,    -1,   953,    -1,
      -1,   956,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1218,  1219,  1220,  1221,   973,    -1,
      -1,   455,    -1,  1227,  1228,    -1,    -1,  1160,   983,    -1,
     985,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   481,    -1,    -1,
      -1,    -1,    -1,   487,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   498,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1218,  1219,  1220,  1221,    -1,
      -1,    -1,    -1,    -1,  1227,  1228,    -1,    -1,  1053,    -1,
     765,    -1,    -1,   768,    -1,  1060,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,  1073,    -1,
     458,    -1,   556,    -1,    -1,    -1,   560,    -1,    -1,  1084,
      -1,    -1,    -1,    -1,   568,    -1,    -1,    -1,  1093,    -1,
      -1,    -1,  1097,    -1,    -1,    -1,   124,   812,   486,    -1,
      -1,    -1,   490,   818,   492,    -1,    -1,   495,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,   170,   259,   260,   261,   262,   263,   264,   177,
      -1,    -1,    -1,    -1,    -1,  1160,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    12,    -1,   283,    -1,   285,
      -1,    -1,    19,    -1,    -1,    -1,   292,    -1,    44,    45,
      46,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   310,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    -1,    -1,    -1,   323,   324,    -1,
      -1,    -1,    -1,  1218,  1219,  1220,  1221,   932,    -1,   934,
      -1,    -1,  1227,  1228,   939,    -1,    -1,   942,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   951,    -1,   953,    -1,
      -1,   956,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   973,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   983,    -1,
     985,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,
      -1,   765,    -1,   149,   768,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   161,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,   170,    -1,    -1,    -1,    -1,   812,   195,
     177,    -1,    -1,    -1,   818,    -1,    -1,    -1,  1053,    -1,
      -1,    -1,   458,    -1,    -1,  1060,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1073,    -1,
      -1,   749,   750,    -1,    -1,    -1,    -1,   755,   756,  1084,
     486,   759,   760,    -1,   490,    -1,   492,    -1,  1093,   495,
      -1,   769,  1097,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   257,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   265,
     266,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     276,    -1,    -1,    -1,    -1,   803,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1160,    -1,   156,   932,    -1,
     934,    -1,    -1,    -1,    -1,   939,    -1,    -1,   942,    -1,
     169,    -1,    -1,   172,    -1,   174,    -1,   951,    -1,   953,
      -1,     3,   956,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    -1,    20,   973,
      -1,    -1,    -1,    25,    -1,   361,    -1,    -1,    -1,   983,
      -1,   985,    34,  1218,  1219,  1220,  1221,    -1,    40,    -1,
      -1,    -1,  1227,  1228,    46,    -1,    -1,    -1,    50,    -1,
     386,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,   922,   923,    -1,    -1,    70,    71,
      -1,   929,    -1,   931,    -1,    -1,    78,    -1,    -1,    -1,
     938,    -1,    -1,   941,    -1,    87,   944,    -1,   946,    -1,
     948,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,   961,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,   451,    -1,    -1,    -1,  1073,
      -1,    -1,    -1,   981,    -1,    -1,    -1,    -1,    22,   465,
    1084,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,  1093,
      -1,    -1,    36,  1097,    38,    39,    -1,    -1,    -1,    -1,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    -1,     3,   749,   750,    -1,    -1,    -1,    -1,   755,
     756,   507,    -1,   759,   760,   511,    17,    18,    -1,    20,
      -1,    -1,    -1,    77,    25,    79,    -1,    81,   524,    83,
      -1,    -1,    86,    34,    88,    -1,    90,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,    46,  1160,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,   109,    -1,    -1,   112,    -1,
      -1,   115,    63,    -1,    -1,    -1,    -1,    -1,  1086,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,   574,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,   587,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,
     101,    -1,   103,    -1,    -1,   106,   602,    -1,    -1,   110,
      -1,   165,   113,    -1,    -1,   116,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1154,  1155,    -1,    -1,
    1158,  1159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1169,    -1,  1171,    -1,  1173,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   664,   170,
      -1,   172,    -1,    -1,    -1,    -1,   922,   923,    -1,    -1,
      -1,    -1,     3,   929,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,   941,    17,    18,   944,    20,
     946,    22,   948,    -1,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    79,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    90,
      -1,    -1,    -1,    -1,   770,    96,    97,    98,    99,   100,
     101,   777,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,   115,   116,    -1,   793,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,   803,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
    1086,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,    -1,    -1,    -1,
     856,    -1,    -1,   859,    -1,    -1,   862,    -1,    -1,   865,
      -1,     0,     1,    -1,     3,    -1,     5,     6,     7,     8,
       9,    10,   878,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,   960,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,   978,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    19,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,    -1,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    -1,    33,    34,    -1,    36,    -1,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    -1,    53,    54,    -1,    56,    57,    58,
      59,    60,    -1,    62,    63,    64,    65,    66,    67,    68,
      -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,    78,
      79,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    33,
      34,    -1,    36,    -1,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    -1,    53,
      54,    -1,    56,    57,    58,    59,    60,    -1,    62,    63,
      64,    65,    66,    67,    68,    -1,    70,    71,    72,    -1,
      74,    -1,    -1,    77,    78,    79,    80,    -1,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
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
      -1,   170,    -1,   172,    -1,   174,   175,     1,   177,     3,
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
      -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    -1,    -1,    50,
      -1,    -1,    53,    -1,    55,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,   140,
     141,   142,   143,   144,   145,    -1,   147,    -1,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,    -1,   166,   167,   168,   169,   170,
      -1,   172,    -1,   174,    -1,     3,   177,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    -1,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    30,    -1,    -1,    33,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    43,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    62,    63,    64,    65,    -1,    67,
      68,    -1,    70,    71,    72,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,   119,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
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
     143,   144,    -1,    -1,    -1,    -1,   149,   150,    -1,    -1,
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
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,
     168,    -1,   170,    -1,   172,   173,   174,    -1,    -1,   177,
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
     173,   174,    -1,     3,   177,     5,     6,     7,     8,     9,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,
      20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,
      40,    41,    42,    -1,    -1,    45,    46,    -1,    48,    -1,
      50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,
     100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,
     110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,
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
     107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,
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
      -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,
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
      69,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
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
      -1,    -1,    78,    79,    80,    -1,    82,    -1,    -1,    -1,
      -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,
      -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
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
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,   149,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,
     170,   171,   172,    -1,   174,    -1,    -1,   177,     3,     4,
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
      82,    -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,
      -1,    93,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
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
      -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,     3,
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
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,
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
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,
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
      -1,    -1,    -1,   110,   111,    -1,   113,    -1,    -1,   116,
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
      -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
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
      -1,    -1,    -1,    -1,     1,    -1,     3,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    12,    -1,    -1,    -1,   149,
      17,    18,    19,    20,    -1,    -1,    -1,    -1,    25,   159,
      -1,    -1,   162,   163,   164,    -1,    -1,    34,   168,    36,
      -1,    -1,   172,    40,   174,    -1,    -1,   177,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
     147,    -1,    -1,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,   163,    -1,    -1,   166,
     167,   168,   169,    -1,     3,   172,     5,     6,     7,     8,
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
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,     3,   174,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    36,    -1,    -1,
      -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,    -1,
      59,    60,    61,    -1,    63,    -1,    65,    -1,    67,    68,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,    88,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,   105,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,   118,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,     3,     4,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,
      18,    -1,    20,    -1,    22,   164,   165,    25,    -1,    -1,
      -1,   170,    -1,   172,    -1,   174,    34,    -1,    36,    -1,
      -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,    -1,
      -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,    67,
      68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    80,    -1,    82,    -1,    -1,    -1,    -1,    87,
      88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   142,    -1,    -1,    -1,   146,    -1,
      -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   164,    -1,    -1,    -1,
      -1,    -1,   170,    -1,   172,   173,   174,     3,    -1,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,    -1,     3,    -1,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    17,    18,    -1,    20,    -1,    22,   164,    -1,
      25,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,     3,
      -1,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,   164,
      -1,    25,    -1,    -1,    -1,   170,    -1,   172,    -1,   174,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,     3,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,   142,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,   172,    -1,
     174,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    17,    18,   106,    20,    -1,
      -1,   110,    -1,    25,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,     3,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    12,    -1,    -1,    50,    -1,
      -1,    53,    19,    20,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    34,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,   172,    -1,    87,    -1,    -1,    55,    -1,
      -1,    -1,    -1,    -1,    96,    97,    63,    99,    -1,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
     172,   138,   139,   140,   141,   142,   143,   144,   145,    -1,
     147,    -1,    -1,    -1,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,     3,   163,    -1,    -1,   166,
     167,   168,   169,   170,    12,    -1,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    96,    97,
      -1,    99,    -1,   101,    19,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    32,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,    -1,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
     168,   169,    -1,    12,    89,    -1,    -1,    -1,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
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
     163,    -1,    89,   166,   167,   168,    93,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,    -1,    -1,   133,   134,   135,   136,
      -1,   138,   139,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    -1,   166,
     167,   168,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   124,
      -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
     165,   166,   167,   168,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,   120,   166,   167,
     168,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    89,    -1,    -1,    -1,    93,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    -1,   120,   166,   167,   168,   124,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    12,    93,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    48,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,   124,    -1,
     166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    12,    93,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,   102,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    32,    -1,    -1,   120,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    89,   175,    -1,   177,    93,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,    -1,   166,   167,   168,   124,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    -1,   166,
     167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,   124,
     170,    -1,    -1,    -1,    89,    -1,    -1,   177,    93,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,   124,
     165,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
     165,   166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    12,
      93,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     123,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   102,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    13,   166,   167,   168,    -1,    -1,    19,    -1,
     173,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,   177,    93,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,   120,   166,   167,   168,   124,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    12,    -1,
     166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,   165,   166,   167,   168,
     124,    -1,    -1,    -1,    -1,    89,    -1,    -1,   177,    93,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
     124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,   173,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   123,   124,   166,   167,   168,    -1,    -1,    89,
      -1,   173,    -1,    93,    -1,   177,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,   124,   166,   167,   168,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   177,    19,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      32,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,   177,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,   124,   166,   167,   168,    -1,    -1,    89,
      -1,   173,    -1,    93,    -1,   177,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,   124,   166,   167,   168,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,   177,    19,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      32,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,   177,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,   124,   166,   167,   168,    -1,    -1,    12,
      89,    -1,    -1,   175,    93,   177,    19,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    32,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,   157,   158,
     159,   160,   161,    -1,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    -1,    89,   166,   167,   168,    93,    -1,    -1,    -1,
      12,    13,    -1,    -1,   177,   102,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,    -1,    -1,    -1,    12,    89,    -1,    -1,
     177,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,   140,   141,
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
     166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
      -1,    19,    -1,    -1,    -1,   175,    -1,   177,   102,    -1,
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
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,   175,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,    89,   166,   167,   168,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,    89,
     166,   167,   168,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,    -1,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,    89,   166,   167,   168,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,    -1,    -1,   157,   158,   159,   160,   161,     1,   163,
       3,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,     3,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,    17,    18,   106,    20,    -1,    -1,   110,    -1,    25,
     113,    27,    -1,   116,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,
      46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    -1,   103,     3,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,
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
     225,   226,   227,   228,   229,   231,   232,   234,   236,   239,
     240,   241,   242,   243,   247,   248,   253,   254,   255,   256,
     259,   260,   267,   268,   270,   271,   272,   273,   275,   276,
     301,   302,   306,   307,   328,   329,   330,   331,   332,   333,
     334,   342,   343,   344,   345,   346,   349,   350,   351,   352,
     353,   354,   355,   356,   358,   359,   360,   361,   362,   165,
      22,    36,    41,    42,    45,    56,    88,   101,   170,   174,
     239,   268,   328,   333,   344,   345,   346,   349,   351,   352,
     120,   335,   336,     3,   209,   346,   335,   346,   109,   308,
      90,   209,   186,   322,   346,     8,   190,   186,   172,     3,
      17,    18,    20,    25,    34,    40,    46,    50,    53,    63,
      70,    71,    78,    87,    96,    97,    99,   101,   103,   106,
     110,   113,   116,   211,   213,    11,   109,    79,   122,   233,
     346,   233,   346,   233,   346,    27,   115,   235,   346,    83,
      86,   194,   172,   211,   211,   211,   172,   211,   309,   310,
      33,   198,   216,   346,   251,   252,   346,   346,    18,    78,
      96,   113,   346,   346,   346,     8,   172,   223,   222,     4,
     296,   321,   346,   107,   108,   165,   346,   348,   346,   216,
     346,   346,   346,   100,   172,   186,   346,   346,   187,   198,
     187,   198,   346,   235,   346,   346,   346,   346,   346,   346,
     346,     1,   171,   184,   199,   321,   111,   150,   296,   323,
     324,   348,   233,   321,   346,   357,   346,    81,   186,   170,
      47,    85,   114,   195,    26,   307,   346,     8,   255,   346,
     347,    56,   145,   257,   211,     1,    31,   211,   261,   263,
     266,   172,   284,    54,    74,    84,   291,    27,    79,    90,
     109,   292,    27,    79,    90,   109,   290,   211,   303,   304,
     309,   164,   165,   156,   346,    12,    19,    32,    89,    93,
     124,   140,   141,   143,   144,   145,   147,   148,   149,   151,
     152,   153,   154,   155,   157,   158,   159,   160,   161,   162,
     163,   166,   167,   168,   177,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     169,   280,   172,   174,    89,    93,   346,   346,   346,   211,
     321,    56,   172,   186,   165,   170,   198,    48,   335,   306,
     165,   146,   165,   189,    22,    36,    39,    88,   191,   194,
     119,   212,   296,   325,   326,   327,   348,   170,   211,   172,
     216,    33,    48,   216,   120,   216,   338,    33,    48,   216,
     338,   216,   338,    48,   216,   338,   211,   211,   102,   198,
     102,   125,   198,   280,   195,   325,   172,   172,   198,   186,
     111,   172,   211,   311,   312,     1,   145,   316,    48,   146,
     186,   216,   146,   216,    13,   172,   172,   216,   325,   224,
     224,   146,   165,   346,   346,   165,   170,   216,   172,   325,
     165,   244,   244,   216,   216,   165,   171,   171,   184,   146,
     171,   346,   146,   173,   146,   173,   175,   338,    48,   146,
     175,   338,   123,   146,   175,     8,     1,   171,   199,   205,
     206,   346,   201,   346,    66,    37,    73,   165,   255,   257,
     230,   273,   211,   321,   170,   171,     8,   265,   125,   146,
     171,    27,    48,    52,    76,    79,    90,   109,   185,   285,
     286,   287,   288,   289,   292,    90,     1,   145,   315,    90,
       1,     3,    12,    17,    19,    20,    25,    40,    46,    53,
      55,    63,    70,    71,    87,    99,   103,   106,   110,   116,
     140,   141,   142,   143,   144,   145,   147,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   163,   166,
     167,   168,   169,   172,   210,   211,   213,   277,   278,   279,
     280,   328,   125,   305,   146,   165,   165,   346,   346,   346,
     346,   233,   346,   233,   346,   346,   346,   346,   346,   346,
     346,     3,    20,    34,    63,   103,   109,   212,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,    69,   348,   348,   348,   348,
     348,   325,   325,   233,   346,   233,   346,    33,    48,    33,
      48,   102,   172,    48,   175,   211,    27,    48,    90,   115,
     337,   340,   341,   346,   362,   233,   346,   216,   309,   346,
     191,   346,   125,   173,   146,    48,   321,    45,   346,   233,
     346,   172,   216,    45,   346,   233,   346,   216,   216,   233,
     346,   216,   125,   125,   186,    35,   186,   346,    35,   346,
      66,   173,   326,   211,   237,   238,   312,   146,   173,    34,
      50,    97,   101,   174,   215,   317,   329,   125,   313,   346,
     310,   251,   211,   306,   346,   346,   173,   296,   346,     1,
     249,   327,   173,    21,   245,   171,   173,   173,   323,   173,
     323,   186,   175,   233,   346,   175,   186,   346,   175,   346,
     175,   346,   171,   171,   146,   165,    13,   148,   146,   165,
      13,    37,    73,   211,   140,   141,   142,   143,   144,   159,
     163,   168,   200,   279,   280,   281,   346,   200,   202,   257,
     170,   306,   165,   172,     1,   258,   264,   266,   346,   262,
      48,    90,   288,   146,   173,   172,   211,   316,   174,   215,
     314,   329,   105,   293,   172,   283,   346,   140,   148,   283,
     283,   317,   329,   304,   172,   174,   165,   165,   165,   165,
     165,   165,   173,   175,    45,   233,   346,    45,   233,   346,
     326,   233,   346,   172,    48,    90,   146,   173,    17,    20,
      25,    46,    53,    63,    71,    87,    99,   110,   116,   328,
      89,    89,   335,   335,   165,   165,   296,   348,   327,   346,
     173,   346,    33,   216,    33,   216,   339,   340,   346,    33,
     216,   338,    33,   216,   338,   216,   338,   216,   338,   346,
     346,    35,   186,    35,    35,   186,   102,   198,   211,   173,
     146,   173,   173,   311,   316,     1,   321,    69,   348,   211,
     173,   173,   171,    75,   117,   171,   250,   173,   172,   198,
     211,   246,   186,   175,   338,   175,   338,   186,   123,   205,
     212,   170,   281,   346,   111,   346,   200,   202,   146,   165,
      13,   165,   170,   258,   309,   326,   171,    31,    83,    86,
     171,   185,   218,   221,   266,   263,   211,   287,   312,   145,
     320,    61,   118,   297,     1,   175,   321,   297,   286,   173,
       3,   103,   278,   280,   173,   175,   346,    33,    33,   346,
      33,    33,   173,   175,   175,   326,   340,   316,   328,   328,
     216,   216,   216,   102,    45,   346,    45,   346,   146,   173,
     102,    45,   346,   216,    45,   346,   216,   216,   216,   186,
     186,   346,   186,    35,   165,   165,   238,   198,   313,   175,
      48,   175,   172,   172,    33,   186,   321,   246,   145,   198,
      45,   186,   346,   175,    45,   186,   346,   175,   346,   200,
      13,    37,    73,    37,    73,   165,   165,   281,   346,   346,
     258,   171,   165,   173,     8,   220,   218,   320,   173,   174,
     215,   296,   319,   329,   150,   282,   313,     3,    92,   103,
     298,   299,   300,   346,   269,   175,   314,   329,   175,   274,
     173,   283,   283,   102,    45,    45,   102,    45,    45,    45,
      45,   173,   313,   346,   346,   346,   340,   346,   346,   346,
      35,   186,   346,   317,   329,   325,   325,   186,   216,   173,
     346,   346,   186,   346,   186,   171,   111,   346,   200,   202,
     200,   202,    13,   171,   165,   219,   221,   282,   313,   320,
     175,   321,   296,   346,   300,   118,   146,   125,   151,   153,
     154,   157,   158,    61,   197,   217,   295,   314,   329,   165,
     294,   295,   346,   346,   346,   346,   346,   346,   102,   102,
     102,   102,   186,   175,   173,   173,   198,   102,   102,   165,
     165,   165,   165,   346,   221,   282,   313,   296,   318,   319,
     329,    48,   175,   346,   299,   300,   300,   300,   300,   300,
     300,   298,   102,   102,   102,   102,   102,   102,   346,   346,
     346,   346,   329,   164,   164,   346,   346,   346,   318,   319,
     175,   318
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
     227,   228,   229,   229,   229,   230,   229,   231,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   233,   234,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   235,   235,   236,   236,
     237,   237,   238,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   240,   240,   240,   240,
     240,   240,   241,   241,   241,   242,   242,   242,   243,   243,
     243,   243,   244,   244,   245,   245,   245,   246,   246,   247,
     248,   248,   249,   249,   250,   250,   250,   251,   251,   251,
     252,   252,   253,   254,   254,   255,   256,   256,   256,   257,
     257,   258,   258,   258,   259,   259,   260,   261,   261,   262,
     261,   263,   264,   263,   265,   263,   266,   266,   267,   269,
     268,   270,   271,   271,   271,   272,   274,   273,   275,   275,
     275,   275,   275,   276,   277,   277,   278,   278,   278,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   279,   279,   279,   279,   279,
     279,   279,   279,   279,   279,   280,   280,   280,   280,   280,
     280,   280,   280,   280,   280,   280,   280,   281,   281,   282,
     282,   282,   283,   283,   284,   285,   285,   286,   286,   287,
     287,   287,   287,   287,   287,   288,   288,   289,   289,   289,
     289,   289,   289,   289,   289,   289,   290,   290,   290,   290,
     290,   290,   291,   291,   291,   292,   292,   292,   292,   292,
     292,   293,   293,   294,   294,   295,   295,   296,   297,   297,
     297,   297,   297,   298,   298,   299,   299,   299,   299,   299,
     299,   299,   300,   300,   301,   302,   302,   302,   303,   303,
     304,   305,   305,   305,   306,   306,   306,   306,   306,   308,
     307,   307,   309,   309,   310,   310,   311,   311,   311,   312,
     312,   312,   313,   313,   313,   314,   314,   314,   314,   314,
     314,   314,   315,   315,   315,   315,   315,   316,   316,   316,
     316,   316,   317,   317,   317,   317,   318,   318,   318,   319,
     319,   319,   319,   319,   320,   320,   320,   320,   320,   321,
     321,   321,   321,   322,   322,   323,   323,   323,   324,   324,
     325,   325,   326,   326,   327,   327,   327,   327,   328,   328,
     329,   329,   329,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   330,   330,   330,   330,   330,
     330,   330,   330,   330,   330,   331,   331,   331,   331,   331,
     331,   331,   331,   331,   331,   331,   331,   331,   331,   331,
     331,   331,   332,   333,   334,   334,   334,   334,   334,   334,
     334,   334,   335,   335,   336,   337,   337,   338,   339,   339,
     340,   340,   340,   341,   341,   341,   341,   341,   341,   342,
     342,   343,   343,   343,   343,   343,   344,   344,   344,   344,
     344,   345,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   347,
     347,   348,   348,   348,   349,   349,   349,   349,   350,   350,
     350,   350,   350,   351,   351,   351,   352,   352,   352,   352,
     352,   352,   353,   353,   353,   353,   354,   354,   355,   355,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   357,   357,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   358,   358,
     358,   358,   358,   358,   358,   358,   358,   358,   359,   359,
     359,   359,   359,   359,   359,   360,   360,   360,   360,   361,
     361,   361,   361,   362,   362,   362,   362,   362,   362,   362
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
       5,     5,     0,     2,     3,     2,     3,     4,     3,     1,
       1,     3,     3,     5,     5,     2,     1,     1,     1,     0,
       2,     0,     2,     3,     3,     3,     3,     1,     2,     0,
       4,     1,     0,     4,     0,     3,     1,     3,     1,     0,
       7,     0,     1,     1,     1,     1,     0,     7,     4,     4,
       6,     6,     4,     2,     1,     3,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     2,     0,     3,     3,     1,     3,     0,     1,     4,
       5,     4,     5,     6,     6,     0,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     1,     0,     1,     2,     1,     1,
       1,     0,     1,     1,     1,     1,     1,     1,     0,     2,
       2,     4,     4,     1,     3,     3,     3,     3,     3,     3,
       3,     2,     1,     1,     3,     1,     2,     2,     1,     3,
       2,     0,     2,     2,     1,     2,     1,     1,     1,     0,
       5,     3,     1,     3,     3,     5,     1,     1,     3,     1,
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
#line 6479 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 605 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6485 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 606 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6491 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 613 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6499 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 621 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6507 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 625 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6516 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 633 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6522 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: include_module_stmt  */
#line 634 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6528 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: block_stmt  */
#line 635 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6534 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: use_stmt  */
#line 636 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6540 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: import_stmt  */
#line 637 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6546 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: require_stmt  */
#line 638 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6552 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 639 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6558 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: implements_stmt  */
#line 640 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6564 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: interface_stmt  */
#line 641 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6570 "bison-chpl-lib.cpp"
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
#line 6587 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: try_stmt  */
#line 655 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6593 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: return_stmt  */
#line 656 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6599 "bison-chpl-lib.cpp"
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
#line 6612 "bison-chpl-lib.cpp"
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
#line 6625 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: TLABEL ident_def stmt  */
#line 676 "chpl.ypp"
  {
    if ((yyvsp[0].commentsAndStmt).stmt && ((yyvsp[0].commentsAndStmt).stmt->isFor() || (yyvsp[0].commentsAndStmt).stmt->isWhile() || (yyvsp[0].commentsAndStmt).stmt->isDoWhile())) {
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
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TYIELD expr TSEMI  */
#line 699 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6663 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: error TSEMI  */
#line 706 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclState();
  }
#line 6672 "bison-chpl-lib.cpp"
    break;

  case 27: /* tryable_stmt: assignment_stmt  */
#line 713 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6678 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: if_stmt  */
#line 714 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6684 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: loop_stmt  */
#line 715 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6690 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: select_stmt  */
#line 716 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6696 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: manage_stmt  */
#line 717 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6702 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 718 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6708 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: throw_stmt  */
#line 719 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6714 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 721 "chpl.ypp"
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
#line 6732 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 735 "chpl.ypp"
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
#line 6751 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 750 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6763 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 758 "chpl.ypp"
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
#line 6779 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL do_stmt  */
#line 770 "chpl.ypp"
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
#line 6794 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TON expr do_stmt  */
#line 781 "chpl.ypp"
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
#line 6810 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 793 "chpl.ypp"
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
#line 6826 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL do_stmt  */
#line 805 "chpl.ypp"
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
#line 6841 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSYNC stmt  */
#line 816 "chpl.ypp"
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
#line 6858 "bison-chpl-lib.cpp"
    break;

  case 44: /* $@1: %empty  */
#line 833 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6866 "bison-chpl-lib.cpp"
    break;

  case 45: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 deprecated_decl_base  */
#line 837 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6874 "bison-chpl-lib.cpp"
    break;

  case 46: /* $@2: %empty  */
#line 841 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6882 "bison-chpl-lib.cpp"
    break;

  case 47: /* deprecated_decl_stmt: TDEPRECATED $@2 deprecated_decl_base  */
#line 845 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6890 "bison-chpl-lib.cpp"
    break;

  case 50: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 857 "chpl.ypp"
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
#line 6911 "bison-chpl-lib.cpp"
    break;

  case 51: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 877 "chpl.ypp"
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
#line 6930 "bison-chpl-lib.cpp"
    break;

  case 52: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 892 "chpl.ypp"
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
#line 6949 "bison-chpl-lib.cpp"
    break;

  case 53: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 907 "chpl.ypp"
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
#line 6966 "bison-chpl-lib.cpp"
    break;

  case 54: /* opt_access_control: %empty  */
#line 922 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6972 "bison-chpl-lib.cpp"
    break;

  case 55: /* opt_access_control: TPUBLIC  */
#line 923 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6979 "bison-chpl-lib.cpp"
    break;

  case 56: /* opt_access_control: TPRIVATE  */
#line 925 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6986 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_prototype: %empty  */
#line 930 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6992 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_prototype: TPROTOTYPE  */
#line 931 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6999 "bison-chpl-lib.cpp"
    break;

  case 59: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 937 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7009 "bison-chpl-lib.cpp"
    break;

  case 60: /* block_stmt_body: TLCBR TRCBR  */
#line 955 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7017 "bison-chpl-lib.cpp"
    break;

  case 61: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 959 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7025 "bison-chpl-lib.cpp"
    break;

  case 62: /* block_stmt_body: TLCBR error TRCBR  */
#line 963 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7037 "bison-chpl-lib.cpp"
    break;

  case 63: /* block_stmt: block_stmt_body  */
#line 974 "chpl.ypp"
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
#line 7056 "bison-chpl-lib.cpp"
    break;

  case 64: /* stmt_ls: toplevel_stmt  */
#line 992 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7062 "bison-chpl-lib.cpp"
    break;

  case 65: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 993 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7068 "bison-chpl-lib.cpp"
    break;

  case 66: /* renames_ls: expr  */
#line 998 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7076 "bison-chpl-lib.cpp"
    break;

  case 67: /* renames_ls: all_op_name  */
#line 1002 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7084 "bison-chpl-lib.cpp"
    break;

  case 68: /* renames_ls: expr TAS expr  */
#line 1006 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7093 "bison-chpl-lib.cpp"
    break;

  case 69: /* renames_ls: renames_ls TCOMMA expr  */
#line 1011 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7101 "bison-chpl-lib.cpp"
    break;

  case 70: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1015 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1019 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7118 "bison-chpl-lib.cpp"
    break;

  case 72: /* use_renames_ls: expr  */
#line 1029 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(node);
  }
#line 7127 "bison-chpl-lib.cpp"
    break;

  case 73: /* use_renames_ls: expr TAS expr  */
#line 1034 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7137 "bison-chpl-lib.cpp"
    break;

  case 74: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1040 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7148 "bison-chpl-lib.cpp"
    break;

  case 75: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1047 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7157 "bison-chpl-lib.cpp"
    break;

  case 76: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1052 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7168 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1059 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7180 "bison-chpl-lib.cpp"
    break;

  case 78: /* opt_only_ls: %empty  */
#line 1069 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7186 "bison-chpl-lib.cpp"
    break;

  case 79: /* opt_only_ls: renames_ls  */
#line 1070 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7192 "bison-chpl-lib.cpp"
    break;

  case 80: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1075 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7200 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1079 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7211 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1086 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7223 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1094 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7236 "bison-chpl-lib.cpp"
    break;

  case 84: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1103 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7247 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1110 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7259 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1118 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7272 "bison-chpl-lib.cpp"
    break;

  case 87: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1130 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7280 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_expr: expr  */
#line 1137 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
  }
#line 7288 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_expr: expr TDOT all_op_name  */
#line 1141 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot));
  }
#line 7297 "bison-chpl-lib.cpp"
    break;

  case 90: /* import_expr: expr TAS ident_use  */
#line 1146 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as));
  }
#line 7307 "bison-chpl-lib.cpp"
    break;

  case 91: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1152 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)));
  }
#line 7317 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_ls: import_expr  */
#line 1160 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7323 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_ls: import_ls TCOMMA import_expr  */
#line 1161 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7329 "bison-chpl-lib.cpp"
    break;

  case 94: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1166 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7339 "bison-chpl-lib.cpp"
    break;

  case 95: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1175 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7347 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1179 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7355 "bison-chpl-lib.cpp"
    break;

  case 97: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1183 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7363 "bison-chpl-lib.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1187 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7371 "bison-chpl-lib.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1191 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7379 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1195 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7387 "bison-chpl-lib.cpp"
    break;

  case 101: /* opt_label_ident: %empty  */
#line 1203 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7393 "bison-chpl-lib.cpp"
    break;

  case 102: /* opt_label_ident: TIDENT  */
#line 1204 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7399 "bison-chpl-lib.cpp"
    break;

  case 103: /* ident_fn_def: TIDENT  */
#line 1208 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7405 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: TNONE  */
#line 1209 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'none'"); }
#line 7411 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_fn_def: TTHIS  */
#line 1210 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7417 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_fn_def: TFALSE  */
#line 1211 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'false'"); }
#line 7423 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_fn_def: TTRUE  */
#line 1212 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'true'"); }
#line 7429 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_fn_def: internal_type_ident_def  */
#line 1213 "chpl.ypp"
                           { std::string msg = "attempt to redefine reserved type '";
                             msg += (yyvsp[0].uniqueStr).c_str();
                             msg += "'";
                             (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), msg); }
#line 7438 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_def: TIDENT  */
#line 1219 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7444 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: TNONE  */
#line 1220 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'none'"); }
#line 7450 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_def: TTHIS  */
#line 1221 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'this'"); }
#line 7456 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_def: TFALSE  */
#line 1222 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'false'"); }
#line 7462 "bison-chpl-lib.cpp"
    break;

  case 113: /* ident_def: TTRUE  */
#line 1223 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "attempt to redefine reserved word 'true'"); }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 114: /* ident_def: internal_type_ident_def  */
#line 1224 "chpl.ypp"
                           { std::string msg = "attempt to redefine reserved type '";
                             msg += (yyvsp[0].uniqueStr).c_str();
                             msg += "'";
                             (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), msg); }
#line 7477 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_use: TIDENT  */
#line 1239 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7483 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_use: TTHIS  */
#line 1240 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7489 "bison-chpl-lib.cpp"
    break;

  case 136: /* scalar_type: TBOOL  */
#line 1273 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TENUM  */
#line 1274 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7501 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TINT  */
#line 1275 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7507 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TUINT  */
#line 1276 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7513 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TREAL  */
#line 1277 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7519 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TIMAG  */
#line 1278 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7525 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TCOMPLEX  */
#line 1279 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7531 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TBYTES  */
#line 1280 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7537 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TSTRING  */
#line 1281 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7543 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TLOCALE  */
#line 1282 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7549 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TNOTHING  */
#line 1283 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7555 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TVOID  */
#line 1284 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7561 "bison-chpl-lib.cpp"
    break;

  case 152: /* do_stmt: TDO stmt  */
#line 1298 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7567 "bison-chpl-lib.cpp"
    break;

  case 153: /* do_stmt: block_stmt  */
#line 1299 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7573 "bison-chpl-lib.cpp"
    break;

  case 154: /* return_stmt: TRETURN TSEMI  */
#line 1304 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7584 "bison-chpl-lib.cpp"
    break;

  case 155: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1311 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7595 "bison-chpl-lib.cpp"
    break;

  case 157: /* $@3: %empty  */
#line 1322 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7603 "bison-chpl-lib.cpp"
    break;

  case 158: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@3 class_level_stmt  */
#line 1326 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7611 "bison-chpl-lib.cpp"
    break;

  case 159: /* $@4: %empty  */
#line 1330 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7619 "bison-chpl-lib.cpp"
    break;

  case 160: /* deprecated_class_level_stmt: TDEPRECATED $@4 class_level_stmt  */
#line 1334 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7627 "bison-chpl-lib.cpp"
    break;

  case 161: /* class_level_stmt: TSEMI  */
#line 1341 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7635 "bison-chpl-lib.cpp"
    break;

  case 162: /* class_level_stmt: inner_class_level_stmt  */
#line 1345 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7645 "bison-chpl-lib.cpp"
    break;

  case 163: /* $@5: %empty  */
#line 1350 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7652 "bison-chpl-lib.cpp"
    break;

  case 164: /* class_level_stmt: TPUBLIC $@5 inner_class_level_stmt  */
#line 1352 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7661 "bison-chpl-lib.cpp"
    break;

  case 165: /* $@6: %empty  */
#line 1356 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7668 "bison-chpl-lib.cpp"
    break;

  case 166: /* class_level_stmt: TPRIVATE $@6 inner_class_level_stmt  */
#line 1358 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7677 "bison-chpl-lib.cpp"
    break;

  case 174: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1376 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7686 "bison-chpl-lib.cpp"
    break;

  case 175: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1381 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7695 "bison-chpl-lib.cpp"
    break;

  case 176: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1386 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7704 "bison-chpl-lib.cpp"
    break;

  case 177: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1391 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7712 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_start: TFORWARDING  */
#line 1398 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7721 "bison-chpl-lib.cpp"
    break;

  case 179: /* extern_or_export: TEXTERN  */
#line 1405 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7727 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_or_export: TEXPORT  */
#line 1406 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7733 "bison-chpl-lib.cpp"
    break;

  case 181: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1411 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7743 "bison-chpl-lib.cpp"
    break;

  case 182: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1421 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7752 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1427 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7763 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1434 "chpl.ypp"
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
#line 7785 "bison-chpl-lib.cpp"
    break;

  case 185: /* $@7: %empty  */
#line 1452 "chpl.ypp"
  {
    // This will be consumed when building the first variable in the list.
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7794 "bison-chpl-lib.cpp"
    break;

  case 186: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@7 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1457 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7803 "bison-chpl-lib.cpp"
    break;

  case 187: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1465 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7811 "bison-chpl-lib.cpp"
    break;

  case 188: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1472 "chpl.ypp"
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
#line 7833 "bison-chpl-lib.cpp"
    break;

  case 189: /* loop_stmt: TWHILE expr do_stmt  */
#line 1490 "chpl.ypp"
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
#line 7850 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1503 "chpl.ypp"
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
#line 7867 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1516 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7875 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1520 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7883 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1524 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7891 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1528 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7899 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1532 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7907 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFOR expr do_stmt  */
#line 1536 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7915 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1540 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7923 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1544 "chpl.ypp"
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
#line 7945 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1562 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7953 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1566 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7961 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1570 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7969 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1574 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7977 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFORALL expr do_stmt  */
#line 1578 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7985 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1582 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7993 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1586 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8001 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1590 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8009 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1594 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8017 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1598 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8025 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1602 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8033 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1606 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8041 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1610 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8049 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1614 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8057 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1618 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8065 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1622 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8073 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1626 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8081 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1630 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8089 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1634 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8097 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1638 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8105 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1642 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8113 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1646 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8121 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1650 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8130 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1655 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8139 "bison-chpl-lib.cpp"
    break;

  case 223: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1663 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8149 "bison-chpl-lib.cpp"
    break;

  case 224: /* if_stmt: TIF expr TTHEN stmt  */
#line 1672 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8157 "bison-chpl-lib.cpp"
    break;

  case 225: /* if_stmt: TIF expr block_stmt  */
#line 1676 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8165 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1680 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8173 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1684 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8181 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1688 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8189 "bison-chpl-lib.cpp"
    break;

  case 229: /* if_stmt: TIF ifvar block_stmt  */
#line 1692 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8197 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1696 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8205 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1700 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8213 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1704 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8222 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1709 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1714 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8240 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1719 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8249 "bison-chpl-lib.cpp"
    break;

  case 236: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1727 "chpl.ypp"
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
#line 8268 "bison-chpl-lib.cpp"
    break;

  case 237: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1742 "chpl.ypp"
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
#line 8287 "bison-chpl-lib.cpp"
    break;

  case 238: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1760 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8295 "bison-chpl-lib.cpp"
    break;

  case 239: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1764 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8303 "bison-chpl-lib.cpp"
    break;

  case 240: /* ifc_formal_ls: ifc_formal  */
#line 1770 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8309 "bison-chpl-lib.cpp"
    break;

  case 241: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1771 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8315 "bison-chpl-lib.cpp"
    break;

  case 242: /* ifc_formal: ident_def  */
#line 1776 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8321 "bison-chpl-lib.cpp"
    break;

  case 255: /* implements_type_ident: implements_type_error_ident  */
#line 1794 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8333 "bison-chpl-lib.cpp"
    break;

  case 262: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1823 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8341 "bison-chpl-lib.cpp"
    break;

  case 263: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1827 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8349 "bison-chpl-lib.cpp"
    break;

  case 264: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1831 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8357 "bison-chpl-lib.cpp"
    break;

  case 265: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1838 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8365 "bison-chpl-lib.cpp"
    break;

  case 266: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1842 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8373 "bison-chpl-lib.cpp"
    break;

  case 267: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1846 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8381 "bison-chpl-lib.cpp"
    break;

  case 268: /* try_stmt: TTRY tryable_stmt  */
#line 1853 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8389 "bison-chpl-lib.cpp"
    break;

  case 269: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1857 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8397 "bison-chpl-lib.cpp"
    break;

  case 270: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1861 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8405 "bison-chpl-lib.cpp"
    break;

  case 271: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1865 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8413 "bison-chpl-lib.cpp"
    break;

  case 272: /* catch_expr_ls: %empty  */
#line 1871 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8419 "bison-chpl-lib.cpp"
    break;

  case 273: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1872 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8425 "bison-chpl-lib.cpp"
    break;

  case 274: /* catch_expr: TCATCH block_stmt  */
#line 1877 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8433 "bison-chpl-lib.cpp"
    break;

  case 275: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1881 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8441 "bison-chpl-lib.cpp"
    break;

  case 276: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1885 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8449 "bison-chpl-lib.cpp"
    break;

  case 277: /* catch_expr_inner: ident_def  */
#line 1892 "chpl.ypp"
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
#line 8466 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1905 "chpl.ypp"
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
#line 8483 "bison-chpl-lib.cpp"
    break;

  case 279: /* throw_stmt: TTHROW expr TSEMI  */
#line 1921 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8493 "bison-chpl-lib.cpp"
    break;

  case 280: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1929 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8501 "bison-chpl-lib.cpp"
    break;

  case 281: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1933 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8511 "bison-chpl-lib.cpp"
    break;

  case 282: /* when_stmt_ls: %empty  */
#line 1941 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8517 "bison-chpl-lib.cpp"
    break;

  case 283: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1942 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8523 "bison-chpl-lib.cpp"
    break;

  case 284: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1947 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8531 "bison-chpl-lib.cpp"
    break;

  case 285: /* when_stmt: TOTHERWISE stmt  */
#line 1951 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8540 "bison-chpl-lib.cpp"
    break;

  case 286: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1956 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8549 "bison-chpl-lib.cpp"
    break;

  case 287: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 1964 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8558 "bison-chpl-lib.cpp"
    break;

  case 288: /* manager_expr: expr TAS ident_def  */
#line 1969 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8566 "bison-chpl-lib.cpp"
    break;

  case 289: /* manager_expr: expr  */
#line 1973 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8574 "bison-chpl-lib.cpp"
    break;

  case 290: /* manager_expr_ls: manager_expr  */
#line 1979 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8580 "bison-chpl-lib.cpp"
    break;

  case 291: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 1980 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8586 "bison-chpl-lib.cpp"
    break;

  case 292: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 1985 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8594 "bison-chpl-lib.cpp"
    break;

  case 293: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1994 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8603 "bison-chpl-lib.cpp"
    break;

  case 294: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1999 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8614 "bison-chpl-lib.cpp"
    break;

  case 295: /* class_start: class_tag ident_def  */
#line 2010 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8622 "bison-chpl-lib.cpp"
    break;

  case 296: /* class_tag: TCLASS  */
#line 2016 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8628 "bison-chpl-lib.cpp"
    break;

  case 297: /* class_tag: TRECORD  */
#line 2017 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8634 "bison-chpl-lib.cpp"
    break;

  case 298: /* class_tag: TUNION  */
#line 2018 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8640 "bison-chpl-lib.cpp"
    break;

  case 299: /* opt_inherit: %empty  */
#line 2022 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8646 "bison-chpl-lib.cpp"
    break;

  case 300: /* opt_inherit: TCOLON expr_ls  */
#line 2023 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8652 "bison-chpl-lib.cpp"
    break;

  case 301: /* class_level_stmt_ls: %empty  */
#line 2027 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8661 "bison-chpl-lib.cpp"
    break;

  case 302: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2032 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8669 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2036 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8677 "bison-chpl-lib.cpp"
    break;

  case 304: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2043 "chpl.ypp"
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
#line 8698 "bison-chpl-lib.cpp"
    break;

  case 305: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2060 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8712 "bison-chpl-lib.cpp"
    break;

  case 306: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2073 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8720 "bison-chpl-lib.cpp"
    break;

  case 307: /* enum_ls: deprecated_enum_item  */
#line 2080 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 308: /* enum_ls: enum_ls TCOMMA  */
#line 2085 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8739 "bison-chpl-lib.cpp"
    break;

  case 309: /* $@8: %empty  */
#line 2091 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8748 "bison-chpl-lib.cpp"
    break;

  case 310: /* enum_ls: enum_ls TCOMMA $@8 deprecated_enum_item  */
#line 2096 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8757 "bison-chpl-lib.cpp"
    break;

  case 312: /* $@9: %empty  */
#line 2105 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8765 "bison-chpl-lib.cpp"
    break;

  case 313: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@9 enum_item  */
#line 2109 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8773 "bison-chpl-lib.cpp"
    break;

  case 314: /* $@10: %empty  */
#line 2113 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8781 "bison-chpl-lib.cpp"
    break;

  case 315: /* deprecated_enum_item: TDEPRECATED $@10 enum_item  */
#line 2117 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8789 "bison-chpl-lib.cpp"
    break;

  case 316: /* enum_item: ident_def  */
#line 2124 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8800 "bison-chpl-lib.cpp"
    break;

  case 317: /* enum_item: ident_def TASSIGN expr  */
#line 2131 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8813 "bison-chpl-lib.cpp"
    break;

  case 318: /* lambda_decl_start: TLAMBDA  */
#line 2143 "chpl.ypp"
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
#line 8829 "bison-chpl-lib.cpp"
    break;

  case 319: /* $@11: %empty  */
#line 2159 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8839 "bison-chpl-lib.cpp"
    break;

  case 320: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_lifetime_where $@11 function_body_stmt  */
#line 2165 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-6].functionParts);
      fp.name = STR("lambda");
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
#line 8861 "bison-chpl-lib.cpp"
    break;

  case 322: /* linkage_spec: linkage_spec_empty  */
#line 2188 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8867 "bison-chpl-lib.cpp"
    break;

  case 323: /* linkage_spec: TINLINE  */
#line 2189 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8874 "bison-chpl-lib.cpp"
    break;

  case 324: /* linkage_spec: TOVERRIDE  */
#line 2191 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8881 "bison-chpl-lib.cpp"
    break;

  case 325: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2197 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8889 "bison-chpl-lib.cpp"
    break;

  case 326: /* $@12: %empty  */
#line 2206 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name);
    }
#line 8899 "bison-chpl-lib.cpp"
    break;

  case 327: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@12 opt_function_body_stmt  */
#line 2212 "chpl.ypp"
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
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 328: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2232 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8932 "bison-chpl-lib.cpp"
    break;

  case 329: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2240 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 330: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2248 "chpl.ypp"
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
#line 8959 "bison-chpl-lib.cpp"
    break;

  case 331: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2259 "chpl.ypp"
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
#line 8974 "bison-chpl-lib.cpp"
    break;

  case 332: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2270 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8984 "bison-chpl-lib.cpp"
    break;

  case 333: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2279 "chpl.ypp"
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
#line 9000 "bison-chpl-lib.cpp"
    break;

  case 335: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2294 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9006 "bison-chpl-lib.cpp"
    break;

  case 338: /* fn_ident: ident_def TBANG  */
#line 2301 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9016 "bison-chpl-lib.cpp"
    break;

  case 379: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2359 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9022 "bison-chpl-lib.cpp"
    break;

  case 380: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2360 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9028 "bison-chpl-lib.cpp"
    break;

  case 381: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2361 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9034 "bison-chpl-lib.cpp"
    break;

  case 382: /* opt_formal_ls: %empty  */
#line 2365 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9040 "bison-chpl-lib.cpp"
    break;

  case 383: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2366 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9046 "bison-chpl-lib.cpp"
    break;

  case 384: /* req_formal_ls: TLP formal_ls TRP  */
#line 2370 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9052 "bison-chpl-lib.cpp"
    break;

  case 385: /* formal_ls_inner: formal  */
#line 2374 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9058 "bison-chpl-lib.cpp"
    break;

  case 386: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2375 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9064 "bison-chpl-lib.cpp"
    break;

  case 387: /* formal_ls: %empty  */
#line 2379 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9070 "bison-chpl-lib.cpp"
    break;

  case 388: /* formal_ls: formal_ls_inner  */
#line 2380 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9076 "bison-chpl-lib.cpp"
    break;

  case 389: /* formal: opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2385 "chpl.ypp"
  {
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9089 "bison-chpl-lib.cpp"
    break;

  case 390: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2394 "chpl.ypp"
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
#line 9104 "bison-chpl-lib.cpp"
    break;

  case 391: /* formal: opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2405 "chpl.ypp"
  {
    (yyval.expr) = VarArgFormal::build(BUILDER, LOC((yyloc)),
                             /*attributes*/ nullptr,
                             /*name*/ (yyvsp[-2].uniqueStr),
                             /*intent*/ (yyvsp[-3].intentTag),
                             toOwned((yyvsp[-1].expr)),
                             toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9118 "bison-chpl-lib.cpp"
    break;

  case 392: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
#line 2415 "chpl.ypp"
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
#line 9134 "bison-chpl-lib.cpp"
    break;

  case 393: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
#line 2428 "chpl.ypp"
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
#line 9149 "bison-chpl-lib.cpp"
    break;

  case 394: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type formal_var_arg_expr  */
#line 2440 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
  }
#line 9157 "bison-chpl-lib.cpp"
    break;

  case 395: /* opt_formal_intent_tag: %empty  */
#line 2446 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9166 "bison-chpl-lib.cpp"
    break;

  case 396: /* opt_formal_intent_tag: required_intent_tag  */
#line 2451 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9175 "bison-chpl-lib.cpp"
    break;

  case 397: /* required_intent_tag: TIN  */
#line 2458 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9181 "bison-chpl-lib.cpp"
    break;

  case 398: /* required_intent_tag: TINOUT  */
#line 2459 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9187 "bison-chpl-lib.cpp"
    break;

  case 399: /* required_intent_tag: TOUT  */
#line 2460 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9193 "bison-chpl-lib.cpp"
    break;

  case 400: /* required_intent_tag: TCONST TIN  */
#line 2461 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9199 "bison-chpl-lib.cpp"
    break;

  case 401: /* required_intent_tag: TCONST TREF  */
#line 2462 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9205 "bison-chpl-lib.cpp"
    break;

  case 402: /* required_intent_tag: TCONST  */
#line 2463 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9211 "bison-chpl-lib.cpp"
    break;

  case 403: /* required_intent_tag: TPARAM  */
#line 2464 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9217 "bison-chpl-lib.cpp"
    break;

  case 404: /* required_intent_tag: TREF  */
#line 2465 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9223 "bison-chpl-lib.cpp"
    break;

  case 405: /* required_intent_tag: TTYPE  */
#line 2466 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9229 "bison-chpl-lib.cpp"
    break;

  case 406: /* opt_this_intent_tag: %empty  */
#line 2470 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9235 "bison-chpl-lib.cpp"
    break;

  case 407: /* opt_this_intent_tag: TPARAM  */
#line 2471 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9241 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_this_intent_tag: TREF  */
#line 2472 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9247 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_this_intent_tag: TCONST TREF  */
#line 2473 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9253 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_this_intent_tag: TCONST  */
#line 2474 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9259 "bison-chpl-lib.cpp"
    break;

  case 411: /* opt_this_intent_tag: TTYPE  */
#line 2475 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9265 "bison-chpl-lib.cpp"
    break;

  case 412: /* proc_iter_or_op: TPROC  */
#line 2479 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9271 "bison-chpl-lib.cpp"
    break;

  case 413: /* proc_iter_or_op: TITER  */
#line 2480 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 414: /* proc_iter_or_op: TOPERATOR  */
#line 2481 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9283 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_ret_tag: %empty  */
#line 2485 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9289 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_ret_tag: TCONST  */
#line 2486 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 9295 "bison-chpl-lib.cpp"
    break;

  case 417: /* opt_ret_tag: TCONST TREF  */
#line 2487 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 418: /* opt_ret_tag: TREF  */
#line 2488 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 9307 "bison-chpl-lib.cpp"
    break;

  case 419: /* opt_ret_tag: TPARAM  */
#line 2489 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9313 "bison-chpl-lib.cpp"
    break;

  case 420: /* opt_ret_tag: TTYPE  */
#line 2490 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9319 "bison-chpl-lib.cpp"
    break;

  case 421: /* opt_throws_error: %empty  */
#line 2494 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 422: /* opt_throws_error: TTHROWS  */
#line 2495 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_function_body_stmt: TSEMI  */
#line 2498 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9337 "bison-chpl-lib.cpp"
    break;

  case 424: /* opt_function_body_stmt: function_body_stmt  */
#line 2499 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9343 "bison-chpl-lib.cpp"
    break;

  case 425: /* function_body_stmt: block_stmt_body  */
#line 2503 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9349 "bison-chpl-lib.cpp"
    break;

  case 426: /* function_body_stmt: return_stmt  */
#line 2504 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9355 "bison-chpl-lib.cpp"
    break;

  case 427: /* query_expr: TQUERIEDIDENT  */
#line 2508 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9361 "bison-chpl-lib.cpp"
    break;

  case 428: /* opt_lifetime_where: %empty  */
#line 2513 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9367 "bison-chpl-lib.cpp"
    break;

  case 429: /* opt_lifetime_where: TWHERE expr  */
#line 2515 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9373 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2517 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9379 "bison-chpl-lib.cpp"
    break;

  case 431: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2519 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9385 "bison-chpl-lib.cpp"
    break;

  case 432: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2521 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9391 "bison-chpl-lib.cpp"
    break;

  case 433: /* lifetime_components_expr: lifetime_expr  */
#line 2526 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9397 "bison-chpl-lib.cpp"
    break;

  case 434: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2528 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9403 "bison-chpl-lib.cpp"
    break;

  case 435: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2533 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9409 "bison-chpl-lib.cpp"
    break;

  case 436: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2535 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9415 "bison-chpl-lib.cpp"
    break;

  case 437: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2537 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9421 "bison-chpl-lib.cpp"
    break;

  case 438: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2539 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9427 "bison-chpl-lib.cpp"
    break;

  case 439: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2541 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9433 "bison-chpl-lib.cpp"
    break;

  case 440: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2543 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 441: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2545 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9445 "bison-chpl-lib.cpp"
    break;

  case 442: /* lifetime_ident: TIDENT  */
#line 2549 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9451 "bison-chpl-lib.cpp"
    break;

  case 443: /* lifetime_ident: TTHIS  */
#line 2550 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9457 "bison-chpl-lib.cpp"
    break;

  case 444: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2555 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9466 "bison-chpl-lib.cpp"
    break;

  case 445: /* type_alias_decl_stmt_start: TTYPE  */
#line 2563 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9474 "bison-chpl-lib.cpp"
    break;

  case 446: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2567 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9483 "bison-chpl-lib.cpp"
    break;

  case 447: /* type_alias_decl_stmt_start: TEXTERN TTYPE  */
#line 2572 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteLinkage(Decl::EXTERN);
  }
#line 9492 "bison-chpl-lib.cpp"
    break;

  case 448: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2580 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9500 "bison-chpl-lib.cpp"
    break;

  case 449: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2584 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9508 "bison-chpl-lib.cpp"
    break;

  case 450: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
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
#line 9530 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_init_type: %empty  */
#line 2611 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9536 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_init_type: TASSIGN type_level_expr  */
#line 2613 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9542 "bison-chpl-lib.cpp"
    break;

  case 453: /* opt_init_type: TASSIGN array_type  */
#line 2615 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9551 "bison-chpl-lib.cpp"
    break;

  case 454: /* var_decl_type: TPARAM  */
#line 2622 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9557 "bison-chpl-lib.cpp"
    break;

  case 455: /* var_decl_type: TCONST TREF  */
#line 2623 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9563 "bison-chpl-lib.cpp"
    break;

  case 456: /* var_decl_type: TREF  */
#line 2624 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9569 "bison-chpl-lib.cpp"
    break;

  case 457: /* var_decl_type: TCONST  */
#line 2625 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9575 "bison-chpl-lib.cpp"
    break;

  case 458: /* var_decl_type: TVAR  */
#line 2626 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9581 "bison-chpl-lib.cpp"
    break;

  case 459: /* $@13: %empty  */
#line 2631 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9590 "bison-chpl-lib.cpp"
    break;

  case 460: /* var_decl_stmt: TCONFIG $@13 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2635 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9599 "bison-chpl-lib.cpp"
    break;

  case 461: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2640 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9608 "bison-chpl-lib.cpp"
    break;

  case 462: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2648 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9616 "bison-chpl-lib.cpp"
    break;

  case 463: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2652 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9624 "bison-chpl-lib.cpp"
    break;

  case 464: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
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
#line 9644 "bison-chpl-lib.cpp"
    break;

  case 465: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
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
#line 9662 "bison-chpl-lib.cpp"
    break;

  case 466: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2692 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9670 "bison-chpl-lib.cpp"
    break;

  case 467: /* tuple_var_decl_component: ident_def  */
#line 2696 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9678 "bison-chpl-lib.cpp"
    break;

  case 468: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2700 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9686 "bison-chpl-lib.cpp"
    break;

  case 469: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2707 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9692 "bison-chpl-lib.cpp"
    break;

  case 470: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2709 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9698 "bison-chpl-lib.cpp"
    break;

  case 471: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2711 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9704 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_init_expr: %empty  */
#line 2717 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2718 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9716 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2719 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9722 "bison-chpl-lib.cpp"
    break;

  case 475: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2725 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9730 "bison-chpl-lib.cpp"
    break;

  case 476: /* ret_array_type: TLSBR TRSBR  */
#line 2729 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 9738 "bison-chpl-lib.cpp"
    break;

  case 477: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2733 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9746 "bison-chpl-lib.cpp"
    break;

  case 478: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2737 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 9754 "bison-chpl-lib.cpp"
    break;

  case 479: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2741 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9762 "bison-chpl-lib.cpp"
    break;

  case 480: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2745 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9770 "bison-chpl-lib.cpp"
    break;

  case 481: /* ret_array_type: TLSBR error TRSBR  */
#line 2749 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "invalid expression for domain of array return type");
  }
#line 9778 "bison-chpl-lib.cpp"
    break;

  case 482: /* opt_ret_type: %empty  */
#line 2755 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9784 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_ret_type: TCOLON type_level_expr  */
#line 2756 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9790 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_ret_type: TCOLON ret_array_type  */
#line 2757 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9796 "bison-chpl-lib.cpp"
    break;

  case 485: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2758 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9802 "bison-chpl-lib.cpp"
    break;

  case 486: /* opt_ret_type: error  */
#line 2759 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9808 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_type: %empty  */
#line 2764 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9814 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_type: TCOLON type_level_expr  */
#line 2765 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9820 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_type: TCOLON array_type  */
#line 2766 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9826 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2767 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9832 "bison-chpl-lib.cpp"
    break;

  case 491: /* opt_type: error  */
#line 2768 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9838 "bison-chpl-lib.cpp"
    break;

  case 492: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2789 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9846 "bison-chpl-lib.cpp"
    break;

  case 493: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2793 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9854 "bison-chpl-lib.cpp"
    break;

  case 494: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2797 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9862 "bison-chpl-lib.cpp"
    break;

  case 495: /* array_type: TLSBR error TRSBR  */
#line 2801 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 9870 "bison-chpl-lib.cpp"
    break;

  case 496: /* opt_formal_array_elt_type: %empty  */
#line 2807 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9876 "bison-chpl-lib.cpp"
    break;

  case 497: /* opt_formal_array_elt_type: type_level_expr  */
#line 2808 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9882 "bison-chpl-lib.cpp"
    break;

  case 498: /* opt_formal_array_elt_type: query_expr  */
#line 2809 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9888 "bison-chpl-lib.cpp"
    break;

  case 499: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2814 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9897 "bison-chpl-lib.cpp"
    break;

  case 500: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2819 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9905 "bison-chpl-lib.cpp"
    break;

  case 501: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2827 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9914 "bison-chpl-lib.cpp"
    break;

  case 502: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2832 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 503: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2836 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9930 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_formal_type: %empty  */
#line 2842 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9936 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_formal_type: TCOLON type_level_expr  */
#line 2843 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9942 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_formal_type: TCOLON query_expr  */
#line 2844 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9948 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2845 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9954 "bison-chpl-lib.cpp"
    break;

  case 508: /* opt_formal_type: TCOLON formal_array_type  */
#line 2846 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9960 "bison-chpl-lib.cpp"
    break;

  case 509: /* expr_ls: expr  */
#line 2852 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9966 "bison-chpl-lib.cpp"
    break;

  case 510: /* expr_ls: query_expr  */
#line 2853 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9972 "bison-chpl-lib.cpp"
    break;

  case 511: /* expr_ls: expr_ls TCOMMA expr  */
#line 2854 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9978 "bison-chpl-lib.cpp"
    break;

  case 512: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2855 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9984 "bison-chpl-lib.cpp"
    break;

  case 513: /* simple_expr_ls: expr  */
#line 2859 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9990 "bison-chpl-lib.cpp"
    break;

  case 514: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2860 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9996 "bison-chpl-lib.cpp"
    break;

  case 515: /* tuple_component: TUNDERSCORE  */
#line 2864 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10002 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_component: opt_try_expr  */
#line 2865 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10008 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_component: query_expr  */
#line 2866 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10014 "bison-chpl-lib.cpp"
    break;

  case 518: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2871 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10022 "bison-chpl-lib.cpp"
    break;

  case 519: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2875 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10030 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_actual_ls: %empty  */
#line 2881 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10036 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_actual_ls: actual_ls  */
#line 2882 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10042 "bison-chpl-lib.cpp"
    break;

  case 522: /* actual_ls: actual_expr  */
#line 2887 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10051 "bison-chpl-lib.cpp"
    break;

  case 523: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2892 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10061 "bison-chpl-lib.cpp"
    break;

  case 524: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2900 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10067 "bison-chpl-lib.cpp"
    break;

  case 525: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2901 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10073 "bison-chpl-lib.cpp"
    break;

  case 526: /* actual_expr: query_expr  */
#line 2902 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10079 "bison-chpl-lib.cpp"
    break;

  case 527: /* actual_expr: opt_try_expr  */
#line 2903 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10085 "bison-chpl-lib.cpp"
    break;

  case 528: /* ident_expr: ident_use  */
#line 2907 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10091 "bison-chpl-lib.cpp"
    break;

  case 529: /* ident_expr: scalar_type  */
#line 2908 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10097 "bison-chpl-lib.cpp"
    break;

  case 530: /* type_level_expr: sub_type_level_expr  */
#line 2920 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 10103 "bison-chpl-lib.cpp"
    break;

  case 531: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2922 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10109 "bison-chpl-lib.cpp"
    break;

  case 532: /* type_level_expr: TQUESTION  */
#line 2924 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10115 "bison-chpl-lib.cpp"
    break;

  case 538: /* sub_type_level_expr: TSINGLE expr  */
#line 2935 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10121 "bison-chpl-lib.cpp"
    break;

  case 539: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2937 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10127 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2939 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10133 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2941 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10139 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2943 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10149 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TATOMIC expr  */
#line 2949 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10155 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TSYNC expr  */
#line 2951 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10161 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TOWNED  */
#line 2954 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10167 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TOWNED expr  */
#line 2956 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10173 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TUNMANAGED  */
#line 2958 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10179 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2960 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10185 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TSHARED  */
#line 2962 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10191 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TSHARED expr  */
#line 2964 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10197 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TBORROWED  */
#line 2966 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10203 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TBORROWED expr  */
#line 2968 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10209 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TCLASS  */
#line 2971 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10215 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TRECORD  */
#line 2973 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10221 "bison-chpl-lib.cpp"
    break;

  case 555: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2978 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10234 "bison-chpl-lib.cpp"
    break;

  case 556: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2987 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10247 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TFOR expr TDO expr  */
#line 2996 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10259 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
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
#line 10277 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
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
#line 10295 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
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
#line 10313 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3046 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10326 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3055 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10339 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr: TFORALL expr TDO expr  */
#line 3064 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10351 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
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
#line 10369 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
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
#line 10387 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
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
#line 10405 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr: TLSBR expr_ls TRSBR expr  */
#line 3114 "chpl.ypp"
  {
    // TODO: We have to handle the possibility of [1..2, 3..4] here.
    if ((yyvsp[-2].exprList)->size() > 1) {
      const char* msg = "Invalid iterand expression";
      (yyval.expr) = context->raiseError((yylsp[-2]), msg);
    } else {
      auto iterand = context->consumeList((yyvsp[-2].exprList))[0].release();
      assert(iterand);
      (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), /*index*/ nullptr,
                              toOwned(iterand),
                              /*withClause*/ nullptr,
                              BlockStyle::IMPLICIT,
                              context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                              /*isExpressionLevel*/ true).release();
    }
  }
#line 10426 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3131 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10439 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3140 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10452 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
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
#line 10471 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
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
#line 10490 "bison-chpl-lib.cpp"
    break;

  case 572: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
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
#line 10504 "bison-chpl-lib.cpp"
    break;

  case 573: /* nil_expr: TNIL  */
#line 3199 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10510 "bison-chpl-lib.cpp"
    break;

  case 581: /* stmt_level_expr: io_expr TIO expr  */
#line 3215 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 582: /* opt_task_intent_ls: %empty  */
#line 3219 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10522 "bison-chpl-lib.cpp"
    break;

  case 583: /* opt_task_intent_ls: task_intent_clause  */
#line 3220 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10528 "bison-chpl-lib.cpp"
    break;

  case 584: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3225 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10538 "bison-chpl-lib.cpp"
    break;

  case 585: /* task_intent_ls: intent_expr  */
#line 3233 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10544 "bison-chpl-lib.cpp"
    break;

  case 586: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3234 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10550 "bison-chpl-lib.cpp"
    break;

  case 587: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3239 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10560 "bison-chpl-lib.cpp"
    break;

  case 588: /* forall_intent_ls: intent_expr  */
#line 3247 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10566 "bison-chpl-lib.cpp"
    break;

  case 589: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3248 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10572 "bison-chpl-lib.cpp"
    break;

  case 590: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
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
#line 10591 "bison-chpl-lib.cpp"
    break;

  case 591: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3268 "chpl.ypp"
  { auto ident = Identifier::build(BUILDER, LOC((yylsp[-2])), (yyvsp[-2].uniqueStr));
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), std::move(ident), toOwned((yyvsp[0].expr))).release();
  }
#line 10599 "bison-chpl-lib.cpp"
    break;

  case 592: /* intent_expr: expr TREDUCE ident_expr  */
#line 3272 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10607 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_var_prefix: TCONST  */
#line 3278 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10613 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_var_prefix: TIN  */
#line 3279 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10619 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_var_prefix: TCONST TIN  */
#line 3280 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10625 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_var_prefix: TREF  */
#line 3281 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10631 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_var_prefix: TCONST TREF  */
#line 3282 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10637 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_var_prefix: TVAR  */
#line 3283 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10643 "bison-chpl-lib.cpp"
    break;

  case 600: /* io_expr: io_expr TIO expr  */
#line 3289 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10649 "bison-chpl-lib.cpp"
    break;

  case 601: /* new_maybe_decorated: TNEW  */
#line 3294 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10655 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW TOWNED  */
#line 3296 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10661 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_maybe_decorated: TNEW TSHARED  */
#line 3298 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10667 "bison-chpl-lib.cpp"
    break;

  case 604: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3300 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10673 "bison-chpl-lib.cpp"
    break;

  case 605: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3302 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10679 "bison-chpl-lib.cpp"
    break;

  case 606: /* new_expr: new_maybe_decorated expr  */
#line 3308 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10687 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
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
#line 10703 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
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
#line 10719 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
#line 10736 "bison-chpl-lib.cpp"
    break;

  case 610: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
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
#line 10754 "bison-chpl-lib.cpp"
    break;

  case 611: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3369 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10762 "bison-chpl-lib.cpp"
    break;

  case 621: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 3388 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10770 "bison-chpl-lib.cpp"
    break;

  case 622: /* expr: expr TCOLON expr  */
#line 3392 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10778 "bison-chpl-lib.cpp"
    break;

  case 623: /* expr: expr TDOTDOT expr  */
#line 3396 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10787 "bison-chpl-lib.cpp"
    break;

  case 624: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 3401 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10796 "bison-chpl-lib.cpp"
    break;

  case 625: /* expr: expr TDOTDOT  */
#line 3406 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10805 "bison-chpl-lib.cpp"
    break;

  case 626: /* expr: TDOTDOT expr  */
#line 3411 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10815 "bison-chpl-lib.cpp"
    break;

  case 627: /* expr: TDOTDOTOPENHIGH expr  */
#line 3417 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10825 "bison-chpl-lib.cpp"
    break;

  case 628: /* expr: TDOTDOT  */
#line 3423 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10835 "bison-chpl-lib.cpp"
    break;

  case 629: /* opt_expr: %empty  */
#line 3454 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10841 "bison-chpl-lib.cpp"
    break;

  case 630: /* opt_expr: expr  */
#line 3455 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10847 "bison-chpl-lib.cpp"
    break;

  case 631: /* opt_try_expr: TTRY expr  */
#line 3459 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10853 "bison-chpl-lib.cpp"
    break;

  case 632: /* opt_try_expr: TTRYBANG expr  */
#line 3460 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10859 "bison-chpl-lib.cpp"
    break;

  case 633: /* opt_try_expr: expr  */
#line 3461 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10865 "bison-chpl-lib.cpp"
    break;

  case 639: /* call_base_expr: expr TBANG  */
#line 3478 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10873 "bison-chpl-lib.cpp"
    break;

  case 640: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3481 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10879 "bison-chpl-lib.cpp"
    break;

  case 643: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
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
#line 10895 "bison-chpl-lib.cpp"
    break;

  case 644: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
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
#line 10911 "bison-chpl-lib.cpp"
    break;

  case 645: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3512 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10919 "bison-chpl-lib.cpp"
    break;

  case 646: /* dot_expr: expr TDOT ident_use  */
#line 3519 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10925 "bison-chpl-lib.cpp"
    break;

  case 647: /* dot_expr: expr TDOT TTYPE  */
#line 3521 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10931 "bison-chpl-lib.cpp"
    break;

  case 648: /* dot_expr: expr TDOT TDOMAIN  */
#line 3523 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10937 "bison-chpl-lib.cpp"
    break;

  case 649: /* dot_expr: expr TDOT TLOCALE  */
#line 3525 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10943 "bison-chpl-lib.cpp"
    break;

  case 650: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3527 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10953 "bison-chpl-lib.cpp"
    break;

  case 651: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3533 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10963 "bison-chpl-lib.cpp"
    break;

  case 652: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3545 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10969 "bison-chpl-lib.cpp"
    break;

  case 653: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3547 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10977 "bison-chpl-lib.cpp"
    break;

  case 654: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3551 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10985 "bison-chpl-lib.cpp"
    break;

  case 655: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3555 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10993 "bison-chpl-lib.cpp"
    break;

  case 656: /* bool_literal: TFALSE  */
#line 3561 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10999 "bison-chpl-lib.cpp"
    break;

  case 657: /* bool_literal: TTRUE  */
#line 3562 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 11005 "bison-chpl-lib.cpp"
    break;

  case 658: /* str_bytes_literal: STRINGLITERAL  */
#line 3566 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11011 "bison-chpl-lib.cpp"
    break;

  case 659: /* str_bytes_literal: BYTESLITERAL  */
#line 3567 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11017 "bison-chpl-lib.cpp"
    break;

  case 662: /* literal_expr: INTLITERAL  */
#line 3573 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 11023 "bison-chpl-lib.cpp"
    break;

  case 663: /* literal_expr: REALLITERAL  */
#line 3574 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11029 "bison-chpl-lib.cpp"
    break;

  case 664: /* literal_expr: IMAGLITERAL  */
#line 3575 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11035 "bison-chpl-lib.cpp"
    break;

  case 665: /* literal_expr: CSTRINGLITERAL  */
#line 3576 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11041 "bison-chpl-lib.cpp"
    break;

  case 666: /* literal_expr: TNONE  */
#line 3577 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11047 "bison-chpl-lib.cpp"
    break;

  case 667: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3579 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11055 "bison-chpl-lib.cpp"
    break;

  case 668: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3583 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11063 "bison-chpl-lib.cpp"
    break;

  case 669: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3587 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11071 "bison-chpl-lib.cpp"
    break;

  case 670: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3591 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11079 "bison-chpl-lib.cpp"
    break;

  case 671: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3595 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11087 "bison-chpl-lib.cpp"
    break;

  case 672: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3599 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11096 "bison-chpl-lib.cpp"
    break;

  case 673: /* assoc_expr_ls: expr TALIAS expr  */
#line 3608 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11105 "bison-chpl-lib.cpp"
    break;

  case 674: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3613 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11115 "bison-chpl-lib.cpp"
    break;

  case 675: /* binary_op_expr: expr TPLUS expr  */
#line 3621 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11121 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr TMINUS expr  */
#line 3622 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11127 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TSTAR expr  */
#line 3623 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11133 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TDIVIDE expr  */
#line 3624 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11139 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3625 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11145 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3626 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11151 "bison-chpl-lib.cpp"
    break;

  case 681: /* binary_op_expr: expr TMOD expr  */
#line 3627 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11157 "bison-chpl-lib.cpp"
    break;

  case 682: /* binary_op_expr: expr TEQUAL expr  */
#line 3628 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11163 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3629 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11169 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3630 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11175 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3631 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11181 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TLESS expr  */
#line 3632 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11187 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TGREATER expr  */
#line 3633 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11193 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TBAND expr  */
#line 3634 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11199 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TBOR expr  */
#line 3635 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11205 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TBXOR expr  */
#line 3636 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11211 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TAND expr  */
#line 3637 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11217 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TOR expr  */
#line 3638 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11223 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TEXP expr  */
#line 3639 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11229 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TBY expr  */
#line 3640 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11235 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr TALIGN expr  */
#line 3641 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11241 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr THASH expr  */
#line 3642 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11247 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TDMAPPED expr  */
#line 3643 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11253 "bison-chpl-lib.cpp"
    break;

  case 698: /* unary_op_expr: TPLUS expr  */
#line 3647 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11259 "bison-chpl-lib.cpp"
    break;

  case 699: /* unary_op_expr: TMINUS expr  */
#line 3648 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11265 "bison-chpl-lib.cpp"
    break;

  case 700: /* unary_op_expr: TMINUSMINUS expr  */
#line 3649 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11271 "bison-chpl-lib.cpp"
    break;

  case 701: /* unary_op_expr: TPLUSPLUS expr  */
#line 3650 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11277 "bison-chpl-lib.cpp"
    break;

  case 702: /* unary_op_expr: TBANG expr  */
#line 3651 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11283 "bison-chpl-lib.cpp"
    break;

  case 703: /* unary_op_expr: expr TBANG  */
#line 3652 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11291 "bison-chpl-lib.cpp"
    break;

  case 704: /* unary_op_expr: TBNOT expr  */
#line 3655 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11297 "bison-chpl-lib.cpp"
    break;

  case 705: /* reduce_expr: expr TREDUCE expr  */
#line 3660 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11305 "bison-chpl-lib.cpp"
    break;

  case 706: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3664 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11313 "bison-chpl-lib.cpp"
    break;

  case 707: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3668 "chpl.ypp"
  {
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-2])), (yyvsp[-2].uniqueStr));
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), std::move(ident), toOwned((yyvsp[0].expr))).release();
  }
#line 11322 "bison-chpl-lib.cpp"
    break;

  case 708: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3673 "chpl.ypp"
  {
    auto ident = Identifier::build(BUILDER, LOC((yylsp[-2])), (yyvsp[-2].uniqueStr));
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), std::move(ident), toOwned((yyvsp[0].expr))).release();
  }
#line 11331 "bison-chpl-lib.cpp"
    break;

  case 709: /* scan_expr: expr TSCAN expr  */
#line 3681 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11339 "bison-chpl-lib.cpp"
    break;

  case 710: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3685 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11347 "bison-chpl-lib.cpp"
    break;

  case 711: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3689 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11355 "bison-chpl-lib.cpp"
    break;

  case 712: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3693 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11363 "bison-chpl-lib.cpp"
    break;


#line 11367 "bison-chpl-lib.cpp"

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
