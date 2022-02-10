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
  YYSYMBOL_access_control = 194,           /* access_control  */
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
  YYSYMBOL_lambda_decl_expr = 267,         /* lambda_decl_expr  */
  YYSYMBOL_linkage_spec_empty = 268,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 269,             /* linkage_spec  */
  YYSYMBOL_fn_decl_stmt_complete = 270,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 271,             /* fn_decl_stmt  */
  YYSYMBOL_272_11 = 272,                   /* $@11  */
  YYSYMBOL_fn_decl_stmt_inner = 273,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 274,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 275,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 276,                 /* fn_ident  */
  YYSYMBOL_op_ident = 277,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 278,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 279,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 280,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 281,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 282,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 283,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 284,                /* formal_ls  */
  YYSYMBOL_formal = 285,                   /* formal  */
  YYSYMBOL_opt_formal_intent_tag = 286,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 287,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 288,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 289,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 290,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 291,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 292,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 293,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 294,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 295,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 296, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 297,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 298,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 299,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 300, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 301, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 302, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 303,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 304,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 305,            /* var_decl_stmt  */
  YYSYMBOL_306_12 = 306,                   /* $@12  */
  YYSYMBOL_var_decl_stmt_inner_ls = 307,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 308,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 309, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 310, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 311,            /* opt_init_expr  */
  YYSYMBOL_ret_array_type = 312,           /* ret_array_type  */
  YYSYMBOL_opt_ret_type = 313,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 314,                 /* opt_type  */
  YYSYMBOL_array_type = 315,               /* array_type  */
  YYSYMBOL_opt_formal_array_elt_type = 316, /* opt_formal_array_elt_type  */
  YYSYMBOL_formal_array_type = 317,        /* formal_array_type  */
  YYSYMBOL_opt_formal_type = 318,          /* opt_formal_type  */
  YYSYMBOL_expr_ls = 319,                  /* expr_ls  */
  YYSYMBOL_simple_expr_ls = 320,           /* simple_expr_ls  */
  YYSYMBOL_tuple_component = 321,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 322,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 323,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 324,                /* actual_ls  */
  YYSYMBOL_actual_expr = 325,              /* actual_expr  */
  YYSYMBOL_ident_expr = 326,               /* ident_expr  */
  YYSYMBOL_type_level_expr = 327,          /* type_level_expr  */
  YYSYMBOL_sub_type_level_expr = 328,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 329,                 /* for_expr  */
  YYSYMBOL_cond_expr = 330,                /* cond_expr  */
  YYSYMBOL_nil_expr = 331,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 332,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 333,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 334,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 335,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 336,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 337,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 338,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 339,          /* task_var_prefix  */
  YYSYMBOL_io_expr = 340,                  /* io_expr  */
  YYSYMBOL_new_maybe_decorated = 341,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 342,                 /* new_expr  */
  YYSYMBOL_let_expr = 343,                 /* let_expr  */
  YYSYMBOL_expr = 344,                     /* expr  */
  YYSYMBOL_opt_expr = 345,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 346,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 347,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 348,           /* call_base_expr  */
  YYSYMBOL_call_expr = 349,                /* call_expr  */
  YYSYMBOL_dot_expr = 350,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 351,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 352,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 353,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 354,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 355,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 356,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 357,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 358,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 359,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 360       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 325 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include other helper functions for the parser
  #include "parser-help.h"


#line 483 "bison-chpl-lib.cpp"

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
#define YYLAST   20733

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  180
/* YYNRULES -- Number of rules.  */
#define YYNRULES  718
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
       0,   605,   605,   609,   610,   615,   616,   624,   628,   636,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     659,   660,   661,   662,   671,   680,   703,   710,   718,   719,
     720,   721,   722,   723,   724,   725,   739,   754,   762,   774,
     785,   797,   809,   820,   836,   838,   837,   846,   845,   856,
     857,   861,   881,   896,   911,   927,   928,   930,   935,   936,
     941,   959,   963,   967,   978,   997,   998,  1002,  1006,  1010,
    1015,  1019,  1023,  1033,  1038,  1044,  1051,  1056,  1063,  1074,
    1075,  1079,  1083,  1090,  1098,  1107,  1114,  1122,  1134,  1141,
    1145,  1152,  1158,  1167,  1168,  1172,  1181,  1185,  1189,  1193,
    1197,  1201,  1210,  1211,  1215,  1216,  1217,  1218,  1219,  1220,
    1223,  1224,  1225,  1226,  1227,  1228,  1240,  1241,  1252,  1253,
    1254,  1255,  1256,  1257,  1258,  1259,  1260,  1261,  1262,  1263,
    1264,  1265,  1266,  1267,  1268,  1269,  1270,  1274,  1275,  1276,
    1277,  1278,  1279,  1280,  1281,  1282,  1283,  1284,  1285,  1292,
    1293,  1294,  1295,  1299,  1300,  1304,  1311,  1321,  1323,  1322,
    1331,  1330,  1341,  1345,  1351,  1351,  1357,  1357,  1366,  1367,
    1368,  1369,  1370,  1371,  1372,  1376,  1381,  1386,  1391,  1398,
    1406,  1407,  1411,  1420,  1426,  1434,  1453,  1452,  1465,  1472,
    1490,  1503,  1516,  1520,  1524,  1528,  1532,  1536,  1540,  1544,
    1562,  1566,  1570,  1574,  1578,  1582,  1586,  1590,  1594,  1598,
    1602,  1606,  1610,  1614,  1618,  1622,  1626,  1630,  1634,  1638,
    1642,  1646,  1650,  1655,  1663,  1672,  1676,  1680,  1684,  1688,
    1692,  1696,  1700,  1704,  1709,  1714,  1719,  1727,  1742,  1760,
    1762,  1767,  1768,  1773,  1778,  1779,  1780,  1781,  1782,  1783,
    1784,  1785,  1786,  1787,  1788,  1789,  1790,  1802,  1803,  1804,
    1805,  1814,  1815,  1819,  1821,  1823,  1828,  1830,  1832,  1837,
    1841,  1845,  1849,  1856,  1857,  1861,  1865,  1869,  1876,  1889,
    1905,  1913,  1917,  1926,  1927,  1931,  1935,  1940,  1948,  1953,
    1957,  1964,  1965,  1969,  1978,  1983,  1994,  2001,  2002,  2003,
    2007,  2008,  2012,  2016,  2020,  2027,  2044,  2057,  2064,  2069,
    2076,  2075,  2088,  2090,  2089,  2098,  2097,  2108,  2115,  2127,
    2135,  2139,  2140,  2142,  2147,  2157,  2154,  2182,  2190,  2198,
    2209,  2220,  2229,  2247,  2248,  2252,  2253,  2254,  2263,  2264,
    2265,  2266,  2267,  2268,  2269,  2270,  2271,  2272,  2273,  2274,
    2275,  2276,  2277,  2278,  2279,  2280,  2281,  2282,  2283,  2284,
    2285,  2286,  2287,  2288,  2292,  2293,  2294,  2295,  2296,  2297,
    2298,  2299,  2300,  2301,  2302,  2303,  2308,  2309,  2313,  2314,
    2315,  2319,  2320,  2324,  2328,  2329,  2333,  2334,  2338,  2347,
    2358,  2368,  2380,  2392,  2400,  2404,  2412,  2413,  2414,  2415,
    2416,  2417,  2418,  2419,  2420,  2424,  2425,  2426,  2427,  2428,
    2429,  2433,  2434,  2435,  2439,  2440,  2441,  2442,  2443,  2444,
    2448,  2449,  2452,  2453,  2457,  2458,  2462,  2467,  2468,  2470,
    2472,  2474,  2479,  2481,  2486,  2488,  2490,  2492,  2494,  2496,
    2498,  2503,  2504,  2508,  2516,  2520,  2525,  2533,  2537,  2544,
    2565,  2566,  2568,  2576,  2577,  2578,  2579,  2580,  2585,  2584,
    2593,  2601,  2605,  2612,  2628,  2645,  2649,  2653,  2660,  2662,
    2664,  2671,  2672,  2673,  2678,  2682,  2686,  2690,  2694,  2698,
    2702,  2709,  2710,  2711,  2712,  2713,  2718,  2719,  2720,  2721,
    2722,  2742,  2746,  2750,  2754,  2761,  2762,  2763,  2767,  2772,
    2780,  2785,  2789,  2796,  2797,  2798,  2799,  2800,  2806,  2807,
    2808,  2809,  2813,  2814,  2818,  2819,  2820,  2824,  2828,  2835,
    2836,  2840,  2845,  2854,  2855,  2856,  2857,  2861,  2862,  2873,
    2875,  2877,  2883,  2884,  2885,  2886,  2887,  2888,  2890,  2892,
    2894,  2896,  2902,  2904,  2907,  2909,  2911,  2913,  2915,  2917,
    2919,  2921,  2924,  2926,  2931,  2940,  2949,  2957,  2971,  2985,
    2999,  3008,  3017,  3025,  3039,  3053,  3067,  3084,  3093,  3102,
    3117,  3135,  3153,  3161,  3162,  3163,  3164,  3165,  3166,  3167,
    3168,  3173,  3174,  3178,  3187,  3188,  3192,  3201,  3202,  3206,
    3221,  3225,  3232,  3233,  3234,  3235,  3236,  3237,  3241,  3242,
    3247,  3249,  3251,  3253,  3255,  3261,  3268,  3272,  3276,  3280,
    3287,  3297,  3298,  3299,  3300,  3301,  3302,  3303,  3304,  3305,
    3306,  3310,  3314,  3319,  3324,  3329,  3335,  3341,  3373,  3374,
    3378,  3379,  3380,  3384,  3385,  3386,  3387,  3396,  3397,  3400,
    3401,  3402,  3406,  3418,  3430,  3437,  3439,  3441,  3443,  3445,
    3451,  3464,  3465,  3469,  3473,  3480,  3481,  3485,  3486,  3490,
    3491,  3492,  3493,  3494,  3495,  3496,  3497,  3501,  3505,  3509,
    3513,  3517,  3526,  3531,  3540,  3541,  3542,  3543,  3544,  3545,
    3546,  3547,  3548,  3549,  3550,  3551,  3552,  3553,  3554,  3555,
    3556,  3557,  3558,  3559,  3560,  3561,  3562,  3566,  3567,  3568,
    3569,  3570,  3571,  3574,  3578,  3582,  3586,  3590,  3597,  3601,
    3605,  3609,  3617,  3618,  3619,  3620,  3621,  3622,  3623
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
  "when_stmt_ls", "when_stmt", "manager_expr", "manager_expr_ls",
  "manage_stmt", "class_decl_stmt", "class_start", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
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

