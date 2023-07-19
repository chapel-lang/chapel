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
  YYSYMBOL_attribute_receiver = 190,       /* attribute_receiver  */
  YYSYMBOL_attribute_group = 191,          /* attribute_group  */
  YYSYMBOL_attribute_decl_stmt_ls = 192,   /* attribute_decl_stmt_ls  */
  YYSYMBOL_attribute_decl_stmt = 193,      /* attribute_decl_stmt  */
  YYSYMBOL_attribute_decl_begin = 194,     /* attribute_decl_begin  */
  YYSYMBOL_opt_attribute_decl_toolspace = 195, /* opt_attribute_decl_toolspace  */
  YYSYMBOL_opt_attribute_actuals = 196,    /* opt_attribute_actuals  */
  YYSYMBOL_attribute_actuals_ls = 197,     /* attribute_actuals_ls  */
  YYSYMBOL_attribute_actual = 198,         /* attribute_actual  */
  YYSYMBOL_module_decl_start = 199,        /* module_decl_start  */
  YYSYMBOL_module_decl_stmt = 200,         /* module_decl_stmt  */
  YYSYMBOL_opt_access_control = 201,       /* opt_access_control  */
  YYSYMBOL_opt_prototype = 202,            /* opt_prototype  */
  YYSYMBOL_include_module_stmt = 203,      /* include_module_stmt  */
  YYSYMBOL_block_stmt_body = 204,          /* block_stmt_body  */
  YYSYMBOL_block_stmt = 205,               /* block_stmt  */
  YYSYMBOL_stmt_ls = 206,                  /* stmt_ls  */
  YYSYMBOL_renames_ls = 207,               /* renames_ls  */
  YYSYMBOL_use_renames_ls = 208,           /* use_renames_ls  */
  YYSYMBOL_opt_only_ls = 209,              /* opt_only_ls  */
  YYSYMBOL_use_stmt = 210,                 /* use_stmt  */
  YYSYMBOL_import_stmt = 211,              /* import_stmt  */
  YYSYMBOL_import_internal_type_ident = 212, /* import_internal_type_ident  */
  YYSYMBOL_import_expr = 213,              /* import_expr  */
  YYSYMBOL_import_ls = 214,                /* import_ls  */
  YYSYMBOL_require_stmt = 215,             /* require_stmt  */
  YYSYMBOL_assignment_stmt = 216,          /* assignment_stmt  */
  YYSYMBOL_opt_label_ident = 217,          /* opt_label_ident  */
  YYSYMBOL_reserved_word_ident = 218,      /* reserved_word_ident  */
  YYSYMBOL_ident_def = 219,                /* ident_def  */
  YYSYMBOL_ident_use = 220,                /* ident_use  */
  YYSYMBOL_internal_type_ident_def = 221,  /* internal_type_ident_def  */
  YYSYMBOL_scalar_type = 222,              /* scalar_type  */
  YYSYMBOL_reserved_type_ident_use = 223,  /* reserved_type_ident_use  */
  YYSYMBOL_do_stmt = 224,                  /* do_stmt  */
  YYSYMBOL_return_stmt = 225,              /* return_stmt  */
  YYSYMBOL_class_level_stmt = 226,         /* class_level_stmt  */
  YYSYMBOL_227_1 = 227,                    /* $@1  */
  YYSYMBOL_228_2 = 228,                    /* $@2  */
  YYSYMBOL_inner_class_level_stmt = 229,   /* inner_class_level_stmt  */
  YYSYMBOL_forwarding_decl_stmt = 230,     /* forwarding_decl_stmt  */
  YYSYMBOL_forwarding_decl_start = 231,    /* forwarding_decl_start  */
  YYSYMBOL_extern_or_export = 232,         /* extern_or_export  */
  YYSYMBOL_extern_export_decl_stmt_start = 233, /* extern_export_decl_stmt_start  */
  YYSYMBOL_extern_export_decl_stmt = 234,  /* extern_export_decl_stmt  */
  YYSYMBOL_235_3 = 235,                    /* $@3  */
  YYSYMBOL_236_4 = 236,                    /* $@4  */
  YYSYMBOL_extern_block_stmt = 237,        /* extern_block_stmt  */
  YYSYMBOL_loop_stmt = 238,                /* loop_stmt  */
  YYSYMBOL_zippered_iterator = 239,        /* zippered_iterator  */
  YYSYMBOL_if_stmt = 240,                  /* if_stmt  */
  YYSYMBOL_ifvar = 241,                    /* ifvar  */
  YYSYMBOL_interface_stmt = 242,           /* interface_stmt  */
  YYSYMBOL_ifc_formal_ls = 243,            /* ifc_formal_ls  */
  YYSYMBOL_ifc_formal = 244,               /* ifc_formal  */
  YYSYMBOL_implements_type_ident = 245,    /* implements_type_ident  */
  YYSYMBOL_implements_type_error_ident = 246, /* implements_type_error_ident  */
  YYSYMBOL_implements_stmt = 247,          /* implements_stmt  */
  YYSYMBOL_ifc_constraint = 248,           /* ifc_constraint  */
  YYSYMBOL_try_stmt = 249,                 /* try_stmt  */
  YYSYMBOL_catch_expr_ls = 250,            /* catch_expr_ls  */
  YYSYMBOL_catch_expr = 251,               /* catch_expr  */
  YYSYMBOL_catch_expr_inner = 252,         /* catch_expr_inner  */
  YYSYMBOL_throw_stmt = 253,               /* throw_stmt  */
  YYSYMBOL_select_stmt = 254,              /* select_stmt  */
  YYSYMBOL_when_stmt_ls = 255,             /* when_stmt_ls  */
  YYSYMBOL_when_stmt = 256,                /* when_stmt  */
  YYSYMBOL_manager_expr = 257,             /* manager_expr  */
  YYSYMBOL_manager_expr_ls = 258,          /* manager_expr_ls  */
  YYSYMBOL_manage_stmt = 259,              /* manage_stmt  */
  YYSYMBOL_class_decl_stmt = 260,          /* class_decl_stmt  */
  YYSYMBOL_class_start = 261,              /* class_start  */
  YYSYMBOL_class_tag = 262,                /* class_tag  */
  YYSYMBOL_opt_inherit = 263,              /* opt_inherit  */
  YYSYMBOL_class_level_stmt_ls = 264,      /* class_level_stmt_ls  */
  YYSYMBOL_enum_decl_stmt = 265,           /* enum_decl_stmt  */
  YYSYMBOL_enum_header_lcbr = 266,         /* enum_header_lcbr  */
  YYSYMBOL_enum_ls = 267,                  /* enum_ls  */
  YYSYMBOL_268_5 = 268,                    /* $@5  */
  YYSYMBOL_269_6 = 269,                    /* $@6  */
  YYSYMBOL_enum_item = 270,                /* enum_item  */
  YYSYMBOL_lambda_decl_start = 271,        /* lambda_decl_start  */
  YYSYMBOL_lambda_decl_expr = 272,         /* lambda_decl_expr  */
  YYSYMBOL_273_7 = 273,                    /* $@7  */
  YYSYMBOL_linkage_spec_empty = 274,       /* linkage_spec_empty  */
  YYSYMBOL_linkage_spec = 275,             /* linkage_spec  */
  YYSYMBOL_opt_fn_type_formal_ls = 276,    /* opt_fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal_ls = 277,        /* fn_type_formal_ls  */
  YYSYMBOL_fn_type_formal = 278,           /* fn_type_formal  */
  YYSYMBOL_opt_fn_type_ret_type = 279,     /* opt_fn_type_ret_type  */
  YYSYMBOL_fn_type = 280,                  /* fn_type  */
  YYSYMBOL_fn_expr = 281,                  /* fn_expr  */
  YYSYMBOL_282_8 = 282,                    /* $@8  */
  YYSYMBOL_283_9 = 283,                    /* $@9  */
  YYSYMBOL_fn_decl_stmt_complete = 284,    /* fn_decl_stmt_complete  */
  YYSYMBOL_fn_decl_stmt = 285,             /* fn_decl_stmt  */
  YYSYMBOL_286_10 = 286,                   /* $@10  */
  YYSYMBOL_fn_decl_stmt_inner = 287,       /* fn_decl_stmt_inner  */
  YYSYMBOL_fn_decl_stmt_start = 288,       /* fn_decl_stmt_start  */
  YYSYMBOL_fn_decl_receiver_expr = 289,    /* fn_decl_receiver_expr  */
  YYSYMBOL_fn_ident = 290,                 /* fn_ident  */
  YYSYMBOL_op_ident = 291,                 /* op_ident  */
  YYSYMBOL_assignop_ident = 292,           /* assignop_ident  */
  YYSYMBOL_all_op_name = 293,              /* all_op_name  */
  YYSYMBOL_formal_var_arg_expr = 294,      /* formal_var_arg_expr  */
  YYSYMBOL_opt_formal_ls = 295,            /* opt_formal_ls  */
  YYSYMBOL_req_formal_ls = 296,            /* req_formal_ls  */
  YYSYMBOL_formal_ls_inner = 297,          /* formal_ls_inner  */
  YYSYMBOL_formal_ls = 298,                /* formal_ls  */
  YYSYMBOL_formal = 299,                   /* formal  */
  YYSYMBOL_formal_ident_def = 300,         /* formal_ident_def  */
  YYSYMBOL_named_formal = 301,             /* named_formal  */
  YYSYMBOL_opt_formal_intent_tag = 302,    /* opt_formal_intent_tag  */
  YYSYMBOL_required_intent_tag = 303,      /* required_intent_tag  */
  YYSYMBOL_opt_this_intent_tag = 304,      /* opt_this_intent_tag  */
  YYSYMBOL_proc_iter_or_op = 305,          /* proc_iter_or_op  */
  YYSYMBOL_opt_ret_tag = 306,              /* opt_ret_tag  */
  YYSYMBOL_opt_throws_error = 307,         /* opt_throws_error  */
  YYSYMBOL_opt_function_body_stmt = 308,   /* opt_function_body_stmt  */
  YYSYMBOL_function_body_stmt = 309,       /* function_body_stmt  */
  YYSYMBOL_query_expr = 310,               /* query_expr  */
  YYSYMBOL_opt_lifetime_where = 311,       /* opt_lifetime_where  */
  YYSYMBOL_lifetime_components_expr = 312, /* lifetime_components_expr  */
  YYSYMBOL_lifetime_expr = 313,            /* lifetime_expr  */
  YYSYMBOL_lifetime_ident = 314,           /* lifetime_ident  */
  YYSYMBOL_type_alias_decl_stmt = 315,     /* type_alias_decl_stmt  */
  YYSYMBOL_type_alias_decl_stmt_start = 316, /* type_alias_decl_stmt_start  */
  YYSYMBOL_type_alias_decl_stmt_inner_ls = 317, /* type_alias_decl_stmt_inner_ls  */
  YYSYMBOL_type_alias_decl_stmt_inner = 318, /* type_alias_decl_stmt_inner  */
  YYSYMBOL_opt_init_type = 319,            /* opt_init_type  */
  YYSYMBOL_var_decl_type = 320,            /* var_decl_type  */
  YYSYMBOL_var_decl_stmt = 321,            /* var_decl_stmt  */
  YYSYMBOL_322_11 = 322,                   /* $@11  */
  YYSYMBOL_var_decl_stmt_inner_ls = 323,   /* var_decl_stmt_inner_ls  */
  YYSYMBOL_var_decl_stmt_inner = 324,      /* var_decl_stmt_inner  */
  YYSYMBOL_tuple_var_decl_component = 325, /* tuple_var_decl_component  */
  YYSYMBOL_tuple_var_decl_stmt_inner_ls = 326, /* tuple_var_decl_stmt_inner_ls  */
  YYSYMBOL_opt_init_expr = 327,            /* opt_init_expr  */
  YYSYMBOL_formal_or_ret_type_expr = 328,  /* formal_or_ret_type_expr  */
  YYSYMBOL_ret_type = 329,                 /* ret_type  */
  YYSYMBOL_colon_ret_type = 330,           /* colon_ret_type  */
  YYSYMBOL_opt_ret_type = 331,             /* opt_ret_type  */
  YYSYMBOL_opt_type = 332,                 /* opt_type  */
  YYSYMBOL_formal_type = 333,              /* formal_type  */
  YYSYMBOL_colon_formal_type = 334,        /* colon_formal_type  */
  YYSYMBOL_opt_colon_formal_type = 335,    /* opt_colon_formal_type  */
  YYSYMBOL_expr_ls = 336,                  /* expr_ls  */
  YYSYMBOL_tuple_component = 337,          /* tuple_component  */
  YYSYMBOL_tuple_expr_ls = 338,            /* tuple_expr_ls  */
  YYSYMBOL_opt_actual_ls = 339,            /* opt_actual_ls  */
  YYSYMBOL_actual_ls = 340,                /* actual_ls  */
  YYSYMBOL_actual_expr = 341,              /* actual_expr  */
  YYSYMBOL_ident_expr = 342,               /* ident_expr  */
  YYSYMBOL_sub_type_level_expr = 343,      /* sub_type_level_expr  */
  YYSYMBOL_for_expr = 344,                 /* for_expr  */
  YYSYMBOL_bracket_loop_expr = 345,        /* bracket_loop_expr  */
  YYSYMBOL_cond_expr = 346,                /* cond_expr  */
  YYSYMBOL_nil_expr = 347,                 /* nil_expr  */
  YYSYMBOL_stmt_level_expr = 348,          /* stmt_level_expr  */
  YYSYMBOL_opt_task_intent_ls = 349,       /* opt_task_intent_ls  */
  YYSYMBOL_task_intent_clause = 350,       /* task_intent_clause  */
  YYSYMBOL_task_intent_ls = 351,           /* task_intent_ls  */
  YYSYMBOL_forall_intent_clause = 352,     /* forall_intent_clause  */
  YYSYMBOL_forall_intent_ls = 353,         /* forall_intent_ls  */
  YYSYMBOL_intent_expr = 354,              /* intent_expr  */
  YYSYMBOL_task_var_prefix = 355,          /* task_var_prefix  */
  YYSYMBOL_new_maybe_decorated = 356,      /* new_maybe_decorated  */
  YYSYMBOL_new_expr = 357,                 /* new_expr  */
  YYSYMBOL_let_expr = 358,                 /* let_expr  */
  YYSYMBOL_range_literal_expr = 359,       /* range_literal_expr  */
  YYSYMBOL_cast_expr = 360,                /* cast_expr  */
  YYSYMBOL_tuple_expand_expr = 361,        /* tuple_expand_expr  */
  YYSYMBOL_super_expr = 362,               /* super_expr  */
  YYSYMBOL_expr = 363,                     /* expr  */
  YYSYMBOL_opt_expr = 364,                 /* opt_expr  */
  YYSYMBOL_opt_try_expr = 365,             /* opt_try_expr  */
  YYSYMBOL_lhs_expr = 366,                 /* lhs_expr  */
  YYSYMBOL_call_base_expr = 367,           /* call_base_expr  */
  YYSYMBOL_call_expr = 368,                /* call_expr  */
  YYSYMBOL_dot_expr = 369,                 /* dot_expr  */
  YYSYMBOL_parenthesized_expr = 370,       /* parenthesized_expr  */
  YYSYMBOL_bool_literal = 371,             /* bool_literal  */
  YYSYMBOL_str_bytes_literal = 372,        /* str_bytes_literal  */
  YYSYMBOL_literal_expr = 373,             /* literal_expr  */
  YYSYMBOL_assoc_expr_ls = 374,            /* assoc_expr_ls  */
  YYSYMBOL_binary_op_expr = 375,           /* binary_op_expr  */
  YYSYMBOL_unary_op_expr = 376,            /* unary_op_expr  */
  YYSYMBOL_reduce_expr = 377,              /* reduce_expr  */
  YYSYMBOL_scan_expr = 378,                /* scan_expr  */
  YYSYMBOL_reduce_scan_op_expr = 379       /* reduce_scan_op_expr  */
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

#line 503 "bison-chpl-lib.cpp"

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
#define YYLAST   19108

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  181
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  199
/* YYNRULES -- Number of rules.  */
#define YYNRULES  741
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1312

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
       0,   635,   635,   639,   640,   645,   646,   654,   658,   668,
     673,   680,   681,   682,   683,   684,   685,   686,   687,   688,
     689,   690,   703,   704,   705,   714,   723,   727,   734,   742,
     743,   744,   745,   746,   747,   748,   749,   763,   778,   786,
     798,   809,   821,   833,   844,   860,   861,   865,   866,   867,
     871,   875,   876,   880,   884,   891,   903,   911,   914,   924,
     926,   933,   938,   947,   951,   971,   986,  1001,  1017,  1018,
    1020,  1025,  1026,  1031,  1049,  1053,  1057,  1068,  1087,  1088,
    1092,  1096,  1100,  1105,  1109,  1113,  1123,  1128,  1134,  1141,
    1146,  1153,  1164,  1165,  1169,  1174,  1182,  1191,  1201,  1209,
    1218,  1231,  1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,
    1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,  1267,
    1271,  1276,  1281,  1287,  1296,  1297,  1301,  1310,  1314,  1318,
    1322,  1326,  1330,  1339,  1340,  1344,  1345,  1346,  1347,  1348,
    1352,  1353,  1365,  1366,  1371,  1372,  1373,  1374,  1375,  1376,
    1377,  1378,  1379,  1380,  1381,  1382,  1383,  1384,  1385,  1386,
    1387,  1388,  1389,  1393,  1394,  1395,  1396,  1397,  1398,  1399,
    1400,  1401,  1402,  1403,  1404,  1411,  1412,  1413,  1414,  1418,
    1419,  1423,  1430,  1440,  1444,  1450,  1450,  1456,  1456,  1465,
    1466,  1467,  1468,  1469,  1470,  1471,  1475,  1480,  1485,  1490,
    1497,  1505,  1506,  1510,  1519,  1525,  1533,  1552,  1551,  1561,
    1560,  1573,  1580,  1598,  1611,  1624,  1628,  1632,  1636,  1640,
    1644,  1648,  1652,  1672,  1676,  1680,  1684,  1688,  1692,  1696,
    1700,  1704,  1708,  1712,  1716,  1720,  1724,  1728,  1732,  1736,
    1740,  1744,  1748,  1752,  1756,  1760,  1765,  1773,  1782,  1786,
    1790,  1794,  1798,  1802,  1806,  1810,  1814,  1819,  1824,  1829,
    1837,  1852,  1870,  1874,  1881,  1882,  1887,  1892,  1893,  1894,
    1895,  1896,  1897,  1898,  1899,  1900,  1901,  1902,  1903,  1904,
    1913,  1914,  1915,  1916,  1925,  1926,  1930,  1934,  1938,  1945,
    1949,  1953,  1960,  1964,  1968,  1972,  1979,  1980,  1984,  1988,
    1992,  1999,  2012,  2028,  2036,  2040,  2049,  2050,  2054,  2058,
    2063,  2071,  2076,  2080,  2087,  2088,  2092,  2101,  2106,  2117,
    2124,  2125,  2126,  2130,  2131,  2135,  2139,  2143,  2147,  2151,
    2158,  2175,  2188,  2195,  2200,  2207,  2206,  2216,  2222,  2221,
    2236,  2243,  2255,  2273,  2270,  2298,  2302,  2303,  2305,  2310,
    2311,  2315,  2316,  2320,  2323,  2325,  2330,  2331,  2342,  2362,
    2361,  2377,  2376,  2394,  2404,  2401,  2435,  2443,  2451,  2462,
    2473,  2482,  2497,  2498,  2502,  2503,  2504,  2513,  2514,  2515,
    2516,  2517,  2518,  2519,  2520,  2521,  2522,  2523,  2524,  2525,
    2526,  2527,  2528,  2529,  2530,  2531,  2532,  2533,  2534,  2535,
    2536,  2537,  2541,  2542,  2543,  2544,  2545,  2546,  2547,  2548,
    2549,  2550,  2551,  2552,  2557,  2558,  2562,  2563,  2567,  2568,
    2572,  2573,  2577,  2578,  2582,  2583,  2587,  2591,  2592,  2596,
    2600,  2605,  2610,  2615,  2620,  2628,  2632,  2640,  2641,  2642,
    2643,  2644,  2645,  2646,  2647,  2648,  2652,  2653,  2654,  2655,
    2656,  2657,  2661,  2662,  2663,  2667,  2668,  2669,  2670,  2671,
    2672,  2673,  2677,  2678,  2681,  2682,  2686,  2687,  2688,  2692,
    2697,  2698,  2700,  2702,  2704,  2709,  2711,  2716,  2718,  2720,
    2722,  2724,  2726,  2728,  2733,  2734,  2738,  2747,  2751,  2759,
    2763,  2770,  2791,  2792,  2797,  2798,  2799,  2800,  2801,  2806,
    2805,  2814,  2822,  2826,  2833,  2851,  2870,  2874,  2878,  2885,
    2887,  2889,  2896,  2897,  2898,  2902,  2906,  2907,  2908,  2912,
    2913,  2917,  2918,  2922,  2923,  2924,  2925,  2929,  2930,  2934,
    2938,  2939,  2945,  2946,  2950,  2951,  2955,  2959,  2966,  2967,
    2971,  2976,  2985,  2986,  2990,  2991,  2998,  2999,  3000,  3001,
    3002,  3003,  3005,  3007,  3009,  3011,  3017,  3019,  3022,  3024,
    3026,  3028,  3030,  3032,  3034,  3036,  3038,  3040,  3045,  3056,
    3067,  3077,  3093,  3109,  3125,  3134,  3143,  3151,  3165,  3179,
    3196,  3200,  3204,  3208,  3212,  3216,  3220,  3227,  3245,  3253,
    3254,  3255,  3256,  3257,  3258,  3259,  3263,  3264,  3268,  3277,
    3278,  3282,  3291,  3292,  3296,  3310,  3314,  3321,  3322,  3323,
    3324,  3325,  3326,  3330,  3332,  3334,  3336,  3338,  3344,  3351,
    3363,  3375,  3388,  3405,  3412,  3417,  3422,  3427,  3433,  3439,
    3469,  3476,  3483,  3484,  3488,  3490,  3491,  3493,  3495,  3496,
    3497,  3498,  3501,  3502,  3503,  3504,  3505,  3506,  3507,  3508,
    3509,  3513,  3514,  3518,  3519,  3520,  3524,  3525,  3526,  3527,
    3536,  3537,  3540,  3541,  3542,  3546,  3558,  3570,  3577,  3579,
    3581,  3583,  3585,  3591,  3604,  3605,  3609,  3613,  3620,  3621,
    3625,  3626,  3630,  3631,  3632,  3633,  3634,  3635,  3636,  3637,
    3642,  3647,  3651,  3655,  3659,  3667,  3672,  3681,  3682,  3683,
    3684,  3685,  3686,  3687,  3688,  3689,  3690,  3691,  3692,  3693,
    3694,  3695,  3696,  3697,  3698,  3699,  3700,  3701,  3702,  3703,
    3707,  3708,  3709,  3710,  3711,  3712,  3715,  3719,  3723,  3727,
    3731,  3738,  3742,  3746,  3750,  3758,  3759,  3760,  3761,  3762,
    3763,  3764
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
  "decl_base", "attribute_receiver", "attribute_group",
  "attribute_decl_stmt_ls", "attribute_decl_stmt", "attribute_decl_begin",
  "opt_attribute_decl_toolspace", "opt_attribute_actuals",
  "attribute_actuals_ls", "attribute_actual", "module_decl_start",
  "module_decl_stmt", "opt_access_control", "opt_prototype",
  "include_module_stmt", "block_stmt_body", "block_stmt", "stmt_ls",
  "renames_ls", "use_renames_ls", "opt_only_ls", "use_stmt", "import_stmt",
  "import_internal_type_ident", "import_expr", "import_ls", "require_stmt",
  "assignment_stmt", "opt_label_ident", "reserved_word_ident", "ident_def",
  "ident_use", "internal_type_ident_def", "scalar_type",
  "reserved_type_ident_use", "do_stmt", "return_stmt", "class_level_stmt",
  "$@1", "$@2", "inner_class_level_stmt", "forwarding_decl_stmt",
  "forwarding_decl_start", "extern_or_export",
  "extern_export_decl_stmt_start", "extern_export_decl_stmt", "$@3", "$@4",
  "extern_block_stmt", "loop_stmt", "zippered_iterator", "if_stmt",
  "ifvar", "interface_stmt", "ifc_formal_ls", "ifc_formal",
  "implements_type_ident", "implements_type_error_ident",
  "implements_stmt", "ifc_constraint", "try_stmt", "catch_expr_ls",
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
  "sub_type_level_expr", "for_expr", "bracket_loop_expr", "cond_expr",
  "nil_expr", "stmt_level_expr", "opt_task_intent_ls",
  "task_intent_clause", "task_intent_ls", "forall_intent_clause",
  "forall_intent_ls", "intent_expr", "task_var_prefix",
  "new_maybe_decorated", "new_expr", "let_expr", "range_literal_expr",
  "cast_expr", "tuple_expand_expr", "super_expr", "expr", "opt_expr",
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

