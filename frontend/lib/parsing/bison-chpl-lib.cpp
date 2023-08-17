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
  YYSYMBOL_TDMAPPED = 31,                  /* TDMAPPED  */
  YYSYMBOL_TDO = 32,                       /* TDO  */
  YYSYMBOL_TDOMAIN = 33,                   /* TDOMAIN  */
  YYSYMBOL_TELSE = 34,                     /* TELSE  */
  YYSYMBOL_TENUM = 35,                     /* TENUM  */
  YYSYMBOL_TEXCEPT = 36,                   /* TEXCEPT  */
  YYSYMBOL_TEXPORT = 37,                   /* TEXPORT  */
  YYSYMBOL_TEXTERN = 38,                   /* TEXTERN  */
  YYSYMBOL_TFALSE = 39,                    /* TFALSE  */
  YYSYMBOL_TFOR = 40,                      /* TFOR  */
  YYSYMBOL_TFORALL = 41,                   /* TFORALL  */
  YYSYMBOL_TFOREACH = 42,                  /* TFOREACH  */
  YYSYMBOL_TFORWARDING = 43,               /* TFORWARDING  */
  YYSYMBOL_TIF = 44,                       /* TIF  */
  YYSYMBOL_TIMAG = 45,                     /* TIMAG  */
  YYSYMBOL_TIMPORT = 46,                   /* TIMPORT  */
  YYSYMBOL_TIN = 47,                       /* TIN  */
  YYSYMBOL_TINCLUDE = 48,                  /* TINCLUDE  */
  YYSYMBOL_TINDEX = 49,                    /* TINDEX  */
  YYSYMBOL_TINLINE = 50,                   /* TINLINE  */
  YYSYMBOL_TINOUT = 51,                    /* TINOUT  */
  YYSYMBOL_TINT = 52,                      /* TINT  */
  YYSYMBOL_TITER = 53,                     /* TITER  */
  YYSYMBOL_TINITEQUALS = 54,               /* TINITEQUALS  */
  YYSYMBOL_TIMPLEMENTS = 55,               /* TIMPLEMENTS  */
  YYSYMBOL_TINTERFACE = 56,                /* TINTERFACE  */
  YYSYMBOL_TLABEL = 57,                    /* TLABEL  */
  YYSYMBOL_TLAMBDA = 58,                   /* TLAMBDA  */
  YYSYMBOL_TLET = 59,                      /* TLET  */
  YYSYMBOL_TLIFETIME = 60,                 /* TLIFETIME  */
  YYSYMBOL_TLOCAL = 61,                    /* TLOCAL  */
  YYSYMBOL_TLOCALE = 62,                   /* TLOCALE  */
  YYSYMBOL_TMANAGE = 63,                   /* TMANAGE  */
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
  YYSYMBOL_TPROCLP = 86,                   /* TPROCLP  */
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
  YYSYMBOL_TATMARK = 140,                  /* TATMARK  */
  YYSYMBOL_TBANG = 141,                    /* TBANG  */
  YYSYMBOL_TBAND = 142,                    /* TBAND  */
  YYSYMBOL_TBNOT = 143,                    /* TBNOT  */
  YYSYMBOL_TBOR = 144,                     /* TBOR  */
  YYSYMBOL_TBXOR = 145,                    /* TBXOR  */
  YYSYMBOL_TCOLON = 146,                   /* TCOLON  */
  YYSYMBOL_TCOMMA = 147,                   /* TCOMMA  */
  YYSYMBOL_TDIVIDE = 148,                  /* TDIVIDE  */
  YYSYMBOL_TDOT = 149,                     /* TDOT  */
  YYSYMBOL_TDOTDOT = 150,                  /* TDOTDOT  */
  YYSYMBOL_TDOTDOTDOT = 151,               /* TDOTDOTDOT  */
  YYSYMBOL_TEQUAL = 152,                   /* TEQUAL  */
  YYSYMBOL_TEXP = 153,                     /* TEXP  */
  YYSYMBOL_TGREATER = 154,                 /* TGREATER  */
  YYSYMBOL_TGREATEREQUAL = 155,            /* TGREATEREQUAL  */
  YYSYMBOL_THASH = 156,                    /* THASH  */
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
  YYSYMBOL_stmt_base = 187,                /* stmt_base  */
  YYSYMBOL_tryable_stmt = 188,             /* tryable_stmt  */
  YYSYMBOL_decl_base = 189,                /* decl_base  */
  YYSYMBOL_collect_attributes = 190,       /* collect_attributes  */
  YYSYMBOL_attribute_receiver = 191,       /* attribute_receiver  */
  YYSYMBOL_attribute_group = 192,          /* attribute_group  */
  YYSYMBOL_attribute_decl_stmt_ls = 193,   /* attribute_decl_stmt_ls  */
  YYSYMBOL_attribute_decl_stmt = 194,      /* attribute_decl_stmt  */
  YYSYMBOL_attribute_decl_begin = 195,     /* attribute_decl_begin  */
  YYSYMBOL_opt_attribute_decl_toolspace = 196, /* opt_attribute_decl_toolspace  */
  YYSYMBOL_opt_attribute_actuals = 197,    /* opt_attribute_actuals  */
  YYSYMBOL_attribute_actuals_ls = 198,     /* attribute_actuals_ls  */
  YYSYMBOL_attribute_actual = 199,         /* attribute_actual  */
  YYSYMBOL_module_decl_start = 200,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 201,         /* module_decl_stmt  */
  YYSYMBOL_opt_access_control = 202,       /* opt_access_control  */
  YYSYMBOL_opt_prototype = 203,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 204,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 205,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 206,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 207,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 208,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 209,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 210,              /* opt_only_ls  */
  YYSYMBOL_use_stmt = 211,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 212,              /* import_stmt  */
  YYSYMBOL_import_internal_type_ident = 213, /* import_internal_type_ident  */
  YYSYMBOL_import_expr = 214,              /* import_expr  */
  YYSYMBOL_import_ls = 215,                /* import_ls  */
  YYSYMBOL_require_stmt = 216,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 217,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 218,          /* opt_label_ident  */
  YYSYMBOL_reserved_word_ident = 219,      /* reserved_word_ident  */
  YYSYMBOL_ident_def = 220,                /* ident_def  */
  YYSYMBOL_ident_use = 221,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 222,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 223,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 224,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 225,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 226,              /* return_stmt  */
  YYSYMBOL_class_level_stmt = 227,         /* class_level_stmt  */
  YYSYMBOL_228_1 = 228,                    /* $@1  */
  YYSYMBOL_229_2 = 229,                    /* $@2  */
  YYSYMBOL_inner_class_level_stmt = 230,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 231,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 232,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 233,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 234, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 235,  /* extern_export_decl_stmt  */
  YYSYMBOL_236_3 = 236,                    /* $@3  */
  YYSYMBOL_237_4 = 237,                    /* $@4  */
  YYSYMBOL_extern_block_stmt = 238,        /* extern_block_stmt  */
  YYSYMBOL_no_loop_attributes = 239,       /* no_loop_attributes  */
  YYSYMBOL_loop_stmt = 240,                /* loop_stmt  */
  YYSYMBOL_loop_stmt_base = 241,           /* loop_stmt_base  */
  YYSYMBOL_zippered_iterator = 242,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 243,                  /* if_stmt  */
  YYSYMBOL_ifvar = 244,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 245,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 246,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 247,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 248,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 249, /* implements_type_error_ident  */
  YYSYMBOL_interface_expr_ident = 250,     /* interface_expr_ident  */
  YYSYMBOL_interface_expr_call = 251,      /* interface_expr_call  */
  YYSYMBOL_interface_expr = 252,           /* interface_expr  */
  YYSYMBOL_interface_ident_ls = 253,       /* interface_ident_ls  */
  YYSYMBOL_unary_implements_stmt_idents = 254, /* unary_implements_stmt_idents  */
  YYSYMBOL_unary_implements_stmt_call = 255, /* unary_implements_stmt_call  */
  YYSYMBOL_binary_implements_stmt = 256,   /* binary_implements_stmt  */
  YYSYMBOL_ifc_constraint = 257,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 258,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 259,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 260,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 261,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 262,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 263,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 264,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 265,                /* when_stmt  */
  YYSYMBOL_manager_expr = 266,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 267,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 268,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 269,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 270,              /* class_start  */
  YYSYMBOL_class_tag = 271,                /* class_tag  */
  YYSYMBOL_opt_inherit = 272,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 273,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 274,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 275,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 276,                  /* enum_ls  */
  YYSYMBOL_277_5 = 277,                    /* $@5  */
  YYSYMBOL_278_6 = 278,                    /* $@6  */
  YYSYMBOL_enum_item = 279,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 280,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 281,         /* lambda_decl_expr  */
  YYSYMBOL_282_7 = 282,                    /* $@7  */
  YYSYMBOL_linkage_spec_empty = 283,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 284,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 285,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 286,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 287,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 288,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 289,                  /* fn_type  */
  YYSYMBOL_fn_expr = 290,                  /* fn_expr  */
  YYSYMBOL_291_8 = 291,                    /* $@8  */
  YYSYMBOL_292_9 = 292,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_complete = 293,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 294,             /* fn_decl_stmt  */
  YYSYMBOL_295_10 = 295,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 296,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 297,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 298,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 299,                 /* fn_ident  */
  YYSYMBOL_op_ident = 300,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 301,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 302,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 303,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 304,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 305,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 306,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 307,                /* formal_ls  */
  YYSYMBOL_formal = 308,                   /* formal  */
  YYSYMBOL_formal_ident_def = 309,         /* formal_ident_def  */
  YYSYMBOL_named_formal = 310,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 311,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 312,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 313,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 314,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 315,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 316,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 317,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 318,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 319,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 320,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 321, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 322,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 323,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 324,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 325, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 326, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 327, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 328,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 329,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 330,            /* var_decl_stmt  */
  YYSYMBOL_331_11 = 331,                   /* $@11  */
  YYSYMBOL_var_decl_stmt_inner_ls = 332,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 333,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 334, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 335, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 336,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 337,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 338,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 339,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 340,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 341,                 /* opt_type  */
  YYSYMBOL_formal_type = 342,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 343,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 344,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 345,                  /* expr_ls  */
  YYSYMBOL_tuple_component = 346,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 347,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 348,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 349,                /* actual_ls  */
  YYSYMBOL_actual_expr = 350,              /* actual_expr  */
  YYSYMBOL_ident_expr = 351,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 352,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 353,                 /* for_expr  */
  YYSYMBOL_for_expr_base = 354,            /* for_expr_base  */
  YYSYMBOL_bracket_loop_expr = 355,        /* bracket_loop_expr  */
  YYSYMBOL_bracket_loop_expr_base = 356,   /* bracket_loop_expr_base  */
  YYSYMBOL_cond_expr = 357,                /* cond_expr  */
  YYSYMBOL_nil_expr = 358,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 359,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 360,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 361,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 362,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 363,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 364,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 365,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 366,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 367,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 368,                 /* new_expr  */
  YYSYMBOL_let_expr = 369,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 370,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 371,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 372,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 373,               /* super_expr  */
  YYSYMBOL_expr = 374,                     /* expr  */
  YYSYMBOL_opt_expr = 375,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 376,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 377,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 378,           /* call_base_expr  */
  YYSYMBOL_call_expr = 379,                /* call_expr  */
  YYSYMBOL_dot_expr = 380,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 381,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 382,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 383,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 384,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 385,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 386,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 387,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 388,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 389,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 390       /* reduce_scan_op_expr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 328 "chpl.ypp"

  // include the definition of methods for ParserContext
  #include "ParserContextImpl.h"
  // include helper macros for use in writing productions
  #include "parser-help.h"
  // include implementation of error logging hook
  #include "parser-yyerror.h"

#line 514 "bison-chpl-lib.cpp"

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
#define YYLAST   20920

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  210
/* YYNRULES -- Number of rules.  */
#define YYNRULES  754
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1332

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
       0,   639,   639,   643,   644,   649,   650,   658,   662,   672,
     677,   684,   685,   686,   687,   688,   689,   690,   691,   693,
     694,   695,   696,   709,   710,   711,   720,   729,   733,   740,
     748,   749,   750,   751,   752,   753,   754,   755,   769,   784,
     792,   804,   815,   827,   839,   850,   866,   867,   871,   881,
     882,   886,   890,   891,   895,   899,   906,   918,   926,   929,
     939,   941,   948,   953,   962,   966,   986,  1001,  1016,  1032,
    1033,  1035,  1040,  1041,  1046,  1064,  1068,  1072,  1083,  1102,
    1103,  1107,  1111,  1115,  1120,  1124,  1128,  1138,  1143,  1149,
    1156,  1161,  1168,  1179,  1180,  1184,  1189,  1197,  1206,  1216,
    1224,  1233,  1246,  1254,  1255,  1256,  1257,  1258,  1259,  1260,
    1261,  1262,  1263,  1264,  1265,  1266,  1267,  1268,  1269,  1270,
    1282,  1286,  1291,  1296,  1302,  1311,  1312,  1316,  1325,  1329,
    1333,  1337,  1341,  1345,  1354,  1355,  1359,  1360,  1361,  1362,
    1363,  1367,  1368,  1380,  1381,  1386,  1387,  1388,  1389,  1390,
    1391,  1392,  1393,  1394,  1395,  1396,  1397,  1398,  1399,  1400,
    1401,  1402,  1403,  1404,  1408,  1409,  1410,  1411,  1412,  1413,
    1414,  1415,  1416,  1417,  1418,  1419,  1426,  1427,  1428,  1429,
    1433,  1434,  1438,  1445,  1455,  1459,  1465,  1465,  1471,  1471,
    1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,  1491,  1496,
    1501,  1506,  1513,  1521,  1522,  1526,  1535,  1541,  1549,  1568,
    1567,  1577,  1576,  1589,  1595,  1598,  1599,  1603,  1622,  1636,
    1650,  1654,  1658,  1662,  1666,  1670,  1674,  1678,  1698,  1702,
    1706,  1710,  1714,  1718,  1722,  1726,  1730,  1734,  1738,  1742,
    1746,  1750,  1754,  1758,  1762,  1766,  1770,  1774,  1778,  1782,
    1786,  1791,  1799,  1808,  1812,  1816,  1820,  1824,  1828,  1832,
    1836,  1840,  1845,  1850,  1855,  1863,  1878,  1896,  1900,  1907,
    1908,  1913,  1918,  1919,  1920,  1921,  1922,  1923,  1924,  1925,
    1926,  1927,  1928,  1929,  1930,  1939,  1940,  1941,  1942,  1951,
    1952,  1956,  1963,  1970,  1971,  1975,  1979,  1986,  1993,  2001,
    2008,  2012,  2016,  2023,  2027,  2031,  2035,  2042,  2043,  2047,
    2051,  2055,  2062,  2075,  2091,  2099,  2103,  2112,  2113,  2117,
    2121,  2126,  2135,  2140,  2144,  2151,  2152,  2156,  2165,  2170,
    2181,  2188,  2189,  2190,  2194,  2195,  2199,  2203,  2207,  2211,
    2215,  2222,  2239,  2252,  2259,  2264,  2271,  2270,  2280,  2286,
    2285,  2300,  2307,  2319,  2337,  2334,  2362,  2366,  2367,  2369,
    2374,  2375,  2379,  2380,  2384,  2387,  2389,  2394,  2395,  2406,
    2426,  2425,  2441,  2440,  2458,  2468,  2465,  2499,  2507,  2515,
    2526,  2537,  2546,  2561,  2562,  2566,  2567,  2568,  2577,  2578,
    2579,  2580,  2581,  2582,  2583,  2584,  2585,  2586,  2587,  2588,
    2589,  2590,  2591,  2592,  2593,  2594,  2595,  2596,  2597,  2598,
    2599,  2600,  2601,  2605,  2606,  2607,  2608,  2609,  2610,  2611,
    2612,  2613,  2614,  2615,  2616,  2621,  2622,  2626,  2627,  2631,
    2632,  2636,  2637,  2641,  2642,  2646,  2647,  2651,  2655,  2656,
    2660,  2664,  2669,  2674,  2679,  2684,  2692,  2696,  2704,  2705,
    2706,  2707,  2708,  2709,  2710,  2711,  2712,  2716,  2717,  2718,
    2719,  2720,  2721,  2725,  2726,  2727,  2731,  2732,  2733,  2734,
    2735,  2736,  2737,  2741,  2742,  2745,  2746,  2750,  2751,  2752,
    2756,  2761,  2762,  2764,  2766,  2768,  2773,  2775,  2780,  2782,
    2784,  2786,  2788,  2790,  2792,  2797,  2798,  2802,  2811,  2815,
    2823,  2827,  2834,  2855,  2856,  2861,  2862,  2863,  2864,  2865,
    2870,  2869,  2878,  2886,  2890,  2897,  2915,  2934,  2938,  2942,
    2949,  2951,  2953,  2960,  2961,  2962,  2966,  2970,  2971,  2972,
    2976,  2977,  2981,  2982,  2986,  2987,  2988,  2989,  2993,  2994,
    2998,  3002,  3003,  3009,  3010,  3014,  3015,  3019,  3023,  3030,
    3031,  3035,  3040,  3049,  3050,  3054,  3055,  3062,  3063,  3064,
    3065,  3066,  3067,  3069,  3071,  3073,  3075,  3081,  3083,  3086,
    3088,  3090,  3092,  3094,  3096,  3098,  3100,  3102,  3104,  3109,
    3113,  3123,  3133,  3142,  3157,  3172,  3187,  3197,  3207,  3216,
    3231,  3246,  3264,  3268,  3272,  3276,  3280,  3284,  3288,  3292,
    3299,  3317,  3325,  3326,  3327,  3328,  3329,  3330,  3331,  3335,
    3336,  3340,  3349,  3350,  3354,  3363,  3364,  3368,  3382,  3386,
    3393,  3394,  3395,  3396,  3397,  3398,  3402,  3404,  3406,  3408,
    3410,  3416,  3423,  3435,  3447,  3460,  3477,  3484,  3489,  3494,
    3499,  3505,  3511,  3541,  3548,  3555,  3556,  3560,  3562,  3563,
    3565,  3567,  3568,  3569,  3570,  3573,  3574,  3575,  3576,  3577,
    3578,  3579,  3580,  3581,  3585,  3586,  3590,  3591,  3592,  3596,
    3597,  3598,  3599,  3608,  3609,  3612,  3613,  3614,  3618,  3630,
    3642,  3649,  3651,  3653,  3655,  3657,  3663,  3676,  3677,  3681,
    3685,  3692,  3693,  3697,  3698,  3702,  3703,  3704,  3705,  3706,
    3707,  3708,  3709,  3714,  3719,  3724,  3729,  3734,  3743,  3748,
    3757,  3758,  3759,  3760,  3761,  3762,  3763,  3764,  3765,  3766,
    3767,  3768,  3769,  3770,  3771,  3772,  3773,  3774,  3775,  3776,
    3777,  3778,  3779,  3783,  3784,  3785,  3786,  3787,  3788,  3791,
    3795,  3799,  3803,  3807,  3814,  3818,  3822,  3826,  3834,  3835,
    3836,  3837,  3838,  3839,  3840
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
  "TCONFIG", "TCONST", "TCONTINUE", "TDEFER", "TDELETE", "TDMAPPED", "TDO",
  "TDOMAIN", "TELSE", "TENUM", "TEXCEPT", "TEXPORT", "TEXTERN", "TFALSE",
  "TFOR", "TFORALL", "TFOREACH", "TFORWARDING", "TIF", "TIMAG", "TIMPORT",
  "TIN", "TINCLUDE", "TINDEX", "TINLINE", "TINOUT", "TINT", "TITER",
  "TINITEQUALS", "TIMPLEMENTS", "TINTERFACE", "TLABEL", "TLAMBDA", "TLET",
  "TLIFETIME", "TLOCAL", "TLOCALE", "TMANAGE", "TMINUSMINUS", "TMODULE",
  "TNEW", "TNIL", "TNOINIT", "TNONE", "TNOTHING", "TON", "TONLY",
  "TOPERATOR", "TOTHERWISE", "TOUT", "TOVERRIDE", "TOWNED", "TPARAM",
  "TPLUSPLUS", "TPRAGMA", "TPRIMITIVE", "TPRIVATE", "TPROC", "TPROTOTYPE",
  "TPUBLIC", "TPROCLP", "TREAL", "TRECORD", "TREDUCE", "TREF", "TREQUIRE",
  "TRETURN", "TSCAN", "TSELECT", "TSERIAL", "TSHARED", "TSINGLE",
  "TSPARSE", "TSTRING", "TSUBDOMAIN", "TSYNC", "TTHEN", "TTHIS", "TTHROW",
  "TTHROWS", "TTRUE", "TTRY", "TTRYBANG", "TTYPE", "TUINT", "TUNDERSCORE",
  "TUNION", "TUNMANAGED", "TUSE", "TVAR", "TVOID", "TWHEN", "TWHERE",
  "TWHILE", "TWITH", "TYIELD", "TZIP", "TALIAS", "TAND", "TASSIGN",
  "TASSIGNBAND", "TASSIGNBOR", "TASSIGNBXOR", "TASSIGNDIVIDE",
  "TASSIGNEXP", "TASSIGNLAND", "TASSIGNLOR", "TASSIGNMINUS", "TASSIGNMOD",
  "TASSIGNMULTIPLY", "TASSIGNPLUS", "TASSIGNREDUCE", "TASSIGNSL",
  "TASSIGNSR", "TATMARK", "TBANG", "TBAND", "TBNOT", "TBOR", "TBXOR",
  "TCOLON", "TCOMMA", "TDIVIDE", "TDOT", "TDOTDOT", "TDOTDOTDOT", "TEQUAL",
  "TEXP", "TGREATER", "TGREATEREQUAL", "THASH", "TLESS", "TLESSEQUAL",
  "TMINUS", "TMOD", "TNOTEQUAL", "TOR", "TPLUS", "TQUESTION", "TSEMI",
  "TSHIFTLEFT", "TSHIFTRIGHT", "TSTAR", "TSWAP", "TLCBR", "TRCBR", "TLP",
  "TRP", "TLSBR", "TRSBR", "TNOELSE", "TDOTDOTOPENHIGH", "TUPLUS",
  "TUMINUS", "TLNOT", "$accept", "program", "toplevel_stmt_ls",
  "toplevel_stmt", "pragma_ls", "stmt", "stmt_base", "tryable_stmt",
  "decl_base", "collect_attributes", "attribute_receiver",
  "attribute_group", "attribute_decl_stmt_ls", "attribute_decl_stmt",
  "attribute_decl_begin", "opt_attribute_decl_toolspace",
  "opt_attribute_actuals", "attribute_actuals_ls", "attribute_actual",
  "module_decl_start", "module_decl_stmt", "opt_access_control",
  "opt_prototype", "include_module_stmt", "block_stmt_body", "block_stmt",
  "stmt_ls", "renames_ls", "use_renames_ls", "opt_only_ls", "use_stmt",
  "import_stmt", "import_internal_type_ident", "import_expr", "import_ls",
  "require_stmt", "assignment_stmt", "opt_label_ident",
  "reserved_word_ident", "ident_def", "ident_use",
  "internal_type_ident_def", "scalar_type", "reserved_type_ident_use",
  "do_stmt", "return_stmt", "class_level_stmt", "$@1", "$@2",
  "inner_class_level_stmt", "forwarding_decl_stmt",
  "forwarding_decl_start", "extern_or_export",
  "extern_export_decl_stmt_start", "extern_export_decl_stmt", "$@3", "$@4",
  "extern_block_stmt", "no_loop_attributes", "loop_stmt", "loop_stmt_base",
  "zippered_iterator", "if_stmt", "ifvar", "interface_stmt",
  "ifc_formal_ls", "ifc_formal", "implements_type_ident",
  "implements_type_error_ident", "interface_expr_ident",
  "interface_expr_call", "interface_expr", "interface_ident_ls",
  "unary_implements_stmt_idents", "unary_implements_stmt_call",
  "binary_implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
  "catch_expr", "catch_expr_inner", "throw_stmt", "select_stmt",
  "when_stmt_ls", "when_stmt", "manager_expr", "manager_expr_ls",
  "manage_stmt", "class_decl_stmt", "class_start", "class_tag",
  "opt_inherit", "class_level_stmt_ls", "enum_decl_stmt",
  "enum_header_lcbr", "enum_ls", "$@5", "$@6", "enum_item",
  "lambda_decl_start", "lambda_decl_expr", "$@7", "linkage_spec_empty",
  "linkage_spec", "opt_fn_type_formal_ls", "fn_type_formal_ls",
  "fn_type_formal", "opt_fn_type_ret_type", "fn_type", "fn_expr", "$@8",
  "$@9", "fn_decl_stmt_complete", "fn_decl_stmt", "$@10",
  "fn_decl_stmt_inner", "fn_decl_stmt_start", "fn_decl_receiver_expr",
  "fn_ident", "op_ident", "assignop_ident", "all_op_name",
  "formal_var_arg_expr", "opt_formal_ls", "req_formal_ls",
  "formal_ls_inner", "formal_ls", "formal", "formal_ident_def",
  "named_formal", "opt_formal_intent_tag", "required_intent_tag",
  "opt_this_intent_tag", "proc_iter_or_op", "opt_ret_tag",
  "opt_throws_error", "opt_function_body_stmt", "function_body_stmt",
  "query_expr", "opt_lifetime_where", "lifetime_components_expr",
  "lifetime_expr", "lifetime_ident", "type_alias_decl_stmt",
  "type_alias_decl_stmt_start", "type_alias_decl_stmt_inner_ls",
  "type_alias_decl_stmt_inner", "opt_init_type", "var_decl_type",
  "var_decl_stmt", "$@11", "var_decl_stmt_inner_ls", "var_decl_stmt_inner",
  "tuple_var_decl_component", "tuple_var_decl_stmt_inner_ls",
  "opt_init_expr", "formal_or_ret_type_expr", "ret_type", "colon_ret_type",
  "opt_ret_type", "opt_type", "formal_type", "colon_formal_type",
  "opt_colon_formal_type", "expr_ls", "tuple_component", "tuple_expr_ls",
  "opt_actual_ls", "actual_ls", "actual_expr", "ident_expr",
  "sub_type_level_expr", "for_expr", "for_expr_base", "bracket_loop_expr",
  "bracket_loop_expr_base", "cond_expr", "nil_expr", "stmt_level_expr",
  "opt_task_intent_ls", "task_intent_clause", "task_intent_ls",
  "forall_intent_clause", "forall_intent_ls", "intent_expr",
  "task_var_prefix", "new_maybe_decorated", "new_expr", "let_expr",
  "range_literal_expr", "cast_expr", "tuple_expand_expr", "super_expr",
  "expr", "opt_expr", "opt_try_expr", "lhs_expr", "call_base_expr",
  "call_expr", "dot_expr", "parenthesized_expr", "bool_literal",
  "str_bytes_literal", "literal_expr", "assoc_expr_ls", "binary_op_expr",
  "unary_op_expr", "reduce_expr", "scan_expr", "reduce_scan_op_expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-1115)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-755)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1115,   100,  4689, -1115,   -34,    96, -1115, -1115, -1115, -1115,
   -1115, -1115, -1115, 13614,    85,   225,   113, 15988,   212, 20674,
      85,   234,   191,   252,   225,  6089, 13614,   139, 20729, -1115,
     341,   306, -1115, 11164,   339,   367,   200, -1115,   344, 20729,
   20729, 20729, -1115,  2346, 12214,   349, 13614, 13614,    48, -1115,
     378,   405, 13614, -1115, 15988, -1115, 13614,   477,   321,   220,
     232, 15336,   459, 20804, -1115, 13614,  9589, 13614, 12214, 15988,
   13614,   416,   475,   373,  6089,   497, 13614,   510,  8189,  8189,
   -1115,   519, -1115, 15988, -1115,   524, 11339, -1115,   579, 13614,
   -1115, 13614, -1115, -1115, 15816, 13614, -1115, 11514, -1115, -1115,
   -1115,  5039,  8714, 13614, -1115,  5739, -1115, -1115, -1115, -1115,
   20218,   470, -1115,    34,   451, -1115,   192, -1115, -1115, -1115,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115,
    8364, -1115,  8889, -1115, -1115,    27, -1115, -1115, -1115,   574,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115,
     486, 20729, -1115, 20414,   488,   435, -1115,   211, -1115, -1115,
   -1115,   179,   247, -1115, -1115, 20729,  2346, -1115,   482,   509,
   -1115, -1115, -1115,   513,   514, 13614,   516,   520, -1115, -1115,
   -1115, 19911,  3146,    84,   523,   527, -1115, -1115,   427, -1115,
   -1115, -1115, -1115, -1115,   221, -1115, -1115, -1115, 13614, 20729,
   -1115, 13614, 13614,    87,   638,   474, -1115, -1115, -1115, -1115,
   19911,   484, -1115, -1115,   525,  6089, -1115, -1115,   529,   149,
     528, -1115,   172, -1115,   536, -1115,   244, 19911,  9764, -1115,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115,
   -1115, -1115, -1115, -1115,   534, -1115, -1115, 20729, 20729,   -20,
    4175, -1115, -1115,   619,  9764,   537, -1115,   541,   266,   181,
    6089,   666,    42,     2, -1115,  6089, -1115, -1115, 17434, -1115,
      24, 18022,   522, -1115,   542,   545, -1115, 17434,   149,   522,
   -1115,  9764,   540,   540,    64,     0, -1115,    55, -1115, -1115,
   -1115, -1115,  9939,  9939, -1115,   432, -1115,   535,   565, -1115,
   -1115,  1931,   573, -1115, -1115, 19911,   329, 13614, 13614, -1115,
      -8, -1115, -1115, 19911,   555, 18067, -1115, 17434,   149, 19911,
     549,  9764, -1115, 19911, 18174, -1115, -1115, -1115, -1115, -1115,
     149,   557, -1115,   216,   216,  1449,   522,   522,   272, -1115,
   -1115,  5214,     9, -1115, 13614,   156,   170, -1115,  1449,   715,
   -1115, -1115, 20729, -1115, 20729, -1115,  5039, -1115,    77, -1115,
     619, -1115, -1115, -1115,   721,   554,  5389, 13614, -1115, 13614,
     662, -1115, -1115,  2746,   136,   486, 19911,   294, 13614,  6089,
   11689, 12389, 12389, 11164, 11864, -1115, -1115, -1115, 20729, 13614,
     559, -1115,   560, 20469,   607,   183, -1115,   288,   179, -1115,
   -1115, -1115, -1115,   644, -1115, -1115, -1115, -1115,    74,   648,
   -1115, -1115, -1115, 15646,   614,   330, -1115,   340,   485, -1115,
     593, 13614, 13614, 13614, 12389, 12389, 13614,   493, 13614, 13614,
   13614, 13614, 13614,   298, 15816, 13614, 13614, 13614, 13614, 13614,
   13614, 13614, 13614, 13614, 13614, 13614, 13614, 13614, 13614, 13614,
   13614,   676, -1115, -1115, -1115, -1115, -1115, 11339, 11339, -1115,
   -1115, -1115, -1115, 11339, -1115, -1115, 11339, 11339,  9764, 10114,
   12389, 12389, 18219,   575, 13614, 13614, 12564, 20729,  9064, -1115,
   -1115, -1115,  2346, -1115, 13614, -1115,   621,   576,   601, -1115,
   -1115, -1115,   626,   629,  6089,   722,  6089, -1115,   723, 13614,
     693,   587, 11339, -1115, 20729, -1115, 20729, -1115, -1115, -1115,
     666, -1115, -1115,   208, -1115, 13789,   634, 13614,  2346, -1115,
   -1115, 13614, -1115, 20544, 13614, 13614, -1115,   595, -1115, -1115,
   -1115, -1115, 20599, -1115,   179, 15510, -1115,   666, -1115,   620,
   13789, -1115, 19911, 19911, -1115,   528, -1115,    52, -1115, 11339,
     598, -1115,   753,   753, -1115, -1115, -1115, -1115, 12739, -1115,
   18259,  9239, -1115,  9414, -1115, -1115, -1115, 11689, 12389, 12389,
   -1115,   631, 10289, -1115,   437, -1115,  5564, -1115,   353, 18379,
     370,  3485, 20729,  8014,  7839, -1115,   486,   605, -1115,   172,
   -1115,  3093,   659, 20729,   609,    41, 17130,    28,  3002,    28,
   16978,    41, 17434, 16131,   -46,    90, 18425,   -77,   276, -1115,
   -1115,   618,   637,  1771, -1115, -1115, 13614,   152, -1115, -1115,
     229, -1115, -1115,    47, -1115, -1115,  7664, -1115,   680, -1115,
     615,   640, -1115,   642, -1115,   645,   646,   647,   650, -1115,
     651,   652,   653,   654,   655,   656,   657, -1115, -1115, -1115,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, 13614,
     668,   658,   615, -1115,   615, -1115, 13614, -1115, 20729, -1115,
   -1115,  2555,  2555,   471, -1115,   471, -1115,   471, 15381,   391,
    1601,  1994,   149,   216, -1115,   500, -1115, -1115, -1115, -1115,
   -1115,  1449, 19023,   471,  3811,  3811,  2555,  3811,  3811,  1346,
     216, 19023,  4328,  1346,   522,   522,   216,  1449,   632,   649,
     664,   665,   669,   670,   613,   635, -1115,   471, -1115,   471,
   13614, 11339, 17510, 17586,    95,   639,    86, -1115, -1115, -1115,
     235, -1115,  1619, 19956,   533,   384, 19911, 11339, -1115, 11339,
   13614, 13614,   778,  6089,   779, 18465,  6089, 17662, 20729, -1115,
     246, -1115, -1115,   259, -1115,   263,   666,    42, -1115, 19911,
   10464, -1115, 19911, -1115, -1115, -1115, 20729, 18541, 18617, -1115,
     620,   672, -1115,   267, 13789, -1115,   108, -1115, 13614, -1115,
     671,    13,   643, -1115,  2028, -1115, -1115, -1115, -1115, -1115,
   -1115, -1115, 17206, 17054,   116, -1115, -1115,   277, -1115, -1115,
   -1115, -1115, 13614, -1115,   137,  4481, 13614, -1115, 12914,  8014,
    7839, -1115, 10639,   538, 10814,   546,   551, 10989,  8539,   552,
     403, -1115, -1115, -1115, 18657,   673,   674,   661, -1115, 20729,
    2346, 12389,    41, 13614,   772, 13614, -1115,  6264, 12389, -1115,
     675, -1115,    41,  6439, 12389, -1115,    41, -1115,    41, 12389,
   -1115,    41, -1115, -1115, 19911,  6089,   677, 12389, 13089,  4864,
     681, 13614, 13264, -1115, 11339, -1115,   678,  2517, 19911, 20729,
     470,   586, -1115,   680, -1115, -1115, -1115, -1115, -1115,    23,
     388, -1115, 18703, -1115, 16237, -1115, -1115, 19911, -1115,   667,
     682, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115,   291,
   13964, 12389, 14139, 12389, 12389, 16131, 11339, -1115, -1115,  9064,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115,
   -1115, -1115,    39, 12389, 12389, -1115, -1115, -1115, 19911, 19911,
    6089, -1115,  6089, 13614, -1115,  6089,   788,   679,   683, 20729,
     528, -1115, -1115,   634, -1115, -1115, -1115,   686,   687,   108,
   13614,   680,   620, -1115, 12039, -1115, -1115, 19911, -1115,  6089,
   13614, -1115, -1115, -1115, 20729, -1115,   691,   528, 12389, 12389,
   12389,  6089, 11339, -1115, -1115, -1115, -1115, -1115,   500, -1115,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115,
   -1115, -1115, -1115,  8014, -1115, -1115, 18809,    44, 17754,   407,
     685,  8014, -1115, 13614, -1115, -1115, 20099,   424,   426,    85,
   18863, -1115, 18903, 13614,   297, 11164, 19911,    62, 17830,  9064,
   -1115, 11164, 19911,    33, 17282, -1115, -1115,    28, 17358, -1115,
   -1115,  6089,   -28, 17906, -1115, -1115, 19911,  6089, 19911, -1115,
   18983,   300, -1115, -1115, -1115, -1115, 20285, 20331, -1115, -1115,
   20469, -1115,    23,   142, 13614, -1115,   696,   689, -1115,   615,
     615, -1115, -1115, -1115, 13614,   791, 19063, 13614,   792, 19143,
     688, 19103,   315, -1115,   634,   318,   335, -1115, -1115, 19911,
     793,  6089, -1115, -1115, -1115, -1115,  9764,  9764, -1115, -1115,
   -1115, -1115,   108, 19911, -1115,    81,    82,   692, 13614, -1115,
      41, 17434,    28, 17358,   -18, 17982, -1115,   224, 13439,  8014,
    7839,  8014,  7839, -1115, -1115, -1115, 19263, 19911, 20145, -1115,
   -1115, -1115,    41,    41, -1115, 17434, -1115, 16318,  6614, -1115,
    6789, -1115,   342, -1115, 16398,  6964, -1115,    41,  7139, -1115,
      41, -1115,    41, -1115,    41, -1115,  7314,   697,  7489,   699,
   -1115, 13614,   683, 20331, -1115, -1115, -1115, -1115, -1115,   140,
   -1115,   165, -1115,   317, 19223,    12, -1115, -1115, -1115, 19339,
   14314, 14489, 19415, 14664, 14839, 15014, 15189, -1115, -1115,  6089,
   -1115,   694,   695, -1115, -1115,  6089, -1115,   528, 19911,  6089,
    6089, -1115, -1115, 19911,   429,   701,   433,   710, 13614, -1115,
   -1115, -1115, -1115,  6089, 11164, 19911, 11164, 19911,  9064, -1115,
    6089, 11164, 19911, -1115, 11164, 19911, -1115, -1115, -1115, 11164,
   -1115, 19911,  6089, 11164, -1115, 19911,  6089, 19911, -1115,    32,
   -1115, 13614,   142,   140,   140,   140,   140,   140,   140,   142,
    5914, -1115, -1115, -1115, -1115, -1115, 13614, 13614, 13614, 13614,
   13614, 13614, 13614, 13614, -1115,   714,   716,   659, -1115, -1115,
   -1115, -1115, -1115, 19911, 18465, 16478, 16558, -1115, 18465, 16638,
   16718, 16798, -1115, 16878, -1115, -1115, 19911, -1115, -1115, -1115,
   -1115, -1115, -1115, -1115,   698, -1115, 19491, 19567, 19643, 19719,
   19795, 19871, -1115, -1115,  6089,  6089,  6089,  6089,  6089,  6089,
   13614, 13614, 13614, 13614, 13614, 13614, 18465, 18465, 18465, 18465,
   18465, 18465
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   143,   480,   697,   698,   699,
     693,   694,   700,   214,   609,   134,   164,   575,   171,   577,
     609,   170,   510,   508,   134,     0,   214,   289,   165,   204,
     203,   691,   202,   214,   169,    69,   290,   358,   166,     0,
       0,     0,   353,     0,   214,   173,   214,   214,   626,   601,
     701,   174,   214,   359,   569,   505,   214,     0,     0,   188,
     186,   446,   168,   578,   507,   214,   214,   214,   214,   573,
     214,     0,   172,     0,     0,   144,   214,   692,   214,   214,
     498,   167,   333,   571,   509,   175,   214,   750,     0,   214,
     752,   214,   753,   754,   642,   214,   751,   214,   650,   184,
     749,     0,   214,   214,     4,     0,     5,    10,    11,    12,
      48,    51,    52,    58,     0,    46,    72,    13,    78,    14,
      15,    16,    17,    30,   555,   556,    24,    47,   185,   195,
     214,   205,   664,   196,    18,     0,    32,    31,    21,     0,
     284,   197,    19,    20,   660,    23,    36,    33,    34,   194,
     334,     0,   192,     0,     0,   657,   357,     0,   654,   190,
     374,   466,   457,   652,   193,     0,     0,   191,   669,   648,
     647,   651,   559,   557,     0,   214,   658,   659,   663,   662,
     661,     0,   558,     0,   670,   671,   672,   695,   696,   653,
     561,   560,   655,   656,     0,    29,   577,   165,   214,     0,
     578,   214,   214,     0,     0,   657,   669,   557,   658,   659,
     567,   558,   670,   671,     0,     0,   610,   135,     0,   576,
       0,   499,     0,   506,     0,    22,     0,   543,   214,   141,
     145,   157,   151,   150,   159,   138,   149,   160,   146,   161,
     136,   162,   155,   148,   156,   154,   152,   153,   137,   139,
     147,   158,   163,   142,     0,   140,   213,     0,     0,     0,
       0,    71,    70,    72,   214,   291,   295,     0,     0,     0,
       0,     0,     0,     0,   513,     0,   181,    41,     0,   325,
       0,   324,   735,   630,   627,   628,   629,     0,   570,   736,
       7,   214,   356,   356,   453,   178,   448,   179,   449,   450,
     454,   455,   177,   176,   456,   446,   539,     0,   361,   362,
     364,     0,   447,   538,   366,   526,     0,   214,   214,   182,
     654,   645,   668,   646,     0,     0,    44,     0,   574,   562,
       0,   214,    45,   568,     0,   303,    48,   307,   304,   307,
     572,     0,    57,   737,   739,   640,   734,   733,     0,    75,
      79,     0,     0,   545,   214,     0,     0,   546,   641,     0,
       6,   331,     0,   203,     0,   332,     0,    49,     0,     9,
      72,    50,    53,    56,     0,    55,     0,   214,    73,   214,
       0,   510,   201,     0,   693,   334,   665,   209,   214,     0,
     214,   214,   214,   214,   214,   216,   579,   592,     0,   214,
       0,   330,     0,     0,   351,     0,   344,   446,   466,   464,
     465,   463,   382,   468,   467,   471,   470,   472,     0,   461,
     458,   459,   462,     0,   503,     0,   500,     0,   649,    35,
     631,   214,   214,   214,   214,   214,   214,   738,   214,   214,
     214,   214,   214,     0,   639,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   413,   420,   421,   422,   417,   419,   214,   214,   415,
     418,   416,   414,   214,   424,   423,   214,   214,   214,   214,
     214,   214,     0,     0,   214,   214,   214,     0,   214,    37,
      25,    38,     0,    26,   214,    39,   555,     0,   550,   551,
     554,   343,     0,     0,     0,   258,     0,   413,   254,   214,
       0,     0,   214,   298,     0,   297,     0,   268,    27,   517,
       0,   518,   520,     0,   537,   214,   523,   214,     0,   180,
      40,   214,   327,     0,   214,   214,    42,     0,   189,   187,
     451,   452,     0,   447,   466,   446,   439,     0,   438,   541,
     214,   127,   666,   667,   372,     0,   183,     0,    43,   214,
       0,   314,   305,   306,    28,    77,    76,    80,   214,   702,
       0,   214,   687,   214,   689,     8,   291,   214,   214,   214,
     215,    58,   214,    54,     0,    66,     0,   125,     0,   120,
       0,    87,     0,   214,   214,   198,   334,     0,   211,     0,
     208,   609,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   593,     0,     0,   543,     0,   301,   293,
     294,     0,   335,     0,   342,   348,   214,   346,   341,   431,
       0,   433,   437,     0,   469,   531,     0,   533,   473,   460,
     429,   141,   409,   145,   407,   151,   150,   149,   146,   411,
     161,   162,   148,   152,   137,   147,   163,   406,   388,   391,
     389,   390,   412,   401,   392,   405,   397,   395,   408,   396,
     394,   399,   404,   393,   398,   402,   403,   400,   410,   214,
     385,     0,   429,   386,   429,   383,   214,   502,     0,   497,
     512,   730,   729,   732,   741,   740,   745,   744,   726,   723,
     724,   725,   643,   713,   143,     0,   683,   684,   144,   682,
     681,   637,   717,   728,   722,   720,   731,   721,   719,   711,
     716,   718,   727,   710,   714,   715,   712,   638,     0,     0,
       0,     0,     0,     0,     0,     0,   743,   742,   747,   746,
     214,   214,     0,     0,     0,   301,   620,   621,   623,   625,
       0,   612,     0,     0,     0,     0,   544,   214,   564,   214,
     214,   214,   257,     0,   253,     0,     0,     0,     0,   563,
       0,   296,   271,     0,   269,     0,   521,     0,   536,   535,
     214,   515,   636,   514,   326,   323,     0,     0,     0,   680,
     541,   367,   363,     0,   214,   542,   523,   365,   214,   371,
       0,     0,     0,   565,     0,   308,   703,   644,   688,   547,
     690,   548,     0,     0,     0,    59,    60,     0,    62,    64,
      68,    67,   214,   102,     0,     0,   214,    95,   214,   214,
     214,    65,   214,   388,   214,   389,   390,   214,   214,   400,
       0,   425,   426,    82,    81,    94,     0,     0,   336,     0,
       0,   214,     0,   214,     0,   214,   226,     0,   214,   225,
       0,   234,     0,     0,   214,   232,     0,   242,     0,   214,
     240,     0,   219,   218,   594,     0,     0,   214,   214,     0,
       0,   214,   214,   706,   214,   299,     0,   356,   352,     0,
       0,   446,   432,   473,   529,   528,   527,   530,   474,   481,
     446,   381,     0,   387,     0,   377,   378,   504,   501,     0,
       0,   133,   131,   132,   130,   129,   128,   678,   679,     0,
     214,   214,   214,   214,   214,   704,   214,   622,   624,   214,
     611,   164,   171,   170,   169,   166,   173,   174,   168,   172,
     167,   175,     0,   214,   214,   511,   553,   552,   266,   265,
       0,   260,     0,   214,   256,     0,   262,     0,   300,     0,
       0,   519,   522,   523,   524,   525,   322,     0,     0,   523,
     214,   473,   541,   540,   214,   442,   440,   373,   316,     0,
     214,   315,   318,   566,     0,   309,   312,     0,   214,   214,
     214,     0,   214,    61,   126,   123,   103,   115,   109,   108,
     107,   117,   104,   118,   113,   106,   114,   112,   110,   111,
     105,   116,   119,   214,   122,   121,    90,    89,    88,     0,
       0,   214,   199,   214,   200,   336,   356,     0,     0,   609,
     609,   222,     0,   214,     0,   214,   582,     0,     0,   214,
     235,   214,   588,     0,     0,   233,   243,     0,     0,   241,
     250,     0,     0,     0,   705,   248,   595,     0,   708,   707,
       0,     0,   329,   188,   186,   328,   356,   356,   337,   347,
       0,   434,   481,     0,   214,   375,   436,     0,   384,   429,
     429,   685,   686,   300,   214,     0,     0,   214,     0,     0,
       0,     0,     0,   613,   523,   669,   669,   259,   255,   600,
     261,     0,    74,   270,   267,   516,   214,   214,   443,   441,
     368,   369,   523,   428,   320,     0,     0,     0,   214,   310,
       0,     0,     0,     0,     0,     0,    63,     0,   214,   214,
     214,   214,   214,    96,    99,    85,    84,    83,   356,   206,
     212,   210,     0,     0,   217,     0,   252,     0,     0,   224,
       0,   223,     0,   615,     0,     0,   230,     0,     0,   228,
       0,   238,     0,   236,     0,   251,     0,     0,     0,     0,
     249,   214,   302,   356,   339,   338,   350,   354,   495,     0,
     496,   483,   486,     0,   482,     0,   430,   379,   380,     0,
     214,   214,     0,   214,   214,   214,   214,   302,   617,     0,
     264,     0,     0,   445,   444,     0,   319,     0,   313,     0,
       0,   124,    92,    91,     0,     0,     0,     0,   214,   207,
     221,   220,   227,     0,   214,   581,   214,   580,   214,   614,
       0,   214,   587,   231,   214,   586,   229,   239,   237,   214,
     246,   597,     0,   214,   244,   596,     0,   709,   340,     0,
     494,   214,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   475,   477,   479,   376,   476,   214,   214,   214,   214,
     214,   214,   214,   214,   263,   632,   633,   321,   311,    98,
     101,    97,   100,    86,   585,     0,     0,   616,   591,     0,
       0,     0,   247,     0,   245,   355,   485,   487,   488,   491,
     492,   493,   489,   490,   484,   478,     0,     0,     0,     0,
       0,     0,   634,   635,     0,     0,     0,     0,     0,     0,
     214,   214,   214,   214,   214,   214,   584,   583,   590,   589,
     599,   598
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1115, -1115, -1115,    -1,   -59,  2726, -1115,   241,   736, -1115,
   -1115,   564,  -150,  -107, -1115,   270, -1115, -1115,  -167, -1115,
   -1115,   143,   570, -1115,  -547,  3395,   503,  -583, -1115,  -804,
   -1115, -1115, -1115,    38, -1115, -1115, -1115,   857, -1115,    22,
    -171, -1115, -1115,  -441,  3249, -1114,  -852, -1115, -1115,   358,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115,     7, -1115,
     515,   660, -1115,   492, -1115, -1115,   -72,  1633, -1115,  -344,
     490, -1115, -1115, -1115, -1115, -1115, -1115, -1115,   550, -1115,
     -94, -1115, -1115, -1115, -1115,   360, -1115, -1115, -1115,  -103,
   -1115,  -355,  -809, -1115, -1115, -1115, -1115, -1115,  -390, -1115,
     130, -1115, -1115, -1115, -1115, -1115,   347, -1115,   461, -1115,
   -1115, -1115, -1115,   508, -1115, -1115, -1115, -1115,    -6,  -401,
    -163,  -791,  -922,  -651, -1115,     1, -1115,     8,   361,   -47,
     597,   -44, -1115, -1115,  -363,  -835, -1115,  -349, -1115,  -168,
    -354,  -346,  -800, -1115, -1115,    59,   222, -1115,  -201,   782,
   -1115,  -154,   381,   138,  -245,  -772,  -611, -1115, -1115, -1115,
    -618,  -523, -1115,  -753,   -19,   -81, -1115,  -253,  -492,  -541,
      -2, -1115, -1115, -1115, -1115, -1115, -1115,   311, -1115,   -14,
   -1115, -1115,  -519, -1115,  -901, -1115, -1115,   518,  1019, -1115,
   -1115, -1115, -1115,  2909, -1115,   -50,  1336, -1115,  1426,  1814,
   -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115, -1115,  -483
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   350,   105,   764,   107,   108,   109,   368,
     369,   110,   111,   112,   113,   375,   583,   817,   818,   114,
     115,   116,   380,   117,   118,   119,   351,   845,   590,   846,
     120,   121,  1014,   587,   588,   122,   123,   218,   253,   272,
     124,   255,   125,   306,   859,   126,   127,   293,   292,   128,
     129,   130,   131,   132,   133,   599,   849,   134,   203,   136,
     395,   605,   137,   259,   138,   773,   774,   204,   140,   266,
     267,   621,   268,   141,   142,   143,   144,   145,   562,   805,
     987,   146,   147,   801,   982,   279,   280,   148,   149,   150,
     151,   400,   887,   152,   153,   405,   889,   890,   406,   154,
     205,  1249,   156,   157,   307,   308,   309,   971,   158,   321,
     555,   798,   159,   160,  1185,   161,   162,   681,   682,   841,
     842,   843,   975,   901,   408,   630,  1077,   631,   549,   632,
     311,   543,   423,   412,   418,   899,  1264,  1265,   163,  1075,
    1181,  1182,  1183,   164,   165,   425,   426,   687,   166,   167,
     222,   273,   274,   522,   523,   781,   313,   897,   637,   638,
     526,   314,   795,   796,   352,   355,   356,   497,   498,   499,
     206,   169,   170,   396,   171,   397,   172,   207,   174,   215,
     216,   750,   866,  1152,   751,   752,   175,   208,   209,   178,
     179,   180,   322,   181,   387,   500,   211,   183,   212,   213,
     186,   187,   188,   189,   617,   190,   191,   192,   193,   194
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     168,   104,   305,   403,   372,   754,   220,   226,   799,   135,
     840,   511,   427,   625,   310,   893,   324,   312,   802,   477,
     770,   492,   683,   168,   976,   896,  1020,   797,  1093,   385,
     597,   905,   135,   906,  1015,  1068,   341,   969,   537,  1026,
     524,   819,   373,   524,  1260,   633,   316,  1108,   524,   527,
     254,   388,   357,   800,   619,  -289,   275,   496,  1072,   389,
     275,   265,   269,   270,  1260,  1155,   283,   390,   391,   392,
     882,  1263,   168,   275,   860,   635,   168,   168,   560,  -534,
    1129,   135,   504,  1073,   778,   135,   135,   979,   862,  -534,
     868,   871,   860,   496,  1148,   876,   880,   509,   883,   168,
       3,   388,   860,   168,    66,   388,  -532,  -534,   135,   389,
    -290,   540,   135,  1205,   275,   554,  1130,   577,   578,   392,
     496,   390,   391,   392,    66,   284,  -317,   484,   485,   875,
     980,   195,   155,   927,  -532,  1263,  1111,   877,  1153,  -534,
     704,  1074,   924,  1178,   285,  1178,   393,  1166,   860,   528,
     366,  -272,  -534,   860,   541,   155,   568,  1209,   361,   963,
     496,   286,  -370,   990,  -534,  -534,  -532,  -534,  -273,  -317,
     771,   531,   228,   401,  1068,   404,   928,  1261,   263,  -532,
     569,   791,   366,   374,   981,   525,  -534,   424,   525,  -534,
    1203,  1105,  -532,   525,   366,   895,   393,  1109,   366,    23,
     393,   394,   366,   366,   155,   214,   413,  -534,   155,   155,
     860,   366,  -534,   168,  1174,  1175,  1138,  -534,   947,  1112,
     636,   483,   135,  -317,   365,   734,   735,   264,   217,   494,
    1135,   155,   366,   780,  1179,   155,   860,   878,   377,  -532,
     708,   847,   878,  1180,  -532,  1180,  1019,   433,    82,   919,
      55,   579,   366,   370,   414,   394,   478,   415,   479,   974,
     684,   486,    64,   494,   409,   879,   -71,  -279,   168,   416,
     925,   973,   710,   168,   419,   775,   378,   135,   -70,   502,
     503,   596,   135,  1251,   410,   -71,  1068,    84,   417,  -278,
     437,   991,  -349,   521,   411,   880,   372,   -70,   443,  -345,
     221,   704,   793,   571,   -71,   434,   379,   496,   496,   435,
     480,   228,  1252,   173,   481,   294,   -70,   573,   705,   335,
     338,  1248,  1198,  -345,  1094,   420,  1215,  1287,  1217,   572,
     627,   706,   786,   548,   -71,   296,   173,   421,   755,   298,
    1204,   496,   223,   574,    37,   155,   -70,  -356,   305,   168,
     567,   366,   256,   516,   628,   776,   422,   437,   135,  1110,
     707,  -287,   441,   299,   168,   443,   300,  -356,    57,   446,
      53,  1021,   264,   135,   168,   615,   891,  -356,   301,  1250,
     622,   777,   929,   135,   254,   173,   576,   168,   496,   173,
     173,   494,  1061,   759,  -277,  1211,   135,   304,   850,  -274,
     155,   708,   892,   598,  -281,   155,   959,   709,   930,   495,
     776,   496,   173,   514,   776,   294,   173,   729,   730,   958,
     618,   685,   433,   731,   992,   404,   732,   733,  1187,  1188,
    1127,   515,   960,  -285,  1092,   296,   961,   195,   759,   298,
     972,  -291,  1253,   565,   494,   680,   754,   759,   884,   261,
     993,   819,   262,  1298,  1299,  1300,  1301,  1302,  1303,   294,
    -282,   629,   759,   299,  1083,  -619,   300,   744,    57,  1254,
    1146,  1255,  1256,  1172,  1257,  1258,   494,   688,   301,   296,
     434,   155,  -618,   298,   435,   290,   305,   528,  1197,  1228,
     809,  -619,   811,   291,   551,   689,   155,   304,   310,  1069,
     822,   312,   168,   683,   168,   690,   155,   299,  -618,   745,
     300,   135,   359,   135,  -276,  1229,   330,   826,   823,   155,
     176,   357,   301,   357,  1157,  1160,   173,   320,  1162,  1164,
    -280,   528,   437,  1167,  1169,   827,   576,   441,   772,   442,
     443,   304,   521,   176,   446,   331,  1214,   320,  1216,   945,
    1021,   453,  -286,   433,  1021,   785,   754,   457,   458,   459,
     814,  -435,   361,   320,   548,  -288,    22,    23,  1022,   521,
     496,   688,  1133,   528,  -275,   362,  1021,    29,   363,  -283,
    1021,   173,   342,    32,   168,   567,   173,   852,   496,  1140,
      37,  1141,   176,   135,  1279,   364,   176,   176,  1281,  -677,
    -606,  -677,   195,  1157,  1160,  1167,  1169,  -676,   820,  -676,
      88,   434,   437,   294,   831,   435,    53,   441,    55,   176,
     443,   376,   944,   176,   446,   854,   481,  -752,   365,   398,
      64,  -752,   399,   296,   155,  -753,   155,   298,  1262,  -753,
    -754,  -749,   336,   336,  -754,  -749,  -676,  -603,  -676,    80,
     538,   539,    82,   995,   710,    84,  -673,  -675,  -673,  -675,
     407,   299,   173,   437,   300,  -674,    57,  -674,   441,   229,
     442,   443,   909,   428,   910,   446,   301,   173,  -602,   429,
    1176,  -607,   453,   230,   231,  -608,   232,   173,  -605,   320,
     459,   233,  -604,   487,   490,   304,  1028,   488,   366,   234,
     173,   493,  1262,   378,   501,   235,   513,   946,   544,   512,
     424,   236,   545,   496,   534,   237,   155,   535,   238,   550,
     556,   559,   564,   575,   581,   320,   582,   592,   239,   623,
     965,   624,   626,   176,   634,   240,   241,  1067,   639,   686,
    1070,  1080,   443,   242,   728,   754,   757,   741,   759,   758,
     942,   760,   320,   243,   761,   496,   763,   766,   768,   780,
     769,   168,   244,   245,   168,   246,   794,   247,   789,   248,
     135,   803,   249,   135,   804,   848,   250,   519,   853,   251,
     374,   855,   252,   885,   494,   898,   917,   900,   176,  -143,
     957,  -164,   320,   176,  -171,  -170,  -169,   911,   521,  -166,
    -173,  -174,  -168,  -172,  -144,  -167,  -175,   904,   966,   903,
     918,   926,   950,   952,   912,   173,   983,   173,   970,  1033,
    1021,   496,  1101,  1190,  1193,  1126,   986,  1199,  1066,   913,
     914,  1025,   305,   510,   915,   916,  1034,  1118,   520,  1024,
    1081,   305,   978,   891,  1102,  1252,   367,  1039,  -292,  1062,
    1134,   815,  1051,  1201,  1202,   168,  1057,  1082,  1106,  1107,
     994,   168,  1186,  1195,   135,  1207,  1280,  1275,  1276,   176,
     135,   424,  1242,   168,  1246,  1282,  1067,   168,  1312,   586,
    1313,   224,   135,   580,   176,   611,   135,  1103,   620,   563,
    1117,   784,   792,   155,   176,   600,   155,   173,  1079,  1071,
    1295,  1076,   542,   790,  1177,  1304,  1297,   176,  1027,   783,
     908,   404,   382,     0,   962,     0,  1173,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   680,     0,   320,   320,
       0,     0,     0,     0,   320,   496,   496,   320,   320,   320,
     320,  1095,  1096,     0,     0,     0,     0,     0,   168,     0,
     168,     0,     0,   168,     0,     0,     0,   135,     0,   135,
     372,  1116,   135,   372,     0,     0,     0,  1066,     0,     0,
       0,     0,     0,   320,     0,     0,     0,   168,     0,     0,
       0,   772,     0,     0,   509,     0,  1115,   155,  1067,   168,
       0,   509,     0,   155,     0,     0,     0,     0,   135,     0,
       0,     0,     0,     0,     0,   155,   986,     0,     0,   155,
       0,     0,     0,     0,     0,  1142,  1143,     0,     0,     0,
     320,   177,   176,     0,   176,     0,     0,     0,     0,     0,
       0,     0,   320,     0,   320,     0,     0,     0,     0,     0,
       0,     0,     0,   320,   177,     0,     0,     0,     0,   168,
       0,   607,   609,     0,   614,   168,     0,     0,   135,     0,
       0,     0,     0,     0,   135,     0,   372,     0,     0,     0,
       0,     0,     0,     0,   173,     0,     0,   173,     0,  1066,
     155,     0,   155,     0,     0,   155,     0,     0,     0,     0,
       0,     0,   404,   177,   694,   696,     0,   177,   177,   168,
       0,     0,     0,     0,   176,     0,     0,     0,   135,   155,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     177,   155,   509,   509,   177,     0,   509,   509,   509,     0,
     509,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     736,   738,     0,     0,     0,     0,   168,     0,   168,     0,
       0,     0,     0,   168,     0,   135,   168,   135,     0,     0,
       0,     0,   135,     0,   168,   135,   168,     0,   173,     0,
       0,     0,     0,   135,   173,   135,     0,     0,     0,     0,
       0,   155,     0,     0,     0,     0,   173,   155,     0,     0,
     173,     0,     0,     0,     0,     0,     0,   168,     0,     0,
       0,     0,   320,   168,     0,     0,   135,   168,   168,     0,
       0,     0,   135,     0,     0,     0,   135,   135,   320,     0,
     320,   168,     0,     0,     0,     0,     0,     0,   168,     0,
     135,   155,     0,     0,   177,     0,     0,   135,   607,   614,
     168,   320,     0,     0,   168,     0,     0,     0,     0,   135,
       0,     0,     0,   135,     0,     0,     0,     0,   168,  1305,
       0,   173,     0,   173,     0,     0,   173,   135,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   155,     0,
     155,   176,     0,     0,   176,   155,     0,     0,   155,   177,
     173,     0,     0,     0,   177,     0,   155,     0,   155,     0,
       0,     0,   173,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   168,   168,   168,   168,   168,   168,     0,     0,
       0,   135,   135,   135,   135,   135,   135,     0,     0,   155,
       0,     0,     0,     0,     0,   155,     0,     0,   182,   155,
     155,     0,     0,     0,     0,   320,     0,     0,     0,     0,
       0,     0,     0,   155,     0,     0,     0,     0,     0,     0,
     155,   182,   173,     0,     0,     0,     0,     0,   173,     0,
     177,     0,   155,     0,     0,   176,   155,   433,     0,     0,
       0,   176,     0,     0,     0,   177,     0,   320,     0,     0,
     155,     0,     0,   176,     0,   177,     0,   176,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   177,     0,
     182,     0,   173,     0,   182,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,     0,
       0,     0,     0,     0,     0,   434,     0,   182,     0,   435,
       0,   182,     0,     0,   155,   155,   155,   155,   155,   155,
       0,   184,     0,   320,     0,     0,     0,     0,     0,   173,
       0,   173,     0,     0,     0,     0,   173,     0,   176,   173,
     176,     0,     0,   176,     0,     0,     0,   173,     0,   173,
     433,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,     0,   176,     0,   446,
     184,     0,     0,     0,   184,   184,   453,     0,     0,   176,
     173,  1029,   457,   458,   459,     0,   173,     0,  1037,     0,
     173,   173,     0,   177,  1043,   177,     0,   184,     0,  1047,
       0,   184,     0,     0,   173,     0,     0,  1052,   434,     0,
       0,   173,   435,     0,     0,     0,     0,     0,     0,     0,
       0,   182,     0,   173,     0,     0,     0,   173,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   320,   320,   176,
       0,   173,     0,     0,     0,   176,     0,     0,     0,     0,
       0,  1085,     0,  1088,  1090,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,     0,
       0,     0,   446,   694,   736,   177,   182,     0,   452,   453,
       0,   182,   456,     0,     0,   457,   458,   459,     0,   176,
       0,     0,   704,     0,     0,   173,   173,   173,   173,   173,
     173,     0,   433,     0,     0,   139,   931,     0,     0,   932,
       0,   184,     0,     0,   933,     0,     0,     0,  1120,  1122,
    1124,     0,     0,     0,   197,     0,     0,     0,   139,     0,
       0,     0,     0,     0,   934,     0,   176,     0,   176,     0,
       0,   935,     0,   176,     0,     0,   176,     0,     0,     0,
       0,   936,     0,     0,   176,     0,   176,   182,     0,   937,
     434,     0,     0,     0,   435,     0,   184,     0,     0,     0,
       0,   184,   182,     0,     0,     0,   938,   139,     0,     0,
       0,     0,   182,     0,     0,     0,     0,   176,   939,     0,
       0,     0,   708,   176,     0,   182,     0,   176,   176,   940,
       0,     0,     0,     0,   139,   941,     0,     0,   139,     0,
       0,   176,   437,   438,     0,     0,   440,   441,   176,   442,
     443,     0,     0,     0,   446,     0,     0,     0,     0,     0,
     176,   453,     0,     0,   176,     0,     0,   457,   458,   459,
       0,     0,   886,     0,     0,     0,     0,   184,   176,     0,
       0,     0,   177,     0,     0,   177,     0,     0,     0,     0,
       0,     0,   184,  -336,     0,     0,     0,  -336,  -336,     0,
       0,     0,   184,     0,     0,     0,  -336,     0,  -336,  -336,
       0,     0,     0,     0,  -336,   184,   185,     0,     0,     0,
       0,  -336,     0,     0,  -336,     0,  -336,     0,     0,     0,
       0,     0,   176,   176,   176,   176,   176,   176,     0,   185,
     182,     0,   182,     0,  -336,     0,     0,  -336,   139,  -336,
       0,  -336,     0,  -336,  -336,     0,  -336,     0,     0,  -336,
       0,  -336,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   177,     0,     0,     0,
    -336,     0,   177,  -336,     0,     0,  -336,     0,   185,     0,
       0,     0,   185,   185,   177,     0,     0,     0,   177,     0,
       0,     0,     0,   139,     0,     0,     0,     0,   139,     0,
       0,  -336,     0,     0,     0,   185,     0,     0,     0,   185,
       0,     0,   182,     0,     0,     0,     0,     0,     0,     0,
     184,     0,   184,     0,   229,     0,  -336,     0,     0,     0,
       0,     0,  -336,     0,     0,     0,     0,     0,   230,   231,
       0,   232,     0,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,   234,     0,     0,     0,     0,   177,
     235,   177,     0,     0,   177,     0,   236,     0,     0,     0,
     237,     0,     0,   238,   139,     0,     0,     0,     0,     0,
       0,     0,     0,   239,     0,     0,     0,     0,   177,   139,
     240,   241,     0,     0,     0,     0,     0,     0,   242,   139,
     177,     0,   184,     0,     0,     0,     0,     0,   243,     0,
       0,     0,   139,     0,     0,   433,     0,   244,   245,   185,
     246,   229,   247,     0,   248,     0,     0,   249,     0,     0,
       0,   250,   546,     0,   251,   230,   231,   252,   232,     0,
       0,     0,     0,   233,     0,     0,     0,     0,     0,     0,
       0,   234,     0,     0,     0,     0,     0,   235,     0,     0,
     177,     0,     0,   236,     0,     0,   177,   237,     0,     0,
     238,     0,     0,   434,   185,     0,     0,   435,     0,   185,
     239,     0,     0,     0,     0,     0,     0,   240,   241,   182,
       0,     0,   182,   547,     0,   242,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   243,     0,     0,     0,     0,
     177,     0,     0,     0,   244,   245,     0,   246,     0,   247,
       0,   248,     0,     0,   249,   437,   438,   139,   250,   139,
     441,   251,   442,   443,   252,     0,     0,   446,     0,     0,
       0,     0,     0,     0,   453,     0,     0,     0,     0,     0,
     457,   458,   459,     0,     0,   185,     0,   177,     0,   177,
       0,     0,     0,     0,   177,     0,     0,   177,     0,     0,
     185,     0,     0,     0,     0,   177,     0,   177,     0,   184,
     185,     0,   184,   182,     0,     0,     0,     0,   366,   182,
     984,     0,     0,   185,     0,     0,     0,     0,     0,     0,
       0,   182,     0,     0,     0,   182,     0,     0,   177,   139,
       0,     0,     0,     0,   177,     0,     0,     0,   177,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   177,     0,     0,     0,     0,     0,     0,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   177,     0,     0,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   177,
       0,     0,     0,   184,     0,     0,   182,     0,   182,   184,
       0,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   184,     0,     0,     0,   184,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   182,     0,     0,   185,     0,
     185,     0,     0,     0,     0,     0,     0,   182,     0,     0,
       0,     0,     0,   177,   177,   177,   177,   177,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   229,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   230,   231,     0,   232,     0,     0,     0,
       0,   233,     0,     0,     0,     0,   184,     0,   184,   234,
       0,   184,     0,     0,     0,   235,     0,   182,     0,     0,
       0,   236,     0,   182,     0,   237,   139,     0,   238,   139,
     185,     0,     0,     0,     0,   184,     0,     0,   239,     0,
       0,     0,     0,     0,     0,   240,   241,   184,     0,     0,
       0,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   243,     0,     0,     0,   182,     0,     0,
       0,     0,   244,   245,     0,   246,     0,   247,     0,   248,
       0,     0,   249,     0,     0,     0,   250,     0,     0,   251,
       0,     0,   252,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   184,     0,     0,
       0,     0,     0,   184,   182,     0,   182,     0,     0,     0,
     139,   182,     0,     0,   182,     0,   139,     0,     0,     0,
       0,     0,   182,     0,   182,     0,     0,     0,   139,     0,
       0,     0,   139,     0,     0,     0,     0,     0,   271,     0,
       0,     0,     0,     0,     0,     0,     0,   184,     0,     0,
       0,     0,     0,     0,     0,   182,     0,     0,     0,   361,
       0,   182,     0,    22,    23,   182,   182,     0,     0,     0,
       0,     0,   362,     0,    29,   363,     0,     0,     0,   182,
      32,     0,     0,     0,     0,     0,   182,    37,     0,     0,
       0,     0,   364,     0,   184,     0,   184,   185,   182,     0,
     185,   184,   182,   139,   184,   139,   433,     0,   139,     0,
       0,     0,   184,    53,   184,    55,   182,    57,     0,  1063,
       0,     0,  1064,     0,     0,   365,     0,    64,     0,     0,
       0,     0,   139,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   139,   184,    80,     0,     0,    82,
       0,   184,    84,     0,     0,   184,   184,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,   435,   184,
     182,   182,   182,   182,   182,   182,   184,    88,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   184,     0,
       0,   185,   184,     0,     0,     0,     0,   185,     0,   436,
       0,     0,    99,     0,   139,     0,   184,     0,  1065,   185,
     139,     0,     0,   185,     0,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,     0,   450,   451,   452,   453,   454,   455,   456,     0,
       0,   457,   458,   459,     0,     0,     0,     0,   106,     0,
       0,     0,   460,     0,   139,     0,     0,     0,     0,     0,
     184,   184,   184,   184,   184,   184,     0,     0,     0,     0,
       0,   225,     0,     0,     0,     0,     0,     0,   431,     0,
       0,     0,     0,     0,   185,   432,   185,     0,     0,   185,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,   139,   593,   139,     0,     0,     0,     0,   139,     0,
       0,   139,     0,   185,     0,     0,     0,     0,     0,   139,
     332,   139,     0,     0,     0,   185,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   594,     0,
       0,     0,     0,     0,     0,     0,     0,   106,     0,     0,
       0,   360,   139,     0,     0,   434,     0,     0,   139,   435,
       0,     0,   139,   139,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   139,     0,     0,     0,
       0,     0,     0,   139,     0,   185,     0,     0,     0,     0,
     436,   185,     0,     0,     0,   139,     0,     0,     0,   139,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,   139,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   595,   457,   458,   459,   185,     0,     0,     0,     0,
       0,     0,   210,   460,     0,     0,   219,     0,     0,     0,
       0,     0,     0,     0,     0,   227,     0,     0,     0,     0,
       0,   489,   260,     0,     0,     0,     0,   139,   139,   139,
     139,   139,   139,   278,     0,   281,   282,     0,     0,     0,
       0,   287,   185,   288,   185,   289,     0,     0,     0,   185,
     315,     0,   185,     0,   227,   323,   325,   327,   328,   329,
     185,     0,   185,   333,     0,   334,     0,     0,     0,     0,
       0,     0,   340,     0,     0,   323,   518,     0,   343,     0,
     344,   529,     0,   345,   346,     0,   347,     0,     0,     0,
     227,   323,   358,   185,   431,     0,     0,     0,     0,   185,
       0,   432,     0,   185,   185,     0,     0,     0,     0,     0,
       0,     0,     0,   433,   863,     0,     0,   185,     0,   383,
       0,   386,     0,     0,   185,     0,     0,     0,     0,   864,
       0,     0,     0,     0,     0,     0,   185,     0,     0,     0,
     185,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   185,     0,     0,   106,     0,     0,
       0,     0,     0,     0,   430,     0,     0,     0,     0,     0,
       0,   434,   106,     0,     0,   435,     0,     0,     0,     0,
       0,     0,   106,     0,     0,   431,     0,   482,     0,     0,
     333,   227,   432,     0,     0,   602,     0,     0,     0,     0,
       0,     0,   860,     0,   433,     0,   436,     0,   185,   185,
     185,   185,   185,   185,     0,     0,     0,   323,     0,     0,
     851,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,     0,     0,   457,   458,
     459,     0,   366,   323,     0,     0,     0,     0,     0,   460,
       0,     0,   434,     0,     0,     0,   435,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     323,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   329,   333,   214,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,   552,   553,     0,     0,
     762,     0,     0,     0,   437,   438,     0,   439,   440,   441,
     323,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,     0,   457,
     458,   459,     0,   570,     0,     0,     0,     0,     0,     0,
     460,   461,   462,   463,   464,   465,   466,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   589,     0,   591,     0,
       0,     0,     0,   277,     0,     0,     0,   601,     0,   606,
     608,   610,   612,   616,     0,     0,     0,     0,   227,     0,
       0,     0,   106,     0,     0,   476,     0,   326,  -673,     0,
    -673,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     691,   692,   693,   695,   697,   698,     0,   699,   700,   701,
     702,   703,     0,   711,   712,   713,   714,   715,   716,   717,
     718,   719,   720,   721,   722,   723,   724,   725,   726,   727,
       0,     0,     0,     0,     0,     0,   323,   323,     0,     0,
       0,     0,   323,     0,     0,   323,   323,   323,   323,   737,
     739,     0,     0,   742,   743,   616,     0,   753,     0,     0,
       0,     0,     0,   756,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   765,     0,     0,   767,     0,
       0,   323,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   779,     0,   782,     0,     0,   276,
     281,     0,     0,   787,   788,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   315,     0,     0,     0,     0,   315,
       0,     0,     0,   276,     0,     0,     0,     0,   323,     0,
       0,     0,     0,   337,   339,     0,     0,   756,     0,     0,
     323,     0,   323,     0,     0,     0,   812,   813,   227,   951,
       0,   323,   954,     0,     0,     0,     0,   431,   828,     0,
       0,     0,   844,   844,   432,   371,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,   829,   874,     0,     0,     0,     0,   530,     0,   532,
       0,     0,     0,     0,     0,   888,   536,     0,     0,     0,
       0,     0,     0,     0,     0,   315,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   830,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,     0,   558,     0,   435,     0,
       0,     0,     0,   529,     0,     0,     0,     0,   902,   529,
       0,     0,     0,     0,     0,   907,     0,     0,     0,     0,
       0,  1050,     0,     0,     0,  1055,     0,     0,     0,   436,
       0,     0,     0,     0,     0,   491,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   765,
     323,   457,   458,   459,   505,   508,     0,     0,     0,     0,
       0,     0,   460,     0,   517,     0,   323,     0,   323,   948,
     949,     0,     0,   276,     0,   276,  1097,     0,  1098,     0,
       0,  1100,   276,     0,     0,     0,     0,     0,     0,   323,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   315,     0,  1114,     0,   977,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1055,     0,     0,
       0,     0,   276,     0,     0,     0,     0,     0,     0,     0,
       0,   589,     0,     0,     0,  1016,     0,  1018,   844,   844,
       0,   343,     0,   344,     0,     0,   346,   347,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1030,     0,  1032,     0,   227,     0,  1036,  1038,     0,     0,
       0,     0,  1042,  1044,     0,     0,     0,  1165,  1048,     0,
       0,     0,     0,  1170,     0,     0,  1053,   756,  1056,     0,
    1058,  1060,     0,   323,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1200,     0,  1036,
    1086,  1042,  1089,  1091,  1056,   323,     0,     0,   753,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   695,   737,   856,     0,   861,   865,   867,   870,
     872,   873,  1099,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   529,     0,   529,     0,     0,   315,
       0,   529,     0,  1113,   529,     0,     0,     0,     0,   227,
       0,     0,  1240,     0,  1244,     0,     0,  1121,  1123,  1125,
     434,   323,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   844,     0,     0,  1274,     0,     0,     0,     0,
    1136,  1277,  1137,     0,     0,  1240,  1244,     0,     0,     0,
       0,     0,  1145,     0,  1147,     0,     0,     0,   753,     0,
    1154,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,     0,   446,     0,     0,     0,  1292,     0,
     452,   453,  1294,     0,   456,     0,     0,   457,   458,   459,
       0,     0,     0,  1184,     0,     0,   106,     0,   460,     0,
       0,     0,     0,  1189,     0,     0,  1192,     0,     0,     0,
     276,   276,   276,   276,   276,   276,   276,   276,     0,     0,
       0,     0,     0,     0,     0,   323,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1208,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1213,   844,   844,
     844,   844,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1225,     0,  1227,
       0,     0,   865,     0,  1232,     0,     0,  1235,     0,     0,
       0,     0,     0,     0,     0,  1241,     0,  1245,     0,     0,
    1247,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1225,
    1227,  1031,  1232,  1235,  1241,  1245,     0,     0,     0,     0,
       0,  1040,     0,     0,     0,  1045,     0,  1046,     0,     0,
    1049,     0,     0,     0,     0,     0,     0,  1283,     0,     0,
       0,     0,  1284,  1285,     0,  1286,     0,   753,     0,  1288,
    1289,     0,     0,  1290,     0,     0,     0,     0,  1291,     0,
       0,     0,  1293,     0,     0,     0,     0,     0,     0,     0,
    1296,     0,   956,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1284,  1306,  1307,  1288,  1308,
    1309,  1310,  1311,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,   985,
       0,     0,     0,     0,     0,     0,   433,   276,   276,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1326,  1327,  1328,  1329,  1330,  1331,  1326,
    1327,  1328,  1329,  1330,  1331,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   276,     0,     0,
       0,   276,     0,   276,   434,     0,   276,     0,   435,     0,
       0,     0,     0,     0,     0,     0,     0,   506,     0,     0,
       0,     0,     0,     0,     0,     0,  1149,  1151,     0,     0,
       0,     0,  1156,  1159,     0,     0,  1161,  1163,     0,   436,
     507,   462,   463,   464,   465,   466,     0,     0,   469,   470,
     471,   472,     0,   474,   475,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
       0,   457,   458,   459,     0,   366,     0,     0,     0,     0,
       0,     0,   460,     0,     0,  1104,     0,     0,     0,   433,
       0,     0,     0,     0,     0,  1206,     0,     0,     0,  1149,
    1151,  1156,  1159,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1119,     0,     0,     0,     0,     0,     0,     0,
       0,  1220,  1221,     0,  1222,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1233,     0,     0,  1236,
       0,  1237,     0,  1238,     0,     0,     0,   434,     0,     0,
       0,   435,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   276,   276,     0,     0,     0,     0,   276,   276,
       0,     0,   276,   276,     0,     0,     0,     0,     0,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   704,   450,   451,   452,   453,   454,
       0,   456,     0,   642,   457,   458,   459,     0,   996,   997,
     644,   998,     0,     0,     0,   460,   999,     0,     0,     0,
       0,   276,     0,     0,   706,   276,   276,   276,   276,     0,
       0,     0,     0,     0,     0,     0,  1000,     0,     0,     0,
    1001,     0,     0,  1002,     0,   649,     0,   276,   276,     0,
     276,     0,   508,   707,     0,     0,     0,     0,     0,   508,
       0,  1003,   276,     0,     0,   276,     0,   276,  1004,   276,
       0,     0,     0,     0,     0,     0,     0,     0,  1005,     0,
       0,     0,     0,     0,     0,     0,     0,  1006,  1007,     0,
    1008,     0,  1009,     0,   708,     0,     0,     0,     0,     0,
     709,  1010,     0,     0,  1011,     0,     0,  1012,     0,     0,
       0,     0,  1278,     0,     0,     0,   507,   462,   463,   464,
     465,   466,     0,     0,   469,   470,   471,   472,     0,   474,
     475,     0,   657,   658,   659,   660,   661,   662,     0,   663,
       0,     0,     0,   664,   665,   666,   667,   668,   669,   670,
     671,   672,   673,     0,   674,     0,     0,   675,   676,   677,
     678,  1013,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     508,   508,     0,     0,   508,   508,   508,     0,   508,    -2,
       4,     0,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -214,    21,    22,    23,    24,    25,    26,
       0,  -214,    27,     0,    28,     0,    29,    30,    31,  -214,
    -214,  -214,    32,    33,    34,   -69,     0,    35,    36,    37,
       0,    38,  -356,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -356,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -356,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -69,    84,    85,     0,     0,  -214,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -214,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,  -704,     0,    13,    14,
      15,    16,    17,  -704,    18,     0,    19,    20,  -214,    21,
      22,    23,    24,    25,    26,  -704,  -214,    27,  -704,    28,
       0,    29,    30,    31,  -214,  -214,  -214,    32,    33,    34,
     -69,     0,    35,    36,    37,     0,    38,  -356,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -69,
      48,    49,     0,    50,    51,    52,     0,  -356,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -356,   -69,    60,
      61,    62,    63,  -704,    64,    65,    66,  -704,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -69,    84,
      85,     0,     0,  -214,     0,    86,     0,     0,  -704,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,  -704,    91,  -704,  -704,
    -704,  -704,  -704,  -704,  -704,     0,  -704,  -704,  -704,  -704,
    -704,  -704,  -704,  -704,  -704,  -704,  -704,  -704,    98,    99,
    -704,  -704,  -704,     0,   101,  -704,   102,     0,  -214,     0,
     348,  -704,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -214,    21,    22,    23,    24,    25,    26,
       0,  -214,    27,     0,    28,     0,    29,    30,    31,  -214,
    -214,  -214,    32,    33,    34,   -69,     0,    35,    36,    37,
       0,    38,  -356,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -356,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -356,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -69,    84,    85,     0,     0,  -214,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
     349,   102,     0,  -214,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -214,    21,
      22,    23,    24,    25,    26,     0,  -214,    27,     0,    28,
       0,    29,    30,    31,  -214,  -214,  -214,    32,    33,    34,
     -69,     0,    35,    36,    37,     0,    38,  -356,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -69,
      48,    49,     0,    50,    51,    52,     0,  -356,     0,     0,
      53,    54,    55,    56,    57,    58,    59,  -356,   -69,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -69,    84,
      85,     0,     0,  -214,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,   566,   102,     0,  -214,     0,
     584,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -214,    21,    22,    23,    24,    25,    26,
       0,  -214,    27,     0,    28,     0,    29,    30,    31,  -214,
    -214,  -214,    32,    33,    34,   -69,     0,    35,    36,    37,
       0,    38,  -356,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -356,     0,     0,    53,    54,    55,    56,    57,
      58,    59,  -356,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -69,    84,    85,     0,     0,  -214,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
     585,   102,     0,  -214,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -214,    21,
      22,    23,    24,    25,    26,     0,  -214,    27,     0,    28,
       0,    29,    30,    31,  -214,  -214,  -214,    32,    33,    34,
     -69,     0,    35,    36,    37,     0,    38,  -356,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -69,
      48,    49,     0,    50,    51,    52,     0,  -356,     0,     0,
      53,    54,    55,    56,    57,    58,    59,  -356,   -69,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -69,    84,
      85,     0,     0,  -214,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,   821,   102,     0,  -214,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -214,    21,    22,    23,    24,    25,    26,
       0,  -214,    27,     0,    28,     0,    29,    30,    31,  -214,
    -214,  -214,    32,    33,    34,   -69,     0,    35,    36,    37,
       0,    38,  -356,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -356,     0,     0,    53,    54,    55,    56,   359,
      58,    59,  -356,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -69,    84,    85,     0,     0,  -214,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -214,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -214,    21,
      22,    23,    24,    25,    26,     0,  -214,    27,     0,    28,
       0,    29,    30,    31,  -214,  -214,  -214,    32,    33,    34,
     -69,     0,    35,    36,    37,     0,    38,  -356,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -69,
      48,    49,     0,    50,    51,    52,     0,  -356,     0,     0,
      53,    54,    55,    56,    57,    58,    59,  -356,   -69,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -69,    84,
      85,     0,     0,  -214,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,  -214,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -214,    21,    22,    23,    24,    25,    26,
       0,  -214,    27,     0,    28,     0,    29,    30,    31,  -214,
    -214,  -214,    32,    33,    34,   -69,     0,    35,    36,    37,
       0,    38,  -356,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -356,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -356,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -69,    84,    85,     0,     0,  -214,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -214,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -214,    21,
      22,    23,    24,    25,    26,     0,  -214,    27,     0,    28,
       0,    29,    30,    31,  -214,  -214,  -214,    32,  1035,    34,
     -69,     0,    35,    36,    37,     0,    38,  -356,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -69,
      48,    49,     0,    50,    51,    52,     0,  -356,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -356,   -69,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -69,    84,
      85,     0,     0,  -214,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,  -214,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -214,    21,    22,    23,    24,    25,    26,
       0,  -214,    27,     0,    28,     0,    29,    30,    31,  -214,
    -214,  -214,    32,  1041,    34,   -69,     0,    35,    36,    37,
       0,    38,  -356,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -356,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -356,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -69,    84,    85,     0,     0,  -214,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -214,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -214,    21,
      22,    23,    24,    25,    26,     0,  -214,    27,     0,    28,
       0,    29,    30,    31,  -214,  -214,  -214,    32,  1224,    34,
     -69,     0,    35,    36,    37,     0,    38,  -356,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -69,
      48,    49,     0,    50,    51,    52,     0,  -356,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -356,   -69,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -69,    84,
      85,     0,     0,  -214,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,  -214,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -214,    21,    22,    23,    24,    25,    26,
       0,  -214,    27,     0,    28,     0,    29,    30,    31,  -214,
    -214,  -214,    32,  1226,    34,   -69,     0,    35,    36,    37,
       0,    38,  -356,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -356,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -356,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -69,    84,    85,     0,     0,  -214,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -214,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -214,    21,
      22,    23,    24,    25,    26,     0,  -214,    27,     0,    28,
       0,    29,    30,    31,  -214,  -214,  -214,    32,  1231,    34,
     -69,     0,    35,    36,    37,     0,    38,  -356,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -69,
      48,    49,     0,    50,    51,    52,     0,  -356,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -356,   -69,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -69,    84,
      85,     0,     0,  -214,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,  -214,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -214,    21,    22,    23,    24,    25,    26,
       0,  -214,    27,     0,    28,     0,    29,    30,    31,  -214,
    -214,  -214,    32,  1234,    34,   -69,     0,    35,    36,    37,
       0,    38,  -356,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -356,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -356,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -69,    84,    85,     0,     0,  -214,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -214,     0,     4,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,    14,
      15,    16,    17,     0,    18,     0,    19,    20,  -214,    21,
      22,    23,    24,    25,    26,     0,  -214,    27,     0,    28,
       0,    29,    30,    31,  -214,  -214,  -214,    32,  1239,    34,
     -69,     0,    35,    36,    37,     0,    38,  -356,     0,    39,
      40,    41,    42,    43,     0,    44,    45,    46,    47,   -69,
      48,    49,     0,    50,    51,    52,     0,  -356,     0,     0,
      53,    54,    55,    56,     0,    58,    59,  -356,   -69,    60,
      61,    62,    63,     0,    64,    65,    66,     0,    67,    68,
      69,    70,    71,    72,    73,    74,     0,    75,    76,     0,
      77,    78,    79,    80,    81,     0,    82,    83,   -69,    84,
      85,     0,     0,  -214,     0,    86,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,    99,
       0,     0,   100,     0,   101,     0,   102,     0,  -214,     0,
       4,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,    15,    16,    17,     0,    18,
       0,    19,    20,  -214,    21,    22,    23,    24,    25,    26,
       0,  -214,    27,     0,    28,     0,    29,    30,    31,  -214,
    -214,  -214,    32,  1243,    34,   -69,     0,    35,    36,    37,
       0,    38,  -356,     0,    39,    40,    41,    42,    43,     0,
      44,    45,    46,    47,   -69,    48,    49,     0,    50,    51,
      52,     0,  -356,     0,     0,    53,    54,    55,    56,     0,
      58,    59,  -356,   -69,    60,    61,    62,    63,     0,    64,
      65,    66,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,    78,    79,    80,    81,
       0,    82,    83,   -69,    84,    85,     0,     0,  -214,     0,
      86,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,    99,     0,     0,   100,     0,   101,
       0,   102,     0,  -214,     0,   894,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,   295,     0,   197,
       0,     0,     0,    31,  -214,  -214,     0,     0,   198,    34,
       0,     0,     0,   297,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,   302,    71,    72,    73,   303,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,  -214,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,   642,     0,    13,     0,     0,    16,    17,   644,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,   649,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,   507,   462,   463,   464,   465,   466,
       0,     0,   469,   470,   471,   472,     0,   474,   475,     0,
     832,   833,   834,   835,   836,   662,     0,   663,     0,    94,
       0,   664,   665,   666,   667,   668,   669,   670,   837,   672,
     673,    96,   838,    98,   -93,   675,   676,   839,   678,   202,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,   642,     0,    13,     0,
       0,    16,    17,   644,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,   649,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,   507,
     462,   463,   464,   465,   466,     0,     0,   469,   470,   471,
     472,     0,   474,   475,     0,   832,   833,   834,   835,   836,
     662,     0,   663,     0,    94,     0,   664,   665,   666,   667,
     668,   669,   670,   837,   672,   673,    96,   838,    98,     0,
     675,   676,   839,   678,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,    14,     0,    16,    17,     0,    18,
       0,   196,    20,     0,    21,     0,     0,     0,     0,    26,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,    33,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
      44,    45,    46,    47,     0,    48,    49,     0,    50,    51,
      52,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,    67,    68,    69,    70,    71,    72,    73,
      74,     0,    75,    76,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   101,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
     381,    23,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,    55,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,    64,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,    84,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,  -748,     0,
       0,     0,  -748,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,  -398,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,  -398,     0,     0,   100,     0,   202,
    -398,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,   317,   318,     0,    81,   353,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,   354,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,   384,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,    19,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,  -214,
    -214,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,    63,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,    82,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,  -214,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,   746,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,   747,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,   748,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,   749,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,   317,   318,     0,    81,
     353,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,   808,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,   317,   318,     0,    81,   353,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,   810,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,   317,   318,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,   319,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,   317,   318,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,  -549,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,  -214,
    -214,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,  -214,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,   317,   318,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,  -549,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,   317,   318,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,   816,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,   964,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,   317,   318,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,  -406,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,  -406,     0,     0,   100,     0,   202,
    -406,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,  -391,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,  -391,
       0,     0,   100,     0,   202,  -391,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,  -399,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,  -399,     0,     0,   100,     0,   202,
    -399,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,   257,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,   258,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,   317,   318,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,  -748,     0,     0,     0,  -748,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,   603,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,   604,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,   604,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,   613,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,  -427,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,  -427,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,   275,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   101,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,   604,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,   613,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
     806,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,  1017,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,  1054,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,  1059,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
    1212,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,   198,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,   295,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,   198,    34,     0,     0,     0,   297,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,   302,    71,    72,    73,
     303,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,  1084,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,  1087,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,  1267,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,  1268,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,  1270,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,  1271,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,    96,    97,    98,     0,     0,     0,   100,     0,   202,
       0,   102,     0,     0,     0,     0,   103,     5,     6,     7,
       8,     9,    10,    11,    12,     0,     0,     0,    13,     0,
       0,    16,    17,     0,    18,     0,   196,     0,     0,    21,
       0,     0,     0,     0,     0,     0,     0,    27,     0,   197,
       0,     0,     0,    31,     0,     0,     0,     0,  1272,    34,
       0,     0,     0,    36,     0,     0,    38,     0,     0,   199,
       0,     0,    42,    43,     0,     0,    45,     0,    47,     0,
      48,    49,     0,    50,    51,     0,     0,     0,     0,     0,
       0,    54,     0,    56,     0,    58,     0,     0,     0,     0,
      61,    62,   200,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,   201,     0,    75,     0,     0,
      77,     0,     0,     0,    81,     0,     0,    83,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,    87,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    89,    90,    91,    92,    93,
       0,     0,     0,     0,    94,     0,     0,     0,     0,     0,
       0,     0,     0,    95,     0,     0,    96,    97,    98,     0,
       0,     0,   100,     0,   202,     0,   102,     0,     0,     0,
       0,   103,     5,     6,     7,     8,     9,    10,    11,    12,
       0,     0,     0,    13,     0,     0,    16,    17,     0,    18,
       0,   196,     0,     0,    21,     0,     0,     0,     0,     0,
       0,     0,    27,     0,   197,     0,     0,     0,    31,     0,
       0,     0,     0,  1273,    34,     0,     0,     0,    36,     0,
       0,    38,     0,     0,   199,     0,     0,    42,    43,     0,
       0,    45,     0,    47,     0,    48,    49,     0,    50,    51,
       0,     0,     0,     0,     0,     0,    54,     0,    56,     0,
      58,     0,     0,     0,     0,    61,    62,   200,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
     201,     0,    75,     0,     0,    77,     0,     0,     0,    81,
       0,     0,    83,     0,     0,    85,     0,     0,     0,     0,
       0,     0,     0,    87,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      89,    90,    91,    92,    93,     0,     0,     0,     0,    94,
       6,     7,     8,     9,    10,    11,    12,     0,    95,     0,
      13,    96,    97,    98,    17,     0,     0,   100,   196,   202,
       0,   102,     0,   294,     0,     0,   103,     0,     0,     0,
       0,   197,     0,     0,     0,     0,  -214,  -214,     0,     0,
     198,     0,     0,   296,     0,     0,     0,   298,     0,     0,
       0,   199,     0,     0,    42,    43,     0,     0,     0,     0,
      47,     0,    48,    49,     0,     0,     0,     0,     0,     0,
       0,   299,   433,    54,   300,    56,    57,    58,     0,     0,
       0,     0,    61,     0,   200,     0,   301,     0,     0,     0,
       0,     0,    69,     0,    71,     0,    73,     0,     0,     0,
       0,     0,     0,     0,     0,   304,     0,     0,     0,    83,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     434,     0,     0,     0,   435,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,    94,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,    96,    97,
      98,     0,     0,     0,   100,     0,   202,     0,   102,  -360,
    -214,     0,     0,   103,     6,     7,     8,     9,    10,    11,
      12,     0,   437,   438,    13,   439,   440,   441,    17,   442,
     443,   444,   196,   445,   446,   447,   448,   294,   450,   451,
     452,   453,   454,     0,   456,   197,     0,   457,   458,   459,
    -214,  -214,     0,     0,   198,     0,     0,   296,   460,     0,
       0,   298,     0,     0,     0,   199,     0,     0,    42,    43,
       0,     0,     0,     0,    47,     0,    48,    49,     0,     0,
       0,     0,     0,     0,     0,   299,     0,    54,   300,    56,
      57,    58,     0,     0,     0,     0,    61,     0,   200,     0,
     301,     0,     0,     0,     0,     0,    69,     0,    71,     0,
      73,     0,     0,     0,     0,     0,     0,     0,     0,   304,
       0,     0,     0,    83,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   640,     0,   641,
       0,    89,    90,    91,    92,    93,     0,     0,   642,     0,
      94,     0,     0,   643,   231,   644,   645,     0,     0,    95,
       0,   646,    96,    97,    98,     0,     0,     0,   100,   234,
     202,   197,   102,     0,  -214,   235,     0,   103,     0,     0,
       0,   647,     0,     0,     0,   237,     0,     0,   648,     0,
     649,     0,     0,     0,     0,     0,     0,     0,   650,     0,
       0,     0,     0,     0,     0,   240,   651,     0,     0,     0,
       0,     0,     0,   242,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   652,     0,     0,     0,     0,     0,     0,
       0,     0,   244,   245,     0,   653,     0,   247,     0,   654,
       0,     0,   249,     0,     0,     0,   655,     0,     0,   251,
       0,     0,   656,     0,     0,     0,     0,     0,     0,     0,
       0,   507,   462,   463,   464,   465,   466,     0,     0,   469,
     470,   471,   472,     0,   474,   475,     0,   657,   658,   659,
     660,   661,   662,     0,   663,     0,     0,     0,   664,   665,
     666,   667,   668,   669,   670,   671,   672,   673,     0,   674,
       0,     0,   675,   676,   677,   678,     0,     0,   679,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   196,     0,
       0,    21,     0,     0,     0,     0,     0,     0,     0,    27,
       0,   197,     0,     0,     0,    31,  -214,  -214,     0,     0,
     198,    34,     0,     0,     0,    36,     0,     0,    38,     0,
       0,   199,     0,     0,    42,    43,     0,     0,    45,     0,
      47,     0,    48,    49,     0,    50,    51,     0,     0,     0,
       0,     0,     0,    54,     0,    56,     0,    58,     0,     0,
       0,     0,    61,    62,   200,     0,     0,     0,     0,     0,
       0,     0,    69,    70,    71,    72,    73,   201,     0,    75,
       0,     0,    77,     0,     0,     0,    81,     0,     0,    83,
       0,     0,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    89,    90,    91,
      92,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    95,     0,     0,     0,    97,
      98,     0,     0,     0,   100,     0,   202,     0,   102,     0,
    -214,     5,     6,     7,     8,     9,    10,    11,    12,     0,
       0,     0,    13,     0,     0,    16,    17,     0,    18,     0,
     196,     0,     0,    21,     0,     0,     0,     0,     0,     0,
       0,    27,     0,   197,     0,     0,     0,    31,  -214,  -214,
       0,     0,   198,    34,     0,     0,     0,    36,     0,     0,
      38,     0,     0,   199,     0,     0,    42,    43,     0,     0,
      45,     0,    47,     0,    48,    49,     0,    50,    51,     0,
       0,     0,     0,     0,     0,    54,     0,    56,     0,    58,
       0,     0,     0,     0,    61,    62,   200,     0,     0,     0,
       0,     0,     0,     0,    69,    70,    71,    72,    73,   201,
       0,    75,     0,     0,    77,     0,     0,     0,    81,     0,
       0,    83,     0,     0,    85,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
       0,    91,     0,     0,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,    98,   196,     0,     0,    21,     0,   202,     0,
     102,     0,  -214,     0,    27,     0,   197,     0,     0,     0,
      31,  -214,  -214,     0,     0,   198,    34,     0,     0,     0,
      36,     0,     0,    38,     0,     0,   199,     0,     0,    42,
      43,     0,     0,    45,     0,    47,     0,    48,    49,     0,
      50,    51,     0,     0,     0,     0,     0,     0,    54,     0,
      56,     0,    58,     0,     0,     0,     0,    61,    62,   200,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,   201,     0,    75,     0,     0,    77,     0,     0,
     229,    81,     0,     0,    83,     0,     0,    85,     0,   642,
       0,     0,     0,     0,   230,   231,   644,   232,     0,     0,
       0,     0,   233,     0,     0,     0,     0,     0,     0,     0,
     234,     0,    89,     0,    91,     0,   235,     0,     0,     0,
       0,     0,   236,     0,     0,     0,   237,     0,     0,   238,
       0,   649,     0,     0,     0,    98,     0,     0,     0,   239,
       0,     0,     0,   102,     0,  -214,   240,   241,     0,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,     0,     0,     0,     0,     0,
     431,     0,     0,   244,   245,     0,   246,   432,   247,     0,
     248,     0,     0,   249,     0,     0,     0,   250,     0,   433,
     251,     0,     0,   252,     0,     0,     0,     0,     0,     0,
       0,     0,   507,   462,   463,   464,   465,   466,     0,     0,
     469,   470,   471,   472,     0,   474,   475,     0,   657,   658,
     659,   660,   661,   662,     0,   663,     0,     0,     0,   664,
     665,   666,   667,   668,   669,   670,   671,   672,   673,     0,
     674,     0,     0,   675,   676,   677,   678,   434,     0,     0,
     431,   435,     0,     0,     0,     0,     0,   432,     0,     0,
    1223,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   366,     0,
     431,   435,     0,     0,     0,   460,     0,   432,     0,     0,
    1230,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   366,     0,
     431,   435,     0,     0,     0,   460,     0,   432,     0,     0,
    1314,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   366,     0,
     431,   435,     0,     0,     0,   460,     0,   432,     0,     0,
    1315,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   366,     0,
     431,   435,     0,     0,     0,   460,     0,   432,     0,     0,
    1316,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   366,     0,
     431,   435,     0,     0,     0,   460,     0,   432,     0,     0,
    1317,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   366,     0,
     431,   435,     0,     0,     0,   460,     0,   432,     0,     0,
    1318,     0,     0,     0,     0,     0,     0,     0,     0,   433,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,     0,
       0,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,   434,   366,     0,
       0,   435,     0,     0,     0,   460,     0,     0,     0,     0,
    1319,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     431,     0,     0,     0,     0,     0,     0,   432,     0,     0,
       0,     0,   436,   507,   462,   463,   464,   465,   466,   433,
     275,   469,   470,   471,   472,     0,   474,   475,     0,   437,
     438,     0,   439,   440,   441,   869,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,     0,     0,   457,   458,   459,     0,   366,     0,
       0,     0,     0,     0,     0,   460,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   431,   434,     0,     0,
       0,   435,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   275,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   860,     0,
       0,   989,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   431,   434,   457,   458,   459,   435,   366,   432,
       0,     0,     0,     0,     0,   460,     0,     0,     0,     0,
       0,   433,   857,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   860,     0,     0,   858,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   431,   434,
     457,   458,   459,   435,   366,   432,     0,     0,     0,     0,
       0,   460,     0,     0,     0,     0,     0,   433,   275,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   988,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   431,   434,   457,   458,   459,   435,
     366,   432,     0,     0,     0,     0,     0,   460,     0,     0,
       0,     0,     0,   433,  1158,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
     431,   434,   457,   458,   459,   435,   366,   432,     0,     0,
       0,     0,     0,   460,     0,     0,     0,     0,     0,   433,
     275,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   860,     0,     0,     0,   436,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   431,   434,   457,   458,
     459,   435,   366,   432,     0,     0,     0,     0,     0,   460,
       0,     0,     0,     0,     0,   433,   275,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   860,     0,
       0,     0,   436,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   437,
     438,     0,   439,   440,   441,     0,   442,   443,   444,     0,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   431,   434,   457,   458,   459,   435,   366,   432,
       0,     0,     0,     0,     0,   460,     0,     0,     0,     0,
       0,   433,   920,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   921,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   431,   434,
     457,   458,   459,   435,   366,   432,     0,     0,     0,     0,
       0,   460,     0,     0,     0,     0,     0,   433,   922,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   923,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   431,   434,   457,   458,   459,   435,
       0,   432,     0,     0,     0,     0,     0,   460,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   434,   457,   458,   459,   435,     0,     0,     0,     0,
       0,     0,     0,   460,   955,     0,   431,     0,     0,     0,
       0,     0,     0,   432,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   433,   436,     0,     0,     0,
    1131,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,  1132,     0,   457,   458,
     459,     0,   366,     0,     0,     0,     0,     0,     0,   460,
       0,     0,   431,   434,     0,     0,     0,   435,     0,   432,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   433,  1150,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   437,   438,     0,   439,   440,
     441,     0,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,   431,   434,
     457,   458,   459,   435,     0,   432,     0,     0,     0,     0,
       0,   460,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   436,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   437,   438,     0,   439,   440,   441,     0,   442,   443,
     444,     0,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   431,   434,   457,   458,   459,   435,
     366,   432,     0,     0,     0,     0,     0,   460,     0,     0,
       0,     0,     0,   433,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   860,     0,     0,     0,
     436,     0,     0,     0,   431,   533,     0,     0,     0,     0,
       0,   432,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,   433,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   434,   457,   458,   459,   435,     0,     0,     0,   431,
       0,  1168,     0,   460,     0,     0,   432,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   433,     0,
       0,     0,   860,     0,     0,     0,   436,     0,     0,     0,
       0,   434,     0,     0,     0,   435,     0,     0,     0,     0,
       0,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   436,     0,   457,   458,
     459,     0,     0,     0,     0,     0,   434,  1210,     0,   460,
     435,     0,     0,   437,   438,     0,   439,   440,   441,     0,
     442,   443,   444,     0,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   431,     0,   457,   458,
     459,   436,     0,   432,     0,     0,     0,     0,     0,   460,
       0,     0,     0,     0,     0,   433,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   431,     0,   457,   458,   459,     0,   557,   432,     0,
       0,     0,     0,     0,   460,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   434,     0,     0,     0,   435,     0,     0,
       0,   431,     0,     0,     0,     0,     0,     0,   432,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,   436,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,   437,   438,     0,   439,   440,
     441,   740,   442,   443,   444,     0,   445,   446,   447,   448,
     449,   450,   451,   452,   453,   454,   455,   456,     0,   561,
     457,   458,   459,   436,     0,     0,     0,     0,   434,     0,
       0,   460,   435,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   436,     0,   457,   458,   459,     0,     0,
       0,   431,   824,     0,     0,     0,   460,     0,   432,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
     433,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,     0,
       0,     0,   807,     0,     0,     0,   460,   431,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,     0,     0,   431,     0,     0,
       0,     0,     0,     0,   432,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   433,     0,     0,   953,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   434,     0,     0,     0,   435,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   825,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,   881,   436,
       0,     0,     0,   431,   434,     0,   460,     0,   435,     0,
     432,     0,     0,     0,     0,     0,   437,   438,     0,   439,
     440,   441,   433,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   436,
       0,   457,   458,   459,     0,     0,     0,     0,     0,     0,
       0,     0,   460,     0,     0,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   431,
     434,   457,   458,   459,   435,     0,   432,     0,     0,     0,
       0,     0,   460,     0,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,   431,
    1023,     0,     0,     0,     0,     0,   432,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,   433,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,   434,   457,   458,   459,
     435,     0,     0,     0,   967,   431,     0,     0,   460,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,   434,     0,     0,     0,
     435,     0,     0,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   436,     0,   457,   458,   459,     0,     0,     0,     0,
     968,     0,   434,     0,   460,     0,   435,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   431,  1128,   457,   458,   459,     0,   436,   432,     0,
       0,     0,     0,     0,   460,     0,     0,     0,     0,     0,
     433,     0,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,     0,   457,
     458,   459,     0,     0,     0,   431,  1078,     0,     0,     0,
     460,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,     0,     0,     0,   434,     0,
       0,     0,   435,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   431,     0,     0,     0,     0,
       0,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,   434,   439,   440,   441,   435,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,     0,     0,   457,   458,   459,     0,     0,
       0,     0,     0,   214,     0,     0,   460,   436,     0,     0,
       0,     0,   434,     0,     0,   431,   435,     0,     0,     0,
       0,     0,   432,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   433,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   436,     0,   457,
     458,   459,     0,     0,     0,     0,     0,     0,     0,     0,
     460,     0,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   433,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,  1144,   457,
     458,   459,   434,     0,     0,   431,   435,     0,     0,     0,
     460,     0,   432,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   433,  1191,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1171,   436,     0,     0,
       0,     0,   434,     0,     0,   431,   435,     0,     0,     0,
       0,     0,   432,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   433,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,     0,   457,
     458,   459,   434,     0,     0,   431,   435,     0,     0,     0,
     460,     0,   432,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   433,  1194,   446,   447,   448,     0,
     450,   451,   452,   453,     0,     0,   456,   436,     0,   457,
     458,   459,   434,     0,     0,     0,   435,     0,     0,     0,
     460,     0,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   436,     0,   457,
     458,   459,   434,     0,     0,   431,   435,     0,     0,     0,
     460,     0,   432,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   433,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   436,     0,   457,
     458,   459,     0,     0,     0,   431,  1218,     0,  1196,     0,
     460,     0,   432,  1259,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,   433,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,     0,     0,   457,
     458,   459,   434,     0,     0,     0,   435,     0,     0,     0,
     460,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,   431,   434,     0,     0,     0,   435,     0,   432,     0,
       0,     0,     0,     0,   437,   438,     0,   439,   440,   441,
     433,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   436,     0,   457,
     458,   459,     0,     0,     0,     0,     0,     0,     0,     0,
     460,     0,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   431,   434,   457,
     458,   459,   435,     0,   432,     0,     0,     0,     0,     0,
     460,  1266,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   431,   434,   457,   458,   459,   435,     0,
     432,     0,     0,     0,     0,     0,   460,  1269,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   437,   438,     0,   439,
     440,   441,     0,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,   431,
     434,   457,   458,   459,   435,     0,   432,     0,     0,     0,
       0,     0,   460,  1320,     0,     0,     0,     0,   433,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   431,   434,   457,   458,   459,
     435,     0,   432,     0,     0,     0,     0,     0,   460,  1321,
       0,     0,     0,     0,   433,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   436,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   437,   438,
       0,   439,   440,   441,     0,   442,   443,   444,     0,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   455,
     456,   431,   434,   457,   458,   459,   435,     0,   432,     0,
       0,     0,     0,     0,   460,  1322,     0,     0,     0,     0,
     433,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   436,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   437,   438,     0,   439,   440,   441,
       0,   442,   443,   444,     0,   445,   446,   447,   448,   449,
     450,   451,   452,   453,   454,   455,   456,   431,   434,   457,
     458,   459,   435,     0,   432,     0,     0,     0,     0,     0,
     460,  1323,     0,     0,     0,     0,   433,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     437,   438,     0,   439,   440,   441,     0,   442,   443,   444,
       0,   445,   446,   447,   448,   449,   450,   451,   452,   453,
     454,   455,   456,   431,   434,   457,   458,   459,   435,     0,
     432,     0,     0,     0,     0,     0,   460,  1324,     0,     0,
       0,     0,   433,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   436,
       0,     0,     0,   431,     0,     0,     0,     0,     0,     0,
     432,     0,     0,     0,     0,     0,   437,   438,     0,   439,
     440,   441,   433,   442,   443,   444,     0,   445,   446,   447,
     448,   449,   450,   451,   452,   453,   454,   455,   456,     0,
     434,   457,   458,   459,   435,     0,     0,     0,   431,     0,
       0,     0,   460,  1325,     0,   432,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   433,     0,     0,
       0,     0,     0,     0,     0,   436,     0,     0,     0,     0,
     434,     0,     0,     0,   435,     0,     0,     0,     0,     0,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,   436,     0,   457,   458,   459,
       0,     0,     0,     0,     0,   943,     0,     0,   460,   435,
       0,     0,   437,   438,     0,   439,   440,   441,     0,   442,
     443,   444,     0,   445,   446,   447,   448,   449,   450,   451,
     452,   453,   454,   455,   456,     0,     0,   457,   458,   459,
     436,     0,     0,     0,     0,     0,     0,     0,   460,     0,
       0,     0,     0,     0,     0,     0,     0,   437,   438,     0,
     439,   440,   441,     0,   442,   443,   444,     0,   445,   446,
     447,   448,   449,   450,   451,   452,   453,   454,   455,   456,
       0,   361,   457,   458,   459,    22,    23,     0,     0,     0,
       0,     0,     0,   460,   362,     0,    29,   363,     0,     0,
       0,     0,    32,     0,     0,     0,     0,     0,     0,    37,
       0,     0,     0,     0,   364,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   361,     0,     0,
       0,    22,    23,     0,     0,    53,     0,    55,     0,    57,
     362,  1063,    29,   363,  1064,     0,     0,   365,    32,    64,
       0,     0,     0,     0,     0,    37,     0,     0,     0,     0,
     364,     0,     0,     0,     0,     0,     0,     0,    80,     0,
       0,    82,     0,     0,    84,     0,     0,     0,     0,     0,
       0,    53,     0,    55,     0,    57,     0,  1063,     0,     0,
    1064,     0,     0,   365,     0,    64,     0,     0,     0,    88,
     361,     0,     0,     0,    22,    23,     0,     0,     0,     0,
       0,     0,     0,   362,    80,    29,   363,    82,     0,     0,
      84,    32,     0,     0,    99,     0,     0,     0,    37,     0,
    1139,  -356,     0,   364,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -69,     0,    88,     0,     0,     0,     0,
       0,  -356,     0,     0,    53,     0,    55,     0,     0,     0,
      59,  -356,   -69,    60,     0,     0,   365,   361,    64,     0,
      99,    22,    23,     0,     0,     0,  1219,     0,     0,     0,
     362,     0,    29,   363,     0,     0,     0,    80,    32,     0,
      82,     0,     0,    84,     0,    37,     0,     0,     0,     0,
     364,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   361,     0,     0,     0,    22,    23,     0,
       0,    53,     0,    55,     0,   359,   362,  1063,    29,   363,
    1064,     0,     0,   365,    32,    64,     0,     0,     0,     0,
       0,    37,     0,    99,     0,     0,   364,     0,   366,     0,
       0,     0,     0,     0,    80,     0,     0,    82,     0,     0,
      84,     0,     0,     0,     0,     0,     0,    53,     0,    55,
       0,     0,     0,  1063,     0,   402,  1064,   229,     0,   365,
       0,    64,     0,     0,     0,    88,     0,     0,     0,     0,
       0,   230,   231,     0,   232,     0,     0,     0,     0,   233,
      80,     0,     0,    82,     0,     0,    84,   234,     0,     0,
      99,     0,     0,   235,     0,     0,     0,     0,     0,   236,
       0,     0,     0,   237,     0,     0,   238,     0,     0,     0,
       0,    88,   229,     0,     0,     0,   239,     0,     0,     0,
       0,     0,     0,   240,   241,     0,   230,   231,     0,   232,
       0,   242,     0,     0,   233,     0,    99,     0,     0,     0,
       0,   243,   234,     0,     0,     0,     0,     0,   235,     0,
     244,   245,     0,   246,   236,   247,     0,   248,   237,     0,
     249,   238,     0,     0,   250,     0,     0,   251,     0,     0,
     252,   239,     0,     0,     0,     0,     0,     0,   240,   241,
       0,     0,     0,     0,     0,     0,   242,   229,     0,     0,
       0,     0,     0,     0,    88,     0,   243,     0,     0,     0,
       0,   230,   231,     0,   232,   244,   245,     0,   246,   233,
     247,    23,   248,     0,     0,   249,     0,   234,     0,   250,
       0,     0,   251,   235,     0,   252,     0,     0,     0,   236,
       0,     0,     0,   237,     0,     0,   238,     0,     0,     0,
       0,     0,   229,     0,     0,     0,   239,     0,     0,    88,
       0,     0,     0,   240,   241,     0,   230,   231,     0,   232,
       0,   242,    55,     0,   233,     0,     0,     0,     0,     0,
       0,   243,   234,     0,    64,     0,     0,     0,   235,     0,
     244,   245,     0,   246,   236,   247,     0,   248,   237,     0,
     249,   238,     0,     0,   250,     0,     0,   251,     0,    84,
     252,   239,     0,     0,     0,     0,     0,     0,   240,   241,
       0,     0,     0,     0,     0,     0,   242,  -331,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,     0,
       0,  -331,  -331,     0,  -331,   244,   245,     0,   246,  -331,
     247,     0,   248,     0,     0,   249,     0,  -331,     0,   250,
     546,     0,   251,  -331,     0,   252,     0,     0,     0,  -331,
       0,     0,     0,  -331,     0,     0,  -331,     0,     0,     0,
       0,     0,   229,     0,     0,     0,  -331,     0,     0,     0,
       0,     0,     0,  -331,  -331,     0,   230,   231,     0,   232,
       0,  -331,     0,     0,   233,     0,     0,     0,     0,     0,
       0,  -331,   234,     0,     0,     0,     0,     0,   235,     0,
    -331,  -331,     0,  -331,   236,  -331,     0,  -331,   237,     0,
    -331,   238,     0,     0,  -331,     0,     0,  -331,     0,     0,
    -331,   239,     0,     0,     0,     0,     0,     0,   240,   241,
       0,     0,     0,     0,     0,     0,   242,  -332,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,     0,
       0,  -332,  -332,     0,  -332,   244,   245,     0,   246,  -332,
     247,     0,   248,     0,     0,   249,     0,  -332,     0,   250,
       0,     0,   251,  -332,     0,   252,     0,     0,     0,  -332,
       0,     0,     0,  -332,     0,     0,  -332,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -332,     0,     0,     0,
       0,     0,     0,  -332,  -332,     0,     0,     0,     0,     0,
       0,  -332,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -332,     0,     0,     0,     0,     0,     0,     0,     0,
    -332,  -332,     0,  -332,     0,  -332,     0,  -332,     0,     0,
    -332,     0,     0,     0,  -332,     0,     0,  -332,     0,     0,
    -332
};