#define YYPACT_NINF (-1077)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-719)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1077,   100,  3857, -1077,    74,    60, -1077, -1077, -1077, -1077,
   -1077, -1077,  5257,   140,   293,   122, 15083,   255, 20443,   140,
   12223,   268,   259,   239,   293,  5257, 12223,   409,  5257,   207,
   20530, -1077,    44,   382,  9093, 10485, 10485, -1077,  9266,   396,
     430,   292, -1077,   417, 20530, 20530, 20530,   321,  3371, 10658,
     448, 12223, 12223,    49, -1077,   459,   480, 12223, -1077, 15083,
   -1077, 12223,   531,   411,   330,   371,   538, 20617, -1077, 10833,
    8572, 12223, 10658, 15083, 12223,   471,   541,   445,  5257,   570,
   12223,   573,  7528,  7528, -1077,   582, -1077, 15083, -1077,   591,
    9266, 12223, -1077, 12223, -1077, 12223, -1077, -1077, 14600, 12223,
   -1077, 12223, -1077, -1077, -1077,  4207,  7703,  9441, 12223, -1077,
    5082, -1077, -1077, -1077, -1077,   410, -1077,    41, -1077, -1077,
   -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077,
   -1077,  7876, -1077, 11006, -1077, -1077, -1077, -1077, -1077,   613,
   -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077,   535, 20530,
   -1077, 20168,   451, -1077,   224, -1077, -1077,    57,   400, -1077,
   20530,  3371, -1077,   530, -1077,   534, -1077, -1077,   540,   547,
     557, 12223,   564,   574, 19779,  2653,   511,   576,   578, -1077,
   -1077,   512, -1077, -1077, -1077, -1077, -1077,   458, -1077, -1077,
   19779,   563,  5257, -1077, -1077,   579, 12223, -1077, -1077, 12223,
   12223, 12223, 20530, -1077, 12223, 10833, 10833,   650,   515, -1077,
   -1077, -1077, -1077,   -28,   527, -1077, -1077,   584, 16747, -1077,
     220, -1077,   588, -1077,   309, 19779, -1077,  2412,   627,  8747,
   -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077,
   -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077,
   -1077, -1077, -1077, -1077,   585, -1077, -1077, -1077, 20530,   577,
      37, 16353,    13,  3591,    13, 16313, 20530, 20530,    73, 15553,
   -1077, -1077,   672,  8747,   586,   440,  5257,   175,    57,  1646,
      75,    95, -1077,  5257, -1077, -1077, 16787, -1077,    66, 17296,
     621, -1077,   595,   598, -1077, 16787,   -28,   621, -1077,  8747,
    1786,  1786, -1077, -1077,   316, 19779, 12223, 12223, -1077, 19779,
     594, 17403, -1077, 16787,   -28, 19779,   604,  8747, -1077, 19779,
   17448, -1077, -1077, -1077, -1077,   -28,    37, 16787, 17488,   178,
     178,   724,   621,   621,   142, -1077, -1077,  4382,   160, -1077,
   12223, -1077,   -49,   -42, -1077,   -29,   117, 17640,   -61,   724,
     752, -1077,  4557, 12223, -1077, 12223,   697, -1077, -1077,  1878,
     257,   535, 19779,   296, 20530, 10833,   607, -1077,   611,   775,
     660,   203, -1077, -1077, -1077, -1077, -1077, -1077,   698, -1077,
   -1077, -1077,    68,   700, -1077, -1077, -1077, 14432,   666,   329,
   -1077,   341,   537, -1077, 12223,   647, 12223, 12223, 12223, 10485,
   10485, 12223,   553, 12223, 12223, 12223, 12223, 12223,   587, 14600,
   12223, 12223, 12223, 12223, 12223, 12223, 12223, 12223, 12223, 12223,
   12223, 12223, 12223, 12223, 12223, 12223,   725, -1077, -1077, -1077,
   -1077, -1077,  9614,  9614, -1077, -1077, -1077, -1077,  9614, -1077,
   -1077,  9614,  9614,  8747,  8747, 10485, 10485,  8399, -1077, -1077,
   16827, 16979, 17680,   624,    53, 20530,  4732, -1077, 10485,    37,
    3371, -1077, 12223, -1077,  2412, -1077, 20530,   690, -1077, -1077,
     672, 12223,   675, -1077,   628,   658, -1077, -1077, -1077,   759,
   10833, -1077,  5432, 10485, -1077,   636, -1077,    37,  5607, 10485,
   -1077,    37, -1077,    37, 10485, -1077,    37,   684,   685,  5257,
     777,  5257, -1077,   779, 12223,   749,   651,  8747, 20530, -1077,
   -1077,    58, -1077, -1077, -1077, -1077, -1077, -1077,   770,   674,
     653, -1077,  3551, -1077,    64, -1077,  1646, -1077, -1077,   113,
   -1077, 12396,   705, 12223,  3371, -1077, -1077, 12223, -1077, 20255,
   12223, 12223, -1077,   659, -1077, -1077, 10833, -1077, 19779, 19779,
   -1077,    32, -1077,  8747,   662, -1077,   802,   802, -1077, -1077,
   -1077, -1077, -1077, -1077,  9789, -1077, 17720,  8051, -1077,  8226,
   -1077,  5257,   663, 10485,  9964,  4032,   664, 12223, 11179, -1077,
   -1077,   337, -1077,  4907, -1077,   354, 17872,   361, 16505, 20530,
    7355,  7355, -1077,   535,   673,   220, -1077,   128,   699,  1226,
   -1077, -1077, 20530, 12223,   312, -1077, -1077, -1077, 12569,   739,
   -1077,   676,   235, -1077,   704, -1077,   706,   710,   713,   715,
     722, -1077,   726,   721,   727,   729,   730,   456,   733,   732,
     734, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077,
   -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077,
   -1077, -1077, -1077, -1077, 12223, -1077,   741,   746,   745,   676,
   -1077,   676, -1077, 12742, -1077, 20530, -1077, -1077, 17912, 19931,
   19931,   408, -1077,   408, -1077,   408,  1034,   488,  1630,   192,
     -28,   178, -1077,   554, -1077, -1077, -1077, -1077, -1077,   724,
    2599,   408,  1180,  1180, 19931,  1180,  1180,  1330,   178,  2599,
   20007,  1330,   621,   621,   178,   724,   731,   737,   738,   742,
     743,   744,   716,   719, -1077,   408, -1077,   408,    86, -1077,
   -1077, -1077,   124, -1077,   902, 19855,   529, 12915, 10485, 13088,
   10485, 12223,  8747, 10485, 15365,   723,   140, 17957, -1077,   368,
   19779, -1077, 18064,  8747, -1077,  8747, 12223,   129,  9266, 19779,
      38, 17019,  8399, -1077,  9266, 19779,    31, 16550, -1077, -1077,
      13, 16595, -1077, 12223, 12223,   871,  5257,   877, 18109,  5257,
   17059, 20530, -1077,   136, -1077,   143, -1077, -1077, -1077, 20530,
    1067, -1077,  1646,   768,    21,   169,  1646,    75,    -5,    42,
   12223, 12223,  7182, -1077, -1077,   399,  8920, -1077, 19779, -1077,
   -1077, -1077, 20530, 18149, 18301, -1077, -1077, 19779,   747,   156,
     751, -1077,  2184, -1077, -1077, -1077, -1077, -1077, -1077, -1077,
   -1077,  5257,   -18, 17211, -1077, -1077, 19779,  5257, 19779, -1077,
   18342, -1077, -1077, 12223, -1077,    71,  3662, 12223, -1077, 11352,
    7355,  7355, -1077, 12223,   550, 12223,   552,   569, 12223, 10137,
     581,   372, -1077, -1077, -1077, 18383,   769,   760,   756, -1077,
    3371, -1077,  8747,   758,  2340, 20530, -1077, 19779, 20356,  7007,
   -1077, -1077,   362, -1077,    21,   175, -1077, 18501, -1077, 15470,
   -1077, -1077, -1077,   384, -1077,   757,   761, -1077, -1077, -1077,
   -1077, -1077, -1077, -1077, -1077, -1077, -1077,  8399, -1077, -1077,
   -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077,
      52, 10485, 10485, 12223,   883, 18542, 12223,   884, 18583,   173,
     762, 18701,  8747,    37,    37, -1077, -1077, -1077, -1077, -1077,
   16787, -1077, 15629,  5782, -1077,  5957, -1077,   205, -1077, 15705,
    6132, -1077,    37,  6307, -1077,    37, -1077,    37, -1077,    37,
   19779, 19779,  5257, -1077,  5257, 12223, -1077,  5257,   898,   774,
     776, 20530,   584,   768, -1077,   208, 11527,    -8,   269, 12223,
     -20, -1077, -1077,   705,   765,    70, -1077, -1077, -1077,   773,
     778, -1077,  6482, 10833, -1077, -1077, -1077, 20530, -1077,   789,
     584, -1077,  6657,   771,  6832,   772, -1077, 12223, -1077, -1077,
    7355, -1077, 18742,    85, 17251,   385,   784,  7355, -1077, 12223,
   -1077, -1077,  2556,   394,   270, -1077,   943, -1077, -1077, -1077,
    1530, -1077, -1077, -1077, -1077,   783, 14299,    80, -1077,   780,
   -1077,   812,   814,   676,   676, -1077, -1077, -1077,   705,   305,
     323, 18860, 13261, 13434, 18936, 13607, 13780, -1077, 13953, 14126,
     326, -1077, -1077, -1077,  5257,  9266, 19779,  9266, 19779,  8399,
   -1077,  5257,  9266, 19779, -1077,  9266, 19779, -1077, -1077, -1077,
   -1077, -1077, 19779,   924,  5257, -1077, -1077, -1077, -1077,    -8,
     768, 10312, -1077, -1077, -1077,   -35, 10833, -1077, -1077, -1077,
     127, -1077,   134, -1077,   524, 19012, -1077, -1077, -1077, -1077,
   -1077, 10485, 15224,  8747,  8747,  5257, -1077,    72,   787, 12223,
   -1077,  9266, -1077, 19779,  5257,  9266, -1077, 19779,  5257, 19779,
     315, 11700,  7355,  7355,  7355,  7355, -1077, -1077, -1077, 19095,
   19779,  3425, -1077, -1077,   796, -1077,  3049, -1077, -1077, -1077,
     405, 14770,   152, -1077, -1077, -1077, -1077, 12223, 12223, 12223,
   12223, 12223, 12223, 12223, 12223, -1077, 18109, 15781, 15857, -1077,
   18109, 15933, 16009,  5257, -1077, -1077, -1077,    -8, 11875,    89,
   -1077, 19779, -1077, 12223,   269,   127,   127,   127,   127,   127,
     127,   269, 19171, -1077,   506,   790,   794,   627, -1077,   584,
   19779, 16085, -1077, 16161, -1077, -1077, -1077, 19779,   422,   797,
     438,   805, 12223, -1077, -1077,  3049, -1077, -1077,   481, -1077,
   -1077, -1077, 19247, 19323, 19399, 19475, 19551, 19627,  5257,  5257,
    5257,  5257, -1077, -1077, -1077, -1077, -1077, -1077,   338, 10485,
   14911, 19779, -1077, -1077, -1077, -1077, -1077, -1077, -1077,   826,
   14126,   810,   811, -1077,  5257,  5257, -1077, -1077, -1077, -1077,
   19779, -1077, 12223, 12223, 12223, 12223, 12223, 12223, 18109, 18109,
   18109, 18109, 19703, -1077, -1077,   646, -1077, -1077, 18109, 18109,
   12050, -1077
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   116,   661,   662,   663,   657,
     658,   664,     0,   581,   102,   137,   550,   144,   552,   581,
       0,   143,   458,   456,   102,     0,     0,    47,     0,   261,
     138,   181,   180,   655,     0,     0,     0,   179,     0,   142,
      55,   262,   322,   139,     0,     0,     0,     0,     0,     0,
     146,     0,     0,   600,   572,   665,   147,     0,   323,   544,
     453,     0,     0,     0,   166,   164,   141,   553,   455,     0,
       0,     0,     0,   548,     0,     0,   145,     0,     0,   117,
       0,   656,     0,     0,   444,   140,   299,   546,   457,   148,
       0,     0,   714,     0,   716,     0,   717,   718,   627,     0,
     715,   712,   531,   162,   713,     0,     0,     0,     0,     4,
       0,     5,     9,    10,    44,     0,    49,    58,    11,    64,
      12,    13,    14,    15,    28,   527,   528,    21,    50,   163,
     173,     0,   182,   628,   174,    16,    30,    29,    18,     0,
     256,    17,   619,    20,    34,    31,    32,   172,   300,     0,
     170,     0,   616,   321,     0,   168,   324,   414,   405,   171,
       0,     0,   169,   633,   612,   529,   613,   534,   532,     0,
       0,     0,   617,   618,     0,   533,     0,   634,   635,   636,
     659,   660,   611,   536,   535,   614,   615,     0,    27,    22,
     542,     0,     0,   582,   103,     0,     0,   552,   138,     0,
       0,     0,     0,   553,     0,     0,     0,     0,   616,   633,
     532,   617,   618,   551,   533,   634,   635,     0,   581,   445,
       0,   454,     0,    19,     0,   512,    45,   320,     0,   519,
     110,   118,   130,   124,   123,   132,   113,   122,   133,   119,
     134,   111,   135,   128,   121,   129,   127,   125,   126,   112,
     114,   120,   131,   136,     0,   115,   188,   446,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    56,    58,   519,     0,     0,     0,   394,   414,     0,
       0,     0,   461,     0,   154,    39,     0,   291,     0,   290,
     699,   604,   601,   602,   603,     0,   545,   700,     7,   519,
     320,   320,   426,   509,     0,   508,     0,     0,   155,   632,
       0,     0,    42,     0,   549,   537,     0,   519,    43,   543,
       0,   269,   273,   270,   273,   547,     0,     0,     0,   701,
     703,   625,   698,   697,     0,    61,    65,     0,     0,   514,
       0,   516,     0,     0,   515,     0,     0,   508,     0,   626,
       0,     6,     0,     0,    59,     0,     0,   458,   178,     0,
     657,   300,   629,   186,     0,     0,     0,   296,     0,   315,
     317,     0,   308,   312,   412,   413,   411,   332,   415,   418,
     417,   419,     0,   409,   406,   407,   410,     0,   450,     0,
     447,     0,   530,    33,     0,   605,     0,     0,     0,     0,
       0,     0,   702,     0,     0,     0,     0,     0,     0,   624,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   364,   371,   372,   373,
     368,   370,     0,     0,   366,   369,   367,   365,     0,   375,
     374,     0,     0,   519,   519,     0,     0,     0,    35,    23,
       0,     0,     0,     0,     0,     0,     0,    36,     0,     0,
       0,    24,     0,    37,   320,   297,     0,   180,   298,    48,
      58,     0,   527,   525,     0,   520,   521,   526,   307,     0,
       0,   198,     0,     0,   197,     0,   206,     0,     0,     0,
     204,     0,   214,     0,     0,   212,     0,     0,     0,     0,
     230,     0,   364,   226,     0,     0,     0,     0,     0,   240,
      25,   401,   396,   397,   398,   402,   403,   404,   394,   387,
       0,   384,     0,   395,     0,   465,     0,   466,   468,     0,
     490,     0,   471,     0,     0,   153,    38,     0,   293,     0,
       0,     0,    40,     0,   167,   165,     0,    95,   630,   631,
     156,     0,    41,     0,     0,   280,   271,   272,   191,   190,
      26,    63,    62,    66,     0,   666,     0,     0,   651,     0,
     653,     0,     0,     0,     0,     0,     0,     0,     0,   670,
       8,     0,    52,     0,    93,     0,    89,     0,    73,     0,
       0,    79,   175,   300,     0,     0,   185,   267,   301,     0,
     306,   313,     0,     0,   310,   305,   416,   485,     0,   420,
     408,   381,   104,   359,   118,   357,   124,   123,   107,   122,
     119,   362,   134,   105,   135,   121,   125,   106,   108,   120,
     136,   356,   338,   341,   339,   340,   363,   351,   342,   355,
     347,   345,   358,   361,   346,   344,   349,   354,   343,   348,
     352,   353,   350,   360,     0,   335,     0,   109,     0,   381,
     336,   381,   333,     0,   449,     0,   443,   460,   580,   694,
     693,   696,   705,   704,   709,   708,   690,   687,   688,   689,
     621,   677,   116,     0,   647,   648,   117,   646,   645,   622,
     681,   692,   686,   684,   695,   685,   683,   675,   680,   682,
     691,   674,   678,   679,   676,   623,     0,     0,     0,     0,
       0,     0,     0,     0,   707,   706,   711,   710,   592,   593,
     595,   597,     0,   584,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   668,   267,   581,   581,   194,     0,
     513,    46,     0,     0,   539,     0,     0,     0,     0,   556,
       0,     0,     0,   207,     0,   562,     0,     0,   205,   215,
       0,     0,   213,     0,     0,   229,     0,   225,     0,     0,
       0,     0,   538,     0,   243,     0,   241,   399,   400,     0,
     394,   383,     0,   503,   427,     0,   469,     0,   151,   152,
     150,   149,     0,   489,   488,   612,     0,   463,   610,   462,
     292,   289,     0,     0,     0,   644,   511,   510,     0,     0,
       0,   540,     0,   274,   667,   620,   652,   517,   654,   518,
     222,     0,     0,     0,   669,   220,   566,     0,   672,   671,
       0,    54,    53,     0,    88,     0,     0,     0,    81,     0,
       0,    79,    51,   356,   338,   341,   339,   340,   349,   348,
     350,     0,   376,   377,    68,    67,    80,     0,     0,   302,
       0,   264,     0,     0,   320,     0,   316,   318,     0,     0,
     484,   483,   612,   421,   427,   394,   331,     0,   337,     0,
     327,   328,   452,   612,   448,     0,     0,   101,    99,   100,
      98,    97,    96,   642,   643,   594,   596,     0,   583,   137,
     144,   143,   142,   139,   146,   147,   141,   145,   140,   148,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   459,   189,   523,   524,   522,
       0,   224,     0,     0,   196,     0,   195,     0,   587,     0,
       0,   202,     0,     0,   200,     0,   210,     0,   208,     0,
     238,   237,     0,   232,     0,     0,   228,     0,   234,     0,
     266,     0,     0,   503,   385,     0,     0,   471,     0,     0,
       0,   467,   470,   471,     0,     0,   472,   473,   288,     0,
       0,   282,     0,     0,   281,   284,   541,     0,   275,   278,
       0,   223,     0,     0,     0,     0,   221,     0,    94,    91,
       0,    90,    76,    75,    74,     0,     0,     0,   176,     0,
     177,   302,   320,     0,     0,   295,   160,   166,   164,   294,
     320,   303,   157,   314,   311,     0,   475,     0,   325,     0,
     334,   104,   106,   381,   381,   649,   650,   585,   471,   633,
     633,     0,     0,     0,     0,     0,     0,   266,     0,     0,
       0,   193,   192,   199,     0,     0,   555,     0,   554,     0,
     586,     0,     0,   561,   203,     0,   560,   201,   211,   209,
     231,   227,   571,   233,     0,    60,   263,   242,   239,   471,
     503,     0,   506,   505,   507,   612,   378,   390,   388,   441,
       0,   442,   429,   432,     0,   428,   424,   425,   319,   464,
     494,     0,   668,   519,   519,     0,   286,     0,     0,     0,
     276,     0,   218,   568,     0,     0,   216,   567,     0,   673,
       0,     0,     0,    79,     0,    79,    82,    85,    71,    70,
      69,   320,   183,   187,   268,   158,   320,   304,   480,   478,
     612,   668,     0,   382,   329,   330,   589,     0,     0,     0,
       0,     0,     0,     0,     0,   268,   559,     0,     0,   588,
     565,     0,     0,     0,   236,   391,   389,   471,   495,     0,
     380,   379,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   492,   612,     0,     0,   287,   285,     0,
     279,     0,   219,     0,   217,    92,    78,    77,     0,     0,
       0,     0,     0,   184,   265,   320,   161,   479,   612,   422,
     326,   423,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   235,   393,   392,   497,   498,   500,   612,     0,
     668,   431,   433,   434,   437,   438,   439,   435,   436,   430,
       0,   606,   607,   277,     0,     0,    84,    87,    83,    86,
      72,   159,     0,     0,     0,     0,     0,     0,   558,   557,
     564,   563,     0,   499,   501,   612,   608,   609,   570,   569,
     495,   502
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1077, -1077, -1077,    -1,  -275,  2383,   609, -1077, -1077, -1077,
    -198, -1077, -1077,   -23,   707, -1077,  -936,  2437,   625,  -578,
   -1077,  -822, -1077, -1077,   145, -1077, -1077, -1077,   952, -1077,
    2907,   -88,  -378, -1077,  -595,  2771,  -927,   -39, -1077, -1077,
    -848, -1077, -1077,   363, -1077, -1077, -1077, -1077, -1077, -1077,
   -1077, -1077,   214, -1077,   893, -1077, -1077,    23,  2057, -1077,
   -1077, -1077, -1077,   669, -1077,     0, -1077, -1077, -1077, -1077,
     449, -1077, -1077, -1077,  -102, -1077,  -336,  -815, -1077, -1077,
   -1077, -1077,   126, -1077, -1077,  -575,   583, -1077, -1077, -1077,
     637, -1077, -1077, -1077, -1077,   111,  -376,  -134,  -788, -1006,
    -609, -1077, -1077,   131,   219,   484, -1077, -1077, -1077,   735,
   -1077, -1077,  -139,    63,   130,  -174,  -166,  -813, -1077, -1077,
   -1077,   345, -1077,  -205,   888, -1077,  -156,   483,   228,  -506,
    -935,  -970, -1077,  -496,  -660,  -956, -1076,  -928,   -27, -1077,
     163, -1077,  -263,  -486,  -523,   510,  -468, -1077, -1077, -1077,
     886, -1077,   -13, -1077, -1077,  -155, -1077,  -734, -1077, -1077,
   -1077,  1023,  1353,   -12, -1077,     7,  1656, -1077,  1860,  1901,
   -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077, -1077,  -440
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   336,   110,   767,   112,   113,   464,   227,
     114,   115,   116,   117,   356,   118,   119,   120,   337,   856,
     587,   857,   121,   122,   584,   585,   123,   124,   195,   655,
     280,   125,   255,   126,   793,   285,   127,  1021,  1205,  1136,
     128,   301,   300,   129,   130,   131,   132,   133,   134,   595,
     135,   136,   920,   137,   268,   138,   775,   776,   207,   140,
     141,   142,   143,   556,   813,   990,   144,   145,   809,   985,
     287,   288,   146,   147,   148,   149,   366,   864,   150,   151,
     371,   868,   372,   865,   602,   373,   208,   153,   154,   155,
     156,  1142,   157,   158,   658,   659,   852,   853,   854,  1087,
     876,   278,   519,   520,   521,   522,   523,   387,   377,   382,
     874,  1210,  1098,   473,   970,  1092,  1093,  1094,   159,   160,
     389,   390,   664,   161,   162,   220,   281,   282,   528,   529,
     797,   871,   609,   532,   794,  1226,  1084,   967,   338,   224,
     342,   343,   474,   475,   476,   209,   164,   165,   166,   167,
     210,   169,   192,   193,   722,   487,   937,   723,   724,   170,
     171,   211,   212,   174,   363,   477,   214,   176,   215,   216,
     179,   180,   181,   182,   348,   183,   184,   185,   186,   187
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     190,   109,   518,   882,   213,   391,   217,   726,   218,   657,
     506,   660,   851,   870,   225,   460,  1022,   272,   938,  1006,
     785,   773,   261,   263,   265,   594,   269,   866,   784,   469,
     810,   361,  1088,   808,  1096,  1079,   543,   286,  1099,   289,
     290,   442,   304,  1097,  1012,   295,   283,   296,  1001,   297,
     880,  -261,   881,   530,   554,   256,  1139,   305,   309,   311,
     313,   314,   315,   795,   940,   530,   319,   291,   320,   607,
     283,   933,    70,  1165,   682,   325,   530,   310,   327,   328,
     346,   329,   968,   330,   378,   578,   331,   332,   353,   333,
    -504,   485,  1227,   305,   309,   347,   349,   567,  -262,   283,
       3,   733,   485,  1146,   569,   283,   777,  -283,   491,   493,
     496,  -504,   402,   344,   579,  -504,  -244,   796,  1101,   359,
     408,   362,  1122,  -486,   568,  -486,   354,   292,   733,  -481,
    1089,   570,   303,   485,   895,   504,   379,  1229,  -504,   969,
     872,   472,  1086,   533,  1166,   293,   571,   380,   778,  -283,
     456,   485,  1167,   257,  1264,   355,  -486,   992,  1123,   395,
    -481,  1223,   294,  1037,  1022,   573,   381,   229,   303,   341,
     303,  1207,  1022,  -481,   686,   499,   896,  -486,  -245,   454,
     712,   713,  -486,   456,   190,   472,  -481,   450,   451,   452,
     572,   576,   319,   305,   347,   883,  1131,   531,  -486,   574,
    -486,   456,   511,  -283,   470,   459,  1096,   456,   456,   531,
     398,   472,   537,   608,   273,  1097,   574,   309,   546,  1128,
     531,  -486,   929,   512,   398,  -486,   574,   513,   734,   472,
    1091,   982,  1224,  -481,  -486,   574,   456,   485,  -481,   188,
    -486,   534,   456,   456,    70,  1102,   919,    23,   260,   262,
     264,   514,  1173,   661,   515,  1141,    62,   858,   593,   786,
     191,   309,  1005,   574,  1230,   516,   741,   399,   303,   303,
     897,   400,  1089,   983,  1263,   546,   965,  1172,   374,   465,
    1174,   399,   745,  1022,   517,   400,   787,   309,  1206,   961,
    1023,   973,   575,   861,   548,   549,   194,   898,   375,    60,
     862,  1199,   931,  1201,   739,   309,   564,   188,   376,   960,
      68,  -251,   726,   561,  1271,   786,   962,  1209,   402,   745,
     688,   345,   456,   406,  -250,  1159,   408,   984,   566,   221,
     411,   565,   402,   403,   802,    88,   563,   406,   598,   407,
     408,   586,   971,   588,   411,   468,  1047,    42,  -386,   604,
    -320,  1059,   418,   305,   786,   472,   472,  1251,   422,   423,
     424,  1090,  1233,  1234,  1235,  1236,  1237,  1238,   219,    86,
    -320,  1082,  1091,    58,   605,  -110,  1014,   -57,  1060,   229,
    -320,  1080,   668,  -116,   669,   670,   671,   673,   675,   676,
     860,   677,   678,   679,   680,   681,   -57,   689,   690,   691,
     692,   693,   694,   695,   696,   697,   698,   699,   700,   701,
     702,   703,   704,   705,  1038,   -57,   745,   226,   -56,   472,
     309,   309,  1120,  -482,  1144,  1145,   309,   383,   303,   309,
     309,   309,   309,   715,   717,   725,  1050,   -56,  -259,   707,
     708,   470,  1183,  1134,   -57,   709,   737,  -487,   710,   711,
     740,  -591,  -249,   747,  -482,   462,   -56,   726,  -309,   742,
    -487,  1007,   546,  -496,   273,   472,  -474,  -482,   305,  -590,
     749,   751,   745,  -246,   463,   665,   755,   757,  -591,   384,
    -482,   547,   761,  -309,  -496,   -56,  1195,   534,  -496,   768,
     385,  -487,   770,   277,   666,   309,  -590,  -474,  1085,  1155,
     833,   657,   188,   660,  -253,   518,   667,   837,   831,   386,
    -474,  -496,   163,   270,   534,  -257,   271,  -487,  1007,   834,
     398,   798,   163,  -474,  -487,   289,   838,  -482,   803,   804,
    -451,  1007,  -482,   925,   807,   163,  -254,  1008,   163,   298,
     534,   309,  -476,   303,  1198,  -487,  1200,   445,   402,  -451,
    1126,   446,   807,   406,  -491,   309,   408,   309,  1140,  1133,
     411,   823,   807,   826,  -487,   828,   830,  -491,  1007,  -487,
    -474,   316,  -487,  -476,   344,  -474,   344,   399,   855,   855,
     352,   400,   563,   299,  1007,   152,  -476,  1246,   163,  1020,
     682,   867,   163,   163,  -248,   152,  -112,  -252,  -491,  -476,
     518,   942,   945,  1248,  -117,   947,   949,   683,   152,   806,
     456,   152,   508,   672,   674,   163,  -577,   317,   912,   726,
     163,   684,   446,  -640,  -491,  -640,  -258,   806,   402,  -260,
     341,  -491,   341,   406,  1184,   407,   408,   806,  -247,  -716,
     411,  -717,   877,  -716,   472,  -717,  -476,  -255,   418,  1175,
     685,  -476,  -491,   398,   422,   423,   424,   472,  -718,   714,
     716,   152,  -718,   544,   545,   152,   152,   993,   995,   364,
    -713,  -491,   736,  1208,  -713,  1176,  -491,  1177,  1178,  -491,
     365,  1179,  1180,   443,  -641,   444,  -641,  -640,   152,  -640,
     686,   321,   323,   152,  -493,  -574,   687,   750,   392,  -637,
    1228,  -637,   163,   756,  1013,  -573,   455,  -493,   760,  -639,
     399,  -639,   393,   394,   400,   749,   915,   755,   918,   768,
     309,   921,   826,   923,   924,  -638,   885,  -638,   886,  -578,
     817,   309,   819,   309,   930,   447,   932,  1020,  -493,  -579,
     725,  -576,   939,  -575,   449,  1034,   471,   999,   688,   480,
     928,   950,   951,   461,   456,   478,   398,   354,   507,   550,
     580,   402,  1228,   589,  -493,   975,   406,   540,   407,   408,
     541,  -493,  1265,   411,   472,   152,   553,   599,   315,   319,
     347,   418,   600,   601,   309,   603,   163,   822,   606,   424,
     610,   663,  -493,   163,   706,   408,   732,   511,   504,   257,
     743,   744,  1228,   977,   745,   504,   927,   746,   752,   763,
     764,  -493,   766,   399,   769,   771,  -493,   400,   512,  -493,
     780,   586,   513,   812,   772,  1002,   781,  1004,   855,   855,
     796,   329,   805,   330,   472,   811,   332,   333,   821,   827,
    1185,  1186,  1027,   859,   873,   546,   514,   163,   875,   515,
     309,   350,  -137,  -113,  -144,   303,  1020,   347,  -143,   152,
     516,  -111,   163,  -142,   402,   403,   152,   404,   405,   406,
    -139,   407,   408,  -114,  -146,  -147,   411,  -141,  -145,   517,
    -140,   878,  -148,   417,   418,   725,  -115,   421,   168,   893,
     422,   423,   424,   879,   894,   922,   887,   662,   168,   673,
     715,  1041,   888,   889,  1044,   682,   952,   890,   891,   892,
     309,   168,   954,   966,   168,  1007,  1042,  1045,   981,   899,
     152,  1056,   900,  1058,   986,  1010,  1011,   901,  1063,  1015,
    1035,  1066,   303,  1074,  1109,   152,  1036,  1048,   198,  1075,
    1100,  1076,   914,  1072,   917,  1103,  1114,  1118,   902,  1127,
    1104,  1135,  -110,  1143,  -112,   903,  1107,  1095,  1138,  1163,
    1189,  1204,  1247,  1241,   168,   904,   163,  1242,   168,   168,
    1249,   305,  1174,   905,  1266,  1267,   222,   583,   998,   505,
    1113,  1137,  1117,   326,  1077,  1119,   800,  1108,   855,   906,
    1033,   168,   163,   557,  1024,  1129,   168,  1130,   163,   964,
     596,   907,   779,  1211,  1028,   686,  1029,  1239,  1232,   163,
     884,   163,   908,   524,   972,   472,   472,   799,   909,   358,
       0,     0,     0,   504,   504,   172,     0,   504,   504,  1083,
    1056,  1058,     0,  1063,  1066,   172,  1113,  1117,     0,   152,
       0,     0,  1156,  1157,     0,  1158,   303,   725,   172,  1160,
    1161,   172,     0,  1162,  1169,     0,     0,   504,     0,   504,
       0,     0,     0,     0,     0,   152,   398,     0,     0,   347,
       0,   152,     0,     0,  1171,     0,     0,     0,   168,     0,
       0,   163,   152,     0,   152,   163,     0,     0,     0,  1182,
     826,   309,   309,   163,   511,     0,     0,  1190,     0,  1191,
       0,   172,     0,  1193,     0,   172,   172,     0,     0,  1197,
     855,   855,   855,   855,     0,   512,     0,     0,     0,   513,
       0,     0,     0,   399,     0,   672,   714,   400,   172,   826,
       0,     0,     0,   172,     0,  1156,  1212,  1213,  1160,  1214,
    1215,  1216,  1217,   514,   303,     0,   515,     0,    62,  1170,
       0,     0,     0,     0,   152,     0,     0,   516,   152,     0,
       0,  1231,   168,     0,     0,     0,   152,     0,     0,   168,
       0,     0,     0,     0,   402,   403,   517,   404,   405,   406,
       0,   407,   408,   409,     0,   410,   411,   412,   413,     0,
    1250,   415,   416,   417,   418,   419,     0,   421,     0,     0,
     422,   423,   424,     0,     0,     0,  1258,  1259,  1260,  1261,
       0,   425,   398,     0,     0,   172,     0,  1262,   826,     0,
       0,     0,     0,   168,     0,     0,     0,   863,  1117,     0,
       0,  1225,  1268,  1269,   910,     0,     0,     0,   168,     0,
    1258,  1259,  1260,  1261,  1268,  1269,     0,     0,  -302,     0,
       0,     0,  -302,  -302,     0,     0,     0,  -302,  1117,     0,
       0,     0,  -302,     0,  -302,  -302,     0,     0,     0,   399,
    -302,     0,     0,   400,     0,     0,   163,  -302,     0,   163,
    -302,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1225,     0,     0,     0,     0,     0,   172,
    -302,     0,     0,  -302,     0,  -302,   172,  -302,     0,  -302,
    -302,     0,  -302,     0,  -302,     0,  -302,     0,     0,     0,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
       0,   163,   411,  1225,     0,  -302,     0,   163,  -302,   417,
     418,  -302,   168,   421,     0,     0,   422,   423,   424,   152,
       0,     0,   152,     0,     0,   173,     0,   425,     0,     0,
     172,     0,   398,     0,     0,   173,     0,     0,   168,     0,
       0,     0,     0,     0,   168,   172,     0,     0,   173,     0,
       0,   173,     0,     0,     0,   168,     0,   168,     0,     0,
       0,  -302,     0,     0,     0,     0,     0,  -302,     0,     0,
       0,     0,     0,     0,   152,     0,     0,     0,     0,     0,
     152,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,  1039,  1040,   400,     0,     0,     0,     0,     0,     0,
       0,   173,     0,     0,     0,   173,   173,     0,     0,     0,
       0,     0,     0,   163,     0,   163,     0,     0,     0,     0,
     163,     0,     0,   163,     0,     0,     0,   168,   173,     0,
       0,   168,   163,   173,   163,     0,     0,   163,     0,   168,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   172,
       0,     0,   411,     0,     0,     0,     0,     0,     0,     0,
     418,     0,   163,     0,     0,     0,   422,   423,   424,     0,
       0,     0,   163,     0,   163,   172,     0,     0,     0,     0,
       0,   172,     0,     0,     0,     0,   152,     0,   152,     0,
       0,     0,   172,   152,   172,     0,   152,     0,     0,     0,
       0,     0,     0,     0,     0,   152,     0,   152,     0,     0,
     152,     0,     0,     0,     0,   173,     0,     0,     0,     0,
       0,     0,   465,     0,     0,     0,    22,    23,     0,     0,
       0,  1016,     0,     0,   163,   152,   466,     0,    31,   467,
       0,   163,     0,     0,    37,   152,     0,   152,     0,     0,
       0,    42,     0,     0,   163,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   172,     0,     0,     0,   172,     0,
       0,     0,     0,     0,     0,     0,   172,    58,     0,    60,
       0,   350,     0,  1017,     0,   163,  1018,     0,   468,     0,
      68,     0,     0,     0,   163,     0,     0,     0,   163,   173,
       0,     0,     0,     0,     0,     0,   173,   152,     0,    84,
       0,     0,    86,     0,   152,    88,     0,     0,     0,   230,
       0,     0,   168,     0,     0,   168,     0,   152,   175,     0,
       0,     0,   398,   231,   232,     0,   233,     0,   175,     0,
       0,   234,     0,   163,     0,     0,     0,     0,     0,     0,
     235,   175,     0,     0,   175,     0,   236,     0,   152,     0,
     173,     0,   237,     0,     0,   103,   238,   152,     0,   239,
       0,   152,     0,     0,     0,   173,     0,   168,     0,   240,
       0,     0,     0,   168,     0,     0,   241,   242,     0,   399,
       0,     0,     0,   400,   243,     0,     0,     0,   163,   163,
     163,   163,     0,   244,   175,     0,     0,     0,   175,   175,
       0,     0,   245,   246,     0,   247,   152,   248,     0,   249,
       0,     0,   250,     0,   163,   163,   251,   525,     0,   252,
       0,   175,   253,     0,     0,     0,   175,     0,     0,     0,
     402,   403,     0,     0,   405,   406,     0,   407,   408,     0,
       0,     0,   411,     0,     0,     0,     0,     0,     0,   172,
     418,     0,   172,     0,     0,     0,   422,   423,   424,     0,
       0,   152,   152,   152,   152,     0,     0,     0,   465,   173,
       0,     0,    22,    23,     0,     0,     0,     0,   526,   168,
       0,   168,   466,     0,    31,   467,   168,   152,   152,   168,
      37,     0,     0,     0,     0,   173,     0,    42,   168,     0,
     168,   173,     0,   168,   172,     0,     0,     0,   175,     0,
     172,     0,   173,     0,   173,     0,     0,     0,     0,     0,
       0,     0,   177,    58,     0,    60,     0,     0,   168,     0,
       0,     0,   177,     0,   468,     0,    68,     0,   168,     0,
     168,     0,     0,     0,     0,   177,     0,     0,   177,     0,
     396,     0,     0,     0,     0,    84,     0,   397,    86,     0,
       0,    88,     0,   178,     0,     0,     0,     0,     0,     0,
     398,     0,     0,   178,     0,   590,     0,     0,     0,     0,
       0,     0,     0,     0,   173,     0,   178,     0,   173,   178,
       0,     0,   175,     0,     0,     0,   173,     0,   177,   175,
     168,     0,   177,   177,     0,     0,     0,   168,     0,     0,
       0,   591,     0,     0,     0,     0,   172,     0,   172,     0,
     168,     0,     0,   172,     0,   177,   172,   399,     0,     0,
     177,   400,     0,     0,     0,   172,     0,   172,     0,   178,
     172,     0,     0,   178,   178,     0,     0,     0,     0,     0,
       0,   168,     0,   175,     0,     0,     0,     0,     0,     0,
     168,     0,   401,     0,   168,   172,   178,     0,   175,     0,
       0,   178,     0,     0,     0,   172,     0,   172,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,     0,   410,
     411,   412,   413,   414,     0,   415,   416,   417,   418,   419,
     420,   421,     0,   592,   422,   423,   424,     0,     0,   168,
       0,     0,   177,     0,     0,   425,     0,     0,     0,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139,
       0,     0,     0,     0,     0,     0,     0,   172,     0,     0,
       0,     0,   139,     0,   172,   139,     0,     0,     0,     0,
       0,     0,     0,   178,     0,     0,     0,   172,     0,     0,
       0,     0,     0,     0,   168,   168,   168,   168,     0,     0,
       0,     0,   175,     0,     0,     0,     0,     0,     0,   173,
       0,     0,   173,     0,     0,     0,     0,     0,   172,     0,
     168,   168,     0,     0,     0,   139,   177,   172,   175,     0,
       0,   172,     0,   177,   175,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   175,     0,   175,     0,     0,
       0,     0,   139,     0,     0,     0,     0,   139,     0,     0,
       0,     0,     0,     0,   173,     0,     0,   178,     0,     0,
     173,     0,     0,     0,   178,     0,   172,   230,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   177,     0,     0,
       0,   231,   232,     0,   233,     0,     0,     0,     0,   234,
       0,     0,   177,     0,     0,     0,     0,     0,   235,     0,
       0,     0,     0,     0,   236,     0,     0,   175,     0,     0,
     237,   175,     0,     0,   238,     0,     0,   239,   178,   175,
       0,   172,   172,   172,   172,     0,     0,   240,     0,   139,
       0,     0,     0,   178,   241,   242,     0,     0,     0,     0,
       0,     0,   243,     0,     0,     0,     0,   172,   172,     0,
       0,   244,     0,     0,     0,     0,     0,     0,     0,     0,
     245,   246,     0,   247,     0,   248,   173,   249,   173,     0,
     250,     0,     0,   173,   251,     0,   173,   252,     0,     0,
     253,     0,     0,     0,     0,   173,     0,   173,     0,     0,
     173,     0,     0,     0,     0,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   139,     0,   173,     0,     0,     0,     0,
     139,     0,   177,     0,     0,   173,     0,   173,   177,     0,
       0,     0,     0,     0,   456,     0,   987,   178,     0,   177,
       0,   177,   465,     0,     0,     0,    22,    23,     0,     0,
       0,  1016,     0,     0,     0,     0,   466,     0,    31,   467,
       0,     0,     0,   178,    37,   111,     0,     0,     0,   178,
       0,    42,     0,     0,   139,   189,     0,     0,     0,     0,
     178,     0,   178,     0,     0,     0,     0,   173,   223,   139,
       0,   228,     0,     0,   173,     0,     0,    58,     0,    60,
       0,    62,   175,  1017,     0,   175,  1018,   173,   468,     0,
      68,   177,     0,     0,   465,   177,     0,     0,    22,    23,
       0,     0,     0,   177,     0,     0,     0,     0,   466,    84,
      31,   467,    86,     0,     0,    88,    37,     0,   173,     0,
       0,   318,     0,    42,     0,     0,     0,   173,     0,     0,
       0,   173,   178,     0,     0,     0,   178,   175,   -55,     0,
       0,     0,     0,   175,   178,     0,   284,     0,   111,    58,
       0,    60,     0,   351,     0,    64,     0,   -55,    65,     0,
     468,     0,    68,     0,     0,   103,     0,     0,     0,   284,
       0,  1019,     0,   139,     0,     0,   173,     0,     0,   322,
     324,    84,     0,     0,    86,     0,     0,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139,
       0,     0,     0,     0,     0,   139,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,     0,   139,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   173,   173,   173,   173,   448,     0,   103,   465,     0,
       0,     0,    22,    23,     0,     0,     0,  1016,     0,   175,
       0,   175,   466,     0,    31,   467,   175,   173,   173,   175,
      37,     0,     0,     0,     0,     0,     0,    42,   175,     0,
     175,     0,     0,   175,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   177,     0,   139,   177,
       0,   398,   139,    58,     0,    60,     0,    62,   175,  1017,
     139,     0,  1018,     0,   468,     0,    68,     0,   175,     0,
     175,     0,     0,     0,   457,     0,     0,     0,     0,   510,
       0,     0,     0,     0,     0,    84,   535,   178,    86,     0,
     178,    88,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   177,     0,     0,     0,     0,     0,   177,   399,     0,
       0,     0,   400,     0,     0,     0,     0,   284,   284,   284,
     284,   284,   284,     0,     0,   500,   503,     0,     0,     0,
     175,     0,   509,     0,     0,     0,     0,   175,     0,     0,
     111,   103,   178,   284,     0,   284,     0,  1132,   178,     0,
     175,     0,   284,     0,     0,   111,     0,     0,     0,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,     0,
     284,   411,   412,   413,     0,     0,   415,   416,   417,   418,
       0,   175,   421,   284,   284,   422,   423,   424,     0,     0,
     175,     0,     0,     0,   175,     0,   425,     0,   426,   427,
     428,   429,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   177,     0,   177,     0,     0,     0,     0,
     177,     0,     0,   177,     0,     0,     0,     0,     0,  -598,
       0,     0,   177,     0,   177,     0,     0,   177,     0,   175,
       0,     0,   441,   139,     0,  -637,   139,  -637,     0,     0,
       0,     0,     0,     0,   178,     0,   178,     0,     0,   111,
       0,   178,   177,   312,   178,     0,     0,     0,     0,     0,
       0,     0,   177,   178,   177,   178,     0,     0,   178,     0,
       0,     0,     0,     0,     0,   535,     0,     0,     0,     0,
       0,   535,     0,     0,   175,   175,   175,   175,   139,     0,
       0,     0,   765,   178,   139,     0,     0,     0,     0,     0,
       0,     0,     0,   178,     0,   178,   284,     0,     0,     0,
     175,   175,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   177,     0,     0,     0,     0,     0,
       0,   177,     0,     0,   284,     0,     0,     0,   284,     0,
     284,     0,     0,   284,   177,     0,     0,   254,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   274,   275,   276,   820,   178,     0,     0,   825,     0,
       0,     0,   178,     0,     0,   177,   111,     0,     0,     0,
       0,     0,     0,     0,   177,   178,     0,     0,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     139,     0,   139,     0,     0,     0,     0,   139,     0,     0,
     139,     0,     0,     0,     0,     0,   178,     0,     0,   139,
       0,   139,     0,     0,   139,   178,     0,     0,     0,   178,
       0,     0,     0,   177,     0,     0,     0,     0,     0,     0,
       0,   481,   484,   486,   490,   492,   495,     0,     0,   139,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   139,
       0,   139,     0,     0,     0,     0,   367,   536,   370,   538,
       0,     0,     0,     0,   178,     0,   542,   388,     0,     0,
       0,   465,     0,     0,     0,    22,    23,     0,   177,   177,
     177,   177,     0,     0,   552,   466,     0,    31,   467,     0,
       0,     0,     0,    37,     0,     0,     0,   558,   559,     0,
      42,     0,     0,     0,   177,   177,     0,     0,     0,   453,
       0,   139,     0,     0,     0,     0,     0,     0,   139,   178,
     178,   178,   178,     0,     0,     0,    58,     0,    60,     0,
       0,   139,  1017,     0,     0,  1018,     0,   468,     0,    68,
       0,     0,     0,     0,     0,   178,   178,     0,     0,   953,
       0,     0,   956,     0,     0,     0,     0,     0,    84,     0,
       0,    86,   139,     0,    88,   479,     0,     0,     0,     0,
       0,   139,     0,   497,   498,   139,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   527,   284,   284,     0,
       0,     0,     0,   284,   284,     0,     0,   284,   284,     0,
       0,     0,     0,     0,   991,     0,     0,   958,     0,     0,
     996,     0,     0,     0,   103,     0,     0,     0,     0,     0,
     139,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     738,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   988,
       0,     0,     0,     0,     0,     0,     0,     0,   753,     0,
       0,     0,   758,     0,   759,     0,     0,   762,     0,     0,
       0,   597,     0,     0,     0,   139,   139,   139,   139,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   656,     0,     0,     0,     0,     0,
       0,   139,   139,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   535,     0,   535,     0,
       0,     0,     0,   535,     0,     0,   535,     0,     0,     0,
       0,     0,     0,     0,     0,  1070,     0,  1071,     0,     0,
    1073,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     284,   284,   735,     0,     0,  1106,     0,   284,     0,   503,
       0,     0,     0,   254,   230,  1112,   503,  1116,     0,   284,
       0,     0,   284,     0,   284,     0,   284,     0,   231,   232,
       0,   233,     0,     0,     0,     0,   234,     0,     0,  1078,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
       0,   236,     0,     0,     0,   774,     0,   237,     0,     0,
       0,   238,     0,     0,   239,     0,     0,  1110,     0,   783,
       0,     0,     0,   527,   240,     0,     0,     0,     0,     0,
       0,   241,   242,     0,     0,     0,   801,   465,     0,   243,
       0,    22,    23,     0,     0,     0,  1016,  1164,   244,     0,
       0,   466,     0,    31,   467,     0,     0,   245,   246,    37,
     247,     0,   248,     0,   249,     0,    42,   250,     0,     0,
       0,   251,     0,     0,   252,     0,     0,   253,  1187,     0,
       0,     0,     0,     0,     0,     0,   842,  1192,     0,     0,
       0,  1194,    58,     0,    60,     0,    62,     0,  1017,   370,
       0,  1018,     0,   468,     0,    68,     0,     0,     0,     0,
       0,   934,   936,     0,     0,     0,     0,   941,   944,     0,
       0,   946,   948,     0,    84,     0,     0,    86,     0,     0,
      88,     0,     0,   279,   284,     0,  1222,     0,     0,     0,
       0,     0,     0,     0,   230,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   231,   232,
       0,   233,   388,     0,     0,     0,   234,     0,     0,     0,
       0,     0,     0,     0,     0,   235,     0,     0,     0,     0,
     103,   236,     0,     0,   503,   503,  1203,   237,   503,   503,
       0,   238,     0,   396,   239,     0,     0,     0,     0,     0,
     397,     0,     0,     0,   240,     0,     0,     0,     0,     0,
       0,   241,   242,   398,   488,     0,  1243,     0,   503,   243,
     503,     0,     0,     0,     0,     0,     0,     0,   244,   489,
       0,     0,     0,     0,     0,     0,     0,   245,   246,     0,
     247,     0,   248,     0,   249,     0,     0,   250,     0,     0,
       0,   251,     0,     0,   252,   682,     0,   253,     0,     0,
       0,     0,     0,     0,   613,     0,     0,     0,   959,     0,
     399,   615,   683,     0,   400,     0,   963,     0,     0,   527,
       0,     0,     0,   527,  1051,  1052,   684,     0,     0,     0,
       0,  1053,     0,     0,     0,     0,     0,     0,     0,   978,
       0,   485,     0,  1064,     0,   401,  1067,   621,  1068,   989,
    1069,     0,     0,   782,     0,   685,     0,     0,     0,     0,
       0,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,   414,     0,   415,   416,
     417,   418,   419,   420,   421,     0,     0,   422,   423,   424,
       0,   456,     0,     0,     0,   686,     0,     0,   425,     0,
       0,   687,   370,     0,     0,   370,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   656,   502,   427,   428,
     429,   430,   431,     0,     0,   434,   435,   436,   437,     0,
     439,   440,   631,   632,   633,   634,   635,   636,     0,   637,
       0,     0,     0,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,     0,   649,     0,     0,   650,   651,
     652,   653,  1000,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    -2,     4,     0,
       5,     0,     6,     7,     8,     9,    10,    11,   774,     0,
       0,    12,    13,    14,    15,    16,     0,    17,  1188,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,   989,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -55,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -55,    53,    54,     0,    55,    56,    57,
       0,  -320,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -320,   -55,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,   -55,    88,    89,     0,     0,    90,     0,    91,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,   103,     0,     0,   104,     0,   105,     0,   106,
       0,   107,     0,     4,   108,     5,     0,     6,     7,     8,
       9,    10,    11,     0,  -668,     0,    12,    13,    14,    15,
      16,  -668,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,  -668,    28,    29,  -668,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -55,
       0,    40,    41,    42,     0,    43,  -320,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -55,    53,
      54,     0,    55,    56,    57,     0,  -320,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -320,   -55,    65,    66,
      67,  -668,    68,    69,    70,  -668,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,    84,    85,     0,    86,    87,   -55,    88,    89,     0,
       0,    90,     0,    91,     0,     0,  -668,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -668,  -668,    95,  -668,  -668,  -668,  -668,  -668,
    -668,  -668,     0,  -668,  -668,  -668,  -668,  -668,     0,  -668,
    -668,  -668,  -668,  -668,  -668,  -668,  -668,   103,  -668,  -668,
    -668,     0,   105,  -668,   106,     0,   107,     0,   334,  -668,
       5,   302,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -55,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -55,    53,    54,     0,    55,    56,    57,
       0,  -320,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -320,   -55,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,   -55,    88,    89,     0,     0,    90,     0,    91,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,   103,     0,     0,   104,     0,   105,   335,   106,
       0,   107,     0,     4,   108,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -55,
       0,    40,    41,    42,     0,    43,  -320,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -55,    53,
      54,     0,    55,    56,    57,     0,  -320,     0,     0,    58,
      59,    60,    61,    62,    63,    64,  -320,   -55,    65,    66,
      67,     0,    68,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,    84,    85,     0,    86,    87,   -55,    88,    89,     0,
       0,    90,     0,    91,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,   103,     0,     0,
     104,     0,   105,   562,   106,     0,   107,     0,   581,   108,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -55,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -55,    53,    54,     0,    55,    56,    57,
       0,  -320,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -320,   -55,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,   -55,    88,    89,     0,     0,    90,     0,    91,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,   103,     0,     0,   104,     0,   105,   582,   106,
       0,   107,     0,   334,   108,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -55,
       0,    40,    41,    42,     0,    43,  -320,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -55,    53,
      54,     0,    55,    56,    57,     0,  -320,     0,     0,    58,
      59,    60,    61,    62,    63,    64,  -320,   -55,    65,    66,
      67,     0,    68,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,    84,    85,     0,    86,    87,   -55,    88,    89,     0,
       0,    90,     0,    91,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,   103,     0,     0,
     104,     0,   105,   335,   106,     0,   107,     0,     4,   108,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -55,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -55,    53,    54,     0,    55,    56,    57,
       0,  -320,     0,     0,    58,    59,    60,    61,    62,    63,
      64,  -320,   -55,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,   -55,    88,    89,     0,     0,    90,     0,    91,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,   103,     0,     0,   104,     0,   105,   832,   106,
       0,   107,     0,     4,   108,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -55,
       0,    40,    41,    42,     0,    43,  -320,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -55,    53,
      54,     0,    55,    56,    57,     0,  -320,     0,     0,    58,
      59,    60,    61,   350,    63,    64,  -320,   -55,    65,    66,
      67,     0,    68,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,    84,    85,     0,    86,    87,   -55,    88,    89,     0,
       0,    90,     0,    91,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,   103,     0,     0,
     104,     0,   105,     0,   106,     0,   107,     0,     4,   108,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,    38,    39,   -55,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -55,    53,    54,     0,    55,    56,    57,
       0,  -320,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -320,   -55,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,   -55,    88,    89,     0,     0,    90,     0,    91,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,   103,     0,     0,   104,     0,   105,     0,   106,
       0,   107,     0,     4,   108,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,   748,    39,   -55,
       0,    40,    41,    42,     0,    43,  -320,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -55,    53,
      54,     0,    55,    56,    57,     0,  -320,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -320,   -55,    65,    66,
      67,     0,    68,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,    84,    85,     0,    86,    87,   -55,    88,    89,     0,
       0,    90,     0,    91,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,   103,     0,     0,
     104,     0,   105,     0,   106,     0,   107,     0,     4,   108,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,   754,    39,   -55,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -55,    53,    54,     0,    55,    56,    57,
       0,  -320,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -320,   -55,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,   -55,    88,    89,     0,     0,    90,     0,    91,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,   103,     0,     0,   104,     0,   105,     0,   106,
       0,   107,     0,     4,   108,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1055,    39,   -55,
       0,    40,    41,    42,     0,    43,  -320,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -55,    53,
      54,     0,    55,    56,    57,     0,  -320,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -320,   -55,    65,    66,
      67,     0,    68,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,    84,    85,     0,    86,    87,   -55,    88,    89,     0,
       0,    90,     0,    91,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,   103,     0,     0,
     104,     0,   105,     0,   106,     0,   107,     0,     4,   108,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1057,    39,   -55,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -55,    53,    54,     0,    55,    56,    57,
       0,  -320,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -320,   -55,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,   -55,    88,    89,     0,     0,    90,     0,    91,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,   103,     0,     0,   104,     0,   105,     0,   106,
       0,   107,     0,     4,   108,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1062,    39,   -55,
       0,    40,    41,    42,     0,    43,  -320,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -55,    53,
      54,     0,    55,    56,    57,     0,  -320,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -320,   -55,    65,    66,
      67,     0,    68,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,    84,    85,     0,    86,    87,   -55,    88,    89,     0,
       0,    90,     0,    91,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,   103,     0,     0,
     104,     0,   105,     0,   106,     0,   107,     0,     4,   108,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1065,    39,   -55,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -55,    53,    54,     0,    55,    56,    57,
       0,  -320,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -320,   -55,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,   -55,    88,    89,     0,     0,    90,     0,    91,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,   103,     0,     0,   104,     0,   105,     0,   106,
       0,   107,     0,     4,   108,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,  1105,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,    38,    39,   -55,
       0,    40,    41,    42,     0,    43,  -320,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -55,    53,
      54,     0,    55,    56,    57,     0,  -320,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -320,   -55,    65,    66,
      67,     0,    68,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,    84,    85,     0,    86,    87,   -55,    88,    89,     0,
       0,    90,     0,    91,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,   103,     0,     0,
     104,     0,   105,     0,   106,     0,   107,     0,     4,   108,
       5,     0,     6,     7,     8,     9,    10,    11,     0,     0,
       0,    12,    13,    14,    15,    16,     0,    17,     0,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,     0,
      28,    29,     0,    30,     0,    31,    32,    33,    34,    35,
      36,    37,  1111,    39,   -55,     0,    40,    41,    42,     0,
      43,  -320,     0,    44,    45,    46,    47,    48,     0,    49,
      50,    51,    52,   -55,    53,    54,     0,    55,    56,    57,
       0,  -320,     0,     0,    58,    59,    60,    61,     0,    63,
      64,  -320,   -55,    65,    66,    67,     0,    68,    69,    70,
       0,    71,    72,    73,    74,    75,    76,    77,    78,     0,
      79,    80,     0,    81,    82,    83,    84,    85,     0,    86,
      87,   -55,    88,    89,     0,     0,    90,     0,    91,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,   103,     0,     0,   104,     0,   105,     0,   106,
       0,   107,     0,     4,   108,     5,     0,     6,     7,     8,
       9,    10,    11,     0,     0,     0,    12,    13,    14,    15,
      16,     0,    17,     0,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,     0,    28,    29,     0,    30,     0,
      31,    32,    33,    34,    35,    36,    37,  1115,    39,   -55,
       0,    40,    41,    42,     0,    43,  -320,     0,    44,    45,
      46,    47,    48,     0,    49,    50,    51,    52,   -55,    53,
      54,     0,    55,    56,    57,     0,  -320,     0,     0,    58,
      59,    60,    61,     0,    63,    64,  -320,   -55,    65,    66,
      67,     0,    68,    69,    70,     0,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    79,    80,     0,    81,    82,
      83,    84,    85,     0,    86,    87,   -55,    88,    89,     0,
       0,    90,     0,    91,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,   103,     0,     0,
     104,     0,   105,     0,   106,     0,   107,     0,  1025,   108,
       5,   302,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   196,     0,     0,    15,    16,     0,    17,     0,   197,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   198,     0,     0,     0,    33,   199,   200,
       0,     0,   201,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   202,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   203,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   204,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   205,     0,   106,
       0,   206,  1026,   974,   108,     5,   302,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   196,     0,     0,    15,
      16,     0,    17,     0,   197,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   198,     0,
       0,     0,    33,   199,   200,     0,     0,   201,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   202,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     203,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   204,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   205,     0,   106,     0,   206,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,   613,     0,   196,
       0,     0,    15,    16,   615,    17,     0,   197,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   198,     0,     0,     0,    33,   199,   200,     0,     0,
     201,    39,     0,     0,     0,    41,     0,     0,    43,     0,
     621,   202,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   203,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   204,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
     502,   427,   428,   429,   430,   431,     0,     0,   434,   435,
     436,   437,     0,   439,   440,   843,   844,   845,   846,   847,
     636,     0,   637,     0,    98,     0,   638,   639,   640,   641,
     642,   643,   644,   645,   848,   647,   648,   100,   849,   102,
       0,   650,   651,   850,   653,   205,     0,   106,     0,   206,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   196,    13,     0,    15,    16,     0,    17,     0,
     197,    19,    20,    21,     0,     0,     0,     0,    26,     0,
       0,    28,    29,     0,   198,     0,     0,     0,    33,    34,
      35,    36,     0,    38,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   202,     0,     0,    47,    48,     0,
      49,    50,    51,    52,     0,    53,    54,     0,    55,    56,
      57,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   203,     0,     0,     0,
       0,     0,    71,    72,    73,    74,    75,    76,    77,    78,
       0,    79,    80,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,    90,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   105,     0,
     106,     0,   107,     0,     0,   108,     5,   302,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   196,     0,     0,
      15,    16,     0,    17,     0,   197,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   198,
       0,     0,     0,    33,   199,   200,     0,     0,   201,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   202,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   203,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   204,     0,    79,     0,     0,    81,
     306,   307,     0,    85,   339,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,   340,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   205,     0,   106,     0,   206,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     196,     0,     0,    15,    16,     0,    17,     0,   197,     0,
       0,    21,   357,    23,     0,     0,     0,     0,     0,     0,
      29,     0,   198,     0,     0,     0,    33,   199,   200,     0,
       0,   201,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   202,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,    60,    61,     0,    63,     0,
       0,     0,     0,    66,   203,     0,    68,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   204,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,    88,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   205,     0,   106,     0,
     206,     0,     0,   108,     5,   302,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   196,     0,     0,    15,    16,
       0,    17,     0,   197,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   198,     0,     0,
       0,    33,   199,   200,     0,     0,   201,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   202,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   203,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   204,     0,    79,     0,     0,    81,   306,   307,
       0,    85,   339,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   205,     0,   106,   816,   206,     0,     0,   108,     5,
     302,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     196,     0,     0,    15,    16,     0,    17,     0,   197,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   198,     0,     0,     0,    33,   199,   200,     0,
       0,   201,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   202,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   203,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   204,     0,    79,
       0,     0,    81,   306,   307,     0,    85,   339,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   205,     0,   106,   818,
     206,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   196,     0,     0,    15,    16,     0,    17,
       0,   197,     0,     0,    21,     0,   718,     0,     0,     0,
       0,     0,     0,    29,     0,   198,     0,     0,     0,    33,
     199,   200,     0,     0,   201,    39,     0,   719,     0,    41,
       0,     0,    43,     0,     0,   202,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   203,     0,   720,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     204,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,   721,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   205,
       0,   106,     0,   206,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   196,     0,     0,    15,
      16,     0,    17,     0,   197,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   198,     0,
       0,     0,    33,   199,   200,     0,     0,   201,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   202,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     203,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   204,     0,    79,     0,     0,    81,   306,
     307,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,   308,     0,     0,
     104,     0,   205,     0,   106,     0,   206,     0,     0,   108,
       5,   302,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   196,     0,     0,    15,    16,     0,    17,     0,   197,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   198,     0,     0,     0,    33,   199,   200,
       0,     0,   201,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   202,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   203,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   204,     0,
      79,     0,     0,    81,   306,   307,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   205,     0,   106,
       0,   206,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   196,     0,     0,    15,    16,     0,
      17,     0,   197,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   198,     0,     0,     0,
      33,   199,   200,     0,     0,   201,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   202,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,   976,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   203,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   204,     0,    79,     0,     0,    81,   306,   307,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     205,     0,   106,     0,   206,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   196,     0,     0,
      15,    16,     0,    17,     0,   197,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   198,
       0,     0,     0,    33,   199,   200,     0,     0,   201,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   202,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,   258,    61,     0,    63,     0,     0,     0,     0,
      66,   203,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   204,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,   259,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   205,     0,   106,     0,   206,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     196,     0,     0,    15,    16,     0,    17,     0,   197,     0,
       0,    21,     0,   266,     0,     0,     0,     0,     0,     0,
      29,     0,   198,     0,     0,     0,    33,   199,   200,     0,
       0,   201,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   202,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   203,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   204,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,   267,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   205,     0,   106,     0,
     206,     0,     0,   108,     5,   302,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   196,     0,     0,    15,    16,
       0,    17,     0,   197,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   198,     0,     0,
       0,    33,   199,   200,     0,     0,   201,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   202,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   203,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   204,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,   259,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   205,     0,   106,     0,   206,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   196,     0,
       0,    15,    16,     0,    17,     0,   197,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     198,     0,     0,     0,    33,   199,   200,     0,     0,   201,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     202,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   203,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   204,     0,    79,     0,     0,
      81,   306,   307,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   205,     0,   106,     0,   206,     0,
       0,   108,     5,   302,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   196,     0,     0,    15,    16,     0,    17,
       0,   197,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   198,     0,     0,     0,    33,
     199,   200,     0,     0,   201,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   202,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   203,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     204,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   205,
     814,   106,     0,   206,     0,     0,   108,     5,   302,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   196,     0,
       0,    15,    16,     0,    17,     0,   197,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     198,     0,     0,     0,    33,   199,   200,     0,     0,   201,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     202,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   203,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   204,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   205,     0,   106,     0,   206,   824,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   196,     0,     0,    15,    16,     0,    17,     0,   197,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   198,     0,     0,     0,    33,   199,   200,
       0,     0,   201,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   202,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   203,  -712,     0,     0,     0,
    -712,     0,     0,    73,    74,    75,    76,    77,   204,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   205,     0,   106,
       0,   206,     0,     0,   108,     5,   302,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   196,     0,     0,    15,
      16,     0,    17,     0,   197,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   198,     0,
       0,     0,    33,   199,   200,     0,     0,   201,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   202,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     203,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   204,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   205,     0,   106,     0,   206,  1168,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   196,
       0,     0,    15,    16,     0,    17,     0,   197,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   198,     0,     0,     0,    33,   199,   200,     0,     0,
     201,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   202,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   203,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   204,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,   259,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   205,     0,   106,     0,   206,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   196,     0,     0,    15,    16,     0,    17,     0,
     197,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,   283,    29,     0,   198,     0,     0,     0,    33,   199,
     200,     0,     0,   201,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   202,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   203,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   204,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   105,     0,
     106,     0,   206,     0,     0,   108,     5,   302,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   196,     0,     0,
      15,    16,     0,    17,     0,   197,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   198,
       0,     0,     0,    33,   199,   200,     0,     0,   201,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   202,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   203,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   204,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   205,     0,   106,     0,   206,     0,     5,
     108,     6,     7,     8,   360,    10,    11,     0,     0,     0,
     196,     0,     0,    15,    16,     0,    17,     0,    18,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   198,     0,     0,     0,    33,   199,   200,     0,
       0,   201,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   202,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,    67,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   204,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,    86,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   205,     0,   106,     0,
     206,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   196,     0,     0,    15,    16,     0,    17,
       0,   197,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   198,     0,     0,     0,    33,
     199,   200,     0,     0,   201,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   202,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   203,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     204,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   205,
       0,   106,     0,   206,   829,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   196,     0,     0,    15,
      16,     0,    17,     0,   197,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   198,     0,
       0,     0,    33,   199,   200,     0,     0,   201,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   202,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     203,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   204,     0,    79,     0,     0,    81,     0,
       0,     0,    85,  1003,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   205,     0,   106,     0,   206,     0,     0,   108,
       5,   302,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   196,     0,     0,    15,    16,     0,    17,     0,   197,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,   788,     0,   198,     0,     0,     0,    33,   199,   200,
       0,     0,   201,    39,     0,     0,     0,   789,     0,     0,
      43,     0,     0,   202,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   203,     0,     0,     0,     0,
       0,     0,     0,    73,   790,    75,    76,    77,   791,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   205,     0,   106,
       0,  1081,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   196,     0,     0,    15,    16,     0,
      17,     0,   197,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   198,     0,     0,     0,
      33,   199,   200,     0,     0,   201,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   202,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   203,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   204,     0,    79,     0,     0,    81,     0,     0,     0,
      85,  1196,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     205,     0,   106,     0,   206,     0,     0,   108,     5,   302,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   196,
       0,     0,    15,    16,     0,    17,     0,   197,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   198,     0,     0,     0,    33,   199,   200,     0,     0,
     201,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   202,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   203,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   204,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   205,     0,   106,     0,  1081,
       0,     0,   108,     5,   302,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   196,     0,     0,    15,    16,     0,
      17,     0,   197,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   198,     0,     0,     0,
      33,   199,   200,     0,     0,  1154,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   202,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   203,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   204,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     205,     0,   106,     0,   206,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   196,     0,     0,
      15,    16,     0,    17,     0,   197,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   198,
       0,     0,     0,    33,   199,   200,     0,     0,   201,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   202,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   203,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   204,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   205,     0,   106,     0,   206,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     196,     0,     0,    15,    16,     0,    17,     0,   197,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
     788,     0,   198,     0,     0,     0,    33,   199,   200,     0,
       0,   201,    39,     0,     0,     0,   789,     0,     0,    43,
       0,     0,   202,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   203,     0,     0,     0,     0,     0,
       0,     0,    73,   790,    75,    76,    77,   791,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   205,     0,   106,     0,
     792,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   196,     0,     0,    15,    16,     0,    17,
       0,   197,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,   788,     0,   198,     0,     0,     0,    33,
     199,   200,     0,     0,   201,    39,     0,     0,     0,   789,
       0,     0,    43,     0,     0,   202,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   203,     0,     0,
       0,     0,     0,     0,     0,    73,   790,    75,    76,    77,
     791,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
      94,    95,    96,    97,     0,     0,     0,     0,    98,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    99,     0,
       0,   100,   101,   102,     0,     0,     0,   104,     0,   205,
       0,   106,     0,   869,     0,     5,   108,     6,     7,     8,
       9,    10,    11,     0,     0,     0,   196,     0,     0,    15,
      16,     0,    17,     0,   197,     0,     0,    21,     0,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   198,     0,
       0,     0,    33,   199,   200,     0,     0,   201,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   202,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
     203,     0,     0,     0,     0,     0,     0,     0,    73,    74,
      75,    76,    77,   204,     0,    79,     0,     0,    81,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,     0,     0,     0,     0,    92,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    93,    94,    95,    96,    97,     0,     0,     0,
       0,    98,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    99,     0,     0,   100,   101,   102,     0,     0,     0,
     104,     0,   205,     0,   106,     0,   792,     0,     5,   108,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   196,
       0,     0,    15,    16,     0,    17,     0,   197,     0,     0,
      21,     0,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   198,     0,     0,     0,    33,   199,   200,     0,     0,
     913,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   202,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   203,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   204,     0,    79,     0,
       0,    81,     0,     0,     0,    85,     0,     0,    87,     0,
       0,    89,     0,     0,     0,     0,     0,     0,     0,    92,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    94,    95,    96,    97,
       0,     0,     0,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    99,     0,     0,   100,   101,   102,
       0,     0,     0,   104,     0,   205,     0,   106,     0,   206,
       0,     5,   108,     6,     7,     8,     9,    10,    11,     0,
       0,     0,   196,     0,     0,    15,    16,     0,    17,     0,
     197,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,     0,    29,     0,   198,     0,     0,     0,    33,   199,
     200,     0,     0,   916,    39,     0,     0,     0,    41,     0,
       0,    43,     0,     0,   202,     0,     0,    47,    48,     0,
       0,    50,     0,    52,     0,    53,    54,     0,    55,    56,
       0,     0,     0,     0,     0,     0,    59,     0,    61,     0,
      63,     0,     0,     0,     0,    66,   203,     0,     0,     0,
       0,     0,     0,     0,    73,    74,    75,    76,    77,   204,
       0,    79,     0,     0,    81,     0,     0,     0,    85,     0,
       0,    87,     0,     0,    89,     0,     0,     0,     0,     0,
       0,     0,    92,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,    94,
      95,    96,    97,     0,     0,     0,     0,    98,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    99,     0,     0,
     100,   101,   102,     0,     0,     0,   104,     0,   205,     0,
     106,     0,   206,     0,     5,   108,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   196,     0,     0,    15,    16,
       0,    17,     0,   197,     0,     0,    21,     0,     0,     0,
       0,     0,     0,     0,     0,    29,     0,   198,     0,     0,
       0,    33,   199,   200,     0,     0,  1148,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   202,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   203,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   204,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,    92,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    93,    94,    95,    96,    97,     0,     0,     0,     0,
      98,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   100,   101,   102,     0,     0,     0,   104,
       0,   205,     0,   106,     0,   206,     0,     5,   108,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   196,     0,
       0,    15,    16,     0,    17,     0,   197,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,     0,    29,     0,
     198,     0,     0,     0,    33,   199,   200,     0,     0,  1149,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     202,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   203,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   204,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,    92,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    93,    94,    95,    96,    97,     0,
       0,     0,     0,    98,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   100,   101,   102,     0,
       0,     0,   104,     0,   205,     0,   106,     0,   206,     0,
       5,   108,     6,     7,     8,     9,    10,    11,     0,     0,
       0,   196,     0,     0,    15,    16,     0,    17,     0,   197,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
       0,    29,     0,   198,     0,     0,     0,    33,   199,   200,
       0,     0,  1151,    39,     0,     0,     0,    41,     0,     0,
      43,     0,     0,   202,     0,     0,    47,    48,     0,     0,
      50,     0,    52,     0,    53,    54,     0,    55,    56,     0,
       0,     0,     0,     0,     0,    59,     0,    61,     0,    63,
       0,     0,     0,     0,    66,   203,     0,     0,     0,     0,
       0,     0,     0,    73,    74,    75,    76,    77,   204,     0,
      79,     0,     0,    81,     0,     0,     0,    85,     0,     0,
      87,     0,     0,    89,     0,     0,     0,     0,     0,     0,
       0,    92,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    93,    94,    95,
      96,    97,     0,     0,     0,     0,    98,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,   100,
     101,   102,     0,     0,     0,   104,     0,   205,     0,   106,
       0,   206,     0,     5,   108,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   196,     0,     0,    15,    16,     0,
      17,     0,   197,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   198,     0,     0,     0,
      33,   199,   200,     0,     0,  1152,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   202,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   203,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   204,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,    98,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,   100,   101,   102,     0,     0,     0,   104,     0,
     205,     0,   106,     0,   206,     0,     5,   108,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   196,     0,     0,
      15,    16,     0,    17,     0,   197,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   198,
       0,     0,     0,    33,   199,   200,     0,     0,  1153,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   202,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   203,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   204,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,    94,    95,    96,    97,     0,     0,
       0,     0,    98,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    99,     0,     0,   100,   101,   102,     0,     0,
       0,   104,     0,   205,     0,   106,     0,   206,     0,     5,
     108,     6,     7,     8,     9,    10,    11,     0,     0,     0,
     196,     0,     0,    15,    16,     0,    17,     0,   197,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      29,     0,   198,     0,     0,     0,    33,   199,   200,     0,
       0,  1154,    39,     0,     0,     0,    41,     0,     0,    43,
       0,     0,   202,     0,     0,    47,    48,     0,     0,    50,
       0,    52,     0,    53,    54,     0,    55,    56,     0,     0,
       0,     0,     0,     0,    59,     0,    61,     0,    63,     0,
       0,     0,     0,    66,   203,     0,     0,     0,     0,     0,
       0,     0,    73,    74,    75,    76,    77,   204,     0,    79,
       0,     0,    81,     0,     0,     0,    85,     0,     0,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
      92,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    93,    94,    95,    96,
      97,     0,     0,     0,     0,    98,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    99,     0,     0,   100,   101,
     102,     0,     0,     0,   104,     0,   205,     0,   106,     0,
     206,     0,     5,   108,     6,     7,     8,     9,    10,    11,
       0,     0,     0,   196,     0,     0,    15,    16,     0,    17,
       0,   197,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    29,     0,   198,     0,     0,     0,    33,
     199,   200,     0,     0,   201,    39,     0,     0,     0,    41,
       0,     0,    43,     0,     0,   202,     0,     0,    47,    48,
       0,     0,    50,     0,    52,     0,    53,    54,     0,    55,
      56,     0,     0,     0,     0,     0,     0,    59,     0,    61,
       0,    63,     0,     0,     0,     0,    66,   203,     0,     0,
       0,     0,     0,     0,     0,    73,    74,    75,    76,    77,
     204,     0,    79,     0,     0,    81,     0,     0,     0,    85,
       0,     0,    87,     0,     0,    89,     0,     0,     0,     0,
       0,     0,     0,    92,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   611,     0,   612,     0,     0,     0,    93,
      94,    95,    96,    97,   613,     0,     0,     0,    98,   614,
     232,   615,   616,     0,     0,     0,     0,   617,    99,     0,
       0,   100,   101,   102,     0,     0,   235,   104,   198,     0,
       0,   106,   618,   869,     0,     0,   108,     0,   619,     0,
       0,     0,   238,     0,     0,   620,     0,   621,     0,     0,
       0,     0,     0,     0,     0,   622,     0,     0,     0,     0,
       0,     0,   623,   624,     0,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,     0,     0,     0,   625,
       0,     0,     0,     0,     0,     0,     0,     0,   245,   246,
       0,   626,     0,   248,     0,   627,     0,     0,   628,     0,
       0,     0,   629,     0,     0,   252,     0,     0,   630,     0,
       0,     0,     0,     0,     0,     0,     0,   502,   427,   428,
     429,   430,   431,     0,     0,   434,   435,   436,   437,     0,
     439,   440,   631,   632,   633,   634,   635,   636,     0,   637,
       0,     0,     0,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,     0,   649,     0,     0,   650,   651,
     652,   653,     0,     5,   654,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   196,     0,     0,    15,    16,     0,
      17,     0,   197,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   198,     0,     0,     0,
      33,   199,   200,     0,     0,   201,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   202,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   203,     0,
       0,     0,     0,     0,     0,     0,    73,    74,    75,    76,
      77,   204,     0,    79,     0,     0,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      93,    94,    95,    96,    97,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    99,
       0,     0,     0,   101,   102,     0,     0,     0,   104,     0,
     205,     0,   106,     5,   206,     6,     7,     8,     9,    10,
      11,     0,     0,     0,   196,     0,     0,    15,    16,     0,
      17,     0,   197,     0,     0,    21,     0,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   198,     0,     0,     0,
      33,   199,   200,     0,     0,   201,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   202,     0,     0,    47,
      48,  -477,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,   203,     0,
       0,     0,  -477,     0,     0,     0,    73,    74,    75,    76,
      77,   204,     0,    79,     0,  -477,    81,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,  -477,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,     0,     5,   302,     6,     7,     8,     9,
      10,    11,     0,     0,     0,   196,     0,     0,    15,    16,
       0,    17,     0,   197,   102,  -477,    21,     0,     0,     0,
    -477,     0,   106,     0,   869,    29,     0,   198,     0,     0,
       0,    33,   199,   200,     0,     0,   201,    39,     0,     0,
       0,    41,     0,     0,    43,     0,     0,   202,     0,     0,
      47,    48,     0,     0,    50,     0,    52,     0,    53,    54,
       0,    55,    56,     0,     0,     0,     0,     0,     0,    59,
       0,    61,     0,    63,     0,     0,     0,     0,    66,   203,
       0,     0,     0,     0,     0,     0,     0,    73,    74,    75,
      76,    77,   204,     0,    79,     0,     0,    81,     0,     0,
       0,    85,     0,     0,    87,     0,     0,    89,     0,     0,
       0,     0,     0,     0,     0,     0,  -495,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,     0,  -495,     0,     0,
       0,  -495,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,     0,     0,     0,     0,
       0,   205,     0,   106,  -495,  1081,     5,     0,     6,     7,
       8,     9,    10,    11,     0,     0,     0,   196,     0,     0,
      15,    16,     0,    17,     0,   197,     0,     0,    21,     0,
       0,     0,     0,     0,     0,     0,     0,    29,     0,   198,
       0,     0,     0,    33,   199,   200,     0,     0,   201,    39,
       0,     0,     0,    41,     0,     0,    43,     0,     0,   202,
       0,     0,    47,    48,     0,     0,    50,     0,    52,     0,
      53,    54,     0,    55,    56,     0,     0,     0,     0,     0,
       0,    59,     0,    61,     0,    63,     0,     0,     0,     0,
      66,   203,     0,     0,     0,     0,     0,     0,     0,    73,
      74,    75,    76,    77,   204,     0,    79,     0,     0,    81,
       0,     0,     0,    85,     0,     0,    87,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,    95,     0,     5,     0,     6,
       7,     8,     9,    10,    11,     0,     0,     0,   196,     0,
       0,    15,    16,     0,    17,     0,   197,   102,     0,    21,
       0,     0,     0,   205,     0,   106,     0,   206,    29,     0,
     198,     0,     0,     0,    33,   199,   200,     0,     0,   201,
      39,     0,     0,     0,    41,     0,     0,    43,     0,     0,
     202,     0,     0,    47,    48,     0,     0,    50,     0,    52,
       0,    53,    54,     0,    55,    56,     0,     0,     0,     0,
       0,     0,    59,     0,    61,     0,    63,     0,     0,     0,
       0,    66,   203,     0,     0,     0,     0,     0,     0,     0,
      73,    74,    75,    76,    77,   204,     0,    79,     0,     0,
      81,     0,     0,     0,    85,     0,     0,    87,     0,     0,
      89,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    95,     0,     5,     0,
       6,     7,     8,     9,    10,    11,     0,     0,     0,   196,
       0,     0,    15,    16,     0,    17,     0,   197,   102,     0,
      21,     0,     0,     0,   205,     0,   106,     0,   792,    29,
       0,   198,     0,     0,     0,    33,   199,   200,     0,     0,
     201,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   202,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,   203,     0,     0,     0,     0,     0,     0,
       0,    73,    74,    75,    76,    77,   204,     0,    79,     0,
       0,    81,     0,  1031,     0,    85,     0,     0,    87,     0,
       0,    89,   613,     0,     0,     0,     0,   231,   232,   615,
     233,     0,     0,     0,     0,   234,     0,     0,     0,     0,
       0,     0,     0,     0,   235,     0,     0,    95,     0,     0,
     618,     0,     0,     0,     0,     0,   237,     0,     0,     0,
     238,     0,     0,   239,     0,   621,     0,     0,     0,     0,
       0,     0,     0,   240,     0,     0,     0,   106,     0,   206,
     623,   242,     0,     0,     0,     0,     0,     0,   243,     0,
       0,     0,     0,     0,     0,     0,     0,   244,     0,     0,
       0,     0,     0,     0,     0,   396,   245,   246,     0,   247,
       0,   248,   397,  1032,     0,     0,   628,     0,     0,     0,
     251,     0,     0,   252,     0,   398,   253,     0,     0,     0,
       0,     0,     0,     0,     0,   502,   427,   428,   429,   430,
     431,     0,     0,   434,   435,   436,   437,     0,   439,   440,
     631,   632,   633,   634,   635,   636,     0,   637,     0,     0,
       0,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,     0,   649,     0,     0,   650,   651,   652,   653,
       0,   396,   399,     0,     0,     0,   400,     0,   397,     0,
       0,     0,     0,     0,     0,   501,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,   502,   427,
     428,   429,   430,   431,     0,     0,   434,   435,   436,   437,
       0,   439,   440,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,     0,   410,   411,   412,   413,   414,     0,
     415,   416,   417,   418,   419,   420,   421,   396,   399,   422,
     423,   424,   400,   456,   397,     0,     0,     0,     0,     0,
     425,  1054,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,   502,   427,   428,   429,   430,   431,
       0,     0,   434,   435,   436,   437,     0,   439,   440,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,     0,
     410,   411,   412,   413,   414,     0,   415,   416,   417,   418,
     419,   420,   421,   396,   399,   422,   423,   424,   400,   456,
     397,     0,     0,     0,     0,     0,   425,  1061,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   401,
     502,   427,   428,   429,   430,   431,     0,     0,   434,   435,
     436,   437,     0,   439,   440,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,     0,   410,   411,   412,   413,
     414,     0,   415,   416,   417,   418,   419,   420,   421,   396,
     399,   422,   423,   424,   400,   456,   397,     0,     0,     0,
       0,     0,   425,  1218,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,   502,   427,   428,   429,
     430,   431,     0,     0,   434,   435,   436,   437,     0,   439,
     440,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,   414,     0,   415,   416,
     417,   418,   419,   420,   421,   396,   399,   422,   423,   424,
     400,   456,   397,     0,     0,     0,     0,     0,   425,  1219,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   401,   502,   427,   428,   429,   430,   431,     0,     0,
     434,   435,   436,   437,     0,   439,   440,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,     0,   410,   411,
     412,   413,   414,     0,   415,   416,   417,   418,   419,   420,
     421,   396,   399,   422,   423,   424,   400,   456,   397,     0,
       0,     0,     0,     0,   425,  1220,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,   502,   427,
     428,   429,   430,   431,     0,     0,   434,   435,   436,   437,
       0,   439,   440,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,     0,   410,   411,   412,   413,   414,     0,
     415,   416,   417,   418,   419,   420,   421,   396,   399,   422,
     423,   424,   400,   456,   397,     0,     0,     0,     0,     0,
     425,  1221,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,   502,   427,   428,   429,   430,   431,
       0,     0,   434,   435,   436,   437,     0,   439,   440,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,     0,
     410,   411,   412,   413,   414,     0,   415,   416,   417,   418,
     419,   420,   421,   396,   399,   422,   423,   424,   400,   456,
     397,     0,     0,     0,     0,     0,   425,  1244,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   401,
     502,   427,   428,   429,   430,   431,     0,     0,   434,   435,
     436,   437,     0,   439,   440,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,     0,   410,   411,   412,   413,
     414,     0,   415,   416,   417,   418,   419,   420,   421,     0,
     399,   422,   423,   424,   400,   456,     0,     0,     0,     0,
       0,     0,   425,  1245,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,   502,   427,   428,   429,
     430,   431,     0,     0,   434,   435,   436,   437,     0,   439,
     440,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,   414,     0,   415,   416,
     417,   418,   419,   420,   421,   396,     0,   422,   423,   424,
       0,   456,   397,     0,     0,     0,     0,     0,   425,     0,
       0,     0,     0,     0,     0,   398,   283,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   494,     0,     0,     0,   396,     0,     0,     0,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,   482,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   483,   399,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   485,     0,     0,     0,   401,     0,     0,
       0,     0,   399,     0,     0,     0,   400,     0,     0,     0,
       0,     0,     0,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,     0,   410,   411,   412,   413,   414,     0,
     415,   416,   417,   418,   419,   420,   421,   401,     0,   422,
     423,   424,     0,   456,     0,     0,     0,     0,     0,     0,
     425,     0,     0,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,     0,   410,   411,   412,   413,   414,     0,
     415,   416,   417,   418,   419,   420,   421,   396,   839,   422,
     423,   424,     0,   456,   397,     0,     0,     0,     0,     0,
     425,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,   840,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,     0,     0,     0,   841,     0,
       0,     0,   398,   943,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   399,     0,     0,     0,   400,     0,
       0,     0,     0,     0,     0,     0,     0,   396,     0,     0,
       0,     0,     0,     0,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,   283,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,     0,   400,     0,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,     0,   410,   411,   412,   413,
     414,     0,   415,   416,   417,   418,   419,   420,   421,     0,
     485,   422,   423,   424,   401,     0,     0,     0,     0,     0,
       0,     0,   425,     0,   399,     0,     0,     0,   400,     0,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
       0,   410,   411,   412,   413,   414,     0,   415,   416,   417,
     418,   419,   420,   421,     0,   485,   422,   423,   424,   401,
     456,     0,     0,     0,     0,     0,     0,   425,     0,     0,
       0,     0,     0,     0,     0,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,     0,   410,   411,   412,   413,
     414,     0,   415,   416,   417,   418,   419,   420,   421,   396,
       0,   422,   423,   424,     0,   456,   397,     0,     0,     0,
       0,     0,   425,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   458,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
     283,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   399,     0,     0,   396,
     400,     0,     0,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
     727,     0,     0,     0,     0,     0,     0,   191,     0,     0,
       0,   401,     0,     0,     0,   728,   399,     0,     0,     0,
     400,     0,     0,     0,     0,     0,     0,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,     0,   410,   411,
     412,   413,   414,     0,   415,   416,   417,   418,   419,   420,
     421,   401,     0,   422,   423,   424,   399,     0,     0,     0,
     400,     0,     0,     0,   425,     0,     0,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,     0,   410,   411,
     412,   413,   414,     0,   415,   416,   417,   418,   419,   420,
     421,   401,     0,   422,   423,   424,     0,   456,     0,     0,
       0,     0,     0,     0,   425,     0,     0,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,     0,   410,   411,
     412,   413,   414,     0,   415,   416,   417,   418,   419,   420,
     421,   396,     0,   422,   423,   424,     0,     0,   397,     0,
       0,     0,     0,     0,   425,     0,     0,     0,     0,     0,
       0,   398,   729,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   730,     0,     0,
       0,   396,     0,     0,     0,     0,     0,     0,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   398,   935,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,   396,   400,     0,     0,     0,     0,     0,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,     0,     0,     0,   399,     0,
       0,     0,   400,     0,     0,     0,     0,     0,     0,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,     0,
     410,   411,   412,   413,   414,     0,   415,   416,   417,   418,
     419,   420,   421,   401,     0,   422,   423,   424,   399,     0,
       0,     0,   400,     0,     0,     0,   425,     0,     0,   402,
     403,   957,   404,   405,   406,     0,   407,   408,   409,     0,
     410,   411,   412,   413,   414,     0,   415,   416,   417,   418,
     419,   420,   421,   401,     0,   422,   423,   424,     0,   456,
       0,     0,     0,     0,     0,     0,   425,     0,     0,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,     0,
     410,   411,   412,   413,   414,     0,   415,   416,   417,   418,
     419,   420,   421,   396,     0,   422,   423,   424,     0,   456,
     397,     0,     0,     0,     0,     0,   425,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   396,     0,     0,     0,     0,     0,     0,
     397,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,  1124,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,   400,     0,     0,     0,   396,   539,
       0,     0,     0,     0,     0,   397,     0,     0,     0,     0,
       0,     0,     0,     0,  1125,     0,     0,     0,   398,     0,
       0,   485,     0,     0,     0,   401,     0,     0,     0,     0,
     399,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,   414,     0,   415,   416,
     417,   418,   419,   420,   421,   401,     0,   422,   423,   424,
       0,     0,     0,     0,     0,   399,   994,     0,   425,   400,
       0,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,   414,     0,   415,   416,
     417,   418,   419,   420,   421,   396,     0,   422,   423,   424,
     401,     0,   397,     0,     0,     0,     0,     0,   425,     0,
       0,     0,     0,     0,     0,   398,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,     0,   410,   411,   412,
     413,   414,     0,   415,   416,   417,   418,   419,   420,   421,
     396,     0,   422,   423,   424,     0,     0,   397,     0,     0,
       0,     0,     0,   425,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   399,     0,     0,     0,   400,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,   397,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     398,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   399,     0,     0,
       0,   400,     0,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,     0,   410,   411,   412,   413,   414,     0,
     415,   416,   417,   418,   419,   420,   421,     0,     0,   422,
     423,   424,   401,   551,     0,     0,     0,   399,     0,     0,
     425,   400,     0,     0,     0,     0,     0,     0,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,     0,   410,
     411,   412,   413,   414,     0,   415,   416,   417,   418,   419,
     420,   421,   401,   555,   422,   423,   424,     0,     0,     0,
       0,     0,     0,     0,     0,   425,     0,     0,   402,   403,
       0,   404,   405,   406,     0,   407,   408,   409,     0,   410,
     411,   412,   413,   414,     0,   415,   416,   417,   418,   419,
     420,   421,   396,   560,   422,   423,   424,     0,     0,   397,
       0,     0,     0,     0,     0,   425,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   396,     0,     0,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   399,
       0,     0,   396,   400,     0,     0,     0,     0,     0,   397,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   398,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   577,   401,     0,     0,     0,     0,   399,
       0,     0,     0,   400,     0,     0,     0,     0,     0,     0,
     402,   403,   731,   404,   405,   406,     0,   407,   408,   409,
       0,   410,   411,   412,   413,   414,     0,   415,   416,   417,
     418,   419,   420,   421,   401,     0,   422,   423,   424,   399,
       0,     0,     0,   400,     0,     0,     0,   425,     0,     0,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
       0,   410,   411,   412,   413,   414,     0,   415,   416,   417,
     418,   419,   420,   421,   401,     0,   422,   423,   424,     0,
       0,     0,     0,     0,     0,     0,     0,   425,     0,     0,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
       0,   410,   411,   412,   413,   414,     0,   415,   416,   417,
     418,   419,   420,   421,   396,   835,   422,   423,   424,     0,
       0,   397,     0,   815,     0,     0,     0,   425,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   399,     0,     0,     0,   400,     0,     0,     0,   396,
       0,     0,     0,     0,     0,     0,   397,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,   401,     0,     0,     0,
       0,   399,     0,     0,     0,   400,     0,     0,     0,     0,
       0,     0,   402,   403,     0,   404,   405,   406,     0,   407,
     836,   409,     0,   410,   411,   412,   413,   414,     0,   415,
     416,   417,   418,   419,   420,   421,   401,     0,   422,   423,
     424,     0,     0,     0,     0,     0,   399,     0,     0,   425,
     400,     0,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,     0,   410,   411,   412,   413,   414,  -599,   415,
     416,   417,   418,   419,   420,   421,   396,   191,   422,   423,
     424,   401,     0,   397,     0,     0,     0,     0,     0,   425,
       0,     0,     0,     0,     0,     0,   398,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,     0,   410,   411,
     412,   413,   414,     0,   415,   416,   417,   418,   419,   420,
     421,   396,     0,   422,   423,   424,     0,     0,   397,     0,
       0,     0,     0,     0,   425,     0,     0,     0,     0,     0,
       0,   398,     0,     0,   955,     0,     0,     0,     0,     0,
       0,     0,     0,   399,     0,     0,     0,   400,     0,     0,
       0,   396,     0,     0,     0,     0,     0,     0,   397,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,   401,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   399,     0,
       0,     0,   400,     0,   402,   403,     0,   404,   405,   406,
       0,   407,   408,   409,     0,   410,   411,   412,   413,   414,
       0,   415,   416,   417,   418,   419,   420,   421,     0,   926,
     422,   423,   424,   401,     0,     0,     0,     0,   399,     0,
       0,   425,   400,     0,     0,     0,     0,     0,     0,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,     0,
     410,   411,   412,   413,   414,     0,   415,   416,   417,   418,
     419,   420,   421,   401,     0,   422,   423,   424,     0,     0,
       0,     0,     0,     0,     0,     0,   425,     0,     0,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,     0,
     410,   411,   412,   413,   414,     0,   415,   416,   417,   418,
     419,   420,   421,   396,     0,   422,   423,   424,     0,     0,
     397,     0,   979,     0,     0,     0,   425,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   396,     0,     0,     0,     0,     0,
       0,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,   400,   396,  1009,     0,     0,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,     0,     0,     0,
       0,   399,     0,     0,     0,   400,     0,     0,     0,     0,
       0,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,   414,     0,   415,   416,
     417,   418,   419,   420,   421,   997,   401,   422,   423,   424,
       0,     0,   399,     0,   980,     0,   400,     0,   425,     0,
       0,     0,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,     0,   410,   411,   412,   413,   414,     0,   415,
     416,   417,   418,   419,   420,   421,     0,   401,   422,   423,
     424,     0,     0,   396,     0,     0,     0,     0,     0,   425,
     397,     0,     0,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   398,   410,   411,   412,   413,   414,     0,
     415,   416,   417,   418,   419,   420,   421,     0,     0,   422,
     423,   424,     0,     0,   396,     0,     0,     0,     0,     0,
     425,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,  1043,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,   400,   396,     0,     0,     0,     0,
       0,     0,   397,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   398,  1046,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,     0,     0,     0,
       0,   399,     0,     0,     0,   400,     0,     0,     0,     0,
       0,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,   414,     0,   415,   416,
     417,   418,   419,   420,   421,     0,   401,   422,   423,   424,
       0,     0,   399,     0,  1030,     0,   400,     0,   425,     0,
       0,     0,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,     0,   410,   411,   412,   413,   414,     0,   415,
     416,   417,   418,   419,   420,   421,     0,   401,   422,   423,
     424,     0,     0,   396,     0,     0,     0,     0,     0,   425,
     397,     0,     0,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,   398,   410,   411,   412,   413,   414,     0,
     415,   416,   417,   418,   419,   420,   421,     0,     0,   422,
     423,   424,     0,     0,   396,  1121,     0,     0,     0,     0,
     425,   397,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     399,     0,     0,     0,   400,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,     0,     0,     0,
       0,   399,     0,     0,     0,   400,     0,     0,     0,     0,
       0,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,   414,     0,   415,   416,
     417,   418,   419,   420,   421,     0,   401,   422,   423,   424,
       0,     0,   396,     0,     0,     0,  1049,     0,   425,   397,
       0,     0,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,   398,   410,   411,   412,   413,   414,     0,   415,
     416,   417,   418,   419,   420,   421,     0,     0,   422,   423,
     424,     0,     0,     0,     0,     0,     0,     0,     0,   425,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   396,   399,
       0,     0,     0,   400,     0,   397,     0,     0,     0,     0,
       0,     0,  1147,     0,     0,     0,     0,     0,   398,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   401,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     402,   403,     0,   404,   405,   406,     0,   407,   408,   409,
       0,   410,   411,   412,   413,   414,     0,   415,   416,   417,
     418,   419,   420,   421,   396,   399,   422,   423,   424,   400,
       0,   397,     0,     0,     0,     0,     0,   425,  1150,     0,
       0,     0,     0,     0,   398,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     401,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1181,     0,     0,   402,   403,     0,   404,
     405,   406,     0,   407,   408,   409,     0,   410,   411,   412,
     413,   414,     0,   415,   416,   417,   418,   419,   420,   421,
       0,   399,   422,   423,   424,   400,     0,   396,  1202,     0,
       0,     0,     0,   425,   397,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,   401,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,   403,     0,   404,   405,   406,     0,   407,
     408,   409,     0,   410,   411,   412,   413,   414,     0,   415,
     416,   417,   418,   419,   420,   421,     0,     0,   422,   423,
     424,     0,     0,   396,   399,     0,     0,     0,   400,   425,
     397,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,     0,   410,   411,   412,   413,
     414,     0,   415,   416,   417,   418,   419,   420,   421,   396,
     399,   422,   423,   424,   400,     0,   397,     0,     0,     0,
       0,     0,   425,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,   414,     0,   415,   416,
     417,   418,   419,   420,   421,   396,   399,   422,   423,   424,
     400,     0,   397,     0,     0,     0,  1240,     0,   425,  1252,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,     0,   410,   411,
     412,   413,   414,     0,   415,   416,   417,   418,   419,   420,
     421,   396,   399,   422,   423,   424,   400,     0,   397,     0,
       0,     0,     0,     0,   425,  1253,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,     0,   410,   411,   412,   413,   414,     0,
     415,   416,   417,   418,   419,   420,   421,   396,   399,   422,
     423,   424,   400,     0,   397,     0,     0,     0,     0,     0,
     425,  1254,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,     0,
     410,   411,   412,   413,   414,     0,   415,   416,   417,   418,
     419,   420,   421,   396,   399,   422,   423,   424,   400,     0,
     397,     0,     0,     0,     0,     0,   425,  1255,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,     0,   410,   411,   412,   413,
     414,     0,   415,   416,   417,   418,   419,   420,   421,   396,
     399,   422,   423,   424,   400,     0,   397,     0,     0,     0,
       0,     0,   425,  1256,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,   414,     0,   415,   416,
     417,   418,   419,   420,   421,   396,   399,   422,   423,   424,
     400,     0,   397,     0,     0,     0,     0,     0,   425,  1257,
       0,     0,     0,     0,     0,   398,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,     0,   410,   411,
     412,   413,   414,     0,   415,   416,   417,   418,   419,   420,
     421,   396,   399,   422,   423,   424,   400,     0,   397,     0,
       0,     0,     0,     0,   425,     0,     0,     0,     0,     0,
       0,   398,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   401,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   402,   403,     0,   404,   405,   406,     0,
     407,   408,   409,     0,   410,   411,   412,   413,   414,     0,
     415,   416,   417,   418,   419,   420,   421,   396,   399,   422,
     423,   424,   400,     0,   397,     0,     0,     0,  1270,     0,
     425,     0,     0,     0,     0,     0,     0,   398,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   401,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   402,
     403,     0,   404,   405,   406,     0,   407,   408,   409,     0,
     410,   411,   412,   413,   414,     0,   415,   416,   417,   418,
     419,   420,   421,     0,   911,   422,   423,   424,   400,     0,
       0,     0,     0,     0,     0,     0,   425,     0,     0,     0,
       0,     0,     0,   398,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   401,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,   403,     0,   404,   405,
     406,     0,   407,   408,   409,     0,   410,   411,   412,   413,
     414,     0,   415,   416,   417,   418,   419,   420,   421,     0,
     399,   422,   423,   424,   400,     0,     0,     0,     0,     0,
       0,     0,   425,     0,     0,     0,     0,     0,     0,   398,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   402,   403,     0,   404,   405,   406,     0,   407,   408,
     409,     0,   410,   411,   412,   413,     0,     0,   415,   416,
     417,   418,   419,   420,   421,     0,   399,   422,   423,   424,
     400,     0,     0,     0,     0,     0,     0,     0,   425,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   401,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   402,   403,     0,
     404,   405,   406,     0,   407,   408,   409,     0,   410,   411,
     412,   413,     0,     0,   415,   416,   417,   418,   419,   368,
     421,   230,     0,   422,   423,   424,     0,     0,     0,     0,
       0,     0,     0,     0,   425,   231,   232,     0,   233,     0,
       0,     0,     0,   234,     0,     0,     0,     0,     0,   369,
       0,     0,   235,     0,     0,     0,     0,     0,   236,     0,
       0,     0,     0,     0,   237,     0,     0,     0,   238,     0,
       0,   239,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   240,     0,     0,     0,     0,     0,     0,   241,   242,
       0,     0,     0,     0,     0,     0,   243,     0,     0,     0,
       0,     0,     0,     0,     0,   244,     0,     0,   230,     0,
       0,     0,     0,     0,   245,   246,     0,   247,     0,   248,
       0,   249,   231,   232,   250,   233,     0,     0,   251,     0,
     234,   252,    23,     0,   253,     0,     0,     0,     0,   235,
       0,     0,     0,     0,     0,   236,     0,     0,     0,     0,
       0,   237,     0,     0,     0,   238,     0,     0,   239,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   240,     0,
       0,     0,     0,     0,     0,   241,   242,     0,     0,     0,
       0,     0,     0,   243,    60,     0,     0,     0,     0,     0,
       0,     0,   244,     0,     0,    68,     0,     0,     0,     0,
       0,   245,   246,     0,   247,     0,   248,     0,   249,   230,
       0,   250,     0,     0,     0,   251,     0,     0,   252,     0,
      88,   253,     0,   231,   232,     0,   233,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,   369,     0,     0,
     235,     0,     0,     0,     0,     0,   236,     0,     0,     0,
       0,     0,   237,     0,     0,     0,   238,     0,     0,   239,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   240,
       0,     0,     0,     0,     0,     0,   241,   242,     0,     0,
       0,     0,     0,     0,   243,     0,     0,     0,     0,     0,
       0,     0,     0,   244,     0,     0,  -297,     0,     0,     0,
       0,     0,   245,   246,     0,   247,     0,   248,     0,   249,
    -297,  -297,   250,  -297,     0,     0,   251,     0,  -297,   252,
       0,     0,   253,     0,     0,     0,     0,  -297,     0,     0,
       0,     0,     0,  -297,     0,     0,     0,     0,     0,  -297,
       0,     0,     0,  -297,     0,     0,  -297,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -297,     0,     0,     0,
       0,     0,     0,  -297,  -297,     0,     0,     0,     0,     0,
       0,  -297,     0,     0,     0,     0,     0,     0,     0,     0,
    -297,     0,     0,   230,     0,     0,     0,     0,     0,  -297,
    -297,     0,  -297,     0,  -297,     0,  -297,   231,   232,  -297,
     233,     0,     0,  -297,     0,   234,  -297,     0,     0,  -297,
       0,     0,     0,     0,   235,     0,     0,     0,     0,     0,
     236,     0,     0,     0,     0,     0,   237,     0,     0,     0,
     238,     0,     0,   239,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   240,     0,     0,     0,     0,     0,     0,
     241,   242,     0,     0,     0,     0,     0,     0,   243,     0,
       0,     0,     0,     0,     0,     0,     0,   244,     0,     0,
    -298,     0,     0,     0,     0,     0,   245,   246,     0,   247,
       0,   248,     0,   249,  -298,  -298,   250,  -298,     0,     0,
     251,     0,  -298,   252,     0,     0,   253,     0,     0,     0,
       0,  -298,     0,     0,     0,     0,     0,  -298,     0,     0,
       0,     0,     0,  -298,     0,     0,     0,  -298,     0,     0,
    -298,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -298,     0,     0,     0,     0,     0,     0,  -298,  -298,     0,
       0,     0,     0,     0,     0,  -298,     0,     0,     0,     0,
       0,     0,     0,     0,  -298,     0,     0,     0,     0,     0,
       0,     0,     0,  -298,  -298,     0,  -298,     0,  -298,     0,
    -298,     0,     0,  -298,     0,     0,     0,  -298,     0,     0,
    -298,     0,     0,  -298
};