#define YYPACT_NINF (-1118)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-742)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
   -1118,   115,  4057, -1118,   -37,   198, -1118, -1118, -1118, -1118,
   -1118, -1118, -1118, 11586,   146,   176,   218, 13813,   318, 18862,
     146, 11586,   417,   169,   208,   176,  5461, 11586,  5461,   157,
   18917, -1118,   384,   426,  9136, 10011, 10011, -1118,  9311,   440,
     483,   162, -1118,   478, 18917, 18917, 18917, -1118, 14057, 10186,
     514, 11586, 11586,    59, -1118,   521,   526, 11586, -1118, 13813,
   -1118, 11586,   530,   385,   209,   254,  3745,   529, 18992, -1118,
   11586,  8611, 11586, 10186, 13813, 11586,   488,   539,   427,  5461,
     554, 11586,   562,  7561,  7561, -1118,   578, -1118, 13813, -1118,
     588,  9311,  9486, -1118,   611, 11586, -1118, 11586, -1118, -1118,
   13641, 11586, -1118, 11586, -1118, -1118, -1118,  4411,  7911,  9661,
   11586, -1118,  5111, -1118, -1118, -1118, -1118, 18360,   506, -1118,
      37,   479, -1118,   253, -1118, -1118, -1118, -1118, -1118, -1118,
   -1118, -1118, -1118, -1118, -1118, -1118, -1118,  7736, -1118, 10361,
   -1118, -1118, -1118, -1118, -1118,   598, -1118, -1118, -1118, -1118,
   -1118, -1118, -1118, -1118,   508, 18917, -1118, 18602,   484,   308,
   -1118,    36, -1118, -1118, -1118,   306,   252, -1118, -1118, 18917,
   14057, -1118,   490,   493, -1118, -1118, -1118,   495,   496, 11586,
     497,   498, -1118, -1118, -1118, 18168,   783,   400,   501,   507,
   -1118, -1118,   413, -1118, -1118, -1118, -1118, -1118,    58, -1118,
   -1118, -1118, 11586, 11586, 11586, 18917, -1118, 11586, 11586, 10536,
     616,   421, -1118, -1118, -1118, -1118, 18168,   431, -1118, -1118,
     509,  5461, -1118, -1118,   513,   207,   503,  3181, -1118,    56,
   -1118,   515, -1118,   110, 18168,   555,  9486, -1118, -1118, -1118,
   -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118,
   -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118,
   -1118, -1118,   516, -1118, -1118, 18917,   511,    61, 15290,    89,
   15062,    89, 15138, 18917, 18917,   -22,  3851, -1118, -1118,   595,
    9486,   512,   434,  5461,  2032,   103,    78, -1118,  5461, -1118,
   -1118, 15630, -1118,    15, 16260,   559, -1118,   517,   518, -1118,
   15630,   207,   559, -1118,  9486,  2037,  2037,   195,    -4, -1118,
      50, -1118, -1118, -1118, -1118, 11586, 11586, -1118,   274, -1118,
     520,   540, -1118, -1118,  2189,   542, -1118, -1118, 18168,   210,
   11586, 11586, -1118,   -25, -1118, -1118, 18168,   531, 16340, -1118,
   15630,   207, 18168,   519,  9486, -1118, 18168, 16380, -1118, -1118,
   -1118, -1118,   207,    61, 15630,   534, -1118,    24,    24,  2346,
     559,   559,   235, -1118, -1118,  4586,   111, -1118, 11586,   -33,
     -20, -1118, 13956,   -55,    94, 16426,   101,  2346,   686, -1118,
   -1118, 18917, -1118,  9136, 10011, -1118,  4411, 10011, -1118, -1118,
     595, -1118, -1118, -1118, -1118,   692,   525,  4761, 11586, -1118,
   11586,   636, -1118, -1118, 15382,   224,   508, 18168,   461, 18917,
   11586,   536, -1118,   533, 18657,   584,   242, -1118,   236,   306,
   -1118, -1118, -1118, -1118,   620, -1118, -1118, -1118, -1118,    69,
     621, -1118, -1118, -1118, 13471,   589,   246, -1118,   260,   441,
   -1118,   566, 11586, 11586, 11586, 10011, 10011, 11586,   452, 11586,
   11586, 11586, 11586, 11586,   232, 13641, 11586, 11586, 11586, 11586,
   11586, 11586, 11586, 11586, 11586, 11586, 11586, 11586, 11586, 11586,
   11586, 11586,   650, -1118, -1118, -1118, -1118, -1118,  9486,  9486,
   -1118, -1118, -1118, -1118,  9486, -1118, -1118,  9486,  9486,  9486,
    9486, 10011, 10011, 15706, 15782, 16532,   548,    65, 18917,  8086,
   -1118, -1118, -1118, 10011,    61, 14057, -1118, 11586, -1118, 11586,
     596,   549,   576, -1118, -1118, -1118,   678, 11586, -1118,  5636,
   10011, -1118,   556, -1118,    61,  5811, 10011, -1118,    61, -1118,
      61, 10011, -1118,    61,   604,   605,  5461,   700,  5461, -1118,
     701, 11586,   673,   567,  9486, 18917, -1118, -1118, -1118,  2032,
   -1118, -1118,   107, -1118, 11761,   617, 11586, 14057, -1118, -1118,
   11586, -1118, 18732, 11586, 11586, -1118,   568, -1118, -1118, -1118,
   -1118, 18787, -1118,   306, 13335, -1118,  2032, -1118,   597, 11761,
   -1118, 18168, 18168, -1118,   503, -1118,    42, -1118,  9486,   572,
   -1118,   733,   733, -1118, -1118, -1118, -1118, -1118, -1118, 10711,
   -1118, 16578,  8261, -1118,  8436, -1118, 18168,  5461,   580, 10011,
   10886,  4236,   581, 11586, 11061, -1118, -1118, 15462, 15214,   109,
     608,  8786, -1118,   387, -1118,  4936, -1118,   263, 16618,   270,
   15554, 18917,  7386,  7386, -1118,   508,   591, -1118,    56, -1118,
     -35,   612,  1623, -1118, -1118, 11586,   124, -1118, -1118,   113,
   -1118, -1118,    75, -1118, -1118,  7211, -1118,   653, -1118,   590,
     615, -1118,   618, -1118,   622,   625,   629,   631, -1118,   634,
     637,   639,   640,   645,   646,   647, -1118, -1118, -1118, -1118,
   -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118,
   -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118, 11586,   624,
     651,   590, -1118,   590, -1118, 11586, -1118, 18917, -1118, -1118,
    1368,  1368,   386, -1118,   386, -1118,   386, 16220,  1689,  1598,
    1415,   207,    24, -1118,   453, -1118, -1118, -1118, -1118, -1118,
    2346,  2586,   386,   901,   901,  1368,   901,   901,  1444,    24,
    2586, 13206,  1444,   559,   559,    24,  2346,   601,   638,   641,
     642,   643,   648,   626,   594, -1118,   386, -1118,   386, 11936,
   10011, 12111, 10011, 11586,  9486, 10011, 13956,   613,   223, -1118,
   -1118, -1118,   159, -1118,  2227, 18213,    71,   146, 16694, -1118,
     323, 18168, 16770,  9486, -1118,  9486, 11586,   203,  9311, 18168,
      68, 15858,  8086, -1118,  9311, 18168,    91,  1216, -1118, -1118,
      89,  2847, -1118, 11586, 11586,   768,  5461,   771, 16853,  5461,
   15934, 18917, -1118,   204, -1118,   206, -1118,   219,  2032,   103,
   -1118, 18168,  8961, -1118, 18168, -1118, -1118, -1118, 18917, 16929,
   16969, -1118,   597,   664, -1118,   251, 11761, -1118,   269, -1118,
   11586, -1118,   649,   173,   652, -1118,  3538, -1118, -1118, -1118,
   -1118, -1118, -1118, -1118, -1118,  5461,   -21, 16010, -1118, -1118,
   18168,  5461, 18168, -1118, 17015, 10011, 10011, 10011,  5461, -1118,
   -1118,   294, -1118, -1118, -1118, -1118, 11586, -1118,   130, 14162,
   11586, -1118, 11236,  7386,  7386, -1118, 11586,   319, 11586,   329,
     334, 11586,  9836,   467,   338, -1118, -1118, -1118, 17121,   665,
     654,   644, -1118, 18917, 14057, -1118,  9486,   655,  3421, 18168,
   18917,   506,   532, -1118,   653, -1118, -1118, -1118, -1118, -1118,
      13,   502, -1118, 17167, -1118, 14321, -1118, -1118, 18168, -1118,
     656,   657, -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118,
   11586,   779, 17207, 11586,   784, 17359,   316,   658, 17283,  9486,
   -1118, -1118,  8086, -1118, -1118, -1118, -1118, -1118, -1118, -1118,
   -1118, -1118, -1118, -1118, -1118,    51, 10011, 10011,    61,    61,
   -1118, -1118, -1118, -1118, 15630, -1118, 14402,  5986, -1118,  6161,
   -1118,   336, -1118, 14482,  6336, -1118,    61,  6511, -1118,    61,
   -1118,    61, -1118,    61, 18168, 18168,  5461, -1118,  5461, 11586,
   -1118,  5461,   787,   659,   663, 18917,   503, -1118, -1118,   617,
   -1118, -1118, -1118,   662,   666,   269, 11586,   653,   597, -1118,
   11586, -1118, -1118, 18168, -1118,  6686, 11586, -1118, -1118, -1118,
   18917, -1118,   676,   503, -1118,  6861,   660,  7036,   668, -1118,
   11586,    61, 15630,    89,  2847,   -12, 16086,  9486, -1118, -1118,
   -1118, -1118, -1118,   453, -1118, -1118, -1118, -1118, -1118, -1118,
   -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118,  7386, -1118,
   -1118, 17399,    67, 16178,   340,   672,  7386, -1118, 11586, -1118,
   -1118, 18427,   350,   375,   344, -1118, -1118, -1118, -1118,  1796,
   18519, -1118, -1118, 18657, -1118,    13,   140, 11586, -1118,   684,
     674, -1118,   590,   590, -1118, -1118, 17444, 12286, 12461, 17551,
   12636, 12811, -1118, 12986, 13161,   369, -1118,   617,   372,   373,
   -1118, -1118, -1118,  5461,  9311, 18168,  9311, 18168,  8086, -1118,
    5461,  9311, 18168, -1118,  9311, 18168, -1118, -1118, -1118, -1118,
   -1118, 18168,   805,  5461, -1118, -1118, -1118, -1118, -1118,  9486,
    9486, -1118, -1118, -1118, -1118,   269, 18168,  5461, -1118,    22,
     675, 11586, -1118,  9311, -1118, 18168,  5461,  9311, -1118, 18168,
    5461, 18168,  5461,  5461, -1118,   243, 11411,  7386,  7386,  7386,
    7386, -1118, -1118, -1118, 17596, 18168, 18473, -1118, -1118, -1118,
     680, 18519, -1118, -1118, -1118, -1118, -1118,   141, -1118,    31,
   -1118,   247, 17642,    18, -1118, -1118, -1118, 11586, 11586, 11586,
   11586, 11586, 11586, 11586, 11586, -1118, -1118, 16853, 14562, 14642,
   -1118, 16853, 14722, 14802,  5461, -1118,   677,   683, -1118, -1118,
     555, -1118,   503, 18168, 14882, -1118, 14962, -1118, -1118, -1118,
   18168,   383,   681,   389,   693, 11586, -1118, -1118, -1118,    60,
   -1118, 11586,   140,   141,   141,   141,   141,   141,   141,   140,
    5286, -1118, -1118, -1118, -1118, -1118, 17748, 17824, 17900, 17976,
   18052, 18128,  5461,  5461,  5461,  5461, -1118,   685,   687, -1118,
    5461,  5461, -1118, -1118, -1118, -1118, 18168, -1118, 18168, -1118,
   -1118, -1118, -1118, -1118, -1118, -1118,   694, -1118, 11586, 11586,
   11586, 11586, 11586, 11586, 16853, 16853, 16853, 16853, -1118, -1118,
   16853, 16853
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       3,     0,     0,     1,     0,   142,   469,   684,   685,   686,
     680,   681,   687,     0,   596,   133,   163,   564,   170,   566,
     596,     0,   169,   499,   497,   133,     0,     0,     0,   284,
     164,   202,   201,   678,     0,     0,     0,   200,     0,   168,
      68,   285,   347,   165,     0,     0,     0,   342,     0,     0,
     172,     0,     0,   613,   588,   688,   173,     0,   348,   558,
     494,     0,     0,     0,   187,   185,   435,   167,   567,   496,
       0,     0,     0,     0,   562,     0,     0,   171,     0,     0,
     143,     0,   679,     0,     0,   487,   166,   322,   560,   498,
     174,     0,     0,   737,     0,     0,   739,     0,   740,   741,
     629,     0,   738,   735,   637,   183,   736,     0,     0,     0,
       0,     4,     0,     5,    10,    11,    12,   345,    50,    51,
      57,     0,    45,    71,    13,    77,    14,    15,    16,    17,
      29,   544,   545,    23,    46,   184,   194,     0,   203,   651,
     195,    18,    31,    30,    20,     0,   279,    19,   647,    22,
      35,    32,    33,   193,   323,     0,   191,     0,     0,   644,
     346,     0,   641,   189,   363,   455,   446,   639,   192,     0,
       0,   190,   656,   635,   634,   638,   548,   546,     0,     0,
     645,   646,   650,   649,   648,     0,   547,     0,   657,   658,
     659,   682,   683,   640,   550,   549,   642,   643,     0,    28,
     566,   164,     0,     0,     0,     0,   567,     0,     0,     0,
       0,   644,   656,   546,   645,   646,   556,   547,   657,   658,
       0,     0,   597,   134,     0,   565,     0,   596,   488,     0,
     495,     0,    21,     0,   532,     0,   538,   140,   144,   156,
     150,   149,   158,   137,   148,   159,   145,   160,   135,   161,
     154,   147,   155,   153,   151,   152,   136,   138,   146,   157,
     162,   141,     0,   139,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    70,    69,    71,
     538,     0,     0,     0,     0,     0,     0,   502,     0,   180,
      40,     0,   314,     0,   313,   722,   617,   614,   615,   616,
       0,   559,   723,     7,   538,   345,   345,   442,   177,   437,
     178,   438,   439,   443,   444,   176,   175,   445,   435,   528,
       0,   350,   351,   353,     0,   436,   527,   355,   515,     0,
       0,     0,   181,   641,   632,   655,   633,     0,     0,    43,
       0,   563,   551,     0,   538,    44,   557,     0,   292,   296,
     293,   296,   561,     0,     0,     0,    56,   724,   726,   627,
     721,   720,     0,    74,    78,     0,     0,   534,     0,     0,
       0,   535,   580,     0,     0,   532,     0,   628,     0,     6,
     320,     0,   201,     0,     0,   321,     0,     0,    48,     9,
      71,    49,    47,    52,    55,     0,    54,     0,     0,    72,
       0,     0,   499,   199,     0,   680,   323,   652,   207,     0,
       0,     0,   319,     0,     0,   340,     0,   333,   435,   455,
     453,   454,   452,   371,   457,   456,   460,   459,   461,     0,
     450,   447,   448,   451,     0,   492,     0,   489,     0,   636,
      34,   618,     0,     0,     0,     0,     0,     0,   725,     0,
       0,     0,     0,     0,     0,   626,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   402,   409,   410,   411,   406,   408,     0,     0,
     404,   407,   405,   403,     0,   413,   412,     0,     0,   538,
     538,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      36,    24,    37,     0,     0,     0,    25,     0,    38,     0,
     544,     0,   539,   540,   543,   332,     0,     0,   221,     0,
       0,   220,     0,   229,     0,     0,     0,   227,     0,   237,
       0,     0,   235,     0,     0,     0,     0,   253,     0,   402,
     249,     0,     0,     0,     0,     0,   263,    26,   506,     0,
     507,   509,     0,   526,     0,   512,     0,     0,   179,    39,
       0,   316,     0,     0,     0,    41,     0,   188,   186,   440,
     441,     0,   436,   455,   435,   428,     0,   427,   530,     0,
     126,   653,   654,   361,     0,   182,     0,    42,     0,     0,
     303,   294,   295,   214,   213,    27,    76,    75,    79,     0,
     689,     0,     0,   674,     0,   676,   581,     0,     0,     0,
       0,     0,     0,     0,     0,   693,     8,     0,     0,     0,
      57,     0,    53,     0,    65,     0,   124,     0,   119,     0,
      86,     0,     0,    92,   196,   323,     0,   209,     0,   206,
     290,   324,     0,   331,   337,     0,   335,   330,   420,     0,
     422,   426,     0,   458,   520,     0,   522,   462,   449,   418,
     140,   398,   144,   396,   150,   149,   148,   145,   400,   160,
     161,   147,   151,   136,   146,   162,   395,   377,   380,   378,
     379,   401,   390,   381,   394,   386,   384,   397,   385,   383,
     388,   393,   382,   387,   391,   392,   389,   399,     0,   374,
       0,   418,   375,   418,   372,     0,   491,     0,   486,   501,
     717,   716,   719,   728,   727,   732,   731,   713,   710,   711,
     712,   630,   700,   142,     0,   670,   671,   143,   669,   668,
     624,   704,   715,   709,   707,   718,   708,   706,   698,   703,
     705,   714,   697,   701,   702,   699,   625,     0,     0,     0,
       0,     0,     0,     0,     0,   730,   729,   734,   733,     0,
       0,     0,     0,     0,     0,     0,   691,   290,   607,   608,
     610,   612,     0,   599,     0,     0,     0,   596,   596,   217,
       0,   533,     0,     0,   553,     0,     0,     0,     0,   570,
       0,     0,     0,   230,     0,   576,     0,     0,   228,   238,
       0,     0,   236,     0,     0,   252,     0,   248,     0,     0,
       0,     0,   552,     0,   266,     0,   264,     0,   510,     0,
     525,   524,     0,   504,   623,   503,   315,   312,     0,     0,
       0,   667,   530,   356,   352,     0,     0,   531,   512,   354,
       0,   360,     0,     0,     0,   554,     0,   297,   690,   631,
     675,   536,   677,   537,   245,     0,     0,     0,   692,   243,
     582,     0,   695,   694,     0,     0,     0,     0,     0,    58,
      59,     0,    61,    63,    67,    66,     0,   101,     0,     0,
       0,    94,     0,     0,    92,    64,   395,   377,   380,   378,
     379,   388,   387,   389,     0,   414,   415,    81,    80,    93,
       0,     0,   325,     0,     0,   287,     0,     0,   345,   341,
       0,     0,   435,   421,   462,   518,   517,   516,   519,   463,
     470,   435,   370,     0,   376,     0,   366,   367,   493,   490,
       0,     0,   132,   130,   131,   129,   128,   127,   665,   666,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     609,   611,     0,   598,   163,   170,   169,   168,   165,   172,
     173,   167,   171,   166,   174,     0,     0,     0,     0,     0,
     500,   212,   542,   541,     0,   247,     0,     0,   219,     0,
     218,     0,   602,     0,     0,   225,     0,     0,   223,     0,
     233,     0,   231,     0,   261,   260,     0,   255,     0,     0,
     251,     0,   257,     0,   289,     0,     0,   508,   511,   512,
     513,   514,   311,     0,     0,   512,     0,   462,   530,   529,
     416,   431,   429,   362,   305,     0,     0,   304,   307,   555,
       0,   298,   301,     0,   246,     0,     0,     0,     0,   244,
       0,     0,     0,     0,     0,     0,     0,     0,    60,   125,
     122,   102,   114,   108,   107,   106,   116,   103,   117,   112,
     105,   113,   111,   109,   110,   104,   115,   118,     0,   121,
     120,    89,    88,    87,     0,     0,     0,   197,     0,   198,
     325,   345,     0,     0,     0,   318,   187,   185,   317,   345,
     345,   326,   336,     0,   423,   470,     0,     0,   364,   425,
       0,   373,   418,   418,   672,   673,     0,     0,     0,     0,
       0,     0,   289,     0,     0,     0,   600,   512,   656,   656,
     216,   215,   222,     0,     0,   569,     0,   568,     0,   601,
       0,     0,   575,   226,     0,   574,   224,   234,   232,   254,
     250,   587,   256,     0,    73,   286,   265,   262,   505,   538,
     538,   432,   430,   357,   358,   512,   417,     0,   309,     0,
       0,     0,   299,     0,   241,   584,     0,     0,   239,   583,
       0,   696,     0,     0,    62,     0,     0,     0,    92,     0,
      92,    95,    98,    84,    83,    82,   345,   204,   210,   208,
     291,   345,   328,   327,   339,   343,   484,     0,   485,   472,
     475,     0,   471,     0,   419,   368,   369,     0,     0,     0,
       0,     0,     0,     0,     0,   291,   604,   573,     0,     0,
     603,   579,     0,     0,     0,   259,     0,     0,   434,   433,
     310,   308,     0,   302,     0,   242,     0,   240,   123,    91,
      90,     0,     0,     0,     0,     0,   205,   288,   329,     0,
     483,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   464,   466,   468,   365,   465,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   258,   619,   620,   300,
       0,     0,    97,   100,    96,    99,    85,   344,   474,   476,
     477,   480,   481,   482,   478,   479,   473,   467,     0,     0,
       0,     0,     0,     0,   572,   571,   578,   577,   621,   622,
     586,   585
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1118, -1118, -1118,     4,   -53,  2425, -1118,   480,   740, -1118,
   -1118,  -148,  -117, -1118,   240, -1118, -1118,  -186, -1118, -1118,
      41,   583, -1118,  -572,  2507,   468,  -606, -1118,  -856, -1118,
   -1118, -1118,   -10, -1118, -1118, -1118,   839, -1118,  3145,  -162,
   -1118, -1118,  -498,  2892, -1117,  -871, -1118, -1118,   345, -1118,
   -1118, -1118, -1118, -1118, -1118, -1118, -1118,   755,   962, -1118,
     789, -1118, -1118,  -131,  2096, -1118, -1118, -1118, -1118,   538,
   -1118,  -147, -1118, -1118, -1118, -1118,   322, -1118, -1118, -1118,
    -109, -1118,  -373,  -860, -1118, -1118, -1118, -1118, -1118,  -411,
   -1118,   619, -1118, -1118, -1118, -1118, -1118,   310, -1118,   180,
   -1118, -1118, -1118, -1118,   486, -1118, -1118, -1118, -1118,   -38,
    -417,   -95,  -839,  -951,  -634, -1118,   -30, -1118,   -16,   321,
     -51,   579,   -48, -1118, -1118,  -370,  -842, -1118,  -351, -1118,
    -194,  -356,  -347,  -619, -1118, -1118,     3,   217, -1118,  -219,
     788, -1118,  -165,   374,   108,  -438,  -809,  -628, -1118, -1118,
   -1118,  -618,  -547, -1118,  -791,    -7,    26, -1118,  -273,  -528,
    -586,   279, -1118, -1118, -1118, -1118,   816, -1118,    -1, -1118,
   -1118,   -64, -1118,  -768, -1118, -1118,  1190,  1387, -1118, -1118,
   -1118, -1118,   -13, -1118,    14,  1412, -1118,  1896,  1941, -1118,
   -1118, -1118, -1118, -1118, -1118, -1118, -1118, -1118,  -488
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,   364,   112,   807,   114,   115,   116,   389,
     117,   118,   119,   120,   396,   622,   871,   872,   121,   122,
     123,   401,   124,   125,   126,   365,   899,   629,   900,   127,
     128,  1069,   626,   627,   129,   130,   224,   261,   285,   131,
     263,   132,   319,   521,   133,   134,   306,   305,   135,   136,
     137,   138,   139,   140,   638,   903,   141,   142,   267,   143,
     275,   144,   815,   816,   210,   146,   147,   148,   149,   591,
     847,  1033,   150,   151,   843,  1028,   292,   293,   152,   153,
     154,   155,   411,   908,   156,   157,   416,   910,   911,   417,
     158,   211,  1249,   160,   161,   320,   321,   322,  1017,   162,
     334,   584,   840,   163,   164,  1203,   165,   166,   700,   701,
     895,   896,   897,  1021,   922,   419,   649,  1100,   650,   578,
     651,   324,   572,   434,   423,   429,   920,  1264,  1265,   167,
    1098,  1199,  1200,  1201,   168,   169,   436,   437,   706,   170,
     171,   229,   286,   287,   551,   552,   823,   326,   918,   656,
     657,   555,   327,   837,   838,   366,   369,   370,   511,   512,
     513,   212,   173,   174,   175,   176,   213,   178,   221,   222,
     772,   528,   981,   773,   774,   179,   214,   215,   182,   183,
     184,   335,   185,   408,   514,   217,   187,   218,   219,   190,
     191,   192,   193,   376,   194,   195,   196,   197,   198
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     216,   393,   844,   644,   225,   438,   111,   543,   227,   414,
     505,   776,   841,   318,   234,   323,   813,   702,   325,   226,
     233,   268,   270,   272,   982,   276,   894,   917,  1075,  1022,
     406,   566,   839,   636,   914,   873,   291,  1091,   294,   295,
    1070,  1015,  1081,   842,   300,   394,   301,   288,   302,   652,
    1260,  -284,   553,   328,   288,   444,   820,   234,   336,   338,
     340,   341,   342,   329,  1151,   522,   346,   926,   347,   927,
     654,   589,  1095,  1096,   510,   352,   553,   296,   354,   336,
     536,   279,   357,    24,   358,   337,  1263,   359,   360,   420,
     361,   488,  1260,   288,   234,   336,   375,   377,   583,   522,
     977,  -521,   374,  1177,   553,  -285,   355,  -523,   522,   421,
      71,   817,   765,   445,   602,     3,  -306,   446,   510,   422,
     607,   288,   371,   984,   404,   556,   407,   604,   199,  -521,
     905,  1097,  1263,   723,    60,  -523,   297,   906,   835,  1178,
     603,   609,   510,  1196,  1196,  -359,    69,   491,   386,  1251,
    -523,   492,    71,   605,  1035,   298,   867,   916,   390,  -306,
     967,  -521,   560,  1172,   492,   448,   441,  -523,   236,   507,
     452,    89,   299,   454,  -521,  1154,  -523,   457,  1252,   223,
    -523,   541,   510,  1261,  1116,   386,   395,  -521,   386,   493,
     494,   495,   386,  -523,   346,   234,   375,   554,  -523,   973,
    1148,  1009,   497,   833,  1228,   524,  1152,   530,   533,   522,
    1091,   522,   610,  -306,   522,   655,   753,   754,  1192,  1193,
    1186,   554,   280,   336,  -523,   557,   504,  1155,  -523,   522,
     386,   386,  1197,   727,  -521,   723,   946,  1183,   386,  -521,
     766,   610,   569,  1198,  1198,  -523,   380,  1025,   614,   554,
    -523,   333,   724,  -267,   818,   -70,   507,   507,   599,   386,
     912,   386,   901,   307,  -338,   725,   220,   336,  -523,   611,
     950,  -334,   333,  -268,   -70,   508,   615,  1074,   228,   430,
     819,   172,   600,   309,   868,   570,   913,   311,   333,  1019,
    1026,   336,   729,   -70,   726,  -334,   635,   393,   230,   398,
     -69,   307,   342,   346,   776,   172,   952,   172,  1216,   608,
     612,   312,   385,   951,   313,  1091,    62,   581,   582,   -69,
    1248,   309,  1242,   -70,  1244,   311,   314,   510,   510,   236,
     431,   336,   953,   424,   280,   727,    87,   399,   -69,   703,
     780,   728,   432,   828,  1027,   317,  1229,  1117,   448,   312,
     507,   785,   313,  1005,   378,   601,   454,   507,   172,   606,
    1220,   433,   172,   172,   314,   318,   818,   400,   -69,   598,
     617,   618,  1253,  -274,   234,   580,   975,  1004,  1084,  1006,
     619,   425,   510,   317,   426,   628,   172,   630,  1153,   646,
    1076,   172,  1007,   707,   822,   264,   427,   234,   818,  1254,
     199,  1255,  1256,   641,  1257,  1258,   596,   557,  -739,   648,
     876,   708,  -739,   647,  1238,   428,   333,   880,  -740,   904,
    1020,  1115,  -740,  -741,  1018,   709,   510,  -741,   877,   710,
     711,   712,   714,   716,   717,   881,   718,   719,   720,   721,
     722,  1047,   730,   731,   732,   733,   734,   735,   736,   737,
     738,   739,   740,   741,   742,   743,   744,   745,   746,   510,
     333,   873,  1175,   785,   776,   336,   336,  1048,  1205,  1206,
     557,   336,  -273,  -593,   336,   336,   336,   336,   756,   758,
    -663,  -282,  -663,  1128,   333,  1076,   775,  1076,   970,  1112,
     778,   785,   748,   749,   781,  -272,   782,   707,   750,  1092,
     172,   751,   752,  1077,   234,  1181,   789,   791,   702,  1129,
     787,    42,   795,   797,  -345,  1188,   785,  1190,   801,  -606,
    -605,   318,   557,   323,   333,   808,   325,   448,   810,   307,
    1076,   336,   452,  -269,  -345,   454,  1076,    58,   303,   457,
    1189,   821,  1215,   824,  -345,  -606,  -605,   294,  1282,   309,
     829,   830,   199,   311,  1284,   612,  -736,   304,   874,   307,
    -736,   328,   172,   348,   350,   277,   328,   172,   278,  -276,
     637,  1241,   489,  1243,   490,   336,  -280,   312,  1250,   309,
     313,  -277,    62,   311,  -271,  -664,   781,  -664,   343,   336,
     444,   336,   314,  -663,  -275,  -663,   857,   781,   860,   344,
     862,   864,   510,  -660,   386,  -660,   545,   312,   336,  -281,
     313,   317,    62,  -662,   356,  -662,   371,  -283,   371,   898,
     898,   159,   314,   510,  -661,   930,  -661,   931,   851,   598,
     853,  1262,   909,  -270,  1290,  1291,  1292,  1293,  1294,  1295,
     776,   317,   328,  -278,   172,   159,    94,   159,   445,   397,
     567,   568,   446,   409,   410,  -590,   418,   439,   333,   333,
    -589,   440,  -594,  -595,   333,   172,  -592,   333,   333,   333,
     333,   498,  -591,   386,   509,  -424,   172,  1262,   501,   399,
     506,   499,  1194,   517,   544,   923,   515,   574,   579,   563,
     564,   588,   928,   573,   616,   620,   585,   621,   159,   595,
     448,   631,   159,   159,   643,   452,   642,   453,   454,   645,
     653,   658,   457,   704,   705,   454,  1050,   729,   747,   464,
     764,   783,   784,   785,   333,   786,   159,   470,   792,   803,
     804,   159,   986,   989,   806,   809,   991,   993,   811,  1083,
     812,   831,   822,   836,   510,   845,   789,   942,   795,   945,
     808,   336,   948,   860,   846,   855,   861,   395,   919,   507,
    1090,   902,   921,  1093,  -142,   924,   932,  -163,   333,   939,
     336,  -170,   336,   974,  -169,   976,   968,   969,  -168,   775,
    -165,   983,   333,  -172,   333,   949,  -173,   510,  -167,  -171,
     994,   995,  1036,  1038,  -143,  -166,  -174,   972,   172,   938,
     925,   333,   996,   933,   172,   998,   934,   935,   936,   336,
    1016,  1107,  1076,   937,  1080,   172,  1110,   172,   177,  1079,
    1024,  1143,  1161,   328,  1144,  1029,  1085,  1023,  1145,  1104,
    1103,   912,  1105,  1113,  1149,  1166,  1011,  1182,  1150,  1224,
     159,  1252,   177,  1170,   177,  1247,  1283,  1204,  1232,  1308,
    1277,  1309,  1042,  1044,  1046,  1089,  1278,   388,  1285,   318,
     869,  1174,   542,   628,   231,   625,  1049,  1071,   318,  1073,
     898,   898,   392,   357,  1146,   358,  1226,  1227,   360,   361,
     353,   541,   826,  1160,   834,   510,   172,  1102,   541,   592,
     172,  1099,   832,   336,   639,   177,  1094,   571,  1287,   177,
     177,  1195,   159,  1296,   172,  1289,  1082,   159,   472,   473,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,   177,   929,   403,  1008,  1106,   177,     0,
    1109,   825,   444,  1090,     0,     0,   336,     0,     0,   775,
       0,  1191,     0,     0,   333,     0,     0,     0,     0,     0,
       0,     0,   487,   714,   756,  -660,     0,  -660,     0,     0,
       0,     0,     0,   333,  1125,   333,  1127,     0,     0,     0,
       0,  1132,     0,   393,  1135,     0,   393,     0,     0,   986,
     989,  1036,  1038,     0,   159,     0,  1141,   510,   510,     0,
     445,     0,     0,     0,   446,     0,     0,   269,   271,     0,
       0,     0,   333,   328,     0,   159,     0,  1156,     0,     0,
       0,     0,     0,   234,     0,     0,   159,     0,     0,  1159,
       0,     0,  1165,     0,  1169,     0,     0,  1171,  1089,     0,
       0,     0,     0,     0,   336,     0,     0,   177,  1090,     0,
       0,     0,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,     0,   965,   457,   898,     0,     0,     0,     0,
     463,   464,     0,  1184,   467,  1185,     0,   468,   469,   470,
       0,   373,     0,     0,   393,     0,     0,     0,   471,     0,
       0,     0,     0,     0,  1202,   172,   333,     0,   172,     0,
       0,     0,     0,     0,  1125,  1127,     0,  1132,  1135,   177,
    1165,  1169,     0,     0,   177,     0,     0,     0,     0,     0,
    1217,  1218,     0,  1219,     0,   775,     0,  1221,  1222,     0,
       0,  1223,     0,   541,   541,     0,     0,   541,   541,   333,
       0,     0,     0,  1089,   172,     0,   336,   336,   159,   541,
     172,   541,     0,     0,   159,     0,     0,   172,  1233,     0,
    1234,     0,     0,     0,  1236,   159,     0,   159,     0,     0,
       0,     0,     0,  1240,   898,   898,   898,   898,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   177,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   180,     0,  1217,  1266,  1267,  1221,  1268,  1269,
    1270,  1271,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   177,     0,     0,   180,     0,   180,     0,
       0,     0,     0,     0,     0,     0,   159,   333,   442,     0,
     159,     0,  1286,     0,     0,   443,     0,     0,  1288,     0,
       0,     0,     0,     0,   159,  1118,  1119,   444,   987,     0,
       0,     0,     0,     0,     0,     0,   172,     0,   172,  1304,
    1305,  1306,  1307,   172,  1297,     0,   172,  1310,  1311,   180,
       0,     0,     0,   180,   180,   172,     0,   172,     0,     0,
     172,     0,     0,     0,     0,  1304,  1305,  1306,  1307,  1310,
    1311,     0,     0,     0,     0,     0,     0,   180,     0,     0,
       0,     0,   180,     0,   172,   445,     0,     0,     0,   446,
       0,     0,     0,     0,   172,     0,   172,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   333,
     333,     0,     0,     0,     0,   177,   522,     0,     0,     0,
     447,   177,     0,     0,     0,     0,   269,     0,     0,   373,
       0,     0,   177,     0,   177,     0,     0,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
       0,     0,   468,   469,   470,     0,   386,     0,     0,   181,
       0,     0,     0,   471,     0,     0,     0,     0,     0,   444,
       0,     0,   172,     0,     0,     0,     0,   713,   715,   172,
       0,   180,     0,   181,   186,   181,     0,     0,     0,     0,
       0,     0,   172,   177,     0,   159,     0,   177,   159,     0,
       0,     0,     0,     0,     0,     0,   172,     0,   186,     0,
     186,   177,     0,     0,     0,   172,   444,     0,     0,   172,
       0,   172,   172,   755,   757,     0,     0,   445,     0,     0,
       0,   446,     0,     0,     0,   777,   181,     0,     0,     0,
     181,   181,     0,   180,   159,   444,     0,     0,   180,     0,
     159,     0,   790,     0,     0,     0,     0,   159,   796,     0,
       0,   186,   447,   800,   181,   186,   186,     0,     0,   181,
       0,     0,     0,   172,   445,     0,     0,     0,   446,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,   186,
     456,   457,   458,   459,   186,   461,   462,   463,   464,   465,
     466,   467,     0,   445,   468,   469,   470,   446,     0,   172,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   172,   172,   172,   172,   180,   448,   449,     0,   172,
     172,   452,     0,   453,   454,     0,     0,     0,   457,     0,
       0,   856,     0,     0,     0,   464,   180,     0,     0,     0,
       0,   468,   469,   470,     0,   448,   449,   180,   450,   451,
     452,     0,   453,   454,     0,     0,   159,   457,   159,     0,
       0,     0,     0,   159,   464,     0,   159,     0,   181,     0,
     468,   469,   470,     0,     0,   159,     0,   159,     0,     0,
     159,     0,   177,     0,   907,   177,     0,     0,     0,   444,
       0,     0,     0,   186,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   159,  -325,     0,     0,     0,  -325,
    -325,     0,     0,     0,   159,     0,   159,     0,  -325,     0,
    -325,  -325,     0,     0,     0,     0,  -325,     0,     0,     0,
     181,   177,     0,  -325,     0,   181,  -325,   177,     0,     0,
       0,     0,     0,     0,   177,     0,     0,   445,     0,     0,
       0,   446,     0,     0,     0,   186,  -325,     0,     0,  -325,
     186,  -325,     0,  -325,     0,  -325,  -325,     0,  -325,   180,
       0,  -325,     0,  -325,     0,   180,     0,     0,     0,     0,
     444,     0,   941,     0,   944,     0,   180,   947,   180,     0,
       0,     0,  -325,     0,     0,  -325,     0,     0,  -325,   448,
     449,     0,   159,   451,   452,     0,   453,   454,     0,   159,
       0,   457,   181,     0,     0,     0,     0,     0,   464,     0,
       0,     0,   159,  -325,   468,   469,   470,     0,     0,     0,
       0,     0,     0,   181,     0,     0,   159,   186,   445,     0,
       0,     0,   446,     0,   181,   159,     0,     0,  -325,   159,
       0,   159,   159,   177,  -325,   177,     0,   180,   186,     0,
     177,   180,     0,   177,     0,     0,     0,     0,     0,   186,
       0,     0,   177,     0,   177,   180,     0,   177,   380,     0,
       0,     0,    23,    24,     0,     0,     0,  1041,  1043,  1045,
     448,   381,     0,    31,   382,   452,     0,   453,   454,    37,
       0,   177,   457,   159,     0,     0,    42,     0,     0,   464,
       0,   177,     0,   177,     0,   468,   469,   470,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    58,     0,    60,     0,   378,     0,  1086,   159,
       0,  1087,     0,     0,   385,     0,    69,     0,     0,     0,
       0,   159,   159,   159,   159,     0,     0,     0,   188,   159,
     159,     0,     0,     0,     0,    85,   181,     0,    87,     0,
       0,    89,   181,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   188,   181,   188,   181,     0,     0,   713,   755,
       0,   186,     0,     0,     0,     0,    94,   186,     0,   177,
       0,     0,     0,   189,     0,     0,   177,     0,   186,     0,
     186,     0,     0,     0,     0,     0,     0,     0,     0,   177,
       0,   105,     0,     0,     0,     0,     0,   189,     0,   189,
       0,     0,     0,   177,     0,   188,     0,     0,     0,   188,
     188,     0,   177,     0,     0,     0,   177,     0,   177,   177,
       0,     0,     0,     0,   181,     0,   180,     0,   181,   180,
       0,     0,     0,   188,     0,     0,     0,     0,   188,     0,
       0,     0,   181,     0,     0,     0,     0,     0,     0,   186,
     189,     0,     0,   186,   189,   189,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,     0,   186,     0,     0,
     177,     0,     0,     0,     0,   180,     0,     0,   189,   238,
     239,   180,   240,   189,     0,     0,     0,   241,   180,   380,
       0,     0,     0,    23,    24,   242,     0,     0,     0,     0,
       0,   243,   381,     0,    31,   382,   177,   244,     0,     0,
      37,   245,     0,     0,   246,     0,     0,    42,   177,   177,
     177,   177,     0,     0,   247,     0,   177,   177,   145,     0,
       0,   248,   249,     0,     0,     0,     0,     0,     0,   250,
       0,     0,     0,    58,     0,    60,     0,   188,     0,   251,
       0,     0,   145,     0,   145,   385,     0,    69,   252,   253,
       0,   254,     0,   255,     0,   256,     0,     0,   257,     0,
       0,     0,   258,   548,     0,   259,    85,     0,   260,    87,
       0,     0,    89,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   189,     0,     0,     0,     0,   180,     0,   180,
       0,     0,     0,     0,   180,   145,     0,   180,     0,   188,
       0,     0,     0,     0,   188,     0,   180,     0,   180,     0,
       0,   180,   237,   181,     0,     0,   181,     0,     0,     0,
       0,     0,     0,   145,   549,     0,   238,   239,   145,   240,
       0,     0,     0,     0,   241,   180,     0,     0,   186,     0,
       0,   186,   242,     0,   189,   180,     0,   180,   243,   189,
     723,     0,     0,     0,   244,     0,     0,     0,   245,     0,
       0,   246,   181,     0,   954,     0,     0,   955,   181,     0,
       0,   247,   956,     0,     0,   181,     0,     0,   248,   249,
       0,   188,   201,     0,     0,     0,   250,   186,     0,     0,
       0,     0,   957,   186,     0,     0,   251,     0,     0,   958,
     186,     0,   188,     0,     0,   252,   253,     0,   254,   959,
     255,     0,   256,   188,     0,   257,     0,   960,     0,   258,
     575,     0,   259,     0,     0,   260,   189,     0,     0,     0,
       0,     0,     0,   180,   961,     0,     0,   145,     0,     0,
     180,     0,     0,     0,     0,     0,   962,   189,     0,     0,
     727,     0,     0,   180,     0,     0,     0,   963,   189,     0,
       0,     0,     0,   964,     0,     0,     0,   180,     0,     0,
       0,     0,     0,     0,     0,     0,   180,     0,     0,     0,
     180,   576,   180,   180,   181,     0,   181,     0,     0,     0,
       0,   181,     0,     0,   181,     0,     0,   444,     0,   145,
       0,     0,     0,   181,   145,   181,     0,     0,   181,   186,
       0,   186,     0,     0,     0,     0,   186,     0,     0,   186,
       0,     0,     0,     0,     0,     0,     0,     0,   186,     0,
     186,     0,   181,   186,   180,   188,     0,     0,     0,     0,
       0,   188,   181,     0,   181,     0,     0,   113,     0,     0,
       0,     0,   188,     0,   188,   445,     0,   186,     0,   446,
       0,     0,     0,     0,     0,     0,     0,   186,     0,   186,
     180,   232,     0,   235,     0,     0,     0,     0,     0,     0,
     189,   145,   180,   180,   180,   180,   189,     0,     0,     0,
     180,   180,     0,     0,     0,     0,     0,   189,     0,   189,
       0,     0,   145,     0,     0,     0,     0,   448,   449,     0,
     450,   451,   452,   145,   453,   454,     0,     0,     0,   457,
       0,     0,     0,   188,   345,   463,   464,   188,     0,   467,
     181,     0,   468,   469,   470,     0,     0,   181,     0,     0,
       0,   188,     0,     0,     0,     0,     0,     0,     0,     0,
     181,     0,   113,     0,     0,   186,     0,   379,     0,     0,
       0,     0,   186,     0,   181,     0,     0,     0,   189,     0,
       0,     0,   189,   181,     0,   186,   289,   181,     0,   181,
     181,     0,     0,     0,     0,     0,   189,     0,     0,   186,
       0,     0,     0,     0,     0,     0,     0,     0,   186,     0,
     289,     0,   186,     0,   186,   186,     0,     0,     0,     0,
     349,   351,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   181,     0,     0,     0,   145,     0,   444,     0,     0,
       0,   145,     0,     0,   391,     0,     0,     0,     0,     0,
       0,     0,   145,     0,   145,     0,   186,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   500,   181,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   181,
     181,   181,   181,     0,     0,     0,     0,   181,   181,     0,
       0,     0,   186,     0,     0,   445,     0,     0,     0,   446,
       0,     0,     0,     0,   186,   186,   186,   186,     0,     0,
       0,     0,   186,   186,     0,     0,     0,     0,     0,     0,
       0,     0,   188,   145,     0,   188,     0,   145,   547,     0,
       0,     0,     0,   558,     0,     0,     0,     0,     0,     0,
       0,   145,     0,     0,     0,     0,     0,   448,   449,     0,
     450,   451,   452,   502,   453,   454,   455,     0,     0,   457,
     458,   459,     0,   461,   462,   463,   464,   189,     0,   467,
     189,   188,   468,   469,   470,     0,     0,   188,     0,     0,
       0,     0,     0,   471,   188,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   289,   289,   289,   289,   289,   289,
       0,     0,   537,   540,     0,     0,     0,     0,     0,   546,
     113,     0,     0,     0,     0,     0,   189,     0,   289,     0,
     289,     0,   189,     0,     0,     0,     0,   289,     0,   189,
       0,   113,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   113,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   289,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   442,
     289,   289,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,   188,     0,   188,     0,     0,   444,   288,
     188,     0,     0,   188,     0,     0,     0,     0,     0,     0,
       0,     0,   188,     0,   188,     0,     0,   188,     0,     0,
       0,     0,   145,     0,     0,   145,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,     0,
     189,   188,     0,     0,     0,   189,     0,     0,   189,     0,
       0,   188,     0,   188,     0,     0,   445,   189,     0,   189,
     446,   290,   189,     0,   558,     0,     0,     0,     0,     0,
     558,   145,     0,     0,     0,     0,     0,   145,     0,     0,
       0,   805,     0,     0,   145,   339,   189,   522,     0,     0,
       0,   447,     0,     0,     0,     0,   189,     0,   189,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   448,   449,
       0,   450,   451,   452,     0,   453,   454,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   289,     0,   468,   469,   470,     0,   386,     0,   188,
       0,     0,     0,     0,   471,     0,   188,     0,     0,     0,
       0,   289,   854,     0,     0,   289,   859,   289,     0,   188,
     289,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     113,     0,     0,   188,     0,     0,     0,     0,     0,     0,
       0,     0,   188,     0,   189,     0,   188,     0,   188,   188,
       0,   189,     0,   145,     0,   145,     0,     0,     0,     0,
     145,     0,     0,   145,   189,     0,     0,     0,     0,     0,
       0,     0,   145,     0,   145,     0,     0,   145,   189,     0,
       0,     0,     0,     0,     0,     0,     0,   189,     0,     0,
       0,   189,     0,   189,   189,     0,     0,     0,     0,     0,
     188,   145,     0,     0,   289,   289,     0,     0,     0,     0,
       0,   145,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   188,     0,     0,   518,
       0,   523,   527,   529,   532,   189,     0,     0,   188,   188,
     188,   188,     0,     0,     0,   262,   188,   188,     0,     0,
       0,     0,     0,   559,     0,   561,     0,     0,     0,   281,
     282,   283,   565,   442,     0,     0,     0,     0,     0,     0,
     443,   189,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   444,   189,   189,   189,   189,     0,     0,   145,
       0,   189,   189,     0,     0,     0,   145,     0,   503,     0,
       0,   997,   587,     0,  1000,     0,     0,     0,     0,   145,
       0,     0,     0,     0,     0,   593,   594,     0,     0,     0,
       0,     0,     0,   145,     0,     0,     0,     0,     0,     0,
       0,     0,   145,     0,     0,     0,   145,     0,   145,   145,
     445,     0,     0,     0,   446,     0,     0,     0,     0,     0,
    1034,     0,     0,     0,     0,     0,  1039,     0,     0,     0,
       0,     0,     0,   859,     0,     0,     0,   289,   289,     0,
     412,   220,   415,   289,   289,   447,     0,   289,   289,     0,
       0,     0,     0,     0,   435,     0,     0,  1002,     0,     0,
     145,     0,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,     0,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,   468,   469,   470,
     496,     0,     0,  1031,     0,     0,   145,     0,   471,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,   145,
     145,   145,     0,     0,     0,     0,   145,   145,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   779,     0,     0,     0,
       0,     0,   558,     0,   558,     0,     0,     0,     0,   558,
     516,     0,   558,     0,     0,     0,   793,     0,   534,   535,
     798,  1139,   799,  1140,     0,   802,  1142,     0,     0,   550,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   380,     0,     0,     0,    23,    24,     0,
    1158,     0,     0,     0,     0,     0,   381,     0,    31,   382,
    1164,     0,  1168,     0,    37,     0,     0,     0,     0,   577,
       0,    42,     0,     0,     0,   289,   289,     0,     0,     0,
       0,   289,     0,   540,     0,     0,     0,     0,     0,     0,
     540,     0,     0,   289,     0,     0,   289,    58,   289,    60,
     289,    62,     0,  1086,     0,     0,  1087,     0,     0,   385,
     527,    69,     0,  1147,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   262,     0,     0,     0,
      85,     0,     0,    87,     0,     0,    89,     0,     0,     0,
    1162,   237,     0,     0,     0,     0,     0,     0,   289,   289,
     289,   289,     0,     0,   640,   238,   239,     0,   240,   415,
       0,    94,     0,   241,     0,     0,     0,     0,  1225,     0,
       0,   242,     0,     0,     0,     0,     0,   243,     0,   699,
       0,     0,  1230,   244,     0,     0,   105,   245,     0,     0,
     246,  1235,  1088,     0,     0,  1237,     0,  1164,  1168,     0,
     247,     0,     0,     0,     0,     0,     0,   248,   249,     0,
       0,     0,     0,     0,     0,   250,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   251,     0,     0,     0,     0,
       0,     0,     0,     0,   252,   253,     0,   254,     0,   255,
       0,   256,     0,   767,   257,     0,     0,     0,   258,  1276,
       0,   259,     0,     0,   260,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   289,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   978,   980,     0,   113,     0,     0,   985,   988,
     814,     0,   990,   992,   550,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   827,   386,     0,
    1030,     0,     0,     0,     0,     0,   577,     0,     0,     0,
       0,   550,     0,     0,     0,   540,   540,     0,     0,   540,
     540,     0,     0,     0,     0,     0,     0,     0,     0,  1279,
       0,   540,     0,   540,     0,     0,     0,     0,     0,     6,
       7,     8,     9,    10,    11,    12,     0,     0,     0,    13,
       0,     0,     0,    17,     0,     0,     0,   200,     0,     0,
       0,     0,   307,     0,     0,     0,   885,     0,     0,     0,
     201,     0,     0,     0,     0,   202,   203,     0,     0,   204,
       0,     0,   309,     0,     0,     0,   311,     0,     0,     0,
     205,     0,     0,    47,    48,     0,     0,     0,     0,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,     0,
     312,     0,    59,   313,    61,    62,    63,     0,     0,     0,
       0,    66,     0,   206,     0,   314,     0,     0,     0,     0,
       0,    74,     0,    76,     0,    78,     0,     0,     0,     0,
       0,     0,   435,     0,   317,     0,     0,     0,    88,     0,
    1120,  1121,     0,   442,     0,     0,  1122,     0,     0,    93,
     443,     0,     0,     0,     0,     0,     0,     0,  1133,     0,
       0,  1136,   444,  1137,     0,  1138,    95,    96,    97,    98,
      99,     0,     0,     0,     0,   100,     0,     0,     0,     0,
       0,     0,     0,     0,   101,     0,     0,   102,   103,   104,
       0,     0,     0,   106,     0,   208,     0,   108,  -349,   209,
       0,     0,   110,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   978,   980,   985,   988,     0,     0,     0,
     445,     0,     0,     0,   446,     0,     0,     0,     0,     0,
       0,     0,     0,   538,     0,     0,  1003,     0,     0,     0,
       0,     0,     0,   550,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1012,     0,   447,   539,   473,   474,   475,
     476,   477,     0,     0,   480,   481,   482,   483,     0,   485,
     486,  1032,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,     0,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,   468,   469,   470,
       0,   386,     0,     0,     0,     0,     0,     0,   471,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   435,     0,
       0,  1231,     0,     0,     0,   415,     0,    -2,     4,     0,
       5,     6,     7,     8,     9,    10,    11,    12,     0,     0,
     699,    13,    14,    15,    16,    17,     0,    18,     0,    19,
      20,    21,    22,    23,    24,    25,    26,    27,     0,    28,
      29,     0,    30,     0,    31,    32,    33,    34,    35,    36,
      37,    38,    39,   -68,     0,    40,    41,    42,     0,    43,
    -345,     0,    44,    45,    46,    47,    48,     0,    49,    50,
      51,    52,   -68,    53,    54,     0,    55,    56,    57,     0,
    -345,     0,     0,    58,    59,    60,    61,    62,    63,    64,
    -345,   -68,    65,    66,    67,    68,     0,    69,    70,    71,
     814,    72,    73,    74,    75,    76,    77,    78,    79,     0,
      80,    81,     0,    82,    83,    84,    85,    86,     0,    87,
      88,   -68,    89,    90,     0,  1032,    91,     0,    92,     0,
       0,    93,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    94,    95,    96,
      97,    98,    99,     0,     0,     0,     0,   100,     0,     0,
       0,     0,     0,     0,     0,     0,   101,     0,     0,   102,
     103,   104,   105,     0,     0,   106,     0,   107,     0,   108,
       0,   109,     0,     0,   110,     0,     0,     4,   415,     5,
       6,     7,     8,     9,    10,    11,    12,     0,  -691,     0,
      13,    14,    15,    16,    17,  -691,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,  -691,    28,    29,
    -691,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -68,     0,    40,    41,    42,     0,    43,  -345,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -68,    53,    54,     0,    55,    56,    57,     0,  -345,
       0,     0,    58,    59,    60,    61,     0,    63,    64,  -345,
     -68,    65,    66,    67,    68,  -691,    69,    70,    71,  -691,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
     -68,    89,    90,     0,     0,    91,     0,    92,     0,     0,
    -691,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,  -691,    97,
    -691,  -691,  -691,  -691,  -691,  -691,  -691,     0,  -691,  -691,
    -691,  -691,  -691,  -691,  -691,  -691,  -691,  -691,  -691,  -691,
     104,   105,  -691,  -691,  -691,     0,   107,  -691,   108,     0,
     109,     0,   362,  -691,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -68,     0,    40,
      41,    42,     0,    43,  -345,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -68,    53,    54,     0,
      55,    56,    57,     0,  -345,     0,     0,    58,    59,    60,
      61,    62,    63,    64,  -345,   -68,    65,    66,    67,    68,
       0,    69,    70,    71,     0,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,    81,     0,    82,    83,    84,
      85,    86,     0,    87,    88,   -68,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,   105,     0,     0,   106,
       0,   107,   363,   108,     0,   109,     0,     4,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -68,     0,    40,    41,    42,     0,    43,  -345,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -68,    53,    54,     0,    55,    56,    57,     0,  -345,
       0,     0,    58,    59,    60,    61,    62,    63,    64,  -345,
     -68,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
     -68,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,   597,   108,     0,
     109,     0,   623,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -68,     0,    40,
      41,    42,     0,    43,  -345,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -68,    53,    54,     0,
      55,    56,    57,     0,  -345,     0,     0,    58,    59,    60,
      61,    62,    63,    64,  -345,   -68,    65,    66,    67,    68,
       0,    69,    70,    71,     0,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,    81,     0,    82,    83,    84,
      85,    86,     0,    87,    88,   -68,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,   105,     0,     0,   106,
       0,   107,   624,   108,     0,   109,     0,     4,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -68,     0,    40,    41,    42,     0,    43,  -345,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -68,    53,    54,     0,    55,    56,    57,     0,  -345,
       0,     0,    58,    59,    60,    61,    62,    63,    64,  -345,
     -68,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
     -68,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,   875,   108,     0,
     109,     0,     4,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -68,     0,    40,
      41,    42,     0,    43,  -345,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -68,    53,    54,     0,
      55,    56,    57,     0,  -345,     0,     0,    58,    59,    60,
      61,   378,    63,    64,  -345,   -68,    65,    66,    67,    68,
       0,    69,    70,    71,     0,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,    81,     0,    82,    83,    84,
      85,    86,     0,    87,    88,   -68,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,   105,     0,     0,   106,
       0,   107,     0,   108,     0,   109,     0,     4,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -68,     0,    40,    41,    42,     0,    43,  -345,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -68,    53,    54,     0,    55,    56,    57,     0,  -345,
       0,     0,    58,    59,    60,    61,    62,    63,    64,  -345,
     -68,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
     -68,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,     0,   108,     0,
     109,     0,     4,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,    38,    39,   -68,     0,    40,
      41,    42,     0,    43,  -345,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -68,    53,    54,     0,
      55,    56,    57,     0,  -345,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -345,   -68,    65,    66,    67,    68,
       0,    69,    70,    71,     0,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,    81,     0,    82,    83,    84,
      85,    86,     0,    87,    88,   -68,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,   105,     0,     0,   106,
       0,   107,     0,   108,     0,   109,     0,     4,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
     788,    39,   -68,     0,    40,    41,    42,     0,    43,  -345,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -68,    53,    54,     0,    55,    56,    57,     0,  -345,
       0,     0,    58,    59,    60,    61,     0,    63,    64,  -345,
     -68,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
     -68,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,     0,   108,     0,
     109,     0,     4,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,   794,    39,   -68,     0,    40,
      41,    42,     0,    43,  -345,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -68,    53,    54,     0,
      55,    56,    57,     0,  -345,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -345,   -68,    65,    66,    67,    68,
       0,    69,    70,    71,     0,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,    81,     0,    82,    83,    84,
      85,    86,     0,    87,    88,   -68,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,   105,     0,     0,   106,
       0,   107,     0,   108,     0,   109,     0,     4,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1124,    39,   -68,     0,    40,    41,    42,     0,    43,  -345,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -68,    53,    54,     0,    55,    56,    57,     0,  -345,
       0,     0,    58,    59,    60,    61,     0,    63,    64,  -345,
     -68,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
     -68,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,     0,   108,     0,
     109,     0,     4,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,  1126,    39,   -68,     0,    40,
      41,    42,     0,    43,  -345,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -68,    53,    54,     0,
      55,    56,    57,     0,  -345,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -345,   -68,    65,    66,    67,    68,
       0,    69,    70,    71,     0,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,    81,     0,    82,    83,    84,
      85,    86,     0,    87,    88,   -68,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,   105,     0,     0,   106,
       0,   107,     0,   108,     0,   109,     0,     4,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1131,    39,   -68,     0,    40,    41,    42,     0,    43,  -345,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -68,    53,    54,     0,    55,    56,    57,     0,  -345,
       0,     0,    58,    59,    60,    61,     0,    63,    64,  -345,
     -68,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
     -68,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,     0,   108,     0,
     109,     0,     4,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,  1134,    39,   -68,     0,    40,
      41,    42,     0,    43,  -345,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -68,    53,    54,     0,
      55,    56,    57,     0,  -345,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -345,   -68,    65,    66,    67,    68,
       0,    69,    70,    71,     0,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,    81,     0,    82,    83,    84,
      85,    86,     0,    87,    88,   -68,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,   105,     0,     0,   106,
       0,   107,     0,   108,     0,   109,     0,     4,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,  1157,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -68,     0,    40,    41,    42,     0,    43,  -345,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -68,    53,    54,     0,    55,    56,    57,     0,  -345,
       0,     0,    58,    59,    60,    61,     0,    63,    64,  -345,
     -68,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
     -68,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,     0,   108,     0,
     109,     0,     4,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,    15,    16,    17,
       0,    18,     0,    19,    20,    21,    22,    23,    24,    25,
      26,    27,     0,    28,    29,     0,    30,     0,    31,    32,
      33,    34,    35,    36,    37,  1163,    39,   -68,     0,    40,
      41,    42,     0,    43,  -345,     0,    44,    45,    46,    47,
      48,     0,    49,    50,    51,    52,   -68,    53,    54,     0,
      55,    56,    57,     0,  -345,     0,     0,    58,    59,    60,
      61,     0,    63,    64,  -345,   -68,    65,    66,    67,    68,
       0,    69,    70,    71,     0,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,    81,     0,    82,    83,    84,
      85,    86,     0,    87,    88,   -68,    89,    90,     0,     0,
      91,     0,    92,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    94,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,   105,     0,     0,   106,
       0,   107,     0,   108,     0,   109,     0,     4,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,    14,    15,    16,    17,     0,    18,     0,    19,    20,
      21,    22,    23,    24,    25,    26,    27,     0,    28,    29,
       0,    30,     0,    31,    32,    33,    34,    35,    36,    37,
    1167,    39,   -68,     0,    40,    41,    42,     0,    43,  -345,
       0,    44,    45,    46,    47,    48,     0,    49,    50,    51,
      52,   -68,    53,    54,     0,    55,    56,    57,     0,  -345,
       0,     0,    58,    59,    60,    61,     0,    63,    64,  -345,
     -68,    65,    66,    67,    68,     0,    69,    70,    71,     0,
      72,    73,    74,    75,    76,    77,    78,    79,     0,    80,
      81,     0,    82,    83,    84,    85,    86,     0,    87,    88,
     -68,    89,    90,     0,     0,    91,     0,    92,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    94,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,   105,     0,     0,   106,     0,   107,     0,   108,     0,
     109,     0,   915,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,   308,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
     310,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,   315,    76,
      77,    78,   316,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,   661,     0,
      13,     0,     0,    16,    17,   663,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
     668,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,   539,   473,   474,   475,   476,   477,     0,     0,   480,
     481,   482,   483,     0,   485,   486,     0,   886,   887,   888,
     889,   890,   681,     0,   682,     0,   100,     0,   683,   684,
     685,   686,   687,   688,   689,   891,   691,   692,   102,   892,
     104,     0,   694,   695,   893,   697,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,    14,     0,    16,    17,
       0,    18,     0,   200,    20,    21,    22,     0,     0,     0,
       0,    27,     0,    28,    29,     0,   201,     0,     0,     0,
      33,    34,    35,    36,     0,    38,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,    49,    50,    51,    52,     0,    53,    54,     0,
      55,    56,    57,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,    72,    73,    74,    75,    76,
      77,    78,    79,     0,    80,    81,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
      91,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   107,     0,   108,     0,   109,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,   402,    24,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,    60,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,    69,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,    89,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,   330,   331,
       0,    86,   367,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,   368,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,   768,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,   769,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,   770,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,   771,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,   330,   331,
       0,    86,   367,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,   850,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,   330,   331,     0,    86,   367,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,   852,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,   330,   331,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,   332,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,   330,   331,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,   870,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,  1010,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,   330,   331,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,   265,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,   266,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,   273,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,   274,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,   330,   331,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,   266,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,   372,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,  -735,     0,     0,     0,  -735,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,   266,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,   288,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   107,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,   405,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,    19,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,    68,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,    87,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,   372,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,   848,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,   858,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,   863,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,  1072,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,  1239,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,   308,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
     310,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,   315,    76,
      77,    78,   316,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     940,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   943,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
    1208,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,  1209,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
    1211,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,  1212,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     0,     0,   110,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,     0,   200,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
    1213,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
       0,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,     0,     0,     0,     0,     0,     0,
      93,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    95,    96,    97,
      98,    99,     0,     0,     0,     0,   100,     0,     0,     0,
       0,     0,     0,     0,     0,   101,     0,     0,   102,   103,
     104,     0,     0,     0,   106,     0,   208,     0,   108,     0,
     209,     0,     0,   110,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,  1214,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,   444,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,    93,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   445,     0,     0,     0,   446,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,   100,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,   102,   103,   104,     0,     0,     0,   106,
     447,   208,     0,   108,     0,   209,     0,     0,   110,     6,
       7,     8,     9,    10,    11,    12,     0,   448,   449,    13,
     450,   451,   452,    17,   453,   454,   455,   200,   456,   457,
     458,   459,   307,   461,   462,   463,   464,   465,     0,   467,
     201,     0,   468,   469,   470,   202,   203,     0,     0,   204,
       0,     0,   309,   471,     0,     0,   311,     0,     0,     0,
     205,     0,     0,    47,    48,     0,     0,     0,     0,    52,
       0,    53,    54,     0,     0,     0,     0,     0,     0,     0,
     312,     0,    59,   313,    61,    62,    63,     0,     0,     0,
       0,    66,     0,   206,     0,   314,     0,     0,     0,     0,
       0,    74,     0,    76,     0,    78,     0,     0,     0,     0,
       0,     0,     0,     0,   317,     0,     0,     0,    88,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    93,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   659,     0,   660,     0,    95,    96,    97,    98,
      99,     0,     0,   661,     0,   100,     0,     0,   662,   239,
     663,   664,     0,     0,   101,     0,   665,   102,   103,   104,
       0,     0,     0,   106,   242,   208,   201,   108,     0,   209,
     243,     0,   110,     0,     0,     0,   666,     0,     0,     0,
     245,     0,     0,   667,     0,   668,     0,     0,     0,     0,
       0,     0,     0,   669,     0,     0,     0,     0,     0,     0,
     248,   670,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   671,     0,
       0,     0,     0,     0,     0,     0,     0,   252,   253,     0,
     672,     0,   255,     0,   673,     0,     0,   257,     0,     0,
       0,   674,     0,     0,   259,     0,     0,   675,     0,     0,
       0,     0,     0,     0,     0,     0,   539,   473,   474,   475,
     476,   477,     0,     0,   480,   481,   482,   483,     0,   485,
     486,     0,   676,   677,   678,   679,   680,   681,     0,   682,
       0,     0,     0,   683,   684,   685,   686,   687,   688,   689,
     690,   691,   692,     0,   693,     0,     0,   694,   695,   696,
     697,     0,     0,   698,     5,     6,     7,     8,     9,    10,
      11,    12,     0,     0,     0,    13,     0,     0,    16,    17,
       0,    18,     0,   200,     0,     0,    22,     0,     0,     0,
       0,     0,     0,     0,    29,     0,   201,     0,     0,     0,
      33,   202,   203,     0,     0,   204,    39,     0,     0,     0,
      41,     0,     0,    43,     0,     0,   205,     0,     0,    47,
      48,     0,     0,    50,     0,    52,     0,    53,    54,     0,
      55,    56,     0,     0,     0,     0,     0,     0,    59,     0,
      61,     0,    63,     0,     0,     0,     0,    66,    67,   206,
       0,     0,     0,     0,     0,     0,     0,    74,    75,    76,
      77,    78,   207,     0,    80,     0,     0,    82,     0,     0,
       0,    86,     0,     0,    88,     0,     0,    90,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    95,    96,    97,    98,    99,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     101,     0,     0,     0,   103,   104,     0,     0,     0,   106,
       0,   208,     0,   108,     0,   209,     5,     6,     7,     8,
       9,    10,    11,    12,     0,     0,     0,    13,     0,     0,
      16,    17,     0,    18,     0,   200,     0,     0,    22,     0,
       0,     0,     0,     0,     0,     0,    29,     0,   201,     0,
       0,     0,    33,   202,   203,     0,     0,   204,    39,     0,
       0,     0,    41,     0,     0,    43,     0,     0,   205,     0,
       0,    47,    48,     0,     0,    50,     0,    52,     0,    53,
      54,     0,    55,    56,     0,     0,     0,     0,     0,     0,
      59,     0,    61,     0,    63,     0,     0,     0,     0,    66,
      67,   206,     0,     0,     0,     0,     0,     0,     0,    74,
      75,    76,    77,    78,   207,     0,    80,     0,     0,    82,
       0,     0,     0,    86,     0,     0,    88,     0,     0,    90,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    95,     0,    97,     0,     0,     5,
       6,     7,     8,     9,    10,    11,    12,     0,     0,     0,
      13,     0,     0,    16,    17,     0,    18,   104,   200,     0,
       0,    22,     0,   208,     0,   108,     0,   209,     0,    29,
       0,   201,     0,     0,     0,    33,   202,   203,     0,     0,
     204,    39,     0,     0,     0,    41,     0,     0,    43,     0,
       0,   205,     0,     0,    47,    48,     0,     0,    50,     0,
      52,     0,    53,    54,     0,    55,    56,     0,     0,     0,
       0,     0,     0,    59,     0,    61,     0,    63,     0,     0,
       0,     0,    66,    67,   206,     0,     0,     0,     0,     0,
       0,     0,    74,    75,    76,    77,    78,   207,     0,    80,
     237,     0,    82,     0,     0,     0,    86,     0,     0,    88,
       0,     0,    90,     0,   238,   239,     0,   240,     0,     0,
       0,     0,   241,     0,     0,     0,     0,     0,     0,     0,
     242,     0,     0,     0,     0,     0,   243,    95,     0,    97,
       0,     0,   244,     0,     0,     0,   245,     0,     0,   246,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   247,
     104,     0,     0,     0,     0,     0,   248,   249,   108,     0,
     209,     0,     0,     0,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   251,     0,     0,     0,     0,     0,
       0,     0,     0,   252,   253,     0,   254,     0,   255,     0,
     256,     0,     0,   257,     0,   723,     0,   258,     0,     0,
     259,     0,     0,   260,   661,     0,     0,     0,     0,  1051,
    1052,   663,  1053,     0,     0,     0,     0,  1054,     0,     0,
       0,     0,     0,     0,     0,   725,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1055,     0,     0,
       0,  1056,     0,     0,  1057,     0,   668,     0,     0,     0,
       0,     0,     0,     0,   726,     0,     0,     0,     0,   284,
       0,     0,  1058,     0,     0,     0,     0,     0,     0,  1059,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1060,
       0,     0,     0,     0,     0,     0,     0,     0,  1061,  1062,
       0,  1063,     0,  1064,     0,   727,     0,     0,     0,     0,
       0,   728,  1065,     0,     0,  1066,     0,     0,  1067,     0,
       0,     0,     0,     0,     0,     0,     0,   539,   473,   474,
     475,   476,   477,     0,     0,   480,   481,   482,   483,     0,
     485,   486,     0,   676,   677,   678,   679,   680,   681,     0,
     682,     0,     0,     0,   683,   684,   685,   686,   687,   688,
     689,   690,   691,   692,   237,   693,     0,     0,   694,   695,
     696,   697,  1068,   661,     0,     0,     0,     0,   238,   239,
     663,   240,     0,     0,     0,     0,   241,     0,     0,     0,
       0,     0,     0,     0,   242,     0,     0,     0,     0,     0,
     243,     0,     0,     0,     0,     0,   244,     0,     0,     0,
     245,     0,     0,   246,     0,   668,     0,     0,     0,     0,
       0,     0,     0,   247,     0,     0,     0,     0,     0,     0,
     248,   249,     0,     0,     0,     0,     0,     0,   250,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,     0,
       0,     0,     0,     0,   442,     0,     0,   252,   253,     0,
     254,   443,   255,     0,   256,     0,     0,   257,     0,     0,
       0,   258,     0,   444,   259,     0,     0,   260,     0,     0,
       0,     0,     0,     0,     0,     0,   539,   473,   474,   475,
     476,   477,     0,     0,   480,   481,   482,   483,     0,   485,
     486,     0,   676,   677,   678,   679,   680,   681,     0,   682,
       0,     0,     0,   683,   684,   685,   686,   687,   688,   689,
     690,   691,   692,     0,   693,     0,     0,   694,   695,   696,
     697,   445,     0,     0,   442,   446,     0,     0,     0,     0,
       0,   443,     0,     0,  1123,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   539,   473,   474,
     475,   476,   477,     0,     0,   480,   481,   482,   483,     0,
     485,   486,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,   468,   469,
     470,   445,   386,     0,   442,   446,     0,     0,     0,   471,
       0,   443,     0,     0,  1130,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   539,   473,   474,
     475,   476,   477,     0,     0,   480,   481,   482,   483,     0,
     485,   486,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,   468,   469,
     470,   445,   386,     0,   442,   446,     0,     0,     0,   471,
       0,   443,     0,     0,  1272,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   539,   473,   474,
     475,   476,   477,     0,     0,   480,   481,   482,   483,     0,
     485,   486,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,   468,   469,
     470,   445,   386,     0,   442,   446,     0,     0,     0,   471,
       0,   443,     0,     0,  1273,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   539,   473,   474,
     475,   476,   477,     0,     0,   480,   481,   482,   483,     0,
     485,   486,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,   468,   469,
     470,   445,   386,     0,   442,   446,     0,     0,     0,   471,
       0,   443,     0,     0,  1274,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   539,   473,   474,
     475,   476,   477,     0,     0,   480,   481,   482,   483,     0,
     485,   486,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,   468,   469,
     470,   445,   386,     0,   442,   446,     0,     0,     0,   471,
       0,   443,     0,     0,  1275,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   539,   473,   474,
     475,   476,   477,     0,     0,   480,   481,   482,   483,     0,
     485,   486,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,   468,   469,
     470,   445,   386,     0,   442,   446,     0,     0,     0,   471,
       0,   443,     0,     0,  1280,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,   539,   473,   474,
     475,   476,   477,     0,     0,   480,   481,   482,   483,     0,
     485,   486,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,   468,   469,
     470,   445,   386,     0,     0,   446,     0,     0,     0,   471,
       0,     0,     0,     0,  1281,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   442,     0,     0,     0,     0,     0,
       0,   443,     0,     0,     0,     0,   447,   539,   473,   474,
     475,   476,   477,   444,   525,   480,   481,   482,   483,     0,
     485,   486,     0,   448,   449,     0,   450,   451,   452,   526,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,   468,   469,
     470,     0,   386,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     442,   445,     0,     0,     0,   446,     0,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   444,
     288,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   522,     0,     0,   531,   447,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   442,   445,   468,   469,
     470,   446,   386,   443,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,   444,   288,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   522,     0,
       0,   866,   447,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   442,   445,   468,   469,   470,   446,   386,   443,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   444,   519,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   522,     0,     0,   520,   447,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,     0,   445,
     468,   469,   470,   446,   386,     0,     0,     0,     0,     0,
       0,   471,     0,     0,   442,     0,     0,     0,     0,     0,
       0,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   444,   447,     0,     0,     0,   632,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   633,     0,   468,   469,   470,     0,
     386,     0,     0,     0,     0,     0,     0,   471,     0,     0,
       0,   445,     0,     0,   442,   446,     0,     0,     0,     0,
       0,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   444,   288,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,     0,     0,   865,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,   634,   468,   469,
     470,   445,     0,     0,     0,   446,     0,     0,     0,   471,
       0,     0,     0,     0,     0,     0,   442,   882,     0,     0,
       0,     0,     0,   443,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   444,   447,     0,     0,     0,
     883,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   884,     0,   468,   469,
     470,     0,   386,     0,     0,     0,     0,     0,     0,   471,
       0,     0,   442,   445,     0,     0,     0,   446,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   444,   288,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   442,   445,
     468,   469,   470,   446,     0,   443,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   444,   759,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   760,   447,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   442,   445,   468,   469,   470,   446,
     386,   443,     0,     0,     0,     0,     0,   471,     0,     0,
       0,     0,     0,   444,   761,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   762,
     447,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     442,   445,   468,   469,   470,   446,     0,   443,     0,     0,
       0,     0,     0,   471,     0,     0,     0,     0,     0,   444,
     979,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,   442,   445,   468,   469,
     470,   446,     0,   443,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,   444,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   447,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   442,   445,   468,   469,   470,   446,   386,   443,
       0,     0,     0,     0,     0,   471,  1001,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   442,   445,
     468,   469,   470,   446,   386,   443,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     522,     0,     0,     0,   447,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,   445,   468,   469,   470,   446,
       0,     0,     0,     0,     0,  1037,     0,   471,     0,     0,
     442,     0,     0,     0,     0,     0,     0,   443,     0,     0,
       0,     0,     0,     0,     0,     0,   522,     0,     0,   444,
     447,     0,     0,     0,  1179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
    1180,   444,   468,   469,   470,     0,     0,     0,     0,     0,
       0,  1173,     0,   471,     0,     0,     0,   445,     0,     0,
       0,   446,   442,   562,     0,     0,     0,     0,     0,   443,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   447,     0,     0,     0,     0,     0,     0,   445,
       0,     0,     0,   446,     0,     0,     0,     0,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,     0,     0,   468,   469,   470,     0,     0,   445,
       0,     0,   442,   446,     0,   471,     0,     0,     0,   443,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,   444,   456,   457,   458,   459,     0,   461,   462,   463,
     464,   465,     0,   467,   447,     0,   468,   469,   470,     0,
       0,     0,   442,     0,     0,     0,     0,   471,     0,   443,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,   444,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,     0,   468,   469,   470,   445,
       0,     0,     0,   446,     0,     0,     0,   471,   442,     0,
       0,     0,     0,     0,     0,   443,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,   447,     0,     0,     0,     0,   445,
       0,     0,     0,   446,     0,     0,     0,     0,     0,     0,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   447,     0,   468,   469,   470,     0,
     586,     0,     0,     0,     0,   445,     0,   471,     0,   446,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   442,   590,   468,   469,   470,   613,
     447,   443,     0,     0,     0,     0,     0,   471,     0,     0,
       0,     0,     0,   444,     0,     0,     0,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     442,     0,   468,   469,   470,     0,     0,   443,     0,     0,
       0,     0,     0,   471,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   445,     0,     0,     0,   446,     0,     0,     0,     0,
     442,   878,     0,     0,   763,     0,     0,   443,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,   447,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   445,     0,     0,
       0,   446,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,   468,   469,
     470,     0,   447,     0,     0,     0,   442,   445,     0,   471,
       0,   446,     0,   443,     0,     0,     0,     0,     0,   448,
     449,     0,   450,   451,   452,   444,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   447,     0,   468,   469,   470,     0,     0,     0,
       0,   849,     0,     0,     0,   471,     0,     0,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   879,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   442,   445,   468,   469,   470,   446,     0,   443,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   220,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,     0,   445,
     468,   469,   470,   446,     0,   442,     0,     0,     0,     0,
       0,   471,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,   999,     0,     0,
       0,     0,     0,     0,   447,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,     0,   971,   468,   469,   470,     0,
       0,   442,   445,     0,     0,     0,   446,   471,   443,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     444,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   447,     0,     0,
       0,   442,     0,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,   448,   449,     0,   450,   451,   452,
     444,   453,   454,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,   445,   468,
     469,   470,   446,     0,     0,     0,     0,   442,     0,     0,
     471,     0,     0,     0,   443,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   444,     0,     0,     0,
       0,     0,     0,   447,     0,     0,     0,     0,   445,     0,
       0,     0,   446,     0,     0,     0,     0,     0,     0,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
       0,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   447,     0,   468,   469,   470,     0,     0,
       0,     0,  1013,     0,   445,     0,   471,     0,   446,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
       0,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   442,  1078,   468,   469,   470,  1040,   447,
     443,     0,  1014,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   444,     0,     0,     0,   448,   449,     0,   450,
     451,   452,     0,   453,   454,   455,     0,   456,   457,   458,
     459,   460,   461,   462,   463,   464,   465,   466,   467,   442,
       0,   468,   469,   470,     0,     0,   443,     0,     0,     0,
       0,     0,   471,     0,     0,     0,     0,     0,   444,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     445,     0,     0,     0,   446,     0,     0,     0,     0,   442,
       0,     0,     0,     0,     0,     0,   443,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   444,  1108,
       0,     0,     0,     0,     0,   447,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   445,     0,     0,     0,
     446,     0,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,     0,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,   468,   469,   470,
       0,   447,     0,     0,     0,   442,   445,     0,   471,     0,
     446,     0,   443,     0,     0,     0,     0,     0,   448,   449,
       0,   450,   451,   452,   444,   453,   454,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   447,     0,   468,   469,   470,     0,     0,     0,     0,
    1101,     0,     0,     0,   471,     0,     0,     0,   448,   449,
       0,   450,   451,   452,     0,   453,   454,   455,     0,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   442,   445,   468,   469,   470,   446,     0,   443,     0,
       0,     0,     0,     0,   471,     0,     0,     0,     0,     0,
     444,  1111,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   447,     0,     0,
       0,   442,  1176,     0,     0,     0,     0,     0,   443,     0,
       0,     0,     0,     0,   448,   449,     0,   450,   451,   452,
     444,   453,   454,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,   445,   468,
     469,   470,   446,     0,     0,     0,   442,     0,  1114,     0,
     471,     0,     0,   443,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   444,     0,     0,     0,     0,
       0,     0,     0,   447,     0,     0,     0,     0,   445,     0,
       0,     0,   446,     0,     0,     0,     0,     0,     0,     0,
     448,   449,     0,   450,   451,   452,     0,   453,   454,   455,
       0,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   447,     0,   468,   469,   470,     0,     0,
       0,     0,     0,   445,     0,     0,   471,   446,     0,     0,
     448,   449,     0,   450,   451,   452,  1207,   453,   454,   455,
       0,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   442,     0,   468,   469,   470,   447,     0,
     443,     0,     0,     0,     0,     0,   471,     0,     0,     0,
       0,     0,   444,     0,     0,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   442,  1245,
     468,   469,   470,     0,     0,   443,     0,     0,     0,     0,
       0,   471,     0,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     445,     0,     0,     0,   446,     0,     0,     0,     0,     0,
       0,     0,     0,  1210,   442,     0,     0,     0,     0,     0,
       0,   443,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   444,     0,   447,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   445,     0,     0,     0,   446,
       0,     0,   448,   449,     0,   450,   451,   452,     0,   453,
     454,   455,  1259,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,     0,     0,   468,   469,   470,
     447,     0,     0,     0,     0,     0,     0,     0,   471,     0,
       0,   445,     0,     0,     0,   446,     0,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     442,     0,   468,   469,   470,     0,   447,   443,     0,     0,
       0,     0,     0,   471,     0,     0,     0,     0,     0,   444,
       0,     0,     0,   448,   449,     0,   450,   451,   452,     0,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,     0,   468,   469,
     470,     0,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   442,   445,     0,     0,
       0,   446,     0,   443,     0,     0,     0,     0,     0,     0,
    1298,     0,     0,     0,     0,   444,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   447,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   442,   445,   468,   469,   470,   446,     0,   443,
       0,     0,     0,     0,     0,   471,  1299,     0,     0,     0,
       0,   444,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   447,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   448,   449,     0,   450,   451,
     452,     0,   453,   454,   455,     0,   456,   457,   458,   459,
     460,   461,   462,   463,   464,   465,   466,   467,   442,   445,
     468,   469,   470,   446,     0,   443,     0,     0,     0,     0,
       0,   471,  1300,     0,     0,     0,     0,   444,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   448,   449,     0,   450,   451,   452,     0,   453,   454,
     455,     0,   456,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   467,   442,   445,   468,   469,   470,   446,
       0,   443,     0,     0,     0,     0,     0,   471,  1301,     0,
       0,     0,     0,   444,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     447,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   448,   449,     0,
     450,   451,   452,     0,   453,   454,   455,     0,   456,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     442,   445,   468,   469,   470,   446,     0,   443,     0,     0,
       0,     0,     0,   471,  1302,     0,     0,     0,     0,   444,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   447,     0,     0,     0,
     442,     0,     0,     0,     0,     0,     0,   443,     0,     0,
       0,     0,     0,   448,   449,     0,   450,   451,   452,   444,
     453,   454,   455,     0,   456,   457,   458,   459,   460,   461,
     462,   463,   464,   465,   466,   467,     0,   445,   468,   469,
     470,   446,     0,     0,     0,   442,     0,     0,     0,   471,
    1303,     0,   443,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   444,     0,     0,     0,     0,     0,
       0,     0,   447,     0,     0,     0,     0,   445,     0,     0,
       0,   446,     0,     0,     0,     0,     0,     0,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,   447,     0,   468,   469,   470,     0,     0,     0,
       0,     0,   966,     0,     0,   471,   446,     0,     0,   448,
     449,     0,   450,   451,   452,     0,   453,   454,   455,     0,
     456,   457,   458,   459,   460,   461,   462,   463,   464,   465,
     466,   467,     0,     0,   468,   469,   470,   447,     0,     0,
       0,     0,     0,     0,     0,   471,     0,     0,     0,     0,
       0,     0,     0,     0,   448,   449,     0,   450,   451,   452,
       0,   453,   454,   455,     0,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,   467,     0,     0,   468,
     469,   470,   380,     0,    21,     0,    23,    24,     0,     0,
     471,     0,    28,     0,     0,   381,     0,    31,   382,     0,
     383,   384,    36,    37,     0,     0,     0,     0,     0,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -68,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    58,     0,    60,     0,
       0,     0,    64,     0,   -68,    65,     0,     0,   385,   380,
      69,     0,     0,    23,    24,     0,     0,     0,     0,     0,
       0,     0,   381,     0,    31,   382,     0,     0,     0,    85,
      37,     0,    87,     0,     0,    89,     0,    42,     0,    91,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   380,     0,     0,     0,    23,
      24,     0,     0,    58,     0,    60,     0,    62,   381,  1086,
      31,   382,  1087,     0,     0,   385,    37,    69,     0,     0,
       0,     0,     0,    42,     0,   105,     0,     0,     0,     0,
     386,     0,     0,     0,   387,     0,    85,     0,     0,    87,
       0,   380,    89,     0,     0,    23,    24,     0,     0,    58,
       0,    60,     0,    62,   381,  1086,    31,   382,  1087,     0,
       0,   385,    37,    69,     0,     0,     0,    94,     0,    42,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,     0,     0,    87,     0,     0,    89,     0,
       0,     0,   105,     0,     0,    58,     0,    60,  1187,     0,
       0,  1086,     0,   413,  1087,   237,     0,   385,     0,    69,
       0,     0,     0,    94,     0,     0,     0,     0,     0,   238,
     239,     0,   240,     0,     0,     0,     0,   241,    85,     0,
       0,    87,     0,     0,    89,   242,     0,     0,   105,     0,
       0,   243,     0,     0,  1246,     0,     0,   244,     0,     0,
       0,   245,     0,     0,   246,     0,     0,     0,     0,    94,
     237,     0,     0,     0,   247,     0,     0,     0,     0,     0,
       0,   248,   249,     0,   238,   239,     0,   240,     0,   250,
       0,     0,   241,     0,   105,     0,     0,     0,     0,   251,
     242,     0,     0,     0,     0,     0,   243,     0,   252,   253,
       0,   254,   244,   255,     0,   256,   245,     0,   257,   246,
       0,     0,   258,     0,     0,   259,     0,     0,   260,   247,
       0,     0,     0,     0,     0,     0,   248,   249,     0,     0,
       0,     0,     0,     0,   250,   237,     0,     0,     0,     0,
       0,     0,    94,     0,   251,     0,     0,     0,     0,   238,
     239,     0,   240,   252,   253,     0,   254,   241,   255,    24,
     256,     0,     0,   257,     0,   242,     0,   258,     0,     0,
     259,   243,     0,   260,     0,     0,     0,   244,     0,     0,
       0,   245,     0,     0,   246,     0,     0,     0,     0,     0,
     237,     0,     0,     0,   247,     0,     0,    94,     0,     0,
       0,   248,   249,     0,   238,   239,     0,   240,     0,   250,
      60,     0,   241,     0,     0,     0,     0,     0,     0,   251,
     242,     0,    69,     0,     0,     0,   243,     0,   252,   253,
       0,   254,   244,   255,     0,   256,   245,     0,   257,   246,
       0,     0,   258,     0,     0,   259,     0,    89,   260,   247,
       0,     0,     0,     0,     0,     0,   248,   249,     0,     0,
       0,     0,     0,     0,   250,  -320,     0,     0,     0,     0,
       0,     0,     0,     0,   251,     0,     0,     0,     0,  -320,
    -320,     0,  -320,   252,   253,     0,   254,  -320,   255,     0,
     256,     0,     0,   257,     0,  -320,     0,   258,   575,     0,
     259,  -320,     0,   260,     0,     0,     0,  -320,     0,     0,
       0,  -320,     0,     0,  -320,     0,     0,     0,     0,     0,
     237,     0,     0,     0,  -320,     0,     0,     0,     0,     0,
       0,  -320,  -320,     0,   238,   239,     0,   240,     0,  -320,
       0,     0,   241,     0,     0,     0,     0,     0,     0,  -320,
     242,     0,     0,     0,     0,     0,   243,     0,  -320,  -320,
       0,  -320,   244,  -320,     0,  -320,   245,     0,  -320,   246,
       0,     0,  -320,     0,     0,  -320,     0,     0,  -320,   247,
       0,     0,     0,     0,     0,     0,   248,   249,     0,     0,
       0,     0,     0,     0,   250,  -321,     0,     0,     0,     0,
       0,     0,     0,     0,   251,     0,     0,     0,     0,  -321,
    -321,     0,  -321,   252,   253,     0,   254,  -321,   255,     0,
     256,     0,     0,   257,     0,  -321,     0,   258,     0,     0,
     259,  -321,     0,   260,     0,     0,     0,  -321,     0,     0,
       0,  -321,     0,     0,  -321,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -321,     0,     0,     0,     0,     0,
       0,  -321,  -321,     0,     0,     0,     0,     0,     0,  -321,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -321,
       0,     0,     0,     0,     0,     0,     0,     0,  -321,  -321,
       0,  -321,     0,  -321,     0,  -321,     0,     0,  -321,     0,
       0,     0,  -321,     0,     0,  -321,     0,     0,  -321
};