static const yytype_int16 yycheck[] =
{
       2,     2,    61,   153,   111,   488,    20,    26,   555,     2,
     593,   264,   166,   403,    61,   633,    66,    61,   559,   182,
     512,   222,   423,    25,   796,   636,   830,   550,   929,   132,
     385,   682,    25,   684,   825,   887,    86,   790,   291,   848,
       1,   582,     8,     1,    32,   408,    65,   969,     1,    47,
      28,    24,   102,     1,   398,    55,    32,   228,   893,    32,
      32,    39,    40,    41,    32,    32,    18,    40,    41,    42,
     147,  1185,    74,    32,   120,     1,    78,    79,   331,    32,
      36,    74,   102,    60,   525,    78,    79,    74,   607,    47,
     609,   610,   120,   264,    32,   614,   615,   260,   175,   101,
       0,    24,   120,   105,    92,    24,    32,    60,   101,    32,
      55,    47,   105,    32,    32,   123,    72,    40,    41,    42,
     291,    40,    41,    42,    92,    77,    74,    40,    41,   175,
     117,   165,     2,    47,    60,  1249,   971,    47,  1039,    92,
       3,   118,    47,     3,    96,     3,   119,   175,   120,   147,
     170,    55,   105,   120,    90,    25,   147,   175,    22,   777,
     331,   113,   170,    47,   125,   118,    92,   125,    55,   117,
     514,   147,   172,   151,  1026,   153,    90,   165,    35,   105,
     171,   544,   170,   149,   171,   146,   147,   165,   146,   147,
    1112,   963,   118,   146,   170,   636,   119,   969,   170,    27,
     119,   174,   170,   170,    74,   120,    27,   165,    78,    79,
     120,   170,   173,   215,  1066,  1067,  1025,   170,   759,   972,
     146,   199,   215,   171,    88,   478,   479,   172,     3,   147,
    1021,   101,   170,   125,    92,   105,   120,   147,    46,   165,
     103,   596,   147,   103,   170,   103,   829,    31,   112,   741,
      78,   174,   170,   110,    75,   174,   172,    78,   174,   151,
     423,   174,    90,   147,    53,   175,    46,    55,   270,    90,
     175,   794,   443,   275,    27,   520,    84,   270,    46,   257,
     258,   384,   275,   118,    73,    65,  1138,   115,   109,    55,
     141,   175,   140,   271,    83,   814,   403,    65,   149,   147,
     109,     3,   547,   147,    84,    89,   114,   478,   479,    93,
      89,   172,   147,     2,    93,    27,    84,   147,    20,    78,
      79,  1173,  1094,   171,   942,    78,  1130,  1228,  1132,   173,
     147,    33,   533,   311,   114,    47,    25,    90,   492,    51,
    1112,   512,    90,   173,    50,   215,   114,    53,   407,   351,
     351,   170,    11,   172,   171,   147,   109,   141,   351,   970,
      62,    55,   146,    75,   366,   149,    78,    73,    80,   153,
      76,   147,   172,   366,   376,   394,   147,    83,    90,  1179,
     399,   173,   147,   376,   362,    74,   364,   389,   559,    78,
      79,   147,   884,   147,    55,   171,   389,   109,   599,    55,
     270,   103,   173,   109,    55,   275,   147,   109,   173,   165,
     147,   582,   101,   147,   147,    27,   105,   467,   468,   173,
     398,   423,    31,   473,   147,   403,   476,   477,  1079,  1080,
    1013,   165,   173,    55,   926,    47,   173,   165,   147,    51,
     173,   165,   125,   171,   147,   423,   929,   147,   172,    82,
     173,   992,    85,  1253,  1254,  1255,  1256,  1257,  1258,    27,
      55,   173,   147,    75,   173,   147,    78,   486,    80,   152,
     173,   154,   155,   173,   157,   158,   147,   147,    90,    47,
      89,   351,   147,    51,    93,     8,   545,   147,   173,   147,
     571,   173,   573,   172,   165,   165,   366,   109,   545,   889,
     147,   545,   504,   904,   506,   165,   376,    75,   173,   487,
      78,   504,    80,   506,    55,   173,   100,   147,   165,   389,
       2,   571,    90,   573,  1043,  1044,   215,    66,  1047,  1048,
      55,   147,   141,  1052,  1053,   165,   514,   146,   516,   148,
     149,   109,   520,    25,   153,   172,  1129,    86,  1131,   165,
     147,   160,    55,    31,   147,   533,  1039,   166,   167,   168,
     579,   173,    22,   102,   542,    55,    26,    27,   165,   547,
     741,   147,   165,   147,    55,    35,   147,    37,    38,    55,
     147,   270,     3,    43,   586,   586,   275,   601,   759,   165,
      50,   165,    74,   586,   165,    55,    78,    79,   165,   172,
     165,   174,   165,  1122,  1123,  1124,  1125,   172,   171,   174,
     140,    89,   141,    27,   592,    93,    76,   146,    78,   101,
     149,   170,    89,   105,   153,   603,    93,    89,    88,    55,
      90,    93,   146,    47,   504,    89,   506,    51,  1185,    93,
      89,    89,    78,    79,    93,    93,   172,   165,   174,   109,
     292,   293,   112,   824,   825,   115,   172,   172,   174,   174,
     172,    75,   351,   141,    78,   172,    80,   174,   146,     3,
     148,   149,   172,   164,   174,   153,    90,   366,   165,   165,
    1070,   165,   160,    17,    18,   165,    20,   376,   165,   228,
     168,    25,   165,    55,   165,   109,   850,   172,   170,    33,
     389,   165,  1249,    84,   170,    39,   165,   757,   173,   172,
     688,    45,   147,   884,   172,    49,   586,   172,    52,   146,
     165,   172,   165,     8,     3,   264,   172,    65,    62,   170,
     780,   171,   125,   215,    90,    69,    70,   887,    90,   125,
     890,   904,   149,    77,    68,  1228,   125,   172,   147,   173,
     752,   125,   291,    87,   125,   926,    34,    34,    65,   125,
     173,   763,    96,    97,   766,    99,   146,   101,   173,   103,
     763,   173,   106,   766,    21,   170,   110,   111,   119,   113,
     149,   172,   116,   165,   147,   105,   173,   172,   270,   149,
     768,   149,   331,   275,   149,   149,   149,   165,   776,   149,
     149,   149,   149,   149,   149,   149,   149,   149,   786,   141,
     175,   172,    34,    34,   165,   504,   173,   506,   146,    47,
     147,   992,    34,    32,    32,   992,   804,    34,   887,   165,
     165,   170,   891,   263,   165,   165,   855,   146,   172,   165,
     173,   900,   171,   147,   165,   147,   110,   172,   165,   171,
     165,   581,   175,  1106,  1107,   857,   175,   175,   172,   172,
     822,   863,   173,   175,   857,   173,   165,   173,   173,   351,
     863,   849,   175,   875,   175,   165,  1026,   879,   164,   376,
     164,    24,   875,   368,   366,   393,   879,   959,   398,   339,
     984,   531,   545,   763,   376,   387,   766,   586,   904,   891,
    1249,   900,   305,   542,  1072,  1259,  1252,   389,   849,   528,
     688,   889,   130,    -1,   776,    -1,  1066,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   904,    -1,   467,   468,
      -1,    -1,    -1,    -1,   473,  1106,  1107,   476,   477,   478,
     479,   943,   944,    -1,    -1,    -1,    -1,    -1,   950,    -1,
     952,    -1,    -1,   955,    -1,    -1,    -1,   950,    -1,   952,
    1067,   980,   955,  1070,    -1,    -1,    -1,  1026,    -1,    -1,
      -1,    -1,    -1,   512,    -1,    -1,    -1,   979,    -1,    -1,
      -1,   959,    -1,    -1,  1147,    -1,   979,   857,  1138,   991,
      -1,  1154,    -1,   863,    -1,    -1,    -1,    -1,   991,    -1,
      -1,    -1,    -1,    -1,    -1,   875,   984,    -1,    -1,   879,
      -1,    -1,    -1,    -1,    -1,  1029,  1030,    -1,    -1,    -1,
     559,     2,   504,    -1,   506,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   571,    -1,   573,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   582,    25,    -1,    -1,    -1,    -1,  1051,
      -1,   391,   392,    -1,   394,  1057,    -1,    -1,  1051,    -1,
      -1,    -1,    -1,    -1,  1057,    -1,  1173,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   763,    -1,    -1,   766,    -1,  1138,
     950,    -1,   952,    -1,    -1,   955,    -1,    -1,    -1,    -1,
      -1,    -1,  1070,    74,   434,   435,    -1,    78,    79,  1101,
      -1,    -1,    -1,    -1,   586,    -1,    -1,    -1,  1101,   979,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     101,   991,  1285,  1286,   105,    -1,  1289,  1290,  1291,    -1,
    1293,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     480,   481,    -1,    -1,    -1,    -1,  1148,    -1,  1150,    -1,
      -1,    -1,    -1,  1155,    -1,  1148,  1158,  1150,    -1,    -1,
      -1,    -1,  1155,    -1,  1166,  1158,  1168,    -1,   857,    -1,
      -1,    -1,    -1,  1166,   863,  1168,    -1,    -1,    -1,    -1,
      -1,  1051,    -1,    -1,    -1,    -1,   875,  1057,    -1,    -1,
     879,    -1,    -1,    -1,    -1,    -1,    -1,  1199,    -1,    -1,
      -1,    -1,   741,  1205,    -1,    -1,  1199,  1209,  1210,    -1,
      -1,    -1,  1205,    -1,    -1,    -1,  1209,  1210,   757,    -1,
     759,  1223,    -1,    -1,    -1,    -1,    -1,    -1,  1230,    -1,
    1223,  1101,    -1,    -1,   215,    -1,    -1,  1230,   578,   579,
    1242,   780,    -1,    -1,  1246,    -1,    -1,    -1,    -1,  1242,
      -1,    -1,    -1,  1246,    -1,    -1,    -1,    -1,  1260,  1260,
      -1,   950,    -1,   952,    -1,    -1,   955,  1260,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1148,    -1,
    1150,   763,    -1,    -1,   766,  1155,    -1,    -1,  1158,   270,
     979,    -1,    -1,    -1,   275,    -1,  1166,    -1,  1168,    -1,
      -1,    -1,   991,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1314,  1315,  1316,  1317,  1318,  1319,    -1,    -1,
      -1,  1314,  1315,  1316,  1317,  1318,  1319,    -1,    -1,  1199,
      -1,    -1,    -1,    -1,    -1,  1205,    -1,    -1,     2,  1209,
    1210,    -1,    -1,    -1,    -1,   884,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1223,    -1,    -1,    -1,    -1,    -1,    -1,
    1230,    25,  1051,    -1,    -1,    -1,    -1,    -1,  1057,    -1,
     351,    -1,  1242,    -1,    -1,   857,  1246,    31,    -1,    -1,
      -1,   863,    -1,    -1,    -1,   366,    -1,   926,    -1,    -1,
    1260,    -1,    -1,   875,    -1,   376,    -1,   879,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   389,    -1,
      74,    -1,  1101,    -1,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     2,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,   101,    -1,    93,
      -1,   105,    -1,    -1,  1314,  1315,  1316,  1317,  1318,  1319,
      -1,    25,    -1,   992,    -1,    -1,    -1,    -1,    -1,  1148,
      -1,  1150,    -1,    -1,    -1,    -1,  1155,    -1,   950,  1158,
     952,    -1,    -1,   955,    -1,    -1,    -1,  1166,    -1,  1168,
      31,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,    -1,   979,    -1,   153,
      74,    -1,    -1,    -1,    78,    79,   160,    -1,    -1,   991,
    1199,   851,   166,   167,   168,    -1,  1205,    -1,   858,    -1,
    1209,  1210,    -1,   504,   864,   506,    -1,   101,    -1,   869,
      -1,   105,    -1,    -1,  1223,    -1,    -1,   877,    89,    -1,
      -1,  1230,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   215,    -1,  1242,    -1,    -1,    -1,  1246,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1106,  1107,  1051,
      -1,  1260,    -1,    -1,    -1,  1057,    -1,    -1,    -1,    -1,
      -1,   921,    -1,   923,   924,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,    -1,
      -1,    -1,   153,   943,   944,   586,   270,    -1,   159,   160,
      -1,   275,   163,    -1,    -1,   166,   167,   168,    -1,  1101,
      -1,    -1,     3,    -1,    -1,  1314,  1315,  1316,  1317,  1318,
    1319,    -1,    31,    -1,    -1,     2,    17,    -1,    -1,    20,
      -1,   215,    -1,    -1,    25,    -1,    -1,    -1,   988,   989,
     990,    -1,    -1,    -1,    35,    -1,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    45,    -1,  1148,    -1,  1150,    -1,
      -1,    52,    -1,  1155,    -1,    -1,  1158,    -1,    -1,    -1,
      -1,    62,    -1,    -1,  1166,    -1,  1168,   351,    -1,    70,
      89,    -1,    -1,    -1,    93,    -1,   270,    -1,    -1,    -1,
      -1,   275,   366,    -1,    -1,    -1,    87,    74,    -1,    -1,
      -1,    -1,   376,    -1,    -1,    -1,    -1,  1199,    99,    -1,
      -1,    -1,   103,  1205,    -1,   389,    -1,  1209,  1210,   110,
      -1,    -1,    -1,    -1,   101,   116,    -1,    -1,   105,    -1,
      -1,  1223,   141,   142,    -1,    -1,   145,   146,  1230,   148,
     149,    -1,    -1,    -1,   153,    -1,    -1,    -1,    -1,    -1,
    1242,   160,    -1,    -1,  1246,    -1,    -1,   166,   167,   168,
      -1,    -1,     1,    -1,    -1,    -1,    -1,   351,  1260,    -1,
      -1,    -1,   763,    -1,    -1,   766,    -1,    -1,    -1,    -1,
      -1,    -1,   366,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    -1,   376,    -1,    -1,    -1,    35,    -1,    37,    38,
      -1,    -1,    -1,    -1,    43,   389,     2,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    53,    -1,    55,    -1,    -1,    -1,
      -1,    -1,  1314,  1315,  1316,  1317,  1318,  1319,    -1,    25,
     504,    -1,   506,    -1,    73,    -1,    -1,    76,   215,    78,
      -1,    80,    -1,    82,    83,    -1,    85,    -1,    -1,    88,
      -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   857,    -1,    -1,    -1,
     109,    -1,   863,   112,    -1,    -1,   115,    -1,    74,    -1,
      -1,    -1,    78,    79,   875,    -1,    -1,    -1,   879,    -1,
      -1,    -1,    -1,   270,    -1,    -1,    -1,    -1,   275,    -1,
      -1,   140,    -1,    -1,    -1,   101,    -1,    -1,    -1,   105,
      -1,    -1,   586,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     504,    -1,   506,    -1,     3,    -1,   165,    -1,    -1,    -1,
      -1,    -1,   171,    -1,    -1,    -1,    -1,    -1,    17,    18,
      -1,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,   950,
      39,   952,    -1,    -1,   955,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,   351,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,   979,   366,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,   376,
     991,    -1,   586,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,   389,    -1,    -1,    31,    -1,    96,    97,   215,
      99,     3,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,   111,    -1,   113,    17,    18,   116,    20,    -1,
      -1,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,
    1051,    -1,    -1,    45,    -1,    -1,  1057,    49,    -1,    -1,
      52,    -1,    -1,    89,   270,    -1,    -1,    93,    -1,   275,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,   763,
      -1,    -1,   766,   172,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
    1101,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,    -1,   106,   141,   142,   504,   110,   506,
     146,   113,   148,   149,   116,    -1,    -1,   153,    -1,    -1,
      -1,    -1,    -1,    -1,   160,    -1,    -1,    -1,    -1,    -1,
     166,   167,   168,    -1,    -1,   351,    -1,  1148,    -1,  1150,
      -1,    -1,    -1,    -1,  1155,    -1,    -1,  1158,    -1,    -1,
     366,    -1,    -1,    -1,    -1,  1166,    -1,  1168,    -1,   763,
     376,    -1,   766,   857,    -1,    -1,    -1,    -1,   170,   863,
     172,    -1,    -1,   389,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   875,    -1,    -1,    -1,   879,    -1,    -1,  1199,   586,
      -1,    -1,    -1,    -1,  1205,    -1,    -1,    -1,  1209,  1210,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1223,    -1,    -1,    -1,    -1,    -1,    -1,  1230,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1242,    -1,    -1,    -1,  1246,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1260,
      -1,    -1,    -1,   857,    -1,    -1,   950,    -1,   952,   863,
      -1,   955,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   875,    -1,    -1,    -1,   879,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   979,    -1,    -1,   504,    -1,
     506,    -1,    -1,    -1,    -1,    -1,    -1,   991,    -1,    -1,
      -1,    -1,    -1,  1314,  1315,  1316,  1317,  1318,  1319,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    17,    18,    -1,    20,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,   950,    -1,   952,    33,
      -1,   955,    -1,    -1,    -1,    39,    -1,  1051,    -1,    -1,
      -1,    45,    -1,  1057,    -1,    49,   763,    -1,    52,   766,
     586,    -1,    -1,    -1,    -1,   979,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,   991,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,  1101,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1051,    -1,    -1,
      -1,    -1,    -1,  1057,  1148,    -1,  1150,    -1,    -1,    -1,
     857,  1155,    -1,    -1,  1158,    -1,   863,    -1,    -1,    -1,
      -1,    -1,  1166,    -1,  1168,    -1,    -1,    -1,   875,    -1,
      -1,    -1,   879,    -1,    -1,    -1,    -1,    -1,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1199,    -1,    -1,    -1,    22,
      -1,  1205,    -1,    26,    27,  1209,  1210,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,  1223,
      43,    -1,    -1,    -1,    -1,    -1,  1230,    50,    -1,    -1,
      -1,    -1,    55,    -1,  1148,    -1,  1150,   763,  1242,    -1,
     766,  1155,  1246,   950,  1158,   952,    31,    -1,   955,    -1,
      -1,    -1,  1166,    76,  1168,    78,  1260,    80,    -1,    82,
      -1,    -1,    85,    -1,    -1,    88,    -1,    90,    -1,    -1,
      -1,    -1,   979,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   991,  1199,   109,    -1,    -1,   112,
      -1,  1205,   115,    -1,    -1,  1209,  1210,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,  1223,
    1314,  1315,  1316,  1317,  1318,  1319,  1230,   140,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1242,    -1,
      -1,   857,  1246,    -1,    -1,    -1,    -1,   863,    -1,   124,
      -1,    -1,   165,    -1,  1051,    -1,  1260,    -1,   171,   875,
    1057,    -1,    -1,   879,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,    -1,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,     2,    -1,
      -1,    -1,   177,    -1,  1101,    -1,    -1,    -1,    -1,    -1,
    1314,  1315,  1316,  1317,  1318,  1319,    -1,    -1,    -1,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    12,    -1,
      -1,    -1,    -1,    -1,   950,    19,   952,    -1,    -1,   955,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,  1148,    36,  1150,    -1,    -1,    -1,    -1,  1155,    -1,
      -1,  1158,    -1,   979,    -1,    -1,    -1,    -1,    -1,  1166,
      74,  1168,    -1,    -1,    -1,   991,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,   105,  1199,    -1,    -1,    89,    -1,    -1,  1205,    93,
      -1,    -1,  1209,  1210,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1223,    -1,    -1,    -1,
      -1,    -1,    -1,  1230,    -1,  1051,    -1,    -1,    -1,    -1,
     124,  1057,    -1,    -1,    -1,  1242,    -1,    -1,    -1,  1246,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,  1260,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,   165,   166,   167,   168,  1101,    -1,    -1,    -1,    -1,
      -1,    -1,    13,   177,    -1,    -1,    17,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    -1,    -1,    -1,    -1,
      -1,   215,    33,    -1,    -1,    -1,    -1,  1314,  1315,  1316,
    1317,  1318,  1319,    44,    -1,    46,    47,    -1,    -1,    -1,
      -1,    52,  1148,    54,  1150,    56,    -1,    -1,    -1,  1155,
      61,    -1,  1158,    -1,    65,    66,    67,    68,    69,    70,
    1166,    -1,  1168,    74,    -1,    76,    -1,    -1,    -1,    -1,
      -1,    -1,    83,    -1,    -1,    86,   270,    -1,    89,    -1,
      91,   275,    -1,    94,    95,    -1,    97,    -1,    -1,    -1,
     101,   102,   103,  1199,    12,    -1,    -1,    -1,    -1,  1205,
      -1,    19,    -1,  1209,  1210,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,  1223,    -1,   130,
      -1,   132,    -1,    -1,  1230,    -1,    -1,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,  1242,    -1,    -1,    -1,
    1246,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1260,    -1,    -1,   351,    -1,    -1,
      -1,    -1,    -1,    -1,   175,    -1,    -1,    -1,    -1,    -1,
      -1,    89,   366,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,   376,    -1,    -1,    12,    -1,   198,    -1,    -1,
     201,   202,    19,    -1,    -1,   389,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    31,    -1,   124,    -1,  1314,  1315,
    1316,  1317,  1318,  1319,    -1,    -1,    -1,   228,    -1,    -1,
      47,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,   170,   264,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   302,   303,   120,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   317,   318,    -1,    -1,
     504,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
     331,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,   354,    -1,    -1,    -1,    -1,    -1,    -1,
     177,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   377,    -1,   379,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,   388,    -1,   390,
     391,   392,   393,   394,    -1,    -1,    -1,    -1,   399,    -1,
      -1,    -1,   586,    -1,    -1,   169,    -1,    68,   172,    -1,
     174,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     431,   432,   433,   434,   435,   436,    -1,   438,   439,   440,
     441,   442,    -1,   444,   445,   446,   447,   448,   449,   450,
     451,   452,   453,   454,   455,   456,   457,   458,   459,   460,
      -1,    -1,    -1,    -1,    -1,    -1,   467,   468,    -1,    -1,
      -1,    -1,   473,    -1,    -1,   476,   477,   478,   479,   480,
     481,    -1,    -1,   484,   485,   486,    -1,   488,    -1,    -1,
      -1,    -1,    -1,   494,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   506,    -1,    -1,   509,    -1,
      -1,   512,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   525,    -1,   527,    -1,    -1,    44,
     531,    -1,    -1,   534,   535,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   545,    -1,    -1,    -1,    -1,   550,
      -1,    -1,    -1,    68,    -1,    -1,    -1,    -1,   559,    -1,
      -1,    -1,    -1,    78,    79,    -1,    -1,   568,    -1,    -1,
     571,    -1,   573,    -1,    -1,    -1,   577,   578,   579,   763,
      -1,   582,   766,    -1,    -1,    -1,    -1,    12,    13,    -1,
      -1,    -1,   593,   594,    19,   110,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    36,   613,    -1,    -1,    -1,    -1,   278,    -1,   280,
      -1,    -1,    -1,    -1,    -1,   626,   287,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   636,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,   327,    -1,    93,    -1,
      -1,    -1,    -1,   857,    -1,    -1,    -1,    -1,   679,   863,
      -1,    -1,    -1,    -1,    -1,   686,    -1,    -1,    -1,    -1,
      -1,   875,    -1,    -1,    -1,   879,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,   220,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   740,
     741,   166,   167,   168,   259,   260,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,   269,    -1,   757,    -1,   759,   760,
     761,    -1,    -1,   278,    -1,   280,   950,    -1,   952,    -1,
      -1,   955,   287,    -1,    -1,    -1,    -1,    -1,    -1,   780,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   794,    -1,   979,    -1,   798,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   991,    -1,    -1,
      -1,    -1,   327,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   822,    -1,    -1,    -1,   826,    -1,   828,   829,   830,
      -1,   832,    -1,   834,    -1,    -1,   837,   838,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     851,    -1,   853,    -1,   855,    -1,   857,   858,    -1,    -1,
      -1,    -1,   863,   864,    -1,    -1,    -1,  1051,   869,    -1,
      -1,    -1,    -1,  1057,    -1,    -1,   877,   878,   879,    -1,
     881,   882,    -1,   884,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1101,    -1,   920,
     921,   922,   923,   924,   925,   926,    -1,    -1,   929,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   943,   944,   605,    -1,   607,   608,   609,   610,
     611,   612,   953,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1148,    -1,  1150,    -1,    -1,   970,
      -1,  1155,    -1,   974,  1158,    -1,    -1,    -1,    -1,   980,
      -1,    -1,  1166,    -1,  1168,    -1,    -1,   988,   989,   990,
      89,   992,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1013,    -1,    -1,  1199,    -1,    -1,    -1,    -1,
    1021,  1205,  1023,    -1,    -1,  1209,  1210,    -1,    -1,    -1,
      -1,    -1,  1033,    -1,  1035,    -1,    -1,    -1,  1039,    -1,
    1041,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,    -1,   153,    -1,    -1,    -1,  1242,    -1,
     159,   160,  1246,    -1,   163,    -1,    -1,   166,   167,   168,
      -1,    -1,    -1,  1074,    -1,    -1,  1260,    -1,   177,    -1,
      -1,    -1,    -1,  1084,    -1,    -1,  1087,    -1,    -1,    -1,
     605,   606,   607,   608,   609,   610,   611,   612,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1106,  1107,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1118,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1128,  1129,  1130,
    1131,  1132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1148,    -1,  1150,
      -1,    -1,   813,    -1,  1155,    -1,    -1,  1158,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1166,    -1,  1168,    -1,    -1,
    1171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1190,
    1191,   852,  1193,  1194,  1195,  1196,    -1,    -1,    -1,    -1,
      -1,   862,    -1,    -1,    -1,   866,    -1,   868,    -1,    -1,
     871,    -1,    -1,    -1,    -1,    -1,    -1,  1218,    -1,    -1,
      -1,    -1,  1223,  1224,    -1,  1226,    -1,  1228,    -1,  1230,
    1231,    -1,    -1,  1234,    -1,    -1,    -1,    -1,  1239,    -1,
      -1,    -1,  1243,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1251,    -1,   767,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1266,  1267,  1268,  1269,  1270,
    1271,  1272,  1273,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,   804,
      -1,    -1,    -1,    -1,    -1,    -1,    31,   812,   813,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1314,  1315,  1316,  1317,  1318,  1319,  1320,
    1321,  1322,  1323,  1324,  1325,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   852,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   862,    -1,    -1,
      -1,   866,    -1,   868,    89,    -1,   871,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   102,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1037,  1038,    -1,    -1,
      -1,    -1,  1043,  1044,    -1,    -1,  1047,  1048,    -1,   124,
     125,   126,   127,   128,   129,   130,    -1,    -1,   133,   134,
     135,   136,    -1,   138,   139,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      -1,   166,   167,   168,    -1,   170,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,   960,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,  1116,    -1,    -1,    -1,  1120,
    1121,  1122,  1123,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   987,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1142,  1143,    -1,  1145,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1157,    -1,    -1,  1160,
      -1,  1162,    -1,  1164,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1037,  1038,    -1,    -1,    -1,    -1,  1043,  1044,
      -1,    -1,  1047,  1048,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,     3,   157,   158,   159,   160,   161,
      -1,   163,    -1,    12,   166,   167,   168,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,   177,    25,    -1,    -1,    -1,
      -1,  1116,    -1,    -1,    33,  1120,  1121,  1122,  1123,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,  1142,  1143,    -1,
    1145,    -1,  1147,    62,    -1,    -1,    -1,    -1,    -1,  1154,
      -1,    70,  1157,    -1,    -1,  1160,    -1,  1162,    77,  1164,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,    -1,
     109,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,  1207,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,    -1,   148,
      -1,    -1,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,   163,    -1,    -1,   166,   167,   168,
     169,   170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1285,  1286,    -1,    -1,  1289,  1290,  1291,    -1,  1293,     0,
       1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,   170,   171,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,   171,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    15,
      16,    17,    18,    -1,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    32,    33,    -1,    35,
      -1,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    -1,    48,    49,    50,    -1,    52,    53,    -1,    55,
      56,    57,    58,    59,    -1,    61,    62,    63,    64,    65,
      66,    67,    -1,    69,    70,    71,    -1,    73,    -1,    -1,
      76,    77,    78,    79,    -1,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    90,    91,    92,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,    -1,   103,   104,    -1,
     106,   107,   108,   109,   110,    -1,   112,   113,   114,   115,
     116,    -1,    -1,   119,    -1,   121,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
       1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    16,    17,    18,    -1,    20,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    32,    33,    -1,    35,    -1,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    -1,    48,    49,    50,
      -1,    52,    53,    -1,    55,    56,    57,    58,    59,    -1,
      61,    62,    63,    64,    65,    66,    67,    -1,    69,    70,
      71,    -1,    73,    -1,    -1,    76,    77,    78,    79,    -1,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    90,
      91,    92,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,   107,   108,   109,   110,
      -1,   112,   113,   114,   115,   116,    -1,    -1,   119,    -1,
     121,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,     1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,   174,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    -1,    14,    -1,    -1,    17,    18,    19,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    54,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,   125,   126,   127,   128,   129,   130,
      -1,    -1,   133,   134,   135,   136,    -1,   138,   139,    -1,
     141,   142,   143,   144,   145,   146,    -1,   148,    -1,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    12,    -1,    14,    -1,
      -1,    17,    18,    19,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    54,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,    -1,    -1,   133,   134,   135,
     136,    -1,   138,   139,    -1,   141,   142,   143,   144,   145,
     146,    -1,   148,    -1,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,    -1,
     166,   167,   168,   169,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    15,    -1,    17,    18,    -1,    20,
      -1,    22,    23,    -1,    25,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      61,    62,    63,    64,    -1,    66,    67,    -1,    69,    70,
      71,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,    -1,   103,   104,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      26,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    78,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    89,    -1,
      -1,    -1,    93,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,   147,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,   107,   108,    -1,   110,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,   151,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,   112,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    47,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,   173,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,   107,   108,    -1,   110,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,   173,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,   173,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,
      41,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,   174,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,   175,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,   173,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,   107,   108,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,   147,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,   147,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,   168,    -1,   170,   171,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,   147,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    27,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,   107,   108,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    89,    -1,    -1,    -1,    93,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    78,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,   122,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,   175,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,   147,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,   173,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    32,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,   122,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,   175,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
     171,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,   111,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,   175,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,   175,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
     111,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,
      -1,   162,   163,   164,    -1,    -1,    -1,   168,    -1,   170,
      -1,   172,    -1,    -1,    -1,    -1,   177,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,
      -1,    17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    44,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,
      -1,    -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,
      66,    67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99,   100,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,   143,   144,   145,
      -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,    -1,
      -1,    -1,   168,    -1,   170,    -1,   172,    -1,    -1,    -1,
      -1,   177,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,
      -1,    22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    -1,    -1,    -1,    39,    -1,
      -1,    -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,
      -1,    62,    -1,    64,    -1,    66,    67,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,
      81,    -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,
     101,    -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,
      -1,    -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,   143,   144,   145,    -1,    -1,    -1,    -1,   150,
       4,     5,     6,     7,     8,     9,    10,    -1,   159,    -1,
      14,   162,   163,   164,    18,    -1,    -1,   168,    22,   170,
      -1,   172,    -1,    27,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    35,    -1,    -1,    -1,    -1,    40,    41,    -1,    -1,
      44,    -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    31,    77,    78,    79,    80,    81,    -1,    -1,
      -1,    -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    -1,    98,    -1,   100,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,   113,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,
     174,    -1,    -1,   177,     4,     5,     6,     7,     8,     9,
      10,    -1,   141,   142,    14,   144,   145,   146,    18,   148,
     149,   150,    22,   152,   153,   154,   155,    27,   157,   158,
     159,   160,   161,    -1,   163,    35,    -1,   166,   167,   168,
      40,    41,    -1,    -1,    44,    -1,    -1,    47,   177,    -1,
      -1,    51,    -1,    -1,    -1,    55,    -1,    -1,    58,    59,
      -1,    -1,    -1,    -1,    64,    -1,    66,    67,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,
      80,    81,    -1,    -1,    -1,    -1,    86,    -1,    88,    -1,
      90,    -1,    -1,    -1,    -1,    -1,    96,    -1,    98,    -1,
     100,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,     3,
      -1,   141,   142,   143,   144,   145,    -1,    -1,    12,    -1,
     150,    -1,    -1,    17,    18,    19,    20,    -1,    -1,   159,
      -1,    25,   162,   163,   164,    -1,    -1,    -1,   168,    33,
     170,    35,   172,    -1,   174,    39,    -1,   177,    -1,    -1,
      -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,    -1,   141,   142,   143,
     144,   145,   146,    -1,   148,    -1,    -1,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,   163,
      -1,    -1,   166,   167,   168,   169,    -1,    -1,   172,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,    -1,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,     3,     4,     5,     6,     7,     8,     9,    10,    -1,
      -1,    -1,    14,    -1,    -1,    17,    18,    -1,    20,    -1,
      22,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    33,    -1,    35,    -1,    -1,    -1,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    -1,    -1,    49,    -1,    -1,
      52,    -1,    -1,    55,    -1,    -1,    58,    59,    -1,    -1,
      62,    -1,    64,    -1,    66,    67,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    79,    -1,    81,
      -1,    -1,    -1,    -1,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    98,    99,   100,   101,
      -1,   103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
      -1,   143,    -1,    -1,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,   164,    22,    -1,    -1,    25,    -1,   170,    -1,
     172,    -1,   174,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
       3,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    12,
      -1,    -1,    -1,    -1,    17,    18,    19,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,   141,    -1,   143,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,   164,    -1,    -1,    -1,    62,
      -1,    -1,    -1,   172,    -1,   174,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    96,    97,    -1,    99,    19,   101,    -1,
     103,    -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    31,
     113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   125,   126,   127,   128,   129,   130,    -1,    -1,
     133,   134,   135,   136,    -1,   138,   139,    -1,   141,   142,
     143,   144,   145,   146,    -1,   148,    -1,    -1,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
     163,    -1,    -1,   166,   167,   168,   169,    89,    -1,    -1,
      12,    93,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    89,   170,    -1,
      12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    89,   170,    -1,
      12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    89,   170,    -1,
      12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    89,   170,    -1,
      12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    89,   170,    -1,
      12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    89,   170,    -1,
      12,    93,    -1,    -1,    -1,   177,    -1,    19,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    -1,
      -1,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    89,   170,    -1,
      -1,    93,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,   124,   125,   126,   127,   128,   129,   130,    31,
      32,   133,   134,   135,   136,    -1,   138,   139,    -1,   141,
     142,    -1,   144,   145,   146,    47,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,   170,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    89,    -1,    -1,
      -1,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    47,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    47,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,   170,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    89,   166,   167,   168,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   177,   102,    -1,    12,    -1,    -1,    -1,
      -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,   124,    -1,    -1,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    72,    -1,   166,   167,
     168,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    12,    89,    -1,    -1,    -1,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    12,    13,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    31,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    89,   166,   167,   168,    93,    -1,    -1,    -1,    12,
      -1,   175,    -1,   177,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,   120,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   124,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    89,   175,    -1,   177,
      93,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    -1,   166,   167,
     168,   124,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    -1,   166,   167,   168,    -1,   170,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,   141,   142,    -1,   144,   145,
     146,   102,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,   165,
     166,   167,   168,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,   177,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    12,    13,    -1,    -1,    -1,   177,    -1,    19,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      31,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,   173,    -1,    -1,    -1,   177,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    12,    -1,    -1,
      -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    34,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,   123,   124,
      -1,    -1,    -1,    12,    89,    -1,   177,    -1,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   124,
      -1,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    12,
      13,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    31,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    89,   166,   167,   168,
      93,    -1,    -1,    -1,   173,    12,    -1,    -1,   177,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
     173,    -1,    89,    -1,   177,    -1,    93,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    13,   166,   167,   168,    -1,   124,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    -1,    -1,    -1,    12,   173,    -1,    -1,    -1,
     177,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,
      -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    89,   144,   145,   146,    93,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,   120,    -1,    -1,   177,   124,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,
      -1,    -1,    19,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    31,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   124,    -1,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    31,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,   165,   166,
     167,   168,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,
     177,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   123,   124,    -1,    -1,
      -1,    -1,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,
      -1,    -1,    19,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    31,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,
     177,    -1,    19,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    31,    32,   153,   154,   155,    -1,
     157,   158,   159,   160,    -1,    -1,   163,   124,    -1,   166,
     167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
     177,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   124,    -1,   166,
     167,   168,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,
     177,    -1,    19,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    31,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   124,    -1,   166,
     167,   168,    -1,    -1,    -1,    12,    13,    -1,   175,    -1,
     177,    -1,    19,    60,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    31,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,
     177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    12,    89,    -1,    -1,    -1,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   124,    -1,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     177,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,   102,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      89,   166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    12,    89,   166,   167,   168,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    12,    89,   166,
     167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,
     177,   102,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    89,   166,   167,   168,    93,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    31,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    -1,
      89,   166,   167,   168,    93,    -1,    -1,    -1,    12,    -1,
      -1,    -1,   177,   102,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   124,    -1,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,   177,    93,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    22,   166,   167,   168,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,   177,    35,    -1,    37,    38,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    -1,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    26,    27,    -1,    -1,    76,    -1,    78,    -1,    80,
      35,    82,    37,    38,    85,    -1,    -1,    88,    43,    90,
      -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,
      -1,   112,    -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    80,    -1,    82,    -1,    -1,
      85,    -1,    -1,    88,    -1,    90,    -1,    -1,    -1,   140,
      22,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    35,   109,    37,    38,   112,    -1,    -1,
     115,    43,    -1,    -1,   165,    -1,    -1,    -1,    50,    -1,
     171,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    73,    -1,    -1,    76,    -1,    78,    -1,    -1,    -1,
      82,    83,    84,    85,    -1,    -1,    88,    22,    90,    -1,
     165,    26,    27,    -1,    -1,    -1,   171,    -1,    -1,    -1,
      35,    -1,    37,    38,    -1,    -1,    -1,   109,    43,    -1,
     112,    -1,    -1,   115,    -1,    50,    -1,    -1,    -1,    -1,
      55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
      -1,    76,    -1,    78,    -1,    80,    35,    82,    37,    38,
      85,    -1,    -1,    88,    43,    90,    -1,    -1,    -1,    -1,
      -1,    50,    -1,   165,    -1,    -1,    55,    -1,   170,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,   112,    -1,    -1,
     115,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    -1,    -1,    82,    -1,     1,    85,     3,    -1,    88,
      -1,    90,    -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,
      -1,    17,    18,    -1,    20,    -1,    -1,    -1,    -1,    25,
     109,    -1,    -1,   112,    -1,    -1,   115,    33,    -1,    -1,
     165,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,   140,     3,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    17,    18,    -1,    20,
      -1,    77,    -1,    -1,    25,    -1,   165,    -1,    -1,    -1,
      -1,    87,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      96,    97,    -1,    99,    45,   101,    -1,   103,    49,    -1,
     106,    52,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,     3,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,    87,    -1,    -1,    -1,
      -1,    17,    18,    -1,    20,    96,    97,    -1,    99,    25,
     101,    27,   103,    -1,    -1,   106,    -1,    33,    -1,   110,
      -1,    -1,   113,    39,    -1,   116,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    62,    -1,    -1,   140,
      -1,    -1,    -1,    69,    70,    -1,    17,    18,    -1,    20,
      -1,    77,    78,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    33,    -1,    90,    -1,    -1,    -1,    39,    -1,
      96,    97,    -1,    99,    45,   101,    -1,   103,    49,    -1,
     106,    52,    -1,    -1,   110,    -1,    -1,   113,    -1,   115,
     116,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    17,    18,    -1,    20,    96,    97,    -1,    99,    25,
     101,    -1,   103,    -1,    -1,   106,    -1,    33,    -1,   110,
     111,    -1,   113,    39,    -1,   116,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,     3,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    17,    18,    -1,    20,
      -1,    77,    -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,
      96,    97,    -1,    99,    45,   101,    -1,   103,    49,    -1,
     106,    52,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116,    62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,
      -1,    -1,    -1,    -1,    -1,    -1,    77,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,
      -1,    17,    18,    -1,    20,    96,    97,    -1,    99,    25,
     101,    -1,   103,    -1,    -1,   106,    -1,    33,    -1,   110,
      -1,    -1,   113,    39,    -1,   116,    -1,    -1,    -1,    45,
      -1,    -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    87,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    -1,    99,    -1,   101,    -1,   103,    -1,    -1,
     106,    -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,
     116
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   182,   183,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    10,    14,    15,    16,    17,    18,    20,    22,
      23,    25,    26,    27,    28,    29,    30,    33,    35,    37,
      38,    39,    43,    44,    45,    48,    49,    50,    52,    55,
      56,    57,    58,    59,    61,    62,    63,    64,    66,    67,
      69,    70,    71,    76,    77,    78,    79,    80,    81,    82,
      85,    86,    87,    88,    90,    91,    92,    94,    95,    96,
      97,    98,    99,   100,   101,   103,   104,   106,   107,   108,
     109,   110,   112,   113,   115,   116,   121,   124,   140,   141,
     142,   143,   144,   145,   150,   159,   162,   163,   164,   165,
     168,   170,   172,   177,   184,   185,   186,   187,   188,   189,
     192,   193,   194,   195,   200,   201,   202,   204,   205,   206,
     211,   212,   216,   217,   221,   223,   226,   227,   230,   231,
     232,   233,   234,   235,   238,   239,   240,   243,   245,   248,
     249,   254,   255,   256,   257,   258,   262,   263,   268,   269,
     270,   271,   274,   275,   280,   281,   283,   284,   289,   293,
     294,   296,   297,   319,   324,   325,   329,   330,   351,   352,
     353,   355,   357,   358,   359,   367,   368,   369,   370,   371,
     372,   374,   377,   378,   379,   380,   381,   382,   383,   384,
     386,   387,   388,   389,   390,   165,    22,    35,    44,    55,
      88,   101,   170,   239,   248,   281,   351,   358,   368,   369,
     374,   377,   379,   380,   120,   360,   361,     3,   218,   374,
     360,   109,   331,    90,   218,   186,   345,   374,   172,     3,
      17,    18,    20,    25,    33,    39,    45,    49,    52,    62,
      69,    70,    77,    87,    96,    97,    99,   101,   103,   106,
     110,   113,   116,   219,   220,   222,    11,    27,   115,   244,
     374,    82,    85,   202,   172,   220,   250,   251,   253,   220,
     220,   172,   220,   332,   333,    32,   206,   225,   374,   266,
     267,   374,   374,    18,    77,    96,   113,   374,   374,   374,
       8,   172,   229,   228,    27,    33,    47,    49,    51,    75,
      78,    90,    97,   101,   109,   185,   224,   285,   286,   287,
     310,   311,   312,   337,   342,   374,   345,   107,   108,   165,
     289,   290,   373,   374,   376,   374,   225,   374,   374,   374,
     100,   172,   186,   374,   374,   188,   192,   206,   188,   206,
     374,   376,     3,   374,   374,   374,   374,   374,     1,   171,
     184,   207,   345,   111,   151,   346,   347,   376,   374,    80,
     186,    22,    35,    38,    55,    88,   170,   189,   190,   191,
     202,   206,   194,     8,   149,   196,   170,    46,    84,   114,
     203,    26,   330,   374,     8,   270,   374,   375,    24,    32,
      40,    41,    42,   119,   174,   241,   354,   356,    55,   146,
     272,   220,     1,   193,   220,   276,   279,   172,   305,    53,
      73,    83,   314,    27,    75,    78,    90,   109,   315,    27,
      78,    90,   109,   313,   220,   326,   327,   332,   164,   165,
     374,    12,    19,    31,    89,    93,   124,   141,   142,   144,
     145,   146,   148,   149,   150,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   166,   167,   168,
     177,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   169,   301,   172,   174,
      89,    93,   374,   220,    40,    41,   174,    55,   172,   186,
     165,   206,   329,   165,   147,   165,   221,   348,   349,   350,
     376,   170,   220,   220,   102,   206,   102,   125,   206,   301,
     203,   348,   172,   165,   147,   165,   172,   206,   186,   111,
     172,   220,   334,   335,     1,   146,   341,    47,   147,   186,
     225,   147,   225,    13,   172,   172,   225,   348,   230,   230,
      47,    90,   311,   312,   173,   147,   111,   172,   220,   309,
     146,   165,   374,   374,   123,   291,   165,   170,   225,   172,
     348,   165,   259,   259,   165,   171,   171,   184,   147,   171,
     374,   147,   173,   147,   173,     8,   220,    40,    41,   174,
     241,     3,   172,   197,     1,   171,   207,   214,   215,   374,
     209,   374,    65,    36,    72,   165,   270,   272,   109,   236,
     294,   374,   186,    78,   122,   242,   374,   242,   374,   242,
     374,   244,   374,   175,   242,   345,   374,   385,   220,   250,
     251,   252,   345,   170,   171,   279,   125,   147,   171,   173,
     306,   308,   310,   315,    90,     1,   146,   339,   340,    90,
       1,     3,    12,    17,    19,    20,    25,    45,    52,    54,
      62,    70,    87,    99,   103,   110,   116,   141,   142,   143,
     144,   145,   146,   148,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   163,   166,   167,   168,   169,   172,
     220,   298,   299,   300,   301,   351,   125,   328,   147,   165,
     165,   374,   374,   374,   242,   374,   242,   374,   374,   374,
     374,   374,   374,   374,     3,    20,    33,    62,   103,   109,
     221,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,    68,   376,
     376,   376,   376,   376,   348,   348,   242,   374,   242,   374,
     102,   172,   374,   374,   345,   220,    27,    47,    90,   115,
     362,   365,   366,   374,   390,   332,   374,   125,   173,   147,
     125,   125,   186,    34,   186,   374,    34,   374,    65,   173,
     349,   250,   220,   246,   247,   335,   147,   173,   224,   374,
     125,   336,   374,   333,   266,   220,   329,   374,   374,   173,
     309,   315,   287,   335,   146,   343,   344,   342,   292,   205,
       1,   264,   350,   173,    21,   260,   171,   173,   173,   346,
     173,   346,   374,   374,   345,   196,   173,   198,   199,   350,
     171,   171,   147,   165,    13,   149,   147,   165,    13,    36,
      72,   220,   141,   142,   143,   144,   145,   159,   163,   168,
     208,   300,   301,   302,   374,   208,   210,   272,   170,   237,
     329,    47,   360,   119,   220,   172,   225,    32,    47,   225,
     120,   225,   363,    32,    47,   225,   363,   225,   363,    47,
     225,   363,   225,   225,   374,   175,   363,    47,   147,   175,
     363,   123,   147,   175,   172,   165,     1,   273,   374,   277,
     278,   147,   173,   341,     1,   224,   337,   338,   105,   316,
     172,   304,   374,   141,   149,   304,   304,   374,   327,   172,
     174,   165,   165,   165,   165,   165,   165,   173,   175,   349,
      32,    47,    32,    47,    47,   175,   172,    47,    90,   147,
     173,    17,    20,    25,    45,    52,    62,    70,    87,    99,
     110,   116,   351,    89,    89,   165,   376,   350,   374,   374,
      34,   186,    34,    34,   186,   102,   206,   220,   173,   147,
     173,   173,   334,   341,    68,   376,   220,   173,   173,   344,
     146,   288,   173,   342,   151,   303,   336,   374,   171,    74,
     117,   171,   265,   173,   172,   206,   220,   261,    47,    47,
      47,   175,   147,   173,   214,   221,    17,    18,    20,    25,
      45,    49,    52,    70,    77,    87,    96,    97,    99,   101,
     110,   113,   116,   170,   213,   302,   374,   111,   374,   208,
     210,   147,   165,    13,   165,   170,   273,   326,   332,   242,
     374,   225,   374,    47,   345,    44,   374,   242,   374,   172,
     225,    44,   374,   242,   374,   225,   225,   242,   374,   225,
     186,   175,   242,   374,   175,   186,   374,   175,   374,   175,
     374,   349,   171,    82,    85,   171,   185,   193,   227,   279,
     193,   308,   316,    60,   118,   320,   306,   307,   173,   299,
     301,   173,   175,   173,    44,   242,   374,    44,   242,   374,
     242,   374,   349,   365,   341,   351,   351,   186,   186,   374,
     186,    34,   165,   247,   206,   336,   172,   172,   303,   336,
     337,   316,   344,   374,   186,   239,   345,   261,   146,   206,
     242,   374,   242,   374,   242,   374,   199,   208,    13,    36,
      72,    36,    72,   165,   165,   302,   374,   374,   273,   171,
     165,   165,   360,   360,   165,   374,   173,   374,    32,   225,
      32,   225,   364,   365,   374,    32,   225,   363,    32,   225,
     363,   225,   363,   225,   363,   186,   175,   363,   175,   363,
     186,   123,   173,   193,   227,   227,   279,   320,     3,    92,
     103,   321,   322,   323,   374,   295,   173,   304,   304,   374,
      32,    32,   374,    32,    32,   175,   175,   173,   336,    34,
     186,   348,   348,   303,   336,    32,   225,   173,   374,   175,
     175,   171,   111,   374,   208,   210,   208,   210,    13,   171,
     225,   225,   225,   102,    44,   374,    44,   374,   147,   173,
     102,    44,   374,   225,    44,   374,   225,   225,   225,    44,
     186,   374,   175,    44,   186,   374,   175,   374,   227,   282,
     323,   118,   147,   125,   152,   154,   155,   157,   158,    60,
      32,   165,   205,   226,   317,   318,   102,    44,    44,   102,
      44,    44,    44,    44,   186,   173,   173,   186,   206,   165,
     165,   165,   165,   374,   374,   374,   374,   365,   374,   374,
     374,   374,   186,   374,   186,   318,   374,   322,   323,   323,
     323,   323,   323,   323,   321,   184,   374,   374,   374,   374,
     374,   374,   164,   164,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   374,   374,   374,   374,
     374,   374
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   188,   189,   189,   190,   191,
     191,   192,   193,   193,   194,   194,   194,   195,   196,   196,
     197,   197,   198,   198,   199,   200,   201,   201,   201,   202,
     202,   202,   203,   203,   204,   205,   205,   205,   206,   207,
     207,   208,   208,   208,   208,   208,   208,   209,   209,   209,
     209,   209,   209,   210,   210,   211,   211,   211,   211,   211,
     211,   211,   212,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     214,   214,   214,   214,   214,   215,   215,   216,   217,   217,
     217,   217,   217,   217,   218,   218,   219,   219,   219,   219,
     219,   220,   220,   221,   221,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   224,   224,   224,   224,
     225,   225,   226,   226,   227,   227,   228,   227,   229,   227,
     230,   230,   230,   230,   230,   230,   230,   230,   231,   231,
     231,   231,   232,   233,   233,   234,   235,   235,   235,   236,
     235,   237,   235,   238,   239,   240,   240,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   241,   241,   241,   241,   241,   241,   241,   241,
     241,   241,   242,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   244,   244,   245,   245,   246,
     246,   247,   248,   248,   248,   248,   248,   248,   248,   248,
     248,   248,   248,   248,   248,   249,   249,   249,   249,   249,
     249,   250,   251,   252,   252,   253,   253,   254,   255,   256,
     257,   257,   257,   258,   258,   258,   258,   259,   259,   260,
     260,   260,   261,   261,   262,   263,   263,   264,   264,   265,
     265,   265,   266,   266,   266,   267,   267,   268,   269,   269,
     270,   271,   271,   271,   272,   272,   273,   273,   273,   273,
     273,   274,   274,   275,   276,   276,   277,   276,   276,   278,
     276,   279,   279,   280,   282,   281,   283,   284,   284,   284,
     285,   285,   286,   286,   287,   287,   287,   288,   288,   289,
     291,   290,   292,   290,   293,   295,   294,   296,   296,   296,
     296,   296,   297,   298,   298,   299,   299,   299,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   302,   302,   303,   303,   304,
     304,   305,   305,   306,   306,   307,   307,   308,   309,   309,
     310,   310,   310,   310,   310,   310,   311,   311,   312,   312,
     312,   312,   312,   312,   312,   312,   312,   313,   313,   313,
     313,   313,   313,   314,   314,   314,   315,   315,   315,   315,
     315,   315,   315,   316,   316,   317,   317,   318,   318,   318,
     319,   320,   320,   320,   320,   320,   321,   321,   322,   322,
     322,   322,   322,   322,   322,   323,   323,   324,   325,   325,
     326,   326,   327,   328,   328,   329,   329,   329,   329,   329,
     331,   330,   330,   332,   332,   333,   333,   334,   334,   334,
     335,   335,   335,   336,   336,   336,   337,   338,   338,   338,
     339,   339,   340,   340,   341,   341,   341,   341,   342,   342,
     343,   344,   344,   345,   345,   346,   346,   347,   347,   348,
     348,   349,   349,   350,   350,   351,   351,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   353,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   354,   355,   356,   356,   356,   356,   356,   356,   356,
     357,   358,   359,   359,   359,   359,   359,   359,   359,   360,
     360,   361,   362,   362,   363,   364,   364,   365,   365,   365,
     366,   366,   366,   366,   366,   366,   367,   367,   367,   367,
     367,   368,   368,   368,   368,   368,   369,   370,   370,   370,
     370,   370,   370,   371,   372,   373,   373,   374,   374,   374,
     374,   374,   374,   374,   374,   374,   374,   374,   374,   374,
     374,   374,   374,   374,   375,   375,   376,   376,   376,   377,
     377,   377,   377,   378,   378,   378,   378,   378,   379,   379,
     379,   380,   380,   380,   380,   380,   380,   381,   381,   381,
     381,   382,   382,   383,   383,   384,   384,   384,   384,   384,
     384,   384,   384,   384,   384,   384,   384,   384,   385,   385,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   386,   386,   386,   386,   386,   386,   386,
     386,   386,   386,   387,   387,   387,   387,   387,   387,   387,
     388,   388,   388,   388,   389,   389,   389,   389,   390,   390,
     390,   390,   390,   390,   390
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     3,     3,     3,     3,     2,
       1,     1,     1,     1,     1,     2,     1,     3,     3,     3,
       3,     2,     3,     3,     2,     2,     1,     1,     0,     1,
       1,     1,     1,     2,     3,     2,     2,     2,     0,     3,
       2,     3,     1,     3,     1,     4,     3,     4,     4,     0,
       1,     1,     0,     1,     6,     2,     3,     3,     1,     1,
       2,     1,     1,     3,     3,     3,     5,     1,     3,     3,
       3,     5,     5,     0,     1,     4,     6,     8,     8,     6,
       8,     8,     4,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     3,     5,     1,     3,     3,     4,     4,
       4,     4,     4,     4,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     3,     1,     1,     0,     3,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     5,
       5,     2,     1,     1,     1,     1,     6,     7,     3,     0,
       6,     0,     6,     2,     0,     3,     2,     5,     3,     3,
       6,     6,     4,     5,     5,     3,     3,     6,     5,     6,
       5,     6,     3,     4,     3,     4,     5,     6,     5,     6,
       3,     4,     3,     4,     6,     7,     6,     7,     4,     5,
       4,     5,     4,     4,     3,     6,     5,     4,     3,     6,
       5,     6,     5,     8,     7,     4,     4,     6,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     1,     1,     1,     3,     3,     3,     4,
       5,     3,     6,     2,     2,     3,     3,     0,     2,     2,
       3,     5,     1,     3,     3,     5,     5,     0,     2,     3,
       2,     4,     4,     3,     1,     1,     3,     3,     5,     5,
       2,     1,     1,     1,     0,     2,     0,     2,     3,     3,
       4,     3,     3,     3,     1,     2,     0,     4,     2,     0,
       5,     1,     3,     1,     0,     8,     0,     1,     1,     1,
       0,     1,     1,     3,     1,     3,     1,     0,     2,     6,
       0,     3,     0,     4,     1,     0,     7,     4,     4,     6,
       6,     4,     2,     1,     3,     1,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     0,
       3,     2,     3,     1,     3,     0,     1,     1,     1,     1,
       4,     5,     4,     5,     6,     6,     0,     1,     1,     1,
       1,     2,     2,     1,     1,     1,     1,     0,     1,     1,
       2,     1,     1,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     0,     1,     1,     1,     1,     2,     1,
       1,     0,     2,     2,     4,     4,     1,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     1,     3,     1,     2,
       1,     3,     2,     0,     2,     1,     2,     1,     1,     1,
       0,     5,     3,     1,     3,     3,     5,     1,     1,     3,
       1,     2,     3,     0,     2,     2,     1,     1,     1,     1,
       2,     1,     0,     1,     0,     2,     2,     1,     1,     1,
       2,     0,     1,     1,     3,     1,     1,     3,     3,     0,
       1,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     4,     4,     4,     5,     2,     2,     1,
       2,     1,     2,     1,     2,     1,     2,     1,     1,     2,
       6,     6,     4,     9,     9,     7,     6,     6,     4,     9,
       9,     7,     2,     2,     3,     4,     6,     6,     9,     9,
       6,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     4,     1,     3,     4,     1,     3,     4,     3,     3,
       1,     1,     2,     1,     2,     1,     1,     2,     2,     2,
       2,     2,     8,     8,     9,     9,     4,     3,     3,     2,
       2,     2,     1,     3,     4,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     4,     4,
       4,     3,     3,     3,     3,     5,     5,     3,     4,     3,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     4,     5,     4,     5,     3,     5,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1
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
#line 6606 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 643 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6612 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 644 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6618 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6626 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6634 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 663 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6643 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 673 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6651 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 678 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6659 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6665 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6671 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6677 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6683 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6689 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 690 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6695 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 691 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6701 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: unary_implements_stmt_call  */
#line 693 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6707 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: binary_implements_stmt  */
#line 694 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6713 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: interface_stmt  */
#line 695 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6719 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: TDEFER stmt  */
#line 697 "chpl.ypp"
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
#line 6736 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: try_stmt  */
#line 709 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6742 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: return_stmt  */
#line 710 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6748 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 712 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6761 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 721 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6774 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TLABEL ident_def stmt  */
#line 730 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6782 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 734 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6793 "bison-chpl-lib.cpp"
    break;

  case 29: /* stmt_base: error TSEMI  */
#line 741 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6802 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: assignment_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6808 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: if_stmt  */
#line 749 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6814 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: loop_stmt  */
#line 750 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6820 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: select_stmt  */
#line 751 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6826 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: manage_stmt  */
#line 752 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6832 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 753 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6838 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: throw_stmt  */
#line 754 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6844 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 756 "chpl.ypp"
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
#line 6862 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 770 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    YYLTYPE locBodyAnchor = context->makeLocationAtLast((yylsp[-1]));
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]),
                               false, locBodyAnchor, (yyvsp[0].commentsAndStmt));
    CHPL_ASSERT(blockStyle == BlockStyle::EXPLICIT);
    auto taskBodies = context->consumeAndFlattenTopLevelBlocks(exprLst);
    auto node = Cobegin::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].withClause)),
                               std::move(taskBodies));
    CommentsAndStmt ret = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(ret);
  }