static const yytype_int16 yycheck[] =
{
      12,     2,   277,   663,    16,   161,    19,   447,    20,   387,
     273,   387,   590,   608,    26,   220,   864,    40,   752,   841,
     526,   507,    34,    35,    36,   361,    38,   602,   524,   227,
     553,   133,   967,     1,   970,   963,   299,    49,   973,    51,
      52,   175,    69,   970,   859,    57,    33,    59,   836,    61,
     659,    56,   661,     1,   317,    11,  1026,    69,    70,    71,
      72,    73,    74,   531,    33,     1,    78,    18,    80,     1,
      33,    33,    92,  1079,     3,    87,     1,    70,    90,    91,
     107,    93,    61,    95,    27,   146,    98,    99,    47,   101,
     125,   120,  1168,   105,   106,   107,   108,   146,    56,    33,
       0,    48,   120,  1038,   146,    33,    48,    75,   263,   264,
     265,   146,   140,   106,   175,   150,    56,   125,    48,   131,
     148,   133,    37,    48,   173,    61,    85,    78,    48,    61,
       3,   173,    69,   120,    48,   269,    79,    48,   173,   118,
     608,   229,   150,    48,  1079,    96,   175,    90,    90,   117,
     170,   120,  1080,   109,  1230,   114,    92,   175,    73,   171,
      92,  1167,   113,   897,  1012,    48,   109,   172,   105,   106,
     107,  1141,  1020,   105,   103,   102,    90,   125,    56,   206,
     443,   444,   118,   170,   196,   273,   118,   199,   200,   201,
     345,   346,   204,   205,   206,   663,  1011,   145,   146,   146,
     125,   170,    27,   171,   227,   218,  1142,   170,   170,   145,
      32,   299,   146,   145,   172,  1142,   146,   229,   146,  1007,
     145,   146,   745,    48,    32,   173,   146,    52,   175,   317,
     103,    75,  1167,   165,   170,   146,   170,   120,   170,   165,
     165,   146,   170,   170,    92,   175,   732,    27,    34,    35,
      36,    76,   118,   387,    79,   175,    81,   593,   360,   146,
     120,   273,   840,   146,   175,    90,   464,    89,   205,   206,
     146,    93,     3,   117,  1230,   146,   782,  1090,    54,    22,
     146,    89,   146,  1131,   109,    93,   173,   299,  1136,   146,
     865,   787,   175,   165,   306,   307,     3,   173,    74,    79,
     172,  1123,   173,  1125,   460,   317,   146,   165,    84,   173,
      90,    56,   752,   171,  1270,   146,   173,   165,   140,   146,
     408,   107,   170,   145,    56,  1059,   148,   171,   340,    90,
     152,   171,   140,   141,   539,   115,   337,   145,   365,   147,
     148,   353,   173,   355,   152,    88,   173,    51,   173,   146,
      54,   146,   160,   365,   146,   443,   444,  1205,   166,   167,
     168,    92,  1175,  1176,  1177,  1178,  1179,  1180,   109,   112,
      74,   966,   103,    77,   171,   140,   862,    47,   173,   172,
      84,   173,   394,   148,   396,   397,   398,   399,   400,   401,
     595,   403,   404,   405,   406,   407,    66,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   423,   424,   425,   910,    85,   146,     8,    47,   507,
     432,   433,  1000,    61,  1033,  1034,   438,    27,   365,   441,
     442,   443,   444,   445,   446,   447,   922,    66,    56,   432,
     433,   464,  1102,   173,   114,   438,   458,    48,   441,   442,
     462,   146,    56,   480,    92,   146,    85,   897,   146,   471,
      61,   146,   146,   125,   172,   553,    61,   105,   480,   146,
     482,   483,   146,    56,   165,   146,   488,   489,   173,    79,
     118,   165,   494,   171,   146,   114,   171,   146,   150,   501,
      90,    92,   504,   172,   165,   507,   173,    92,   966,   173,
     146,   879,   165,   879,    56,   780,   165,   146,   171,   109,
     105,   173,     2,    83,   146,    56,    86,   118,   146,   165,
      32,   533,    12,   118,   125,   537,   165,   165,   540,   541,
     146,   146,   170,   165,   546,    25,    56,   165,    28,     8,
     146,   553,    61,   480,  1122,   146,  1124,    89,   140,   165,
     165,    93,   564,   145,    48,   567,   148,   569,  1026,   165,
     152,   573,   574,   575,   165,   577,   578,    61,   146,   170,
     165,   100,   173,    92,   567,   170,   569,    89,   590,   591,
     170,    93,   583,   172,   146,     2,   105,   165,    78,   864,
       3,   603,    82,    83,    56,    12,   140,    56,    92,   118,
     875,   756,   757,   165,   148,   760,   761,    20,    25,   546,
     170,    28,   172,   399,   400,   105,   165,   172,    89,  1059,
     110,    34,    93,   172,   118,   174,    56,   564,   140,    56,
     567,   125,   569,   145,  1102,   147,   148,   574,    56,    89,
     152,    89,   654,    93,   732,    93,   165,    56,   160,   125,
      63,   170,   146,    32,   166,   167,   168,   745,    89,   445,
     446,    78,    93,   300,   301,    82,    83,   822,   823,    56,
      89,   165,   458,  1141,    93,   151,   170,   153,   154,   173,
     145,   157,   158,   172,   172,   174,   174,   172,   105,   174,
     103,    82,    83,   110,    48,   165,   109,   483,   164,   172,
    1168,   174,   192,   489,   860,   165,    56,    61,   494,   172,
      89,   174,   165,   156,    93,   727,   728,   729,   730,   731,
     732,   733,   734,   736,   737,   172,   172,   174,   174,   165,
     567,   743,   569,   745,   746,   172,   748,  1012,    92,   165,
     752,   165,   754,   165,   165,   879,   119,   835,   836,   172,
     743,   763,   764,   165,   170,   170,    32,    85,   172,   165,
       8,   140,  1230,    66,   118,   792,   145,   172,   147,   148,
     172,   125,  1240,   152,   862,   192,   172,   170,   790,   791,
     792,   160,   171,     8,   796,   125,   276,   573,    90,   168,
      90,   125,   146,   283,    69,   148,   172,    27,   932,   109,
     125,   173,  1270,   796,   146,   939,   743,    48,   172,   125,
     125,   165,    35,    89,    35,    66,   170,    93,    48,   173,
     146,   833,    52,    21,   173,   837,   173,   839,   840,   841,
     125,   843,   173,   845,   922,   173,   848,   849,   175,   175,
    1103,  1104,   869,   170,   105,   146,    76,   337,   172,    79,
     862,    81,   148,   140,   148,   792,  1131,   869,   148,   276,
      90,   140,   352,   148,   140,   141,   283,   143,   144,   145,
     148,   147,   148,   140,   148,   148,   152,   148,   148,   109,
     148,   140,   148,   159,   160,   897,   140,   163,     2,   173,
     166,   167,   168,   148,   175,   172,   165,   387,    12,   911,
     912,   913,   165,   165,   916,     3,    35,   165,   165,   165,
     922,    25,    35,   145,    28,   146,    33,    33,   171,    17,
     337,   933,    20,   935,   173,   165,   170,    25,   940,   171,
     173,   943,   869,    35,   145,   352,   175,   175,    36,   165,
     175,   165,   728,   955,   730,   172,   175,   175,    46,   165,
     172,     8,   140,   173,   140,    53,   983,   969,   175,    35,
     173,   165,   165,   173,    78,    63,   456,   173,    82,    83,
     165,   983,   146,    71,   164,   164,    24,   352,   833,   272,
     992,  1020,   994,    90,   961,   997,   537,   987,  1000,    87,
     879,   105,   482,   324,   868,  1007,   110,  1009,   488,   780,
     363,    99,   518,  1142,   874,   103,   875,  1181,  1174,   499,
     665,   501,   110,   278,   786,  1103,  1104,   534,   116,   131,
      -1,    -1,    -1,  1157,  1158,     2,    -1,  1161,  1162,   966,
    1042,  1043,    -1,  1045,  1046,    12,  1048,  1049,    -1,   456,
      -1,    -1,  1054,  1055,    -1,  1057,   983,  1059,    25,  1061,
    1062,    28,    -1,  1065,  1081,    -1,    -1,  1191,    -1,  1193,
      -1,    -1,    -1,    -1,    -1,   482,    32,    -1,    -1,  1081,
      -1,   488,    -1,    -1,  1086,    -1,    -1,    -1,   192,    -1,
      -1,   571,   499,    -1,   501,   575,    -1,    -1,    -1,  1101,
    1102,  1103,  1104,   583,    27,    -1,    -1,  1109,    -1,  1111,
      -1,    78,    -1,  1115,    -1,    82,    83,    -1,    -1,  1121,
    1122,  1123,  1124,  1125,    -1,    48,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    89,    -1,   911,   912,    93,   105,  1141,
      -1,    -1,    -1,   110,    -1,  1147,  1148,  1149,  1150,  1151,
    1152,  1153,  1154,    76,  1081,    -1,    79,    -1,    81,  1086,
      -1,    -1,    -1,    -1,   571,    -1,    -1,    90,   575,    -1,
      -1,  1173,   276,    -1,    -1,    -1,   583,    -1,    -1,   283,
      -1,    -1,    -1,    -1,   140,   141,   109,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,    -1,
    1202,   157,   158,   159,   160,   161,    -1,   163,    -1,    -1,
     166,   167,   168,    -1,    -1,    -1,  1218,  1219,  1220,  1221,
      -1,   177,    32,    -1,    -1,   192,    -1,  1229,  1230,    -1,
      -1,    -1,    -1,   337,    -1,    -1,    -1,     1,  1240,    -1,
      -1,  1168,  1244,  1245,   724,    -1,    -1,    -1,   352,    -1,
    1252,  1253,  1254,  1255,  1256,  1257,    -1,    -1,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,  1270,    -1,
      -1,    -1,    36,    -1,    38,    39,    -1,    -1,    -1,    89,
      44,    -1,    -1,    93,    -1,    -1,   766,    51,    -1,   769,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1230,    -1,    -1,    -1,    -1,    -1,   276,
      74,    -1,    -1,    77,    -1,    79,   283,    81,    -1,    83,
      84,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   821,   152,  1270,    -1,   109,    -1,   827,   112,   159,
     160,   115,   456,   163,    -1,    -1,   166,   167,   168,   766,
      -1,    -1,   769,    -1,    -1,     2,    -1,   177,    -1,    -1,
     337,    -1,    32,    -1,    -1,    12,    -1,    -1,   482,    -1,
      -1,    -1,    -1,    -1,   488,   352,    -1,    -1,    25,    -1,
      -1,    28,    -1,    -1,    -1,   499,    -1,   501,    -1,    -1,
      -1,   165,    -1,    -1,    -1,    -1,    -1,   171,    -1,    -1,
      -1,    -1,    -1,    -1,   821,    -1,    -1,    -1,    -1,    -1,
     827,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,   911,   912,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    82,    83,    -1,    -1,    -1,
      -1,    -1,    -1,   933,    -1,   935,    -1,    -1,    -1,    -1,
     940,    -1,    -1,   943,    -1,    -1,    -1,   571,   105,    -1,
      -1,   575,   952,   110,   954,    -1,    -1,   957,    -1,   583,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   456,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     160,    -1,   982,    -1,    -1,    -1,   166,   167,   168,    -1,
      -1,    -1,   992,    -1,   994,   482,    -1,    -1,    -1,    -1,
      -1,   488,    -1,    -1,    -1,    -1,   933,    -1,   935,    -1,
      -1,    -1,   499,   940,   501,    -1,   943,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   952,    -1,   954,    -1,    -1,
     957,    -1,    -1,    -1,    -1,   192,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    -1,    -1,  1054,   982,    36,    -1,    38,    39,
      -1,  1061,    -1,    -1,    44,   992,    -1,   994,    -1,    -1,
      -1,    51,    -1,    -1,  1074,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   571,    -1,    -1,    -1,   575,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   583,    77,    -1,    79,
      -1,    81,    -1,    83,    -1,  1105,    86,    -1,    88,    -1,
      90,    -1,    -1,    -1,  1114,    -1,    -1,    -1,  1118,   276,
      -1,    -1,    -1,    -1,    -1,    -1,   283,  1054,    -1,   109,
      -1,    -1,   112,    -1,  1061,   115,    -1,    -1,    -1,     3,
      -1,    -1,   766,    -1,    -1,   769,    -1,  1074,     2,    -1,
      -1,    -1,    32,    17,    18,    -1,    20,    -1,    12,    -1,
      -1,    25,    -1,  1163,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    25,    -1,    -1,    28,    -1,    40,    -1,  1105,    -1,
     337,    -1,    46,    -1,    -1,   165,    50,  1114,    -1,    53,
      -1,  1118,    -1,    -1,    -1,   352,    -1,   821,    -1,    63,
      -1,    -1,    -1,   827,    -1,    -1,    70,    71,    -1,    89,
      -1,    -1,    -1,    93,    78,    -1,    -1,    -1,  1218,  1219,
    1220,  1221,    -1,    87,    78,    -1,    -1,    -1,    82,    83,
      -1,    -1,    96,    97,    -1,    99,  1163,   101,    -1,   103,
      -1,    -1,   106,    -1,  1244,  1245,   110,   111,    -1,   113,
      -1,   105,   116,    -1,    -1,    -1,   110,    -1,    -1,    -1,
     140,   141,    -1,    -1,   144,   145,    -1,   147,   148,    -1,
      -1,    -1,   152,    -1,    -1,    -1,    -1,    -1,    -1,   766,
     160,    -1,   769,    -1,    -1,    -1,   166,   167,   168,    -1,
      -1,  1218,  1219,  1220,  1221,    -1,    -1,    -1,    22,   456,
      -1,    -1,    26,    27,    -1,    -1,    -1,    -1,   172,   933,
      -1,   935,    36,    -1,    38,    39,   940,  1244,  1245,   943,
      44,    -1,    -1,    -1,    -1,   482,    -1,    51,   952,    -1,
     954,   488,    -1,   957,   821,    -1,    -1,    -1,   192,    -1,
     827,    -1,   499,    -1,   501,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     2,    77,    -1,    79,    -1,    -1,   982,    -1,
      -1,    -1,    12,    -1,    88,    -1,    90,    -1,   992,    -1,
     994,    -1,    -1,    -1,    -1,    25,    -1,    -1,    28,    -1,
      12,    -1,    -1,    -1,    -1,   109,    -1,    19,   112,    -1,
      -1,   115,    -1,     2,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    12,    -1,    37,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   571,    -1,    25,    -1,   575,    28,
      -1,    -1,   276,    -1,    -1,    -1,   583,    -1,    78,   283,
    1054,    -1,    82,    83,    -1,    -1,    -1,  1061,    -1,    -1,
      -1,    73,    -1,    -1,    -1,    -1,   933,    -1,   935,    -1,
    1074,    -1,    -1,   940,    -1,   105,   943,    89,    -1,    -1,
     110,    93,    -1,    -1,    -1,   952,    -1,   954,    -1,    78,
     957,    -1,    -1,    82,    83,    -1,    -1,    -1,    -1,    -1,
      -1,  1105,    -1,   337,    -1,    -1,    -1,    -1,    -1,    -1,
    1114,    -1,   124,    -1,  1118,   982,   105,    -1,   352,    -1,
      -1,   110,    -1,    -1,    -1,   992,    -1,   994,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    -1,   165,   166,   167,   168,    -1,    -1,  1163,
      -1,    -1,   192,    -1,    -1,   177,    -1,    -1,    -1,     2,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1054,    -1,    -1,
      -1,    -1,    25,    -1,  1061,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   192,    -1,    -1,    -1,  1074,    -1,    -1,
      -1,    -1,    -1,    -1,  1218,  1219,  1220,  1221,    -1,    -1,
      -1,    -1,   456,    -1,    -1,    -1,    -1,    -1,    -1,   766,
      -1,    -1,   769,    -1,    -1,    -1,    -1,    -1,  1105,    -1,
    1244,  1245,    -1,    -1,    -1,    78,   276,  1114,   482,    -1,
      -1,  1118,    -1,   283,   488,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   499,    -1,   501,    -1,    -1,
      -1,    -1,   105,    -1,    -1,    -1,    -1,   110,    -1,    -1,
      -1,    -1,    -1,    -1,   821,    -1,    -1,   276,    -1,    -1,
     827,    -1,    -1,    -1,   283,    -1,  1163,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   337,    -1,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
      -1,    -1,   352,    -1,    -1,    -1,    -1,    -1,    34,    -1,
      -1,    -1,    -1,    -1,    40,    -1,    -1,   571,    -1,    -1,
      46,   575,    -1,    -1,    50,    -1,    -1,    53,   337,   583,
      -1,  1218,  1219,  1220,  1221,    -1,    -1,    63,    -1,   192,
      -1,    -1,    -1,   352,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    78,    -1,    -1,    -1,    -1,  1244,  1245,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,   933,   103,   935,    -1,
     106,    -1,    -1,   940,   110,    -1,   943,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,   952,    -1,   954,    -1,    -1,
     957,    -1,    -1,    -1,    -1,    -1,   456,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   276,    -1,   982,    -1,    -1,    -1,    -1,
     283,    -1,   482,    -1,    -1,   992,    -1,   994,   488,    -1,
      -1,    -1,    -1,    -1,   170,    -1,   172,   456,    -1,   499,
      -1,   501,    22,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    36,    -1,    38,    39,
      -1,    -1,    -1,   482,    44,     2,    -1,    -1,    -1,   488,
      -1,    51,    -1,    -1,   337,    12,    -1,    -1,    -1,    -1,
     499,    -1,   501,    -1,    -1,    -1,    -1,  1054,    25,   352,
      -1,    28,    -1,    -1,  1061,    -1,    -1,    77,    -1,    79,
      -1,    81,   766,    83,    -1,   769,    86,  1074,    88,    -1,
      90,   571,    -1,    -1,    22,   575,    -1,    -1,    26,    27,
      -1,    -1,    -1,   583,    -1,    -1,    -1,    -1,    36,   109,
      38,    39,   112,    -1,    -1,   115,    44,    -1,  1105,    -1,
      -1,    78,    -1,    51,    -1,    -1,    -1,  1114,    -1,    -1,
      -1,  1118,   571,    -1,    -1,    -1,   575,   821,    66,    -1,
      -1,    -1,    -1,   827,   583,    -1,    49,    -1,   105,    77,
      -1,    79,    -1,   110,    -1,    83,    -1,    85,    86,    -1,
      88,    -1,    90,    -1,    -1,   165,    -1,    -1,    -1,    72,
      -1,   171,    -1,   456,    -1,    -1,  1163,    -1,    -1,    82,
      83,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   482,
      -1,    -1,    -1,    -1,    -1,   488,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   499,    -1,   501,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1218,  1219,  1220,  1221,   192,    -1,   165,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    -1,   933,
      -1,   935,    36,    -1,    38,    39,   940,  1244,  1245,   943,
      44,    -1,    -1,    -1,    -1,    -1,    -1,    51,   952,    -1,
     954,    -1,    -1,   957,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   766,    -1,   571,   769,
      -1,    32,   575,    77,    -1,    79,    -1,    81,   982,    83,
     583,    -1,    86,    -1,    88,    -1,    90,    -1,   992,    -1,
     994,    -1,    -1,    -1,   217,    -1,    -1,    -1,    -1,   276,
      -1,    -1,    -1,    -1,    -1,   109,   283,   766,   112,    -1,
     769,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   821,    -1,    -1,    -1,    -1,    -1,   827,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,   260,   261,   262,
     263,   264,   265,    -1,    -1,   268,   269,    -1,    -1,    -1,
    1054,    -1,   275,    -1,    -1,    -1,    -1,  1061,    -1,    -1,
     337,   165,   821,   286,    -1,   288,    -1,   171,   827,    -1,
    1074,    -1,   295,    -1,    -1,   352,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     313,   152,   153,   154,    -1,    -1,   157,   158,   159,   160,
      -1,  1105,   163,   326,   327,   166,   167,   168,    -1,    -1,
    1114,    -1,    -1,    -1,  1118,    -1,   177,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   933,    -1,   935,    -1,    -1,    -1,    -1,
     940,    -1,    -1,   943,    -1,    -1,    -1,    -1,    -1,   156,
      -1,    -1,   952,    -1,   954,    -1,    -1,   957,    -1,  1163,
      -1,    -1,   169,   766,    -1,   172,   769,   174,    -1,    -1,
      -1,    -1,    -1,    -1,   933,    -1,   935,    -1,    -1,   456,
      -1,   940,   982,    72,   943,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   992,   952,   994,   954,    -1,    -1,   957,    -1,
      -1,    -1,    -1,    -1,    -1,   482,    -1,    -1,    -1,    -1,
      -1,   488,    -1,    -1,  1218,  1219,  1220,  1221,   821,    -1,
      -1,    -1,   499,   982,   827,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   992,    -1,   994,   459,    -1,    -1,    -1,
    1244,  1245,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1054,    -1,    -1,    -1,    -1,    -1,
      -1,  1061,    -1,    -1,   487,    -1,    -1,    -1,   491,    -1,
     493,    -1,    -1,   496,  1074,    -1,    -1,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    44,    45,    46,   571,  1054,    -1,    -1,   575,    -1,
      -1,    -1,  1061,    -1,    -1,  1105,   583,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1114,  1074,    -1,    -1,  1118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     933,    -1,   935,    -1,    -1,    -1,    -1,   940,    -1,    -1,
     943,    -1,    -1,    -1,    -1,    -1,  1105,    -1,    -1,   952,
      -1,   954,    -1,    -1,   957,  1114,    -1,    -1,    -1,  1118,
      -1,    -1,    -1,  1163,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   260,   261,   262,   263,   264,   265,    -1,    -1,   982,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   992,
      -1,   994,    -1,    -1,    -1,    -1,   149,   286,   151,   288,
      -1,    -1,    -1,    -1,  1163,    -1,   295,   160,    -1,    -1,
      -1,    22,    -1,    -1,    -1,    26,    27,    -1,  1218,  1219,
    1220,  1221,    -1,    -1,   313,    36,    -1,    38,    39,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,   326,   327,    -1,
      51,    -1,    -1,    -1,  1244,  1245,    -1,    -1,    -1,   202,
      -1,  1054,    -1,    -1,    -1,    -1,    -1,    -1,  1061,  1218,
    1219,  1220,  1221,    -1,    -1,    -1,    77,    -1,    79,    -1,
      -1,  1074,    83,    -1,    -1,    86,    -1,    88,    -1,    90,
      -1,    -1,    -1,    -1,    -1,  1244,  1245,    -1,    -1,   766,
      -1,    -1,   769,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,   112,  1105,    -1,   115,   258,    -1,    -1,    -1,    -1,
      -1,  1114,    -1,   266,   267,  1118,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   279,   750,   751,    -1,
      -1,    -1,    -1,   756,   757,    -1,    -1,   760,   761,    -1,
      -1,    -1,    -1,    -1,   821,    -1,    -1,   770,    -1,    -1,
     827,    -1,    -1,    -1,   165,    -1,    -1,    -1,    -1,    -1,
    1163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     459,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   812,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   487,    -1,
      -1,    -1,   491,    -1,   493,    -1,    -1,   496,    -1,    -1,
      -1,   364,    -1,    -1,    -1,  1218,  1219,  1220,  1221,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   387,    -1,    -1,    -1,    -1,    -1,
      -1,  1244,  1245,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   933,    -1,   935,    -1,
      -1,    -1,    -1,   940,    -1,    -1,   943,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   952,    -1,   954,    -1,    -1,
     957,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     923,   924,   455,    -1,    -1,   982,    -1,   930,    -1,   932,
      -1,    -1,    -1,   466,     3,   992,   939,   994,    -1,   942,
      -1,    -1,   945,    -1,   947,    -1,   949,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,   962,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,   508,    -1,    46,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    -1,   990,    -1,   522,
      -1,    -1,    -1,   526,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,   539,    22,    -1,    78,
      -1,    26,    27,    -1,    -1,    -1,    31,  1074,    87,    -1,
      -1,    36,    -1,    38,    39,    -1,    -1,    96,    97,    44,
      99,    -1,   101,    -1,   103,    -1,    51,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,  1105,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   589,  1114,    -1,    -1,
      -1,  1118,    77,    -1,    79,    -1,    81,    -1,    83,   602,
      -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,   750,   751,    -1,    -1,    -1,    -1,   756,   757,    -1,
      -1,   760,   761,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,   172,  1107,    -1,  1163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,   665,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,
     165,    40,    -1,    -1,  1157,  1158,   171,    46,  1161,  1162,
      -1,    50,    -1,    12,    53,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,    32,    33,    -1,  1189,    -1,  1191,    78,
    1193,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,     3,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,   771,    -1,
      89,    19,    20,    -1,    93,    -1,   779,    -1,    -1,   782,
      -1,    -1,    -1,   786,   923,   924,    34,    -1,    -1,    -1,
      -1,   930,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   802,
      -1,   120,    -1,   942,    -1,   124,   945,    55,   947,   812,
     949,    -1,    -1,   172,    -1,    63,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   170,    -1,    -1,    -1,   103,    -1,    -1,   177,    -1,
      -1,   109,   865,    -1,    -1,   868,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   879,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,    -1,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    -1,    -1,   166,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,     1,    -1,
       3,    -1,     5,     6,     7,     8,     9,    10,   961,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,  1107,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,   987,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    12,    -1,    14,    15,    16,    17,
      18,    19,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,   170,   171,   172,    -1,   174,    -1,     1,   177,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,   171,   172,
      -1,   174,    -1,     1,   177,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,   171,   172,    -1,   174,    -1,     1,   177,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,   171,   172,
      -1,   174,    -1,     1,   177,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,   171,   172,    -1,   174,    -1,     1,   177,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,   171,   172,
      -1,   174,    -1,     1,   177,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      33,    34,    -1,    36,    -1,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    -1,
      53,    54,    -1,    56,    57,    58,    59,    60,    -1,    62,
      63,    64,    65,    66,    67,    68,    -1,    70,    71,    72,
      -1,    74,    -1,    -1,    77,    78,    79,    80,    -1,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,     1,   177,     3,    -1,     5,     6,     7,
       8,     9,    10,    -1,    -1,    -1,    14,    15,    16,    17,
      18,    -1,    20,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    -1,    53,    54,    -1,    56,    57,
      58,    59,    60,    -1,    62,    63,    64,    65,    66,    67,
      68,    -1,    70,    71,    72,    -1,    74,    -1,    -1,    77,
      78,    79,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    -1,    90,    91,    92,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,    -1,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,   112,   113,   114,   115,   116,    -1,
      -1,   119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
     168,    -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,
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
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,   175,     1,   177,     3,     4,     5,     6,     7,
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
       5,     6,     7,     8,     9,    10,    -1,    12,    -1,    14,
      -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,    -1,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,    -1,
      45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      55,    56,    -1,    -1,    59,    60,    -1,    -1,    63,    -1,
      65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,   140,   141,   142,   143,   144,
     145,    -1,   147,    -1,   149,    -1,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
      -1,   166,   167,   168,   169,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    15,    -1,    17,    18,    -1,    20,    -1,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    30,    -1,
      -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
      42,    43,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,
      62,    63,    64,    65,    -1,    67,    68,    -1,    70,    71,
      72,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,
      82,    -1,    -1,    -1,    -1,    87,    88,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
      -1,   103,   104,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,   119,    -1,    -1,
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
     107,   108,    -1,   110,   111,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    79,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
      -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,
     149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,   173,   174,    -1,    -1,   177,     3,
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
      -1,    -1,   106,   107,   108,    -1,   110,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,    -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,
     174,    -1,     3,   177,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    36,    -1,    -1,    -1,    40,
      41,    42,    -1,    -1,    45,    46,    -1,    48,    -1,    50,
      -1,    -1,    53,    -1,    -1,    56,    -1,    -1,    59,    60,
      -1,    -1,    63,    -1,    65,    -1,    67,    68,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    80,
      -1,    82,    -1,    -1,    -1,    -1,    87,    88,    -1,    90,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,   115,   116,    -1,    -1,    -1,    -1,
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
      98,    99,   100,   101,    -1,   103,    -1,    -1,   106,   107,
     108,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
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
     103,    -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,
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
      60,    -1,    -1,    63,    -1,    65,    -1,    67,    68,    69,
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
     170,    -1,   172,    -1,   174,    -1,     3,   177,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    34,    -1,    36,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    45,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    56,
      -1,    -1,    59,    60,    -1,    -1,    63,    -1,    65,    -1,
      67,    68,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    79,    80,    -1,    82,    -1,    -1,    -1,    -1,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,
      -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,    -1,
      -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,     3,
     177,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,    -1,
      -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,    63,
      -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,    -1,
      -1,    -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,    -1,
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
     106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
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
     171,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
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
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,   175,
       3,   177,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    34,    -1,    36,    -1,    -1,    -1,    40,    41,    42,
      -1,    -1,    45,    46,    -1,    -1,    -1,    50,    -1,    -1,
      53,    -1,    -1,    56,    -1,    -1,    59,    60,    -1,    -1,
      63,    -1,    65,    -1,    67,    68,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    78,    -1,    80,    -1,    82,
      -1,    -1,    -1,    -1,    87,    88,    89,    -1,    -1,    -1,
      93,    -1,    -1,    96,    97,    98,    99,   100,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,   142,   143,   144,    -1,    -1,    -1,
      -1,   149,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    96,    97,    98,    99,   100,   101,    -1,   103,    -1,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,
      -1,     3,   177,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    34,    -1,    36,    -1,    -1,    -1,    40,    41,
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
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,   112,   113,
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
      -1,   172,    -1,   174,   175,     3,   177,     5,     6,     7,
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
      -1,    -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,
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
     110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
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
      -1,    -1,    -1,     1,    -1,     3,    -1,    -1,    -1,   140,
     141,   142,   143,   144,    12,    -1,    -1,    -1,   149,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    34,   168,    36,    -1,
      -1,   172,    40,   174,    -1,    -1,   177,    -1,    46,    -1,
      -1,    -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,
      78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,   140,   141,   142,   143,   144,   145,    -1,   147,
      -1,    -1,    -1,   151,   152,   153,   154,   155,   156,   157,
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
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   142,   143,   144,    -1,    -1,    -1,    -1,    -1,
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
     110,    -1,    -1,   113,    -1,    -1,   116,    -1,   118,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   142,    -1,     3,     4,     5,     6,     7,     8,
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
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   142,    -1,    -1,    -1,   146,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,    -1,   142,    -1,     3,    -1,     5,
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
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   142,    -1,     3,    -1,
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
      -1,   106,    -1,     3,    -1,   110,    -1,    -1,   113,    -1,
      -1,   116,    12,    -1,    -1,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    34,    -1,    -1,   142,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,   172,    -1,   174,
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
      -1,    48,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,   124,    -1,   166,
     167,   168,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    13,   166,
     167,   168,    -1,   170,    19,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,
      -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
     120,   166,   167,   168,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    89,    -1,    -1,    -1,    93,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    -1,   120,   166,   167,   168,   124,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,   170,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    12,
      93,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    48,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,   170,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,   102,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,   170,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    -1,   166,   167,   168,    -1,   170,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,    13,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    73,    -1,    -1,    -1,    32,    -1,
      -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    89,   175,    -1,   177,    93,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    -1,   166,   167,   168,
     124,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    32,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      32,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,   124,   170,    -1,    -1,    -1,    89,    -1,    -1,
     177,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,   124,   165,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,   141,
      -1,   143,   144,   145,    -1,   147,   148,   149,    -1,   151,
     152,   153,   154,   155,    -1,   157,   158,   159,   160,   161,
     162,   163,    12,   165,   166,   167,   168,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    12,    93,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   123,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
     140,   141,   102,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
     140,   141,    -1,   143,   144,   145,    -1,   147,   148,   149,
      -1,   151,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,   162,   163,    12,    13,   166,   167,   168,    -1,
      -1,    19,    -1,   173,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,   177,
      93,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,   120,   166,   167,
     168,   124,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    32,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,   140,   141,    -1,   143,   144,   145,
      -1,   147,   148,   149,    -1,   151,   152,   153,   154,   155,
      -1,   157,   158,   159,   160,   161,   162,   163,    -1,   165,
     166,   167,   168,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,   177,    93,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    -1,   166,   167,   168,    -1,    -1,
      19,    -1,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    12,    13,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,   123,   124,   166,   167,   168,
      -1,    -1,    89,    -1,   173,    -1,    93,    -1,   177,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,   124,   166,   167,
     168,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   177,
      19,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    32,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
     177,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,   124,   166,   167,   168,
      -1,    -1,    89,    -1,   173,    -1,    93,    -1,   177,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,   124,   166,   167,
     168,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,   177,
      19,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    32,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,
     177,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,   124,   166,   167,   168,
      -1,    -1,    12,    -1,    -1,    -1,   175,    -1,   177,    19,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    32,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,    89,
      -1,    -1,    -1,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,
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
      -1,    -1,    -1,    61,    -1,    -1,   140,   141,    -1,   143,
     144,   145,    -1,   147,   148,   149,    -1,   151,   152,   153,
     154,   155,    -1,   157,   158,   159,   160,   161,   162,   163,
      -1,    89,   166,   167,   168,    93,    -1,    12,    13,    -1,
      -1,    -1,    -1,   177,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,   147,
     148,   149,    -1,   151,   152,   153,   154,   155,    -1,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,    -1,    12,    89,    -1,    -1,    -1,    93,   177,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    -1,    -1,    -1,   175,    -1,   177,   102,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,   102,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,   155,    -1,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,
      -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,   155,    -1,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   140,   141,    -1,   143,   144,   145,    -1,
     147,   148,   149,    -1,   151,   152,   153,   154,   155,    -1,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,   175,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,    -1,   143,   144,   145,    -1,   147,   148,   149,    -1,
     151,   152,   153,   154,   155,    -1,   157,   158,   159,   160,
     161,   162,   163,    -1,    89,   166,   167,   168,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,   141,    -1,   143,   144,
     145,    -1,   147,   148,   149,    -1,   151,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      89,   166,   167,   168,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    32,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,    -1,   143,   144,   145,    -1,   147,   148,
     149,    -1,   151,   152,   153,   154,    -1,    -1,   157,   158,
     159,   160,   161,   162,   163,    -1,    89,   166,   167,   168,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,    -1,
     143,   144,   145,    -1,   147,   148,   149,    -1,   151,   152,
     153,   154,    -1,    -1,   157,   158,   159,   160,   161,     1,
     163,     3,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   177,    17,    18,    -1,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,    50,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,    70,    71,
      -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,     3,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    17,    18,   106,    20,    -1,    -1,   110,    -1,
      25,   113,    27,    -1,   116,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,
      -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    87,    -1,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,     3,
      -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,
     115,   116,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      34,    -1,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    46,    -1,    -1,    -1,    50,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      17,    18,   106,    20,    -1,    -1,   110,    -1,    25,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    46,
      -1,    -1,    -1,    50,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      87,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    96,
      97,    -1,    99,    -1,   101,    -1,   103,    17,    18,   106,
      20,    -1,    -1,   110,    -1,    25,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    46,    -1,    -1,    -1,
      50,    -1,    -1,    53,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    -1,    -1,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    78,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,
       3,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,    99,
      -1,   101,    -1,   103,    17,    18,   106,    20,    -1,    -1,
     110,    -1,    25,   113,    -1,    -1,   116,    -1,    -1,    -1,
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
     225,   226,   227,   228,   229,   231,   232,   234,   236,   239,
     240,   241,   242,   243,   247,   248,   253,   254,   255,   256,
     259,   260,   267,   268,   269,   270,   271,   273,   274,   299,
     300,   304,   305,   326,   327,   328,   329,   330,   331,   332,
     340,   341,   342,   343,   344,   347,   348,   349,   350,   351,
     352,   353,   354,   356,   357,   358,   359,   360,   165,   186,
     344,   120,   333,   334,     3,   209,    14,    22,    36,    41,
      42,    45,    56,    88,   101,   170,   174,   239,   267,   326,
     331,   342,   343,   344,   347,   349,   350,   333,   344,   109,
     306,    90,   209,   186,   320,   344,     8,   190,   186,   172,
       3,    17,    18,    20,    25,    34,    40,    46,    50,    53,
      63,    70,    71,    78,    87,    96,    97,    99,   101,   103,
     106,   110,   113,   116,   211,   213,    11,   109,    79,   122,
     233,   344,   233,   344,   233,   344,    27,   115,   235,   344,
      83,    86,   194,   172,   211,   211,   211,   172,   282,   172,
     211,   307,   308,    33,   198,   216,   344,   251,   252,   344,
     344,    18,    78,    96,   113,   344,   344,   344,     8,   172,
     223,   222,     4,   294,   319,   344,   107,   108,   165,   344,
     346,   344,   216,   344,   344,   344,   100,   172,   186,   344,
     344,   187,   198,   187,   198,   344,   235,   344,   344,   344,
     344,   344,   344,   344,     1,   171,   184,   199,   319,   111,
     150,   294,   321,   322,   346,   233,   319,   344,   355,   344,
      81,   186,   170,    47,    85,   114,   195,    26,   305,   344,
       8,   255,   344,   345,    56,   145,   257,   211,     1,    31,
     211,   261,   263,   266,    54,    74,    84,   289,    27,    79,
      90,   109,   290,    27,    79,    90,   109,   288,   211,   301,
     302,   307,   164,   165,   156,   344,    12,    19,    32,    89,
      93,   124,   140,   141,   143,   144,   145,   147,   148,   149,
     151,   152,   153,   154,   155,   157,   158,   159,   160,   161,
     162,   163,   166,   167,   168,   177,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   169,   278,   172,   174,    89,    93,   172,   186,   165,
     344,   344,   344,   211,   319,    56,   170,   198,    48,   333,
     304,   165,   146,   165,   189,    22,    36,    39,    88,   191,
     194,   119,   212,   294,   323,   324,   325,   346,   170,   211,
     172,   216,    33,    48,   216,   120,   216,   336,    33,    48,
     216,   336,   216,   336,    48,   216,   336,   211,   211,   102,
     198,   102,   125,   198,   278,   195,   323,   172,   172,   198,
     186,    27,    48,    52,    76,    79,    90,   109,   185,   283,
     284,   285,   286,   287,   290,   111,   172,   211,   309,   310,
       1,   145,   314,    48,   146,   186,   216,   146,   216,    13,
     172,   172,   216,   323,   224,   224,   146,   165,   344,   344,
     165,   170,   216,   172,   323,   165,   244,   244,   216,   216,
     165,   171,   171,   184,   146,   171,   344,   146,   173,   146,
     173,   175,   336,    48,   146,   175,   336,   123,   146,   175,
       8,     1,   171,   199,   205,   206,   344,   201,   344,    66,
      37,    73,   165,   255,   257,   230,   271,   211,   319,   170,
     171,     8,   265,   125,   146,   171,    90,     1,   145,   313,
      90,     1,     3,    12,    17,    19,    20,    25,    40,    46,
      53,    55,    63,    70,    71,    87,    99,   103,   106,   110,
     116,   140,   141,   142,   143,   144,   145,   147,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   163,
     166,   167,   168,   169,   172,   210,   211,   213,   275,   276,
     277,   278,   326,   125,   303,   146,   165,   165,   344,   344,
     344,   344,   233,   344,   233,   344,   344,   344,   344,   344,
     344,   344,     3,    20,    34,    63,   103,   109,   212,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,    69,   346,   346,   346,
     346,   346,   323,   323,   233,   344,   233,   344,    27,    48,
      90,   115,   335,   338,   339,   344,   360,    33,    48,    33,
      48,   102,   172,    48,   175,   211,   233,   344,   216,   307,
     344,   191,   344,   125,   173,   146,    48,   319,    45,   344,
     233,   344,   172,   216,    45,   344,   233,   344,   216,   216,
     233,   344,   216,   125,   125,   186,    35,   186,   344,    35,
     344,    66,   173,   324,   211,   237,   238,    48,    90,   286,
     146,   173,   172,   211,   314,   310,   146,   173,    34,    50,
      97,   101,   174,   215,   315,   327,   125,   311,   344,   308,
     251,   211,   304,   344,   344,   173,   294,   344,     1,   249,
     325,   173,    21,   245,   171,   173,   173,   321,   173,   321,
     186,   175,   233,   344,   175,   186,   344,   175,   344,   175,
     344,   171,   171,   146,   165,    13,   148,   146,   165,    13,
      37,    73,   211,   140,   141,   142,   143,   144,   159,   163,
     168,   200,   277,   278,   279,   344,   200,   202,   257,   170,
     304,   165,   172,     1,   258,   264,   266,   344,   262,   174,
     215,   312,   327,   105,   291,   172,   281,   344,   140,   148,
     281,   281,   315,   327,   302,   172,   174,   165,   165,   165,
     165,   165,   165,   173,   175,    48,    90,   146,   173,    17,
      20,    25,    46,    53,    63,    71,    87,    99,   110,   116,
     326,    89,    89,    45,   233,   344,    45,   233,   344,   324,
     233,   344,   172,   333,   333,   165,   165,   294,   346,   325,
     344,   173,   344,    33,   216,    33,   216,   337,   338,   344,
      33,   216,   336,    33,   216,   336,   216,   336,   216,   336,
     344,   344,    35,   186,    35,    35,   186,   102,   198,   211,
     173,   146,   173,   211,   285,   310,   145,   318,    61,   118,
     295,   173,   309,   314,     1,   319,    69,   346,   211,   173,
     173,   171,    75,   117,   171,   250,   173,   172,   198,   211,
     246,   186,   175,   336,   175,   336,   186,   123,   205,   212,
     170,   279,   344,   111,   344,   200,   202,   146,   165,    13,
     165,   170,   258,   307,   324,   171,    31,    83,    86,   171,
     185,   218,   221,   266,   263,     1,   175,   319,   295,   284,
     173,     3,   103,   276,   278,   173,   175,   338,   314,   326,
     326,   344,    33,    33,   344,    33,    33,   173,   175,   175,
     324,   216,   216,   216,   102,    45,   344,    45,   344,   146,
     173,   102,    45,   344,   216,    45,   344,   216,   216,   216,
     186,   186,   344,   186,    35,   165,   165,   238,   198,   318,
     173,   174,   215,   294,   317,   327,   150,   280,   311,     3,
      92,   103,   296,   297,   298,   344,   197,   217,   293,   311,
     175,    48,   175,   172,   172,    33,   186,   319,   246,   145,
     198,    45,   186,   344,   175,    45,   186,   344,   175,   344,
     200,    13,    37,    73,    37,    73,   165,   165,   279,   344,
     344,   258,   171,   165,   173,     8,   220,   218,   175,   312,
     327,   175,   272,   173,   281,   281,   311,   102,    45,    45,
     102,    45,    45,    45,    45,   173,   344,   344,   344,   338,
     344,   344,   344,    35,   186,   280,   311,   318,   175,   319,
     294,   344,   298,   118,   146,   125,   151,   153,   154,   157,
     158,    61,   344,   315,   327,   323,   323,   186,   216,   173,
     344,   344,   186,   344,   186,   171,   111,   344,   200,   202,
     200,   202,    13,   171,   165,   219,   221,   312,   327,   165,
     292,   293,   344,   344,   344,   344,   344,   344,   102,   102,
     102,   102,   186,   280,   311,   294,   316,   317,   327,    48,
     175,   344,   297,   298,   298,   298,   298,   298,   298,   296,
     175,   173,   173,   198,   102,   102,   165,   165,   165,   165,
     344,   221,   102,   102,   102,   102,   102,   102,   344,   344,
     344,   344,   344,   316,   317,   327,   164,   164,   344,   344,
     175,   316
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   186,   186,
     186,   186,   186,   186,   186,   186,   186,   186,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   188,   189,   188,   190,   188,   191,
     191,   192,   193,   193,   193,   194,   194,   194,   195,   195,
     196,   197,   197,   197,   198,   199,   199,   200,   200,   200,
     200,   200,   200,   201,   201,   201,   201,   201,   201,   202,
     202,   203,   203,   203,   203,   203,   203,   203,   204,   205,
     205,   205,   205,   206,   206,   207,   208,   208,   208,   208,
     208,   208,   209,   209,   210,   210,   210,   210,   210,   210,
     211,   211,   211,   211,   211,   211,   212,   212,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   215,
     215,   215,   215,   216,   216,   217,   217,   218,   219,   218,
     220,   218,   221,   221,   222,   221,   223,   221,   224,   224,
     224,   224,   224,   224,   224,   225,   225,   225,   225,   226,
     227,   227,   228,   229,   229,   229,   230,   229,   231,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   232,   232,   232,   232,   232,   232,
     232,   232,   232,   232,   233,   234,   234,   234,   234,   234,
     234,   234,   234,   234,   234,   234,   234,   235,   235,   236,
     236,   237,   237,   238,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   240,   240,   240,
     240,   240,   240,   241,   241,   241,   242,   242,   242,   243,
     243,   243,   243,   244,   244,   245,   245,   245,   246,   246,
     247,   248,   248,   249,   249,   250,   250,   250,   251,   251,
     251,   252,   252,   253,   254,   254,   255,   256,   256,   256,
     257,   257,   258,   258,   258,   259,   259,   260,   261,   261,
     262,   261,   263,   264,   263,   265,   263,   266,   266,   267,
     268,   269,   269,   269,   270,   272,   271,   273,   273,   273,
     273,   273,   274,   275,   275,   276,   276,   276,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   277,   277,   277,   277,   277,   277,
     277,   277,   277,   277,   278,   278,   278,   278,   278,   278,
     278,   278,   278,   278,   278,   278,   279,   279,   280,   280,
     280,   281,   281,   282,   283,   283,   284,   284,   285,   285,
     285,   285,   285,   285,   286,   286,   287,   287,   287,   287,
     287,   287,   287,   287,   287,   288,   288,   288,   288,   288,
     288,   289,   289,   289,   290,   290,   290,   290,   290,   290,
     291,   291,   292,   292,   293,   293,   294,   295,   295,   295,
     295,   295,   296,   296,   297,   297,   297,   297,   297,   297,
     297,   298,   298,   299,   300,   300,   300,   301,   301,   302,
     303,   303,   303,   304,   304,   304,   304,   304,   306,   305,
     305,   307,   307,   308,   308,   309,   309,   309,   310,   310,
     310,   311,   311,   311,   312,   312,   312,   312,   312,   312,
     312,   313,   313,   313,   313,   313,   314,   314,   314,   314,
     314,   315,   315,   315,   315,   316,   316,   316,   317,   317,
     317,   317,   317,   318,   318,   318,   318,   318,   319,   319,
     319,   319,   320,   320,   321,   321,   321,   322,   322,   323,
     323,   324,   324,   325,   325,   325,   325,   326,   326,   327,
     327,   327,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   329,   329,
     329,   330,   331,   332,   332,   332,   332,   332,   332,   332,
     332,   333,   333,   334,   335,   335,   336,   337,   337,   338,
     338,   338,   339,   339,   339,   339,   339,   339,   340,   340,
     341,   341,   341,   341,   341,   342,   342,   342,   342,   342,
     343,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   345,   345,
     346,   346,   346,   347,   347,   347,   347,   348,   348,   348,
     348,   348,   349,   349,   349,   350,   350,   350,   350,   350,
     350,   351,   351,   351,   351,   352,   352,   353,   353,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   355,   355,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   357,   357,   357,
     357,   357,   357,   357,   358,   358,   358,   358,   359,   359,
     359,   359,   360,   360,   360,   360,   360,   360,   360
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     2,     3,     3,     3,     3,     2,     1,     1,
       1,     1,     1,     2,     1,     3,     3,     3,     3,     2,
       3,     3,     2,     2,     1,     0,     4,     0,     3,     1,
       1,     4,     3,     4,     4,     0,     1,     1,     0,     1,
       6,     2,     3,     3,     1,     1,     2,     1,     1,     3,
       3,     3,     5,     1,     3,     3,     3,     5,     5,     0,
       1,     4,     6,     8,     8,     6,     8,     8,     4,     1,
       3,     3,     5,     1,     3,     3,     4,     4,     4,     4,
       4,     4,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     3,     1,     0,     4,
       0,     3,     1,     1,     0,     3,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     3,     5,     5,     2,     1,
       1,     1,     1,     6,     7,     3,     0,     6,     2,     5,
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
       0,     2,     0,     2,     3,     3,     3,     3,     1,     2,
       0,     4,     1,     0,     4,     0,     3,     1,     3,     6,
       0,     1,     1,     1,     1,     0,     7,     4,     4,     6,
       6,     4,     2,     1,     3,     1,     1,     2,     1,     1,
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
#line 605 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6492 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 609 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6498 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 610 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6504 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 617 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6512 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 625 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6520 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 629 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6529 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: deprecated_decl_stmt  */
#line 637 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6535 "bison-chpl-lib.cpp"
    break;

  case 11: /* stmt: include_module_stmt  */
#line 638 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6541 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt: block_stmt  */
#line 639 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6547 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt: use_stmt  */
#line 640 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6553 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt: import_stmt  */
#line 641 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6559 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt: require_stmt  */
#line 642 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6565 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt: extern_block_stmt  */
#line 643 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6571 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt: implements_stmt  */
#line 644 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6577 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt: interface_stmt  */
#line 645 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6583 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt: TDEFER stmt  */
#line 647 "chpl.ypp"
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
#line 6600 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt: try_stmt  */
#line 659 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6606 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt: return_stmt  */
#line 660 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6612 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt: TATOMIC stmt  */
#line 661 "chpl.ypp"
                            { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 6618 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt: TBREAK opt_label_ident TSEMI  */
#line 663 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6631 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt: TCONTINUE opt_label_ident TSEMI  */
#line 672 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6644 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt: TLABEL ident_def stmt  */
#line 681 "chpl.ypp"
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
#line 6671 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt: TYIELD expr TSEMI  */
#line 704 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6682 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt: error TSEMI  */
#line 711 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclState();
  }
#line 6691 "bison-chpl-lib.cpp"
    break;

  case 28: /* tryable_stmt: assignment_stmt  */
#line 718 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6697 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: if_stmt  */
#line 719 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6703 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: loop_stmt  */
#line 720 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6709 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: select_stmt  */
#line 721 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6715 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: manage_stmt  */
#line 722 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6721 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 723 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6727 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: throw_stmt  */
#line 724 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6733 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 726 "chpl.ypp"
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
#line 6751 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 740 "chpl.ypp"
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
#line 6770 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TDELETE simple_expr_ls TSEMI  */
#line 755 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6782 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 763 "chpl.ypp"
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
#line 6798 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL do_stmt  */
#line 775 "chpl.ypp"
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
#line 6813 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TON expr do_stmt  */
#line 786 "chpl.ypp"
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
#line 6829 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 798 "chpl.ypp"
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
#line 6845 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL do_stmt  */
#line 810 "chpl.ypp"
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
#line 6860 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSYNC stmt  */
#line 821 "chpl.ypp"
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
#line 6877 "bison-chpl-lib.cpp"
    break;

  case 45: /* $@1: %empty  */
#line 838 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 6885 "bison-chpl-lib.cpp"
    break;

  case 46: /* deprecated_decl_stmt: TDEPRECATED STRINGLITERAL $@1 deprecated_decl_base  */
#line 842 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6893 "bison-chpl-lib.cpp"
    break;

  case 47: /* $@2: %empty  */
#line 846 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 6901 "bison-chpl-lib.cpp"
    break;

  case 48: /* deprecated_decl_stmt: TDEPRECATED $@2 deprecated_decl_base  */
#line 850 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 51: /* module_decl_start: access_control opt_prototype TMODULE ident_def  */
#line 862 "chpl.ypp"
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
#line 6930 "bison-chpl-lib.cpp"
    break;

  case 52: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 882 "chpl.ypp"
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
#line 6949 "bison-chpl-lib.cpp"
    break;

  case 53: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 897 "chpl.ypp"
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
#line 6968 "bison-chpl-lib.cpp"
    break;

  case 54: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 912 "chpl.ypp"
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
#line 6985 "bison-chpl-lib.cpp"
    break;

  case 55: /* access_control: %empty  */
#line 927 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6991 "bison-chpl-lib.cpp"
    break;

  case 56: /* access_control: TPUBLIC  */
#line 928 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6998 "bison-chpl-lib.cpp"
    break;

  case 57: /* access_control: TPRIVATE  */
#line 930 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7005 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_prototype: %empty  */
#line 935 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7011 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_prototype: TPROTOTYPE  */
#line 936 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7018 "bison-chpl-lib.cpp"
    break;

  case 60: /* include_module_stmt: TINCLUDE access_control opt_prototype TMODULE ident_def TSEMI  */
#line 942 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7028 "bison-chpl-lib.cpp"
    break;

  case 61: /* block_stmt_body: TLCBR TRCBR  */
#line 960 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7036 "bison-chpl-lib.cpp"
    break;

  case 62: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 964 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7044 "bison-chpl-lib.cpp"
    break;

  case 63: /* block_stmt_body: TLCBR error TRCBR  */
#line 968 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7056 "bison-chpl-lib.cpp"
    break;

  case 64: /* block_stmt: block_stmt_body  */
#line 979 "chpl.ypp"
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
#line 7075 "bison-chpl-lib.cpp"
    break;

  case 65: /* stmt_ls: toplevel_stmt  */
#line 997 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7081 "bison-chpl-lib.cpp"
    break;

  case 66: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 998 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7087 "bison-chpl-lib.cpp"
    break;

  case 67: /* renames_ls: expr  */
#line 1003 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7095 "bison-chpl-lib.cpp"
    break;

  case 68: /* renames_ls: all_op_name  */
#line 1007 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7103 "bison-chpl-lib.cpp"
    break;

  case 69: /* renames_ls: expr TAS expr  */
#line 1011 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7112 "bison-chpl-lib.cpp"
    break;

  case 70: /* renames_ls: renames_ls TCOMMA expr  */
#line 1016 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7120 "bison-chpl-lib.cpp"
    break;

  case 71: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1020 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7128 "bison-chpl-lib.cpp"
    break;

  case 72: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1024 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7137 "bison-chpl-lib.cpp"
    break;

  case 73: /* use_renames_ls: expr  */
#line 1034 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(node);
  }