static const yytype_int16 yycheck[] =
{
      13,   118,   588,   414,    17,   170,     2,   280,    21,   157,
     229,   499,   584,    66,    27,    66,   544,   434,    66,    20,
      27,    34,    35,    36,   792,    38,   632,   655,   884,   838,
     139,   304,   579,   406,   652,   621,    49,   908,    51,    52,
     879,   832,   902,     1,    57,     8,    59,    32,    61,   419,
      32,    55,     1,    66,    32,    31,   554,    70,    71,    72,
      73,    74,    75,    70,  1015,   120,    79,   701,    81,   703,
       1,   344,   914,    60,   236,    88,     1,    18,    91,    92,
     102,    40,    95,    27,    97,    71,  1203,   100,   101,    53,
     103,   186,    32,    32,   107,   108,   109,   110,   123,   120,
      32,    32,   109,    36,     1,    55,    92,    32,   120,    73,
      92,   549,    47,    89,   147,     0,    74,    93,   280,    83,
     175,    32,   108,    32,   137,    47,   139,   147,   165,    60,
     165,   118,  1249,     3,    78,    60,    77,   172,   576,    72,
     173,    47,   304,     3,     3,   170,    90,    89,   170,   118,
      47,    93,    92,   173,   175,    96,    47,   655,   117,   117,
      89,    92,   147,   175,    93,   141,   179,    92,   172,   147,
     146,   115,   113,   149,   105,  1017,   125,   153,   147,     3,
     105,   276,   344,   165,   952,   170,   149,   118,   170,   202,
     203,   204,   170,   118,   207,   208,   209,   146,   147,   785,
    1009,   819,   209,   573,  1155,   269,  1015,   271,   272,   120,
    1081,   120,   147,   171,   120,   146,   489,   490,  1089,  1090,
    1080,   146,   172,   236,   173,   147,   227,  1018,   125,   120,
     170,   170,    92,   103,   165,     3,   764,  1076,   170,   170,
     175,   147,    47,   103,   103,   170,    22,    74,   147,   146,
     147,    71,    20,    55,   147,    46,   147,   147,   147,   170,
     147,   170,   635,    27,   140,    33,   120,   280,   165,   175,
      47,   147,    92,    55,    65,   165,   175,   883,   109,    27,
     173,     2,   171,    47,   175,    90,   173,    51,   108,   836,
     117,   304,   454,    84,    62,   171,   405,   414,    90,    46,
      46,    27,   315,   316,   792,    26,   147,    28,  1117,   373,
     374,    75,    88,    90,    78,  1186,    80,   330,   331,    65,
    1191,    47,  1178,   114,  1180,    51,    90,   489,   490,   172,
      78,   344,   173,    27,   172,   103,   112,    84,    84,   434,
     505,   109,    90,   562,   171,   109,  1155,   965,   141,    75,
     147,   147,    78,   147,    80,   368,   149,   147,    79,   372,
    1128,   109,    83,    84,    90,   418,   147,   114,   114,   365,
     383,   384,   125,    55,   387,   165,   173,   173,   906,   173,
     387,    75,   544,   109,    78,   398,   107,   400,  1016,   147,
     147,   112,   173,   147,   125,    11,    90,   410,   147,   152,
     165,   154,   155,   410,   157,   158,   171,   147,    89,   173,
     147,   165,    93,   171,   171,   109,   236,   147,    89,   638,
     151,   949,    93,    89,   173,   165,   588,    93,   165,   442,
     443,   444,   445,   446,   447,   165,   449,   450,   451,   452,
     453,   147,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   621,
     280,  1047,  1068,   147,   952,   478,   479,   173,  1102,  1103,
     147,   484,    55,   165,   487,   488,   489,   490,   491,   492,
     172,    55,   174,   147,   304,   147,   499,   147,   165,   173,
     503,   147,   478,   479,   507,    55,   509,   147,   484,   910,
     221,   487,   488,   165,   517,   165,   519,   520,   925,   173,
     517,    50,   525,   526,    53,   165,   147,   173,   531,   147,
     147,   574,   147,   574,   344,   538,   574,   141,   541,    27,
     147,   544,   146,    55,    73,   149,   147,    76,     8,   153,
     165,   554,   173,   556,    83,   173,   173,   560,   165,    47,
     563,   564,   165,    51,   165,   619,    89,   172,   171,    27,
      93,   574,   283,    83,    84,    82,   579,   288,    85,    55,
     109,  1177,   172,  1179,   174,   588,    55,    75,  1197,    47,
      78,    55,    80,    51,    55,   172,   599,   174,   100,   602,
      31,   604,    90,   172,    55,   174,   609,   610,   611,   172,
     613,   614,   764,   172,   170,   174,   172,    75,   621,    55,
      78,   109,    80,   172,     3,   174,   602,    55,   604,   632,
     633,     2,    90,   785,   172,   172,   174,   174,   602,   625,
     604,  1203,   645,    55,  1253,  1254,  1255,  1256,  1257,  1258,
    1128,   109,   655,    55,   365,    26,   140,    28,    89,   170,
     305,   306,    93,    55,   146,   165,   172,   164,   478,   479,
     165,   165,   165,   165,   484,   386,   165,   487,   488,   489,
     490,    55,   165,   170,   119,   173,   397,  1249,   165,    84,
     165,   172,  1093,   172,   172,   698,   170,   147,   146,   172,
     172,   172,   705,   173,     8,     3,   165,   172,    79,   165,
     141,    65,    83,    84,   171,   146,   170,   148,   149,   125,
      90,    90,   153,   434,   125,   149,   878,   879,    68,   160,
     172,   125,   173,   147,   544,    47,   107,   168,   172,   125,
     125,   112,   796,   797,    34,    34,   800,   801,    65,   904,
     173,   173,   125,   146,   906,   173,   759,   760,   761,   762,
     763,   764,   765,   766,    21,   175,   175,   149,   105,   147,
     908,   170,   172,   911,   149,   141,   165,   149,   588,   175,
     783,   149,   785,   786,   149,   788,   777,   778,   149,   792,
     149,   794,   602,   149,   604,   172,   149,   949,   149,   149,
     803,   804,   856,   857,   149,   149,   149,   783,   519,   173,
     149,   621,    34,   165,   525,    34,   165,   165,   165,   822,
     146,    32,   147,   165,   170,   536,    32,   538,     2,   165,
     171,    34,   146,   836,   165,   173,   171,   840,   165,   173,
     925,   147,   175,   175,   172,   175,   822,   165,   172,    34,
     221,   147,    26,   175,    28,   165,   165,   173,   173,   164,
     173,   164,   865,   866,   867,   908,   173,   117,   165,   912,
     620,  1047,   279,   876,    25,   397,   876,   880,   921,   882,
     883,   884,   117,   886,  1005,   888,  1149,  1150,   891,   892,
      91,   976,   560,  1030,   574,  1047,   607,   925,   983,   351,
     611,   921,   571,   906,   408,    79,   912,   318,  1249,    83,
      84,  1095,   283,  1259,   625,  1252,   903,   288,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   107,   707,   137,   818,   940,   112,    -1,
     943,   557,    31,  1081,    -1,    -1,   949,    -1,    -1,   952,
      -1,  1089,    -1,    -1,   764,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   169,   966,   967,   172,    -1,   174,    -1,    -1,
      -1,    -1,    -1,   783,   977,   785,   979,    -1,    -1,    -1,
      -1,   984,    -1,  1090,   987,    -1,  1093,    -1,    -1,  1043,
    1044,  1045,  1046,    -1,   365,    -1,   999,  1149,  1150,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    35,    36,    -1,
      -1,    -1,   822,  1016,    -1,   386,    -1,  1020,    -1,    -1,
      -1,    -1,    -1,  1026,    -1,    -1,   397,    -1,    -1,  1026,
      -1,    -1,  1035,    -1,  1037,    -1,    -1,  1040,  1081,    -1,
      -1,    -1,    -1,    -1,  1047,    -1,    -1,   221,  1186,    -1,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   774,   153,  1068,    -1,    -1,    -1,    -1,
     159,   160,    -1,  1076,   163,  1078,    -1,   166,   167,   168,
      -1,   109,    -1,    -1,  1191,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,  1097,   806,   906,    -1,   809,    -1,
      -1,    -1,    -1,    -1,  1107,  1108,    -1,  1110,  1111,   283,
    1113,  1114,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,
    1123,  1124,    -1,  1126,    -1,  1128,    -1,  1130,  1131,    -1,
      -1,  1134,    -1,  1218,  1219,    -1,    -1,  1222,  1223,   949,
      -1,    -1,    -1,  1186,   855,    -1,  1149,  1150,   519,  1234,
     861,  1236,    -1,    -1,   525,    -1,    -1,   868,  1161,    -1,
    1163,    -1,    -1,    -1,  1167,   536,    -1,   538,    -1,    -1,
      -1,    -1,    -1,  1176,  1177,  1178,  1179,  1180,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   365,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     2,    -1,  1207,  1208,  1209,  1210,  1211,  1212,
    1213,  1214,   386,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   397,    -1,    -1,    26,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   607,  1047,    12,    -1,
     611,    -1,  1245,    -1,    -1,    19,    -1,    -1,  1251,    -1,
      -1,    -1,    -1,    -1,   625,   966,   967,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   977,    -1,   979,  1272,
    1273,  1274,  1275,   984,  1260,    -1,   987,  1280,  1281,    79,
      -1,    -1,    -1,    83,    84,   996,    -1,   998,    -1,    -1,
    1001,    -1,    -1,    -1,    -1,  1298,  1299,  1300,  1301,  1302,
    1303,    -1,    -1,    -1,    -1,    -1,    -1,   107,    -1,    -1,
      -1,    -1,   112,    -1,  1025,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,  1035,    -1,  1037,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1149,
    1150,    -1,    -1,    -1,    -1,   519,   120,    -1,    -1,    -1,
     124,   525,    -1,    -1,    -1,    -1,   384,    -1,    -1,   387,
      -1,    -1,   536,    -1,   538,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   166,   167,   168,    -1,   170,    -1,    -1,     2,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,  1123,    -1,    -1,    -1,    -1,   445,   446,  1130,
      -1,   221,    -1,    26,     2,    28,    -1,    -1,    -1,    -1,
      -1,    -1,  1143,   607,    -1,   806,    -1,   611,   809,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1157,    -1,    26,    -1,
      28,   625,    -1,    -1,    -1,  1166,    31,    -1,    -1,  1170,
      -1,  1172,  1173,   491,   492,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   503,    79,    -1,    -1,    -1,
      83,    84,    -1,   283,   855,    31,    -1,    -1,   288,    -1,
     861,    -1,   520,    -1,    -1,    -1,    -1,   868,   526,    -1,
      -1,    79,   124,   531,   107,    83,    84,    -1,    -1,   112,
      -1,    -1,    -1,  1224,    89,    -1,    -1,    -1,    93,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,   107,
     152,   153,   154,   155,   112,   157,   158,   159,   160,   161,
     162,   163,    -1,    89,   166,   167,   168,    93,    -1,  1260,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,  1272,  1273,  1274,  1275,   365,   141,   142,    -1,  1280,
    1281,   146,    -1,   148,   149,    -1,    -1,    -1,   153,    -1,
      -1,   609,    -1,    -1,    -1,   160,   386,    -1,    -1,    -1,
      -1,   166,   167,   168,    -1,   141,   142,   397,   144,   145,
     146,    -1,   148,   149,    -1,    -1,   977,   153,   979,    -1,
      -1,    -1,    -1,   984,   160,    -1,   987,    -1,   221,    -1,
     166,   167,   168,    -1,    -1,   996,    -1,   998,    -1,    -1,
    1001,    -1,   806,    -1,     1,   809,    -1,    -1,    -1,    31,
      -1,    -1,    -1,   221,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1025,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    -1,  1035,    -1,  1037,    -1,    35,    -1,
      37,    38,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
     283,   855,    -1,    50,    -1,   288,    53,   861,    -1,    -1,
      -1,    -1,    -1,    -1,   868,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,   283,    73,    -1,    -1,    76,
     288,    78,    -1,    80,    -1,    82,    83,    -1,    85,   519,
      -1,    88,    -1,    90,    -1,   525,    -1,    -1,    -1,    -1,
      31,    -1,   760,    -1,   762,    -1,   536,   765,   538,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,   141,
     142,    -1,  1123,   145,   146,    -1,   148,   149,    -1,  1130,
      -1,   153,   365,    -1,    -1,    -1,    -1,    -1,   160,    -1,
      -1,    -1,  1143,   140,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    -1,   386,    -1,    -1,  1157,   365,    89,    -1,
      -1,    -1,    93,    -1,   397,  1166,    -1,    -1,   165,  1170,
      -1,  1172,  1173,   977,   171,   979,    -1,   607,   386,    -1,
     984,   611,    -1,   987,    -1,    -1,    -1,    -1,    -1,   397,
      -1,    -1,   996,    -1,   998,   625,    -1,  1001,    22,    -1,
      -1,    -1,    26,    27,    -1,    -1,    -1,   865,   866,   867,
     141,    35,    -1,    37,    38,   146,    -1,   148,   149,    43,
      -1,  1025,   153,  1224,    -1,    -1,    50,    -1,    -1,   160,
      -1,  1035,    -1,  1037,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    80,    -1,    82,  1260,
      -1,    85,    -1,    -1,    88,    -1,    90,    -1,    -1,    -1,
      -1,  1272,  1273,  1274,  1275,    -1,    -1,    -1,     2,  1280,
    1281,    -1,    -1,    -1,    -1,   109,   519,    -1,   112,    -1,
      -1,   115,   525,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    26,   536,    28,   538,    -1,    -1,   966,   967,
      -1,   519,    -1,    -1,    -1,    -1,   140,   525,    -1,  1123,
      -1,    -1,    -1,     2,    -1,    -1,  1130,    -1,   536,    -1,
     538,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1143,
      -1,   165,    -1,    -1,    -1,    -1,    -1,    26,    -1,    28,
      -1,    -1,    -1,  1157,    -1,    79,    -1,    -1,    -1,    83,
      84,    -1,  1166,    -1,    -1,    -1,  1170,    -1,  1172,  1173,
      -1,    -1,    -1,    -1,   607,    -1,   806,    -1,   611,   809,
      -1,    -1,    -1,   107,    -1,    -1,    -1,    -1,   112,    -1,
      -1,    -1,   625,    -1,    -1,    -1,    -1,    -1,    -1,   607,
      79,    -1,    -1,   611,    83,    84,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,    -1,   625,    -1,    -1,
    1224,    -1,    -1,    -1,    -1,   855,    -1,    -1,   107,    17,
      18,   861,    20,   112,    -1,    -1,    -1,    25,   868,    22,
      -1,    -1,    -1,    26,    27,    33,    -1,    -1,    -1,    -1,
      -1,    39,    35,    -1,    37,    38,  1260,    45,    -1,    -1,
      43,    49,    -1,    -1,    52,    -1,    -1,    50,  1272,  1273,
    1274,  1275,    -1,    -1,    62,    -1,  1280,  1281,     2,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    76,    -1,    78,    -1,   221,    -1,    87,
      -1,    -1,    26,    -1,    28,    88,    -1,    90,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,   111,    -1,   113,   109,    -1,   116,   112,
      -1,    -1,   115,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   221,    -1,    -1,    -1,    -1,   977,    -1,   979,
      -1,    -1,    -1,    -1,   984,    79,    -1,   987,    -1,   283,
      -1,    -1,    -1,    -1,   288,    -1,   996,    -1,   998,    -1,
      -1,  1001,     3,   806,    -1,    -1,   809,    -1,    -1,    -1,
      -1,    -1,    -1,   107,   172,    -1,    17,    18,   112,    20,
      -1,    -1,    -1,    -1,    25,  1025,    -1,    -1,   806,    -1,
      -1,   809,    33,    -1,   283,  1035,    -1,  1037,    39,   288,
       3,    -1,    -1,    -1,    45,    -1,    -1,    -1,    49,    -1,
      -1,    52,   855,    -1,    17,    -1,    -1,    20,   861,    -1,
      -1,    62,    25,    -1,    -1,   868,    -1,    -1,    69,    70,
      -1,   365,    35,    -1,    -1,    -1,    77,   855,    -1,    -1,
      -1,    -1,    45,   861,    -1,    -1,    87,    -1,    -1,    52,
     868,    -1,   386,    -1,    -1,    96,    97,    -1,    99,    62,
     101,    -1,   103,   397,    -1,   106,    -1,    70,    -1,   110,
     111,    -1,   113,    -1,    -1,   116,   365,    -1,    -1,    -1,
      -1,    -1,    -1,  1123,    87,    -1,    -1,   221,    -1,    -1,
    1130,    -1,    -1,    -1,    -1,    -1,    99,   386,    -1,    -1,
     103,    -1,    -1,  1143,    -1,    -1,    -1,   110,   397,    -1,
      -1,    -1,    -1,   116,    -1,    -1,    -1,  1157,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1166,    -1,    -1,    -1,
    1170,   172,  1172,  1173,   977,    -1,   979,    -1,    -1,    -1,
      -1,   984,    -1,    -1,   987,    -1,    -1,    31,    -1,   283,
      -1,    -1,    -1,   996,   288,   998,    -1,    -1,  1001,   977,
      -1,   979,    -1,    -1,    -1,    -1,   984,    -1,    -1,   987,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   996,    -1,
     998,    -1,  1025,  1001,  1224,   519,    -1,    -1,    -1,    -1,
      -1,   525,  1035,    -1,  1037,    -1,    -1,     2,    -1,    -1,
      -1,    -1,   536,    -1,   538,    89,    -1,  1025,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1035,    -1,  1037,
    1260,    26,    -1,    28,    -1,    -1,    -1,    -1,    -1,    -1,
     519,   365,  1272,  1273,  1274,  1275,   525,    -1,    -1,    -1,
    1280,  1281,    -1,    -1,    -1,    -1,    -1,   536,    -1,   538,
      -1,    -1,   386,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,   397,   148,   149,    -1,    -1,    -1,   153,
      -1,    -1,    -1,   607,    79,   159,   160,   611,    -1,   163,
    1123,    -1,   166,   167,   168,    -1,    -1,  1130,    -1,    -1,
      -1,   625,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1143,    -1,   107,    -1,    -1,  1123,    -1,   112,    -1,    -1,
      -1,    -1,  1130,    -1,  1157,    -1,    -1,    -1,   607,    -1,
      -1,    -1,   611,  1166,    -1,  1143,    49,  1170,    -1,  1172,
    1173,    -1,    -1,    -1,    -1,    -1,   625,    -1,    -1,  1157,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1166,    -1,
      73,    -1,  1170,    -1,  1172,  1173,    -1,    -1,    -1,    -1,
      83,    84,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1224,    -1,    -1,    -1,   519,    -1,    31,    -1,    -1,
      -1,   525,    -1,    -1,   117,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   536,    -1,   538,    -1,  1224,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   221,  1260,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1272,
    1273,  1274,  1275,    -1,    -1,    -1,    -1,  1280,  1281,    -1,
      -1,    -1,  1260,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,  1272,  1273,  1274,  1275,    -1,    -1,
      -1,    -1,  1280,  1281,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   806,   607,    -1,   809,    -1,   611,   283,    -1,
      -1,    -1,    -1,   288,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   625,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,   226,   148,   149,   150,    -1,    -1,   153,
     154,   155,    -1,   157,   158,   159,   160,   806,    -1,   163,
     809,   855,   166,   167,   168,    -1,    -1,   861,    -1,    -1,
      -1,    -1,    -1,   177,   868,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   267,   268,   269,   270,   271,   272,
      -1,    -1,   275,   276,    -1,    -1,    -1,    -1,    -1,   282,
     365,    -1,    -1,    -1,    -1,    -1,   855,    -1,   291,    -1,
     293,    -1,   861,    -1,    -1,    -1,    -1,   300,    -1,   868,
      -1,   386,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   397,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   340,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    12,
     353,   354,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,   977,    -1,   979,    -1,    -1,    31,    32,
     984,    -1,    -1,   987,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   996,    -1,   998,    -1,    -1,  1001,    -1,    -1,
      -1,    -1,   806,    -1,    -1,   809,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   977,    -1,
     979,  1025,    -1,    -1,    -1,   984,    -1,    -1,   987,    -1,
      -1,  1035,    -1,  1037,    -1,    -1,    89,   996,    -1,   998,
      93,    49,  1001,    -1,   519,    -1,    -1,    -1,    -1,    -1,
     525,   855,    -1,    -1,    -1,    -1,    -1,   861,    -1,    -1,
      -1,   536,    -1,    -1,   868,    73,  1025,   120,    -1,    -1,
      -1,   124,    -1,    -1,    -1,    -1,  1035,    -1,  1037,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   504,    -1,   166,   167,   168,    -1,   170,    -1,  1123,
      -1,    -1,    -1,    -1,   177,    -1,  1130,    -1,    -1,    -1,
      -1,   524,   607,    -1,    -1,   528,   611,   530,    -1,  1143,
     533,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     625,    -1,    -1,  1157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1166,    -1,  1123,    -1,  1170,    -1,  1172,  1173,
      -1,  1130,    -1,   977,    -1,   979,    -1,    -1,    -1,    -1,
     984,    -1,    -1,   987,  1143,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   996,    -1,   998,    -1,    -1,  1001,  1157,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1166,    -1,    -1,
      -1,  1170,    -1,  1172,  1173,    -1,    -1,    -1,    -1,    -1,
    1224,  1025,    -1,    -1,   617,   618,    -1,    -1,    -1,    -1,
      -1,  1035,    -1,  1037,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1260,    -1,    -1,   267,
      -1,   269,   270,   271,   272,  1224,    -1,    -1,  1272,  1273,
    1274,  1275,    -1,    -1,    -1,    30,  1280,  1281,    -1,    -1,
      -1,    -1,    -1,   291,    -1,   293,    -1,    -1,    -1,    44,
      45,    46,   300,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      19,  1260,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    31,  1272,  1273,  1274,  1275,    -1,    -1,  1123,
      -1,  1280,  1281,    -1,    -1,    -1,  1130,    -1,    47,    -1,
      -1,   806,   340,    -1,   809,    -1,    -1,    -1,    -1,  1143,
      -1,    -1,    -1,    -1,    -1,   353,   354,    -1,    -1,    -1,
      -1,    -1,    -1,  1157,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1166,    -1,    -1,    -1,  1170,    -1,  1172,  1173,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
     855,    -1,    -1,    -1,    -1,    -1,   861,    -1,    -1,    -1,
      -1,    -1,    -1,   868,    -1,    -1,    -1,   790,   791,    -1,
     155,   120,   157,   796,   797,   124,    -1,   800,   801,    -1,
      -1,    -1,    -1,    -1,   169,    -1,    -1,   810,    -1,    -1,
    1224,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     205,    -1,    -1,   846,    -1,    -1,  1260,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1272,  1273,
    1274,  1275,    -1,    -1,    -1,    -1,  1280,  1281,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   504,    -1,    -1,    -1,
      -1,    -1,   977,    -1,   979,    -1,    -1,    -1,    -1,   984,
     265,    -1,   987,    -1,    -1,    -1,   524,    -1,   273,   274,
     528,   996,   530,   998,    -1,   533,  1001,    -1,    -1,   284,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    22,    -1,    -1,    -1,    26,    27,    -1,
    1025,    -1,    -1,    -1,    -1,    -1,    35,    -1,    37,    38,
    1035,    -1,  1037,    -1,    43,    -1,    -1,    -1,    -1,   324,
      -1,    50,    -1,    -1,    -1,   968,   969,    -1,    -1,    -1,
      -1,   974,    -1,   976,    -1,    -1,    -1,    -1,    -1,    -1,
     983,    -1,    -1,   986,    -1,    -1,   989,    76,   991,    78,
     993,    80,    -1,    82,    -1,    -1,    85,    -1,    -1,    88,
     618,    90,    -1,  1006,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   381,    -1,    -1,    -1,
     109,    -1,    -1,   112,    -1,    -1,   115,    -1,    -1,    -1,
    1033,     3,    -1,    -1,    -1,    -1,    -1,    -1,  1041,  1042,
    1043,  1044,    -1,    -1,   409,    17,    18,    -1,    20,   414,
      -1,   140,    -1,    25,    -1,    -1,    -1,    -1,  1143,    -1,
      -1,    33,    -1,    -1,    -1,    -1,    -1,    39,    -1,   434,
      -1,    -1,  1157,    45,    -1,    -1,   165,    49,    -1,    -1,
      52,  1166,   171,    -1,    -1,  1170,    -1,  1172,  1173,    -1,
      62,    -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,
      -1,   103,    -1,   498,   106,    -1,    -1,    -1,   110,  1224,
      -1,   113,    -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1159,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   790,   791,    -1,  1260,    -1,    -1,   796,   797,
     545,    -1,   800,   801,   549,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   562,   170,    -1,
     172,    -1,    -1,    -1,    -1,    -1,   571,    -1,    -1,    -1,
      -1,   576,    -1,    -1,    -1,  1218,  1219,    -1,    -1,  1222,
    1223,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1232,
      -1,  1234,    -1,  1236,    -1,    -1,    -1,    -1,    -1,     4,
       5,     6,     7,     8,     9,    10,    -1,    -1,    -1,    14,
      -1,    -1,    -1,    18,    -1,    -1,    -1,    22,    -1,    -1,
      -1,    -1,    27,    -1,    -1,    -1,   631,    -1,    -1,    -1,
      35,    -1,    -1,    -1,    -1,    40,    41,    -1,    -1,    44,
      -1,    -1,    47,    -1,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    64,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    78,    79,    80,    81,    -1,    -1,    -1,
      -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,   707,    -1,   109,    -1,    -1,    -1,   113,    -1,
     968,   969,    -1,    12,    -1,    -1,   974,    -1,    -1,   124,
      19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   986,    -1,
      -1,   989,    31,   991,    -1,   993,   141,   142,   143,   144,
     145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,   164,
      -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,   174,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1041,  1042,  1043,  1044,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    -1,    -1,   811,    -1,    -1,    -1,
      -1,    -1,    -1,   818,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   828,    -1,   124,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,   846,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   903,    -1,
      -1,  1159,    -1,    -1,    -1,   910,    -1,     0,     1,    -1,
       3,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
     925,    14,    15,    16,    17,    18,    -1,    20,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    32,
      33,    -1,    35,    -1,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    -1,    48,    49,    50,    -1,    52,
      53,    -1,    55,    56,    57,    58,    59,    -1,    61,    62,
      63,    64,    65,    66,    67,    -1,    69,    70,    71,    -1,
      73,    -1,    -1,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    90,    91,    92,
    1005,    94,    95,    96,    97,    98,    99,   100,   101,    -1,
     103,   104,    -1,   106,   107,   108,   109,   110,    -1,   112,
     113,   114,   115,   116,    -1,  1030,   119,    -1,   121,    -1,
      -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,
     143,   144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,
      -1,   174,    -1,    -1,   177,    -1,    -1,     1,  1093,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    15,    16,    17,    18,    19,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,   171,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    16,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    -1,    32,    33,    -1,    35,    -1,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    -1,    48,
      49,    50,    -1,    52,    53,    -1,    55,    56,    57,    58,
      59,    -1,    61,    62,    63,    64,    65,    66,    67,    -1,
      69,    70,    71,    -1,    73,    -1,    -1,    76,    77,    78,
      79,    -1,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    90,    91,    92,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,   116,    -1,    -1,
     119,    -1,   121,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   140,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,     1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    15,    16,    17,    18,    -1,    20,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    -1,    32,    33,
      -1,    35,    -1,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    -1,    48,    49,    50,    -1,    52,    53,
      -1,    55,    56,    57,    58,    59,    -1,    61,    62,    63,
      64,    65,    66,    67,    -1,    69,    70,    71,    -1,    73,
      -1,    -1,    76,    77,    78,    79,    -1,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    90,    91,    92,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,    -1,   103,
     104,    -1,   106,   107,   108,   109,   110,    -1,   112,   113,
     114,   115,   116,    -1,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   140,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,   165,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,     1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    12,    -1,
      14,    -1,    -1,    17,    18,    19,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      54,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,    -1,    -1,   133,
     134,   135,   136,    -1,   138,   139,    -1,   141,   142,   143,
     144,   145,   146,    -1,   148,    -1,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,    -1,   166,   167,   168,   169,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    15,    -1,    17,    18,
      -1,    20,    -1,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    30,    -1,    32,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    42,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    61,    62,    63,    64,    -1,    66,    67,    -1,
      69,    70,    71,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,   103,   104,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
     119,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    78,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
      -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,   151,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    47,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    90,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,   115,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
      -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,   173,   174,    -1,    -1,   177,     3,
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
      -1,    -1,   106,   107,   108,    -1,   110,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,   165,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
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
      -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,   173,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,   107,   108,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    78,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,   122,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    27,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,   115,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
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
      -1,    -1,   106,   107,   108,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,   175,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    89,    -1,    -1,    -1,    93,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,   122,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,    -1,    22,    -1,
      -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    32,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
      -1,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,   112,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
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
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,   175,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,   171,   172,    -1,   174,    -1,    -1,   177,     3,
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
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,   175,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,   175,    -1,   177,     3,
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
      -1,    -1,   106,    -1,    -1,    -1,   110,   111,    -1,   113,
      -1,    -1,   116,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,   111,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
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
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
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
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
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
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
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
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     3,
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
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,   143,
     144,   145,    -1,    -1,    -1,    -1,   150,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   159,    -1,    -1,   162,   163,
     164,    -1,    -1,    -1,   168,    -1,   170,    -1,   172,    -1,
     174,    -1,    -1,   177,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    31,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,   150,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,   162,   163,   164,    -1,    -1,    -1,   168,
     124,   170,    -1,   172,    -1,   174,    -1,    -1,   177,     4,
       5,     6,     7,     8,     9,    10,    -1,   141,   142,    14,
     144,   145,   146,    18,   148,   149,   150,    22,   152,   153,
     154,   155,    27,   157,   158,   159,   160,   161,    -1,   163,
      35,    -1,   166,   167,   168,    40,    41,    -1,    -1,    44,
      -1,    -1,    47,   177,    -1,    -1,    51,    -1,    -1,    -1,
      55,    -1,    -1,    58,    59,    -1,    -1,    -1,    -1,    64,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    77,    78,    79,    80,    81,    -1,    -1,    -1,
      -1,    86,    -1,    88,    -1,    90,    -1,    -1,    -1,    -1,
      -1,    96,    -1,    98,    -1,   100,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,   113,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     1,    -1,     3,    -1,   141,   142,   143,   144,
     145,    -1,    -1,    12,    -1,   150,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,   159,    -1,    25,   162,   163,   164,
      -1,    -1,    -1,   168,    33,   170,    35,   172,    -1,   174,
      39,    -1,   177,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    -1,
      99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,    -1,   148,
      -1,    -1,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,   163,    -1,    -1,   166,   167,   168,
     169,    -1,    -1,   172,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,
      -1,    20,    -1,    22,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    44,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    -1,    55,    -1,    -1,    58,
      59,    -1,    -1,    62,    -1,    64,    -1,    66,    67,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      79,    -1,    81,    -1,    -1,    -1,    -1,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,    98,
      99,   100,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   141,   142,   143,   144,   145,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     159,    -1,    -1,    -1,   163,   164,    -1,    -1,    -1,   168,
      -1,   170,    -1,   172,    -1,   174,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    -1,    -1,    14,    -1,    -1,
      17,    18,    -1,    20,    -1,    22,    -1,    -1,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    55,    -1,
      -1,    58,    59,    -1,    -1,    62,    -1,    64,    -1,    66,
      67,    -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    79,    -1,    81,    -1,    -1,    -1,    -1,    86,
      87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,
      97,    98,    99,   100,   101,    -1,   103,    -1,    -1,   106,
      -1,    -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,   143,    -1,    -1,     3,
       4,     5,     6,     7,     8,     9,    10,    -1,    -1,    -1,
      14,    -1,    -1,    17,    18,    -1,    20,   164,    22,    -1,
      -1,    25,    -1,   170,    -1,   172,    -1,   174,    -1,    33,
      -1,    35,    -1,    -1,    -1,    39,    40,    41,    -1,    -1,
      44,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,    -1,
      -1,    55,    -1,    -1,    58,    59,    -1,    -1,    62,    -1,
      64,    -1,    66,    67,    -1,    69,    70,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    -1,    79,    -1,    81,    -1,    -1,
      -1,    -1,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    96,    97,    98,    99,   100,   101,    -1,   103,
       3,    -1,   106,    -1,    -1,    -1,   110,    -1,    -1,   113,
      -1,    -1,   116,    -1,    17,    18,    -1,    20,    -1,    -1,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    -1,    -1,    -1,    -1,    39,   141,    -1,   143,
      -1,    -1,    45,    -1,    -1,    -1,    49,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
     164,    -1,    -1,    -1,    -1,    -1,    69,    70,   172,    -1,
     174,    -1,    -1,    -1,    77,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    96,    97,    -1,    99,    -1,   101,    -1,
     103,    -1,    -1,   106,    -1,     3,    -1,   110,    -1,    -1,
     113,    -1,    -1,   116,    12,    -1,    -1,    -1,    -1,    17,
      18,    19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,   172,
      -1,    -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,    -1,    -1,
      -1,   109,   110,    -1,    -1,   113,    -1,    -1,   116,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,   143,   144,   145,   146,    -1,
     148,    -1,    -1,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,     3,   163,    -1,    -1,   166,   167,
     168,   169,   170,    12,    -1,    -1,    -1,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,    -1,
      49,    -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,    -1,
      69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    96,    97,    -1,
      99,    19,   101,    -1,   103,    -1,    -1,   106,    -1,    -1,
      -1,   110,    -1,    31,   113,    -1,    -1,   116,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   125,   126,   127,   128,
     129,   130,    -1,    -1,   133,   134,   135,   136,    -1,   138,
     139,    -1,   141,   142,   143,   144,   145,   146,    -1,   148,
      -1,    -1,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,   163,    -1,    -1,   166,   167,   168,
     169,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    12,    93,    -1,    -1,    -1,   177,
      -1,    19,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    -1,    -1,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    89,   170,    -1,    -1,    93,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,   102,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,    31,    32,   133,   134,   135,   136,    -1,
     138,   139,    -1,   141,   142,    -1,   144,   145,   146,    47,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,   170,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      12,    89,    -1,    -1,    -1,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   120,    -1,    -1,    47,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,   170,    19,    -1,    -1,    -1,    -1,    -1,   177,
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
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    89,
     166,   167,   168,    93,   170,    -1,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,   124,    -1,    -1,    -1,    36,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    72,    -1,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    89,    -1,    -1,    12,    93,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,   165,   166,   167,
     168,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
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
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    32,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    47,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
     170,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    12,    89,   166,   167,
     168,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,   170,    19,
      -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,   170,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     120,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    89,   166,   167,   168,    93,
      -1,    -1,    -1,    -1,    -1,   175,    -1,   177,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   120,    -1,    -1,    31,
     124,    -1,    -1,    -1,    36,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      72,    31,   166,   167,   168,    -1,    -1,    -1,    -1,    -1,
      -1,   175,    -1,   177,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,    -1,    -1,    89,
      -1,    -1,    12,    93,    -1,   177,    -1,    -1,    -1,    19,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    31,   152,   153,   154,   155,    -1,   157,   158,   159,
     160,   161,    -1,   163,   124,    -1,   166,   167,   168,    -1,
      -1,    -1,    12,    -1,    -1,    -1,    -1,   177,    -1,    19,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    31,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,    -1,   166,   167,   168,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,   177,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,
      -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   124,    -1,   166,   167,   168,    -1,
     170,    -1,    -1,    -1,    -1,    89,    -1,   177,    -1,    93,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,   165,   166,   167,   168,   123,
     124,    19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,
      12,    13,    -1,    -1,   102,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,   124,    -1,    -1,    -1,    12,    89,    -1,   177,
      -1,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    31,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   120,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    -1,    89,
     166,   167,   168,    93,    -1,    12,    -1,    -1,    -1,    -1,
      -1,   177,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   165,   166,   167,   168,    -1,
      -1,    12,    89,    -1,    -1,    -1,    93,   177,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    89,   166,
     167,   168,    93,    -1,    -1,    -1,    -1,    12,    -1,    -1,
     177,    -1,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,   173,    -1,    89,    -1,   177,    -1,    93,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    13,   166,   167,   168,   123,   124,
      19,    -1,   173,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,    -1,   141,   142,    -1,   144,
     145,   146,    -1,   148,   149,   150,    -1,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,    12,
      -1,   166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    12,
      -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    31,    32,
      -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,
      93,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    -1,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
      -1,   124,    -1,    -1,    -1,    12,    89,    -1,   177,    -1,
      93,    -1,    19,    -1,    -1,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    31,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,    -1,
     173,    -1,    -1,    -1,   177,    -1,    -1,    -1,   141,   142,
      -1,   144,   145,   146,    -1,   148,   149,   150,    -1,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,    12,    89,   166,   167,   168,    93,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,
      -1,    12,    13,    -1,    -1,    -1,    -1,    -1,    19,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      31,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    89,   166,
     167,   168,    93,    -1,    -1,    -1,    12,    -1,   175,    -1,
     177,    -1,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     141,   142,    -1,   144,   145,   146,    -1,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   124,    -1,   166,   167,   168,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,   177,    93,    -1,    -1,
     141,   142,    -1,   144,   145,   146,   102,   148,   149,   150,
      -1,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,    12,    -1,   166,   167,   168,   124,    -1,
      19,    -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,
      -1,    -1,    31,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    13,
     166,   167,   168,    -1,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,    -1,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   102,    12,    -1,    -1,    -1,    -1,    -1,
      -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    -1,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,   148,
     149,   150,    60,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,    -1,    -1,   166,   167,   168,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,    -1,
      -1,    89,    -1,    -1,    -1,    93,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    -1,   166,   167,   168,    -1,   124,    19,    -1,    -1,
      -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    -1,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    -1,   166,   167,
     168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   177,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    12,    89,    -1,    -1,
      -1,    93,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,
     102,    -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    12,    89,   166,   167,   168,    93,    -1,    19,
      -1,    -1,    -1,    -1,    -1,   177,   102,    -1,    -1,    -1,
      -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,
     146,    -1,   148,   149,   150,    -1,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,    12,    89,
     166,   167,   168,    93,    -1,    19,    -1,    -1,    -1,    -1,
      -1,   177,   102,    -1,    -1,    -1,    -1,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   141,   142,    -1,   144,   145,   146,    -1,   148,   149,
     150,    -1,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    12,    89,   166,   167,   168,    93,
      -1,    19,    -1,    -1,    -1,    -1,    -1,   177,   102,    -1,
      -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     124,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,   142,    -1,
     144,   145,   146,    -1,   148,   149,   150,    -1,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      12,    89,   166,   167,   168,    93,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   177,   102,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   124,    -1,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    19,    -1,    -1,
      -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,    31,
     148,   149,   150,    -1,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,    -1,    89,   166,   167,
     168,    93,    -1,    -1,    -1,    12,    -1,    -1,    -1,   177,
     102,    -1,    19,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   124,    -1,    -1,    -1,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   124,    -1,   166,   167,   168,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    -1,   177,    93,    -1,    -1,   141,
     142,    -1,   144,   145,   146,    -1,   148,   149,   150,    -1,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,    -1,    -1,   166,   167,   168,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   177,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,   142,    -1,   144,   145,   146,
      -1,   148,   149,   150,    -1,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,    -1,    -1,   166,
     167,   168,    22,    -1,    24,    -1,    26,    27,    -1,    -1,
     177,    -1,    32,    -1,    -1,    35,    -1,    37,    38,    -1,
      40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,
      -1,    -1,    82,    -1,    84,    85,    -1,    -1,    88,    22,
      90,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    35,    -1,    37,    38,    -1,    -1,    -1,   109,
      43,    -1,   112,    -1,    -1,   115,    -1,    50,    -1,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    22,    -1,    -1,    -1,    26,
      27,    -1,    -1,    76,    -1,    78,    -1,    80,    35,    82,
      37,    38,    85,    -1,    -1,    88,    43,    90,    -1,    -1,
      -1,    -1,    -1,    50,    -1,   165,    -1,    -1,    -1,    -1,
     170,    -1,    -1,    -1,   174,    -1,   109,    -1,    -1,   112,
      -1,    22,   115,    -1,    -1,    26,    27,    -1,    -1,    76,
      -1,    78,    -1,    80,    35,    82,    37,    38,    85,    -1,
      -1,    88,    43,    90,    -1,    -1,    -1,   140,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   109,    -1,    -1,   112,    -1,    -1,   115,    -1,
      -1,    -1,   165,    -1,    -1,    76,    -1,    78,   171,    -1,
      -1,    82,    -1,     1,    85,     3,    -1,    88,    -1,    90,
      -1,    -1,    -1,   140,    -1,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    -1,    -1,    -1,    -1,    25,   109,    -1,
      -1,   112,    -1,    -1,   115,    33,    -1,    -1,   165,    -1,
      -1,    39,    -1,    -1,   171,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,   140,
       3,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    17,    18,    -1,    20,    -1,    77,
      -1,    -1,    25,    -1,   165,    -1,    -1,    -1,    -1,    87,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    96,    97,
      -1,    99,    45,   101,    -1,   103,    49,    -1,   106,    52,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,     3,    -1,    -1,    -1,    -1,
      -1,    -1,   140,    -1,    87,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    96,    97,    -1,    99,    25,   101,    27,
     103,    -1,    -1,   106,    -1,    33,    -1,   110,    -1,    -1,
     113,    39,    -1,   116,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    62,    -1,    -1,   140,    -1,    -1,
      -1,    69,    70,    -1,    17,    18,    -1,    20,    -1,    77,
      78,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      33,    -1,    90,    -1,    -1,    -1,    39,    -1,    96,    97,
      -1,    99,    45,   101,    -1,   103,    49,    -1,   106,    52,
      -1,    -1,   110,    -1,    -1,   113,    -1,   115,   116,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    96,    97,    -1,    99,    25,   101,    -1,
     103,    -1,    -1,   106,    -1,    33,    -1,   110,   111,    -1,
     113,    39,    -1,   116,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
       3,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    17,    18,    -1,    20,    -1,    77,
      -1,    -1,    25,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      33,    -1,    -1,    -1,    -1,    -1,    39,    -1,    96,    97,
      -1,    99,    45,   101,    -1,   103,    49,    -1,   106,    52,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116,    62,
      -1,    -1,    -1,    -1,    -1,    -1,    69,    70,    -1,    -1,
      -1,    -1,    -1,    -1,    77,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    87,    -1,    -1,    -1,    -1,    17,
      18,    -1,    20,    96,    97,    -1,    99,    25,   101,    -1,
     103,    -1,    -1,   106,    -1,    33,    -1,   110,    -1,    -1,
     113,    39,    -1,   116,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    -1,    -1,    -1,    -1,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    87,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    96,    97,
      -1,    99,    -1,   101,    -1,   103,    -1,    -1,   106,    -1,
      -1,    -1,   110,    -1,    -1,   113,    -1,    -1,   116
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int16 yystos[] =
{
       0,   182,   183,     0,     1,     3,     4,     5,     6,     7,
       8,     9,    10,    14,    15,    16,    17,    18,    20,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    32,    33,
      35,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      48,    49,    50,    52,    55,    56,    57,    58,    59,    61,
      62,    63,    64,    66,    67,    69,    70,    71,    76,    77,
      78,    79,    80,    81,    82,    85,    86,    87,    88,    90,
      91,    92,    94,    95,    96,    97,    98,    99,   100,   101,
     103,   104,   106,   107,   108,   109,   110,   112,   113,   115,
     116,   119,   121,   124,   140,   141,   142,   143,   144,   145,
     150,   159,   162,   163,   164,   165,   168,   170,   172,   174,
     177,   184,   185,   186,   187,   188,   189,   191,   192,   193,
     194,   199,   200,   201,   203,   204,   205,   210,   211,   215,
     216,   220,   222,   225,   226,   229,   230,   231,   232,   233,
     234,   237,   238,   240,   242,   245,   246,   247,   248,   249,
     253,   254,   259,   260,   261,   262,   265,   266,   271,   272,
     274,   275,   280,   284,   285,   287,   288,   310,   315,   316,
     320,   321,   342,   343,   344,   345,   346,   347,   348,   356,
     357,   358,   359,   360,   361,   363,   366,   367,   368,   369,
     370,   371,   372,   373,   375,   376,   377,   378,   379,   165,
      22,    35,    40,    41,    44,    55,    88,   101,   170,   174,
     245,   272,   342,   347,   357,   358,   363,   366,   368,   369,
     120,   349,   350,     3,   217,   363,   349,   363,   109,   322,
      90,   217,   186,   336,   363,   186,   172,     3,    17,    18,
      20,    25,    33,    39,    45,    49,    52,    62,    69,    70,
      77,    87,    96,    97,    99,   101,   103,   106,   110,   113,
     116,   218,   219,   221,    11,    78,   122,   239,   363,   239,
     363,   239,   363,    27,   115,   241,   363,    82,    85,   201,
     172,   219,   219,   219,   172,   219,   323,   324,    32,   205,
     224,   363,   257,   258,   363,   363,    18,    77,    96,   113,
     363,   363,   363,     8,   172,   228,   227,    27,    33,    47,
      49,    51,    75,    78,    90,    97,   101,   109,   185,   223,
     276,   277,   278,   301,   302,   303,   328,   333,   363,   336,
     107,   108,   165,   280,   281,   362,   363,   365,   363,   224,
     363,   363,   363,   100,   172,   186,   363,   363,   188,   205,
     188,   205,   363,   241,   363,   365,     3,   363,   363,   363,
     363,   363,     1,   171,   184,   206,   336,   111,   151,   337,
     338,   365,   175,   239,   336,   363,   374,   363,    80,   186,
      22,    35,    38,    40,    41,    88,   170,   174,   189,   190,
     201,   205,   238,   193,     8,   149,   195,   170,    46,    84,
     114,   202,    26,   321,   363,     8,   261,   363,   364,    55,
     146,   263,   219,     1,   192,   219,   267,   270,   172,   296,
      53,    73,    83,   305,    27,    75,    78,    90,   109,   306,
      27,    78,    90,   109,   304,   219,   317,   318,   323,   164,
     165,   363,    12,    19,    31,    89,    93,   124,   141,   142,
     144,   145,   146,   148,   149,   150,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   166,   167,
     168,   177,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   169,   292,   172,
     174,    89,    93,   363,   363,   363,   219,   336,    55,   172,
     186,   165,   205,    47,   349,   320,   165,   147,   165,   119,
     220,   339,   340,   341,   365,   170,   219,   172,   224,    32,
      47,   224,   120,   224,   352,    32,    47,   224,   352,   224,
     352,    47,   224,   352,   219,   219,   102,   205,   102,   125,
     205,   292,   202,   339,   172,   172,   205,   186,   111,   172,
     219,   325,   326,     1,   146,   332,    47,   147,   186,   224,
     147,   224,    13,   172,   172,   224,   339,   229,   229,    47,
      90,   302,   303,   173,   147,   111,   172,   219,   300,   146,
     165,   363,   363,   123,   282,   165,   170,   224,   172,   339,
     165,   250,   250,   224,   224,   165,   171,   171,   184,   147,
     171,   363,   147,   173,   147,   173,   363,   175,   352,    47,
     147,   175,   352,   123,   147,   175,     8,   363,   363,   336,
       3,   172,   196,     1,   171,   206,   213,   214,   363,   208,
     363,    65,    36,    72,   165,   261,   263,   109,   235,   285,
     219,   336,   170,   171,   270,   125,   147,   171,   173,   297,
     299,   301,   306,    90,     1,   146,   330,   331,    90,     1,
       3,    12,    17,    19,    20,    25,    45,    52,    54,    62,
      70,    87,    99,   103,   110,   116,   141,   142,   143,   144,
     145,   146,   148,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   163,   166,   167,   168,   169,   172,   219,
     289,   290,   291,   292,   342,   125,   319,   147,   165,   165,
     363,   363,   363,   239,   363,   239,   363,   363,   363,   363,
     363,   363,   363,     3,    20,    33,    62,   103,   109,   220,
     363,   363,   363,   363,   363,   363,   363,   363,   363,   363,
     363,   363,   363,   363,   363,   363,   363,    68,   365,   365,
     365,   365,   365,   339,   339,   239,   363,   239,   363,    32,
      47,    32,    47,   102,   172,    47,   175,   219,    27,    47,
      90,   115,   351,   354,   355,   363,   379,   239,   363,   224,
     323,   363,   363,   125,   173,   147,    47,   336,    44,   363,
     239,   363,   172,   224,    44,   363,   239,   363,   224,   224,
     239,   363,   224,   125,   125,   186,    34,   186,   363,    34,
     363,    65,   173,   340,   219,   243,   244,   326,   147,   173,
     223,   363,   125,   327,   363,   324,   257,   219,   320,   363,
     363,   173,   300,   306,   278,   326,   146,   334,   335,   333,
     283,   204,     1,   255,   341,   173,    21,   251,   171,   173,
     173,   337,   173,   337,   186,   175,   239,   363,   175,   186,
     363,   175,   363,   175,   363,    47,    47,    47,   175,   195,
     173,   197,   198,   341,   171,   171,   147,   165,    13,   149,
     147,   165,    13,    36,    72,   219,   141,   142,   143,   144,
     145,   159,   163,   168,   207,   291,   292,   293,   363,   207,
     209,   263,   170,   236,   320,   165,   172,     1,   264,   363,
     268,   269,   147,   173,   332,     1,   223,   328,   329,   105,
     307,   172,   295,   363,   141,   149,   295,   295,   363,   318,
     172,   174,   165,   165,   165,   165,   165,   165,   173,   175,
      44,   239,   363,    44,   239,   363,   340,   239,   363,   172,
      47,    90,   147,   173,    17,    20,    25,    45,    52,    62,
      70,    87,    99,   110,   116,   342,    89,    89,   349,   349,
     165,   165,   365,   341,   363,   173,   363,    32,   224,    32,
     224,   353,   354,   363,    32,   224,   352,    32,   224,   352,
     224,   352,   224,   352,   363,   363,    34,   186,    34,    34,
     186,   102,   205,   219,   173,   147,   173,   173,   325,   332,
      68,   365,   219,   173,   173,   335,   146,   279,   173,   333,
     151,   294,   327,   363,   171,    74,   117,   171,   256,   173,
     172,   205,   219,   252,   186,   175,   352,   175,   352,   186,
     123,   239,   363,   239,   363,   239,   363,   147,   173,   213,
     220,    17,    18,    20,    25,    45,    49,    52,    70,    77,
      87,    96,    97,    99,   101,   110,   113,   116,   170,   212,
     293,   363,   111,   363,   207,   209,   147,   165,    13,   165,
     170,   264,   317,   323,   340,   171,    82,    85,   171,   185,
     192,   226,   270,   192,   299,   307,    60,   118,   311,   297,
     298,   173,   290,   292,   173,   175,   363,    32,    32,   363,
      32,    32,   173,   175,   175,   340,   354,   332,   342,   342,
     224,   224,   224,   102,    44,   363,    44,   363,   147,   173,
     102,    44,   363,   224,    44,   363,   224,   224,   224,   186,
     186,   363,   186,    34,   165,   165,   244,   205,   327,   172,
     172,   294,   327,   328,   307,   335,   363,    32,   186,   336,
     252,   146,   205,    44,   186,   363,   175,    44,   186,   363,
     175,   363,   175,   175,   198,   207,    13,    36,    72,    36,
      72,   165,   165,   293,   363,   363,   264,   171,   165,   165,
     173,   192,   226,   226,   270,   311,     3,    92,   103,   312,
     313,   314,   363,   286,   173,   295,   295,   102,    44,    44,
     102,    44,    44,    44,    44,   173,   327,   363,   363,   363,
     354,   363,   363,   363,    34,   186,   339,   339,   294,   327,
     186,   224,   173,   363,   363,   186,   363,   186,   171,   111,
     363,   207,   209,   207,   209,    13,   171,   165,   226,   273,
     314,   118,   147,   125,   152,   154,   155,   157,   158,    60,
      32,   165,   204,   225,   308,   309,   363,   363,   363,   363,
     363,   363,   102,   102,   102,   102,   186,   173,   173,   205,
     102,   102,   165,   165,   165,   165,   363,   309,   363,   313,
     314,   314,   314,   314,   314,   314,   312,   184,   102,   102,
     102,   102,   102,   102,   363,   363,   363,   363,   164,   164,
     363,   363
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int16 yyr1[] =
{
       0,   181,   182,   183,   183,   184,   184,   185,   185,   186,
     186,   187,   187,   187,   187,   187,   187,   187,   187,   187,
     187,   187,   187,   187,   187,   187,   187,   187,   187,   188,
     188,   188,   188,   188,   188,   188,   188,   188,   188,   188,
     188,   188,   188,   188,   188,   189,   189,   190,   190,   190,
     191,   192,   192,   193,   193,   193,   194,   195,   195,   196,
     196,   197,   197,   198,   199,   200,   200,   200,   201,   201,
     201,   202,   202,   203,   204,   204,   204,   205,   206,   206,
     207,   207,   207,   207,   207,   207,   208,   208,   208,   208,
     208,   208,   209,   209,   210,   210,   210,   210,   210,   210,
     210,   211,   212,   212,   212,   212,   212,   212,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   212,   212,   213,
     213,   213,   213,   213,   214,   214,   215,   216,   216,   216,
     216,   216,   216,   217,   217,   218,   218,   218,   218,   218,
     219,   219,   220,   220,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   223,   223,   223,   223,   224,
     224,   225,   225,   226,   226,   227,   226,   228,   226,   229,
     229,   229,   229,   229,   229,   229,   230,   230,   230,   230,
     231,   232,   232,   233,   234,   234,   234,   235,   234,   236,
     234,   237,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   238,   238,   238,   238,   239,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     241,   241,   242,   242,   243,   243,   244,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     246,   246,   246,   246,   246,   246,   247,   247,   247,   248,
     248,   248,   249,   249,   249,   249,   250,   250,   251,   251,
     251,   252,   252,   253,   254,   254,   255,   255,   256,   256,
     256,   257,   257,   257,   258,   258,   259,   260,   260,   261,
     262,   262,   262,   263,   263,   264,   264,   264,   264,   264,
     265,   265,   266,   267,   267,   268,   267,   267,   269,   267,
     270,   270,   271,   273,   272,   274,   275,   275,   275,   276,
     276,   277,   277,   278,   278,   278,   279,   279,   280,   282,
     281,   283,   281,   284,   286,   285,   287,   287,   287,   287,
     287,   288,   289,   289,   290,   290,   290,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   291,   291,   291,   291,   291,   291,   291,   291,
     291,   291,   292,   292,   292,   292,   292,   292,   292,   292,
     292,   292,   292,   292,   293,   293,   294,   294,   295,   295,
     296,   296,   297,   297,   298,   298,   299,   300,   300,   301,
     301,   301,   301,   301,   301,   302,   302,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   304,   304,   304,   304,
     304,   304,   305,   305,   305,   306,   306,   306,   306,   306,
     306,   306,   307,   307,   308,   308,   309,   309,   309,   310,
     311,   311,   311,   311,   311,   312,   312,   313,   313,   313,
     313,   313,   313,   313,   314,   314,   315,   316,   316,   317,
     317,   318,   319,   319,   320,   320,   320,   320,   320,   322,
     321,   321,   323,   323,   324,   324,   325,   325,   325,   326,
     326,   326,   327,   327,   327,   328,   329,   329,   329,   330,
     330,   331,   331,   332,   332,   332,   332,   333,   333,   334,
     335,   335,   336,   336,   337,   337,   338,   338,   339,   339,
     340,   340,   341,   341,   342,   342,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   343,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   344,   344,   344,
     345,   345,   345,   345,   345,   345,   345,   346,   347,   348,
     348,   348,   348,   348,   348,   348,   349,   349,   350,   351,
     351,   352,   353,   353,   354,   354,   354,   355,   355,   355,
     355,   355,   355,   356,   356,   356,   356,   356,   357,   357,
     357,   357,   357,   358,   359,   359,   359,   359,   359,   359,
     360,   361,   362,   362,   363,   363,   363,   363,   363,   363,
     363,   363,   363,   363,   363,   363,   363,   363,   363,   363,
     363,   364,   364,   365,   365,   365,   366,   366,   366,   366,
     367,   367,   367,   367,   367,   368,   368,   368,   369,   369,
     369,   369,   369,   369,   370,   370,   370,   370,   371,   371,
     372,   372,   373,   373,   373,   373,   373,   373,   373,   373,
     373,   373,   373,   373,   373,   374,   374,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     376,   376,   376,   376,   376,   376,   376,   377,   377,   377,
     377,   378,   378,   378,   378,   379,   379,   379,   379,   379,
     379,   379
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     2,     3,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     1,     3,     3,     3,     3,     2,     1,
       1,     1,     1,     1,     2,     1,     3,     3,     3,     3,
       2,     3,     3,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     2,     2,     2,     0,     3,     2,
       3,     1,     3,     1,     4,     3,     4,     4,     0,     1,
       1,     0,     1,     6,     2,     3,     3,     1,     1,     2,
       1,     1,     3,     3,     3,     5,     1,     3,     3,     3,
       5,     5,     0,     1,     4,     6,     8,     8,     6,     8,
       8,     4,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     5,     1,     3,     3,     4,     4,     4,
       4,     4,     4,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     2,     3,     1,     1,     0,     3,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     5,     5,     2,
       1,     1,     1,     1,     6,     7,     3,     0,     6,     0,
       6,     2,     5,     3,     3,     6,     6,     4,     5,     5,
       3,     3,     6,     5,     6,     5,     6,     3,     4,     3,
       4,     5,     6,     5,     6,     3,     4,     3,     4,     6,
       7,     6,     7,     4,     5,     4,     5,     4,     4,     3,
       6,     5,     4,     3,     6,     5,     6,     5,     8,     7,
       4,     4,     6,     3,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     6,     4,     7,     5,
       3,     6,     2,     2,     3,     3,     0,     2,     2,     3,
       5,     1,     3,     3,     5,     5,     0,     2,     3,     2,
       3,     4,     3,     1,     1,     3,     3,     5,     5,     2,
       1,     1,     1,     0,     2,     0,     2,     3,     3,     4,
       3,     3,     3,     1,     2,     0,     4,     2,     0,     5,
       1,     3,     1,     0,     8,     0,     1,     1,     1,     0,
       1,     1,     3,     1,     3,     1,     0,     2,     6,     0,
       3,     0,     4,     1,     0,     7,     4,     4,     6,     6,
       4,     2,     1,     3,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     0,     3,
       2,     3,     1,     3,     0,     1,     1,     1,     1,     4,
       5,     4,     5,     6,     6,     0,     1,     1,     1,     1,
       2,     2,     1,     1,     1,     1,     0,     1,     1,     2,
       1,     1,     1,     1,     1,     0,     1,     1,     2,     1,
       1,     1,     0,     1,     1,     1,     1,     2,     1,     1,
       0,     2,     2,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     3,     1,     2,     1,
       3,     2,     0,     2,     1,     2,     1,     1,     1,     0,
       5,     3,     1,     3,     3,     5,     1,     1,     3,     1,
       2,     3,     0,     2,     2,     1,     1,     1,     1,     2,
       1,     0,     1,     0,     2,     2,     1,     1,     1,     2,
       0,     1,     1,     3,     1,     1,     3,     3,     0,     1,
       1,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     4,     4,     4,     5,     2,     2,     1,     2,
       1,     2,     1,     2,     1,     2,     1,     1,     6,     6,
       4,     9,     9,     7,     6,     6,     4,     9,     9,     7,
       2,     3,     4,     6,     6,     9,     9,     6,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     4,     1,
       3,     4,     1,     3,     4,     3,     3,     1,     1,     2,
       1,     2,     1,     1,     2,     2,     2,     2,     2,     8,
       8,     9,     9,     4,     3,     3,     2,     2,     2,     1,
       3,     4,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
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
#line 635 "chpl.ypp"
                                      { context->topLevelStatements = (yyvsp[0].exprList); }
#line 6216 "bison-chpl-lib.cpp"
    break;

  case 3: /* toplevel_stmt_ls: %empty  */
#line 639 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList(); }
#line 6222 "bison-chpl-lib.cpp"
    break;

  case 4: /* toplevel_stmt_ls: toplevel_stmt_ls toplevel_stmt  */
#line 640 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6228 "bison-chpl-lib.cpp"
    break;

  case 6: /* toplevel_stmt: pragma_ls stmt  */
#line 647 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 6236 "bison-chpl-lib.cpp"
    break;

  case 7: /* pragma_ls: TPRAGMA STRINGLITERAL  */
#line 655 "chpl.ypp"
  {
    context->notePragma((yyloc), (yyvsp[0].expr));
  }
#line 6244 "bison-chpl-lib.cpp"
    break;

  case 8: /* pragma_ls: pragma_ls TPRAGMA STRINGLITERAL  */
#line 659 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-1]), (yylsp[0]));
    context->notePragma(loc, (yyvsp[0].expr));
  }
#line 6253 "bison-chpl-lib.cpp"
    break;

  case 9: /* stmt: attribute_group attribute_receiver  */
#line 669 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6261 "bison-chpl-lib.cpp"
    break;

  case 10: /* stmt: stmt_base  */
#line 674 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt);
  }
#line 6269 "bison-chpl-lib.cpp"
    break;

  case 12: /* stmt_base: decl_base  */
#line 681 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6275 "bison-chpl-lib.cpp"
    break;

  case 13: /* stmt_base: include_module_stmt  */
#line 682 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6281 "bison-chpl-lib.cpp"
    break;

  case 14: /* stmt_base: block_stmt  */
#line 683 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6287 "bison-chpl-lib.cpp"
    break;

  case 15: /* stmt_base: use_stmt  */
#line 684 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6293 "bison-chpl-lib.cpp"
    break;

  case 16: /* stmt_base: import_stmt  */
#line 685 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6299 "bison-chpl-lib.cpp"
    break;

  case 17: /* stmt_base: require_stmt  */
#line 686 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6305 "bison-chpl-lib.cpp"
    break;

  case 18: /* stmt_base: extern_block_stmt  */
#line 687 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6311 "bison-chpl-lib.cpp"
    break;

  case 19: /* stmt_base: implements_stmt  */
#line 688 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6317 "bison-chpl-lib.cpp"
    break;

  case 20: /* stmt_base: interface_stmt  */
#line 689 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6323 "bison-chpl-lib.cpp"
    break;

  case 21: /* stmt_base: TDEFER stmt  */
#line 691 "chpl.ypp"
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
#line 6340 "bison-chpl-lib.cpp"
    break;

  case 22: /* stmt_base: try_stmt  */
#line 703 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyloc), (yyvsp[0].commentsAndStmt)); }
#line 6346 "bison-chpl-lib.cpp"
    break;

  case 23: /* stmt_base: return_stmt  */
#line 704 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6352 "bison-chpl-lib.cpp"
    break;

  case 24: /* stmt_base: TBREAK opt_label_ident TSEMI  */
#line 706 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Break::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6365 "bison-chpl-lib.cpp"
    break;

  case 25: /* stmt_base: TCONTINUE opt_label_ident TSEMI  */
#line 715 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto ident = !(yyvsp[-1].uniqueStr).isEmpty() ? Identifier::build(BUILDER, LOC((yylsp[-1])), (yyvsp[-1].uniqueStr))
                               : nullptr;
    auto node = Continue::build(BUILDER, LOC((yyloc)), std::move(ident));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6378 "bison-chpl-lib.cpp"
    break;

  case 26: /* stmt_base: TLABEL ident_def stmt  */
#line 724 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildLabelStmt((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].commentsAndStmt));
  }