#line 6881 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 785 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6893 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 793 "chpl.ypp"
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
#line 6909 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TLOCAL do_stmt  */
#line 805 "chpl.ypp"
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
#line 6924 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TON expr do_stmt  */
#line 816 "chpl.ypp"
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
#line 6940 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 828 "chpl.ypp"
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
#line 6956 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSERIAL do_stmt  */
#line 840 "chpl.ypp"
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
#line 6971 "bison-chpl-lib.cpp"
    break;

  case 45: /* tryable_stmt: TSYNC stmt  */
#line 851 "chpl.ypp"
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
#line 6988 "bison-chpl-lib.cpp"
    break;

  case 48: /* collect_attributes: %empty  */
#line 871 "chpl.ypp"
  {
    auto attributeGroup = context->buildAttributeGroup((yyloc));
    if (attributeGroup != nullptr) {
      context->resetAttributeGroupPartsState();
    }
    context->loopAttributes.push_back(std::move(attributeGroup));
  }
#line 7000 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 896 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), true, (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 7008 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace  */
#line 900 "chpl.ypp"
 {
  // this allows us to distinguish between @attribute() and @attribute
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, (yyvsp[0].exprList), lst);
 }
#line 7019 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_stmt: attribute_decl_begin STRINGLITERAL  */
#line 907 "chpl.ypp"
 {
  // this allows us to maintain @unstable "message" without requiring parens
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  auto actual = makeMaybeNamedActual((yyvsp[0].expr));
  lst->push_back(actual);
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, context->makeList(), lst);
 }