#line 7146 "bison-chpl-lib.cpp"
    break;

  case 74: /* use_renames_ls: expr TAS expr  */
#line 1039 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7156 "bison-chpl-lib.cpp"
    break;

  case 75: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1045 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as));
    (yyval.exprList) = context->makeList(node);
  }
#line 7167 "bison-chpl-lib.cpp"
    break;

  case 76: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1052 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7176 "bison-chpl-lib.cpp"
    break;

  case 77: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1057 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7187 "bison-chpl-lib.cpp"
    break;

  case 78: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1064 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7199 "bison-chpl-lib.cpp"
    break;

  case 79: /* opt_only_ls: %empty  */
#line 1074 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7205 "bison-chpl-lib.cpp"
    break;

  case 80: /* opt_only_ls: renames_ls  */
#line 1075 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7211 "bison-chpl-lib.cpp"
    break;

  case 81: /* use_stmt: access_control TUSE use_renames_ls TSEMI  */
#line 1080 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7219 "bison-chpl-lib.cpp"
    break;

  case 82: /* use_stmt: access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1084 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7230 "bison-chpl-lib.cpp"
    break;

  case 83: /* use_stmt: access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1091 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7242 "bison-chpl-lib.cpp"
    break;

  case 84: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1099 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
  }
#line 7255 "bison-chpl-lib.cpp"
    break;

  case 85: /* use_stmt: access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1108 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7266 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_stmt: access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1115 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7278 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_stmt: access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1123 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
  }
#line 7291 "bison-chpl-lib.cpp"
    break;

  case 88: /* import_stmt: access_control TIMPORT import_ls TSEMI  */