#line 6386 "bison-chpl-lib.cpp"
    break;

  case 27: /* stmt_base: TYIELD opt_try_expr TSEMI  */
#line 728 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Yield::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6397 "bison-chpl-lib.cpp"
    break;

  case 28: /* stmt_base: error TSEMI  */
#line 735 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = STMT((yyloc), ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
    context->resetDeclStateOnError();
  }
#line 6406 "bison-chpl-lib.cpp"
    break;

  case 29: /* tryable_stmt: assignment_stmt  */
#line 742 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6412 "bison-chpl-lib.cpp"
    break;

  case 30: /* tryable_stmt: if_stmt  */
#line 743 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6418 "bison-chpl-lib.cpp"
    break;

  case 31: /* tryable_stmt: loop_stmt  */
#line 744 "chpl.ypp"
                            { (yyval.commentsAndStmt) = (yyvsp[0].commentsAndStmt); }
#line 6424 "bison-chpl-lib.cpp"
    break;

  case 32: /* tryable_stmt: select_stmt  */
#line 745 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6430 "bison-chpl-lib.cpp"
    break;

  case 33: /* tryable_stmt: manage_stmt  */
#line 746 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6436 "bison-chpl-lib.cpp"
    break;

  case 34: /* tryable_stmt: stmt_level_expr TSEMI  */