#line 7032 "bison-chpl-lib.cpp"
    break;

  case 57: /* attribute_decl_begin: TATMARK TIDENT  */
#line 919 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 7041 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: %empty  */
#line 926 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 7049 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 930 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 7060 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP TRP  */
#line 940 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 7066 "bison-chpl-lib.cpp"
    break;

  case 61: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 942 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 7074 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actual  */
#line 949 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 63: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 954 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 7093 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 967 "chpl.ypp"
    {
      // take into account location of public/private, if any
      auto loc = context->declStartLoc((yylsp[-1]));

      ModuleParts parts = {
        .comments=context->gatherComments(loc),
        .attributeGroup=context->buildAttributeGroup((yyloc)).release(),
        .visibility=(yyvsp[-3].visibilityTag),
        .kind=(yyvsp[-2].moduleKind),
        .name=(yyvsp[0].uniqueStr)
      };

      (yyval.moduleParts) = parts;
      context->resetDeclState();
      context->clearComments();
    }
#line 7114 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 987 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-1]));

      ModuleParts parts = (yyvsp[-2].moduleParts);
      ParserExprList* body = context->makeList();
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-2])), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 7133 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 1002 "chpl.ypp"
    {
      context->clearCommentsBefore((yylsp[-2]));

      ModuleParts parts = (yyvsp[-3].moduleParts);
      ParserExprList* body = (yyvsp[-1].exprList);
      context->appendList(body, context->gatherComments((yylsp[0])));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 7152 "bison-chpl-lib.cpp"
    break;

  case 68: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1017 "chpl.ypp"
    {
      ModuleParts parts = (yyvsp[-3].moduleParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      ParserExprList* body = context->makeList(std::move(err));
      auto mod = Module::build(BUILDER, LOC((yylsp[-3])), toOwned(parts.attributeGroup),
                               parts.visibility,
                               parts.name,
                               parts.kind,
                               context->consumeList(body));
      CommentsAndStmt cs = {parts.comments, mod.release()};
      (yyval.commentsAndStmt) = cs;
    }
#line 7169 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: %empty  */
#line 1032 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 7175 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPUBLIC  */
#line 1033 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 7182 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_access_control: TPRIVATE  */
#line 1035 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 7189 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: %empty  */
#line 1040 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 7195 "bison-chpl-lib.cpp"
    break;

  case 73: /* opt_prototype: TPROTOTYPE  */
#line 1041 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 7202 "bison-chpl-lib.cpp"
    break;

  case 74: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1047 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 7212 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR TRCBR  */
#line 1065 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 7220 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1069 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 7228 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt_body: TLCBR error TRCBR  */
#line 1073 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 7240 "bison-chpl-lib.cpp"
    break;

  case 78: /* block_stmt: block_stmt_body  */
#line 1084 "chpl.ypp"
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
#line 7259 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: toplevel_stmt  */
#line 1102 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 7265 "bison-chpl-lib.cpp"
    break;

  case 80: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1103 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 7271 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: expr  */
#line 1108 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 7279 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: all_op_name  */
#line 1112 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7287 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: expr TAS expr  */
#line 1116 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 7296 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA expr  */
#line 1121 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 7304 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1125 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 7312 "bison-chpl-lib.cpp"
    break;

  case 86: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1129 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 7321 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr  */
#line 1139 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7330 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS expr  */
#line 1144 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7340 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1150 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 7351 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1157 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 7360 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1162 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7371 "bison-chpl-lib.cpp"
    break;

  case 92: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1169 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 7383 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: %empty  */
#line 1179 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 7389 "bison-chpl-lib.cpp"
    break;

  case 94: /* opt_only_ls: renames_ls  */
#line 1180 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 7395 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1185 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7404 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1190 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7416 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1198 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7429 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1207 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7443 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1217 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7455 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1225 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7468 "bison-chpl-lib.cpp"
    break;

  case 101: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1234 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7482 "bison-chpl-lib.cpp"
    break;

  case 102: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1247 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7491 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr  */
#line 1283 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7499 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT all_op_name  */
#line 1287 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7508 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1292 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7517 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TAS ident_use  */
#line 1297 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7527 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1303 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7537 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_expr  */
#line 1311 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7543 "bison-chpl-lib.cpp"
    break;

  case 126: /* import_ls: import_ls TCOMMA import_expr  */
#line 1312 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7549 "bison-chpl-lib.cpp"
    break;

  case 127: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1317 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7559 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1326 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7567 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1330 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7575 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1334 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7583 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1338 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7591 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1342 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7599 "bison-chpl-lib.cpp"
    break;

  case 133: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1346 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7607 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: %empty  */
#line 1354 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7613 "bison-chpl-lib.cpp"
    break;

  case 135: /* opt_label_ident: TIDENT  */
#line 1355 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7619 "bison-chpl-lib.cpp"
    break;

  case 143: /* ident_use: TIDENT  */
#line 1380 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7625 "bison-chpl-lib.cpp"
    break;

  case 144: /* ident_use: TTHIS  */
#line 1381 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7631 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TBOOL  */
#line 1408 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7637 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TENUM  */
#line 1409 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7643 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TINT  */
#line 1410 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7649 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TUINT  */
#line 1411 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7655 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TREAL  */
#line 1412 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7661 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TIMAG  */
#line 1413 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7667 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TCOMPLEX  */
#line 1414 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7673 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TBYTES  */
#line 1415 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7679 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TSTRING  */
#line 1416 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7685 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TLOCALE  */
#line 1417 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7691 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TNOTHING  */
#line 1418 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7697 "bison-chpl-lib.cpp"
    break;

  case 175: /* scalar_type: TVOID  */
#line 1419 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7703 "bison-chpl-lib.cpp"
    break;

  case 180: /* do_stmt: TDO stmt  */
#line 1433 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7709 "bison-chpl-lib.cpp"
    break;

  case 181: /* do_stmt: block_stmt  */
#line 1434 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7715 "bison-chpl-lib.cpp"
    break;

  case 182: /* return_stmt: TRETURN TSEMI  */
#line 1439 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7726 "bison-chpl-lib.cpp"
    break;

  case 183: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1446 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7737 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: TSEMI  */
#line 1456 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7745 "bison-chpl-lib.cpp"
    break;

  case 185: /* class_level_stmt: inner_class_level_stmt  */
#line 1460 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7755 "bison-chpl-lib.cpp"
    break;

  case 186: /* $@1: %empty  */
#line 1465 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7762 "bison-chpl-lib.cpp"
    break;

  case 187: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1467 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7771 "bison-chpl-lib.cpp"
    break;

  case 188: /* $@2: %empty  */
#line 1471 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 189: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1473 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7787 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1492 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7796 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1497 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7805 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1502 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7814 "bison-chpl-lib.cpp"
    break;

  case 201: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1507 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7822 "bison-chpl-lib.cpp"
    break;

  case 202: /* forwarding_decl_start: TFORWARDING  */
#line 1514 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7831 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_or_export: TEXTERN  */
#line 1521 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7837 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_or_export: TEXPORT  */
#line 1522 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7843 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1527 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7853 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1537 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7862 "bison-chpl-lib.cpp"
    break;

  case 207: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1543 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7873 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1550 "chpl.ypp"
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
#line 7895 "bison-chpl-lib.cpp"
    break;

  case 209: /* $@3: %empty  */
#line 1568 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7903 "bison-chpl-lib.cpp"
    break;

  case 210: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1572 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7912 "bison-chpl-lib.cpp"
    break;

  case 211: /* $@4: %empty  */
#line 1577 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7921 "bison-chpl-lib.cpp"
    break;

  case 212: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1582 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7930 "bison-chpl-lib.cpp"
    break;

  case 213: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1590 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7938 "bison-chpl-lib.cpp"
    break;

  case 214: /* no_loop_attributes: %empty  */
#line 1595 "chpl.ypp"
                    { context->loopAttributes.push_back(nullptr); }
#line 7944 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: attribute_group collect_attributes loop_stmt_base  */
#line 1598 "chpl.ypp"
                                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7950 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: no_loop_attributes loop_stmt_base  */
#line 1599 "chpl.ypp"
                                    { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 7956 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt_base: TDO stmt TWHILE expr TSEMI  */
#line 1604 "chpl.ypp"
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
                               toOwned((yyvsp[-1].expr)),
                               context->popLoopAttributeGroup());
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7979 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt_base: TWHILE expr do_stmt  */
#line 1623 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(body),
                             context->popLoopAttributeGroup());
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7997 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt_base: TWHILE ifvar do_stmt  */
#line 1637 "chpl.ypp"
  {
    std::vector<ParserComment>* comments;
    ParserExprList* exprLst;
    BlockStyle blockStyle;
    context->prepareStmtPieces(comments, exprLst, blockStyle, (yylsp[-2]), (yylsp[0]), (yyvsp[0].blockOrDo));
    auto body = context->consumeToBlock((yylsp[0]), exprLst);
    auto node = While::build(BUILDER, LOC((yylsp[-2])), toOwned((yyvsp[-1].expr)),
                             blockStyle,
                             std::move(body),
                             context->popLoopAttributeGroup());
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 8015 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt_base: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1651 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8023 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt_base: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1655 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8031 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt_base: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1659 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8039 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt_base: TFOR expr TIN expr do_stmt  */
#line 1663 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8047 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt_base: TFOR expr TIN zippered_iterator do_stmt  */
#line 1667 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8055 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt_base: TFOR expr do_stmt  */
#line 1671 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8063 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt_base: TFOR zippered_iterator do_stmt  */
#line 1675 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 8071 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt_base: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1679 "chpl.ypp"
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
                           /*isParam*/ true,
                           context->popLoopAttributeGroup());
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    context->resetAttributeGroupPartsState();
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 8095 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt_base: TFORALL expr TIN expr do_stmt  */
#line 1699 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8103 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt_base: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1703 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8111 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt_base: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1707 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8119 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt_base: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1711 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8127 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt_base: TFORALL expr do_stmt  */
#line 1715 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8135 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt_base: TFORALL expr forall_intent_clause do_stmt  */
#line 1719 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8143 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt_base: TFORALL zippered_iterator do_stmt  */
#line 1723 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8151 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt_base: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1727 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8159 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt_base: TFOREACH expr TIN expr do_stmt  */
#line 1731 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8167 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt_base: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1735 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8175 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1739 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8183 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt_base: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1743 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8191 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt_base: TFOREACH expr do_stmt  */
#line 1747 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8199 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt_base: TFOREACH expr forall_intent_clause do_stmt  */
#line 1751 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8207 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt_base: TFOREACH zippered_iterator do_stmt  */
#line 1755 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 8215 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt_base: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1759 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 8223 "bison-chpl-lib.cpp"
    break;

  case 244: /* loop_stmt_base: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1763 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8231 "bison-chpl-lib.cpp"
    break;

  case 245: /* loop_stmt_base: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1767 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8239 "bison-chpl-lib.cpp"
    break;

  case 246: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1771 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8247 "bison-chpl-lib.cpp"
    break;

  case 247: /* loop_stmt_base: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1775 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8255 "bison-chpl-lib.cpp"
    break;

  case 248: /* loop_stmt_base: TLSBR expr_ls TRSBR stmt  */
#line 1779 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8263 "bison-chpl-lib.cpp"
    break;

  case 249: /* loop_stmt_base: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1783 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8271 "bison-chpl-lib.cpp"
    break;

  case 250: /* loop_stmt_base: TLSBR zippered_iterator TRSBR stmt  */
#line 1787 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 8280 "bison-chpl-lib.cpp"
    break;

  case 251: /* loop_stmt_base: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1792 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 8289 "bison-chpl-lib.cpp"
    break;

  case 252: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1800 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 8299 "bison-chpl-lib.cpp"
    break;

  case 253: /* if_stmt: TIF expr TTHEN stmt  */
#line 1809 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8307 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF expr block_stmt  */
#line 1813 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8315 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1817 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8323 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1821 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8331 "bison-chpl-lib.cpp"
    break;

  case 257: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1825 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8339 "bison-chpl-lib.cpp"
    break;

  case 258: /* if_stmt: TIF ifvar block_stmt  */
#line 1829 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 8347 "bison-chpl-lib.cpp"
    break;

  case 259: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1833 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8355 "bison-chpl-lib.cpp"
    break;

  case 260: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1837 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8363 "bison-chpl-lib.cpp"
    break;

  case 261: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1841 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8372 "bison-chpl-lib.cpp"
    break;

  case 262: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1846 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 8381 "bison-chpl-lib.cpp"
    break;

  case 263: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1851 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8390 "bison-chpl-lib.cpp"
    break;

  case 264: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1856 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 8399 "bison-chpl-lib.cpp"
    break;

  case 265: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1864 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                   /*attributeGroup*/ nullptr,
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
#line 8418 "bison-chpl-lib.cpp"
    break;

  case 266: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1879 "chpl.ypp"
  {
    auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                   /*attributeGroup*/ nullptr,
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
#line 8437 "bison-chpl-lib.cpp"
    break;

  case 267: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1897 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8445 "bison-chpl-lib.cpp"
    break;

  case 268: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1901 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8453 "bison-chpl-lib.cpp"
    break;

  case 269: /* ifc_formal_ls: ifc_formal  */
#line 1907 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8459 "bison-chpl-lib.cpp"
    break;

  case 270: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1908 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8465 "bison-chpl-lib.cpp"
    break;

  case 271: /* ifc_formal: ident_def  */
#line 1913 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8471 "bison-chpl-lib.cpp"
    break;

  case 284: /* implements_type_ident: implements_type_error_ident  */
#line 1931 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8480 "bison-chpl-lib.cpp"
    break;

  case 291: /* interface_expr_ident: ident_def  */
#line 1957 "chpl.ypp"
  {
    (yyval.expr) = context->buildInterfaceExpr((yylsp[0]), (yyvsp[0].uniqueStr), nullptr).release();
  }
#line 8488 "bison-chpl-lib.cpp"
    break;

  case 292: /* interface_expr_call: ident_def TLP actual_ls TRP  */
#line 1964 "chpl.ypp"
  {
    (yyval.expr) = context->buildInterfaceExpr(YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)).release();
  }
#line 8496 "bison-chpl-lib.cpp"
    break;

  case 295: /* interface_ident_ls: interface_expr_ident  */
#line 1976 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 8504 "bison-chpl-lib.cpp"
    break;

  case 296: /* interface_ident_ls: interface_ident_ls TCOMMA interface_expr_ident  */
#line 1980 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 8512 "bison-chpl-lib.cpp"
    break;

  case 297: /* unary_implements_stmt_idents: TIMPLEMENTS interface_ident_ls TSEMI  */
#line 1987 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yyvsp[-1].exprList));
  }