#line 1135 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
  }
#line 7299 "bison-chpl-lib.cpp"
    break;

  case 89: /* import_expr: expr  */
#line 1142 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)));
  }
#line 7307 "bison-chpl-lib.cpp"
    break;

  case 90: /* import_expr: expr TDOT all_op_name  */
#line 1146 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-2].expr)),
                                        VisibilityClause::NONE,
                                        context->consume(ident));
  }
#line 7318 "bison-chpl-lib.cpp"
    break;

  case 91: /* import_expr: expr TAS ident_use  */
#line 1153 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as));
  }
#line 7328 "bison-chpl-lib.cpp"
    break;

  case 92: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1159 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)));
  }
#line 7338 "bison-chpl-lib.cpp"
    break;

  case 93: /* import_ls: import_expr  */
#line 1167 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7344 "bison-chpl-lib.cpp"
    break;

  case 94: /* import_ls: import_ls TCOMMA import_expr  */
#line 1168 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7350 "bison-chpl-lib.cpp"
    break;

  case 95: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1173 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7360 "bison-chpl-lib.cpp"
    break;

  case 96: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1182 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7368 "bison-chpl-lib.cpp"
    break;

  case 97: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1186 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7376 "bison-chpl-lib.cpp"
    break;

  case 98: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1190 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7384 "bison-chpl-lib.cpp"
    break;

  case 99: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1194 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7392 "bison-chpl-lib.cpp"
    break;

  case 100: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1198 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7400 "bison-chpl-lib.cpp"
    break;

  case 101: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1202 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7408 "bison-chpl-lib.cpp"
    break;

  case 102: /* opt_label_ident: %empty  */