#line 747 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt(STMT((yyloc),(yyvsp[-1].expr))); }
#line 6442 "bison-chpl-lib.cpp"
    break;

  case 35: /* tryable_stmt: throw_stmt  */
#line 748 "chpl.ypp"
                            { (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt)); }
#line 6448 "bison-chpl-lib.cpp"
    break;

  case 36: /* tryable_stmt: TBEGIN opt_task_intent_ls stmt  */
#line 750 "chpl.ypp"
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
#line 6466 "bison-chpl-lib.cpp"
    break;

  case 37: /* tryable_stmt: TCOBEGIN opt_task_intent_ls block_stmt  */
#line 764 "chpl.ypp"
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
#line 6485 "bison-chpl-lib.cpp"
    break;

  case 38: /* tryable_stmt: TDELETE expr_ls TSEMI  */
#line 779 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Delete::build(BUILDER, LOC((yyloc)), std::move(exprs));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 6497 "bison-chpl-lib.cpp"
    break;

  case 39: /* tryable_stmt: TLOCAL expr do_stmt  */
#line 787 "chpl.ypp"
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
#line 6513 "bison-chpl-lib.cpp"
    break;

  case 40: /* tryable_stmt: TLOCAL do_stmt  */
#line 799 "chpl.ypp"
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
#line 6528 "bison-chpl-lib.cpp"
    break;

  case 41: /* tryable_stmt: TON expr do_stmt  */
#line 810 "chpl.ypp"
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
#line 6544 "bison-chpl-lib.cpp"
    break;

  case 42: /* tryable_stmt: TSERIAL expr do_stmt  */
#line 822 "chpl.ypp"
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
#line 6560 "bison-chpl-lib.cpp"
    break;

  case 43: /* tryable_stmt: TSERIAL do_stmt  */
#line 834 "chpl.ypp"
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
#line 6575 "bison-chpl-lib.cpp"
    break;

  case 44: /* tryable_stmt: TSYNC stmt  */
#line 845 "chpl.ypp"
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
#line 6592 "bison-chpl-lib.cpp"
    break;

  case 53: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace opt_attribute_actuals  */
#line 881 "chpl.ypp"
 {
  context->noteAttribute((yylsp[-2]), (yyvsp[-2].expr), true, (yyvsp[-1].exprList), (yyvsp[0].maybeNamedActualList));
 }
#line 6600 "bison-chpl-lib.cpp"
    break;

  case 54: /* attribute_decl_stmt: attribute_decl_begin opt_attribute_decl_toolspace  */
#line 885 "chpl.ypp"
 {
  // this allows us to distinguish between @attribute() and @attribute
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, (yyvsp[0].exprList), lst);
 }
#line 6611 "bison-chpl-lib.cpp"
    break;

  case 55: /* attribute_decl_stmt: attribute_decl_begin STRINGLITERAL  */
#line 892 "chpl.ypp"
 {
  // this allows us to maintain @unstable "message" without requiring parens
  // but should not be needed once `@unstable "msg"` is deprecated and gone
  auto lst = new MaybeNamedActualList();
  auto actual = makeMaybeNamedActual((yyvsp[0].expr));
  lst->push_back(actual);
  context->noteAttribute((yylsp[-1]), (yyvsp[-1].expr), false, context->makeList(), lst);
 }
#line 6624 "bison-chpl-lib.cpp"
    break;

  case 56: /* attribute_decl_begin: TATMARK TIDENT  */
#line 904 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    (yyval.expr) = ident;
  }
#line 6633 "bison-chpl-lib.cpp"
    break;

  case 57: /* opt_attribute_decl_toolspace: %empty  */
#line 911 "chpl.ypp"
  {
    (yyval.exprList)=context->makeList();
  }
#line 6641 "bison-chpl-lib.cpp"
    break;

  case 58: /* opt_attribute_decl_toolspace: TDOT TIDENT opt_attribute_decl_toolspace  */
#line 915 "chpl.ypp"
  {
    // had to create this as a list first, otherwise they get inserted in the
    // wrong order.
    auto lst = context->makeList(context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr)));
    (yyval.exprList) = context->appendList(lst, (yyvsp[0].exprList));
  }
#line 6652 "bison-chpl-lib.cpp"
    break;

  case 59: /* opt_attribute_actuals: TLP TRP  */
#line 925 "chpl.ypp"
  { (yyval.maybeNamedActualList)=new MaybeNamedActualList(); }
#line 6658 "bison-chpl-lib.cpp"
    break;

  case 60: /* opt_attribute_actuals: TLP attribute_actuals_ls TRP  */
#line 927 "chpl.ypp"
  {
    (yyval.maybeNamedActualList) = (yyvsp[-1].maybeNamedActualList);
  }
#line 6666 "bison-chpl-lib.cpp"
    break;

  case 61: /* attribute_actuals_ls: attribute_actual  */
#line 934 "chpl.ypp"
  { auto lst = new MaybeNamedActualList();
    lst->push_back((yyvsp[0].maybeNamedActual));
    (yyval.maybeNamedActualList) = lst;
  }
#line 6675 "bison-chpl-lib.cpp"
    break;

  case 62: /* attribute_actuals_ls: attribute_actuals_ls TCOMMA attribute_actual  */
#line 939 "chpl.ypp"
  {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
  }
#line 6685 "bison-chpl-lib.cpp"
    break;

  case 64: /* module_decl_start: opt_access_control opt_prototype TMODULE ident_def  */
#line 952 "chpl.ypp"
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
#line 6706 "bison-chpl-lib.cpp"
    break;

  case 65: /* module_decl_stmt: module_decl_start TLCBR TRCBR  */