#line 8520 "bison-chpl-lib.cpp"
    break;

  case 298: /* unary_implements_stmt_call: TIMPLEMENTS interface_expr_call TSEMI  */
#line 1994 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), context->makeList((yyvsp[-1].expr)));
  }
#line 8528 "bison-chpl-lib.cpp"
    break;

  case 299: /* binary_implements_stmt: implements_type_ident TIMPLEMENTS interface_expr TSEMI  */
#line 2002 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), context->makeList((yyvsp[-1].expr)));
  }
#line 8536 "bison-chpl-lib.cpp"
    break;

  case 300: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2009 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8544 "bison-chpl-lib.cpp"
    break;

  case 301: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 2013 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8552 "bison-chpl-lib.cpp"
    break;

  case 302: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 2017 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8560 "bison-chpl-lib.cpp"
    break;

  case 303: /* try_stmt: TTRY tryable_stmt  */
#line 2024 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8568 "bison-chpl-lib.cpp"
    break;

  case 304: /* try_stmt: TTRYBANG tryable_stmt  */
#line 2028 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8576 "bison-chpl-lib.cpp"
    break;

  case 305: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 2032 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8584 "bison-chpl-lib.cpp"
    break;

  case 306: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 2036 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8592 "bison-chpl-lib.cpp"
    break;

  case 307: /* catch_expr_ls: %empty  */