#line 1210 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7414 "bison-chpl-lib.cpp"
    break;

  case 103: /* opt_label_ident: TIDENT  */
#line 1211 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7420 "bison-chpl-lib.cpp"
    break;

  case 104: /* ident_fn_def: TIDENT  */
#line 1215 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7426 "bison-chpl-lib.cpp"
    break;

  case 105: /* ident_fn_def: TNONE  */
#line 1216 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7432 "bison-chpl-lib.cpp"
    break;

  case 106: /* ident_fn_def: TTHIS  */
#line 1217 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7438 "bison-chpl-lib.cpp"
    break;

  case 107: /* ident_fn_def: TFALSE  */
#line 1218 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7444 "bison-chpl-lib.cpp"
    break;

  case 108: /* ident_fn_def: TTRUE  */
#line 1219 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7450 "bison-chpl-lib.cpp"
    break;

  case 109: /* ident_fn_def: internal_type_ident_def  */
#line 1220 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7456 "bison-chpl-lib.cpp"
    break;

  case 110: /* ident_def: TIDENT  */
#line 1223 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7462 "bison-chpl-lib.cpp"
    break;

  case 111: /* ident_def: TNONE  */
#line 1224 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'none'"); }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 112: /* ident_def: TTHIS  */
#line 1225 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'this'"); }
#line 7474 "bison-chpl-lib.cpp"
    break;

  case 113: /* ident_def: TFALSE  */