#line 972 "chpl.ypp"
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
#line 6725 "bison-chpl-lib.cpp"
    break;

  case 66: /* module_decl_stmt: module_decl_start TLCBR stmt_ls TRCBR  */
#line 987 "chpl.ypp"
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
#line 6744 "bison-chpl-lib.cpp"
    break;

  case 67: /* module_decl_stmt: module_decl_start TLCBR error TRCBR  */
#line 1002 "chpl.ypp"
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
#line 6761 "bison-chpl-lib.cpp"
    break;

  case 68: /* opt_access_control: %empty  */
#line 1017 "chpl.ypp"
           { (yyval.visibilityTag) = context->noteVisibility(Decl::DEFAULT_VISIBILITY); }
#line 6767 "bison-chpl-lib.cpp"
    break;

  case 69: /* opt_access_control: TPUBLIC  */
#line 1018 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PUBLIC); }
#line 6774 "bison-chpl-lib.cpp"
    break;

  case 70: /* opt_access_control: TPRIVATE  */
#line 1020 "chpl.ypp"
           { context->noteDeclStartLoc((yylsp[0]));
             (yyval.visibilityTag) = context->noteVisibility(Decl::PRIVATE); }
#line 6781 "bison-chpl-lib.cpp"
    break;

  case 71: /* opt_prototype: %empty  */
#line 1025 "chpl.ypp"
             { (yyval.moduleKind) = Module::DEFAULT_MODULE_KIND; }
#line 6787 "bison-chpl-lib.cpp"
    break;

  case 72: /* opt_prototype: TPROTOTYPE  */
#line 1026 "chpl.ypp"
             { context->noteDeclStartLoc((yylsp[0]));
               (yyval.moduleKind) = Module::PROTOTYPE;  }
#line 6794 "bison-chpl-lib.cpp"
    break;

  case 73: /* include_module_stmt: TINCLUDE opt_access_control opt_prototype TMODULE ident_def TSEMI  */
#line 1032 "chpl.ypp"
  {
    auto node = Include::build(BUILDER, LOC((yyloc)), context->visibility, (yyvsp[-3].moduleKind), (yyvsp[-1].uniqueStr));
    (yyval.commentsAndStmt) = STMT((yyloc), node.release());
    context->visibility = Decl::DEFAULT_VISIBILITY;
  }
#line 6804 "bison-chpl-lib.cpp"
    break;

  case 74: /* block_stmt_body: TLCBR TRCBR  */
#line 1050 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-1]), (yylsp[0]), nullptr);
  }
#line 6812 "bison-chpl-lib.cpp"
    break;

  case 75: /* block_stmt_body: TLCBR stmt_ls TRCBR  */
#line 1054 "chpl.ypp"
  {
    (yyval.exprList) = context->blockToParserExprList((yylsp[-2]), (yylsp[0]), (yyvsp[-1].exprList));
  }
#line 6820 "bison-chpl-lib.cpp"
    break;

  case 76: /* block_stmt_body: TLCBR error TRCBR  */
#line 1058 "chpl.ypp"
  {
    context->clearComments();
    // create a list of stmts that just has an ErroneousExpr
    auto errorExpr = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    ParserExprList* lst = context->makeList(errorExpr.release());
    (yyval.exprList) = lst;
  }
#line 6832 "bison-chpl-lib.cpp"
    break;

  case 77: /* block_stmt: block_stmt_body  */
#line 1069 "chpl.ypp"
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
#line 6851 "bison-chpl-lib.cpp"
    break;

  case 78: /* stmt_ls: toplevel_stmt  */
#line 1087 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt)); }
#line 6857 "bison-chpl-lib.cpp"
    break;

  case 79: /* stmt_ls: stmt_ls toplevel_stmt  */
#line 1088 "chpl.ypp"
                                       { context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 6863 "bison-chpl-lib.cpp"
    break;

  case 80: /* renames_ls: expr  */
#line 1093 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].expr));
  }
#line 6871 "bison-chpl-lib.cpp"
    break;

  case 81: /* renames_ls: all_op_name  */
#line 1097 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6879 "bison-chpl-lib.cpp"
    break;

  case 82: /* renames_ls: expr TAS expr  */
#line 1101 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->makeList(as);
  }
#line 6888 "bison-chpl-lib.cpp"
    break;

  case 83: /* renames_ls: renames_ls TCOMMA expr  */
#line 1106 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 6896 "bison-chpl-lib.cpp"
    break;

  case 84: /* renames_ls: renames_ls TCOMMA all_op_name  */
#line 1110 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
  }
#line 6904 "bison-chpl-lib.cpp"
    break;

  case 85: /* renames_ls: renames_ls TCOMMA expr TAS expr  */
#line 1114 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), as);
  }
#line 6913 "bison-chpl-lib.cpp"
    break;

  case 86: /* use_renames_ls: expr  */
#line 1124 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6922 "bison-chpl-lib.cpp"
    break;

  case 87: /* use_renames_ls: expr TAS expr  */
#line 1129 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6932 "bison-chpl-lib.cpp"
    break;

  case 88: /* use_renames_ls: expr TAS TUNDERSCORE  */
#line 1135 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    auto node = context->buildVisibilityClause((yyloc), toOwned(as), false);
    (yyval.exprList) = context->makeList(node);
  }
#line 6943 "bison-chpl-lib.cpp"
    break;

  case 89: /* use_renames_ls: use_renames_ls TCOMMA expr  */
#line 1142 "chpl.ypp"
  {
    auto node = context->buildVisibilityClause((yylsp[0]), toOwned((yyvsp[0].expr)), false);
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), node);
  }
#line 6952 "bison-chpl-lib.cpp"
    break;

  case 90: /* use_renames_ls: use_renames_ls TCOMMA expr TAS expr  */
#line 1147 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned((yyvsp[0].expr)));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6963 "bison-chpl-lib.cpp"
    break;

  case 91: /* use_renames_ls: use_renames_ls TCOMMA expr TAS TUNDERSCORE  */
#line 1154 "chpl.ypp"
  {
    auto ident = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), toOwned(ident));
    auto locVisClause = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildVisibilityClause(locVisClause, toOwned(as), false);
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 6975 "bison-chpl-lib.cpp"
    break;

  case 92: /* opt_only_ls: %empty  */
#line 1164 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 6981 "bison-chpl-lib.cpp"
    break;

  case 93: /* opt_only_ls: renames_ls  */
#line 1165 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 6987 "bison-chpl-lib.cpp"
    break;

  case 94: /* use_stmt: opt_access_control TUSE use_renames_ls TSEMI  */
#line 1170 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildMultiUseStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 6996 "bison-chpl-lib.cpp"
    break;

  case 95: /* use_stmt: opt_access_control TUSE expr TEXCEPT renames_ls TSEMI  */
#line 1175 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7008 "bison-chpl-lib.cpp"
    break;

  case 96: /* use_stmt: opt_access_control TUSE expr TAS expr TEXCEPT renames_ls TSEMI  */
#line 1183 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7021 "bison-chpl-lib.cpp"
    break;

  case 97: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TEXCEPT renames_ls TSEMI  */
#line 1192 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::EXCEPT,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7035 "bison-chpl-lib.cpp"
    break;

  case 98: /* use_stmt: opt_access_control TUSE expr TONLY opt_only_ls TSEMI  */
#line 1202 "chpl.ypp"
  {
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-3]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-5].visibilityTag), toOwned((yyvsp[-3].expr)),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7047 "bison-chpl-lib.cpp"
    break;

  case 99: /* use_stmt: opt_access_control TUSE expr TAS expr TONLY opt_only_ls TSEMI  */
#line 1210 "chpl.ypp"
  {
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), toOwned((yyvsp[-3].expr)));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7060 "bison-chpl-lib.cpp"
    break;

  case 100: /* use_stmt: opt_access_control TUSE expr TAS TUNDERSCORE TONLY opt_only_ls TSEMI  */
#line 1219 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[-3]), (yyvsp[-3].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-5]), (yylsp[-3]), toOwned((yyvsp[-5].expr)), std::move(ident));
    auto locVisibilityClause = context->makeSpannedLocation((yylsp[-5]), (yylsp[-1]));
    (yyval.commentsAndStmt) = context->buildSingleUseStmt((yyloc), locVisibilityClause, (yyvsp[-7].visibilityTag), toOwned(as),
                                     VisibilityClause::ONLY,
                                     (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7074 "bison-chpl-lib.cpp"
    break;

  case 101: /* import_stmt: opt_access_control TIMPORT import_ls TSEMI  */
#line 1232 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImportStmt((yyloc), (yyvsp[-3].visibilityTag), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7083 "bison-chpl-lib.cpp"
    break;

  case 119: /* import_expr: expr  */
#line 1268 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[0].expr)), true);
  }
#line 7091 "bison-chpl-lib.cpp"
    break;

  case 120: /* import_expr: expr TDOT all_op_name  */
#line 1272 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7100 "bison-chpl-lib.cpp"
    break;

  case 121: /* import_expr: expr TDOT import_internal_type_ident  */
#line 1277 "chpl.ypp"
  {
    auto dot = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release();
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(dot), true);
  }
#line 7109 "bison-chpl-lib.cpp"
    break;

  case 122: /* import_expr: expr TAS ident_use  */
#line 1282 "chpl.ypp"
  {
    auto ident = toOwned(context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)));
    auto as = context->buildAsExpr((yylsp[-2]), (yylsp[0]), toOwned((yyvsp[-2].expr)), std::move(ident));
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned(as), true);
  }
#line 7119 "bison-chpl-lib.cpp"
    break;

  case 123: /* import_expr: expr TDOT TLCBR renames_ls TRCBR  */
#line 1288 "chpl.ypp"
  {
    (yyval.expr) = context->buildVisibilityClause((yyloc), toOwned((yyvsp[-4].expr)),
                                        VisibilityClause::BRACES,
                                        context->consumeList((yyvsp[-1].exprList)), true);
  }
#line 7129 "bison-chpl-lib.cpp"
    break;

  case 124: /* import_ls: import_expr  */
#line 1296 "chpl.ypp"
                                { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 7135 "bison-chpl-lib.cpp"
    break;

  case 125: /* import_ls: import_ls TCOMMA import_expr  */
#line 1297 "chpl.ypp"
                                { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 7141 "bison-chpl-lib.cpp"
    break;

  case 126: /* require_stmt: TREQUIRE expr_ls TSEMI  */
#line 1302 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Require::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 7151 "bison-chpl-lib.cpp"
    break;

  case 127: /* assignment_stmt: lhs_expr assignop_ident opt_try_expr TSEMI  */
#line 1311 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7159 "bison-chpl-lib.cpp"
    break;

  case 128: /* assignment_stmt: lhs_expr TSWAP opt_try_expr TSEMI  */
#line 1315 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7167 "bison-chpl-lib.cpp"
    break;

  case 129: /* assignment_stmt: lhs_expr TASSIGNREDUCE opt_try_expr TSEMI  */
#line 1319 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7175 "bison-chpl-lib.cpp"
    break;

  case 130: /* assignment_stmt: lhs_expr TASSIGNLAND opt_try_expr TSEMI  */
#line 1323 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7183 "bison-chpl-lib.cpp"
    break;

  case 131: /* assignment_stmt: lhs_expr TASSIGNLOR opt_try_expr TSEMI  */
#line 1327 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr)));
    }
#line 7191 "bison-chpl-lib.cpp"
    break;

  case 132: /* assignment_stmt: lhs_expr TASSIGN TNOINIT TSEMI  */
#line 1331 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildBinOp((yyloc), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr))));
    }
#line 7199 "bison-chpl-lib.cpp"
    break;

  case 133: /* opt_label_ident: %empty  */
#line 1339 "chpl.ypp"
         { (yyval.uniqueStr) = STR(""); }
#line 7205 "bison-chpl-lib.cpp"
    break;

  case 134: /* opt_label_ident: TIDENT  */
#line 1340 "chpl.ypp"
         { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7211 "bison-chpl-lib.cpp"
    break;

  case 142: /* ident_use: TIDENT  */
#line 1365 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7217 "bison-chpl-lib.cpp"
    break;

  case 143: /* ident_use: TTHIS  */
#line 1366 "chpl.ypp"
                           { (yyval.uniqueStr) = (yyvsp[0].uniqueStr); }
#line 7223 "bison-chpl-lib.cpp"
    break;

  case 163: /* scalar_type: TBOOL  */
#line 1393 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7229 "bison-chpl-lib.cpp"
    break;

  case 164: /* scalar_type: TENUM  */
#line 1394 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7235 "bison-chpl-lib.cpp"
    break;

  case 165: /* scalar_type: TINT  */
#line 1395 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7241 "bison-chpl-lib.cpp"
    break;

  case 166: /* scalar_type: TUINT  */
#line 1396 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7247 "bison-chpl-lib.cpp"
    break;

  case 167: /* scalar_type: TREAL  */
#line 1397 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7253 "bison-chpl-lib.cpp"
    break;

  case 168: /* scalar_type: TIMAG  */
#line 1398 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7259 "bison-chpl-lib.cpp"
    break;

  case 169: /* scalar_type: TCOMPLEX  */
#line 1399 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7265 "bison-chpl-lib.cpp"
    break;

  case 170: /* scalar_type: TBYTES  */
#line 1400 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7271 "bison-chpl-lib.cpp"
    break;

  case 171: /* scalar_type: TSTRING  */
#line 1401 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7277 "bison-chpl-lib.cpp"
    break;

  case 172: /* scalar_type: TLOCALE  */
#line 1402 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7283 "bison-chpl-lib.cpp"
    break;

  case 173: /* scalar_type: TNOTHING  */
#line 1403 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7289 "bison-chpl-lib.cpp"
    break;

  case 174: /* scalar_type: TVOID  */
#line 1404 "chpl.ypp"
           { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 7295 "bison-chpl-lib.cpp"
    break;

  case 179: /* do_stmt: TDO stmt  */
#line 1418 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), true }; }
#line 7301 "bison-chpl-lib.cpp"
    break;

  case 180: /* do_stmt: block_stmt  */
#line 1419 "chpl.ypp"
                { (yyval.blockOrDo) = { (yyvsp[0].commentsAndStmt), false }; }
#line 7307 "bison-chpl-lib.cpp"
    break;

  case 181: /* return_stmt: TRETURN TSEMI  */
#line 1424 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-1]));
    auto node = Return::build(BUILDER, LOC((yyloc)), /*value*/ nullptr);
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7318 "bison-chpl-lib.cpp"
    break;

  case 182: /* return_stmt: TRETURN opt_try_expr TSEMI  */
#line 1431 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7329 "bison-chpl-lib.cpp"
    break;

  case 183: /* class_level_stmt: TSEMI  */
#line 1441 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = STMT((yyloc), context->buildEmptyStmt((yyloc)));
    }
#line 7337 "bison-chpl-lib.cpp"
    break;

  case 184: /* class_level_stmt: inner_class_level_stmt  */
#line 1445 "chpl.ypp"
    {
      // visibility should be default when inner_class_level_stmt is parsed
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7347 "bison-chpl-lib.cpp"
    break;

  case 185: /* $@1: %empty  */
#line 1450 "chpl.ypp"
          {context->noteDeclStartLoc((yylsp[0]));
           context->noteVisibility(Decl::PUBLIC);}
#line 7354 "bison-chpl-lib.cpp"
    break;

  case 186: /* class_level_stmt: TPUBLIC $@1 inner_class_level_stmt  */
#line 1452 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7363 "bison-chpl-lib.cpp"
    break;

  case 187: /* $@2: %empty  */
#line 1456 "chpl.ypp"
           {context->noteDeclStartLoc((yylsp[0]));
            context->noteVisibility(Decl::PRIVATE);}
#line 7370 "bison-chpl-lib.cpp"
    break;

  case 188: /* class_level_stmt: TPRIVATE $@2 inner_class_level_stmt  */
#line 1458 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->finishStmt((yyvsp[0].commentsAndStmt));
      context->visibility = Decl::DEFAULT_VISIBILITY;
    }
#line 7379 "bison-chpl-lib.cpp"
    break;

  case 196: /* forwarding_decl_stmt: forwarding_decl_start expr TSEMI  */
#line 1476 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-2].attributeGroup)), toOwned((yyvsp[-1].expr)),
                                        VisibilityClause::NONE, nullptr);
    }
#line 7388 "bison-chpl-lib.cpp"
    break;

  case 197: /* forwarding_decl_stmt: forwarding_decl_start expr TEXCEPT renames_ls TSEMI  */
#line 1481 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::EXCEPT, (yyvsp[-1].exprList));
    }
#line 7397 "bison-chpl-lib.cpp"
    break;

  case 198: /* forwarding_decl_stmt: forwarding_decl_start expr TONLY opt_only_ls TSEMI  */
#line 1486 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-4].attributeGroup)), toOwned((yyvsp[-3].expr)),
                                        VisibilityClause::ONLY, (yyvsp[-1].exprList));
    }
#line 7406 "bison-chpl-lib.cpp"
    break;

  case 199: /* forwarding_decl_stmt: forwarding_decl_start var_decl_stmt  */
#line 1491 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildForwardingDecl((yyloc), toOwned((yyvsp[-1].attributeGroup)), (yyvsp[0].commentsAndStmt));
    }
#line 7414 "bison-chpl-lib.cpp"
    break;

  case 200: /* forwarding_decl_start: TFORWARDING  */
#line 1498 "chpl.ypp"
  {
    (yyval.attributeGroup) = context->buildAttributeGroup((yyloc)).release();
    context->resetAttributeGroupPartsState();
  }
#line 7423 "bison-chpl-lib.cpp"
    break;

  case 201: /* extern_or_export: TEXTERN  */
#line 1505 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXTERN; }
#line 7429 "bison-chpl-lib.cpp"
    break;

  case 202: /* extern_or_export: TEXPORT  */
#line 1506 "chpl.ypp"
          { (yyval.linkageTag) = Decl::EXPORT; }
#line 7435 "bison-chpl-lib.cpp"
    break;

  case 203: /* extern_export_decl_stmt_start: extern_or_export  */
#line 1511 "chpl.ypp"
  {
    // Sets the start location only if it is still unset.
    context->noteDeclStartLoc((yylsp[0]));
    (yyval.linkageTag) = context->noteLinkage((yyvsp[0].linkageTag));
  }
#line 7445 "bison-chpl-lib.cpp"
    break;

  case 204: /* extern_export_decl_stmt: extern_export_decl_stmt_start class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1521 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-5]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7454 "bison-chpl-lib.cpp"
    break;

  case 205: /* extern_export_decl_stmt: extern_export_decl_stmt_start STRINGLITERAL class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 1527 "chpl.ypp"
  {
    // Set the linkage name since it will be nullptr otherwise.
    (yyvsp[-4].typeDeclParts).linkageName = (yyvsp[-5].expr);
    (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yylsp[-6]), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
    context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
  }
#line 7465 "bison-chpl-lib.cpp"
    break;

  case 206: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr fn_decl_stmt  */
#line 1534 "chpl.ypp"
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
#line 7487 "bison-chpl-lib.cpp"
    break;

  case 207: /* $@3: %empty  */
#line 1552 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[0].expr));
  }
#line 7495 "bison-chpl-lib.cpp"
    break;

  case 208: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr $@3 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 1556 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7504 "bison-chpl-lib.cpp"
    break;

  case 209: /* $@4: %empty  */
#line 1561 "chpl.ypp"
  {
    context->storeVarDeclLinkageName((yyvsp[-1].expr));
    context->noteVarDeclKind(Variable::TYPE);
  }
#line 7513 "bison-chpl-lib.cpp"
    break;

  case 210: /* extern_export_decl_stmt: extern_export_decl_stmt_start opt_expr TTYPE $@4 type_alias_decl_stmt_inner_ls TSEMI  */
#line 1566 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 7522 "bison-chpl-lib.cpp"
    break;

  case 211: /* extern_block_stmt: TEXTERN EXTERNCODE  */
#line 1574 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildExternBlockStmt((yylsp[-1]), (yyvsp[0].sizedStr));
  }
#line 7530 "bison-chpl-lib.cpp"
    break;

  case 212: /* loop_stmt: TDO stmt TWHILE expr TSEMI  */
#line 1581 "chpl.ypp"
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
#line 7552 "bison-chpl-lib.cpp"
    break;

  case 213: /* loop_stmt: TWHILE expr do_stmt  */
#line 1599 "chpl.ypp"
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
#line 7569 "bison-chpl-lib.cpp"
    break;

  case 214: /* loop_stmt: TWHILE ifvar do_stmt  */
#line 1612 "chpl.ypp"
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
#line 7586 "bison-chpl-lib.cpp"
    break;

  case 215: /* loop_stmt: TCOFORALL expr TIN expr opt_task_intent_ls do_stmt  */
#line 1625 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7594 "bison-chpl-lib.cpp"
    break;

  case 216: /* loop_stmt: TCOFORALL expr TIN zippered_iterator opt_task_intent_ls do_stmt  */
#line 1629 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7602 "bison-chpl-lib.cpp"
    break;

  case 217: /* loop_stmt: TCOFORALL expr opt_task_intent_ls do_stmt  */
#line 1633 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildCoforallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7610 "bison-chpl-lib.cpp"
    break;

  case 218: /* loop_stmt: TFOR expr TIN expr do_stmt  */
#line 1637 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7618 "bison-chpl-lib.cpp"
    break;

  case 219: /* loop_stmt: TFOR expr TIN zippered_iterator do_stmt  */
#line 1641 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7626 "bison-chpl-lib.cpp"
    break;

  case 220: /* loop_stmt: TFOR expr do_stmt  */
#line 1645 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7634 "bison-chpl-lib.cpp"
    break;

  case 221: /* loop_stmt: TFOR zippered_iterator do_stmt  */
#line 1649 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), (yyvsp[0].blockOrDo));
  }
#line 7642 "bison-chpl-lib.cpp"
    break;

  case 222: /* loop_stmt: TFOR TPARAM ident_def TIN expr do_stmt  */
#line 1653 "chpl.ypp"
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
                           context->buildAttributeGroup((yyloc)));
    CommentsAndStmt cs = { .comments=comments, .stmt=node.release() };
    context->resetAttributeGroupPartsState();
    (yyval.commentsAndStmt) = context->finishStmt(cs);
  }
#line 7666 "bison-chpl-lib.cpp"
    break;

  case 223: /* loop_stmt: TFORALL expr TIN expr do_stmt  */
#line 1673 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7674 "bison-chpl-lib.cpp"
    break;

  case 224: /* loop_stmt: TFORALL expr TIN expr forall_intent_clause do_stmt  */
#line 1677 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7682 "bison-chpl-lib.cpp"
    break;

  case 225: /* loop_stmt: TFORALL expr TIN zippered_iterator do_stmt  */
#line 1681 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7690 "bison-chpl-lib.cpp"
    break;

  case 226: /* loop_stmt: TFORALL expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1685 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7698 "bison-chpl-lib.cpp"
    break;

  case 227: /* loop_stmt: TFORALL expr do_stmt  */
#line 1689 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7706 "bison-chpl-lib.cpp"
    break;

  case 228: /* loop_stmt: TFORALL expr forall_intent_clause do_stmt  */
#line 1693 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7714 "bison-chpl-lib.cpp"
    break;

  case 229: /* loop_stmt: TFORALL zippered_iterator do_stmt  */
#line 1697 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7722 "bison-chpl-lib.cpp"
    break;

  case 230: /* loop_stmt: TFORALL zippered_iterator forall_intent_clause do_stmt  */
#line 1701 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForallLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7730 "bison-chpl-lib.cpp"
    break;

  case 231: /* loop_stmt: TFOREACH expr TIN expr do_stmt  */
#line 1705 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7738 "bison-chpl-lib.cpp"
    break;

  case 232: /* loop_stmt: TFOREACH expr TIN expr forall_intent_clause do_stmt  */
#line 1709 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7746 "bison-chpl-lib.cpp"
    break;

  case 233: /* loop_stmt: TFOREACH expr TIN zippered_iterator do_stmt  */
#line 1713 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[0]), (yyvsp[-3].expr), (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7754 "bison-chpl-lib.cpp"
    break;

  case 234: /* loop_stmt: TFOREACH expr TIN zippered_iterator forall_intent_clause do_stmt  */
#line 1717 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[0]), (yyvsp[-4].expr), (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7762 "bison-chpl-lib.cpp"
    break;

  case 235: /* loop_stmt: TFOREACH expr do_stmt  */
#line 1721 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7770 "bison-chpl-lib.cpp"
    break;

  case 236: /* loop_stmt: TFOREACH expr forall_intent_clause do_stmt  */
#line 1725 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7778 "bison-chpl-lib.cpp"
    break;

  case 237: /* loop_stmt: TFOREACH zippered_iterator do_stmt  */
#line 1729 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-2]), (yylsp[-2]), (yylsp[0]), nullptr, (yyvsp[-1].expr), nullptr, (yyvsp[0].blockOrDo));
  }
#line 7786 "bison-chpl-lib.cpp"
    break;

  case 238: /* loop_stmt: TFOREACH zippered_iterator forall_intent_clause do_stmt  */
#line 1733 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildForeachLoopStmt((yylsp[-3]), (yylsp[-3]), (yylsp[0]), nullptr, (yyvsp[-2].expr), (yyvsp[-1].withClause), (yyvsp[0].blockOrDo));
  }
#line 7794 "bison-chpl-lib.cpp"
    break;

  case 239: /* loop_stmt: TLSBR expr_ls TIN expr TRSBR stmt  */
#line 1737 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7802 "bison-chpl-lib.cpp"
    break;

  case 240: /* loop_stmt: TLSBR expr_ls TIN expr forall_intent_clause TRSBR stmt  */
#line 1741 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7810 "bison-chpl-lib.cpp"
    break;

  case 241: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator TRSBR stmt  */
#line 1745 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-5]), (yylsp[-4]), (yylsp[-1]), (yyvsp[-4].exprList), (yyvsp[-2].expr), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7818 "bison-chpl-lib.cpp"
    break;

  case 242: /* loop_stmt: TLSBR expr_ls TIN zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1749 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-6]), (yylsp[-5]), (yylsp[-1]), (yyvsp[-5].exprList), (yyvsp[-3].expr), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7826 "bison-chpl-lib.cpp"
    break;

  case 243: /* loop_stmt: TLSBR expr_ls TRSBR stmt  */
#line 1753 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-2].exprList), nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7834 "bison-chpl-lib.cpp"
    break;

  case 244: /* loop_stmt: TLSBR expr_ls forall_intent_clause TRSBR stmt  */
#line 1757 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-3].exprList), (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7842 "bison-chpl-lib.cpp"
    break;

  case 245: /* loop_stmt: TLSBR zippered_iterator TRSBR stmt  */
#line 1761 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-3]), (yylsp[-2]), (yylsp[-1]), iterExprs, nullptr, (yyvsp[0].commentsAndStmt));
  }
#line 7851 "bison-chpl-lib.cpp"
    break;

  case 246: /* loop_stmt: TLSBR zippered_iterator forall_intent_clause TRSBR stmt  */
#line 1766 "chpl.ypp"
  {
    auto iterExprs = context->makeList((yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildBracketLoopStmt((yylsp[-4]), (yylsp[-3]), (yylsp[-1]), iterExprs, (yyvsp[-2].withClause), (yyvsp[0].commentsAndStmt));
  }
#line 7860 "bison-chpl-lib.cpp"
    break;

  case 247: /* zippered_iterator: TZIP TLP expr_ls TRP  */
#line 1774 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = Zip::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.expr) = node.release();
  }
#line 7870 "bison-chpl-lib.cpp"
    break;

  case 248: /* if_stmt: TIF expr TTHEN stmt  */
#line 1783 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7878 "bison-chpl-lib.cpp"
    break;

  case 249: /* if_stmt: TIF expr block_stmt  */
#line 1787 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7886 "bison-chpl-lib.cpp"
    break;

  case 250: /* if_stmt: TIF expr TTHEN stmt TELSE stmt  */
#line 1791 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7894 "bison-chpl-lib.cpp"
    break;

  case 251: /* if_stmt: TIF expr block_stmt TELSE stmt  */
#line 1795 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7902 "bison-chpl-lib.cpp"
    break;

  case 252: /* if_stmt: TIF ifvar TTHEN stmt  */
#line 1799 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-3]), (yylsp[-1]), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7910 "bison-chpl-lib.cpp"
    break;

  case 253: /* if_stmt: TIF ifvar block_stmt  */
#line 1803 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-2]), (yylsp[0]), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt));
  }
#line 7918 "bison-chpl-lib.cpp"
    break;

  case 254: /* if_stmt: TIF ifvar TTHEN stmt TELSE stmt  */
#line 1807 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-3]), (yylsp[-1]), (yyvsp[-4].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7926 "bison-chpl-lib.cpp"
    break;

  case 255: /* if_stmt: TIF ifvar block_stmt TELSE stmt  */
#line 1811 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[-2]), (yylsp[-1]), (yyvsp[-3].expr), (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7934 "bison-chpl-lib.cpp"
    break;

  case 256: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt  */
#line 1815 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-3]), (yyvsp[-4].expr), (yyvsp[-3].uniqueStr), (yyvsp[-2].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-5]), (yylsp[-1]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7943 "bison-chpl-lib.cpp"
    break;

  case 257: /* if_stmt: TIF expr assignop_ident expr block_stmt  */
#line 1820 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-2]), (yyvsp[-3].expr), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-4]), (yylsp[0]), op, (yyvsp[0].commentsAndStmt));
  }
#line 7952 "bison-chpl-lib.cpp"
    break;

  case 258: /* if_stmt: TIF expr assignop_ident expr TTHEN stmt TELSE stmt  */
#line 1825 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-5]), (yyvsp[-6].expr), (yyvsp[-5].uniqueStr), (yyvsp[-4].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(true, (yylsp[-7]), (yylsp[-3]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7961 "bison-chpl-lib.cpp"
    break;

  case 259: /* if_stmt: TIF expr assignop_ident expr block_stmt TELSE stmt  */
#line 1830 "chpl.ypp"
  {
    auto op = context->buildBinOp((yylsp[-4]), (yyvsp[-5].expr), (yyvsp[-4].uniqueStr), (yyvsp[-3].expr));
    (yyval.commentsAndStmt) = context->buildConditionalStmt(false, (yylsp[-6]), (yylsp[-2]), (yylsp[-1]), op, (yyvsp[-2].commentsAndStmt), (yyvsp[0].commentsAndStmt));
  }
#line 7970 "bison-chpl-lib.cpp"
    break;

  case 260: /* ifvar: TVAR ident_def TASSIGN expr  */
#line 1838 "chpl.ypp"
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
#line 7989 "bison-chpl-lib.cpp"
    break;

  case 261: /* ifvar: TCONST ident_def TASSIGN expr  */
#line 1853 "chpl.ypp"
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
#line 8008 "bison-chpl-lib.cpp"
    break;

  case 262: /* interface_stmt: TINTERFACE ident_def TLP ifc_formal_ls TRP block_stmt  */
#line 1871 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-4].uniqueStr), (yyvsp[-2].exprList), (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8016 "bison-chpl-lib.cpp"
    break;

  case 263: /* interface_stmt: TINTERFACE ident_def block_stmt  */
#line 1875 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildInterfaceStmt((yyloc), (yyvsp[-1].uniqueStr), nullptr, (yylsp[0]), (yyvsp[0].commentsAndStmt));
  }
#line 8024 "bison-chpl-lib.cpp"
    break;

  case 264: /* ifc_formal_ls: ifc_formal  */
#line 1881 "chpl.ypp"
                                  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8030 "bison-chpl-lib.cpp"
    break;

  case 265: /* ifc_formal_ls: ifc_formal_ls TCOMMA ifc_formal  */
#line 1882 "chpl.ypp"
                                  { context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8036 "bison-chpl-lib.cpp"
    break;

  case 266: /* ifc_formal: ident_def  */
#line 1887 "chpl.ypp"
             { (yyval.expr) = context->buildInterfaceFormal((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 8042 "bison-chpl-lib.cpp"
    break;

  case 279: /* implements_type_ident: implements_type_error_ident  */
#line 1905 "chpl.ypp"
  {
    context->syntax((yyloc), "type '%s' cannot implement an interface.", (yyvsp[0].uniqueStr).c_str());
    (yyval.uniqueStr) = (yyvsp[0].uniqueStr);
  }
#line 8051 "bison-chpl-lib.cpp"
    break;

  case 286: /* implements_stmt: TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1931 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8059 "bison-chpl-lib.cpp"
    break;

  case 287: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TSEMI  */
#line 1935 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-3]), (yyvsp[-3].uniqueStr), (yylsp[-1]), (yyvsp[-1].uniqueStr), nullptr);
  }
#line 8067 "bison-chpl-lib.cpp"
    break;

  case 288: /* implements_stmt: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP TSEMI  */
#line 1939 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildImplementsStmt((yyloc), (yylsp[-6]), (yyvsp[-6].uniqueStr), YLOC2((yylsp[-4]), (yylsp[-1])), (yyvsp[-4].uniqueStr), (yyvsp[-2].maybeNamedActualList));
  }
#line 8075 "bison-chpl-lib.cpp"
    break;

  case 289: /* ifc_constraint: TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1946 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8083 "bison-chpl-lib.cpp"
    break;

  case 290: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def  */
#line 1950 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yylsp[0]), (yyvsp[0].uniqueStr), nullptr);
  }
#line 8091 "bison-chpl-lib.cpp"
    break;

  case 291: /* ifc_constraint: implements_type_ident TIMPLEMENTS ident_def TLP actual_ls TRP  */
#line 1954 "chpl.ypp"
  {
    (yyval.expr) = context->buildImplementsConstraint((yyloc), (yylsp[-5]), (yyvsp[-5].uniqueStr), YLOC2((yylsp[-3]), (yylsp[0])), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList));
  }
#line 8099 "bison-chpl-lib.cpp"
    break;

  case 292: /* try_stmt: TTRY tryable_stmt  */
#line 1961 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), false);
  }
#line 8107 "bison-chpl-lib.cpp"
    break;

  case 293: /* try_stmt: TTRYBANG tryable_stmt  */
#line 1965 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryExprStmt((yyloc), (yyvsp[0].commentsAndStmt), true);
  }
#line 8115 "bison-chpl-lib.cpp"
    break;

  case 294: /* try_stmt: TTRY block_stmt catch_expr_ls  */
#line 1969 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), false);
  }
#line 8123 "bison-chpl-lib.cpp"
    break;

  case 295: /* try_stmt: TTRYBANG block_stmt catch_expr_ls  */
#line 1973 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildTryCatchStmt((yyloc), (yyvsp[-1].commentsAndStmt), (yyvsp[0].exprList), true);
  }
#line 8131 "bison-chpl-lib.cpp"
    break;

  case 296: /* catch_expr_ls: %empty  */
#line 1979 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8137 "bison-chpl-lib.cpp"
    break;

  case 297: /* catch_expr_ls: catch_expr_ls catch_expr  */
#line 1980 "chpl.ypp"
                            { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].expr)); }
#line 8143 "bison-chpl-lib.cpp"
    break;

  case 298: /* catch_expr: TCATCH block_stmt  */
#line 1985 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), nullptr, (yyvsp[0].commentsAndStmt), false);
  }
#line 8151 "bison-chpl-lib.cpp"
    break;

  case 299: /* catch_expr: TCATCH catch_expr_inner block_stmt  */
#line 1989 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-1].expr), (yyvsp[0].commentsAndStmt), false);
  }
#line 8159 "bison-chpl-lib.cpp"
    break;

  case 300: /* catch_expr: TCATCH TLP catch_expr_inner TRP block_stmt  */
#line 1993 "chpl.ypp"
  {
    (yyval.expr) = context->buildCatch((yyloc), (yyvsp[-2].expr), (yyvsp[0].commentsAndStmt), true);
  }
#line 8167 "bison-chpl-lib.cpp"
    break;

  case 301: /* catch_expr_inner: ident_def  */
#line 2000 "chpl.ypp"
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
#line 8184 "bison-chpl-lib.cpp"
    break;

  case 302: /* catch_expr_inner: ident_def TCOLON expr  */
#line 2013 "chpl.ypp"
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
#line 8201 "bison-chpl-lib.cpp"
    break;

  case 303: /* throw_stmt: TTHROW expr TSEMI  */
#line 2029 "chpl.ypp"
  {
    auto comments = context->gatherComments((yylsp[-2]));
    auto node = Throw::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-1].expr)));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8211 "bison-chpl-lib.cpp"
    break;

  case 304: /* select_stmt: TSELECT expr TLCBR when_stmt_ls TRCBR  */
#line 2037 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildSelectStmt((yyloc), toOwned((yyvsp[-3].expr)), (yyvsp[-1].exprList));
  }
#line 8219 "bison-chpl-lib.cpp"
    break;

  case 305: /* select_stmt: TSELECT expr TLCBR error TRCBR  */
#line 2041 "chpl.ypp"
  {
    auto comments = context->gatherComments((yyloc));
    auto node = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
    (yyval.commentsAndStmt) = { .comments=comments, .stmt=node.release() };
  }