#line 2042 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8598 "bison-chpl-lib.cpp"
    break;

  case 308: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 2043 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8604 "bison-chpl-lib.cpp"
    break;

  case 309: /* catch_expr: TCATCH block_stmt  */
#line 2048 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8612 "bison-chpl-lib.cpp"
    break;

  case 310: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 2052 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8620 "bison-chpl-lib.cpp"
    break;

  case 311: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 2056 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8628 "bison-chpl-lib.cpp"
    break;

  case 312: /* catch_expr_inner: ident_def  */
#line 2063 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
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
#line 8645 "bison-chpl-lib.cpp"
    break;

  case 313: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2076 "chpl.ypp"
  {
    (yyval.expr) = Variable::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
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
#line 8662 "bison-chpl-lib.cpp"
    break;

  case 314: /* throw_stmt: TTHROW expr TSEMI  */
#line 2092 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8672 "bison-chpl-lib.cpp"
    break;

  case 315: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2100 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8680 "bison-chpl-lib.cpp"
    break;

  case 316: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2104 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8690 "bison-chpl-lib.cpp"
    break;

  case 317: /* when_stmt_ls: %empty  */
#line 2112 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8696 "bison-chpl-lib.cpp"
    break;

  case 318: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2113 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8702 "bison-chpl-lib.cpp"
    break;

  case 319: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2118 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8710 "bison-chpl-lib.cpp"
    break;

  case 320: /* when_stmt: TOTHERWISE stmt  */
#line 2122 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8719 "bison-chpl-lib.cpp"
    break;

  case 321: /* when_stmt: TOTHERWISE no_loop_attributes TDO stmt  */
#line 2127 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
    context->popLoopAttributeGroup();
  }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 322: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2136 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8738 "bison-chpl-lib.cpp"
    break;

  case 323: /* manager_expr: expr TAS ident_def  */