#line 1226 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'false'"); }
#line 7480 "bison-chpl-lib.cpp"
    break;

  case 114: /* ident_def: TTRUE  */
#line 1227 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word 'true'"); }
#line 7486 "bison-chpl-lib.cpp"
    break;

  case 115: /* ident_def: internal_type_ident_def  */
#line 1228 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); ERROR((yyloc), "redefining reserved word"); }
#line 7492 "bison-chpl-lib.cpp"
    break;

  case 116: /* ident_use: TIDENT  */
#line 1240 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7498 "bison-chpl-lib.cpp"
    break;

  case 117: /* ident_use: TTHIS  */
#line 1241 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7504 "bison-chpl-lib.cpp"
    break;

  case 137: /* scalar_type: TBOOL  */
#line 1274 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7510 "bison-chpl-lib.cpp"
    break;

  case 138: /* scalar_type: TENUM  */
#line 1275 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7516 "bison-chpl-lib.cpp"
    break;

  case 139: /* scalar_type: TINT  */
#line 1276 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7522 "bison-chpl-lib.cpp"
    break;

  case 140: /* scalar_type: TUINT  */
#line 1277 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7528 "bison-chpl-lib.cpp"
    break;

  case 141: /* scalar_type: TREAL  */
#line 1278 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7534 "bison-chpl-lib.cpp"
    break;

  case 142: /* scalar_type: TIMAG  */
#line 1279 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7540 "bison-chpl-lib.cpp"
    break;

  case 143: /* scalar_type: TCOMPLEX  */
#line 1280 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7546 "bison-chpl-lib.cpp"
    break;

  case 144: /* scalar_type: TBYTES  */
#line 1281 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7552 "bison-chpl-lib.cpp"
    break;

  case 145: /* scalar_type: TSTRING  */
#line 1282 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7558 "bison-chpl-lib.cpp"
    break;

  case 146: /* scalar_type: TLOCALE  */
#line 1283 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7564 "bison-chpl-lib.cpp"
    break;

  case 147: /* scalar_type: TNOTHING  */
#line 1284 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7570 "bison-chpl-lib.cpp"
    break;

  case 148: /* scalar_type: TVOID  */
#line 1285 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7576 "bison-chpl-lib.cpp"
    break;

  case 153: /* do_stmt: TDO stmt  */
#line 1299 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7582 "bison-chpl-lib.cpp"
    break;

  case 154: /* do_stmt: block_stmt  */
#line 1300 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7588 "bison-chpl-lib.cpp"
    break;

  case 155: /* return_stmt: TRETURN TSEMI  */
#line 1305 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7599 "bison-chpl-lib.cpp"
    break;

  case 156: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1312 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 158: /* $@3: %empty  */
#line 1323 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 7618 "bison-chpl-lib.cpp"
    break;

  case 159: /* deprecated_class_level_stmt: TDEPRECATED STRINGLITERAL $@3 class_level_stmt  */
#line 1327 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7626 "bison-chpl-lib.cpp"
    break;

  case 160: /* $@4: %empty  */
#line 1331 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 7634 "bison-chpl-lib.cpp"
    break;

  case 161: /* deprecated_class_level_stmt: TDEPRECATED $@4 class_level_stmt  */
#line 1335 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 7642 "bison-chpl-lib.cpp"
    break;

  case 162: /* class_level_stmt: TSEMI  */
#line 1342 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), nullptr);
    }
#line 7650 "bison-chpl-lib.cpp"
    break;

  case 163: /* class_level_stmt: inner_class_level_stmt  */
#line 1346 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7660 "bison-chpl-lib.cpp"
    break;

  case 164: /* $@5: %empty  */
#line 1351 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7667 "bison-chpl-lib.cpp"
    break;

  case 165: /* class_level_stmt: TPUBLIC $@5 inner_class_level_stmt  */
#line 1353 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7676 "bison-chpl-lib.cpp"
    break;

  case 166: /* $@6: %empty  */
#line 1357 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7683 "bison-chpl-lib.cpp"
    break;

  case 167: /* class_level_stmt: TPRIVATE $@6 inner_class_level_stmt  */
#line 1359 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7692 "bison-chpl-lib.cpp"
    break;

  case 175: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1377 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attribute)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7701 "bison-chpl-lib.cpp"
    break;

  case 176: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1382 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7710 "bison-chpl-lib.cpp"
    break;

  case 177: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1387 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attribute)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7719 "bison-chpl-lib.cpp"
    break;

  case 178: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1392 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attribute)), (yyvsp[0].commentsAndStmt));
    }
#line 7727 "bison-chpl-lib.cpp"
    break;

  case 179: /* forwarding_decl_start: TFORWARDING  */
#line 1399 "chpl.ypp"
  {
    (yyval.attribute) = context->buildAttributes((yyloc)).release();
    context->resetAttributePartsState();
  }
#line 7736 "bison-chpl-lib.cpp"
    break;

  case 180: /* extern_or_export: TEXTERN  */
#line 1406 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7742 "bison-chpl-lib.cpp"
    break;

  case 181: /* extern_or_export: TEXPORT  */
#line 1407 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7748 "bison-chpl-lib.cpp"
    break;

  case 182: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1412 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7758 "bison-chpl-lib.cpp"
    break;

  case 183: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1422 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7767 "bison-chpl-lib.cpp"
    break;

  case 184: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1428 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 185: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1435 "chpl.ypp"
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
    (yyval.commentsAndStmt) = context->buildExternExportFunctionDecl((yyloc), fp);
  }
#line 7800 "bison-chpl-lib.cpp"
    break;

  case 186: /* $@7: %empty  */
#line 1453 "chpl.ypp"
  {
    // This will be consumed when building the first variable in the list.
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7809 "bison-chpl-lib.cpp"
    break;

  case 187: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@7 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1458 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7818 "bison-chpl-lib.cpp"
    break;

  case 188: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1466 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7826 "bison-chpl-lib.cpp"
    break;

  case 189: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1473 "chpl.ypp"
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
#line 7848 "bison-chpl-lib.cpp"
    break;

  case 190: /* loop_stmt: TWHILE expr do_stmt  */
#line 1491 "chpl.ypp"
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
#line 7865 "bison-chpl-lib.cpp"
    break;

  case 191: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1504 "chpl.ypp"
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
#line 7882 "bison-chpl-lib.cpp"
    break;

  case 192: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1517 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7890 "bison-chpl-lib.cpp"
    break;

  case 193: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1521 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7898 "bison-chpl-lib.cpp"
    break;

  case 194: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1525 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7906 "bison-chpl-lib.cpp"
    break;

  case 195: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1529 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7914 "bison-chpl-lib.cpp"
    break;

  case 196: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1533 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7922 "bison-chpl-lib.cpp"
    break;

  case 197: /* loop_stmt: TFOR expr do_stmt  */
#line 1537 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7930 "bison-chpl-lib.cpp"
    break;

  case 198: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1541 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7938 "bison-chpl-lib.cpp"
    break;

  case 199: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1545 "chpl.ypp"
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
#line 7960 "bison-chpl-lib.cpp"
    break;

  case 200: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1563 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7968 "bison-chpl-lib.cpp"
    break;

  case 201: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1567 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7976 "bison-chpl-lib.cpp"
    break;

  case 202: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1571 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7984 "bison-chpl-lib.cpp"
    break;

  case 203: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1575 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7992 "bison-chpl-lib.cpp"
    break;

  case 204: /* loop_stmt: TFORALL expr do_stmt  */
#line 1579 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8000 "bison-chpl-lib.cpp"
    break;

  case 205: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1583 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8008 "bison-chpl-lib.cpp"
    break;

  case 206: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1587 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8016 "bison-chpl-lib.cpp"
    break;

  case 207: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1591 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8024 "bison-chpl-lib.cpp"
    break;

  case 208: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1595 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8032 "bison-chpl-lib.cpp"
    break;

  case 209: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1599 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8040 "bison-chpl-lib.cpp"
    break;

  case 210: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1603 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8048 "bison-chpl-lib.cpp"
    break;

  case 211: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1607 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8056 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1611 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8064 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1615 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8072 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1619 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8080 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1623 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8088 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1627 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8096 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1631 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8104 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1635 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8112 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1639 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8120 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1643 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8128 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8136 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1651 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8145 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1656 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8154 "bison-chpl-lib.cpp"
    break;

  case 224: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1664 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8164 "bison-chpl-lib.cpp"
    break;

  case 225: /* if_stmt: TIF expr TTHEN stmt  */
#line 1673 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8172 "bison-chpl-lib.cpp"
    break;

  case 226: /* if_stmt: TIF expr block_stmt  */
#line 1677 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8180 "bison-chpl-lib.cpp"
    break;

  case 227: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1681 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8188 "bison-chpl-lib.cpp"
    break;

  case 228: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1685 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8196 "bison-chpl-lib.cpp"
    break;

  case 229: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1689 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8204 "bison-chpl-lib.cpp"
    break;

  case 230: /* if_stmt: TIF ifvar block_stmt  */
#line 1693 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8212 "bison-chpl-lib.cpp"
    break;

  case 231: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1697 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8220 "bison-chpl-lib.cpp"
    break;

  case 232: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1701 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8228 "bison-chpl-lib.cpp"
    break;

  case 233: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1705 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8237 "bison-chpl-lib.cpp"
    break;

  case 234: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1710 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8246 "bison-chpl-lib.cpp"
    break;

  case 235: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1715 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8255 "bison-chpl-lib.cpp"
    break;

  case 236: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1720 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8264 "bison-chpl-lib.cpp"
    break;

  case 237: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1728 "chpl.ypp"
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
#line 8283 "bison-chpl-lib.cpp"
    break;

  case 238: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1743 "chpl.ypp"
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
#line 8302 "bison-chpl-lib.cpp"
    break;

  case 239: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1761 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8308 "bison-chpl-lib.cpp"
    break;

  case 240: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1763 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8314 "bison-chpl-lib.cpp"
    break;

  case 241: /* ifc_formal_ls: ifc_formal  */
#line 1767 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8320 "bison-chpl-lib.cpp"
    break;

  case 242: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1768 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8326 "bison-chpl-lib.cpp"
    break;

  case 243: /* ifc_formal: ident_def  */
#line 1773 "chpl.ypp"
             { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8332 "bison-chpl-lib.cpp"
    break;

  case 256: /* implements_type_ident: implements_type_error_ident  */
#line 1791 "chpl.ypp"
  {
    std::string s = "type ";
    s += "'"; s += (yyvsp[0].uniqueStr).c_str(); s += "'";
    s += " not allowed to implement an interface";
    context->noteError((yyloc), s);
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8344 "bison-chpl-lib.cpp"
    break;

  case 263: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1820 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8350 "bison-chpl-lib.cpp"
    break;

  case 264: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1822 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8356 "bison-chpl-lib.cpp"
    break;

  case 265: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1824 "chpl.ypp"
    { (yyval.commentsAndStmt) = TODOSTMT((yyloc)); }
#line 8362 "bison-chpl-lib.cpp"
    break;

  case 266: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1829 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8368 "bison-chpl-lib.cpp"
    break;

  case 267: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1831 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8374 "bison-chpl-lib.cpp"
    break;

  case 268: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1833 "chpl.ypp"
    { (yyval.expr) = TODOEXPR((yyloc)); }
#line 8380 "bison-chpl-lib.cpp"
    break;

  case 269: /* try_stmt: TTRY tryable_stmt  */
#line 1838 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8388 "bison-chpl-lib.cpp"
    break;

  case 270: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1842 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8396 "bison-chpl-lib.cpp"
    break;

  case 271: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1846 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8404 "bison-chpl-lib.cpp"
    break;

  case 272: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1850 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8412 "bison-chpl-lib.cpp"
    break;

  case 273: /* catch_expr_ls: %empty  */
#line 1856 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8418 "bison-chpl-lib.cpp"
    break;

  case 274: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1857 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8424 "bison-chpl-lib.cpp"
    break;

  case 275: /* catch_expr: TCATCH block_stmt  */
#line 1862 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 276: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1866 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8440 "bison-chpl-lib.cpp"
    break;

  case 277: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1870 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8448 "bison-chpl-lib.cpp"
    break;

  case 278: /* catch_expr_inner: ident_def  */
#line 1877 "chpl.ypp"
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
#line 8465 "bison-chpl-lib.cpp"
    break;

  case 279: /* catch_expr_inner: ident_def TCOLON expr  */
#line 1890 "chpl.ypp"
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
#line 8482 "bison-chpl-lib.cpp"
    break;

  case 280: /* throw_stmt: TTHROW expr TSEMI  */
#line 1906 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8492 "bison-chpl-lib.cpp"
    break;

  case 281: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 1914 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8500 "bison-chpl-lib.cpp"
    break;

  case 282: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 1918 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8510 "bison-chpl-lib.cpp"
    break;

  case 283: /* when_stmt_ls: %empty  */
#line 1926 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8516 "bison-chpl-lib.cpp"
    break;

  case 284: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 1927 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8522 "bison-chpl-lib.cpp"
    break;

  case 285: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 1932 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8530 "bison-chpl-lib.cpp"
    break;

  case 286: /* when_stmt: TOTHERWISE stmt  */
#line 1936 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8539 "bison-chpl-lib.cpp"
    break;

  case 287: /* when_stmt: TOTHERWISE TDO stmt  */
#line 1941 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8548 "bison-chpl-lib.cpp"
    break;

  case 288: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 1949 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8557 "bison-chpl-lib.cpp"
    break;

  case 289: /* manager_expr: expr TAS ident_def  */
#line 1954 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8565 "bison-chpl-lib.cpp"
    break;

  case 290: /* manager_expr: expr  */
#line 1958 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8573 "bison-chpl-lib.cpp"
    break;

  case 291: /* manager_expr_ls: manager_expr  */
#line 1964 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8579 "bison-chpl-lib.cpp"
    break;

  case 292: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 1965 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8585 "bison-chpl-lib.cpp"
    break;

  case 293: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 1970 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8593 "bison-chpl-lib.cpp"
    break;

  case 294: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1979 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8602 "bison-chpl-lib.cpp"
    break;

  case 295: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 1984 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8613 "bison-chpl-lib.cpp"
    break;

  case 296: /* class_start: class_tag ident_def  */
#line 1995 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8621 "bison-chpl-lib.cpp"
    break;

  case 297: /* class_tag: TCLASS  */
#line 2001 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8627 "bison-chpl-lib.cpp"
    break;

  case 298: /* class_tag: TRECORD  */
#line 2002 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8633 "bison-chpl-lib.cpp"
    break;

  case 299: /* class_tag: TUNION  */
#line 2003 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8639 "bison-chpl-lib.cpp"
    break;

  case 300: /* opt_inherit: %empty  */
#line 2007 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8645 "bison-chpl-lib.cpp"
    break;

  case 301: /* opt_inherit: TCOLON expr_ls  */
#line 2008 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8651 "bison-chpl-lib.cpp"
    break;

  case 302: /* class_level_stmt_ls: %empty  */
#line 2012 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8660 "bison-chpl-lib.cpp"
    break;

  case 303: /* class_level_stmt_ls: class_level_stmt_ls deprecated_class_level_stmt  */
#line 2017 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8668 "bison-chpl-lib.cpp"
    break;

  case 304: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls deprecated_class_level_stmt  */
#line 2021 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8676 "bison-chpl-lib.cpp"
    break;

  case 305: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2028 "chpl.ypp"
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
#line 8697 "bison-chpl-lib.cpp"
    break;

  case 306: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2045 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8711 "bison-chpl-lib.cpp"
    break;

  case 307: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2058 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8719 "bison-chpl-lib.cpp"
    break;

  case 308: /* enum_ls: deprecated_enum_item  */
#line 2065 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8728 "bison-chpl-lib.cpp"
    break;

  case 309: /* enum_ls: enum_ls TCOMMA  */
#line 2070 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8738 "bison-chpl-lib.cpp"
    break;

  case 310: /* $@8: %empty  */
#line 2076 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributePartsState();
  }
#line 8747 "bison-chpl-lib.cpp"
    break;

  case 311: /* enum_ls: enum_ls TCOMMA $@8 deprecated_enum_item  */
#line 2081 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributePartsState();
  }
#line 8756 "bison-chpl-lib.cpp"
    break;

  case 313: /* $@9: %empty  */
#line 2090 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), (yyvsp[0].expr));
  }
#line 8764 "bison-chpl-lib.cpp"
    break;

  case 314: /* deprecated_enum_item: TDEPRECATED STRINGLITERAL $@9 enum_item  */
#line 2094 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8772 "bison-chpl-lib.cpp"
    break;

  case 315: /* $@10: %empty  */
#line 2098 "chpl.ypp"
  {
    context->noteDeprecation((yyloc), nullptr);
  }
#line 8780 "bison-chpl-lib.cpp"
    break;

  case 316: /* deprecated_enum_item: TDEPRECATED $@10 enum_item  */
#line 2102 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 8788 "bison-chpl-lib.cpp"
    break;

  case 317: /* enum_item: ident_def  */
#line 2109 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8799 "bison-chpl-lib.cpp"
    break;

  case 318: /* enum_item: ident_def TASSIGN expr  */
#line 2116 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributes((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8812 "bison-chpl-lib.cpp"
    break;

  case 319: /* lambda_decl_expr: TLAMBDA req_formal_ls opt_ret_tag opt_type opt_lifetime_where function_body_stmt  */
#line 2128 "chpl.ypp"
  {
    // TODO (dlongnecke): Leave this unimplemented for now? Since we will
    // be addressing FCFs in 1.26...
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 8822 "bison-chpl-lib.cpp"
    break;

  case 321: /* linkage_spec: linkage_spec_empty  */
#line 2139 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8828 "bison-chpl-lib.cpp"
    break;

  case 322: /* linkage_spec: TINLINE  */
#line 2140 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8835 "bison-chpl-lib.cpp"
    break;

  case 323: /* linkage_spec: TOVERRIDE  */
#line 2142 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8842 "bison-chpl-lib.cpp"
    break;

  case 324: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2148 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8850 "bison-chpl-lib.cpp"
    break;

  case 325: /* $@11: %empty  */
#line 2157 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name);
    }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 326: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@11 opt_function_body_stmt  */
#line 2163 "chpl.ypp"
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
#line 8881 "bison-chpl-lib.cpp"
    break;

  case 327: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2183 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8893 "bison-chpl-lib.cpp"
    break;

  case 328: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2191 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = (yyvsp[-1].uniqueStr);
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8905 "bison-chpl-lib.cpp"
    break;

  case 329: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2199 "chpl.ypp"
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
#line 8920 "bison-chpl-lib.cpp"
    break;

  case 330: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2210 "chpl.ypp"
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
#line 8935 "bison-chpl-lib.cpp"
    break;

  case 331: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2221 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8945 "bison-chpl-lib.cpp"
    break;

  case 332: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2230 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-1].functionParts);
      context->noteDeclStartLoc((yylsp[0]));
      // Preserve the first location, to avoid desync on start line number.
      auto loc = context->declStartLoc((yylsp[0]));
      fp.startLocFirstLine = loc.first_line;
      fp.startLocFirstColumn = loc.first_column;
      fp.comments = context->gatherComments(loc);
      fp.attributes = context->buildAttributes((yyloc)).release();
      fp.visibility = context->visibility;
      context->resetAttributePartsState();
      fp.kind = (yyvsp[0].functionKind);
      (yyval.functionParts) = fp;
    }
#line 8964 "bison-chpl-lib.cpp"
    break;

  case 334: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2248 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8970 "bison-chpl-lib.cpp"
    break;

  case 337: /* fn_ident: ident_def TBANG  */
#line 2255 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 378: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2313 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8986 "bison-chpl-lib.cpp"
    break;

  case 379: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2314 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8992 "bison-chpl-lib.cpp"
    break;

  case 380: /* formal_var_arg_expr: TDOTDOTDOT query_expr  */
#line 2315 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8998 "bison-chpl-lib.cpp"
    break;

  case 381: /* opt_formal_ls: %empty  */
#line 2319 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9004 "bison-chpl-lib.cpp"
    break;

  case 382: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2320 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9010 "bison-chpl-lib.cpp"
    break;

  case 383: /* req_formal_ls: TLP formal_ls TRP  */
#line 2324 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9016 "bison-chpl-lib.cpp"
    break;

  case 384: /* formal_ls_inner: formal  */
#line 2328 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9022 "bison-chpl-lib.cpp"
    break;

  case 385: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2329 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9028 "bison-chpl-lib.cpp"
    break;

  case 386: /* formal_ls: %empty  */
#line 2333 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9034 "bison-chpl-lib.cpp"
    break;

  case 387: /* formal_ls: formal_ls_inner  */
#line 2334 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9040 "bison-chpl-lib.cpp"
    break;

  case 388: /* formal: opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
#line 2339 "chpl.ypp"
  {
    (yyval.expr) = Formal::build(BUILDER, LOC((yyloc)), /*attributes*/ nullptr,
                       /*name*/ (yyvsp[-2].uniqueStr),
                       /*intent*/ (yyvsp[-3].intentTag),
                       toOwned((yyvsp[-1].expr)),
                       toOwned((yyvsp[0].expr))).release();
    context->noteIsBuildingFormal(false);
  }
#line 9053 "bison-chpl-lib.cpp"
    break;

  case 389: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type opt_init_expr  */
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
#line 9068 "bison-chpl-lib.cpp"
    break;

  case 390: /* formal: opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
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
#line 9082 "bison-chpl-lib.cpp"
    break;

  case 391: /* formal: pragma_ls opt_formal_intent_tag ident_def opt_formal_type formal_var_arg_expr  */
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
#line 9098 "bison-chpl-lib.cpp"
    break;

  case 392: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type opt_init_expr  */
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
#line 9113 "bison-chpl-lib.cpp"
    break;

  case 393: /* formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_formal_type formal_var_arg_expr  */
#line 2394 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "variable-length argument may not be grouped in a tuple");
  }
#line 9121 "bison-chpl-lib.cpp"
    break;

  case 394: /* opt_formal_intent_tag: %empty  */
#line 2400 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9130 "bison-chpl-lib.cpp"
    break;

  case 395: /* opt_formal_intent_tag: required_intent_tag  */
#line 2405 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9139 "bison-chpl-lib.cpp"
    break;

  case 396: /* required_intent_tag: TIN  */
#line 2412 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9145 "bison-chpl-lib.cpp"
    break;

  case 397: /* required_intent_tag: TINOUT  */
#line 2413 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9151 "bison-chpl-lib.cpp"
    break;

  case 398: /* required_intent_tag: TOUT  */
#line 2414 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9157 "bison-chpl-lib.cpp"
    break;

  case 399: /* required_intent_tag: TCONST TIN  */
#line 2415 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9163 "bison-chpl-lib.cpp"
    break;

  case 400: /* required_intent_tag: TCONST TREF  */
#line 2416 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9169 "bison-chpl-lib.cpp"
    break;

  case 401: /* required_intent_tag: TCONST  */
#line 2417 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9175 "bison-chpl-lib.cpp"
    break;

  case 402: /* required_intent_tag: TPARAM  */
#line 2418 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9181 "bison-chpl-lib.cpp"
    break;

  case 403: /* required_intent_tag: TREF  */
#line 2419 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9187 "bison-chpl-lib.cpp"
    break;

  case 404: /* required_intent_tag: TTYPE  */
#line 2420 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9193 "bison-chpl-lib.cpp"
    break;

  case 405: /* opt_this_intent_tag: %empty  */
#line 2424 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9199 "bison-chpl-lib.cpp"
    break;

  case 406: /* opt_this_intent_tag: TPARAM  */
#line 2425 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9205 "bison-chpl-lib.cpp"
    break;

  case 407: /* opt_this_intent_tag: TREF  */
#line 2426 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9211 "bison-chpl-lib.cpp"
    break;

  case 408: /* opt_this_intent_tag: TCONST TREF  */
#line 2427 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9217 "bison-chpl-lib.cpp"
    break;

  case 409: /* opt_this_intent_tag: TCONST  */
#line 2428 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9223 "bison-chpl-lib.cpp"
    break;

  case 410: /* opt_this_intent_tag: TTYPE  */
#line 2429 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9229 "bison-chpl-lib.cpp"
    break;

  case 411: /* proc_iter_or_op: TPROC  */
#line 2433 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9235 "bison-chpl-lib.cpp"
    break;

  case 412: /* proc_iter_or_op: TITER  */
#line 2434 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9241 "bison-chpl-lib.cpp"
    break;

  case 413: /* proc_iter_or_op: TOPERATOR  */
#line 2435 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9247 "bison-chpl-lib.cpp"
    break;

  case 414: /* opt_ret_tag: %empty  */