#line 8229 "bison-chpl-lib.cpp"
    break;

  case 306: /* when_stmt_ls: %empty  */
#line 2049 "chpl.ypp"
                          { (yyval.exprList) = context->makeList(); }
#line 8235 "bison-chpl-lib.cpp"
    break;

  case 307: /* when_stmt_ls: when_stmt_ls when_stmt  */
#line 2050 "chpl.ypp"
                          { (yyval.exprList) = context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt)); }
#line 8241 "bison-chpl-lib.cpp"
    break;

  case 308: /* when_stmt: TWHEN expr_ls do_stmt  */
#line 2055 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), (yyvsp[-1].exprList), (yyvsp[0].blockOrDo));
  }
#line 8249 "bison-chpl-lib.cpp"
    break;

  case 309: /* when_stmt: TOTHERWISE stmt  */
#line 2059 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=false };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8258 "bison-chpl-lib.cpp"
    break;

  case 310: /* when_stmt: TOTHERWISE TDO stmt  */
#line 2064 "chpl.ypp"
  {
    BlockOrDo blockOrDo = { .cs=(yyvsp[0].commentsAndStmt), .usesDo=true };
    (yyval.commentsAndStmt) = context->buildWhenStmt((yyloc), nullptr, blockOrDo);
  }
#line 8267 "bison-chpl-lib.cpp"
    break;

  case 311: /* manager_expr: expr TAS var_decl_type ident_def  */
#line 2072 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-3].expr), (yyvsp[-1].variableKind), (yylsp[0]), (yyvsp[0].uniqueStr));
    context->resetDeclState();
  }
#line 8276 "bison-chpl-lib.cpp"
    break;

  case 312: /* manager_expr: expr TAS ident_def  */
#line 2077 "chpl.ypp"
  {
    (yyval.expr) = context->buildManagerExpr((yyloc), (yyvsp[-2].expr), (yylsp[0]), (yyvsp[0].uniqueStr));
  }
#line 8284 "bison-chpl-lib.cpp"
    break;

  case 313: /* manager_expr: expr  */
#line 2081 "chpl.ypp"
  {
    (yyval.expr) = (yyvsp[0].expr);
  }
#line 8292 "bison-chpl-lib.cpp"
    break;

  case 314: /* manager_expr_ls: manager_expr  */
#line 2087 "chpl.ypp"
                                        { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8298 "bison-chpl-lib.cpp"
    break;

  case 315: /* manager_expr_ls: manager_expr_ls TCOMMA manager_expr  */
#line 2088 "chpl.ypp"
                                        { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8304 "bison-chpl-lib.cpp"
    break;

  case 316: /* manage_stmt: TMANAGE manager_expr_ls do_stmt  */
#line 2093 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildManageStmt((yyloc), (yyvsp[-1].exprList), (yylsp[0]), (yyvsp[0].blockOrDo));
  }
#line 8312 "bison-chpl-lib.cpp"
    break;

  case 317: /* class_decl_stmt: class_start opt_inherit TLCBR class_level_stmt_ls TRCBR  */
#line 2102 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), (yyvsp[-1].exprList), (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8321 "bison-chpl-lib.cpp"
    break;

  case 318: /* class_decl_stmt: class_start opt_inherit TLCBR error TRCBR  */
#line 2107 "chpl.ypp"
    {
      auto contents =
        context->makeList(ErroneousExpression::build(BUILDER, LOC((yylsp[-1]))));
      (yyval.commentsAndStmt) = context->buildAggregateTypeDecl((yyloc), (yyvsp[-4].typeDeclParts), (yylsp[-3]), (yyvsp[-3].exprList), (yylsp[-2]), contents, (yylsp[0]));
      context->exitScope((yyvsp[-4].typeDeclParts).tag, (yyvsp[-4].typeDeclParts).name);
    }
#line 8332 "bison-chpl-lib.cpp"
    break;

  case 319: /* class_start: class_tag ident_def  */
#line 2118 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-1]), (yyvsp[0].uniqueStr), (yyvsp[-1].astTag));
  }
#line 8340 "bison-chpl-lib.cpp"
    break;

  case 320: /* class_tag: TCLASS  */
#line 2124 "chpl.ypp"
           { (yyval.astTag) = asttags::Class; }
#line 8346 "bison-chpl-lib.cpp"
    break;

  case 321: /* class_tag: TRECORD  */
#line 2125 "chpl.ypp"
           { (yyval.astTag) = asttags::Record; }
#line 8352 "bison-chpl-lib.cpp"
    break;

  case 322: /* class_tag: TUNION  */
#line 2126 "chpl.ypp"
           { (yyval.astTag) = asttags::Union; }
#line 8358 "bison-chpl-lib.cpp"
    break;

  case 323: /* opt_inherit: %empty  */
#line 2130 "chpl.ypp"
                  { (yyval.exprList) = nullptr; }
#line 8364 "bison-chpl-lib.cpp"
    break;

  case 324: /* opt_inherit: TCOLON expr_ls  */