#line 2141 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8746 "bison-chpl-lib.cpp"
    break;

  case 324: /* manager_expr: expr  */
#line 2145 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8754 "bison-chpl-lib.cpp"
    break;

  case 325: /* manager_expr_ls: manager_expr  */
#line 2151 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8760 "bison-chpl-lib.cpp"
    break;

  case 326: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2152 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8766 "bison-chpl-lib.cpp"
    break;

  case 327: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2157 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8774 "bison-chpl-lib.cpp"
    break;

  case 328: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2166 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8783 "bison-chpl-lib.cpp"
    break;

  case 329: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2171 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8794 "bison-chpl-lib.cpp"
    break;

  case 330: /* class_start: class_tag ident_def  */
#line 2182 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8802 "bison-chpl-lib.cpp"
    break;

  case 331: /* class_tag: TCLASS  */
#line 2188 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8808 "bison-chpl-lib.cpp"
    break;

  case 332: /* class_tag: TRECORD  */
#line 2189 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8814 "bison-chpl-lib.cpp"
    break;

  case 333: /* class_tag: TUNION  */
#line 2190 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8820 "bison-chpl-lib.cpp"
    break;

  case 334: /* opt_inherit: %empty  */
#line 2194 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8826 "bison-chpl-lib.cpp"
    break;

  case 335: /* opt_inherit: TCOLON expr_ls  */
#line 2195 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8832 "bison-chpl-lib.cpp"
    break;

  case 336: /* class_level_stmt_ls: %empty  */
#line 2199 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8841 "bison-chpl-lib.cpp"
    break;

  case 337: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2204 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8849 "bison-chpl-lib.cpp"
    break;

  case 338: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2208 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8857 "bison-chpl-lib.cpp"
    break;

  case 339: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2212 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8865 "bison-chpl-lib.cpp"
    break;

  case 340: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2216 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8873 "bison-chpl-lib.cpp"
    break;

  case 341: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2223 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      ParserExprList* list = (yyvsp[-1].exprList);
      // get any comments after the last element but before the closing brace
      context->appendList(list, context->gatherComments((yylsp[0])));

      auto decl = Enum::build(BUILDER, LOC((yyloc)), toOwned(parts.attributeGroup),
                              parts.visibility,
                              parts.name,
                              context->consumeList(list));
      CommentsAndStmt cs = {parts.comments, decl.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8894 "bison-chpl-lib.cpp"
    break;

  case 342: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2240 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8908 "bison-chpl-lib.cpp"
    break;

  case 343: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2253 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8916 "bison-chpl-lib.cpp"
    break;

  case 344: /* enum_ls: enum_item  */
#line 2260 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8925 "bison-chpl-lib.cpp"
    break;

  case 345: /* enum_ls: enum_ls TCOMMA  */
#line 2265 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8935 "bison-chpl-lib.cpp"
    break;

  case 346: /* $@5: %empty  */
#line 2271 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8944 "bison-chpl-lib.cpp"
    break;

  case 347: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2276 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8953 "bison-chpl-lib.cpp"
    break;

  case 348: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2281 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8962 "bison-chpl-lib.cpp"
    break;

  case 349: /* $@6: %empty  */
#line 2286 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8971 "bison-chpl-lib.cpp"
    break;

  case 350: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2291 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8980 "bison-chpl-lib.cpp"
    break;

  case 351: /* enum_item: ident_def  */
#line 2301 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8991 "bison-chpl-lib.cpp"
    break;

  case 352: /* enum_item: ident_def TASSIGN expr  */
#line 2308 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 9004 "bison-chpl-lib.cpp"
    break;

  case 353: /* lambda_decl_start: TLAMBDA  */
#line 2320 "chpl.ypp"
    {
      FunctionParts fp = context->makeFunctionParts(false, false);
      context->noteDeclStartLoc((yylsp[0]));
      auto loc = context->declStartLoc((yyloc));
      fp.comments = context->gatherComments(loc);
      fp.attributeGroup = context->buildAttributeGroup((yyloc)).release();
      fp.visibility = context->visibility;
      context->resetAttributeGroupPartsState();
      fp.kind = Function::PROC;
      (yyval.functionParts) = fp;
    }
#line 9020 "bison-chpl-lib.cpp"
    break;

  case 354: /* $@7: %empty  */
#line 2337 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 9030 "bison-chpl-lib.cpp"
    break;

  case 355: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2343 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-7].functionParts);
      fp.name = context->buildIdent((yylsp[-7]), STR("lambda"));
      fp.formals = (yyvsp[-6].exprList);
      fp.returnIntent = (yyvsp[-5].returnTag);
      fp.returnType = (yyvsp[-4].expr);
      fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS);
      WhereAndLifetime wl = (yyvsp[-2].lifetimeAndWhere);
      fp.lifetime = wl.lifetime;
      fp.where = wl.where;
      fp.body = (yyvsp[0].exprList);

      context->clearComments();
      context->exitScope(asttags::Function, STR("lambda"));

      (yyval.expr) = context->buildLambda((yyloc), fp);
    }
#line 9052 "bison-chpl-lib.cpp"
    break;

  case 357: /* linkage_spec: linkage_spec_empty  */
#line 2366 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 9058 "bison-chpl-lib.cpp"
    break;

  case 358: /* linkage_spec: TINLINE  */
#line 2367 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 9065 "bison-chpl-lib.cpp"
    break;

  case 359: /* linkage_spec: TOVERRIDE  */
#line 2369 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 9072 "bison-chpl-lib.cpp"
    break;

  case 360: /* opt_fn_type_formal_ls: %empty  */
#line 2374 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 9078 "bison-chpl-lib.cpp"
    break;

  case 361: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2375 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9084 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_type_formal_ls: fn_type_formal  */
#line 2379 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9090 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2380 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9096 "bison-chpl-lib.cpp"
    break;

  case 364: /* fn_type_formal: named_formal  */
#line 2385 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9102 "bison-chpl-lib.cpp"
    break;

  case 365: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2388 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 9108 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_type_formal: formal_type  */
#line 2390 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 9114 "bison-chpl-lib.cpp"
    break;

  case 367: /* opt_fn_type_ret_type: %empty  */
#line 2394 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 9120 "bison-chpl-lib.cpp"
    break;

  case 368: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2395 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9126 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2408 "chpl.ypp"
  {
    FunctionParts fp = context->makeFunctionParts(false, false);
    fp.kind = Function::PROC;
    fp.name = context->buildIdent((yylsp[-5]), STR("proc"));
    fp.formals = (yyvsp[-4].exprList);
    fp.returnIntent = (yyvsp[-2].returnTag);
    fp.returnType = (yyvsp[-1].expr);
    fp.throws = ((yyvsp[0].throwsTag) == ThrowsTag_THROWS);
    fp.body = nullptr;
    fp.comments = nullptr;
    fp.attributeGroup = nullptr;
    fp.visibility = context->visibility;
    (yyval.functionParts) = fp;
  }
#line 9145 "bison-chpl-lib.cpp"
    break;

  case 370: /* $@8: %empty  */
#line 2426 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9155 "bison-chpl-lib.cpp"
    break;

  case 371: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2432 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9168 "bison-chpl-lib.cpp"
    break;

  case 372: /* $@9: %empty  */
#line 2441 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 9178 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2447 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 9191 "bison-chpl-lib.cpp"
    break;

  case 374: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2459 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 9199 "bison-chpl-lib.cpp"
    break;

  case 375: /* $@10: %empty  */
#line 2468 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 9213 "bison-chpl-lib.cpp"
    break;

  case 376: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2478 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-6].functionParts);
    fp.returnIntent = (yyvsp[-5].returnTag);
    fp.returnType = (yyvsp[-4].expr);
    fp.throws = ((yyvsp[-3].throwsTag) == ThrowsTag_THROWS);
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
#line 9236 "bison-chpl-lib.cpp"
    break;

  case 377: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2500 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9248 "bison-chpl-lib.cpp"
    break;

  case 378: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2508 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9260 "bison-chpl-lib.cpp"
    break;

  case 379: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2516 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])), /*attributeGroup*/ nullptr,
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9275 "bison-chpl-lib.cpp"
    break;

  case 380: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2527 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-5].functionParts);
      fp.thisIntent = (yyvsp[-4].intentTag);
      fp.receiver = Formal::build(BUILDER, LOC((yylsp[-3])), /*attributeGroup*/ nullptr,
                                  STR("this"), (yyvsp[-4].intentTag), toOwned((yyvsp[-3].expr)),
                                  nullptr).release();
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 9290 "bison-chpl-lib.cpp"
    break;

  case 381: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2538 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 9300 "bison-chpl-lib.cpp"
    break;

  case 382: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2547 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-1].functionParts);
      context->noteDeclStartLoc((yylsp[0]));
      auto loc = context->declStartLoc((yyloc));
      fp.comments = context->gatherComments(loc);
      fp.attributeGroup = context->buildAttributeGroup((yyloc)).release();
      fp.visibility = context->visibility;
      context->resetAttributeGroupPartsState();
      fp.kind = (yyvsp[0].functionKind);
      (yyval.functionParts) = fp;
    }
#line 9316 "bison-chpl-lib.cpp"
    break;

  case 384: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2562 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 9322 "bison-chpl-lib.cpp"
    break;

  case 387: /* fn_ident: ident_def TBANG  */
#line 2569 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 427: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2626 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 9338 "bison-chpl-lib.cpp"
    break;

  case 428: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2627 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 9344 "bison-chpl-lib.cpp"
    break;

  case 429: /* opt_formal_ls: %empty  */
#line 2631 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 9350 "bison-chpl-lib.cpp"
    break;

  case 430: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2632 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9356 "bison-chpl-lib.cpp"
    break;

  case 431: /* req_formal_ls: TLP TRP  */
#line 2636 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 9362 "bison-chpl-lib.cpp"
    break;

  case 432: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2637 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9368 "bison-chpl-lib.cpp"
    break;

  case 433: /* formal_ls_inner: formal  */
#line 2641 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9374 "bison-chpl-lib.cpp"
    break;

  case 434: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2642 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9380 "bison-chpl-lib.cpp"
    break;

  case 435: /* formal_ls: %empty  */
#line 2646 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 9386 "bison-chpl-lib.cpp"
    break;

  case 436: /* formal_ls: formal_ls_inner  */
#line 2647 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9392 "bison-chpl-lib.cpp"
    break;

  case 440: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2661 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9400 "bison-chpl-lib.cpp"
    break;

  case 441: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2666 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9408 "bison-chpl-lib.cpp"
    break;

  case 442: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2671 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9416 "bison-chpl-lib.cpp"
    break;

  case 443: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2676 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 9424 "bison-chpl-lib.cpp"
    break;

  case 444: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2681 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 9432 "bison-chpl-lib.cpp"
    break;

  case 445: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2686 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 9440 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_formal_intent_tag: %empty  */
#line 2692 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 9449 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_formal_intent_tag: required_intent_tag  */
#line 2697 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 9458 "bison-chpl-lib.cpp"
    break;

  case 448: /* required_intent_tag: TIN  */
#line 2704 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9464 "bison-chpl-lib.cpp"
    break;

  case 449: /* required_intent_tag: TINOUT  */
#line 2705 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9470 "bison-chpl-lib.cpp"
    break;

  case 450: /* required_intent_tag: TOUT  */
#line 2706 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9476 "bison-chpl-lib.cpp"
    break;

  case 451: /* required_intent_tag: TCONST TIN  */
#line 2707 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9482 "bison-chpl-lib.cpp"
    break;

  case 452: /* required_intent_tag: TCONST TREF  */
#line 2708 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9488 "bison-chpl-lib.cpp"
    break;

  case 453: /* required_intent_tag: TCONST  */
#line 2709 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9494 "bison-chpl-lib.cpp"
    break;

  case 454: /* required_intent_tag: TPARAM  */
#line 2710 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9500 "bison-chpl-lib.cpp"
    break;

  case 455: /* required_intent_tag: TREF  */
#line 2711 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9506 "bison-chpl-lib.cpp"
    break;

  case 456: /* required_intent_tag: TTYPE  */
#line 2712 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9512 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_this_intent_tag: %empty  */
#line 2716 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9518 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_this_intent_tag: TPARAM  */
#line 2717 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9524 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_this_intent_tag: TREF  */
#line 2718 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9530 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_this_intent_tag: TCONST TREF  */
#line 2719 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9536 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_this_intent_tag: TCONST  */
#line 2720 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9542 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_this_intent_tag: TTYPE  */
#line 2721 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9548 "bison-chpl-lib.cpp"
    break;

  case 463: /* proc_iter_or_op: TPROC  */