#line 2439 "chpl.ypp"
              { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9253 "bison-chpl-lib.cpp"
    break;

  case 415: /* opt_ret_tag: TCONST  */
#line 2440 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST; }
#line 9259 "bison-chpl-lib.cpp"
    break;

  case 416: /* opt_ret_tag: TCONST TREF  */
#line 2441 "chpl.ypp"
              { (yyval.returnTag) = Function::CONST_REF; }
#line 9265 "bison-chpl-lib.cpp"
    break;

  case 417: /* opt_ret_tag: TREF  */
#line 2442 "chpl.ypp"
              { (yyval.returnTag) = Function::REF; }
#line 9271 "bison-chpl-lib.cpp"
    break;

  case 418: /* opt_ret_tag: TPARAM  */
#line 2443 "chpl.ypp"
              { (yyval.returnTag) = Function::PARAM; }
#line 9277 "bison-chpl-lib.cpp"
    break;

  case 419: /* opt_ret_tag: TTYPE  */
#line 2444 "chpl.ypp"
              { (yyval.returnTag) = Function::TYPE; }
#line 9283 "bison-chpl-lib.cpp"
    break;

  case 420: /* opt_throws_error: %empty  */
#line 2448 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9289 "bison-chpl-lib.cpp"
    break;

  case 421: /* opt_throws_error: TTHROWS  */
#line 2449 "chpl.ypp"
          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9295 "bison-chpl-lib.cpp"
    break;

  case 422: /* opt_function_body_stmt: TSEMI  */
#line 2452 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9301 "bison-chpl-lib.cpp"
    break;

  case 423: /* opt_function_body_stmt: function_body_stmt  */
#line 2453 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9307 "bison-chpl-lib.cpp"
    break;

  case 424: /* function_body_stmt: block_stmt_body  */
#line 2457 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9313 "bison-chpl-lib.cpp"
    break;

  case 425: /* function_body_stmt: return_stmt  */
#line 2458 "chpl.ypp"
                    { context->clearComments(); (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 9319 "bison-chpl-lib.cpp"
    break;

  case 426: /* query_expr: TQUERIEDIDENT  */
#line 2462 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9325 "bison-chpl-lib.cpp"
    break;

  case 427: /* opt_lifetime_where: %empty  */
#line 2467 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9331 "bison-chpl-lib.cpp"
    break;

  case 428: /* opt_lifetime_where: TWHERE expr  */
#line 2469 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9337 "bison-chpl-lib.cpp"
    break;

  case 429: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2471 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9343 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2473 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9349 "bison-chpl-lib.cpp"
    break;

  case 431: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2475 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9355 "bison-chpl-lib.cpp"
    break;

  case 432: /* lifetime_components_expr: lifetime_expr  */
#line 2480 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9361 "bison-chpl-lib.cpp"
    break;

  case 433: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2482 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9367 "bison-chpl-lib.cpp"
    break;

  case 434: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2487 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9373 "bison-chpl-lib.cpp"
    break;

  case 435: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2489 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9379 "bison-chpl-lib.cpp"
    break;

  case 436: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2491 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9385 "bison-chpl-lib.cpp"
    break;

  case 437: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2493 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9391 "bison-chpl-lib.cpp"
    break;

  case 438: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2495 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9397 "bison-chpl-lib.cpp"
    break;

  case 439: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2497 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9403 "bison-chpl-lib.cpp"
    break;

  case 440: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2499 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9409 "bison-chpl-lib.cpp"
    break;

  case 441: /* lifetime_ident: TIDENT  */
#line 2503 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9415 "bison-chpl-lib.cpp"
    break;

  case 442: /* lifetime_ident: TTHIS  */
#line 2504 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9421 "bison-chpl-lib.cpp"
    break;

  case 443: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2509 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9430 "bison-chpl-lib.cpp"
    break;

  case 444: /* type_alias_decl_stmt_start: TTYPE  */
#line 2517 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9438 "bison-chpl-lib.cpp"
    break;

  case 445: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2521 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9447 "bison-chpl-lib.cpp"
    break;

  case 446: /* type_alias_decl_stmt_start: TEXTERN TTYPE  */
#line 2526 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteLinkage(Decl::EXTERN);
  }
#line 9456 "bison-chpl-lib.cpp"
    break;

  case 447: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2534 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9464 "bison-chpl-lib.cpp"
    break;

  case 448: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2538 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9472 "bison-chpl-lib.cpp"
    break;

  case 449: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
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
#line 9494 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_init_type: %empty  */
#line 2565 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9500 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_init_type: TASSIGN type_level_expr  */
#line 2567 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9506 "bison-chpl-lib.cpp"
    break;

  case 452: /* opt_init_type: TASSIGN array_type  */
#line 2569 "chpl.ypp"
  {
    // Cannot be a type_level_expr as expr inherits from type_level_expr.
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 9515 "bison-chpl-lib.cpp"
    break;

  case 453: /* var_decl_type: TPARAM  */
#line 2576 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9521 "bison-chpl-lib.cpp"
    break;

  case 454: /* var_decl_type: TCONST TREF  */
#line 2577 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9527 "bison-chpl-lib.cpp"
    break;

  case 455: /* var_decl_type: TREF  */
#line 2578 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9533 "bison-chpl-lib.cpp"
    break;

  case 456: /* var_decl_type: TCONST  */
#line 2579 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9539 "bison-chpl-lib.cpp"
    break;

  case 457: /* var_decl_type: TVAR  */
#line 2580 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9545 "bison-chpl-lib.cpp"
    break;

  case 458: /* $@12: %empty  */
#line 2585 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9554 "bison-chpl-lib.cpp"
    break;

  case 459: /* var_decl_stmt: TCONFIG $@12 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2589 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9563 "bison-chpl-lib.cpp"
    break;

  case 460: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2594 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 461: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2602 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9580 "bison-chpl-lib.cpp"
    break;

  case 462: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2606 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9588 "bison-chpl-lib.cpp"
    break;

  case 463: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
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
#line 9608 "bison-chpl-lib.cpp"
    break;

  case 464: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
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
#line 9626 "bison-chpl-lib.cpp"
    break;

  case 465: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2646 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9634 "bison-chpl-lib.cpp"
    break;

  case 466: /* tuple_var_decl_component: ident_def  */
#line 2650 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9642 "bison-chpl-lib.cpp"
    break;

  case 467: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2654 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9650 "bison-chpl-lib.cpp"
    break;

  case 468: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2661 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 469: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2663 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9662 "bison-chpl-lib.cpp"
    break;

  case 470: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2665 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9668 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_init_expr: %empty  */
#line 2671 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2672 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9680 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2673 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9686 "bison-chpl-lib.cpp"
    break;

  case 474: /* ret_array_type: TLSBR TRSBR type_level_expr  */
#line 2679 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9694 "bison-chpl-lib.cpp"
    break;

  case 475: /* ret_array_type: TLSBR TRSBR  */
#line 2683 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), nullptr, nullptr);
  }
#line 9702 "bison-chpl-lib.cpp"
    break;

  case 476: /* ret_array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2687 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 477: /* ret_array_type: TLSBR expr_ls TRSBR  */
#line 2691 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-1]), (yyvsp[-1].exprList), /*typeExpr*/ nullptr);
  }
#line 9718 "bison-chpl-lib.cpp"
    break;

  case 478: /* ret_array_type: TLSBR TRSBR ret_array_type  */
#line 2695 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9726 "bison-chpl-lib.cpp"
    break;

  case 479: /* ret_array_type: TLSBR expr_ls TRSBR ret_array_type  */
#line 2699 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9734 "bison-chpl-lib.cpp"
    break;

  case 480: /* ret_array_type: TLSBR error TRSBR  */
#line 2703 "chpl.ypp"
  {
    (yyval.expr) = ERROR((yyloc), "invalid expression for domain of array return type");
  }
#line 9742 "bison-chpl-lib.cpp"
    break;

  case 481: /* opt_ret_type: %empty  */
#line 2709 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9748 "bison-chpl-lib.cpp"
    break;

  case 482: /* opt_ret_type: TCOLON type_level_expr  */
#line 2710 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9754 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_ret_type: TCOLON ret_array_type  */
#line 2711 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9760 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_ret_type: TCOLON reserved_type_ident_use  */
#line 2712 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9766 "bison-chpl-lib.cpp"
    break;

  case 485: /* opt_ret_type: error  */
#line 2713 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9772 "bison-chpl-lib.cpp"
    break;

  case 486: /* opt_type: %empty  */
#line 2718 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9778 "bison-chpl-lib.cpp"
    break;

  case 487: /* opt_type: TCOLON type_level_expr  */
#line 2719 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9784 "bison-chpl-lib.cpp"
    break;

  case 488: /* opt_type: TCOLON array_type  */
#line 2720 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9790 "bison-chpl-lib.cpp"
    break;

  case 489: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2721 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9796 "bison-chpl-lib.cpp"
    break;

  case 490: /* opt_type: error  */
#line 2722 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9802 "bison-chpl-lib.cpp"
    break;

  case 491: /* array_type: TLSBR expr_ls TRSBR type_level_expr  */
#line 2743 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9810 "bison-chpl-lib.cpp"
    break;

  case 492: /* array_type: TLSBR expr_ls TRSBR array_type  */
#line 2747 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9818 "bison-chpl-lib.cpp"
    break;

  case 493: /* array_type: TLSBR expr_ls TIN expr TRSBR type_level_expr  */
#line 2751 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9826 "bison-chpl-lib.cpp"
    break;

  case 494: /* array_type: TLSBR error TRSBR  */
#line 2755 "chpl.ypp"
  {
    (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))).release();
  }
#line 9834 "bison-chpl-lib.cpp"
    break;

  case 495: /* opt_formal_array_elt_type: %empty  */
#line 2761 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9840 "bison-chpl-lib.cpp"
    break;

  case 496: /* opt_formal_array_elt_type: type_level_expr  */
#line 2762 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9846 "bison-chpl-lib.cpp"
    break;

  case 497: /* opt_formal_array_elt_type: query_expr  */
#line 2763 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9852 "bison-chpl-lib.cpp"
    break;

  case 498: /* formal_array_type: TLSBR TRSBR opt_formal_array_elt_type  */
#line 2768 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9861 "bison-chpl-lib.cpp"
    break;

  case 499: /* formal_array_type: TLSBR expr_ls TRSBR opt_formal_array_elt_type  */
#line 2773 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9869 "bison-chpl-lib.cpp"
    break;

  case 500: /* formal_array_type: TLSBR TRSBR formal_array_type  */
#line 2781 "chpl.ypp"
  {
    auto domainLoc = context->makeSpannedLocation((yylsp[-2]), (yylsp[-1]));
    (yyval.expr) = context->buildArrayType((yyloc), domainLoc, /*domainExprs*/ nullptr, (yyvsp[0].expr));
  }
#line 9878 "bison-chpl-lib.cpp"
    break;

  case 501: /* formal_array_type: TLSBR expr_ls TRSBR formal_array_type  */
#line 2786 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayType((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9886 "bison-chpl-lib.cpp"
    break;

  case 502: /* formal_array_type: TLSBR expr_ls TIN expr TRSBR opt_formal_array_elt_type  */
#line 2790 "chpl.ypp"
  {
    (yyval.expr) = context->buildArrayTypeWithIndex((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 9894 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_formal_type: %empty  */
#line 2796 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9900 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_formal_type: TCOLON type_level_expr  */
#line 2797 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9906 "bison-chpl-lib.cpp"
    break;

  case 505: /* opt_formal_type: TCOLON query_expr  */
#line 2798 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9912 "bison-chpl-lib.cpp"
    break;

  case 506: /* opt_formal_type: TCOLON reserved_type_ident_use  */
#line 2799 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9918 "bison-chpl-lib.cpp"
    break;

  case 507: /* opt_formal_type: TCOLON formal_array_type  */
#line 2800 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9924 "bison-chpl-lib.cpp"
    break;

  case 508: /* expr_ls: expr  */
#line 2806 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9930 "bison-chpl-lib.cpp"
    break;

  case 509: /* expr_ls: query_expr  */
#line 2807 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9936 "bison-chpl-lib.cpp"
    break;

  case 510: /* expr_ls: expr_ls TCOMMA expr  */
#line 2808 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9942 "bison-chpl-lib.cpp"
    break;

  case 511: /* expr_ls: expr_ls TCOMMA query_expr  */
#line 2809 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9948 "bison-chpl-lib.cpp"
    break;

  case 512: /* simple_expr_ls: expr  */
#line 2813 "chpl.ypp"
                                   { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9954 "bison-chpl-lib.cpp"
    break;

  case 513: /* simple_expr_ls: simple_expr_ls TCOMMA expr  */
#line 2814 "chpl.ypp"
                                   { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9960 "bison-chpl-lib.cpp"
    break;

  case 514: /* tuple_component: TUNDERSCORE  */
#line 2818 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9966 "bison-chpl-lib.cpp"
    break;

  case 515: /* tuple_component: opt_try_expr  */
#line 2819 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9972 "bison-chpl-lib.cpp"
    break;

  case 516: /* tuple_component: query_expr  */
#line 2820 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9978 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2825 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9986 "bison-chpl-lib.cpp"
    break;

  case 518: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2829 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9994 "bison-chpl-lib.cpp"
    break;

  case 519: /* opt_actual_ls: %empty  */
#line 2835 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10000 "bison-chpl-lib.cpp"
    break;

  case 520: /* opt_actual_ls: actual_ls  */
#line 2836 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10006 "bison-chpl-lib.cpp"
    break;

  case 521: /* actual_ls: actual_expr  */
#line 2841 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10015 "bison-chpl-lib.cpp"
    break;

  case 522: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2846 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10025 "bison-chpl-lib.cpp"
    break;

  case 523: /* actual_expr: ident_use TASSIGN query_expr  */
#line 2854 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10031 "bison-chpl-lib.cpp"
    break;

  case 524: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2855 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10037 "bison-chpl-lib.cpp"
    break;

  case 525: /* actual_expr: query_expr  */
#line 2856 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10043 "bison-chpl-lib.cpp"
    break;

  case 526: /* actual_expr: opt_try_expr  */
#line 2857 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10049 "bison-chpl-lib.cpp"
    break;

  case 527: /* ident_expr: ident_use  */
#line 2861 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10055 "bison-chpl-lib.cpp"
    break;

  case 528: /* ident_expr: scalar_type  */
#line 2862 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10061 "bison-chpl-lib.cpp"
    break;

  case 529: /* type_level_expr: sub_type_level_expr  */
#line 2874 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 10067 "bison-chpl-lib.cpp"
    break;

  case 530: /* type_level_expr: sub_type_level_expr TQUESTION  */
#line 2876 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10073 "bison-chpl-lib.cpp"
    break;

  case 531: /* type_level_expr: TQUESTION  */
#line 2878 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10079 "bison-chpl-lib.cpp"
    break;

  case 537: /* sub_type_level_expr: TSINGLE expr  */
#line 2889 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10085 "bison-chpl-lib.cpp"
    break;

  case 538: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 2891 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10091 "bison-chpl-lib.cpp"
    break;

  case 539: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 2893 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10097 "bison-chpl-lib.cpp"
    break;

  case 540: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 2895 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10103 "bison-chpl-lib.cpp"
    break;

  case 541: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 2897 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10113 "bison-chpl-lib.cpp"
    break;

  case 542: /* sub_type_level_expr: TATOMIC expr  */
#line 2903 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10119 "bison-chpl-lib.cpp"
    break;

  case 543: /* sub_type_level_expr: TSYNC expr  */
#line 2905 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10125 "bison-chpl-lib.cpp"
    break;

  case 544: /* sub_type_level_expr: TOWNED  */
#line 2908 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10131 "bison-chpl-lib.cpp"
    break;

  case 545: /* sub_type_level_expr: TOWNED expr  */
#line 2910 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10137 "bison-chpl-lib.cpp"
    break;

  case 546: /* sub_type_level_expr: TUNMANAGED  */
#line 2912 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10143 "bison-chpl-lib.cpp"
    break;

  case 547: /* sub_type_level_expr: TUNMANAGED expr  */
#line 2914 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10149 "bison-chpl-lib.cpp"
    break;

  case 548: /* sub_type_level_expr: TSHARED  */
#line 2916 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10155 "bison-chpl-lib.cpp"
    break;

  case 549: /* sub_type_level_expr: TSHARED expr  */
#line 2918 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10161 "bison-chpl-lib.cpp"
    break;

  case 550: /* sub_type_level_expr: TBORROWED  */
#line 2920 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10167 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TBORROWED expr  */
#line 2922 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10173 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TCLASS  */
#line 2925 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10179 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TRECORD  */
#line 2927 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10185 "bison-chpl-lib.cpp"
    break;

  case 554: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 2932 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10198 "bison-chpl-lib.cpp"
    break;

  case 555: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 2941 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10211 "bison-chpl-lib.cpp"
    break;

  case 556: /* for_expr: TFOR expr TDO expr  */
#line 2950 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[0].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false).release();
  }
#line 10223 "bison-chpl-lib.cpp"
    break;

  case 557: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
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
#line 10241 "bison-chpl-lib.cpp"
    break;

  case 558: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
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
#line 10259 "bison-chpl-lib.cpp"
    break;

  case 559: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
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
#line 10277 "bison-chpl-lib.cpp"
    break;

  case 560: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3000 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10290 "bison-chpl-lib.cpp"
    break;

  case 561: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3009 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10303 "bison-chpl-lib.cpp"
    break;

  case 562: /* for_expr: TFORALL expr TDO expr  */
#line 3018 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[0].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 10315 "bison-chpl-lib.cpp"
    break;

  case 563: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
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
#line 10333 "bison-chpl-lib.cpp"
    break;

  case 564: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
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
#line 10351 "bison-chpl-lib.cpp"
    break;

  case 565: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
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
#line 10369 "bison-chpl-lib.cpp"
    break;

  case 566: /* for_expr: TLSBR expr_ls TRSBR expr  */
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
#line 10390 "bison-chpl-lib.cpp"
    break;

  case 567: /* for_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3085 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10403 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3094 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), (yyvsp[-4].exprList));
    (yyval.expr) = BracketLoop::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                            /*withClause*/ nullptr,
                            BlockStyle::IMPLICIT,
                            context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                            /*isExpressionLevel*/ true).release();
  }
#line 10416 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
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
#line 10435 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
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
#line 10454 "bison-chpl-lib.cpp"
    break;

  case 571: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
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
#line 10468 "bison-chpl-lib.cpp"
    break;

  case 572: /* nil_expr: TNIL  */
#line 3153 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10474 "bison-chpl-lib.cpp"
    break;

  case 580: /* stmt_level_expr: io_expr TIO expr  */
#line 3169 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10480 "bison-chpl-lib.cpp"
    break;

  case 581: /* opt_task_intent_ls: %empty  */
#line 3173 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10486 "bison-chpl-lib.cpp"
    break;

  case 582: /* opt_task_intent_ls: task_intent_clause  */
#line 3174 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10492 "bison-chpl-lib.cpp"
    break;

  case 583: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3179 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10502 "bison-chpl-lib.cpp"
    break;

  case 584: /* task_intent_ls: intent_expr  */
#line 3187 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10508 "bison-chpl-lib.cpp"
    break;

  case 585: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3188 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10514 "bison-chpl-lib.cpp"
    break;

  case 586: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3193 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10524 "bison-chpl-lib.cpp"
    break;

  case 587: /* forall_intent_ls: intent_expr  */
#line 3201 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10530 "bison-chpl-lib.cpp"
    break;

  case 588: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3202 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10536 "bison-chpl-lib.cpp"
    break;

  case 589: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
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
#line 10555 "bison-chpl-lib.cpp"
    break;

  case 590: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3222 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 10563 "bison-chpl-lib.cpp"
    break;

  case 591: /* intent_expr: expr TREDUCE ident_expr  */
#line 3226 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10571 "bison-chpl-lib.cpp"
    break;

  case 592: /* task_var_prefix: TCONST  */
#line 3232 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10577 "bison-chpl-lib.cpp"
    break;

  case 593: /* task_var_prefix: TIN  */
#line 3233 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10583 "bison-chpl-lib.cpp"
    break;

  case 594: /* task_var_prefix: TCONST TIN  */
#line 3234 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10589 "bison-chpl-lib.cpp"
    break;

  case 595: /* task_var_prefix: TREF  */
#line 3235 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10595 "bison-chpl-lib.cpp"
    break;

  case 596: /* task_var_prefix: TCONST TREF  */
#line 3236 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10601 "bison-chpl-lib.cpp"
    break;

  case 597: /* task_var_prefix: TVAR  */
#line 3237 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10607 "bison-chpl-lib.cpp"
    break;

  case 599: /* io_expr: io_expr TIO expr  */
#line 3243 "chpl.ypp"
  { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10613 "bison-chpl-lib.cpp"
    break;

  case 600: /* new_maybe_decorated: TNEW  */
#line 3248 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10619 "bison-chpl-lib.cpp"
    break;

  case 601: /* new_maybe_decorated: TNEW TOWNED  */
#line 3250 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10625 "bison-chpl-lib.cpp"
    break;

  case 602: /* new_maybe_decorated: TNEW TSHARED  */
#line 3252 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10631 "bison-chpl-lib.cpp"
    break;

  case 603: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3254 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10637 "bison-chpl-lib.cpp"
    break;

  case 604: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3256 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10643 "bison-chpl-lib.cpp"
    break;

  case 605: /* new_expr: new_maybe_decorated expr  */
#line 3262 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10651 "bison-chpl-lib.cpp"
    break;

  case 606: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3269 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10659 "bison-chpl-lib.cpp"
    break;

  case 607: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3273 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10667 "bison-chpl-lib.cpp"
    break;

  case 608: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3277 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10675 "bison-chpl-lib.cpp"
    break;

  case 609: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3281 "chpl.ypp"
  {
    (yyval.expr) = TODOEXPR((yyloc));
  }
#line 10683 "bison-chpl-lib.cpp"
    break;

  case 610: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3288 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10691 "bison-chpl-lib.cpp"
    break;

  case 620: /* expr: TLP TDOTDOTDOT expr TRP  */
#line 3307 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10699 "bison-chpl-lib.cpp"
    break;

  case 621: /* expr: expr TCOLON expr  */
#line 3311 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10707 "bison-chpl-lib.cpp"
    break;

  case 622: /* expr: expr TDOTDOT expr  */
#line 3315 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10716 "bison-chpl-lib.cpp"
    break;

  case 623: /* expr: expr TDOTDOTOPENHIGH expr  */
#line 3320 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10725 "bison-chpl-lib.cpp"
    break;

  case 624: /* expr: expr TDOTDOT  */
#line 3325 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10734 "bison-chpl-lib.cpp"
    break;

  case 625: /* expr: TDOTDOT expr  */
#line 3330 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10744 "bison-chpl-lib.cpp"
    break;

  case 626: /* expr: TDOTDOTOPENHIGH expr  */
#line 3336 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10754 "bison-chpl-lib.cpp"
    break;

  case 627: /* expr: TDOTDOT  */
#line 3342 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10764 "bison-chpl-lib.cpp"
    break;

  case 628: /* opt_expr: %empty  */
#line 3373 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10770 "bison-chpl-lib.cpp"
    break;

  case 629: /* opt_expr: expr  */
#line 3374 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10776 "bison-chpl-lib.cpp"
    break;

  case 630: /* opt_try_expr: TTRY expr  */
#line 3378 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10782 "bison-chpl-lib.cpp"
    break;

  case 631: /* opt_try_expr: TTRYBANG expr  */
#line 3379 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10788 "bison-chpl-lib.cpp"
    break;

  case 632: /* opt_try_expr: expr  */
#line 3380 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10794 "bison-chpl-lib.cpp"
    break;

  case 638: /* call_base_expr: expr TBANG  */
#line 3397 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10802 "bison-chpl-lib.cpp"
    break;

  case 639: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3400 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10808 "bison-chpl-lib.cpp"
    break;

  case 642: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3407 "chpl.ypp"
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
#line 10824 "bison-chpl-lib.cpp"
    break;

  case 643: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3419 "chpl.ypp"
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
#line 10840 "bison-chpl-lib.cpp"
    break;

  case 644: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3431 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10848 "bison-chpl-lib.cpp"
    break;

  case 645: /* dot_expr: expr TDOT ident_use  */
#line 3438 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10854 "bison-chpl-lib.cpp"
    break;

  case 646: /* dot_expr: expr TDOT TTYPE  */
#line 3440 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10860 "bison-chpl-lib.cpp"
    break;

  case 647: /* dot_expr: expr TDOT TDOMAIN  */
#line 3442 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10866 "bison-chpl-lib.cpp"
    break;

  case 648: /* dot_expr: expr TDOT TLOCALE  */
#line 3444 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10872 "bison-chpl-lib.cpp"
    break;

  case 649: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3446 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10882 "bison-chpl-lib.cpp"
    break;

  case 650: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3452 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10892 "bison-chpl-lib.cpp"
    break;

  case 651: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3464 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10898 "bison-chpl-lib.cpp"
    break;

  case 652: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3466 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10906 "bison-chpl-lib.cpp"
    break;

  case 653: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3470 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10914 "bison-chpl-lib.cpp"
    break;

  case 654: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3474 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10922 "bison-chpl-lib.cpp"
    break;

  case 655: /* bool_literal: TFALSE  */
#line 3480 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10928 "bison-chpl-lib.cpp"
    break;

  case 656: /* bool_literal: TTRUE  */
#line 3481 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10934 "bison-chpl-lib.cpp"
    break;

  case 657: /* str_bytes_literal: STRINGLITERAL  */
#line 3485 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10940 "bison-chpl-lib.cpp"
    break;

  case 658: /* str_bytes_literal: BYTESLITERAL  */
#line 3486 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10946 "bison-chpl-lib.cpp"
    break;

  case 661: /* literal_expr: INTLITERAL  */
#line 3492 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10952 "bison-chpl-lib.cpp"
    break;

  case 662: /* literal_expr: REALLITERAL  */
#line 3493 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10958 "bison-chpl-lib.cpp"
    break;

  case 663: /* literal_expr: IMAGLITERAL  */
#line 3494 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10964 "bison-chpl-lib.cpp"
    break;

  case 664: /* literal_expr: CSTRINGLITERAL  */
#line 3495 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10970 "bison-chpl-lib.cpp"
    break;

  case 665: /* literal_expr: TNONE  */
#line 3496 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10976 "bison-chpl-lib.cpp"
    break;

  case 666: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3498 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10984 "bison-chpl-lib.cpp"
    break;

  case 667: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3502 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10992 "bison-chpl-lib.cpp"
    break;

  case 668: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3506 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11000 "bison-chpl-lib.cpp"
    break;

  case 669: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3510 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11008 "bison-chpl-lib.cpp"
    break;

  case 670: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3514 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11016 "bison-chpl-lib.cpp"
    break;

  case 671: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3518 "chpl.ypp"
  {
    // TODO (dlongnecke): Record trailing comma?
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11025 "bison-chpl-lib.cpp"
    break;

  case 672: /* assoc_expr_ls: expr TALIAS expr  */
#line 3527 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11034 "bison-chpl-lib.cpp"
    break;

  case 673: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3532 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11044 "bison-chpl-lib.cpp"
    break;

  case 674: /* binary_op_expr: expr TPLUS expr  */
#line 3540 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11050 "bison-chpl-lib.cpp"
    break;

  case 675: /* binary_op_expr: expr TMINUS expr  */
#line 3541 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11056 "bison-chpl-lib.cpp"
    break;

  case 676: /* binary_op_expr: expr TSTAR expr  */
#line 3542 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11062 "bison-chpl-lib.cpp"
    break;

  case 677: /* binary_op_expr: expr TDIVIDE expr  */
#line 3543 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11068 "bison-chpl-lib.cpp"
    break;

  case 678: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3544 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11074 "bison-chpl-lib.cpp"
    break;

  case 679: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3545 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11080 "bison-chpl-lib.cpp"
    break;

  case 680: /* binary_op_expr: expr TMOD expr  */
#line 3546 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11086 "bison-chpl-lib.cpp"
    break;

  case 681: /* binary_op_expr: expr TEQUAL expr  */
#line 3547 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11092 "bison-chpl-lib.cpp"
    break;

  case 682: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3548 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11098 "bison-chpl-lib.cpp"
    break;

  case 683: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3549 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11104 "bison-chpl-lib.cpp"
    break;

  case 684: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3550 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11110 "bison-chpl-lib.cpp"
    break;

  case 685: /* binary_op_expr: expr TLESS expr  */
#line 3551 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11116 "bison-chpl-lib.cpp"
    break;

  case 686: /* binary_op_expr: expr TGREATER expr  */
#line 3552 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11122 "bison-chpl-lib.cpp"
    break;

  case 687: /* binary_op_expr: expr TBAND expr  */
#line 3553 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11128 "bison-chpl-lib.cpp"
    break;

  case 688: /* binary_op_expr: expr TBOR expr  */
#line 3554 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11134 "bison-chpl-lib.cpp"
    break;

  case 689: /* binary_op_expr: expr TBXOR expr  */
#line 3555 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11140 "bison-chpl-lib.cpp"
    break;

  case 690: /* binary_op_expr: expr TAND expr  */
#line 3556 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11146 "bison-chpl-lib.cpp"
    break;

  case 691: /* binary_op_expr: expr TOR expr  */
#line 3557 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11152 "bison-chpl-lib.cpp"
    break;

  case 692: /* binary_op_expr: expr TEXP expr  */
#line 3558 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11158 "bison-chpl-lib.cpp"
    break;

  case 693: /* binary_op_expr: expr TBY expr  */
#line 3559 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11164 "bison-chpl-lib.cpp"
    break;

  case 694: /* binary_op_expr: expr TALIGN expr  */
#line 3560 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11170 "bison-chpl-lib.cpp"
    break;

  case 695: /* binary_op_expr: expr THASH expr  */
#line 3561 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11176 "bison-chpl-lib.cpp"
    break;

  case 696: /* binary_op_expr: expr TDMAPPED expr  */
#line 3562 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11182 "bison-chpl-lib.cpp"
    break;

  case 697: /* unary_op_expr: TPLUS expr  */
#line 3566 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11188 "bison-chpl-lib.cpp"
    break;

  case 698: /* unary_op_expr: TMINUS expr  */
#line 3567 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11194 "bison-chpl-lib.cpp"
    break;

  case 699: /* unary_op_expr: TMINUSMINUS expr  */
#line 3568 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 11200 "bison-chpl-lib.cpp"
    break;

  case 700: /* unary_op_expr: TPLUSPLUS expr  */
#line 3569 "chpl.ypp"
                                 { (yyval.expr) = TODOEXPR((yyloc)); /* warn */ }
#line 11206 "bison-chpl-lib.cpp"
    break;

  case 701: /* unary_op_expr: TBANG expr  */
#line 3570 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11212 "bison-chpl-lib.cpp"
    break;

  case 702: /* unary_op_expr: expr TBANG  */
#line 3571 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11220 "bison-chpl-lib.cpp"
    break;

  case 703: /* unary_op_expr: TBNOT expr  */
#line 3574 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11226 "bison-chpl-lib.cpp"
    break;

  case 704: /* reduce_expr: expr TREDUCE expr  */
#line 3579 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11234 "bison-chpl-lib.cpp"
    break;

  case 705: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3583 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11242 "bison-chpl-lib.cpp"
    break;

  case 706: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3587 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11250 "bison-chpl-lib.cpp"
    break;

  case 707: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3591 "chpl.ypp"
  {
    (yyval.expr) = Reduce::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11258 "bison-chpl-lib.cpp"
    break;

  case 708: /* scan_expr: expr TSCAN expr  */
#line 3598 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11266 "bison-chpl-lib.cpp"
    break;

  case 709: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3602 "chpl.ypp"
  {
    (yyval.expr) = context->buildCustomScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11274 "bison-chpl-lib.cpp"
    break;

  case 710: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3606 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11282 "bison-chpl-lib.cpp"
    break;

  case 711: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3610 "chpl.ypp"
  {
    (yyval.expr) = Scan::build(BUILDER, LOC((yyloc)), (yyvsp[-2].uniqueStr), toOwned((yyvsp[0].expr))).release();
  }
#line 11290 "bison-chpl-lib.cpp"
    break;


#line 11294 "bison-chpl-lib.cpp"

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