#line 2131 "chpl.ypp"
                  { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8370 "bison-chpl-lib.cpp"
    break;

  case 325: /* class_level_stmt_ls: %empty  */
#line 2135 "chpl.ypp"
  {
    /* nothing */
    (yyval.exprList) = context->makeList();
  }
#line 8379 "bison-chpl-lib.cpp"
    break;

  case 326: /* class_level_stmt_ls: class_level_stmt_ls class_level_stmt  */
#line 2140 "chpl.ypp"
  {
    context->appendList((yyvsp[-1].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8387 "bison-chpl-lib.cpp"
    break;

  case 327: /* class_level_stmt_ls: class_level_stmt_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2144 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 8395 "bison-chpl-lib.cpp"
    break;

  case 328: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls class_level_stmt  */
#line 2148 "chpl.ypp"
  {
    context->appendList((yyvsp[-2].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8403 "bison-chpl-lib.cpp"
    break;

  case 329: /* class_level_stmt_ls: class_level_stmt_ls pragma_ls attribute_decl_stmt_ls class_level_stmt  */
#line 2152 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), context->buildPragmaStmt((yylsp[0]), (yyvsp[0].commentsAndStmt)));
  }
#line 8411 "bison-chpl-lib.cpp"
    break;

  case 330: /* enum_decl_stmt: enum_header_lcbr enum_ls TRCBR  */
#line 2159 "chpl.ypp"
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
#line 8432 "bison-chpl-lib.cpp"
    break;

  case 331: /* enum_decl_stmt: enum_header_lcbr error TRCBR  */
#line 2176 "chpl.ypp"
    {
      TypeDeclParts parts = (yyvsp[-2].typeDeclParts);
      auto err = ErroneousExpression::build(BUILDER, LOC((yylsp[-1])));
      CommentsAndStmt cs = {parts.comments, err.release()};
      (yyval.commentsAndStmt) = cs;
      context->exitScope(asttags::Enum, parts.name);
      context->resetDeclState();
      context->clearComments();
    }
#line 8446 "bison-chpl-lib.cpp"
    break;

  case 332: /* enum_header_lcbr: TENUM ident_def TLCBR  */
#line 2189 "chpl.ypp"
  {
    (yyval.typeDeclParts) = context->enterScopeAndBuildTypeDeclParts((yylsp[-2]), (yyvsp[-1].uniqueStr), asttags::Enum);
  }
#line 8454 "bison-chpl-lib.cpp"
    break;

  case 333: /* enum_ls: enum_item  */
#line 2196 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8463 "bison-chpl-lib.cpp"
    break;

  case 334: /* enum_ls: enum_ls TCOMMA  */
#line 2201 "chpl.ypp"
  {
    (yyval.exprList) = (yyvsp[-1].exprList);
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8473 "bison-chpl-lib.cpp"
    break;

  case 335: /* $@5: %empty  */
#line 2207 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8482 "bison-chpl-lib.cpp"
    break;

  case 336: /* enum_ls: enum_ls TCOMMA $@5 enum_item  */
#line 2212 "chpl.ypp"
  {
    context->appendList((yyvsp[-3].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8491 "bison-chpl-lib.cpp"
    break;

  case 337: /* enum_ls: attribute_decl_stmt_ls enum_item  */
#line 2217 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8500 "bison-chpl-lib.cpp"
    break;

  case 338: /* $@6: %empty  */
#line 2222 "chpl.ypp"
  {
    context->clearCommentsBefore((yylsp[0]));
    context->resetAttributeGroupPartsState();
  }
#line 8509 "bison-chpl-lib.cpp"
    break;

  case 339: /* enum_ls: enum_ls TCOMMA $@6 attribute_decl_stmt_ls enum_item  */
#line 2227 "chpl.ypp"
  {
    context->appendList((yyvsp[-4].exprList), (yyvsp[0].commentsAndStmt));
    context->resetAttributeGroupPartsState();
  }
#line 8518 "bison-chpl-lib.cpp"
    break;

  case 340: /* enum_item: ident_def  */
#line 2237 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[0].uniqueStr));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
    }
#line 8529 "bison-chpl-lib.cpp"
    break;

  case 341: /* enum_item: ident_def TASSIGN expr  */
#line 2244 "chpl.ypp"
    {
      auto decl = EnumElement::build(BUILDER, LOC((yyloc)),
                                     context->buildAttributeGroup((yyloc)),
                                     (yyvsp[-2].uniqueStr),
                                     toOwned((yyvsp[0].expr)));
      (yyval.commentsAndStmt) = STMT((yyloc), decl.release());
      context->clearCommentsBefore((yylsp[0]));
    }
#line 8542 "bison-chpl-lib.cpp"
    break;

  case 342: /* lambda_decl_start: TLAMBDA  */
#line 2256 "chpl.ypp"
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
#line 8558 "bison-chpl-lib.cpp"
    break;

  case 343: /* $@7: %empty  */
#line 2273 "chpl.ypp"
    {
      context->clearComments();
      context->resetDeclState();
      context->enterScope(asttags::Function, STR("lambda"));
    }
#line 8568 "bison-chpl-lib.cpp"
    break;

  case 344: /* lambda_decl_expr: lambda_decl_start req_formal_ls opt_ret_tag opt_type opt_throws_error opt_lifetime_where $@7 function_body_stmt  */
#line 2279 "chpl.ypp"
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
#line 8590 "bison-chpl-lib.cpp"
    break;

  case 346: /* linkage_spec: linkage_spec_empty  */
#line 2302 "chpl.ypp"
                     { (yyval.functionParts) = context->makeFunctionParts(false, false); }
#line 8596 "bison-chpl-lib.cpp"
    break;

  case 347: /* linkage_spec: TINLINE  */
#line 2303 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(true, false); }
#line 8603 "bison-chpl-lib.cpp"
    break;

  case 348: /* linkage_spec: TOVERRIDE  */
#line 2305 "chpl.ypp"
                     { context->noteDeclStartLoc((yylsp[0]));
                       (yyval.functionParts) = context->makeFunctionParts(false, true); }
#line 8610 "bison-chpl-lib.cpp"
    break;

  case 349: /* opt_fn_type_formal_ls: %empty  */
#line 2310 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList(); }
#line 8616 "bison-chpl-lib.cpp"
    break;

  case 350: /* opt_fn_type_formal_ls: fn_type_formal_ls  */
#line 2311 "chpl.ypp"
                                          { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8622 "bison-chpl-lib.cpp"
    break;

  case 351: /* fn_type_formal_ls: fn_type_formal  */
#line 2315 "chpl.ypp"
                                          { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8628 "bison-chpl-lib.cpp"
    break;

  case 352: /* fn_type_formal_ls: fn_type_formal_ls TCOMMA fn_type_formal  */
#line 2316 "chpl.ypp"
                                          { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8634 "bison-chpl-lib.cpp"
    break;

  case 353: /* fn_type_formal: named_formal  */
#line 2321 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 8640 "bison-chpl-lib.cpp"
    break;

  case 354: /* fn_type_formal: required_intent_tag TCOLON formal_type  */
#line 2324 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yylsp[-2]), (yyvsp[-2].intentTag), (yyvsp[0].expr)); }
#line 8646 "bison-chpl-lib.cpp"
    break;

  case 355: /* fn_type_formal: formal_type  */
#line 2326 "chpl.ypp"
  { (yyval.expr) = context->buildAnonFormal((yyloc), (yyvsp[0].expr)); }
#line 8652 "bison-chpl-lib.cpp"
    break;

  case 356: /* opt_fn_type_ret_type: %empty  */
#line 2330 "chpl.ypp"
                                  { (yyval.expr) = nullptr; }
#line 8658 "bison-chpl-lib.cpp"
    break;

  case 357: /* opt_fn_type_ret_type: TCOLON formal_or_ret_type_expr  */
#line 2331 "chpl.ypp"
                                  { (yyval.expr) = (yyvsp[0].expr); }
#line 8664 "bison-chpl-lib.cpp"
    break;

  case 358: /* fn_type: TPROCLP opt_fn_type_formal_ls TRP opt_ret_tag opt_fn_type_ret_type opt_throws_error  */
#line 2344 "chpl.ypp"
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
#line 8683 "bison-chpl-lib.cpp"
    break;

  case 359: /* $@8: %empty  */
#line 2362 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8693 "bison-chpl-lib.cpp"
    break;

  case 360: /* fn_expr: fn_type $@8 block_stmt_body  */
#line 2368 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-2].functionParts);
    fp.isBodyNonBlockExpression = false;
    fp.body = (yyvsp[0].exprList);
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8706 "bison-chpl-lib.cpp"
    break;

  case 361: /* $@9: %empty  */
#line 2377 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();
    context->enterScope(asttags::Function, STR("proc"));
  }
#line 8716 "bison-chpl-lib.cpp"
    break;

  case 362: /* fn_expr: fn_type TALIAS $@9 expr  */
#line 2383 "chpl.ypp"
  {
    FunctionParts fp = (yyvsp[-3].functionParts);
    fp.isBodyNonBlockExpression = true;
    fp.body = context->makeList((yyvsp[0].expr));
    context->clearComments();
    context->exitScope(asttags::Function, STR("proc"));
    (yyval.functionParts) = fp;
  }
#line 8729 "bison-chpl-lib.cpp"
    break;

  case 363: /* fn_decl_stmt_complete: fn_decl_stmt  */
#line 2395 "chpl.ypp"
    {
      (yyval.commentsAndStmt) = context->buildRegularFunctionDecl((yyloc), (yyvsp[0].functionParts));
    }
#line 8737 "bison-chpl-lib.cpp"
    break;

  case 364: /* $@10: %empty  */
#line 2404 "chpl.ypp"
  {
    context->clearComments();
    context->resetDeclState();

    // May never have been built if there was a syntax error.
    if ((yyvsp[-4].functionParts).errorExpr == nullptr) {
      context->enterScope(asttags::Function, (yyvsp[-4].functionParts).name->name());
    }
  }
#line 8751 "bison-chpl-lib.cpp"
    break;

  case 365: /* fn_decl_stmt: fn_decl_stmt_inner opt_ret_tag opt_ret_type opt_throws_error opt_lifetime_where $@10 opt_function_body_stmt  */
#line 2414 "chpl.ypp"
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
#line 8774 "bison-chpl-lib.cpp"
    break;

  case 366: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_ident opt_formal_ls  */
#line 2436 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8786 "bison-chpl-lib.cpp"
    break;

  case 367: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag assignop_ident opt_formal_ls  */
#line 2444 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.thisIntent = (yyvsp[-2].intentTag);
      fp.name = context->buildIdent((yylsp[-1]), (yyvsp[-1].uniqueStr));
      fp.formals = (yyvsp[0].exprList);
      (yyval.functionParts) = fp;
    }
#line 8798 "bison-chpl-lib.cpp"
    break;

  case 368: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT fn_ident opt_formal_ls  */
#line 2452 "chpl.ypp"
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
#line 8813 "bison-chpl-lib.cpp"
    break;

  case 369: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag fn_decl_receiver_expr TDOT assignop_ident opt_formal_ls  */
#line 2463 "chpl.ypp"
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
#line 8828 "bison-chpl-lib.cpp"
    break;

  case 370: /* fn_decl_stmt_inner: fn_decl_stmt_start opt_this_intent_tag error opt_formal_ls  */
#line 2474 "chpl.ypp"
    {
      FunctionParts fp = (yyvsp[-3].functionParts);
      fp.errorExpr = ErroneousExpression::build(BUILDER, LOC((yyloc))).release();
      (yyval.functionParts) = fp;
    }
#line 8838 "bison-chpl-lib.cpp"
    break;

  case 371: /* fn_decl_stmt_start: linkage_spec proc_iter_or_op  */
#line 2483 "chpl.ypp"
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
#line 8854 "bison-chpl-lib.cpp"
    break;

  case 373: /* fn_decl_receiver_expr: TLP expr TRP  */
#line 2498 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[-1].expr); }
#line 8860 "bison-chpl-lib.cpp"
    break;

  case 376: /* fn_ident: ident_def TBANG  */
#line 2505 "chpl.ypp"
  {
    std::string s = (yyvsp[-1].uniqueStr).c_str();
    s += "!";
    (yyval.uniqueStr) = STR(s.c_str());
  }
#line 8870 "bison-chpl-lib.cpp"
    break;

  case 416: /* formal_var_arg_expr: TDOTDOTDOT  */
#line 2562 "chpl.ypp"
                         { (yyval.expr) = nullptr; }
#line 8876 "bison-chpl-lib.cpp"
    break;

  case 417: /* formal_var_arg_expr: TDOTDOTDOT expr  */
#line 2563 "chpl.ypp"
                         { (yyval.expr) = (yyvsp[0].expr); }
#line 8882 "bison-chpl-lib.cpp"
    break;

  case 418: /* opt_formal_ls: %empty  */
#line 2567 "chpl.ypp"
                     { (yyval.exprList) = context->parenlessMarker; }
#line 8888 "bison-chpl-lib.cpp"
    break;

  case 419: /* opt_formal_ls: TLP formal_ls TRP  */
#line 2568 "chpl.ypp"
                     { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8894 "bison-chpl-lib.cpp"
    break;

  case 420: /* req_formal_ls: TLP TRP  */
#line 2572 "chpl.ypp"
                            { (yyval.exprList) = context->makeList(); }
#line 8900 "bison-chpl-lib.cpp"
    break;

  case 421: /* req_formal_ls: TLP formal_ls_inner TRP  */
#line 2573 "chpl.ypp"
                            { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 8906 "bison-chpl-lib.cpp"
    break;

  case 422: /* formal_ls_inner: formal  */
#line 2577 "chpl.ypp"
                                 { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 8912 "bison-chpl-lib.cpp"
    break;

  case 423: /* formal_ls_inner: formal_ls_inner TCOMMA formal  */
#line 2578 "chpl.ypp"
                                 { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 8918 "bison-chpl-lib.cpp"
    break;

  case 424: /* formal_ls: %empty  */
#line 2582 "chpl.ypp"
                           { (yyval.exprList) = context->makeList(); }
#line 8924 "bison-chpl-lib.cpp"
    break;

  case 425: /* formal_ls: formal_ls_inner  */
#line 2583 "chpl.ypp"
                           { (yyval.exprList) = (yyvsp[0].exprList); }
#line 8930 "bison-chpl-lib.cpp"
    break;

  case 429: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2597 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8938 "bison-chpl-lib.cpp"
    break;

  case 430: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type opt_init_expr  */
#line 2602 "chpl.ypp"
  {
    (yyval.expr) = context->buildFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8946 "bison-chpl-lib.cpp"
    break;

  case 431: /* named_formal: opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2607 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8954 "bison-chpl-lib.cpp"
    break;

  case 432: /* named_formal: pragma_ls opt_formal_intent_tag formal_ident_def opt_colon_formal_type formal_var_arg_expr  */
#line 2612 "chpl.ypp"
  {
    (yyval.expr) = context->buildVarArgFormal((yyloc), (yyvsp[-3].intentTag), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr), (yyvsp[0].expr), true);
  }
#line 8962 "bison-chpl-lib.cpp"
    break;

  case 433: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type opt_init_expr  */
#line 2617 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleFormal((yyloc), (yyvsp[-5].intentTag), (yyvsp[-3].exprList), (yyvsp[-1].expr), (yyvsp[0].expr));
  }
#line 8970 "bison-chpl-lib.cpp"
    break;

  case 434: /* named_formal: opt_formal_intent_tag TLP tuple_var_decl_stmt_inner_ls TRP opt_colon_formal_type formal_var_arg_expr  */
#line 2622 "chpl.ypp"
  {
    (yyval.expr) = context->syntax((yyloc), "variable-length argument may not be grouped in a tuple.");
  }
#line 8978 "bison-chpl-lib.cpp"
    break;

  case 435: /* opt_formal_intent_tag: %empty  */
#line 2628 "chpl.ypp"
             {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = Formal::DEFAULT_INTENT;
  }
#line 8987 "bison-chpl-lib.cpp"
    break;

  case 436: /* opt_formal_intent_tag: required_intent_tag  */
#line 2633 "chpl.ypp"
  {
    context->noteIsBuildingFormal(true);
    (yyval.intentTag) = (yyvsp[0].intentTag);
  }
#line 8996 "bison-chpl-lib.cpp"
    break;

  case 437: /* required_intent_tag: TIN  */
#line 2640 "chpl.ypp"
              { (yyval.intentTag) = Formal::IN; }
#line 9002 "bison-chpl-lib.cpp"
    break;

  case 438: /* required_intent_tag: TINOUT  */
#line 2641 "chpl.ypp"
              { (yyval.intentTag) = Formal::INOUT; }
#line 9008 "bison-chpl-lib.cpp"
    break;

  case 439: /* required_intent_tag: TOUT  */
#line 2642 "chpl.ypp"
              { (yyval.intentTag) = Formal::OUT; }
#line 9014 "bison-chpl-lib.cpp"
    break;

  case 440: /* required_intent_tag: TCONST TIN  */
#line 2643 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_IN; }
#line 9020 "bison-chpl-lib.cpp"
    break;

  case 441: /* required_intent_tag: TCONST TREF  */
#line 2644 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST_REF; }
#line 9026 "bison-chpl-lib.cpp"
    break;

  case 442: /* required_intent_tag: TCONST  */
#line 2645 "chpl.ypp"
              { (yyval.intentTag) = Formal::CONST; }
#line 9032 "bison-chpl-lib.cpp"
    break;

  case 443: /* required_intent_tag: TPARAM  */
#line 2646 "chpl.ypp"
              { (yyval.intentTag) = Formal::PARAM; }
#line 9038 "bison-chpl-lib.cpp"
    break;

  case 444: /* required_intent_tag: TREF  */
#line 2647 "chpl.ypp"
              { (yyval.intentTag) = Formal::REF; }
#line 9044 "bison-chpl-lib.cpp"
    break;

  case 445: /* required_intent_tag: TTYPE  */
#line 2648 "chpl.ypp"
              { (yyval.intentTag) = Formal::TYPE; }
#line 9050 "bison-chpl-lib.cpp"
    break;

  case 446: /* opt_this_intent_tag: %empty  */
#line 2652 "chpl.ypp"
                { (yyval.intentTag) = Formal::DEFAULT_INTENT; }
#line 9056 "bison-chpl-lib.cpp"
    break;

  case 447: /* opt_this_intent_tag: TPARAM  */
#line 2653 "chpl.ypp"
                { (yyval.intentTag) = Formal::PARAM; }
#line 9062 "bison-chpl-lib.cpp"
    break;

  case 448: /* opt_this_intent_tag: TREF  */
#line 2654 "chpl.ypp"
                { (yyval.intentTag) = Formal::REF; }
#line 9068 "bison-chpl-lib.cpp"
    break;

  case 449: /* opt_this_intent_tag: TCONST TREF  */
#line 2655 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST_REF; }
#line 9074 "bison-chpl-lib.cpp"
    break;

  case 450: /* opt_this_intent_tag: TCONST  */
#line 2656 "chpl.ypp"
                { (yyval.intentTag) = Formal::CONST; }
#line 9080 "bison-chpl-lib.cpp"
    break;

  case 451: /* opt_this_intent_tag: TTYPE  */
#line 2657 "chpl.ypp"
                { (yyval.intentTag) = Formal::TYPE; }
#line 9086 "bison-chpl-lib.cpp"
    break;

  case 452: /* proc_iter_or_op: TPROC  */
#line 2661 "chpl.ypp"
            { (yyval.functionKind) = Function::PROC; }
#line 9092 "bison-chpl-lib.cpp"
    break;

  case 453: /* proc_iter_or_op: TITER  */
#line 2662 "chpl.ypp"
            { (yyval.functionKind) = Function::ITER; }
#line 9098 "bison-chpl-lib.cpp"
    break;

  case 454: /* proc_iter_or_op: TOPERATOR  */
#line 2663 "chpl.ypp"
            { (yyval.functionKind) = Function::OPERATOR; }
#line 9104 "bison-chpl-lib.cpp"
    break;

  case 455: /* opt_ret_tag: %empty  */
#line 2667 "chpl.ypp"
                    { (yyval.returnTag) = Function::DEFAULT_RETURN_INTENT; }
#line 9110 "bison-chpl-lib.cpp"
    break;

  case 456: /* opt_ret_tag: TOUT  */
#line 2668 "chpl.ypp"
                    { (yyval.returnTag) = Function::OUT; }
#line 9116 "bison-chpl-lib.cpp"
    break;

  case 457: /* opt_ret_tag: TCONST  */
#line 2669 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST; }
#line 9122 "bison-chpl-lib.cpp"
    break;

  case 458: /* opt_ret_tag: TCONST TREF  */
#line 2670 "chpl.ypp"
                    { (yyval.returnTag) = Function::CONST_REF; }
#line 9128 "bison-chpl-lib.cpp"
    break;

  case 459: /* opt_ret_tag: TREF  */
#line 2671 "chpl.ypp"
                    { (yyval.returnTag) = Function::REF; }
#line 9134 "bison-chpl-lib.cpp"
    break;

  case 460: /* opt_ret_tag: TPARAM  */
#line 2672 "chpl.ypp"
                    { (yyval.returnTag) = Function::PARAM; }
#line 9140 "bison-chpl-lib.cpp"
    break;

  case 461: /* opt_ret_tag: TTYPE  */
#line 2673 "chpl.ypp"
                    { (yyval.returnTag) = Function::TYPE; }
#line 9146 "bison-chpl-lib.cpp"
    break;

  case 462: /* opt_throws_error: %empty  */
#line 2677 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_DEFAULT; }
#line 9152 "bison-chpl-lib.cpp"
    break;

  case 463: /* opt_throws_error: TTHROWS  */
#line 2678 "chpl.ypp"
                          { (yyval.throwsTag) = ThrowsTag_THROWS; }
#line 9158 "bison-chpl-lib.cpp"
    break;

  case 464: /* opt_function_body_stmt: TSEMI  */
#line 2681 "chpl.ypp"
                      { context->clearComments(); (yyval.exprList) = nullptr; }
#line 9164 "bison-chpl-lib.cpp"
    break;

  case 465: /* opt_function_body_stmt: function_body_stmt  */
#line 2682 "chpl.ypp"
                      { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9170 "bison-chpl-lib.cpp"
    break;

  case 466: /* function_body_stmt: block_stmt_body  */
#line 2686 "chpl.ypp"
                    { (yyval.exprList) = (yyvsp[0].exprList); }
#line 9176 "bison-chpl-lib.cpp"
    break;

  case 467: /* function_body_stmt: TDO toplevel_stmt  */
#line 2687 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt)); }
#line 9182 "bison-chpl-lib.cpp"
    break;

  case 468: /* function_body_stmt: return_stmt  */
#line 2688 "chpl.ypp"
                    { (yyval.exprList) = context->buildSingleStmtRoutineBody((yyvsp[0].commentsAndStmt), &((yylsp[0]))); }
#line 9188 "bison-chpl-lib.cpp"
    break;

  case 469: /* query_expr: TQUERIEDIDENT  */
#line 2692 "chpl.ypp"
                  { (yyval.expr) = context->buildTypeQuery((yyloc), (yyvsp[0].uniqueStr)); }
#line 9194 "bison-chpl-lib.cpp"
    break;

  case 470: /* opt_lifetime_where: %empty  */
#line 2697 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, nullptr); }
#line 9200 "bison-chpl-lib.cpp"
    break;

  case 471: /* opt_lifetime_where: TWHERE expr  */
#line 2699 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), nullptr); }
#line 9206 "bison-chpl-lib.cpp"
    break;

  case 472: /* opt_lifetime_where: TLIFETIME lifetime_components_expr  */
#line 2701 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime(nullptr, (yyvsp[0].exprList)); }
#line 9212 "bison-chpl-lib.cpp"
    break;

  case 473: /* opt_lifetime_where: TWHERE expr TLIFETIME lifetime_components_expr  */
#line 2703 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[-2].expr), (yyvsp[0].exprList)); }
#line 9218 "bison-chpl-lib.cpp"
    break;

  case 474: /* opt_lifetime_where: TLIFETIME lifetime_components_expr TWHERE expr  */
#line 2705 "chpl.ypp"
  { (yyval.lifetimeAndWhere) = makeWhereAndLifetime((yyvsp[0].expr), (yyvsp[-2].exprList)); }
#line 9224 "bison-chpl-lib.cpp"
    break;

  case 475: /* lifetime_components_expr: lifetime_expr  */
#line 2710 "chpl.ypp"
  { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9230 "bison-chpl-lib.cpp"
    break;

  case 476: /* lifetime_components_expr: lifetime_components_expr TCOMMA lifetime_expr  */
#line 2712 "chpl.ypp"
  { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9236 "bison-chpl-lib.cpp"
    break;

  case 477: /* lifetime_expr: lifetime_ident TASSIGN lifetime_ident  */
#line 2717 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9242 "bison-chpl-lib.cpp"
    break;

  case 478: /* lifetime_expr: lifetime_ident TLESS lifetime_ident  */
#line 2719 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9248 "bison-chpl-lib.cpp"
    break;

  case 479: /* lifetime_expr: lifetime_ident TLESSEQUAL lifetime_ident  */
#line 2721 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9254 "bison-chpl-lib.cpp"
    break;

  case 480: /* lifetime_expr: lifetime_ident TEQUAL lifetime_ident  */
#line 2723 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9260 "bison-chpl-lib.cpp"
    break;

  case 481: /* lifetime_expr: lifetime_ident TGREATER lifetime_ident  */
#line 2725 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9266 "bison-chpl-lib.cpp"
    break;

  case 482: /* lifetime_expr: lifetime_ident TGREATEREQUAL lifetime_ident  */
#line 2727 "chpl.ypp"
    { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9272 "bison-chpl-lib.cpp"
    break;

  case 483: /* lifetime_expr: TRETURN lifetime_ident  */
#line 2729 "chpl.ypp"
    { (yyval.expr) = Return::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[0].expr))).release(); }
#line 9278 "bison-chpl-lib.cpp"
    break;

  case 484: /* lifetime_ident: TIDENT  */
#line 2733 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9284 "bison-chpl-lib.cpp"
    break;

  case 485: /* lifetime_ident: TTHIS  */
#line 2734 "chpl.ypp"
         { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9290 "bison-chpl-lib.cpp"
    break;

  case 486: /* type_alias_decl_stmt: type_alias_decl_stmt_start type_alias_decl_stmt_inner_ls TSEMI  */
#line 2739 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9299 "bison-chpl-lib.cpp"
    break;

  case 487: /* type_alias_decl_stmt_start: TTYPE  */
#line 2748 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
  }
#line 9307 "bison-chpl-lib.cpp"
    break;

  case 488: /* type_alias_decl_stmt_start: TCONFIG TTYPE  */
#line 2752 "chpl.ypp"
  {
    (yyval.variableKind) = context->noteVarDeclKind(Variable::TYPE);
    context->noteIsVarDeclConfig(true);
  }
#line 9316 "bison-chpl-lib.cpp"
    break;

  case 489: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner  */
#line 2760 "chpl.ypp"
  {
    (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
  }
#line 9324 "bison-chpl-lib.cpp"
    break;

  case 490: /* type_alias_decl_stmt_inner_ls: type_alias_decl_stmt_inner_ls TCOMMA type_alias_decl_stmt_inner  */
#line 2764 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
  }
#line 9332 "bison-chpl-lib.cpp"
    break;

  case 491: /* type_alias_decl_stmt_inner: ident_def opt_init_type  */
#line 2771 "chpl.ypp"
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
#line 9354 "bison-chpl-lib.cpp"
    break;

  case 492: /* opt_init_type: %empty  */
#line 2791 "chpl.ypp"
  { (yyval.expr) = nullptr; }
#line 9360 "bison-chpl-lib.cpp"
    break;

  case 493: /* opt_init_type: TASSIGN expr  */
#line 2793 "chpl.ypp"
  { (yyval.expr) = (yyvsp[0].expr); }
#line 9366 "bison-chpl-lib.cpp"
    break;

  case 494: /* var_decl_type: TPARAM  */
#line 2797 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::PARAM); }
#line 9372 "bison-chpl-lib.cpp"
    break;

  case 495: /* var_decl_type: TCONST TREF  */
#line 2798 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST_REF); }
#line 9378 "bison-chpl-lib.cpp"
    break;

  case 496: /* var_decl_type: TREF  */
#line 2799 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::REF); }
#line 9384 "bison-chpl-lib.cpp"
    break;

  case 497: /* var_decl_type: TCONST  */
#line 2800 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::CONST); }
#line 9390 "bison-chpl-lib.cpp"
    break;

  case 498: /* var_decl_type: TVAR  */
#line 2801 "chpl.ypp"
              { (yyval.variableKind) = context->noteVarDeclKind(Variable::VAR); }
#line 9396 "bison-chpl-lib.cpp"
    break;

  case 499: /* $@11: %empty  */
#line 2806 "chpl.ypp"
  {
    // Use a mid-rule action to thread along 'isVarDeclConfig'.
    context->noteIsVarDeclConfig(true);
  }
#line 9405 "bison-chpl-lib.cpp"
    break;

  case 500: /* var_decl_stmt: TCONFIG $@11 var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2810 "chpl.ypp"
                                             {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9414 "bison-chpl-lib.cpp"
    break;

  case 501: /* var_decl_stmt: var_decl_type var_decl_stmt_inner_ls TSEMI  */
#line 2815 "chpl.ypp"
  {
    (yyval.commentsAndStmt) = context->buildVarOrMultiDeclStmt((yyloc), (yyvsp[-1].exprList));
    context->resetDeclState();
  }
#line 9423 "bison-chpl-lib.cpp"
    break;

  case 502: /* var_decl_stmt_inner_ls: var_decl_stmt_inner  */
#line 2823 "chpl.ypp"
    {
      (yyval.exprList) = context->makeList((yyvsp[0].commentsAndStmt));
    }
#line 9431 "bison-chpl-lib.cpp"
    break;

  case 503: /* var_decl_stmt_inner_ls: var_decl_stmt_inner_ls TCOMMA var_decl_stmt_inner  */
#line 2827 "chpl.ypp"
    {
      (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].commentsAndStmt));
    }
#line 9439 "bison-chpl-lib.cpp"
    break;

  case 504: /* var_decl_stmt_inner: ident_def opt_type opt_init_expr  */
#line 2834 "chpl.ypp"
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
#line 9461 "bison-chpl-lib.cpp"
    break;

  case 505: /* var_decl_stmt_inner: TLP tuple_var_decl_stmt_inner_ls TRP opt_type opt_init_expr  */
#line 2852 "chpl.ypp"
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
#line 9481 "bison-chpl-lib.cpp"
    break;

  case 506: /* tuple_var_decl_component: TUNDERSCORE  */
#line 2871 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9489 "bison-chpl-lib.cpp"
    break;

  case 507: /* tuple_var_decl_component: ident_def  */
#line 2875 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[0].uniqueStr));
  }
#line 9497 "bison-chpl-lib.cpp"
    break;

  case 508: /* tuple_var_decl_component: TLP tuple_var_decl_stmt_inner_ls TRP  */
#line 2879 "chpl.ypp"
  {
    (yyval.expr) = context->buildTupleComponent((yyloc), (yyvsp[-1].exprList));
  }
#line 9505 "bison-chpl-lib.cpp"
    break;

  case 509: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_component  */
#line 2886 "chpl.ypp"
    { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9511 "bison-chpl-lib.cpp"
    break;

  case 510: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA  */
#line 2888 "chpl.ypp"
    { (yyval.exprList) = (yyvsp[-1].exprList); }
#line 9517 "bison-chpl-lib.cpp"
    break;

  case 511: /* tuple_var_decl_stmt_inner_ls: tuple_var_decl_stmt_inner_ls TCOMMA tuple_var_decl_component  */
#line 2890 "chpl.ypp"
    { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9523 "bison-chpl-lib.cpp"
    break;

  case 512: /* opt_init_expr: %empty  */
#line 2896 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9529 "bison-chpl-lib.cpp"
    break;

  case 513: /* opt_init_expr: TASSIGN TNOINIT  */
#line 2897 "chpl.ypp"
                        { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9535 "bison-chpl-lib.cpp"
    break;

  case 514: /* opt_init_expr: TASSIGN opt_try_expr  */
#line 2898 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9541 "bison-chpl-lib.cpp"
    break;

  case 515: /* formal_or_ret_type_expr: expr  */
#line 2902 "chpl.ypp"
                        { (yyval.expr) = context->sanitizeArrayType((yyloc), (yyvsp[0].expr)); }
#line 9547 "bison-chpl-lib.cpp"
    break;

  case 516: /* ret_type: formal_or_ret_type_expr  */
#line 2906 "chpl.ypp"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 9553 "bison-chpl-lib.cpp"
    break;

  case 517: /* ret_type: reserved_type_ident_use  */
#line 2907 "chpl.ypp"
                          { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9559 "bison-chpl-lib.cpp"
    break;

  case 518: /* ret_type: error  */
#line 2908 "chpl.ypp"
                          { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9565 "bison-chpl-lib.cpp"
    break;

  case 519: /* colon_ret_type: TCOLON ret_type  */
#line 2912 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 9571 "bison-chpl-lib.cpp"
    break;

  case 520: /* colon_ret_type: error  */
#line 2913 "chpl.ypp"
                  { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9577 "bison-chpl-lib.cpp"
    break;

  case 521: /* opt_ret_type: %empty  */
#line 2917 "chpl.ypp"
                    { (yyval.expr) = nullptr; }
#line 9583 "bison-chpl-lib.cpp"
    break;

  case 523: /* opt_type: %empty  */
#line 2922 "chpl.ypp"
                                 { (yyval.expr) = nullptr; }
#line 9589 "bison-chpl-lib.cpp"
    break;

  case 524: /* opt_type: TCOLON expr  */
#line 2923 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9595 "bison-chpl-lib.cpp"
    break;

  case 525: /* opt_type: TCOLON reserved_type_ident_use  */
#line 2924 "chpl.ypp"
                                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9601 "bison-chpl-lib.cpp"
    break;

  case 526: /* opt_type: error  */
#line 2925 "chpl.ypp"
                                 { (yyval.expr) = ErroneousExpression::build(BUILDER, LOC((yylsp[0]))).release(); }
#line 9607 "bison-chpl-lib.cpp"
    break;

  case 527: /* formal_type: formal_or_ret_type_expr  */
#line 2929 "chpl.ypp"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 9613 "bison-chpl-lib.cpp"
    break;

  case 528: /* formal_type: reserved_type_ident_use  */
#line 2930 "chpl.ypp"
                            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9619 "bison-chpl-lib.cpp"
    break;

  case 529: /* colon_formal_type: TCOLON formal_type  */
#line 2934 "chpl.ypp"
                                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9625 "bison-chpl-lib.cpp"
    break;

  case 530: /* opt_colon_formal_type: %empty  */
#line 2938 "chpl.ypp"
                        { (yyval.expr) = nullptr; }
#line 9631 "bison-chpl-lib.cpp"
    break;

  case 531: /* opt_colon_formal_type: colon_formal_type  */
#line 2939 "chpl.ypp"
                        { (yyval.expr) = (yyvsp[0].expr); }
#line 9637 "bison-chpl-lib.cpp"
    break;

  case 532: /* expr_ls: expr  */
#line 2945 "chpl.ypp"
                             { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 9643 "bison-chpl-lib.cpp"
    break;

  case 533: /* expr_ls: expr_ls TCOMMA expr  */
#line 2946 "chpl.ypp"
                             { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 9649 "bison-chpl-lib.cpp"
    break;

  case 534: /* tuple_component: TUNDERSCORE  */
#line 2950 "chpl.ypp"
                { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9655 "bison-chpl-lib.cpp"
    break;

  case 535: /* tuple_component: opt_try_expr  */
#line 2951 "chpl.ypp"
                { (yyval.expr) = (yyvsp[0].expr); }
#line 9661 "bison-chpl-lib.cpp"
    break;

  case 536: /* tuple_expr_ls: tuple_component TCOMMA tuple_component  */
#line 2956 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList(context->makeList((yyvsp[-2].expr)), (yyvsp[0].expr));
  }
#line 9669 "bison-chpl-lib.cpp"
    break;

  case 537: /* tuple_expr_ls: tuple_expr_ls TCOMMA tuple_component  */
#line 2960 "chpl.ypp"
  {
    (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 9677 "bison-chpl-lib.cpp"
    break;

  case 538: /* opt_actual_ls: %empty  */
#line 2966 "chpl.ypp"
             { (yyval.maybeNamedActualList) = new MaybeNamedActualList(); }
#line 9683 "bison-chpl-lib.cpp"
    break;

  case 539: /* opt_actual_ls: actual_ls  */
#line 2967 "chpl.ypp"
             { (yyval.maybeNamedActualList) = (yyvsp[0].maybeNamedActualList); }
#line 9689 "bison-chpl-lib.cpp"
    break;

  case 540: /* actual_ls: actual_expr  */
#line 2972 "chpl.ypp"
    { MaybeNamedActualList* lst = new MaybeNamedActualList();
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9698 "bison-chpl-lib.cpp"
    break;

  case 541: /* actual_ls: actual_ls TCOMMA actual_expr  */
#line 2977 "chpl.ypp"
    {
      MaybeNamedActualList* lst = (yyvsp[-2].maybeNamedActualList);
      lst->push_back((yyvsp[0].maybeNamedActual));
      (yyval.maybeNamedActualList) = lst;
    }
#line 9708 "bison-chpl-lib.cpp"
    break;

  case 542: /* actual_expr: ident_use TASSIGN opt_try_expr  */
#line 2985 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr), (yyvsp[-2].uniqueStr)); }
#line 9714 "bison-chpl-lib.cpp"
    break;

  case 543: /* actual_expr: opt_try_expr  */
#line 2986 "chpl.ypp"
                                 { (yyval.maybeNamedActual) = makeMaybeNamedActual((yyvsp[0].expr)); }
#line 9720 "bison-chpl-lib.cpp"
    break;

  case 544: /* ident_expr: ident_use  */
#line 2990 "chpl.ypp"
                 { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 9726 "bison-chpl-lib.cpp"
    break;

  case 545: /* ident_expr: scalar_type  */
#line 2991 "chpl.ypp"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 9732 "bison-chpl-lib.cpp"
    break;

  case 551: /* sub_type_level_expr: TSINGLE expr  */
#line 3004 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9738 "bison-chpl-lib.cpp"
    break;

  case 552: /* sub_type_level_expr: TINDEX TLP opt_actual_ls TRP  */
#line 3006 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9744 "bison-chpl-lib.cpp"
    break;

  case 553: /* sub_type_level_expr: TDOMAIN TLP opt_actual_ls TRP  */
#line 3008 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9750 "bison-chpl-lib.cpp"
    break;

  case 554: /* sub_type_level_expr: TSUBDOMAIN TLP opt_actual_ls TRP  */
#line 3010 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActualList)); }
#line 9756 "bison-chpl-lib.cpp"
    break;

  case 555: /* sub_type_level_expr: TSPARSE TSUBDOMAIN TLP actual_expr TRP  */
#line 3012 "chpl.ypp"
  {
    auto locInner = context->makeSpannedLocation((yylsp[-3]), (yylsp[0]));
    auto inner = context->buildTypeConstructor(locInner, (yyvsp[-3].uniqueStr), (yyvsp[-1].maybeNamedActual));
    (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-4].uniqueStr), inner);
  }
#line 9766 "bison-chpl-lib.cpp"
    break;

  case 556: /* sub_type_level_expr: TATOMIC expr  */
#line 3018 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9772 "bison-chpl-lib.cpp"
    break;

  case 557: /* sub_type_level_expr: TSYNC expr  */
#line 3020 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9778 "bison-chpl-lib.cpp"
    break;

  case 558: /* sub_type_level_expr: TOWNED  */
#line 3023 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9784 "bison-chpl-lib.cpp"
    break;

  case 559: /* sub_type_level_expr: TOWNED expr  */
#line 3025 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9790 "bison-chpl-lib.cpp"
    break;

  case 560: /* sub_type_level_expr: TUNMANAGED  */
#line 3027 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9796 "bison-chpl-lib.cpp"
    break;

  case 561: /* sub_type_level_expr: TUNMANAGED expr  */
#line 3029 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9802 "bison-chpl-lib.cpp"
    break;

  case 562: /* sub_type_level_expr: TSHARED  */
#line 3031 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9808 "bison-chpl-lib.cpp"
    break;

  case 563: /* sub_type_level_expr: TSHARED expr  */
#line 3033 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9814 "bison-chpl-lib.cpp"
    break;

  case 564: /* sub_type_level_expr: TBORROWED  */
#line 3035 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9820 "bison-chpl-lib.cpp"
    break;

  case 565: /* sub_type_level_expr: TBORROWED expr  */
#line 3037 "chpl.ypp"
  { (yyval.expr) = context->buildTypeConstructor((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 9826 "bison-chpl-lib.cpp"
    break;

  case 566: /* sub_type_level_expr: TCLASS  */
#line 3039 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9832 "bison-chpl-lib.cpp"
    break;

  case 567: /* sub_type_level_expr: TRECORD  */
#line 3041 "chpl.ypp"
  { (yyval.expr) = Identifier::build(BUILDER, LOC((yylsp[0])), (yyvsp[0].uniqueStr)).release(); }
#line 9838 "bison-chpl-lib.cpp"
    break;

  case 568: /* for_expr: TFOR expr TIN expr TDO expr  */
#line 3046 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
    context->resetAttributeGroupPartsState();
  }
#line 9853 "bison-chpl-lib.cpp"
    break;

  case 569: /* for_expr: TFOR expr TIN zippered_iterator TDO expr  */
#line 3057 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9868 "bison-chpl-lib.cpp"
    break;

  case 570: /* for_expr: TFOR expr TDO expr  */
#line 3068 "chpl.ypp"
  {
    (yyval.expr) = For::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                    BlockStyle::IMPLICIT,
                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                    /*isExpressionLevel*/ true,
                    /*isParam*/ false,
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9882 "bison-chpl-lib.cpp"
    break;

  case 571: /* for_expr: TFOR expr TIN expr TDO TIF expr TTHEN expr  */
#line 3078 "chpl.ypp"
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
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9902 "bison-chpl-lib.cpp"
    break;

  case 572: /* for_expr: TFOR expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3094 "chpl.ypp"
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
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9922 "bison-chpl-lib.cpp"
    break;

  case 573: /* for_expr: TFOR expr TDO TIF expr TTHEN expr  */
#line 3110 "chpl.ypp"
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
                    context->buildAttributeGroup((yyloc))).release();
                    context->resetAttributeGroupPartsState();
  }
#line 9942 "bison-chpl-lib.cpp"
    break;

  case 574: /* for_expr: TFORALL expr TIN expr TDO expr  */
#line 3126 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9955 "bison-chpl-lib.cpp"
    break;

  case 575: /* for_expr: TFORALL expr TIN zippered_iterator TDO expr  */
#line 3135 "chpl.ypp"
  {
    auto index = context->buildLoopIndexDecl((yylsp[-4]), toOwned((yyvsp[-4].expr)));
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), std::move(index), toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9968 "bison-chpl-lib.cpp"
    break;

  case 576: /* for_expr: TFORALL expr TDO expr  */
#line 3144 "chpl.ypp"
  {
    (yyval.expr) = Forall::build(BUILDER, LOC((yyloc)), /*index*/ nullptr, toOwned((yyvsp[-2].expr)),
                       /*withClause*/ nullptr,
                       BlockStyle::IMPLICIT,
                       context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                       /*isExpressionLevel*/ true).release();
  }
#line 9980 "bison-chpl-lib.cpp"
    break;

  case 577: /* for_expr: TFORALL expr TIN expr TDO TIF expr TTHEN expr  */
#line 3152 "chpl.ypp"
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
#line 9998 "bison-chpl-lib.cpp"
    break;

  case 578: /* for_expr: TFORALL expr TIN zippered_iterator TDO TIF expr TTHEN expr  */
#line 3166 "chpl.ypp"
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
#line 10016 "bison-chpl-lib.cpp"
    break;

  case 579: /* for_expr: TFORALL expr TDO TIF expr TTHEN expr  */
#line 3180 "chpl.ypp"
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
#line 10034 "bison-chpl-lib.cpp"
    break;

  case 580: /* bracket_loop_expr: TLSBR TRSBR  */
#line 3197 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc));
  }
#line 10042 "bison-chpl-lib.cpp"
    break;

  case 581: /* bracket_loop_expr: TLSBR TRSBR expr  */
#line 3201 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-1]), (yyvsp[0].expr));
  }
#line 10050 "bison-chpl-lib.cpp"
    break;

  case 582: /* bracket_loop_expr: TLSBR expr_ls TRSBR expr  */
#line 3205 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-2]), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10058 "bison-chpl-lib.cpp"
    break;

  case 583: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR expr  */
#line 3209 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10066 "bison-chpl-lib.cpp"
    break;

  case 584: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR expr  */
#line 3213 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-4]), (yyvsp[-4].exprList), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10074 "bison-chpl-lib.cpp"
    break;

  case 585: /* bracket_loop_expr: TLSBR expr_ls TIN expr TRSBR TIF expr TTHEN expr  */
#line 3217 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10082 "bison-chpl-lib.cpp"
    break;

  case 586: /* bracket_loop_expr: TLSBR expr_ls TIN zippered_iterator TRSBR TIF expr TTHEN expr  */
#line 3221 "chpl.ypp"
  {
    (yyval.expr) = context->buildBracketLoopExpr((yyloc), (yylsp[-7]), (yylsp[-3]), (yyvsp[-7].exprList), (yyvsp[-5].expr), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10090 "bison-chpl-lib.cpp"
    break;

  case 587: /* cond_expr: TIF expr TTHEN expr TELSE expr  */
#line 3228 "chpl.ypp"
  {
    auto node  = Conditional::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[-2]), (yyvsp[-2].expr)),
                                    BlockStyle::IMPLICIT,
                                    context->consumeToBlock((yylsp[0]), (yyvsp[0].expr)),
                                    /*isExpressionLevel*/ true);
    (yyval.expr) = node.release();
  }
#line 10104 "bison-chpl-lib.cpp"
    break;

  case 588: /* nil_expr: TNIL  */
#line 3245 "chpl.ypp"
            { (yyval.expr) = context->buildIdent((yylsp[0]), (yyvsp[0].uniqueStr)); }
#line 10110 "bison-chpl-lib.cpp"
    break;

  case 596: /* opt_task_intent_ls: %empty  */
#line 3263 "chpl.ypp"
                                { (yyval.withClause) = nullptr; }
#line 10116 "bison-chpl-lib.cpp"
    break;

  case 597: /* opt_task_intent_ls: task_intent_clause  */
#line 3264 "chpl.ypp"
                                { (yyval.withClause) = (yyvsp[0].withClause); }
#line 10122 "bison-chpl-lib.cpp"
    break;

  case 598: /* task_intent_clause: TWITH TLP task_intent_ls TRP  */
#line 3269 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10132 "bison-chpl-lib.cpp"
    break;

  case 599: /* task_intent_ls: intent_expr  */
#line 3277 "chpl.ypp"
                                      { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10138 "bison-chpl-lib.cpp"
    break;

  case 600: /* task_intent_ls: task_intent_ls TCOMMA intent_expr  */
#line 3278 "chpl.ypp"
                                      { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10144 "bison-chpl-lib.cpp"
    break;

  case 601: /* forall_intent_clause: TWITH TLP forall_intent_ls TRP  */
#line 3283 "chpl.ypp"
  {
    auto exprs = context->consumeList((yyvsp[-1].exprList));
    auto node = WithClause::build(BUILDER, LOC((yyloc)), std::move(exprs));
    (yyval.withClause) = node.release();
  }
#line 10154 "bison-chpl-lib.cpp"
    break;

  case 602: /* forall_intent_ls: intent_expr  */
#line 3291 "chpl.ypp"
                                       { (yyval.exprList) = context->makeList((yyvsp[0].expr)); }
#line 10160 "bison-chpl-lib.cpp"
    break;

  case 603: /* forall_intent_ls: forall_intent_ls TCOMMA intent_expr  */
#line 3292 "chpl.ypp"
                                       { (yyval.exprList) = context->appendList((yyvsp[-2].exprList), (yyvsp[0].expr)); }
#line 10166 "bison-chpl-lib.cpp"
    break;

  case 604: /* intent_expr: task_var_prefix ident_expr opt_type opt_init_expr  */
#line 3297 "chpl.ypp"
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
#line 10184 "bison-chpl-lib.cpp"
    break;

  case 605: /* intent_expr: reduce_scan_op_expr TREDUCE ident_expr  */
#line 3311 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10192 "bison-chpl-lib.cpp"
    break;

  case 606: /* intent_expr: expr TREDUCE ident_expr  */
#line 3315 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduceIntent((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10200 "bison-chpl-lib.cpp"
    break;

  case 607: /* task_var_prefix: TCONST  */
#line 3321 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST;     }
#line 10206 "bison-chpl-lib.cpp"
    break;

  case 608: /* task_var_prefix: TIN  */
#line 3322 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::IN;        }
#line 10212 "bison-chpl-lib.cpp"
    break;

  case 609: /* task_var_prefix: TCONST TIN  */
#line 3323 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_IN;  }
#line 10218 "bison-chpl-lib.cpp"
    break;

  case 610: /* task_var_prefix: TREF  */
#line 3324 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::REF;       }
#line 10224 "bison-chpl-lib.cpp"
    break;

  case 611: /* task_var_prefix: TCONST TREF  */
#line 3325 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::CONST_REF; }
#line 10230 "bison-chpl-lib.cpp"
    break;

  case 612: /* task_var_prefix: TVAR  */
#line 3326 "chpl.ypp"
               { (yyval.taskIntent) = TaskVar::VAR;       }
#line 10236 "bison-chpl-lib.cpp"
    break;

  case 613: /* new_maybe_decorated: TNEW  */
#line 3331 "chpl.ypp"
    { (yyval.newManagement) = New::DEFAULT_MANAGEMENT; }
#line 10242 "bison-chpl-lib.cpp"
    break;

  case 614: /* new_maybe_decorated: TNEW TOWNED  */
#line 3333 "chpl.ypp"
    { (yyval.newManagement) = New::OWNED; }
#line 10248 "bison-chpl-lib.cpp"
    break;

  case 615: /* new_maybe_decorated: TNEW TSHARED  */
#line 3335 "chpl.ypp"
    { (yyval.newManagement) = New::SHARED; }
#line 10254 "bison-chpl-lib.cpp"
    break;

  case 616: /* new_maybe_decorated: TNEW TUNMANAGED  */
#line 3337 "chpl.ypp"
    { (yyval.newManagement) = New::UNMANAGED; }
#line 10260 "bison-chpl-lib.cpp"
    break;

  case 617: /* new_maybe_decorated: TNEW TBORROWED  */
#line 3339 "chpl.ypp"
    { (yyval.newManagement) = New::BORROWED; }
#line 10266 "bison-chpl-lib.cpp"
    break;

  case 618: /* new_expr: new_maybe_decorated expr  */
#line 3345 "chpl.ypp"
  {
    (yyval.expr) = context->buildNewExpr((yyloc), (yyvsp[-1].newManagement), (yyvsp[0].expr));
  }
#line 10274 "bison-chpl-lib.cpp"
    break;

  case 619: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3352 "chpl.ypp"
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
#line 10290 "bison-chpl-lib.cpp"
    break;

  case 620: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP  */
#line 3364 "chpl.ypp"
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
#line 10306 "bison-chpl-lib.cpp"
    break;

  case 621: /* new_expr: TNEW TOWNED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3376 "chpl.ypp"
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
#line 10323 "bison-chpl-lib.cpp"
    break;

  case 622: /* new_expr: TNEW TSHARED TLP expr TRP TLP opt_actual_ls TRP TQUESTION  */
#line 3389 "chpl.ypp"
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
#line 10341 "bison-chpl-lib.cpp"
    break;

  case 623: /* let_expr: TLET var_decl_stmt_inner_ls TIN expr  */
#line 3406 "chpl.ypp"
  {
    (yyval.expr) = context->buildLetExpr((yyloc), (yyvsp[-2].exprList), (yyvsp[0].expr));
  }
#line 10349 "bison-chpl-lib.cpp"
    break;

  case 624: /* range_literal_expr: expr TDOTDOT expr  */
#line 3413 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10358 "bison-chpl-lib.cpp"
    break;

  case 625: /* range_literal_expr: expr TDOTDOTOPENHIGH expr  */
#line 3418 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH, toOwned((yyvsp[-2].expr)),
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10367 "bison-chpl-lib.cpp"
    break;

  case 626: /* range_literal_expr: expr TDOTDOT  */
#line 3423 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT, toOwned((yyvsp[-1].expr)),
                      /*upperBound*/ nullptr).release();
  }
#line 10376 "bison-chpl-lib.cpp"
    break;

  case 627: /* range_literal_expr: TDOTDOT expr  */
#line 3428 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10386 "bison-chpl-lib.cpp"
    break;

  case 628: /* range_literal_expr: TDOTDOTOPENHIGH expr  */
#line 3434 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::OPEN_HIGH,
                      /*lowerBound*/ nullptr,
                      toOwned((yyvsp[0].expr))).release();
  }
#line 10396 "bison-chpl-lib.cpp"
    break;

  case 629: /* range_literal_expr: TDOTDOT  */
#line 3440 "chpl.ypp"
  {
    (yyval.expr) = Range::build(BUILDER, LOC((yyloc)), Range::DEFAULT,
                      /*lowerBound*/ nullptr,
                      /*upperBound*/ nullptr).release();
  }
#line 10406 "bison-chpl-lib.cpp"
    break;

  case 630: /* cast_expr: expr TCOLON expr  */
#line 3470 "chpl.ypp"
  {
    (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
  }
#line 10414 "bison-chpl-lib.cpp"
    break;

  case 631: /* tuple_expand_expr: TLP TDOTDOTDOT expr TRP  */
#line 3477 "chpl.ypp"
  {
    (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-2].uniqueStr), (yyvsp[-1].expr));
  }
#line 10422 "bison-chpl-lib.cpp"
    break;

  case 632: /* super_expr: fn_expr  */
#line 3483 "chpl.ypp"
          { (yyval.expr) = context->buildFunctionExpr((yyloc), (yyvsp[0].functionParts)); }
#line 10428 "bison-chpl-lib.cpp"
    break;

  case 636: /* expr: sub_type_level_expr TQUESTION  */
#line 3492 "chpl.ypp"
  { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10434 "bison-chpl-lib.cpp"
    break;

  case 637: /* expr: TQUESTION  */
#line 3494 "chpl.ypp"
  { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10440 "bison-chpl-lib.cpp"
    break;

  case 641: /* expr: fn_type  */
#line 3499 "chpl.ypp"
  { (yyval.expr) = context->buildFunctionType((yyloc), (yyvsp[0].functionParts)); }
#line 10446 "bison-chpl-lib.cpp"
    break;

  case 651: /* opt_expr: %empty  */
#line 3513 "chpl.ypp"
                  { (yyval.expr) = nullptr; }
#line 10452 "bison-chpl-lib.cpp"
    break;

  case 652: /* opt_expr: expr  */
#line 3514 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10458 "bison-chpl-lib.cpp"
    break;

  case 653: /* opt_try_expr: TTRY expr  */
#line 3518 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), false); }
#line 10464 "bison-chpl-lib.cpp"
    break;

  case 654: /* opt_try_expr: TTRYBANG expr  */
#line 3519 "chpl.ypp"
                  { (yyval.expr) = context->buildTryExpr((yyloc), (yyvsp[0].expr), true); }
#line 10470 "bison-chpl-lib.cpp"
    break;

  case 655: /* opt_try_expr: super_expr  */
#line 3520 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10476 "bison-chpl-lib.cpp"
    break;

  case 661: /* call_base_expr: expr TBANG  */
#line 3537 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                             STR("postfix!"),
                                                             (yyvsp[-1].expr)); }
#line 10484 "bison-chpl-lib.cpp"
    break;

  case 662: /* call_base_expr: sub_type_level_expr TQUESTION  */
#line 3540 "chpl.ypp"
                                { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[0].uniqueStr), (yyvsp[-1].expr)); }
#line 10490 "bison-chpl-lib.cpp"
    break;

  case 665: /* call_expr: call_base_expr TLP opt_actual_ls TRP  */
#line 3547 "chpl.ypp"
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
#line 10506 "bison-chpl-lib.cpp"
    break;

  case 666: /* call_expr: call_base_expr TLSBR opt_actual_ls TRSBR  */
#line 3559 "chpl.ypp"
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
#line 10522 "bison-chpl-lib.cpp"
    break;

  case 667: /* call_expr: TPRIMITIVE TLP opt_actual_ls TRP  */
#line 3571 "chpl.ypp"
    {
      (yyval.expr) = context->buildPrimCall((yyloc), (yyvsp[-1].maybeNamedActualList));
    }
#line 10530 "bison-chpl-lib.cpp"
    break;

  case 668: /* dot_expr: expr TDOT ident_use  */
#line 3578 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10536 "bison-chpl-lib.cpp"
    break;

  case 669: /* dot_expr: expr TDOT TTYPE  */
#line 3580 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10542 "bison-chpl-lib.cpp"
    break;

  case 670: /* dot_expr: expr TDOT TDOMAIN  */
#line 3582 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10548 "bison-chpl-lib.cpp"
    break;

  case 671: /* dot_expr: expr TDOT TLOCALE  */
#line 3584 "chpl.ypp"
    { (yyval.expr) = Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-2].expr)), (yyvsp[0].uniqueStr)).release(); }
#line 10554 "bison-chpl-lib.cpp"
    break;

  case 672: /* dot_expr: expr TDOT TBYTES TLP TRP  */
#line 3586 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         false).release();
    }
#line 10564 "bison-chpl-lib.cpp"
    break;

  case 673: /* dot_expr: expr TDOT TBYTES TLSBR TRSBR  */
#line 3592 "chpl.ypp"
    {
      (yyval.expr) = FnCall::build(BUILDER, LOC((yyloc)),
                         Dot::build(BUILDER, LOC((yyloc)), toOwned((yyvsp[-4].expr)), (yyvsp[-2].uniqueStr)),
                         true).release();
    }
#line 10574 "bison-chpl-lib.cpp"
    break;

  case 674: /* parenthesized_expr: TLP tuple_component TRP  */
#line 3604 "chpl.ypp"
                                    { (yyval.expr) = (yyvsp[-1].expr); }
#line 10580 "bison-chpl-lib.cpp"
    break;

  case 675: /* parenthesized_expr: TLP tuple_component TCOMMA TRP  */
#line 3606 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consume((yyvsp[-2].expr))).release();
  }
#line 10588 "bison-chpl-lib.cpp"
    break;

  case 676: /* parenthesized_expr: TLP tuple_expr_ls TRP  */
#line 3610 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10596 "bison-chpl-lib.cpp"
    break;

  case 677: /* parenthesized_expr: TLP tuple_expr_ls TCOMMA TRP  */
#line 3614 "chpl.ypp"
  {
    (yyval.expr) = Tuple::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10604 "bison-chpl-lib.cpp"
    break;

  case 678: /* bool_literal: TFALSE  */
#line 3620 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), false).release(); }
#line 10610 "bison-chpl-lib.cpp"
    break;

  case 679: /* bool_literal: TTRUE  */
#line 3621 "chpl.ypp"
         { (yyval.expr) = BoolLiteral::build(BUILDER, LOC((yyloc)), true).release(); }
#line 10616 "bison-chpl-lib.cpp"
    break;

  case 680: /* str_bytes_literal: STRINGLITERAL  */
#line 3625 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10622 "bison-chpl-lib.cpp"
    break;

  case 681: /* str_bytes_literal: BYTESLITERAL  */
#line 3626 "chpl.ypp"
                  { (yyval.expr) = (yyvsp[0].expr); }
#line 10628 "bison-chpl-lib.cpp"
    break;

  case 684: /* literal_expr: INTLITERAL  */
#line 3632 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), INTLITERAL); }
#line 10634 "bison-chpl-lib.cpp"
    break;

  case 685: /* literal_expr: REALLITERAL  */
#line 3633 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), REALLITERAL); }
#line 10640 "bison-chpl-lib.cpp"
    break;

  case 686: /* literal_expr: IMAGLITERAL  */
#line 3634 "chpl.ypp"
                 { (yyval.expr) = context->buildNumericLiteral((yyloc), (yyvsp[0].uniqueStr), IMAGLITERAL); }
#line 10646 "bison-chpl-lib.cpp"
    break;

  case 687: /* literal_expr: CSTRINGLITERAL  */
#line 3635 "chpl.ypp"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 10652 "bison-chpl-lib.cpp"
    break;

  case 688: /* literal_expr: TNONE  */
#line 3636 "chpl.ypp"
                      { (yyval.expr) = context->buildIdent((yyloc), (yyvsp[0].uniqueStr)); }
#line 10658 "bison-chpl-lib.cpp"
    break;

  case 689: /* literal_expr: TLCBR expr_ls TRCBR  */
#line 3638 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10667 "bison-chpl-lib.cpp"
    break;

  case 690: /* literal_expr: TLCBR expr_ls TCOMMA TRCBR  */
#line 3643 "chpl.ypp"
  {
    (yyval.expr) = Domain::build(BUILDER, LOC((yyloc)), true,
                       context->consumeList((yyvsp[-2].exprList))).release();
  }
#line 10676 "bison-chpl-lib.cpp"
    break;

  case 691: /* literal_expr: TLSBR expr_ls TRSBR  */
#line 3648 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList))).release();
  }
#line 10684 "bison-chpl-lib.cpp"
    break;

  case 692: /* literal_expr: TLSBR expr_ls TCOMMA TRSBR  */
#line 3652 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true).release();
  }
#line 10692 "bison-chpl-lib.cpp"
    break;

  case 693: /* literal_expr: TLSBR assoc_expr_ls TRSBR  */
#line 3656 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-1].exprList)), false, true).release();
  }
#line 10700 "bison-chpl-lib.cpp"
    break;

  case 694: /* literal_expr: TLSBR assoc_expr_ls TCOMMA TRSBR  */
#line 3660 "chpl.ypp"
  {
    (yyval.expr) = Array::build(BUILDER, LOC((yyloc)), context->consumeList((yyvsp[-2].exprList)), true, true).release();
  }
#line 10708 "bison-chpl-lib.cpp"
    break;

  case 695: /* assoc_expr_ls: expr TALIAS expr  */
#line 3668 "chpl.ypp"
  {
    auto node = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->makeList(node);
  }
#line 10717 "bison-chpl-lib.cpp"
    break;

  case 696: /* assoc_expr_ls: assoc_expr_ls TCOMMA expr TALIAS expr  */
#line 3673 "chpl.ypp"
  {
    auto loc = context->makeSpannedLocation((yylsp[-2]), (yylsp[0]));
    auto node = context->buildBinOp(loc, (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr));
    (yyval.exprList) = context->appendList((yyvsp[-4].exprList), node);
  }
#line 10727 "bison-chpl-lib.cpp"
    break;

  case 697: /* binary_op_expr: expr TPLUS expr  */
#line 3681 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10733 "bison-chpl-lib.cpp"
    break;

  case 698: /* binary_op_expr: expr TMINUS expr  */
#line 3682 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10739 "bison-chpl-lib.cpp"
    break;

  case 699: /* binary_op_expr: expr TSTAR expr  */
#line 3683 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10745 "bison-chpl-lib.cpp"
    break;

  case 700: /* binary_op_expr: expr TDIVIDE expr  */
#line 3684 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10751 "bison-chpl-lib.cpp"
    break;

  case 701: /* binary_op_expr: expr TSHIFTLEFT expr  */
#line 3685 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10757 "bison-chpl-lib.cpp"
    break;

  case 702: /* binary_op_expr: expr TSHIFTRIGHT expr  */
#line 3686 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10763 "bison-chpl-lib.cpp"
    break;

  case 703: /* binary_op_expr: expr TMOD expr  */
#line 3687 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10769 "bison-chpl-lib.cpp"
    break;

  case 704: /* binary_op_expr: expr TEQUAL expr  */
#line 3688 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10775 "bison-chpl-lib.cpp"
    break;

  case 705: /* binary_op_expr: expr TNOTEQUAL expr  */
#line 3689 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10781 "bison-chpl-lib.cpp"
    break;

  case 706: /* binary_op_expr: expr TLESSEQUAL expr  */
#line 3690 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10787 "bison-chpl-lib.cpp"
    break;

  case 707: /* binary_op_expr: expr TGREATEREQUAL expr  */
#line 3691 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10793 "bison-chpl-lib.cpp"
    break;

  case 708: /* binary_op_expr: expr TLESS expr  */
#line 3692 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10799 "bison-chpl-lib.cpp"
    break;

  case 709: /* binary_op_expr: expr TGREATER expr  */
#line 3693 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10805 "bison-chpl-lib.cpp"
    break;

  case 710: /* binary_op_expr: expr TBAND expr  */
#line 3694 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10811 "bison-chpl-lib.cpp"
    break;

  case 711: /* binary_op_expr: expr TBOR expr  */
#line 3695 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10817 "bison-chpl-lib.cpp"
    break;

  case 712: /* binary_op_expr: expr TBXOR expr  */
#line 3696 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10823 "bison-chpl-lib.cpp"
    break;

  case 713: /* binary_op_expr: expr TAND expr  */
#line 3697 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10829 "bison-chpl-lib.cpp"
    break;

  case 714: /* binary_op_expr: expr TOR expr  */
#line 3698 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10835 "bison-chpl-lib.cpp"
    break;

  case 715: /* binary_op_expr: expr TEXP expr  */
#line 3699 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10841 "bison-chpl-lib.cpp"
    break;

  case 716: /* binary_op_expr: expr TBY expr  */
#line 3700 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10847 "bison-chpl-lib.cpp"
    break;

  case 717: /* binary_op_expr: expr TALIGN expr  */
#line 3701 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10853 "bison-chpl-lib.cpp"
    break;

  case 718: /* binary_op_expr: expr THASH expr  */
#line 3702 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10859 "bison-chpl-lib.cpp"
    break;

  case 719: /* binary_op_expr: expr TDMAPPED expr  */
#line 3703 "chpl.ypp"
                           { (yyval.expr) = context->buildBinOp((yyloc), (yyvsp[-2].expr), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10865 "bison-chpl-lib.cpp"
    break;

  case 720: /* unary_op_expr: TPLUS expr  */
#line 3707 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10871 "bison-chpl-lib.cpp"
    break;

  case 721: /* unary_op_expr: TMINUS expr  */
#line 3708 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10877 "bison-chpl-lib.cpp"
    break;

  case 722: /* unary_op_expr: TMINUSMINUS expr  */
#line 3709 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10883 "bison-chpl-lib.cpp"
    break;

  case 723: /* unary_op_expr: TPLUSPLUS expr  */
#line 3710 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10889 "bison-chpl-lib.cpp"
    break;

  case 724: /* unary_op_expr: TBANG expr  */
#line 3711 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10895 "bison-chpl-lib.cpp"
    break;

  case 725: /* unary_op_expr: expr TBANG  */
#line 3712 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc),
                                                              STR("postfix!"),
                                                              (yyvsp[-1].expr)); }
#line 10903 "bison-chpl-lib.cpp"
    break;

  case 726: /* unary_op_expr: TBNOT expr  */
#line 3715 "chpl.ypp"
                                 { (yyval.expr) = context->buildUnaryOp((yyloc), (yyvsp[-1].uniqueStr), (yyvsp[0].expr)); }
#line 10909 "bison-chpl-lib.cpp"
    break;

  case 727: /* reduce_expr: expr TREDUCE expr  */
#line 3720 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10917 "bison-chpl-lib.cpp"
    break;

  case 728: /* reduce_expr: expr TREDUCE zippered_iterator  */
#line 3724 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10925 "bison-chpl-lib.cpp"
    break;

  case 729: /* reduce_expr: reduce_scan_op_expr TREDUCE expr  */
#line 3728 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10933 "bison-chpl-lib.cpp"
    break;

  case 730: /* reduce_expr: reduce_scan_op_expr TREDUCE zippered_iterator  */
#line 3732 "chpl.ypp"
  {
    (yyval.expr) = context->buildReduce((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10941 "bison-chpl-lib.cpp"
    break;

  case 731: /* scan_expr: expr TSCAN expr  */
#line 3739 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10949 "bison-chpl-lib.cpp"
    break;

  case 732: /* scan_expr: expr TSCAN zippered_iterator  */
#line 3743 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].expr), (yyvsp[0].expr));
  }
#line 10957 "bison-chpl-lib.cpp"
    break;

  case 733: /* scan_expr: reduce_scan_op_expr TSCAN expr  */
#line 3747 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10965 "bison-chpl-lib.cpp"
    break;

  case 734: /* scan_expr: reduce_scan_op_expr TSCAN zippered_iterator  */
#line 3751 "chpl.ypp"
  {
    (yyval.expr) = context->buildScan((yyloc), (yylsp[-2]), (yyvsp[-2].uniqueStr), (yyvsp[0].expr));
  }
#line 10973 "bison-chpl-lib.cpp"
    break;


#line 10977 "bison-chpl-lib.cpp"

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