#line 2725 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9554 "bison-chpl-lib.cpp"
    break;

  case 464: /* proc_iter_or_op: TITER  */
#line 2726 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9560 "bison-chpl-lib.cpp"
    break;

  case 465: /* proc_iter_or_op: TOPERATOR  */
#line 2727 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9566 "bison-chpl-lib.cpp"
    break;

  case 466: /* opt_ret_tag: %empty  */
#line 2731 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9572 "bison-chpl-lib.cpp"
    break;

  case 467: /* opt_ret_tag: TOUT  */
#line 2732 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 9578 "bison-chpl-lib.cpp"
    break;

  case 468: /* opt_ret_tag: TCONST  */
#line 2733 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9584 "bison-chpl-lib.cpp"
    break;

  case 469: /* opt_ret_tag: TCONST TREF  */
#line 2734 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9590 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_ret_tag: TREF  */
#line 2735 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9596 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_ret_tag: TPARAM  */
#line 2736 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9602 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_ret_tag: TTYPE  */
#line 2737 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9608 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_throws_error: %empty  */
#line 2741 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9614 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_throws_error: TTHROWS  */
#line 2742 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9620 "bison-chpl-lib.cpp"
    break;

  case 475: /* opt_function_body_stmt: TSEMI  */
#line 2745 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9626 "bison-chpl-lib.cpp"
    break;

  case 476: /* opt_function_body_stmt: function_body_stmt  */
#line 2746 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9632 "bison-chpl-lib.cpp"
    break;

  case 477: /* function_body_stmt: block_stmt_body  */
#line 2750 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9638 "bison-chpl-lib.cpp"
    break;

  case 478: /* function_body_stmt: TDO toplevel_stmt  */
#line 2751 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9644 "bison-chpl-lib.cpp"
    break;

  case 479: /* function_body_stmt: return_stmt  */
#line 2752 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9650 "bison-chpl-lib.cpp"
    break;

  case 480: /* query_expr: TQUERIEDIDENT  */
#line 2756 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9656 "bison-chpl-lib.cpp"
    break;

  case 481: /* opt_lifetime_where: %empty  */
#line 2761 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9662 "bison-chpl-lib.cpp"
    break;

  case 482: /* opt_lifetime_where: TWHERE expr  */
#line 2763 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9668 "bison-chpl-lib.cpp"
    break;

  case 483: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2765 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9674 "bison-chpl-lib.cpp"
    break;

  case 484: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2767 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9680 "bison-chpl-lib.cpp"
    break;

  case 485: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2769 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9686 "bison-chpl-lib.cpp"
    break;

  case 486: /* lifetime_components_expr: lifetime_expr  */
#line 2774 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9692 "bison-chpl-lib.cpp"
    break;

  case 487: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2776 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9698 "bison-chpl-lib.cpp"
    break;

  case 488: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2781 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9704 "bison-chpl-lib.cpp"
    break;

  case 489: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2783 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9710 "bison-chpl-lib.cpp"
    break;

  case 490: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2785 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9716 "bison-chpl-lib.cpp"
    break;

  case 491: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2787 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9722 "bison-chpl-lib.cpp"
    break;

  case 492: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2789 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9728 "bison-chpl-lib.cpp"
    break;

  case 493: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2791 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9734 "bison-chpl-lib.cpp"
    break;

  case 494: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2793 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9740 "bison-chpl-lib.cpp"
    break;

  case 495: /* lifetime_ident: TIDENT  */
#line 2797 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9746 "bison-chpl-lib.cpp"
    break;

  case 496: /* lifetime_ident: TTHIS  */
#line 2798 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9752 "bison-chpl-lib.cpp"
    break;

  case 497: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2803 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9761 "bison-chpl-lib.cpp"
    break;

  case 498: /* type_alias_decl_stmt_start: TTYPE  */
#line 2812 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9769 "bison-chpl-lib.cpp"
    break;

  case 499: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2816 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9778 "bison-chpl-lib.cpp"
    break;

  case 500: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2824 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9786 "bison-chpl-lib.cpp"
    break;

  case 501: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2828 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9794 "bison-chpl-lib.cpp"
    break;

  case 502: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2835 "chpl.ypp"
  {
    // TODO (dlongnecke-cray): Add a helper to build this and var_decl_stmt.
    auto node = Variable::build(BUILDER, LOC((yyloc)),
                                /*attributeGroup*/ nullptr,
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
#line 9816 "bison-chpl-lib.cpp"
    break;

  case 503: /* opt_init_type: %empty  */
#line 2855 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9822 "bison-chpl-lib.cpp"
    break;

  case 504: /* opt_init_type: TASSIGN expr  */
#line 2857 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9828 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_type: TPARAM  */
#line 2861 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9834 "bison-chpl-lib.cpp"
    break;

  case 506: /* var_decl_type: TCONST TREF  */
#line 2862 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9840 "bison-chpl-lib.cpp"
    break;

  case 507: /* var_decl_type: TREF  */
#line 2863 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9846 "bison-chpl-lib.cpp"
    break;

  case 508: /* var_decl_type: TCONST  */
#line 2864 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9852 "bison-chpl-lib.cpp"
    break;

  case 509: /* var_decl_type: TVAR  */
#line 2865 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9858 "bison-chpl-lib.cpp"
    break;

  case 510: /* $@11: %empty  */
#line 2870 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9867 "bison-chpl-lib.cpp"
    break;

  case 511: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2874 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9876 "bison-chpl-lib.cpp"
    break;

  case 512: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2879 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9885 "bison-chpl-lib.cpp"
    break;

  case 513: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2887 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9893 "bison-chpl-lib.cpp"
    break;

  case 514: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2891 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9901 "bison-chpl-lib.cpp"
    break;

  case 515: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2898 "chpl.ypp"
    {
      // delay building the attributes for a variable, then update it later
      // in the context during the call to buildVarOrMultiDeclStmt
      auto varDecl = Variable::build(BUILDER, LOC((yyloc)),
                                     /*attributGroup*/nullptr,
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
#line 9923 "bison-chpl-lib.cpp"
    break;

  case 516: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2916 "chpl.ypp"
    {
      auto intentOrKind = (TupleDecl::IntentOrKind) context->varDeclKind;
      // delay building the attributes for a TupleDecl, then update it later
      // in the context during the call to buildVarOrMultiDeclStmt
      auto tupleDecl = TupleDecl::build(BUILDER, LOC((yyloc)),
                                        /*attributeGroup*/ nullptr,
                                        context->visibility,
                                        context->linkage,
                                        intentOrKind,
                                        context->consumeList((yyvsp[-3].exprList)),
                                        toOwned((yyvsp[-1].expr)),
                                        toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yylsp[-4]), tupleDecl.release());
      context->clearComments();
    }
#line 9943 "bison-chpl-lib.cpp"
    break;

  case 517: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2935 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9951 "bison-chpl-lib.cpp"
    break;

  case 518: /* tuple_var_decl_component: ident_def  */
#line 2939 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9959 "bison-chpl-lib.cpp"
    break;

  case 519: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2943 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9967 "bison-chpl-lib.cpp"
    break;

  case 520: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2950 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9973 "bison-chpl-lib.cpp"
    break;

  case 521: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2952 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9979 "bison-chpl-lib.cpp"
    break;

  case 522: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2954 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9985 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_init_expr: %empty  */
#line 2960 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9991 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2961 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9997 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2962 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10003 "bison-chpl-lib.cpp"
    break;

  case 526: /* formal_or_ret_type_expr: expr  */
#line 2966 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 10009 "bison-chpl-lib.cpp"
    break;

  case 527: /* ret_type: formal_or_ret_type_expr  */
#line 2970 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 10015 "bison-chpl-lib.cpp"
    break;

  case 528: /* ret_type: reserved_type_ident_use  */
#line 2971 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10021 "bison-chpl-lib.cpp"
    break;

  case 529: /* ret_type: error  */
#line 2972 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10027 "bison-chpl-lib.cpp"
    break;

  case 530: /* colon_ret_type: TCOLON ret_type  */
#line 2976 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10033 "bison-chpl-lib.cpp"
    break;

  case 531: /* colon_ret_type: error  */
#line 2977 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10039 "bison-chpl-lib.cpp"
    break;

  case 532: /* opt_ret_type: %empty  */
#line 2981 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 10045 "bison-chpl-lib.cpp"
    break;

  case 534: /* opt_type: %empty  */
#line 2986 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 10051 "bison-chpl-lib.cpp"
    break;

  case 535: /* opt_type: TCOLON expr  */
#line 2987 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10057 "bison-chpl-lib.cpp"
    break;

  case 536: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2988 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10063 "bison-chpl-lib.cpp"
    break;

  case 537: /* opt_type: error  */
#line 2989 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 10069 "bison-chpl-lib.cpp"
    break;

  case 538: /* formal_type: formal_or_ret_type_expr  */
#line 2993 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 10075 "bison-chpl-lib.cpp"
    break;

  case 539: /* formal_type: reserved_type_ident_use  */
#line 2994 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10081 "bison-chpl-lib.cpp"
    break;

  case 540: /* colon_formal_type: TCOLON formal_type  */
#line 2998 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10087 "bison-chpl-lib.cpp"
    break;

  case 541: /* opt_colon_formal_type: %empty  */
#line 3002 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 10093 "bison-chpl-lib.cpp"
    break;

  case 542: /* opt_colon_formal_type: colon_formal_type  */
#line 3003 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 10099 "bison-chpl-lib.cpp"
    break;

  case 543: /* expr_ls: expr  */
#line 3009 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10105 "bison-chpl-lib.cpp"
    break;

  case 544: /* expr_ls: expr_ls TCOMMA expr  */
#line 3010 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10111 "bison-chpl-lib.cpp"
    break;

  case 545: /* tuple_component: TUNDERSCORE  */
#line 3014 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10117 "bison-chpl-lib.cpp"
    break;

  case 546: /* tuple_component: opt_try_expr  */
#line 3015 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 10123 "bison-chpl-lib.cpp"
    break;

  case 547: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 3020 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 10131 "bison-chpl-lib.cpp"
    break;

  case 548: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 3024 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10139 "bison-chpl-lib.cpp"
    break;

  case 549: /* opt_actual_ls: %empty  */
#line 3030 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 10145 "bison-chpl-lib.cpp"
    break;

  case 550: /* opt_actual_ls: actual_ls  */
#line 3031 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 10151 "bison-chpl-lib.cpp"
    break;

  case 551: /* actual_ls: actual_expr  */
#line 3036 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10160 "bison-chpl-lib.cpp"
    break;

  case 552: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 3041 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 10170 "bison-chpl-lib.cpp"
    break;

  case 553: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 3049 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 10176 "bison-chpl-lib.cpp"
    break;

  case 554: /* actual_expr: opt_try_expr  */
#line 3050 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 10182 "bison-chpl-lib.cpp"
    break;

  case 555: /* ident_expr: ident_use  */
#line 3054 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10188 "bison-chpl-lib.cpp"
    break;

  case 556: /* ident_expr: scalar_type  */
#line 3055 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 10194 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSINGLE expr  */
#line 3068 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3070 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10206 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3072 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10212 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3074 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3076 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 10228 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TATOMIC expr  */
#line 3082 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10234 "bison-chpl-lib.cpp"
    break;

  case 568: /* sub_type_level_expr: TSYNC expr  */
#line 3084 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10240 "bison-chpl-lib.cpp"
    break;

  case 569: /* sub_type_level_expr: TOWNED  */
#line 3087 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10246 "bison-chpl-lib.cpp"
    break;

  case 570: /* sub_type_level_expr: TOWNED expr  */
#line 3089 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10252 "bison-chpl-lib.cpp"
    break;

  case 571: /* sub_type_level_expr: TUNMANAGED  */
#line 3091 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10258 "bison-chpl-lib.cpp"
    break;

  case 572: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3093 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10264 "bison-chpl-lib.cpp"
    break;

  case 573: /* sub_type_level_expr: TSHARED  */
#line 3095 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10270 "bison-chpl-lib.cpp"
    break;

  case 574: /* sub_type_level_expr: TSHARED expr  */
#line 3097 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10276 "bison-chpl-lib.cpp"
    break;

  case 575: /* sub_type_level_expr: TBORROWED  */
#line 3099 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10282 "bison-chpl-lib.cpp"
    break;

  case 576: /* sub_type_level_expr: TBORROWED expr  */
#line 3101 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10288 "bison-chpl-lib.cpp"
    break;

  case 577: /* sub_type_level_expr: TCLASS  */
#line 3103 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10294 "bison-chpl-lib.cpp"
    break;

  case 578: /* sub_type_level_expr: TRECORD  */
#line 3105 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 10300 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr: no_loop_attributes for_expr_base  */
#line 3109 "chpl.ypp"
                                   { (yyval.expr) = (yyvsp[0].expr); }
#line 10306 "bison-chpl-lib.cpp"
    break;

  case 580: /* for_expr_base: TFOR expr TIN expr TDO expr  */
#line 3114 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10320 "bison-chpl-lib.cpp"
    break;

  case 581: /* for_expr_base: TFOR expr TIN zippered_iterator TDO expr  */
#line 3124 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10334 "bison-chpl-lib.cpp"
    break;

  case 582: /* for_expr_base: TFOR expr TDO expr  */
#line 3134 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10347 "bison-chpl-lib.cpp"
    break;

  case 583: /* for_expr_base: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3143 "chpl.ypp"
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
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10366 "bison-chpl-lib.cpp"
    break;

  case 584: /* for_expr_base: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3158 "chpl.ypp"
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
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10385 "bison-chpl-lib.cpp"
    break;

  case 585: /* for_expr_base: TFOR expr TDO TIF expr TTHEN expr  */
#line 3173 "chpl.ypp"
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
                    /*isParam*/ false,
                    context->popLoopAttributeGroup()).release();
  }
#line 10404 "bison-chpl-lib.cpp"
    break;

  case 586: /* for_expr_base: TFORALL expr TIN expr TDO expr  */
#line 3188 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10418 "bison-chpl-lib.cpp"
    break;

  case 587: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3198 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10432 "bison-chpl-lib.cpp"
    break;

  case 588: /* for_expr_base: TFORALL expr TDO expr  */
#line 3208 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10445 "bison-chpl-lib.cpp"
    break;

  case 589: /* for_expr_base: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3217 "chpl.ypp"
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
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10464 "bison-chpl-lib.cpp"
    break;

  case 590: /* for_expr_base: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3232 "chpl.ypp"
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
                      /*isExpressionLevel*/ true,
                      context->popLoopAttributeGroup()).release();
  }
#line 10483 "bison-chpl-lib.cpp"
    break;

  case 591: /* for_expr_base: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3247 "chpl.ypp"
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
                       /*isExpressionLevel*/ true,
                       context->popLoopAttributeGroup()).release();
  }
#line 10502 "bison-chpl-lib.cpp"
    break;

  case 592: /* bracket_loop_expr: no_loop_attributes bracket_loop_expr_base  */
#line 3264 "chpl.ypp"
                                            { (yyval.expr) = (yyvsp[0].expr); context->popLoopAttributeGroup(); }
#line 10508 "bison-chpl-lib.cpp"
    break;

  case 593: /* bracket_loop_expr_base: TLSBR TRSBR  */
#line 3269 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10516 "bison-chpl-lib.cpp"
    break;

  case 594: /* bracket_loop_expr_base: TLSBR TRSBR expr  */
#line 3273 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10524 "bison-chpl-lib.cpp"
    break;

  case 595: /* bracket_loop_expr_base: TLSBR expr_ls TRSBR expr  */
#line 3277 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10532 "bison-chpl-lib.cpp"
    break;

  case 596: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3281 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10540 "bison-chpl-lib.cpp"
    break;

  case 597: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3285 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10548 "bison-chpl-lib.cpp"
    break;

  case 598: /* bracket_loop_expr_base: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3289 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10556 "bison-chpl-lib.cpp"
    break;

  case 599: /* bracket_loop_expr_base: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3293 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10564 "bison-chpl-lib.cpp"
    break;

  case 600: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3300 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10578 "bison-chpl-lib.cpp"
    break;

  case 601: /* nil_expr: TNIL  */
#line 3317 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10584 "bison-chpl-lib.cpp"
    break;

  case 609: /* opt_task_intent_ls: %empty  */
#line 3335 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10590 "bison-chpl-lib.cpp"
    break;

  case 610: /* opt_task_intent_ls: task_intent_clause  */
#line 3336 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10596 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3341 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10606 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_intent_ls: intent_expr  */
#line 3349 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10612 "bison-chpl-lib.cpp"
    break;

  case 613: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3350 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10618 "bison-chpl-lib.cpp"
    break;

  case 614: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3355 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 615: /* forall_intent_ls: intent_expr  */
#line 3363 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10634 "bison-chpl-lib.cpp"
    break;

  case 616: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3364 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10640 "bison-chpl-lib.cpp"
    break;

  case 617: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3369 "chpl.ypp"
  {
    if (auto ident = (yyvsp[-2].expr)->toIdentifier()) {
      auto name = ident->name();
      auto node = TaskVar::build(BUILDER, LOC((yyloc)), /*attributeGroup*/ nullptr,
                                 name,
                                 /*intent*/ (yyvsp[-3].taskIntent),
                                 toOwned((yyvsp[-1].expr)),
                                 toOwned((yyvsp[0].expr)));
      (yyval.expr) = node.release();
    } else {
      (yyval.expr) = context->syntax((yyloc), "expected identifier for task variable name.");
    }
  }
#line 10658 "bison-chpl-lib.cpp"
    break;

  case 618: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3383 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10666 "bison-chpl-lib.cpp"
    break;

  case 619: /* intent_expr: expr TREDUCE ident_expr  */
#line 3387 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10674 "bison-chpl-lib.cpp"
    break;

  case 620: /* task_var_prefix: TCONST  */
#line 3393 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10680 "bison-chpl-lib.cpp"
    break;

  case 621: /* task_var_prefix: TIN  */
#line 3394 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10686 "bison-chpl-lib.cpp"
    break;

  case 622: /* task_var_prefix: TCONST TIN  */
#line 3395 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10692 "bison-chpl-lib.cpp"
    break;

  case 623: /* task_var_prefix: TREF  */
#line 3396 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10698 "bison-chpl-lib.cpp"
    break;

  case 624: /* task_var_prefix: TCONST TREF  */
#line 3397 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10704 "bison-chpl-lib.cpp"
    break;

  case 625: /* task_var_prefix: TVAR  */
#line 3398 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10710 "bison-chpl-lib.cpp"
    break;

  case 626: /* new_maybe_decorated: TNEW  */
#line 3403 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10716 "bison-chpl-lib.cpp"
    break;

  case 627: /* new_maybe_decorated: TNEW TOWNED  */
#line 3405 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10722 "bison-chpl-lib.cpp"
    break;

  case 628: /* new_maybe_decorated: TNEW TSHARED  */
#line 3407 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10728 "bison-chpl-lib.cpp"
    break;

  case 629: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3409 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10734 "bison-chpl-lib.cpp"
    break;

  case 630: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3411 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10740 "bison-chpl-lib.cpp"
    break;

  case 631: /* new_expr: new_maybe_decorated expr  */
#line 3417 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10748 "bison-chpl-lib.cpp"
    break;

  case 632: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3424 "chpl.ypp"
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
#line 10764 "bison-chpl-lib.cpp"
    break;

  case 633: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3436 "chpl.ypp"
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
#line 10780 "bison-chpl-lib.cpp"
    break;

  case 634: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3448 "chpl.ypp"
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
#line 10797 "bison-chpl-lib.cpp"
    break;

  case 635: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3461 "chpl.ypp"
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
#line 10815 "bison-chpl-lib.cpp"
    break;

  case 636: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3478 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10823 "bison-chpl-lib.cpp"
    break;

  case 637: /* range_literal_expr: expr TDOTDOT expr  */
#line 3485 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10832 "bison-chpl-lib.cpp"
    break;

  case 638: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3490 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10841 "bison-chpl-lib.cpp"
    break;

  case 639: /* range_literal_expr: expr TDOTDOT  */
#line 3495 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10850 "bison-chpl-lib.cpp"
    break;

  case 640: /* range_literal_expr: TDOTDOT expr  */
#line 3500 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10860 "bison-chpl-lib.cpp"
    break;

  case 641: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3506 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10870 "bison-chpl-lib.cpp"
    break;

  case 642: /* range_literal_expr: TDOTDOT  */
#line 3512 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10880 "bison-chpl-lib.cpp"
    break;

  case 643: /* cast_expr: expr TCOLON expr  */
#line 3542 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10888 "bison-chpl-lib.cpp"
    break;

  case 644: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3549 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10896 "bison-chpl-lib.cpp"
    break;

  case 645: /* super_expr: fn_expr  */
#line 3555 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10902 "bison-chpl-lib.cpp"
    break;

  case 649: /* expr: sub_type_level_expr TQUESTION  */
#line 3564 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10908 "bison-chpl-lib.cpp"
    break;

  case 650: /* expr: TQUESTION  */
#line 3566 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10914 "bison-chpl-lib.cpp"
    break;

  case 654: /* expr: fn_type  */
#line 3571 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10920 "bison-chpl-lib.cpp"
    break;

  case 664: /* opt_expr: %empty  */
#line 3585 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10926 "bison-chpl-lib.cpp"
    break;

  case 665: /* opt_expr: expr  */
#line 3586 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10932 "bison-chpl-lib.cpp"
    break;

  case 666: /* opt_try_expr: TTRY expr  */
#line 3590 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10938 "bison-chpl-lib.cpp"
    break;

  case 667: /* opt_try_expr: TTRYBANG expr  */
#line 3591 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10944 "bison-chpl-lib.cpp"
    break;

  case 668: /* opt_try_expr: super_expr  */
#line 3592 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10950 "bison-chpl-lib.cpp"
    break;

  case 674: /* call_base_expr: expr TBANG  */
#line 3609 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10958 "bison-chpl-lib.cpp"
    break;

  case 675: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3612 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10964 "bison-chpl-lib.cpp"
    break;

  case 678: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3619 "chpl.ypp"
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
#line 10980 "bison-chpl-lib.cpp"
    break;

  case 679: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3631 "chpl.ypp"
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
#line 10996 "bison-chpl-lib.cpp"
    break;

  case 680: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3643 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 11004 "bison-chpl-lib.cpp"
    break;

  case 681: /* dot_expr: expr TDOT ident_use  */
#line 3650 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11010 "bison-chpl-lib.cpp"
    break;

  case 682: /* dot_expr: expr TDOT TTYPE  */
#line 3652 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11016 "bison-chpl-lib.cpp"
    break;

  case 683: /* dot_expr: expr TDOT TDOMAIN  */
#line 3654 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11022 "bison-chpl-lib.cpp"
    break;

  case 684: /* dot_expr: expr TDOT TLOCALE  */
#line 3656 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 11028 "bison-chpl-lib.cpp"
    break;

  case 685: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3658 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 11038 "bison-chpl-lib.cpp"
    break;

  case 686: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3664 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 11048 "bison-chpl-lib.cpp"
    break;

  case 687: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3676 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 11054 "bison-chpl-lib.cpp"
    break;

  case 688: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3678 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 11062 "bison-chpl-lib.cpp"
    break;

  case 689: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3682 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11070 "bison-chpl-lib.cpp"
    break;

  case 690: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3686 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11078 "bison-chpl-lib.cpp"
    break;

  case 691: /* bool_literal: TFALSE  */
#line 3692 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 11084 "bison-chpl-lib.cpp"
    break;

  case 692: /* bool_literal: TTRUE  */
#line 3693 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 11090 "bison-chpl-lib.cpp"
    break;

  case 693: /* str_bytes_literal: STRINGLITERAL  */
#line 3697 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11096 "bison-chpl-lib.cpp"
    break;

  case 694: /* str_bytes_literal: BYTESLITERAL  */
#line 3698 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 11102 "bison-chpl-lib.cpp"
    break;

  case 697: /* literal_expr: INTLITERAL  */
#line 3704 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 11108 "bison-chpl-lib.cpp"
    break;

  case 698: /* literal_expr: REALLITERAL  */
#line 3705 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 11114 "bison-chpl-lib.cpp"
    break;

  case 699: /* literal_expr: IMAGLITERAL  */
#line 3706 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 11120 "bison-chpl-lib.cpp"
    break;

  case 700: /* literal_expr: CSTRINGLITERAL  */
#line 3707 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 11126 "bison-chpl-lib.cpp"
    break;

  case 701: /* literal_expr: TNONE  */
#line 3708 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 11132 "bison-chpl-lib.cpp"
    break;

  case 702: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3710 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 11141 "bison-chpl-lib.cpp"
    break;

  case 703: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3715 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 11150 "bison-chpl-lib.cpp"
    break;

  case 704: /* literal_expr: no_loop_attributes TLSBR expr_ls TRSBR  */
#line 3720 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
    context->popLoopAttributeGroup();
  }
#line 11159 "bison-chpl-lib.cpp"
    break;

  case 705: /* literal_expr: no_loop_attributes TLSBR expr_ls TCOMMA TRSBR  */
#line 3725 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
    context->popLoopAttributeGroup();
  }
#line 11168 "bison-chpl-lib.cpp"
    break;

  case 706: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TRSBR  */
#line 3730 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
    context->popLoopAttributeGroup();
  }
#line 11177 "bison-chpl-lib.cpp"
    break;

  case 707: /* literal_expr: no_loop_attributes TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3735 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
    context->popLoopAttributeGroup();
  }
#line 11186 "bison-chpl-lib.cpp"
    break;

  case 708: /* assoc_expr_ls: expr TALIAS expr  */
#line 3744 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 11195 "bison-chpl-lib.cpp"
    break;

  case 709: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3749 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 11205 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TPLUS expr  */
#line 3757 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11211 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TMINUS expr  */
#line 3758 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11217 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TSTAR expr  */
#line 3759 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11223 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TDIVIDE expr  */
#line 3760 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11229 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3761 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11235 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3762 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11241 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TMOD expr  */
#line 3763 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11247 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TEQUAL expr  */
#line 3764 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11253 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3765 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11259 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3766 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11265 "bison-chpl-lib.cpp"
    break;

  case 720: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3767 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11271 "bison-chpl-lib.cpp"
    break;

  case 721: /* binary_op_expr: expr TLESS expr  */
#line 3768 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11277 "bison-chpl-lib.cpp"
    break;

  case 722: /* binary_op_expr: expr TGREATER expr  */
#line 3769 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11283 "bison-chpl-lib.cpp"
    break;

  case 723: /* binary_op_expr: expr TBAND expr  */
#line 3770 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11289 "bison-chpl-lib.cpp"
    break;

  case 724: /* binary_op_expr: expr TBOR expr  */
#line 3771 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11295 "bison-chpl-lib.cpp"
    break;

  case 725: /* binary_op_expr: expr TBXOR expr  */
#line 3772 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11301 "bison-chpl-lib.cpp"
    break;

  case 726: /* binary_op_expr: expr TAND expr  */
#line 3773 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11307 "bison-chpl-lib.cpp"
    break;

  case 727: /* binary_op_expr: expr TOR expr  */
#line 3774 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11313 "bison-chpl-lib.cpp"
    break;

  case 728: /* binary_op_expr: expr TEXP expr  */
#line 3775 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11319 "bison-chpl-lib.cpp"
    break;

  case 729: /* binary_op_expr: expr TBY expr  */
#line 3776 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11325 "bison-chpl-lib.cpp"
    break;

  case 730: /* binary_op_expr: expr TALIGN expr  */
#line 3777 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11331 "bison-chpl-lib.cpp"
    break;

  case 731: /* binary_op_expr: expr THASH expr  */
#line 3778 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11337 "bison-chpl-lib.cpp"
    break;

  case 732: /* binary_op_expr: expr TDMAPPED expr  */
#line 3779 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11343 "bison-chpl-lib.cpp"
    break;

  case 733: /* unary_op_expr: TPLUS expr  */
#line 3783 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11349 "bison-chpl-lib.cpp"
    break;

  case 734: /* unary_op_expr: TMINUS expr  */
#line 3784 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11355 "bison-chpl-lib.cpp"
    break;

  case 735: /* unary_op_expr: TMINUSMINUS expr  */
#line 3785 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11361 "bison-chpl-lib.cpp"
    break;

  case 736: /* unary_op_expr: TPLUSPLUS expr  */
#line 3786 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11367 "bison-chpl-lib.cpp"
    break;

  case 737: /* unary_op_expr: TBANG expr  */
#line 3787 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11373 "bison-chpl-lib.cpp"
    break;

  case 738: /* unary_op_expr: expr TBANG  */
#line 3788 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 11381 "bison-chpl-lib.cpp"
    break;

  case 739: /* unary_op_expr: TBNOT expr  */
#line 3791 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 11387 "bison-chpl-lib.cpp"
    break;

  case 740: /* reduce_expr: expr TREDUCE expr  */
#line 3796 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11395 "bison-chpl-lib.cpp"
    break;

  case 741: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3800 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11403 "bison-chpl-lib.cpp"
    break;

  case 742: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3804 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11411 "bison-chpl-lib.cpp"
    break;

  case 743: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3808 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11419 "bison-chpl-lib.cpp"
    break;

  case 744: /* scan_expr: expr TSCAN expr  */
#line 3815 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11427 "bison-chpl-lib.cpp"
    break;

  case 745: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3819 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 11435 "bison-chpl-lib.cpp"
    break;

  case 746: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3823 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11443 "bison-chpl-lib.cpp"
    break;

  case 747: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3827 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 11451 "bison-chpl-lib.cpp"
    break;


#line 11455 "bison-chpl-lib.cpp"

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
